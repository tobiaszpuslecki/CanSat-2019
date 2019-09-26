/*
 * cmd_parser.c
 *
 *  Created on: May 28, 2019
 *      Author: Jakub Pal
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "cansat_includes.h"

#include "main.h"
#include "cmd_parser.h"

#define TERM_NEXT(_gh)      (_gh)->p.term_str[((_gh)->p.term_pos = 0)] = 0; (_gh)->p.term_num++;

#define CRC_ADD(_gh, ch)    (_gh)->p.crc_calc ^= (uint8_t)(ch)
#define TERM_ADD(_gh, ch)   if ((_gh)->p.term_pos < (sizeof((_gh)->p.term_str) - 1)) {  \
        (_gh)->p.term_str[(_gh)->p.term_pos++] = (ch);  \
        (_gh)->p.term_str[(_gh)->p.term_pos] = 0;   \
    }

#define CHTN(x)             (((x) >= '0' && (x) <= '9') ? ((x) - '0') : (((x) >= 'a' && (x) <= 'z') ? ((x) - 'a' + 10) : (((x) >= 'A' && (x) <= 'Z') ? ((x) - 'A' + 10) : 0)))

#define MSG_UNKNOWN 0
#define MSG_CMD 1

/**
 * \brief           Parse received term
 * \param[in]       gh: CMD handle
 * \return          `1` on success, `0` otherwise
 */
static uint8_t parse_term(cmd_t* gh){
    if (gh->p.term_num == 0) {                  /* Check string type */
        if (strncmp(gh->p.term_str, "$CMD", 4) == 0) {
            gh->p.stat = MSG_CMD;
        } else {
            gh->p.stat = MSG_UNKNOWN;          /* Invalid statement for library */
        }
        return 1;
    }

    /* Start parsing terms */
    if (gh->p.stat == MSG_UNKNOWN){

    } else if (gh->p.stat == MSG_CMD){
    	if (strncmp(gh->p.term_str, "CALIB", 5) == 0){
    		gh->cmd = XBEE_CmdCalibRcvCallback;
    	} else if (strncmp(gh->p.term_str, "OPEN", 4) == 0) {
    		gh->cmd = XBEE_CmdOpenRcvCallback;
    	} else if (strncmp(gh->p.term_str, "CLOSE", 5) == 0) {
    		gh->cmd = XBEE_CmdCloseRcvCallback;
    	} else if (strncmp(gh->p.term_str, "ABORT", 5) == 0) {
    		gh->cmd = XBEE_CmdAbortRcvCallback;
    	}
    }
    return 1;
}

/**
 * \brief           Compare calculated CRC with received CRC
 * \param[in]       gh: CMD handle
 * \return          `1` on success, `0` otherwise
 */
static uint8_t check_crc(cmd_t* gh) {
    uint8_t crc;
    crc = (uint8_t)((CHTN(gh->p.term_str[0]) & 0x0F) << 0x04) | (CHTN(gh->p.term_str[1]) & 0x0F);   /* Convert received CRC from string (hex) to number */
    return gh->p.crc_calc == crc;               /* They must match! */
}

uint8_t process_cmd(cmd_t* gh, const void* data, size_t len){
	const uint8_t* d = data;

	    while (len--) {                             /* Process all bytes */
	        if (*d == '$') {                        /* Check for beginning of NMEA line */
	            memset(&gh->p, 0x00, sizeof(gh->p));/* Reset private memory */
	            TERM_ADD(gh, *d);                   /* Add character to term */
	        } else if (*d == ',') {                 /* Term separator character */
	            parse_term(gh);                     /* Parse term we have currently in memory */
	            CRC_ADD(gh, *d);                    /* Add character to CRC computation */
	            TERM_NEXT(gh);                      /* Start with next term */
	        } else if (*d == '*') {                 /* Start indicates end of data for CRC computation */
	            parse_term(gh);                     /* Parse term we have currently in memory */
	            gh->p.star = 1;                     /* STAR detected */
	            TERM_NEXT(gh);                      /* Start with next term */
	        } else if (*d == '\r') {
	            if (check_crc(gh)) {                /* Check for CRC result */
	            	gh->cmd();
	            }
	        } else {
	            if (!gh->p.star) {                  /* Add to CRC only if star not yet detected */
	                CRC_ADD(gh, *d);                /* Add to CRC */
	            }
	            TERM_ADD(gh, *d);                   /* Add character to term */
	        }
	        d++;                                    /* Process next character */
	    }
	    return 1;
}

void XBEE_CmdCalibRcvCallback(){
	extern bool RADIO_SIGNAL_RECEIVED;
	extern bool SET_REF_VAL;
	RADIO_SIGNAL_RECEIVED = true;
	SET_REF_VAL = true;
}

void XBEE_CmdOpenRcvCallback(){
  	open_container();
}

void XBEE_CmdCloseRcvCallback(){
  	close_container();
}

void XBEE_CmdAbortRcvCallback(){
	lastStateFileWrite(PRELAUNCH_CHAR);
	NVIC_SystemReset();
}


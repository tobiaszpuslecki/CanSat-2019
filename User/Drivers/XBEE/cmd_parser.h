/*
 * cmd_parser.h
 *
 *  Created on: May 28, 2019
 *      Author: Jakub Pal
 */

#ifndef DRIVERS_XBEE_CMD_PARSER_H_
#define DRIVERS_XBEE_CMD_PARSER_H_

#include <stdint.h>

typedef struct {
#if !__DOXYGEN__
    struct {
        uint8_t stat;                           /*!< Statement index */
        char term_str[13];                      /*!< Current term in string format */
        uint8_t term_pos;                       /*!< Current index position in term */
        uint8_t term_num;                       /*!< Current term number */

        uint8_t star;                           /*!< Star detected flag */

        uint8_t crc_calc;                       /*!< Calculated CRC string */
    } p;                                        /*!< Structure with private data */
    void (*cmd)();
#endif /* !__DOXYGEN__ */
} cmd_t;


uint8_t process_cmd(cmd_t* gh, const void* data, size_t len);

void XBEE_CmdCalibRcvCallback();
void XBEE_CmdOpenRcvCallback();
void XBEE_CmdCloseRcvCallback();
void XBEE_CmdAbortRcvCallback();

#endif /* DRIVERS_XBEE_CMD_PARSER_H_ */

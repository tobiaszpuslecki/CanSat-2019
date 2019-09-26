#ifndef CANSAT_FILE_SYSTEM_H
#define CANSAT_FILE_SYSTEM_H

#include "cansat_file_names.h"
#include "../Application/Includes/cansat_includes.h"

//#include <stdio.h>
#include "XBEE/printf.h"
#include <stdlib.h>
#include <string.h>

FATFS fs_area; 

FATFS fs_area_log;
FRESULT sd_res_log;
FIL log_file;
UINT sd_bw_log;

char filenameLog[50];



/*
  Function reads character from last state file and returns it
*/
char lastStateFileRead();
// /**/
void lastStateFileWrite(char val);
//
// /* Function reads reference values from reference file and returns it */
// reference_t referenceFileRead()
// /**/
//void referenceFileWrite(reference_t referenceStruct);
//
/*
  Function returns num field of given line.
  Delimiter is ;
  Line will be like:
  field1;field2;filer3;field4 ...
*/
/*const*/ char* getField(char* line_, int num);

/*
  Function returns numValue field of fieldValue line from file.
  Delimiter is ;
  Line will be like:
  field1;field2;filer3;field4 ...
  field1;field2;filer3;field4 ...
*/
//int getFieldFromFile(FIL* stream, int lineNo, int field, const int buffSize);


void openLoggerFile();
void closeLoggerFile();

void writeLog();

//void referenceFileRead(reference_t*, data_struct*);
//
//void referenceFileWrite(reference_t);


#endif /* CANSAT_FILE_SYSTEM_H */

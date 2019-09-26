#include "cansat_file_system.h"


/*
  Function returns num field of given line.
  Delimiter is ;
  Line will be like:
  field1;field2;filer3;field4 ...
*/
/*const*/ char* getField(char* line_, int num)
{
    //char *line = strdup(line_);
	char* line = malloc(strlen(line_)+1);
	strcpy(line, line_);

    /*const*/ char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n"))
    {
        if (!--num)
        {
        	free((char*)line);
        	return tok;
        }

    }
	free((char*)line);
    return NULL;
}

void referenceFileRead(reference_t* referenceStruct, flight_data* data)
{
		FRESULT sdResult;
		FIL filetest;
		UINT br;

		char* tempChar;
		char* tempResult;
		char buf[100] = {0};


		f_mount(&fs_area, "", 0);

		sdResult = f_open(&filetest, reference_values_file_name, FA_READ | FA_OPEN_ALWAYS);

		tempChar = f_gets(buf, sizeof(buf)/sizeof(buf[0]), &filetest);
		f_close(&filetest);

		tempResult = getField(tempChar, 1);
		referenceStruct->pressure = atoi(tempResult);

		tempResult = getField(tempChar, 2);
		referenceStruct->pitch = atoi(tempResult);

		tempResult = getField(tempChar, 3);
		referenceStruct->roll = atoi(tempResult);

		tempResult = getField(tempChar, 4);
		referenceStruct->counter = atoi(tempResult);
		data->packet_count = atoi(tempResult);

		//free(buf);
		free(tempResult);
		free(tempChar);

}

void referenceFileWrite(reference_t referenceStruct)
{

	FRESULT sd_res;
	FIL my_file;
	UINT sd_bw;


	char bufc[100] = {0};

	f_mount(&fs_area, "", 0);

	sd_res = f_open(&my_file, reference_values_file_name, FA_WRITE | FA_CREATE_ALWAYS);


	sprintf(bufc, "%d;%d;%d;%d", referenceStruct.pressure,referenceStruct.pitch,referenceStruct.roll,data.packet_count); // referenceStruct.counter
	f_write(&my_file, bufc, sizeof(bufc)/sizeof(bufc[0]), &sd_bw);

	f_sync(&my_file);
	f_close(&my_file);

	//free(bufc);
}

char lastStateFileRead()
{
	FRESULT sdResult;
	FIL lastStateFile;
	UINT br;

	char buf[3];

	f_mount(&fs_area, "", 0);

	sdResult = f_open(&lastStateFile, last_state_file_name, FA_READ | FA_OPEN_ALWAYS);

	sdResult = f_read(&lastStateFile, buf, 1, &br);

	f_close(&lastStateFile);

	return buf[0];
}

void lastStateFileWrite(char val)
{
	char buf[3];
	buf[0]=val;
	FRESULT sd_res;
	FIL my_file;
	UINT sd_bw;

	f_mount(&fs_area, "", 0);

	sd_res = f_open(&my_file, last_state_file_name, FA_WRITE | FA_CREATE_ALWAYS);

	f_write(&my_file, buf, 1, &sd_bw);
	f_sync(&my_file);
	f_close(&my_file);
}


void openLoggerFile()
{
	f_mount(&fs_area_log, "", 0);
	//sd_res_log = f_open(&log_file, logger_file_name, FA_WRITE | FA_CREATE_ALWAYS);

	char buffer[100];

	for(int fidx=0; fidx<1000; fidx++)
	{
		sprintf(filenameLog, "log%03d.csv", fidx);
		sd_res_log = f_open(&log_file, filenameLog, FA_WRITE | FA_CREATE_NEW);
		if( sd_res_log == FR_OK )
		{
			log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "File idx: %d", fidx);
			break;
		}
	}
	// Write the column names to the file:
	strcpy(buffer, "stamp,pres,acc1,acc2,acc3,gyro1,gyro2,state\n");
	f_write(&log_file, buffer, sizeof(buffer), &sd_bw_log);
	f_sync(&log_file);


}

void closeLoggerFile()
{
	f_close(&log_file);
}

FRESULT open_append(FIL* fp, const char* path)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

void writeLog()
{
	char buflog[100] = {0};

    f_mount(&fs_area, "", 0);
    FRESULT fr = open_append(&log_file, filenameLog);
    if (fr != FR_OK) return;

    int buffSize = 0;

	//"stamp,pres,acc1,acc2,acc3,gyro1,gyro2,state"
	buffSize += sprintf(buflog + buffSize, "%d,%d", HAL_GetTick(), data.pressure);
	buffSize += sprintf(buflog + buffSize, "%d,%d,",  dataExtra.acc[0], dataExtra.acc[1]);
	buffSize += sprintf(buflog + buffSize, "%d,%d,%d,", dataExtra.acc[2], dataExtra.gyro[0], dataExtra.gyro[1]);
	buffSize += sprintf(buflog + buffSize, "%c\n", data.software_state+'0');

	f_write(&log_file, buflog, buffSize, &sd_bw_log);

    f_close(&log_file);
}



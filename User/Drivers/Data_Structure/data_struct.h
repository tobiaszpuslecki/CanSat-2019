#ifndef __incl_data_struct
#define __incl_data_struct

#include "stdint.h"
#include "cansat_includes.h"

#define ACC_MAX_NUM 10
#define PRES_MAX_NUM 5



typedef struct data_struct
{
    int16_t acc[3]; //xyz
    int16_t gyro[3]; //xyz
    float previousAltitude;

}data_struct;

data_struct dataExtra;
flight_data data;


MS5611 ms5611;

int32_t pres_values[PRES_MAX_NUM];

int32_t acc_z_values[ACC_MAX_NUM];



#endif

#ifndef CANSAT_BOOT_H
#define CANSAT_BOOT_H

#include "../../Application/Includes/cansat_includes.h"

typedef enum err_t
{
  RESULT_OK = 0,
  RESULT_FAIL = 1
}err_t;

typedef struct bootErrors_t
{
    err_t servo;
    FRESULT fs;
    err_t mpu;
    err_t baro;
    err_t ina;
    err_t gps;
    err_t xbee;
}bootErrors_t;

typedef struct reference_t
{
  unsigned char lastStateChar;
  uint32_t pressure;
  uint32_t pitch;
  uint32_t roll;
  uint32_t counter;
}reference_t;

reference_t referenceStruct;

/*
  Functions to inits return err_t as result
  xConfigCycle is length of config cycle
*/
err_t SDInit(int sdConfigCycle);
err_t MPUInit(int mpuConfigCycle);
err_t INAInit(int voltageConfigCycle);
err_t BaroInit(int baroConfigCycle);
err_t ServoInit();


/*
  Function prints error logs based on err struct
*/
int bootLogErrors(bootErrors_t err);

/*
  Main function to init all sensors and state machine
*/
err_t stateBootFunction(void);

#endif /* CANSAT_BOOT_H */

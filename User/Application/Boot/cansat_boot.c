#include "cansat_boot.h"

err_t SDInit(int sdConfigCycle)
{
  const uint8_t mountParameter = 1;
  const uint16_t sdDelayTime = 500;

  HAL_GPIO_WritePin(SD_POWER_GPIO_Port, SD_POWER_Pin, GPIO_PIN_RESET);  // Power ON SD card
  HAL_Delay(sdDelayTime);
  SD_spi_handle = &hspi1;
  log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "SD pre-init successed");

  while(sdConfigCycle--)
  {
	  FRESULT sdResult;

	  sdResult = f_mount(&fs_area, "", mountParameter);
      if( FR_OK != sdResult )
      {
        log_data(LOG_TYPE_WARRNING, LOG_DEVICE_SYSTEM, "SD init error! %d", sdConfigCycle);
      }
      else
      {
        log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "SD init successed");
        return RESULT_OK;
      }
  }
  return RESULT_FAIL;
}

err_t MPUInit(int mpuConfigCycle)
{
  uint16_t properMPUName = 0x71;

  while (mpuConfigCycle--)
  {
    resetMPU9250();
    initMPU9250();

    if( properMPUName != readMpuName() )
    {
      log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MPU9250, "MPU init error! %d", mpuConfigCycle);
    }
    else
    {
      log_data(LOG_TYPE_INFO, LOG_DEVICE_MPU9250, "MPU init successed");
       return RESULT_OK;
    }
  }
  return RESULT_FAIL;
}

err_t INAInit(int voltageConfigCycle)
{
  // Voltage variables
  HAL_StatusTypeDef voltageConfigResult = -1;
  HAL_StatusTypeDef voltageCalibrationResult = -1;
  float shuntValue = 0.1;
  float maxCurrent = 3.2;

  while(voltageConfigCycle--)
  {
    INA219_Init( &hi2c1 );
    voltageConfigResult = INA219_configure( INA219_RANGE_16V, INA219_GAIN_320MV, INA219_BUS_RES_12BIT, INA219_SHUNT_RES_11BIT_1S, INA219_MODE_SHUNT_BUS_CONT );
    voltageCalibrationResult = INA219_calibrate(shuntValue, maxCurrent);

    if ( !voltageConfigResult && !voltageCalibrationResult)
    {
      log_data(LOG_TYPE_INFO, LOG_DEVICE_INA219, "INA219 init successed");
      return RESULT_OK;
    }
    else
    {
      log_data(LOG_TYPE_WARRNING, LOG_DEVICE_INA219, "INA219 init error! %d", voltageConfigCycle);
    }
  }

  return RESULT_FAIL;
}

err_t BaroInit(int baroConfigCycle)
{
  const uint16_t baroDelayTime = 100;

  // Barometer variables
  //MS5611 baroData;
  MS5611_result baroResult;

  while(baroConfigCycle--)
  {
    baroResult = MS5611_init(&hi2c1, &ms5611, MS5611_ULTRA_HIGH_RES);
    if( MS5611_OK != baroResult )
    {
      log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "MS5611 init error! %d", baroConfigCycle);
    }
    else   // Calibrate zero lever for 'MS5611_calculateAltitude':
    {
      HAL_Delay(2*baroDelayTime);
      MS5611_requestTemperature();
      HAL_Delay(baroDelayTime);
      MS5611_getTemperature( &ms5611 );

      MS5611_requestPressure();
      HAL_Delay(baroDelayTime);
      MS5611_getPressure( &ms5611 );

      MS5611_setLevelZero( &ms5611 );
      log_data(LOG_TYPE_INFO, LOG_DEVICE_MS5611, "MS5611 init successed");
      return RESULT_OK;
    }
  }
  return RESULT_FAIL;
}

err_t ServoInit()
{
  // Servo init

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	servos_init(TIM3);
	log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Servo init successed");
	return RESULT_OK;
}

err_t GPSInit()
{
	ublox_init(&huart3);

	return RESULT_OK;
}

err_t XBeeInit()
{
	xbee_init(&huart1);

	return RESULT_OK;
}

int bootLogErrors(bootErrors_t err)
{
  if(err.servo)
  {
    log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: Servo init failed!");
  }
  if(err.fs)
  {
    log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: SD card init failed!");
  }
  if(err.mpu)
  {
    log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: MPU sensor init failed!");
  }
  if(err.baro)
  {
    log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: Barometer sensor init failed!");
  }
  if(err.ina)
  {
    log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: Voltage sensor init failed!");
  }


  return (int)(err.servo + err.fs + err.mpu + err.baro + err.ina);
}

void LEDBlink(uint8_t n)
{
	for(uint8_t i = 0; i<n; i++)
	{
	    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_Delay(50);
	    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
	}
}



err_t stateBootFunction(void)
{
    LEDBlink(3);

    bootErrors_t err;
    err.servo = ServoInit();
    err.fs = SDInit(4);
    err.mpu = MPUInit(4);
    err.baro = BaroInit(4);
    err.ina = INAInit(4);
    err.gps = GPSInit();
    err.xbee = XBeeInit();

    readAccelData(&dataExtra.acc);

    if(bootLogErrors(err))
    {
    	LEDBlink(10);
    }
    else
    {
    	LEDBlink(3);
    }

    openLoggerFile();
    HAL_Delay(50);

    if(err.fs == FR_OK)
    {
		referenceStruct.lastStateChar = lastStateFileRead();
		referenceFileRead(&referenceStruct, &data);
    }
    else
    {
        log_data(LOG_TYPE_CRITICAL, LOG_DEVICE_SYSTEM, "CRITICAL ERROR: MSM failed!");
        referenceStruct.lastStateChar = ERROR_CHAR;
    }

    log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Last state:  %c ", referenceStruct.lastStateChar);

    StateMachine_t mainStateMachine;

    StateMachineReturn_t initRes = stateMachine_init(&mainStateMachine, statesInit, statesList, numStates);

    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_3);
    HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_4);

    switch (referenceStruct.lastStateChar)
    {
      case PRELAUNCH_CHAR:  stateMachine_run(&mainStateMachine, &statePrelaunch,0); break;
      case READY_CHAR:  	stateMachine_run(&mainStateMachine, &stateReady,0); break;
      case FLIGHTUP_CHAR:  	stateMachine_run(&mainStateMachine, &stateFlightup,0); break;
      case CONTAINER_CHAR:  stateMachine_run(&mainStateMachine, &stateContainer,0); break;
      case AUTOGYRO_CHAR:  	stateMachine_run(&mainStateMachine, &stateAutogyro,0); break;
      case BREAKING_CHAR: 	stateMachine_run(&mainStateMachine, &stateBreaking,0); break;
      case TOUCHDOWN_CHAR:  stateMachine_run(&mainStateMachine, &stateTouchdown,0); break;
      default: 				stateMachine_run(&mainStateMachine, &stateContainer,0);
    }
    return RESULT_OK;
}

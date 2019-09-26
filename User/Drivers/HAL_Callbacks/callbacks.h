#include "stm32f1xx_hal.h"
#include "tim.h"
#include "../../Application/Logger/Logger.h"
#include "../Data_Structure/data_struct.h"
#include "../MS5611/ms5611.h"
#include "../MPU9250/MPU9250.h"
#include "../INA219/ina219.h"


//callback function for TIM2 Input Capture
//this function is called when IC interrupt occurs
//used for measuring rotor speed with hall sensor
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

//callback function for TIM2 Perion Elapsed
//this function is called when timer's counter reach ARR value
//used for measuring rotor speed with hall sensor
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

//callback function for TIM4 Output Compare interrupt
//used for timing data acquisition and logging
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);


//callback function for GPIO interrupt
//used for MPU interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#include "callbacks.h"
#include "GPS/gps.h"
#include <FileSystem/cansat_file_system.h>


extern bool RADIO_SIGNAL_RECEIVED;
extern bool SET_REF_VAL;

long unsigned int requestCounter = 0;

int posAcc = 0;
int32_t newAvgAcc = 0;
long sumAcc = 0;
char avgBufAcc[20];

int posBaro = 0;
int32_t newAvgBaro = 0;
long sumBaro = 0;
char avgBufBaro[20];

float pressureLvlZero=11320.0;
bool zeroLevelSet = false;


int32_t movingAvg(int32_t *ptrArrNumbers, long *ptrSum, int pos, int len, int32_t nextNum) {
  //Subtract the oldest number from the prev sum, add the new number
  *ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
  //Assign the nextNum to the position in the array
  ptrArrNumbers[pos] = nextNum;
  //return the average
  return *ptrSum / len;
}


//+ these comments will be removed if problem with uart was resolved

//callback function for TIM2 Input Capture
//this function is called when IC interrupt occurs
//used for measuring rotor speed with hall sensor
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

}

//callback function for TIM2 Perion Elapsed
//this function is called when timer's counter reach ARR value
//used for measuring rotor speed with hall sensor
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}



//callback function for TIM4 Output Compare interrupt
//used for timing data acquisition and logging
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM4){
		switch (htim->Channel) {
			case HAL_TIM_ACTIVE_CHANNEL_1:
				//interrupt every 50 ms
				//increase CCR1 register value
				if (__HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1) >= 9500){
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
				}
				else{
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1) + 500);
				}
                //get INA219, MS5611


				//MS5611




				if(requestCounter%4==0)
				{
					if (MS5611_requestTemperature() != MS5611_OK){
						//+log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "requestPressure failed");
					}

				} else if (requestCounter%4-1==0) {
					if (MS5611_getTemperature(&ms5611) != MS5611_OK){
						//+log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "getTemperature failed");
					} else{
						data.temp = ms5611.temperature;
						//+log_data(LOG_TYPE_INFO, LOG_DEVICE_MS5611, "Temperature=%ld", data.temperature);
					}

					if (MS5611_requestPressure() != MS5611_OK){
						//+log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "requestPressure failed");
					}
				} else if(requestCounter%4-2==0) {
					if (MS5611_getPressure(&ms5611) != MS5611_OK){
						//+log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "getPressure failed");
					} else {
						data.pressure = ms5611.pressure;

						newAvgBaro = movingAvg(pres_values, &sumBaro, posBaro, PRES_MAX_NUM, data.pressure);
						posBaro++;
						if (posBaro >= PRES_MAX_NUM){posBaro = 0;}
						data.pressure = newAvgBaro;

						if( (!zeroLevelSet && HAL_GetTick() > 20000) || SET_REF_VAL)
						{
							zeroLevelSet=true;
							ms5611.altitude = newAvgBaro;
							MS5611_setLevelZero(&ms5611);
							SET_REF_VAL = false;
						}

						//ms5611.pressure = newAvgBaro;
						MS5611_calculateAltitude(&ms5611);
						data.altitude = ms5611.altitude;
						//log_data(LOG_TYPE_WARRNING, LOG_DEVICE_MS5611, "Alt: %d", (int)data.altitude);

					//int lg = sprintf(avgBufBaro,"%d\n", newAvgBaro);
					//HAL_UART_Transmit(&huart2, avgBufBaro, lg, HAL_MAX_DELAY);
					}
				}

				++requestCounter;


				//MS5611 end


				//INA219
				if (INA219_readShuntVoltage_mV(&data.voltage) != HAL_OK){
					//+log_data(LOG_TYPE_WARRNING, LOG_DEVICE_INA219, "readShuntVoltage failed");
				}
				else{
					//+log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Voltage=%d", data.voltage);
				}
				//HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
				break;
			case HAL_TIM_ACTIVE_CHANNEL_2:
				//interrupt every 100 ms
				//increase CCR2 register value
				if (__HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_2) >= 9000){
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 300);
				}
				else{
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_2) + 1000);
				}
				//SD write
				writeLog();
				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

				// SD end
				break;
			case HAL_TIM_ACTIVE_CHANNEL_3:
				//interrupt every 1s
				data.mission_time = HAL_GetTick()/1000;
				data.gps_latitude = hgps.latitude;
				data.gps_longitude = hgps.longitude;
				data.gps_altitude = hgps.altitude;
				data.gps_sats = hgps.sats_in_use;
				data.gps_seconds = hgps.seconds;
				data.gps_minutes = hgps.minutes;
				data.gps_hours = hgps.hours;
				++data.packet_count;
				if(RADIO_SIGNAL_RECEIVED){
					xbee_sendData(&data);
				}
				break;
			default:
				break;
		}
	}
}


//callback function for GPIO interrupt
//used for MPU interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//MPU9250
	if(GPIO_Pin == MPU_INT_Pin){
		readAccelData(&dataExtra.acc);

		newAvgAcc = movingAvg(acc_z_values, &sumAcc, posAcc, ACC_MAX_NUM, dataExtra.acc[2]);
		posAcc++;
		if (posAcc >= ACC_MAX_NUM){posAcc = 0;}

		dataExtra.acc[2] = newAvgAcc;

//		int lg = sprintf(avgBufAcc,"%d\n", newAvgAcc);
//		HAL_UART_Transmit(&huart2, avgBufAcc, lg, HAL_MAX_DELAY);
		readGyroData(&dataExtra.gyro);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart1){
		XBEE_RxCpltCallback(huart);
	}
	else if (huart == &huart2){

	}
	else if (huart == &huart3){
		GPS_RxCpltCallback(huart);

		//log_data(LOG_TYPE_WARRNING, LOG_DEVICE_INA219, "%f, %f",hgps.latitude, hgps.longitude);
	}
}


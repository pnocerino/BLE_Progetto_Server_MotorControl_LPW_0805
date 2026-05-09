#include "sensor_manager.h"
#include "main.h"
#include "adc.h"
#include "bas_app.h"
#include "ess_app.h"
#include "app_conf.h"
#include "ess.h"
#include "stm32_timer.h"
#include "stm32_rtos.h"
#include "dbg_trace.h"
#include "log_module.h"

ALIGN_32BYTES (uint16_t adc_buffer[2]);
volatile uint8_t data_ready_flag = 0;

static float last_battery_voltage = 0.0f;
static float last_temperature = 0.0f;

void Sensor_Manager_Init(void) {
	data_ready_flag = 0;
}


void Sensor_Manager_Start_Scan(void) {
	data_ready_flag = 0;
		HAL_ADC_Stop_DMA(&hadc4);
		__HAL_ADC_CLEAR_FLAG(&hadc4, ADC_FLAG_OVR);
		HAL_ADC_Start_DMA(&hadc4, (uint32_t*) adc_buffer, 2);
}

uint8_t Sensor_Manager_Is_Data_Ready(void) {
	return data_ready_flag;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC4) {

		data_ready_flag = 1;
		//HAL_ADC_Stop_DMA(&hadc4);
		UTIL_SEQ_SetTask(1U << TASK_MEASURE_MOTOR_TEMPERATURE, CFG_SEQ_PRIO_1);
	}
}

uint8_t Get_Battery_Percentage(void) {
	uint8_t percentage;
	if (last_battery_voltage >= 3.0f)
		percentage = 100;
	else if (last_battery_voltage <= 2.0f)
		percentage = 50;
	else
		percentage = (uint8_t) ((last_battery_voltage - 2.0f) * 100.0f);
	return percentage;
}

int16_t Get_Temperature_Celsius(void) {
	return (int16_t) (last_temperature * 100);
}

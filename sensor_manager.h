/*
 * sensor_manager.h
 *
 *  Created on: May 3, 2026
 *      Author: sherr
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>

void Sensor_Manager_Init(void);
void Sensor_Manager_Start_Scan(void);
uint8_t Sensor_Manager_Is_Data_Ready(void);

uint8_t Get_Battery_Percentage(void);
int16_t Get_Temperature_Celsius(void);


#endif /* SENSOR_MANAGER_H */

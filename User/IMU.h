#include "config.h"


//********************************************************************************
#ifndef	_IMU_H_
#define	_IMU_H_
#include "CyberryPotter.h"
#include "stm32f10x.h"

void IMU_Data_Print(void);
void IMU_Initialize_Data(void);
//void IMU_Kalman_Filter(uint8_t i);
void IMU_LP_Filter(uint8_t i);
void IMU_Get_Data(uint8_t index);

#endif	//_IMU_H_

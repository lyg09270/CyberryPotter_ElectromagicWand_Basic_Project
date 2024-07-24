#include "config.h"


//********************************************************************************
#ifndef	_IMU_H_
#define	_IMU_H_
#include "CyberryPotter.h"
#include "stm32f10x.h"

#ifdef SYSTEM_MODE_DATA_COLLECT
void IMU_Data_Print(void);
#endif //SYSTEM_MODE_DATA_COLLECT
void IMU_Sample_Start(void);
void IMU_Get_Data(uint8_t index);

#endif	//_IMU_H_

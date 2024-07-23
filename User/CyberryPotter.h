#include "config.h"

//********************************************************************************
#ifndef	_CYBERRY_POTTER_H_
#define	_CYBERRY_POTTER_H_
#include "hardware.h"
#include "Delay.h"
#include "IMU.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>


typedef enum eSystem_Mode{
        SYSTEM_IDLE = 0,
        SYSTEM_RECORD = 1,
        SYSTEM_TRANSMIT = 2
}eSystem_Mode;

typedef enum eButton_Status{
        BUTTON_RELEASE = 0,
        BUTTON_HOLD = 1
}eButton_Status;     

typedef enum eButton_Time_Count{
        BUTTON_IDLE = 0,
        BUTTON_HOLD_SHORT = 1,
        BUTTON_HOLD_LONG = 2,
        BUTTON_RELEASE_SHORT = 3,
        BUTTON_SHORT_AND_LONG = 4
}eButton_Time_Count;    

typedef enum eIMU_STATUS{
        IMU_Idle = 0,
        IMU_Sampling = 1,
        IMU_Sampled = 2,
	IMU_Sample = 3
}eIMU_STATUS;

typedef enum eSerial_Status{
        Serial_Idle = 0,
        Serial_Receiving = 1,
        Serial_Received = 2
}eSerial_Status;

typedef struct Cyberry_Potter_Status_Typedef{
        eSystem_Mode System_Mode;
        eButton_Status Button_Status;
        eButton_Time_Count Button_Time_Count;
        uint8_t Button_Click_Count;
        eIMU_STATUS IMU_Status;
        eSerial_Status Serial_Status;
}Cyberry_Potter_Status_Typedef;

void Cyberry_Potter_System_Status_Update(void);
void System_Init(void);
void Cyberry_Potter(void);

#endif	//_CYBERRY_POTTER_H_

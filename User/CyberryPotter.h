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
	BUTTON_IDLE = 0,
        BUTTON_RELEASE = 1,
        BUTTON_HOLD = 2,
	BUTTON_HOLD_LONG = 3,
	BUTTON_HOLD_VERY_LONG = 4
	
}eButton_Status;     

typedef enum eIMU_STATUS{
        IMU_Idle = 0,
        IMU_Sampling = 1,
        IMU_Sampled = 2,
	IMU_Sample = 3
}eIMU_STATUS;

typedef enum eLED_LED{
        LED_IDLE = 0,
        LED_5HZ = 1,
        LED_10HZ = 2,
	LED_ALWAYS_ON = 3
}eLED_STATUS;

typedef enum eSerial_Status{
        Serial_Idle = 0,
        Serial_Receiving = 1,
        Serial_Received = 2
}eSerial_Status;

typedef struct Cyberry_Potter_Status_Typedef{
        eSystem_Mode System_Mode;
        eButton_Status Button_Status;
        uint8_t Button_Press_Count;
        uint8_t Button_Click_Count;
        eIMU_STATUS IMU_Status;
	eLED_STATUS LED_Status;
        eSerial_Status Serial_Status;
}Cyberry_Potter_Status_Typedef;

void Cyberry_Potter_System_Status_Update(void);
void System_Init(void);
void Cyberry_Potter(void);
void LED_Blink(void);

#endif	//_CYBERRY_POTTER_H_

#include "config.h"

//********************************************************************************
#ifndef	_SYSTEM_STATUS_H_
#define	_SYSTEM_STATUS_H_

typedef enum eButton_Status{
        Button_Release = 0,
        Button_Hold = 1
}eButton_Status;     

typedef enum eButton_Time_Count{
        BUTTON_IDLE = 0,
        BUTTON_SHORT = 1,
        BUTTON_HOLD_LONG = 2,
        BUTTON_RELEASE_SHORT = 3
}eButton_Time_Count;    

typedef enum eSignal_Status{
        SIGNAL_EMPTY = 0,
        SIGNAL_LOADED = 1,
        SIGNAL_RECORDING = 2,
        SIGNAL_RECORDED = 3,
        SIGNAL_SENDING = 4,
        SIGNAL_SENT = 5
}eSignal_Status;

typedef enum eSignal_Type{
        Signal_Type_None = 0,
        Signal_Type_IR = 1,
        Signal_Type_FR_433MHZ = 2
}eSignal_Type;

typedef enum eLED_STATUS{
        LED_OFF = 0,
        LED_ON = 1,
        LED_BLINK_2HZ = 2,
        LED_BLINK_5HZ = 3,
        LED_BLINK_10HZ = 4
}eLED_STATUS;

typedef enum eIMU_STATUS{
        IMU_IDLE = 0,
        IMU_Sampling = 1,
        IMU_Sampled = 2
}eIMU_STATUS;

typedef enum eSerial_Status{
        Serial_Idle = 0,
        Serial_Receiving = 1,
        Serial_Received = 2
}eSerial_Status;

typedef struct Cyberry_Potter_Status_Typedef{
        eButton_Status Button_Status;
        eButton_Time_Count Button_Count;
        uint8_t Button_Click_Count;
        eSignal_Status Signal_Status;
        eSignal_Type Signal_Type;
        eLED_STATUS LED_Status;
        eIMU_STATUS IMU_Status;
        eSerial_Status Serial_Status;
}Cyberry_Potter_Status_Typedef;


#endif	//_SYSTEM_STATUS_H_

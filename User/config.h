#ifndef _CONFIG_H_
#define _CONFIG_H_

// This file contains compile-time configurations for Cyberry Potter magical wand internal system.


#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define running mode.
// NOTE: You can change the wand into different mode by the following defines.
//      When you change into another mode,you should comment the define of previous mode. 
// CYBERRY_POTTER_MODE_NORMAL: Normal mode.
// CYBERRY_POTTER_MODE_DATA_COLLECTOR: Running at data collecting mode to collect 
//      data from IMU and then send those data to serial port.Those data are used
//      to training motion identify model.

//#define CYBERRY_POTTER_MODE_NORMAL
#define CYBERRY_POTTER_MODE_DATA_COLLECTOR


//Define system configuration
#define SYSTEM_FREQUENCY (72000000)
#define SYSTEM_MODE_NORMAL
//#define SYSTEM_MODE_DATA_COLLECT

// Define Serial baud rate
// #define BAUD_RATE 9600
//#define BAUD_RATE (115200)
#define BAUD_RATE (921600)

//Define TIM4
#define TIM4_PER_INTERRUPT_MS (1)

//Define BUTTON 
#define BUTTON_IDLE_SHORT_THRESHOLD_MS (5)
#define BUTTON_SHORT_LONG_THRESHOLD_MS (1000)
#define BUTTON_LONG_VERYLONG_THRESHOLD_MS (3000)

//Define some useful constant
#define PI (3.14159265f)

//Define IMU
#define IMU_IS_MPU6050
#define IMU_ACC_MAX 4
#define IMU_GYRO_MAX 500
#define IMU_DATA_PRINT_HEADER "IMU\n"
#define IMU_DEFAULT_HZ  (40)
#define IMU_SAMPLING_TIME_MS (2000)
#define IMU_SAMPLING_TIME_S (1.5)
#define IMU_SEQUENCE_LENGTH_MAX (60)
#define IMU_SAMPLING_DELAY_MS (IMU_SAMPLING_TIME_MS/IMU_SEQUENCE_LENGTH_MAX)
#define IMU_TIME_INTERVAL_S (0.025)
#define IMU_ACC_TRANS_CONSTANT (8192.0)  //+-4g
//+-500 degrees raw value to degree constant is divided by 16.384
//+-500 to radian is divided by (73.537*180/PI) = 4213.359738
#define IMU_GYRO_TRANS_RADIAN_CONSTANT (4213.359738) \

//Define CNN
//The out put of model must bigger than this value unless it will give a unrecognized message
#define OUPUT_THRESHOLD 103 

#endif  //_CONFIG_H_

#include "config.h"
#include "CyberryPotter.h"
#include "stm32f10x.h"
#include "IIC.h"
#include "MPU6050_Reg.h"

typedef enum IMU_ACC_Index{
	AccX = 0,
	AccY = 1,
	AccZ = 2,

}IMU_ACC_Index;

typedef enum IMU_GYRO_Index{
	Roll = 0,
	Pitch = 1,
	Yaw = 2

}IMU_GYRO_Index;

extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;

//measured data beginning with m, d means derivative ,mdAngle is measured angular velovity in this case.
float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
float IMU_Data_mGyro[IMU_SEQUENCE_LENGTH_MAX][3];
int16_t IMU_bias[6] = {0,0,0,0,0,0};


//This function is used to print the Acc data to your computer
//IMU_DATA_PRINT_HEADER is the header that the script use to identify which the following messages is IMU data or not.
#ifdef SYSTEM_MODE_DATA_COLLECT
void IMU_Data_Print(void){
	int16_t i = 0;
	printf(IMU_DATA_PRINT_HEADER);
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
	printf("%f %f %f %f %f %f\n",
		IMU_Data_mAcc[i][AccX], IMU_Data_mAcc[i][AccY], IMU_Data_mAcc[i][AccZ],
		IMU_Data_mdAngle[i][Roll], IMU_Data_mdAngle[i][Pitch], IMU_Data_mdAngle[i][Yaw]);
	}
}
#endif //SYSTEM_MODE_DATA_COLLECT

void IMU_Sample_Start(void)
{
	EXTI_Stop();
	Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
	LED_OFF;
	IMU_START();
	Cyberry_Potter_Status.IMU_Status = IMU_Sampling;
}

#ifndef IMU_OFFSET_TEST
void IMU_Get_Data(uint8_t i)
{
	uint8_t temp[6];
	int16_t IMU_Received[6];
	IIC_read(0x68,MPU6050_RA_ACCEL_XOUT_H,6,temp);
	IMU_Received[AccX] = (temp[0] << 8) + temp[1] - IMU_bias[AccX];
	IMU_Received[AccY] = (temp[2] << 8) + temp[3] - IMU_bias[AccY];
	IMU_Received[AccZ] = (temp[4] << 8) + temp[5] - IMU_bias[AccZ];
	
	IMU_Data_mAcc[i][AccX] = IMU_Received[AccX] / IMU_ACC_TRANS_CONSTANT;
	IMU_Data_mAcc[i][AccY] = IMU_Received[AccY] / IMU_ACC_TRANS_CONSTANT;
	IMU_Data_mAcc[i][AccZ] = IMU_Received[AccZ] / IMU_ACC_TRANS_CONSTANT;
	
	IIC_read(0x68,MPU6050_RA_GYRO_XOUT_H,6,temp);
	IMU_Received[Roll] = (temp[0] << 8) + temp[1] - IMU_bias[Roll];
	IMU_Received[Pitch] = (temp[2] << 8) + temp[3]- IMU_bias[Pitch];
	IMU_Received[Yaw] = (temp[4] << 8) + temp[5]  - IMU_bias[Yaw];
	
	IMU_Data_mGyro[i][Roll] = IMU_Received[Roll] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
	IMU_Data_mGyro[i][Pitch] = IMU_Received[Pitch] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
	IMU_Data_mGyro[i][Yaw] = IMU_Received[Yaw] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
}

#endif //IMU_OFFSET_TEST

#ifdef IMU_OFFSET_TEST
void IMU_Get_Data(uint8_t i)
{
	uint8_t temp[6];
	int16_t IMU_Received[3];
	IIC_read(0x68,MPU6050_RA_ACCEL_XOUT_H,6,temp);
	IMU_Received[AccX] = (temp[0] << 8) + temp[1] - IMU_bias[AccX];
	IMU_Received[AccY] = (temp[2] << 8) + temp[3] - IMU_bias[AccY];
	IMU_Received[AccZ] = (temp[4] << 8) + temp[5] - IMU_bias[AccZ];
	
	printf("AccX:%d AccY:%d AccZ:%d\n",
		IMU_Received[AccX],IMU_Received[AccY],IMU_Received[AccZ]);
}
#endif //IMU_OFFSET_TEST

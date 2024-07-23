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

#define LP_CONST 0.2

//measured data beginning with m, d means derivative ,mdAngle is measured angular velovity in this case.
float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
float IMU_Data_mdAngle[IMU_SEQUENCE_LENGTH_MAX][3];
float IMU_Data_mAngle[IMU_SEQUENCE_LENGTH_MAX][3];

//predict data beginning with p
float IMU_Data_pACC[IMU_SEQUENCE_LENGTH_MAX][3];
float IMU_Data_pAngle[IMU_SEQUENCE_LENGTH_MAX][3];

//Kalman filter
float K_Gain[2][2];
float K_Covariance[2][2];

//
int16_t IMU_bias[6] = {0,0,1228,-30,32,40};


//fast inverse sqrt function
//https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}


//When this function is called, EXTI line 5 reletive to PB5 will be enable.
//MCU is going to get data from IMU when IMU pull down the PB5
//The sampling rate can be configure by modify IMU_DEFAULT_HZ in config.h
void IMU_Sample_Start(void)
{
	Cyberry_Potter_Status.IMU_Status = IMU_Sampling;
}


//This function is used to print the Acc data to your computer
//IMU_DATA_PRINT_HEADER is the header that the script use to identify which the following messages is IMU data or not.
void IMU_Data_Print(void){
	int16_t i = 0;
	printf(IMU_DATA_PRINT_HEADER);
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
	printf("%f %f %f %f %f %f\n",
		IMU_Data_mAcc[i][AccX], IMU_Data_mAcc[i][AccY], IMU_Data_mAcc[i][AccZ],
		IMU_Data_mdAngle[i][Roll], IMU_Data_mdAngle[i][Pitch], IMU_Data_mdAngle[i][Yaw]);
	}
//	printf("%f %f %f %f %f %f\n",
//		IMU_Data_mAcc[i][AccX], IMU_Data_mAcc[i][AccY], IMU_Data_mAcc[i][AccZ],
//		IMU_Data_pAngle[i][Roll], IMU_Data_pAngle[i][Pitch], IMU_Data_pAngle[i][Yaw]);
//	}
}

void IMU_Initialize_Data(void)
{
	//Clear All Data 
	memset(IMU_Data_pACC,0,sizeof(IMU_Data_pACC));
	memset(IMU_Data_pAngle,0,sizeof(IMU_Data_pAngle));
	
	IMU_Data_pACC[0][AccX] = IMU_Data_mAcc[0][AccX];
	IMU_Data_pACC[0][AccY] = IMU_Data_mAcc[0][AccY];
	IMU_Data_pACC[0][AccZ] = IMU_Data_mAcc[0][AccZ];
	
	IMU_Data_pAngle[0][AccX] = IMU_Data_mdAngle[0][Roll];
	IMU_Data_pAngle[0][AccY] = IMU_Data_mdAngle[0][Pitch];
	IMU_Data_pAngle[0][AccZ] = IMU_Data_mdAngle[0][Yaw];
	
//	//Get Initial Angle
//	//Roll_0 = atan(y_0/z_0)
//	IMU_Data_pAngle[0][Roll] = atan(IMU_Data_mAcc[0][AccY] / IMU_Data_mAcc[0][AccZ]);
//	//Pitch_0 = atan(x0 / sqrt( (y_0)^2 + (z_0)^2 ) )  Note:Q_sqrt is fast inverse sqrt function.
//	IMU_Data_pAngle[0][Pitch] = -atan(IMU_Data_mAcc[0][AccX] * 
//		Q_rsqrt(IMU_Data_mAcc[0][AccY]*IMU_Data_mAcc[0][AccY] + IMU_Data_mAcc[0][AccZ] * IMU_Data_mAcc[0][AccZ]));
//	//Yaw = 0 at initial state.
//	IMU_Data_pAngle[0][Yaw] = 0;
	
//	//
//	memset(K_Gain,0,sizeof(K_Gain));
//	
//	//
//	K_Covariance[0][0] = 1;
//	K_Covariance[0][1] = 0;
//	K_Covariance[1][0] = 0;
//	K_Covariance[1][1] = 1;
}

void IMU_Get_Data(uint8_t i)
{
	uint8_t temp[6];
	int16_t IMU_Received[6];
	IIC_read(0x68,MPU6050_RA_ACCEL_XOUT_H,6,temp);
	IMU_Received[AccX] = (temp[0] << 8) + temp[1] - IMU_bias[AccX];
	IMU_Received[AccY] = (temp[2] << 8) + temp[3] - IMU_bias[AccY];
	IMU_Received[AccZ] = (temp[4] << 8) + temp[5] - IMU_bias[AccZ];
	
	IIC_read(0x68,MPU6050_RA_GYRO_XOUT_H,6,temp);
	IMU_Received[Roll] = (temp[0] << 8) + temp[1] - IMU_bias[Roll];
	IMU_Received[Pitch] = (temp[2] << 8) + temp[3]- IMU_bias[Pitch];
	IMU_Received[Yaw] = (temp[4] << 8) + temp[5]  - IMU_bias[Yaw];
	
	IMU_Data_mAcc[i][AccX] = IMU_Received[AccX] / IMU_ACC_TRANS_CONSTANT;
	IMU_Data_mAcc[i][AccY] = IMU_Received[AccY] / IMU_ACC_TRANS_CONSTANT;
	IMU_Data_mAcc[i][AccZ] = IMU_Received[AccZ] / IMU_ACC_TRANS_CONSTANT;

	IMU_Data_mdAngle[i][Roll] = IMU_Received[Roll] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
	IMU_Data_mdAngle[i][Pitch] = IMU_Received[Pitch] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
	IMU_Data_mdAngle[i][Yaw] = IMU_Received[Yaw] / IMU_GYRO_TRANS_RADIAN_CONSTANT;
}

//void IMU_LP_Filter(uint8_t i)
//{
//	IMU_Data_pACC[i][AccX] = IMU_Data_pACC[i-1][AccX] + LP_CONST * (IMU_Data_mAcc[AccX][i] - IMU_Data_pACC[i-1][AccX]);
//	IMU_Data_pACC[i][AccY] = IMU_Data_pACC[i-1][AccY] + LP_CONST * (IMU_Data_mAcc[AccY][i] - IMU_Data_pACC[i-1][AccY]);
//	IMU_Data_pACC[i][AccZ] = IMU_Data_pACC[i-1][AccZ] + LP_CONST * (IMU_Data_mAcc[AccZ][i] - IMU_Data_pACC[i-1][AccZ]);
//	
//	IMU_Data_pAngle[i][AccX] = IMU_Data_mdAngle[i][Roll];
//	IMU_Data_pAngle[i][AccY] = IMU_Data_mdAngle[i][Pitch];
//	IMU_Data_pAngle[i][AccZ] = IMU_Data_mdAngle[i][Yaw];
//}

//void IMU_Kalman_Filter(uint8_t i)
//{
//	float dt = IMU_TIME_INTERVAL_S;
//	IMU_Data_mdAngle[i][Roll] = IMU_Data_pACC[i-1][AccX] + ((sin(IMU_Data_pAngle[i-1][Pitch]) * sin(IMU_Data_pAngle[i-1][Roll])) / cos(IMU_Data_pAngle[i-1][Pitch])) * 
//			IMU_Data_pACC[i-1][AccY] + ((sin(IMU_Data_pAngle[i-1][Pitch]) * cos(IMU_Data_pAngle[i-1][Roll])) / cos(IMU_Data_pAngle[i-1][Pitch])) * IMU_Data_pACC[i-1][AccZ];
//	IMU_Data_mdAngle[i][Pitch] = cos(IMU_Data_pAngle[i-1][Roll]) * IMU_Data_pACC[i-1][AccY] - sin(IMU_Data_pAngle[i-1][Roll]) * IMU_Data_pACC[i-1][AccZ]; 
//	IMU_Data_pAngle[i][Roll] = IMU_Data_pAngle[i-1][Roll] + (dt * IMU_Data_mdAngle[i][Roll]);
//	IMU_Data_pAngle[i][Pitch] = IMU_Data_pAngle[i-1][Pitch] + (dt * IMU_Data_mdAngle[i][Pitch]);
//	
//	K_Covariance[0][0] = K_Covariance[0][0] + 0.0025;
//	K_Covariance[1][1] = K_Covariance[1][1] + 0.0025;

//	K_Gain[0][0] = K_Gain[0][0] / (K_Gain[0][0] + 0.3);
//	K_Gain[1][1] = K_Gain[1][1] / (K_Gain[1][1] + 0.3);
//	
//	//Roll_0 = atan(y_0/z_0)
//	IMU_Data_mAngle[i][Roll] = atan(IMU_Data_mAcc[i][AccY] / IMU_Data_mAcc[i][AccZ]);
//	//Pitch_0 = atan(x0 / sqrt( (y_0)^2 + (z_0)^2 ) )  Note:Q_sqrt is fast inverse sqrt function.
//	IMU_Data_mAngle[i][Pitch] = -atan(IMU_Data_mAcc[i][AccX] * 
//		Q_rsqrt(IMU_Data_mAcc[0][AccY]*IMU_Data_mAcc[i][AccY] + IMU_Data_mAcc[i][AccZ] * IMU_Data_mAcc[i][AccZ]));
//	
//	IMU_Data_pAngle[i][Roll]  = IMU_Data_pAngle[i][Roll] + K_Gain[0][0] * (IMU_Data_mAngle[i][Roll] - IMU_Data_pAngle[i][Roll]);
//	IMU_Data_pAngle[i][Pitch] = IMU_Data_pAngle[i][Pitch] + K_Gain[1][1] * (IMU_Data_mAngle[i][Pitch] - IMU_Data_pAngle[i][Pitch]);
//	
//	K_Covariance[0][0] = (1 - K_Gain[0][0]) * K_Covariance[0][0];
//	K_Covariance[1][1] = (1 - K_Gain[1][1]) * K_Covariance[1][1];
//	
//	IMU_Data_pACC[i][AccX] = -sin(IMU_Data_pAngle[i][Pitch]);
//	IMU_Data_pACC[i][AccY] = sin(IMU_Data_pAngle[i][Roll]) * cos(IMU_Data_pAngle[i][Pitch]);
//	IMU_Data_pACC[i][AccZ] = cos(IMU_Data_pAngle[i][Roll]) * cos(IMU_Data_pAngle[i][Pitch]);
//	
//	IMU_Data_pACC[i][AccX] =IMU_Data_mAcc[i][AccX] - IMU_Data_pACC[i][AccX];
//	IMU_Data_pACC[i][AccY] =IMU_Data_mAcc[i][AccY] - IMU_Data_pACC[i][AccY];
//	IMU_Data_pACC[i][AccZ] =IMU_Data_mAcc[i][AccZ] - IMU_Data_pACC[i][AccZ];
//	
//	IMU_Data_pACC[i][AccX] = cos(IMU_Data_pAngle[i][Pitch]) * IMU_Data_pACC[i][AccX] + sin(IMU_Data_pAngle[i][Pitch]) * sin(IMU_Data_pAngle[i][Roll]) * 
//				IMU_Data_pACC[i][AccY] + sin(IMU_Data_pAngle[i][Pitch]) * cos(IMU_Data_pAngle[i][Roll]) * IMU_Data_pACC[i][AccZ];
//	IMU_Data_pACC[i][AccY] = cos(IMU_Data_pAngle[i][Roll]) * IMU_Data_pACC[i][AccY] - sin(IMU_Data_pAngle[i][Roll]) * IMU_Data_pACC[i][AccZ];
//	IMU_Data_pACC[i][AccZ] = -sin(IMU_Data_pAngle[i][Pitch]) * IMU_Data_pACC[i][AccX] + cos(IMU_Data_pAngle[i][Pitch]) * sin(IMU_Data_pAngle[i][Roll]) * 
//	IMU_Data_pACC[i][AccY] + cos(IMU_Data_pAngle[i][Pitch]) * cos(IMU_Data_pAngle[i][Roll]) * IMU_Data_pACC[i][AccZ];
//	
//}                    

//void IMU_Get_Data(uint8_t index)
//{
//	uint8_t temp[6];
//	int16_t IMU_Received[6];
//	IIC_read(0x68,MPU6050_RA_ACCEL_XOUT_H,6,temp);
//	IMU_Received[AccX] = (temp[0] << 8) + temp[1] - IMU_bias[AccX];
//	IMU_Received[AccY] = (temp[2] << 8) + temp[3] - IMU_bias[AccY];
//	IMU_Received[AccZ] = (temp[4] << 8) + temp[5] - IMU_bias[AccZ];
//	
//	IIC_read(0x68,MPU6050_RA_GYRO_XOUT_H,6,temp);
//	IMU_Received[Roll] = (temp[0] << 8) + temp[1] - IMU_bias[Roll];
//	IMU_Received[Pitch] = (temp[2] << 8) + temp[3]- IMU_bias[Pitch];
//	IMU_Received[Yaw] = (temp[4] << 8) + temp[5]  - IMU_bias[Yaw];
//	
//	IMU_Data_ACC[index * 3] =     IMU_Received[AccX] / IMU_ACC_TRANS_CONSTANT;
//	IMU_Data_ACC[index * 3 + 1] = IMU_Received[AccY] / IMU_ACC_TRANS_CONSTANT;
//	IMU_Data_ACC[index * 3 + 2] = IMU_Received[AccZ] / IMU_ACC_TRANS_CONSTANT;
//}

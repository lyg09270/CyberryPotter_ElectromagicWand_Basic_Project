#include "config.h"
#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include <stdio.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "IIC.h"

static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		//printf("setting bias succesfully ......\r\n");
    }
}

static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
	unsigned short scalar;
	scalar = inv_row_2_scale(mtx);
	scalar |= inv_row_2_scale(mtx + 3) << 3;
	scalar |= inv_row_2_scale(mtx + 6) << 6;


	return scalar;
}

void MPU6050_Init(void)
{
	IIC_Init();
        long bias = 0;
        struct int_param_s *int_param;
        unsigned char temp[1]={0};
        IIC_read(0x68,MPU6050_RA_WHO_AM_I,1,temp);
	if(temp[0]!=0x68)NVIC_SystemReset();
	if(!mpu_init(int_param)){
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)){
			 //printf("mpu_set_sensor complete ......\r\n");
		}
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)){
			 //printf("mpu_configure_fifo complete ......\r\n");
		}
		if(!mpu_set_sample_rate(IMU_DEFAULT_HZ)){
			 //printf("mpu_set_sample_rate complete ......\r\n");
		}
		if(!dmp_load_motion_driver_firmware()){
			//printf("dmp_load_motion_driver_firmware complete ......\r\n");
		}
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))){
			 //printf("dmp_set_orientation complete ......\r\n");
		}
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL)){
			 //printf("dmp_enable_feature complete ......\r\n");
		    }
		if(!dmp_set_fifo_rate(IMU_DEFAULT_HZ)){
			 //printf("dmp_set_fifo_rate complete ......\r\n");
		}
		run_self_test();
			if(!mpu_set_dmp_state(0)){
				 //printf("mpu_set_dmp_state complete ......\r\n");
			}
		if(!dmp_set_gyro_bias(&bias)){
			  //printf("dmp_set_gyro_bias ......\r\n");
		}
		mpu_set_accel_fsr(4);
		mpu_set_gyro_fsr(500);
		printf("dmp init complete\r\n");
	  }
}



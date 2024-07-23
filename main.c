#include "CyberryPotter.h"
#include "weights.h"
#include "nnom.h"

extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;
//extern uint16_t IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
//extern float IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
extern float IMU_Data_mAngle[IMU_SEQUENCE_LENGTH_MAX][3];
extern float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
int8_t data_to_feed[3*IMU_SEQUENCE_LENGTH_MAX];
#define SCALE (pow(2,INPUT_1_OUTPUT_DEC))
#ifdef NNOM_USING_STATIC_MEMORY
	uint8_t static_buf[1024 * 8];
#endif //NNOM_USING_STATIC_MEMORY


//void model_feed_data(void)
//{
//	uint16_t i = 0;
//	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
//		nnom_input_data[i*6] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][0] * SCALE)));
//		nnom_input_data[i*6+1] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][1] * SCALE)));
//		nnom_input_data[i*6+2] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][2] * SCALE)));
//		nnom_input_data[i*6+3] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][0] * SCALE)));
//		nnom_input_data[i*6+4] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][1] * SCALE)));
//		nnom_input_data[i*6+5] = (int8_t)fmin(127, fmax(-128, round(IMU_Data_mAcc[i][2] * SCALE)));
//	}
//}

void model_feed_data(void)
{
	const double scale = SCALE;
	uint16_t i = 0;
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
		nnom_input_data[i*3] = (int8_t)round(IMU_Data_mAcc[i][0] * scale);
		nnom_input_data[i*3+1] = (int8_t)round(IMU_Data_mAcc[i][1] * scale);
		nnom_input_data[i*3+2] = (int8_t)round(IMU_Data_mAcc[i][2] * scale);
		//printf("rd1:%.2f,rd2:%.2f,rd3:%.2f",round(IMU_Data_mAcc[i][0] * scale),round(IMU_Data_mAcc[i][1] * scale),round(IMU_Data_mAcc[i][2] * scale));
	}
}

void IMU_ACC_Print(void)
{
	uint16_t i = 0;
	for(i = 0; i < 239;i++){
		printf("%d," ,nnom_input_data[i]);
	}
}

void model_get_output(void)
{
	uint8_t i = 0;
	int8_t max_output = -128;
	int8_t ret = 0;
	for(i = 0; i < 12;i++){
		printf("Output[%d] = %.2f %%\n",i,(nnom_output_data[i] / 127.0)*100);
		if(nnom_output_data[i] >= max_output){
			max_output = nnom_output_data[i] ;
			ret = i;
		}
	}
	if(max_output >= 64){
		ret = ret;
	}
	else{
		ret = -1;
	}
	
	switch(ret){
		case -1:
			printf("Unrecognized");
			break;
		case 0:
			printf("RightAngle");
			break;
		case 1:
			printf("SharpAngle");
			break;
		case 2:
			printf("Lightning");
			break;
		case 3:
			printf("Triangle");
			break;
		case 4:
			printf("Letter_h");
			break;
		case 5:
			printf("Letter_R");
			break;
		case 6:
			printf("Letter_W");
			break;
		case 7:
			printf("Letter_phi");
			break;
		case 8:
			printf("Circle");
			break;
		case 9:
			printf("UpAndDown");
			break;
		case 10:
			printf("Horn");
			break;
		case 11:
			printf("Wave");
			break;
	}
	printf("\n");
}


int  main(void)
{       
        System_Init();
        printf("Hello!\n");
        GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	#ifdef NNOM_USING_STATIC_MEMORY
		nnom_set_static_buf(static_buf, sizeof(static_buf)); 
	#endif //NNOM_USING_STATIC_MEMORY
	nnom_model_t* model;
	model = nnom_model_create();
	
	while(1)
	{
                Cyberry_Potter_System_Status_Update();
	
	if(Cyberry_Potter_Status.System_Mode == SYSTEM_TRANSMIT){
		if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD){
			IMU_START();	
			Cyberry_Potter_Status.Button_Status = BUTTON_RELEASE;
		}
		if(Cyberry_Potter_Status.IMU_Status == IMU_Sampled){
			Delay_ms(200);
			
			IMU_Data_Print();
			
//			model_feed_data();
//			model_run(model);
//			//IMU_ACC_Print();
//			model_get_output();
			Cyberry_Potter_Status.IMU_Status = IMU_Idle;
		}
			
	}
	else if(Cyberry_Potter_Status.System_Mode == SYSTEM_RECORD){
		
	}
	//SYSTEM_IDLE
	else{
		
	}     
                
	}
}

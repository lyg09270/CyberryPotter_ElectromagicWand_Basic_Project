#include "CyberryPotter.h"
#include "weights.h"
#include "nnom.h"

extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;
//extern uint16_t IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
//extern float IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
extern float IMU_Data_mAngle[IMU_SEQUENCE_LENGTH_MAX][3];
extern float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
#define SCALE (pow(2,INPUT_1_OUTPUT_DEC))

#ifdef NNOM_USING_STATIC_MEMORY
	uint8_t static_buf[1024 * 8];
#endif //NNOM_USING_STATIC_MEMORY

void model_feed_data(void)
{
	const double scale = SCALE;
	uint16_t i = 0;
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
		nnom_input_data[i*3] = (int8_t)round(IMU_Data_mAcc[i][0] * scale);
		nnom_input_data[i*3+1] = (int8_t)round(IMU_Data_mAcc[i][1] * scale);
		nnom_input_data[i*3+2] = (int8_t)round(IMU_Data_mAcc[i][2] * scale);
	}
}

int8_t model_get_output(void)
{
	uint8_t i = 0;
	int8_t max_output = -128;
	int8_t ret = 0;
	for(i = 0; i < 13;i++){
		printf("Output[%d] = %.2f %%\n",i,(nnom_output_data[i] / 127.0)*100);
		if(nnom_output_data[i] >= max_output){
			max_output = nnom_output_data[i] ;
			ret = i;
		}
	}
	if(max_output >= OUPUT_THRESHOLD){
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
		case 12:
			printf("Unrecognized");
			break;
	}
	printf("\n");
	return ret;
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
	while(1){
		
		//SYSTEM_TRANSMIT
		if(Cyberry_Potter_Status.System_Mode == SYSTEM_TRANSMIT){
			
			
			if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD && Cyberry_Potter_Status.IMU_Status != IMU_Sampling){
				IMU_Sample_Start();	
				printf("HOLD\n");
			}
			else{
				Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;	
			}
			
			if(Cyberry_Potter_Status.IMU_Status == IMU_Sampled){
				printf("IMU_Sampled");
				LED_ON;
				#ifdef SYSTEM_MODE_DATA_COLLECT
				Delay_ms(200);
				IMU_Data_Print();
				#endif //SYSTEM_MODE_DATA_COLLECT
				model_feed_data();
				model_run(model);
				printf("%d",model_get_output());
				Cyberry_Potter_Status.IMU_Status = IMU_Idle;
				EXTI_Restore();
			}
			
				
		}
		//SYSTEM_RECORD
		else if(Cyberry_Potter_Status.System_Mode == SYSTEM_RECORD){
			
		}
		
		//SYSTEM_IDLE
		else{ 
			Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
		}     
		    
	}
	
}

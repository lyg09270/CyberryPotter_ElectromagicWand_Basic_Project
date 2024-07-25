#include "CyberryPotter.h"
#include "weights.h"
#include "nnom.h"

extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;
//extern uint16_t IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
//extern float IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
extern float IMU_Data_mAngle[IMU_SEQUENCE_LENGTH_MAX][3];
extern float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
int8_t model_output = -1;
#define SCALE (pow(2,INPUT_1_OUTPUT_DEC))

typedef enum eModel_Output{
	Unrecognized = -1,
	RightAngle = 0,
	SharpAngle = 1,
	Lightning = 2,
	Triangle = 3,
	Letter_h = 4,
	letter_R = 5,
	letter_W = 6,
	letter_phi = 7,
	Circle = 8,
	UpAndDown = 9,
	Horn = 10,
	Wave = 11,
	NoMotion = 12
}eModel_Output;

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
		case Unrecognized:
			printf("Unrecognized");
			break;
		case RightAngle:
			printf("RightAngle");
			break;
		case SharpAngle:
			printf("SharpAngle");
			break;
		case Lightning:
			printf("Lightning");
			break;
		case Triangle:
			printf("Triangle");
			break;
		case Letter_h:
			printf("Letter_h");
			break;
		case letter_R:
			printf("Letter_R");
			break;
		case letter_W:
			printf("Letter_W");
			break;
		case letter_phi:
			printf("Letter_phi");
			break;
		case Circle:
			printf("Circle");
			break;
		case UpAndDown:
			printf("UpAndDown");
			break;
		case Horn:
			printf("Horn");
			break;
		case Wave:
			printf("Wave");
			break;
		case NoMotion:
			printf("Unrecognized");
			break;
	}
	printf("\n");
	return ret;
}

void Model_LED(int8_t model_output)
{
	switch(model_output){
		case RightAngle:
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
			break;
		case SharpAngle:
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
			break;
		case Lightning:
			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
			break;
		case Triangle:
			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
			break;
		case Letter_h:
			GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
			break;
		case letter_R:
			GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
			break;
		case letter_W:
			GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
			break;
		case letter_phi:
			GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
			break;
	}
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
		Cyberry_Potter_Status.System_Mode = SYSTEM_TRANSMIT;
		if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD && Cyberry_Potter_Status.IMU_Status == IMU_Idle){
			IMU_Sample_Start();	
			printf("HOLD\n");
		}
		else if(Cyberry_Potter_Status.Button_Status != BUTTON_IDLE){
			Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
		}

		if(Cyberry_Potter_Status.IMU_Status == IMU_Sampled){
			LED_ON;
			#ifdef SYSTEM_MODE_DATA_COLLECT
			Delay_ms(200);
			IMU_Data_Print();
			#endif //SYSTEM_MODE_DATA_COLLECT
			
			#ifndef SYSTEM_MODE_DATA_COLLECT
			model_feed_data();
			model_run(model);
			model_output = model_get_output();
			printf("%d",model_output);
			Model_LED(model_output);
			#endif //SYSTEM_MODE_DATA_COLLECT
			Cyberry_Potter_Status.IMU_Status = IMU_Idle;
			EXTI_Restore();
		}
		

	}
}

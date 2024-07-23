#ifdef __cplusplus 
extern "C" {
#endif

#include "CyberryPotter.h"
#include "model.h"

#ifdef __cplusplus 
}
#endif

	
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"




extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;
extern float IMU_Acc_Series;

int  main(void)
{       
        System_Init();
        printf("Hello!\n");
        GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
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

#include "CyberryPotter.h"

Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;

uint16_t i = 0;
uint16_t TIM4_Count;

void System_Init(void)
{
        Hardware_Init();
        Cyberry_Potter_Status.Button_Click_Count = 0;
        Cyberry_Potter_Status.Button_Status = BUTTON_RELEASE;
        Cyberry_Potter_Status.Button_Time_Count = BUTTON_IDLE;
        Cyberry_Potter_Status.Serial_Status = Serial_Idle;

	LED_ON;
}

//IMU read
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET){
		IMU_Get_Data(i);
//		if(i > 0)
//			//IMU_Kalman_Filter(i);
//			IMU_LP_Filter(i);
//		else{
//			IMU_Initialize_Data();
//		}
		i++;
		if(i >= IMU_SEQUENCE_LENGTH_MAX){
			Cyberry_Potter_Status.IMU_Status = IMU_Sampled;
			i = 0;
			IMU_STOP();
		}	
	EXTI_ClearITPendingBit(EXTI_Line5);	
        }
}

//Functions About Button******************************************************//
void Button_Status_Update(void)
{
	TIM4_Count++; 
        //When the button is pressed
        if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD){
		//The threshold between short press and long press is BUTTON_LONG_SHORT_THRESHOLD_100ms
		
                if (TIM4_Count == BUTTON_LONG_SHORT_THRESHOLD_100ms){
                        TIM4_Count = BUTTON_LONG_SHORT_THRESHOLD_100ms;
			
                        //BUTTON_SHORT_AND_LONG
                        if(Cyberry_Potter_Status.Button_Click_Count >= 1){
                                Cyberry_Potter_Status.Button_Time_Count = BUTTON_SHORT_AND_LONG;
                        }
                        //BUTTON_HOLD_LONG
                        else{
                                Cyberry_Potter_Status.Button_Time_Count = BUTTON_HOLD_LONG;
                        }
		}	
		//Button Hold long time keep status
		else if(TIM4_Count > BUTTON_LONG_SHORT_THRESHOLD_100ms){
			TIM4_Count = BUTTON_LONG_SHORT_THRESHOLD_100ms + 1;
		}
                
		//BUTTON_HOLD_SHORT
                else{
                        Cyberry_Potter_Status.Button_Time_Count = BUTTON_HOLD_SHORT;
                }
        }     
	
        //When the button is released
        else{
                //The threshold between idle and wait next long press is BUTTON_RELEASE_THRESHOLD_100ms  
                if (TIM4_Count >= BUTTON_RELEASE_THRESHOLD_100ms){
                        Cyberry_Potter_Status.Button_Time_Count = BUTTON_IDLE;
			Cyberry_Potter_Status.Button_Status = BUTTON_RELEASE;
                        Cyberry_Potter_Status.Button_Click_Count = 0;
                        TIM4_Count = 0;
                        TIMER_FOR_BUTTON_DISABLE;
                }
		//BUTTON_RELEASE_SHORT 
                else{
                        Cyberry_Potter_Status.Button_Time_Count = BUTTON_RELEASE_SHORT;
                        Cyberry_Potter_Status.Button_Click_Count = 1;
                }   
        }
}


//Button EXTI handler*******************************************************//
void EXTI0_IRQHandler(void)
{
	Delay_ms(10);
        if(EXTI_GetITStatus(EXTI_Line0)==SET){
		TIM4_Count = 0;
		//Button is pressed
                if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == RESET){
                        Cyberry_Potter_Status.Button_Status = BUTTON_HOLD;
                        TIMER_FOR_BUTTON_ENABLE;
                }
		//Button is relessed
                else{
                        Cyberry_Potter_Status.Button_Status = BUTTON_RELEASE;
                }
                
                EXTI_ClearITPendingBit(EXTI_Line0);
        }
	Delay_ms(10);
}

//Button status update for a given time
void TIM4_IRQHandler(void)
{
        if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET){
               Button_Status_Update();
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}


void LED_Blink(void)
{
        while(1){
                LED_ON;
		Delay_ms(500);
                LED_OFF;    
		Delay_ms(500);		
        }
}



void Cyberry_Potter_System_Status_Update(void)
{
	if(Cyberry_Potter_Status.Button_Time_Count == BUTTON_SHORT_AND_LONG){
	      switch(Cyberry_Potter_Status.System_Mode){
		      case SYSTEM_IDLE:
				Cyberry_Potter_Status.System_Mode = SYSTEM_TRANSMIT;
				printf("SYSTEM_TRANSMIT\n");
				break;
		      case SYSTEM_TRANSMIT:
				Cyberry_Potter_Status.System_Mode = SYSTEM_RECORD;
				printf("SYSTEM_RECORD\n");
				break;
		      case SYSTEM_RECORD:
				Cyberry_Potter_Status.System_Mode = SYSTEM_IDLE;
				printf("SYSTEM_IDLE\n");
				break;
	      }
	}
	else {
		
	}
	Cyberry_Potter_Status.Button_Time_Count = BUTTON_IDLE;
}

//Main cycle**********************************************************************//
void Cyberry_Potter(void)
{
	
	
}

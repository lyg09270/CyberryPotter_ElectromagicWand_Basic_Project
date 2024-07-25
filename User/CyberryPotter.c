#include "CyberryPotter.h"

Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;

volatile uint16_t time_hold_count_ms = 0;
volatile uint16_t time_release_count_ms = 0;
volatile uint16_t i = 0;

void System_Init(void)
{
        Hardware_Init();
        Cyberry_Potter_Status.Button_Click_Count = 0;
        Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
        Cyberry_Potter_Status.Serial_Status = Serial_Idle;
	Cyberry_Potter_Status.IMU_Status = IMU_Idle;
	Cyberry_Potter_Status.LED_Status = LED_ALWAYS_ON;
	Cyberry_Potter_Status.System_Mode = SYSTEM_TRANSMIT;
	LED_ON;
}

void Cyberry_Potter_System_Status_Update(void)
{
	if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD_VERY_LONG){
	      switch(Cyberry_Potter_Status.System_Mode){
		      case SYSTEM_IDLE:
				Cyberry_Potter_Status.System_Mode = SYSTEM_TRANSMIT;
				Cyberry_Potter_Status.LED_Status = LED_10HZ;
				printf("SYSTEM_TRANSMIT\n");
				break;
		      case SYSTEM_TRANSMIT:
				Cyberry_Potter_Status.System_Mode = SYSTEM_RECORD;
				Cyberry_Potter_Status.LED_Status = LED_5HZ;
				printf("SYSTEM_RECORD\n");
				break;
		      case SYSTEM_RECORD:
				Cyberry_Potter_Status.System_Mode = SYSTEM_IDLE;
				Cyberry_Potter_Status.LED_Status = LED_IDLE;
				printf("SYSTEM_IDLE\n");
				break;
	      }  
	      LED_Blink();
	}
	else {
	}
	
}

//IMU read
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET){
		IMU_Get_Data(i);
		i++;
		if(i >= IMU_SEQUENCE_LENGTH_MAX){
			Cyberry_Potter_Status.IMU_Status = IMU_Sampled;
			i = 0;
			IMU_STOP();
		}	
	EXTI_ClearITPendingBit(EXTI_Line5);	
        }
}

//Button EXTI handler*******************************************************//
void EXTI0_IRQHandler(void)
{
        if(EXTI_GetITStatus(EXTI_Line0)==SET){
		//If previous button status is handled.Then button status must change to IDLE.
		if(Cyberry_Potter_Status.Button_Status == BUTTON_IDLE){
			TIMER_FOR_BUTTON_ENABLE;
		}
		else{}//Else do nothing
                EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

//Button status update for a given time
void TIM4_IRQHandler(void)
{
	static uint8_t previous_stuatus = 1;
        if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET){
		//Read Button status on GPIO for now
		uint8_t current_status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		
		//If status for now and previous is same,and button is held
		if(previous_stuatus == current_status && current_status == 0){
			
			time_hold_count_ms++;
			//If button status is stable for BUTTON_LONG_VERYLONG_THRESHOLD_MS
			if(time_hold_count_ms >= BUTTON_LONG_VERYLONG_THRESHOLD_MS){
					Cyberry_Potter_Status.Button_Status = BUTTON_HOLD_VERY_LONG;
					//Cyberry_Potter_System_Status_Update();
					Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
					time_release_count_ms = 0;
					TIMER_FOR_BUTTON_DISABLE;
				}
		}
		//If status for now and previous is same,and button is released
		else if(previous_stuatus == current_status && current_status == 1){
			time_release_count_ms++;
			//If button status is stable for BUTTON_IDLE_SHORT_THRESHOLD_MS 
			if(time_release_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS){
				
				//If button status is stable for BUTTON_SHORT_LONG_THRESHOLD_MS
				if(time_hold_count_ms >= BUTTON_SHORT_LONG_THRESHOLD_MS){
					Cyberry_Potter_Status.Button_Status = BUTTON_HOLD_LONG;
					printf("long ");
				}
				//If button status is stable for BUTTON_IDLE_SHORT_THRESHOLD_MS 
				else if(time_hold_count_ms >= BUTTON_IDLE_SHORT_THRESHOLD_MS){
					Cyberry_Potter_Status.Button_Status = BUTTON_HOLD;
					printf("hold ");
				}
				printf("hold:%d",time_hold_count_ms);
				time_hold_count_ms = 0;
				time_release_count_ms = 0;
				TIMER_FOR_BUTTON_DISABLE;
			}
			else{}	//Else do nothing.
		}
		//If status for now and previous is NOT same.
		else{}
			
		//Status update 
		previous_stuatus = current_status;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}


void LED_Blink(void)
{
	int i = 0;
        if(Cyberry_Potter_Status.LED_Status == LED_5HZ){
		for(i = 0; i < 6; i++){
                LED_OFF;
		Delay_ms(100);
                LED_ON;    
		Delay_ms(100);		
		}
		Cyberry_Potter_Status.LED_Status = LED_ALWAYS_ON;
        }
	else if(Cyberry_Potter_Status.LED_Status == LED_10HZ){
		for(i = 0; i < 11; i++){
                LED_OFF;
		Delay_ms(50);
                LED_ON;    
		Delay_ms(50);		
		}
		Cyberry_Potter_Status.LED_Status = LED_ALWAYS_ON;
        }
	else if (Cyberry_Potter_Status.LED_Status == LED_ALWAYS_ON){
		LED_ON;
	}
	else{
		LED_ON;
		Delay_ms(200);
		LED_OFF;
		Delay_ms(500);
		LED_ON;
		Delay_ms(300);
		Cyberry_Potter_Status.LED_Status = LED_ALWAYS_ON;
	}
}

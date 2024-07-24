#include "hardware.h"

#ifdef IMU_IS_MPU6050 
	#include "MPU6050_Reg.h"
	#include "MPU6050.h"
#endif //IMU_IS_MPU6050

void IMU_WriteReg(uint8_t RegAddress, uint8_t Data);

void Hardware_Init()
{
//RCC periphral init****************************************************//   
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
        
        GPIO_InitTypeDef GPIO_InitStruct;
        TIM_TimeBaseInitTypeDef TIM_TimerBaseInitStruct;
        EXTI_InitTypeDef EXTI_InitStruct;
        NVIC_InitTypeDef NVIC_InitStruct;
	
//Button Init****************************************************//    
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStruct);
        
        EXTI_InitStruct.EXTI_Line = EXTI_Line0;
        EXTI_InitStruct.EXTI_LineCmd = ENABLE;
        EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        EXTI_Init(&EXTI_InitStruct);
        
        NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
        NVIC_Init(&NVIC_InitStruct);
//TIM4 Init******************************************************//          
        TIM_TimerBaseInitStruct.TIM_Prescaler = 3600 - 1;
	TIM_TimerBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimerBaseInitStruct.TIM_Period = 20 - 1;                   //1ms per interrupt
	TIM_TimerBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIM_TimerBaseInitStruct);
        
        TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 4;
	NVIC_Init(&NVIC_InitStruct);   
        
//USART init*****************************************//
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BAUD_RATE;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1,ENABLE);
        printf("Hello!\n");
//LED init*****************************************//      
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
 //IMU init*****************************************//    	
	
	#ifdef IMU_IS_MPU6050
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5); 
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line5 ;
        EXTI_InitStruct.EXTI_LineCmd = ENABLE;
        EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 6;
        NVIC_Init(&NVIC_InitStruct);
	
	IMU_STOP();
	MPU6050_Init();
	#endif //IMU_IS_MPU6050	
}

int fputc( int ch, FILE *f )
{
        while((USART1->SR & (1 << 7) ) == 0);
        USART1->DR = ch;
	return ch;
}

void EXTI_Stop(void)
{
	EXTI->IMR &= ~(EXTI_Line0);
	EXTI->IMR &= ~(EXTI_Line4);
	EXTI->IMR &= ~(EXTI_Line8);
}

void EXTI_Restore(void)
{
	EXTI->IMR |= EXTI_Line0;
	EXTI->IMR |= EXTI_Line4;
	EXTI->IMR |= EXTI_Line8;
}

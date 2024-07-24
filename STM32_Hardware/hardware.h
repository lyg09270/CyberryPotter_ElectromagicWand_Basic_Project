#include "config.h"
#include "stm32f10x.h"

//********************************************************************************
#ifndef	_HARDWARE_H_
#define	_HARDWARE_H_

#define IMU_STOP() EXTI->IMR &= ~(EXTI_Line5)
#define IMU_START() EXTI->IMR |= EXTI_Line5

#define TIMER_FOR_BUTTON_ENABLE TIM_Cmd(TIM4,ENABLE)
#define TIMER_FOR_BUTTON_DISABLE TIM_Cmd(TIM4,DISABLE)

#define LED_ON GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET)
#define LED_OFF GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET)

void Hardware_Init(void);
void EXTI_Stop(void);
void EXTI_Restore(void);

#endif	//_HARDWARE_H_


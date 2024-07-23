/********************************************************************************
  * @file    IIC.h
  * @author  Mushroom
  * @version  1.0.0
  * @date    1-Oct-20
  * @brief   This file prodides MPU6050 functions implementation
  *******************************************************************************/

#ifndef	_IIC_H_
#define	_IIC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header

/* Functions -----------------------------------------------------------------*/
void IIC_Init(void);
void IIC_Send_Byte(uint8_t SendData);
uint8_t IIC_Receive_Byte(void);
int IIC_write(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t const *data);
int IIC_read(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t *data);

#endif	//_IIC_H_

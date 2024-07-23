/**
  ******************************************************************************
  * @file    IIC.c
  * @author  Mushroom
  * @version  1.0.0
  * @date    1-Oct-20
  * @brief   This file prodides MPU6050 functions implementation
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "IIC.h"

/* Macros --------------------------------------------------------------------*/
#define IIC_SDA(BitVal) GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)BitVal);
#define IIC_SCL(BitVal) GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)BitVal);
#define IIC_READ_SCL_PIN() GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
#define IIC_Delay(us) Delay_us(us) 
#define IIC_DELAY_TIME 5	//200kHz

//Fuctions implementation

/********************************************************************************
  * @brief Initialize GPIO that software IIC will uses.
  * @param  None
  * @retval None 
  *******************************************************************************/
void IIC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//IIC GPIO initialization
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	IIC_SDA(1);
	IIC_SCL(1);
}

/********************************************************************************
  * @brief Write a bit to SDA pin.
  * @param  BitVal can be 1 or 0 
  * @retval None 
  *******************************************************************************/
static void IIC_W_SDA(uint8_t BitVal)
{
	IIC_SDA(BitVal);
	IIC_Delay(IIC_DELAY_TIME);
}

/********************************************************************************
  * @brief Write a bit to SCL pin.
  * @param  BitVal can be 1 or 0 
  * @retval None 
  *******************************************************************************/
static void IIC_W_SCL(uint8_t BitVal)
{
	IIC_SCL(BitVal);
	IIC_Delay(IIC_DELAY_TIME);
}

/********************************************************************************
  * @brief Read a bit from SDA pin.
  * @param  None
  * @retval Bitval: 1 or 0 
  *******************************************************************************/
static uint8_t IIC_R_SDA(void)
{
	uint8_t BitVal = 0;
	//IIC_Delay(IIC_DELAY_TIME);
	BitVal = IIC_READ_SCL_PIN();
	return BitVal;
}

/********************************************************************************
  * @brief Genrate a IIC start signal.
  * @param  None
  * @retval None
  *******************************************************************************/
static void IIC_Start(void)
{
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	IIC_W_SDA(0);
	IIC_W_SCL(0);
}

/********************************************************************************
  * @brief Genrate a IIC stop signal.
  * @param  None
  * @retval None
  *******************************************************************************/
static void IIC_Stop(void)
{
	IIC_W_SDA(0);
	IIC_W_SCL(1);
	IIC_W_SDA(1);
}

/********************************************************************************
  * @brief  Send a byte of data to some devices.
  * @param  uint8_t SendData: A byte of data that need to send to some devices.
  * @retval None
  *******************************************************************************/
void IIC_Send_Byte(uint8_t SendData)
{
	int i = 0;
	for(i = 0; i < 8; i++){
		IIC_W_SDA(SendData & (0x80 >> i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
	
}

/********************************************************************************
  * @brief  Receive a byte of data from some devices.
  * @param  None
  * @retval uint8_t SendData: A byte of data that received from some devices.
  *******************************************************************************/
uint8_t IIC_Receive_Byte(void)
{
	uint8_t i = 0;
	uint8_t ReceivedData = 0;
	
	IIC_W_SDA(1);
	for(i = 0; i < 8; i++){
                IIC_W_SCL(1);
		if (IIC_R_SDA() == 1){
			ReceivedData |= (0x80 >> i);
		}
		IIC_W_SCL(0);
	}
	return ReceivedData;
}

/********************************************************************************
  * @brief Genrate a IIC ACK or NACK signal.
  * @param  Ack: 1 = NACK    0 = ACK
  * @retval None
  *******************************************************************************/
static void IIC_Send_Ack(uint8_t Ack)
{
	IIC_W_SDA(Ack);
	IIC_W_SCL(1);
	IIC_W_SCL(0);
}

/********************************************************************************
  * @brief Recieve a IIC ACK or NACK signal.
  * @param  None
  * @retval Ack: 1 = NACK    0 = ACK.
  *******************************************************************************/
static uint8_t IIC_Receive_Ack(void)
{
        uint8_t AckBit;	
	IIC_W_SDA(1);
	IIC_W_SCL(1);
        AckBit = IIC_R_SDA();
	IIC_W_SCL(0);
	return AckBit;
}

/********************************************************************************
  * @brief Write a set of data or one byte of data to target register of target device.
  * @param  uint8_t slave_addr : Address of target device.
  * @param  uint8_t reg_addr : Address of target register of target device.
  * @param  uint8_t length : How many byte of data in this transmission 
  * @param  uint8_t const *data : Pointer to first of the set of data.
  * @retval 0 for success. 
  *******************************************************************************/
int IIC_write(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t const *data)
{
        int i = 0;
        IIC_Start();
	IIC_Send_Byte(slave_addr << 1);
	IIC_Receive_Ack();
	IIC_Send_Byte(reg_addr);
        IIC_Receive_Ack();
        for(i = 0; i < length; i++)
        {
                IIC_Send_Byte(data[i]);
                IIC_Receive_Ack();      
        }
	IIC_Stop();
        return 0;
}

/********************************************************************************
  * @brief Read a set of data or one byte of data to target register of target device.
  * @param  uint8_t slave_addr : Address of target device.
  * @param  uint8_t reg_addr : Address of target register of target device.
  * @param  uint8_t length : How many bytes of data in this transmission 
  * @param  uint8_t const *data : Pointer to first of the set of data.
  * @retval 0 for success. 
  *******************************************************************************/
int IIC_read(uint8_t slave_addr, uint8_t reg_addr,uint8_t length, uint8_t *data)
{
	IIC_Start();
	IIC_Send_Byte(slave_addr << 1);
	IIC_Receive_Ack();
	IIC_Send_Byte(reg_addr);
	IIC_Receive_Ack();
	
	IIC_Start();
	IIC_Send_Byte((slave_addr << 1) + 1);
	IIC_Receive_Ack();
        while (length) {
                if (length == 1){
                    *data = IIC_Receive_Byte();
                    IIC_Send_Ack(1);
                }
                else{
                    *data = IIC_Receive_Byte();
                    IIC_Send_Ack(0);
                }
                data++;
                length--;
        }
        
	IIC_Stop();
        return 0;
}

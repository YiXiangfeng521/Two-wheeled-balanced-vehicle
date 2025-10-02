#ifndef __MPU_SOFT_IIC_H
#define __MPU_SOFT_IIC_H

#include "stm32f10x.h"
#include <inttypes.h>

#define Soft_I2C_SDA 		  GPIO_Pin_3
#define Soft_I2C_SCL 		  GPIO_Pin_4
#define Soft_I2C_PORT     GPIOB
#define MPU6050_GPIO_CLK  RCC_APB2Periph_GPIOB

//SDA方向设置（PB3）
#define MPU_SDA_IN()  {Soft_I2C_PORT ->CRL &= 0XFFFF0FFF;Soft_I2C_PORT ->CRL |= 8<<12;}       //上拉/下拉     输入模式
#define MPU_SDA_OUT() {Soft_I2C_PORT ->CRL &= 0XFFFF0FFF;Soft_I2C_PORT ->CRL |= 7<<12;}		    //通用开漏输出  输出模式（最大速度50MHz ）

/* 定义写SCL和读写SDA的宏，以增加代码的可移植性和可阅读性 */
#define MPU6050_I2C_SCL_1()     GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SCL)		          /* SCL = 1 */
#define MPU6050_I2C_SCL_0()     GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SCL)		        /* SCL = 0 */																																						        
#define MPU6050_I2C_SDA_1()     GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SDA)		          /* SDA = 1 */
#define MPU6050_I2C_SDA_0()     GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SDA)		        /* SDA = 0 */
#define MPU6050_I2C_SDA_READ()  GPIO_ReadInputDataBit(Soft_I2C_PORT, Soft_I2C_SDA)	  /* 读SDA口线状态 */


void MPU_IIC_Delay(void);								//IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);								//发送IIC开始信号
void MPU_IIC_Stop(void);	  						//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 _ucByte);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void MPU_IIC_Ack(void);									//IIC发送ACK信号
void MPU_IIC_NAck(void);								//IIC不发送ACK信号

#endif /*__MPU_SOFT_IIC_H*/

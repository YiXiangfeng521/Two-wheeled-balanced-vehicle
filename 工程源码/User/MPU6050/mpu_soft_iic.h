#ifndef __MPU_SOFT_IIC_H
#define __MPU_SOFT_IIC_H

#include "stm32f10x.h"
#include <inttypes.h>

#define Soft_I2C_SDA 		  GPIO_Pin_3
#define Soft_I2C_SCL 		  GPIO_Pin_4
#define Soft_I2C_PORT     GPIOB
#define MPU6050_GPIO_CLK  RCC_APB2Periph_GPIOB

//SDA�������ã�PB3��
#define MPU_SDA_IN()  {Soft_I2C_PORT ->CRL &= 0XFFFF0FFF;Soft_I2C_PORT ->CRL |= 8<<12;}       //����/����     ����ģʽ
#define MPU_SDA_OUT() {Soft_I2C_PORT ->CRL &= 0XFFFF0FFF;Soft_I2C_PORT ->CRL |= 7<<12;}		    //ͨ�ÿ�©���  ���ģʽ������ٶ�50MHz ��

/* ����дSCL�Ͷ�дSDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#define MPU6050_I2C_SCL_1()     GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SCL)		          /* SCL = 1 */
#define MPU6050_I2C_SCL_0()     GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SCL)		        /* SCL = 0 */																																						        
#define MPU6050_I2C_SDA_1()     GPIO_SetBits(Soft_I2C_PORT, Soft_I2C_SDA)		          /* SDA = 1 */
#define MPU6050_I2C_SDA_0()     GPIO_ResetBits(Soft_I2C_PORT, Soft_I2C_SDA)		        /* SDA = 0 */
#define MPU6050_I2C_SDA_READ()  GPIO_ReadInputDataBit(Soft_I2C_PORT, Soft_I2C_SDA)	  /* ��SDA����״̬ */


void MPU_IIC_Delay(void);								//IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);								//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  						//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 _ucByte);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);									//IIC����ACK�ź�
void MPU_IIC_NAck(void);								//IIC������ACK�ź�

#endif /*__MPU_SOFT_IIC_H*/

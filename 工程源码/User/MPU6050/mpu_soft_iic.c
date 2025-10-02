/**
  ******************************************************************************
  * @file    mpu_soft_iic.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-28
  * @brief   该文件通过软件方式在GPIO上模拟出IIC通信
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
	
#include "mpu_soft_iic.h"

/**********************************************************************
 * 函数名称： MPU_IIC_Delay
 * 功能描述： IIC延时函数
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU_IIC_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205K Hz 
		循环次数为7时， SCL频率  = 347K Hz
	 	循环次数为5时， SCL频率  = 421K Hz
	  （MPU6050模块的iic通信最大速度为400kHz）
	*/
	for (i = 0; i < 10; i++);
}

/**********************************************************************
 * 函数名称： MPU_IIC_Init
 * 功能描述： 初始化作为SDA和SCL的GPIO
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(MPU6050_GPIO_CLK|RCC_APB2Periph_AFIO,ENABLE);	
	
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);    //对应问题汇总的NO.7
	
  GPIO_InitStructure.GPIO_Pin = Soft_I2C_SCL|Soft_I2C_SDA;	 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 				  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				  
  GPIO_Init(Soft_I2C_PORT , &GPIO_InitStructure);					
	
  MPU6050_I2C_SCL_1();MPU6050_I2C_SDA_1();	                 //默认释放SCL和SDA总线（IIC空闲）	
 
}

/**********************************************************************
 * 函数名称： MPU_IIC_Start
 * 功能描述： 模拟IIC起始信号
              START:when CLK is high,DATA change form high to low 
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();      //sda线为输出模式
	MPU6050_I2C_SDA_1();	  	  
	MPU6050_I2C_SCL_1();
	MPU_IIC_Delay();
 	MPU6050_I2C_SDA_0();
	MPU_IIC_Delay();
	MPU6050_I2C_SCL_0();//钳住I2C总线，准备发送或接收数据 
}	  
/**********************************************************************
 * 函数名称： MPU_IIC_Stop
 * 功能描述： 模拟IIC停止信号
              STOP:when CLK is high DATA change form low to high 
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();
	MPU6050_I2C_SCL_0();
	MPU6050_I2C_SDA_0();
 	MPU_IIC_Delay();
	MPU6050_I2C_SCL_1();  
	MPU6050_I2C_SDA_1();
	MPU_IIC_Delay();							   	
}
      
/**********************************************************************
 * 函数名称： MPU_IIC_Wait_Ack
 * 功能描述： 等待应答信号到来
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 1 - 接收应答失败
              0 - 接收应答成功
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();                        //SDA设置为输入模式
	MPU6050_I2C_SDA_1();MPU_IIC_Delay();	   
	MPU6050_I2C_SCL_1();MPU_IIC_Delay();	 
	while(MPU6050_I2C_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU6050_I2C_SCL_0();  
	return 0;  
} 
/**********************************************************************
 * 函数名称： MPU_IIC_Ack
 * 功能描述： 产生ACK应答
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU_IIC_Ack(void)
{
	MPU6050_I2C_SCL_0();
	MPU_SDA_OUT();
	MPU6050_I2C_SDA_0();
	MPU_IIC_Delay();
	MPU6050_I2C_SCL_1();
	MPU_IIC_Delay();
	MPU6050_I2C_SCL_0();
}
/**********************************************************************
 * 函数名称： MPU_IIC_NAck
 * 功能描述： 无ACK应答
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/	    
void MPU_IIC_NAck(void)
{
	MPU6050_I2C_SCL_0();
	MPU_SDA_OUT();
	MPU6050_I2C_SDA_1();
	MPU_IIC_Delay();
	MPU6050_I2C_SCL_1();
	MPU_IIC_Delay();
	MPU6050_I2C_SCL_0();
}					 				     		
/**********************************************************************
 * 函数名称： MPU_IIC_Send_Byte
 * 功能描述： IIC发送一个字节
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 1 - 从机有应答
							0 - 从机无应答
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/	
void MPU_IIC_Send_Byte(u8 _ucByte)
{                        

		uint8_t i;
	
   MPU_SDA_OUT(); 
	 MPU6050_I2C_SCL_0();     //拉低时钟开始数据传输
	
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			MPU6050_I2C_SDA_1();
		}
		else
		{
			MPU6050_I2C_SDA_0();
		}
		_ucByte <<= 1;	/* 左移一个bit */
		
		MPU6050_I2C_SCL_1();
		MPU_IIC_Delay();	
		MPU6050_I2C_SCL_0();
		MPU_IIC_Delay();
	
	}
} 	      
/**********************************************************************
 * 函数名称： MPU_IIC_Read_Byte
 * 功能描述： IIC读取一个字节
 * 输入参数： 1 - 发送ACK
							0 - 发送nACK
 * 输出参数： 无
 *            无
 * 返 回 值： 读取到的一字节数据
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/	
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();
    for(i=0;i<8;i++ )
	{
        MPU6050_I2C_SCL_0(); 
        MPU_IIC_Delay();
		MPU6050_I2C_SCL_1();
        receive<<=1;
        if(MPU6050_I2C_SDA_READ())receive++;   
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();
    else
        MPU_IIC_Ack();  
    return receive;
}



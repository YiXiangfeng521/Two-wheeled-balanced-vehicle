/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-28
  * @brief   MPU6050的驱动程序
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
	
#include "mpu6050.h"

static uint16_t Gyro_range;										//保存陀螺仪传感器满量程范围
static uint8_t Accel_range;										//保存加速度计满量程范围
static uint16_t FreOfSample;									//保存采样率
volatile static uint8_t Data_updated_Flag = 0;//数据就绪标志位

/**********************************************************************
 * 函数名称： MPU_Write_Len
 * 功能描述： IIC连续写(写器件地址、寄存器地址、数据)
 * 输入参数： addr:器件地址       reg:寄存器地址
              len:写入数据的长度  buf:数据缓存区
 * 输出参数： 无
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i;
	
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);      //发送器件地址+写命令(0为写,1为读)	
	if(MPU_IIC_Wait_Ack())							 //等待应答
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);						 //写寄存器地址
    MPU_IIC_Wait_Ack();		             //等待应答
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	       //发送数据
		if(MPU_IIC_Wait_Ack())		         //等待ACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
    MPU_IIC_Stop();
	return 0;
}

/**********************************************************************
 * 函数名称： MPU_Read_Len
 * 功能描述： IIC连续读(写入器件地址后,读寄存器地址、数据)
 * 输入参数： addr:器件地址         reg:要读的寄存器地址
              len:要读取的数据长度  buf:读取到的数据存储区
 * 输出参数： 无
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
		MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|0);		//发送器件地址+写命令
		if(MPU_IIC_Wait_Ack())						//等待应答
		{
			MPU_IIC_Stop();		 
			return 1;
		}
    MPU_IIC_Send_Byte(reg);						//写寄存器地址
    MPU_IIC_Wait_Ack();								//等待应答
    MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|1);		//发送器件地址+读命令	
    MPU_IIC_Wait_Ack();								//等待应答 
		while(len)
		{
			if(len==1) *buf=MPU_IIC_Read_Byte(0);   //读数据,发送nACK 
			else 			 *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK  
			len--;
			buf++;
		}
    MPU_IIC_Stop();	//产生一个停止条件 
		return 0;	
}

/**********************************************************************
 * 函数名称： MPU_Write_Byte
 * 功能描述： IIC写一个字节
 * 输入参数： data:写入的数据    reg:要写的寄存器地址
 * 输出参数： 无
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Write_Byte(u8 reg,u8 data)
{
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令
	if(MPU_IIC_Wait_Ack())						 //等待应答
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Send_Byte(reg);						//写寄存器地址
  MPU_IIC_Wait_Ack();								//等待应答
	MPU_IIC_Send_Byte(data);					//发送数据
	if(MPU_IIC_Wait_Ack())	  				//等待ACK
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Stop();
	return 0;
}

/**********************************************************************
 * 函数名称： MPU_Read_Byte
 * 功能描述： IIC读一个字节
 * 输入参数： reg: 要读的寄存器地址
 * 输出参数： 无
 * 返 回 值： 读取到的数据
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);		//发送器件地址+写命令	
	MPU_IIC_Wait_Ack();										//等待应答 
  MPU_IIC_Send_Byte(reg);								//写寄存器地址
  MPU_IIC_Wait_Ack();										//等待应答
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);		//发送器件地址+读命令	
  MPU_IIC_Wait_Ack();										//等待应答 
	res=MPU_IIC_Read_Byte(0);							//读取数据,发送nACK 
  MPU_IIC_Stop();												//产生一个停止条件 
	return res;		
}

/**********************************************************************
 * 函数名称： MPU_Init
 * 功能描述： 初始化MPU6050
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 初始化成功  
							1 - 初始化失败
 * 修改日期        版本号     修改人      修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming    创建
 * 2025/09/29        V1.0     shiyaoming    设置中断
 ***********************************************************************/
u8 MPU_Init(void)
{ 
	u8 res;
  
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  mdelay(100);                            //等待MPU6050复位完成
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050，关闭睡眠模式 
	MPU_Set_Gyro_Fsr(3);										//将陀螺仪传感器的量程设置为±2000dps
	MPU_Set_Accel_Fsr(0);										//将加速度传感器的量程设置为±2g
	
	MPU_Set_Rate(200);											//设置采样率200Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X01);    //使能数据就绪中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)												//器件ID正确,即res = MPU_ADDR = 0x68
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL 陀螺仪X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作
		MPU_Set_Rate(100);										//设置采样率为100Hz
 	}
	else 
		return 1;    													//地址设置错误,返回1
																
	return 0;			 													//地址设置正确,返回0
}

/**********************************************************************
 * 函数名称： MPU_Set_Gyro_Fsr
 * 功能描述： 设置MPU6050中陀螺仪传感器满量程范围
 * 输入参数： 0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 设置成功  1 - 设置失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	switch(fsr)
	{
		case 0: Gyro_range = 250;break;
		case 1: Gyro_range = 500;break;
		case 2: Gyro_range = 1000;break;
		case 3: Gyro_range = 2000;break;
		default: Gyro_range = 0;break;
	}
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3); 
	
}

/**********************************************************************
 * 函数名称： MPU_Set_Accel_Fsr
 * 功能描述： 设置MPU6050中加速度传感器满量程范围
 * 输入参数： 0,±2g;1,±4g;2,±8g;3,±16g
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 设置成功  1 - 设置失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	switch(fsr)
	{
		case 0: Accel_range = 2;break;
		case 1: Accel_range = 4;break;
		case 2: Accel_range = 8;break;
		case 3: Accel_range = 16;break;
		default:Accel_range = 0;break;
	}
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3); //设置加速度传感器满量程范围  
}

/**********************************************************************
 * 函数名称： MPU_Set_LPF
 * 功能描述： 设置MPU6050的数字低通滤波器
 * 输入参数： lpf:数字低通滤波频率(Hz)
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 设置成功  1 - 设置失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);
}

/**********************************************************************
 * 函数名称： MPU_Set_Rate
 * 功能描述： 设置MPU6050的采样率(Fs=1KHz)
 * 输入参数： rate:4~1000(Hz)
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 设置成功  1 - 设置失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	
	FreOfSample = rate;
	
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	
	data=1000/rate-1;
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置采样频率分频器
 	return MPU_Set_LPF(rate/2);								//自动设置LPF为采样率的一半
}

/**********************************************************************
 * 函数名称： MPU_Get_Temperature
 * 功能描述： 得到温度传感器值
 * 输入参数： rate:4~1000(Hz)
 * 输出参数： 无
 *            无
 * 返 回 值： 温度值(扩大了100倍)
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
short MPU_Get_Temperature(void)
{
   u8 buf[2]; 
   short raw;
	 float temp;
	
	 MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
   raw=((u16)buf[0]<<8)|buf[1];
	
   temp=36.53+((double)raw)/340;
   return temp*100;
}

/**********************************************************************
 * 函数名称： MPU_Get_Gyroscope
 * 功能描述： 得到陀螺仪值(原始值)
 * 输入参数： 变量地址
 * 输出参数： gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  u8 buf[6],res;
	
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];
		*gy=((u16)buf[2]<<8)|buf[3];
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
  return res;
}

/**********************************************************************
 * 函数名称： MPU_Get_Accelerometer
 * 功能描述： 得到加速度值(原始值)
 * 输入参数： 变量地址
 * 输出参数： ax,ay,az:加速度传感器x,y,z轴的原始读数(带符号)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}
/**********************************************************************
 * 函数名称： MPU_Cal_Gyroscope
 * 功能描述： 利用陀螺仪的原始值计算出角速度
 * 输入参数： 变量地址
 * 输出参数： gx,gy,gz:x,y,z轴的角速度(单位：dps)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
int8_t MPU_Cal_Gyroscope(float *gx,float *gy,float *gz)
{
	short gx_raw,gy_raw,gz_raw;
	u8 ret;
	
	ret = MPU_Get_Gyroscope(&gx_raw,&gy_raw,&gz_raw);
	
	*gx = gx_raw/32768.0f*Gyro_range;
	*gy = gy_raw/32768.0f*Gyro_range;
	*gz = gz_raw/32768.0f*Gyro_range;
	
	return ret;
}
/**********************************************************************
 * 函数名称： MPU_Cal_Accelerometer
 * 功能描述： 利用加速度计的原始值计算出加速度
 * 输入参数： 变量地址
 * 输出参数： ax,ay,az:x,y,z轴的加速度(单位：g（重力加速度）)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         创建
 ***********************************************************************/
int8_t MPU_Cal_Accelerometer(float *ax,float *ay,float *az)
{
	short ax_raw,ay_raw,az_raw;
	u8 ret;
	
	ret = MPU_Get_Accelerometer(&ax_raw,&ay_raw,&az_raw);
	
	*ax = ax_raw/32768.0f*Accel_range;
	*ay = ay_raw/32768.0f*Accel_range;
	*az = az_raw/32768.0f*Accel_range;
	
	return ret;
}
/**********************************************************************
 * 函数名称： MPU_Cal_EulerAngles_by_Gyr
 * 功能描述： 利用角速度解算出欧拉角
 * 输入参数： 变量地址
 * 输出参数： Yaw:偏航角 Pitch：俯仰角 Roll：横滚角  (单位：度)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
int8_t MPU_Cal_EulerAngles_by_Gyr(float *Yaw,float *Pitch,float *Roll)
{
	float gx,gy,gz;
	u8 ret;

		ret = MPU_Cal_Gyroscope(&gx,&gy,&gz);
		if(ret == 0)
		{
			*Yaw = (*Yaw)+gz*(1/(float)FreOfSample);
			*Pitch = (*Pitch)+gy*(1/(float)FreOfSample);
			*Roll = (*Roll)+gx*(1/(float)FreOfSample);

		}
		return ret;
	
}
/**********************************************************************
 * 函数名称： MPU_Cal_EulerAngles_by_Acc
 * 功能描述： 利用加速度解算出欧拉角
 * 输入参数： 变量地址
 * 输出参数： Yaw:偏航角 Pitch：俯仰角 Roll：横滚角  (单位：度)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人      修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming     创建
 * 2025/09/30        V1.0     shiyaoming     快速三角函数运算
 ***********************************************************************/
int8_t MPU_Cal_EulerAngles_by_Acc(float *Yaw,float *Pitch,float *Roll)
{
		float ax,ay,az;
		u8 ret;

		ret = MPU_Cal_Accelerometer(&ax,&ay,&az);
	
		if(ret == 0)
		{
			*Yaw = 0;
			
#ifdef USE_FASTMATH
			*Pitch = atan2_lookup(-ax,az)/M_PI*180;
			*Roll = atan2_lookup(ay,az)/M_PI*180;
#else
			*Pitch = atan2(-ax,az)/M_PI*180;
			*Roll = atan2(ay,az)/M_PI*180;
#endif
		}
		return ret;
	
}
/**********************************************************************
 * 函数名称： MPU_Cal_EulerAngles
 * 功能描述： 利用一阶互补滤波将加速度解算出的欧拉角和陀螺仪解算出的欧拉角融合
 * 输入参数： 变量地址
 * 输出参数： Yaw:偏航角 Pitch：俯仰角 Roll：横滚角  (单位：度)
 * 返 回 值： 0 - 成功  1 - 失败
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
int8_t MPU_Cal_EulerAngles(float *Yaw,float *Pitch,float *Roll)
{
	static float Yaw_g=0,Pitch_g=0,Roll_g=0;
	float Yaw_a=0,Pitch_a=0,Roll_a=0;
	u8 ret;

	 ret = MPU_Cal_EulerAngles_by_Gyr(&Yaw_g,&Pitch_g,&Roll_g);
   ret = MPU_Cal_EulerAngles_by_Acc(&Yaw_a,&Pitch_a,&Roll_a);
		
		if(ret == 0)
		{
			*Yaw = Yaw_g;
			*Pitch = ALPHA*Pitch_g+(1-ALPHA)*Pitch_a;
			*Roll = ALPHA*Roll_g+(1-ALPHA)*Roll_a;
			
      Yaw_g = *Yaw;Pitch_g = *Pitch;Roll_g = *Roll;
			
		}
		return ret;
	
}
/**********************************************************************
 * 函数名称： Set_Flags
 * 功能描述： 设置数据就绪标志位
 * 输入参数： sta：0 or 1
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
void Set_Flags(uint8_t sta)
{
	Data_updated_Flag = sta;
}	
/**********************************************************************
 * 函数名称： Get_Flags
 * 功能描述： 获取数据就绪标志位
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 数据未就绪； 1 - 数据已就绪
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
uint8_t Get_Flags(void)
{
	return Data_updated_Flag;
}				
	

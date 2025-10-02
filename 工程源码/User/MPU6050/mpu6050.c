/**
  ******************************************************************************
  * @file    mpu6050.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-28
  * @brief   MPU6050����������
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
	
#include "mpu6050.h"

static uint16_t Gyro_range;										//���������Ǵ����������̷�Χ
static uint8_t Accel_range;										//������ٶȼ������̷�Χ
static uint16_t FreOfSample;									//���������
volatile static uint8_t Data_updated_Flag = 0;//���ݾ�����־λ

/**********************************************************************
 * �������ƣ� MPU_Write_Len
 * ���������� IIC����д(д������ַ���Ĵ�����ַ������)
 * ��������� addr:������ַ       reg:�Ĵ�����ַ
              len:д�����ݵĳ���  buf:���ݻ�����
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
 ***********************************************************************/
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i;
	
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);      //����������ַ+д����(0Ϊд,1Ϊ��)	
	if(MPU_IIC_Wait_Ack())							 //�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);						 //д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		             //�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	       //��������
		if(MPU_IIC_Wait_Ack())		         //�ȴ�ACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
    MPU_IIC_Stop();
	return 0;
}

/**********************************************************************
 * �������ƣ� MPU_Read_Len
 * ���������� IIC������(д��������ַ��,���Ĵ�����ַ������)
 * ��������� addr:������ַ         reg:Ҫ���ļĴ�����ַ
              len:Ҫ��ȡ�����ݳ���  buf:��ȡ�������ݴ洢��
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
 ***********************************************************************/
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
		MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|0);		//����������ַ+д����
		if(MPU_IIC_Wait_Ack())						//�ȴ�Ӧ��
		{
			MPU_IIC_Stop();		 
			return 1;
		}
    MPU_IIC_Send_Byte(reg);						//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();								//�ȴ�Ӧ��
    MPU_IIC_Start();
		MPU_IIC_Send_Byte((addr<<1)|1);		//����������ַ+������	
    MPU_IIC_Wait_Ack();								//�ȴ�Ӧ�� 
		while(len)
		{
			if(len==1) *buf=MPU_IIC_Read_Byte(0);   //������,����nACK 
			else 			 *buf=MPU_IIC_Read_Byte(1);		//������,����ACK  
			len--;
			buf++;
		}
    MPU_IIC_Stop();	//����һ��ֹͣ���� 
		return 0;	
}

/**********************************************************************
 * �������ƣ� MPU_Write_Byte
 * ���������� IICдһ���ֽ�
 * ��������� data:д�������    reg:Ҫд�ļĴ�����ַ
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
 ***********************************************************************/
u8 MPU_Write_Byte(u8 reg,u8 data)
{
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����
	if(MPU_IIC_Wait_Ack())						 //�ȴ�Ӧ��
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Send_Byte(reg);						//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();								//�ȴ�Ӧ��
	MPU_IIC_Send_Byte(data);					//��������
	if(MPU_IIC_Wait_Ack())	  				//�ȴ�ACK
	{
		MPU_IIC_Stop();
		return 1;
	}
  MPU_IIC_Stop();
	return 0;
}

/**********************************************************************
 * �������ƣ� MPU_Read_Byte
 * ���������� IIC��һ���ֽ�
 * ��������� reg: Ҫ���ļĴ�����ַ
 * ��������� ��
 * �� �� ֵ�� ��ȡ��������
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
 ***********************************************************************/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
	
  MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);		//����������ַ+д����	
	MPU_IIC_Wait_Ack();										//�ȴ�Ӧ�� 
  MPU_IIC_Send_Byte(reg);								//д�Ĵ�����ַ
  MPU_IIC_Wait_Ack();										//�ȴ�Ӧ��
  MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);		//����������ַ+������	
  MPU_IIC_Wait_Ack();										//�ȴ�Ӧ�� 
	res=MPU_IIC_Read_Byte(0);							//��ȡ����,����nACK 
  MPU_IIC_Stop();												//����һ��ֹͣ���� 
	return res;		
}

/**********************************************************************
 * �������ƣ� MPU_Init
 * ���������� ��ʼ��MPU6050
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� 0 - ��ʼ���ɹ�  
							1 - ��ʼ��ʧ��
 * �޸�����        �汾��     �޸���      �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming    ����
 * 2025/09/29        V1.0     shiyaoming    �����ж�
 ***********************************************************************/
u8 MPU_Init(void)
{ 
	u8 res;
  
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  mdelay(100);                            //�ȴ�MPU6050��λ���
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050���ر�˯��ģʽ 
	MPU_Set_Gyro_Fsr(3);										//�������Ǵ���������������Ϊ��2000dps
	MPU_Set_Accel_Fsr(0);										//�����ٶȴ���������������Ϊ��2g
	
	MPU_Set_Rate(200);											//���ò�����200Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X01);    //ʹ�����ݾ����ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)												//����ID��ȷ,��res = MPU_ADDR = 0x68
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);//����CLKSEL,PLL ������X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);//���ٶ��������Ƕ�����
		MPU_Set_Rate(100);										//���ò�����Ϊ100Hz
 	}
	else 
		return 1;    													//��ַ���ô���,����1
																
	return 0;			 													//��ַ������ȷ,����0
}

/**********************************************************************
 * �������ƣ� MPU_Set_Gyro_Fsr
 * ���������� ����MPU6050�������Ǵ����������̷�Χ
 * ��������� 0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
 * ��������� ��
 *            ��
 * �� �� ֵ�� 0 - ���óɹ�  1 - ����ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Set_Accel_Fsr
 * ���������� ����MPU6050�м��ٶȴ����������̷�Χ
 * ��������� 0,��2g;1,��4g;2,��8g;3,��16g
 * ��������� ��
 *            ��
 * �� �� ֵ�� 0 - ���óɹ�  1 - ����ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3); //���ü��ٶȴ����������̷�Χ  
}

/**********************************************************************
 * �������ƣ� MPU_Set_LPF
 * ���������� ����MPU6050�����ֵ�ͨ�˲���
 * ��������� lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
 * ��������� ��
 *            ��
 * �� �� ֵ�� 0 - ���óɹ�  1 - ����ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Set_Rate
 * ���������� ����MPU6050�Ĳ�����(Fs=1KHz)
 * ��������� rate:4~1000(Hz)
 * ��������� ��
 *            ��
 * �� �� ֵ�� 0 - ���óɹ�  1 - ����ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
 ***********************************************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	
	FreOfSample = rate;
	
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	
	data=1000/rate-1;
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//���ò���Ƶ�ʷ�Ƶ��
 	return MPU_Set_LPF(rate/2);								//�Զ�����LPFΪ�����ʵ�һ��
}

/**********************************************************************
 * �������ƣ� MPU_Get_Temperature
 * ���������� �õ��¶ȴ�����ֵ
 * ��������� rate:4~1000(Hz)
 * ��������� ��
 *            ��
 * �� �� ֵ�� �¶�ֵ(������100��)
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Get_Gyroscope
 * ���������� �õ�������ֵ(ԭʼֵ)
 * ��������� ������ַ
 * ��������� gx,gy,gz:������x,y,z���ԭʼ����(������)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Get_Accelerometer
 * ���������� �õ����ٶ�ֵ(ԭʼֵ)
 * ��������� ������ַ
 * ��������� ax,ay,az:���ٶȴ�����x,y,z���ԭʼ����(������)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Cal_Gyroscope
 * ���������� ���������ǵ�ԭʼֵ��������ٶ�
 * ��������� ������ַ
 * ��������� gx,gy,gz:x,y,z��Ľ��ٶ�(��λ��dps)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Cal_Accelerometer
 * ���������� ���ü��ٶȼƵ�ԭʼֵ��������ٶ�
 * ��������� ������ַ
 * ��������� ax,ay,az:x,y,z��ļ��ٶ�(��λ��g���������ٶȣ�)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Cal_EulerAngles_by_Gyr
 * ���������� ���ý��ٶȽ����ŷ����
 * ��������� ������ַ
 * ��������� Yaw:ƫ���� Pitch�������� Roll�������  (��λ����)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
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
 * �������ƣ� MPU_Cal_EulerAngles_by_Acc
 * ���������� ���ü��ٶȽ����ŷ����
 * ��������� ������ַ
 * ��������� Yaw:ƫ���� Pitch�������� Roll�������  (��λ����)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���      �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming     ����
 * 2025/09/30        V1.0     shiyaoming     �������Ǻ�������
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
 * �������ƣ� MPU_Cal_EulerAngles
 * ���������� ����һ�׻����˲������ٶȽ������ŷ���Ǻ������ǽ������ŷ�����ں�
 * ��������� ������ַ
 * ��������� Yaw:ƫ���� Pitch�������� Roll�������  (��λ����)
 * �� �� ֵ�� 0 - �ɹ�  1 - ʧ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
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
 * �������ƣ� Set_Flags
 * ���������� �������ݾ�����־λ
 * ��������� sta��0 or 1
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
 ***********************************************************************/
void Set_Flags(uint8_t sta)
{
	Data_updated_Flag = sta;
}	
/**********************************************************************
 * �������ƣ� Get_Flags
 * ���������� ��ȡ���ݾ�����־λ
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0 - ����δ������ 1 - �����Ѿ���
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
 ***********************************************************************/
uint8_t Get_Flags(void)
{
	return Data_updated_Flag;
}				
	

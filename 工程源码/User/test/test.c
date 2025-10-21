/**
  ******************************************************************************
  * @file    test.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   �ṩ��ģ��Ĳ��Գ���
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "test.h"


/**********************************************************************
 * �������ƣ� VoltageOfBattery_Test
 * ���������� ������ص�ѹ�Ĳ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
void VoltageOfBattery_Test(void)
{
	float value;
	
	ADC_Config();
	
	while(1)
	{   
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET)
		{
			DMA_ClearFlag(DMA1_FLAG_TC1);
			
			value = Get_VoltageOfBattery();
			printf("%.2f\n",value);             //���������ݺ��һ��'\n',������λ���޷�����������
			
		}
		  
	}
	
}
/**********************************************************************
 * �������ƣ� Motor_Test
 * ���������� ��������Ĳ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/10        V1.0     shiyaoming         ����
 ***********************************************************************/
void Motor_Test(void)
{
	
	Motor_Init();
	
	mdelay(2000);                       //Ӳ����ʼ��������ʱ2�룬�Է�ֹ���ͻȻת������Σ��
	/*������A�����ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(A,MOTOR_FWD);
	Set_Motor_Speed(A,500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);
	mdelay(1000);
	Set_Motor_Speed(A,3000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);
	mdelay(1000);
	Set_Motor_Speed(A,500);
	mdelay(1000);
  Set_Motor_Speed(A,0);
	/*A���ֹͣ��������B�����ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(B,MOTOR_FWD);
	Set_Motor_Speed(B,500);
	mdelay(1000);
	Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(B,3000);
	mdelay(1000);
	Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(B,500);
	mdelay(1000);
  Set_Motor_Speed(B,0);
	/*B���ֹͣ����������̨���ͬʱ��ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(A,MOTOR_REV);Set_Motor_Direction(B,MOTOR_REV);
	Set_Motor_Speed(A,500);Set_Motor_Speed(B,500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(A,3000);Set_Motor_Speed(B,3000);
	mdelay(3000);
	Set_Motor_Speed(A,2500);Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(A,500);Set_Motor_Speed(B,500);
	mdelay(1000);
  Set_Motor_Speed(A,0);Set_Motor_Speed(B,0);
	
	
	
	while(1)
	{
		  
		  
	}
	
}

/**********************************************************************
 * �������ƣ� Encoder_Test
 * ���������� ������������ٵĲ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         ����
 ***********************************************************************/
void Encoder_Test(void)
{
	uint16_t v = 0;
	int speed_A = 0,speed_B = 0;
	uint32_t last_tick1 = 0,last_tick2 = 0;
	
	Encoder_Init();
	
	Set_Motor_Direction(A,MOTOR_FWD);Set_Motor_Direction(B,MOTOR_FWD);
	while(1)
	{
		/*ÿ��2������һ�µ��ת��*/
		if(GetTick()-last_tick1>=200)
		{
			last_tick1 = GetTick();
			
			v += 500;
			if(v>=3500)//�޷�
			{
				v = 0;Set_Motor_Direction(A,MOTOR_REV);Set_Motor_Direction(B,MOTOR_REV);
			}
			Set_Motor_Speed(A,v);Set_Motor_Speed(B,v);
		}
		/*ÿ10ms��ȡһ�μ�������ֵ���൱�ڵ��ת�٣�*/
		if(GetTick()-last_tick2>=1)
		{
			last_tick2 = GetTick();
			speed_A = Read_Speed(A);speed_B = Read_Speed(B);
			//printf("speed_A = %d\n",speed_A);printf("speed_B = %d\n",speed_B);
			printf("%d,%d\n",speed_A,speed_B);
		}
		/*35�����ֹͣת��*/
		while(GetTick()>=3500)
		{
			v = 0;
			Set_Motor_Speed(A,v);Set_Motor_Speed(B,v);
		}
	}
}
/**********************************************************************
 * �������ƣ� MPU6050_Test
 * ���������� MPU6050ģ��Ĳ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming      ����
 * 2025/09/29        V1.0     shiyaoming      ���Ӳ���
 ***********************************************************************/
void MPU6050_Test(void)
{
	
	MPU_Init();
	MPU6050_EXTI_Init();

#if 0 //�ò���Ϊ��ȡMPU6050�м��ٶȼơ������ǡ��¶ȼƵ�ԭʼ���ݵĲ���
	short gx_raw,gy_raw,gz_raw;
	short ax_raw,ay_raw,az_raw;
	float gx,gy,gz;
	float ax,ay,az;
	short temperature;
	
	while(1)
	{
		MPU_Get_Accelerometer(&ax_raw,&ay_raw,&az_raw);
		MPU_Get_Gyroscope(&gx_raw,&gy_raw,&gz_raw);
		MPU_Cal_Accelerometer(&ax,&ay,&az);
		MPU_Cal_Gyroscope(&gx,&gy,&gz);
		temperature = MPU_Get_Temperature();
		
//		printf("Accel: %f(%d);%f(%d);%f(%d)\n",ax,ax_raw,ay,ay_raw,az,az_raw);
//		printf("Gyros: %f(%d);%f(%d);%f(%d)\n",gx,gx_raw,gy,gy_raw,gz,gz_raw);
		printf("%f,%f,%f,%f,%f,%f\n",ax,ay,az,gx,gy,gz);
		printf("Temperature: %f\n",temperature/100.0f);
		
		mdelay(10);
	}
#elif 1 //�ò���Ϊ����MPU6050�����ݽ����ŷ���ǵĲ���
  float Yaw=0,Pitch=0,Roll=0;

	while(1)
	{
		
		if(Get_Flags() ==1)
		{
			Set_Flags(0);
			
			MPU_Cal_EulerAngles(&Yaw,&Pitch,&Roll); 
		
		  printf("%f,%f,%f\n",Yaw,Pitch,Roll);
			
		}
	
	}

#endif
}
/**********************************************************************
 * �������ƣ� PID_Compute_Test
 * ���������� ����PID������������
 * ��������� pid - pid����ĵ�ַ
 *						FB  - ����ֵ
 * ��������� ��
 * �� �� ֵ�� ���������
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/10/15        V1.0     shiyaoming         ����
 ***********************************************************************/
static float PID_Compute_Test(PID_TypeDef *pid,float FB)
{
	float Out,Err;

	
	Err = pid->SP - FB;                           //��������ΪPID�����������룩
	/*ϵͳ������ĵ�һ��PID������Ե�΢�ֺͻ��ֿ��ƣ�����ֻ���б������ƣ�*/
	if(pid->cnt != 0)
	{
		pid->D_Err = (Err - pid->Err_Last);
		
		pid->S_Err += (0.5*Err+0.5*pid->Err_Last);
		//���л����޷�
		if(pid->S_Err>MOTOR_PWM_TIM_Period) pid->S_Err = MOTOR_PWM_TIM_Period;
		else if(pid->S_Err<-MOTOR_PWM_TIM_Period) pid->S_Err = -MOTOR_PWM_TIM_Period;
	}
	
	Out = pid->Kp*Err+pid->Ki*pid->S_Err+pid->Kd*pid->D_Err;//����PID���������
	
	pid->cnt = 1;                                           //ֻҪPID�����������й�����λcnt
	
	pid->Err_Last = Err;                                    
	
	return Out;
}
/**********************************************************************
 * �������ƣ� PID_CtrlMotor_Test
 * ���������� ����PI�������Ե����ת�٣����бջ�����
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/10/15        V1.0     shiyaoming         ����
 ***********************************************************************/
void PID_CtrlMotor_Test(void)
{
	/*����ʵ����һ��PID����������*/
	PID_TypeDef pid_test = {
 .SP = 83,
 .Kp = 245,
 .Kd = 0,
 .Ki = 1,
 .S_Err = 0,
 .D_Err = 0,
 .Err_Last = 0,
 .cnt = 0,
 .PID_Compute = PID_Compute_Test
};
	uint32_t last_tick = 0;
	int v;
  float out;

	while(1)
	{
		/*ϵͳ����30s��رյ��*/
		if(GetTick()>=3000)
		{
			
			Set_Motor_Speed(A,0);
		}
		/*ÿ���10ms����һ��PI����*/
		else if(GetTick()-last_tick>=1)
		{
			last_tick = GetTick();
			
			v = Read_Speed(A);//��ȡ���ʵ��ת�٣���Ϊ��������
			
			printf("%.3f,%d\n",pid_test.SP,v);
			
			out = pid_test.PID_Compute(&pid_test,v);//����PID����
			/*�������Ĺ鲢*/
			if(out<0)
			{
				if(out <= -MOTOR_PWM_TIM_Period) out = MOTOR_PWM_TIM_Period;
				else out = -out;
				Set_Motor_Direction(A,MOTOR_REV);
			}
			else
			{
				if(out >= MOTOR_PWM_TIM_Period) out = MOTOR_PWM_TIM_Period;
				else out = out;
				Set_Motor_Direction(A,MOTOR_FWD);
			}
	
			Set_Motor_Speed(A,out);//���PID��������������ص������
		}
	
		
	}
}

/**
  ******************************************************************************
  * @file    test.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   提供各模块的测试程序
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "test.h"


/**********************************************************************
 * 函数名称： VoltageOfBattery_Test
 * 功能描述： 测量电池电压的测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
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
			printf("%.2f\n",value);             //必须在数据后跟一个'\n',否则上位机无法解析到数据
			
		}
		  
	}
	
}
/**********************************************************************
 * 函数名称： Motor_Test
 * 功能描述： 电机驱动的测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/10        V1.0     shiyaoming         创建
 ***********************************************************************/
void Motor_Test(void)
{
	
	Motor_Init();
	
	mdelay(2000);                       //硬件初始化后先延时2秒，以防止电机突然转动产生危险
	/*先驱动A电机正转（转速逐渐加快再逐渐减弱）*/
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
	/*A电机停止后再驱动B电机正转（转速逐渐加快再逐渐减弱）*/
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
	/*B电机停止后再驱动两台电机同时反转（转速逐渐加快再逐渐减弱）*/
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
 * 函数名称： Encoder_Test
 * 功能描述： 电机编码器测速的测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         创建
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
		/*每隔2秒提升一下电机转速*/
		if(GetTick()-last_tick1>=200)
		{
			last_tick1 = GetTick();
			
			v += 500;
			if(v>=3500)//限幅
			{
				v = 0;Set_Motor_Direction(A,MOTOR_REV);Set_Motor_Direction(B,MOTOR_REV);
			}
			Set_Motor_Speed(A,v);Set_Motor_Speed(B,v);
		}
		/*每10ms读取一次计数器数值（相当于电机转速）*/
		if(GetTick()-last_tick2>=1)
		{
			last_tick2 = GetTick();
			speed_A = Read_Speed(A);speed_B = Read_Speed(B);
			//printf("speed_A = %d\n",speed_A);printf("speed_B = %d\n",speed_B);
			printf("%d,%d\n",speed_A,speed_B);
		}
		/*35秒后电机停止转动*/
		while(GetTick()>=3500)
		{
			v = 0;
			Set_Motor_Speed(A,v);Set_Motor_Speed(B,v);
		}
	}
}
/**********************************************************************
 * 函数名称： MPU6050_Test
 * 功能描述： MPU6050模块的测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/28        V1.0     shiyaoming      创建
 * 2025/09/29        V1.0     shiyaoming      增加测试
 ***********************************************************************/
void MPU6050_Test(void)
{
	
	MPU_Init();
	MPU6050_EXTI_Init();

#if 0 //该部分为获取MPU6050中加速度计、陀螺仪、温度计的原始数据的测试
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
#elif 1 //该部分为利用MPU6050的数据解算出欧拉角的测试
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
 * 函数名称： PID_Compute_Test
 * 功能描述： 进行PID控制器的运算
 * 输入参数： pid - pid对象的地址
 *						FB  - 反馈值
 * 输出参数： 无
 * 返 回 值： 控制器输出
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/10/15        V1.0     shiyaoming         创建
 ***********************************************************************/
static float PID_Compute_Test(PID_TypeDef *pid,float FB)
{
	float Out,Err;

	
	Err = pid->SP - FB;                           //计算误差（作为PID控制器的输入）
	/*系统启动后的第一次PID运算忽略掉微分和积分控制（即，只进行比例控制）*/
	if(pid->cnt != 0)
	{
		pid->D_Err = (Err - pid->Err_Last);
		
		pid->S_Err += (0.5*Err+0.5*pid->Err_Last);
		//进行积分限幅
		if(pid->S_Err>MOTOR_PWM_TIM_Period) pid->S_Err = MOTOR_PWM_TIM_Period;
		else if(pid->S_Err<-MOTOR_PWM_TIM_Period) pid->S_Err = -MOTOR_PWM_TIM_Period;
	}
	
	Out = pid->Kp*Err+pid->Ki*pid->S_Err+pid->Kd*pid->D_Err;//计算PID控制器输出
	
	pid->cnt = 1;                                           //只要PID控制器被运行过就置位cnt
	
	pid->Err_Last = Err;                                    
	
	return Out;
}
/**********************************************************************
 * 函数名称： PID_CtrlMotor_Test
 * 功能描述： 利用PI控制器对电机（转速）进行闭环控制
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/10/15        V1.0     shiyaoming         创建
 ***********************************************************************/
void PID_CtrlMotor_Test(void)
{
	/*首先实例化一个PID控制器对象*/
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
		/*系统启动30s后关闭电机*/
		if(GetTick()>=3000)
		{
			
			Set_Motor_Speed(A,0);
		}
		/*每间隔10ms进行一次PI控制*/
		else if(GetTick()-last_tick>=1)
		{
			last_tick = GetTick();
			
			v = Read_Speed(A);//读取电机实际转速（作为反馈量）
			
			printf("%.3f,%d\n",pid_test.SP,v);
			
			out = pid_test.PID_Compute(&pid_test,v);//进行PID运算
			/*输出结果的归并*/
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
	
			Set_Motor_Speed(A,out);//最后将PID控制器的输出加载到电机上
		}
	
		
	}
}

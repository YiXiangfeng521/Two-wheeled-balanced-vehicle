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
		/*每50ms读取一次计数器数值（相当于电机转速）*/
		if(GetTick()-last_tick2>=5)
		{
			last_tick2 = GetTick();
			speed_A = Read_Speed(A);speed_B = Read_Speed(B);
			printf("speed_A = %d\n",speed_A);printf("speed_B = %d\n",speed_B);
		}
		/*35秒后电机停止转动*/
		while(GetTick()>=3500)
		{
			v = 0;
			Set_Motor_Speed(A,v);Set_Motor_Speed(B,v);
		}
	}
}


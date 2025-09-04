/**
  ******************************************************************************
  * @file    main.c
  * @author  ShiYaoming
  * @version V2.0
  * @date    2025-08-28
  * @brief   双轮平衡小车 Main program body
  ******************************************************************************
  * @attention
  *
  * 实验平台:STM32F103C8T6（大鱼电子控制板）
  * 
  *
  ******************************************************************************
  */
#include "Timbase.h"  
#include "adc.h"
#include "usart.h"
#include "test.h"
 /**
  * @brief  主函数
  * @param  无
  * @retval int
  */
    	
int main(void)
{
	  
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// 设置中断分组为Group_4
	  BASIC_TIM_Config();                             //配置TIM3产生时基，每10ms产生一个tick，且产生一个TRGO触发一次ADC转换
    USART1_Config();                                //配置 USART1 与电脑上位机通信打印测试信息
/*------------------------各模块的测试程序-------------------------------------*/
	
	  //VoltageOfBattery_Test();
	
/*------------------------------------------------------------------------------*/
	 
		while(1)
		{
			
		}
	
}
/*********************************************END OF FILE**********************/

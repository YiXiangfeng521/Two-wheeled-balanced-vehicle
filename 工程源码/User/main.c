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
#include "led.h"  
#include "mpu6050.h"
#include "test.h"
 /**
  * @brief  主函数
  * @param  无
  * @retval int
  */	
int main(void)
{
	/*------------------------各模块的初始化配置-------------------------------------*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// 设置中断分组为Group_4
	  BASIC_TIM_Config();                            
    USART1_Config();
	  ADC_Config();
    LED_GPIO_Config();
	  Motor_Init();
	  Encoder_Init();
	  MPU_IIC_Init();			
	  MPU_Init();
	  MPU6050_EXTI_Init();
/*------------------------各模块的测试程序-------------------------------------*/
	
	  //VoltageOfBattery_Test();
	  //Motor_Test();
	  //Encoder_Test();
	  //MPU6050_Test();
		//PID_CtrlMotor_Test();

		
		
	 
/*------------------------------------------------------------------------------*/
	 
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET) //当最新的电压值更新时，读取并控制LED亮度
			{
				DMA_ClearFlag(DMA1_FLAG_TC1);
				Display_VoltageOfBattery_byLED();
			}
			
		}
	
}
/*********************************************END OF FILE**********************/

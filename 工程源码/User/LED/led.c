/**
  ******************************************************************************
  * @file    led.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-09
  * @brief   LED的驱动程序
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "led.h"   

unsigned char pwm_duty = 0;         //输出到LED1的模拟PWM信号的占空比（0%~100%）

/**********************************************************************
 * 函数名称： LED_GPIO_Config
 * 功能描述： 初始化LED控制引脚
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
void LED_GPIO_Config(void)
{		
		
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK, ENABLE);
	  /*关闭LSE，使得PC14 PC15 作为GPIO使用*/
	  RCC_LSEConfig(RCC_LSE_OFF);
	
	
	  GPIO_StructInit(&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
   	/*PC14最大的输出速度只能为2MHz*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	

		/* 先关闭led1	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
			
}
/*********************************************END OF FILE**********************/

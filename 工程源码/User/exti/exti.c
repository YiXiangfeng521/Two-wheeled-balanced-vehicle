/**
  ******************************************************************************
  * @file    exti.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-29
  * @brief   该文件用于配置外部中断
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "exti.h"

/**********************************************************************
 * 函数名称： NVIC_Configuration
 * 功能描述： 初始化外部中断（MPU6050数据就绪）相关的NVIC寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure = {0};
  

  /* 配置中断源：MPU6050的INT引脚 */
  NVIC_InitStructure.NVIC_IRQChannel = MPU6050_INT_EXTI_IRQ;
  /* 因为该中断关系到小车姿态的获取以及控制器的执行，是本系统最重要的中断，所以配置抢占（主）优先级最高 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

/**********************************************************************
 * 函数名称： MPU6050_EXTI_Init
 * 功能描述： 初始化MPU6050数据就绪相关的外部中断
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         创建
 ***********************************************************************/
void MPU6050_EXTI_Init(void)
{
	//*****************************GPIO配置*****************************
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(MPU6050_INT_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=MPU6050_INT_GPIO_PIN;
	GPIO_Init(MPU6050_INT_GPIO_PORT ,&GPIO_InitStruct);	
	//*****************************EXTI配置*****************************
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);
	
	GPIO_EXTILineConfig(MPU6050_INT_EXTI_PORTSOURCE,MPU6050_INT_EXTI_PINSOURCE );
	
	EXTI_InitStruct.EXTI_Line=MPU6050_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling; //与MPU6050初始化配置相关
	EXTI_Init(&EXTI_InitStruct);
	//*****************************NVIC配置*****************************
	NVIC_Configuration();
	
}

/*********************************************END OF FILE********************************************/

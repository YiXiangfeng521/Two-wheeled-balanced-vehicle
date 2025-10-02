/**
  ******************************************************************************
  * @file    exti.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-29
  * @brief   ���ļ����������ⲿ�ж�
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "exti.h"

/**********************************************************************
 * �������ƣ� NVIC_Configuration
 * ���������� ��ʼ���ⲿ�жϣ�MPU6050���ݾ�������ص�NVIC�Ĵ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
 ***********************************************************************/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure = {0};
  

  /* �����ж�Դ��MPU6050��INT���� */
  NVIC_InitStructure.NVIC_IRQChannel = MPU6050_INT_EXTI_IRQ;
  /* ��Ϊ���жϹ�ϵ��С����̬�Ļ�ȡ�Լ���������ִ�У��Ǳ�ϵͳ����Ҫ���жϣ�����������ռ���������ȼ���� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

/**********************************************************************
 * �������ƣ� MPU6050_EXTI_Init
 * ���������� ��ʼ��MPU6050���ݾ�����ص��ⲿ�ж�
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/29        V1.0     shiyaoming         ����
 ***********************************************************************/
void MPU6050_EXTI_Init(void)
{
	//*****************************GPIO����*****************************
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(MPU6050_INT_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=MPU6050_INT_GPIO_PIN;
	GPIO_Init(MPU6050_INT_GPIO_PORT ,&GPIO_InitStruct);	
	//*****************************EXTI����*****************************
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO ,ENABLE);
	
	GPIO_EXTILineConfig(MPU6050_INT_EXTI_PORTSOURCE,MPU6050_INT_EXTI_PINSOURCE );
	
	EXTI_InitStruct.EXTI_Line=MPU6050_INT_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling; //��MPU6050��ʼ���������
	EXTI_Init(&EXTI_InitStruct);
	//*****************************NVIC����*****************************
	NVIC_Configuration();
	
}

/*********************************************END OF FILE********************************************/

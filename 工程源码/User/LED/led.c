/**
  ******************************************************************************
  * @file    led.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-09
  * @brief   LED����������
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "led.h"   

unsigned char pwm_duty = 0;         //�����LED1��ģ��PWM�źŵ�ռ�ձȣ�0%~100%��

/**********************************************************************
 * �������ƣ� LED_GPIO_Config
 * ���������� ��ʼ��LED��������
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
void LED_GPIO_Config(void)
{		
		
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd( LED1_GPIO_CLK, ENABLE);
	  /*�ر�LSE��ʹ��PC14 PC15 ��ΪGPIOʹ��*/
	  RCC_LSEConfig(RCC_LSE_OFF);
	
	
	  GPIO_StructInit(&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
   	/*PC14��������ٶ�ֻ��Ϊ2MHz*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	

		/* �ȹر�led1	*/
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
			
}
/*********************************************END OF FILE**********************/

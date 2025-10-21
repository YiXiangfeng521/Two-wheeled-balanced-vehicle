/**
  ******************************************************************************
  * @file    main.c
  * @author  ShiYaoming
  * @version V2.0
  * @date    2025-08-28
  * @brief   ˫��ƽ��С�� Main program body
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:STM32F103C8T6��������ӿ��ư壩
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
  * @brief  ������
  * @param  ��
  * @retval int
  */	
int main(void)
{
	/*------------------------��ģ��ĳ�ʼ������-------------------------------------*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// �����жϷ���ΪGroup_4
	  BASIC_TIM_Config();                            
    USART1_Config();
	  ADC_Config();
    LED_GPIO_Config();
	  Motor_Init();
	  Encoder_Init();
	  MPU_IIC_Init();			
	  MPU_Init();
	  MPU6050_EXTI_Init();
/*------------------------��ģ��Ĳ��Գ���-------------------------------------*/
	
	  //VoltageOfBattery_Test();
	  //Motor_Test();
	  //Encoder_Test();
	  //MPU6050_Test();
		//PID_CtrlMotor_Test();

		
		
	 
/*------------------------------------------------------------------------------*/
	 
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET) //�����µĵ�ѹֵ����ʱ����ȡ������LED����
			{
				DMA_ClearFlag(DMA1_FLAG_TC1);
				Display_VoltageOfBattery_byLED();
			}
			
		}
	
}
/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    DCmotor_brush_control.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-09
  * @brief   ֱ����ˢ�������������
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "DCmotor_brush_control.h"


static motor_dir_t direction  = MOTOR_FWD;     // ��¼�����ת����
static uint16_t    dutyfactor = 0;             // ��¼ռ�ձ�

/**********************************************************************
 * �������ƣ� MOTOR_IN_GPIO_Init
 * ���������� ��ʼ�����Ƶ�������GPIO��TB6612�ϵ�IN1��IN2��
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
static void MOTOR_IN_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(MOTOR_BIN1_CLK|MOTOR_BIN2_CLK|MOTOR_AIN1_CLK|MOTOR_AIN2_CLK, ENABLE); 
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  MOTOR_BIN1_PIN;
  GPIO_Init(MOTOR_BIN1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  MOTOR_BIN2_PIN;
  GPIO_Init(MOTOR_BIN2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  MOTOR_AIN1_PIN;
  GPIO_Init(MOTOR_AIN1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  MOTOR_AIN2_PIN;
  GPIO_Init(MOTOR_AIN2_PORT, &GPIO_InitStructure);
  
}

/**********************************************************************
 * �������ƣ� MOTOR_PWM_GPIO_Init
 * ���������� ��ʼ�����Ƶ���ٶȵ�GPIO��TB6612�ϵ�PWM���ţ�
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
static void MOTOR_PWM_GPIO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(MOTOR_PWMB_CLK|MOTOR_PWMA_CLK, ENABLE); 

  /* ���ö�ʱ��1ͨ��1�������ģʽ�������������ģʽ */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_PWMB_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PWMB_PORT, &GPIO_InitStructure);

  /* ���ö�ʱ��1ͨ��4�������ģʽ */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_PWMA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PWMA_PORT, &GPIO_InitStructure);

}

/**********************************************************************
 * �������ƣ� MOTOR_PWM_TIM_Init
 * ���������� ����TIM1ΪPWM���ģʽ����ʼ��TIM1����ؼĴ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
static void MOTOR_PWM_TIM_Init(void)
{	
	//********************��ʱ��ʱ������*******************************
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  MOTOR_PWM_TIM_APBxClock_FUN(MOTOR_PWM_TIM_CLK, ENABLE);
	//����ʱ����ʱ��Դ����Ϊ�ڲ�ʱ�ӣ���ʹ��STM32�ڲ�ʱ��CK_INT=72M��Ϊ��ʱ��ʱ��
  TIM_InternalClockConfig(MOTOR_PWM_TIM);
	
	
  TIM_TimeBaseStructure.TIM_Period = MOTOR_PWM_TIM_Period ;   
  /* ����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz,�������Ƶ�ʣ�72MHz/3600=20KHz */
  /* ͬһ����ʱ���Ĳ�ͬͨ��PWM��Ƶ��ֻ������ͬ�ģ���ͬͨ����������Ϊ��ͬ��ռ�ձ� */
  TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_PWM_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseStructure);
	
	//********************��ʱ���������*******************************
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  /* ռ�ձȳ�ʼֵ����Ϊ0 */
  TIM_OCInitStructure.TIM_Pulse = 0;
  /* ���ͨ����Ч��ƽ�������ã�����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* ��ʱ���Ƚ����ͨ��1Ԥװ�����ã�ʹ��Ԥװ�� */
  TIM_OC1PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
 /* ��ʼ����ʱ��ͨ��4���PWM */
  TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* ��ʱ���Ƚ����ͨ��4Ԥװ�����ã�ʹ��Ԥװ�� */
  TIM_OC4PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
  // �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
  TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
  //ʹ��ARR�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(MOTOR_PWM_TIM, ENABLE); 
  /* ʹ�ܶ�ʱ�� */
  TIM_Cmd(MOTOR_PWM_TIM, ENABLE);
  
}

/**********************************************************************
 * �������ƣ� NVIC_Configuration
 * ���������� ��ʼ��TIM1�ж���ص�NVIC�Ĵ���
              ֮���Կ�TIM1���ж���Ҫ��Ϊ����PC14�����ģ��PWM�ź�
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0}; 
    	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = MOTOR_PWM_TIM_IRQ ;	
		// ���������ȼ�Ϊ 15
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
	  // ���������ȼ��������ж����ȼ�����4��SubPriority�����Բ����䣩
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**********************************************************************
 * �������ƣ� Motor_Init
 * ���������� ���Ӳ����ʼ���Ľӿں���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
void Motor_Init(void)
{
  MOTOR_PWM_TIM_Init();
	MOTOR_PWM_GPIO_Init();
	NVIC_Configuration();
  MOTOR_IN_GPIO_Init();
}

/**********************************************************************
 * �������ƣ� Set_Motor_Speed
 * ���������� �������뵽ѡ�е����PWM�źŵ�ռ�ձ��Կ���ת��
 * ��������� CH - Ҫ����A�������B���
              v  - CCR��ֵ��0~3600��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
void Set_Motor_Speed(MOTORx CH,uint16_t v)
{
  dutyfactor = v;
	
	switch(CH)
	{
		case B : TIM_SetCompare1(MOTOR_PWM_TIM, dutyfactor);break;//���Bռ��TIM1��ͨ��1
		case A : TIM_SetCompare4(MOTOR_PWM_TIM, dutyfactor);break;//���Aռ��TIM1��ͨ��4

	}
  
}

/**********************************************************************
 * �������ƣ� Set_Motor_Direction
 * ���������� ����ѡ�е����ת������
 * ��������� CH  - Ҫ����A�������B���
              dir - ת������
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         ����
 ***********************************************************************/
void Set_Motor_Direction(MOTORx CH,motor_dir_t dir)
{
  direction = dir;
	/*�����߼��ο�TB6612���ƹ�����ֵ��*/
  switch(CH)
  {
	  case B :
		  if (direction == MOTOR_REV)
			{
				GPIO_ResetBits(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN );
				GPIO_SetBits(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN );
			}
			else
			{
				GPIO_SetBits(MOTOR_BIN2_PORT, MOTOR_BIN2_PIN );
				GPIO_ResetBits(MOTOR_BIN1_PORT, MOTOR_BIN1_PIN );
			} break;
	  case A : 
			if (direction == MOTOR_FWD)
			{
				GPIO_SetBits(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN );
				GPIO_ResetBits(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN );
			}
			else
			{
				GPIO_SetBits(MOTOR_AIN1_PORT, MOTOR_AIN1_PIN );
				GPIO_ResetBits(MOTOR_AIN2_PORT, MOTOR_AIN2_PIN );
			}  break;
	  
  }
 
}


/*********************************************END OF FILE********************************************/


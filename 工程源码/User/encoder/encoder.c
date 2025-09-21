/**
  ******************************************************************************
  * @file    encoder.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-18
  * @brief   �������������������
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
	
#include "encoder.h" 

/**********************************************************************
 * �������ƣ� Encoder_GPIO_Init
 * ���������� ��ʼ������������ӿڵ�GPIO����TIM��CH1��CH2��
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         ����
 ***********************************************************************/
static void Encoder_GPIO_Init(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
 
  RCC_APB2PeriphClockCmd(ENCODER_B_TIM_CH1_GPIO_CLK|ENCODER_A_TIM_CH1_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  ENCODER_B_TIM_CH1_PIN|ENCODER_B_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(ENCODER_B_TIM_CH1_PORT, &GPIO_InitStructure);	
	
  GPIO_InitStructure.GPIO_Pin =  ENCODER_A_TIM_CH1_PIN|ENCODER_A_TIM_CH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
  GPIO_Init(ENCODER_A_TIM_CH1_PORT, &GPIO_InitStructure);
}
/**********************************************************************
 * �������ƣ� Encoder_TIM_Init
 * ���������� ���õ�����������õ���TIM��ʹ֮�����ڱ�����ģʽ
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         ����
 ***********************************************************************/
static void Encoder_TIM_Init(void)
{
  
	 ENCODER_B_TIM_APBxClock_FUN(ENCODER_B_TIM_CLK,ENABLE);
   ENCODER_A_TIM_APBxClock_FUN(ENCODER_A_TIM_CLK,ENABLE);
/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period=ENCODER_B_TIM_PERIOD;	
	TIM_TimeBaseStructure.TIM_Prescaler= ENCODER_B_TIM_PSC;	
	// ���������������ɱ������йܣ������������
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	TIM_TimeBaseInit(ENCODER_B_TIM, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period=ENCODER_A_TIM_PERIOD;	
	TIM_TimeBaseStructure.TIM_Prescaler= ENCODER_A_TIM_PSC;	
	TIM_TimeBaseInit(ENCODER_A_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/
  TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	//��������������ź��Ƿ��࣬����ƥ����ת��ͼ�����������ΪRising��ʾ�����ࣩ
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//ʵ���ϻᱻ����ı������ӿ����ø���
	/*������������ѡ���������ģʽ�޹�*/
	
/*TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; */
	
	/*������������ѡ���������ģʽ�޹�*/
	
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(ENCODER_B_TIM, &TIM_ICInitStructure);
	TIM_ICInit(ENCODER_A_TIM, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER_B_TIM, &TIM_ICInitStructure);
	TIM_ICInit(ENCODER_A_TIM, &TIM_ICInitStructure);
	
	
  TIM_EncoderInterfaceConfig(ENCODER_B_TIM, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3(4��Ƶ�������źŲ�����)
  TIM_EncoderInterfaceConfig(ENCODER_A_TIM, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3(4��Ƶ�������źŷ���)

	 // ��ʼ����ֵ����
	TIM_SetCounter(ENCODER_B_TIM,0);
	// ʹ�ܼ�����
	TIM_Cmd(ENCODER_B_TIM, ENABLE);

	 // ��ʼ����ֵ����
	TIM_SetCounter(ENCODER_A_TIM,0);
	// ʹ�ܼ�����
	TIM_Cmd(ENCODER_A_TIM, ENABLE);
}
/**********************************************************************
 * �������ƣ� Encoder_Init
 * ���������� ���������Ӳ����ʼ���Ľӿں���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         ����
 ***********************************************************************/
void Encoder_Init(void)
{
	
	Encoder_TIM_Init();
	Encoder_GPIO_Init();	
  
}

/**********************************************************************
 * �������ƣ� Read_Speed
 * ���������� ��ȡѡ�е����ת�٣��ú���Ӧÿ��һ����ʱ����������һ�Σ�
 * ��������� CH - A�������B���
 * ��������� ��
 * �� �� ֵ�� ת�٣�������������ת����������ת��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         ����
 ***********************************************************************/
int Read_Speed(MOTORx CH)
{
	int value;
	
	switch(CH)
	{
		//1.�ɼ��������ļ���ֵ�����档2.����ʱ���ļ���ֵ���㡣
		case B:value=(short)TIM_GetCounter(ENCODER_B_TIM);TIM_SetCounter(ENCODER_B_TIM,0);break;
		case A:value=(short)TIM_GetCounter(ENCODER_A_TIM);TIM_SetCounter(ENCODER_A_TIM,0);break;
		default:value=0;
	}
	
	return value;
}
/*
֪ʶ�㣺ΪʲôҪ��TIM_GetCounter����ֵǿ��ת��Ϊshort��

��Ϊ��TIM_GetCounter�ķ���ֵ������uint16_t,Ҳ�����޷��ŵĶ����ͣ�
����ǿ��ת��Ϊshort�Ϳ��Դ��������ţ��ֱ��ʾ�������ת��
�ڶ�ʱ����ʼ��������Ѿ��Ѽ�������ǰֵ��0����������ת��
��ô�ʹ�0��ʼһֱ���ϼ�������������2^15-1����������ת��
��ô���������¼�����ֵ��Ϊ65535��ǿ������ת�����Ϊ-1����������-2^15;
*/
/*********************************************END OF FILE**********************/

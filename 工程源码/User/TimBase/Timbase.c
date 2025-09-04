/**
  ******************************************************************************
  * @file    timbase.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   ����TIM3����ʱ����ÿ10ms����һ��tick���Ҳ���һ��TRGO����һ��ADCת��
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "Timbase.h" 

__IO uint32_t uwTick; // 10ms��ʱ���� 

/**********************************************************************
 * �������ƣ� NVIC_Configuration
 * ���������� ��ʼ��TIM3�ж���ص�NVIC�Ĵ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0}; 
    	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
	  // ������ռ���ȼ��������ж����ȼ�����4��SubPriority�����Բ����䣩
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************
 * �������ƣ� TIM_Register_Init
 * ���������� ����TIM3�Ĺ���ģʽ����ʼ��TIM3�������ؼĴ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void TIM_Register_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	 //����ʱ����ʱ��Դ����Ϊ�ڲ�ʱ�ӣ���ʹ��STM32�ڲ�ʱ��CK_INT=72M��Ϊ��ʱ��ʱ��
  	TIM_InternalClockConfig(BASIC_TIM);
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�������¼�
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

    TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		
		// �ظ���������ֵ,ֻ�и߼���ʱ������
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  
		// ���������������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		//����TIM3_TRGOÿ10ms����һ������
		TIM_SelectOutputTrigger(BASIC_TIM,TIM_TRGOSource_Update);
}

/**********************************************************************
 * �������ƣ� BASIC_TIM_Config
 * ���������� ����TIM3�Ľӿں���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
void BASIC_TIM_Config(void)
{
	  TIM_Register_Init();
	  NVIC_Configuration();
		// ʹ��TIM3
    TIM_Cmd(BASIC_TIM, ENABLE);	
}

__weak void IncTick(void)
{
  uwTick += 1;
}

__weak uint32_t GetTick(void)
{
  return uwTick;
}
/**********************************************************************
 * �������ƣ� system_get_ns
 * ���������� ���ϵͳʱ��(��λns)
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� ϵͳʱ��(��λns)
 ***********************************************************************/
uint64_t system_get_us(void)
{
    
    uint32_t ms = GetTick();
    uint64_t cnt,us;
   

    cnt = TIM_GetCounter(BASIC_TIM);//cnt��us
  

    us = ms*1000;
    us += cnt;
    return us;
}
/**********************************************************************
 * �������ƣ� udelay
 * ���������� us�������ʱ����(����rt-thread�Ĵ���)
 * ��������� us - ��ʱ����us
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
void udelay(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint16_t reload = BASIC_TIM->ARR;

    ticks = us * (reload+1) / (1000);  /* ����reload��Ӧ1ms */
    told = TIM_GetCounter(BASIC_TIM);//cnt��us;
    while (1)
    {
        tnow = TIM_GetCounter(BASIC_TIM);//cnt��us
        if (tnow != told)
        {
            if (tnow > told)
            {
                tcnt += tnow - told;
            }
            else
            {
                tcnt += (reload+1) - told + tnow;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }

}

/**********************************************************************
 * �������ƣ� mdelay
 * ���������� ms�������ʱ����
 * ��������� ms - ��ʱ����ms
 * ��������� ��
 * �� �� ֵ�� ��
 ***********************************************************************/
void mdelay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
        udelay(1000);
}
/*********************************************END OF FILE**********************/

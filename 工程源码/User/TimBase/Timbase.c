/**
  ******************************************************************************
  * @file    timbase.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   配置TIM3产生时基，每10ms产生一个tick，且产生一个TRGO触发一次ADC转换
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "Timbase.h" 

__IO uint32_t uwTick; // 10ms计时变量 

/**********************************************************************
 * 函数名称： NVIC_Configuration
 * 功能描述： 初始化TIM3中断相关的NVIC寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0}; 
    	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
	  // 设置抢占优先级（由于中断优先级分组4无SubPriority，所以不用配）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**********************************************************************
 * 函数名称： TIM_Register_Init
 * 功能描述： 配置TIM3的工作模式，初始化TIM3外设的相关寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void TIM_Register_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	 //将定时器的时钟源设置为内部时钟，即使用STM32内部时钟CK_INT=72M作为定时器时钟
  	TIM_InternalClockConfig(BASIC_TIM);
	
	/*--------------------时基结构体初始化-------------------------*/
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新事件
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

    TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		
		// 重复计数器的值,只有高级定时器才有
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  
		// 开启计数器更新中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		//设置TIM3_TRGO每10ms产生一个脉冲
		TIM_SelectOutputTrigger(BASIC_TIM,TIM_TRGOSource_Update);
}

/**********************************************************************
 * 函数名称： BASIC_TIM_Config
 * 功能描述： 配置TIM3的接口函数
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
void BASIC_TIM_Config(void)
{
	  TIM_Register_Init();
	  NVIC_Configuration();
		// 使能TIM3
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
 * 函数名称： system_get_ns
 * 功能描述： 获得系统时间(单位ns)
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 系统时间(单位ns)
 ***********************************************************************/
uint64_t system_get_us(void)
{
    
    uint32_t ms = GetTick();
    uint64_t cnt,us;
   

    cnt = TIM_GetCounter(BASIC_TIM);//cnt个us
  

    us = ms*1000;
    us += cnt;
    return us;
}
/**********************************************************************
 * 函数名称： udelay
 * 功能描述： us级别的延时函数(复制rt-thread的代码)
 * 输入参数： us - 延时多少us
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void udelay(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint16_t reload = BASIC_TIM->ARR;

    ticks = us * (reload+1) / (1000);  /* 假设reload对应1ms */
    told = TIM_GetCounter(BASIC_TIM);//cnt个us;
    while (1)
    {
        tnow = TIM_GetCounter(BASIC_TIM);//cnt个us
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
 * 函数名称： mdelay
 * 功能描述： ms级别的延时函数
 * 输入参数： ms - 延时多少ms
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void mdelay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
        udelay(1000);
}
/*********************************************END OF FILE**********************/

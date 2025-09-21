/**
  ******************************************************************************
  * @file    encoder.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-18
  * @brief   电机编码器的驱动程序
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
	
#include "encoder.h" 

/**********************************************************************
 * 函数名称： Encoder_GPIO_Init
 * 功能描述： 初始化电机编码器接口的GPIO（即TIM的CH1和CH2）
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         创建
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
 * 函数名称： Encoder_TIM_Init
 * 功能描述： 配置电机编码器所用到的TIM，使之工作在编码器模式
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         创建
 ***********************************************************************/
static void Encoder_TIM_Init(void)
{
  
	 ENCODER_B_TIM_APBxClock_FUN(ENCODER_B_TIM_CLK,ENABLE);
   ENCODER_A_TIM_APBxClock_FUN(ENCODER_A_TIM_CLK,ENABLE);
/*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period=ENCODER_B_TIM_PERIOD;	
	TIM_TimeBaseStructure.TIM_Prescaler= ENCODER_B_TIM_PSC;	
	// 计数器计数方向由编码器托管，不受软件配置
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	TIM_TimeBaseInit(ENCODER_B_TIM, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period=ENCODER_A_TIM_PERIOD;	
	TIM_TimeBaseStructure.TIM_Prescaler= ENCODER_A_TIM_PSC;	
	TIM_TimeBaseInit(ENCODER_A_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/
  TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	//用来设置输入的信号是否反相，用来匹配电机转向和计数方向（配置为Rising表示不反相）
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//实际上会被下面的编码器接口配置覆盖
	/*下面两个配置选项与编码器模式无关*/
	
/*TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; */
	
	/*上面两个配置选项与编码器模式无关*/
	
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(ENCODER_B_TIM, &TIM_ICInitStructure);
	TIM_ICInit(ENCODER_A_TIM, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(ENCODER_B_TIM, &TIM_ICInitStructure);
	TIM_ICInit(ENCODER_A_TIM, &TIM_ICInitStructure);
	
	
  TIM_EncoderInterfaceConfig(ENCODER_B_TIM, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3(4倍频，输入信号不反相)
  TIM_EncoderInterfaceConfig(ENCODER_A_TIM, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);//使用编码器模式3(4倍频，输入信号反相)

	 // 初始计数值清零
	TIM_SetCounter(ENCODER_B_TIM,0);
	// 使能计数器
	TIM_Cmd(ENCODER_B_TIM, ENABLE);

	 // 初始计数值清零
	TIM_SetCounter(ENCODER_A_TIM,0);
	// 使能计数器
	TIM_Cmd(ENCODER_A_TIM, ENABLE);
}
/**********************************************************************
 * 函数名称： Encoder_Init
 * 功能描述： 电机编码器硬件初始化的接口函数
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         创建
 ***********************************************************************/
void Encoder_Init(void)
{
	
	Encoder_TIM_Init();
	Encoder_GPIO_Init();	
  
}

/**********************************************************************
 * 函数名称： Read_Speed
 * 功能描述： 读取选中电机的转速（该函数应每隔一定的时间间隔被调用一次）
 * 输入参数： CH - A电机或者B电机
 * 输出参数： 无
 * 返 回 值： 转速（正数代表电机正转，负数代表反转）
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/18        V1.0     shiyaoming         创建
 ***********************************************************************/
int Read_Speed(MOTORx CH)
{
	int value;
	
	switch(CH)
	{
		//1.采集编码器的计数值并保存。2.将定时器的计数值清零。
		case B:value=(short)TIM_GetCounter(ENCODER_B_TIM);TIM_SetCounter(ENCODER_B_TIM,0);break;
		case A:value=(short)TIM_GetCounter(ENCODER_A_TIM);TIM_SetCounter(ENCODER_A_TIM,0);break;
		default:value=0;
	}
	
	return value;
}
/*
知识点：为什么要把TIM_GetCounter返回值强制转化为short？

因为：TIM_GetCounter的返回值类型是uint16_t,也就是无符号的短整型，
把它强制转化为short就可以带上正负号，分别表示电机正反转。
在定时器初始化的最后已经把计数器当前值清0，假设电机正转，
那么就从0开始一直向上计数，最大计数到2^15-1；电机如果反转，
那么计数器向下计数，值变为65535，强制类型转化后变为-1，最大计数到-2^15;
*/
/*********************************************END OF FILE**********************/

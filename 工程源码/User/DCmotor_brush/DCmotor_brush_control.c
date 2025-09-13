/**
  ******************************************************************************
  * @file    DCmotor_brush_control.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-09
  * @brief   直流有刷电机的驱动程序
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "DCmotor_brush_control.h"


static motor_dir_t direction  = MOTOR_FWD;     // 记录电机旋转方向
static uint16_t    dutyfactor = 0;             // 记录占空比

/**********************************************************************
 * 函数名称： MOTOR_IN_GPIO_Init
 * 功能描述： 初始化控制电机方向的GPIO（TB6612上的IN1和IN2）
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
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
 * 函数名称： MOTOR_PWM_GPIO_Init
 * 功能描述： 初始化控制电机速度的GPIO（TB6612上的PWM引脚）
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
static void MOTOR_PWM_GPIO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(MOTOR_PWMB_CLK|MOTOR_PWMA_CLK, ENABLE); 

  /* 配置定时器1通道1输出引脚模式：复用推挽输出模式 */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_PWMB_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PWMB_PORT, &GPIO_InitStructure);

  /* 配置定时器1通道4输出引脚模式 */
  GPIO_InitStructure.GPIO_Pin =  MOTOR_PWMA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTOR_PWMA_PORT, &GPIO_InitStructure);

}

/**********************************************************************
 * 函数名称： MOTOR_PWM_TIM_Init
 * 功能描述： 配置TIM1为PWM输出模式，初始化TIM1的相关寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
static void MOTOR_PWM_TIM_Init(void)
{	
	//********************定时器时基配置*******************************
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  MOTOR_PWM_TIM_APBxClock_FUN(MOTOR_PWM_TIM_CLK, ENABLE);
	//将定时器的时钟源设置为内部时钟，即使用STM32内部时钟CK_INT=72M作为定时器时钟
  TIM_InternalClockConfig(MOTOR_PWM_TIM);
	
	
  TIM_TimeBaseStructure.TIM_Period = MOTOR_PWM_TIM_Period ;   
  /* 设置预分频：不预分频，即为72MHz,输出脉冲频率：72MHz/3600=20KHz */
  /* 同一个定时器的不同通道PWM的频率只能是相同的，不同通道可以设置为不同的占空比 */
  TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_PWM_TIM_Prescaler;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseStructure);
	
	//********************定时器输出配置*******************************
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  /* 占空比初始值设置为0 */
  TIM_OCInitStructure.TIM_Pulse = 0;
  /* 输出通道有效电平极性配置，当定时器计数值小于CCR1_Val时为高电平 */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* 定时器比较输出通道1预装载配置：使能预装载 */
  TIM_OC1PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
 /* 初始化定时器通道4输出PWM */
  TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* 定时器比较输出通道4预装载配置：使能预装载 */
  TIM_OC4PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
  // 主输出使能，当使用的是通用定时器时，这句不需要
  TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
  //使能ARR上的预装载寄存器
  TIM_ARRPreloadConfig(MOTOR_PWM_TIM, ENABLE); 
  /* 使能定时器 */
  TIM_Cmd(MOTOR_PWM_TIM, ENABLE);
  
}

/**********************************************************************
 * 函数名称： NVIC_Configuration
 * 功能描述： 初始化TIM1中断相关的NVIC寄存器
              之所以开TIM1的中断主要是为了在PC14上输出模拟PWM信号
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure = {0}; 
    	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = MOTOR_PWM_TIM_IRQ ;	
		// 设置主优先级为 15
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
	  // 设置子优先级（由于中断优先级分组4无SubPriority，所以不用配）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**********************************************************************
 * 函数名称： Motor_Init
 * 功能描述： 电机硬件初始化的接口函数
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
void Motor_Init(void)
{
  MOTOR_PWM_TIM_Init();
	MOTOR_PWM_GPIO_Init();
	NVIC_Configuration();
  MOTOR_IN_GPIO_Init();
}

/**********************************************************************
 * 函数名称： Set_Motor_Speed
 * 功能描述： 设置输入到选中电机的PWM信号的占空比以控制转速
 * 输入参数： CH - 要控制A电机还是B电机
              v  - CCR的值（0~3600）
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
void Set_Motor_Speed(MOTORx CH,uint16_t v)
{
  dutyfactor = v;
	
	switch(CH)
	{
		case B : TIM_SetCompare1(MOTOR_PWM_TIM, dutyfactor);break;//电机B占用TIM1的通道1
		case A : TIM_SetCompare4(MOTOR_PWM_TIM, dutyfactor);break;//电机A占用TIM1的通道4

	}
  
}

/**********************************************************************
 * 函数名称： Set_Motor_Direction
 * 功能描述： 设置选中电机的转动方向
 * 输入参数： CH  - 要控制A电机还是B电机
              dir - 转动方向
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/09        V1.0     shiyaoming         创建
 ***********************************************************************/
void Set_Motor_Direction(MOTORx CH,motor_dir_t dir)
{
  direction = dir;
	/*控制逻辑参考TB6612控制功能真值表*/
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


#include "DCmotor_brush_control.h"


static motor_dir_t direction  = MOTOR_FWD;     // 记录电机旋转方向
static uint16_t    dutyfactor = 0;             // 记录占空比

static void MOTOR_IN_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(MOTOR_BIN1_CLK|MOTOR_BIN2_CLK|MOTOR_AIN1_CLK|MOTOR_AIN2_CLK, ENABLE); 
	
  GPIO_StructInit(&GPIO_InitStructure);
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

/**
  * 函数功能: 配置TIMx复用输出PWM时用到的I/O
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void MOTOR_PWM_GPIO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能定时器通道引脚GPIO时钟 */
  RCC_APB2PeriphClockCmd(MOTOR_PWMB_CLK|MOTOR_PWMA_CLK, ENABLE); 

	GPIO_StructInit(&GPIO_InitStructure);
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

/**
  * 函数功能: 配置TIMx输出的PWM信号的模式，如周期、极性、占空比
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */

static void MOTOR_PWM_TIM_Init(void)
{	//********************定时器时基配置*******************************
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 使能定时器时钟 */
    MOTOR_PWM_TIM_APBxClock_FUN(MOTOR_PWM_TIM_CLK, ENABLE);
	 //将定时器的时钟源设置为内部时钟，即使用STM32内部时钟CK_INT=72M作为定时器时钟
  	TIM_InternalClockConfig(MOTOR_PWM_TIM);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  /* 定时器基本参数始终 */		
  /* 当定时器从0计数到7200-1，即为7200次，为一个定时周期 */
  TIM_TimeBaseStructure.TIM_Period = MOTOR_PWM_TIM_Period ;   
  /* 设置预分频：不预分频，即为72MHz,输出脉冲频率：72MHz/7200=10KHz */
  /* 同一个定时器的不同通道PWM的频率只能是相同的，不同通道可以设置为不同的占空比 */
  TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_PWM_TIM_Prescaler;
  /* 向上计数模式 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
  TIM_TimeBaseInit(MOTOR_PWM_TIM, &TIM_TimeBaseStructure);
	
	//********************定时器输出配置*******************************
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
  /* 定时器输出通道模式配置 */
  /* 模式配置：PWM模式1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  /* 输出状态设置：使能输出 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  
  /* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
  TIM_OCInitStructure.TIM_Pulse = 0;
  /* 当定时器计数值小于CCR1_Val时为高电平 */
 //*****这俩有效电平只有都设置为一样，才能起到互补输出的作用*****
	// 输出通道有效电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

 //*****这俩有效电平只有都设置为一样，才能起到互补输出的作用*****
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
 
  /* 初始化定时器通道1输出PWM */
  TIM_OC1Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* 定时器比较输出通道1预装载配置：使能预装载 */
  TIM_OC1PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable);
 /* 初始化定时器通道4输出PWM */
  TIM_OC4Init(MOTOR_PWM_TIM, &TIM_OCInitStructure);
  /* 定时器比较输出通道4预装载配置：使能预装载 */
  TIM_OC4PreloadConfig(MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
    // 主输出使能，当使用的是通用定时器时，这句不需要，因为通用定时器不分主输出和互补输出
  TIM_CtrlPWMOutputs(MOTOR_PWM_TIM, ENABLE);
 
  TIM_ARRPreloadConfig(MOTOR_PWM_TIM, ENABLE); //使能TIMx在ARR上的预装载寄存器
  /* 使能定时器 */
  TIM_Cmd(MOTOR_PWM_TIM, ENABLE);
  

}

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
    NVIC_InitStructure.NVIC_IRQChannel = MOTOR_PWM_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;	 
	  // 设置抢占优先级（由于中断优先级分组4无SubPriority，所以不用配）
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  电机初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
  MOTOR_PWM_TIM_Init();
	MOTOR_PWM_GPIO_Init();
	NVIC_Configuration();
  MOTOR_IN_GPIO_Init();
}

/**
  * @brief  设置电机速度
  * @param  v: 速度（占空比）
  * @retval 无
  */
void Set_Motor_Speed(MOTORx CH,uint16_t v)
{
  dutyfactor = v;
	switch(CH)
	{
		case B : TIM_SetCompare1(MOTOR_PWM_TIM, dutyfactor);break;
		case A : TIM_SetCompare4(MOTOR_PWM_TIM, dutyfactor);break;

	}
  

}

/**
  * @brief  设置电机方向
  * @param  无
  * @retval 无
  */

void Set_Motor_Direction(MOTORx CH,motor_dir_t dir)
{
  direction = dir;
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


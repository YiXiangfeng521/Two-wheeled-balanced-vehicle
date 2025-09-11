#ifndef __MOTOR_CONTROL_H
#define	__MOTOR_CONTROL_H

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f10x.h"


//引脚定义

#define  MOTOR_BIN1_PIN                  GPIO_Pin_14
#define  MOTOR_BIN1_PORT                 GPIOB
#define  MOTOR_BIN1_CLK                  RCC_APB2Periph_GPIOB


#define  MOTOR_BIN2_PIN                  GPIO_Pin_15
#define  MOTOR_BIN2_PORT                 GPIOB
#define  MOTOR_BIN2_CLK                  RCC_APB2Periph_GPIOB

#define  MOTOR_AIN1_PIN                  GPIO_Pin_13
#define  MOTOR_AIN1_PORT                 GPIOB
#define  MOTOR_AIN1_CLK                  RCC_APB2Periph_GPIOB


#define  MOTOR_AIN2_PIN                  GPIO_Pin_12
#define  MOTOR_AIN2_PORT                 GPIOB
#define  MOTOR_AIN2_CLK                  RCC_APB2Periph_GPIOB



#define MOTOR_PWM_TIM                    TIM1
#define MOTOR_PWM_TIM_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define MOTOR_PWM_TIM_CLK                RCC_APB2Periph_TIM1
#define MOTOR_PWM_TIM_Period             (3600-1)
#define MOTOR_PWM_TIM_Prescaler          (1-1)

#define MOTOR_PWMB_CLK                   RCC_APB2Periph_GPIOA
#define MOTOR_PWMB_PORT                  GPIOA
#define MOTOR_PWMB_PIN                   GPIO_Pin_8
#define MOTOR_PWMA_CLK                   RCC_APB2Periph_GPIOA
#define MOTOR_PWMA_PORT                  GPIOA
#define MOTOR_PWMA_PIN                   GPIO_Pin_11

#define            MOTOR_PWM_TIM_IRQ               TIM1_UP_IRQn
#define            MOTOR_PWM_TIM_IRQHandler        TIM1_UP_IRQHandler
/* 电机方向控制枚举 */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

typedef enum
{
  A = 0,
  B,
}MOTORx;





void Motor_Init(void);
void Set_Motor_Speed(MOTORx CH,uint16_t v);
void Set_Motor_Direction(MOTORx CH,motor_dir_t dir);



#endif /* __MOTOR_CONTROL_H*/

#ifndef __ENCODER_H
#define __ENCODER_H


#include "DCmotor_brush_control.h"


#define            ENCODER_B_TIM                   TIM4
#define            ENCODER_B_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            ENCODER_B_TIM_CLK               RCC_APB1Periph_TIM4
#define            ENCODER_B_TIM_PERIOD            (65536 - 1)
#define            ENCODER_B_TIM_PSC               0

#define            ENCODER_A_TIM                   TIM2
#define            ENCODER_A_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            ENCODER_A_TIM_CLK               RCC_APB1Periph_TIM2
#define            ENCODER_A_TIM_PERIOD            (65536 - 1)
#define            ENCODER_A_TIM_PSC               0

#define            ENCODER_A_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ENCODER_A_TIM_CH1_PORT          GPIOA
#define            ENCODER_A_TIM_CH1_PIN           GPIO_Pin_0
#define            ENCODER_A_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ENCODER_A_TIM_CH2_PORT          GPIOA
#define            ENCODER_A_TIM_CH2_PIN           GPIO_Pin_1

#define            ENCODER_B_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ENCODER_B_TIM_CH1_PORT          GPIOB
#define            ENCODER_B_TIM_CH1_PIN           GPIO_Pin_6
#define            ENCODER_B_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ENCODER_B_TIM_CH2_PORT          GPIOB
#define            ENCODER_B_TIM_CH2_PIN           GPIO_Pin_7



void Encoder_Init(void);
int Read_Speed(MOTORx CH);

#endif	/* __ENCODER_H */



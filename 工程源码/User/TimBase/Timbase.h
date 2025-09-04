#ifndef _TIMEBASE_H
#define _TIMEBASE_H


#include "stm32f10x.h"


#define            BASIC_TIM                   TIM3
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM3
#define            BASIC_TIM_Period            (10000-1)
#define            BASIC_TIM_Prescaler         (72-1)

#define            BASIC_TIM_IRQ               TIM3_IRQn
#define            BASIC_TIM_IRQHandler        TIM3_IRQHandler


void BASIC_TIM_Config(void);
void IncTick(void);
uint32_t GetTick(void);
uint64_t system_get_us(void);
void udelay(uint32_t us);
void mdelay(uint32_t ms);

#endif	/* _TIMEBASE_H */



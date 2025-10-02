#ifndef  _EXTI_H
#define  _EXTI_H

#include "stm32f10x.h" 

#define MPU6050_INT_GPIO_PORT         GPIOB
#define MPU6050_INT_GPIO_CLK          RCC_APB2Periph_GPIOB
#define MPU6050_INT_GPIO_PIN          GPIO_Pin_5

#define MPU6050_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define MPU6050_INT_EXTI_PINSOURCE    GPIO_PinSource5
#define MPU6050_INT_EXTI_LINE         EXTI_Line5

#define MPU6050_INT_EXTI_IRQ          EXTI9_5_IRQn
#define MPU6050_IRQHandler            EXTI9_5_IRQHandler



void MPU6050_EXTI_Init(void);

#endif



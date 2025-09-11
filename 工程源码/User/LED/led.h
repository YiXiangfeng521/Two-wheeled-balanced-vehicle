#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

extern unsigned char pwm_duty;

#define pwm_cnt_MAX 100       //调整该值可改变输出到LED1的模拟PWM信号的频率（f_PWM==20kHz/pwm_cnt_MAX）

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_PORT    	GPIOC			             
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define LED1_GPIO_PIN		    GPIO_Pin_14			  


#define ON  0
#define OFF 1

/* 使用标准的固件库控制IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)


void LED_GPIO_Config(void);

#endif /* __LED_H */

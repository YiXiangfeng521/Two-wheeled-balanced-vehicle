#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"

extern unsigned char pwm_duty;

#define pwm_cnt_MAX 100       //������ֵ�ɸı������LED1��ģ��PWM�źŵ�Ƶ�ʣ�f_PWM==20kHz/pwm_cnt_MAX��

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_PORT    	GPIOC			             
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOC	
#define LED1_GPIO_PIN		    GPIO_Pin_14			  


#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)


void LED_GPIO_Config(void);

#endif /* __LED_H */

#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"


extern __IO uint16_t ADC_ConvertedValue;


#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB  
#define    ADC_PORT                      GPIOB
#define    ADC_PIN                       GPIO_Pin_0
// ADC 通道宏定义
#define    ADC_CHANNEL                   ADC_Channel_8
// ADC相关DMA宏定义
#define    ADC_DMA_CLK 									 RCC_AHBPeriph_DMA1
#define    ADC_DMA_CHANNEL               DMA1_Channel1      //具体使用哪一个需要参考参考手册的DMA章节



void ADC_Config(void);
float Get_VoltageOfBattery(void);

#endif /* __ADC_H */


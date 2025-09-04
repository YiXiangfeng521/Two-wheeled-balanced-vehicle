#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"


extern __IO uint16_t ADC_ConvertedValue;


#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOB  
#define    ADC_PORT                      GPIOB
#define    ADC_PIN                       GPIO_Pin_0
// ADC ͨ���궨��
#define    ADC_CHANNEL                   ADC_Channel_8
// ADC���DMA�궨��
#define    ADC_DMA_CLK 									 RCC_AHBPeriph_DMA1
#define    ADC_DMA_CHANNEL               DMA1_Channel1      //����ʹ����һ����Ҫ�ο��ο��ֲ��DMA�½�



void ADC_Config(void);
float Get_VoltageOfBattery(void);

#endif /* __ADC_H */


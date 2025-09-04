/**
  ******************************************************************************
  * @file    adc.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   ����ADC1���ڲ�����صĵ�ѹ
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "adc.h"

__IO uint16_t ADC_ConvertedValue;//�����洢ADCת����ɺ��12bit����
 	   
/**********************************************************************
 * �������ƣ� ADC_GPIO_Init
 * ���������� ��ʼ��ADC����������
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void ADC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	RCC_APB2PeriphClockCmd( ADC_GPIO_CLK, ENABLE );
	
	GPIO_StructInit(&GPIO_InitStructure);
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		
}
/**********************************************************************
 * �������ƣ� ADC_Register_Init
 * ���������� ����ADC1�Ĺ���ģʽ����ʼ��ADC1�������ؼĴ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void ADC_Register_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	 
	RCC_APB2PeriphClockCmd( ADC_CLK, ENABLE );
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	ADC_StructInit(&ADC_InitStructure);
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫɨ�裬��ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 

	// ����Ҫ����ת����ÿ10ms��TIM3_TRGO����һ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// ��TIM3_TRGO��Ϊ����ͨ�����ⲿ����ת��Դ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// ����ͨ��ת��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_ExternalTrigConvCmd(ADCx,ENABLE);

	// ���� ADC ����ͨ��ת��˳��Ͳ���ʱ�䣨���ʵ����ã����巽���ο������ĵ�˵����
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1,ADC_SampleTime_1Cycles5);

  ADC_DMACmd(ADCx, ENABLE);
}
/**********************************************************************
 * �������ƣ� DMA_Register_Init
 * ���������� ����DMA1�Ĺ���ģʽ����ʼ����ؼĴ��������ڽ�ADC1��DR�Ĵ���
              ��ֵ���˵��ڴ�
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void DMA_Register_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);

	DMA_StructInit(&DMA_InitStructure);
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADCx->DR ) );
	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������СΪ1���������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStructure.DMA_BufferSize = 1;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ��ֻҪ�����ݾͲ��ϵش���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
}
/**********************************************************************
 * �������ƣ� ADC_Config
 * ���������� ����ADC1�Ľӿں���
 * ��������� ��
 * ��������� ��
 *            
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
void ADC_Config(void)
{
	ADC_Register_Init();
	ADC_GPIO_Init();
	DMA_Register_Init();
	// ����ADC
	ADC_Cmd(ADCx, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADCx);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADCx);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADCx));
}
/**********************************************************************
 * �������ƣ� Get_VoltageOfBattery
 * ���������� ����õ���ص�ʵ�ʵ�ѹֵ 
 * ��������� ��
 * ��������� ��
 *            
 * �� �� ֵ�� VoltageOfBattery-��ص�ʵ�ʵ�ѹֵ����λV��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
float Get_VoltageOfBattery(void)
{
	float VoltageOfBattery; 
	
	VoltageOfBattery = ADC_ConvertedValue/4095.0f*3.3f*11; //��11֮ǰ��ֵ�ǵ�ص�ѹ��ѹ�����ֵ
	
	return VoltageOfBattery;
	
}


/*********************************************END OF FILE**********************/

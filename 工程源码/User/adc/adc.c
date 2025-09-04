/**
  ******************************************************************************
  * @file    adc.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   配置ADC1用于测量电池的电压
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "adc.h"

__IO uint16_t ADC_ConvertedValue;//用来存储ADC转换完成后的12bit数据
 	   
/**********************************************************************
 * 函数名称： ADC_GPIO_Init
 * 功能描述： 初始化ADC的输入引脚
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void ADC_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	RCC_APB2PeriphClockCmd( ADC_GPIO_CLK, ENABLE );
	
	GPIO_StructInit(&GPIO_InitStructure);
	// 配置 ADC IO 引脚模式
	// 必须为模拟输入
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);		
}
/**********************************************************************
 * 函数名称： ADC_Register_Init
 * 功能描述： 配置ADC1的工作模式，初始化ADC1外设的相关寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void ADC_Register_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	 
	RCC_APB2PeriphClockCmd( ADC_CLK, ENABLE );
	// 配置ADC时钟为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	ADC_StructInit(&ADC_InitStructure);
	// ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要扫描，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 

	// 不需要连续转换，每10ms由TIM3_TRGO触发一次
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	// 把TIM3_TRGO作为规则通道的外部触发转换源
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// 规则通道转换1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_ExternalTrigConvCmd(ADCx,ENABLE);

	// 配置 ADC 规则通道转换顺序和采样时间（需适当配置，具体方法参考配套文档说明）
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1,ADC_SampleTime_1Cycles5);

  ADC_DMACmd(ADCx, ENABLE);
}
/**********************************************************************
 * 函数名称： DMA_Register_Init
 * 功能描述： 配置DMA1的工作模式，初始化相关寄存器，用于将ADC1的DR寄存器
              的值搬运到内存
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void DMA_Register_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);

	DMA_StructInit(&DMA_InitStructure);
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADCx->DR ) );
	
	// 存储器地址，实际上就是一个内部SRAM的变量
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小为1，缓冲区的大小应该等于存储器的大小
	DMA_InitStructure.DMA_BufferSize = 1;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式，只要有数据就不断地传输
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
}
/**********************************************************************
 * 函数名称： ADC_Config
 * 功能描述： 配置ADC1的接口函数
 * 输入参数： 无
 * 输出参数： 无
 *            
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
void ADC_Config(void)
{
	ADC_Register_Init();
	ADC_GPIO_Init();
	DMA_Register_Init();
	// 开启ADC
	ADC_Cmd(ADCx, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC开始校准
	ADC_StartCalibration(ADCx);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADCx));
}
/**********************************************************************
 * 函数名称： Get_VoltageOfBattery
 * 功能描述： 计算得到电池的实际电压值 
 * 输入参数： 无
 * 输出参数： 无
 *            
 * 返 回 值： VoltageOfBattery-电池的实际电压值（单位V）
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
float Get_VoltageOfBattery(void)
{
	float VoltageOfBattery; 
	
	VoltageOfBattery = ADC_ConvertedValue/4095.0f*3.3f*11; //乘11之前的值是电池电压分压后的数值
	
	return VoltageOfBattery;
	
}


/*********************************************END OF FILE**********************/

/**
  ******************************************************************************
  * @file    usart.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   将USART1作为和电脑上位机通信的串口，用于打印各种调试信息
  ******************************************************************************
  * @attention
  *   拷贝请注明来处。
  ******************************************************************************
  */
#include "usart.h"


/**********************************************************************
 * 函数名称： USART1_GPIO_Init
 * 功能描述： 初始化USART1的TX和RX的GPIO
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void USART1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);	
}
/**********************************************************************
 * 函数名称： USART1_Register_Init
 * 功能描述： 配置USART1的工作模式，初始化USART1外设的相关寄存器
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
static void USART1_Register_Init(void)
{
	USART_InitTypeDef USART_InitStructure;

	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	USART_StructInit(&USART_InitStructure);
	
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
}
/**********************************************************************
 * 函数名称： USART1_Config
 * 功能描述： 配置USART1的接口函数
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
void USART1_Config(void)
{
	USART1_Register_Init();
	USART1_GPIO_Init(); 
  // 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);		
}


/**********************************************************************
 * 函数名称： USART1_Config
 * 功能描述： 重定向c库函数printf到串口，重定向后可使用printf函数
 * 输入参数： 
 * 输出参数： 
 *            
 * 返 回 值： ch-被发送出去的数据
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口1 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送数据寄存器空 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	    
		return (ch);
}
/**********************************************************************
 * 函数名称： USART1_Config
 * 功能描述： 重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
 * 输入参数： 
 * 输出参数： 
 *            
 * 返 回 值： 串口1接收到的数据
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         创建
 ***********************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


/**
  ******************************************************************************
  * @file    usart.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   ��USART1��Ϊ�͵�����λ��ͨ�ŵĴ��ڣ����ڴ�ӡ���ֵ�����Ϣ
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "usart.h"


/**********************************************************************
 * �������ƣ� USART1_GPIO_Init
 * ���������� ��ʼ��USART1��TX��RX��GPIO
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
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
 * �������ƣ� USART1_Register_Init
 * ���������� ����USART1�Ĺ���ģʽ����ʼ��USART1�������ؼĴ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
static void USART1_Register_Init(void)
{
	USART_InitTypeDef USART_InitStructure;

	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	USART_StructInit(&USART_InitStructure);
	
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// �����������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
}
/**********************************************************************
 * �������ƣ� USART1_Config
 * ���������� ����USART1�Ľӿں���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
void USART1_Config(void)
{
	USART1_Register_Init();
	USART1_GPIO_Init(); 
  // ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);		
}


/**********************************************************************
 * �������ƣ� USART1_Config
 * ���������� �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
 * ��������� 
 * ��������� 
 *            
 * �� �� ֵ�� ch-�����ͳ�ȥ������
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����1 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ��������ݼĴ����� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	    
		return (ch);
}
/**********************************************************************
 * �������ƣ� USART1_Config
 * ���������� �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
 * ��������� 
 * ��������� 
 *            
 * �� �� ֵ�� ����1���յ�������
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


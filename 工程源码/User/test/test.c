/**
  ******************************************************************************
  * @file    test.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-04
  * @brief   �ṩ��ģ��Ĳ��Գ���
  ******************************************************************************
  * @attention
  *   ������ע��������
  ******************************************************************************
  */
#include "test.h"


/**********************************************************************
 * �������ƣ� VoltageOfBattery_Test
 * ���������� ������ص�ѹ�Ĳ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/04        V1.0     shiyaoming         ����
 ***********************************************************************/
void VoltageOfBattery_Test(void)
{
	float value;
	
	ADC_Config();
	
	while(1)
	{   
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET)
		{
			DMA_ClearFlag(DMA1_FLAG_TC1);
			
			value = Get_VoltageOfBattery();
			printf("%.2f\n",value);             //���������ݺ��һ��'\n',������λ���޷�����������
			
		}
		  
	}
	
}
/**********************************************************************
 * �������ƣ� Motor_Test
 * ���������� ��������Ĳ��Գ���
 * ��������� ��
 * ��������� ��
 *            ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���        �޸�����
 * -----------------------------------------------
 * 2025/09/10        V1.0     shiyaoming         ����
 ***********************************************************************/
void Motor_Test(void)
{
	
	Motor_Init();
	
	mdelay(2000);                       //Ӳ����ʼ��������ʱ2�룬�Է�ֹ���ͻȻת������Σ��
	/*������A�����ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(A,MOTOR_FWD);
	Set_Motor_Speed(A,500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);
	mdelay(1000);
	Set_Motor_Speed(A,3000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);
	mdelay(1000);
	Set_Motor_Speed(A,500);
	mdelay(1000);
  Set_Motor_Speed(A,0);
	/*A���ֹͣ��������B�����ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(B,MOTOR_FWD);
	Set_Motor_Speed(B,500);
	mdelay(1000);
	Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(B,3000);
	mdelay(1000);
	Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(B,500);
	mdelay(1000);
  Set_Motor_Speed(B,0);
	/*B���ֹͣ����������̨���ͬʱ��ת��ת���𽥼ӿ����𽥼�����*/
	Set_Motor_Direction(A,MOTOR_REV);Set_Motor_Direction(B,MOTOR_REV);
	Set_Motor_Speed(A,500);Set_Motor_Speed(B,500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(A,2500);Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(A,3000);Set_Motor_Speed(B,3000);
	mdelay(3000);
	Set_Motor_Speed(A,2500);Set_Motor_Speed(B,2500);
	mdelay(1000);
	Set_Motor_Speed(A,2000);Set_Motor_Speed(B,2000);
	mdelay(1000);
	Set_Motor_Speed(A,1500);Set_Motor_Speed(B,1500);
	mdelay(1000);
	Set_Motor_Speed(A,1000);Set_Motor_Speed(B,1000);
	mdelay(1000);
	Set_Motor_Speed(A,500);Set_Motor_Speed(B,500);
	mdelay(1000);
  Set_Motor_Speed(A,0);Set_Motor_Speed(B,0);
	
	
	
	while(1)
	{
		  
		  
	}
	
}




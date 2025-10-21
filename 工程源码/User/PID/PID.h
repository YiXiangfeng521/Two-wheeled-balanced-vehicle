#ifndef _PID_H
#define _PID_H



#include "stm32f10x.h"
#include <math.h>
#include <stdlib.h>

/*����PID��������*/
typedef struct PID_TYPE
{
 float SP;//����ֵ
 float Kp;//����ϵ��
 float Kd;//΢��ϵ�� 
 float Ki;//����ϵ��
 float S_Err;//����ֵ
 float D_Err;//΢��ֵ
 float Err_Last;//��һ�ε����ֵ
 uint32_t cnt;//���ڼ�¼PID�������Ƿ��ǵ�һ�����еı�־
 float (*PID_Compute)(struct PID_TYPE *pid,float FB);//PID���㺯��ָ��
 
}PID_TypeDef;


#endif /*_PID_H*/

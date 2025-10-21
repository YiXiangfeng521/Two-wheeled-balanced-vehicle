#ifndef _PID_H
#define _PID_H



#include "stm32f10x.h"
#include <math.h>
#include <stdlib.h>

/*定义PID控制器类*/
typedef struct PID_TYPE
{
 float SP;//期望值
 float Kp;//比例系数
 float Kd;//微分系数 
 float Ki;//积分系数
 float S_Err;//积分值
 float D_Err;//微分值
 float Err_Last;//上一次的误差值
 uint32_t cnt;//用于记录PID控制器是否是第一次运行的标志
 float (*PID_Compute)(struct PID_TYPE *pid,float FB);//PID运算函数指针
 
}PID_TypeDef;


#endif /*_PID_H*/

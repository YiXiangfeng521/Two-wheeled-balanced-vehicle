#ifndef _TEST_H
#define _TEST_H

#include "Timbase.h" 
#include "usart.h"
#include "adc.h"
#include "DCmotor_brush_control.h"
#include "encoder.h" 
#include "mpu6050.h"
#include "pid.h"

void VoltageOfBattery_Test(void);
void Motor_Test(void);
void Encoder_Test(void);
void MPU6050_Test(void);
float PID_Compute_Test(PID_TypeDef *pid,float FB);
void PID_CtrlMotor_Test(void);
#endif /*_TEST_H*/



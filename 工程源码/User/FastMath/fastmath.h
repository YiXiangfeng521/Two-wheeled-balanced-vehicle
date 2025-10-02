#ifndef _FASTMATH_H
#define _FASTMATH_H

#include <math.h>
#include <stdio.h>


//相关配置
#define RADIAN_STEP		0.001

//常用的值
#define M_PI        3.1415926535897932
#define M_2_PI     6.2831853071795862        //pi*2
#define M_PI_2     1.5707963267948965        //pi/2 
#define M_180_D_PI  57.29577951308232        //180/pi
#define M_PI_D180   0.017453292519943        //pi/180
#define M_E			2.71828182845904523536   // e		
#define M_E_P2		6.580885991017920		//2^e
#define TABLE_SIZE ((int)( M_PI_2 / RADIAN_STEP ) + 1 )//?只记录0到pi/2的表



double Abs_float(double a);
_Bool judge_float_equal(double value_1, double value_2);
double pow_custom(double base, int exponent);
double sqrt_newton(double a);
double sqrt_lookup(float value);
int find_closest_index(double value, const double* table);
double sin_lookup(double radian);
double cos_lookup(double radian);
double tan_lookup(double radian);
double asin_lookup(double value);
double acos_lookup(double value);
double atan_lookup(double value);
double atan2_lookup(double y, double x);

#endif	/*_FASTMATH_H*/


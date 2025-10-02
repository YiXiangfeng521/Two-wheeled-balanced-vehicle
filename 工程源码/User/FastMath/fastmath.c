/**
  ******************************************************************************
  * @file    fastmath.c
  * @author  ShiYaoming
  * @version V1.0
  * @date    2025-09-30
  * @brief   ���ʵ����ѧ���еĳ�������
  ******************************************************************************
  * @attention
  *   ԭ���ߣ�https://gitee.com/jasonakzhang/my_math
  ******************************************************************************
  */
	
#include "fastmath.h"
#include "my_math_table.h"

//�����;���ֵ
double Abs_float(double a)
{
    if (a >= 0)return a;
    else return (a * -(1.0));
}

//�Ƚϸ������Ƿ����
_Bool judge_float_equal(double value_1, double value_2)
{
    //��ֵ�����������ã�������Ϊ��ֵ���С��1e-4�����
    if (Abs_float(value_1 - value_2) < 1.0e-4)return 1;
    return 0;
}

//powֻ��������������
double pow_custom(double base, int exponent) 
{
    double result = 1.0;
    // ͨ��ѭ���� base �������� exponent ��
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

//ţ�ٱƽ������������ȿɵ������ߣ�������ʱ�䳤
double sqrt_newton(double a) 
{
    if (a < 0)return -1; // ���ش���ֵ����ʾ������Ч 
    double x0 = a / 3.0; // ��ʼ�²�ֵ�������ֵ�󣬳�����Խ�󣬵�������Խ�٣���ͬʱ��С��ֵ�ĵ������������ӣ������趨
    double x1;
    double error = 1e-4; // �趨�����ֵ 
    int max_iterations = 1000; // ��������������ֹ��ѭ�� 

    for (int i = 0; i < max_iterations; ++i) {
        x1 = (x0 + a / x0) / 2.0; // ţ�ٵ�����ʽ 
        if (Abs_float(x1 - x0) < error) {
            break;
        }
        x0 = x1;
    }

    return x1;
}




// ���ַ�����������ֵ�ͱ����ֵ���������ֵ�ڱ��е�λ��
int find_closest_index(double value, const double* table) 
{
    int left = 0;
    int right = TABLE_SIZE - 1;
    int mid;
    do{
        mid = left + (right - left) / 2;
        if (value < table[mid])right = mid - 1;
        else if (value > table[mid]) left = mid + 1;
        else return mid;//��ֹ����ֵ�պúͱ�ֵ���
    } while (left <= right);
    return mid;
}

double sin_lookup(double radian) 
{
    // ������ֵ������ -pi �� pi
    while (radian < -M_PI) {
        radian += M_2_PI;
    }
    while (radian > M_PI) {
        radian -= M_2_PI;
    }

    // ���
    if (radian < 0) {
        if (radian < -M_PI_2) {
            // -pi �� -pi/2 �յ���ʽ
            int index = (int)(((M_PI + radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return -sin_table[index] - ((M_PI + radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        } else {
            // -pi/2 �� 0 �յ���ʽ
            int index = (int)(((-radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return -sin_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        }
    } else {
        if (radian < M_PI_2) {
            // 0 �� pi/2 ֱ�Ӳ��
            int index = (int)(((radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return sin_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        } else {
            // pi/2 �� pi �յ���ʽ
            int index = (int)(((M_PI - radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return sin_table[index] + ((M_PI - radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        }
    }
}

double cos_lookup(double radian)
{
    //�յ���ʽ
    return sin_lookup(M_PI_2 - radian);
}

double tan_lookup(double radian)
{
    //������ֵ������-pi/2��pi/2
    while (radian < -M_PI_2)
    {
        radian += M_PI;
    }
    while (radian > M_PI_2)
    {
        radian -= M_PI;
    }
    // ���
    if (radian > 0) {
        int index = (int)((radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            return tan_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return tan_table[index];
        }
    } else {
        int index = (int)((-radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            return -tan_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return -tan_table[index];
        }
    }
}

double asin_lookup(double value) 
{
    if (value < -1.0 || value > 1.0) {
        // ����ֵ������Χ�����ش���ֵ��ԭ����NAN������ȡ0
        return 0;
    }
    if (value > 0) {
        int index = find_closest_index(value, sin_table);
        if (index < TABLE_SIZE - 1) {
            return RADIAN_STEP*index + ((value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return M_PI_2;
        }
    } else {
        int index = find_closest_index(-value, sin_table);
        if (index < TABLE_SIZE - 1) {
            return -RADIAN_STEP*index - ((-value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return -M_PI_2;
        }
    }
}

double acos_lookup(double value) 
{
    //��Ч��ʽ
    return (M_PI_2 - asin_lookup(value));
}

double atan_lookup(double value) {
        if (value > 0) {
        int index = find_closest_index(value, tan_table);
        if (index < TABLE_SIZE - 1) {
            return RADIAN_STEP * index + ((value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return M_PI_2;
        }
    } else {
        int index = find_closest_index(-value, tan_table);
        if (index < TABLE_SIZE - 1) {
            return -RADIAN_STEP * index - ((-value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return -M_PI_2;
        }
    }
}

double atan2_lookup(double y, double x) 
{
    if (judge_float_equal(x,0)) {
        if (y > 0) {
            return M_PI_2;
        }
        else if (y < 0) {
            return -M_PI_2;
        }
        else {
            return 0;
        }
    }
    if (x > 0) {
        return atan_lookup(y / x);
    }
    else if (y >= 0) {
        return atan_lookup(y / x) + M_PI;
    }
    else {
        return atan_lookup(y / x) - M_PI;
    }
}

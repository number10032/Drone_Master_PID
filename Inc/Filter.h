#ifndef _FILTER_H_
#define _FILTER_H_
#include "stm32g0xx_hal.h"
#include "struct_all.h"
/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Calculate_FilteringCoefficient(float Time, float Frequency);
void ACC_IIR_Filter(struct _acc *Acc_in, struct _acc *Acc_out);
void Gyro_Filter(struct _gyro *Gyro_in, struct _gyro *Gyro_out);

#endif


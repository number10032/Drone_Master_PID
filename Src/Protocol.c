/*
  ΢������Դ���Ȩ�����纽Э�з������Ŷ����У�δ�����Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
	�뱾�����زο��������纽Э΢�����Ὺ��ָ�ϣ����ݶԱ��װ�������������Լ�Ӳ����ض�������ϸ�Ľ���
  �����ͬѧ���˸���������ĸĽ������κν��飬����ʱ�����Ǳ�����ϵ��
	���ߣ�PhillWeston
	��ϵ���䣺2436559745@qq.com
*/
#include "Maths.h"
#include "struct_all.h"

/******************************************************************************
����ԭ�ͣ�	void Print_MSP_RAW_IMU(void)
��    �ܣ�	���ʹ�����ԭʼ����
*******************************************************************************/ 
void Print_MSP_RAW_IMU(void)
{
	uint8_t	data[32];
	int16_t send_data;
	
	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 18;
	data[4] = MSP_RAW_IMU;
	
	send_data = (int16_t)(acc.x/4);
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(acc.y/4);
	data[7] =  send_data & 0xFF ;
	data[8] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(acc.z/4);
	data[9] =  send_data & 0xFF ;
	data[10] = (send_data >> 8) & 0xFF;

	send_data = (int16_t)(gyro.y);
	data[11] =  send_data & 0xFF ;
	data[12] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(gyro.x);
	data[13] =  send_data & 0xFF ;
	data[14] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(-gyro.z);
	data[15] =  send_data & 0xFF ;
	data[16] = (send_data >> 8) & 0xFF;

	send_data = (int16_t)(filter_acc.x*0.75f);
	data[17] =  send_data & 0xFF ;
	data[18] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(filter_acc.y*0.75f);
	data[19] =  send_data & 0xFF ;
	data[20] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(filter_acc.z*0.75f);
	data[21] =  send_data & 0xFF ;
	data[22] = (send_data >> 8) & 0xFF;
	
	data[23] = Get_Checksum(data);
	
	Bluetooth_Send_TX(data, 32);
}

/******************************************************************************
����ԭ�ͣ�	void Print_MSP_ATTITUDE(void)
��    �ܣ�	������̬����
*******************************************************************************/ 
void Print_MSP_ATTITUDE(void)
{
	uint8_t	data[32];
	int16_t send_data;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//������λ��
	data[3] = 6;
	data[4] = MSP_ATTITUDE;
	//data[5]��data[6]����һ����ʮ��λ����̬����-rollֵ
	send_data = (int16_t)(out_angle.roll * 10);
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	//data[7]��data[8]����һ����ʮ��λ����̬����-pitchֵ
	send_data = (int16_t)(out_angle.pitch * 10);
	data[7] =  send_data & 0xFF ;
	data[8] = (send_data >> 8) & 0xFF;
	//data[9]��data[10]����һ����ʮ��λ����̬����-yawֵ��ע�������Ǹ���
	send_data = (int16_t)(-out_angle.yaw);
	data[9] =  send_data & 0xFF ;
	data[10] = (send_data >> 8) & 0xFF;
	
	data[11] = Get_Checksum(data);
	
	Bluetooth_Send_TX(data, 32);
}

/******************************************************************************
����ԭ�ͣ�	void Print_MSP_FLY_DATA(void)
��    �ܣ�	���ͷɿ����ݣ��Զ���ͨѶ��
*******************************************************************************/ 
void Print_MSP_FLY_DATA(void)
{
	uint8_t	data[32];
	int16_t send_data;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//����ң����
	data[3] = 23;
	data[4] = MSP_FLY_DATA;
	//data[5]��data[6]����һ����ʮ��λ�ĵ�ص�ѹֵ
	send_data = Battery_Fly;//��ص�ѹֵ��100��
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	//data[7]��data[8]����һ����ʮ��λ��MOS1��PWMֵ
	data[7] =   (TIM1->CCR1+1000) & 0xFF;
	data[8] =  ((TIM1->CCR1+1000) >> 8) & 0xFF;
	//data[9]��data[10]����һ����ʮ��λ��MOS2��PWMֵ
	data[9] =   (TIM1->CCR2+1000) & 0xFF;
	data[10] =  ((TIM1->CCR2+1000) >> 8) & 0xFF;
	//data[11]��data[12]����һ����ʮ��λ��MOS3��PWMֵ
	data[11] =    (TIM1->CCR3+1000) & 0xFF;
	data[12] =  ((TIM1->CCR3+1000) >> 8) & 0xFF;
	//data[12]��data[14]����һ����ʮ��λ��MOS4��PWMֵ
	data[13] =   (TIM1->CCR4+1000) & 0xFF;
	data[14] =  ((TIM1->CCR4+1000) >> 8) & 0xFF;
	//����pid������roll�ṹ���kp,ki,kdֵ
	data[15] = (uint8_t)(roll.kp*10);
	data[16] = (uint8_t)(roll.ki*1000);
	data[17] = (uint8_t)(roll.kd);
	//���������ǽ�����roll�ṹ���kp,ki,kdֵ
	data[18]  = (uint8_t)(gyro_roll.kp*10);
	data[19]  = (uint8_t)(gyro_roll.ki*1000);
	data[20] = (uint8_t)(gyro_roll.kd);
	//���������ǽ�����yaw�ṹ���kp,ki,kdֵ
	data[21] = (uint8_t)(gyro_yaw.kp*10);
	data[22] = (uint8_t)(gyro_yaw.ki*1000);
	data[23] = (uint8_t)(gyro_yaw.kd);
////////////////////////////////////////////////////
	data[24] =  I2C_Erro & 0xFF;
	data[25] = (I2C_Erro>>8) & 0xFF;
	data[26] = (I2C_Erro>>16) & 0xFF;
	data[27] = (I2C_Erro>>24) & 0xFF;
	
	data[28] = Get_Checksum(data);

	Bluetooth_Send_TX(data, 32);
}

/*
  ΢������Դ���Ȩ�����纽Э�з������Ŷ����У�δ�����Ŷ�ͬ�⣬�������������ϴ�����Դ�롣
	�뱾�����زο��������纽Э΢�����Ὺ��ָ�ϣ����ݶԱ��װ�������������Լ�Ӳ����ض�������ϸ�Ľ���
  �����ͬѧ���˸���������ĸĽ������κν��飬����ʱ�����Ǳ�����ϵ��
	���ߣ�PhillWeston
	��ϵ���䣺2436559745@qq.com
*/
#include "Control.h"
#include "Advanced.h"
#include "struct_all.h"

/******************************************************************************
							�궨��
*******************************************************************************/ 

/******************************************************************************
							��������
*******************************************************************************/ 
uint8_t Bluetooth_RXDATA[32];//Bluetooth���յ�������
uint8_t Bluetooth_TXDATA[32];//Bluetooth��Ҫ���͵�����
//static uint8_t TX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���ص�ַ
//static uint8_t RX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//���յ�ַ
static uint16_t Bluetooth_Erro;

/******************************************************************************
����ԭ�ͣ�	static void Bluetooth_Analyse(void)
��    �ܣ�	����Bluetooth�յ�������֡
*******************************************************************************/
static void Bluetooth_Analyse(void)
{
	uint8_t sum = 0,i;
	uint8_t len = Bluetooth_RXDATA[3] + 5;
	for(i=3;i<len;i++)
		sum ^= Bluetooth_RXDATA[i];
	if( sum!=Bluetooth_RXDATA[len] )	return;	//����У��
	if( Bluetooth_RXDATA[0] != '$' )	return;	//����У��
	if( Bluetooth_RXDATA[1] != 'M' )	return;	//����У��
	if( Bluetooth_RXDATA[2] != '<' )	return;	//��λ����ң���·����ɿ�����
	
	if( Bluetooth_RXDATA[4] == MSP_RC )//�������־
	{
		Rc.ROLL 	= ( (uint16_t)(Bluetooth_RXDATA[6])  << 8 ) | Bluetooth_RXDATA[5];
		Rc.PITCH	= ( (uint16_t)(Bluetooth_RXDATA[8])  << 8 ) | Bluetooth_RXDATA[7];
		Rc.YAW 		= ( (uint16_t)(Bluetooth_RXDATA[10]) << 8 ) | Bluetooth_RXDATA[9];
		Rc.THROTTLE = ( (uint16_t)(Bluetooth_RXDATA[12]) << 8 ) | Bluetooth_RXDATA[11];

		Rc.AUX1 	= ( (uint16_t)(Bluetooth_RXDATA[14]) << 8 ) | Bluetooth_RXDATA[13];
		Rc.AUX2		= ( (uint16_t)(Bluetooth_RXDATA[16]) << 8 ) | Bluetooth_RXDATA[15];
		Rc.AUX3		= ( (uint16_t)(Bluetooth_RXDATA[18]) << 8 ) | Bluetooth_RXDATA[17];
		Rc.AUX4 	= ( (uint16_t)(Bluetooth_RXDATA[20]) << 8 ) | Bluetooth_RXDATA[19];	
		
		RC_Limit(&Rc);	
		RC_LOCK();
	}
	if( Bluetooth_RXDATA[4] == MSP_SET_PID )//�������־
	{
		roll.kp  = (float)Bluetooth_RXDATA[5]/10.f;	
		roll.ki  = (float)Bluetooth_RXDATA[6]/1000.0f;
		roll.kd  = (float)Bluetooth_RXDATA[7];
		pitch.kp = roll.kp;
		pitch.ki = roll.ki;
		pitch.kd = roll.kd;

		gyro_roll.kp  = (float)Bluetooth_RXDATA[8]/10.f;
		gyro_roll.ki  = (float)Bluetooth_RXDATA[9]/1000.f;
		gyro_roll.kd  = (float)Bluetooth_RXDATA[10];
		gyro_pitch.kp = gyro_roll.kp;
		gyro_pitch.ki = gyro_roll.ki;
		gyro_pitch.kd = gyro_roll.kd;
		
		gyro_yaw.kp = (float)Bluetooth_RXDATA[11]/10.f;
		gyro_yaw.ki = (float)Bluetooth_RXDATA[12]/1000.f;
		gyro_yaw.kd = (float)Bluetooth_RXDATA[13];
		
		EEPROM_SAVE_PID();
	}
	if( Bluetooth_RXDATA[4] == MSP_ACC_CALIBRATION )//�������־
	{
		Do_ACC_Offset();//У�����ٶȼ�
	}
	if( Bluetooth_RXDATA[4] == MSP_MAG_CALIBRATION )//�������־
	{
		Do_GYRO_Offset();//У��������
	}
	if( Bluetooth_RXDATA[4] == MSP_RESET_CONF )//�������־
	{
		PID_Reset();//����PID����
	}
}

/******************************************************************************
����ԭ�ͣ�	void Send_Data_Back(void)
��    �ܣ�	��ң�ط�������
*******************************************************************************/
void Send_Data_Back(void)
{
	static uint8_t turn_count;
	turn_count++;
	switch(turn_count)
	{
		case 1: Print_MSP_RAW_IMU();	break;
		case 2: Print_MSP_ATTITUDE();	break;
		case 3: Print_MSP_FLY_DATA();	
				turn_count=0;			break;
		default:turn_count=0;
	}
}

/******************************************************************************
����ԭ�ͣ�	void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len)
��    �ܣ�	Bluetooth�������ݰ�
*******************************************************************************/
void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len)
{
	HAL_UART_Transmit(&huart2, tx_buf, sizeof(tx_buf), 0xffff);
}

/******************************************************************************
����ԭ�ͣ�	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
��    �ܣ�	UART2�ж�
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Transmit(&huart2, Bluetooth_RXDATA, 1, 100);    // ���յ����ֽ�ԭ�����ͳ�ȥ(���Ե�ʱ�������Ҫ)
	HAL_UART_Receive_IT(&huart2, Bluetooth_RXDATA, 1);       // ����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����
}

/******************************************************************************
����ԭ�ͣ�	void Bluetooth_Connect(void)
��    �ܣ�	Bluetooth���Ӻ���
*******************************************************************************/
void Bluetooth_Connect(void)//1KHZ
{
	Bluetooth_Erro ++;
	if(Bluetooth_Erro==1)
	{
		Bluetooth_Analyse();//�������������յ�������֡
		Send_Data_Back();  //��ң�ط�������
	}
	if(Bluetooth_Erro%100==0)  //0.1sδ������������ ����ͼ����ң��
	{	
		//NRF24L01_IRQ();    //����жϱ�־λ
	}
	if(Bluetooth_Erro>=1000)   //1sδ������������ ,�������ᣬ���ͣת����ֹ���ⷢ��
	{
		Bluetooth_Erro = 1;
		//FailSafe();
		Rc_Lock=1;  //����
	}
	
	Battery_Fly=(int16_t)(2.0f*ADC_Value[0]/ADC_Value[1]*1.2f*100);
	if(Battery_Fly>330)
	{
		if(Rc.AUX4 & Led_Mode)  //ҹ��ģʽ
			LED_ON();
		else
			LED_OFF();
	}
	else
	{
		LED_ON_Frequency(2);
	}
}

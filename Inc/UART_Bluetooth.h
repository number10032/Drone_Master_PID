#ifndef _UART_BLUETOOTH_H_
#define _UART_BLUETOOTH_H_
#include "stm32g0xx_hal.h"

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
extern uint8_t Bluetooth_RXDATA[32];//Bluetooth���յ�������
extern uint8_t Bluetooth_TXDATA[32];//Bluetooth��Ҫ���͵�����

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Bluetooth_Connect(void);
void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len);
#endif

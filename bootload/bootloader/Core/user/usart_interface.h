/*
 * usart_interface.h
 *
 *  Created on: Jul 11, 2025
 *      Author: Administrator
 */

#ifndef USER_USART_INTERFACE_H_
#define USER_USART_INTERFACE_H_

#include "main.h"

#define	TX1_Size		1088
#define	RX1_Size		1088

//调试串口
extern uint8_t  buffer_TX1[TX1_Size];
extern uint8_t  buffer_RX1[RX1_Size];
extern uint16_t Rx1_Count;
extern uint8_t  Rx1Sign;

//发送数据函数
extern void usartSend(void* date, uint16_t size);


#endif /* USER_USART_INTERFACE_H_ */

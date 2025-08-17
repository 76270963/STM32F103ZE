/*
 * usart_interface.c
 *
 *  Created on: Jul 11, 2025
 *      Author: Administrator
 */


#include "usart_interface.h"

uint8_t  buffer_TX1[TX1_Size];
uint8_t  buffer_RX1[RX1_Size];
uint8_t  Rx1Sign;
uint16_t Rx1_Count = 0;


#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


#define HAL_USART_DMA				// DMA 串口发送数据
PUTCHAR_PROTOTYPE
{
	#ifdef HAL_USART_DMA
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ch, 1);		//DMA串口发送数据
		while(!__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC));
	#else
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);	//普通串口发送数据
	#endif
	    return ch;
}


void usartSend(void* date, uint16_t size)
{
	size = size > TX1_Size ? TX1_Size : size;
	memcpy(buffer_TX1, date, size);
	HAL_UART_Transmit_DMA(&huart1, buffer_TX1, size);
}

//接收完成中断函数
void USART_IRQHandler_myself(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
			HAL_UART_DMAStop(&huart1);
			Rx1_Count = RX1_Size - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
			Rx1Sign = 1;
			HAL_UART_Receive_DMA(&huart1,buffer_RX1,RX1_Size);
		}
	}
}
//DMA接收溢出错误，回调清除错误标志
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// 检查是否发生帧错误
	if (huart->ErrorCode & HAL_UART_ERROR_FE)
	{
		__HAL_UART_CLEAR_FEFLAG(huart);
		huart->RxState = HAL_UART_STATE_READY;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}

	// 检查是否发生噪声错误
	if (huart->ErrorCode & HAL_UART_ERROR_NE)
	{
		__HAL_UART_CLEAR_NEFLAG(huart);
		huart->RxState = HAL_UART_STATE_READY;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}

	// 检查是否发生溢出错误
	if (huart->ErrorCode & HAL_UART_ERROR_ORE)
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		huart->RxState = HAL_UART_STATE_READY;
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
	}

	if(huart->Instance == USART1)
		HAL_UART_Receive_DMA(&huart1,buffer_RX1,RX1_Size);

}



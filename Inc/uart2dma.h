/*
 * uart2dma.h
 *
 *  Created on: Apr 8, 2024
 *      Author: kccistc
 */

#ifndef INC_UART2DMA_H_
#define INC_UART2DMA_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define UART2_DMA_RXSIZE	256

#define GET_U2_DMA_SIZE()		(huart2.hdmarx->Instance->NDTR)
#define GET_U2_DMA_MAX_SIZE()	(huart2.RxXferSize)

void Uart2_DMA_Config(void);
unsigned char Uart2_DMA_Getchar(void);
void Uart2_Thread(void);
void UserPacket_Thread(void);

extern unsigned int uart2_rx_dma_cnt;
extern unsigned char Data1;

#endif /* INC_UART2DMA_H_ */

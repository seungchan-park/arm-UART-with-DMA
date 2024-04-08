/*
 * uart2dma.c
 *
 *  Created on: Apr 8, 2024
 *      Author: kccistc
 */
#include "uart2dma.h"

extern UART_HandleTypeDef huart2;

unsigned char uart2_rxdma_buff[UART2_DMA_RXSIZE]={0, };
unsigned char uart2_rxdma_data[UART2_DMA_RXSIZE]={0, };

unsigned int uart2_wr_dma_cnt=0, uart2_rd_dma_cnt=0;
unsigned int uart2_rx_dma_cnt=0;

short unsigned int Uart2_Old_Cnt=0;

unsigned char uart_data[256] = {0, };
unsigned char uart_rx_cnt = 0;

unsigned char Data1 = 0, Data2 = 0, Data3 = 0, Data4 = 0, Data5 = 0;

void Uart2_DMA_Config(void){
	HAL_UART_Receive_DMA(&huart2, uart2_rxdma_buff, UART2_DMA_RXSIZE);
}

unsigned char Uart2_DMA_Getchar(void){
	unsigned char data=0;

	if(uart2_rx_dma_cnt){
		data=uart2_rxdma_data[uart2_rd_dma_cnt++];
		uart2_rd_dma_cnt%=UART2_DMA_RXSIZE;
		--uart2_rx_dma_cnt;
	}

	return data;
}

void Uart2_Thread(void){
	short unsigned int New_Cnt=0;

	New_Cnt=GET_U2_DMA_MAX_SIZE()-GET_U2_DMA_SIZE();
	while(New_Cnt!=Uart2_Old_Cnt){
		uart2_rxdma_data[uart2_wr_dma_cnt++]=uart2_rxdma_buff[Uart2_Old_Cnt];
		uart2_wr_dma_cnt%=UART2_DMA_RXSIZE;
		++uart2_rx_dma_cnt;

		++Uart2_Old_Cnt;
		Uart2_Old_Cnt%=GET_U2_DMA_MAX_SIZE();
	}
}

void UserPacket_Thread(void){
	unsigned char chkSUM = 0;

	Uart2_Thread();

	if(sec1ms_uart_timeout>3000){
		sec1ms_uart_timeout=0;
		uart_rx_cnt=0;
		sprintf(str, "UART TIMEOUT\n");
		HAL_UART_Transmit(&huart2, (unsigned char*)str, strlen(str), 50);
	}
	if(uart2_rx_dma_cnt){
		uart_data[uart_rx_cnt]=Uart2_DMA_Getchar();
		sec1ms_uart_timeout=0;
		++uart_rx_cnt;

		switch(uart_rx_cnt){
		case 1:
			if(uart_data[uart_rx_cnt-1]!=0xAA)uart_rx_cnt=0;
			break;

		case 2:
			if(uart_data[uart_rx_cnt-1]!=0x55)uart_rx_cnt=0;
			break;

		case 8:
			chkSUM = uart_data[0] + uart_data[1] + uart_data[2] + uart_data[3] + uart_data[4] + uart_data[5] + uart_data[6];
			if (chkSUM == uart_data[7]) {
				Data1 = uart_data[2];
				Data2 = uart_data[3];
				Data3 = uart_data[4];
				Data4 = uart_data[5];
				Data5 = uart_data[6];

				sprintf(str, "Packet: %3d, %3d, %3d, %3d, %3d\n", Data1, Data2, Data3, Data4, Data5);
				HAL_UART_Transmit(&huart2, (unsigned char*)str, strlen(str), 50);

			}
			uart_rx_cnt = 0;
			break;
		}
	}
}


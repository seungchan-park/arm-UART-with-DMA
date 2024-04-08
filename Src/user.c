/*
 * user.c
 *
 *  Created on: Apr 8, 2024
 *      Author: kccistc
 */
#include "user.h"

char str[256]= {0, };
unsigned int sec1ms_uart_timeout=0;
unsigned char data1_flag = 0;
unsigned char led_cnt = 0;
unsigned int sec1ms1 = 0;

void User_Task1(void){

	Uart2_DMA_Config();
	while(1){
		UserPacket_Thread();
		if (Data1 && data1_flag == 0) {
			data1_flag = 1;
			led_cnt = Data1;
			Data1 = 0;
		}
		if(data1_flag){
			if (sec1ms1 > 300) {

				if (led_cnt) {
					--led_cnt;
					HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				}
				else {
					data1_flag = 0;
				}
				sec1ms1 = 0;
			}
		}
	}
}

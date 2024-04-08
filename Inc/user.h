/*
 * user.h
 *
 *  Created on: Apr 8, 2024
 *      Author: kccistc
 */

#ifndef INC_USER_H_
#define INC_USER_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "uart2dma.h"

void User_Task1(void);

extern unsigned int sec1ms1, sec1ms2;
extern unsigned int sec1ms_uart_timeout;

extern char str[256];

#endif /* INC_USER_H_ */

/*
 * Bluetooh.h
 *
 *  Created on: Nov 17, 2021
 *      Author: Tunç
 */

#ifndef BLUETOOH_H_
#define BLUETOOH_H_
#define CORE_CM7
#include "Bluetooh.h"
#include "stm32h745xx.h"
#include <stdbool.h>
void init_Bluetooh(USART_TypeDef *USARTx);
bool isConnection_established(void);
unsigned long get_baudRate_bluetooh(void);
void set_Baud_Rate(short baudRateSelection);
short get_Role(void);

#endif /* BLUETOOH_H_ */

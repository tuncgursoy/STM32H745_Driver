/*
 * Gpio.h
 *
 *  Created on: Oct 24, 2021
 *      Author: Tunç
 */

#ifndef GPIO_H_
#define GPIO_H_
#include <stdbool.h>
#include "stm32h745xx.h"
bool enanblePORT(GPIO_TypeDef* Port);
bool GPIO_MODER(GPIO_TypeDef* Port,short Pin,bool bit_0, bool bit_1 );
bool GPIO_BSRR(GPIO_TypeDef* Port,short pin, bool isItReset);
#endif /* GPIO_H_ */

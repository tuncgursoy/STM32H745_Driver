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
void enablePORT(GPIO_TypeDef* Port);
void Set_GPIO_MODER(GPIO_TypeDef* Port,short Pin,short type);
short Get_GPIO_MODER(GPIO_TypeDef* Port,short pin);
void Set_GPIO_OTYPER(GPIO_TypeDef* Port,short pin,bool bit);
bool Get_GPIO_OTYPER(GPIO_TypeDef* Port,short pin);
void Set_GPIO_OSPEEDR(GPIO_TypeDef* Port,short Pin, short type);
short Get_GPIO_OSPEEDR(GPIO_TypeDef* Port,short pin);
void Set_GPIO_PUPDR(GPIO_TypeDef* Port,short Pin, short type);
short Get_GPIO_PUPDR(GPIO_TypeDef* Port,short pin, short type);
bool Get_GPIO_IDR(GPIO_TypeDef* Port,short pin);
void Set_GPIO_ODR(GPIO_TypeDef* Port,short pin,bool wantToON);
bool Get_GPIO_ODR(GPIO_TypeDef* Port,short pin);
void Set_GPIO_BSRR(GPIO_TypeDef* Port,short pin, bool isItReset);
void Set_GPIO_AFR(GPIO_TypeDef* Port,short pin,short AFx);
#endif /* GPIO_H_ */

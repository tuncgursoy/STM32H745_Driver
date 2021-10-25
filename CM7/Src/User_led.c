
/*
 * user_led.c
 *
 *  Created on: Oct 24, 2021
 *      Author: Tunç
 */
#define CORE_CM7 1

#include "Gpio.h"
#include <User_led.h>
#include "stm32h745xx.h"
#include <stdbool.h>

void ld1_init(void)
{
	enanblePORT(GPIOB);
	GPIO_MODER(GPIOB, 0, true,false);
}
void ld2_init(void)
{
	enanblePORT(GPIOE);
	GPIO_MODER(GPIOE, 1, true,false);
}
void ld3_init(void)
{
	enanblePORT(GPIOB);
	GPIO_MODER(GPIOB, 14, true,false);
}
void ld1_on(void)
{
	GPIO_BSRR(GPIOB, 0, false);
}
void ld2_on(void)
{
	GPIO_BSRR(GPIOE, 1, false);
}

void ld3_on(void)
{
	GPIO_BSRR(GPIOB, 14, false);
}
void ld1_off(void)
{
	GPIO_BSRR(GPIOB, 0, true);
}
void ld2_off(void)
{
	GPIO_BSRR(GPIOE, 1, true);
}

void ld3_off(void)
{
	GPIO_BSRR(GPIOB, 14, true);
}



/*
 * Systick.c
 *
 *  Created on: Oct 26, 2021
 *      Author: Tunç
 */
#define CORE_CM7

#include "stm32h745xx.h"
#include "Systick.h"
#define SYSTICK_LOAD_VAL (64000000/100) //because the cpu is 64MHZ
#define CTRL_ENABLE		 (1U<<0)
#define CTRL_CLKSRC		 (1U<<2)
#define CTRL_COUNTFLAG	 (1U<<16)

void systickDelayMs(int delay)
{
	/*Configure systick*/
	/*Reload with number of clocks per millisecond*/
	SysTick->LOAD =SYSTICK_LOAD_VAL;
	/*Clear systick current value register*/
	SysTick->VAL = 0 ;
	/*Enable systick and select clk src*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0 ; i<delay ; i++)
	{
		//Wait until CountFlag is set
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0 );
	}
	SysTick->CTRL = 0 ;
}


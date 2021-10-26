/*
 * Gpio.c
 *
 *  Created on: Oct 24, 2021
 *      Author: Tunç
 */

#define CORE_CM7 1
#include "stm32h745xx.h"
#include "Gpio.h"
#include <stdbool.h>
#define GPIOAEN (1U<<0)
#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)
#define GPIODEN (1U<<3)
#define GPIOEEN (1U<<4)
#define GPIOFEN (1U<<5)
#define GPIOGEN (1U<<6)
#define GPIOHEN (1U<<7)
#define GPIOIEN (1U<<8)
#define GPIOJEN (1U<<9)
#define GPIOKEN (1U<<10)


void enanblePORT(GPIO_TypeDef* Port)
{
	if (Port==GPIOA) {
		RCC->AHB4ENR |= GPIOAEN;
		//Making sure bit is changed
		while(!(RCC->AHB4ENR & GPIOAEN));
	} else if(Port==GPIOB) {
		RCC->AHB4ENR |= GPIOBEN;
		while(!(RCC->AHB4ENR & GPIOBEN));

	}else if(Port==GPIOC) {
		RCC->AHB4ENR |= GPIOCEN;
		while(!(RCC->AHB4ENR & GPIOCEN));

	}else if(Port==GPIOD) {
		RCC->AHB4ENR |= GPIODEN;
		while(!(RCC->AHB4ENR & GPIODEN));

	}else if(Port==GPIOE) {
		RCC->AHB4ENR |= GPIOEEN;
		while(!(RCC->AHB4ENR & GPIOEEN));

	}else if(Port==GPIOF) {
		RCC->AHB4ENR |= GPIOFEN;
		while(!(RCC->AHB4ENR & GPIOFEN));

	}else if(Port==GPIOG) {
		RCC->AHB4ENR |= GPIOGEN;
		while(!(RCC->AHB4ENR & GPIOGEN));

	}else if(Port==GPIOH) {
		RCC->AHB4ENR |= GPIOHEN;
		while(!(RCC->AHB4ENR & GPIOHEN));

	}else if(Port==GPIOI) {
		RCC->AHB4ENR |= GPIOIEN;
		while(!(RCC->AHB4ENR & GPIOIEN));

	}else if(Port==GPIOJ) {
		RCC->AHB4ENR |= GPIOJEN;
		while(!(RCC->AHB4ENR & GPIOJEN));

	}else if(Port==GPIOK) {
		RCC->AHB4ENR |= GPIOKEN;
		while(!(RCC->AHB4ENR & GPIOKEN));

	}else
	{
	}
}
/* Mode
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * type : 0 to 3
 * configure the I/O mode.
 * 0: Input mode
 * 1: General purpose output mode
 * 2: Alternate function mode
 * 3: Analog mode (reset state)
 * Note : If type argument is entered wrong pin configuration is becomes Input mode
 * */
void Set_GPIO_MODER(GPIO_TypeDef* Port,short Pin,short type)
{
	bool bit_0, bit_1 ;
	switch(type)
	{
	case 0 :
		bit_0 = false ;
		bit_1 = false ;
		break ;
	case 1 :
		bit_0 =  true ;
		bit_1 = false ;
		break;
	case 2 :
		bit_0 = false ;
		bit_1 = true  ;
		break;
	case 3 :
		bit_0 = true ;
		bit_1 = true ;
		break;
	default:
		bit_0 = false;
		bit_1 = false;
	}
	if(bit_0)
	{
		Port->MODER |= (1U<<(Pin*2));
	}else
	{
		Port->MODER &= ~(1U<<((Pin*2)));
	}
	if(bit_1)
	{
		Port->MODER |= (1U<<((Pin*2)+1));
	}else
	{
		Port->MODER &= ~(1U<<((Pin*2)+1));
	}
}


/* Mode
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 *
 * Get the I/O mode.
 *
 * Returns ->
 * 0: Input mode
 * 1: General purpose output mode
 * 2: Alternate function mode
 * 3: Analog mode (reset state)
 * */
short Get_GPIO_MODER(GPIO_TypeDef* Port,short pin)
{
	bool bit_0, bit_1 ;
	if ((Port->OSPEEDR & (1U<<(pin*2))))
		{
			bit_0 = true ;
	}else
		{
			bit_0 = false;
		}

	if (Port->OSPEEDR & (1U<<((pin*2)+1)))
		{
			bit_1 = true ;
	}else
		{
			bit_1 = false;
		}
	if(bit_0)
	{
		if(bit_1)
		{
			return 3 ;
		}else
		{
			return 1 ;
		}
	}else
	{
		if(bit_1)
		{
			return 2 ;
		}else
		{
			return 0 ;
		}
	}
}

/* Output Type
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * bit type : Boolean{true(1), false(0)}
 * configure the I/O output type.
 * false: Output push-pull (reset state)
 * true : Output open-drain
 */
void Set_GPIO_OTYPER(GPIO_TypeDef* Port,short pin,bool bit)
{
 if (bit)
 {
	 Port->OTYPER |= (1U<<pin);
 }else
 {
	 Port->OTYPER &= ~(1U<<pin);
 }
}

/* Output Type
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * bit type : Boolean{true(1), false(0)}
 * Get the I/O output type.
 * Returns ->
 * false(0): Output push-pull (reset state)
 * true(1) : Output open-drain
 * */
bool Get_GPIO_OTYPER(GPIO_TypeDef* Port,short pin)
{
	bool bit_0 ;
	if ((Port->OTYPER & (1U<<(pin))))
		{
			bit_0 = true ;
		}else
		{
			bit_0 = false;
		}


	if(bit_0)
	{
		return true ;
	}else
	{
		return false ;
	}
}


/* Output Speed
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * type : 0 to 3
 * Configure the I/O output speed.
 * 0: Low speed
 * 1: Medium speed
 * 2: High speed
 * 3: Very high speed
 * Note : If  type argument is entered wrong pin configuration is becomes Low Speed
 */
void Set_GPIO_OSPEEDR(GPIO_TypeDef* Port,short Pin, short type)
{
	bool bit_0, bit_1 ;
	switch(type)
	{
	case 0 :
		bit_0 = false ;
		bit_1 = false ;
		break ;
	case 1 :
		bit_0 =  true ;
		bit_1 = false ;
		break;
	case 2 :
		bit_0 = false ;
		bit_1 = true  ;
		break;
	case 3 :
		bit_0 = true ;
		bit_1 = true ;
		break;
	default:
		bit_0 = false;
		bit_1 = false;
	}
	if(bit_0)
	{
		Port->OSPEEDR |= (1U<<(Pin*2));
	}else
	{
		Port->OSPEEDR &= ~(1U<<((Pin*2)));
	}
	if(bit_1)
	{
		Port->OSPEEDR |= (1U<<((Pin*2)+1));
	}else
	{
		Port->OSPEEDR &= ~(1U<<((Pin*2)+1));
	}

}



/* Output Speed
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * type : 0 to 3
 * Get the I/O output speed.
 * Returns ->
 * 0: Low speed
 * 1: Medium speed
 * 2: High speed
 * 3: Very high speed
 * Note : If  type argument is entered wrong pin configuration is becomes Low Speed
 */
short Get_GPIO_OSPEEDR(GPIO_TypeDef* Port,short pin)
{
	bool bit_0, bit_1 ;
	if ((Port->OSPEEDR & (1U<<(pin*2))))
		{
			bit_0 = true ;
		}else
		{
			bit_0 = false;
		}

	if (Port->OSPEEDR & (1U<<((pin*2)+1)))
		{
			bit_1 = true ;
		}else
		{
			bit_1 = false;
		}
	if(bit_0)
		{
			if(bit_1)
			{
				return 3 ;
			}else
			{
				return 1 ;
			}
		}else
		{
			if(bit_1)
			{
				return 2 ;
			}else
			{
				return 0 ;
			}
		}

}








/* Pull-up/pull-down
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * type : 0 to 3
 * Configure the I/O pull-up or pull-down.
 * 0: No pull-up, pull-down
 * 1: Pull-up
 * 2: Pull-down
 * 3: Reserved
 * Note : If  type argument is entered wrong pin configuration is becomes Low Speed
 */
void Set_GPIO_PUPDR(GPIO_TypeDef* Port,short Pin, short type)
{
	bool bit_0, bit_1 ;
	switch(type)
	{
	case 0 :
		bit_0 = false ;
		bit_1 = false ;
		break ;
	case 1 :
		bit_0 =  true ;
		bit_1 = false ;
		break;
	case 2 :
		bit_0 = false ;
		bit_1 = true  ;
		break;
	case 3 :
		bit_0 = true ;
		bit_1 = true ;
		break;
	default:
		bit_0 = false;
		bit_1 = false;
	}
	if(bit_0)
	{
		Port->PUPDR |= (1U<<(Pin*2));
	}else
	{
		Port->PUPDR &= ~(1U<<((Pin*2)));
	}
	if(bit_1)
	{
		Port->PUPDR |= (1U<<((Pin*2)+1));
	}else
	{
		Port->PUPDR &= ~(1U<<((Pin*2)+1));
	}
}

/* Pull-up/pull-down
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * type : 0 to 3
 * Get the I/O pull-up or pull-down.
 * Returns ->
 * 0: No pull-up, pull-down
 * 1: Pull-up
 * 2: Pull-down
 * 3: Reserved
 * Note : If  type argument is entered wrong pin configuration is becomes Low Speed
 */
short Get_GPIO_PUPDR(GPIO_TypeDef* Port,short pin, short type)
{
	bool bit_0, bit_1 ;
		if ((Port->PUPDR & (1U<<(pin*2))))
		{
			bit_0 = true ;
		}else
		{
			bit_0 = false;
		}

		if (Port->PUPDR & (1U<<((pin*2)+1)))
		{
			bit_1 = true ;
		}else
		{
			bit_1 = false;
		}
		if(bit_0)
		{
			if(bit_1)
			{
				return 3 ;
			}else
			{
				return 1 ;
			}
		}else
		{
			if(bit_1)
			{
				return 2 ;
			}else
			{
				return 0 ;
			}
		}
}

/*
 *  Input data
 * Pin :  0 to 15
 * contain the input value of the corresponding I/O port.
 *
 * */
bool Get_GPIO_IDR(GPIO_TypeDef* Port,short pin)
{
	return (Port->IDR & pin);
}

/*
 * Output data
 * Pin :  0 to 15
 * Set output data I/O pin
 */
void Set_GPIO_ODR(GPIO_TypeDef* Port,short pin,bool wantToON)
{
	if(wantToON)
	{
		Port->ODR |= (1U<<pin);
	}else
	{
		Port->ODR &= ~(1U<<pin);
	}
}

/*
 * Output data
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * Get output data I/O pin
 */
bool Get_GPIO_ODR(GPIO_TypeDef* Port,short pin)
{
	return (Port->ODR & (1U<<pin));
}


/*
 * Bit set/reset register
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * These bits are write-only. A read to these bits returns the value 0x0000.
 * isReset Port x reset I/O pin
 * isReset Type : boolean{true(1), false(0)}
 * 0: No action on the corresponding ODRx bit
 * 1: Resets/Sets the corresponding ODRx bit
 */
void Set_GPIO_BSRR(GPIO_TypeDef* Port,short pin, bool isItReset)
{
	if(isItReset)
	{
		Port->BSRR |= (1U<<(pin+16));
	}else
	{
		Port->BSRR |= (1U<<(pin));
	}
}


/* Alternate function
 * Configure alternate function I/Os
 * Port:  GPIOA to GPIOK
 * Pin :  0 to 15
 * AFx : 0 to 15
 *0: AF0
 *1: AF1
 *2: AF2
 *3: AF3
 *4: AF4
 *5: AF5
 *6: AF6
 *7: AF7
 *8: AF8
 *9: AF9
 *10: AF10
 *11: AF11
 *12: AF12
 *13: AF13
 *14: AF14
 *15: AF15
 * Note if Afx entered bigger than 15 AFx becomes AF0
 */
void Set_GPIO_AFR(GPIO_TypeDef* Port,short pin,short AFx)
{
	bool bit_0,bit_1,bit_2,bit_3;
	switch(AFx)
	{
	case 0:
		bit_0 = false ;
		bit_1 = false ;
		bit_2 = false ;
		bit_3 = false ;
		break;
	case 1:
		bit_0 = true  ;
		bit_1 = false ;
		bit_2 = false ;
		bit_3 = false ;
		break;
	case 2:
		bit_0 = false ;
		bit_1 = true  ;
		bit_2 = false ;
		bit_3 = false ;
		break;
	case 3:
		bit_0 = true  ;
		bit_1 = true  ;
		bit_2 = false ;
		bit_3 = false ;
		break;
	case 4:
		bit_0 = false ;
		bit_1 = false ;
		bit_2 = true  ;
		bit_3 = false ;
		break;
	case 5:
		bit_0 = true  ;
		bit_1 = false ;
		bit_2 = true  ;
		bit_3 = false ;
		break;
	case 6:
		bit_0 = false  ;
		bit_1 = true ;
		bit_2 = true  ;
		bit_3 = false ;
		break;
	case 7:
		bit_0 = true  ;
		bit_1 = true  ;
		bit_2 = true  ;
		bit_3 = false ;
		break;
	case 8:
		bit_0 = false  ;
		bit_1 = false  ;
		bit_2 = false  ;
		bit_3 = true ;
		break;
	case 9:
		bit_0 = true  ;
		bit_1 = false  ;
		bit_2 = false  ;
		bit_3 = true ;
		break;
	case 10:
		bit_0 = false  ;
		bit_1 = true   ;
		bit_2 = false  ;
		bit_3 = true   ;
		break;
	case 11:
		bit_0 = true  ;
		bit_1 = true   ;
		bit_2 = false  ;
		bit_3 = true   ;
		break;
	case 12:
		bit_0 = false  ;
		bit_1 = false  ;
		bit_2 = true  ;
		bit_3 = true ;
		break;
	case 13:
		bit_0 = true  ;
		bit_1 = false  ;
		bit_2 = true  ;
		bit_3 = true ;
		break;
	case 14:
		bit_0 = false  ;
		bit_1 = true  ;
		bit_2 = true  ;
		bit_3 = true ;
		break;
	case 15:
		bit_0 = true;
		bit_1 = true  ;
		bit_2 = true  ;
		bit_3 = true ;
		break;
	default:
		bit_0 = false ;
		bit_1 = false ;
		bit_2 = false ;
		bit_3 = false ;
		break;

	}
	if(pin<8&& pin >-1)
	{
		if(bit_0)
		{
			Port->AFR[0] |= (1U<<((pin*4)));
		}else
		{
			Port->AFR[0] &= ~(1U<<((pin*4)));
		}

		if(bit_1)
		{
			Port->AFR[0] |= (1U<<((pin*4)+1));
		}else
		{
			Port->AFR[0] &= ~(1U<<((pin*4)+1));
		}

		if(bit_2)
		{
			Port->AFR[0] |= (1U<<((pin*4)+2));
		}else
		{
			Port->AFR[0] &= ~(1U<<((pin*4)+2));
		}
		if(bit_3)
		{
			Port->AFR[0] |= (1U<<((pin*4)+3));
		}else
		{
			Port->AFR[0] &= ~(1U<<((pin*4)+3));
		}
	}else if(pin>7 &&pin<16)
	{
		if(bit_0)
		{
			Port->AFR[1] |= (1U<<(((pin-8)*4)));
		}else
		{
			Port->AFR[1] &= ~(1U<<(((pin-8)*4)));
		}

		if(bit_1)
		{
			Port->AFR[1] |= (1U<<(((pin-8)*4)+1));
		}else
		{
			Port->AFR[1] &= ~(1U<<(((pin-8)*4)+1));
		}

		if(bit_2)
		{
			Port->AFR[1] |= (1U<<(((pin-8)*4)+2));
		}else
		{
			Port->AFR[1] &= ~(1U<<(((pin-8)*4)+2));
		}
		if(bit_3)
		{
			Port->AFR[1] |= (1U<<(((pin-8)*4)+3));
		}else
		{
			Port->AFR[1] &= ~(1U<<(((pin-8)*4)+3));
		}
	}else
	{

	}
}




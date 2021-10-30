/*
 * uart.c
 *
 *  Created on: Oct 27, 2021
 *      Author: Tunç
 */
#define CORE_CM7
#include "stm32h745xx.h"
#include "Uart.h"
#include <stdbool.h>
#include "Gpio.h"
#include "Clock.h"

#define public
#define private static

#define ENRUSART1 	(1U<<4)  //APB2
#define ENRUSART2	(1U<<17) //APB1L
#define ENRUSART3	(1U<<18) //APB1L
#define ENRUSART4	(1U<<19) //APB1L
#define ENRUSART5	(1U<<20) //APB1L
#define ENRUSART6	(1U<<5)  //APB2
#define ENRUSART7	(1U<<30) //APB1L
#define ENRUSART8	(1U<<31) //APB1L


#define ENUE		(1U<<0) // UART Enable bit
#define ENRE		(1U<<2) // Receiver enable
#define ENTE		(1U<<3)	// Transmitter enable


#define ISR_TXE (1U<<7)
#define ISR_RXNE (1U<<5)

#define APB1_CLK get_APB1_Preipheral_Clock();
#define APB2_CLK get_APB2_Preipheral_Clock();
#define UART_BaudRate 4800

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PerihCLK,uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PerihCLK,uint32_t BaudRate);

unsigned long get_PerihCLK(USART_TypeDef *USARTx)
{
	if(USART1==USARTx || USART6 == USARTx)
	{
		return APB2_CLK;
	}else
	{
		return APB1_CLK;
	}
}

private void sampling_mode_and_FIFO (USART_TypeDef *USARTx, bool isitoversamplingby16,bool isit_FIFO)
{
	if(isit_FIFO)
	{
		USARTx->CR1 |= (1U<<29);
		if(!isitoversamplingby16)
		{
			USARTx->CR1 |= (1U<<15);
		}
	}else
	{
		USARTx->CR1 &= ~(1U<<29);
		if(isitoversamplingby16)
		{
			USARTx->CR1 |= (1U<<15);
		}
	}


}


private void enable_USART1(void)
{
	RCC->APB2ENR |= ENRUSART1 ;
}
private void enable_USART2(void)
{
	RCC->APB1LENR |= ENRUSART2 ;
}
private void enable_USART3(void)
{
	RCC->APB1LENR |= ENRUSART3 ;
}
private void enable_USART4(void)
{
	RCC->APB1LENR |= ENRUSART4 ;
}
private void enable_USART5(void)
{
	RCC->APB1LENR |= ENRUSART5 ;
}
private void enable_USART6(void)
{
	RCC->APB2ENR |= ENRUSART6 ;
}
private void enable_USART7(void)
{
	RCC->APB2ENR |= ENRUSART7 ;
}
private void enable_USART8(void)
{
	RCC->APB2ENR |= ENRUSART8 ;
}



private void selectedUART(USART_TypeDef* USART)
{
	if(USART==USART1)
	{
		enable_USART1();
	}else if (USART==USART2)
	{
		enable_USART2();
	}else if (USART==USART2)
	{
		enable_USART2();
	}else if (USART==USART3)
	{
		enable_USART3();
	}else if (USART==UART4)
	{
		enable_USART4();
	}else if (USART==UART5)
	{
		enable_USART5();
	}else if (USART==USART6)
	{
		enable_USART6();
	}else if (USART==UART7)
	{
		enable_USART7();
	}else if (USART==UART8)
	{
		enable_USART8();
	}else {
		//USART is do not exists
		while(1);
	}
}

private void do_bitConfiguration(USART_TypeDef* USART)
{
	USART->CR1 |= ENUE;
	USART->CR1 |= ENRE;
	USART->CR1 |= ENTE;
}

private void enable_UART(GPIO_TypeDef* GPIOx,USART_TypeDef* USART,short TxPin,short RxPin,short AFx)
{
	if(!(USART->CR1 & ENUE))
	{
		selectedUART(USART);
		enablePORT(GPIOx);
		Set_GPIO_MODER(GPIOx, TxPin, 2);//TX
		Set_GPIO_MODER(GPIOx, RxPin, 2); //RX
		Set_GPIO_AFR(GPIOx, TxPin, AFx);
		Set_GPIO_AFR(GPIOx, RxPin, AFx);
		sampling_mode_and_FIFO(USART,false,false);
		uart_set_baudrate(USART,get_PerihCLK(USART),UART_BaudRate);

	}else
	{
		//That UART is already defined
		while(1);
	}
	do_bitConfiguration(USART);
}
private void enable_UART_differentPins(GPIO_TypeDef* GPIOx_TX,GPIO_TypeDef* GPIOx_RX,USART_TypeDef* USART,short TxPin,short RxPin,short AFx_TX,short AFx_RX)
{
	if(!(USART->CR1 & ENUE))
	{
		selectedUART(USART);
		enablePORT(GPIOx_TX);
		enablePORT(GPIOx_RX);
		Set_GPIO_MODER(GPIOx_TX, TxPin, 2);//TX
		Set_GPIO_MODER(GPIOx_RX, RxPin, 2); //RX
		Set_GPIO_AFR(GPIOx_TX, TxPin, AFx_TX);
		Set_GPIO_AFR(GPIOx_RX, RxPin, AFx_RX);
		sampling_mode_and_FIFO(USART,false,false);
		uart_set_baudrate(USART,get_PerihCLK(USART),UART_BaudRate);

	}else
	{
		//That UART is already defined
		while(1);
	}
	do_bitConfiguration(USART);

}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				 PA9  PA10   7
 * 				 PB6  PB7	 7
 * 				 PB14 PB15	 4
 */
public void enable_UART1(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,USART1,TxPin, RxPin, AFx);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				 PA2   PA3   7
 * 				 PD5   PB6	 7
 */
public void enable_UART2(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,USART2,TxPin, RxPin, AFx);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PB10  PB11   7
 * 				PC10  PC11	 7
 * 				PD8   PD9	 7
 */
public void enable_UART3(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,USART3,TxPin, RxPin, AFx);
}


/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PA0   PA1    8
 * 				PA12  PA11	 6
 * 				PB9   PB8	 8
 * 				PC10  PC11	 8
 * 				PD1   PD0	 8
 * 				PH13  PH14	 8
 */
public void enable_UART4(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,UART4,TxPin, RxPin, AFx);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PB6   PB5    14
 * 				PB13  PB12	 14
 * 				PC12  PD2	 8	//Do not use this function for define this pin configuration UARTDifferent Pin Usage function enable_UART5_Different_Pins
 */
public void enable_UART5(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,UART5,TxPin, RxPin, AFx);

}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PC12  PD2	 8
 */
public void enable_UART5_Different_Pins(GPIO_TypeDef* GPIOx_TX,GPIO_TypeDef* GPIOx_RX,short TxPin,short RxPin,short AFx_TX,short AFx_RX)
{
	enable_UART_differentPins(GPIOx_TX, GPIOx_RX, UART5,TxPin, RxPin, AFx_TX, AFx_RX);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PC6   PC7	 7
 * 				PG14  PG9	 7
 */
public void enable_UART6(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,USART6,TxPin, RxPin, AFx);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PA15   PA8	 11
 * 				PB4    PB3	 11
 * 				PE8	   PE7	 7
 * 				PF7	   PF6	 7
 */
public void enable_UART7(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,UART7,TxPin, RxPin, AFx);
}

/*
 * Pin Pairs: || Tx || Rx || AFx
 * 				PE1	   PE0	 8
 * 				PJ8	   PJ9	 8
 */
public void enable_UART8(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx)
{
	enable_UART(GPIOx,UART8,TxPin, RxPin, AFx);
}


char uart_read(USART_TypeDef* USART)
{
	/*Make sure the receive data register is not empty*/
	while(!(USART->ISR & ISR_RXNE));
	return (int)USART->RDR;

}
void uart_write(USART_TypeDef* USART,int ch)
{
	/*Make Sure the transmit data register is empty*/
	//Write to transmit data register
	while(!(USART->ISR & ISR_TXE));
	USART->TDR = (ch & 0xFF);


}

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PerihCLK,uint32_t BaudRate)
		{

	if(((USARTx->CR1 & (1U<<15)) && !(USARTx->CR1 & (1U<<29)))||(!(USARTx->CR1 & (1U<<15)) && (USARTx->CR1 & (1U<<29))))
	{
		USARTx->BRR = compute_uart_bd(PerihCLK,BaudRate)*2;

	}else
	{
		USARTx->BRR = (compute_uart_bd(PerihCLK,BaudRate));

	}



		}
static uint16_t compute_uart_bd(uint32_t PerihCLK,uint32_t BaudRate)
{

		return ((PerihCLK + (BaudRate/2U))/BaudRate);

}

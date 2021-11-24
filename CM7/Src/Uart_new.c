/*
 * Uart_new.c
 *
 *  Created on: Nov 15, 2021
 *      Author: Tunç
 */
#define CORE_CM7
#include "stm32h745xx.h"
#include "Uart.h"
#include <stdbool.h>
#include "Gpio.h"
#include "Systick.h"
#include "User_led.h"
#include "Clock.h"
#include <string.h>
#include <stdlib.h>

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

unsigned long Uart1_Baud_Rate;
unsigned long Uart2_Baud_Rate;
unsigned long Uart3_Baud_Rate;
unsigned long Uart4_Baud_Rate;
unsigned long Uart5_Baud_Rate;
unsigned long Uart6_Baud_Rate;
unsigned long Uart7_Baud_Rate;
unsigned long Uart8_Baud_Rate;

uint8_t* UART1Buffer;
uint8_t* UART2Buffer;
uint8_t* UART3Buffer;
uint8_t* UART4Buffer;
uint8_t* UART5Buffer;
uint8_t* UART6Buffer;
uint8_t* UART7Buffer;
uint8_t* UART8Buffer;


unsigned long size_buffer_rx_UART1= 0;
unsigned long size_buffer_rx_UART2= 0;
unsigned long size_buffer_rx_UART3= 0;
unsigned long size_buffer_rx_UART4= 0;
unsigned long size_buffer_rx_UART5= 0;
unsigned long size_buffer_rx_UART6= 0;
unsigned long size_buffer_rx_UART7= 0;
unsigned long size_buffer_rx_UART8= 0;

unsigned long current_loc_buffer_rx_UART1= 0;
unsigned long current_loc_buffer_rx_UART2= 0;
unsigned long current_loc_buffer_rx_UART3= 0;
unsigned long current_loc_buffer_rx_UART4= 0;
unsigned long current_loc_buffer_rx_UART5= 0;
unsigned long current_loc_buffer_rx_UART6= 0;
unsigned long current_loc_buffer_rx_UART7= 0;
unsigned long current_loc_buffer_rx_UART8= 0;

//----------------------------------
void Uart_init(GPIO_TypeDef* GPIOx,USART_TypeDef *USARTx,short TxPin, short Rxpin, short Afx,unsigned long BaudRate,unsigned long size_of_buffer);
unsigned long get_PerihCLK(USART_TypeDef *USARTx);
void sampling_mode_and_FIFO (USART_TypeDef *USARTx, bool isitoversamplingby16,bool isit_FIFO);
void enable_USART1(void);
void enable_USART2(void);
void enable_USART3(void);
void enable_USART4(void);
void enable_USART5(void);
void enable_USART6(void);
void enable_USART7(void);
void enable_USART8(void);
void selectedUART_enable(USART_TypeDef* USART);
void set_baudRate(USART_TypeDef* USART ,unsigned long baud_rate);
unsigned long get_baudRate(USART_TypeDef* USART);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PerihCLK,uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PerihCLK,uint32_t BaudRate);
void uart_send_bit(USART_TypeDef* USART,int ch);
void uart_send_string(USART_TypeDef* USART, char* string, int size);
char* uart_get_buffer(USART_TypeDef* USART,unsigned long *Size);
char uart_get_buffer_BYTE(USART_TypeDef* USART);
void init_Buffer(USART_TypeDef* USART,unsigned long size);
void UART1_rx_interrupt(USART_TypeDef* USART);
uint8_t* get_Buffer(USART_TypeDef* USARTx);
unsigned long get_current_loc_Buffer (USART_TypeDef *USARTx);
void disable_UART(USART_TypeDef* USARTx);

void increase_Buffer(USART_TypeDef* USARTx);
void decrease_Buffer(USART_TypeDef* USARTx);

void error(char* ErrorName);
void clear_Buffer(uint8_t* Buffer, unsigned long size);




//----------------------------------


/*	UART1
 * Pin Pairs: || Tx || Rx || AFx
 * 				 PA9  PA10   7
 * 				 PB6  PB7	 7
 * 				 PB14 PB15	 4
UART2
 * Pin Pairs: || Tx || Rx || AFx
 * 				 PA2   PA3   7
 * 				 PD5   PB6	 7
UART3
 * Pin Pairs: || Tx || Rx || AFx
 * 				PB10  PB11   7
 * 				PC10  PC11	 7
 * 				PD8   PD9	 7 //St link
UART4
 * Pin Pairs: || Tx || Rx || AFx
 * 				PA0   PA1    8
 * 				PA12  PA11	 6
 * 				PB9   PB8	 8
 * 				PC10  PC11	 8
 * 				PD1   PD0	 8
 * 				PH13  PH14	 8
 UART5
 * Pin Pairs: || Tx || Rx || AFx
 * 				PB6   PB5    14
 * 				PB13  PB12	 14
UART6
 * Pin Pairs: || Tx || Rx || AFx
 * 				PC6   PC7	 7
 * 				PG14  PG9	 7
UART7
 * Pin Pairs: || Tx || Rx || AFx
 * 				PA15   PA8	 11
 * 				PB4    PB3	 11
 * 				PE8	   PE7	 7
 * 				PF7	   PF6	 7
UART8
 * Pin Pairs: || Tx || Rx || AFx
 * 				PE1	   PE0	 8
 * 				PJ8	   PJ9	 8
 */
void Uart_init(GPIO_TypeDef* GPIOx,USART_TypeDef *USARTx,short TxPin, short Rxpin, short Afx,unsigned long BaudRate,unsigned long size_of_buffer)
{
	 if(!(USARTx->CR1 & ENUE))
	 	{
			ld3_init();
			ld2_init();
		 	selectedUART_enable(USARTx);
	 		enablePORT(GPIOx);
	 		Set_GPIO_MODER(GPIOx, TxPin, 2);//TX
	 		Set_GPIO_MODER(GPIOx, Rxpin, 2); //RX
	 		Set_GPIO_AFR(GPIOx, TxPin, Afx);
	 		Set_GPIO_AFR(GPIOx, Rxpin, Afx);
	 		USARTx->CR1 |= RXNEIE;
	 		USARTx->CR3 |= (1U<<12); //disable ORE error
	 		set_baudRate(USARTx, BaudRate);
	 		sampling_mode_and_FIFO(USARTx,false,false);
	 		init_Buffer(USARTx, size_of_buffer);

	 	}else
	 	{
	 		//That UART is already defined
	 		error("UART is already defined");
	 	}
		USARTx->CR1 |= ENUE;
		USARTx->CR1 |= ENRE;
		USARTx->CR1 |= ENTE;
}


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


void sampling_mode_and_FIFO (USART_TypeDef *USARTx, bool isitoversamplingby16,bool isit_FIFO)
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


 void enable_USART1(void)
{
	RCC->APB2ENR |= ENRUSART1 ;
}
 void enable_USART2(void)
{
	RCC->APB1LENR |= ENRUSART2 ;
}
 void enable_USART3(void)
{
	RCC->APB1LENR |= ENRUSART3 ;
}
 void enable_USART4(void)
{
	RCC->APB1LENR |= ENRUSART4 ;
}
 void enable_USART5(void)
{
	RCC->APB1LENR |= ENRUSART5 ;
}
 void enable_USART6(void)
{
	RCC->APB2ENR |= ENRUSART6 ;
}
 void enable_USART7(void)
{
	RCC->APB2ENR |= ENRUSART7 ;
}
 void enable_USART8(void)
{
	RCC->APB2ENR |= ENRUSART8 ;
}


 void selectedUART_enable(USART_TypeDef* USART)
 {
 	if(USART==USART1)
 	{
 		NVIC_EnableIRQ(USART1_IRQn);
 		enable_USART1();
 	}else if (USART==USART2)
 	{
 		NVIC_EnableIRQ(USART2_IRQn);
 		enable_USART2();
 	}else if (USART==USART3)
 	{
 		NVIC_EnableIRQ(USART3_IRQn);
 		enable_USART3();
 	}else if (USART==UART4)
 	{
 		NVIC_EnableIRQ(UART5_IRQn);
 		enable_USART4();
 	}else if (USART==UART5)
 	{
 		NVIC_EnableIRQ(UART5_IRQn);
 		enable_USART5();
 	}else if (USART==USART6)
 	{
 		NVIC_EnableIRQ(USART6_IRQn);
 		enable_USART6();
 	}else if (USART==UART7)
 	{
 		NVIC_EnableIRQ(UART7_IRQn);
 		enable_USART7();
 	}else if (USART==UART8)
 	{
 		NVIC_EnableIRQ(UART8_IRQn);
 		enable_USART8();
 	}else {
 		//USART is do not exists
 		error("USART do not exists");
 	}
 }
 void set_baudRate(USART_TypeDef* USART ,unsigned long baud_rate)
 {
	 uart_set_baudrate(USART, get_PerihCLK(USART), baud_rate);
 }

 unsigned long get_baudRate(USART_TypeDef* USART)
 {
	 if(USART==USART1)
	  	{
		 return Uart1_Baud_Rate;
	  	}else if (USART==USART2)
	  	{
			 return Uart2_Baud_Rate;
	  	}else if (USART==USART3)
	  	{
			 return Uart3_Baud_Rate;
	  	}else if (USART==UART4)
	  	{
			 return Uart4_Baud_Rate;
	  	}else if (USART==UART5)
	  	{
			 return Uart5_Baud_Rate;
	  	}else if (USART==USART6)
	  	{
	  		return Uart6_Baud_Rate;
	  	}else if (USART==UART7)
	  	{
			 return Uart7_Baud_Rate;
	  	}else if (USART==UART8)
	  	{
			 return Uart8_Baud_Rate;
	  	}else {
	  		//USART is do not exists
	  		error("USART do not exists for Get Baud Rate");
	  		return -99 ;
	  	}
 }

 static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PerihCLK,uint32_t BaudRate)
 {
		if(((USARTx->CR1 & (1U<<15)) && !(USARTx->CR1 & (1U<<29)))||(!(USARTx->CR1 & (1U<<15)) && (USARTx->CR1 & (1U<<29))))
		{
			USARTx->BRR = compute_uart_bd(PerihCLK,BaudRate);

		}else
		{
			USARTx->BRR = (compute_uart_bd(PerihCLK,BaudRate)*2);

		}
 }

 static uint16_t compute_uart_bd(uint32_t PerihCLK,uint32_t BaudRate)
 {
	 return ((PerihCLK + (BaudRate/2U))/BaudRate);
 }

 void uart_send_bit(USART_TypeDef* USART,int ch)
 {
		/*Make Sure the transmit data register is empty*/
		//Write to transmit data register
		while(!(USART->ISR & ISR_TXE));
		USART->TDR = (ch & 0xFF);
 }
 void uart_send_string(USART_TypeDef* USART, char* string, int size)
 {
	 for(int i= 0 ; i<size ; i++)
	 {
		 if((size+1)%20==0)
		 {
			 systickDelayMs(10);
		 }
		 ld3_on();
	 	while(!(USART->ISR & ISR_TXE));
	 	USART->TDR = (string[i] & 0xFF );
	 	ld3_off();
	 }
 }

 char* uart_get_buffer(USART_TypeDef* USART,unsigned long *Size)
 {
	 char* temp = malloc(sizeof(char)*get_current_loc_Buffer(USART));
	 unsigned long temp_loc = get_current_loc_Buffer(USART);
	 *Size = get_current_loc_Buffer(USART) ;
	 while(get_current_loc_Buffer(USART)!=0)
	 {
	 	char temp_uart_buffer_byte = uart_get_buffer_BYTE(USART);
	 	if(temp_uart_buffer_byte!=255)
	 	{
			temp[temp_loc-1] =temp_uart_buffer_byte;
			temp_loc--;
	 	}else
	 	{
	 		break ;
	 	}
	 }

	 return temp ;
 }

 char uart_get_buffer_BYTE(USART_TypeDef* USART)
 {
	 unsigned long loc = get_current_loc_Buffer(USART);
	 if(loc !=0)
	 {
		 uint8_t* temp =  get_Buffer(USART);
		 char temp_char = temp[loc-1];
		 temp[loc-1] = '\000';
		 decrease_Buffer(USART);
		 return temp_char ;
	 }else
	 {
		 return 255 ;
	 }
 }

 void UART1_rx_interrupt(USART_TypeDef* USART)
 {
	 	 ld2_on();
	 	 char key= USART->RDR;
	 	uint8_t *temp = UART1Buffer;
	 	temp[current_loc_buffer_rx_UART1]=key ;
	 	current_loc_buffer_rx_UART1++;
	 	ld2_off();
 }
 unsigned long get_current_loc_Buffer (USART_TypeDef *USARTx)
{
						if(USARTx == USART1)
						{
							return current_loc_buffer_rx_UART1  ;
						}else if (USARTx == USART2)
						{
							return current_loc_buffer_rx_UART2 ;
						}else if (USARTx == USART3)
						{
							return current_loc_buffer_rx_UART3 ;
						}else if (USARTx == UART4)
						{
							return current_loc_buffer_rx_UART4;
						}else if(USARTx == UART5)
						{
							return current_loc_buffer_rx_UART5;

						}else if(USARTx == USART6)
						{
							return current_loc_buffer_rx_UART6;
						}else if(USARTx == UART7)
						{
							return current_loc_buffer_rx_UART7 ;
						}else if(USARTx == UART8)
						{
							return current_loc_buffer_rx_UART8 ;
						}else
						{
							error("USART is not Exists Current Loc Buffer");
							return -99 ;
						}
}


void disable_UART(USART_TypeDef* USARTx)
 {
	 USARTx->CR1 &= ~ENUE;
 }
 void init_Buffer(USART_TypeDef* USARTx,unsigned long size)
 {
	 	 	 	 	 	 	 	 if(USARTx == USART1)
		 						{
		 							 UART1Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if (USARTx == USART2)
		 						{
		 							 UART2Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if (USARTx == USART3)
		 						{
		 							 UART3Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if (USARTx == UART4)
		 						{
		 							 UART4Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if(USARTx == UART5)
		 						{
		 							 UART5Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if(USARTx == USART6)
		 						{
		 							 UART6Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if(USARTx == UART7)
		 						{
		 							 UART7Buffer = malloc(sizeof(uint8_t)*size);
		 						}else if(USARTx == UART8)
		 						{
		 							 UART8Buffer = malloc(sizeof(uint8_t)*size);
		 						}
 }

 uint8_t* get_Buffer(USART_TypeDef* USARTx)
 {
	 	 	 	 	 	 	 	 if(USARTx == USART1)
		 						{
		 							return  UART1Buffer;
		 						}else if (USARTx == USART2)
		 						{
		 							return UART2Buffer ;
		 						}else if (USARTx == USART3)
		 						{
		 							return UART3Buffer ;
		 						}else if (USARTx == UART4)
		 						{
		 							return UART4Buffer;
		 						}else if(USARTx == UART5)
		 						{
		 							return UART5Buffer;

		 						}else if(USARTx == USART6)
		 						{
		 							return UART6Buffer;
		 						}else if(USARTx == UART7)
		 						{
		 							return UART7Buffer ;
		 						}else if(USARTx == UART8)
		 						{
		 							return UART8Buffer ;
		 						}else
		 						{
		 							error("Error in get_Buffer because of the USART");
		 							return NULL;
		 						}
 }
 void increase_Buffer(USART_TypeDef* USARTx)
 {
	 	 	 	 	 	 	 	 	 if(USARTx == USART1)
			 						{
			 						 	 current_loc_buffer_rx_UART1++;
			 						}else if (USARTx == USART2)
			 						{
			 						 	 current_loc_buffer_rx_UART2++;
			 						}else if (USARTx == USART3)
			 						{
			 						 	 current_loc_buffer_rx_UART3++;
			 						}else if (USARTx == UART4)
			 						{
			 						 	 current_loc_buffer_rx_UART4++;
			 						}else if(USARTx == UART5)
			 						{
			 						 	 current_loc_buffer_rx_UART5++;
			 						}else if(USARTx == USART6)
			 						{
			 						 	 current_loc_buffer_rx_UART6++;
			 						}else if(USARTx == UART7)
			 						{
			 						 	 current_loc_buffer_rx_UART7++;
			 						}else if(USARTx == UART8)
			 						{
			 						 	 current_loc_buffer_rx_UART8++;
			 						}
 }
 void decrease_Buffer(USART_TypeDef* USARTx)
 {
	 	 	 	 	 	 	 	 	 if(USARTx == USART1)
			 						{
			 						 	 current_loc_buffer_rx_UART1--;
			 						}else if (USARTx == USART2)
			 						{
			 						 	 current_loc_buffer_rx_UART2--;
			 						}else if (USARTx == USART3)
			 						{
			 						 	 current_loc_buffer_rx_UART3--;
			 						}else if (USARTx == UART4)
			 						{
			 						 	 current_loc_buffer_rx_UART4--;
			 						}else if(USARTx == UART5)
			 						{
			 						 	 current_loc_buffer_rx_UART5--;
			 						}else if(USARTx == USART6)
			 						{
			 						 	 current_loc_buffer_rx_UART6--;
			 						}else if(USARTx == UART7)
			 						{
			 						 	 current_loc_buffer_rx_UART7--;
			 						}else if(USARTx == UART8)
			 						{
			 						 	 current_loc_buffer_rx_UART8--;
			 						}
 }


void error(char* ErrorName)
{
	using_as_error_driver_UART(1000);

}
void clear_Buffer(uint8_t* Buffer, unsigned long size)
{
	for(int i = 0 ; i<size;i++)
	{
		Buffer[i]= '\000';
	}
}












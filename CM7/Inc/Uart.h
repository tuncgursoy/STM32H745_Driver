/*
 * uart.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Tunç
 */

#ifndef UART_H_
#define UART_H_

#define RXNEIE		(1U<<5)	// Rx Interrupt
#define ISR_RXNE (1U<<5)

void Uart_init(GPIO_TypeDef* GPIOx,USART_TypeDef *USARTx,short TxPin, short Rxpin, short Afx,unsigned long BaudRate,unsigned long size_of_buffer);
void uart_send_bit(USART_TypeDef* USART,int ch);
void uart_send_string(USART_TypeDef* USART, char* string, int size);
char* uart_get_buffer(USART_TypeDef* USART);
char uart_get_buffer_BYTE(USART_TypeDef* USART);
void UART3_rx_interrupt(USART_TypeDef* USART);
#endif /* UART_H_ */

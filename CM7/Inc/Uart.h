/*
 * uart.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Tunç
 */

#ifndef UART_H_
#define UART_H_


void enable_UART1(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART2(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART3(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART4(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART5(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART5_Different_PortPins(GPIO_TypeDef* GPIOx_TX,GPIO_TypeDef* GPIOx_RX,short TxPin,short RxPin,short AFx_TX,short AFx_RX);
void enable_UART6(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART7(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);
void enable_UART8(GPIO_TypeDef* GPIOx,short TxPin,short RxPin,short AFx);

#endif /* UART_H_ */

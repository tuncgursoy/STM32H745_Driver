#define CORE_CM7
#include "User_led.h"
#include "Systick.h"
#include "stm32h745xx.h"
#include "Clock.h"
#include "Uart.h"
#include <stdlib.h>

#define BufferSize 1024
uint8_t* Buffer ;
unsigned long count;

void clear_arr(uint8_t* Buffer,int size);

int main(void)
{

	SysClockConfig();
	enablePORT(GPIOC);
	Set_GPIO_MODER(GPIOC, 13, 0);
	Uart_init(GPIOD, USART3, 8, 9, 7, 9600, 1024);
	count= 0 ;
	while(1)
	{
		if(Get_GPIO_IDR(GPIOC, 13)==1)
		{

			Buffer = get_Buffer(USART3);
		}
		systickDelayMs(10);

	}
	return 0 ;
}
void USART3_IRQHandler(void)
{
	//Check if RXNE is set
	if((USART3->ISR & ISR_RXNE))
	{
		//Do Something
		UART3_rx_interrupt(USART3);
	}
}
void clear_arr(uint8_t* Buffer,int size)
{
	for(int i = 0 ;i<size ; i++)
	{
		Buffer[i]= '\000';
	}
}

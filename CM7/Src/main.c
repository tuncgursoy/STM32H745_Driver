#define CORE_CM7
#include "User_led.h"
#include "Systick.h"
#include "stm32h745xx.h"
#include "Clock.h"
#include "Bluetooh.h"
#include "Uart.h"
#include <stdlib.h>

#define BufferSize 1024
char* Buffer ;
unsigned long count;

void clear_arr(char* Buffer,int size);

int main(void)
{

	SysClockConfig();
	enablePORT(GPIOC);
		Uart_init(GPIOB, USART1, 6, 7, 7, 9600, 1024);
		init_Bluetooh(USART1);
		char* version = get_Version();
		char* addr = get_BLEAddr();
		char* name = get_Name() ;
		set_Name("Master");
		name = get_Name() ;
		set_Pin("123456");
		short broadcast = get_Broadcast_Interval();
		char* pin = get_Pin();
		short authecation = get_Authentication_type();
		unsigned long Baud_Rate = get_Baud_Rate();
		set_Baud_Rate(0);
		unsigned long size;
		while(1)
		{

			strcpy(Buffer,uart_get_buffer(USART1, &size));
			uart_send_string(USART1, Buffer, size);
			systickDelayMs(50);
		}
		return 0 ;


	return 0 ;
}
void USART1_IRQHandler(void)
{
	//Check if RXNE is set
	if((USART1->ISR & ISR_RXNE))
	{
		//Do Something
		UART1_rx_interrupt(USART1);
	}
}
void clear_arr(char* Buffer,int size)
{
	for(int i = 0 ;i<size ; i++)
	{
		Buffer[i]= '\000';
	}
}

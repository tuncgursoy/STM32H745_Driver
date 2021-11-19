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
	while(1)
	{


		systickDelayMs(10);
	}
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

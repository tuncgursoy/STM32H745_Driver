#define CORE_CM7
#include "User_led.h"
#include "Systick.h"
#include "stm32h745xx.h"
#include "Clock.h"
#include "Uart.h"
uint8_t s ;
int main(void)
{
	SysClockConfig();
	enable_UART3(GPIOD, 8, 9, 7);
	uint8_t s =' ';
	while(1)
	{
		s = uart_read(USART3);
		systickDelayMs(10);
	}
	return 0 ;
}

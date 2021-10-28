#define CORE_CM7
#include "User_led.h"
#include "Systick.h"
#include "stm32h745xx.h"
#include "Clock.h"
int main(void)
{
	SysClockConfig();
	ld1_init();
	ld2_init();
	ld3_init();

	while(1)
	{
		ld1_on();
		systickDelayMs(100);
		ld1_off();
		ld2_on();
		systickDelayMs(100);
		ld2_off();
		ld3_on();
		systickDelayMs(100);
		ld3_off();

	}
	return 0 ;
}

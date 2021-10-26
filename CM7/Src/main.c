#define CORE_CM7
#include "User_led.h"
#include "stm32h745xx.h"

int main(void)
{
	ld1_init();
	ld2_init();
	ld3_init();

	while(1)
	{
		ld1_on();
		for(int i = 0 ; i<200000; i++);
		ld1_off();
		ld2_on();
		for(int i = 0 ; i<200000; i++);
		ld2_off();
		ld3_on();
		for(int i = 0 ; i<200000; i++);
		ld3_off();

	}
	return 0 ;
}

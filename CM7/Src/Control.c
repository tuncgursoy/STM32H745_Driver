/*
 * Control.c
 *
 *  Created on: Oct 24, 2021
 *      Author: Tunç
 */
#include <stdbool.h>
#include "User_led.h"
#include "Control.h"
void checkbool(bool value)
 {
	 if(!value)
	 {
		ld3_init();
		while(1)
		{
			ld3_on();
			for(int i= 0 ;i<100000;i++);
			ld3_off();
			for(int i= 0 ;i<100000;i++);
		}
	 }
 }


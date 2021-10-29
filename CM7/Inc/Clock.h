/*
 * Clock.h
 *
 *  Created on: Oct 28, 2021
 *      Author: Tunç
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void SysClockConfig(void);
unsigned long  get_CPU1_Clock();
unsigned long  get_CPU1_Systick_Clock();
unsigned long  get_CPU2_Clock();
unsigned long  get_CPU2_Systick_Clock();
unsigned long get_AXI_Preipheral_Clock();
unsigned long get_HCLK3_Preipheral_Clock();
unsigned long get_APB3_Preipheral_Clock();
unsigned long get_AHB1_2_Preipheral_Clock();
unsigned long get_APB1_Preipheral_Clock();
unsigned long get_APB1_Timer_Clock();
unsigned long get_APB2_Preipheral_Clock();
unsigned long get_APB2_Timer_Clock();
unsigned long get_APB4_Preiphreal(void);
unsigned long get_APB4_Preiphreal_Clock(void);


#endif /* CLOCK_H_ */

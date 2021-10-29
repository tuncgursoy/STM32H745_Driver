/*
 * Clock.c
 *
 *  Created on: Oct 28, 2021
 *      Author: Tunç
 */
#define CORE_CM7
#include "stm32h745xx.h"
#include <stdbool.h>

#define HSEON	(1U<<16) // HSE clock enable
#define HSERDY	(1U<<17) // HSE clock ready flag

long decimalToBinary(int decimalnum)
{
    long binarynum = 0;
    int rem, temp = 1;

    while (decimalnum!=0)
    {
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}


/*
 * Set and reset by software to select the PLL clock source.
 * These bits can be written only when all PLLs are disabled.
 * In order to save power, when no PLL is used, the value of PLLSRC must be set to 3 .
 * 0: HSI selected as PLL clock (hsi_ck) (default after reset)
 * 1: CSI selected as PLL clock (csi_ck)
 * 2: HSE selected as PLL clock (hse_ck)
 * 3: No clock send to DIVMx divider and PLLs
 *
 */
void PLLSRC_Select(short select)
{
	if(select== 0)
	{
		RCC->PLLCKSELR &= ~(1U<<0);
		RCC->PLLCKSELR &= ~(1U<<1);
	}else if(select ==1)
	{
		RCC->PLLCKSELR |=  (1U<<0);
		RCC->PLLCKSELR &= ~(1U<<1);
	}else if(select == 2)
	{
		RCC->PLLCKSELR &= ~(1U<<0);
		RCC->PLLCKSELR |=  (1U<<1);
	}else
	{
		RCC->PLLCKSELR |=  (1U<<0);
		RCC->PLLCKSELR |=  (1U<<1);
	}
}
/* Prescaler for PLL1
 * Set and cleared by software to configure the prescaler of the PLL1.
 * The hardware does not allow any modification of this prescaler when PLL1 is enabled (PLL1ON = ‘1’).
 * In order to save power when PLL1 is not used, the value of DIVM1 must be set to ’0’.
 * 0: prescaler disabled
 * 1: division by 1 (bypass)
 * 2: division by 2
 * 3: division by 3
 * ...
 * 32: division by 32 (default after reset)
 * ...
 * 63: division by 63
 */
void DIVM1(short prescaler)
{
	long temp = decimalToBinary(prescaler);
	for(int i = 4 ; i<10; i++)
	{
		short s =  ( temp % 10);
		if(s==1)
		{
			RCC->PLLCKSELR |= (1U<<i);
		}else
		{
			RCC->PLLCKSELR &= ~(1U<<i);
		}
		temp /= 10;
	}

}

/* Prescaler for PLL2
 * Set and cleared by software to configure the prescaler of the PLL2.
 * The hardware does not allow any modification of this prescaler when PLL2 is enabled (PLL2ON = ‘1’).
 * In order to save power when PLL2 is not used, the value of DIVM2 must be set to ’0’.
 * 0: prescaler disabled
 * 1: division by 1 (bypass)
 * 2: division by 2
 * 3: division by 3
 * ...
 * 32: division by 32 (default after reset)
 * ...
 * 63: division by 63
 */
void DIVM2(short prescaler)
{
	long temp = decimalToBinary(prescaler);
	for(int i = 12 ; i<18; i++)
	{
		short s =  ( temp % 10);
		if(s==1)
		{
			RCC->PLLCKSELR |= (1U<<i);
		}else
		{
			RCC->PLLCKSELR &= ~(1U<<i);
		}
		temp /= 10;
	}

}

/*
 * Prescaler for PLL3
 * Set and cleared by software to configure the prescaler of the PLL3.
 * The hardware does not allow any modification of this prescaler when PLL3 is enabled (PLL3ON = ‘1’).
 * In order to save power when PLL3 is not used, the value of DIVM3 must be set to ‘0’.
 * 0: prescaler disabled (default after reset)
 * 1: division by 1 (bypass)
 * 2: division by 2
 * 3: division by 3
 * ...
 * 32: division by 32 (default after reset)
 * ...
 * 63: division by 63
 */


/*
 * System clock switch status
 * Set and reset by hardware to indicate which clock source is used as system clock.
 * 0: HSI used as system clock (hsi_ck) (default after reset)
 * 1: CSI used as system clock (csi_ck)
 * 2: HSE used as system clock (hse_ck)
 * 3: PLL1 used as system clock (pll1_p_ck)
 * others: Reserved
 */
void  SystemClockStatus(short CFGR)
{
	RCC->CFGR &= ~(1U<<2);
	bool bit_0, bit_1;
	switch(CFGR)
	{
	case 0:
		bit_0 = 0;
		bit_1 = 0;
		break;
	case 1:
		bit_0 = 1;
		bit_1 = 0;
		break;
	case 2:
		bit_0 = 0;
		bit_1 = 1;
			break;
	case 3:
		bit_0 = 1;
		bit_1 = 1;
			break;
	default:
		bit_0 = 0;
		bit_1 = 0;
			break;

	}


	if(bit_0)
	{
		RCC-> CFGR |= (1U<<0);
	}else
	{
		RCC-> CFGR &= ~(1U<<0);

	}
	if(bit_1)
		{
			RCC-> CFGR |= (1U<<1);
		}else
		{
			RCC-> CFGR &= ~(1U<<1);

		}
		if(bit_1==0){
			if(bit_0==0)
			{
			    while (!(((((RCC->CFGR & (1U<<3))==bit_0) && ((RCC->CFGR & (1U<<4))==bit_1)))));

			}else
			{
			    while (!(((((RCC->CFGR & (1U<<3))==(1U<<3)) && ((RCC->CFGR & (1U<<4))==bit_1)))));

			}
		}else
		{
			if(bit_0==0)
						{
						    while (!(((((RCC->CFGR & (1U<<3))==bit_0) && ((RCC->CFGR & (1U<<4))==(1U<<4))))));

						}else
						{
						    while (!(((((RCC->CFGR & (1U<<3))==(1U<<3)) && ((RCC->CFGR & (1U<<4))==(1U<<4))))));

						}
		}

}



void DIVM3(short prescaler)
{
	long temp = decimalToBinary(prescaler);
	for(int i = 20 ; i<26; i++)
	{
		short s =  ( temp % 10);
		if(s==1)
		{
			RCC->PLLCKSELR |= (1U<<i);
		}else
		{
			RCC->PLLCKSELR &= ~(1U<<i);
		}
		temp /= 10;
	}
}
/*
 *
 * DIVN1 :::
 * Set and reset by software to control the multiplication factor of the VCO.
 * These bits can be written only when the PLL is disabled (PLL1ON = ‘0’ and PLL1RDY = ‘0’).
 * 0x003: DIVN1 = 4
 * 0x004: DIVN1 = 5
 * 0x005: DIVN1 = 6
 * ...
 * 0x080: DIVN1 = 129 (default after reset)
 * ...
 * 0x1FF: DIVN1 = 512
 * Others: wrong configurations
 *
 *
 * DIVP1 :::
 * PLL1 DIVP division factor
 * Set and reset by software to control the frequency of the pll1_p_ck clock.
 * These bits can be written only when the PLL1 is disabled (PLL1ON = ‘0’ and PLL1RDY = ‘0’).
 * Note that odd division factors are not allowed.
 * 0: pll1_p_ck = vco1_ck
 * 1: pll1_p_ck = vco1_ck / 2 (default after reset)
 * 2: Not allowed
 * 3: pll1_p_ck = vco1_ck / 4
 * ...
 * 127: pll1_p_ck = vco1_ck / 128
 *
 * DIVQ1 :::
 * DIVQ1[6:0]: PLL1 DIVQ division factor
 * Set and reset by software to control the frequency of the pll1_q_ck clock.
 * These bits can be written only when the PLL1 is disabled (PLL1ON = ‘0’ and PLL1RDY = ‘0’).
 * 0: pll1_q_ck = vco1_ck
 * 1: pll1_q_ck = vco1_ck / 2 (default after reset)
 * 2: pll1_q_ck = vco1_ck / 3
 * 3: pll1_q_ck = vco1_ck / 4
 * ...
 * 127: pll1_q_ck = vco1_ck / 128
 */
void PLL1DIVR(uint8_t DIVN1, short DIVP1, short DIVQ1, short DIVR1)
{
	if(DIVN1<=0x1FF && DIVN1>=0x000){
	RCC->PLL1DIVR  = DIVN1  ;
	long temp = decimalToBinary(DIVP1);
		for(int i = 9 ; i<16; i++)
		{
			short s =  ( temp % 10);
			if(s==1)
			{
				RCC->PLL1DIVR |= (1U<<i);
			}else
			{
				RCC->PLL1DIVR &= ~(1U<<i);
			}
			temp /= 10;
		}
	temp = decimalToBinary(DIVQ1);
		for(int i = 16; i<23; i++)
		{
			short s =  ( temp % 10);
			if(s==1)
			{
				RCC->PLL1DIVR |= (1U<<i);
			}else
			{
				RCC->PLL1DIVR &= ~(1U<<i);
			}
			temp /= 10;
		}
	temp = decimalToBinary(DIVR1);
		for(int i =24 ; i<31; i++)
		{
			short s =  ( temp % 10);
			if(s==1)
			{
				RCC->PLL1DIVR |= (1U<<i);
			}else
			{
				RCC->PLL1DIVR &= ~(1U<<i);
			}
			temp /= 10;
	}

	}else
	{
		//Wrong Configuration
		while(1);
	}

}
/*
 *  PLL1 input frequency range
 *  Set and reset by software to select the proper reference frequency range used for PLL1.
 *  This bit must be written before enabling the PLL1.
 *  0: The PLL1 input (ref1_ck) clock range frequency is between 1 and 2 MHz (default after reset)
 *  1: The PLL1 input (ref1_ck) clock range frequency is between 2 and 4 MHz
 *  2: The PLL1 input (ref1_ck) clock range frequency is between 4 and 8 MHz
 *  3: The PLL1 input (ref1_ck) clock range frequency is between 8 and 16 MHz
 */
void input_frequency_range (short PLL1RGE)
{
	switch(PLL1RGE)
	{
	case 0 :
		RCC->PLLCFGR &= ~(1U<<2);
		RCC->PLLCFGR &= ~(1U<<3);
		break ;
	case 1 :
		RCC->PLLCFGR |= (1U<<2);
		RCC->PLLCFGR &= ~(1U<<3);
		break;
	case 2 :
		RCC->PLLCFGR |= (1U<<3);
		RCC->PLLCFGR &= ~(1U<<2);
		break;
	case 3 :
		RCC->PLLCFGR |= (1U<<3);
		RCC->PLLCFGR |= (1U<<2);
	default:
		RCC->PLLCFGR &= ~(1U<<2);
		RCC->PLLCFGR &= ~(1U<<3);
	}
}

/*
 *
 * Set and reset by software to select the proper VCO frequency range used for PLL1.
 * These bits must be written before enabling the PLL1.
 * 0: Wide VCO range: 192 to 836 MHz (default after reset)
 * 1: Medium VCO range: 150 to 420 MHz
 */
void PLL1VCOSEL(bool wideOrMedium)
{
	if(wideOrMedium)
	{
		RCC->PLLCFGR |= (1U<<1);
	}else
	{
		RCC->PLLCFGR &= ~(1U<<1);
	}
}
void enable_PLL1_OutputDividers(void)
{
	RCC ->PLLCFGR |= (1U<<16) ;
	RCC ->PLLCFGR |= (1U<<17) ;
	RCC ->PLLCFGR |= (1U<<18) ;
}
void enable_PLL1_FractionalDivider(void)
{
	RCC ->PLLCFGR |= (1U<<0) ;
}
void StartPLL1(void)
{
	RCC->CR |= (1U<<24); //Start PLL1
	while (!(RCC->CR &(1U<<25))); //Wait until ON
}
/*
 *  D1 domain Core prescaler
 * Set and reset by software to control D1 domain CPU clock division factor.
 * Changing this division ratio has an impact on the frequency of CPU1 clock, all bus matrix clocks
 * and CPU2 clock.
 * The clocks are divided by the new prescaler factor. This factor ranges from 1 to 16 periods of the
 * slowest APB clock among rcc_pclk[4:1] after D1CPRE update. The application can check if the
 * new division factor is taken into account by reading back this register.
 * 0: sys_ck not divided (default after reset)
 * 1: sys_ck divided by 2
 * 2: sys_ck divided by 4
 * 3: sys_ck divided by 8
 * 4: sys_ck divided by 16
 * 5: sys_ck divided by 64
 * 6: sys_ck divided by 128
 * 7: sys_ck divided by 256
 * 8: sys_ck divided by 512
 */
void D1CFGR_D1CPRE(short D1CPRE)
{
	switch(D1CPRE)
	{
	case 0 :
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR &= ~(1U<<11);
		break;
	case 1 :
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR |= (1U<<11);
	    break;
	case 2:
		RCC->D1CFGR |= (1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR |= (1U<<11);
	    break;
	case 3:
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR |= (1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR |= (1U<<11);
	    break;
	case 4:
		RCC->D1CFGR |= (1U<<8);
		RCC->D1CFGR |= (1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR |= (1U<<11);
		break;
	case 5 :
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR |= (1U<<10);
		RCC->D1CFGR |= (1U<<11);
		break;
	case 6 :
		RCC->D1CFGR |= (1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR |= (1U<<10);
		RCC->D1CFGR |= (1U<<11);
		break;
	case 7 :
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR |= (1U<<9);
		RCC->D1CFGR |= (1U<<10);
		RCC->D1CFGR |= (1U<<11);
		break;
	case 8 :
		RCC->D1CFGR |= (1U<<8);
		RCC->D1CFGR |= (1U<<9);
		RCC->D1CFGR |= (1U<<10);
		RCC->D1CFGR |= (1U<<11);
		break;
	default:
		RCC->D1CFGR &= ~(1U<<8);
		RCC->D1CFGR &= ~(1U<<9);
		RCC->D1CFGR &= ~(1U<<10);
		RCC->D1CFGR &= ~(1U<<11);
		break;
	}
}




/*
 *  D1 domain AHB prescaler
 * Set and reset by software to control the division factor of rcc_hclk3 and rcc_aclk. Changing this
 * division ratio has an impact on the frequency of all bus matrix clocks and CPU2 clock.
 * 0: rcc_hclk3 = sys_d1cpre_ck (default after reset)
 * 1: rcc_hclk3 = sys_d1cpre_ck / 2
 * 2: rcc_hclk3 = sys_d1cpre_ck / 4
 * 3: rcc_hclk3 = sys_d1cpre_ck / 8
 * 4: rcc_hclk3 = sys_d1cpre_ck / 16
 * 5: rcc_hclk3 = sys_d1cpre_ck / 64
 * 6: rcc_hclk3 = sys_d1cpre_ck / 128
 * 7: rcc_hclk3 = sys_d1cpre_ck / 256
 * 8: rcc_hclk3 = sys_d1cpre_ck / 512
 * Note: The clocks are divided by the new prescaler factor from1 to 16 periods of the slowest APB
 * clock among rcc_pclk[4:1] after HPRE update.
 * Note: Note also that rcc_hclk3 = rcc_aclk.
 */
void D1CFGR_HPRE(short HPRE)
{
	switch(HPRE)
	{
	case 0 :
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR &= ~(1U<<3);
		break;
	case 1 :
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR |= (1U<<3);
	    break;
	case 2:
		RCC->D1CFGR |= (1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR |= (1U<<3);
	    break;
	case 3:
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR |= (1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR |= (1U<<3);
	    break;
	case 4:
		RCC->D1CFGR |= (1U<<0);
		RCC->D1CFGR |= (1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR |= (1U<<3);
		break;
	case 5 :
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR |= (1U<<2);
		RCC->D1CFGR |= (1U<<3);
		break;
	case 6 :
		RCC->D1CFGR |= (1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR |= (1U<<2);
		RCC->D1CFGR |= (1U<<3);
		break;
	case 7 :
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR |= (1U<<1);
		RCC->D1CFGR |= (1U<<2);
		RCC->D1CFGR |= (1U<<3);
		break;
	case 8 :
		RCC->D1CFGR |= (1U<<0);
		RCC->D1CFGR |= (1U<<1);
		RCC->D1CFGR |= (1U<<2);
		RCC->D1CFGR |= (1U<<3);
		break;
	default:
		RCC->D1CFGR &= ~(1U<<0);
		RCC->D1CFGR &= ~(1U<<1);
		RCC->D1CFGR &= ~(1U<<2);
		RCC->D1CFGR &= ~(1U<<3);
		break;
	}
}


/*
 * System clock switch
 * Set and reset by software to select system clock source (sys_ck).
 * Set by hardware in order to:
 * – force the selection of the HSI or CSI (depending on STOPWUCK selection) when leaving a
 * system Stop mode
 * – force the selection of the HSI in case of failure of the HSE when used directly or indirectly
 * as system clock.
 * 0: HSI selected as system clock (hsi_ck) (default after reset)
 * 1: CSI selected as system clock (csi_ck)
 * 2: HSE selected as system clock (hse_ck)
 * 3: PLL1 selected as system clock (pll1_p_ck)
 */
void System_Clock_MUX(short SW)
{
	RCC->CFGR &= ~(1U<<2);
	switch(SW)
	{
	case 0 :
		RCC->CFGR &= ~(1U<<0);
		RCC->CFGR &= ~(1U<<1);
		break;

	case 1 :
		RCC->CFGR |= (1U<<0);
		RCC->CFGR &= ~(1U<<1);
		break;
	case 2 :
		RCC->CFGR &= ~(1U<<0);
		RCC->CFGR |= (1U<<1);
		break;
	case 3 :
		RCC->CFGR |= (1U<<0);
		RCC->CFGR |= (1U<<1);
		break;
	default :
		RCC->CFGR &= ~(1U<<0);
		RCC->CFGR &= ~(1U<<1);
		break;
	}
	switch(SW)
	{
	case 0 :
		while(!(((RCC->CFGR & (1U<<0))==0)&&((RCC->CFGR & (1U<<1))==0)));
		break;
	case 1 :
		while(!(((RCC->CFGR & (1U<<0))==1)&&((RCC->CFGR & (1U<<1))==0)));
		break;
	case 2 :
		while(!(((RCC->CFGR & (1U<<0))==0)&&((RCC->CFGR & (1U<<1))==(1U<<1))));
		break;
	case 3 :
		while(!(((RCC->CFGR & (1U<<0))==1)&&((RCC->CFGR & (1U<<1))==(1U<<1))));
		break;
	default :
		while(!(((RCC->CFGR & (1U<<0))==0)&&((RCC->CFGR & (1U<<1))==0)));
		break;

	}
}
/*
 * D1 domain APB3 prescaler
 * Set and reset by software to control the division factor of rcc_pclk3.
 * The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk3 after D1PPRE
 * write.
 * 0: rcc_pclk3 = rcc_hclk3 (default after reset)
 * 1: rcc_pclk3 = rcc_hclk3 / 2
 * 2: rcc_pclk3 = rcc_hclk3 / 4
 * 3: rcc_pclk3 = rcc_hclk3 / 8
 * 4: rcc_pclk3 = rcc_hclk3 / 16
 */
void D1PPRE(short D1PPRE)
{
	switch (D1PPRE)
	{
	case 0 :
		RCC->D1CFGR &= ~(1U<<4);
		RCC->D1CFGR &= ~(1U<<5);
		RCC->D1CFGR &= ~(1U<<6);
		break;
	case 1 :
		RCC->D1CFGR &= ~(1U<<4);
		RCC->D1CFGR &= ~(1U<<5);
		RCC->D1CFGR |= (1U<<6);
	case 2 :
		RCC->D1CFGR |= (1U<<4);
		RCC->D1CFGR &= ~(1U<<5);
		RCC->D1CFGR |= (1U<<6);
	case 3 :
		RCC->D1CFGR &= ~(1U<<4);
		RCC->D1CFGR |= (1U<<5);
		RCC->D1CFGR |= (1U<<6);
	case 4 :
		RCC->D1CFGR |= (1U<<4);
		RCC->D1CFGR |= (1U<<5);
		RCC->D1CFGR |= (1U<<6);
	default:
		RCC->D1CFGR &= ~(1U<<4);
		RCC->D1CFGR &= ~(1U<<5);
		RCC->D1CFGR &= ~(1U<<6);
		break;
	}
}
/*
 *  D2 domain APB1 prescaler
 *  Set and reset by software to control D2 domain APB1 clock division factor.
 *  The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk1 after D2PPRE1
 *  write.
 *  0: rcc_pclk1 = rcc_hclk1 (default after reset)
 *  1: rcc_pclk1 = rcc_hclk1 / 2
 *  2: rcc_pclk1 = rcc_hclk1 / 4
 *  3: rcc_pclk1 = rcc_hclk1 / 8
 *  4: rcc_pclk1 = rcc_hclk1 / 16
 *  Bits 3:0 Reserved, must be kept at reset value.
 */
void D2PPRE1(short D2PPRE1)
{
	switch (D2PPRE1)
	{
	case 0 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR &= ~(1U<<6);
		break;
	case 1 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 2 :
		RCC->D2CFGR |= (1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 3 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR |= (1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 4 :
		RCC->D2CFGR |= (1U<<4);
		RCC->D2CFGR |= (1U<<5);
		RCC->D2CFGR |= (1U<<6);
	default:
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR &= ~(1U<<6);
		break;
	}
}
/*
 *  D2 domain APB2 prescaler
 * Set and reset by software to control D2 domain APB2 clock division factor.
 * The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk1 after D2PPRE2
 * write.
 * 0: rcc_pclk2 = rcc_hclk1 (default after reset)
 * 1: rcc_pclk2 = rcc_hclk1 / 2
 * 2: rcc_pclk2 = rcc_hclk1 / 4
 * 3: rcc_pclk2 = rcc_hclk1 / 8
 * 4: rcc_pclk2 = rcc_hclk1 / 16
 * Bit 7 Reserved, must be kept at reset value
 */
void D2PPRE2(short D2PPRE2)
{
	switch (D2PPRE2)
	{
	case 0 :
		RCC->D2CFGR &= ~(1U<<8);
		RCC->D2CFGR &= ~(1U<<9);
		RCC->D2CFGR &= ~(1U<<10);
		break;
	case 1 :
		RCC->D2CFGR &= ~(1U<<8);
		RCC->D2CFGR &= ~(1U<<9);
		RCC->D2CFGR |= (1U<<10);
	case 2 :
		RCC->D2CFGR |= (1U<<8);
		RCC->D2CFGR &= ~(1U<<9);
		RCC->D2CFGR |= (1U<<10);
	case 3 :
		RCC->D2CFGR &= ~(1U<<8);
		RCC->D2CFGR |= (1U<<9);
		RCC->D2CFGR |= (1U<<10);
	case 4 :
		RCC->D2CFGR |= (1U<<8);
		RCC->D2CFGR |= (1U<<9);
		RCC->D2CFGR |= (1U<<10);
	default:
		RCC->D2CFGR &= ~(1U<<8);
		RCC->D2CFGR &= ~(1U<<9);
		RCC->D2CFGR &= ~(1U<<10);
		break;
	}
}
/*
 *  D3 domain APB4 prescaler
Set and reset by software to control D3 domain APB4 clock division factor.
The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk4 after D3PPRE
write.
0xx: rcc_pclk4 = rcc_hclk4 (default after reset)
100: rcc_pclk4 = rcc_hclk4 / 2
101: rcc_pclk4 = rcc_hclk4 / 4
110: rcc_pclk4 = rcc_hclk4 / 8
111: rcc_pclk4 = rcc_hclk4 / 16
Bits 3:0 Reserved, must be kept at reset value
 */
void D3PPRE(short D3PPRE)
{
	switch (D3PPRE)
	{
	case 0 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR &= ~(1U<<6);
		break;
	case 1 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 2 :
		RCC->D2CFGR |= (1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 3 :
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR |= (1U<<5);
		RCC->D2CFGR |= (1U<<6);
	case 4 :
		RCC->D2CFGR |= (1U<<4);
		RCC->D2CFGR |= (1U<<5);
		RCC->D2CFGR |= (1U<<6);
	default:
		RCC->D2CFGR &= ~(1U<<4);
		RCC->D2CFGR &= ~(1U<<5);
		RCC->D2CFGR &= ~(1U<<6);
		break;
	}
}

/*
 * PLL Source Mux : HSE
 * DIWM1: /1
 * DIWN1: X120
 * DIWP1: /2
 * DIWQ1: /2
 * DIWR1: /2
 * System Clock Mux : PLLCLK
 * @SystemClock: 480 MHz
 * D1CPRE : /1
 * D1HPRE : /2
 * D1PPRE : /2
 * D2PPRE1 : /2
 * D2PPRE2 : /2
 * D3PPRE : /2
 * @Brief
 * 		CPU1: 480 MHz
 * 		CPU1 Systick : 480 MHz
 * 		CPU2: 240 MHz
 * 		CPU2 Systick : 240 MHz
 * 		AXI Peripheral Clocks: 240MHz
 * 		HCLK3 : 240MHz
 * 		APB3 Peripheral Clocks: 120MHz
 * 		APB1,2 Peripheral Clocks: 240MHz
 * 		APB1 Peripheral Clocks: 120MHz
 * 		APB1 Timer Clocks: 240MHz
 * 		APB2 Peripheral Clocks: 120MHz
 * 		APB2 Timer Clocks: 240MHz
 * 		APB4 Peripheral Clocks: 120MHz
 * 		APB4 Timer Clocks: 240MHz
 */
void SysClockConfig(void)
{
	RCC->CR |= HSEON; //Enable  the HSE Clock
	while(!(RCC->CR & HSERDY)); //Wait until HSE is ready
	//PLLSRC Source Mux
	PLLSRC_Select(2);
	//DIVM1 PPL1
	DIVM1(1);
	PLL1DIVR(120, 1, 1, 1);
	input_frequency_range(2);
	PLL1VCOSEL(0);
	enable_PLL1_OutputDividers();
	enable_PLL1_FractionalDivider();
	StartPLL1();
	D1CFGR_HPRE(1);
	D1CFGR_D1CPRE(0);
	SystemClockStatus(3);
	D1PPRE(1);
	D2PPRE1(1);
	D2PPRE1(1);
	D3PPRE(1);
}
unsigned long  get_SYSCLK()
{
	return 8000000*120/2;
}

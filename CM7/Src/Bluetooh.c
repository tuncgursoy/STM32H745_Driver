/*
 * Bluetooh.c
 *
 *  Created on: Nov 17, 2021
 *      Author: Tunç
 */

#define CORE_CM7
#include "Bluetooh.h"
#include "stm32h745xx.h"
#include "Uart.h"
#include <stdbool.h>
#include "Systick.h"
#include <string.h>
#include <stdlib.h>


static USART_TypeDef* Bluetooh_USART ;

//--------------------------------------
void init_Bluetooh(USART_TypeDef *USARTx);
bool isConnection_established(void);
unsigned long get_baudRate_bluetooh(void);
void set_Baud_Rate(short baudRateSelection);
short get_Role(void);

//---------------------------------
void init_Bluetooh(USART_TypeDef *USARTx)
{
	Bluetooh_USART = USARTx;
}
bool isConnection_established(void)
{
	systickDelayMs(100);
	char* temp ;
	unsigned long size = 0 ;
	char* cmd = "AT";
	short try_count = -1 ;
	//Wait until receive bluetooh responds
	while(size <=2){
	if(size <=2)
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
	temp = uart_get_buffer(Bluetooh_USART, &size);
	try_count++ ;
	if(size <=2)
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
	systickDelayMs(10);
	if(try_count ==1000)
	{
		error("Connection is not Established");
	}
	}
	if(strcmp(temp,"OK"))
	{
		return true ;
	}else
	{
		return false ;
	}
}
unsigned long get_baudRate_bluetooh(void)
{
	systickDelayMs(100);
	char* temp ;
		unsigned long size = 0 ;
		char* cmd = "AT+BAUD\r\n";
		short try_count = -1 ;
		//Wait until receive bluetooh responds
		while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
			size = 0 ;
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
		systickDelayMs(200);
		temp = uart_get_buffer(Bluetooh_USART, &size);
		try_count++ ;
		if(try_count ==1000)
		{
			return false ;
		}
		}
		 char * token = strtok(temp, "\r\n");
		 token = strtok(token, "=");
		 token = strtok(NULL," ");
		 if(token == NULL)
		 {
			 return get_baudRate_bluetooh();
		 }
		 if (!((((char)token[0])>=48)&&(((char)token[0])<=52))){error("Get_Baud Rate not integer");}
		switch((char)*token)
		{
		case 48:
			return 115200;
		case 49:
			return 57600;
		case 50:
			return 38400;
		case 51:
			return 19200;
		case 52:
			return 9600;
		default:
			error("BlueTooh Baud Rate Error");
		}
		return NULL;

}


/*
 * 0 = 115200
 * 1 = 58600
 * 2 = 38400
 * 3 = 19200
 * 4 = 9600*/
void set_Baud_Rate(short baudRateSelection)
{

	systickDelayMs(100);
	if(!(baudRateSelection>-1&& baudRateSelection<5))
		return;
	else
		baudRateSelection+=48;
	char* temp ;
	unsigned long size = 0 ;
	char cmd[10] = "AT+BAUD \r\n";
	cmd[7] = (char)baudRateSelection;
	short try_count = -1 ;
	//Wait until receive bluetooh responds
	while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
	size = 0 ;
	uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
	systickDelayMs(200);
	temp = uart_get_buffer(Bluetooh_USART, &size);
	try_count++ ;
	if(try_count ==1000)
	{
		error("Set BAUD rate is failed");
	}
}
	char * token = strtok(temp, "\r\n");

}

/*
 * 0 SLAVE
 * 1 MASTER
 * 2 Sensor
 * 3 IBeacon
 * 4 WeChat
 *
 * */
short get_Role(void)
{

	systickDelayMs(100);
	char* temp ;
		unsigned long size = 0 ;
		char* cmd = "AT+ROLE\r\n";
		short try_count = -1 ;
		//Wait until receive bluetooh responds
		while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
			size = 0 ;
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
		systickDelayMs(200);
		temp = uart_get_buffer(Bluetooh_USART, &size);
		try_count++ ;
		if(try_count ==1000)
		{
			error("Get Role Failed");
		}
		}
		 char * token = strtok(temp, "\r\n");
		 token = strtok(token, "=");
		 token = strtok(NULL," ");
		 if(token == NULL){ error("Get Baud strtok failed");}
		 if (!((((char)token[0])>=48)&&(((char)token[0])<=52))){error("Get_Baud Rate not integer");}
		switch((char)*token)
		{
		case 48:
			return 0;
		case 49:
			return 1;
		case 50:
			return 2;
		case 51:
			return 3;
		case 52:
			return 4;
		default:
			error("BlueTooh Baud Rate Error");
		}
		return NULL;

}



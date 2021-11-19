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
short get_Role(void);
void set_Role(short Role);
char* get_Version(void);
char* get_BLEAddr(void);
char* get_Name(void);
void set_Name(char* name);
char* get_Pin(void);
void set_Pin(char* Pin);
short get_Broadcast_Interval(void);
void set_Broadcast_Interval(short parameter);
short get_Authentication_type(void);
void set_Authentication_type(short type);
unsigned long get_Baud_Rate(void);
void set_Baud_Rate(short parameter);
char* get_Service_UUID(void);
void set_Service_UUID(char* UUID);
char* get_IBeacon_UUID(void);
void set_IBeacon_UUID(char* UUID);
char* get_IBeacon_Major(void);
void set_IBeacon_Major(char* major);
char* get_IBeacon_Minor(void);
void set_IBeacon_Minor(char* minor);
void reset_BLE(void); // Resets after 500Ms
void sleep_BLE(void);
void start_Scanning(void);
void stop_Scanning(void);
char* connect_Device_info(short parameter);
char* scan_Mac_Addr(short parameter);
char* bind_BLE(char* MAC);
void bind_BLE_info(void);
char* unbind_BLE(void);
char* get_number_of_slaved_can_be_scanned(short parameter);
void disconn_BLE(void);

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
		if(try_count ==100)
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
/*
 * 0 SLAVE
 * 1 MASTER
 * 2 Sensor
 * 3 IBeacon
 * 4 WeChat
 *
 * */
void set_Role(short Role)
{
	systickDelayMs(100);
	if(!(Role>-1 && Role<5) )
		error("Role  Selection not in parameters");
	else
		Role+=48;
	char* temp ;
	unsigned long size = 0 ;
	char cmd[10] = "AT+ROLE \r\n";
	cmd[7] = (char)Role;
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
}

char* get_Version(void)
{
	systickDelayMs(100);
	char* temp ;
	unsigned long size = 0 ;
	char* cmd = "AT+VERSION\r\n";
	short try_count = -1 ;
	while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
		size = 0 ;
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
		systickDelayMs(100);
		temp = uart_get_buffer(Bluetooh_USART, &size);
		try_count++ ;
		if(try_count ==1000)
		{
			error("Get Version is failed");
		}
	}
	 char * token = strtok(temp, "\r\n");
		if(token == NULL)
		 {
			 return get_Version();
		 }else
		 {
			 return token ;
		 }
}

char* get_BLEAddr(void)
{
	systickDelayMs(100);
	char* temp ;
	unsigned long size = 0 ;
	char* cmd = "AT+LADDR\r\n";
	short try_count = -1 ;
	while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
		size = 0 ;
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
		systickDelayMs(100);
		temp = uart_get_buffer(Bluetooh_USART, &size);
		try_count++ ;
		if(try_count ==1000)
		{
			error("Get BLE ADDR is failed");
		}
	}
	 char * token = strtok(temp, "\r\n");
	 token = strtok(token, "=");
	 token = strtok(NULL," ");
		if(token == NULL)
		 {
			 return get_BLEAddr();
		 }else
		 {
			 return token ;
		 }
}
char* get_Name(void)
{
	systickDelayMs(100);
	char* temp ;
	unsigned long size = 0 ;
	char* cmd = "AT+NAME\r\n";
	short try_count = -1 ;
	while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
		size = 0 ;
		uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
		systickDelayMs(100);
		temp = uart_get_buffer(Bluetooh_USART, &size);
		try_count++ ;
		if(try_count ==1000)
		{
			error("Get NAME is failed");
		}
	}
	 char * token = strtok(temp, "\r\n");
	 token = strtok(token, "=");
	 token = strtok(NULL," ");
		if(token == NULL)
		 {
			 return get_Name();
		 }else
		 {
			 return token ;
		 }
}
void set_Name(char* name)
{
	systickDelayMs(100);
		char* temp ;
		unsigned long size = 0 ;
		char* cmd = "AT+NAME";
		cmd = strcat(cmd,name);
		cmd = strcat(cmd,"\r\n");
		short try_count = -1 ;
		while(size <=2 ||(temp[0]=='E'&&temp[1]=='R') ){
			size = 0 ;
			uart_send_string(Bluetooh_USART, cmd, strlen(cmd));
			systickDelayMs(100);
			temp = uart_get_buffer(Bluetooh_USART, &size);
			try_count++ ;
			if(try_count ==1000)
			{
				error("Set NAME is failed");
			}
		}
		 char * token = strtok(temp, "\r\n");
		 token = strtok(token, "=");
		 token = strtok(NULL," ");
			if(token == NULL)
			 {
				 return get_Name();
			 }else
			 {
				 return token ;
			 }
}




unsigned long get_Baud_Rate(void)
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
		error("Baud Rate is not between specifyed parameters");
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



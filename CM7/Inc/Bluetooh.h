/*
 * Bluetooh.h
 *
 *  Created on: Nov 17, 2021
 *      Author: Tunç
 */

#ifndef BLUETOOH_H_
#define BLUETOOH_H_
#define CORE_CM7
#include "Bluetooh.h"
#include "stm32h745xx.h"
#include <stdbool.h>
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

#endif /* BLUETOOH_H_ */

/* 
 * File:   UART.h
 * Author: Reo
 *
 * Created on 2017/01/21, 18:51
 */


#ifndef UART_H
#define	UART_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include "typeDefine.h"

void InitSerial(void);
void putChar(UBYTE c);                  // Writes a character to the serial port
void putString(UBYTE *);
UBYTE getChar(void);
void putCrLf(void);

void putError(void);
void putOk(void);
void NM_waddress(UBYTE, UBYTE, UBYTE);
void TXOBC_waddress(UBYTE, UBYTE, UBYTE);
void sendCommand(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void commandSwitchUART(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);

#endif	/* UART_H */

/* 
 * File:   I2C.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 0:41
 */

#include <xc.h>
#include "Type_define.h"

#ifndef I2C_H
#define	I2C_H

#define I2Cbps 400000
#define I2Cnull 0xFF

void InitI2CMaster(const UDWORD c);
void interruptI2C(void);
void I2CMasterWait(char);                                               
int I2CMasterStart(unsigned char, unsigned char);                                              
int I2CMasterRepeatedStart(unsigned char, unsigned char);                                   
int I2CMasterStop(void);                                               
int I2CMasterWrite(UBYTE);                                        
int I2CMasterRead(UBYTE);  

void WriteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE *);
void WriteOneByteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE);
void WriteCheckByteToEEPROMs(UBYTE,UBYTE,UBYTE,UBYTE);
void ReadDataFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT);
void ReadOneByteToEEProM(UBYTE,UBYTE,UBYTE,UBYTE);
void ReadDataAndDataSizeFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT *);
void commandSwitchI2C(UBYTE , UBYTE , UBYTE *, UBYTE *);


#endif	/* I2C_H */


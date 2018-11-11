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

//#define I2Cbps 400000 //400kbps
#define I2Cbps 100000 //100kbps
#define I2Cnull 0xFF

#define I2C_baud_rate_high 400000  //400kbps
#define I2C_baud_rate_low  100000  //100kbps

#define TEST_DATA_SIZE 4

/*******************************************************************************
*setting
******************************************************************************/ 
void InitI2CMaster(const UDWORD c);
void I2CMasterWait(void);
void I2CMasterStart(void);
void I2CMasterRepeatedStart(void);
void I2CMasterStop(void);


/*******************************************************************************
*Method for EEPROM Write 
******************************************************************************/
void I2CMasterWrite(UBYTE);
UBYTE I2CMasterRead(UBYTE);
void WriteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE *);
void WriteOneByteToEEPROM(UBYTE,UBYTE,UBYTE,UBYTE);
void WriteOneByteToMainAndSubB0EEPROM(UBYTE addressHigh,UBYTE addressLow,UBYTE data);
void WriteLastCommandIdToEEPROM(UBYTE);
void WriteCheckByteToEEPROMs(UBYTE,UBYTE,UBYTE,UBYTE);
void WriteCheckByteToEEPROMsWithDataSize(UBYTE,UBYTE,UBYTE,UBYTE*,UBYTE);

/*******************************************************************************
*Method for EEPROM Read
******************************************************************************/
void ReadDataFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT);
UBYTE ReadEEPROM(UBYTE, UBYTE, UBYTE);
void ReadDataAndDataSizeFromEEPROM(UBYTE ,UBYTE ,UBYTE,UBYTE *,UINT *);
void commandSwitchI2C(UBYTE, UBYTE, UBYTE, UBYTE *, UBYTE, UBYTE *);
void commandSwitchEEPROM(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);

void debugForCommandSwitchI2C(void);        //for debug
void debugForCommandSwitchEEPROM(void); //for debug

#endif	/* I2C_H */


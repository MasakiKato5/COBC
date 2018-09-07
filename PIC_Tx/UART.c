#include <xc.h>
#include "UART.h"
#include "Type_define.h"
#include "EEPROM.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "CW.h"
#include "WDT.h"
#include "pinDefine.h"
#include "CRC16.h"


UINT B0_select;
UINT DownlinkTimes;

void Init_SERIAL(void){
    SPBRG  = 10;                   // boudrate is 1200 bps
    GIE    = 1;
    PEIE   = 1;
    BRGH   = 0;                   	// Fast baudrate
	SYNC   = 0;						// Asynchronous
	SPEN   = 1;						// Enable serial port pins
	CREN   = 1;						// Enable reception
	SREN   = 0;						// No effect
    RCIF   = 0;                     // RX frag Reset
	TXIE   = 0;						// Disble tx interrupts
	RCIE   = 1;						// Enable rx interrupts
	TX9    = 0;						// 8-bit transmission
	RX9    = 0;						// 8-bit reception
	TXEN   = 0;						// Reset transmitter
	TXEN   = 1;						// Enable the transmitter
}

UBYTE getChar(void){                //TODO: add time out feature
    /**/
	if(FERR || OERR) // If over run error, then reset the receiver
	{
        CREN = 0;
        NOP();
        CREN = 1;
    }
	while(!RCIF);
    return RCREG;
}

void putChar(UBYTE byte){
    while(!TXIF);  
	TXREG = byte;
}

UBYTE get3byte(void){                //TODO: add time out feature
    /**/
	if(FERR || OERR) // If over run error, then reset the receiver
	{
        CREN = 0;
        NOP();
        CREN = 1;
    }
	while(RCIF != 0);
    return RCREG;
}



//void putstr(UBYTE *x)
//{
//    while(*x != '\0'){
//        putch(*x);
//        x++;
//    }
//}

//void putcrlf(void){
//    putch('\r');
//    putch('\n');
//}
//
//void put_error(void){
//    putch('E');
//    putch('R');
//    putch('R');
//    putch('O');
//    putch('R');
//    putch('!');
//}
//
//void put_ok(void){
//    putch('O');
//    putch('K');
//    putch('!');
//}

//void NM_waddress(UBYTE NM_wad_header, UBYTE whigh_address, UBYTE wlow_address){
//    putch(NM_wad_header);
//    putch(whigh_address);
//    putch(wlow_address);
//}




/*
void CwDownlink(UBYTE RXDATA[]){
    
}

void FMDownlink(UBYTE RXDATA[]){
    UINT B0_select = (UINT)RXDATA[2] & 0x80;
    UINT DownlinkTimes = (UINT)RXDATA[2] & 0x7F;
    switch(B0_select){
        case 0x00:
            ReadDataFromEEPROM(EEPROM_address,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMCmdData,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                ReadDataFromEEPROM(EEPROM_subaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMCmdData,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
        case 0x80:
            ReadDataFromEEPROM(EEPROM_Maddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
            if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMCmdData,29)){
                EEPROMCmdData[31] = 0x0F;
            }else{
                ReadDataFromEEPROM(EEPROM_subMaddress,RXDATA[3],RXDATA[4], EEPROMCmdData,EEPROMCmdDataLength);
                if(crc16(0,EEPROMCmdData,29) == CRC_check(EEPROMCmdData,29)){
                    EEPROMCmdData[31] = 0x6F;
                }else{
                    EEPROMCmdData[31] = 0xFF;
                }
            }
            break;
    }
    __delay_ms(200);
    FMPTT = 1;
    for(int SendCounter = 0; SendCounter < DownlinkTimes; SendCounter++){
        SendPacket(EEPROMCmdData);
        __delay_ms(300);
    }
}

*/
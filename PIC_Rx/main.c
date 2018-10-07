//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <PIC16LF877A.h>
#include "typeDefine.h"
#include "pinDefine.h"
#include "MPU.h"
#include "time.h"
#include "UART.h"
#include "decodeAX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "EPS.h"
#include "WDT.h"
#include "CRC16.h"

// PIC16F887 Configuration Bit Settings
#pragma config FOSC     = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE     = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE    = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN    = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP      = OFF       // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD      = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT      = OFF       // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP       = OFF       // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//Receive command from TX_PIC
void interrupt InterReceiver(UBYTE *RXDATA, UBYTE COMMAND_SIZE){
    //volatile static int intr_counter;
    if (RCIF == 1) {                              //The USART receive buffer is full
        for (int i = 0; i < COMMAND_SIZE; i++){
            RXDATA[i] = getChar();
            NOP();
        }
        
        RCIF = 0;  //USART Receive Interrupt Flag is reset
        
       //TODO add case RXDATA[0]!=t
        if(crc16(0,RXDATA,6) == checkCRC(RXDATA,6)){
            //TODO:change commands from TX_PIC
            /*
            switch(RXDATA[1]){
                case 0x75:
                    downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
                    break;
                case 0x63:
//                    CwDownLink(RXDATA);
                    break;
                case 0x66:
                    downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6]);
                    break;
                case 0x61:
                    cutWire(RXDATA[2],RXDATA[3]);
                    break;
            }
            */
        }else{
            ///TODO:コマンドCRCダメだった時の処理
        }
    }
}

void main(void) {
    
    /*---Initialization---*/
    /*----------------------------------------------------------------------*/
    InitSerial();
    InitMPU();
    InitI2CMaster(I2Cbps);
    //InitWDT();
    
    LED_WHITE = 1;              //for debugging of init
    __delay_ms(1000);           //wait until 5V is generated by EPS
    
    //PLL setting (common Phase-Locked Loop)
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
    
    LED_WHITE = 0;              //for debugging of PLL setting
    __delay_ms(1000);           //wait for circuit of PLL
    
    while(1){
        
        /*measure the runtime of the getBitLoop*/    //for normal run not needed
        /*------------------------------------------------------------------*/
        //while(1){
            //getbit();
            //debugLEDyellow();   //6us
        //}
        
        /*-------------------------------------------------------------------*/
        //FIXME:[start]for debug to test to change h/l and i/o
//        UBYTE m;
//        UBYTE s; 
//        putChar(0xaa);
//         changeInOut(0x0a, 0b00011001);
//         putChar(TRISA);
//         m = ReadEEPROM(MAIN_EEPROM_ADDRESS, TRISA_addressHigh, TRISA_addressLow);
//         m++;
//         putChar(m);
//         s = ReadEEPROM(SUB_EEPROM_ADDRESS, TRISA_addressHigh, TRISA_addressLow);
//         s++;
//         putChar(s);
//         __delay_ms(1000);
//         
//         putChar(0xbb);
//         changeInOut(0x0b, 0b00100001);
//         putChar(TRISB);
//         __delay_ms(1000); 
//         
//         putChar(0xcc);
//         changeInOut(0x0c, 0b10011001);
//         putChar(TRISC);
//         __delay_ms(1000);   
//         
//         putChar(0xdd);
//         changeInOut(0x0d, 0b00001101);
//         putChar(TRISD);
//         __delay_ms(1000);   
//         
//         putChar(0xee);
//         changeInOut(0x0e, 0b00000111);
//         putChar(TRISE);
//         __delay_ms(1000); 
        //FIXME:[finish]for debug to test to change h/l and i/o
        /*-------------------------------------------------------------------*/
         
         /*-------------------------------------------------------------------*/
        //FIXME:[start]for debug to test to change h/l and i/o
//         putChar(0xaa);
//         changeHighLow(0x0a, 0b00000001);
//         //changeHighLow(0x0a, 0b00000010);
//         putChar(RA0);      
//         __delay_ms(1000);
//         
//         putChar(0xbb);
//        // changeHighLow(0x0b, 0b00010000);
//         changeHighLow(0x0b, 0b00000001);
//         putChar(RB0);      
//         __delay_ms(1000);
//         
//         putChar(0xcc);
//         changeHighLow(0x0c, 0x01);
//         //putChar(PORTC);
//         putChar(RC0);
//         __delay_ms(1000);
//         
//         putChar(0xdd);
//         changeHighLow(0x0d, 0x01);
//         //putChar(PORTD); 
//         putChar(RD0);
//         __delay_ms(1000);   
//         
//         putChar(0xee);
//         changeHighLow(0x0e, 0x01);
//         //putChar(PORTE);
//         putChar(RE0);
//         __delay_ms(1000); 
        //FIXME:[finish]for debug to test to change h/l and i/o
        /*-------------------------------------------------------------------*/
        
        /*-------------------------------------------------------------------*/
        //FIXME:for debug to test switch power supply start
        UBYTE onOff = 0x01;
        UBYTE timeHigh = 0x02;
        UBYTE timeLow = 0x00;
        UBYTE Abyte = 2;
        UINT aint = 2;
        UWORD along = 2;
        UWORD blong = 2000;
        UWORD clong = 2000000;
        
        //__delay_s(2);succeed
        //__delay_s(0x02);succeed
        //_delay(((2)*(_XTAL_FREQ/4.0)));succeed
        //_delay(((0x02)*(_XTAL_FREQ/4.0)));succeed
        //_delay(((2.0)*(_XTAL_FREQ/4.0)));succeed
        putChar(0xaa);
        delay_s(timeHigh);
        putChar(0xBC);  
        delay_ms(blong);
        putChar(0xDC);
        delay_us(clong);
        putChar(0xEC);
        
//        for(UBYTE i=1; i<10; i++){
//            SEP_SW = HIGH;
//            RBF_SW = HIGH;
//            putChar(0x11);
//            __delay_ms(2000); 
//            SEP_SW = LOW;
//            RBF_SW = LOW;
//            putChar(0x22);
//            __delay_ms(2000); 
//        }
//        for(UBYTE i=1; i<5; i++){
//            putChar(i);
//            onOff = 0x01;
//            switchPowerEPS(onOff, timeHigh, timeLow);
//            __delay_ms(1000);
//            onOff = 0x00;
//            switchPowerEPS(onOff, timeHigh, timeLow);
//            __delay_ms(1000);            
//        }   
        for(UBYTE i=1; i<5; i++){
            putChar(i);
            onOff = 0x01;
            timeHigh = 0x02;
            timeLow = 0x00;
            switchPowerEPS(onOff, timeHigh, timeLow);
            putChar(0xCA);
            __delay_ms(2000);
        }   
        putChar(0xbb);
        //FIXME:for debug to test switch power supply finish
        /*-------------------------------------------------------------------*/
        
        
        /*---Receive command data---*/ 
        /*------------------------------------------------------------------*/
        UBYTE *commandData;         //data of uplink command     
        //for information on EEPROM see data sheet: 24LC1025        
        UBYTE B0select;             //control byte B0 of EEPROM
        UBYTE wHighAddress;         //address high byte of EEPROM
        UBYTE wLowAddress;          //address low byte of EEPROM
        UBYTE mainControlByte;      //control byte of main EEPROM
        UBYTE subControlByte;       //control byte of sub EEPROM
        
        commandData = receiveDataPacket();
        B0select = commandData[20];
        wHighAddress = commandData[21];
        wLowAddress = commandData[22];
        mainControlByte = MAIN_EEPROM_ADDRESS | B0select;
        subControlByte = SUB_EEPROM_ADDRESS | B0select;
        
        //printf("%s\r\n", commandData); //used for debugging with computer
                
        LED_WHITE = 1; //debugging receive command from ground station
        __delay_ms(100);
        LED_WHITE = 0;

        
        /*---Write uplink command in EEPROM---*/
        /*------------------------------------------------------------------*/
        WriteToEEPROM(mainControlByte,wHighAddress,wLowAddress,commandData);
        WriteToEEPROM(subControlByte,wHighAddress,wLowAddress,commandData);
        
        
        /*---Send command using UART to OBC and TXCOBC---*/
        /*------------------------------------------------------------------*/
        sendCommand('g', 'u', B0select, wHighAddress, wLowAddress, 0x00);
        
          
        /*---Define if command target is RXCOBC 'R' and read in task target ---*/
        /*------------------------------------------------------------------*/
        if(commandData[0]=='R'){                //command target = PIC_RX
            //Task target
            if(commandData[2] == 'r'){          //task target =  PIC_RX
                // Command type
                switch(commandData[3]){         //Process command type
                case 'm': /*change sattelite mode*/
                    commandSwitchSatMode(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'p': /*power supply*/
                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'n': /*radio unit*/
                    commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'i':/*I2C*/
                    commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'u':/*UART*/
                    commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'w':/*WDT (watch dog timer)*/
                    commandWDT(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                    //TODO: write function directly here or in MPU.c
                    break;
                case 'r':/*internal processing*/
                    commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
                    break;
                default:
                    //TODO: error message
                    break;
                }

            }
        }else{
            //debugging if coomand target is not RXCOBC
            LED_WHITE = 1;  
            __delay_ms(1000);
            LED_WHITE = 0;
        }
        
        
        /*---Old command switch case---*/ //kept for reference can be deleted once new switch code is finished and tested
//        if(commandData[0]=='R'){                //command target = PIC_RX
//            //Task target
//            if(commandData[2] == 'r'){          //task target =  PIC_RX
//                // Command type
//                switch(commandData[3]){
//                case 'E': /*EPS kill*/
//                    Reset_EPS();
//                    __delay_ms(5000);
//                    //以下の数字は初期設定時と変化しているためもう一度定義
//                    //本来なら変化する文字列を他に用意したほうが良いかもしれない
//                    // values for Nprg are changed in setNprg function so they have to be reset
//                    //TODO: make seperate function for set-up
//                    int FMTX_Nprg[5]     =   {8,7,5,0,1};   // Nprg = 87300 = Ftx / 0.05 [436.500MHz]
//                    int CWTX_Nprg[5]     =   {0,1,4,0,0};   // Nprg = 1747(* see 301ACWPLL-20080520.pdf *) [436.750MHz]
//                    int FMRX_Nprg[5]     =   {2,4,9,1,6};   // Nprg = 24887 = (Frx - 21.4) / 0.05 [145.835MHz]
//                    //reset PLL setting (because it gets lost during shutdown)
//                    FMTX(FMTX_Nref, FMTX_Nprg);
//                    CWTX(CWTX_Nref, CWTX_Nprg);
//                    FMRX(FMRX_Nref, FMRX_Nprg);
//                    __delay_ms(500);
//                    break;
//                case 'I':
//                    // I2C mode
//                    break;
//                case '3':
//                    // 
//                    break;
//                case 'N':
//                    // NanoMind
//                    break;
//                case 'T':
//                    // send TXPIC by I2C
//                    break;
//                default:
//                    // error
//                    break;
//                }
//
//            }else if(commandData[2] == 't'){      //task target =  PIC_TX       
//
//            }else if(commandData[2] == 'o'){      //task target =  OBC       
//
//            }else if(commandData[2] == '5'){      //task target =  5R8G       
//
//            }
//        }else{
//            //debugging if coomand target is not RXCOBC
//            LED_WHITE = 1;  
//            __delay_ms(1000);
//            LED_WHITE = 0;
//        }
    
        __delay_ms(500);
    }
    return;
}

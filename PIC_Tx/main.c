//#include <stdio.h>
//#include <stdlib.h>
#include <xc.h>
#include <PIC16F886.h>
#include "UART.h"
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
#include "encode_AX25.h"
#include "I2C.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "WDT.h"
#include "CW.h"
#include "pinDefine.h"
#include "CRC16.h"
#include "ADC.h"

void interrupt InterReceiver(void);

#define COMMAND_SIZE 10;


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

/* PIC16F887 Configuration Bit Settings */
#pragma config FOSC     = HS            // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE     = OFF           // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    = ON            // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    = ON            // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP       = OFF           // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      = OFF           // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    = OFF            // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO     = OFF            // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN    = OFF            // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP      = OFF           // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
#pragma config BOR4V    = BOR40V        // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      = OFF           // Flash Program Memory Self Write Enable bits (Write protection off)


//test_interrupt
//pc-->pic-->pc 
//void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        RXDATA++;
//        putChar('S');
//        RCIF = 0;
//    }
//}

/**/
//test_get EEPROM address
//pc-->pic-->pc 
//void interrupt InterReceiver( void ){
//    UBYTE RXDATA[3];
//    UINT COMMAND_SIZE;
//    COMMAND_SIZE =3;
//    if (RCIF == 1) {
//        for(UINT i=0; i<COMMAND_SIZE; i++){
//            RXDATA[i] =getChar();
//            putChar(RXDATA[i]);
//            NOP();
//        }
//        putChar('s');
//        RCIF = 0;
//    }
//}


void interrupt InterReceiver(void){
    putChar('I');
    
    int commandSize;
    commandSize = 10;
    
    UBYTE RXDATA[10];//array size = commandSize
//    volatile static int intr_counter;

    if (RCIF == 1) {
        for (int i = 0; i < commandSize; i++){
            RXDATA[i] = getChar();
        }
        
        /*----------------------------------------------*/
        //FIXME:[start]debug for test to getChar
        for (int i = 0; i < commandSize; i++){
            putChar(RXDATA[i]);
            NOP();
        }
        //FIXME:[finish]debug for test to getChar
        /*----------------------------------------------*/
        
       //TODO add case RXDATA[0]!=t or g
        UWORD crcResult, crcValue;
        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
        crcResult = crc16(0,RXDATA,8);
        crcValue =  CRC_check(RXDATA,8);
        crcResultHigh = crcResult>>8;
        crcResultLow = crcResult & 0x00FF;
        crcValueHigh = crcValue>>8;
        crcValueLow = crcValue & 0x00FF;
        
        /*----------------------------------------------*/
        //FIXME:[start]debug for test to CRCcheck
        putChar(crcResultHigh);
        putChar(crcResultLow);
        putChar(crcValueHigh);
        putChar(crcValueLow);
        //FIXME:[finish]debug for test to CRCcheck
        /*----------------------------------------------*/
        
        if(crcResult == crcValue){
            putChar('C');
            
            if (RXDATA[0]!='t' && RXDATA[0]!='g' ){
                //RXDATA[0]??申?申??申?申t??申?申??申?申g??申?申����鐃�??申?申??申?申??申?申??申?申??申?申??申?申??申?申��鐃�??申?申??申?申
                //TODO:add error messege
            } else {
                switch(RXDATA[1]){
                    case 0x75:  //'u'
                        putChar('R');                        
                        downlinkReceivedCommand(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5]);
                        break;
                    case 0x63: //'c'
    //                    CwDownLink(RXDATA);
                        break;
                    case 0x66:  //'f'
                        downlinkFMSignal(RXDATA[2],RXDATA[3],RXDATA[4],RXDATA[5],RXDATA[6],RXDATA[7]);
                        break;
                    case 0x61:  //'a'
                        cutWire(RXDATA[2],RXDATA[3]);
                        break;
                    case 0x68: //'h'
                        measureDcDcTemperature(RXDATA[3], RXDATA[4], RXDATA[5]);
                        break;
                        
                }
            }
        }else{
            
            putChar('D');
            //??申?申R??申?申}??申?申??申?申??申?申hCRC??申?申_??申?申??申?申??申?申??申?申??申?申??申?申??申?申??申?申??申?申??申?申??申?申��鐃�??申?申??申?申
            //add error messege
        }
        RCIF = 0;
    }
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter >= 100 ) {
//            intr_counter = 0;
//        }
//         //0.5sec???RB0???????    
//        if ( intr_counter <= 50 || intr_counter > 51) {
////            PORTAbits.RA0 = 1;
//        } else {
////            PORTAbits.RA0 = 0;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
}

/*******************************************************************************
*test for interrupt
******************************************************************************/
//FIXME:[start]test for interrupt
// void interrupt interReceiverTest( void ){
//    UBYTE RXDATA;
//    if (RCIF == 1) {
//        RXDATA = getChar();
//        //RXDATA++;
//        //putChar('G');
//        putChar(RXDATA);
       
//        switch (RXDATA){
//             case 'h':
//                 putChar('H');
//                 HEATER = 1;
//                 for(int i=0;i<300;i++) delay_ms(1000);
//                 HEATER = 0;
//                 break;
//            case 'i':
//                HEATER = 1;
//                for(int i=0;i<60;i++) delay_ms(1000);
//                HEATER = 0;
//                break;
//             case 'c':
//                 putChar('C');
//                 putChar('W');
                
//                 for(int i=0;i<120;i++){
//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(50);
//                 CWKEY = 0;
//                 __delay_ms(50);

//                 CWKEY = 1;
//                 __delay_ms(150);
//                 CWKEY = 0;
//                 __delay_ms(50);
//                 }

//                 putChar('C');
//                 putChar('W');
//                 putChar('2');
//                 break;
//             case 'f':
//                 putChar('F');
//                 putChar('m');
//                 CWKEY = 0;
//         //            __delay_ms(2000);
//                 __delay_ms(300);
//                 UBYTE EEPROMTestData[36];
//                 EEPROMTestData[0] = 'o';
//                 EEPROMTestData[1] = 'r';
//                 EEPROMTestData[2] = 'i';
//                 EEPROMTestData[3] = '1';
//                 for(int i=4;i<36;i++) EEPROMTestData[i] = 'A';
//                 __delay_ms(300);
//                 FMPTT = 1;
//                 SendPacket(EEPROMTestData);
//                 FMPTT = 0;
//                 __delay_ms(300);
//                 break;
                

//        }
//        RCIF = 0;
//    }
// }

/*******************************************************************************
*test for timer
******************************************************************************/  
//void interrupt timer(void){
    
//    if(INTCONbits.TMR0IF){
//        INTCONbits.TMR0IF = 0;
//        TMR0L = 0x00;
//        timer_counter++;
//        constant_timer_counter++;
//    }
//    if(timer_counter >= 62){
//        //  past 1 second
//        increment_globalClock();
//        timer_counter = 0;
//        //sendCanData(&globalClock);
//    }
//    interruptI2C();
//}

//void interrupt intr(void){
//    volatile static int intr_counter;
//    if ( PIR1bits.TMR1IF == 1 ) {
//        TMR1 = TIMER_INTERVAL;  // ?????????
// 
//        intr_counter++;
//        if ( intr_counter >= 100 ) {
//            intr_counter = 0;
//        }
// 
//        // 0.5sec???RB0???????    
//        if ( intr_counter <= 50 || intr_counter > 51) {
//            PORTAbits.RA0 = 1;
//        } else {
//            PORTAbits.RA0 = 0;
//        }
// 
//        PIR1bits.TMR1IF = 0;    // ???????????
//    }   
// 
//    return;
//}


void main(void) {

    __delay_ms(1000);
    /*???申?申??申?申???申?申??申?申???申?申??申?申???申?申??申?申???申?申??申?申???申?申??申?申*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');
    
    
    while(1){
        putChar('m');
        __delay_ms(1000);

        //TODO send pulse to WDT
        //TODO check AD value
        //ADC();
        //TODO send CW command
        
        /*---------------------------------------------------------------*/
        //FIXME:[start]debug for downlink FM signal        
        // FMPTT = 1;
        // UBYTE eDataField[36];
        // for(UBYTE i=0; i<36; i++){
        //     eDataField[i] = i;
        // }
        // SendPacket(eDataField);
        // __delay_ms(2000);
        //  FMPTT = 0;
        //FIXME:[finish]debug for downlink CW signal
        /*---------------------------------------------------------------*/

        /*---------------------------------------------------------------*/
        //FIXME:[start]check for CW signal       
//       __delay_ms(1000);
//        CWKEY = 1;
//        __delay_ms(2000);
//        CWKEY =0;
        //FIXME:[finish]check for  CW signal
        /*---------------------------------------------------------------*/
        
        /*---------------------------------------------------------------*/
        //FIXME:[start]debug for downlink CW signal
        testForCwFunctions();
        //FIXME:[finish]debug for downlink CW signal
        /*---------------------------------------------------------------*/
        
       /*----------------------------------------------------------*/
       //FIXME : method for test to measure ADC start
//       putChar(0xaa);
//       putChar(0xaa);
//       putChar(0xaa);
//       measureAllChanelADC();
//       putChar(0xbb);
//       putChar(0xbb);
//       putChar(0xbb);
       //method for test to measure ADC finish
       /*----------------------------------------------------------*/
       
       /*---------------------------------------------------------------*/
       //FIXME:test for measure battery boltage start
       UBYTE battery_voltage_data[battery_data_size];
       putChar('V');
       putChar('S');
       measureBatteryVoltage(battery_voltage_data);
       for(UBYTE i=0; i<battery_data_size; i++){
           putChar(battery_voltage_data[i]);
       }
       putChar('V');
       putChar('F');
       //FIXME:test for measure battery boltage finish
       /*---------------------------------------------------------------*/
       
    }
    //return;
}

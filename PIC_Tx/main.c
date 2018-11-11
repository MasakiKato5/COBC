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



//test for interrupt
void interrupt interReceiverTest( void ){
   UBYTE RXDATA;
   if (RCIF == 1) {
       RXDATA = getChar();
       //RXDATA++;
       //putChar('G');
       putChar(RXDATA);
       
       switch (RXDATA){
            case 'h':
                putChar('H');
                HEATER = 1;
                for(int i=0;i<300;i++) delay_ms(1000);
                HEATER = 0;
                break;
           case 'i':
               HEATER = 1;
               for(int i=0;i<60;i++) delay_ms(1000);
               HEATER = 0;
               break;
            case 'c':
                putChar('C');
                putChar('W');
                
                for(int i=0;i<120;i++){
                CWKEY = 1;
                __delay_ms(50);
                CWKEY = 0;
                __delay_ms(50);

                CWKEY = 1;
                __delay_ms(50);
                CWKEY = 0;
                __delay_ms(50);

                CWKEY = 1;
                __delay_ms(50);
                CWKEY = 0;
                __delay_ms(50);

                CWKEY = 1;
                __delay_ms(150);
                CWKEY = 0;
                __delay_ms(50);
                }

                putChar('C');
                putChar('W');
                putChar('2');
                break;
            case 'f':
                putChar('F');
                putChar('m');
                CWKEY = 0;
        //            __delay_ms(2000);
                __delay_ms(300);
                UBYTE EEPROMTestData[22];
                EEPROMTestData[0] = 'o';
                EEPROMTestData[1] = 'r';
                EEPROMTestData[2] = 'i';
                EEPROMTestData[3] = '1';
                for(int i=4;i<21;i++) EEPROMTestData[i] = 'A';
                EEPROMTestData[21] = 'B';
                __delay_ms(300);
                FMPTT = 1;
                SendPacket(EEPROMTestData,22);
                FMPTT = 0;
                __delay_ms(300);
                break;
                

       }
       RCIF = 0;
   }
}
    
    
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
    /*??��?��??��?��??��?��??��?��??��?��??��?��*/
    Init_SERIAL();
    Init_MPU();
    InitI2CMaster(I2Cbps);
//    Init_WDT();
    delay_s(TURN_ON_WAIT_TIME);   //wait for PLL satting by RXCOBC and start CW downlink
    putChar('S');
    
    RA1 = 1;
    
    
    while(1){
        /*
        while(1){
            //getbit();
            //debug_ledy();   //6us
            putch('O');
            __delay_ms(500);
        }*/
        putChar('m');
       __delay_ms(10000);
//        FMPTT = 1;
//        __delay_ms(2000);
//        FMPTT = 0;
        //TODO check AD value
        //TODO send CW command
        //TODO send pulse to WDT
        
    }
    //return;
}

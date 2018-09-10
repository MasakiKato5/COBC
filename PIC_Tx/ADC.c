/*
 * File:   main.c
 * Author: Hohmann Britta
 * Revision history: v1.0
 * 
 * Program Description: This Program Allows
 * implements ADC for PIN RA3 of PIC16F886
 * function is checked using the onboard LED
 * 
 * Created on 2018/05/08, 16:54
 */

/*******************************************************************************
*Includes and defines
******************************************************************************/
//#include "PIC16F886_Internal.h"
#include "Type_define.h"
#include "UART.h"
#include "time.h"
#include "ADC.h"
#include <xc.h>
#include <stdio.h>
/*******************************************************************************
* Function: void initMain()
*
* Returns: Nothing
*
* Description: Contains initializations for main
*
* Usage: initMain()
******************************************************************************/
void initADC(){
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    //----------------------
    // Run at 8 MHz //if this value is changed, ADC clock frequency must be adjusted accordingly
    //Clock determined by FOSC in configuration bits
    SCS = 0;
    //Frequency select bits
//    IRCF0 = 1;
//    IRCF1 = 1;
//    IRCF2 = 1;
    //-----------------------
    
    // Set PIN B2 as output
//    TRISBbits.TRISB1 = 0;
    
    
    // Set PIN A2 as analog input
    TRISAbits.TRISA2 = 1;
    ANSELbits.ANS2 = 1;
    // Set PIN A3 as analog input
    TRISAbits.TRISA3 = 1;
    ANSELbits.ANS3 = 1;
    // Set PIN A5 as analog input
    TRISAbits.TRISA5 = 1;
    ANSELbits.ANS4 = 1;
    // Set PIN B1 as analog input
    TRISBbits.TRISB1 = 1;
    ANSELHbits.ANS10 = 1;
    // Set channel select to AN3
//    ADCON0bits.CHS = 0b0011;
//    ADCON0bits.CHS3 = 0;
//    ADCON0bits.CHS2 = 0;
//    ADCON0bits.CHS1 = 1;
//    ADCON0bits.CHS0 = 1;
    // Set ADC reference Voltage
    ADCON1bits.VCFG1 = 0;   //Vref- = Vss
    ADCON1bits.VCFG0 = 0;   //Vref+ = Vdd
    // Set ADC conversion clock source, conversion time is 4us
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS1 = 0;
    //Set interrupt control /// ? Is this really needed???
    PIE1bits.ADIE = 0;  //disable ADC interrupt
    PIR1bits.ADIF = 0;  //ADC has not completed or has not been started 
    // Set result formatting to right justified
    ADCON1bits.ADFM = 1;
    
    // Zero ADRESL and ADRESH
    ADRESL = 0;
    ADRESH = 0;

}
UWORD adc_read(){
    //Turn ADC on
    ADCON0bits.ADON = 1;
    //Sample Channel
    __delay_us(100);
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO_nDONE);
    //give out ADC result
    UWORD value;
    //value = (ADRESH<<8)+ADRESL;
    value = (ADRESH<<8) | ADRESL;
    return(value);
}
void adc_led_Test(UBYTE voltage){
    if(voltage<1000){//Blink two times
        for(UBYTE i=0;i<2;i++){
            PORTBbits.RB1 = 1;
            __delay_ms(300);
            PORTBbits.RB1 = 0;
            __delay_ms(300);
        }
    } 
    else if(voltage<3300){//Blink three times
        for(UBYTE i=0;i<3;i++){
            PORTBbits.RB1 = 1;
            __delay_ms(200);
            PORTBbits.RB1 = 0;
            __delay_ms(200);
        }
    }
    else {
        PORTBbits.RB1 = 1;
        __delay_ms(1000);
    }
}
/*******************************************************************************
* Function: Main
*
* Returns: Nothing
*
 * Description: Program entry point
******************************************************************************/
void ADC() {
    
    putChar('1');
    initADC();
    //Init_SERIAL();
    UWORD adcValue_1;
    UWORD adcValue_2;
    UWORD adcValue_3;
    UWORD adcValue_4;
    //while(1){ 
        putChar('2');
        //Set LED off
        PORTBbits.RB1 = 0;
        ADCON0bits.CHS = 0b0010;
        adcValue_1 = adc_read();
        ADCON0bits.CHS = 0b0011;
        adcValue_2 = adc_read();
        ADCON0bits.CHS = 0b0100;
        adcValue_3 = adc_read();
        ADCON0bits.CHS = 0b1010;
        adcValue_4 = adc_read();
        //Further instructions on what should be done with result
//        float adcVoltage_1;
//        float adcVoltage_2;
//        float adcVoltage_3;
//        float adcVoltage_4;
        UWORD adcVoltage_1;
        UWORD adcVoltage_2;
        UWORD adcVoltage_3;
        UWORD adcVoltage_4;
        
        
        putChar('3');
    //    adcVoltage = adcValue;
//        adcVoltage_1 = ((float)adcValue_1*3300) / 1024 ;
//        adcVoltage_2 = ((float)adcValue_2*3300) / 1024 ;
//        adcVoltage_3 = ((float)adcValue_3*3300) / 1024 ;
//        adcVoltage_4 = ((float)adcValue_4*3300) / 1024 ;
        
        //adcVoltage_1 = adcValue_1*0.064 + 0.0392 ;  TODO :maku siki
        adcVoltage_2 = adcValue_2 * 0.064 + 0.0392 ;
        adcVoltage_3 = adcValue_3 * 0.064 + 0.0392 ;
        adcVoltage_4 = adcValue_4 * 0.064 + 0.0392 ;
    //    printf("%f %f %f %f \r\n",adcVoltage_1,adcVoltage_2,adcVoltage_3,adcVoltage_4);
    //    printf("%d %d %d %d \r\n",adcValue_1,adcValue_2,adcValue_3,adcValue_4);
        putChar(adcVoltage_1);
        putChar(adcVoltage_2);
        putChar(adcVoltage_3);
        putChar(adcVoltage_4);
        //Test functionality of ADC
    //    adc_led_Test(adcVoltage);

        //Update every second
        __delay_ms(1000);
        putChar('4');
        //Set clears if necessary

    //}
return;
}



/* 
 * File:   Initmpu.h
 * Author: Reo
 *
 * Created on 2017/01/21, 20:52
 */
#include <xc.h>  

#ifndef INITMPU_H
#define	INITMPU_H

#include "typeDefine.h"
//#include "pinDefine.h"

#define HIGH 1
#define LOW  0
#define LED_Y_ON() LED_YELLOW = HIGH;
#define LED_Y_OFF() LED_YELLOW = LOW;

/*Initial Operation*/
#define OBC_ALIVE   1
#define OBC_DIED    0
#define OnOff_forCutWIRE    0x01
#define CutWIRE_LONG_highTime  0x09  //LONG:2500ms
#define CutWIRE_LONG_lowTime   0xC4
#define CutWIRE_SHORT_highTime 0x07  //SHORT:2000ms 
#define CutWIRE_SHORT_lowTime  0xD0
#define cuttimes_forCutWIRE 3 
#define EPS_OFF 0x00 //for switchPowerEPS 
#define EPS_ON  0x01 //for switchPowerEPS

void InitMPU(void);

/*---for debug---*/
void LEDOnOff(void);
void debugLEDyellow(void);

UBYTE invertState(UBYTE);

void onOff5R8G(UBYTE, UBYTE, UBYTE);
void onOffOBC(UBYTE, UBYTE, UBYTE);
void onOffWDT(UBYTE, UBYTE, UBYTE);
void switchPowerSpply1pin(UBYTE, UBYTE, UBYTE, UBYTE);

void killEPS(void);
void onEPS(void);
void resetEPS(void);
void switchPowerEPS(UBYTE, UBYTE, UBYTE);
void reviveEPS(UBYTE, UBYTE);

void changeInOut(UINT, UBYTE);
void changeHighLow(UINT, UBYTE);
void changeXtalFrequency(UBYTE);

void commandSwitchPowerSupply(UBYTE, UBYTE, UBYTE, UBYTE);
void commandSwitchIntProcess(UBYTE, UBYTE, UBYTE);

void onNtrxPowerSupplyCIB(UBYTE,UBYTE);
void offNtrxPowerSupplyCIB(void);

#endif /*INITMPU_H*/


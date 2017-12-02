//<<<<<<< HEAD:PIC_Rx/decode_AX25.c
#include <xc.h>
//#include <PIC16F887.h>
#include "UART.h"
#include "Init_MPU.h"
#include "Type_define.h"
#include "time.h"
#include "decode_AX25.h"
#include "encode_AX25.h"

void UART_TX(UINT,UINT);
void waitByte(UINT,UINT);
UINT getbit(void);
void waitFlag(void);
void getdata(void);
void conv_DI_UARTRX(void);
UBYTE reverse_bit8(UBYTE x);
const UINT commandSize = 32;


#define bit_H 0x01
#define bit_L 0x00
#define bit_dH 0x80
#define bit_dL 0x00

static UINT rcv_state = 0;
UBYTE dPacket[50];
UBYTE dData[32];
UINT  dPacketnum = 0;
UBYTE dfcshi, dfcslo;

UINT getbit(void);
void waitFlag(void);
void getdata(void);
UINT fcscheck(void);
void putAX25(void);
void conv_DI_UARTRX(void);

//  bit�T���v�����O�̃^�C�~���O������Ȃ��悤bit���]���Ƃɓ�������v���O����
//  getbitloop�����ƃV�r�A�Ɍ����Ă��邩�璍��(hspan���܂��܂������Ă���)  
UINT getbit(void){
    static UINT oldbit;
    oldbit = FX614_RXD;
    for(UINT i=0;i<getbitloop;i++){
        if(FX614_RXD != oldbit){
            __delay_us(hspan);
            led_yellow= 1- led_yellow;
            return 0;
        }
    }
    return 1;
}

//  Flag�Ń\�t�g�X�P���`&��������v���O����
void waitFlag(void){
    UINT flag_judge;
    UBYTE buf = 0xff;
    rcv_state = 0;
    while(rcv_state < 2){
        while(buf != Flag_AX25){    //�t���O������܂ő҂�
            flag_judge = getbit();
            buf = buf << 1;         //LSB, MSB�ɉ����ăr�b�g�V�t�g�̕����Cbit_H, bit_L��ύX�D
            if(flag_judge == 0){
                buf = buf | bit_L;
            }else{
                buf = buf | bit_H;
            }
        }
        
        //while���������buf�̓t���O�ł͂Ȃ�ucall�̐擪1�o�C�g���D�����͈Ⴄ�ق����ǂ��C������D
        //�r�b�g�l�߂��ꂽ�炻�̕��ԈႦ�ăo�b�t�@�Ɋi�[���Ă��܂�
        while(buf == Flag_AX25){
            for(UINT i=0;i<8;i++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
        }
        if(buf == (mycall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (mycall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (mycall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (mycall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (mycall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (mycall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                            }
                        }else{
                            dPacketnum = 0;
                        }
                    }else{
                        dPacketnum = 0;
                    }
                }else{
                    dPacketnum = 0;
                }
            }else{
                dPacketnum = 0;
            }
        }else{
            dPacketnum = 0;
        }
        /**/
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        dPacket[dPacketnum] = buf;
        dPacketnum ++;
        for(UINT j=0;j<8;j++){
            flag_judge = getbit();
            buf = buf >> 1;
            if(flag_judge == 0){
                buf = buf | bit_dL;
            }else{
                buf = buf | bit_dH;
            }
        }
        if(buf == (ucall[0] << 1)){
            dPacket[dPacketnum] = buf;
            dPacketnum ++;
            for(UINT j=0;j<8;j++){
                flag_judge = getbit();
                buf = buf >> 1;
                if(flag_judge == 0){
                    buf = buf | bit_dL;
                }else{
                    buf = buf | bit_dH;
                }
            }
            if(buf == (ucall[1] << 1)){
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
                for(UINT j=0;j<8;j++){
                    flag_judge = getbit();
                    buf = buf >> 1;
                    if(flag_judge == 0){
                        buf = buf | bit_dL;
                    }else{
                        buf = buf | bit_dH;
                    }
                }
                if(buf == (ucall[2] << 1)){
                    dPacket[dPacketnum] = buf;
                    dPacketnum ++;
                    for(UINT j=0;j<8;j++){
                        flag_judge = getbit();
                        buf = buf >> 1;
                        if(flag_judge == 0){
                            buf = buf | bit_dL;
                        }else{
                            buf = buf | bit_dH;
                        }
                    }
                    if(buf == (ucall[3] << 1)){
                        dPacket[dPacketnum] = buf;
                        dPacketnum ++;
                        for(UINT j=0;j<8;j++){
                            flag_judge = getbit();
                            buf = buf >> 1;
                            if(flag_judge == 0){
                                buf = buf | bit_dL;
                            }else{
                                buf = buf | bit_dH;
                            }
                        }
                        if(buf == (ucall[4] << 1)){
                            dPacket[dPacketnum] = buf;
                            dPacketnum ++;
                            for(UINT j=0;j<8;j++){
                                flag_judge = getbit();
                                buf = buf >> 1;
                                if(flag_judge == 0){
                                    buf = buf | bit_dL;
                                }else{
                                    buf = buf | bit_dH;
                                }
                            }
                            if(buf == (ucall[5] << 1)){
                                dPacket[dPacketnum] = buf;
                                dPacketnum ++;
                                rcv_state ++;
                            }else{
                                dPacketnum = 0;
                                rcv_state = 0;
                            }
                        }else{
                            dPacketnum = 0;
                            rcv_state = 0;
                        }
                    }else{
                        dPacketnum = 0;
                        rcv_state = 0;
                    }
                }else{
                    dPacketnum = 0;
                    rcv_state = 0;
                }
            }else{
                dPacketnum = 0;
                rcv_state = 0;
            }
        }else{
            dPacketnum = 0;
            rcv_state = 0;
        }
    }
}

//  11111�̎��_�~�[�f�[�^����ł���v���O����
/**/
void getdata(void){
    UBYTE buf = 0x00;
    UINT  dstuff = 0;
    UBYTE dammy_data;
    UINT  bufstore_counter = 0;
    UINT  dbitstatus = 0;
    
    while(rcv_state == 2){
        dbitstatus = getbit();
        if(dstuff == 5){
            if(dbitstatus == 1){
                rcv_state ++;
                break;
            }else{
                dstuff = 0;
            }
        }else{
            buf = buf >> 1;
            if(dbitstatus == 0){
                buf = buf | bit_dL;
                dstuff = 0;
            }else{
                buf = buf | bit_dH;
                dstuff ++;
            }
            bufstore_counter ++;
            if(bufstore_counter == 8){  //�f�o�b�N�p�D�{���̓o�b�t�@�ɂ��߂Ă����D
                bufstore_counter = 0;
                dPacket[dPacketnum] = buf;
                dPacketnum ++;
            }
        }
    }
}

UINT fcscheck(void){
    UBYTE bt,byte;//, dfcslo, dfcshi;
    dfcslo = dfcshi = 0xff;
    while(rcv_state == 3){
        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnum��mycall,SSID,ucall,Control,PID,Data,FCS��byte���D-2��FCS���������D
            byte = dPacket[i];
            for(UINT i=0;i<8;i++){
                bt = byte & bit_H;
                #asm
                    BCF 03,0
                    RRF _dfcshi,F
                    RRF _dfcslo,F
                #endasm
                if(((STATUS & bit_H)^bt) == bit_H){
                    dfcshi = dfcshi ^ 0x84;
                    dfcslo = dfcslo ^ 0x08;
                }
                byte = byte >> 1;
            }
        }
        dfcslo = dfcslo ^ 0xff;
        dfcshi = dfcshi ^ 0xff;
        
        //  �f�o�b�N���͂����̓R�����g�A�E�g����ق����ǂ������H
        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnum�Ԗڂ̃o�b�t�@�̓t���O���i�[����Ă��܂��Ă���͂�
            //putAX25();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 1;
        }else{
            //put_error();
            //putcrlf();
            //dPacketnum = 0;
            //rcv_state = 0;
            return 0;
        }
    }
}

void ReceivePacket(void){
    waitFlag();
    getdata();
    fcscheck();
}

UBYTE *ReceivePacket_data(void){
    UINT fcschecker;
    //UBYTE dData[];
    
    waitFlag();
//    led_white = 1;
    getdata();
//    led_white = 0;
    fcschecker = fcscheck();
    
    if(fcschecker == 1){
        for(UINT i=0; i<commandSize; i++){
            dData[i] = dPacket[i+20];
        }
        dPacketnum = 0;
        rcv_state = 0;
        return dData;
    }else{
        dPacketnum = 0;
        rcv_state = 0;
        return 0x00;
    }
}


void putAX25(void){
    for(UINT i=0;i<6;i++){
        dPacket[i] = dPacket[i] >> 1;
        dPacket[i+7] = dPacket[i+7] >> 1;
    }
    for(UINT i=0;i<6;i++){
        putch(dPacket[i]);
    }
    putch('>');
    for(UINT i=0;i<6;i++){
        putch(dPacket[i+7]);
    }
    putch(':');
    for(UINT i=0;i<dPacketnum-18;i++){
        putch(dPacket[i+16]);
    }
    return;
}

//  �f�W�^���C���v�b�g��UARTRX�Ƃ��Ďg����v���O����
void conv_DI_UARTRX(void){
    UBYTE buf_dammy = 0x00;
    UBYTE buf = 0x00;
    
    getbit();
    while(FX614_RXD == 0){  //��X�͊��荞�ݏ����DSTARTbit�ҋ@
        for(UINT i=0;i<8;i++){
            getbit();
            buf_dammy = buf_dammy << 1;
            if(FX614_RXD == 0){
                buf_dammy = buf_dammy | bit_L;
            }else{
                buf_dammy = buf_dammy | bit_H;
            }
        }
        getbit();   //STOPbit�p
        buf = reverse_bit8(buf_dammy);
        putch(buf);
    }
}

//  MNB,LNB�̋t�]��������v���O����
UBYTE reverse_bit8(UBYTE x){
	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
	return (x << 4) | (x >> 4);
}
// =======
//#include <xc.h>
////#include <PIC16F887.h>
//#include "UART.h"
//#include "Init_MPU.h"
//#include "Type_define.h"
//#include "time.h"
//#include "decode_AX25.h"
//#include "encode_AX25.h"
//
//void UART_TX(UINT,UINT);
//void waitByte(UINT,UINT);
//UINT getbit(void);
//void waitFlag(void);
//void getdata(void);
//void conv_DI_UARTRX(void);
//UBYTE reverse_bit8(UBYTE x);
//
//
//#define bit_H 0x01
//#define bit_L 0x00
//#define bit_dH 0x80
//#define bit_dL 0x00
//
//static UINT rcv_state = 0;
//UBYTE dPacket[50];
//UBYTE dData[32];
//UINT  dPacketnum = 0;
//UBYTE dfcshi, dfcslo;
//
//UINT getbit(void);
//void waitFlag(void);
//void getdata(void);
//UINT fcscheck(void);
//void putAX25(void);
//void conv_DI_UARTRX(void);
//
////  bitサンプリングのタイミングがずれな�?ようbit反転ごとに同期するプログラ�?
////  getbitloopが割とシビアに効�?てくるから注�?(hspanもまあまあ利�?てくる)  
//UINT getbit(void){
//    static UINT oldbit;
//    oldbit = FX614_RXD;
//    for(UINT i=0;i<getbitloop;i++){
//        if(FX614_RXD != oldbit){
//            __delay_us(hspan);
//            return 0;
//        }
//    }
//    return 1;
//}
//
////  Flagでソフトスケル�?&同期するプログラ�?
//void waitFlag(void){
//    UINT flag_judge;
//    UBYTE buf = 0xff;
//    rcv_state = 0;
//    while(rcv_state < 2){
//        while(buf != Flag_AX25){    //wait for flag
//            flag_judge = getbit();
//            buf = buf << 1;         //LSB, MSBに応じてビットシフトの方向，bit_H, bit_Lを変更?�?
//            if(flag_judge == 0){
//                buf = buf | bit_L;
//            }else{
//                buf = buf | bit_H;
//            }
//        }
//        
//        //while抜けた後�?�bufはフラグではなくucallの先�?�1バイト�??��やり方は違うほ�?が良�?気がする?�?
//        //ビット詰めされたらその�?間違えてバッファに格納してしま�?
//        while(buf == Flag_AX25){
//            for(UINT i=0;i<8;i++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//        }
//        if(buf == (mycall[0] << 1)){
//            dPacket[dPacketnum] = buf;
//            dPacketnum ++;
//            for(UINT j=0;j<8;j++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//            if(buf == (mycall[1] << 1)){
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//                for(UINT j=0;j<8;j++){
//                    flag_judge = getbit();
//                    buf = buf >> 1;
//                    if(flag_judge == 0){
//                        buf = buf | bit_dL;
//                    }else{
//                        buf = buf | bit_dH;
//                    }
//                }
//                if(buf == (mycall[2] << 1)){
//                    dPacket[dPacketnum] = buf;
//                    dPacketnum ++;
//                    for(UINT j=0;j<8;j++){
//                        flag_judge = getbit();
//                        buf = buf >> 1;
//                        if(flag_judge == 0){
//                            buf = buf | bit_dL;
//                        }else{
//                            buf = buf | bit_dH;
//                        }
//                    }
//                    if(buf == (mycall[3] << 1)){
//                        dPacket[dPacketnum] = buf;
//                        dPacketnum ++;
//                        for(UINT j=0;j<8;j++){
//                            flag_judge = getbit();
//                            buf = buf >> 1;
//                            if(flag_judge == 0){
//                                buf = buf | bit_dL;
//                            }else{
//                                buf = buf | bit_dH;
//                            }
//                        }
//                        if(buf == (mycall[4] << 1)){
//                            dPacket[dPacketnum] = buf;
//                            dPacketnum ++;
//                            for(UINT j=0;j<8;j++){
//                                flag_judge = getbit();
//                                buf = buf >> 1;
//                                if(flag_judge == 0){
//                                    buf = buf | bit_dL;
//                                }else{
//                                    buf = buf | bit_dH;
//                                }
//                            }
//                            if(buf == (mycall[5] << 1)){
//                                dPacket[dPacketnum] = buf;
//                                dPacketnum ++;
//                                rcv_state ++;
//                            }else{
//                                dPacketnum = 0;
//                            }
//                        }else{
//                            dPacketnum = 0;
//                        }
//                    }else{
//                        dPacketnum = 0;
//                    }
//                }else{
//                    dPacketnum = 0;
//                }
//            }else{
//                dPacketnum = 0;
//            }
//        }else{
//            dPacketnum = 0;
//        }
//        /**/
//        for(UINT j=0;j<8;j++){
//            flag_judge = getbit();
//            buf = buf >> 1;
//            if(flag_judge == 0){
//                buf = buf | bit_dL;
//            }else{
//                buf = buf | bit_dH;
//            }
//        }
//        dPacket[dPacketnum] = buf;
//        dPacketnum ++;
//        for(UINT j=0;j<8;j++){
//            flag_judge = getbit();
//            buf = buf >> 1;
//            if(flag_judge == 0){
//                buf = buf | bit_dL;
//            }else{
//                buf = buf | bit_dH;
//            }
//        }
//        if(buf == (ucall[0] << 1)){
//            dPacket[dPacketnum] = buf;
//            dPacketnum ++;
//            for(UINT j=0;j<8;j++){
//                flag_judge = getbit();
//                buf = buf >> 1;
//                if(flag_judge == 0){
//                    buf = buf | bit_dL;
//                }else{
//                    buf = buf | bit_dH;
//                }
//            }
//            if(buf == (ucall[1] << 1)){
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//                for(UINT j=0;j<8;j++){
//                    flag_judge = getbit();
//                    buf = buf >> 1;
//                    if(flag_judge == 0){
//                        buf = buf | bit_dL;
//                    }else{
//                        buf = buf | bit_dH;
//                    }
//                }
//                if(buf == (ucall[2] << 1)){
//                    dPacket[dPacketnum] = buf;
//                    dPacketnum ++;
//                    for(UINT j=0;j<8;j++){
//                        flag_judge = getbit();
//                        buf = buf >> 1;
//                        if(flag_judge == 0){
//                            buf = buf | bit_dL;
//                        }else{
//                            buf = buf | bit_dH;
//                        }
//                    }
//                    if(buf == (ucall[3] << 1)){
//                        dPacket[dPacketnum] = buf;
//                        dPacketnum ++;
//                        for(UINT j=0;j<8;j++){
//                            flag_judge = getbit();
//                            buf = buf >> 1;
//                            if(flag_judge == 0){
//                                buf = buf | bit_dL;
//                            }else{
//                                buf = buf | bit_dH;
//                            }
//                        }
//                        if(buf == (ucall[4] << 1)){
//                            dPacket[dPacketnum] = buf;
//                            dPacketnum ++;
//                            for(UINT j=0;j<8;j++){
//                                flag_judge = getbit();
//                                buf = buf >> 1;
//                                if(flag_judge == 0){
//                                    buf = buf | bit_dL;
//                                }else{
//                                    buf = buf | bit_dH;
//                                }
//                            }
//                            if(buf == (ucall[5] << 1)){
//                                dPacket[dPacketnum] = buf;
//                                dPacketnum ++;
//                                rcv_state ++;
//                            }else{
//                                dPacketnum = 0;
//                                rcv_state = 0;
//                            }
//                        }else{
//                            dPacketnum = 0;
//                            rcv_state = 0;
//                        }
//                    }else{
//                        dPacketnum = 0;
//                        rcv_state = 0;
//                    }
//                }else{
//                    dPacketnum = 0;
//                    rcv_state = 0;
//                }
//            }else{
//                dPacketnum = 0;
//                rcv_state = 0;
//            }
//        }else{
//            dPacketnum = 0;
//            rcv_state = 0;
//        }
//    }
//}
//
////  11111の次�?ミ�?��?ータ判定できるプログラ�?
///**/
//void getdata(void){
//    UBYTE buf = 0x00;
//    UINT  dstuff = 0;
//    UBYTE dammy_data;
//    UINT  bufstore_counter = 0;
//    UINT  dbitstatus = 0;
//    
//    while(rcv_state == 2){
//        dbitstatus = getbit();
//        if(dstuff == 5){
//            if(dbitstatus == 1){
//                rcv_state ++;
//                break;
//            }else{
//                dstuff = 0;
//            }
//        }else{
//            buf = buf >> 1;
//            if(dbitstatus == 0){
//                buf = buf | bit_dL;
//                dstuff = 0;
//            }else{
//                buf = buf | bit_dH;
//                dstuff ++;
//            }
//            bufstore_counter ++;
//            if(bufstore_counter == 8){  //�?バック用?��本来はバッファにためて�?く�?
//                bufstore_counter = 0;
//                dPacket[dPacketnum] = buf;
//                dPacketnum ++;
//            }
//        }
//    }
//}
//
//UINT fcscheck(void){
//    UBYTE bt,byte;//, dfcslo, dfcshi;
//    dfcslo = dfcshi = 0xff;
//    while(rcv_state == 3){
//        for(UINT i=0;i<dPacketnum-2;i++){      //dPacketnumはmycall,SSID,ucall,Control,PID,Data,FCSのbyte数?�?-2でFCS�?を除く�?
//            byte = dPacket[i];
//            for(UINT i=0;i<8;i++){
//                bt = byte & bit_H;
//                #asm
//                    BCF 03,0
//                    RRF _dfcshi,F
//                    RRF _dfcslo,F
//                #endasm
//                if(((STATUS & bit_H)^bt) == bit_H){
//                    dfcshi = dfcshi ^ 0x84;
//                    dfcslo = dfcslo ^ 0x08;
//                }
//                byte = byte >> 1;
//            }
//        }
//        dfcslo = dfcslo ^ 0xff;
//        dfcshi = dfcshi ^ 0xff;
//        
//        //  �?バック中はここはコメントアウトするほ�?が良�?かも?�?
//        if((dPacket[dPacketnum-2] == dfcslo) && (dPacket[dPacketnum-1] == dfcshi)){     //dPacketnum番目のバッファはフラグが�?�納されてしまって�?る�?��?
//            //putAX25();
//            //putcrlf();
//            //dPacketnum = 0;
//            //rcv_state = 0;
//            return 1;
//        }else{
//            //put_error();
//            //putcrlf();
//            //dPacketnum = 0;
//            //rcv_state = 0;
//            return 0;
//        }
//    }
//}
//
//void ReceivePacket(void){
//    waitFlag();
//    getdata();
//    fcscheck();
//}
//
//UBYTE *ReceivePacket_data(void){
//    UINT fcschecker;
//    //UBYTE dData[];
//    
//    waitFlag();
//    getdata();
//    fcschecker = fcscheck();
//    
//    if(fcschecker == 1){
//        for(UINT i=4; i<dPacketnum-18; i++){    // i=0 -> i=4 ("ori1"の次から)
//            dData[i] = dPacket[i+16];
//        }
//        dPacketnum = 0;
//        rcv_state = 0;
//        return dData;
//    }else{
//        dPacketnum = 0;
//        rcv_state = 0;
//        return 0x00;
//    }
//}
//
//
//void putAX25(void){
//    for(UINT i=0;i<6;i++){
//        dPacket[i] = dPacket[i] >> 1;
//        dPacket[i+7] = dPacket[i+7] >> 1;
//    }
//    for(UINT i=0;i<6;i++){
//        putch(dPacket[i]);
//    }
//    putch('>');
//    for(UINT i=0;i<6;i++){
//        putch(dPacket[i+7]);
//    }
//    putch(':');
//    for(UINT i=0;i<dPacketnum-18;i++){
//        putch(dPacket[i+16]);
//    }
//    return;
//}
//
////  �?ジタルインプットをUARTRXとして使えるプログラ�?
//void conv_DI_UARTRX(void){
//    UBYTE buf_dammy = 0x00;
//    UBYTE buf = 0x00;
//    
//    getbit();
//    while(FX614_RXD == 0){  //後�??は割り込み処�??��STARTbit�?�?
//        for(UINT i=0;i<8;i++){
//            getbit();
//            buf_dammy = buf_dammy << 1;
//            if(FX614_RXD == 0){
//                buf_dammy = buf_dammy | bit_L;
//            }else{
//                buf_dammy = buf_dammy | bit_H;
//            }
//        }
//        getbit();   //STOPbit?��p
//        buf = reverse_bit8(buf_dammy);
//        putch(buf);
//    }
//}
//
////  MNB,LNBの�?転処�?するプログラ�?
//UBYTE reverse_bit8(UBYTE x){
//	x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
//	x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
//	return (x << 4) | (x >> 4);
//>>>>>>> DH_dev:decode_AX25.c
//}
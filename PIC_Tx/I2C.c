
#include "UART.h"
#include "I2C.h"
#include "Type_define.h"
#include "EEPROM.h"
#include "OkError.h"

#define _XTAL_FREQ 10000000
//UBYTE EEPROMData[16];
//UINT EEPROMDataLength;




int AckCheck;
int CollisionCheck;

/*******************************************************************************
*setting
******************************************************************************/
void InitI2CMaster(const UDWORD c){//Init Master Synchronous Serial Port(MSSP)
  SSPCON = 0b00101000;          //MSSP Control Register: Synchronous Serial Port Enabled;I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
  SSPCON2 = 0;                  //MSSP Control Register 2:
  SSPADD = (_XTAL_FREQ/(4*c))-1; //MSSP Address Register: the lower seven bits of SSPADD act as the baud rate generator reload value
  SSPSTAT = 0;                  //MSSP Status Register
  PIE1bits.SSPIE  = 1 ;               // enable MSSP interrupt
  PIE2bits.BCLIE  = 1 ;               // enable bus collision interrupt
  PIR1bits.SSPIF  = 0 ;               // clear MSSP interrupt flag
  PIR2bits.BCLIF  = 0 ;               // clear bus collision flag
}

void interruptI2C(void)
{
     if (PIR1bits.SSPIF == 1) {
          if (AckCheck == 1) {
              AckCheck = 0;
          }
          PIR1bits.SSPIF = 0;
     }
     if (PIR2bits.BCLIF == 1) {
          CollisionCheck = 1;
          PIR2bits.BCLIF = 0;
     }
}

void I2CMasterWait(char mask){
  while ((SSPSTAT & mask) || (SSPCON2 & 0x1F));
  //SSPSTAT : 0x05 -> transmit is not in progress & buffer empty
  //          0x04 -> transmit is not in progress
  //SSPCON2 : ack,receive,start,restart,stop is idle
}


int I2CMasterStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  SEN = 1;                      //SEN Start Condition Enable; bit 0 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.SEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1 ;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1 ;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterRepeatedStart(UBYTE slave_address,UBYTE rw){
//  I2CMasterWait();
//  RSEN = 1;                     //Repeated Start Condition Enabled bit (Master mode only); bit 1 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RSEN = 1 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     AckCheck = 1;
     SSPBUF = (char)((slave_address<<1)+rw);
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}

int I2CMasterStop(void){
//  I2CMasterWait();
//  PEN = 1;                      //Stop Condition Enable bit (Master mode only); bit 2 of SSPCON2
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.PEN = 1 ;
     if (CollisionCheck == 1) return -1 ;
     else                     return  0 ;
}

int I2CMasterWrite(UBYTE dataByte){                               
//  I2CMasterWait();
//  SSPBUF = dataByte;                   //Serial Receive/Transmit Buffer Register
     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1;
     AckCheck = 1;
     SSPBUF = dataByte;
     while (AckCheck);
     if (CollisionCheck == 1) return -1;
     return SSPCON2bits.ACKSTAT;
}


int I2CMasterRead(UBYTE address){
     int data_from_slave ;

     CollisionCheck = 0 ;
     I2CMasterWait(0x5) ;
     SSPCON2bits.RCEN = 1;      //  enable receive from slave
     I2CMasterWait(0x4) ;
     if (CollisionCheck == 1) return -1 ;
     data_from_slave = SSPBUF;
     I2CMasterWait(0x5) ;
     if (CollisionCheck == 1) return -1 ;
     SSPCON2bits.ACKDT = address;
     SSPCON2bits.ACKEN = 1;
     return data_from_slave;
}

int WriteOneByteToEEPROMonce(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    int ans = -1;
    ans = I2CMasterStart(addressEEPROM,0);               //Start condition
    if(ans == 0){
        ans = I2CMasterWrite(addressHigh);              //Adress High Byte
        if(ans != 0) return -1;
        ans = I2CMasterWrite(addressLow);           //Adress Low Byte
        if(ans != 0) return -1;
        ans = I2CMasterWrite(data);             //Data
        if(ans != 0) return -1;
    }else return -1;
    ans = I2CMasterStop();
    __delay_ms(5);
    return ans;
}

void WriteOneByteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    int ans = -1;
    while(ans != 0){
        ans = WriteOneByteToEEPROMonce(addressEEPROM,addressHigh,addressLow,data);
        __delay_ms(10);
    }
}

void WriteOneByteToMainAndSubB0EEPROM(UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    WriteOneByteToEEPROM(EEPROM_address,addressHigh,addressLow,data);
    WriteOneByteToEEPROM(EEPROM_subaddress,addressHigh,addressLow,data);
}

void WriteCheckByteToEEPROMs(UBYTE B0Select,UBYTE addressHigh,UBYTE addressLow,UBYTE data){
    UBYTE mainAddress;
    UBYTE subAddress;
    mainAddress = (UBYTE)(EEPROM_address | B0Select);
    subAddress = (UBYTE)(EEPROM_subaddress | B0Select);
    WriteOneByteToEEPROM(mainAddress,addressHigh,addressLow,data);
    WriteOneByteToEEPROM(subAddress,addressHigh,addressLow,data);
}

void WriteLastCommandIdToEEPROM(UBYTE last_command_ID){
    WriteCheckByteToEEPROMs(B0select_for_TXCOBCLastCommandID, HighAddress_for_TXCOBCLastCommandID, LowAddress_for_TXCOBCLastCommandID, last_command_ID);
}


/*******************************************************************************
*Method for EEPROM Read
******************************************************************************/
// void ReadDataFromEEPROM(UBYTE Address7Bytes,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT EEPROMDataLength){

//     UBYTE Address = (UBYTE)(Address7Bytes << 1);
//     UBYTE ReadAddress = (UBYTE)(Address | 0x01);
//     I2CMasterStart();                       //Start condition
//     I2CMasterWrite(Address);                //7 bit address + Write
//     I2CMasterWrite(high_address);           //Adress High Byte
//     I2CMasterWrite(low_address);            //Adress Low Byte
//     I2CMasterRepeatedStart();               //Restart condition
    
//     I2CMasterWrite(ReadAddress);            //7 bit address + Read
//     for(UINT i = 0; i < EEPROMDataLength - 1; i++){
//         ReadData[i] = I2CMasterRead(1);     //Read + Acknowledge
//     }
//     ReadData[EEPROMDataLength - 1] = I2CMasterRead(0);
//     I2CMasterStop();                        //Stop condition
// }



int ReadEEPROMonce(UBYTE address,UBYTE high_address,UBYTE low_address){
    int dat;
    int ans = -1;
    ans = I2CMasterStart(address,0);         //Start condition
    if(ans == 0){
        ans = I2CMasterWrite(high_address);    //Adress High Byte
        if(ans == -1) return -1;
        ans = I2CMasterWrite(low_address);    //Adress Low Byte
        if(ans == -1) return -1;
        ans = I2CMasterRepeatedStart(address,1);         //Restart condition
        if(ans == -1) return -1;
        dat = I2CMasterRead(1); //Read + Acknowledge
        if(dat == -1) return -1;
    }else return -1;
    ans = I2CMasterStop();
    if(ans == -1) return -1;
    __delay_ms(5);
    return dat;
}

UBYTE ReadEEPROM(UBYTE address,UBYTE high_address,UBYTE low_address){
    UBYTE dat;
    int ans = -1;
    while(ans == -1){
        ans = ReadEEPROMonce(address,high_address,low_address);
        __delay_ms(10);
    }
    dat = (UBYTE)ans;
    return dat;
}


//TODO:need debug
// void ReadDataAndDataSizeFromEEPROM(UBYTE Address7Bytes,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT *EEPROMDataLength){
//     UBYTE Address = (UBYTE)(Address7Bytes << 1);
//     UBYTE ReadAddress = (UBYTE)(Address | 0x01);
//     I2CMasterStart();                       //Start condition
//     I2CMasterWrite(Address);                //7 bit address + Write
//     I2CMasterWrite(high_address);           //Adress High Byte
//     I2CMasterWrite(low_address);            //Adress Low Byte
//     I2CMasterRepeatedStart();               //Restart condition
    
//     I2CMasterWrite(ReadAddress);            //7 bit address + Read
//     for (*EEPROMDataLength = 0; ReadData[*EEPROMDataLength]!= I2Cnull; *EEPROMDataLength++);
//     for(UINT i = 0; i < *EEPROMDataLength; i++){
//         ReadData[i] = I2CMasterRead(1);     //Read + Acknowledge
//     }
//     ReadData[*EEPROMDataLength] = I2CMasterRead(0);
//     I2CMasterStop();          //Stop condition
    
//     //for denbugging
//     /*
//     for(UINT j = 0; j < *EEPROMDataLength; j++){
//     //putch(ReadData[0]);
//     //for(UINT j = 0; j < 5; j++){
//         putch(ReadData[j]);
//     }
//     putcrlf();*/
//     __delay_ms(200);
// }

//process command data if the command type is 'I2C'
void commandSwitchI2C(UBYTE command, UBYTE slaveAdress, UBYTE *dataHigh, UBYTE *dataLow){ 
    switch(command){    
        case 'w': //I2C write
//            I2CMasterWrite(slaveAdress);//TODO: check if method 'I2C write' is correct
//            I2CMasterWrite(dataHigh);
//            I2CMasterWrite(dataLow);
            break;
        case 'r': //I2C read
//            dataHigh = I2CMasterRead(slaveAdress);//TODO: check if method 'I2C read' is correct, especially do the pointers work as intended
//            dataLow = I2CMasterRead(slaveAdress); //TODO: what should happen with read in data?
            //TODO: write data to EEPROM
            //TODO: send Address where it is written to TXCOBC
            break;
        case 't': //I2C test
            //TODO: write method for I2C test
            //TODO: write test data to EEPROM
            //TODO: read EEPRON
            //TODO: send EEPROM address to TXCOBC
            break;
        case 'c': //I2C buffer clear
            //TODO: write method for I2C buffer clear 
            //TODO: clear: SSPBUF
            break;
        case 'b': //change I2C baud rate
            //TODO: write method for change I2C baud rate
            break;
        case 's': //set as a slave ic
            //TODO: write method for set as a slave ic
            break;
        case 'i': //measure IMU
            //TODO: write method for measure IMU
            break;
        default:
            switchError(error_I2C_commandSwitchI2C);
            break;
    }
}

void commandSwitchEEPROM(UBYTE command, UBYTE slaveAdress, UBYTE dataHigh, UBYTE dataLow, UBYTE data1, UBYTE data2){ 
    UBYTE data_length = 0;
    UBYTE *read_data;
    switch(command){    
        case 'w': //write data to EEPROM
            //TODO:now send data is only 1byte. change data size
            WriteToEEPROM(slaveAdress, dataHigh, dataLow, &data1);  //data1 is the data to send
            break;
        case 'r': //read data from EEPROM
            data_length = data1;
            //FIXME : read_data is a single pointer, maybe not worked as you think
            ReadDataFromEEPROM(slaveAdress, dataHigh, dataLow, read_data, (UINT)data1);
            if(data2=='f'){
                //fm downlink
            } else if(data2=='c'){
               // cw downlink
            } else {
                //TODO:add error
            }
            break;
        case 'e': //read any size of data from EEPROM 
            /* this function for read any size of data from EEPROM
             * 1.read data from EEPROM
             * 2.get data size
             * 3.read data from EEPROM (RX pic gets data size at step2)
             * 4.get any size of data 
             */
            data_length = ReadEEPROM(slaveAdress, dataHigh, dataLow);
            ReadDataFromEEPROM(slaveAdress, dataHigh, dataLow, read_data, data_length);
            
        case 't': //EEPROM test
//            TestEEPROM(slaveAdress);
            break;
        default:
            // switchError(error_I2C_commandSwitchEEPROM);
            break;
    }
}


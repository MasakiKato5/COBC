/* 
 * File:   CRC16.h
 * Author: Descartes
 * content: functions for cyclic redundancy check
 * Created on 2018/05/23, 0:45
 */

#ifndef CRC16_H
#define	CRC16_H

#include "typeDefine.h"

UWORD crc16(UWORD, UBYTE *, UBYTE);
UWORD identifyCRC16(UBYTE *);                                          //???   this method does not exist in cfile
UWORD checkCRC(UBYTE *, UBYTE);

#endif	/* CRC16_H */


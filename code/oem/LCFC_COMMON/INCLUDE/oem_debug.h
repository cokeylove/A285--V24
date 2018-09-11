//*****************************************************************************
//
//  oem_debug.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef	OEM_Debug_H
#define OEM_Debug_H

#define DebugRamAddr 0x00080B00
#define DebugRamRange 0xFF
#define BASE_MASK   0xFF00
#define LINE_MASK   0xFFF0

extern void Save_All_RamandGPIO(void);
extern BOOL CheckDebugCardIN (void);
extern BOOL CheckDebugCardOut (void);
extern void P80LedOut(BYTE bData);
extern void PollingBIOS80Port(void);
extern void RamDebug(unsigned char dbgcode);
extern void RamDebugWord(unsigned int dbgword);
extern void RamDebugDword(unsigned long dbgdword);


///////////////////SMbus Debug Card/////////////////////
#define SendECCode		0x01
#define SendBIOSCode	0x02
#define SendECWBIOSCode	0x03
#define ReadID			0x04


void Ec2iWrite (
  BYTE  RegOffset,
  BYTE  RegValue
);

BYTE Ec2iRead (
  BYTE  RegOffset
);

void DumpLpcReg (
  unsigned char Ldn
);

void DumpReg (
  unsigned short RegBase
);

extern void DumpAllReg (
  void
);

extern void Init_UART(void);


#endif

//*****************************************************************************
//
//  oem_flash.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef	OEM_FLASH_H
#define	OEM_FLASH_H


extern void EC_Mirror_release(void);

extern void SetLPCClockFreeRun(void);
extern void RamProgram(BYTE useport);
extern BYTE Read_Eflash_Byte(BYTE FA2,BYTE FA1,BYTE FA0);
extern BYTE Erase_Eflash_1K(BYTE FA2,BYTE FA1,BYTE FA0);
extern void Write_Eflash_1Byte(BYTE FA2,BYTE FA1,BYTE FA0,BYTE OneByte);



#endif

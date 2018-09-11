//*****************************************************************************
//
//  oem_init.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************
#ifndef OEM_INI_H
#define OEM_INI_H

extern void Oem_StartUp(void);
extern void Oem_Initialization(void);
extern void Init_ClearRam(void);
extern void Clear_RAM_Range(uchar_8 *nAddr, WORD nSize);
extern void Hook_ECExitFollowMode(void);
extern BYTE Hook_ECRetunrMainFuncKeepCondition(void);
extern void Init_SMBus_Regs(void);
extern void Init_Regs(void);
#if chPWSW1WDT
extern void OEM_RESETPWSWTIMER1(void);	
extern void OEM_PWRSWWDTEn(void);
#endif  //chPWSW1WDT


typedef struct REG_INIT_DEF
{
    uchar_8 *address;
    BYTE    initdata;
} sREG_INIT_DEF;

#if Auto_Detect_ITE_chip
extern void Auto_Setting_ITE_CHIP(void);
#endif


#endif


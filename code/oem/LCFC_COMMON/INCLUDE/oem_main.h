//*****************************************************************************
//
//  oem_main.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_MAIN_H
#define OEM_MAIN_H

#define  RAM_DETECT_STATUS_COUNT	20 //add for Enhanced Beep alarm when MEM lost -  //A485D00087:
//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
extern void Hook_Service_1ms(void);
extern void Hook_Timer1msEvent( );
extern void Hook_Timer5msEvent(void);
extern void Hook_Timer10msEventA(void);
extern void Hook_Timer10msEventB(void);
extern void Hook_Timer50msEventA(void);
extern void Hook_Timer50msEventB(void);
extern void Hook_Timer50msEventC(void);
extern void Hook_Timer100msEventA(void);
extern void Hook_Timer100msEventB(void);
extern void Hook_Timer100msEventC(void);
extern void Hook_Timer500msEventA(void);
extern void Hook_Timer500msEventB(void);
extern void Hook_Timer500msEventC(void);
extern void Hook_Timer1SecEventA(void);
extern void Hook_Timer1SecEventB(void);
extern void Hook_Timer1SecEventC(void);
extern void Hook_Timer1MinEvent(void);
extern void service_OEM_1(void);
extern void service_OEM_2(void);
extern void service_OEM_3(void);
extern void service_OEM_4(void);
extern void service_reserved_16(void);
extern void service_reserved_17(void);
extern void service_reserved_18(void);
extern void service_reserved_19(void);
extern void service_reserved_20(void);
extern void service_reserved_21(void);
extern void service_reserved_22(void);
extern void service_reserved_23(void);
extern void service_reserved_24(void);
extern void service_reserved_25(void);
extern void service_reserved_26(void);
extern void service_reserved_27(void);
extern void service_reserved_28(void);
extern void service_reserved_29(void);
extern void service_reserved_30(void);
extern void service_reserved_31(void);
extern BYTE Hook_Only_Timer1msEvent(void);
extern BYTE OEM_SkipMainServiceFunc(void);
extern BYTE Hook_Start_EC_Handle_Task(void);


#endif


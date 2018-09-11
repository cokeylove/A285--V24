//*****************************************************************************
//
//  oem_lpc.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_LPC_H
#define OEM_LPC_H

//unsigned char Display_Debug_Message;  //Display debug message via serial port
                    //1: Enable, 0: Disable
//extern BYTE Display_Debug_Message;

extern void MuteProcess(void);
extern void CameraProcess(void);
extern void WirelessProcess(void);
#if Support_IFFS
extern void IFFSProcess(void);
#endif // Support_IFFS
extern void SetACIN_Int(void);
extern void SetVGA_AC_DET(void);
extern void SetPWRSW_Int(void);

extern void SetUSBExTimer(void);
//A485D00090--->Start
extern void SetUSBExTimerAC(void);
extern void SetBAT1INOUT_Int(void);
extern void SetBAT2INOUT_Int(void);
extern void SetLANWake_Int(void);
extern void SetPDC_Int(void);
extern void SetSLPS3_Int(void);
//A485D00090--->End
extern void WakeUp_DO_Function(void);
extern void ISCT_Process(void);
extern void ISCT_TimerCnt(void);
extern void ISCT_MinutetoSec(void);
extern void ISCT_SECtoChk(void);
extern void ISCT_OPEN_APS(void);

#if SW_ISCT
#define ISCT_BAT_LOW      20    // Inhibit ISCT wake battery low capacity condition.
#define ISCT_BAT_OvTpLimit    50    // Inhibit ISCT wake 50 degree.
#define ISCT_BAT_UnTpLimit    0xFB  // Inhibit ISCT wake -5 degree
void ISCT_Behavior(void);
#endif


#endif

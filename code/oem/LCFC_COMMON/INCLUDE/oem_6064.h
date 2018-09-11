//*****************************************************************************
//
//  oem_6064.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_6064_H
#define OEM_6064_H

extern void Hook_60Port(BYTE P_KBHIData);
extern void Hook_64Port(BYTE P_KBHICmd);
extern void Hook_Keyboard_Cmd(BYTE kbcmd);
extern void Hook_Keyboard_Cmd_Parameter(BYTE kbcmdp);
extern void Hook_Mouse_D4Cmd(BYTE mscmd);
extern void Hook_Mouse_90Cmd(BYTE mscmd);
extern void Hook_Mouse_91Cmd(BYTE mscmd);
extern void Hook_Mouse_92Cmd(BYTE mscmd);
extern void Hook_Mouse_93Cmd(BYTE mscmd);

extern void Cmd_40(BYTE sCount);
extern void Cmd_41_A0(BYTE nPort);
extern void Cmd_41_A1(BYTE nPort);
extern void Cmd_41_A2(BYTE nPort);
extern void Cmd_41_A3(BYTE nPort);
extern void Cmd_42(BYTE eFlashBank);
extern void Cmd_43(BYTE nPort,BYTE nData);
extern void Cmd_44(BYTE nPort);
extern void Cmd_45(BYTE nPort, BYTE sCount);
#if Support_COMPAL_MFG_TOOL
extern void Cmd_46(BYTE nPort,BYTE nData);
#endif //Support_COMPAL_MFG_TOOL
extern void Cmd_47(BYTE nPort, BYTE nData);
extern void Cmd_4E(BYTE nPort, BYTE nOffSet);
extern void Cmd_51(BYTE nPort);
extern void Cmd_52_A0(BYTE nPort);
extern void Cmd_52_A5(BYTE nPort);
extern void Cmd_52_A3(BYTE nPort);
extern void Cmd_52_A4(BYTE nPort);
extern void Cmd_53(BYTE nPort,BYTE nDataH,BYTE nDataM,BYTE nDataL);
extern void Cmd_56(BYTE nPort);
extern void Cmd_58(BYTE sCount);
extern void Cmd_59(BYTE nPort,BYTE nData, BYTE nData2);
#if EC_Brightness_Ctrl
extern void Cmd_5C(BYTE nPort);
extern void Cmd_5D(BYTE sCount);
#endif
extern void Cmd_97(BYTE nData);
#if Support_RFID 
extern BYTE Cmd_01(BYTE nDataH,BYTE nDataL,BYTE* ref);
extern BYTE Cmd_02(BYTE nDataH,BYTE nDataL,BYTE nData);
extern BYTE Cmd_09(BYTE nData,BYTE* ref);
extern void Cmd_0A(BYTE nDataH,BYTE nDataL);
#endif 
extern void Cmd_B0(BYTE nPort,BYTE nData);
extern void Cmd_B1(BYTE nDataH, BYTE nDataL);
extern void Cmd_B2(BYTE nDataH, BYTE nDataL, BYTE nData);
extern void Cmd_B3(BYTE nPort,BYTE nDataH,BYTE nDataL);
extern void Cmd_B7(BYTE nPort,BYTE nData);

extern void Cmd_B4(void);

#define SysAction_Nothing                   0x01
#define SysAction_EC_Shutdown               0x02
#define SysAction_EC_Restart                0x03
#define SysAction_SysShutdown_EC_AutoBoot   0x04

#endif


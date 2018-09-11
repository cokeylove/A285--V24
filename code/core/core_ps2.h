/*
 * ****************************************************************************
 * TITLE: CORE_PS2.H - Header File for PS/2 device Handler (PS2.C)
 *
 * Copyright (c) 1983-2009, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 * ****************************************************************************
 */
#ifndef CORE_PS2_H
#define CORE_PS2_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
/* For Dos mode only */
#define PS2_Pack_Debug      0
/* Enable Debug keyboard command to keyboard and ACK bytes */
#define PS2_KBCMD           0
/* Enable Debug keyboard command to TP or moues and ACK bytes */
#define PS2_MSCMD           0

#define PS2_Port0_CLK_CtrlReg	GPCRF0
#define PS2_Port0_Data_CtrlReg	GPCRF1
#define PS2_Port1_CLK_CtrlReg	GPCRF2
#define PS2_Port1_Data_CtrlReg	GPCRF3
#define PS2_Port2_CLK_CtrlReg	GPCRF4
#define PS2_Port2_Data_CtrlReg	GPCRF5

#define ScanAUXStep_ScanPort    0xF0
#define ScanAUXStep_ScanPort0   0xF0
#define ScanAUXStep_ScanPort1   0xF1
#define ScanAUXStep_ScanPort2   0xF2

#define ScanAUXStep_WaitPort    0xE0
#define ScanAUXStep_WaitPort0   0xE0
#define ScanAUXStep_WaitPort1   0xE1 
#define ScanAUXStep_WaitPort2   0xE2

#define ScanAUXStep_Skip        0xD0
#define ScanAUXStep_Skip0       0xD0
#define ScanAUXStep_Skip1       0xD1
#define ScanAUXStep_Skip2       0xD2

#define ScanAUXStep_Init        0xC0
#define ScanAUXStep_Init0       0xC0
#define ScanAUXStep_Init1       0xC1
#define ScanAUXStep_Init2       0xC2

#define ScanAUXStep_InitFail    0xC8
#define ScanAUXStep_InitFail0   0xC8
#define ScanAUXStep_InitFail1   0xC9
#define ScanAUXStep_InitFail2   0xCA

#define ScanAUXStep_CheckType   0xB0
#define ScanAUXStep_CheckType0  0xB0
#define ScanAUXStep_CheckType1  0xB1
#define ScanAUXStep_CheckType2  0xB2

#define ScanAUXStep_CheckTypeFail   0xB8
#define ScanAUXStep_CheckTypeFail0  0xB8
#define ScanAUXStep_CheckTypeFail1  0xB9
#define ScanAUXStep_CheckTypeFail2  0xBA

#define ScanAUXStep_UpdateKB    0xA0
#define ScanAUXStep_UpdateKB0   0xA0
#define ScanAUXStep_UpdateKB1   0xA1
#define ScanAUXStep_UpdateKB2   0xA2

#define ScanAUXStep_CheckOK     0x80
#define ScanAUXStep_CheckOK1    0x80
#define ScanAUXStep_CheckOK2    0x81
#define ScanAUXStep_CheckOK3    0x82

/* 1200 ms */
#define DeviceResetCmdITTime    120
/* 50 ~ 60 ms */
#define InactiveTime		    6
/* 70 ~ 80 ms */
#define CheckDeviceTimeOut      8
/* Skip scan aux devices for keyboard. (10ms timer base) */
#define KBSkipScanAUXTime       6
/* Skip scan aux devices for mouse. (10ms timer base) */
#define MSSkipScanAUXTime       6
/* EC On/Off PS2 device time-out */
#define PS2_OnOffTimeOutCUNT    5
/* Time base is 30ms 30x17=510ms */
#define ResetCmdTOTime          17

#define PS2_ReceiveMode         0x17
#define PS2_InhibitMode         0x11

#define SendCmdToKB             0x01
#define SendCmdToMS             0x02

#define ScanAllPS2Channel       0x00
#define ScanMouseChannel        0x01
#define ScanKeyboardChannel     0x02

#define AUXScanIndexFree        0x00
#define AUXScanIndexInUse       0xFF

#define PS2_Transmission_Mode   0x01
#define PS2_Receive_Mode        0x02

/* ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Ps2_Init(void);
extern void service_PS2_data(void);
extern void CheckAUXDeviceType(void);
extern void SendCmdtoMouse(BYTE PortNum);
extern void SendCmdtoMUX(BYTE port);
extern void ECSend2Port( BYTE PortNum, BYTE PortData);
extern void Send2Port( BYTE PortNum, BYTE PortData, BYTE action);
extern BYTE Send2PortNWait( BYTE PortNum, BYTE cmd, BYTE bytecunt);
extern void AUXDevicePlugOutClearVariables(void);
extern void PollingAUXDeviceCenter(void);
extern void service_ps2(void);
extern void PS2DeviceInactive(void);
extern void PS2Deviceactive(void);
extern void PS2ReceiveMode_OneChannel(BYTE PortNum, BYTE SetInhibitTime);
extern void InhibitAUXDevice(void);
extern void SendFromAux(BYTE auxdata);
extern void SetPS2InhibitTime(BYTE time);
extern void SetPS2SkipScanTime(BYTE time);
extern void ScanAUXDevice(BYTE scan_selection);
extern void ScanAUXDeviceStep(void);
extern void ScanAUXPortX(void);
extern void EnableAUXDevice(void);
extern void ConfigExtKeyboard(BYTE portnum);
extern BYTE SetIntellimouse(BYTE DriverType, BYTE port);
extern BYTE ReadMouseID(BYTE port);
extern BYTE ConfigExtMouse(BYTE portnum);
extern BYTE bExtAUXTimeOutCheck(BYTE channel, BYTE p_mode);
extern BYTE CheckAuxMux(void);
extern void KBCmdAckByteCunt(BYTE ackcunt);
extern void SetOtherKBNeedUpdataFlag(BYTE KBCmd);
extern void SetStartScanAUXFlag(void);
extern void AUXData_To_Host_nWait(BYTE data_byte, BYTE portnum);
extern void ReSendPS2PendingData(void);
extern void CheckNSetMainKBCHN(BYTE PortNum);
extern void CheckNSetMainMSCHN(BYTE PortNum);
extern void ResetMSPendingData(void);
extern void DisablePS2Port_0(void);
extern void DisablePS2Port_1(void);
extern void DisablePS2Port_2(void);
extern void EnablePS2Port_0(void);
extern void EnablePS2Port_1(void);
extern void EnablePS2Port_2(void);
extern void PS2_PowerOff(void);
extern void PS2_PowerOn(void);
extern void PowerOffClearTPPendingData(void);
extern void SetPS2CmdACKCounter(BYTE ackcunt);
extern BYTE WaitECSend2PortACK(BYTE PortNum);
extern void WaitPS2DeviceACK(BYTE PortNum);
extern BYTE PS2CheckPendingISR(void);
extern void SetAUXScanIndexHS(BYTE status);
extern void ClearAUXScanTimeOutCunt(void);

/* 
 * ****************************************************************************
 * structure 
 * ****************************************************************************
 */
typedef struct PS2Struct
{
    uchar_8 	*ps2ctrl;
    uchar_8 	*ps2int;
    uchar_8 	*ps2status;	
	uchar_8 	*ps2data;
	uchar_8 	*ps2ier;
    uchar_8 	*ps2isr;
	uchar_8     ctrlbit;
} sPS2Struct;
extern const sPS2Struct asPS2Struct[];

#endif


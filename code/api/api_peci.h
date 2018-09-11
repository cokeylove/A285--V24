/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_peci.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_PECI_H
#define API_PECI_H

/* 
 * ****************************************************************************
 * Global variable (memory address allocation follow project )
 * ****************************************************************************
 */
extern BYTE   PECI_CRC8;
extern BYTE   PECI_CompletionCode;

/* 
 * ****************************************************************************
 * Selection
 * ****************************************************************************
 */
/* 1 : softwave calculate AW_FCS, 0 : H.W. calculate AW_FCS */
#define PECI_Softwave_AWFCS         0

/* Host ID = PECI_HostID << 1 (bit7 ~ bit0) */
#define PECI_HostID                 0x01

/* 
 * ****************************************************************************
 * Define PECI command
 * ****************************************************************************
 */
#define PECI_CMD_GetDIB             0xF7
#define PECI_CMD_Pin                0x00
#define PECI_CMD_GetTemp            0x01
#define PECI_CMD_RdPkgConfig        0xA1  
#define PECI_CMD_WrPkgConfig        0xA5  
#define PECI_CMD_RdIAMSR            0xB1
#define PECI_CMD_WrIAMSR            0xB5
#define PECI_CMD_RdPCIConfig        0x61
#define PECI_CMD_WrPCIConfig        0x65
#define PECI_CMD_RdPCIConfigLocal   0xE1
#define PECI_CMD_WrPCIConfigLocal   0xE5

/* 
 * ****************************************************************************
 * Define completion code
 * ****************************************************************************
 */
#define PECI_CC_Valid               0x40
#define PECI_CC_TimeOut             0x80
#define PECI_CC_Ignored             0x90

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Init_PECI(void);
extern void PECI_HostEnable(void);
extern void PECI_HostDisable(void);
extern BYTE PECI_CheckHostBusy(void);
extern BYTE PECI_CheckHostFinish(void);
extern void ResetPECIStatus(void);
extern void PECI_HostControl(BYTE control);
extern BYTE PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_GetDIB(
            BYTE addr, BYTE *ReadData, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_GetTemp(
            BYTE addr, BYTE *ReadData, BYTE Domain, BYTE ReadLen,BYTE WriteLen);
extern BYTE PECI_RdPkgConfig(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry,
            BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE PECI_WrPkgConfig(BYTE addr, BYTE *WriteData, BYTE Domain,
            BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen,
            BYTE WriteLen);
extern BYTE PECI_RdIAMSR(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry,
            BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
#endif

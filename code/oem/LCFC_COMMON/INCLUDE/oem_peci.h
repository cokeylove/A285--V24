/*-----------------------------------------------------------------------------
* TITLE: API_PECI.H
*
* Author : Dino
*
* History : 
* ------------------------------
* Version : 1.00
* ------------------------------
*
* Note : To link [api_xxx.o] if related api function be used.
*
*---------------------------------------------------------------------------*/

#ifndef OEM_PECI_H
#define OEM_PECI_H

//*****************************************************************************
// Global variable (memory address allocation follow project )
//*****************************************************************************
extern BYTE   PECI_CRC8;
extern BYTE   PECI_CompletionCode;

//-----------------------------------------------------------------------------
// Define CPU address
//-----------------------------------------------------------------------------
#define PECI_CPU_ADDR       				0x30

//-----------------------------------------------------------------------------
// Define processor Tjmax
//-----------------------------------------------------------------------------
#define PECI_CPU_Tjmax      				100
#define PECI_Read_TjmaxTO   				10      				// Re-try to read CPU Tjmax via PECI

//*****************************************************************************
// Selection
//*****************************************************************************
#define PECI_Softwave_AWFCS         		0       				// 1 : softwave calculate AW_FCS
                                            									// 0 : H.W. calculate AW_FCS
#define PECI_HostID                 			0x01    				// Host ID = PECI_HostID << 1 (bit7 ~ bit0)

// ----------------------------------------------------------------------------
// PECI3.0 domain parameter definition
// ----------------------------------------------------------------------------
#define PECI_Domain_0        				0x00
#define PECI_Domain_1           			0x01

// ----------------------------------------------------------------------------
// The index definition of PECI3.0 read/write Pkg command
// ----------------------------------------------------------------------------
#define PECI_Index_PTR          			2       				// Package temperature read
#define PECI_Index_TTR          			16      				// Read the Maximum Allowed Processor Temperature
#define PECI_Index_DRCTR        			22      				// DRAM rank channel temperature read
#define PECI_Index_PPL1         			26      				// Power limit 1
#define PECI_Index_PPL2         			27      				// Power limit 2
#define PECI_Index_PPSU         			28      				// Power SKU unit
#define PECI_Index_PPSL         			29      				// Package power and time limit information
#define PECI_Index_PPSH         			30      				// Package power and time limit information
#define PECI_Index_DRAM					24
                                        
//*****************************************************************************
// Define PECI command
//*****************************************************************************
#define PECI_CMD_GetDIB             			0xF7
#define PECI_CMD_Pin                			0x00
#define PECI_CMD_GetTemp            		0x01
#define PECI_CMD_RdPkgConfig        		0xA1  
#define PECI_CMD_WrPkgConfig        		0xA5  
#define PECI_CMD_RdIAMSR            		0xB1
#define PECI_CMD_WrIAMSR            		0xB5
#define PECI_CMD_RdPCIConfig        		0x61
#define PECI_CMD_WrPCIConfig        		0x65
#define PECI_CMD_RdPCIConfigLocal   		0xE1
#define PECI_CMD_WrPCIConfigLocal   		0xE5

//*****************************************************************************
// Define completion code
//*****************************************************************************
#define PECI_CC_Valid               			0x40
#define PECI_CC_TimeOut             			0x80
#define PECI_CC_Ignored             			0x90

#define PSTB_RepeatCycle0           			0
#define PSTB_RepeatCycle1           			1
#define PSTB_RepeatCycle2           			2
#define PSTB_RepeatCycle3           			3
#define PSTB_RepeatInterval         			4           				// 10ms timer base
#define PSTB_StopOnError            			5
#define PSTB_ClientAddress          			6
#define PSTB_WriteLength            			7
#define PSTB_ReadLength             			8
#define PSTB_CommandSpecificData    		9
#define PSTB_CmdCode                			PSTB_CommandSpecificData

//*****************************************************************************
// Function prototype
//*****************************************************************************
extern void 	Init_PECI(void);
extern void 	PECI_HostEnable(void);
extern void 	PECI_HostDisable(void);
extern BYTE 	PECI_CheckHostBusy(void);
extern BYTE 	PECI_CheckHostFinish(void);
extern void 	ResetPECIStatus(void);
extern void 	PECI_HostControl(BYTE control);
extern BYTE 	PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen);
extern BYTE 	PECI_GetDIB(BYTE addr, BYTE *ReadData, BYTE ReadLen, BYTE WriteLen);
extern BYTE 	PECI_GetTemp(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen);
extern BYTE 	PECI_RdPkgConfig(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE 	PECI_WrPkgConfig(BYTE addr, BYTE *WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern BYTE 	PECI_RdIAMSR(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen);
extern void 	ReadCPUTemp(void);
#endif

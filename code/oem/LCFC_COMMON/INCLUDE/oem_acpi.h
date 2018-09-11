//*****************************************************************************
//
//  oem_acpi.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef	OEM_ACPI_H
#define	OEM_ACPI_H

#define		EVENT_BUFFER_SIZE	9
#define 	PENDING_BUFFER      8


extern void Hook_62Port(BYTE p_PM1Data);
extern void Hook_66Port(BYTE p_PM1Cmd);
extern void Hook_ACPICommand(void);
extern void OEM_ACPI_Gen_Int(void);
extern void ResetSCIEvent(void);
extern void ECQEvent(BYTE sci_number);
extern void ECSMI_SCIEvent(BYTE smi_number);
extern BYTE ECSMIEvent(BYTE NewSMI);
extern BYTE Hook_ReadMapECSpace(BYTE MapIndex);
extern void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1);
extern void OSSMbusComd(void);

#define ECNameSpace        OEMRAMA




#endif

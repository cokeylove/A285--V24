/*
 * ****************************************************************************
 * TITLE: CORE_ACPI.H
 *
 * Copyright (c) 1983-2006, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 * ****************************************************************************
 */
 
#ifndef CORE_ACPI_H
#define CORE_ACPI_H

/*
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define ACPICmdSCI          0x00
#define QeventSCI           0x01

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void service_pci2(void);
extern void service_pci2f(void);
extern void Read_External_Ram(void);
extern void Write_External_Ram(void);
extern void SendFromAcpi(BYTE ecdata);
extern BYTE CheckBurstMode(void);
extern void SetTotalBurstTime(void);
extern void ACPI_Gen_Int(BYTE Qevent);
extern void Core_62Port(BYTE p_PM1Data);
extern void Core_66Port(BYTE p_PM1Cmd);

#endif 


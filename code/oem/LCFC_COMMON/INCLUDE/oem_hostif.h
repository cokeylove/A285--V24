//*****************************************************************************
//
//  oem_hostif.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_HOSTIF_H
#define OEM_HOSTIF_H

extern void Data_To_PM1(BYTE data_byte);
extern void PM1_DataPending(BYTE nPending);
extern BYTE GetPM1_PendingData(void);
extern void Data_To_PM2(BYTE data_byte);
extern void PM2_DataPending(BYTE nPending);
extern BYTE GetPM2_PendingData(void);
extern void Command_Process_To_Host(void);
extern void InitSio(void);
extern void InitSysMemory2ECRam(void);
extern void Data2Port(BYTE nPort, BYTE rData);
extern void MultiB2Port(BYTE nPort, BYTE rData);



#endif

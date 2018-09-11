/*-----------------------------------------------------------------------------
 * TITLE: OEM_RFID.H
 *
 * Author : AC & CM
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *---------------------------------------------------------------------------*/

#ifndef	OEM_RFID_H
#define	OEM_RFID_H

#define EEPROM_SERIAL_INFO_AREA			0		//Serialization Information Area (Block #0, #1)'SER#'
#define EEPROM_SERIAL_INFO_AREA_B0	0
#define EEPROM_SERIAL_INFO_AREA_B1	1
#define EEPROM_USER_INFO_AREA				2		//User Information Area (Block #2, #3)'USR#'
#define EEPROM_USER_INFO_AREA_B2		2
#define EEPROM_USER_INFO_AREA_B3		3
#define EEPROM_CONFIG_INFO_AREA			4		//Configuration Information Area (Block #4, #5)'CON#'
#define EEPROM_CONFIG_INFO_AREA_B4	4
#define EEPROM_CONFIG_INFO_AREA_B5	5

#define RFID_EEPROMA2			EFLASHSIZE
#define RFID_EEPROMA1_RFID1		0xE0	// RFID address = 0x1E000 ~ 0x1FBFF (1K+5K+1K)?
#define RFID_EEPROMA1_N1		0xE4	// RFID newer data address 1
#define RFID_EEPROMA1_N2		0xE8	// RFID newer data address 2
#define RFID_EEPROMA1_RFID2		0xEC	// RFID newer data address 3
#define RFID_EEPROMA1_N4		0xF0	// RFID newer data address 4
#define RFID_EEPROMA1_N5		0xF4	// RFID newer data address 5
#define RFID_EEPROMA1_index		0xF8	// This is index for RFID data address

#define RFID_EEPROMA1_RFID3		0xD0
#define RFID_EEPROMA1_N6		0xD4	// RFID newer data address 1
#define RFID_EEPROMA1_N7		0xD8	// RFID newer data address 2
#define RFID_EEPROMA1_index2	0xDC	// This is index for RFID data address



#define RFID_EEPROM_RN0			0xFB80
#define RFID_EEPROM_WN0			0xFB40
#define RFID_EEPROM_RN1			0xFB20
#define RFID_EEPROM_WN1			0xFB10
#define RFID_EEPROM_RN2			0xFB08
#define RFID_EEPROM_WN2			0xFB04
#define RFID_EEPROM_RN3			0xFB02
#define RFID_EEPROM_WN3			0xFB01
#define RFID_EEPROM_RN4			0xFA80
#define RFID_EEPROM_WN4			0xFA40
#define RFID_EEPROM_RN5			0xFA20
#define RFID_EEPROM_WN5			0xFA10
#if (RFID_EEPROMA2 == EFLASH192KB)
#define RFID_EEPROM_RN6			0xD320
#define RFID_EEPROM_WN6			0xD120
#else
#define RFID_EEPROM_RN6			0xDF20
#define RFID_EEPROM_WN6			0xDD20
#endif

#define dRFID_EEPROM_RN0			0xF980
#define dRFID_EEPROM_WN0			0xF940
#define dRFID_EEPROM_RN1			0xF920
#define dRFID_EEPROM_WN1			0xF910
#define dRFID_EEPROM_RN2			0xF908
#define dRFID_EEPROM_WN2			0xF904
#define dRFID_EEPROM_RN3			0xF902
#define dRFID_EEPROM_WN3			0xF901
#define dRFID_EEPROM_RN4			0xF880
#define dRFID_EEPROM_WN4			0xF840
#define dRFID_EEPROM_RN5			0xF820
#define dRFID_EEPROM_WN5			0xF810
#if (RFID_EEPROMA2 == EFLASH192KB)
#define dRFID_EEPROM_RN6			0xCF20
#define dRFID_EEPROM_WN6			0xCD20
#else
#define dRFID_EEPROM_RN6			0xDB20
#define dRFID_EEPROM_WN6			0xD920
#endif


#define RFID_PROCTA2			EFLASHSIZE
#define RFID_PROCTA1			0xFF
#define INITMARK                0x78                



extern BYTE readRFIDEEprom(BYTE BANKx,BYTE addr);
extern BYTE readRFIDindex(BYTE BANKx,BYTE addr);
extern void writeRFIDEEprom(BYTE BANKx,BYTE addr,BYTE nData);
extern void RFID_Update_EEPROMB07(BYTE region);
extern void unlockRFIDProtect(void);
extern void writeRFIDProtect(BYTE BANKx,BYTE acc);

extern void initRFIDProtect(void);

#endif

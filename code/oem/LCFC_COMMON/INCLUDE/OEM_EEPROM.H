/*-----------------------------------------------------------------------------
 * TITLE: OEM_EEPROM.H
 *
 * Author : Ivan.Chen
 *
 * Note : EEPROM define.
 *---------------------------------------------------------------------------*/
 
#ifndef OEM_EEPROM_H
#define OEM_EEPROM_H

#define SYSTEM_ENABLE_G3_WAKEUP_LAN		(1)

extern void Chk_pDevStus(void);
extern void Update_EEPROMMark(void);
extern void WRITE_DATA2EEPROM(UINT8* ram_pntr, WORD size, BYTE EEPROMBANK, WORD offset);
extern void UPDATA_EEPROM_DATA(UINT8* ram_pntr, WORD size, BYTE EEPROMBANK, WORD offset);
extern void Update_16ByteEEPROM(void);
extern void Erase_EEPROMAll(void);
#if(uMBID_LoadRom)
extern void Update_ECEEPROM(void);
#endif
#if SYSTEM_ENABLE_G3_WAKEUP_LAN
//A285D00041:Fix sysem cannot be WoLAN from S5 (G3 then plugin AC) with magic packet
extern void Chk_WOLStatus(void);
#endif
//0x29C00 ~ 0x29CFF
//dump GPIO
#define EEPROM_GPIO_DEGUG    0x9C
//0x2A000 ~ 0x2AFFF
//dump all ram by Fn=SA
#define EEPROM_ALL_RAM0       0xA0
#define EEPROM_ALL_RAM4       0xA4
#define EEPROM_ALL_RAM8       0xA8
#define EEPROM_ALL_RAMC       0xAC
//0x2B000 ~ 0x2BFFF
//EEPROM

#define EEPROMA2              EFLASHSIZE
#define EEPROMA1_16Byte       0xB8
#define EEPROMM1_16Byte_end   0xBB

#define EEPROM_Temp_Swith     0xBC

//BANK 0
#define EEPROM_Bank0          0xB0
#define EEPROM_modify_flag    0xEB
#define EEPROM_KB_Type        0xF0

//BANK 1
#define EEPROM_Bank1          0xB1

//BANK 2
#define EEPROM_Bank2          0xB2

//BANK 3
#define EEPROM_Bank3          0xB3

//BANK 4
#define EEPROM_Bank4          0xB4

//BANK 5
#define EEPROM_Bank5          0xB5

//BANK 6
#define EEPROM_Bank6          0xB6

//BANK 7
#define EEPROM_Bank7          0xB7
#define EEPROM_Bank7_L16B     0xB7F0
#define EEPROM_TYPEC_STATUS   0x02
#define EEPROM_PORT80_HISTORY 0xC0 //0xC0 ~ 0xCE
#define EEPROM_MBATStopCharge  0xD0
#define EEPROM_MBATStartCharge 0xD1
#define EEPROM_SBATStopCharge  0xD2
#define EEPROM_SBATStartCharge 0xD3
#define EEPROM_MBID           0xE0
#define EEPROM_DevStus        0xE5
 #define EEPROM_DevStus_USB   BIT0
 #define EEPROM_DevStus_AOU_DC_Dis   BIT1
#define EEPROM_GPIO_STATUS    0xEF
#define EEPROM_SAVE_PORT80    0xF1

#if SYSTEM_ENABLE_G3_WAKEUP_LAN
//A285D00041:Fix sysem cannot be WoLAN from S5 (G3 then plugin AC) with magic packet
#define EEPROM_WOLStatus        0xEA
#define EEPROM_WOLDCEn          BIT0
#define EEPROM_WOLACEn          BIT1
#endif

#endif

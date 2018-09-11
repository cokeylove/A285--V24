#ifndef _ROM_H_
#define _ROM_H_
/* ****************************************************************************
 *
 *  Rom.h - for romlib v501
 *        -
 *        - Version infomation in 0x7006C ~ 0x7006F
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2013/12/13
 *   Modify on: 2014/09/12
 * 
 * ***************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../api/direct_map/direct_map.h"

//----------------------------------------------------------------------------
//
//  SPI flash command code
//
//----------------------------------------------------------------------------
#define SPICmd_WRSR             0x01    // Write Status Register
#define SPICmd_BYTEProgram      0x02    // Program One Byte Data
#define SPICmd_ReadData         0x03    // Read Byte
#define SPICmd_WRDI             0x04    // Write diaable

#define SPICmd_ReadStatus       0x05    // Read Status Register
#define SPIStatus_BUSY          BIT0    // 1 = Internal Write operation is in progressn
#define SPIStatus_WEL           BIT1    // 1 = Device is memory Write enabled
#define SSTSPIStatus_AAI        BIT6    // 1 = AAI programming mode 

#define SPICmd_WREN             0x06    // Write Enable
#define SPICmd_HighSpeedRead    0x0B    // High-Speed Read
#define SPICmd_EWSR             0x50    // Enable Write Status Register
#define SPICmd_RDID             0xAB    // Read ID
#define SPICmd_DeviceID         0x9F    // Manufacture ID command
#define SPICmd_AAIWordProgram   0xAD    // Auto Address Increment Programming (word)
#define SPICmd_AAIProgram       0xAF    // Auto Address Increment Programming

#define SPICmd_SectorErase      0xD7    // ITE eFlash Sector Erase(1K bytes) Command.
#define SPICmd_Erase4KByte      0x20    // Erase 4 KByte block of memory array
#define SPICmd_Erase32KByte     0x52    // Erase 32 KByte block of memory array
#define SPICmd_Erase64KByte     0xD8    // Erase 64 KByte block of memory array

//----------------------------------------------------------------------------
//
//  SPI status setting
//
//----------------------------------------------------------------------------
#define SPIStatus_ProtectAll    0xFC
#define SPIStatus_UnlockAll     0x00
#define SPIStatus_WriteEnable   0x02

//----------------------------------------------------------------------------
//
//  SPI internal/external flash SELECTION
//
//----------------------------------------------------------------------------
#define SPI_selection_internal  0x4F
#define SPI_selection_external  0x0F

//----------------------------------------------------------------------------
//
//  CPU clock selection
//
//----------------------------------------------------------------------------
enum _CPU_CLOCK_SEL_
{
    CPU_CLOCK_8MHz = 1,
    CPU_CLOCK_16MHz,    
    CPU_CLOCK_24MHz,    
    CPU_CLOCK_32MHz,    
    CPU_CLOCK_48MHz,    
    CPU_CLOCK_64MHz,    
    CPU_CLOCK_72MHz,    
    CPU_CLOCK_96MHz
};

//----------------------------------------------------------------------------
//
//  Define function type
//
//----------------------------------------------------------------------------
#ifdef DEF_BUILD_ROM // {
#include "rom_e.h"
#else   // } DEF_BUILD_ROM

typedef char* (*ROM_FUN_PC_PC_CPC_UL)(char*, const char*, unsigned long);
typedef void* (*ROM_FUN_PV_PV_I_UL)(void*, int, unsigned long);
typedef void* (*ROM_FUN_PV_PV_PV_UL)(void*, void*, unsigned long);

typedef int   (*ROM_FUN_I_I)(int);
typedef int   (*ROM_FUN_I_PV_PV_UL)(void*, void*, unsigned long);

typedef void  (*ROM_FUN_V_V)(void);
typedef void  (*ROM_FUN_V_UC)(unsigned char);
typedef void  (*ROM_FUN_V_UC_UC)(unsigned char, unsigned char);
typedef void  (*ROM_FUN_V_UL_UC)(unsigned long, unsigned char);
typedef void  (*ROM_FUN_V_UC_UC_UC)(unsigned char, unsigned char, unsigned char);
typedef void  (*ROM_FUN_V_UC_PUC_UC)(unsigned char, unsigned char*, unsigned char);
typedef void  (*ROM_FUN_V_UC_UC_UL)(unsigned char, unsigned char, unsigned long);
typedef void  (*ROM_FUN_V_UC_UL_PUC_UL)(unsigned char, unsigned long, unsigned char*, unsigned long);

typedef unsigned char  (*ROM_FUN_UC_UC)(unsigned char);

//----------------------------------------------------------------------------
//
//
//  Extern Function
//
//
//----------------------------------------------------------------------------
#define ROM_CODE_BASE                              (0x70000UL)

//-------------------------------------
//  Prototype:
//      void spi_write_status(BYTE Select, BYTE WStatus, BYTE Enable);
//
//  Description:
//      Write status (command 0x01).
//  
//  Parameters:
//      Select: Indirect Selection
//          0x4F: internal flash.
//          0x0F: external flash.
//
//      WStatus: SPI Write Status:
//          Status to be set.
//
//      Enable: ucEnableWriteStatusReg:
//          For SST flash chip (ID: 0xBF), this value must be set to 1
//
//  Return Value:
//      None.
//-------------------------------------
#define spi_write_status                     ((ROM_FUN_V_UC_UC_UC) ROM_CODE_BASE + 0x014e)

//-------------------------------------
//  Prototype:
//      void spi_erase(unsigned char Select, unsigned char EraseCmd, unsigned long Addr)
//
//  Description:
//      Erase flash.
//  
//  Parameters:
//      Select: Indirect Selection
//          0x4F: internal flash.
//          0x0F: external flash.
//
//      EraseCmd: SPI Flash Erase Cmd
//          0xD7: erase 1k bytes (internal flash only).
//          0x20: erase 4k bytes.
//          0x52: erase 32k bytes.
//          0xD8: erase 64k bytes.
//
//      Addr: SPI Flash Address
//
//  Return Value:
//      None.
//-------------------------------------
#define spi_erase                            ((ROM_FUN_V_UC_UC_UL) ROM_CODE_BASE + 0x0190)

//-------------------------------------
//  Prototype:
//      void spi_write_enable(unsigned char Select, unsigned char Enable)
//
//  Description:
//      Write enable.
//  
//  Parameters:
//      Select: Indirect Selection
//          0x4F: internal flash.
//          0x0F: external flash.
//
//      Enable: Enable Write Status Reg. For SST flash chip (ID: 0xBF), this value must be set to 1.
//
//  Return Value:
//      None.
//-------------------------------------
#define spi_write_enable                        ((ROM_FUN_V_UC_UC) ROM_CODE_BASE + 0x01be)

//-------------------------------------
//  Prototype:
//      void spi_write_disable(unsigned char Select)
//
//  Description:
//      Write disable.
//  
//  Parameters:
//      Select: Indirect Selection
//          0x4F: internal flash.
//          0x0F: external flash.
//
//  Return Value:
//      None.
//-------------------------------------
#define spi_write_disable                          ((ROM_FUN_V_UC) ROM_CODE_BASE + 0x01ee)

//-------------------------------------
//  Prototype:
//      void spi_write_aai_word(BYTE Select, DWORD Addr, BYTE Buffer[], DWORD ByteCount);
//
//  Description:
//      Write data to flash (AAI word program).
//  
//  Parameters:
//      Select: Indirect Selection
//          0x4F: internal flash.
//          0x0F: external flash.
//
//      Addr: SPI Flash Address
//
//      Buffer[]: Pointer to data buffer.
//
//      ByteCount: Number of bytes to write.
//
//  Return Value:
//      None.
//-------------------------------------
#define spi_write_aai_word               ((ROM_FUN_V_UC_UL_PUC_UL) ROM_CODE_BASE + 0x0212)

//  
//  eFlash Control
//
//-------------------------------------
//  Prototype:
//      void eflash_to_direct_map(unsigned long Addr, unsigned char Index);
//
//  Description:
//      Copy code from embedded flash to direct map SRAM.
//  
//  Parameters:
//      Addr: SPI Flash Address
//
//      Index: SRAM segment index (0~12).
//
//  Return Value:
//      None.
//-------------------------------------
#define eflash_to_direct_map                    ((ROM_FUN_V_UL_UC) ROM_CODE_BASE + 0x027a)

//-------------------------------------
//  Prototype:
//      unsigned char eflash_fast_read_fn(unsigned char flag);
//
//  Description:
//      Enable or not the SPI Fast Read Function (HW).
//  
//  Parameters:
//      flag:
//          TRUE: Enable.
//          FALSE: Disable
//
//  Return Value:
//      TRUE : Enable.
//      FALSE: Disable.
//-------------------------------------
#define eflash_fast_read_fn                       ((ROM_FUN_UC_UC) ROM_CODE_BASE + 0x0000)

//
//  IMMU
//
//-------------------------------------
//  Prototype:
//      void immu_reset_ext(void)
//
//  Description:
//      IMMU tag SRAM reset.
//  
//  Parameters:
//      None.
//
//  Return Value:
//      None.
//-------------------------------------
#define immu_reset_ext                              ((ROM_FUN_V_V) ROM_CODE_BASE + 0x003a)

//-------------------------------------
//  Prototype:
//      unsigned char cpu_clock_select_ext(unsigned char Mode);
//
//  Description:
//      Select CPU clock.
//  
//  Parameters:
//      Mode: 
//          CPU_CLOCK_8MHz  = 1
//          CPU_CLOCK_16MHz = 2
//          CPU_CLOCK_24MHz = 3
//          CPU_CLOCK_32MHz = 4    
//          CPU_CLOCK_48MHz = 5    
//          CPU_CLOCK_64MHz = 6    
//          CPU_CLOCK_72MHz = 7
//          CPU_CLOCK_96MHz = 8
//
//  Return Value:
//      Mode is returned.
//-------------------------------------
#define cpu_clock_select_ext                      ((ROM_FUN_UC_UC) ROM_CODE_BASE + 0x039c)

#endif // } ! DEF_BUILD_ROM
//----------------------------------------------------------------------------
#endif //_ROM_H_

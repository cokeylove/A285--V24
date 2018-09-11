/*
 * ****************************************************************************
 *
 *  core_flash.h
 *
 *  Copyright (c) 2012- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/11/21
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */
#ifndef CORE_FLASH_H
#define CORE_FLASH_H

/*
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define SSTID					0xBF
#define SSTID_Old			    0x01

#define WinbondID				0xEF
#define AtmelID					0x9F
#define STID					0x20
#define SpansionID				0x01
#define MXICID					0xC2
#define AMICID					0x37
#define EONID					0x1C
#define ESMTID					0x8C

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void ITE_Flash_Utility(void);
extern void FlashECCode(void);
extern void Reset_Immu_Tag_Sram(void);
extern void Do_SPI_Read_Status(void);
extern void Do_SPI_Write_Status(BYTE statusvalue);
extern void Do_SPI_Read_ID(void);
extern void Do_SPI_Erase(BYTE EraseCmd, BYTE Addr2, BYTE Addr1, BYTE Addr0);
//extern void Do_SPI_Write_256Bytes(
//    BYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0);
extern void Do_SPI_Write_Enable(void);
extern void Do_SPI_Write_Disable(void);
extern void ECIndirectFastRead(
    BYTE add2, BYTE add1, BYTE add0,
    BYTE selection, WORD length, BYTE *bufferindex);
extern void Init_EC_Indirect_Selection(BYTE selection);
//extern void Direct_Map_Core_Flash(void);
//extern void CacheDma(BYTE sramnum,DWORD addr, BYTE option);
extern void eflash_rescan_signature(void);		// alex add 
extern void Mirror_Code_WriteOnce(void);
extern void SetSPIFlashPwr(BYTE pwr_on_off);
extern void spi_write_byte(BYTE u8IndirectSelection, DWORD u32SPIAddress,  BYTE *pBuffer, DWORD u32ByteCount);

extern void Eflash_CmdRamCode(void);


/* 
 * ****************************************************************************
 * structure
 * ****************************************************************************
 */
typedef struct DMAScratchSRAM
{
	uchar_8 *scarh	;	 
	uchar_8	*scarm	;
	uchar_8	*scarl	;
} sDMAScratchSRAM;

#endif


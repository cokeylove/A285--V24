/* 
 * ****************************************************************************
 *
 *  core_memory.c
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/14
 *
 *      Author: ITE00580, Dino Li
 *
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 * [Kernel Memory Rang] 0x00080000 ~ 0x000801FF 512 bytes
 *
 * 0x00080000-0x000800FF   kernel code RAM 0
 * 0x00080100-0x000801FF   kernel code RAM 1
 *
 * [OEM Memory Rang] 0x00080200 ~ 0x00080FFF 3.5K bytes
 *
 * 0x00080200-0x000802FF   OEM RAM 2
 * 0x00080300-0x000803FF   OEM RAM 3 
 * 0x00080400-0x000804FF   OEM RAM 4
 * 0x00080500-0x000805FF   OEM RAM 5
 * 0x00080600-0x000806FF   OEM RAM 6
 * 0x00080700-0x000807FF   OEM RAM 7
 * 0x00080800-0x000808FF   OEM RAM 8
 * 0x00080900-0x000809FF   OEM RAM 9
 * 0x00080A00-0x00080AFF   OEM RAM A
 * 0x00080B00-0x00080BFF   OEM RAM B
 * 0x00080C00-0x00080CFF   OEM RAM C
 * 0x00080D00-0x00080DFF   OEM RAM D 
 * 0x00080E00-0x00080EFF   OEM RAM E
 * 0x00080F00-0x00080FFF   OEM RAM F
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/* 
 * ****************************************************************************
 * global variables
 * ****************************************************************************
 */
const CBYTE *Scanner_Table_Pntr;
const CBYTE *Extendkey_Table_Pntr;
const CBYTE *Scanner_LOC_Pntr;

BYTE *SPIDataPointer;


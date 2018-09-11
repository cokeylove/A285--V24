//*****************************************************************************
//
//  oem_ver.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************
 
#ifndef OEM_Ver_H
#define OEM_Ver_H

//#define OEM_VERSION_RODATA __attribute__ ((section ("._OEM_VERSION_RODATA ")))

#define OEM_VERSION              __attribute__ ((section (".OEM_VERSION")))       

//extern const CBYTE OEM_VERSION P_model[];              	// Project information
//extern const CBYTE OEM_VERSION P_date[];             		// latest modified date
//extern const CBYTE OEM_VERSION P_rev[];             		// Project revision
//extern const CBYTE OEM_VERSION P_digit[];                   	// Project extended rev 7
//extern const CBYTE OEM_VERSION P_ID[]; 
//extern const CBYTE OEM_VERSION P_Proj[]; 
//extern const CBYTE OEM_VERSION P_BuildDate[];
extern const CBYTE OEM_VERSION ProjectInfoI[];
extern const CBYTE OEM_VERSION ProjectInfoI_Module[];
extern const CBYTE OEM_VERSION ProjectInfoII[];
extern const CBYTE OEM_VERSION ProjectInfoII_Module[];
extern const CBYTE OEM_VERSION CurBuildDate[];
extern const CBYTE OEM_VERSION CurBuildTime[];

/////////////////////////////////////////////////////////////
#define ProjectID0L   0x14  // Proect ID  (low byte)
#define ProjectID0H   0x00  // Proect ID  (high byte)
#define REV0_BYTE0    0x00  // Major number from 0 to 7
#define REV0_BYTE1    0x01  // 2nd version from 1 to 99
#define REV0_BYTE2    0x00  // 'A'   // Type of KB-BIOS from 'A' to 'Z'
#define REV0_BYTE3    0x01  // Verify byte from 0 to 15, 0 is ROM other is T01-T15
#define REV0_BYTE4    0x01  // Series number of EC F/W release

#define CHIP_TYPE0    0x0B  // 8186 - Chip type
#define tModuName0    "DAx85" // for module name


//Cmd_52_A0   report LCFC project name
#define OEM_PROJECT1  "DA285"			//"DA285"    //14"	Windu
#define OEM_PROJECT2  "DA285"			//12"	Wolvrine


//R04HTxxW  Windu AMD
//R0NHTxxW  Wolvrine AMD
#define OEM_BuildID1 'R'
#define OEM_BuildID2 '0'
#define OEM_BuildID3 'X'//( IS14Inch ? '4' : 'N' )
#define OEM_BuildID4 'H'
#define OEM_BuildID5 'T'
#define OEM_BuildID6 ((REV0_BYTE4 >> 4)+0x30)
#define OEM_BuildID7 ((REV0_BYTE4&0x0F)+0x30)
#define OEM_BuildID8 'W'


#endif

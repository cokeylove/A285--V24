//*****************************************************************************
//
//  oem_ver.c
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************
 
//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

/*
const OEM_VERSION_RODATA CBYTE oem_version_table[32] = 
{
    "Firmware Version"
};
*/
//----------------------------------------------------------------------------
// Project Information
//----------------------------------------------------------------------------
//const CBYTE OEM_VERSION P_model[]     = "Yoga5$";              	// Project information
//const CBYTE OEM_VERSION P_date[]      = "12/03/2015$";           // latest modified date
//const CBYTE OEM_VERSION P_rev[]       = "2JEC00T7$";         	// Project revision
//const CBYTE OEM_VERSION P_digit[]     = {01,02};                 // Project extended rev 7
//const CBYTE OEM_VERSION P_ID[]    	  = "COMPAL"; 
//const CBYTE OEM_VERSION P_Proj[]      = "KONA1"; 
//const CBYTE OEM_VERSION P_BuildDate[] = "Build Date:"__TIME__ " "__DATE__ "$";		// alex + for build date confirm

const CBYTE OEM_VERSION ProjectInfoI[] =
{
  ProjectID0L,
  ProjectID0H,
  REV0_BYTE0,
  REV0_BYTE1,
  REV0_BYTE2,
  REV0_BYTE3,
  REV0_BYTE4,

  CHIP_TYPE0,
  //OEM_PROJECT1,  // for module name
};
const CBYTE OEM_VERSION ProjectInfoI_Module[] = OEM_PROJECT1;

/*
const CBYTE OEM_VERSION ProjectInfoII[] =
{
  ProjectID0L,
  ProjectID0H,
  REV0_BYTE0,
  REV0_BYTE1,
  REV0_BYTE2,
  REV0_BYTE3,
  REV0_BYTE4,

  CHIP_TYPE0,
  //OEM_PROJECT2,  // for module name
};
const CBYTE OEM_VERSION ProjectInfoII_Module[] = OEM_PROJECT2;
*/
const CBYTE OEM_VERSION BUILD_ID[] =
{
  OEM_BuildID1,
  OEM_BuildID2,
  OEM_BuildID3,
  OEM_BuildID4,
  OEM_BuildID5,
  OEM_BuildID6,
  OEM_BuildID7,
  OEM_BuildID8,
};


const CBYTE OEM_VERSION CurBuildDate[] =
{
  __DATE__,
};

const CBYTE OEM_VERSION CurBuildTime[] =
{
  __TIME__,
};


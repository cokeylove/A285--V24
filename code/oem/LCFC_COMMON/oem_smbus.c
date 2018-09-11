//*****************************************************************************
//
//  oem_smbus.c
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

//----------------------------------------------------------------------------
// [rblk_bcis] the byte count of read smbus read block protocol
//----------------------------------------------------------------------------
void Hook_GetSMBusReadBlockByteCount(BYTE rblk_bc)
{
	//
  	// A Block Read or Write is allowed to transfer a maximum of 32 data bytes. 
  	//
  	if (rblk_bc > 0x20) {
    	rblk_bc = 0x20;
  	}
	uSMBusBcnt = rblk_bc;
}

/*
********************************************************************************
* Function name: Set_Smbus_Communication_Done
*
* Description:
*
*          This is used to set SMbus communication done to Status area.
*
* Arguments:
*    int           length 
*    unsigned char status : result of communication
* Return Values:
*    none
********************************************************************************
*/

void Set_Smbus_Communication_Done(BYTE length, BYTE status) {
  uSMBusBcnt  = length;
  Update_Smbus_Status(status | SMB_STS_DONE);
  uSMBusPrtcl = 0;
//	ECSMI_SCIEvent(AM_SMB_EVENT);  //BIOS not support this Q event?
}

/*
********************************************************************************
* Function name: Update_Smbus_Status
*
* Description:
*
*          This is used to modify Smbus Status byte.
*
* Arguments:
*    unsigned char status
* Return Values:
*    none
********************************************************************************
*/

void Update_Smbus_Status(BYTE status) {
    //CmnDisableInt();
    uSMBusSts = status | (uSMBusSts & SMB_STS_ALARM);
    //CmnEnableInt();
}




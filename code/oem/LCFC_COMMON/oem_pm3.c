//*****************************************************************************
//
//  oem_pm3.c
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

// 20160913 Build Port 86 +
//----------------------------------------------------------------------------
// Process Command/Data received from System via PMC3 interface
//----------------------------------------------------------------------------
void service_pci4(void)
{
	if ( IS_MASK_CLEAR(PM3STS,P_IBF) )
	{
		return;
	}
	
	if ( PM3STS & P_C_D)	// Command port
	{
		PM3Cmd	= PM3DI;  // Load command
		Hook_86Port(PM3Cmd);
		EnablePMC3IBFInt();
		
	}
}


//----------------------------------------------------------------------------
// The function of debuging 86 port
//----------------------------------------------------------------------------
void Hook_86Port(BYTE Cmd)
{
	SET_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
	if(IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepActive))
  {
    if (0 != (MSB_MASK & Cmd)){
			SET_MASK(EnhancedPcBeep_Status,BeepActive);
		}
		//SET_MASK(EnhancedPcBeep_Status,BeepBusy);
  	//EnhancedPcBeep_DATA[0] = 0x02;
  	EnhancedPcBeep_DATA[1] = Cmd;
  	//EnhancedPcBeep_ID = 0x01;
  }
  
  switch(Cmd)
  {
	
    default:
      break;
  }
}
// 20160913 Build Port 86 -


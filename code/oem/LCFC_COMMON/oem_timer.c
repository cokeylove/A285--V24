//*****************************************************************************
//
//  oem_timer.c
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

void ExternalWDTNow(void)
{
	ETWCFG = EWDKEYEN;
	EWDKEYR = 0x5A;						// External WDTreset
	while(1);
}


void InitClearExTimerHighByte3(void)		
{
	ET3CNTLH3R=0x00;
	ET4CNTLH3R=0x00;
	ET6CNTLH3R=0x00;
	ET8CNTLH3R=0x00;
}


//----------------------------------------------------------------------------
// Enable internal watch dog
//----------------------------------------------------------------------------
void EnableInternalWDT(void)
{
  Enable_External_WDT(1000);	// WDT Reset Time be 1sec.
}

//----------------------------------------------------------------------------
// Disable internal watch dog
//----------------------------------------------------------------------------
void DisableInternalWDT(void)
{
  Disable_External_WDT();        // Disable internal watch dog
}

//----------------------------------------------------------------------------
// Disable internal watch dog
//----------------------------------------------------------------------------
void ResetInternalWDT(void)
{
  Reset_External_WDT();        // Reset internal watch dog counter
}

void SystemAutoOn(void)
{
  if( (AutoTimer&0x7F) == 0x01 )
  {
    AutoTimer--;
    if( IS_MASK_SET(AutoTimer,b7AutoBootTm) )
    {
      AutoTimer = 0;  // Clear Auto timer.
      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
    }
    else
    {
      if ( SystemIsS5 )
      { AutoTimer = 0x83; }
      else
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BOOTFAIL_ID);
        AutoTimer = 0x83;

#if (PLATFORM_CHIP == INTEL)
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S0_S5;
#else // PLATFORM_CHIP
        pAMDPWRBTNCnt = AMDPWRBTNWK;
#endif // PLATFORM_CHIP
      }
    }
  }
  else
  {
    if( (AutoTimer&0x7F) > 0x01 )
    {
      if( IS_MASK_SET(AutoTimer,b7AutoBootTm) )
      {
        if ( SystemIsS5 )
        { AutoTimer--; }
      }
      else
      { AutoTimer--; }
    }
  }
}


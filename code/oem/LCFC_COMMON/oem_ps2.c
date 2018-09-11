//*****************************************************************************
//
//  oem_ps2.c
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



#ifdef Hook_Func_DisableTP     // For internal TB disable
//*****************************************************************************
// Hook function for disabling/enabling internal TB.
//
//  parameter :
//      none
//
//  return :
//      InternalTB_Disable : Internal TB is disabled.
//      InternalTB_Enable  : Internal TB is Enabled.
//
//  Note : Always return "InternalTB_Enable" if you don't understand the 
//          the use of this function.
//
//*****************************************************************************


// ----------------------------------------------------------------------------
// To define "Hook_ECACKMouseDriver" is necessary
// If define "Hook_ECACKMouseDriver" EC firmware need Respond mouse driver 
//  via Hook_ECRespondtoMouseDriver function
// ----------------------------------------------------------------------------
void Hook_ECRespondtoMouseDriver(BYTE mscmd)
{
    
}

// ----------------------------------------------------------------------------
// Hook kernel service_ps2 function
// ----------------------------------------------------------------------------
void Hook_service_ps2(BYTE ps2_channel, BYTE ps2_data)
{

}

// ----------------------------------------------------------------------------
// Disable port 0 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_0(void)
{
    DisablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Disable port 1 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_1(void)
{
    DisablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Disable port 2 ( send 0xF5 command to device)
// ----------------------------------------------------------------------------
void Hook_DisablePS2Port_2(void)
{
    DisablePS2Port_2();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_0(void)
{
    EnablePS2Port_0();
}

// ----------------------------------------------------------------------------
// Enable port 1 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_1(void)
{
    EnablePS2Port_1();
}

// ----------------------------------------------------------------------------
// Enable port 0 ( send 0xF4 command to device) 
// ----------------------------------------------------------------------------
void Hook_EnablePS2Port_2(void)
{
    EnablePS2Port_2();
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void Hook_TPOnlyLowLevelFunc(void)
{

}

void SendSynapticsIDSequence()
{
    BYTE Temp;

  Temp=0;
  do {
    Send2PortNWait(0x02, 0xE8, 1);
    Send2PortNWait(0x02, 0x00, 1);
    Temp++;
  }while(Temp<4);

  Send2PortNWait(0x02, 0xE9, 4);
}

void SendElanIDSequence()
{
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE6, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}

void SendAlpsIDSequence()
{
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE7, 1);
    Send2PortNWait(0x02, 0xE9, 4);
}

void ReadTPid()
{
    //BYTE Temp;
    SendSynapticsIDSequence();

    if(TPID3 == 0x47)
    {
      // TPID1 should be 0xFA, following 3bytes: 0x??, 0x47, 0x??
      TP_Type = 0x02;    // Synatics TouchPad.
    }
    else
    {
      SendAlpsIDSequence();
      // TPID1 should be 0xFA, following 3bytes: 0x73, 0x03, 0x??
      if ((TPID2 == 0x73) && (TPID3 == 0x03))
      {
          TP_Type = 0x04;  // Alps 3+2BCP.
      }


    }

}

BYTE Hook_DisableInternalTB(void)
{
    BYTE result;

    #if TouchPad_only               // For Touchpad only project.
    
    result = InternalTB_Enable;     // return internal tb enable or disable here by condition.
    
    #else
    result = InternalTB_Enable;     // Always return Internal TB is Enabled here.
    #endif
    
    return(result);
}
#endif



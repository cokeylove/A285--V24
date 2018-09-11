//*****************************************************************************
//
//  oem_pm2.c
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
// The function of debuging 68 port
//----------------------------------------------------------------------------
void Hook_68Port(BYTE p_PM2Data)
{
#if Support_RFID
  BYTE i,j;		
#endif  //Support_RFID  
  switch(p_PM2Data)
  {
#if  Support_RFID 
	case 0x01:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_01(PM2Data1,PM2Data,&j);
		if( i == TRUE )
			Data2Port(0x68,j);
	  }
	  break;
	case 0x02:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_02(PM2Data2,PM2Data1,PM2Data);
		if( i == TRUE )
			Data2Port(0x68,0);
	  }
	  break;
	case 0x09:		
	  if( PM2DataCount == 0x01 )
	  {
	  	i = Cmd_09(PM2Data,&j);
		if( i == TRUE )
			Data2Port(0x68,j);
	  }
	  break;
    case 0x0A:		
	  if( PM2DataCount == 0x01 )
	  {
	  	Cmd_0A(PM2Data1,PM2Data);
	  }
	  break;
#endif //Support_RFID
    case 0xB1:
      if( PM2DataCount == 0x01 )
      {
        Cmd_B1(PM2Data1,PM2Data);
      }
      break;
    case 0xB7:
      if( PM2DataCount == 0x01 )
      {
        Cmd_B7(0x68,PM2Data);
      }
      break;
   
   //A285D00060 start+
   #if ENABLE_64G_KEY_WORK_RUN
   case 0xBC:
        //USB_ON_INPUT;
         USB_ON_OUTPUT;
         USB_ON_LOW();
         AOU_EN_OFF();
        USB_Delay = PM2Data;
        if(USB_Delay == 0)
        {USB_Delay = 0x0A;}
	 break;
   #endif    
   //A285D00060 end+
  }
}

//----------------------------------------------------------------------------
// The function of debuging 6C port
//----------------------------------------------------------------------------
void Hook_6CPort(BYTE p_PM2Cmd)
{
  switch(p_PM2Cmd)
  {
#if Support_RFID
  	case 0x01:		
  	  PM2DataCount=0x02;
	  break;
	case 0x02:		
	  PM2DataCount=0x03;
	  break;
	case 0x09:		
	  PM2DataCount=0x01;
	  break;
	case 0x0A:		
	  PM2DataCount=0x02;
	  break;
#endif //Support_RFID
	#if Support_LenovoAPS
    case 0x10:
      APS_Cmd_10(0x68);
      break;
    case 0x11:
      APS_Cmd_11(0x68);
      break;
    case 0x12:
      APS_Cmd_12(0x68);
      break;
    case 0x13:
      APS_Cmd_13(0x68);
      break;
    case 0x14:
      APS_Cmd_14(0x68);
      break;
    case 0x15:
      APS_Cmd_15(0x68);
      break;
    case 0x16:
      APS_Cmd_16(0x68);
      break;
    case 0x17:
      APS_Cmd_17(0x68);
      break;
    case 0x18:
      APS_Cmd_18(0x68);
      break;
    #endif  // Support_LenovoAPS
    case 0xB1:
      PM2DataCount=0x02;
      break;
//[Project][CC20130423][Start] Add EC idle function for 68/6C port.
    case 0xB4:
      Cmd_B4();
      break;
//[Project][CC20130423][End] Add EC idle function for 68/6C port.
    case 0xB7:
      PM2DataCount=0x01;
      break;
    //A285D00060 start+
   #if ENABLE_64G_KEY_WORK_RUN
   case 0xBC:
        PM2DataCount=0x01;
	 break;
   #endif    
   //A285D00060 end+
    default:
      break;
  }
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via 68/6C interface
//----------------------------------------------------------------------------
void service_pci3(void)
{
  if ( IS_MASK_CLEAR(PM2STS,P_IBF) )
  {
    return;
  }

  if ( PM2STS & P_C_D)  // Command port
  {
    PM2Cmd  = PM2DI;  // Load command
    EnablePMCIBFInt();
    Hook_6CPort(PM2Cmd);
  }
  else          // Data port
  {
    PM2Data2 = PM2Data1;
    PM2Data1 = PM2Data;
    PM2Data = PM2DI;
    if(PM2DataCount==0x00)
    {
      
      EnablePMCIBFInt();
    }
    else
    {
      
      EnablePMCIBFInt();
      if (PM2DataCount!=0x00)
      {
        Hook_68Port(PM2Cmd);
        PM2DataCount--;
      }
    }
  }
}




//*****************************************************************************
//
//  oem_acpi.c
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

void Data_To_ACPI(BYTE data_byte)
{
	PM1DO = data_byte;
}


/* ----------------------------------------------------------------------------
 * FUNCTION: SMI_Gen_Int - Generate an SMI interrupt.
 * ------------------------------------------------------------------------- */
void SMI_Gen_Int(void)
{
	if (SystemIsS0)
	{
		Hook_SMION();
		Delay1MS(5);
		Hook_SMIOFF();
	}
}

/////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// The hook function of 62 port data
//  Note :
//  PM1Step != 0x00 is necessary.
//----------------------------------------------------------------------------
void Hook_62Port(BYTE p_PM1Cmd)
{
  BYTE i;
#if   Support_RFID
  BYTE j;
#endif //Support_RFID  

  switch(PM1Cmd)
  {
#if   Support_RFID
	case 0x01:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_01(PM1Data1,PM1Data,&j);
		if( i == TRUE )
			Data2Port(0x62,j);
	  }
	  break;
	case 0x02:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_02(PM1Data2,PM1Data1,PM1Data);
		if( i == TRUE )
			Data2Port(0x62,0x00);
	  }
	  break;
	case 0x09:		
	  if( PM1Step == 0x01 )
	  {
	  	i = Cmd_09(PM1Data,&j);
		if( i == TRUE )
			Data2Port(0x62,j);
	  }
	  break;
    case 0x0A:		
	  if( PM1Step == 0x01 )
	  {
	  	Cmd_0A(PM1Data1,PM1Data);
	  }
	  break;
#endif //Support_RFID
    case 0x40:
      Cmd_40(PM1Data);
      break;
    case 0x41:
      if(PM1Data==0xA1) Cmd_41_A1(0x62);
      break;
    case 0x42:
      Cmd_42(PM1Data);
      break;
    case 0x43:
      Cmd_43(0x62,PM1Data);
      break;
    case 0x45:
       Cmd_45(0x62,PM1Data);
       break;
    case 0x46:
      break;
    case 0x47:
      Cmd_47(0x62,PM1Data);
      break;
    case 0x49:
      break;
    case 0x4B:
      if (PM1Step == 0x04)
      {
        i = PM1Data;
        break;
      }
      if (PM1Step == 0x03)
      {
        eEEPROMAddrsss = PM1Data;
        if ( PM1Data == 0xE0 )  // Check MBID address high byte.
        { SET_MASK(MBID_Reload, b0MBID_High); }
        break;
      }
      if (PM1Step == 0x02)
      {
        eEEPROMBank = PM1Data | EEPROM_Bank0;  
        if ( PM1Data == 0x07 )  // Check MBID address low byte.
        { SET_MASK(MBID_Reload, b1MBID_LOW); }
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMData  = PM1Data;
        if ( (MBID_Reload & 0x03) == 0x03 )
        { uMBID = PM1Data; }
        Update_EEPROMMark();
        Data2Port(0x62, 0x00);
      }

      if ( (MBID_Reload & 0x03) == 0x03 )
      {
        LoadKBMatrixToRAM();  // Reload KB matrix.
        MBID_Reload = 0;
      }
      else
      { MBID_Reload = 0; }
      break;

    case 0x4C:
      if (PM1Step == 0x03)
      {
        i = PM1Data;
        break;
      }
      if (PM1Step == 0x02)
      {
        eEEPROMAddrsss = PM1Data;
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMBank = PM1Data | EEPROM_Bank0;  
        Cmd_4E(0x62,eEEPROMAddrsss);
      }
      break;

    case 0x4D:
      if (PM1Step == 0x02)
      {
        eEEPROMAddrsss = PM1Data;
        break;
      }
      if (PM1Step == 0x01)
      {
        eEEPROMData  = PM1Data;
        Update_EEPROMMark();
        Data2Port(0x62, 0x00);
      }
      break;

    case 0x4E:
      eEEPROMAddrsss = PM1Data;
      Cmd_4E(0x62,eEEPROMAddrsss);
      break;

    case 0x51:
      break;

    case 0x52:
      if(PM1Data==0xA0) Cmd_52_A0(0x62);
      if(PM1Data==0xA3) Cmd_52_A3(0x62);
      if(PM1Data==0xA4) Cmd_52_A4(0x62);
      if(PM1Data==0xA5) Cmd_52_A5(0x62);
      break;

    case 0x53:
      if (PM1Step == 0x01)
      {
        Cmd_53(0x62,PM1Data2,PM1Data1,PM1Data);
      }
      break;
    case 0x56:
      break;
    case 0x58:
      Cmd_58(PM1Data);
      break;
    case 0x59:
      Cmd_59(0x62,PM1Data,0);
      break;
#if EC_Brightness_Ctrl      
    case 0x5C:
      Cmd_5C(0x62);
      break;
    case 0x5D:
      Cmd_5D(PM1Data);
      break;
#endif      
    case 0xB0:
      Cmd_B0(0x62,PM1Data);
      break;

    case 0xB1:
      Cmd_B1(PM1Data1,PM1Data);
      break;      
//[Project][CC20130425][Start]Add first used date feature.
    case 0xB2:
      if (PM1Step == 0x01)
      {
        Cmd_B2(PM1Data2,PM1Data1,PM1Data);
      }
      break;
//[Project][CC20130425][End]Add first used date feature.
    case 0xB3:
      if (PM1Step == 0x01)
      {
        Cmd_B3(0x62,PM1Data1,PM1Data);
      }
      break;   
   //A285D00060 start+
    #if ENABLE_64G_KEY_WORK_RUN
   case 0xBC:
        //USB_ON_INPUT;
        USB_ON_OUTPUT;
         USB_ON_LOW();
         AOU_EN_OFF();
        USB_Delay = PM1Data;
        if(USB_Delay == 0)
        {USB_Delay = 0x0A;}
	 break;
   #endif
   
   //A285D00060 end+
  }
}

//----------------------------------------------------------------------------
// The hook function of 66 port command.
//----------------------------------------------------------------------------
void Hook_66Port(BYTE p_PM1Cmd)
{
  switch(PM1Cmd)
  {
#if Support_RFID
  	case 0x01:		
  	  PM1Step=0x02;
	  break;
	case 0x02:		
	  PM1Step=0x03;
	  break;
	case 0x09:		
	  PM1Step=0x01;
	  break;
	case 0x0A:		
	  PM1Step=0x02;
	  break;
#endif //Support_RFID
    case 0x40:
      PM1Step=0x01;
      break;
    case 0x41:
      PM1Step=0x01;
      break;
    case 0x42:
      PM1Step=0x01;
      break;
    case 0x43:
      PM1Step=0x01;
      break;
    case 0x44:
      Cmd_44(0x62);
      break;
    case 0x45:
      PM1Step=0x01;
      break;
    case 0x46:
    break;
    case 0x47:
      PM1Step=0x01;
      break;
    case 0x49:
    break;
    case 0x4B:
      PM1Step=0x04;
      break;
    case 0x4C:
      PM1Step=0x03;
      break;
    case 0x4D:
      PM1Step=0x02;
      break;
    case 0x4E:
      PM1Step=0x01;
      break;

    case 0x4F:
      Erase_EEPROMAll();
      break;

    case 0x51:
      Cmd_51(0x62);
      break;
    case 0x52:
      PM1Step=0x01;
      break;
    case 0x53:
      PM1Step=0x02;
      break;
    case 0x56:
      Cmd_56(0x62);
      break;
    case 0x58:
      PM1Step=0x01;
      break;
    case 0x59:
      PM1Step=0x01;
      break;
    case 0x5C:
      break;
    case 0x5D:
      PM1Step=0x01;
      break;
    case 0xB0:
      PM1Step=0x01;
      break;
    case 0xB1:
      PM1Step=0x02;
      break;      
//[Project][CC20130425][Start]Add first used date feature.
    case 0xB2:
      PM1Step=0x03;
      break;
//[Project][CC20130425][End]Add first used date feature.
    case 0xB3:
      PM1Step=0x02;
      break;
    //A285D00060 start+
    #if ENABLE_64G_KEY_WORK_RUN
   case 0xBC:
        PM1Step=0x01;
	 break;
   #endif
   
   //A285D00060 end+
  }
}


#if ACPI_Smbus
BYTE vOSSMbusWBlcok(void)
{
  return (bWSMBusBlock(ACPISMBch, SMbusWBK, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, uSMBusBcnt,SMBus_NeedPEC));
}

BYTE vOSSMbusRBlcok(void)
{
  return (bRSMBusBlock(ACPISMBch, SMbusRBK, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, bRead_I2C_NULL));
}

BYTE vOSSMbusRByte(void)
{
  return (bRWSMBus(ACPISMBch, SMbusRB, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusRWord(void)
{
  return (bRWSMBus(ACPISMBch, SMbusRW, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusWBYTE(void)
{
  return (bRWSMBus(ACPISMBch, SMbusWB, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, SMBus_NeedPEC));
}

BYTE vOSSMbusWWord(void)
{
  return (bRWSMBus(ACPISMBch, SMbusWW, uSMBusAddr, uSMBusCmd,(BYTE *) &uSMBusData0, SMBus_NeedPEC));
}

BYTE DummyFunction(void)
{
	return 0;
}

const FUNCT_PTR_B_V vOSSMbusComd_table[16] =
{                //the low nibble of uSMBusPrtcl
  DummyFunction,    //0x00
  DummyFunction,    //0x01
  DummyFunction,    //0x02-Write Quick Command
  DummyFunction,    //0x03-Read Quick Command
  DummyFunction,    //0x04-Send Byte
  DummyFunction,    //0x05-Receive Byte
  vOSSMbusWBYTE,    //0x06-Write Byte
  vOSSMbusRByte,    //0x07-Read Byte
  vOSSMbusWWord,    //0x08-Write Word
  vOSSMbusRWord,    //0x09-Read Word
  vOSSMbusWBlcok,    //0x0A-Write Block
  vOSSMbusRBlcok,    //0x0B-Read Block
  DummyFunction,    //0x0C-Process Call
  DummyFunction,    //0x0D-Write Block-Read Block Process Call
  DummyFunction,    //0x0E
  DummyFunction    //0x0F
};

// ACPI response to writes to SMBus Protocol register.
void OSSMbusComd(void)
{

	if( (uSMBusPrtcl>=0x01) &&  ((uSMBusPrtcl&0xF0)!= 0x10))	//Due to battery APP will send the 8xH protocol cmd and not 0xH.Setting 0xH and 2xH~FxH are all support SMBus process.
  	{
    	uSMBusSts = 0x00;
    ACPISMBch = chSMB_BAT;
    
    if (SMB_BAT_Exception_Addr == FALSE){
	    if( (uSMBusAddr == SecSmBat_Addr) ){
    	    if (PriBattInhibGET){    //main battery
    			  uSMBusAddr = SmBat_Addr;
    	    }  
	    }
  	  }

    if (SMB_BAT_Exception_Addr){    //for battery FW update
	    if( (uSMBusAddr == SMB_BAT_Exception_Addr) ){
    	    if (SecBattInhibGET){    //second battery
    	      ACPISMBch = chSMB_2ndBAT;
    	    }  
	    }
  	  }
	  
//WA for LB battery FW update	  
	if((uSMBusAddr == 0x16))
	{
    if(BATFWupdate_Delay != 0x00)
		  {return;}
	  if(((uSMBusCmd == 0x36)&&((uSMBusData2 < 0x04))&&((uSMBusPrtcl&0x0F)== 0x0A))
	    ||((uSMBusCmd == 0x35)&&((uSMBusData1 == 0x20))&&((uSMBusPrtcl&0x0F)== 0x08)))
		  {BATFWupdate_Delay = 8;}	
	}	  
	  
        
    if((vOSSMbusComd_table[(uSMBusPrtcl & 0x0F)])()) {
		 	uSMBusBcnt = tmpACPISMBBCnt;
       uSMBusSts = 0x80;
     }else {
       if( IS_MASK_SET(HOSTA_B,BIT0) ) { uSMBusSts = 0x1A; }
       else if( IS_MASK_SET(HOSTA_B,BIT2) ) { uSMBusSts = 0x17; }
       else if( IS_MASK_SET(HOSTA_B,BIT3) ) { uSMBusSts = 0x07; }
       else if( IS_MASK_SET(HOSTA_B,BIT4) ) { uSMBusSts = 0x19; }
       else if( IS_MASK_SET(HOSTA_B,BIT5) ) { uSMBusSts = 0x10; }
       else if( IS_MASK_SET(HOSTA_B,BIT6) ) { uSMBusSts = 0x18; }
    	}
     uSMBusPrtcl = 0x00;
    //ECSMI_SCIEvent(ACPI_ACIN_SCI);  // toggle HOST
    }
}

#endif  // ACPI_Smbus

#define ACPI_I2CBus	TRUE
#ifdef ACPI_I2CBus
BYTE vOSI2CWrite(void)
{
	return(I2C_Write(chSMB_TYPEC, uSMBusAddr, uSMBusBcnt,&uSMBusCmd));		
}

BYTE vOSI2CRead(void)
{
	return(I2C_Read(chSMB_TYPEC, uSMBusAddr, uSMBusBcnt,&uSMBusData0, 0x00));	
}

BYTE vOSI2CWrite2Read(void)
{
	rI2C_DATA = uSMBusCmd;
	rI2C_DATA1[0] = uSMBusData0;			
	return(I2C_Write_to_Read(chSMB_TYPEC, uSMBusAddr, 0x02,&rI2C_DATA, uSMBusBcnt,&uSMBusData0, 0));					
}

const FUNCT_PTR_B_V vOSI2CbusComd_table[16] =
{                	//the low nibble of uSMBusPrtcl
  DummyFunction,    //0x00
  DummyFunction,    //0x01
  DummyFunction,    //0x02
  DummyFunction,    //0x03
  DummyFunction,    //0x04
  DummyFunction,    //0x05
  vOSI2CWrite,    	//0x06-I2C Write
  vOSI2CRead,    	//0x07-I2C Read
  vOSI2CWrite2Read, //0x08-I2C Write to Read
  DummyFunction,    //0x09
  DummyFunction,    //0x0A
  DummyFunction,    //0x0B
  DummyFunction,    //0x0C
  DummyFunction,    //0x0D
  DummyFunction,    //0x0E
  DummyFunction    	//0x0F
};

// ACPI response to writes to SMBus Protocol register.
void OSI2CbusComd(void)
{
	if( (uSMBusPrtcl>=0x11) &&  (uSMBusPrtcl<= 0x1F))
  	{
    	uSMBusSts = 0x00;
        if((vOSI2CbusComd_table[(uSMBusPrtcl & 0x0F)])())
		{
         	uSMBusSts = 0x80;
        }
		else
		{
        if( IS_MASK_SET(HOSTA_A,BIT0) ) { uSMBusSts = 0x1A; }
        else if( IS_MASK_SET(HOSTA_A,BIT2) ) { uSMBusSts = 0x17; }
        else if( IS_MASK_SET(HOSTA_A,BIT3) ) { uSMBusSts = 0x07; }
        else if( IS_MASK_SET(HOSTA_A,BIT4) ) { uSMBusSts = 0x19; }
        else if( IS_MASK_SET(HOSTA_A,BIT5) ) { uSMBusSts = 0x10; }
        else if( IS_MASK_SET(HOSTA_A,BIT6) ) { uSMBusSts = 0x18; }
    }
        uSMBusPrtcl = 0x00;

    //ECSMI_SCIEvent(ACPI_ACIN_SCI);  // toggle HOST
    }
}

#endif  // ACPI_I2CBus

//----------------------------------------------------------------------------
// Hook function of ACPI command
//----------------------------------------------------------------------------
void Hook_ACPICommand(void)
{
	BYTE length;
	BYTE status;
  if (EC_I2C_ADDR == uSMBusAddr)
  {
    if(0x7f & uSMBusPrtcl)
    {
    	status = EcCmd((0x7f & uSMBusPrtcl), uSMBusCmd, uSMBusBcnt, &uSMBusData0, &length);
    	Set_Smbus_Communication_Done(length, status);
    }
  }
#if ACPI_Smbus
  OSSMbusComd();    // ACPI SMBus command  
#endif
	
#ifdef ACPI_I2CBus
	OSI2CbusComd();	
#endif	
}

//-------------------------------------------------------------------------------
// parameter "QeventSCI" only, For a pulsed SCI
//------------------------------------------------------------------------------
void OEM_ACPI_Gen_Int(void)
{
    ACPI_Gen_Int(QeventSCI);
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void ResetSCIEvent(void)
{

    CLEAR_MASK(PM1STS,SCIEVT);
    ECCheckBurstMode = 0;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: ECQEvent -Setup cause flag for an SCI and start the ACPI_Timer.
 *
 * sci_number = 1 through 255.  The Host uses this number to determine the
 * cause of the SCI.
 * ------------------------------------------------------------------------- */
void ECQEvent(BYTE sci_number)
{
  if(IS_MASK_CLEAR(SYS_MISC1,ACPI_OS) || (SysPowState!=SYSTEM_S0))
  {
    return;
  }

  ITempB05 = SCI_Event_In_Index;       // Get the input index.
  SCI_Event_Buffer[ITempB05] = sci_number; // Put the SCI number in the buffer.
   ITempB05++;                  // Increment the index.
   if (ITempB05 >= EVENT_BUFFER_SIZE)
  {
       ITempB05 = 0;
   }
                       // If the buffer is not full, update the input index.
    if (ITempB05 != SCI_Event_Out_Index)
  {
       SCI_Event_In_Index = ITempB05;
    }
  SET_MASK(PM1STS,SCIEVT);
    OEM_ACPI_Gen_Int();
}

void ECSMI_SCIEvent(BYTE smi_number)
{
  if(SystemIsS0)  //A485D00008: Add for SMI leakage in DC S5
  {
   if(IS_MASK_SET(SYS_MISC1,ACPI_OS))
     ECQEvent(smi_number);
   else
     ECSMIEvent(smi_number);
  }
}

/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  <Function Name> SMI_INTERRUPT

  Parameters:   None

  Returns:     None

  Description:   Generate SMIs for Query event request
-----------------------------------------------------------------------------*/
void SMI_INTERRUPT(void)
{
  DisableAllInterrupt();
    SMI_ON();        // set SMI pin low
  //WNCKR = 0x00;           // Delay 15.26 us
    Delay1MS(1);		// Delay 1ms
    SMI_OFF();        // set SMI pin high
    EnableAllInterrupt();
}
/**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  <Function Name> ECSMIEvent

  Parameters:   [NewSMI] -- SMI Query Event Value

  Returns:     [1]:TRUE
          [0]:FALSE

  Description:   Write SMI Query Value to Buffer
-----------------------------------------------------------------------------*/
BYTE ECSMIEvent(BYTE NewSMI)
{

    if( NewSMI == 0x00 )                    //check input SMI Number
    return FALSE;

  pSMIEvent = NewSMI;
    SET_MASK(PM1STS,BIT6);                  //Set SMI Query flag
    SMI_INTERRUPT();                    //Generate SMI Interrupt
    return FALSE;

}


//----------------------------------------------------------------------------
// Read EC ram Space
//----------------------------------------------------------------------------
BYTE Hook_ReadMapECSpace(BYTE MapIndex)
{

  if (MapIndex == 0xCE){
    RamDebug(0x62);
    RamDebug( *((BYTE *)(ECNameSpace+MapIndex)) );    
  }
  
  return( *((BYTE *)(ECNameSpace+MapIndex)) );
}

//----------------------------------------------------------------------------
// Write EC ram Space
//----------------------------------------------------------------------------
void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1)
{
  *((BYTE *)(ECNameSpace+MapIndex)) = data1;

  if (MapIndex == 0xCE){
    RamDebug(0x66);
    RamDebug(data1 );    
  }
  
  switch( MapIndex )   
  {
    case 0x02:
      Chk_pDevStus();
    break;
    case 0x03:   //u4Cfgspace3
    
      if (data1 & BIT2) {
        if (M_BATStopCharge == 100) { // default value
          uThresholdSetting = 0x00;
        } else {
          uThresholdSetting = M_BATStopCharge;
        }

        if (S_BATStopCharge == 100) { // default value
          uThreshold2ndSetting = 0x00;
        } else {
          uThreshold2ndSetting = S_BATStopCharge;
        }
        
      } else {
          uThresholdSetting = M_BATStartCharge;    
          uThreshold2ndSetting = S_BATStartCharge;              
      }

      // For BIT3 Airplane mode.
      if (IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady)) {
         Update_ChargeIC();
         SetPowerBatteryparameter();
      }
    break;

    case 0x0C:   //u4LEDcontrol
      TPG_LED_control();
    break;

    case 0x0D:
#if Support_KBD_BL_LED
			AcpiWriteKbdBacklightLED();
#endif //Support keyboard backlight

#if Support_TP_RESET_control      
      if (RST_TRACK_POINT_GET)
        TP_RESET_ON();
      else 
        TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
      if (BYPASS_CLICKPAD_GET)
        BYPASS_PAD_ON();
      else
        BYPASS_PAD_OFF();  
      break;  
#endif //Support_BYPASS_PAD_control      

      Chk_pDevStus();  

    case 0x22:   //u4InhChgTimerH
      if( (u4InhChgTimerL==0xFF) && (u4InhChgTimerH==0xFF) )
      {
        pInhitTimer=0xFFFF;
      }
      else
      {
        pInhitTimer = (u4InhChgTimerL + ( u4InhChgTimerH * 256 ))*30;  // 30sec
        if (pInhitTimer == 0) {
          PriBattinhibitDis;
          SecBattinhibitDis;
          Inhibit_chargingCLR;
          CLEAR_MASK(M_BATStopChgStatL,ENCMDSTOPCHG);
        }
      }
    break;
    
    case 0x23:   //u4ExtCommand
      TPG_EXTBATCmd();
    break;
    
    case 0x24:   //uThresholdSetting
      EDGEPMBATTProcess(M_BAT);
    break;
    case 0x25:   //uThreshold2ndSetting
      EDGEPMBATTProcess(S_BAT);
    break;
	
    case 0x26:   //Tamper status
    //Bottom tamper
#if Support_BOTTOM_TAMPER_FUNC    
    ClearBottomTamper(data1);
#endif //Support_BOTTOM_TAMPER_FUNC      
    break;

    case 0x2F:   //fan control
      if (!FanAutomaticmodeGet){
        FAN_PWM = FAN_PWM_Min;   //WA for MFG test tool
        Fan_control_update();
      }
    break;
  
    case 0x3D:   //u4PassCtrl
      EdgePasswordProcess();
    break;

	case 0x50:   //uSMBusPrtcl		// A485D00023:Modify Hook_ACPICommand() hook point
      Hook_ACPICommand();
	  break;

    case 0x81:   //u4BattInfoID
      IBMbatteryStore();
    break;

    case 0x8F:   //fan control
      Fan_control_update();
    break;

    case 0xB0:   
    case 0xB1:   
      if ((u4MainBattChgOnCap == 0) && (u4MainBattChgOffCap == 0)){
        CLEAR_MASK(M_BATStopChgStatL, ENB0B3STOP);
      }
    break;

    case 0xB2:   
    case 0xB3:   
      if ((u42ndBattChgOnCap == 0) && (u42ndBattChgOffCap == 0)){
        CLEAR_MASK(S_BATStopChgStatL, ENB0B3STOP);
      }
    break;
    
  }
}







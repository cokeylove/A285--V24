/*----------------------------------------------------------------------------
 * Filename: OEM_Power.C  For Chipset: ITE.IT85XX
 *
 * Function: Power sequnce control function example
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

// Include all header file
#include "..\..\include.h"
//

BYTE Sequence_Null(void)
{
	return 0;
}


void Charge_inhibit2sec(void)
{
  inhibit2sec = 20;    // 100ms*20 = 2000ms
  SET_MASK(M_BATStopChgStatL,ENCHARGESUSP);    // use for detect battery charging suspend
  CHGIC_ptr = 2;
  WriteSmartChgIC();		
  if(SysPowState == SYSTEM_S3_S0){
    SET_MASK(pProject3,uReSendBat1ChgQevent);
  }
}




void TrigRsmrstInDcMode (
  void
)
{
  if (IS_MASK_SET(SYS_STATUS,AC_ADP) || IS_MASK_SET(cCmd,cC_PCHPWR_Keep)) {
    return;
  }

  RSMRST_LOW();
  //
  // By EE's request, pull low EC_WAKE# and PWRBTN_OUT to 'low' to avoid current leakage at S4 in DC mode.
  //
  EC_WAKE_OUT_LOW();
  CLEAR_MASK(SysStatus,LidStatus);
  SET_MASK(S0S5PWRcodition,S0S5PWROFF);

}

void SetS0GPIO(void)
{
  AOURSTCNT = 10;
  CLEAR_MASK(S0S5PWRcodition,S0S5PWROFF);
  SCI_HI();          //RUNSCI Hi
  SMI_OFF();
  v3VLAN_EN_ON();
  v3VWLAN_EN_ON();

  EC_RTCRST_OFF(); // A485D00009 Add for EE requirement

#if Support_GATEA20
  GATEA20_HI();      //GA20 Hi
#endif   
  KBRST_OFF();

#if SW_ISCT
  if(IS_MASK_CLEAR(AOAC_STATUS, ISCT_S0_ISCT))
  {
    AOAC_STATUS = 0xC0;    // Clear ISCT status.
    ISCT_Timer = 0;
  }
  AOAC_PWRON();      // Turn on WLAN power.
#endif // SW_ISCT

#if Support_EC_GS_ON
    EC_GS_ON();
#endif // Support_EC_GS_ON

  TP_CLK_ALT;          // TP_CLK to alternate pin
  TP_DATA_ALT;         // TP_DATA to alternate pin
  SMBus3_CLK_ALT;      // SMBus3 CLK to alternate pin
  SMBus3_DAT_ALT;      // SMBus3 DATA to alternate pin

#if FAN_TABLE_Already
  FAN_PWM_ALT;      // Set FAN_PWM Alt.
#else
  FAN_PWM_OUT;      // Set FAN_PWM OUTPUT.
  EC_FAN_PWM_HI();
#endif  // FAN_TABLE_Already

  FAN_SPEED_ALT;      // Set FAN Speed Alt.
  InitSio();

  KBHISR = 0x00;
  KBHIKDOR = 0x00;
  CLEAR_MASK(KBHISR,SYSF);

  SPCTRL1 = 0x83;      //msmart test
  BRAM3E =0;
  BRAM3F =0;

  Charge_inhibit2sec();

  //
  // Clear thermal error count.
  //
  TMErrCnt = 0;
  VGAErrCnt = 0;
}

void SetS5GPIO(void)
{
  SCI_LOW();      //RUNSCI Hi
  SMI_ON();
  //// A485D00031: Enable AC/DC wake on Lan function --->Start
  if(WOLACWakeEnGet||WOLDCWakeEnGet)
  	{v3VLAN_EN_ON();}
 else
 	{v3VLAN_EN_OFF();}
 // A485D00031:--->End
  v3VWLAN_EN_OFF();

  EC_RTCRST_ON();  // A485D00009 Add for EE requirement
#if Support_GATEA20
  GATEA20_LOW();    //GA20 Hi
#endif  
  KBRST_ON();

  TP_CLK_INDW;    // TP_CLK to Input+pull_DW pin
  TP_DATA_INDW;    // TP_DATA to Input+pull_DW pin
  SMBus3_CLK_INDW;  // SMBus3 CLK to Input+pull_DW pin
  SMBus3_DAT_INDW;  // SMBus3 DATA to Input+pull_DW pin
  FAN_PWM_INDW;    // Set FAN_PWM input+pull_dw.
  FAN_SPEED_INDW;    // Set FAN Speed input+pull_dw.


#if SW_ISCT
  if ( (IS_MASK_CLEAR(AOAC_STATUS, ISCT_Enable))||(SysPowState != SYSTEM_S0_S3) )  // Check ISCT enable?
  {
    AOAC_PWROFF();    // Turn off WLAN power.
#if Support_WLAN_ON_OFF_control
    BTRF_OFF_OFF();
#endif //Support_WLAN_ON_OFF_control
  }
#endif // SW_ISCT

  APSHDDFLAG = 0;
//#if Support_EC_GS_ON
  EC_GS_OFF();
//#endif

#if !Support_AOU5_V1_2
  if ( SysPowState != SYSTEM_S0_S3 )
  {
    //[Project][CC20130508][Start] Modify power behave on Non-AOU USB port.
    if((Read_AC_IN())&&(USB_PWRGET))
    {
  		USB_ON_ON();
    } // Turn on USB power.
    else
    {
      USB_ON_OFF();
    } // turn off USB power.
  }
  else
  {
    if( ((TurnOnUSBGET) || (USB_PWRGET))&&((Read_AC_IN())||(RelativeStateOfChgL > NO_USBDevice_BAT_LOW)))
    {
  		USB_ON_ON();  // Turn on USB power.
    }
    else
    {
      USB_ON_OFF();
    } // turn off USB power.
  }
#endif

  AC_IN_INPUT;
  if( nSwitchableBTNGET )
  {
    SET_MASK(pProject0,b7SWITCHBAK);
  }
  else
  {
    CLEAR_MASK(pProject0,b7SWITCHBAK);
  }
  nSwitchableBTNDIS;    //UMA
}

//============S5 -> S0==========================================================
//
// AMD Power Sequence Subroutines
//
BYTE S5S0_STEP1(void)
{
  GPOTF = 0x00;
  PM_PWRBTN_HI();
  EC_ON_HI();
  //EC_ON2_HI();  //David- for the pin change to KBID
  cPWRONFail = 0x00;
  
  return 0;
}

BYTE S5S0_CHK_VDDPALWPWRGD(void)
{
  if(Read_VDDPALWPWRGD())
  {
    return(1);
  }
  
  return(0);
}

BYTE S5S0_RSMRST_EN(void)
{
  RSMRST_HI();
  
  return 0;
}

BYTE S5S0_PBTN_OUT_LOW(void)
{
  PM_PWRBTN_LOW();
  
  return 0;
}

BYTE S5S0_CHK_S5(void)
{
  if(Read_SLPS5())
  {
    PM_PWRBTN_HI();
    SYSON_HI();
    return(1);
  }
  
  return(0);
}

BYTE S5S0_PBTN_OUT_HI(void)
{
  PM_PWRBTN_HI();
  
  return 0;
}

BYTE S5S0_CHK_S3(void)
{
  if(Read_SLPS3())
  {
    return(1);
  }
  return(0);
}

BYTE S5S0_CP_RESET_OFF(void)
{
#if Support_CP_RESET_control
	CP_RESET_OFF(); //LID_Close_EC# //A485
#endif //Support_CP_RESET_control

	return 0;
}

BYTE S5S0_SUSP_HI(void)
{
  SUSP_ON();
  BEEP_ENABLE_ON();  //D cover switch
  //SPI_PWR_DISABLE_HI(); //Thermal trip
  AOAC_PWRON();
  CURRENT_STATUS &= 0xC0; //Clear flags of entering and resuming from S3 & S4 &S5
  SetS0GPIO();
  
  return 0;
}

BYTE S5S0_USBON(void)
{
  USB_ON_ON();
  nKbProjectID = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_KB_Type);  // read KB Type.	
  switch( nKbProjectID )
  {
    case 'K':  // UK keyboard  ascii=4b
      cKBStatus &= 0xFC;
      cKBStatus |= 0x01;  // Set UK keyboard.
      break;
    case 'J':  // JP keyboard   ascii=4a
      cKBStatus &= 0xFC;
      cKBStatus |= 0x02;  // Set JP keyboard.
      break;
    case 'E':  // EX keyboard   ascii=45
      cKBStatus |= 0x03;  // Set BR keyboard.
      break;
    case 'S':  // US keyboard   ascii=53
    default:   // Other character then set US matrix for default
      cKBStatus &= 0xFC;  // Set US keyboard.
      break;
  }
  LoadKBMatrixToRAM();		

  return 0;
}

BYTE S5S0_CHK_VDDAPWROK(void)
{
  if(Read_VDDAPWROK())
  {
    return(1);
  }
  return(0);
}

BYTE S5S0_CHK_095VSVDDPPG(void)
{
  //if(Read_105VS_VDDPPG()) //David- 171027
  if(CHK_095VS_PGD ()) //David+ 171027
  {
    return(1);
  }
  return(0);
}

BYTE S5S0_APUPWR_EN(void)
{
  APUPWR_EN_ON();
#if NV_GC6
  FB_CLAMP_OFF();
  nGC6ClampOff;
#endif // NV_GC6

#if !Support_SMTADP
  SetPowerBatteryparameter();
#endif
  MuteCount = 4;
  EC_TX_INPUT;
  if( Chk_DEBUG_IN() )
  {
    SET_MASK(pEdgeProjectFlag1, pEdgeProjectDebugIn);
    EC_TX_OUTPUT;
    EC_RX_OUTPUT;
  }
  else
  {
    CLEAR_MASK(pEdgeProjectFlag1, pEdgeProjectDebugIn);
    EC_RX_INPUT;
    EC_TX_INPUT;
  }
  if( (PriBattDisChgGet || SecBattDisChgGet) && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
	  BAT_LEARN_ON();
	  BATLEARNProcess(nAcOffCmdEN);	
    SET_MASK(M_BATStopChgStatL,ENCUSTOMER);
  }

  return 0;
}

BYTE S5S0_TP_RESET_ON(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_ON();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_ON();
#endif //Support_BYPASS_PAD_control

	return 0;
}

BYTE S5S0_CHK_IMVPPOK(void)
{
  if(Read_IMVPPOK())
  {
    return(1);
  }
  return(0);
}

BYTE S5S0_PWRGOOD_HI(void)
{
  PWR_GOOD_D_HI();

  return 0;
}
BYTE S5S0_FCH_PWROK(void)
{
  VGA_TempDly = 14; // Set VGA delay inital temperature 14 *500msec

  return 0;
}

BYTE S5S0_Init_Status(void)
{
  //for project design
#if Support_TP_RESET_control  
	if (!RST_TRACK_POINT_GET)
		TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
	if (!BYPASS_CLICKPAD_GET)
		BYPASS_PAD_OFF();
#endif //Support_BYPASS_PAD_control

  //for LCFC common code
	OEM_SYSTEM_S5_S0();

	return 0;
}
//-------------------------------------------------------------------------------------
//  Do S5 -> S0 power sequnce
//-------------------------------------------------------------------------------------

const sPowerSEQ asPowerSEQS5S0[]=
{
//
// AMD Power Sequence
//
    // Func               // Delay  // Check flag
  { Sequence_Null,         0,       0,  },  // Dummy step always in the top of tabel
  { S5S0_STEP1,           32,       0,  },  // 1
  { S5S0_CHK_VDDPALWPWRGD,20,       1,  },  // 2
  { S5S0_RSMRST_EN,       30,       0,  },  // 3 //david + 10->30
  { S5S0_PBTN_OUT_LOW,    20,       0,  },  // 4
  { S5S0_CHK_S5,           4,       1,  },  // 5
  { S5S0_CHK_S3,          10,       1,  },  // 6
  { S5S0_CP_RESET_OFF,     0,       0,  },  // 7 //david + 0->8 //A485D00010: Modify for TP can not use
  { S5S0_SUSP_HI,         10,       0,  },  // 8
  { S5S0_USBON,            1,       0,  },  // 9
  { S5S0_CHK_095VSVDDPPG, 20,       1,  },  // 10 A //david + 10->20
  { S5S0_APUPWR_EN,        0,       0,  },  // 11 B
  { S5S0_TP_RESET_ON,     10,       0,  },  // 12 C // TP_RESET_ON after SUSP 51ms
  { S5S0_CHK_IMVPPOK,     10,       1,  },  // 13 D //david + 1->10
  { S5S0_PWRGOOD_HI,     126,       0,  },  // 14 E
  { S5S0_Init_Status,      0,       0,  },  // 15 F
};

//============S0 -> S5==========================================================

//
// AMD Power Sequence Subroutines
//

BYTE S0S5_PBTN_OUT_LOW(void)
{
  PM_PWRBTN_LOW();

  return 0;
}

BYTE S0S5_PBTN_OUT_HI(void)
{
  PM_PWRBTN_HI();

  return 0;
}

BYTE S0S5_CHK_S3S5(void)
{
  if(!Read_SLPS3() && !Read_SLPS5())
  {
    return(1);
  }

//   if(DelayDoPowerSeq == 0) {
//     RSMRST_shutdown();
//   }
//   DelayDoPowerSeq--;

  return(0);
}

BYTE S0S5_CHK_S5S3(void)
{
  if( !Read_SLPS5() && !Read_SLPS3())
  {
    if( IS_MASK_SET(pBattLearn,uRefreshmode) )
    {
	  BAT_LEARN_OFF();
	  BATLEARNProcess(nAcOffCmdDIS);	
      PriBattDisChgDis;
      SecBattDisChgDis;      
      pBattLearn = 0;
      CLEAR_MASK(M_BATStopChgStatL,ENCUSTOMER);
    }
  }

  return 0;
}

BYTE S0S5_SUSP_OFF(void)
{
#if Support_CP_RESET_control
  CP_RESET_ON();
#endif //Support_CP_RESET_control
	BEEP_ENABLE_OFF();
  SUSP_OFF();
  SPI_PWR_DISABLE_LOW();
  AOAC_PWROFF();

  SetS5GPIO();
  EC_MUTE_ACT();

  return 0;
}

BYTE S0S5_CHK_095VSVDDPPG(void)
{
  //if(!Read_105VS_VDDPPG())  //David- 171027
   if(!CHK_095VS_PGD ())  //David+ 171027
  {
    return(1);
  }
  return(0);
}

BYTE S0S5_APUPWR_EN_OFF(void)
{
  APUPWR_EN_OFF();
  //
  // Set EC_RSMRST# to low in DC mode.
  //
  if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut) &&
      (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)) &&
      (LOWBATT_3TIMES==0) &&
      ((SysStatus2&0x03)== 0)
     )  // Not cut power in the battery mode for BIOS used.
  {
    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))  // Not cut power in the battery mode for command used.
    {
      SET_MASK(cCmd,cC_PCHPWR_Keep);
      DelayDoPowerSeq = 8000; // 1ms base
    }
  }

  return 0;
}

BYTE S0S5_SYSON_OFF(void)
{
  SYSON_LOW();

  return 0;
}

BYTE S0S5_CHK_IMVPPOK(void)
{
  if(!Read_IMVPPOK())
  { 

    return(1);
  }
  return(0);
}

BYTE S0S5_4S_SD_ECON_OFF(void)
{
	if(IS_MASK_SET(Shutdown_check,PB4s_shutdown))
	{
		EC_ON_LOW();
		RSMRST_LOW();
	}
	return 0;
}
BYTE S0S5_4S_SD_ECON_ON(void)
{
	if(IS_MASK_SET(Shutdown_check,PB4s_shutdown))
	{
		EC_ON_HI();
	}
	CLEAR_MASK(Shutdown_check,PB4s_shutdown);
	return 0;
}
BYTE S0S5_Init_Status(void)
{
  //for project design
  PWR_GOOD_D_LOW();
  
 //for LCFC common code
 OEM_SYSTEM_S0_S5(); 

 return 0;
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S5 power sequnce
//-------------------------------------------------------------------------------------

const sPowerSEQ asPowerSEQS0S5[]=
{
//
// AMD Power Sequence
//
    // Func               // Delay  // Check flag
  { Sequence_Null,         0,       0,  },  // Dummy step always in the top of tabel
  { S0S5_CHK_S5S3,         1,       0,  },  // 1
  { S0S5_SUSP_OFF,         0,       0,  },  // 2
  { S0S5_CHK_095VSVDDPPG,  0,       1,  },  // 3
  { S0S5_APUPWR_EN_OFF,   30,       0,  },  // 4
  { S0S5_SYSON_OFF,       20,       0,  },  // 5
  { S0S5_CHK_IMVPPOK,      1,       1,  },  // 6
  { S0S5_4S_SD_ECON_OFF, 250,       0,  },  // 7
  { S0S5_4S_SD_ECON_ON,    0,       0,  },  // 8
  { S0S5_Init_Status,      0,       0,  },  // 9
};

//============S0 -> S3==========================================================

//
// AMD Power Sequence Subroutines
//

BYTE S0S3_CHK_SLPS3(void)
{
  if(!Read_SLPS3())
  {
    return(1);
  }
  return(0);
}

BYTE S0S3_SUSP_OFF(void)
{
	BEEP_ENABLE_OFF();
  SUSP_OFF();
  SPI_PWR_DISABLE_LOW();
  AOAC_PWROFF();

  SetS5GPIO();

#if SW_ISCT
  ISCT_Again = 59;  // Last 60 Sec count for ISCT used.
#endif // SW_ISCT
  EC_MUTE_ACT();

  return 0;
}

BYTE S0S3_CHK_095VSVDDPPG(void)
{
 // if(!Read_105VS_VDDPPG()) //David- 171027
  if(!CHK_095VS_PGD())	//David+ 171027
  {
    return(1);
  }
  return(0);
}

BYTE S0S3_APUPWR_EN_OFF(void)
{
  APUPWR_EN_OFF();

  return 0;
}

BYTE S0S3_CHK_IMVPPOK(void)
{
  if(!Read_IMVPPOK())
  {
    
    return(1);
  }
  return(0);
}

BYTE S0S3_Init_Status(void)
{ 
	//for project design
	PWR_GOOD_D_LOW(); 
#if Support_CP_RESET_control  
	CP_RESET_ON();
#endif //Support_CP_RESET_control
  
	//for LCFC common code
	OEM_SYSTEM_S0_S3();  

	return 0;
}

//-------------------------------------------------------------------------------------
//  Do S0 -> S3 power sequnce
//-------------------------------------------------------------------------------------
const sPowerSEQ asPowerSEQS0S3[]=
{
//
// AMD Power Sequence
//
    // Func               // Delay  // Check flag
  { Sequence_Null,         0,       0,  },  // Dummy step always in the top of tabel
  { S0S3_CHK_SLPS3,        1,       1,  },  // 1
  { S0S3_SUSP_OFF,         0,       0,  },  // 2
  { S0S3_CHK_095VSVDDPPG,  0,       1,  },  // 3
  { S0S3_APUPWR_EN_OFF,   30,       0,  },  // 4
  { S0S3_CHK_IMVPPOK,      1,       1,  },  // 5
  { S0S3_Init_Status,      0,       0,  },  // 6
};

//============S3 -> S0==========================================================

//
// AMD Power Sequence Subroutines
//
BYTE S3S0_PBTN_OUT_LOW(void)
{
  FNKB_S3Dly = 0;
  PM_PWRBTN_LOW();

  return 0;
}

BYTE S3S0_PBTN_OUT_HI(void)
{
  PM_PWRBTN_HI();
  SysPowState=SYSTEM_S3;

  return 0;
}

BYTE S3S0_CP_RESET_OFF(void)
{
  PM_PWRBTN_HI();
#if Support_CP_RESET_control  
  CP_RESET_OFF();
#endif //Support_CP_RESET_control

	return 0;
}

BYTE S3S0_CHK_S3(void)
{
  if(Read_SLPS3())
  {
    return(1);
  }
  return(0);
}

BYTE S3S0_SUSP_HI(void)
{
  SUSP_ON();
  BEEP_ENABLE_ON();
  SPI_PWR_DISABLE_HI();
  AOAC_PWRON();

  SetS0GPIO();
  USB_ON_ON();

  return 0;
}

BYTE S3S0_CHK_095VSVDDPPG(void)
{
 // if(Read_105VS_VDDPPG())//David- 171027
  if(CHK_095VS_PGD())	//David+ 171027
  {
    return(1);
  }
  return(0);
}

BYTE S3S0_APUPWR_EN_HI(void)
{
  APUPWR_EN_ON();
  MuteCount = 18;  //tony 1208

  return 0;
}

BYTE S3S0_TP_RESET_ON(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_ON();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control  
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_ON();
#endif //Support_BYPASS_PAD_control

	return 0;
}

BYTE S3S0_TP_RESET_OFF(void)
{
#if Support_TP_RESET_control
  if (!RST_TRACK_POINT_GET)
    TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control  
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_OFF();
#endif //Support_BYPASS_PAD_control

	return 0;
}

BYTE S3S0_CHK_IMVPPOK(void)
{
  if(Read_IMVPPOK())
  {
    return(1);
  }
  return(0);
}

BYTE S3S0_PWRGOOD_HI(void)
{
  PWR_GOOD_D_HI();
	VGA_TempDly = 10; // Set VGA delay inital temperature 10 *500msec.
  if( (PriBattDisChgGet || SecBattDisChgGet) && IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
      BAT_LEARN_ON();
	  BATLEARNProcess(nAcOffCmdEN);			
      SET_MASK(M_BATStopChgStatL,ENCUSTOMER);
  }

  return 0;
}

BYTE S3S0_Init_Status(void)
{ 
  //for project design
#if Support_TP_RESET_control  
  if (!RST_TRACK_POINT_GET)
    TP_RESET_OFF();
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
  if (!BYPASS_CLICKPAD_GET)
    BYPASS_PAD_OFF();
#endif //Support_BYPASS_PAD_control

#if SUPPORT_DSx
  DSxPowState = SYSTEM_S3S4;  // Clear Deep status.
#endif  
  CLEAR_MASK(cCmd,cC_PCHPWR_Keep);
  AOU_IFG_Debounce_Cnt = 0;


  //for LCFC common code
  OEM_SYSTEM_S3_S0();

  return 0;
}

//-------------------------------------------------------------------------------------
//  Do S3 -> S0 power sequnce
//-------------------------------------------------------------------------------------

const sPowerSEQ asPowerSEQS3S0[]=
{
//
// AMD Power Sequence
//
    { Sequence_Null,        0,       0,  },  // Dummy step always in the top of tabel
    { S3S0_PBTN_OUT_LOW,   20,       0,  },  // 1
    { S3S0_PBTN_OUT_HI,     0,       0,  },  // 2
    { S3S0_CP_RESET_OFF,    0,       0,  },  // 3
    { S3S0_CHK_S3,          1,       1,  },  // 4
    { S3S0_SUSP_HI,        10,       0,  },  // 5
    { S3S0_CHK_095VSVDDPPG,10,       1,  },  // 6
    { S3S0_APUPWR_EN_HI,    0,       0,  },  // 7
    { S3S0_TP_RESET_ON,    10,       0,  },  // 10 A  // TP_RESET_ON after SUSP  51ms (15+20+16)
    { S3S0_CHK_IMVPPOK,     1,       1,  },  // 8     // T11 = 98ms
    { S3S0_PWRGOOD_HI,    126,       0,  },  // 9
    { S3S0_Init_Status,     0,       0,  },  // 11 B
};

//=============================================================================
void CheckAutoPowerOn(void)
{
  if ((WinFlashMark == autoOnMark1) && (WinFlashMark2 == autoOnMark2) && SystemIsS5)
  {
//    AutoTimer = 0x01;
    AutoTimer = 0x83;
    WinFlashMark = 0;
        WinFlashMark2 = 0;
  }
}

void Check_EC_ON()
{
  if ( (IS_MASK_SET(Thro_Status2,b5BAT_TooLow))&&(SystemIsS5) )
  {
    _nop_();
    _nop_();
    _nop_();
    ChipSettingBeforeSleep();
    LID_readytime = 11 ;  // LID switch wakeup time = 100ms
    CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
    SET_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
    CLEAR_MASK(Thro_Status,b5BAT_TooLow);
    return;
  }
  else
  {
    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)
      && (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
      && (LOWBATT_3TIMES==0)&&((SysStatus2&0x03)== 0)
      )  // Not cut power in the battery mode for BIOS used.
    {
      if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))  // Not cut power in the battery mode for command used.
      {
        if ( ((!USB_PWRGET) || IS_MASK_SET(USB_Charger, b2USB_TmlDis))
          && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)
          )
        {
          _nop_();
          _nop_();
          _nop_();
          ChipSettingBeforeSleep();
        }
        else
        {
          // Check AC mode and battery <30%
          if((M_BATdata_ready != 1) && (S_BATdata_ready != 1))
          { return; }
          if ((EC_AOU_PWR_TYPE & 0xF0) == 0)
          {
            _nop_();
            _nop_();
            _nop_();
            ChipSettingBeforeSleep();
          }

        }
        LID_readytime = 11 ;  // LID switch wakeup time = 100ms
        CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
        SET_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
        return;
      }
    }
  }
  SET_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
  CLEAR_MASK(LENOVOPMFW_Temp,b5EC_ON_EN);
}

const PowerSEQTableList PowerSEQSTBL[4] = {
  {
    asPowerSEQS5S0,
    sizeof (asPowerSEQS5S0) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS0S5,
    sizeof (asPowerSEQS0S5) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS3S0,
    sizeof (asPowerSEQS3S0) / sizeof (sPowerSEQ),
  },
  {
    asPowerSEQS0S3,
    sizeof (asPowerSEQS0S3) / sizeof (sPowerSEQ),
  },
};


void Oem_PowerSequence(BYTE SxSeq)
{
  if ( PowSeqDelay != 0x00 )
  {
    PowSeqDelay--;                   //1m Sec count base
    if ( PowSeqDelay != 0x00 )  return;
  }
  
  while(PWSeqStep<(PowerSEQSTBL[SxSeq].PowerSEQCount))
  {
    if(PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].checkstatus==0x00)  // Do function
    {
      (PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].func)();
      PowSeqDelay = PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].delay;
#if !Support_EC_Port80_Debug      
      EC_OUT_PORT80(PWSeqStep);
#endif //Support_EC_Port80_Debug      
      PWSeqStep++;
      if(PowSeqDelay!=0x00)
      {
        break;
      }
    }
    else  // Check input status pin
    {
      if((PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].func)())
      {
        PowSeqDelay = PowerSEQSTBL[SxSeq].PowerSEQTable[PWSeqStep].delay;
        PWSeqStep++;
      }
      break;
    }
  }
}


//-----------------------------------------------------------------------------
// Handle System Power Control
// Called by service_1mS -> Timer1msEvent -> Hook_Timer1msEvent
//-----------------------------------------------------------------------------
void Oem_SysPowerContrl(void)
{


    // Check if delay doing PowerSeq is needed.
    //if ( DelayDoPowerSeq != 0x00 )
    //{
    //    DelayDoPowerSeq--; 		
    //    if ( DelayDoPowerSeq != 0x00 )
    //    {
    //        return;
    //    }
    //}

  // Check current System PowerState to see what should we do.
  switch (SysPowState)
  {
    case SYSTEM_S0:
      if(!Read_SLPS3())
      {
        if (S3S4DelayCnt > 10)
        {
          PWSeqStep = 1;
          PowSeqDelay = 00;
          SysPowState=SYSTEM_S0_S3;
          if (!Read_SLPS5())
          {
            PWSeqStep = 1;
            PowSeqDelay = 0x00;
            SysPowState=SYSTEM_S0_S5;
            if(CountSecAfterPswPressed>=40)
            {
            	SET_MASK(Shutdown_check,PB4s_shutdown);
            }
          }
          if (IS_MASK_SET(SysStatus,CloseLid))
          {
            CLEAR_MASK(SysStatus,CloseLid);
          }
        }
        else
        {  S3S4DelayCnt++;  }
      }
      else
      {  S3S4DelayCnt = 0;    }
      Check_LPC_Reset();
      OEM_SYSTEM_S0();
    break;

    case SYSTEM_S3:
      OEM_SYSTEM_S3();
      if(Read_SLPS3())  //S3 to S0
      {
        PWSeqStep = 3;
        PowSeqDelay = 20;
        SysPowState = SYSTEM_S3_S0;
      }
    break;

    case SYSTEM_S4:
    case SYSTEM_S5:
			CLEAR_MASK(Shutdown_check,PB4s_shutdown);
      if (DelayDoPowerSeq != 0) {
        DelayDoPowerSeq--;
        if (DelayDoPowerSeq == 0) {
          CLEAR_MASK(cCmd,cC_PCHPWR_Keep);
          TrigRsmrstInDcMode();
        }
      }

      if(Read_SLPS3() && Read_SLPS5())
      {
        DelayDoPowerSeq = 0;
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S5_S0;
      }
      CheckAutoPowerOn();
      if ( (pWorkAround == 0x01)
        && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep)
        && (!WOLDCWakeEnGet)
        && (!LidWakeEnGet)
        )
      {
        pWorkAround = 0x00;
        Check_EC_ON();
      }
#if EnhancedPcBeepSupport
  		OEM_ENHANCED_PC_BEEP_S5_HOOK();
#endif
      OEM_SYSTEM_S5();      
    break;

    case SYSTEM_S5_S0:
      if ( EC_MainPwr() )
      {
    		 if (IS_BAT_switch_ready)
			   {Select_Battery();}
        Oem_PowerSequence(POWERSEQS5toS0);      
        mPWRDelay=0;
      }
      else
      {
        mPWRDelay++;
        if( mPWRDelay > 50 )
        {
          PWSeqStep = 1;
          PowSeqDelay = 0x00;
          SysPowState=SYSTEM_S0_S5;
        }
      }
    break;

    case SYSTEM_S3_S0:
      Oem_PowerSequence(POWERSEQS3toS0);
    break;

    case SYSTEM_S0_S3:
      Oem_PowerSequence(POWERSEQS0toS3);
    break;

    case SYSTEM_S0_S4:
    break;

    case SYSTEM_S0_S5:
      Oem_PowerSequence(POWERSEQS0toS5);
    break;

    case SYSTEM_EC_WDTRST:
    break;

    default:
      SysPowState = SYSTEM_S5;
    break;
  }
}

/*
*******************************************************************************
* Function name: ChipSettingBeforeSleep           
*
* Descriptoin: EC chip setting before ec entry sleep mode
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void ChipSettingBeforeSleep(void)
{
	EC_ON_LOW();
	//EC_ON2_LOW();
	AC_PRESENT_LOW();
	EC_WAKE_OUT_LOW();
    PM_PWRBTN_LOW();  //// A485D00008 Add for leakage of electricity in DC S5
	SMBus3_CLK_INDW;       // Ch3
	SMBus3_DAT_INDW;       // Ch3
	CLEAR_MASK(GS.Status, GS_Init);
	Attach2status = 0;
	
#if Support_TYPE_C
  Oem_Hook_Type_C_BeforeSleep();
#endif //Support_TYPE_C
}
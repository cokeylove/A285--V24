//*****************************************************************************
//
//  oem_power.c
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
//*****************************************************************************


//*****************************************************************************
BOOL Hook_Check_Wake_Up_Event(void)
{
    return  TRUE;
}   
//*****************************************************************************
BYTE Hook_Enable_Wake_Up_Src(BYTE type)
{
    return  TRUE;
}
//*****************************************************************************
BYTE Hook_Disable_Wake_Up_Src(BYTE type)
{
    return  TRUE;
}


//-----------------------------------------------------------------------------
//  The OEM variable of Sx -> S0
//-----------------------------------------------------------------------------
void OEM_SX_S0CommVar(void)
{

	SPCTRL1|=0x80; 		// FOR 80 PORT
	RSTS=0x44;			// vcc power detector option
	Init_Kbd();
	if(PLLFREQR != 0x04)
	{
		ChangePLLFrequency(5);      //for default 48MHz
	}
}

//-----------------------------------------------------------------------------
//  The variable of Sx -> S0
//-----------------------------------------------------------------------------
void SX_S0CommVar(void)
{
	SysPowState=SYSTEM_S0;
	ResetSCIEvent();
	OEM_SX_S0CommVar( );
	
}


//-----------------------------------------------------------------------------
//  The OEM variable of S0 -> Sx
//-----------------------------------------------------------------------------
void OEM_S0_SXCommVar(void)
{
   	
	Init_Kbd( );

}

void S0_SXCommVar(void)
{
#if TouchPad_only
	PowerOffClearTPPendingData();
#endif 
	
	CLEAR_MASK(KBHISR,SYSF);
    Ccb42_SYS_FLAG=0;
	Hook_KBRSTON();


    OEM_S0_SXCommVar();
}


void FnKeyWakeS3(void)
{
	KeyWakeEn;
	EC_WAKE_OUT_LOW();
	Delay1MS(1);
	EC_WAKE_OUT_HI();
}
	

void OEM_SYSTEM_S0 (void)
{
	//add port switch to 20v for cs18 dock cannot support 9v-20180418
	#if Support_TYPE_C
	OEM_TYPE_C_S0_HOOK();
	#endif
}

void OEM_SYSTEM_S3 (void)
{
	
#if Support_TYPE_C
	OEM_TYPE_C_S3_HOOK();
#endif //Support_TYPE_C  
	
	if ( IS_MASK_SET(u4SenseSTS0,Edge_LidStatus) && EC_KBFNON() )
	{
		if (FNKB_S3Dly > 10)
		{
		  	FnKeyWakeS3();
		}
			FNKB_S3Dly++;
	}
	else
	{ 
		FNKB_S3Dly = 0; 
	}
}
	
void OEM_SYSTEM_S5 (void)
{
#if chPWSW1WDT
	CLEAR_MASK(pProject4,pPWSWdisable);
#endif  //chPWSW1WDT  

#if Support_TWO_BATT
  if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))  //Clear AC in  flag
    {
      if (Current_BAT_State != BAT_SEL_INIT){
        BAT_state_target(BAT_SEL_INIT);								// switch to 3
      }
    }
#endif


#if Support_TYPE_C
	OEM_TYPE_C_S5_HOOK();
#endif //Support_TYPE_C	 
}
	
void OEM_SYSTEM_S3_S0 (void)
{
	SX_S0CommVar();
	
	LedPowerOn(); 	// turn on Power LED
	
	Fan_ON_Count = FAN_Boot_On;  // Turn on Fan 3 Sec.
	FAN_PWM = Target_Duty = FAN_PWM_Min; // PWM min. 30%
	
	SetACIN_Int();		
	CURRENT_STATUS &= 0xC0;	  //(JSN20120614) Clear flags of entering and resuming from S3 & S4 &S5
	EC_WAKE_OUT_HI();
	
	
#if Support_IFFS
	if ( (IS_MASK_SET(ACPI_HOTKEY,b5BIOS_IFFS)) )
	{ EC_WAKE_OUT_HI(); }
#endif // Support_IFFS
	if( IS_MASK_SET(pProject0,b7SWITCHBAK) )
	{
		nSwitchableBTNEN;
	}
	else
	{
		nSwitchableBTNDIS;
	}  
	
#if Support_FANLESSS
	capNoFANEn;
#else
	capNoFANDis;
#endif  // Support_FANLESS
#if Support_TYPE_C
    OEM_TYPE_C_S3_S0_HOOK();
#endif //Support_TYPE_C
}
	
void OEM_SYSTEM_S0_S3 (void)
{
	CPU_Shut_Cnt = VGA_Shut_Cnt = 0;	// Clear shutdown conunt.
	SysPowState=SYSTEM_S3;
	DSxPowState = SYSTEM_S3S4;
	CLEAR_MASK(SysStatus2, b4VGATemp_Init);// Restart initial GPU temperature.
	Clr_Thermal_Tab();		// Clear Thermal table value.
	ERR_THMSTS = 0;		   // Clear thermal status fail bit.
	Thro_Status = Thro_Status2 = 0;  // clear Throttling status.
#if SW_ISCT
	AOAC_STATUS &= 0x10;		// Clear ISCT status.
#endif // SW_ISCT
	initMark = 0x00;	// CMW 20121203
	TEMP_Error = 0x00;  // CMW 20121203
	
	FanInitDIS;
	FanErrDIS;
	FanExistDIS;
	FanAutomaticmodeEn;
	FanMaximumspeedCLR;
	u4ManualFanPRM = 0; 
	CLEAR_MASK(uReserve07,b7Fan2Manual);
	FANCPUSeg = 0;
	FANVGASeg = 0;
        FANTSSeg = 0;   //A285D00051: new fan table spec implementation
	S0_SXCommVar();
  GsensorStableDelay = 200;
#if Support_TYPE_C
    OEM_TYPE_C_S0_S3_HOOK();
#endif //Support_TYPE_C
}
	
	
void OEM_SYSTEM_S5_S0 (void)
{
	SX_S0CommVar();
#if SupportReadTPID			
	ReadTPid();
#endif 
	LedPowerOn(); 	// turn on Power LED	
	FanAutomaticmodeEn;	  //Enable fan automatic mode  
	u4ManualFanPRM = 0;
	Fan_ON_Count = FAN_Boot_On;  // Turn on Fan 3 Sec.
	FAN_PWM = Target_Duty = FAN_PWM_Min; // PWM min. 30%
	nBrightValue = 17;	  // Default WIN8 brightness 110~130 nit.
	u4CpuThermal = VGA_TEMP = EXTVGA_TEMP = 32;  // Default temperature value.
	
	ACPI_HOTKEY &= 0x1F;	  // Clear bit6,7, can cut power in the battery mode.
	SYS_STATUS &= 0xF8;	   // Clear OS mode.
	SetACIN_Int();	
	EC_WAKE_OUT_HI();  //  
	
	CLEAR_MASK(cCmd,cC_PCHPWR_Keep);
	GExistEN;
	u4PeriSTS4  |= 0x80;
	CLEAR_MASK(SYS_MISC1,ACPI_OS); 
	
#if Support_FANLESSS
	capNoFANEn;
#else
	capNoFANDis;
#endif  // Support_FANLESS
#if Support_KBD_BL_LED
#if Support_KBD_BL_DETECT
	if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
	{
		KBL_Target_Level_Duty = 100;
		KBL_Interval_Cnt = 1;
		KBL_Interval_Cnt_Temp = KBL_Interval_Cnt;
		SET_MASK(KBL_Status,PowerOnCheck);
	}
#endif //Support keyboard backlight
#if Support_TYPE_C
    OEM_TYPE_C_S5_S0_HOOK();
#endif //Support_TYPE_C
}
	
void OEM_SYSTEM_S0_S5 (void)
{
	S0_SXCommVar();
	
	RSTS=0x04;		//vcc power detector option
	
	DSxPowState = SYSTEM_S3S4;
	SysPowState = SYSTEM_S5;
	CPU_Shut_Cnt = VGA_Shut_Cnt = 0;	// Clear shutdown conunt.
	ERR_THMSTS = 0;		   // Clear thermal status fail bit.
	Thro_Status = Thro_Status2 = 0;  // clear Throttling status.
	CLEAR_MASK(SysStatus2, b4VGATemp_Init);// Restart initial GPU temperature.
	
	FanInitDIS;
	FanErrDIS;
	FanExistDIS;
      //A285D00073 start+
       #if KBC_ENABLE_PS2_CONTROL
       CLEAR_MASK(u4Reserve0A, Chk_Buf_Half_Full_En);
	#endif
       //A285D00073 end+
	if (cSysActionFlag == SysAction_EC_Restart || cSysActionFlag == SysAction_SysShutdown_EC_AutoBoot)
	{
		WinFlashMark = autoOnMark1;	
		WinFlashMark2 = autoOnMark2;
		ExternalWDTNow();
	}
	else if( (WinFlashMark == exitFollowMode) || (cSysActionFlag == SysAction_EC_Shutdown))
	{
		ProcessSID(FLASH_ID);		
		WinFlashMark = autoOnMark1;
		WinFlashMark2 = autoOnMark2;
		ExternalWDTNow();		
	}
	
	Clr_Thermal_Tab();	// Clear Thermal table value.
#if NV_GC6
	FB_CLAMP_OFF();
#endif // NV_GC6
	if(IS_MASK_SET(SysStatus,LidKBIgnore))
	{
		Ccb42_DISAB_KEY = 0;
		Flag.SCAN_INH = 0;	// not Inhibit scanner (internal keyboard).
		Unlock_Scan();	  // UnLock scanner
		CLEAR_MASK(SysStatus,LidKBIgnore);
	}
	
	//
	// Edge projects' MAINPWRON is input and be used to indicate power OTP activion.
	//
	if (!EC_MainPwr()) {
		SET_MASK(SysStatus,ERR_ShuntDownFlag);
		ProcessSID(PWR_OTP_ID);		
	}
	
	if ( IS_MASK_CLEAR(SysStatus, ERR_ShuntDownFlag) )
	{ ProcessSID(S5_ID); }		
	CLEAR_MASK(SysStatus,ERR_ShuntDownFlag);	// Clear Shutdown ID bit.
	initMark = 0x00;	// CMW 20121203 for PS8625 translator
	TEMP_Error = 0x00;  // CMW 20121203 for PS8625 translator
	pWorkAround = 11;  // Keep 5.5s.	//
	ECSleepCount = pWorkAround+1; 	   // Delay entry EC sleep.
	FanAutomaticmodeEn;
	FanMaximumspeedCLR;
	u4ManualFanPRM = 0;  
	CLEAR_MASK(uReserve07,b7Fan2Manual);
	FANCPUSeg = 0;
	FANVGASeg = 0;
        FANTSSeg = 0;   //A285D00051: new fan table spec implementation
	
	if ( IS_MASK_SET(S0S5PWRcodition,S0S5PWROFF))
	{
		PM_PWRBTN_LOW();
	}
	BiosReserved3E2 = 0x00;
	GSENSOR_ERROR_CLR;
	
#if KB_FnSticky_Key      
	STICKY_MODE_CLR;
	STICKY_LOCK_CLR;
  Scanner_State_FN = 0;
  temp_scanner_state.field.tFN = 0;	  
#endif //KB_FnSticky_Key 
    
  GsensorStableDelay = 200;
#if Support_TYPE_C
    OEM_TYPE_C_S0_S5_HOOK();
#endif //Support_TYPE_C
}

void OEM_LPC_Reset(void)
{
#if Support_GATEA20  
	GATEA20_HI();
#endif
	if (Read_D_Cover_SW())
	{
     SET_MASK(TamperSTS, BottomTamper);
   	} 
	
	CLEAR_MASK(KBHISR,SYSF);
	CLEAR_MASK(SYS_MISC1,ACPI_OS);
	CLEAR_MASK(cCmd,cC_BkOff);
	CLEAR_MASK(uReserve07,nFanManual);
	CLEAR_MASK(uReserve07,b7Fan2Manual);
	CLEAR_MASK(LENOVOPMFW,BATTERY_MAIN_CAL);
	CLEAR_MASK(LENOVOPMFW,BATTERY_CAL_END);
	LENOVOPMFW_Temp = 0;
	CLEAR_MASK(ACOFF_SOURCE, BATTLEARN);
  if( Read_AC_IN() && !Read_ACOFF())
  {
  	SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
  }
	ACOFF_LOW();
	BAT_LEARN_OFF();
	CLEAR_MASK(Led_status, BatLED_test);  //clear battery led test mode
  u4Cfgspace4 &= ~u4Cfgspace4BATseq;
  
#if Support_RFID
	//	unlockRFIDProtect();	//Add RFID function.
	CRCDelay = 5; 
#endif  //Support_RFID
	AOURSTCNT = 10;
	
#if KB_FnSticky_Key   
	if ( IS_MASK_CLEAR(CURRENT_STATUS, ENTER_S3))
	{  //if S3 don't clean sticky status
		STICKY_MODE_CLR;
		STICKY_LOCK_CLR;
		Scanner_State_FN = 0;
		temp_scanner_state.field.tFN = 0; 
	 }
#endif //KB_FnSticky_Key
}

	
void Check_LPC_Reset(void)
{
  	if(!Read_LPC_RST() && (pLastLPCstatusGET != Read_LPC_RST()))
  	{ 
		OEM_LPC_Reset();  // initial OEM status.
  	}
	  
  	if (Read_LPC_RST())
		{pLastLPCstatusSET;}
	else
		{pLastLPCstatusCLR;}
	
}
	

	
#if Support_EC_Port80_Debug
void EC_OUT_PORT80(BYTE bDate)
{
  if( Chk_DEBUG_IN() )
  {
		EC_RX_OUTPUT;
		EC_TX_OUTPUT;
		EN_ECPORT80;
		P80LedOut(bDate);	
		DIS_ECPORT80;
  }
}
#endif //Support_EC_Port80_Debug

void ProcessSID(BYTE SID)  // Shutdown ID.
{
  pLastSID4 = pLastSID3;
  pLastSID3 = pLastSID2;
  pLastSID2 = pLastSID;
  if ((SID == S5_ID) && (BIOS_SID != 0)){      //normal shutdown && BIOS shutdown != 0 => BIOS shutdown
    BIOS_SID_REC = BIOS_SID;
    BIOS_SID = 0;
    pLastSID = BIOS_ID;    
  } else {                                     //normal S5 shutdown
    pLastSID  = SID;
  }
#if WDT_Support
  DisableInternalWDT();		
#endif
  RamDebug(0xF1);
  Update_16ByteEEPROM();
  RamDebug(0xFF);
#if WDT_Support
  EnableInternalWDT();		
#endif
  if (SID != S5_ID){
    Save_All_RamandGPIO();
  }
}

	


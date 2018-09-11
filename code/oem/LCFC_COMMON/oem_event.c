//*****************************************************************************
//
//  oem_event.c
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

//-----------------------------------------------------------------------------
// no change function
//-----------------------------------------------------------------------------
void NullEvent(void)
{

}

//-----------------------------------------------------------------------------
// Adapter in function
//-----------------------------------------------------------------------------
void AdapterIn(void)
{
  BYTE Temp8;
  BATTBATLOWSHDProt = FALSE;		// // A285D00025+
#if (PLATFORM_CHIP == INTEL )
  PM_PWRBTN_HI();
  EC_WAKE_OUT_HI();
  EC_ON_HI();
  if(Read_RSMRST())
  {
    	AC_PRESENT_HI();
  }
#endif // PLATFORM_CHIP
    PM_PWRBTN_HI();  //A285D00018: Fix auto boot when pull high RSMRST 
	EC_WAKE_OUT_HI();//A285D00018: Fix auto boot when pull high RSMRST 


	if(PowerOnByAcGET && ((SysPowState == SYSTEM_S4)||(SysPowState == SYSTEM_S5)))
	{
		PSWPressed();
	}
  if ( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
  { cBATTThrottling = 0; }
  
  CLEAR_MASK(M_BATAlarm,ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);

  SET_MASK(SYS_STATUS,AC_ADP);  //set battery flag
  SET_MASK(u4SenseSTS3, SomeAcAdapterIn);

#if !Support_SMTADP
  ECSMI_SCIEvent(ACPI_ACIN_SCI);		
#endif

  if( SystemIsS0 )
  {
    u4BeepID = 0x06;
  }
  //ADPID_ON_EN();
  SetACIN_Int();
  M_BATSMbusFailCount = 0;
  M_BATErrorCnt = 0;
  S_BATSMbusFailCount = 0;
  S_BATErrorCnt = 0;  
  inhibit2sec = 20;
  if((IS_MASK_CLEAR(M_BATStopChgStatH,NotLenovoBattery))&&
  ((IS_MASK_CLEAR(M_BATStopChgStatL,ENCMDSTOPCHG))&&(IS_MASK_CLEAR(M_BATStopChgStatH, PMTHRESHOLD))&&(IS_MASK_CLEAR(M_BATStopChgStatH,ENBOOSTSTOPCHG)))){
    pFakeCHGCnt = inhibit2sec+40;      // 6sec
  }
  SET_MASK(M_BATStopChgStatL,ENCHARGESUSP);    // use for detect battery charging suspend
  CurrentL = 0;
  CurrentH = 0;
  // Set 0x12h and 0x3Fh before set ChargeCurrent and ChargeVoltage.
  CHGIC_ptr = W_chargevoltage;
  WriteSmartChgIC();
  WriteSmartChgIC();
  WriteSmartChgIC();       
  //
  CHGIC_ptr = W_ChargeOption;           

  inhibit2sec = 0;
  //A285D00069 start+
  Port2UnknowAdpRetry=0;
  UnknowAdpRetry=0;
  //u4SenseSTS3 &= ~(BIT3+BIT4); 
  //A285D00069 end+
  EC_ON_HI();
  EC_ON2_HI();  //A285D00066: fix cs18-dock S5 adapter in light unbrightness.
  ReadSmartChgIC();
  Unlock_ShipMode_ACIN();
  if(ACIN_BLINK3 == 0)
  {
	  if (SystemNotS5)
	  {
	    ACIN_BLINK3 = 35;        // LED blink 3times in 1.5sec
	  }
	  else
	  {
	    ACIN_BLINK3 = 30;        // LED blink 3times in 1sec
	  }
	}
  CLEAR_MASK(Attach2status,OcneSwitchPowerSource);
  BAT_Trickle_ACIN_timer = 0;
  Trickle_ChgIC(Disable);
  Hook_CHGIC_AdapterIn();
  M_BAT_Status1 =0;
  S_BAT_Status1 =0;  
  CLEAR_MASK(M_BATFlag1,pEdgeProjectBATWake); 
  CLEAR_MASK(S_BATFlag1,pEdgeProjectBATWake);   
  
	AD_ID_Detect_Delay = 2;

  BAT_Average_curr_sum = 0;
  BAT_Average_curr_CNT = 30;

  // battery related update
  for (Temp8 = 0; Temp8 <=1 ;Temp8++){
	   SetBatteryStopChargeStateFlag(Temp8, AC_IN); 
  } 

#ifdef OEM_Hook_AdapterIn_support
    OEM_Hook_AdapterIn();
#endif // OEM_Hook_AdapterIn_support  

	PowerSourceSwitchDelay = 20;
}

//-----------------------------------------------------------------------------
// Adapter out function
//-----------------------------------------------------------------------------
void AdapterOut(void)
{
  BYTE  AcDis_changed;

  AcDis_changed = 0;

  H_PROCHOT_HI();
  AC_OUT_Prochot_CNT = 11;   //WA : DAx75 prochot fixed 100mS
  AC_PRESENT_LOW();
  VGA_AC_BATT_OFF();
  
  if( IS_MASK_SET(pProject0,bAPUTURBO) )
  {
   	CPUtuborONOFF(Enable);
	  CLEAR_MASK(pProject0,bAPUTURBO);
  }

  if( IS_MASK_SET(pProject0,bHYBAPUTURBO) )
  {
    CPUtuborONOFF(Enable);
	  CLEAR_MASK(pProject0,bHYBAPUTURBO);
   }
  if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
  {
    CPUtuborONOFF(Enable);
    CLEAR_MASK(pProject0,bRSOCAPUTURBO);
   }

  cADPIDTemp = 0;
  AdapterID = 0;
  u4ACAdaptorCap = 0;
  AdpWatt = 0;
  SecAdpWatt = 0;  
  //A285D00069 start+
  Port2UnknowAdpRetry=0;
  UnknowAdpRetry=0;
  //A285D00069 end+
  CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
  CLEAR_MASK(SYS_STATUS,AC_ADP);  //clear battery flag
  CLEAR_MASK(M_BATStopChgStatL,ENOVERVOLTAGE);
  Chk_Battery_Full(M_BAT);
  ADPID_ON_DIS();

  // Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
  cBATTThrottling = CPUMAXPSTATE;
  cGPUThrottling = GPUMAXPSTATE;
  SET_MASK(M_BATAlarm,ADPOUT);
  SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
  ADPI2Sec = 100;
  GPU2Sec = 100;
  // Send QEvent to Bios
  ThrottlingControl();

  inhibit2sec = 20;
  if( SystemIsS0 )
  {
    u4BeepID = 0x06;
  }

  if ( SystemIsS5 )
  {
    ECSleepCount = 3;  // Delay entry EC sleep.
    if ( IS_MASK_CLEAR(ACPI_HOTKEY, b7BIOS_NoShut)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
      && (LOWBATT_3TIMES==0)&&((SysStatus2&0x03)== 0))  // Not cut power in the battery mode for BIOS used.
    {
      if ( IS_MASK_CLEAR(ACPI_HOTKEY, b6Cmd_NoShut) && IS_MASK_CLEAR(cCmd,cC_PCHPWR_Keep))  // Not cut power in the battery mode for command used.
      {
        RSMRST_LOW();
        Delay1MS(20);
#if (PLATFORM_CHIP == INTEL)
        PCH_PWR_EN_OFF();
#endif // PLATFORM_CHIP
      }
    }
  }

  if((PriBattDisChgGet || SecBattDisChgGet)&& IS_MASK_SET(pBattLearn,uRefreshmode) && IS_MASK_SET(pBattLearn,uACoffstchg) )
  {
    BAT_LEARN_ON();
    BATLEARNProcess(nAcOffCmdEN);
    SET_MASK(M_BATStopChgStatL,ENCUSTOMER);
  } else {
    ACOFF_SOURCE=0;
    ACOFF_LOW();

    Hook_CHGIC_AdapterOut(); 
  }

  if (IS_MASK_SET(u4SenseSTS0, ACSts)) {
    ACDis;
    ACattachedDis;
    AcDis_changed = 1;
  }
  if (AcDis_changed) {
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);					
  }
  CLEAR_MASK(u4SenseSTS3, SomeAcAdapterIn);
  CLEAR_MASK(u4PeriSTS3, UNSUPPORTED_AC );
  AttachedPowerNOTsupportDis;  
  AvgCurrentL = 0;
  AvgCurrentH = 0;
  Bat0x0BFakeCnt = 60;
  pFakeCHGCnt = 0;

  CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
  CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
	CLEAR_MASK(uProject0,NeedRecoveryNotSupportADP);
	CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);


  M_CTRL_LOW();  
  S_CTRL_LOW();
  M_BAT_Status1 =0;
  S_BAT_Status1 =0;  
  CLEAR_MASK(M_BATFlag1,pEdgeProjectBATWake); 
  CLEAR_MASK(S_BATFlag1,pEdgeProjectBATWake);   
  Trickle_ChgIC(Disable);

  BAT_Average_curr_sum =0;
  BAT_Average_curr_CNT =0;
  
	ADPI_MUL = ClassicAdaptor_ADPI_MUL;	//

	AD_ID_Detect_Delay = 0;

	BAT_CAPTripFlag = 0x00;		// clear battery cap trip flag.////A485D00027

	PowerSourceSwitchDelay = 20;

#ifdef OEM_Hook_AdapterOut_support
    OEM_Hook_AdapterOut();
#endif // OEM_Hook_AdapterOut_support   
}


//-----------------------------------------------------------------------------
//power button was pressed
//-----------------------------------------------------------------------------
void PSWPressed(void)
{
#if SW_ISCT	
  const CBYTE LShift_Pressed[] = {0x12,0xF0,0x12,0x00};		
#endif
  if( PWRBTN_WDTFLAG != 0x87 )
  PWRBTN_WDTFLAG = 0x78;

  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
    Buffer_Key(0xE0);    //
    Buffer_Key(0x25);    //
    F_Service_SEND = 1;
  }

  EC_ON_HI();
  EC_ON2_HI();//A285D00066: fix cs18-dock S5 adapter in light unbrightness.
  ECSleepCount = 3;
  AutoTimer = 0;    // Clear Auto timer.

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      if (((M_BATdata_ready != 1)&&(S_BATdata_ready != 1))&&(!Read_AC_IN()||Read_ACOFF()))
      {
      	if(M_BATdata_ready != 1)
      	{
        	FirstGetBatData(M_BAT);
        	if (M_BATdata_ready != 1)
        	{
          		M_BATdata_ready = 0xFF;
          		//break;
        	}
      	}
		if(S_BATdata_ready != 1)
      	{
        	FirstGetBatData(S_BAT);
        	if (S_BATdata_ready != 1)
        	{
          		S_BATdata_ready = 0xFF;
          		//break;
        	}
      	}
		if((M_BATdata_ready == 0xFF)&&(S_BATdata_ready == 0xFF))
			{
			break;
		}
      }
	  ////-->Stard A485D00011
      //if(NoBattExistGet && IS_MASK_CLEAR(TypeCStatus,TypeCAdpReady) && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeADPIDReady))
      //{
      //	break;
      //}
	  //-->End A485D00011
      if(NoBattExistGet && AttachedPowerNOTsupportGET)
      {
      	break;
      }
      if (((RelativeStateOfChgL <= 0x05)&&(SecRelativeStateOfChgL <= 0x05))&&(!Read_AC_IN()||Read_ACOFF()))
      {
        LOWBATT_3TIMES = 0;    // Clear delay time.
      }
      if ((!Read_AC_IN()||Read_ACOFF()))      // if DC mode
      {
        M_BATSMbusFailCount = 0;
        M_BATErrorCnt = 0;
        S_BATSMbusFailCount = 0;
        S_BATErrorCnt = 0;     
      }
      if( IS_MASK_SET(SYS_MISC1,Crisis_On) )
      {
        if(  (!EC_KBFNON()) || (!CheckCrisisMode()) )
        {
          CLEAR_MASK(SYS_MISC1,Crisis_On);
        }
      }
	  #if Support_SMTADP //A485D00011
      Edge_SMTADPProcess();
	  #endif

     PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
      break;

    case SYSTEM_S3 :
    case SYSTEM_S0 :
      if( PwrBtnWarnGET )
      {
        if( PowerOffWarnGET )
        {
          u4BeepID = 0x08;
        }
      }
      else
      {
#if SW_ISCT
        if ( IS_MASK_SET(AOAC_STATUS, ISCT_S0_ISCT))  // Check ISCT enable?
        {
          Buffer_String(LShift_Pressed);                // L-shift
        }
        else
#endif // SW_ISCT
        {
#if chPWSW1WDT	
          if (IS_MASK_CLEAR(pProject4,pPWSWdisable))  //power button for 7s reset-2015/7/17 
		  {
	        PM_PWRBTN_LOW();
	      }
#else
			PM_PWRBTN_LOW();
#endif //chPWSW1WDT			
        }
      }
      break;

    default :
      break;
  }
}

//-----------------------------------------------------------------------------
//power button was released
//-----------------------------------------------------------------------------
void PSWReleased(void)
{
  if( PWRBTN_WDTFLAG != 0x87 )
  PWRBTN_WDTFLAG = 0x00;

  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
    Buffer_Key(0xE0);  //
    Buffer_Key(0xF0);  //
    Buffer_Key(0x25);  //
    F_Service_SEND = 1;
  }

  // Clear PswPressedCounter
  CountSecAfterPswPressed = 0;
  
  PSW_COUNTER = T_PSWOFF;  // initialize counter
  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
    case SYSTEM_S0 :
      PM_PWRBTN_HI();
      break;

    default :
      break;
  }
}
//-----------------------------------------------------------------------------
//power button was overrided
//-----------------------------------------------------------------------------
void PSWOverrided(void)
{
  if( Read_EC_PWRBTN() )
  {
    pPWRRSTMark1 = 0x78;
    pPWRRSTMark2 = 0x87;
  }
  else
  {
    pPWRRSTMark1 = 0x00;
    pPWRRSTMark2 = 0x00;
  }
  if(!Read_LID_SW_IN())
  { return; }
#if chPWSW1WDT
	if (IS_MASK_CLEAR(pProject4,pPWSWdisable))  //power button for 7s reset-2015/7/17 
 	{
#endif  //chPWSW1WDT	
	  if (SystemIsS0 || SystemIsS3)
	  {
	    if (Read_EC_PWRBTN())
	    {
	      PSW_COUNTER--;
	      if (PSW_COUNTER == 0)
	      {
	        PWSeqStep = 1;
	        PowSeqDelay = 0x00;
	        SysPowState=SYSTEM_S0_S5;
	      }
	    }
	    else
	    { PSW_COUNTER = T_PSWOFF; }   // initialize counter
#if chPWSW1WDT		
	  }
#endif  //chPWSW1WDT	  
	}
}



//-----------------------------------------------------------------------------
//LAN_WAKE was low
//-----------------------------------------------------------------------------
void LanWakeLow(void)
{
#if Support_EC_LANWake
//  if (uReserve07.fbit.nPmeWakeEN==0) return;
  if (!WOLACWakeEnGet
  	 && IS_MASK_CLEAR(TypeCStatus2,DockingEvent)  //A485D00038 
  	 )
  	 {
    return;
  }
  if ((!WOLDCWakeEnGet) && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)
  	 && IS_MASK_CLEAR(TypeCStatus2,DockingEvent)  //A485D00038
  	 )
  	 {
    return;                // DC mode
  }

//A285D00041:Fix sysem cannot be WoLAN from S5 (G3 then plugin AC) with magic packet
    if(!Read_EC_ON() && !Read_VLAN_PWR())
    {
        return;
    }
  switch (SysPowState)
  {
    case SYSTEM_S4 :
    case SYSTEM_S5 :
    	if(IS_MASK_SET(TypeCStatus2,DockingEvent))
    	{
    		CLEAR_MASK(TypeCStatus2,DockingEvent);
    		EvDockingWOLOn;
			RamDebug(0x99);// A285D00024+
			RamDebug(0x05);// A285D00024+
    	}
    	else
    	{
      PMEWakeEn;
      }
      EC_ON_HI();
      EC_ON2_HI();//A285D00066: fix cs18-dock S5 adapter in light unbrightness.
      EC_WAKE_OUT_LOW();

      if (WOLACWakeEnGet || WOLDCWakeEnGet) {	 // A285D00024+
        PWSeqStep = 1;
        PowSeqDelay = 0x00;
        SysPowState=SYSTEM_S5_S0;
      }
      break;
    case SYSTEM_S3 :
    	if(IS_MASK_SET(TypeCStatus2,DockingEvent))
    	{
    		CLEAR_MASK(TypeCStatus2,DockingEvent);
    		EvDockingWOLOn;
			RamDebug(0x99);
			RamDebug(0x03);
    	}
    	else
    	{
      PMEWakeEn;
      }
#if SW_ISCT
      if ( IS_MASK_SET(AOAC_STATUS, ISCT_Enable) )  // Check ISCT enable?
      {
        PM_PWRBTN_LOW();
        AOAC_STATUS |= 0x80;  // Set PME(LAN) wake function.
      }
      else
#endif // SW_ISCT
      {
        EC_WAKE_OUT_LOW();
      }
      break;
    //A285D00063: add wol flag when power sequence on s3 to s0. start+
    case SYSTEM_S3_S0:
        if(IS_MASK_SET(TypeCStatus2,DockingEvent))
        {
            CLEAR_MASK(TypeCStatus2,DockingEvent);
            EvDockingWOLOn;
            RamDebug(0x99);
            RamDebug(0x30);
        }
        else
        {
            PMEWakeEn;
        }
        break;
    //A285D00063: add wol flag when power sequence on s3 to s0 end.
    case SYSTEM_S0 :
      break;

    default :
      break;
  }
#endif  // Support_EC_LANWake
}

//-----------------------------------------------------------------------------
//LAN_WAKE was hi
//-----------------------------------------------------------------------------
void LanWakeHi(void)
{
#if Support_EC_LANWake
//  if (uReserve07.fbit.nPmeWakeEN==0) return;

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
      EC_WAKE_OUT_HI();
      break;

    case SYSTEM_S0 :
      EC_WAKE_OUT_HI();
      break;

    default :
      EC_WAKE_OUT_HI();
      break;
  }
#endif  // Support_EC_LANWake
}

//-----------------------------------------------------------------------------
//WLAN_WAKE was low
//-----------------------------------------------------------------------------
void WLanWakeLow(void)
{
#if Support_WLAN_WAKE
//  if (uReserve07.fbit.nPmeWakeEN==0) return;
  if (!WOLACWakeEnGet)
    return;
  if ((!WOLDCWakeEnGet)&& IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )
    return;                // DC mode

  switch (SysPowState)
  {
    case SYSTEM_S4 :
    case SYSTEM_S5 :
//      PMEWakeEn;
      EC_ON_HI();
      EC_ON2_HI();//A285D00066: fix cs18-dock S5 adapter in light unbrightness.

      PWSeqStep = 1;
      PowSeqDelay = 0x00;
      SysPowState=SYSTEM_S5_S0;
      break;

    case SYSTEM_S3 :
//      PMEWakeEn;
#if SW_ISCT
      if ( IS_MASK_SET(AOAC_STATUS, ISCT_Enable) )  // Check ISCT enable?
      {
//        PM_PWRBTN_LOW();
//        AOAC_STATUS |= 0x80;  // Set PME(LAN) wake function.
        if(IS_MASK_CLEAR(APSHDDFLAG,b7RTCWAKE))
        {
          ISCT_Timer = 0;
          ISCT_OPEN_APS();											
          APSHDDFLAG = 0;
          SET_MASK( APSHDDFLAG , b6PMEWAKE);
          CLEAR_MASK( APSHDDFLAG , b7RTCWAKE);
          ISCT_Again = 2;  // Last 2 Sec count for ISCT used.
          SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
        }
      }
      else
#endif // SW_ISCT
      {
        //if ( IS_MASK_SET(SYS_STATUS,AC_ADP) )
        { PM_PWRBTN_LOW(); } // Set PME(LAN) wake function.
      }
      break;

    case SYSTEM_S0 :
      break;

    default :
      break;
  }
#endif //Support_WLAN_WAKE  
}

//-----------------------------------------------------------------------------
//WLAN_WAKE was hi
//-----------------------------------------------------------------------------
void WLanWakeHi(void)
{
#if Support_WLAN_WAKE
//  if (uReserve07.fbit.nPmeWakeEN==0) return;

  switch (SysPowState)
  {
    case SYSTEM_S4 :
      break;

    case SYSTEM_S5 :
      break;

    case SYSTEM_S3 :
      PM_PWRBTN_HI();
      break;

    case SYSTEM_S0 :
      PM_PWRBTN_HI();
      break;

    default :
      PM_PWRBTN_HI();
      break;
  }
#endif //Support_WLAN_WAKE  
}

//-----------------------------------------------------------------------------
// battery 1 in function
//-----------------------------------------------------------------------------
void Battery1In(void)
{
  BatteryIn(M_BAT);
  SettingBattOCP(M_BAT);
}

//-----------------------------------------------------------------------------
// battery 1 out function
//-----------------------------------------------------------------------------
void Battery1Out(void)
{
  BatteryOut(M_BAT);
  M_CTRL_LOW();
  
}

//-----------------------------------------------------------------------------
// battery 2 in function
//-----------------------------------------------------------------------------
void Battery2In(void)
{
  	BatteryIn(S_BAT); 
  SettingBattOCP(S_BAT);
}

//-----------------------------------------------------------------------------
// battery 2 out function
//-----------------------------------------------------------------------------
void Battery2Out(void)
{
//check debug card Out
#if Support_SMB_debug_card
  if (CheckDebugCardOut())
    return;
#endif //#if Support_SMB_debug_card

  	BatteryOut(S_BAT);
  	S_CTRL_LOW(); 
}

//-----------------------------------------------------------------------------
// battery in function
//-----------------------------------------------------------------------------
void BatteryIn(int battery_id)
{
  BYTE Temp_BAT_CH, Temp_BAT_ADD;
  if (battery_id == M_BAT)   //1st battery
  {
    Temp_BAT_CH = chSMB_BAT;
    Temp_BAT_ADD = SmBat_Addr;

	CLEAR_MASK(BAT_CAPTripFlag,MBAT_triggered); //A485D00027
////A485D00029:Modify Read BATMakerID&FWUpdate flow.--->Start
	bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr,C_FW_Status,&FirmwareUpdateStatusL,SMBus_NoPEC);
    bRSMBusBlock(chSMB_BAT,SMbusRBK,SmBat_Addr,C_BAT_Info,&BatterymakerIDL, bRead_I2C_NULL);	
//A485D00029:Modify Read BATMakerID&FWUpdate flow.--->end
    //@LCFC_CUSTOMIZE+
    if(IS_MASK_SET(uProject0, uFUDUpdateTimes) && IS_MASK_CLEAR(uProject0,uSysTimeOK))
    {
      // When come here, Get 0x19 command and System is pass.
      // We have to make sure FUD can be updated when Battery0 plug in.
      CLEAR_MASK(uProject0,uFUDUpdateTimes);
      SET_MASK(uProject0,uFUDWriteEN);
    }
    //@LCFC_CUSTOMIZE-    
  }else {                    //2nd battery
    Temp_BAT_CH = chSMB_2ndBAT;
    Temp_BAT_ADD = SecSmBat_Addr;
	CLEAR_MASK(BAT_CAPTripFlag,SBAT_triggered); //A485D00027
//A485D00029:Modify Read BATMakerID&FWUpdate flow.--->Start
	bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr,C_FW_Status,&SecFirmwareUpdateStatusL,SMBus_NoPEC);
    bRSMBusBlock(chSMB_2ndBAT,SMbusRBK,SecSmBat_Addr,C_BAT_Info,&SecBatterymakerIDL, bRead_I2C_NULL);
 //A485D00029:Modify Read BATMakerID&FWUpdate flow.--->end
    //@LCFC_CUSTOMIZE+
    
    if(IS_MASK_SET(uBattFUDStatus, uFUDUpdateTimes) && IS_MASK_CLEAR(uProject0,uSysTimeOK))
    {
      // When come here, Get 0x19 command and System is pass.
      // We have to make sure FUD can be updated when Battery1 plug in.    
      CLEAR_MASK(uBattFUDStatus,uFUDUpdateTimes);
      SET_MASK(uBattFUDStatus,uFUDWriteEN2);
    }
    //@LCFC_CUSTOMIZE-  
  }
  
  SET_MASK(OEM_BAT_1B(PriBattSTS,battery_id),BattExist);    //set battery flag                          

  FirstGetBatData(battery_id);                              //first get battery data

  SET_MASK(BAT_Unship_mode, (M_unship_mode << battery_id)); 
  Unlock_ShipMode(battery_id);  


#if Support_SMB_debug_card
  if (OEM_BAT(M_BATdata_ready, battery_id) !=1){
    if (CheckDebugCardIN()){
      CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),BattExist);
    }
  }
#endif //Support_SMB_debug_card
  
  SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATIn); 
  CHK_Trickle_Charge(battery_id);


  SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATChange);
  Service_Auth_Step = 1;        // start SHA1
#if Thinkpad_Battery_HW_Auth
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
#endif
  CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTUnder20);
  CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTOver20);
  //SetPowerBatteryparameter();
  //Mos: Clean Get_Batt_debounce_count to make it check battery data stable or not?
  Get_Batt_debounce_count = 0;
  if(((Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,(EEPROM_MBATStartCharge + (battery_id << 1) ))!= 0)&&(Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStopCharge + (battery_id << 1)) != 0))
    &&((Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStartCharge + (battery_id << 1))<= 0x64)&&(Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStopCharge + (battery_id << 1))<= 0x64)))
  {
    OEM_BAT(M_BATStartCharge,battery_id) = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStartCharge + (battery_id << 1));
    OEM_BAT(M_BATStopCharge,battery_id) = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStopCharge+ (battery_id << 1));
  }
  else{
    OEM_BAT(M_BATStartCharge,battery_id) = 95;
    OEM_BAT(M_BATStopCharge,battery_id) = 100;
  }

  cBATTThrottling = 0;
  CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
  
#if Lenovo_Authentication
  SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_SWstart);
#endif  //Support_SmartCharge

#if Support_SmartCharge
  WSMbusTemp02=0x00; 
  WSMbusTemp01=0x01;  
  if(bRWSMBus(Temp_BAT_CH, SMbusWW, Temp_BAT_ADD, C_SmartCharge,&WSMbusTemp01,SMBus_NeedPEC))
  {
    OEM_BAT(M_BATSMbusFailCount,battery_id)++;
  }
#endif   //Support_SmartCharge
  CLEAR_MASK(TWR_BAT(TWR00,battery_id),AuthenticationFailed );
  CLEAR_MASK(OEM_BAT_1B(PriBattSTS,battery_id),NonGenBatt);  // BIOS check this flag for SW auth result. Clear this flag will let BIOS check pass.

  Hook_CHGIC_BatIn();
  
#ifdef OEM_Hook_BatteryIn_support
    OEM_Hook_BatteryIn();
#endif // OEM_Hook_Battery1In_support 
}

//-----------------------------------------------------------------------------
// battery out function
//-----------------------------------------------------------------------------
void BatteryOut(int battery_id)
{
  CBYTE* BAT1_pntr;
  
  CLEAR_MASK(OEM_BAT_1B(PriBattSTS,battery_id),BattExist); 
  CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake);
  if ( NoBattExistGet )  //no battery attached
  {
    BAT_Trickle_Charge_step = 0;
    CLR_IDPM_EN;
    H_PROCHOT_HI();
    VGA_AC_BATT_OFF();
    nBattAverTemp =0;
    nBattTempCnt =0;
  // Set P-State highest to force that ChkBattery_OCP will decress P-State step by step.
    cBATTThrottling = CPUMAXPSTATE;
    cGPUThrottling = GPUMAXPSTATE;   
    SET_MASK(OEM_BAT(M_BATAlarm,battery_id),ADPOUT);
    SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
    ADPI2Sec = 200;
    GPU2Sec = 200; 
 // Send QEvent to Bios
    ThrottlingControl();	
    BAT_state_target(BAT_SEL_INIT);
	//A485D000109:--->Start
	if(IS_MASK_SET(SYS_STATUS,AC_ADP))
	{
     if((CurrentRDO!= ObjectPosition20V) || IS_MASK_SET(PDOVoltageFlag, Port1PDOVoltage9V))
		{
			//SendPdoSet(ObjectPosition20V,TYPECPort1);	// Switch PDO to 20V
           		SendPDO9VTo20V(TYPECPort1);
			//adapter port exist reRead watt.
			if(IS_MASK_SET(TypeCStatus,TypeCAdpExist)&&IS_MASK_SET(TypeCStatus,TypeCAdpReady))
			{
				//reRead battery watt
				TypeCAdpPdStableCheck = 5;
				TypeCAdpDetectDelay = 5;
			}
		}
	 
	 if((Port2CurrentRDO != Port2ObjectPosition20V) || IS_MASK_SET(PDOVoltageFlag, Port2PDOVoltage9V))
		{
			//SendPdoSet(Port2ObjectPosition20V,TYPECPort2);	// Switch PDO to 20V
			SendPDO9VTo20V(TYPECPort2);
			//adapter port exist reRead watt.
			if(IS_MASK_SET(TypeCPort2Status,TypeCAdpExist)&&IS_MASK_SET(TypeCPort2Status,TypeCAdpReady))
			{
				//reRead battery watt
				TypeCPort2AdpPdStableCheck = 5;
				TypeCPort2AdpDetectDelay = 5;
			}
		}

	}
		//A485D000109:--->End
  }

  if (IS_MASK_CLEAR(M_BATFlag1,pEdgeProjectBATWake) && IS_MASK_CLEAR(S_BATFlag1,pEdgeProjectBATWake)){
    Trickle_ChgIC(Disable);
  }
  	OEM_BAT(M_BAT_pFKTemp,battery_id) = 0;
	OEM_BAT(M_BAT_Batt0x3CFailRetry5sTimer,battery_id) = 0; 
  CLEAR_MASK(nBatteryStatH, PreBat_DISCHARGE << (battery_id * 2));
  CLEAR_MASK(nBatteryStatH, PreBat_CHARGE << (battery_id * 2));;

  OEM_BAT(M_BATOvrTempCnt,battery_id) =0;
  OEM_BAT(M_BATErrorCnt,battery_id) =0;
  OEM_BAT(M_BATStopChgStatL,battery_id) =0;
  OEM_BAT(M_BATStopChgStatH,battery_id) =0;
  OEM_BAT(M_BAT_Status1, battery_id) =0;


  BAT1_pntr = ManufacturerName + (battery_id << 8);  // Base on address 0x048F.
  for( ITempW01=0; ITempW01<=16; ITempW01++ )  // Clear 048F~049F.
  {
    *BAT1_pntr=0;
    BAT1_pntr++;
  }

  TWR_BAT(RelativeStateOfChgL,battery_id) = 0x00;
  TWR_BAT(RelativeStateOfChgH,battery_id) = 0x00;
  TWR_BATClear(battery_id);			
  IBMbatteryStore();
  RST_ChgTimeOutCnt(battery_id);
  StopChgIC();

  OEM_BAT(M_BATdata_ready,battery_id) = 0;
  OEM_BAT(M_BATSMbusFailCount,battery_id) = 0;
  OEM_BAT(M_BATErrorCnt,battery_id) = 0;
  OEM_BAT(M_BATAlarm,battery_id) = 0;
  SHA1failCnt = 0;
//  Bat1_FPChgFlag = 0;
  SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATIn);
  SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATChange);
  CLEAR_MASK(TWR_BAT(TWR06,battery_id),HWauthSupport);
 

#if Lenovo_Authentication
  
  OEM_BAT(M_BAT_edgeProject4,battery_id) = 0x00; 	// clear Auth status.
  OEM_BAT(M_BAT_uAuthenFlag,battery_id) = 0;
  OEM_BAT(M_BAT_uAuthenNPackH,battery_id) = 0;
  OEM_BAT(M_BAT_uAuthenNPackL,battery_id) = 0;  
  CLEAR_MASK(OEM_BAT(M_BATStopChgStatH,battery_id),NotLenovoBattery);
  CLEAR_MASK(TWR_BAT(TWR00,battery_id),AuthenticationFailed);
#if Thinkpad_Battery_HW_Auth
  //CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
  CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
#endif
  CLEAR_MASK(TWR_BAT(BattAuthStat,battery_id),HWauthFail);
  CLEAR_MASK(OEM_BAT_1B(u4PriBattType, battery_id),u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.

  TWR_BAT(BattAuthStat,battery_id) = 0;
  OEM_BAT(M_BAT_uAuthenFailCnt,battery_id) = 0;
//  p2AuthFailCnt = 0;

  CLEAR_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_SWstart);
  OEM_BAT(M_BATin_delay,battery_id) = 20 ;
  OEM_BAT_1B(uThresholdSetting,battery_id) = 0; //Clear Battery Threhold setting
#endif
  Update_ChargeIC(); 			
  Hook_CHGIC_BatOut();

  Chk_Trickle_Setting();
  
#ifdef OEM_Hook_BatteryOut_support
    OEM_Hook_BatteryOut();
#endif // OEM_Hook_Battery1Out_support 
}




//  +
const BYTE LCtrl_Pressed[] = {0x14,0x00};		

void KBFNPressed(void)
{
  union KEY key;
  //
  // Send scancode by KSI:04 & KSO:0E which coresponding 0x8E in CS13X standard keyboard matrix
  //
  key.field.input = 0x04;
  key.field.output = 0x0C; //A485D00015 0x0E->0x0C
  key.field.debounce_status = 1;
  Send_Scan2(key, MAKE_EVENT);

//   DebugCombineKey=0x01;
  SET_MASK(ComboKeyState, ComboKeyValid);   // ComboKeyValid = TRUE;
  ComboKeyState++;      // Start count, bit0:4 for counter use.
  LastComboKey = 0x8E;  // Fn
}

//const BYTE LCtrl_Released[] = {0xF0,0x14,0x00};

void KBFNReleased(void)
{
  union KEY key;
  //
  // Send scancode by KSI:04 & KSO:0E which coresponding 0x8E in CS13X standard keyboard matrix
  //
  key.field.input = 0x04;
  key.field.output = 0x0C;  //A485D00015 0x0E->0x0C
  key.field.debounce_status = 1;
  Send_Scan2(key, BREAK_EVENT);

//   CombineKeyRN=0x00;
  ComboKeyState &= 0x80;  // Clear ComboKeyState except ECDebugMode. ComboKeyValid = FALSE;
  LastComboKey = 0xFF; // Clear
}
//  -

// ----------------------------------------------------------------------------
// Device insert/remove debounce routine.
// input:   device_id
// ----------------------------------------------------------------------------
const struct sDebounce Debounce_TBL[] = 
{
	// REG         // Flag          // Cunter          // Timer            // press        // release     // nochange
  {&EVT_STATUS1  ,wait_PSW_off    ,&DEBOUNCE_CONT1   ,T_PSW_DEBOUNCE     ,PSWPressed     ,PSWReleased   ,PSWOverrided   },  //0
  {&EVT_STATUS1  ,adapter_in      ,&DEBOUNCE_CONT2   ,T_AC_DEBOUNCE      ,AdapterIn      ,AdapterOut    ,NullEvent      },  //1
  {&EVT_STATUS1  ,M_bat_in        ,&DEBOUNCE_CONT3   ,T_BAT_DEBOUNCE     ,Battery1In     ,Battery1Out   ,NullEvent      },  //2
  {&EVT_STATUS1  ,S_bat_in        ,&DEBOUNCE_CONT4   ,T_BAT_DEBOUNCE     ,Battery2In     ,Battery2Out   ,NullEvent      },  //3
  {&EVT_STATUS1  ,LAN_WAKE_Status ,&DEBOUNCE_CONT5   ,T_LAN_WAKE_DEBOUNCE,LanWakeLow     ,LanWakeHi     ,NullEvent      },  //4
  {&EVT_STATUS1  ,KB_FN_Status    ,&DEBOUNCE_CONT6   ,T_KBFN_DEBOUNCE    ,KBFNPressed    ,KBFNReleased  ,NullEvent      },  //5
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT7   ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //6
  {&EVT_STATUS1  ,WLAN_WAKE_Status,&DEBOUNCE_CONT8   ,T_LAN_WAKE_DEBOUNCE,WLanWakeLow    ,WLanWakeHi    ,NullEvent      },  //7
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT9   ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //8
#if Support_BOTTOM_TAMPER_FUNC
  {&EVT_STATUS2  ,BottomCoverFlag ,&DEBOUNCE_CONT10   ,T_BOTTOM_COVER_DEBOUNCE  ,DCoverOpened,NullEvent     ,NullEvent      },
#else
  {&EVT_STATUS2  ,DummyFlag       ,&DEBOUNCE_CONT10  ,T_EXTEVT_DEBOUNCE  ,NullEvent      ,NullEvent     ,NullEvent      },  //9
#endif  
};


// ----------------------------------------------------------------------------
// EventManager function
//
// Note : 
//	If want to call the function of status not change, please set nochange_func = 1
// ----------------------------------------------------------------------------
void EventManager(BYTE device_id) 
{
	BYTE new_state,old_state;
	BYTE device_flag;
	BYTE *Tmp_XPntr;
	BYTE *Tmp_XPntr1;

	switch(device_id)
  	{
    	case 0 :
#if EventDebug
				new_state = bITEDebug;
#else
        if (((M_BATdata_ready == 0xFF)&&(S_BATdata_ready == 0xFF))&&(!Read_AC_IN()||Read_ACOFF()))  
      			{
        			new_state = !Read_EC_PWRBTN();
        			old_state = Read_EC_PWRBTN();
      			}
      			else
      			{ 
      				if( PWRBTN_WDTFLAG == TRUE )
      				{
      					if( Read_EC_PWRBTN() )
      					{
      						new_state = 0;
      					}
    					else
    					{
    						PWRBTN_WDTFLAG = FALSE;
    					}
      				}
    				else
    				{
            			new_state = Read_EC_PWRBTN(); 
        			}
      			}
#endif	// End of EventDebug
      			break;

    	case 1 :
				#if EventDebug
				new_state = bITEDebug;
				#else
      			new_state = Read_AC_IN();
				#endif	// End of EventDebug
      			break;

    	case 2 :
				#if EventDebug
				new_state = bITEDebug;
				#else
      			//if((M_BATTEMP<0x390))  //A485D00014-
      			if(Read_MAIN_BAT_IN())   //A485D00014+
      			{ new_state = 1; }
      			else
      			{ new_state = 0; }
				#endif	// End of EventDebug
      			break;

    	case 3 :
				#if EventDebug
				new_state = bITEDebug;
				#else
#if Support_TWO_BATT
 	 //if ((S_BATTEMP<0x390))  //A485D00014-
 	 	#if Only_One_BATT
		new_state = 0;  //A285D00009: Change to support single battery
	 	#else
 	 	if(Read_SEC_BAT_IN())  //A485D00014+
		{ new_state = 1; }
		else
		{ new_state = 0; }
	 	#endif	// end of Only_One_Batt
      break;
#else
      new_state = 0x00;
      break;      
#endif //Support_TWO_BATT    
				#endif		// End of EventDebug
      			break;

    	case 4 :
				#if EventDebug
				new_state = bITEDebug;
				#else
				#if  Support_EC_LANWake
      			new_state = (Read_LAN_WAKE_IN()||Read_SYS_LAN_WAKE_IN()); //// A485D00031:Enable AC/DC wake on Lan function
				#else
      			new_state = 0x00;
				#endif  // Support_EC_LANWake
				#endif		// End of EventDebug
      			break;

    	case 5 :
				#if EventDebug
				new_state = bITEDebug;
				#else
				#if Support_EC_KBFN
      			if( EC_KBFNON() )
      			{
        			new_state = 1;
      			}
      			else
      			{
        			new_state = 0;
      			}
				#else
      			new_state = 0x00;
				#endif  // Support_KBFN
				#endif		// End of EventDebug
      			break;

    	case 6 :
				#if EventDebug
				new_state = bITEDebug;
				#else
      			new_state = 0x00;
				#endif		// End of EventDebug
      			break;

    	case 7:
				#if EventDebug
				new_state = bITEDebug;
				#else
				#if Support_WLAN_WAKE      
      			new_state = Read_WLAN_WAKE_IN();
				#else
      			new_state = 0x00;
				#endif
				#endif		// End of EventDebug
      			break;

    	case 8:
				#if EventDebug
				new_state = bITEDebug;
				#else
      			new_state = 0x00;
				#endif		// End of EventDebug
      			break;

    	case 9:
      			#if EventDebug
				new_state = bITEDebug;
				#else
      			new_state = 0x00;
#if Support_BOTTOM_TAMPER_FUNC
           if(Read_D_Cover_SW()) {
             new_state = 1;
           }
#endif        
				#endif		// End of EventDebug
      			break;

    	default :
      			new_state = 0x00;
      			break;
  	}

	Tmp_XPntr = Debounce_TBL[device_id].REG_Event;
	Tmp_XPntr1 = Debounce_TBL[device_id].Cunter_Event;
	device_flag = Debounce_TBL[device_id].Flag_Event;

	old_state = ((*Tmp_XPntr & device_flag) != 0x00);
	if(new_state != old_state)	
	{		 
		if((*Tmp_XPntr1) == 0x00)	
		{	
			(*Tmp_XPntr1) = Debounce_TBL[device_id].Time_Event;	
		}	 
		else 
		{	
			(*Tmp_XPntr1) --;	
			if((*Tmp_XPntr1) == 0)	
			{	 
				if(new_state)	
				{	
					(Debounce_TBL[device_id].press)();
					*Tmp_XPntr |= device_flag;
				}
				else 
				{	
					(Debounce_TBL[device_id].release)();
					*Tmp_XPntr  &= (~device_flag);
				}
			}  
		}  
	}
	else
	{ 	
		*Tmp_XPntr1 = 0x00;	
	//	(Debounce_TBL[device_id].nochange)();		// carl not true  only test
	}
	
	(Debounce_TBL[device_id].nochange)();
}




//*****************************************************************************
//
//  oem_main.c
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
extern int SENSOR_CODE pure_ec_sensor_poweroff_done(void);

#if DEF_HOOK_FUNCTION_WITH_SERVICE_1MS
//*****************************************************************************
//
//  The hook function for "service_1mS()"
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void Hook_Service_1ms(void)
{

}
#endif

//------------------------------------------------------------
// Hook 1ms events
// EventId is 0 ~ 9 cycle
//------------------------------------------------------------
void Hook_Timer1msEvent( )
{
	#if 0
	if(Read_AC_IN_pin())
	{
		if((Attach2status&0x3F) >= 10)
		{
			if(Read_DCIN_ATTACHED2())
			{
				SET_MASK(Attach2status,StableLLevel);
			}
			else
			{
				CLEAR_MASK(Attach2status,StableLLevel);
			}
		}
		else
		{
			Attach2status++;
		}
	}
	else
	{
		Attach2status = 0;
		if(Read_DCIN_ATTACHED2())
		{
			SET_MASK(Attach2status,StableLLevel);
		}
		else
		{
			CLEAR_MASK(Attach2status,StableLLevel);
		}
	}
	#endif
	
	EventId ++;
	if(EventId>=max_EVENTSIZE)
	{
		EventId = 0x00;
	}
	EventManager(EventId);

	Oem_SysPowerContrl();

	ADC_Check_Channelx_Data_Valid();

	if ( !EC_MainPwr() && SystemIsS0 )
  	{
    	PWSeqStep = 1;
    	PowSeqDelay = 0x00;
    	SysPowState = SYSTEM_S0_S5;
	  	SET_MASK(PWRCONTROL,MAINPWRONTrig);
  		//  DPWROK_EC_LOW();
  	}

	#if NV_GC6
  	GC6Process();
	#endif // NV_GC6
	SetVGA_AC_DET();
  	if ( !Read_ENBKL_IN() )
  	{ BKOFF_OFF(); }
  
  	#if NV_GC6
  	if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeGC6EventHI) )
  	{
    	if( !Read_GC6Event_ON() )  // GC6_event Hi
    	{
      		if( GC6EventCnt < 3 )
      		{
        		GC6EventCnt++;
      		}
      		else
      		{
        		CLEAR_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
        		GC6EventCnt = 0;
      		}
    	}
    	else
    	{
      		GC6EventCnt = 0;
    	}
  	}
	#endif // NV_GC6
  
	if (UCSI_50CMD_WA != 0)
	{
	  UCSI_50CMD_WA = 0;
		Hook_ACPICommand();
	}

	#ifdef OEM_Hook_Timer1msEvent_support
    OEM_Hook_Timer1msEvent();
	#endif // OEM_Hook_Timer1msEvent_support

	// add for Enhanced Beep alarm when MEM lost //A485D00087:
	do
	{
		if(!SystemIsS0)
			break;
	
		if(RAM_PLUG_STATUS == Read_EnBeep_Active())
			break;
		
		if(Read_EnBeep_Active())
		{
			if(++RAM_DETECT_PLUG_OUT_TIMES >= RAM_DETECT_STATUS_COUNT)
			{
				RAM_DETECT_PLUG_OUT_TIMES = 0;
				RAM_PLUG_STATUS = Read_EnBeep_Active();
				SET_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
				SET_MASK(EnhancedPcBeep_Status,BeepActive);
				EnhancedPcBeep_DATA[1] = 0x82;
			}
		}
		else
		{
			RAM_DETECT_PLUG_OUT_TIMES = 0;
			RAM_PLUG_STATUS = Read_EnBeep_Active();
		}
	
	}while(0);
	// add for Enhanced Beep alarm when MEM lost -  //A485D00087:

    
   //A285D00060 start+
    #if ENABLE_64G_KEY_WORK_RUN
    if(SystemIsS0)
    {
        if(USB_Delay != 0x00)
        {
            USB_Delay--;
            if(USB_Delay == 0x00)
            {
                USB_ON_OUTPUT;
                USB_ON_HIGH();
                AOU_EN_ON();
            }
        }
    }
    #endif
    
   //A285D00060 end+
  Command_Process_To_Host();
}

//------------------------------------------------------------
// Hook 5ms events
//------------------------------------------------------------
void Hook_Timer5msEvent(void)
{

#if !EN_PwrSeqTest
	#if Support_LenovoAPS
	if(GsensorStableDelay && Read_SUSP())
	{	//G-sensor power source is Vs, need delay after Vs turn on.
		GsensorStableDelay--;
	}
	#if Support_GSNS_I2C
  	ScanGsensorI2C();
	#else
  	ScanGsensorADC();
	#endif

  	if(CRCDelay!=0){
    	CRCDelay--;
    	if(CRCDelay==0){
      		unlockRFIDProtect();
    	}
  	}

  	LnvApsProtect();
	#endif  
	#endif

#if Support_TWO_BATT
  // Priam Start++
 // if( (!PriBattInhibGET )
	//		|| (!SecBattInhibGET)
//			)
  if (Eject_Enable()){
    if (Target_BAT_state == BAT_SEL_S) {
      if(BAT_Eject_Cnt == 0){ 
        StopChgIC();
        BAT_Eject_Cnt ++;
      } 
      else if (BAT_Eject_Cnt < 400 ){
      BAT_Eject_Cnt ++;
      } 
    }
    Batt_Eject_return_Cnt = 100;
    Select_Battery();
  }else{
    if(Batt_Eject_return_Cnt)
  	  {
  		  Batt_Eject_return_Cnt--;
  	  }
  	  else
  	  {
	    BAT_Eject_Cnt = 0; 
	  }
  }

  	Switch_Battery_Process();
#endif
   if( (!PriBattInhibGET )
	  //	  || (!SecBattInhibGET)
  		  )
   	{
  CHK_Trickle_Charge(Polling);
  Switch2Trickle_Charge(Polling);
  }
	//Detect external power source ID
	if(IS_MASK_SET(SYS_STATUS,AC_ADP))
	{
#if Support_TYPE_C
		//if(!Read_DCIN_ATTACHED2())
		//{	//Type-C adaptor
		if(Port_Select == TYPECPort2)
		{
			uEXTERNAL_POWER_SOURCE_ID = 0x42;
		}
		else
		{
			uEXTERNAL_POWER_SOURCE_ID = 0x41;
		}
		//}
		//else
#endif //Support_TYPE_C
		//{	//Ordinary adaptor
		//	uEXTERNAL_POWER_SOURCE_ID = 0x01;
		//}
	}
	else
	{	//Battery only
		uEXTERNAL_POWER_SOURCE_ID = 0x00;
	}
	if(GSensorSelfTestCountX)
	{
		GSensorSelfTestCountX--;
	}
#if Support_KBD_BL_LED
	KbdBacklightLED();
#endif //Support keyboard backlight

#ifdef OEM_Hook_Timer5msEvent_support
    OEM_Hook_Timer5msEvent();
#endif // OEM_Hook_Timer5msEvent_support  
    //A285D00075 start+
    if(PD4ccCMD_WaitCnt)
    {
        PD4ccCMD_WaitCnt--;
        if(0 == PD4ccCMD_WaitCnt)
        {
            CLEAR_MASK(PDStatus, TypeC4CCCmd);
        }
    }
    ///A285D00075 end+
}

//------------------------------------------------------------
// Hook 10ms events
//------------------------------------------------------------
void Hook_Timer10msEventA(void)
{
//WA : DAx75 prochot fixed 100mS
   if (AC_OUT_Prochot_CNT > 1){
     AC_OUT_Prochot_CNT--;
     if (AC_OUT_Prochot_CNT ==1){
       H_PROCHOT_LOW();
       AC_OUT_Prochot_CNT--;
     }
   }
//WA : DAx75 prochot fixed 100mS
    Thermal_monitor_En;

   	if( tmpSSKeyCnt > 0 )
  	{
  	  tmpSSKeyCnt--;
	  if(tmpSSKeyCnt == 0)
	  {
  		simple_code(tmpSSKey2, tmpEvent);  
	  }
  	}

  	Backlight_Control();
	#if !EN_PwrSeqTest

  	if (PWRBTN_pressCnt !=0)
  	{
    	PWRBTN_pressCnt--;
    	if (PWRBTN_pressCnt==0)
    	{
      		PM_PWRBTN_HI();
      		EC_WAKE_OUT_HI();
    	}
  	}
  	if ((LID_readytime != 0) && (Read_EC_ON()))
  	{ LID_readytime--; }
	#endif

  	PowerMonitor();
	#if Support_COMPAL_MFG_TOOL
  	ThermalMailBox();
	#endif


  if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeACCUTOFF) )
  {
//main battery  
    CLEAR_MASK(M_BATAlarm, HybridSTPP);
    CLEAR_MASK(M_BATAlarm, DISTURBO);
    CLEAR_MASK(M_BATStopChgStatH, HybridModeStopChg);
    CLEAR_MASK(M_BATStopChgStatH, ENADPTHROTTLING);
//second battery
    CLEAR_MASK(S_BATAlarm, HybridSTPP);
    CLEAR_MASK(S_BATAlarm, DISTURBO);
    CLEAR_MASK(S_BATStopChgStatH, HybridModeStopChg);
    CLEAR_MASK(S_BATStopChgStatH, ENADPTHROTTLING);
    
    CLEAR_MASK(pEdgeProjectFlag1, pEdgeACCUTOFF);
  }

#if Lenovo_Authentication
  	if( (IS_MASK_SET(M_BAT_uAuthenFlag, uDF_SWstart))
    	    && !PriBattInhibGET && (M_BAT_pFKTemp<30) && (1 == M_BATdata_ready))
  		LAuthProcess(M_BAT);
#endif
  	if( pPWRRSTCnt > 0 )
  	{
    	pPWRRSTCnt--;
    	if( pPWRRSTCnt == 0x00 )
    	{
      		OTP_RESET_LOW();
      		
    	}
  	}

	#if (PLATFORM_CHIP == AMD)
  	if (pAMDPWRBTNCnt > 0)
  	{
    	if(pAMDPWRBTNCnt >= (AMDPWRBTNWK - AMDPWRBTNPULSE)) {
      		PM_PWRBTN_LOW();
    	} else {
      		PM_PWRBTN_HI();
    	}

    	pAMDPWRBTNCnt--;
    	if( SystemNotS0 ) {
      		pAMDPWRBTNCnt = 0x00;
    	} else if ( pAMDPWRBTNCnt == 0x00 ) {
      	ProcessSID(0xEE);
      	RSMRST_shutdown();
    	}
  	}
	#endif // PLATFORM_CHIP

	#ifdef OEM_Hook_Timer10msEventA_support
    OEM_Hook_Timer10msEventA();
	#endif // OEM_Hook_Timer10msEventA_support

	#if Support_TYPE_C
	if(Read_EC_ON())    //A285D00070:fix 3vALW cannot low.
	//if(Read_RSMRST())   //A285D00065: Add PD control for i2c retry--ti spec
	{// Need check Type-C IC is working, HW design the power source is Valw(EC_ON control).
    	OEM_TYPE_C_HOOK();
  	}
  	UcsiHousekeeper(0x01);
	#endif //Support_TYPE_C	
}

//------------------------------------------------------------
// Hook 10ms events
//------------------------------------------------------------
void Hook_Timer10msEventB(void)
{
	#if !EN_PwrSeqTest
	ScanADCChannel();
	//Chk_Wrong_ADP(); //A485D00011
	#endif
	
	if( Read_AC_IN()) {
		#if Support_SMTADP
		//-->Stard A485D00011
		if (IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) && (AdapterID != AdapterID_NON_SUPPORT))
		#else
		if ((AdapterID != AdapterID_NON_SUPPORT) 
                    && (0 != AdapterID))
		#endif
		//-->end A485D00011
		{
		  ACEn;
		  ACattachedEn;
		  CLEAR_MASK( u4PeriSTS3, UNSUPPORTED_AC );
		}
		else
		{
		  ACDis;
		  ACattachedDis;
		  if (Read_AC_IN()) {
			SET_MASK( u4PeriSTS3, UNSUPPORTED_AC );
		  }
		}
		//A485D00055: Enable display adaptor watt--->Start
	 if(SystemIsS0)
	 	{
          u4ACAdaptorCap = CurrentTypeCWatt;
		  //A485D00057: Fix wake on lan Fail--->Start
		  AdapterID = u4ACAdaptorCap;
          //u4ACAdaptorCap = u4ACAdaptorCap;
          AdpWatt = u4ACAdaptorCap;
          SecAdpWatt = u4ACAdaptorCap;
		  //A485D00057: Fix wake on lan Fail--->End
	    }
	   //A485D00055: Enable display adaptor watt--->End
	}
  TWR_Initial();

#if Lenovo_Authentication
  	if( (IS_MASK_SET(S_BAT_uAuthenFlag, uDF_SWstart))
    	&& !SecBattInhibGET && (S_BAT_pFKTemp<30) )
  		LAuthProcess(S_BAT);
#endif

  if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeACINOUTTrigger) && IS_MASK_SET(SYS_STATUS,AC_ADP) )
  {
    if (IS_MASK_SET(u4SenseSTS0, ACSts)) {
      ECSMI_SCIEvent(ACPI_ACIN_SCI);
    } else {
//      ECSMI_SCIEvent(ACPI_ACOUT_SCI);
    }
    CLEAR_MASK(pEdgeProjectFlag1,pEdgeACINOUTTrigger);
  }
	
	if( IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) )
	{
		if(IS_MASK_SET(SYS_STATUS,AC_ADP) && (AD_ID_Detect_Delay == 1))//&& (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
    	{
			//if(!Read_DCIN_ATTACHED2() || (Read_DCIN_ATTACHED2() && (InputCurrent_Watt == ACCutoff_Watt)))
			{
      			if( ADPI_AvgData > WATT_TO_ADC_BASE(ACACOFFEn) )
      			{
        			if( (AdpCUTCnt > ACcutoff_Ts) && (AdpCUTCnt < (ACcutoff_Ts+2)) ) 
        			{
          				SET_MASK(SysStatus,ERR_ShuntDownFlag);
          				ProcessSID (AC_CUTOFF_ID);
          				H_PROCHOT_HI();
          				VGA_AC_BATT_OFF();
          				AdpCUTCnt = ACcutoff_Ts+8;
        //   if( !Read_BATLEARN() )
        //   {
        //     BAT_LEARN_ON();
        //   }
          //if( nAdpOvpGet )
          //{
            BATLEARNProcess(nAdpOvpEN);
            ACOFF_HI();
          //}
         
          SET_MASK(pEdgeProjectFlag1,pEdgeACCUTOFF);
        }
        else
        {
          AdpCUTCnt++;
        }
      }
      else
      {
        AdpCUTCnt = 0;
      }
      }
    }
  }
  else
  {
    AdpCUTCnt = 0;
	  }
	if(Read_AC_IN())
	{
		if(AD_ID_Detect_Delay > 1)
		{
			AD_ID_Detect_Delay--;
		}
	  }

#ifdef OEM_Hook_Timer10msEventB_support
    OEM_Hook_Timer10msEventB();
#endif // OEM_Hook_Timer10msEventB_support

}

//------------------------------------------------------------
// Hook 50ms events A
//------------------------------------------------------------
void Hook_Timer50msEventA(void)
{
//battery FW update delay  
 	if(BATFWupdate_Delay > 0)
 	{
 		BATFWupdate_Delay--;
		if(BATFWupdate_Delay == 0x00)
			{OSSMbusComd();}
 	}	
 
    TPG_LED();
  if( pACDCCnt != 0 )
    pACDCCnt--;

#ifdef OEM_Hook_Timer50msEventA_support
    OEM_Hook_Timer50msEventA();
#endif // OEM_Hook_Timer50msEventA_support
  
}

//------------------------------------------------------------
// Hook 50ms events B
//------------------------------------------------------------
void Hook_Timer50msEventB(void)
{
  BeepControl();
  Keyboard_beep();
#if Support_Tamper_function
  Tamper_DET();  // Check HDD/SSD/ODD detect process.
#endif //Support_Tamper_function

#ifdef OEM_Hook_Timer50msEventB_support
    OEM_Hook_Timer50msEventB();
#endif // OEM_Hook_Timer50msEventB_support

}

//------------------------------------------------------------
// Hook 50ms events C
//------------------------------------------------------------
void Hook_Timer50msEventC(void)
{
	MuteProcess();
	#if !EN_PwrSeqTest
  	Chk_FAN_RPM_Control();  // Check FAN is AP control.
	#endif

	#ifdef OEM_Hook_Timer50msEventC_support
    OEM_Hook_Timer50msEventC();
	#endif // OEM_Hook_Timer50msEventC_support

	#if 0
  	if(!Read_DCIN_ATTACHED2())
	{
    	if(IS_MASK_CLEAR(ACTypeCAlarm,BIT1))
		{
      		SET_MASK(ACTypeCAlarm,BIT1);
      		CLEAR_MASK(ACTypeCAlarm,BIT0);
  //    		H_PROCHOT_HI();
      		VGA_AC_BATT_OFF();
      		cBATTThrottling = CPUMAXPSTATE;
      		cGPUThrottling = GPUMAXPSTATE;
      		SET_MASK(M_BATAlarm,ADPOUT);
      		SET_MASK(GPUAlarm,GPUAlarm_ADPOUT);
      		ADPI2Sec = 100;
     		GPU2Sec = 100;
      		inhibit2sec = 20;
      		// Send QEvent to Bios
      		ThrottlingControl();
    	}
  	}
	else
	{
    	if(IS_MASK_CLEAR(ACTypeCAlarm,BIT0))
		{
      		CLEAR_MASK(ACTypeCAlarm,BIT1);
      		SET_MASK(ACTypeCAlarm,BIT0);
      		cBATTThrottling = 0;
      		CLEAR_MASK(M_BATAlarm,ADPOUT);
      		CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
      		ADPI2Sec = 0;
      		inhibit2sec = 0;
    	}
  	}
	#endif

}

//------------------------------------------------------------
// Hook 100ms events A
//------------------------------------------------------------
void Hook_Timer100msEventA(void)
{
  if (BAT_Unship_CNT !=0){
    BAT_Unship_CNT--;
  }

	#if !EN_PwrSeqTest
	
	#if WDT_Support
  	ResetInternalWDT();
	#endif

  	if (BattExistGet && !PriBattInhibGET )
  	{
    	if( IS_MASK_SET(uProject0,uFUdayClr) )
    	{
      		uSMBusData0 = 0;
      		uSMBusData1 = 0;
      		if(bRWSMBus(chSMB_BAT, SMbusWW, SmBat_Addr, FirstUsedDate, &uSMBusData0,SMBus_NeedPEC))
      		{
      		    //A285D00076+
      		    if(bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr, FirstUsedDate, &uSMBusData0,SMBus_NeedPEC))
                 {
                     if((0x00 == uSMBusData0) && (0x00 == uSMBusData1)) //A285D00076+
                     {
        		CLEAR_MASK(uProject0,uFUdayClr);
        		OptMfgFunction1H = 0;
        		OptMfgFunction1L = 0;
      		}
    	}
      		}
    	}
    OEM_PollingBatData_TASK(M_BAT);
    Trickle_Charge_Mode(M_BAT);

    
  	}

    UpdateECNameSpace(M_BAT);

	#if SW_ISCT
  	ISCT_SECtoChk();
	#endif  // SW_ISCT
	
	#endif  // EN_PwrSeqTest
	
  	AOU_Main();
	#ifdef OEM_Hook_Timer100msEventA_support
    OEM_Hook_Timer100msEventA();
	#endif // OEM_Hook_Timer100msEventA_support
	//if( (BAT_Status&0x07) == BAT_SEL_M )
	//{
	//	BAT_state_target(BAT_SEL_S)
	//}
	//else if((BAT_Status&0x07) == BAT_SEL_S )
	//{
	//	BAT_state_target(BAT_SEL_M)
	//}
}

//------------------------------------------------------------
// Hook 100ms events B
//------------------------------------------------------------
void Hook_Timer100msEventB(void)
{
	#if !EN_PwrSeqTest
	#if Support_WebCam
  	CameraProcess();
	#endif  // Support_WebCam
  	WirelessProcess();    // Control WLAN and BT.
	#if Support_IFFS
  	IFFSProcess();
	#endif // Support_IFFS
	#endif // !EN_PwrSeqTest

#if Support_TWO_BATT
  if (SecBattExistGet && !SecBattInhibGET )
  {
    if( IS_MASK_SET(uBattFUDStatus,uFUdayClr2) )
    {
      uSMBusData0 = 0;
      uSMBusData1 = 0;
      if(bRWSMBus(chSMB_2ndBAT, SMbusWW, SecSmBat_Addr, FirstUsedDate, &uSMBusData0,SMBus_NeedPEC))
      {
        if(bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr, FirstUsedDate, &uSMBusData0,SMBus_NeedPEC))   //A285D00076+
        {
            if((0x00 == uSMBusData0) && (0x00 == uSMBusData1))  //A285D00076+
            {
        CLEAR_MASK(uBattFUDStatus,uFUdayClr2);
        SecOptMfgFunction1H = 0;
        SecOptMfgFunction1L = 0;
      }
    }
      }
    }
    OEM_PollingBatData_TASK(S_BAT);
    Trickle_Charge_Mode(S_BAT);    
  }

  UpdateECNameSpace(S_BAT);

#endif
	
	TPS65988_SelectAdapterPort();
	#ifdef OEM_Hook_Timer100msEventB_support
    OEM_Hook_Timer100msEventB();
	#endif // OEM_Hook_Timer100msEventB_support
}

//------------------------------------------------------------
// Hook 100ms events C
//------------------------------------------------------------
void Hook_Timer100msEventC(void)
{
  //@LCFC_CUSTOMIZE+
  #if DASH_workAroundNofity
    //if(IS_MASK_SET(SLSTA_A,HONOST)&&IS_MASK_SET(SLSTA_A,RCS))
    //{
	
      if(IS_MASK_CLEAR(SMBPCTL_A,SMBDCS))
      {
        OEM_DASH_DEBUG_B++;
        if(OEM_DASH_DEBUG_B >= 0x20)        // 2sec count //A485D00068
        {
          //DASH_DATA_PIN = INPUT;
          //DASH_DATA_PIN = ALT;
          GPCRF3 = INPUT; //A485D00068
		  GPCRF3 = ALT;  //A485D00068
          SET_MASK(RSTC3,RSMBA); //Reset SMBus A //A485D00067: Workaround for TP lose function
          Init_Dash();//A485D00067: Workaround for TP lose function
          OEM_DASH_DEBUG_B = 0x00;
          OEM_DASH_DEBUG_C++; //A485D00068
        }
      }
      else
        OEM_DASH_DEBUG_B = 0x00;
    //}
    //else
    //  OEM_DASH_DEBUG_B = 0x00;
  #endif
  //@LCFC_CUSTOMIZE-   
	PollingBIOS80Port();	// debug card
	#if !EN_PwrSeqTest
  	Oem_Fan_Speed();
  	ThrottlingControl();
    Battery100ms(M_BAT);
    Battery100ms(S_BAT);  
	//A485D00033:Delete unused code.--->Start
  	//if (TouchPadCount !=0)
  	//{ TouchPadCount--; }
  	//A485D00033:Delete unused code.--->Start
	#endif
  	if(AOURSTCNT)
  	{
    	AOURSTCNT--;
  	}
//    ChargeIC_Polling_Task();

  	if (CountSecAfterPswPressed < 255)
  	{
  		if(Read_EC_PWRBTN())
  		{
    		CountSecAfterPswPressed++;
    	}
    }
  //A485D00033:Delete unused code.--->Start
  /*
		if(ACIN_FallINT_Count != 0)
		{
	    if((Wrong_ADP_check_Timeout_Clear&0x0F)>= 4)
	    {
		    if(((Wrong_ADP_check_Timeout_Clear&0xF0)>>4) == ACIN_FallINT_Count)
		    {
			    AC_DROP_COUNT = 0;
			    ACIN_FallINT_Count = 0;
			    Wrong_ADP_check_Timeout_Clear = 0;
		    }
		    else
		    {
			    Wrong_ADP_check_Timeout_Clear = 0;
			    Wrong_ADP_check_Timeout_Clear |= (ACIN_FallINT_Count<<4);
		    }
		  }
		  else
		  {
		  	Wrong_ADP_check_Timeout_Clear++;
		  }
		}
		*/
//A485D00033:Delete unused code.--->End

#if EnhancedPcBeepSupport
  OEM_ENHANCED_PC_BEEP_HOOK();
#endif
	#ifdef OEM_Hook_Timer100msEventC_support
    OEM_Hook_Timer100msEventC();
	#endif // OEM_Hook_Timer100msEventC_support
}

//------------------------------------------------------------
// Hook 500ms events A
//------------------------------------------------------------
void Hook_Timer500msEventA(void)
{
	#if !EN_PwrSeqTest
	#if Support_SMTADP  //A485D00011
  	Edge_SMTADPProcess();
	#endif
    if (BattExistGet)
    {
      Chk_BatSMbusFailCount(M_BAT);
      if (Read_AC_IN() && IS_MASK_CLEAR(BATUPDATEFW,PriBattInhib))
      {
      	WriteSmartChgIC(); //A485D00028:Modify Access charger flow.
        ReadSmartChgIC();   //A485D00028:Modify Access charger flow.
      }
	  else
	  {
	  	#if Support_TWO_BATT
  		if (IS_BAT_switch_ready)
    		{Select_Battery();}
		#endif
	  }
    }
	//A485D00028:Modify Access charger flow.--->Start
    if (Read_AC_IN() && IS_MASK_CLEAR(BATUPDATEFW,PriBattInhib))
    {
		Update_ChargeIC();
    }
    //A485D00028:Modify Access charger flow.--->end
  ThmIC_Temperature();
  if(IS_MASK_SET(BATUPDATEFW, PriBattInhib))
    SET_MASK(StatusKeeper, BatteryFwUpdate);
  if(IS_MASK_SET(StatusKeeper, BatteryFwUpdate) && IS_MASK_CLEAR(BATUPDATEFW, PriBattInhib))
  {
    CLEAR_MASK(StatusKeeper, BatteryFwUpdate);
    FirstGetBatData(M_BAT);
#if Lenovo_Authentication
    M_BAT_uAuthenFlag = 0;
#endif
    Service_Auth_Step = 1;        // start SHA1
  }

	#endif  // EN_PwrSeqTest
//#if Thinkpad_Battery_HW_Auth
  	if(SecDesignCapL !=0 && SecVoltageH != 0)
  	{
    	if (SecBattExistGet)
    	{ LV_BAT_Authentication(); }
  	}
//#endif  // Thinkpad_Battery_HW_Auth

	#ifdef OEM_Hook_Timer500msEventA_support
    OEM_Hook_Timer500msEventA();
	#endif // OEM_Hook_Timer500msEventA_support
	

#if Support_EC_Sleep
			if( CheckCanEnterDeepSleep() )
			{
                #if 0 // for test
					if(IS_MASK_SET(SYS_STATUS,AC_ADP) )
					{
							InitEnterDeepSleepForAC();
							EnableAllInterrupt();	// enable global interrupt			
							Sleep_Now();
							InitWakeFromDeepSleepForAC();
					}
					else
					{
							InitEnterDeepSleep();
							EnableAllInterrupt();	// enable global interrupt
							Sleep_Now();
							InitWakeFromDeepSleep();
					}
                #else
					if(IS_MASK_SET(SYS_STATUS,AC_ADP) )
					{
							InitEnterDeepSleepForAC();
					}
					else
					{
							InitEnterDeepSleep();
					}
				   EnableAllInterrupt();   // enable global interrupt
				   RamDebug(0x78);
				   Sleep_Now();
					RamDebug(0x87);
					if(IS_MASK_SET(SYS_STATUS,AC_ADP))
					{
							InitWakeFromDeepSleepForAC();
					}
					else
					{
						   InitWakeFromDeepSleep();
					}
                #endif
				   WakeUp_DO_Function();
			}
#endif
}

//------------------------------------------------------------
// Hook 500ms events B
//------------------------------------------------------------
void Hook_Timer500msEventB(void)
{
	#if !EN_PwrSeqTest

	#if SW_ISCT
  	ISCT_Process();
	#endif  // SW_ISCT
    if (IS_MASK_SET(SecBattSTS,BattExist))
    {
      Chk_BatSMbusFailCount(S_BAT);
      if (Read_AC_IN() && IS_MASK_CLEAR(BATUPDATEFW,SecBattInhib))
      {
        WriteSmartChgIC(); //A485D00028:Modify Access charger flow.
        ReadSmartChgIC();   //A485D00028:Modify Access charger flow.
      }
	  else
	  {
	  	#if Support_TWO_BATT
  		if (IS_BAT_switch_ready)
    		{Select_Battery();}
		#endif
	  }
    }
	//A485D00028:Modify Access charger flow.
    if (Read_AC_IN() && IS_MASK_CLEAR(BATUPDATEFW,SecBattInhib))
    {
      Update_ChargeIC();
    }
   //A485D00028:Modify Access charger flow.-->end
  if(IS_MASK_SET(BATUPDATEFW, SecBattInhib))
    SET_MASK(StatusKeeper, SecBatteryFwUpdate);
  if(IS_MASK_SET(StatusKeeper, SecBatteryFwUpdate) && IS_MASK_CLEAR(BATUPDATEFW, SecBattInhib))
  {
    CLEAR_MASK(StatusKeeper, SecBatteryFwUpdate);
    FirstGetBatData(S_BAT);
#if Lenovo_Authentication
    S_BAT_uAuthenFlag = 0;
#endif
    Service_Auth_Step = 1;        // start SHA1
  }
  	if( (pWorkAround > 1) && (SystemIsS5) )
  	{ pWorkAround--; }

	#if Support_HUAshutdown
  	if( (SysPowState == SYSTEM_S5_S0) )
  	{
    	if( cPWRONFail <= PWRFAILCnt )
    	{
      		cPWRONFail++;
    	}
    	else
    	{
      		SET_MASK(SysStatus,ERR_ShuntDownFlag);
      		ProcessSID(POWERSEQFAIL_ID);
      		cPWRONFail = 0x00;
      		PWSeqStep = 1;
      		PowSeqDelay = 0x00;
      		SysPowState = SYSTEM_S0_S5;
      		ACIN_BLINK3 = 45; // LED blink 3times in 2.5sec
    	}
  	}
	#endif

	#endif

	#ifdef OEM_Hook_Timer500msEventB_support
    OEM_Hook_Timer500msEventB();
	#endif // OEM_Hook_Timer500msEventB_support
}

//------------------------------------------------------------
// Hook 500ms events C
//------------------------------------------------------------
void Hook_Timer500msEventC(void)
{
	#if !EN_PwrSeqTest
  	if(IS_MASK_CLEAR(SysStatus,LidKBIgnore))
  	{ Lid_Process(); }

//A485D00028:Modify Access charger flow.--->Start
//	if( Read_AC_IN() )
//	{
//		if( (BattExistGet && IS_MASK_CLEAR(BATUPDATEFW,PriBattInhib))
//			|| (SecBattExistGet && IS_MASK_CLEAR(BATUPDATEFW,SecBattInhib))
//			)
//		{
//			WriteSmartChgIC();
//        	ReadSmartChgIC();
//
//			Update_ChargeIC();
//		}
//	}
//A485D00028:Modify Access charger flow.---->end
  	// Check KBC Output Buffer Full
  	#if 0
  	if (IS_MASK_SET(KBHISR,OBF)&&(SystemIsS0))
  	{
    	KB_OBF_count++;
    	if ((KB_OBF_count & 0x0F) > 10)        // OBF=1 over 5sec?
    	{
      		KB_OBF_count = KB_OBF_count + 0x10;    // high nibble for count times of clear
      		KB_OBF_count &= 0xF0;
      		GENIRQ = 0x0C;
    	}
  	}
  	else
  	{ KB_OBF_count &= 0xF0; }
	#endif
	
  	IBMbatteryStore();
  	CHK_Capacity_trip_point();
#endif  // EN_PwrSeqTest

    ChargeIC_InitACDC();

//		if (IS_MASK_CLEAR(ComboKeyState, ECDebugMode))
//		{
//			Hook_ACPICommand();
//		}
//A485D00069: Workaround for BIOS boot up hang logo-->S
        if(WorkaroundforBIOSSMI!=0)
         {
           WorkaroundforBIOSSMIcunt++;
         if(WorkaroundforBIOSSMIcunt==30)
          {
           ECSMI_SCIEvent(0xff);
           WorkaroundforBIOSSMI=0;         
          }
         }
      else
        {
         WorkaroundforBIOSSMIcunt=0;
        }
//A485D00069: Workaround for BIOS boot up hang logo-->E
#ifdef OEM_Hook_Timer500msEventC_support
    OEM_Hook_Timer500msEventC();
#endif // OEM_Hook_Timer500msEventC_support
}

//------------------------------------------------------------
// Hook 1sec events A
//------------------------------------------------------------
void Hook_Timer1SecEventA(void)
{
#if !EN_PwrSeqTest
  Battery1Sec(M_BAT);
  IBMChargeCondition(M_BAT);   //(JSN)
 // Battery1Sec(S_BAT);////A285D00001-
 // IBMChargeCondition(S_BAT); //A285D00001-  //(JSN)
  
  	if (OneOfTwoBattExistGet)
  	{BatteryFirstUseDay();}

  	if ( SystemIsS0 )
  	{
    	if ( (VGA_TempDly != 0) && (IS_MASK_SET(SYS_MISC1, ACPI_OS)) )
    	{ VGA_TempDly--; }
  	}

  	if ( SystemIsS5 )
  	{
    	if (ECSleepCount !=0)
    	{ ECSleepCount--; }
  	}

	#if SW_ISCT
  	ISCT_MinutetoSec();
	#endif  // SW_ISCT

  	if( pInhitTimer )
  	{
    	if( pInhitTimer != 0xFFFF) {
      		pInhitTimer--;
    	}

    	if ( pInhitTimer == 0 ) {
      		PriBattinhibitDis;
      		SecBattinhibitDis;          
      		Inhibit_chargingCLR;  
      		CLEAR_MASK(M_BATStopChgStatL,ENCMDSTOPCHG);
    	}
  	}
  	if ((M_BAT_Batt0x3CFailRetry5sTimer != 0xFF) && (M_BAT_Batt0x3CFailRetry5sTimer > 0)) 
	{
    	M_BAT_Batt0x3CFailRetry5sTimer--;
    	if (M_BAT_Batt0x3CFailRetry5sTimer == 0) {
      		M_BAT_pFKTemp = 0; // Clear pFKTemp to make SW auth re-run again.
 //     		Service_Auth_Step = 1;
      		M_BAT_Batt0x3CFailRetry5sTimer = 0xFF; // Disable retry timer
    	}
  	}

  	if ((S_BAT_Batt0x3CFailRetry5sTimer != 0xFF) && (S_BAT_Batt0x3CFailRetry5sTimer > 0)) 
	{
    	S_BAT_Batt0x3CFailRetry5sTimer--;
    	if (S_BAT_Batt0x3CFailRetry5sTimer == 0) {
      		S_BAT_pFKTemp = 0; // Clear pFKTemp to make SW auth re-run again.
      		Service_Auth_Step = 1;
      		S_BAT_Batt0x3CFailRetry5sTimer = 0xFF; // Disable retry timer
    	}
  	}    
	#endif

	#ifdef OEM_Hook_Timer1SecEventA_support
    OEM_Hook_Timer1SecEventA();
	#endif // OEM_Hook_Timer1SecEventA_support
}

//------------------------------------------------------------
// Hook 1sec events B
//------------------------------------------------------------
void Hook_Timer1SecEventB(void)
{
	#if !EN_PwrSeqTest
  	SystemAutoOn();
  	ChkBattery_abnormal(M_BAT);
//unlock ship mode
  if (IS_MASK_SET(BAT_Unship_mode, M_unship_mode)){     //M & S battery exist 
    Unlock_ShipMode(M_BAT);		// select main battery
  }
//Fix P2P function abnormal A485D00084
 #if Support_TYPE_C
  if(SystemIsS0)
	UcsiFuelStateChanged(0);
#endif
//Fix P2P function abnormal A485D00084
	// for wake charge + //A485D00029:Modify Read BATMakerID&FWUpdate flow.
  	if(IS_MASK_CLEAR(BATUPDATEFW,PriBattInhib) && IS_MASK_SET(EVT_STATUS1,M_bat_in ))
  	{
		if( (BatterymakerIDL == 0x00) && (BatterymakerIDH == 0x00) )
		{
    		bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr,C_FW_Status,&FirmwareUpdateStatusL,SMBus_NoPEC);
    		bRSMBusBlock(chSMB_BAT,SMbusRBK,SmBat_Addr,C_BAT_Info,&BatterymakerIDL, bRead_I2C_NULL);
		}
  	}

	//2nd battery
  	if(IS_MASK_CLEAR(BATUPDATEFW,SecBattInhib) && IS_MASK_SET(EVT_STATUS1,S_bat_in))
  	{
  		if( (SecBatterymakerIDL == 0x00) && (SecBatterymakerIDH == 0x00) )
		{
    		bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr,C_FW_Status,&SecFirmwareUpdateStatusL,SMBus_NoPEC);
    		bRSMBusBlock(chSMB_2ndBAT,SMbusRBK,SecSmBat_Addr,C_BAT_Info,&SecBatterymakerIDL, bRead_I2C_NULL);
  		}
  	}
	// for wake charge - //A485D00029:Modify Read BATMakerID&FWUpdate flow.
#if PLATFORM_DGPU
  	OEM_VGA_Hook();
#endif 

	#endif

	#ifdef OEM_Hook_Timer1SecEventB_support
    OEM_Hook_Timer1SecEventB();
	#endif  // OEM_Hook_Timer1SecEventB_support
	//BIOS WDT for workaround
	if((BIOS_WDT_Cnt > 1)&&(BIOS_WDT_Cnt < 4))
	{
		if(SysPowState == SYSTEM_S5)
		{
			BIOS_WDT_Cnt--;
			if(BIOS_WDT_Cnt == 1)
			{
				BIOS_WDT_Cnt = 0;
	                    PWSeqStep = 1;
	                    PowSeqDelay = 0x00;
	                    SysPowState=SYSTEM_S5_S0;
	              }
	        }		
	}
	else if(BIOS_WDT_Cnt == 4)
	{
		BIOS_WDT_Cnt = 3;
        	SET_MASK(SysStatus,ERR_ShuntDownFlag);
        	ProcessSID(BIOS_WDT_ID);  // 0x28 shutdown ID.
        	RSMRST_shutdown();		
	}
	else if(BIOS_WDT_Cnt > 4)
	{
		BIOS_WDT_Cnt--;
	}
	//BIOS WDT for workaround end
}

//------------------------------------------------------------
// Hook 1sec events C
//------------------------------------------------------------
void Hook_Timer1SecEventC(void)
{
  ChkBattery_abnormal(S_BAT);
//unlock ship mode
  if (IS_MASK_SET(BAT_Unship_mode, S_unship_mode)){     //M & S battery exist 
	  Unlock_ShipMode(S_BAT);		// select main battery
  }
  
  DetectFanId();  // Detect FAN vendor first.
                  // Warning!! This step must be front of other fan related functions.

  Oem_Thermal_Control();  // Read CPU temperature by PECI.
#if !EN_PwrSeqTest
  Fan_control_update();
  if ( IS_MASK_CLEAR(uReserve07,nFanManual))
  {
#if FAN_TABLE_Already
      Oem_Fan_Control();
#else	//Need recovery PROCHOT to low after power protect feature "PROCHOT set high for adaptor plug-out".
		H_PROCHOT_LOW();
		nRealThermalPinDIS;
		CLEAR_MASK(pPROCHOT, b0Thermal_PRCOHOTon);	// for AP display.	
		#endif  // FAN_TABLE_Already
	}
	
	#if FAN_TABLE_Already
	OEM_Throttling_Ctrl();
	#endif  // FAN_TABLE_Already
	#endif  // !EN_PwrSeqTest
	if(thermalTBlMode == PFM_Mode) //A485D00091
		{
	      if(ThermalTBlMode_M != thermalTBlMode)
	 	  {
           ThermalTBlMode_M = thermalTBlMode;
		   #if Support_KBD_BL_LED
          // #if Support_KBD_BL_DETECT
	        // if(Read_KBD_BL_DTCT())
          // #endif //Support keyboard backlight detect
                if(SystemIsS0)
	        {
	        	KBL_HotKey_Blink_Achive = KBL_BLICK_TIMES_HOTKEY_PERFORMANCE; //A285D00043:fix backlight led blink breath as lenovo spec
                      KBL_Hotkey_Blink_Time_Interval = KBL_BLINK_STATUS_TIME_INTERVAL / 5;//(1000 / (KBL_BLICK_TIMES_HOTKEY_PERFORMANCE * 2)) / 5;
                      KBL_Hotkey_Blink_Duty = 100 / KBL_Hotkey_Blink_Time_Interval + ((100 % KBL_Hotkey_Blink_Time_Interval) > 0);
		      	SET_MASK(KBL_Status, HotKeyKbdBlBlick);
				SET_MASK(KBL_Status, HotkeyLED_NONEED_update);
				KBL_Blink_Level_State 		= KBL_BLINK_STATUS_UP;
				KBL_Blink_Level_Duty 		= 0;
				KBL_HotKey_Blink_Times	= 0;
	        }
           #endif //Support keyboard backlight
	     }  //Keyboard belight blinking 3 times
		}
	else
	    {
	    	 if(ThermalTBlMode_M != thermalTBlMode)
	 	      {
                ThermalTBlMode_M = thermalTBlMode;
		     #if Support_KBD_BL_LED
            //  #if Support_KBD_BL_DETECT
	        // if(Read_KBD_BL_DTCT())
            // #endif //Support keyboard backlight detect
                if(SystemIsS0)
	         {
	         	KBL_HotKey_Blink_Achive = KBL_BLICK_TIMES_HOTKEY_COOL;
                      KBL_Hotkey_Blink_Time_Interval = KBL_BLINK_STATUS_TIME_INTERVAL / 5;//(1000 / (KBL_BLICK_TIMES_HOTKEY_COOL * 2)) / 5;
                      KBL_Hotkey_Blink_Duty = 100 / KBL_Hotkey_Blink_Time_Interval + + ((100 % KBL_Hotkey_Blink_Time_Interval) > 0);
		      	SET_MASK(KBL_Status, HotKeyKbdBlBlick);
				SET_MASK(KBL_Status, HotkeyLED_NONEED_update);
				KBL_Blink_Level_State 		= KBL_BLINK_STATUS_UP;
				KBL_Blink_Level_Duty 		= 0;
				KBL_HotKey_Blink_Times	= 0;
	         }
           #endif //Support keyboard backlight
	      }
		  }
	#ifdef OEM_Hook_Timer1SecEventC_support
    OEM_Hook_Timer1SecEventC();
#endif  // OEM_Hook_Timer1SecEventC_support
}

//------------------------------------------------------------
// Hook1min events
//------------------------------------------------------------
void Hook_Timer1MinEvent(void)
{
#if !EN_PwrSeqTest
  Chk_CHG_TimeOut(M_BAT);
  Chk_CHG_TimeOut(S_BAT);
#if SW_ISCT
  	ISCT_TimerCnt();
	#endif  // SW_ISCT
	#endif  // !EN_PwrSeqTest

	#ifdef OEM_Hook_Timer1MinEvent_support
    OEM_Hook_Timer1MinEvent();
	#endif  // OEM_Hook_Timer1MinEvent_support
}

//------------------------------------------------------------
// service_OEM_1
//------------------------------------------------------------
void service_OEM_1(void)
{



}

//------------------------------------------------------------
// service_OEM_2
//------------------------------------------------------------
void service_OEM_2(void)
{

}

//------------------------------------------------------------
// service_OEM_3
//------------------------------------------------------------
void service_OEM_3(void)
{

}

//------------------------------------------------------------
// service_OEM_4
//------------------------------------------------------------
void service_OEM_4(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_16(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_17(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_18(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_19(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_20(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_21(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_22(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_23(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_24(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_25(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_26(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_27(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_28(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_29(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_30(void)
{

}

//------------------------------------------------------------
// service_reserved_16
//------------------------------------------------------------
void service_reserved_31(void)
{

}

//------------------------------------------------------------
//
//------------------------------------------------------------
//void Hook_main_service_H(void) 
//{
//
//}

//------------------------------------------------------------
//
//------------------------------------------------------------
//void Hook_main_service_L(void) 
//{
//
//}

//------------------------------------------------------------
// Timer event control function
// Return :
//  Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
//  All_TimerEvent : All timer events are OK.
//------------------------------------------------------------
BYTE Hook_Only_Timer1msEvent(void)
{
  //A485D00044: Fix Update BIOS&EC FW EC hang up.--->Start
	if( afterFlashMark == afterFlashMode )
		return Only_Timer1msEvent;
  //A485D00044: Fix Update BIOS&EC FW EC hang up.--->End
// Return :
//  Only_Timer1msEvent : Only Timer1msEvent and Hook_Timer1msEvent function
//  All_TimerEvent : All timer events are OK.
    return(All_TimerEvent);
}

//------------------------------------------------------------------
// OEM_SkipMainServiceFunc
//  Note:
//      1. Always return(0xFF|Normal_MainService) to run normal main_service function.
//      2. If you don't understand the use of OEM_SkipMainServiceFunc function, don't change anything.
//-------------------------------------------------------------------
BYTE OEM_SkipMainServiceFunc(void)
{
    
    // Return :
    return(Normal_MainService);
}

//********************************************************
//
//  Return [0xFF] to start handle sensor hub feature.
//
//  parameter :
//      none
//
//  return :
//      0xFF        : 
//      0 ~ 0xFE    :
//
//********************************************************
BYTE Hook_Start_EC_Handle_Task(void)		// sensor hub function carl test
{
#ifndef EC_FEATURE_ONLY_SUPPORT_FUSION				// sensor disable		
	return(0x00);
#else												// sensor enable
	if(SystemIsS0)
	{
		return(0xFF);
	}
	else
	{
		if(pure_ec_sensor_poweroff_done())
			return(0x00);
		else
			return(0xff);
	}
#endif
}





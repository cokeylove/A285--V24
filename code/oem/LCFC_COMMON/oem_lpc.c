//*****************************************************************************
//
//  oem_lpc.c
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



/*****************************************************************************/
// Procedure: MuteProcess                    TimeDiv: 50mSec
// Description:
// GPIO: GPIOJ1
// Referrals:
/*****************************************************************************/
void MuteProcess(void)
{
  if ( SystemIsS0 )
  {  // Check ISCT and command 94 status.
    if( MuteCount == 0 )
    {
      //if ( (IS_MASK_CLEAR(uISCT_2, b3ISCT_MUTE)) && (IS_MASK_CLEAR(cCmd, b5VolMute)) )
      if ((!MutevolumeEGet)
#if SW_ISCT
        && (IS_MASK_CLEAR(uISCT_2, b3ISCT_MUTE))
#endif // SW_ISCT
        )
      {
        EC_MUTE_INACT();
        return;
      }
    }
    else
    { MuteCount--; }
  }
  EC_MUTE_ACT();
}

/*****************************************************************************/
// Procedure: CameraProcess                  TimeDiv: 100mSec
// Description:
// GPIO: 
// Referrals:
/*****************************************************************************/
void CameraProcess(void)
{
  #if Support_WebCam
  if ( SystemIsS0 )
  {

  }
  #endif  // Support_WebCam
}

/*****************************************************************************/
// Procedure: WirelessProcess                  TimeDiv: 100mSec
// Description:
// GPIO: 
// Referrals: disable and enable Wireless.
/*****************************************************************************/
void WirelessProcess(void)
{

  if ( SystemIsS0 && (IS_MASK_SET(SYS_MISC1,ACPI_OS)) )
  {
#if Support_WLAN_ON_OFF_control  
  //WLAN
    if( WLANstsGet )
    {
      BTRF_OFF_ON();
    }
    else
    {
      BTRF_OFF_OFF();
    }
#endif    

#if Support_BT_ON_OFF_control
   //BT
    if( BTstsGet )
    {
      BT_OFF_ON();
    }
    else
    {
      BT_OFF_OFF();
    }   
#endif 
  }
 
}

#if Support_IFFS
void IFFSProcess(void)
{
  if (uIffsCnt == 0 ) return;
  else uIffsCnt--;

  if (uIffsCnt==0)
  {
    CLEAR_MASK(uCritBattWakeThre,IFFS_Enable);
    if (SystemIsS3)
    {
      if (RelativeStateOfChgL < uCritBattWakeThre)
      {
        SET_MASK(uCritBattWakeThre,IFFS_Enable);
              PWSeqStep = 1;
        PowSeqDelay = 1;
              SysPowState=SYSTEM_S3_S0;
      }
    }
  }
}
#endif // Support_IFFS

void SetACIN_Int(void)
{
  if( SysPowState == SYSTEM_S0 || SysPowState == SYSTEM_S5)
  {
    ISR13 = Int_WKO101;
    SET_MASK(IELMR13,Int_WKO101);
    SET_MASK(WUEMR10,WUC_WUI53);
    if (!Read_AC_IN() || SysPowState == SYSTEM_S5)
    {
      CLEAR_MASK(IPOLR13,Int_WKO101);    // rising
    }
    else
    {
      SET_MASK(IPOLR13,Int_WKO101);    // falling
    }
    ISR13 = Int_WKO101;

    SET_MASK(IER13, Int_WKO101);
  }
}
//A485D00090--->Start
void SetBAT1INOUT_Int(void)
{
  if( SysPowState == SYSTEM_S5)
  {
    ISR15 = Int_WKO120;
    SET_MASK(IELMR15,Int_WKO120);
   
    if (Read_MAIN_BAT_IN())
    {
    	CLEAR_MASK(WUEMR13,WUC_WUI72);
      	CLEAR_MASK(IPOLR15,Int_WKO120);    // rising
    }
    else
    {
    	SET_MASK(WUEMR13,WUC_WUI72);
      	SET_MASK(IPOLR15,Int_WKO120);    // falling
    }
    ISR15 = Int_WKO120;

    SET_MASK(IER15, Int_WKO120);
  }
}

void SetBAT2INOUT_Int(void)
{
  if( SysPowState == SYSTEM_S5)
  {
    ISR15 = Int_WKO121;
    SET_MASK(IELMR15,Int_WKO121);
   
    if (Read_SEC_BAT_IN())
    {
    	CLEAR_MASK(WUEMR13,WUC_WUI73);
      	CLEAR_MASK(IPOLR15,Int_WKO121);    // rising
    }
    else
    {
    	SET_MASK(WUEMR13,WUC_WUI73);
      	SET_MASK(IPOLR15,Int_WKO121);    // falling
    }
    ISR15 = Int_WKO121;

    SET_MASK(IER15, Int_WKO121);
  }
}

void SetLANWake_Int(void)
{
  if( SysPowState == SYSTEM_S5)
  {
    ISR3 = Int_WKO21;
    SET_MASK(IELMR3,Int_WKO21);
    SET_MASK(WUEMR2,WUC_WUI1);
	CLEAR_MASK(IPOLR3,Int_WKO21);    // falling
   
    ISR3 = Int_WKO21;

    SET_MASK(IER3, Int_WKO21);
  }
}

void SetPDC_Int(void)
{
	ISR16 = Int_WKO128;
    SET_MASK(IELMR16,Int_WKO128);
    SET_MASK(WUEMR14,WUC_WUI80);
	SET_MASK(IPOLR16,Int_WKO128);    // falling
   
    ISR16 = Int_WKO128;

    SET_MASK(IER16, Int_WKO128);
}

void SetSLPS3_Int(void)
{
	ISR0 = Int_WKO20;
    SET_MASK(IELMR0,Int_WKO20);
    CLEAR_MASK(WUEMR2,WUC_WUI0);
	CLEAR_MASK(IPOLR0,Int_WKO20);    // falling
   
    ISR0 = Int_WKO20;

    SET_MASK(IER0, Int_WKO20);
}

//A485D00090--->END
void SetPWRSW_Int(void)
{
//  GPCRB3  = ALT;  //  ON/OFF
  SET_MASK(IELMR1,Int_WKO25);
  SET_MASK(WUEMR2,WUC_PWRSW);
//  SET_MASK(WUENR2,WUC_PWRSW);
  CLEAR_MASK(IPOLR1,Int_WKO25);        // falling
  SET_MASK(WUESR2, WUC_PWRSW);
  SET_MASK(ISR1, Int_WKO25); 
  SET_MASK(IER1, Int_WKO25);        // Enable PWRSW Interrupt  
}


void SetUSBExTimer(void)
{
	
	Enable_External_Timer_x(USBTimer,tUSBCount);          //Period 1 sec  
	
	Enable_External_Timer_Int_x(USBTimer);
 
}
//A485D00090--->Start
void SetUSBExTimerAC(void)
{
	
	Enable_External_Timer_x(USBTimer,tACUSBCount);          //Period 1 sec  
	
	Enable_External_Timer_Int_x(USBTimer);
 
}
//A485D00090--->END

void SetVGA_AC_DET(void)
{
#if ( (PLATFORM_CHIP == INTEL) && (SUPPORT_DSx) )
  if( SystemIsS0 || SystemIsS3 || (Read_DPWROK()&&SystemIsS5))  //DS3
#else // PLATFORM_CHIP
  if( SystemIsS0 || SystemIsS3 || SystemIsS5)  //DS3
#endif // PLATFORM_CHIP
  {
    //if( pACDCCnt == 0 )
    {
//       if (IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOTTHROT)//(!Read_AC_IN() || IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOTTHROT)
//         || IS_MASK_SET(pEdgeProjectFlag2,pEdgeGPUOCTHROT) || Read_ACOFF() )
      if ((GPU_THROTTLE_SRC != 0) || Read_ACOFF())
      {
        AC_PRESENT_LOW();
        VGA_AC_BATT_OFF();
        //VGA_AC_DET_LO();
        pACDCCnt = 20;
      }
      else
      {
        if(Read_AC_IN()) {
          AC_PRESENT_HI();
        }else{
          AC_PRESENT_LOW();
        }
        VGA_AC_BATT_ON();
        //if(SystemIsS0)
        //{ VGA_AC_DET_HI(); }
        //else
        //{ VGA_AC_DET_LO(); }
        pACDCCnt = 20;
      }
    }
  }
}


void WakeUp_DO_Function(void)
{
  if(ExtTimeCnt == 1)
  { ExtTimeCnt=0; }
  else
  { ExtTimeCnt++; }

  ChkBattery_Percl();		

  Init_VC(3);    // CMW Temp
}

/*****************************************************************************/
// Procedure: ISCT_Process                  TimeDiv: 500mSec
// Description: Intel Smart Connect Technology process
// GPIO:
// Referrals:
/*****************************************************************************/
#if SW_ISCT
void ISCT_Process(void)
{
  if( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemNotS5) )    // Check ISCT enable?
  {
    if(BattExistGet)       // Check BAT1 exist.
    {
      if( RelativeStateOfChgL <= ISCT_BAT_LOW )      // Check BAT under 20%.
      { SET_MASK(AOAC_STATUS, ISCT_BAT_Capy); }  // Battery under 20%
      else
      { CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Capy); }  // Battery over 20%

      // Check BAT over 50 degree amd under -5 degree.
      //if( ( ( nBattAverTemp >= ISCT_BAT_OvTpLimit ) && !( nBattAverTemp & BATUnZeroTemp ) )
      //  || ( (nBattAverTemp & BATUnZeroTemp) && ( nBattAverTemp <= ISCT_BAT_UnTpLimit ) ) )
      // Check BAT over 50 degree.
      if( nBattAverTemp >= ISCT_BAT_OvTpLimit )
      { SET_MASK(AOAC_STATUS, ISCT_BAT_Temp); }  // Battery Over 50 and under -5 degree.
      else
      { CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Temp); }  // Battery under 50 and over -5 degree.
    }
    else
    {
      CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Capy);    // Clear battery capacity of fail bit.
      CLEAR_MASK(AOAC_STATUS, ISCT_BAT_Temp);    // Clear battery temperature of fail bit.
    }

    if( SystemIsS3 )
    {
      if ( (AOAC_STATUS & 0xC0) != 0 )
      {
        SET_MASK(AOAC_STATUS, ISCT_S0_ISCT);  // Set S0_ISCT
        PM_PWRBTN_LOW();
      }
      ISCT_Behavior();
    }
  }
  else
  {
    AOAC_STATUS &= 0xC0;    // Clear ISCT status.
    ISCT_Behavior();
  }

  //#endif  // SW_ISCT
}

/*****************************************************************************/
// Procedure: ISCT_Behavior
// Description: Intel Smart Connect Technology mode behavior.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_Behavior(void)
{
  if( (IS_MASK_SET(AOAC_STATUS, ISCT_S0_ISCT)) && (SystemNotS5) )      // Can do AOAC.
  {
//      AOAC_PWRON();            // ISCT Turn on WLAN.
    uISCT |= 0x10;            // bit4:Disable FAN, bit5:Disable BKOFF.
    uISCT_2 |= 0x20;          // bit3:Disable Mute, bit4:Disable Camera, bit5:Disable Power LED.
    SET_MASK(pPROCHOT, b1ISCT_PROCHOTon);  // Set CPU Prochot function.
  }
  else
  {
//      AOAC_PWRON();            // ISCT Turn on WLAN.
    uISCT &= 0xCF;            // bit4:Enable FAN, bit5:Disable BKOFF.
    uISCT_2 |= 0xC7;          // bit3:Disable Mute, bit4:Enable Camera, bit5:Enable Power LED.
    CLEAR_MASK(pPROCHOT, b1ISCT_PROCHOTon);  // Clear CPU Prochot function.
  }
}

/*****************************************************************************/
// Procedure: ISCT_Timer                    TimeDiv: 1 Min
// Description: Intel Smart Connect Technology timer.
// GPIO:
// Referrals:
/*****************************************************************************/
void ISCT_TimerCnt(void)
{
  if ( (IS_MASK_SET(AOAC_STATUS, ISCT_Enable)) && (SystemIsS3) )  // Check ISCT enable?
  {
    if ( ISCT_Timer == 0 ) { return; }

    if ( ISCT_Timer == 1 )
    {
      ISCT_OPEN_APS();
      SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
    }
    ISCT_Timer--;
  }
}

//=============================================================================
void ISCT_MinutetoSec(void)  // TimeBase: 1Min.
{
  if ( (IS_MASK_SET(APSHDDFLAG , APSHDDPRT_MIN)) && (SystemIsS3)) // when ISCT enable, check count time on last 1 min.
  {
    if (APSHDDCount == ISCT_Again)  // Check ISCT Second.
    {
      SET_MASK( APSHDDFLAG , APSHDDPRT_SEC);
      CLEAR_MASK( APSHDDFLAG , APSHDDPRT_MIN);
      APSHDDCount = 0;
    }
    else
    { APSHDDCount++; }
  }
}
//=============================================================================
//=============================================================================
void ISCT_SECtoChk(void)  // TimeBase: 1Sec.
{
  if ( (IS_MASK_SET(APSHDDFLAG , APSHDDPRT_SEC)) && (SystemIsS3)) //APS power on
  {
    if (APSHDDCount == 1)  //Ready APS sample mode
    {
      if(IS_MASK_CLEAR(APSHDDFLAG , b6PMEWAKE)){
        SET_MASK( APSHDDFLAG , APSHDDPRT_CHK);
        CLEAR_MASK( APSHDDFLAG , b6PMEWAKE);
        SET_MASK( APSHDDFLAG , b7RTCWAKE);
      }
      CLEAR_MASK( APSHDDFLAG , APSHDDPRT_SEC);
      APSHDDCount = 0;
    }
    else
    { APSHDDCount++; }
  }
}
#endif  // SW_ISCT

//=============================================================================

void ISCT_OPEN_APS(void)
{
#if SW_ISCT && Support_LenovoAPS
#if Support_EC_GS_ON    
          EC_GS_ON();
#endif //Support_EC_GS_ON    
          ECQEvent(GSENSOR_PWRON_7D);
          SET_MASK(GS.Status,GS_Enable);
          CLEAR_MASK(GS.Status,GS_Request);

          // Initial after get enable request
          GS.Buf1X_S = 0;
          GS.Buf1X_E = 0;
          GS.Buf1X_N = 0;
          GS.Buf1Y_S = 0;
          GS.Buf1Y_E = 0;
          GS.Buf1Y_N = 0;

          GS.Buf2X_S = 0;
          GS.Buf2X_E = 0;
          GS.Buf2X_N = 0;
          GS.Buf2Y_S = 0;
          GS.Buf2Y_E = 0;
          GS.Buf2Y_N = 0;

          uAPSLastX=0;
          uAPSLastY=0;

          uCountX=0;
          uTotalX=0;
          uCountY=0;
          uTotalY=0;

          uAPSThresholdX=0x04;
          uAPSThresholdY=0x04;
          uAPSTimeOut=40;
          SET_MASK( APSHDDFLAG , APSHDDPRT_MIN);
#endif  // SW_ISCT          
}



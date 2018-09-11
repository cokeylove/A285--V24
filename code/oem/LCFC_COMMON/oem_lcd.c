//*****************************************************************************
//
//  oem_lcd.c
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



//=============================================================================
//   Calculate PWM duty.
//=============================================================================
BYTE Cal_PWM_Duty(BYTE BCalDuty, BYTE BPWMBase)
{
  return ( BCalDuty = (WORD)((BCalDuty * BPWMBase) / 100));
}

/*****************************************************************************/
// Procedure: Backlight_Control                  TimeDiv: 10mSec
// Description:
// GPIO: GPIOG2, GPIOF7
// Referrals:
/*****************************************************************************/
void Backlight_Control(void)
{
  if ( SystemIsS0
#if SW_ISCT
    && (IS_MASK_CLEAR(uISCT, b5ISCT_BKOFF))
#endif // SW_ISCT
    )  // Check S0 and ISCT BKOFF status.
  {
    if ( (Read_ENBKL_IN()) && ( Read_LID_SW_IN() || (!PalBKLidSyncGET) ) )
    //if ( (IS_MASK_CLEAR(cCmd, b3BkOff)) && Read_LID_SW_IN())
    {
#if EC_Brightness_Ctrl
      if (DCR7 == 0)
      {  // force Back light enable after PWM stable.
        if ( (SYS_STATUS & 0x07) == 0x03 )  // Win7
        { BackLight_En_Delay = Bri_Table[cPanelId][BRIGHT_MAX_STEP+3]; }
        else  // Win8
        { BackLight_En_Delay = WIN8_Bri_Tab[cPanelId][WIN8_BRI_MAX_STEP+3]; }
      }
#endif  // EC_Brightness_Ctrl
#if SW_ISCT
      CLEAR_MASK(AOAC_STATUS, ISCT_S0_ISCT);
#endif // SW_ISCT
      APSHDDFLAG = 0;

      // Enable Backlight after PWM stable
      if (BackLight_En_Delay != 0)
      {
        BackLight_En_Delay--;
      }
      else
      {
        BKOFF_ON();
        CLEAR_MASK(pProject0,b0DispToggleEn);
        return;
      }
    }
else
   {BackLight_En_Delay=30;}//David+ fix screen display white line //180604
  }
//else
   //{BackLight_En_Delay=30;}//David+ fix screen display white line //180604
   
  BKOFF_OFF();
}

/*****************************************************************************/
// Procedure: Lid_Process                  TimeDiv: 500mSec
// Description:
// GPIO: GPIOH0, GPIOH3
// Referrals:
/*****************************************************************************/
void Lid_Process(void)
{

  if ((SystemIsS5)||(DSxPowState == SYSTEM_DSxOK) )
  {
//    EC_WAKE_OUT_LOW();
    CLEAR_MASK(SysStatus,LidStatus);
    LID_DEBOUNCE_CNT =0;
    if (Read_LID_SW_IN()) {
      LidStsEn;
    }
  }
  else
  {
#if Support_IFFS
    if ( (IS_MASK_SET(ACPI_HOTKEY,b5BIOS_IFFS)) )
    { return; }
#endif // Support_IFFS
    if( Read_LID_SW_IN() )
    {  //LCD Cover Open (LID OPEN)
      LidStsEn;   //(JSN-2012_1005)

      if(IS_MASK_CLEAR(SysStatus,LidStatus))
      {
        LID_DEBOUNCE_CNT++;
        if( LID_DEBOUNCE_CNT > _LID_DEBOUNCE )  // Debounce 2 timer.
        {
#if Support_LenovoAPS
          GS_D10 |= 0x20;
#endif  // Support_LenovoAPS
          if( SystemIsS0 )
          {
            if( PalBKLidSyncGET )
            {
              ECQEvent(EV_LID_Open);
            }
          }
          // -
//          LidWakeEn;

          if (((DSxPowState == SYSTEM_DSxOK)||((DSxPowState == SYSTEM_S3S4)&&(SystemIsS3)))&&LidOpenGET)
          {
            LidWakeEn;
            EC_WAKE_OUT_LOW();
            CLEAR_MASK(SysStatus,LidStatus);
            PWRBTN_pressCnt = 6;    // delay 60ms.
          }
          else
          {
            SET_MASK(SysStatus,LidStatus);

            LID_DEBOUNCE_CNT = 0;
          }
          CLEAR_MASK(SysStatus,CloseLid);
		  if( IS_MASK_SET(SysStatus2,LidCloseKBMSDis) )
    	  {
     		CLEAR_MASK(SysStatus2,LidCloseKBMSDis);
   			Clear_Key();
   			Enable_Any_Key_Irq();
    	  }
        }
      }
      else
      { LID_DEBOUNCE_CNT =0; }
    }
    else
    {  //LCD Cover Close (LID Close)

      LidStsDis;  //(JSN-2012_1005)
     
      if(IS_MASK_SET(SysStatus,LidStatus))
      {
        LID_DEBOUNCE_CNT++;
        if( LID_DEBOUNCE_CNT > _LID_DEBOUNCE )
        {		

	      SET_MASK(SysStatus2,LidCloseKBMSDis);
          CLEAR_MASK(SysStatus,LidStatus);
#if Support_LenovoAPS
          GS_D10 |= 0x20;
#endif  // Support_LenovoAPS
          LID_DEBOUNCE_CNT = 0;
          if(SystemNotS3)
          {
            if(SystemIsS0)
            {
              SET_MASK(SysStatus,CloseLid);
              if( PalBKLidSyncGET )
              {
                ECQEvent(EV_LID_Close);
              }
            }  // For Deep S3 wake up.
          }
        }
      }
      else
      { LID_DEBOUNCE_CNT = 0; }
    }
  }
}

#if NV_GC6
/*****************************************************************************/
// Procedure: GC6_FBClamp                  TimeDiv: 1 mSec
// Description:
// GPIO:
// Referrals:
/*****************************************************************************/
void GC6Process(void)
{
  if( (IS_MASK_SET(SYS_MISC1, ACPI_OS)) && (SystemIsS0) )
  {
    if( IS_MASK_SET(ACPI_STAT, GC6ENTRY)
      && Read_GC6Event_ON()
      && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeGC6EventHI)
      )
    {

      SET_MASK(pEdgeProjectFlag1,pEdgeGC6EventHI);
      if( Read_FB_CLAMP() )
      {
        nGC6ClampOff;
        FB_CLAMP_OFF();
      }
      else
      {
        nGC6ClampOn;
        FB_CLAMP_ON();
      }

    }
  }
}
#endif // NV_GC6




void OemPwmSetting(BYTE PWM_Channel, BYTE Prescaler_select, WORD Freq, BYTE Duty)
{
	PWM_Channelx_Enable(PWM_Channel);
	PWM_Channelx_Register_Clock_Prescaler(PWM_Channel, Prescaler_select, Freq);
	PWM_Channelx_Output_Duty(PWM_Channel, Duty);
}




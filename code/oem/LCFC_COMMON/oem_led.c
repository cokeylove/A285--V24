/*-----------------------------------------------------------------------------
 * TITLE: OEM_LED.C
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

//********************************************************************
// Include all header file
#include "..\..\include.h"
//
//********************************************************************


/*
********************************************************************************
* Function name: LED_INIT
*
* Description:
*
*       This function is for LED init.
*
* Arguments: none
*
* Return Values: none
*
********************************************************************************
*/

void LED_Initial(void) {
  PWMPOL = PWM_Polarity;
}


/*
*******************************************************************************
* Function name: OEM_Write_Leds
*
* Descriptoin: Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs (including
*              an "Fn" key LED).
*              Updates Led_Data variable and calls send_leds if appropriate.
*
* Arguments: data (LED_ON bit) = Data to write to the LEDs
*         bit 0 - SCROLL, Scroll Lock
*         bit 1 - NUM,    Num Lock
*         bit 2 - CAPS,   Caps Lock
*         bit 3 - OVL,    Fn LED
*         bit 4 - LED4,   Undefined
*         bit 5 - LED5,   Undefined
*         bit 6 - LED6,   Undefined
*         bit 7 - LED7,   Undefined
*
* Return Values: none
*       
*******************************************************************************
*/
void OEM_Write_Leds(BYTE data_byte)
{
  Led_Data = data_byte;
  if ( IS15Inch )  // Check 15"
  {
      if ( (data_byte & BIT1) &&(!NumLockGet))
      {
         Hook_NUMLED_ON();
         NumLockEn;
         ECSMI_SCIEvent(KB_NUMLOCK_3F);		
      }
     else if ( (!(data_byte & BIT1)) &&(NumLockGet))
     {
      Hook_NUMLED_OFF();
      NumLockDis;
      ECSMI_SCIEvent(KB_NUMLOCK_3F);			
     }
  }

    if ( data_byte & BIT2 )
    {
    Hook_CAPLED_ON();
    }
  else
  {
    Hook_CAPLED_OFF();
  }
}


/*
********************************************************************************
* Function name: LedPowerOn
*
* Description:
*
*       This is an event entry of POWERON.
*
* Arguments: none
*
* Return Values: none
*
********************************************************************************
*/

void LedPowerOn(void) {
    LOGO_LED = LED_ON;
#if Support_PWRBTN_LED
    PWRBTN_LED = LED_ON;
    u4LEDcontrol = LED_TURN_ON | POWER_LED;
    TPG_LED_control();
#endif    
    u4LEDcontrol = LED_TURN_ON | THINKPAD_LOGO_LED;
    TPG_LED_control();    
}

/*
********************************************************************************
* Function name: LedPowerOff
*
* Description:
*
*       This is an event entry of POWEROFF.
*
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/

void LedPowerOff(void) {
    LOGO_LED = LED_OFF;
#if Support_PWRBTN_LED
    PWRBTN_LED = LED_OFF;
    u4LEDcontrol = POWER_LED;
	
    TPG_LED_control();
#endif    
    u4LEDcontrol = THINKPAD_LOGO_LED;
    TPG_LED_control();   
}

/*
*******************************************************************************
* Function name: TPG_LED
*
* Descriptoin: This is used to turn on, off or blink LED
*               S0 => turn on 
*               S3 => breathing
*               during S4 => blinking
*               S5 => trun off
*               AC in => Blink 3 times              
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void TPG_LED(void)
{
    BYTE LED_PWM;

#if (Support_BATTCHGLED)
    AC_Charging_LED();
#endif //Support_BATTCHGLED
    
#if Support_MuteLED  
    MuteLED();
#endif //Support_MuteLED

#if Support_FnLKLED  
    FnLKLED();
#endif  //Support_FnLKLED

#if Support_Microphone_Mute_LED
   if (SystemNotS0 || IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )   //shutdown and restart?
   {
      MICMute_LED_OFF();
   }
#endif //Support_Microphone_Mute_LED

  if (SystemIsS5)
  {
    LedPowerOff();
  }
  TPG_LED_control();


  if(LOGO_LED_PWM_REG != ALT){
    LOGO_LED_PWM_REG = ALT;
  }

#if Support_PWRBTN_LED  
  if(PWRBTN_LED_PWM_REG != ALT){
    PWRBTN_LED_PWM_REG = ALT;
  }
#endif //Support_PWRBTN_LED
 

//*********************************************
//AC IN => LED Blink 3 times
//*********************************************
  if (ACIN_BLINK3 !=0)
  {
    LED_PWM = ACin_LED();
    LOGO_LED = LED_PWM;
#if Support_PWRBTN_LED     
    PWRBTN_LED = LED_PWM;
#endif //Support_PWRBTN_LED
    return;
  }

 //******************************
//LED blink on S3
//******************************
  if ((IS_MASK_SET(Led_status,PWRLED_Gradually)&&IS_MASK_SET(Led_status,PWRLED_ON))  \
     || (IS_MASK_SET(Led_status,LOGOLED_Gradually)&&IS_MASK_SET(Led_status,LOGOLED_ON)) \
  )  //
  {
    LED_SLOW_BLINK(); 
    if (IS_MASK_SET(Led_status,LOGOLED_Gradually)&&IS_MASK_SET(Led_status,LOGOLED_ON)){
      LOGO_LED = LED_PWM_Status;
    }

#if Support_PWRBTN_LED
    if (IS_MASK_SET(Led_status,PWRLED_Gradually)&&IS_MASK_SET(Led_status,PWRLED_ON)) {
      if(!Read_LID_SW_IN()){
         PWRBTN_LED = LED_OFF;
         return;
      }
      PWRBTN_LED = LED_PWM_Status;
    }
#endif  //Support_PWRBTN_LED    
  }

  
//*******************************
//LED On
//******************************* 
  PowerLed_Count++;
  if ( PowerLed_Count <= 5 )
  {
#if Support_PWRBTN_LED  
    if (IS_MASK_SET(Led_status,PWRLED_ON) && IS_MASK_CLEAR(Led_status,PWRLED_Gradually)){
      if(!Read_LID_SW_IN()) //A285D00078
      {
            PWRBTN_LED = LED_OFF;
      }
      else
      {
      PWRBTN_LED = LED_ON;  // PWR LED
    }
    }
#endif //Support_PWRBTN_LED    
    if (IS_MASK_SET(Led_status,LOGOLED_ON)&& IS_MASK_CLEAR(Led_status,LOGOLED_Gradually)){
      LOGO_LED = LED_ON;  // PWR LED     
    }
  }

//*******************************
//LED OFF
//******************************* 
#if Support_PWRBTN_LED  
    if (IS_MASK_CLEAR(Led_status,PWRLED_ON)){
      PWRBTN_LED = LED_OFF;  // PWR LED
    }
#endif //Support_PWRBTN_LED    
    if (IS_MASK_CLEAR(Led_status,LOGOLED_ON)){
      LOGO_LED = LED_OFF;  // PWR LED     
    }
 
  
//*******************************
//LED blink during enter S4
//******************************* 
  if( (PowerLed_Count > 5) && (PowerLed_Count < 10) )
  {
#if Support_PWRBTN_LED      
    if (IS_MASK_SET(Led_status,PWRLED_ON)&&IS_MASK_SET(Led_status,PWRLED_Blinking)) {
      PWRBTN_LED = LED_ONOFF(!IS_MASK_SET(Led_status,PWRLED_Blinking)); 
    }
#endif //Support_PWRBTN_LED        
    if  (IS_MASK_SET(Led_status,LOGOLED_ON)&&IS_MASK_SET(Led_status,LOGOLED_Blinking)) {
      LOGO_LED = LED_ONOFF(!IS_MASK_SET(Led_status,LOGOLED_Blinking));  
    }  
  }
  if ( PowerLed_Count >= 10 )
  {
    PowerLed_Count = 0;
  }
}

/*
*******************************************************************************
* Function name: TPG_LED_control
*
* Descriptoin: monitor EC name space 0x0C LED control 
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void TPG_LED_control (void)
{
  if (u4LEDcontrol != LEDcontrol_bak)
  { 
    //update LED state  
    LEDcontrol_bak = u4LEDcontrol;

    switch( u4LEDcontrol & LED_ID)   //bit 3 - 0 LED ID
    {
      case POWER_LED:
        Led_status &= 0xF;      //clear power led status        
        Led_status |= (u4LEDcontrol & 0xF0);   //update power led status
        CLEAR_MASK(Led_status, BatLED_test);  //clear battery led test mode
        break;     
#if (Support_BATTCHGLED)        
      case Battery_LED_0:  // battery Orange LED
        SET_MASK(Led_status, BatLED_test);
        if (u4LEDcontrol & LED_TURN_ON){
          Charging_LED_O;
        }else{
          Charging_LED_OFF;
        }     
        break;      
      case Battery_LED_1:  // battery Green LED   
        SET_MASK(Led_status, BatLED_test);
        if (u4LEDcontrol & LED_TURN_ON){
          Charging_LED_G;
        }else{
          Charging_LED_OFF;
        }           
        break;
#endif //Support_BATTCHGLED        
      case Suspend_LED:       
        break;      
      case Dock_LED_1:    
        break;      
      case Dock_LED_2:    
        break;
      case THINKPAD_LOGO_LED:
        Led_status &= 0xF0;      //clear logo led status
        Led_status |= (u4LEDcontrol >> 4);   //update power led status
        break; 
      case Microphone_mute_LED:   
#if Support_Microphone_Mute_LED
        Microphone_Mute_LED();      
#endif //Support_Microphone_Mute_LED
        break;
      default:
        break;
    }  

    if (IS_MASK_SET(Led_status,PWRLED_Gradually) && IS_MASK_SET(Led_status,LOGOLED_Gradually))  // Gradually flag is chaged
    {
       Led_PWM_step = 0;            // clear PWRLED_Gradually flag
       LED_PWM_Status = 0;
    }
    if (IS_MASK_CLEAR(Led_status,PWRLED_Gradually) || IS_MASK_CLEAR(Led_status,LOGOLED_Gradually))
    {
       Led_PWM_step = 0;            // clear PWRLED_Gradually flag
    }  
  }
}

/*
*******************************************************************************
* Function name: Mute_LED
*
* Descriptoin: BIOS will set EC ram 30.6 by Method( MMTS )
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
#if Support_MuteLED
void MuteLED(void)    
{
 if (SystemIsS0 && IS_MASK_SET(SYS_MISC1, ACPI_OS))   //S0 and in OS ?
  {
  if (MutevolumeEGet)  //mute
      {Mute_LED_ON();}
  else
      {Mute_LED_OFF();}
 }
 else
  {Mute_LED_OFF();}     //if not S0 trun off Mute LED
}
#endif

/*
*******************************************************************************
* Function name: FnLK_LED
*
* Descriptoin: BIOS will set EC ram 00.2 
*              FnLK => 00.2 = 1 (LED off)
*              Fn un LokK => 00.2 = 0 (LED on)
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
#if Support_FnLKLED
void FnLKLED(void)    
{
 if (SystemIsS0 && IS_MASK_SET(SYS_MISC1, ACPI_OS))   //S0 and in OS ?
 {
#if KB_FnSticky_Key 
   if (FnStickyGET) //Sticky enable ?
       {FNLED_ONOFF (STICKY_MODE_GET || STICKY_LOCK_GET);}
   else
#endif //KB_FnSticky_Key   
    {    
      FNLED_ONOFF (FNLKSTATEGET);
    }
 }
 else if (SystemIsS0)
  {FN_LED_ON();}     //if not S0 trun off Mute LED
 else
  {FN_LED_OFF();}     //if not S0 trun off Mute LED
}
#endif

/*
*******************************************************************************
* Function name: Microphone_Mute_LED
*
* Descriptoin: BIOS will set EC ram 0C 
*              Microphone_Mute_LED on => 0Ch set 8Eh
*              Microphone_Mute_LED off => 0Ch set 0Eh               
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
#if Support_Microphone_Mute_LED
void Microphone_Mute_LED(void)    
{
 if (SystemIsS0 && IS_MASK_SET(SYS_MISC1, ACPI_OS))   //S0 and in OS ?
  {
  if (u4LEDcontrolGet)  //Microphone_Mute_LED on ?
      {MICMute_LED_ON();}
  else
      {MICMute_LED_OFF();}
 }
 else
  {MICMute_LED_OFF();}     //if not S0 trun off Mute LED
}
#endif

/*
********************************************************************************
* Function name: AC in LED
*
* Description:
*
*       AC in Blink 3 Times (0.25 s On/ 0.25 s Off , repeat x 3).
*
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/
BYTE ACin_LED(void) {
    BYTE temp8;
    if (SystemNotS5)
    {
      if (ACIN_BLINK3 > 30)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
      else if (ACIN_BLINK3 > 25)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 20)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
      else if (ACIN_BLINK3 > 15)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 10)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
      else if (ACIN_BLINK3 > 5)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 0)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
    }
    else
    {
       if (ACIN_BLINK3 > 25)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 20)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
      else if (ACIN_BLINK3 > 15)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 10)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
      else if (ACIN_BLINK3 > 5)
       temp8 = LED_ON;              //   PWR_LED_ON();
      else if (ACIN_BLINK3 > 0)
       temp8 = LED_OFF;              //   PWR_LED_OFF();
    }
    ACIN_BLINK3--;
    if (ACIN_BLINK3==0)
    {
      temp8 = LED_OFF;              //   PWR_LED_OFF();
      Led_PWM_step &= 0x40;
    }
  return temp8;
}

/*
********************************************************************************
* Function name: LED_SLOW_BLINK
*
* Description:
*      System in Standby
*      LED Behavior : Gradual 1 s On, Gradual 1 s Off, 3 s Off
*     
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/
void LED_SLOW_BLINK(void) {
    //Off for 3 seconds. 
    if(((Led_PWM_step&0x3F) == 0) 	&& IS_MASK_CLEAR(Led_PWM_step,LED_countupdown)) { 
      pPWRLEDS3_off_Count ++;
		 if(pPWRLEDS3_off_Count < S3POWERLED_OFF_Time) {
		   LED_PWM_Status = 0;
		   return ;
		  } else {
		 	 pPWRLEDS3_off_Count = 0;
		  }
	  }
    if ((Led_PWM_step& LED_countupdown)==0) {                // count up
      Led_PWM_step ++;
      LED_PWM_Status++;
      if ((Led_PWM_step&0x3F)>4){
        LED_PWM_Status +=3;
      }
      if ((Led_PWM_step&0x3F)>7){
        LED_PWM_Status +=5;
      }
      if ((Led_PWM_step&0x3F)>10){
        LED_PWM_Status +=9;
      }
      if ((Led_PWM_step&0x3F)>12){
        LED_PWM_Status += 5 ;
      }
      if ((Led_PWM_step&0x3F)>19){
        LED_PWM_Status = 0xFF ;
      }
      if ((Led_PWM_step&0x3F)> 22){
        SET_MASK(Led_PWM_step,LED_countupdown);   // change to count down
      }
    }
    else
    {
      Led_PWM_step --;
      LED_PWM_Status -- ;
      if ((Led_PWM_step&0x3F) >4){
        LED_PWM_Status -=3;
      }
      if ((Led_PWM_step&0x3F) >7){
        LED_PWM_Status -=6;
      }
      if ((Led_PWM_step&0x3F) >10){
        LED_PWM_Status -=9 ;
      }
      if ((Led_PWM_step&0x3F) >12){
        LED_PWM_Status -= 5 ;
      }
      if ((Led_PWM_step&0x3F) >19){
        LED_PWM_Status = 0xFF ;
       }
      if ((Led_PWM_step&0x3F) == 0)
      {
        CLEAR_MASK(Led_PWM_step,LED_countupdown);   // change to count up
        LED_PWM_Status = 0x00;   
      }
    }
}

#if (Support_BATTCHGLED)

void Batt_AC_Charging_LED(BYTE stateOfChgL, BOOL charging)
{
  if(stateOfChgL >= Charging_LED_Green){
    Charging_LED_G;

#if Support_BATTCHGLED_BLINK 
  }else if (stateOfChgL > Charging_LED_Orange)){
    Charging_LED_O;
    Cgr_LED_Count = 0;
  }else if (stateOfChgL <= Charging_LED_Orange){
     Charging_LED_Blink();
  }   

#else
  }else if(charging){
    Charging_LED_O;
    Cgr_LED_Count = 0;
  }else{
    Charging_LED_OFF;
    Cgr_LED_Count = 0;
  }

#endif //Support_BATTCHGLED_BLINK

}

#endif //Support_BATTCHGLED
/*
********************************************************************************
* Function name: AC_Charging_LED
*
* Description:  Charging(0%-90%)   Amber (solid on)
                Charging(90%plus)  Green  (solid on)
                Not Charging            Off
*               Optional: To indicate when Charge is below ~20%, blink LED
*               (blink 0.25 s On/ 0.25 s Off for total 1.25 second duration; 
*               remain off for 3 seconds, then repeat blink-&-off pattern 
*               until charge level is above ~20%)
*               
*     
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/
void AC_Charging_LED (void){
#if (Support_BATTCHGLED)
   if (IS_MASK_SET(Led_status, BatLED_test)){   //in test mode
     return;
   }

   if(IS_MASK_CLEAR(u4SenseSTS3, SomeAcAdapterIn)){
     Charging_LED_OFF;
     Cgr_LED_Count = 0;
     return;

   }

   if(AllBattExistGet){
     if(IS_MASK_SET(PriBattSTS, Charging) || IS_MASK_SET(SecBattSTS, Charging)){
       if(RelativeStateOfChgL < SecRelativeStateOfChgL){
         Batt_AC_Charging_LED(RelativeStateOfChgL, TRUE);
       }else{
         Batt_AC_Charging_LED(SecRelativeStateOfChgL, TRUE);
       }

     }else if((RelativeStateOfChgL >= Charging_LED_Green) && (SecRelativeStateOfChgL >= Charging_LED_Green)){
       Batt_AC_Charging_LED(RelativeStateOfChgL, TRUE);
     }else{
       Charging_LED_OFF;
       Cgr_LED_Count = 0;
     }

   }else if(BattExistGet && !SecBattExistGet){
     Batt_AC_Charging_LED(RelativeStateOfChgL, IS_MASK_SET(PriBattSTS, Charging));

   }else if(!BattExistGet && SecBattExistGet){
     Batt_AC_Charging_LED(SecRelativeStateOfChgL, IS_MASK_SET(SecBattSTS, Charging));

   }else{
     Charging_LED_OFF;
     Cgr_LED_Count = 0;

   }

#endif //(Support_BATTCHGLED)   
}

/*
********************************************************************************
* Function name: Charging_LED_Blink
*
* Description: blink 0.25 s On/ 0.25 s Off for total 1.25 second duration; 
*              remain off for 3 seconds, then repeat blink-&-off pattern until charge level is above ~20%     
*      
*     
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/
void Charging_LED_Blink(void)
{
#if (Support_BATTCHGLED_BLINK)
   Cgr_LED_Count++;
   if ( Cgr_LED_Count <= 5 ){
      Charging_LED_O;      
   }
   else if ( Cgr_LED_Count <= 10 ){
      Charging_LED_OFF;
   }
   else if ( Cgr_LED_Count <= 15 ){
      Charging_LED_O;
   }
   else if ( Cgr_LED_Count <= 20 ){
      Charging_LED_OFF;
   } 
   else if ( Cgr_LED_Count <= 25 ){
      Charging_LED_O;
   }  
   else if ( Cgr_LED_Count < 85 ){
      Charging_LED_OFF;
   }    
   else if ( Cgr_LED_Count == 85 ){
      Cgr_LED_Count = 0;
   }
#endif //(Support_BATTCHGLED_BLINK)   
}

#if Support_KBD_BL_LED
/*
*******************************************************************************
* Function name: AcpiWriteKbdBacklightLED
*
* Descriptoin: 0:Off, 1:Low, 2:High
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void AcpiWriteKbdBacklightLED(void)    
{
#if Support_KBD_BL_DETECT
	if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
	{
		if(DisableKbdBackLightGET)
		{
			KBD_BL_LED = LED_OFF;
			KBL_Target_Level_Duty = 0;
			KBL_Current_Level_Duty = 0;
			u4PeriCtrl3 &= ~PC3_TURN_ON_KEYBOARD_BACK_LIGHT_MASK;
		}
		else if(SystemIsS0)
		{
			if((PC3_TURN_ON_KEYBOARD_BACK_LIGHT >> 6) == KBD_BACKLIGHT_OFF)
			{
				KBL_Target_Level_Duty = 0;
			}
			else if((PC3_TURN_ON_KEYBOARD_BACK_LIGHT >> 6) == KBD_BACKLIGHT_LOW)
			{
				KBL_Target_Level_Duty = KBD_BL_LED_LOW;
			}
			else
			{
				KBL_Target_Level_Duty = KBD_BL_LED_HIGH;
			}
			ChangeKbdBacklightLedIntervalTime();
		}
	}
}
/*
*******************************************************************************
* Function name: ChangeKbdBacklightLedIntervalTime
*
* Descriptoin: 0:Off, 1:Low, 2:High
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void ChangeKbdBacklightLedIntervalTime(void)    
{
	if(KBL_Target_Level_Duty != KBL_Current_Level_Duty)
	{
		if(KBL_Target_Level_Duty > KBL_Current_Level_Duty)
		{
			KBL_Interval_Cnt = 100/(KBL_Target_Level_Duty - KBL_Current_Level_Duty);	//Max change time is 500ms/5ms(polling time)=100
		}
		else	
	  	{
			KBL_Interval_Cnt = 100/(KBL_Current_Level_Duty - KBL_Target_Level_Duty);	//Max change time is 500ms/5ms(polling time)=100
		}
		KBL_Interval_Cnt_Temp = KBL_Interval_Cnt;
	}
}
/*
*******************************************************************************
* Function name: KbdBacklightLED
*
* Descriptoin: High:100% duty, Low:??% duty, Off:0% duty
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void KbdBacklightLED(void)    
{
	BYTE Temp;
#if Support_KBD_BL_DETECT
	if(Read_KBD_BL_DTCT())
	{
		KbdBackLightExistEN;
	}
	else
	{
		KbdBackLightExistDIS;
	}
	if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
	{
		if(KBD_BL_LED_PWM_REG != ALT){
			KBD_BL_LED_PWM_REG = ALT;
		}
		if(SystemIsS0)   //S0
		{
			if(IS_MASK_SET(KBL_Status, HotKeyKbdBlBlick)) //A485D00091
			{
				switch(KBL_Blink_Level_State)
				{
				case KBL_BLINK_STATUS_IDLE:
					CLEAR_MASK(KBL_Status, HotKeyKbdBlBlick);
					KBL_Current_Level_Duty = KBL_Blink_Level_Duty;
					break;
				case KBL_BLINK_STATUS_UP:
					if(KBL_Blink_Level_Duty < KBD_BL_LED_HIGH)
					{
						KBL_Blink_Level_Duty += KBL_Hotkey_Blink_Duty;
					}
					else
					{
						KBL_Blink_Level_Duty = KBD_BL_LED_HIGH;
					}
					
					KBD_BL_LED = Cal_PWM_Duty(KBL_Blink_Level_Duty, KBD_BL_LED_PWM_Max);

					if(++KBL_Status_Time_Count >= KBL_Hotkey_Blink_Time_Interval)
					{
						KBL_Status_Time_Count = 0;
						KBL_Blink_Level_State = KBL_BLINK_STATUS_DOWN;
					}
					break;
				case KBL_BLINK_STATUS_KEEP:
					if(++KBL_Status_Time_Count >= KBL_Hotkey_Blink_Time_Interval)
					{
						KBL_Status_Time_Count = 0;
						//KBL_Blink_Level_State = KBL_BLINK_STATUS_DOWN;
						KBL_Blink_Level_State = KBL_BLINK_STATUS_UP;
					}
					break;
				case KBL_BLINK_STATUS_DOWN:
					if(KBL_Blink_Level_Duty > KBL_Hotkey_Blink_Duty)
					{
						KBL_Blink_Level_Duty -= KBL_Hotkey_Blink_Duty;
					}
					else
					{
						KBL_Blink_Level_Duty = 0;
					}
					
					KBD_BL_LED = Cal_PWM_Duty(KBL_Blink_Level_Duty, KBD_BL_LED_PWM_Max);

					if(++KBL_Status_Time_Count >= KBL_Hotkey_Blink_Time_Interval)
					{
						KBL_Status_Time_Count = 0;
						if(++KBL_HotKey_Blink_Times >= KBL_HotKey_Blink_Achive)
						{
							CLEAR_MASK(KBL_Status, HotKeyKbdBlBlick);
							KBL_Blink_Level_State = KBL_BLINK_STATUS_IDLE;
							KBL_Current_Level_Duty = KBL_Blink_Level_Duty;
						}
						else
						{
							if(KBL_BLICK_TIMES_HOTKEY_PERFORMANCE == KBL_HotKey_Blink_Achive)
							{
							KBL_Blink_Level_State = KBL_BLINK_STATUS_UP;
						}
							else
							{
								//KBL_Blink_Level_State = KBL_BLINK_STATUS_UP;
								KBL_Blink_Level_State = KBL_BLINK_STATUS_KEEP;
							}
						}
					}
					break;
				default:
					CLEAR_MASK(KBL_Status, HotKeyKbdBlBlick);
					KBL_Current_Level_Duty = KBL_Blink_Level_Duty;
					KBL_Blink_Level_State = KBL_BLINK_STATUS_IDLE;
					break;
				}
				
			}		
			else if(KBL_Current_Level_Duty != KBL_Target_Level_Duty)
			{
				if(KBL_Interval_Cnt == 0)
				{
					if(KBL_Current_Level_Duty < KBL_Target_Level_Duty)
					{
						KBL_Current_Level_Duty++;
					}
					else
					{
						KBL_Current_Level_Duty--;
					}
					KBD_BL_LED = Cal_PWM_Duty(KBL_Current_Level_Duty, KBD_BL_LED_PWM_Max);
					KBL_Interval_Cnt = KBL_Interval_Cnt_Temp;
				}
				else
				{
					KBL_Interval_Cnt--;
				}
				if(IS_MASK_CLEAR(KBL_Status, HotkeyLED_NONEED_update))
				SET_MASK(KBL_Status,NeedUpdateAcpiKbdBlStatus);
			}
			else
			{
				if(IS_MASK_SET(KBL_Status, HotkeyLED_NONEED_update))
				{
					CLEAR_MASK(KBL_Status,NeedUpdateAcpiKbdBlStatus);
					CLEAR_MASK(KBL_Status, HotkeyLED_NONEED_update);
				}
				if(IS_MASK_SET(KBL_Status,NeedUpdateAcpiKbdBlStatus))
				{
					if(KBL_Current_Level_Duty == KBD_BL_LED_HIGH)
					{
						Temp = KBD_BACKLIGHT_ON;
					}
					else if(KBL_Current_Level_Duty == KBD_BL_LED_LOW)
					{
						Temp = KBD_BACKLIGHT_LOW;
					}
					else
					{
						Temp = KBD_BACKLIGHT_OFF;
					}
					if((PC3_TURN_ON_KEYBOARD_BACK_LIGHT >> 6) != Temp)
					{
						u4PeriCtrl3 &= ~PC3_TURN_ON_KEYBOARD_BACK_LIGHT_MASK;
						u4PeriCtrl3 |= (Temp << 6);
					}
					CLEAR_MASK(KBL_Status,NeedUpdateAcpiKbdBlStatus);
					ECQEvent(KB_LIGHT_EVENT_1F);
				}
			}
		}
		else
		{     //if not S0 trun off LED
			KBD_BL_LED = LED_OFF;
			KBL_Target_Level_Duty = 0;
			KBL_Current_Level_Duty = 0;
	    		SET_MASK(KBL_Status,NeedUpdateAcpiKbdBlStatus);
		}
		
		if(IS_MASK_SET(KBL_Status,PowerOnCheck))
		{
			if((KBL_Current_Level_Duty == 100)&&(KBL_Target_Level_Duty==100))
			{
				KBL_Target_Level_Duty = 0;
				KBL_Interval_Cnt = 1;
				KBL_Interval_Cnt_Temp = KBL_Interval_Cnt;
			}
			else if((KBL_Target_Level_Duty == 0)&&(KBL_Current_Level_Duty == 0))
			{
				CLEAR_MASK(KBL_Status,PowerOnCheck);
			}
		}
	}
}
#endif
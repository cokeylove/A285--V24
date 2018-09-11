/*-----------------------------------------------------------------------------
 * TITLE: OEM_BEEP.C
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#include "..\..\include.h"


/*
********************************************************************************
* Function name: BeepControl
*
* Description:
*
* Arguments: none
*    
* Return Values: none
*    
********************************************************************************
*/
void BeepControl(void)
{
  if( u4BeepID )
  {
    switch( u4BeepID )
    {
        case 0x01:  // CRITICAL_LOW_BATTERY
            if( BeepCriLowGET )
            {
                if( p2BeepCnt2 < 3 )
                {
                    audBEEPedgeon;
                    if( p2BeepCnt1 <= 50 )
                    {
                        BeepingEN;
                        p2BeepCnt1++;
                        BEEP_1250HZ;
                    }
                    else if( p2BeepCnt1 <= 100 )
                    {
                        p2BeepCnt1++;
                        BEEP_315HZ;
                    }
                    else
                    {
                        p2BeepCnt1 = 0;
                        p2BeepCnt2++;
                    }
                }
                else if( p2BeepCnt2 >= 3 )
                {
                    if( p2BeepCnt3 <= 100)
                    {
                        p2BeepCnt3++;
                        audBEEPoff;
                    }
                    else
                    {
                        if( IS_MASK_CLEAR(p2BeepCtrl,p2RptCnt) )
                        {
                            p2BeepCnt2 = 0;
                            SET_MASK(p2BeepCtrl,p2RptCnt);
                        }
                        else
                        {
                            if( !u4BeepRepeat )
                            {
                                BeepingDIS;
                                u4BeepID = 0;
                            }
                            p2BeepCnt2 = 0;
                            p2BeepCnt3 = 0;
							audBEEPoff;
                            CLEAR_MASK(p2BeepCtrl,p2RptCnt);
                        }
                    }
                }
            }
            break;
        case 0x02:  // LOW_BATTERY
            if( BeepLowGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else if( p2BeepCnt1 <= 64 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 77 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x03:  // SUSPEND
            if( SuspendGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x04:  // VM_SUSPEND
            if( VMSuspendGET )
            {
                if( p2BeepCnt1 <= 25 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else if( p2BeepCnt1 <= 50 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x05:  // RESUME
            if( ResumeGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x06:  // DC_IN_OUT
            if( DCInOutGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1400HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x07:  // POWER_OFF
            if( PowerOffGET )
            {
                if( p2BeepCnt1 <= 25 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1250HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x08:  // POWER_OFF_WARN
            if( PowerOffWarnGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x09:  // NO_HDD_WARN
            if( NoHDDWarnGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 52 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 65 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0A:  // DEAD
            if( DeadGET )
            {
                if( p2BeepCnt1 < 4 )
                {
                    if( p2BeepCnt2 <=100 )
                    {
                        BeepingEN;
                        audBEEPedgeon;
                        p2BeepCnt2++;
                        BEEP_1250HZ;
                    }
                    else
                    {
                        p2BeepCnt1++;
                        p2BeepCnt2 = 0;
                    }
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0B:  // 440_HZ
            if( F440HzGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_440HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0C:  // 315_HZ
            if( F315HzGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0D:  // 315_HZ_2_TIMES
            if( F315Hz2TimesGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else if( p2BeepCnt1 <= 52 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0E:  // 315_HZ_3_TIMES
            if( F315Hz3TimesGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else if( p2BeepCnt1 <= 52 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 65 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_315HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
        case 0x0F:  // INHIBIT_SWAP
            if( InhibitSwapGET )
            {
                if( p2BeepCnt1 <= 13 )
                {
                    BeepingEN;
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 26 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 39 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else if( p2BeepCnt1 <= 52 )
                {
                    audBEEPoff;
                    p2BeepCnt1++;
                }
                else if( p2BeepCnt1 <= 65 )
                {
                    audBEEPedgeon;
                    p2BeepCnt1++;
                    BEEP_1760HZ;
                }
                else
                {
                    audBEEPoff;
                    p2BeepCnt1 = 0;
                    if( !u4BeepRepeat )
                    {
                        BeepingDIS;
                        u4BeepID = 0;
                    }
                }
            }
            break;
    }
  }
  else
  {
  //  p2BeepCnt1 = 0;
  //  p2BeepCnt2 = 0;
  //  p2BeepCnt3 = 0;
  //  audBEEPoff;
  //  BeepingDIS;
  }

}


/*
*******************************************************************************
* Function name: HOOKPWM_Frequency
*
* Descriptoin: The function for PWM feature to set each Prescaler clock source Frequency.
*              EX:  PWMFrequencySet(channel_group, 200,91, 00);
*
* Parameters:
*  [sPWMFlag]
*         bit 0-1    : set pre ch ;(00:ch0 ,01:ch4,01,10:ch6,11:ch7)
*         bit 2      : Pre clock select ( 0:9.2Mhz)
*  [sCTRSet]
*         Each prescaler clock CTR setting
*  [sCicPrsLSB]
*         The prescaler clock time and minimal pulse with of PWM (LSB)
*  [sCicPrsMSB]
*         The prescaler clock time and minimal pulse with of PWM (MSB)
*         ps: Channel 0 just 1 byte. So channel 0 is not support MSB byte.
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void HOOKPWM_Frequency(BYTE sPWMFlag,BYTE sCTRSet,BYTE sCicPrsMSB,BYTE sCicPrsLSB)
{
  WORD pwm_temp, pwm_freq;

     //  ZTIER = 0x00;                                    //Disable PWM
    switch(sPWMFlag&0x03)
    {
      case 0x00: // Pre Ch0
          CTR = sCTRSet;
            C0CPRS = sCicPrsLSB;
            PCFSR =0x03;
            break;

    case 0x01: // Pre Ch4
            CTR1 = sCTRSet;
      pwm_temp = (920000/sCTRSet);
            pwm_freq = (WORD)((sCicPrsMSB<<8)+sCicPrsLSB);
            if (pwm_freq > 0)
            {
              if (pwm_freq >= pwm_temp)
                pwm_freq=pwm_temp;
                pwm_temp=(WORD)(pwm_temp/pwm_freq);
                if (pwm_temp>0) pwm_temp--;
            }
            else
            {
              pwm_temp=0xA000;  //1hz setting
            }
            C4CPRS =pwm_temp;
            C4MCPRS =pwm_temp>>8;
            PCFSR = 0x02;
            break;

       case 0x02: // Pre Ch6
            CTR2 = sCTRSet;
            C6CPRS =sCicPrsLSB;
            C6MCPRS =sCicPrsMSB;
            if(IS_MASK_SET(sPWMFlag, BIT2))
            {
              PCFSR |=0x04;
            }
            else
            {
                PCFSR &=0xFB;
            }
            break;

         case 0x03: // Pre Ch7
            CTR3 = sCTRSet;
            C7CPRS =sCicPrsLSB;
            C7MCPRS =sCicPrsMSB;
            if(IS_MASK_SET(sPWMFlag, BIT2))
              PCFSR |=0x08;
            else
              PCFSR &=0xF7;
            break;

    default:
            break;

    }
     //   ZTIER = 0x02;                                 //Enable PWM
}



/*
*******************************************************************************
* Function name: HOOKPWM_ClockSource
*
* Descriptoin: The function for PWM feature to set Prescaler clock source for each PWM pin.
*              EX:   HOOKPWMClockSourceSet(PWM_Pre_ch1, PWM7);
*
*  Parameters:   [sClockCh]¡V Prescaler Source setting (ex :PWM_Pre_ch0=00, PWM_Pre_chc4=01, PWM_Pre_ch6=10)
*                [sPWMPin] ¡V PWM channel number (PWM0~PWM7 is 0~7)
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void HOOKPWM_ClockSource(BYTE sClockCh,BYTE sPWMPin)
{
  switch(sPWMPin)
    {
      case 0x00:
      PCSSGL &= 0xFC;
            PCSSGL |= sClockCh;
            break;

    case 0x01:
            sClockCh = (sClockCh <<2)&0x0C;
            PCSSGL &= 0xF3;
            PCSSGL |= sClockCh;
            break;

    case 0x02:
            sClockCh = (sClockCh <<4)&0x30;
            PCSSGL &= 0xCF;
            PCSSGL |= sClockCh;
            break;

    case 0x03:
            sClockCh = (sClockCh <<6)&0xC0;
            PCSSGL &= 0x3F;
            PCSSGL |= sClockCh;
            break;

    case 0x04:
            PCSSGH&= 0xFC;
            PCSSGH |= sClockCh;
            break;

    case 0x05:
            sClockCh = (sClockCh <<2)&0x0C;
            PCSSGH&= 0xFC;
            PCSSGH |= sClockCh;
            break;

    case 0x06:
            sClockCh = (sClockCh <<4)&0x30;
            PCSSGH &= 0xCF;
            PCSSGH |= sClockCh;
            break;

    case 0x07:
            sClockCh = (sClockCh <<6)&0xC0;
            PCSSGH &= 0x3F;
            PCSSGH |= sClockCh;
            break;

    default:
            break;

  }
}

/*
*******************************************************************************
* Function name: PwmModulator
*
* Descriptoin: Set PWM output duty cycle percentage by percent and frequency
*              by freq for PWM channel pwm_ch and also enable PWM channel pwm_ch.
*              Parameters:
*
*  Parameters: [percent]¡V PWM output duty cycle percentage (from 0%~100%)
*              [freq]    ¡V PWM output frequency 4600~0000(from  46Khz~ 1Hz)
*              [pwm_ch] ¡V PWM channel number (PWM0~PWM7 is 0~7)
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void PwmModulator(BYTE percent, WORD freq, BYTE pwm_ch)
{
  volatile unsigned char * byte_register_pntr;
  HOOKPWM_Frequency(channel_group, 0xC8, freq>>8, freq);
  HOOKPWM_ClockSource(channel_group, pwm_ch);    //set PWM channel 4
  byte_register_pntr = &DCR0;
  byte_register_pntr += pwm_ch;
  *byte_register_pntr = percent*2;   //duty cycle
}


/*
*******************************************************************************
* Function name: Keyboard beep
*
* Descriptoin: 
*
* Parameters: 
*
* Arguments: none
*
* Return Values: none
*       
*******************************************************************************
*/
void Keyboard_beep(void)
{
  if( (IP_GHOSTBEEP == TRUE) )	
  {
   	if(  IS_MASK_CLEAR(u4Cfgspace3,PhantomKeyIgnor) ) 
  	  {
  		  if( IP_GBEEPCnt <= 5 )
    	  {
       	if( IP_GBEEPCnt == 0 )
        	{
        		Buffer_Key(0x00);    //
    		    F_Service_SEND = 1;
        	}
        	IP_GBEEPCnt++;
    	  }
    	  else
    	  {
        IP_GBEEPCnt = 0;
		 	 IP_GHOSTBEEP = FALSE;
    	  }
  	  }
	  else
	  {
		  IP_GBEEPCnt = 0;
		  IP_GHOSTBEEP = FALSE;
	  }
  }
}
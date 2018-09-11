/*-----------------------------------------------------------------------------
 * TITLE: OEM_BEEP.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *---------------------------------------------------------------------------*/

#ifndef OEM_BEEP_H
#define OEM_BEEP_H

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

extern void BeepControl(void);

#define channel_group  7       // it used CTR0
extern void PwmModulator(BYTE percent, WORD freq, BYTE pwm_ch);
extern void Keyboard_beep(void);

/*===========================================================================
 * EDGE Beep Sound
 *===========================================================================*/
#define BEEP_315HZ		OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,315,50)
#define BEEP_440HZ		OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,440,50)
#define BEEP_1250HZ		OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,1250,50)
#define BEEP_1400HZ		OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,1400,50)
#define BEEP_1760HZ		OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,1760,50)


//--------------------Audio Control Function---------------------------------------
#define audBEEPedgeon  PWM_Channelx_Enable(BEEP_PWM_Channel)//(GPCRA6 = ALT)
#define audBEEPon      OemPwmSetting(BEEP_PWM_Channel,BEEP_PWM_Prescaler,500,50)//(GPCRA6 = ALT; PwmModulator(50,500,1);)  // active Low//detect audio's beep sound
#define audBEEPoff     PWM_Channelx_Disable(BEEP_PWM_Channel)//(GPCRA6 = PortA6_Ctrl)

#endif

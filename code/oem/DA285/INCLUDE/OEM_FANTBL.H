/*-----------------------------------------------------------------------------
 * TITLE: OEM_FANTBL.H
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef  OEM_FANTBL_H
#define OEM_FANTBL_H

//==========F-A-N==============================================================
#define FAN_PWM_Min      Cal_PWM_Duty(45,FAN_PWM_Max)   // 30% Standard PWM Minimum. //A485D00046 change 30 to 45fix fan initil fail
#define FAN_Std_RR1      Cal_PWM_Duty(1,FAN_PWM_Max)    // 1% Ramp rate1.
#define FAN_Std_RR2      Cal_PWM_Duty(2,FAN_PWM_Max)    // 2% Ramp rate2.
#define FAN_duty_offset  Cal_PWM_Duty(5,FAN_PWM_Max)    // 5% Duty cycle offset to target for duty ramp rate stting.

extern void oem_FanTblPtrHook(void);

#endif

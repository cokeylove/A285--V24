/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_pwm.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_PWM_H
#define API_PWM_H

/* 
 * ****************************************************************************
 * pwm formula
 * ****************************************************************************
 */
/*
 * The PWMi output signal duty cycle (in %, when INVPi is 0) is:
 * (DCRi)/(CTR+1) x 100
 * The PWMi output signal cycle time is :
 * (CiCPRS + 1) x (CTR + 1) x T clk
 * The PWMi output signal cycle frequency is :
 * FreqEC / ((CiCPRS + 1) x (CTR + 1))
 */

/* 
 * ****************************************************************************
 * PWM channel selection
 * ****************************************************************************
 */
#define PWM_Channel_0       0
#define PWM_Channel_1       1
#define PWM_Channel_2       2
#define PWM_Channel_3       3
#define PWM_Channel_4       4
#define PWM_Channel_5       5
#define PWM_Channel_6       6
#define PWM_Channel_7       7

#define PWM_PrescalerC4     0
#define PWM_PrescalerC6     1
#define PWM_PrescalerC7     2

#define PWM_CTR1            0
#define PWM_CTR2            1
#define PWM_CTR3            2

/* 8 MHz */
#define PWM_EC_Frequence            8000000

/* 32.769 KHz note : (x100) */
#define PWM_EC_Frequence_32_768K    3276800
#define PWM_Duty_Cycle_CTRx_Min     120

/* No register with this */
#define PWM_NoReg   (uchar_8 *) 0xFFFF 

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void PWM_Channelx_Output_Duty(BYTE p_channel_x, BYTE p_duty);
extern WORD PWM_Channelx_Register_Clock_Prescaler(
    BYTE p_channel_x, BYTE p_Prescaler, DWORD P_channel_frequence);
extern void PWM_Channelx_Enable(BYTE p_channel_x);
extern void PWM_Channelx_Disable(BYTE p_channel_x);
extern void PWM_Channelx_Clock_Prescaler_Selection(BYTE p_channel_x,
    BYTE p_Prescaler);
extern BYTE PWM_Channelx_Clock_Prescaler_Setting_Read(BYTE p_channel_x);
extern void PWM_Set_Prescaler_Clock_Frequency_Select(
    DWORD p_cpr_group, DWORD P_psdv);
extern void PWM_Set_Cycle_Time_Register(DWORD p_ctr_group, DWORD P_ctr_value);
extern WORD PWM_Channelx_Register_Clock_Prescaler_Low_Frequency(
    BYTE p_channel_x, BYTE p_Prescaler, DWORD P_channel_frequence);

/* 
 * ****************************************************************************
 * pwm registers struct
 * ****************************************************************************
 */
typedef struct PWMCtrlStruct
{
    uchar_8     *pwm_cycle_time_reg;	
	uchar_8 	*pwm_cpr_reg_lsb;
    uchar_8 	*pwm_cpr_reg_msb;
    uchar_8 	*pwm_pcfsr_reg;
    uchar_8 	pwm_pcfsr_ctrl_bit0;
} sPWMCtrlStruct;

typedef struct PWMStruct
{
    uchar_8     *pwm_duty_cycle_reg;
    uchar_8     *pwm_prescaler_clock_source_select_reg;
    uchar_8     *pwm_ctrl_pin;
} sPWMStruct;

#endif


/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_pwm.c
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/* 
 * ****************************************************************************
 * pwm struct
 * ****************************************************************************
 */
const sPWMCtrlStruct asPWMCtrlStruct[] = 
{
    /* Don't change this table */
 	{ &CTR1,    &C4CPRS,    &C4MCPRS,   &PCFSR,     BIT1    },
    { &CTR2,    &C6CPRS,    &C6MCPRS,   &PCFSR,     BIT2    },
    { &CTR3,    &C7CPRS,    &C7MCPRS,   &PCFSR,     BIT3    },
};

const sPWMStruct asPWMStruct[] = 
{
 	{ &DCR0,    &PCSSGL,    &GPCRA0    },
 	{ &DCR1,    &PCSSGL,    &GPCRA1    },
 	{ &DCR2,    &PCSSGL,    &GPCRA2    },
 	{ &DCR3,    &PCSSGL,    &GPCRA3    },
    { &DCR4,    &PCSSGH,    &GPCRA4    },
 	{ &DCR5,    &PCSSGH,    &GPCRA5    },
 	{ &DCR6,    &PCSSGH,    &GPCRA6    },
 	{ &DCR7,    &PCSSGH,    &GPCRA7    },
};

/**
 * ****************************************************************************
 * configuring CTRx, PCFSR, prescaler clock source select group high/low and
 * clock prescaler register.
 *
 * @return
 * l_real_frequence_setting, 0xFFFF --> fail, other --> ok
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 * p_Prescaler, PWM_PrescalerC4 | PWM_PrescalerC6 | PWM_PrescalerC7
 * P_channel_frequence, 1 ~ 31000 (Hz)
 *
 * @note
 * 1. clock source always ec frequency ( 8 MHz )
 * 2. always check return value "l_real_frequence_setting" != 0xFFFF
 *    to make sure setting is ok.
 * 3. only support up to three PWM channel have respective frequency setting,
 *    if the number of PWM channel greater than 3,
 *    please share frequency setting.
 *
 * ****************************************************************************
 */
WORD PWM_Channelx_Register_Clock_Prescaler(
    BYTE p_channel_x, BYTE p_Prescaler, DWORD P_channel_frequence)
{
    DWORD l_pwm_cpr;
    DWORD l_real_frequence_setting;
    DWORD l_index;
    DWORD l_deviation;
    DWORD l_pcs_shift;
    DWORD l_pcs_mask;

    /* pre-set setting fail */
    l_real_frequence_setting = 0xFFFF;

    if((p_channel_x <
        (sizeof(asPWMStruct)/sizeof(sPWMStruct)))&&(P_channel_frequence!=0))
    {
        /* deviation for frequency setting */
        l_deviation = (P_channel_frequence/100)+1;

        /* CTR default index */
        l_index=0xFF;

        /* to caculate ctr and cpr */
        while(l_index)
        {
            l_pwm_cpr = PWM_EC_Frequence/((l_index+1)*P_channel_frequence);

            if(l_pwm_cpr)
            {
                l_pwm_cpr--;
            }

            l_real_frequence_setting =
                PWM_EC_Frequence/((l_index+1)*(l_pwm_cpr+1));


            if(l_real_frequence_setting>P_channel_frequence)
            {
                if((l_real_frequence_setting%P_channel_frequence)<l_deviation)
                {
                    break;
                }
            }
            else
            {
                if((P_channel_frequence%l_real_frequence_setting)<l_deviation)
                {
                    break;
                }
            }

            l_index--;

            if(l_index<PWM_Duty_Cycle_CTRx_Min)
            {
                l_real_frequence_setting = 0xFFFF;
                break;
            }
        }

        /* frequency setting is OK */
        if(l_real_frequence_setting!=0xFFFF)
        {
            /* parameter "p_Prescaler" is valid */
            if(p_Prescaler< (sizeof(asPWMCtrlStruct)/sizeof(sPWMCtrlStruct)))
            {
                /*
                 * pin to alt function
                 * *asPWMStruct[p_channel_x].pwm_ctrl_pin = ALT;
                 * to use "PWM_Channelx_Enable" function
                 */
                
                /* inhibit pwm */
                CLEAR_MASK(ZTIER, BIT1);

                /* set cycle time register */
                *asPWMCtrlStruct[p_Prescaler].pwm_cycle_time_reg = l_index;

                /* clock frequency select register = ec clock 8MHz */
                *asPWMCtrlStruct[p_Prescaler].pwm_pcfsr_reg |=
                    asPWMCtrlStruct[p_Prescaler].pwm_pcfsr_ctrl_bit0;

                /*
                 * bit shift for "Prescaler Clock Source Select Group"
                 * register
                 */
                l_pcs_shift = (p_channel_x%4)*2;
                
                l_pcs_mask = (p_Prescaler+1) << l_pcs_shift;

                /* prescaler clock select */
                CLEAR_MASK(
                    *asPWMStruct[
                    p_channel_x].pwm_prescaler_clock_source_select_reg,
                    (BIT(l_pcs_shift)+BIT(l_pcs_shift+1)));
                *asPWMStruct[
                    p_channel_x].pwm_prescaler_clock_source_select_reg |=
                    (BYTE)l_pcs_mask;

                /* clock prescaler register msb */
                *asPWMCtrlStruct[p_Prescaler].pwm_cpr_reg_msb =
                    (BYTE)((l_pwm_cpr & 0x0000FF00) >> 8);

                /* clock prescaler register lsb */
                *asPWMCtrlStruct[p_Prescaler].pwm_cpr_reg_lsb =
                    (BYTE)((l_pwm_cpr & 0x000000FF));

                /* pwm clock counter enable */
                SET_MASK(ZTIER, BIT1);
            }
            else
            {
                /*
                 * parameter "p_Prescaler" isn't valid
                 * to return fail
                 */
                l_real_frequence_setting = 0xFFFF;
            }
        }
    }
    
    return((WORD)l_real_frequence_setting);
}

/**
 * ****************************************************************************
 * configuring CTRx, PCFSR, prescaler clock source select group high/low and
 * clock prescaler register.
 *
 * @return
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 * p_duty, 0 ~ 100
 *
 * ****************************************************************************
 */
void PWM_Channelx_Output_Duty(BYTE p_channel_x, BYTE p_duty)
{
    DWORD l_pcs_shift;
    DWORD l_pcs_mask;
    DWORD l_pcs_reg;

    /* illegal duty cycle */
    if(p_duty > 100)
    {
        /* */
    }
    else
    {
        /* parameter "p_channel_x" is valid. */
        if(p_channel_x < (sizeof(asPWMStruct)/sizeof(sPWMStruct)))
        {
            /* bit shift for "Prescaler Clock Source Select Group" register */
            l_pcs_shift = (p_channel_x%4)*2;

            /*
             * To get "Prescaler Clock Source Select Group" register,
             * original setting
             */
            l_pcs_reg = (*asPWMStruct[
                p_channel_x].pwm_prescaler_clock_source_select_reg);

            /* only bit0 bit1 information */
            l_pcs_mask = (l_pcs_reg >> l_pcs_shift)&0x03; 

            /* index for "asPWMCtrlStruct" table */
            l_pcs_mask -= 1;

            /* duty cycle == 100 */
            if(p_duty==100)
            {
                /* DCRx = CTRx */
                *asPWMStruct[p_channel_x].pwm_duty_cycle_reg =
                    *asPWMCtrlStruct[l_pcs_mask].pwm_cycle_time_reg;
            }
            else
            {
                /* to update DCRx depend on CTRx */
                *asPWMStruct[p_channel_x].pwm_duty_cycle_reg =
                    ((*asPWMCtrlStruct[l_pcs_mask].pwm_cycle_time_reg+1)*p_duty)
                    /100;
            }
        }
    }
}

/**
 * ****************************************************************************
 * To set PWM channel_[x] pin alt function and DCRx==0
 *
 * @return
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 *
 * ****************************************************************************
 */
void PWM_Channelx_Enable(BYTE p_channel_x)
{
    /* parameter "p_channel_x" is valid. */
    if(p_channel_x < (sizeof(asPWMStruct)/sizeof(sPWMStruct)))
    {
        /* default DCRx=0, no output */
        *asPWMStruct[p_channel_x].pwm_duty_cycle_reg = 0x00;
        
        /* pin to alt function */
        *asPWMStruct[p_channel_x].pwm_ctrl_pin = ALT;
    }
}

/**
 * ****************************************************************************
 * To set PWM channel_[x] pin input function and DCRx==0
 *
 * @return
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 *
 * ****************************************************************************
 */
void PWM_Channelx_Disable(BYTE p_channel_x)
{
    /* parameter "p_channel_x" is valid. */
    if(p_channel_x < (sizeof(asPWMStruct)/sizeof(sPWMStruct)))
    {
        /* default DCRx=0, no output */
        *asPWMStruct[p_channel_x].pwm_duty_cycle_reg = 0x00;
        
        /* pin to alt function */
        *asPWMStruct[p_channel_x].pwm_ctrl_pin = INPUT;
    }
}

/**
 * ****************************************************************************
 * To set pwm channel[x] prescaler clock source select register
 *
 * @return
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 * p_Prescaler, PWM_PrescalerC4 | PWM_PrescalerC6 | PWM_PrescalerC7
 *
 * ****************************************************************************
 */
void PWM_Channelx_Clock_Prescaler_Selection(BYTE p_channel_x, BYTE p_Prescaler)
{
    DWORD l_pcs_shift;
    DWORD l_pcs_mask;
    
    /* parameter "p_channel_x" is valid. */
    if(p_channel_x < (sizeof(asPWMStruct)/sizeof(sPWMStruct)))
    {
        /* bit shift for "Prescaler Clock Source Select Group" register */
        l_pcs_shift = (p_channel_x%4)*2;
        
        l_pcs_mask = (p_Prescaler+1) << l_pcs_shift;

        /* prescaler clock select */
        CLEAR_MASK(*asPWMStruct[
            p_channel_x].pwm_prescaler_clock_source_select_reg,
            (BIT(l_pcs_shift)+BIT(l_pcs_shift+1)));
        *asPWMStruct[p_channel_x].pwm_prescaler_clock_source_select_reg |=
            (BYTE)l_pcs_mask;
    }
}

/**
 * ****************************************************************************
 * To read pwm channel[x] prescaler clock source select register setting
 *
 * @return
 * 0, PCFSR
 * 1, C4CPRS
 * 2, C6CPRS
 * 3, C7CPRS
 * FFh, illegal
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 *
 * ****************************************************************************
 */
BYTE PWM_Channelx_Clock_Prescaler_Setting_Read(BYTE p_channel_x)
{
    DWORD l_pcs_shift;
    DWORD l_pcs_mask;
    BYTE l_pcs;

    l_pcs = 0xFF;
    
    /* parameter "p_channel_x" is valid. */
    if(p_channel_x < (sizeof(asPWMStruct)/sizeof(sPWMStruct)))
    {
        /* to read prescaler clock select */
        l_pcs_mask = *asPWMStruct[
            p_channel_x].pwm_prescaler_clock_source_select_reg;
            
        /* bit shift for "Prescaler Clock Source Select Group" register */
        l_pcs_shift = (p_channel_x%4)*2;

        l_pcs = (l_pcs_mask >> l_pcs_shift)&0x03;
    }

    return(l_pcs);
}

/**
 * ****************************************************************************
 * To set divider of PWM input clock.
 *
 * @return
 *
 * @parameter
 * p_cpr_group, PWM_PrescalerC4 | PWM_PrescalerC6 | PWM_PrescalerC7
 * P_psdv, prescaler divider value (0 ~ 0xFFFF)
 *
 * ****************************************************************************
 */
void PWM_Set_Prescaler_Clock_Frequency_Select(DWORD p_cpr_group, DWORD P_psdv)
{
    /* parameter "p_cpr_group" is valided */
    if(p_cpr_group < (sizeof(asPWMCtrlStruct)/sizeof(sPWMCtrlStruct)))
    {
        /* clock prescaler register msb */
        *asPWMCtrlStruct[p_cpr_group].pwm_cpr_reg_msb =
            (BYTE)((P_psdv & 0x0000FF00) >> 8);

        /* clock prescaler register lsb */
        *asPWMCtrlStruct[p_cpr_group].pwm_cpr_reg_lsb =
            (BYTE)((P_psdv & 0x000000FF));
    }
}

/**
 * ****************************************************************************
 * To set CTRx of PWM function
 *
 * @return
 *
 * @parameter
 * p_ctr_group, PWM_CTR1 | PWM_CTR2 | PWM_CTR3
 * P_ctr_value, cycle time (4 ~ 0xFF)
 *
 * ****************************************************************************
 */
void PWM_Set_Cycle_Time_Register(DWORD p_ctr_group, DWORD P_ctr_value)
{
    BYTE l_ctr_value;
    
    /* parameter "p_ctr_group" is valided */
    if(p_ctr_group < (sizeof(asPWMCtrlStruct)/sizeof(sPWMCtrlStruct)))
    {
        /* new cycle time */
        l_ctr_value = (BYTE)(P_ctr_value & 0x000000FF);
        
        /* to set cycle time register if new cycle time setting >= 4 */
        if(l_ctr_value >=4 )
        {
            *asPWMCtrlStruct[p_ctr_group].pwm_cycle_time_reg = l_ctr_value;
        }
    }
}

/**
 * ****************************************************************************
 * configuring CTRx, PCFSR, prescaler clock source select group high/low and
 * clock prescaler register.
 *
 * @return
 * l_real_frequence_setting, 0xFFFF --> fail, other --> ok
 *
 * @parameter
 * p_channel_x, PWM_Channel_0 | PWM_Channel_1 | PWM_Channel_2 | PWM_Channel_3
 *              PWM_Channel_4 | PWM_Channel_5 | PWM_Channel_6 | PWM_Channel_7
 * p_Prescaler, PWM_PrescalerC4 | PWM_PrescalerC6 | PWM_PrescalerC7
 * P_channel_frequence, 1 ~ 10000 ==> ( 0.01 ~ 100 Hz) (100 s ~ 10m s)
 *
 * @note
 * 1. clock source always ( 32.768 KHz )
 * 2. always check return value "l_real_frequence_setting" != 0xFFFF
 *    to make sure setting is ok.
 * 3. only support up to three PWM channel have respective frequency setting,
 * if the number of PWM channel greater than 3 please share frequency setting.
 * ****************************************************************************
 */
WORD PWM_Channelx_Register_Clock_Prescaler_Low_Frequency(
    BYTE p_channel_x, BYTE p_Prescaler, DWORD P_channel_frequence)
{
    DWORD l_pwm_cpr;
    DWORD l_real_frequence_setting;
    DWORD l_index;
    DWORD l_deviation;
    DWORD l_pcs_shift;
    DWORD l_pcs_mask;

    /* pre-set setting fail */
    l_real_frequence_setting = 0xFFFF;

    if((p_channel_x <
        (sizeof(asPWMStruct)/sizeof(sPWMStruct)))&&
        ((P_channel_frequence>0)&&
        (P_channel_frequence<=10000)))
    {
        /* deviation for frequency setting */
        l_deviation = (P_channel_frequence/100)+1;

        /* CTR default index */
        l_index=0xFF;

        /* to caculate ctr and cpr */
        while(l_index)
        {
            l_pwm_cpr =
                PWM_EC_Frequence_32_768K/((l_index+1)*P_channel_frequence);

            if(l_pwm_cpr)
            {
                l_pwm_cpr--;
            }

            l_real_frequence_setting =
                PWM_EC_Frequence_32_768K/((l_index+1)*(l_pwm_cpr+1));


            if(l_real_frequence_setting>P_channel_frequence)
            {
                if((l_real_frequence_setting%P_channel_frequence)<l_deviation)
                {
                    break;
                }
            }
            else
            {
                if((P_channel_frequence%l_real_frequence_setting)<l_deviation)
                {
                    break;
                }
            }

            l_index--;

            if(l_index<PWM_Duty_Cycle_CTRx_Min)
            {
                l_real_frequence_setting = 0xFFFF;
                break;
            }
        }

        /* frequency setting is OK */
        if(l_real_frequence_setting!=0xFFFF)
        {
            //
            // parameter "p_Prescaler" is valid
            //
            if(p_Prescaler< (sizeof(asPWMCtrlStruct)/sizeof(sPWMCtrlStruct)))
            {
                /*
                 * pin to alt function
                 * *asPWMStruct[p_channel_x].pwm_ctrl_pin = ALT;
                 * to use "PWM_Channelx_Enable" function
                 */
                
                /* inhibit pwm */
                CLEAR_MASK(ZTIER, BIT1);

                /* set cycle time register */
                *asPWMCtrlStruct[p_Prescaler].pwm_cycle_time_reg = l_index;

                /* clock frequency select register = 32.7689 KHz */
                *asPWMCtrlStruct[p_Prescaler].pwm_pcfsr_reg &=
                    ~asPWMCtrlStruct[p_Prescaler].pwm_pcfsr_ctrl_bit0;

                /*
                 * bit shift for "Prescaler Clock Source Select Group" register
                 */
                l_pcs_shift = (p_channel_x%4)*2;
                
                l_pcs_mask = (p_Prescaler+1) << l_pcs_shift;

                /* prescaler clock select */
                CLEAR_MASK(*asPWMStruct[
                    p_channel_x].pwm_prescaler_clock_source_select_reg,
                    (BIT(l_pcs_shift)+BIT(l_pcs_shift+1)));

                *asPWMStruct[
                    p_channel_x].pwm_prescaler_clock_source_select_reg |=
                    (BYTE)l_pcs_mask;

                /* clock prescaler register msb */
                *asPWMCtrlStruct[p_Prescaler].pwm_cpr_reg_msb =
                    (BYTE)((l_pwm_cpr & 0x0000FF00) >> 8);

                /* clock prescaler register lsb */
                *asPWMCtrlStruct[p_Prescaler].pwm_cpr_reg_lsb =
                    (BYTE)((l_pwm_cpr & 0x000000FF));

                /* pwm clock counter enable */
                SET_MASK(ZTIER, BIT1);
            }
            else
            {
                /*
                 * parameter "p_Prescaler" isn't valid
                 * to return fail
                 */
                l_real_frequence_setting = 0xFFFF;
            }
        }
    }
    
    return((WORD)l_real_frequence_setting);
}

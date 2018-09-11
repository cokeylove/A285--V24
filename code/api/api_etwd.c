/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_etwd.c
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
 * ETWD Controlling Table
 * ****************************************************************************
 */
const sETWD_Reg asETWD_Reg[] = 
{
    /* ET[x]CTRL,     ET[x]PSR, ET[x]CNTLLR, ET[x]CNTLHR, ET[x]CNTLH2R, */
    { ETWD_ctrl_none, &ET2PSR,  &ET2CNTLLR,  &ET2CNTLHR,  &ET2CNTLH2R,
        &IER7,  &ISR7,&IELMR7,  Int_ET2Intr },  /* ExternalTimer_2 */
 	{ &ET3CTRL,       &ET3PSR,  &ET3CNTLLR,  &ET3CNTLHR,  &ET3CNTLH2R,
        &IER19, &ISR19,&IELMR19, Int_ET3Intr },  /* ExternalTimer_3 */
 	{ &ET4CTRL,       &ET4PSR,  &ET4CNTLLR,  &ET4CNTLHR,  &ET4CNTLH2R,
        &IER19, &ISR19,&IELMR19, Int_ET4Intr },  /* ExternalTimer_4 */
    { &ET5CTRL,       &ET5PSR,  &ET5CNTLLR,  &ET5CNTLHR,  &ET5CNTLH2R,
        &IER19, &ISR19,&IELMR19, Int_ET5Intr },  /* ExternalTimer_5 */
    { &ET6CTRL,       &ET6PSR,  &ET6CNTLLR,  &ET6CNTLHR,  &ET6CNTLH2R,
        &IER19, &ISR19,&IELMR19, Int_ET6Intr },  /* ExternalTimer_6 */
    { &ET7CTRL,       &ET7PSR,  &ET7CNTLLR,  &ET7CNTLHR,  &ET7CNTLH2R,
        &IER19, &ISR19,&IELMR19, Int_ET7Intr },  /* ExternalTimer_7 */
    { &ET8CTRL,       &ET8PSR,  &ET8CNTLLR,  &ET8CNTLHR,  &ET8CNTLH2R,
        &IER10, &ISR10,&IELMR10, Int_ET8Intr },  /* ExternalTimer_8 */
};

/**
 * ****************************************************************************
 * To enable external timer [x], (24bit count-down timer)
 *
 * @return
 *
 * @parameter
 * p_et_selection, ExternalTimer_2 || ExternalTimer_5
 *                 ExternalTimer_6 || ExternalTimer_8
 * p_et_time, 1 ~ 86400000 (ms)
 *
 * @note
 * never use external timer 3, 4, and 7. (for kernel use)
 *
 * ****************************************************************************
 */
void Enable_External_Timer_x(BYTE p_et_selection, DWORD p_et_time)
{
    DWORD l_time_out_ms;
    BYTE l_psr;

    /* valid setting */
    if((p_et_time!=0x00)&&(p_et_time<=86400000))
    {
        /* External Timer Prescaler Select 32 Hz */
        if(p_et_time>16000000)
        {
            l_psr = ETPS_32_Hz;
            l_time_out_ms = (p_et_time / 3125)*100;
        }
        /* External Timer Prescaler Select 1.024 KHz */
        else if(p_et_time>500000)
        {
            l_psr = ETPS_1_024_KHz;
            l_time_out_ms = (p_et_time / 1000)*1024;
        }
        /* External Timer Prescaler Select 32.768 KHz */
        else
        {
            l_psr = ETPS_32_768_KHz;
            l_time_out_ms = (p_et_time * 3276)/100;
        }

        /* external timer 3 ~ 8 */
        if(p_et_selection!=ExternalTimer_2)
        {
            /* To enable timer first */
            *asETWD_Reg[p_et_selection].ETWD_ctrl = (ET_3_8_RST+ET_3_8_EN);
        }

        /* Prescaler */
        *asETWD_Reg[p_et_selection].ETWD_psr = l_psr;

        /* To enable a "time_out_ms" timer */
        *asETWD_Reg[p_et_selection].ETWD_cntlh2r = (BYTE)(l_time_out_ms>>16);
        *asETWD_Reg[p_et_selection].ETWD_cntlhr = (BYTE)(l_time_out_ms>>8);
        *asETWD_Reg[p_et_selection].ETWD_cntllr = (BYTE)(l_time_out_ms);

        /* external timer 3 ~ 8 */
        if(p_et_selection!=ExternalTimer_2)
        {
            SET_MASK((*asETWD_Reg[p_et_selection].ETWD_ctrl), ET_3_8_EN);
        }
    }
}

void Enable_External_Timer_Int_x(BYTE p_et_selection)
{
	/* Edge-triggered, and rising-edge-triggered, 0 --> 1 */
	SET_MASK(*asETWD_Reg[USBTimer].ETWD_intc_edge,asETWD_Reg[USBTimer].ETWD_intc_ctrl);
	
	/* Write to clear external timer 3 interrupt */
	SET_MASK(*asETWD_Reg[USBTimer].ETWD_intc_isr,asETWD_Reg[USBTimer].ETWD_intc_ctrl);
	
	/* Enable external timer 3 interrupt */
	SET_MASK(*asETWD_Reg[USBTimer].ETWD_intc_ier,asETWD_Reg[USBTimer].ETWD_intc_ctrl);
}

/**
 * ****************************************************************************
 * To disable external timer [x], (24bit count-down timer)
 *
 * @return
 *
 * @parameter
 * p_et_selection, ExternalTimer_2 || ExternalTimer_5
 *                 ExternalTimer_6 || ExternalTimer_8
 *
 * @note
 * never use external timer 3, 4, and 7. (for kernel use)
 *
 * ****************************************************************************
 */
void Disable_External_Timer_x(BYTE p_et_selection)
{
    BYTE l_read_clear;

    /* external timer 2, no timer control register */
    if(p_et_selection!=ExternalTimer_2)
    {
        /* Disable timer */
        CLEAR_MASK((*asETWD_Reg[p_et_selection].ETWD_ctrl), ET_3_8_EN);
        /* read clear */
        l_read_clear = *asETWD_Reg[p_et_selection].ETWD_ctrl;
    }

    /* external timer 3 ~ 8, */
     CLEAR_MASK((*asETWD_Reg[p_et_selection].ETWD_intc_ier),
        asETWD_Reg[p_et_selection].ETWD_intc_ctrl);

    *asETWD_Reg[p_et_selection].ETWD_intc_isr =
        asETWD_Reg[p_et_selection].ETWD_intc_ctrl;
}

/**
 * ****************************************************************************
 * To check external timer [x] has counted down to zero or not.
 * If external timer [x] has counted down to zero,
 * return "ExternalTimer_TC" and R/C ETxTC bit.
 *
 * @return
 * ExternalTimer_TC, external timer has counted down to zero.
 * ExternalTimer_NoTC, external timer counting.
 *
 * @parameter
 * p_et_selection, ExternalTimer_2 || ExternalTimer_5
 *                 ExternalTimer_6 || ExternalTimer_8
 *
 * ****************************************************************************
 */
BYTE Check_External_Timer_x_Terminal_Count(BYTE p_et_selection)
{
    #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx
    if(IS_MASK_SET(*asETWD_Reg[p_et_selection].ETWD_intc_isr,
        asETWD_Reg[p_et_selection].ETWD_intc_ctrl))
    {
        *asETWD_Reg[p_et_selection].ETWD_intc_isr =
            asETWD_Reg[p_et_selection].ETWD_intc_ctrl;
        return(ExternalTimer_TC);
    }
    else
    {
        return(ExternalTimer_NoTC);
    }

    #else /* #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx #else */
    BYTE l_read_clear;
    if(IS_MASK_SET((*asETWD_Reg[p_et_selection].ETWD_ctrl), ET_3_8_TC))
    {
        /* read clear */
        l_read_clear = *asETWD_Reg[p_et_selection].ETWD_ctrl;
        return(ExternalTimer_TC);
    }
    else
    {
        return(ExternalTimer_NoTC);
    }

    #endif /* #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx #endif */
}

/**
 * ****************************************************************************
 * To enable external watchdog timer.
 *
 * @return
 *
 * @parameter
 * p_wdt_time, 1000 ~ 1800000 (ms)
 *
 * ****************************************************************************
 */
void Enable_External_WDT(DWORD p_wdt_time)
{
    BYTE l_psr;
    DWORD l_time_out_ms;
    
    if(p_wdt_time>=1000)
    {
        /* valid setting, 1sec ~ 30min */
        if(p_wdt_time<=1800000)
        {
            if(p_wdt_time>60000)
            {
                /* External Timer Prescaler Select 32 Hz */
                l_psr = ETPS_32_Hz;
                l_time_out_ms = (p_wdt_time / 3125)*100;
            }
            else if(p_wdt_time>1800)
            {
                /* External Timer Prescaler Select 1.024 KHz */
                l_psr = ETPS_1_024_KHz;
                l_time_out_ms = (p_wdt_time / 1000)*1024;
            }
            else
            {
                /* External Timer Prescaler Select 32.768 KHz */
                l_psr = ETPS_32_768_KHz;
                l_time_out_ms = (p_wdt_time * 3276)/100;
            }

            /* Prescaler Register */
            ET1PSR = l_psr;

            /* To enable external timer 1 is necessary. */
            ET1CNTLHR = 0xFF;
            ET1CNTLLR = 0xFF;

            /* Select clock after prescaler of the external timer 1 */
            SET_MASK(ETWCFG, (EWDSRC+EWDKEYEN));
            
            /* External WDT can be stopped by setting EWDSCEN bit */
            SET_MASK(ETWCTRL, EWDSCMS);
            
            /* External WDT is counting */
            CLEAR_MASK(ETWCTRL, EWDSCEN);

            /* External WDT Counter High */
            EWDCNTHR = (BYTE)(l_time_out_ms>>8);

            /* External WDT Counter Low */
            EWDCNTLR = (BYTE)(l_time_out_ms);
        }
    }
}

/**
 * ****************************************************************************
 * To re-start external watchdog timer
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Reset_External_WDT(void)
{
    /*
     * External WDT is re-started (touched) if 5Ch is written to this register.
     * Writing with other values causes an External Watchdog Reset.
     * This function is enabled by EWDKEYEN bit.
     */
    EWDKEYR = ExternalWDTKeyRestart;
}

/**
 * ****************************************************************************
 * To disable external watchdog timer
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Disable_External_WDT(void)
{
    /* External WDT Stop Count Enable */
    SET_MASK(ETWCTRL, EWDSCEN);

    /*
     * External WDT is re-started (touched) if 5Ch is written to this register.
     * Writing with other values causes an External Watchdog Reset.
     * This function is enabled by EWDKEYEN bit.
     */
    EWDKEYR = ExternalWDTKeyRestart;
}


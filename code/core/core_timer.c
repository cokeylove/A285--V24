/* 
 * ****************************************************************************
 * core_timer.c
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/**
 * ****************************************************************************
 * To enable external timer 4 "time_out_ms"
 *
 * @return
 *
 * @parameter
 * time_out_ms
 *
 * ****************************************************************************
 */
void Enable_ETimer_T(DWORD time_out_ms)
{
    DWORD l_time_out_ms;
    BYTE l_read_clear;

    if(time_out_ms==0)
    {
        /* minimum 1 ms */
        time_out_ms = 1;
    }

    l_time_out_ms = time_out_ms*33;

    /* To enable timer first for timer counter setting */
    ET4CTRL = ET_3_8_EN;

    /* Prescaler 32.768KHz */
    ET4PSR = ETPS_32_768_KHz;

    /* To enable a "time_out_ms" timer */
    ET4CNTLH3R = (BYTE)(l_time_out_ms>>24);           //cwy1218
    ET4CNTLH2R = (BYTE)(l_time_out_ms>>16);
    ET4CNTLHR = (BYTE)(l_time_out_ms>>8);
    ET4CNTLLR = (BYTE)(l_time_out_ms);

    #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx
    SET_MASK(IELMR19, Int_ET4Intr);
    ISR19 = Int_ET4Intr;
    #endif

    /* Disable timer */
    ET4CTRL = 0x00;

    /* R/C */
    l_read_clear = ET4CTRL;

    /* To reset timer */
    ET4CTRL = ET_3_8_RST;

    /* To enable timer */
    ET4CTRL = ET_3_8_EN;
}

/**
 * ****************************************************************************
 * To disable external timer 4
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Stop_ETimer_T(void)
{
    BYTE l_read_clear;

    /* Disable timer */
    ET4CTRL = 0x00;

    /* RC */
    l_read_clear = ET4CTRL;
    
    #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx
    /* Write to clear isrx */
    ISR19 = Int_ET4Intr;
    #endif
}

/**
 * ****************************************************************************
 * To check external timer 4 has counted down to zero or not.
 *
 * @return
 * ExTimerOverflow, external timer has counted down to zero.
 * ExTimerNoOverflow, external timer counting
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE Check_ETimer_T_Overflow(void)
{    
    #if DEF_TIMER_OVERFLOW_FLAG_USE_ISRx
    /* external timer 4 has counted down to zero. */
    if(IS_MASK_SET(ISR19, Int_ET4Intr))
    {
        /* Write to clear isrx */
        ISR19 = Int_ET4Intr;
        return(ExTimerOverflow);
    }
    else
    {
        return(ExTimerNoOverflow);
    }
    #else
    /* external timer 4 has counted down to zero. */
    if(IS_MASK_SET(ET4CTRL, ET_3_8_TC))
    {
        return(ExTimerOverflow);
    }
    else
    {
        return(ExTimerNoOverflow);
    }
    #endif
}

/**
 * ****************************************************************************
 * external timer 3 and 7 init
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Init_Timers(void)
{
    /* external timer 3 */
    SET_MASK(ET3CTRL, ET_3_8_EN);  

    /* clock source 32.768KHz */
    ET3PSR = ETPS_32_768_KHz;   /* Prescaler 32.768KHz */
    ET3CNTLH3R = 0x00;          
    ET3CNTLH2R = 0x00;          /* To enable a 1ms timer */
    ET3CNTLHR = 0x00;
    ET3CNTLLR = 32;             /* 1ms of 32.768KHz */

    SET_MASK(ET3CTRL, ET_3_8_EN);  

    /* Edge-triggered, and rising-edge-triggered, 0 --> 1 */
    SET_MASK(IELMR19, Int_ET3Intr);

    /* Write to clear external timer 3 interrupt */
    ISR19 = Int_ET3Intr;

    /* Enable external timer 3 interrupt */
    SET_MASK(IER19, Int_ET3Intr);


    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    /* external timer 7 */
    SET_MASK(ET7CTRL, ET_3_8_EN);  

    /* clock source 32.768KHz */
    ET7PSR = ETPS_32_768_KHz;   /* Prescaler 32.768KHz */
    ET7CNTLH2R = 0x00;          /* To enable a 1ms timer */
    ET7CNTLHR = 0x00;
    ET7CNTLLR = 32;             /* 1.22ms of 32.768KHz */

    SET_MASK(ET7CTRL, ET_3_8_EN);  

    /* Edge-triggered, and rising-edge-triggered, 0 --> 1 */
    SET_MASK(IELMR19, Int_ET7Intr);
    ISR19 = Int_ET7Intr;        /* Write to clear external timer 7 interrupt */
    SET_MASK(IER19, Int_ET7Intr);   /* Enable external timer 7 interrupt */
    #endif
}

/**
 * ****************************************************************************
 * delay 1ms use exteernal timer 4 and disabling all interrupt
 *
 * @return
 *
 * @parameter
 * bMS 1 ~ FFFFFFh ms
 *
 * ****************************************************************************
 */
void Delay1MS(DWORD bMS)
{	
    DisableGlobalInt();

    /* To enable 1ms time-out timer */
    Enable_ETimer_T(bMS);

    /* To wait external timer6 overflow */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        ;
    }
    Stop_ETimer_T();
    EnableGlobalInt();
}

/**
 * ****************************************************************************
 * delay 1ms use exteernal timer 4
 *
 * @return
 *
 * @parameter
 * bMS 1 ~ FFFFFFh ms
 *
 * ****************************************************************************
 */
void DelayXms(DWORD bMS)
{
    /* To enable 1ms time-out timer */
    Enable_ETimer_T(bMS);

    /* To wait external timer6 overflow */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        ;
    }
    Stop_ETimer_T();
    
}

/**
 * ****************************************************************************
 * To disable external timer 3 and 7 interrupt.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Core_Disable_Period_External_Timer_Interrupt(void)
{
    /* Disable interrupt of external timer 3 and 7 */
    CLEAR_MASK(IER19, (Int_ET3Intr+Int_ET7Intr));

    /* W/C ISRx of external timer 3 and 7 */
    ISR19 = (Int_ET3Intr+Int_ET7Intr);
}

/**
 * ****************************************************************************
 * To enable external timer 3 and 7 interrupt.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Core_Enable_Period_External_Timer_Interrupt(void)
{
    /* W/C ISRx of external timer 3 and 7 */
    ISR19 = (Int_ET3Intr+Int_ET7Intr);

    /* Enable interrupt of external timer 3 and 7 */
    SET_MASK(IER19, (Int_ET3Intr+Int_ET7Intr));
}

/**
 * ****************************************************************************
 * Instruction delay Xms function  
 *
 * @return
 *
 * @parameter
 * ms
 *
 * ****************************************************************************
 */
void InstructionDelayXms(int ms)
{
    DWORD l_1ms_counter;
    
    while(ms)
    {
        for(l_1ms_counter=0x00; l_1ms_counter<70; l_1ms_counter++)
        {
            /* Delay 15.26 us */
            WNCKR = 0x00;
        }
        ms--;
    }
}


/* 
 * ****************************************************************************
 *
 *  core_power.c
 *
 *  Copyright (c) 2012-
 *
 *  Created on: 2013/01/10
 *
 *      Author: 
 *
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
 *
 * ****************************************************************************
 */
const FUNCT_PTR_V_V mPowerMFnTb[tpmMaxNo] =
{
    PowerM_Doze,
    PowerM_DeepDoze,
    PowerM_Sleep,
};

/**
 * ****************************************************************************
 * To read variable [RSTStatus] to get EC reset source
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckResetSource(void)
{
	RSTStatus = RSTS;

    /* W/C reset source */
    RSTS |= (BIT0+BIT1);
}

/**
 * ****************************************************************************
 * doze mode
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PowerM_Doze(void)
{
    BYTE   ucTemp;
    
    PLLCTRL = 0x00;         /* PLLCTRL = 00b, only cpu clock gating */
    ucTemp  = PLLCTRL;      /* dummy read */
    __nds32__standby_wake_grant();
}

/**
 * ****************************************************************************
 * deep doze mode
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PowerM_DeepDoze(void)
{
    BYTE   ucTemp;
    
    PLLCTRL = 0x03;         /* PLLCTRL = 11b, no disable pll, gating pll out */
    ucTemp  = PLLCTRL;      /* dummy read */
    __nds32__standby_wake_grant();
}

/**
 * ****************************************************************************
 * sleep mode
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PowerM_Sleep(void)
{
    BYTE   ucTemp;
    
    PLLCTRL = 0x01;         /* PLLCTRL = 01b, disable pll */
    ucTemp  = PLLCTRL;      /* dummy read */
    __nds32__standby_wake_grant();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE PowerM_Dispatch(BYTE type)
{
    #if (DEF_HID_FUSION_SLEEP_MODE == 1)
    if(tpmMaxNo <= type) return  FALSE;

    /* Before Power Save */
    hal_stop_hid_fusion_driver();

    hal_rtos_timer_ctl(FALSE);

    Hook_Enable_Wake_Up_Src(type);

    do
    {
        if(NULL != mPowerMFnTb[type]())
        {
            mPowerMFnTb[type]();
        }
        else
        {
            break;
        }
    }while(FALSE == Hook_Check_Wake_Up_Event());

    /* After Power Save */
    Hook_Disable_Wake_Up_Src(type);
    
    hal_rtos_timer_ctl(TRUE);

    hal_start_hid_fusion_driver();
    #endif
    
    return  TRUE;
}




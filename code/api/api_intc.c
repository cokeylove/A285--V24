/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_intc.c
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
 * INTC Controlling Table
 * ****************************************************************************
 */
const sINTC_Reg asINTC_Reg[] = 
{
    /*  IER[x],     ISR[[x],    IELMR[x],   IPOLR[x] */
 	{   &IER0,      &ISR0,      &IELMR0,    &IPOLR0     },
    {   &IER1,      &ISR1,      &IELMR1,    &IPOLR1     },
    {   &IER2,      &ISR2,      &IELMR2,    &IPOLR2     },
    {   &IER3,      &ISR3,      &IELMR3,    &IPOLR3     },
    {   &IER4,      &ISR4,      &IELMR4,    &IPOLR4     },
    {   &IER5,      &ISR5,      &IELMR5,    &IPOLR5     },
    {   &IER6,      &ISR6,      &IELMR6,    &IPOLR6     },
    {   &IER7,      &ISR7,      &IELMR7,    &IPOLR7     },
    {   &IER8,      &ISR8,      &IELMR8,    &IPOLR8     },
    {   &IER9,      &ISR9,      &IELMR9,    &IPOLR9     },
    {   &IER10,     &ISR10,     &IELMR10,   &IPOLR10    },
    {   &IER11,     &ISR11,     &IELMR11,   &IPOLR11    },
    {   &IER12,     &ISR12,     &IELMR12,   &IPOLR12    },
    {   &IER13,     &ISR13,     &IELMR13,   &IPOLR13    },
    {   &IER14,     &ISR14,     &IELMR14,   &IPOLR14    },
    {   &IER15,     &ISR15,     &IELMR15,   &IPOLR15    },
    {   &IER16,     &ISR16,     &IELMR16,   &IPOLR16    },
    {   &IER17,     &ISR17,     &IELMR17,   &IPOLR17    },
    {   &IER18,     &ISR18,     &IELMR18,   &IPOLR18    },
    {   &IER19,     &ISR19,     &IELMR19,   &IPOLR19    },
};

/**
 * ****************************************************************************
 * The function for enabling interrupt of INTC. (complete flow)
 *
 * @return
 *
 * @parameter
 * interrup_index, 
 * Interrupt_INT0 || Interrupt_INT1 || Interrupt_INT2  || Interrupt_INT3
 * Interrupt_INT4 || Interrupt_INT5 || Interrupt_INT6  || Interrupt_INT7
 * ...
 * Interrupt_INT152 || Interrupt_INT153 || Interrupt_INT154 || Interrupt_INT155
 * Interrupt_INT156 || Interrupt_INT157 || Interrupt_INT158 || Interrupt_INT159
 * interrupt_trigger_mode,
 * INT_Trigger_Mode_Set_LevelHigh || INT_Trigger_Mode_Set_LevelLow
 * INT_Trigger_Mode_Set_RisingEdge || INT_Trigger_Mode_Set_FallingEdge
 *
 * ****************************************************************************
 */
void INTC_Enable_INTx(BYTE interrup_index, BYTE interrupt_trigger_mode)
{
    BYTE l_intc_group;
    BYTE l_intc_ctrl;

    if(interrup_index>Interrupt_INT159)
    {
        /* illegal */
    }
    else
    {
        l_intc_group = interrup_index/8;
        l_intc_ctrl = BIT(interrup_index%8);
        
        /* edge trigger */
        if(IS_MASK_SET(interrupt_trigger_mode, INT_Edge_Trigger))
        {
            SET_MASK(*asINTC_Reg[l_intc_group].INTC_IELMRx, l_intc_ctrl);
        }
        /* level trigger */
        else
        { 
            CLEAR_MASK(*asINTC_Reg[l_intc_group].INTC_IELMRx, l_intc_ctrl);
        }

        if(IS_MASK_SET(interrupt_trigger_mode, INT_Polarity1))
        {
            SET_MASK(*asINTC_Reg[l_intc_group].INTC_IPOLRx, l_intc_ctrl);
        }
        else
        {
            CLEAR_MASK(*asINTC_Reg[l_intc_group].INTC_IPOLRx, l_intc_ctrl);
        }

        /* To clear related bit */
        *asINTC_Reg[l_intc_group].INTC_ISRx = l_intc_ctrl;
        /* To enable interrupt */
        SET_MASK((*asINTC_Reg[l_intc_group].INTC_IERx), l_intc_ctrl);
    }
}

/**
 * ****************************************************************************
 * The function for disabling interrupt and clearing corresponding bit of
 * ISRx of INTC.
 *
 * @return
 *
 * @parameter
 * interrup_index, 
 * Interrupt_INT0 || Interrupt_INT1 || Interrupt_INT2  || Interrupt_INT3
 * Interrupt_INT4 || Interrupt_INT5 || Interrupt_INT6  || Interrupt_INT7
 * ...
 * Interrupt_INT152 || Interrupt_INT153 || Interrupt_INT154 || Interrupt_INT155
 * Interrupt_INT156 || Interrupt_INT157 || Interrupt_INT158 || Interrupt_INT159
 *
 * ****************************************************************************
 */
void INTC_Disable_INTx(BYTE interrup_index)
{
    BYTE l_intc_group;
    BYTE l_intc_ctrl;

    if(interrup_index>Interrupt_INT159)
    {
        /* illegal */
    }
    else
    {
        l_intc_group = interrup_index/8;
        l_intc_ctrl = BIT(interrup_index%8);
        
        /* To disable interrupt */
        CLEAR_MASK((*asINTC_Reg[l_intc_group].INTC_IERx), l_intc_ctrl);

        /* To W/C  related bit of ISRx */
        *asINTC_Reg[l_intc_group].INTC_ISRx = l_intc_ctrl;
    }
}

/**
 * ****************************************************************************
 * The function for clearing corresponding bit of ISRx of INTC only.
 *
 * @return
 *
 * @parameter
 * interrup_index, 
 * Interrupt_INT0 || Interrupt_INT1 || Interrupt_INT2  || Interrupt_INT3
 * Interrupt_INT4 || Interrupt_INT5 || Interrupt_INT6  || Interrupt_INT7
 * ...
 * Interrupt_INT152 || Interrupt_INT153 || Interrupt_INT154 || Interrupt_INT155
 * Interrupt_INT156 || Interrupt_INT157 || Interrupt_INT158 || Interrupt_INT159
 *
 * ****************************************************************************
 */
void INTC_Clear_ISRx(BYTE interrup_index)
{
    BYTE l_intc_group;
    BYTE l_intc_ctrl;

    if(interrup_index>Interrupt_INT159)
    {
        /* illegal */
    }
    else
    {
        l_intc_group = interrup_index/8;
        l_intc_ctrl = BIT(interrup_index%8);
        
        /* To W/C related bit of ISRx */
        *asINTC_Reg[l_intc_group].INTC_ISRx = l_intc_ctrl;
    }
}

/**
 * ****************************************************************************
 * The function for disabling corresponding bit of IERx of INTC only.
 *
 * @return
 *
 * @parameter
 * interrup_index, 
 * Interrupt_INT0 || Interrupt_INT1 || Interrupt_INT2  || Interrupt_INT3
 * Interrupt_INT4 || Interrupt_INT5 || Interrupt_INT6  || Interrupt_INT7
 * ...
 * Interrupt_INT152 || Interrupt_INT153 || Interrupt_INT154 || Interrupt_INT155
 * Interrupt_INT156 || Interrupt_INT157 || Interrupt_INT158 || Interrupt_INT159
 *
 * ****************************************************************************
 */
void INTC_Disable_IERx(BYTE interrup_index)
{
    BYTE l_intc_group;
    BYTE l_intc_ctrl;

    if(interrup_index>Interrupt_INT159)
    {
        /* illegal */
    }
    else
    {
        l_intc_group = interrup_index/8;
        l_intc_ctrl = BIT(interrup_index%8);
        
        /* To disable interrupt */
        CLEAR_MASK((*asINTC_Reg[l_intc_group].INTC_IERx), l_intc_ctrl);
    }
}


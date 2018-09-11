/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_ow.c
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
 * Software OW
 * ****************************************************************************
 */

/**
 * ****************************************************************************
 * Reset OW device function
 *
 * @return
 * FFh, Reset device OK.
 * 00h, Reset device fail.
 *
 * @parameter
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
BYTE SW_OW_Device_Reset(void)
{	
    BYTE report;
    BYTE delay;

    DisableAllInterrupt();

    /* Pre-set 0xFF, OK */
    report = 0xFF;
    
    /* Low 500 us, tRSTL */
    OneWireOutput;

    /* Output low */
    OneWireLow;

    /* Delay > 480 us */
    for(delay=0x00; delay<=34; delay++) 
    {
        /* Delay 15.26 us */
        WNCKR = 0x00;
    }

    OneWireInput;

    /* To wait pin high (120us time out) */
    /* (120us time out) */
    for(delay=0x00; delay<8; delay++)
    {
        /* Delay 15.26 us */
        WNCKR = 0x00;
        
        if(OneWireStatusHigh)
        {
            /* 0xFF, OK */
            report = 0xFF;
            break;
        }
        else
        {
            /* 0x00, Fail */
            report = 0x00;
        }
    }

    if(report == 0xFF)
    {
        /* To wait pin high (240us time out) */
        /* (240us time out) */
        for(delay=0x00; delay<=16; delay++)
        {
            /* Delay 15.26 us */
            WNCKR = 0x00;
            
            if(OneWireStatusLow)
            {
                /* 0xFF, OK */
                report = 0xFF;
                break;
            }
            else
            {
                /* 0x00, Fail */
                report = 0x00;
            }
        }
    }
        
    if(report == 0xFF)
    {
        /* To wait pin high (1ms time out) */
        /* (1ms time out) */
        for(delay=0x00; delay<=66; delay++)
        {
            if(OneWireStatusHigh)
            {
                /* 0xFF, OK */
                report = 0xFF;
                break;
            }
            else
            {
                /* 0x00, Fail */
                report = 0x00;
            }
        }
    }
    
    EnableAllInterrupt();
    return(report);
}

/**
 * ****************************************************************************
 * Write one bit function
 *
 * @return
 *
 * @parameter, 0 or 1
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
void SW_OW_Device_WriteBit(BYTE Psidbit)
{
    BYTE delay;
    
    OneWireOutput;
    OneWireLow;

    if(Psidbit)
    {
        OneWireHigh;
    }
    else
    {
        OneWireLow;
    }

    /* Delay 60us */
    for(delay=0x00; delay<4; delay++) 
    {
        /* Delay 15.26 us */
        WNCKR = 0x00;
    }

    OneWireInput;

    /* Delay 15.26 us */
    WNCKR = 0x00;
}

/**
 * ****************************************************************************
 * Write one byte to OW device.
 *
 * @return
 *
 * @parameter, 00h ~ FFh
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
void SW_OW_Device_WriteByte(BYTE PsidByte)
{
    BYTE index;

    DisableAllInterrupt();
	for(index=0x00;index<8;index++)
	{
		if((PsidByte>>index)&0x01)
		{
		    SW_OW_Device_WriteBit(1);
		}
		else
		{
		    SW_OW_Device_WriteBit(0);
		}
	}
    EnableAllInterrupt();
}

/**
 * ****************************************************************************
 * Read one bit function
 *
 * @return, 0 or 1
 *
 * @parameter
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
BYTE SW_OW_Device_ReadBit(void)
{
	BYTE tempdata;
    BYTE delay;
    
    OneWireOutput;
    OneWireLow;
    _nop_();
    OneWireInput;

    /* Delay 15.26 us */
    WNCKR = 0x00;

    if(OneWireStatusHigh)
    {
        tempdata = 1;
    }
    else
    {
        tempdata = 0;
    }

    /* Delay 60us */
    for(delay=0x00; delay<4; delay++) 
    {
        /* Delay 15.26 us */
        WNCKR = 0x00;
    }

    return(tempdata);
}

/**
 * ****************************************************************************
 * Read one byte from OW device.
 *
 * @return, 00h ~ FFh
 *
 * @parameter
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
BYTE SW_OW_Device_ReadByte(void)
{
    BYTE index;
    BYTE readbit;
    BYTE result;

    DisableAllInterrupt();
	result=0x00;
	for(index=0x00;index<8;index++)
	{
		readbit = SW_OW_Device_ReadBit();
		result|=(readbit<<index);
	}
	EnableAllInterrupt();
    
	return(result);
}

/*
 * ****************************************************************************
 * Hardware OW
 * ****************************************************************************
 */

/**
 * ****************************************************************************
 * OW register setting
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void OW_Init_Registers(void)
{
    /* GPIO B2 alt function */
    GPCRB2 = ALT+PULL_UP;

    /*
     * 0x1E05 , Bit0 = 1
     * CGCTRL3R = 0x01;
     */

    /* 0x16F5 , Bit4 = 1 */
    GCR6 |= BIT4;
}

/**
 * ****************************************************************************
 * Reset OW device function
 *
 * @return
 * 01h, Reset device OK.
 * 00h, Reset device fail.
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE HW_OW_Device_Reset(void)
{
    BYTE result;

    /* Pr-set result is fail */
    result = FALSE;

    /* start reset */
    SET_MASK(OW_CTRL, OW_RST);

    /* To enable 5ms time-out timer */
    Enable_ETimer_T(5);

    /* No overflow, no time out */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        /* One-Wire Done */
        if(IS_MASK_SET(OW_STS, OW_DONE))
        {
            /* Write clear */
            SET_MASK(OW_STS, OW_DONE);

            /* return true */
            result = TRUE;
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWLOW_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWLOW_ERR);
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWHIGH_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWHIGH_ERR);
            break;
        }
    }

    Stop_ETimer_T();

    /* return result */
    return(result);
}

/**
 * ****************************************************************************
 * Write one byte to device.
 *
 * @return
 *
 * @parameter, 00h ~ FFh
 *
 * ****************************************************************************
 */
BYTE HW_OW_Device_WriteByte(BYTE writedata)
{
    BYTE result;
    result = FALSE;

    CLEAR_MASK(OW_MOD, OBMOD);
    OW_DATA = writedata;
    SET_MASK(OW_CTRL, OW_WR);

    /* To enable 5ms time-out timer */
    Enable_ETimer_T(5);

    /* No overflow, no time out */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        /* One-Wire Done */
        if(IS_MASK_SET(OW_STS, OW_DONE))
        {
            /* Write clear */
            SET_MASK(OW_STS, OW_DONE);

            /* return true */
            result = TRUE;
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWLOW_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWLOW_ERR);
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWHIGH_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWHIGH_ERR);
            break;
        }
    }

    Stop_ETimer_T();
    return(result);
}

/**
 * ****************************************************************************
 * Read one byte from device.
 *
 * @return, 00h ~ FFh
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE HW_OW_Device_ReadByte(void)
{
    BYTE result;
    result = 0xFF;

    CLEAR_MASK(OW_MOD, OBMOD);
    SET_MASK(OW_CTRL, OW_RD);

    /* To enable 5ms time-out timer */
    Enable_ETimer_T(5);

    /* No overflow, no time out */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        /* One-Wire Done */
        if(IS_MASK_SET(OW_STS, OW_DONE))
        {
            /* Write clear */
            SET_MASK(OW_STS, OW_DONE);

            /* return true */
            result = OW_DATA;
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWLOW_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWLOW_ERR);
            break;
        }
        else if(IS_MASK_SET(OW_STS, OWHIGH_ERR))
        {
            /* Write clear */
            SET_MASK(OW_STS, OWHIGH_ERR);
            break;
        }
    }

    Stop_ETimer_T();
    return(result);
}


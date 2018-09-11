/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_peci.c
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
 * Global variable (memory address allocation follow project )
 * ****************************************************************************
 */
BYTE   PECI_CRC8;
BYTE   PECI_CompletionCode;

/**
 * ****************************************************************************
 * Init Enable PECI interface function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Init_PECI(void)
{
    GPCRF6 = ALT;

    /* Enable PECI */
    GCR2 |= PECIE;
}

/**
 * ****************************************************************************
 * Init timer1 for interface time-out
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PECI_InitTimeOutTimer(void)
{
    /* To enable 5ms time-out timer */
    Enable_ETimer_T(5);
}

/**
 * ****************************************************************************
 * CRC-8 polynomial
 *
 * @return
 *
 * @parameter
 * sourcebyte
 *
 * ****************************************************************************
 */
void PECI_CalcCRC8(BYTE sourcebyte)
{
    #if PECI_Softwave_AWFCS    
    BYTE temp;
	PECI_CRC8 ^=sourcebyte;
    temp = PECI_CRC8;

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp<<1;
        PECI_CRC8 ^= 0x09;
        PECI_CRC8 ^= temp;
    }
    
    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        PECI_CRC8 ^= temp;
    }
    else
    {
        temp = temp<<1;
        PECI_CRC8 ^= 0x07;
        PECI_CRC8 ^= temp;
    }
    #endif /* #if PECI_Softwave_AWFCS #endif */
}

/**
 * ****************************************************************************
 * Enables the PECI host controller.
 *
 * @return
 *
 * @parameter
 * sourcebyte
 *
 * ****************************************************************************
 */
void PECI_HostEnable(void)
{
    HOCTLR |= (FIFOCLR+FCSERR_ABT+PECIHEN+CONCTRL);
}

/**
 * ****************************************************************************
 * Disable the PECI host controller.
 *
 * @return
 *
 * @parameter
 * sourcebyte
 *
 * ****************************************************************************
 */
void PECI_HostDisable(void)
{
    /* HOCTLR = 0x00; */

    /*
     * Do not disable PECI host controller
     * if PECI host controller already enabling.
     */
    HOCTLR = PECIHEN;
}

/**
 * ****************************************************************************
 * The PECI host controller will perform the desired transaction.
 *
 * @return
 *
 * @parameter
 * control, START || AWFCS_EN
 *
 * ****************************************************************************
 */
void PECI_HostControl(BYTE control)
{
    HOCTLR |= control;
}

/**
 * ****************************************************************************
 * Check PECI interface busy or not
 *
 * @return
 * 0, [PECI_InitTimeOutTimer()] time out
 * 1, OK
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE PECI_CheckHostBusy(void)
{
    BYTE result;

    /* pre-set time out */
    result = 0x00;
    
    PECI_InitTimeOutTimer();    

    while(1)
    {
        /* No overflow */
        if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
        {
            /* Host free */
            if(IS_MASK_CLEAR(HOSTAR, HOBY))
            {
                result = 0x01;
                break;
            }
            /* Host Busy */
            else
            {

            }
        }
        /* time out */
        else
        {
            result = 0x00;
            break;
        }
    }

    Stop_ETimer_T();

    return(result);
}

/**
 * ****************************************************************************
 * Check PECI interface finish or not
 *
 * @return
 * 0, time-out or error
 * 1, finish
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE PECI_CheckHostFinish(void)
{
    BYTE error;
    BYTE status,result;

    /* Pre-set error */
    error = 0x04;
    
    PECI_InitTimeOutTimer();

    /* No overflow */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
	{ 
        status = HOSTAR;

        if(status!=0x00)
        {
            if(IS_MASK_SET(status, FINISH))
            {
                error = 0x00;
                break;
            }
            else if(IS_MASK_SET(status, RD_FCS_ERR))
            {
                error = 0x01;
                break;
            }
            else if(IS_MASK_SET(status, WR_FCS_ERR))
            {
                error = 0x01;
                break;
            }
            else if(IS_MASK_SET(status, EXTERR))
            {
                /* Reset PECI interface */
                SET_MASK(RSTC4,RPECI);
                error = 0x02;
                break;
            }
            else if(IS_MASK_SET(status, BUSERR))
            {
                /* Reset PECI interface */
                SET_MASK(RSTC4,RPECI);
                error = 0x02;
                break;
            }
            else if(IS_MASK_SET(status, TEMPERR))
            {
                error = 0x03;
                break;
            }
        }
    }

    if(error!=0x00)
    {
        /* error */
        result=0x00;
    }    
    else
    {
        /* finish */
        result=0x01;
    }

    Stop_ETimer_T();
    
    return(result);
}

/**
 * ****************************************************************************
 * The function of clearing Host Status Register
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ResetPECIStatus(void)
{
    HOSTAR = 0xFE;
}

/**
 * ****************************************************************************
 * The function of clearing Host Status Register
 *
 * @return
 * 1, The targeted address of processor is able to respond
 * 0, no respond
 *
 * @parameter
 * (1) addr : The address of processor
 * (2) ReadLen read length always 0
 * (3) WriteLen Write length always 0
 *
 * ****************************************************************************
 */
BYTE PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done;
    done = 0x00;
    
    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = WriteLen;
    HORDLR = ReadLen;
    
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

/**
 * ****************************************************************************
 * PECI get DIB command
 *
 * @return
 * 1, done
 * 0, error
 *
 * @parameter
 * (1) addr : The address of processor
 * (2) *ReadData : the start address of variable to save data
 * (3) ReadLen read length always 8
 * (4) WriteLen Write length always 1
 *
 * ****************************************************************************
 */
BYTE PECI_GetDIB(BYTE addr, BYTE *ReadData, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, index;
    done = 0x00;
    
    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = WriteLen;
    HORDLR = ReadLen;
    HOCMDR = PECI_CMD_GetDIB;
    
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            for(index=0x00;index<HORDLR;index++)
            {
                *(ReadData+index)=HORDDR;
            }
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

/**
 * ****************************************************************************
 * PECI get temperature command
 *
 * @return
 * 1, done
 * 0, error
 *
 * @parameter
 * (1) addr, The address of processor
 * (2) *ReadData, the start address of variable to save data
 * (3) Domain, 0 or 1. 0, Domain 0, 1 : Domain 1
 * (4) ReadLen read length always 2
 * (5) WriteLen Write length always 1
 *
 * ****************************************************************************
 */
BYTE PECI_GetTemp(
    BYTE addr, BYTE *ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done;
    done = 0x00;

    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = WriteLen;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_GetTemp+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_GetTemp;
    }

    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            *ReadData=HORDDR;
            *(++ReadData)=HORDDR;
            done = 0x01;
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

/**
 * ****************************************************************************
 * Read to the package configuration space (PCS) within the processor
 *
 * @return
 * 1, done
 * 0, error
 *
 * @parameter
 * (1) addr : The address of processor
 * (2) *ReadData : the start address of variable to save data
 * (3) Domain : 0 or 1. 0 : Domain 0, 1 : Domain 1
 * (4) Retry 0 or 1
 * (5) Index
 * (6) LSB of parameter
 * (7) MSB of parameter
 * (8) ReadLen read length 2 or 3 or 5
 * (9) WriteLen write length 5
 *
 * ****************************************************************************
 */
BYTE PECI_RdPkgConfig(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry,
    BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = WriteLen;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdPkgConfig+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdPkgConfig;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }
    
    HOWRDR = Index;
    HOWRDR = LSB;
    HOWRDR = MSB;
        
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            if(HORDLR!=0x00)
            {
                for(cunt=0x00;cunt<HORDLR;cunt++)
                {
                    if(cunt==0x00)
                    {
                        PECI_CompletionCode = HORDDR;
                    }
                    else
                    {
                        *(ReadData+cunt-1)=HORDDR;
                    }
                }

                if(PECI_CompletionCode == PECI_CC_Valid)
                {
                    done = 0x01;
                }
                else
                {
                    done = 0x00;
                }
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

/**
 * ****************************************************************************
 * Write to the package configuration space (PCS) within the processor
 *
 * @return
 * 1, done
 * 0, error
 *
 * @parameter
 * (1) addr : The address of processor
 * (2) *WriteData : the start address of variable to wirte data
 * (3) Domain : 0 or 1. 0 : Domain 0, 1 : Domain 1
 * (4) Retry 0 or 1
 * (5) Index
 * (6) LSB of parameter
 * (7) MSB of parameter
 * (8) ReadLen read length 1
 * (9) WriteLen write length 0x07 or 0x08 or 0x0A
 *
 * ****************************************************************************
 */
BYTE PECI_WrPkgConfig(BYTE addr, BYTE *WriteData, BYTE Domain, BYTE Retry,
    BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_CompletionCode = 0x00;
    PECI_HostEnable();
    
    #if PECI_Softwave_AWFCS
    PECI_CRC8 = 0x00;
    #else
    PECI_HostControl(AWFCS_EN);
    #endif
    
    HOTRADDR = addr;
    PECI_CalcCRC8(addr);
    HOWRLR = WriteLen;
    PECI_CalcCRC8(WriteLen);
    HORDLR = ReadLen;
    PECI_CalcCRC8(ReadLen);

    if(Domain<2)
    {
        HOCMDR = PECI_CMD_WrPkgConfig+Domain;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig+Domain);
    }
    else
    {
        HOCMDR = PECI_CMD_WrPkgConfig;
        PECI_CalcCRC8(PECI_CMD_WrPkgConfig);
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
        PECI_CalcCRC8((PECI_HostID<<1)+Retry);
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
        PECI_CalcCRC8(PECI_HostID<<1);
    }

    HOWRDR = Index;
    PECI_CalcCRC8(Index);
    HOWRDR = LSB;
    PECI_CalcCRC8(LSB);
    HOWRDR = MSB;
    PECI_CalcCRC8(MSB);

    for(cunt=0x00; cunt<(WriteLen-6); cunt++)
    {
        HOWRDR = *(WriteData+cunt);
        PECI_CalcCRC8(*(WriteData+cunt));
    }

    #if PECI_Softwave_AWFCS
    /* Inverted MSb of preliminary FCS reslut */
    PECI_CRC8 ^= 0x80;
    HOWRDR = PECI_CRC8;
    #endif
         
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            PECI_CompletionCode = HORDDR;
            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}

/**
 * ****************************************************************************
 * Read to Model Specific Registers function
 *
 * @return
 * 1, done
 * 0, error
 *
 * @parameter
 * (1) addr : The address of processor
 * (2) *ReadData : the start address of variable to save data
 * (3) Domain : 0 or 1. 0 : Domain 0, 1 : Domain 1
 * (4) Retry 0 or 1
 * (5) ProcessorID
 * (6) LSB of parameter
 * (7) MSB of parameter
 * (8) ReadLen read length 0x02 or 0x03 or 0x05 or 0x09
 9) WriteLen write length 0x05
 *
 * ****************************************************************************
 */
BYTE PECI_RdIAMSR(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry,
    BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
    BYTE done, cunt;
    done = 0x00;
    
    PECI_HostEnable();
    HOTRADDR = addr;
    HOWRLR = WriteLen;
    HORDLR = ReadLen;
    if(Domain<2)
    {
        HOCMDR = PECI_CMD_RdIAMSR+Domain;
    }
    else
    {
        HOCMDR = PECI_CMD_RdIAMSR;
    }

    if(Retry<2)
    {
        HOWRDR = (PECI_HostID<<1)+Retry;
    }
    else
    {
        HOWRDR = (PECI_HostID<<1);
    }
    
    HOWRDR = ProcessorID;
    HOWRDR = LSB;
    HOWRDR = MSB;
        
    if(PECI_CheckHostBusy())
    {
        PECI_HostControl(START);
        if(PECI_CheckHostFinish())
        {
            for(cunt=0x00;cunt<HORDLR;cunt++)
            {
                if(cunt==0x00)
                {
                    PECI_CompletionCode = HORDDR;
                }
                else
                {
                    *(ReadData+cunt-1)=HORDDR;
                }
            }

            if(PECI_CompletionCode == PECI_CC_Valid)
            {
                done = 0x01;
            }
            else
            {
                done = 0x00;
            }
        }
    }

    PECI_HostDisable();
    ResetPECIStatus();
    return(done);
}


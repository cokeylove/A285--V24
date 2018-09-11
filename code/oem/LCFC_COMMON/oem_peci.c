/*-----------------------------------------------------------------------------
* TITLE: API_PECI.C
*
* Author : Dino
*
* History : 
* ------------------------------
* Version : 1.00
* ------------------------------
*
* Note : To link [api_xxx.o] if related api function be used.
*
*---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

//*****************************************************************************
// Global variable (memory address allocation follow project )
//*****************************************************************************
BYTE   PECI_CRC8;
BYTE   PECI_CompletionCode;

//*****************************************************************************
//
// Init Enable PECI interface function
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void Init_PECI(void)
{
    GPCRF6 = ALT;                      
    GCR2 |= PECIE;      // Enable PECI
}

//*****************************************************************************
//
// Init timer1 for interface time-out
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void PECI_InitTimeOutTimer(void)
{
    Enable_ETimer_T(5);         // To enable 5ms time-out timer
}

//*****************************************************************************
//
// CRC-8 polynomial
//
//  parameter :
//      sourcebyte : 
//
//  return :
//      none
//
//*****************************************************************************
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
#endif    
}

//*****************************************************************************
//
// Enables the PECI host controller.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostEnable(void)
{
    HOCTLR |= (FIFOCLR+FCSERR_ABT+PECIHEN+CONCTRL);
}

//*****************************************************************************
//
// Disable the PECI host controller.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostDisable(void)
{
    //HOCTLR = 0x00;
    HOCTLR = PECIHEN;   // Do not disable PECI host controller if PECI host controller already enabling.
}

//*****************************************************************************
//
// The PECI host controller will perform the desired transaction.
//
//  parameter :
//      control : START || AWFCS_EN
//
//  return :
//      none
//
//*****************************************************************************
void PECI_HostControl(BYTE control)
{
    HOCTLR |= control;
}

//*****************************************************************************
//
// Check PECI interface busy or not
//
//  parameter :
//      none
//
// return : 
//          0 : [PECI_InitTimeOutTimer()] time out
//          1 : OK
//
//*****************************************************************************
BYTE PECI_CheckHostBusy(void)
{
    BYTE result;

    result = 0x00;      // pre-set time out
  
    PECI_InitTimeOutTimer();    

    while(1)
    {
        if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)    // No overflow
        {
            if(IS_MASK_CLEAR(HOSTAR, HOBY)) // Host free
            {
                result = 0x01;
                break;
            }
            else    // Host Busy
            {

            }
        }
        else        // time out
        {
            result = 0x00;
            break;
        }
    }

    Stop_ETimer_T();
	
    return(result);
}

//*****************************************************************************
//
// Check PECI interface finish or not
//
//  parameter :
//      none
//
// return : 
//          0 : time-out or error
//          1 : finish
//
//*****************************************************************************
BYTE PECI_CheckHostFinish(void)
{
    BYTE error;
    BYTE status,result;
    error = 0x04;           // Pre-set error


    PECI_InitTimeOutTimer();

    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)     // No overflow
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
                SET_MASK(RSTC4,RPECI);  // Reset PECI interface
                error = 0x02;
                break;
            }
            else if(IS_MASK_SET(status, BUSERR))
            {
                SET_MASK(RSTC4,RPECI);  // Reset PECI interface
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
        result=0x00;    // error
    }    
    else
    {
        result=0x01;    // finish
    }

    Stop_ETimer_T();
   
    return(result);
}

//*****************************************************************************
//
// The function of clearing Host Status Register
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void ResetPECIStatus(void)
{
    HOSTAR = 0xFE;
}

//*****************************************************************************
//
// PECI Ping function
//
//  parameter : 
//              (1) addr : The address of processor
//              (2) ReadLen read length always 0
//              (3) WriteLen Write length always 0
//  return : 
//              1 : The targeted address of processor is able to respond
//              0 : no respond
//
//*****************************************************************************
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

//*****************************************************************************
//
// PECI get temperature command
//
//  parameter : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) ReadLen read length always 8
//              (4) WriteLen Write length always 1
//  return : 
//              1 : done
//              0 : error
//
//*****************************************************************************
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

//*****************************************************************************
//
// PECI get temperature command
//
//  parameter : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) ReadLen read length always 2
//              (5) WriteLen Write length always 1
//  return : 
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_GetTemp(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen)
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

//*****************************************************************************
//
// Read to the package configuration space (PCS) within the processor
//
//  parameter : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 2 or 3 or 5
//              (9) WriteLen write length 5
//  return : 
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_RdPkgConfig(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
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

//*****************************************************************************
//
// Write to the package configuration space (PCS) within the processor
//
//  Input : 
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to wirte data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 1
//              (9) WriteLen write length 0x07 or 0x08 or 0x0A
//  return : 
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_WrPkgConfig(BYTE addr, BYTE *WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
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
    PECI_CRC8 ^= 0x80;          // Inverted MSb of preliminary FCS reslut
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

//*****************************************************************************
//
// Read to Model Specific Registers function
//
//  Input : 
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1¡C 
//                           0 : Domain 0, 1 : Domain 1¡C
//              (4) Retry   0 or 1
//              (5) ProcessorID
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 0x02 or 0x03 or 0x05 or 0x09
//              (9) WriteLen write length 0x05
//  return : 
//              1 : done
//              0 : error
//
//*****************************************************************************
BYTE PECI_RdIAMSR(BYTE addr, BYTE *ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
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

// ----------------------------------------------------------------------------
// The function of PECI3.0 Read the Maximum Allowed Processor Temperature
// ----------------------------------------------------------------------------
void PECI_ReadCPUTJMAX(void)
{
	if(PECI_RdPkgConfig(PECI_CPU_ADDR, PECIReadBuffer, PECI_Domain_0, 0, PECI_Index_TTR, 0x00, 0x00, 0x05, 0x05))
	{
		ReadCPUTjmaxCUNT = 0x00;
		CPUTjmax = PECIReadBuffer[2];
	}
	else
	{
		if(++ReadCPUTjmaxCUNT>=PECI_Read_TjmaxTO)
		{
		    ReadCPUTjmaxCUNT = 0x00;
		    CPUTjmax = PECI_CPU_Tjmax;										// Set default value if re-try PECI_Read_TjmaxTO times
		}
	}
}

// ----------------------------------------------------------------------------
// The function of PECI3.0 CPU temperature read
// ----------------------------------------------------------------------------
void PECI_ReadCPUTemp(void)
{
    WORD cputemp;
    BYTE tempoffset;
    if(PECI_GetTemp(PECI_CPU_ADDR, PECIReadBuffer, PECI_Domain_0, 2, 1)) 		// Read OK
    {
        cputemp = (PECIReadBuffer[1]<<8)+PECIReadBuffer[0];						// MSB+LSB
        cputemp = (~cputemp)+1;                             							// 2's complement
        tempoffset = cputemp>>6;                            							// 1/64 degrees centigrade
        if(tempoffset<=CPUTjmax)        										// PECI thermal reading temperature readings are                
        {                               												// not reliable at temperatures above Tjmax
            PECI_CPU_temp = CPUTjmax-tempoffset;  								// Save temperature
        }
        else
        {
        }
    }
    else       																// Read fail
    {
    																		//RamDebug(0xB1);
    }
}

//-----------------------------------------------------------------------------
// Function Name : ReadCPUTemp
//-----------------------------------------------------------------------------
void ReadCPUTemp(void) 
{
	if(CPUTjmax==0x00)         							// Need to read CPU Tjmax
	{
		PECI_ReadCPUTJMAX();
	}
	else
	{
		PECI_ReadCPUTemp();     						// Use PECI interface to read cpu temperature
	}
}



/* 
 * ****************************************************************************
 *
 *  core_smbus.c
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/13
 *
 *      Author: ITE00580, Dino Li
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
 * SMBus registers table
 * ****************************************************************************
 */

uchar_8 gucI2CSensorErrStatus = 0;

const sSMBus asSMBus[]=
{
	{ &HOCTL_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &D0REG_A, &D1REG_A,	&HOBDB_A,
        &IER1,	&ISR1,  Int_SMBUS0,   &PECERC_A},
	{ &HOCTL_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &D0REG_B, &D1REG_B,	&HOBDB_B,
        &IER1,	&ISR1,  Int_SMBUS1,   &PECERC_B},
	{ &HOCTL_C, &TRASLA_C, &HOCMD_C, &HOSTA_C, &D0REG_C, &D1REG_C,	&HOBDB_C,
        &IER2,	&ISR2,	Int_SMBUS2,   &PECERC_C},
};

#ifndef DEF_CUSTOM_SMBUS_RESET_TABLE
const sResetSMBusS asResetSMBusS[]=
{
    #ifdef __IT8386__ELPM__
    { &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPCRF2, &GPCRF3, &GPDRF,
        (BIT2+BIT3),   &SMBus1RecoverCunt },
    #else
	{ &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPCRB3, &GPCRB4, &GPDRB,
        (BIT3+BIT4),   &SMBus1RecoverCunt },
    #endif
	{ &HOSTA_B, &HOCTL2_B, &SMBPCTL_B, &GPCRC1, &GPCRC2, &GPDRC,
        (BIT1+BIT2),   &SMBus2RecoverCunt },
	{ &HOSTA_C, &HOCTL2_C, &SMBPCTL_C, &GPCRF6, &GPCRF7, &GPDRF,
        (BIT6+BIT7),   &SMBus3RecoverCunt },
}; 
#endif

#if SMB_Channel_AUTO_detect
const sSMBChannel SMBDETCH[]= {SMB_AUTO_Channel};

BYTE SMB_CH_DET (BYTE Addr,BYTE Channel)
{
  BYTE i;

  if (Addr == SMB_BAT_Exception_Addr){
   
    return Channel;
  }

  for(i=0x00;i<sizeof(SMBDETCH)/sizeof(sSMBChannel);i++){
    if (Addr == SMBDETCH[i].SMBAddr){
		
      return SMBDETCH[i].SMBChannel;
    }
  }
  
  return Channel;
}
#endif  //SMB_Channel_AUTO_detect


/**
 * ****************************************************************************
 * short delay for smbus reset
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void DelayInact(void) 
{
    BYTE index;
    BYTE counter;
   for(index=0;index<200;index++)
   {
        counter++;
   }
}

/**
 * ****************************************************************************
 * The function of reseting SMbus 
 *
 * @return
 *
 * @parameter
 * Channel, 0, 1, or 2
 *
 * ****************************************************************************
 */
void ResetSMBus(BYTE Channel)
{
	
	#ifdef PECI_Support
	if(Channel==SMbusCh3)
	{
	   return;
	}
   	#endif
   
	if( Channel >= SMbusCh4 )
	{
		i2c_ch_reset(Channel-SMbusCh4);
		return;
	}
    SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_KILL);
    CLEAR_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_KILL);

    /* clear bits */
	*asResetSMBusS[Channel].SMBusSTA = 0xFE;

    /* SMCLK2 is located on GPC7 */
	if(CheckSMCLK2PinSwitch(Channel)==0x01)
	{
        GPCRC7 = OUTPUT;
	}
    else
    /* SMCLK2 is located on GPF6 */
    {
        *asResetSMBusS[Channel].SMBusPin0 = OUTPUT;
    }
	*asResetSMBusS[Channel].SMBusPin1 = OUTPUT;
    
	DelayInact();

    /* SMCLK2 is located on GPC7 */
    if(CheckSMCLK2PinSwitch(Channel)==0x01)
	{ 
        SET_MASK(GPDRC, BIT7);  /* clock pin */
        SET_MASK(GPDRF, BIT7);  /* data pin */
    }
    else
    /* SMCLK2 is located on GPF6 */
    {
        *asResetSMBusS[Channel].GPIOReg |= asResetSMBusS[Channel].GPIOData;
    }
	DelayInact();

    /* SMCLK2 is located on GPC7 */
    if(CheckSMCLK2PinSwitch(Channel)==0x01)
	{ 
        GPCRC7 = ALT;
    }
    /* SMCLK2 is located on GPF6 */
    else
    {
        *asResetSMBusS[Channel].SMBusPin0 = ALT;
    }
	*asResetSMBusS[Channel].SMBusPin1 = ALT;

	*asResetSMBusS[Channel].SMBusCTL2 = 0x02;
	*asResetSMBusS[Channel].SMBusPinCTL = 0x03;
	DelayInact();
	*asResetSMBusS[Channel].SMBusPinCTL = 0x07;
	*asResetSMBusS[Channel].SMBusCTL2 = 0x11;
	#ifdef SMBusServiceCenterFunc
	OemResetSmbusVariable(Channel);
	#endif
}

/**
 * ****************************************************************************
 * SMbus read byte/word and write byte/word function	 
 *
 * @return
 * 1, OK. 0, fail.
 *
 * @parameter
 * Channel, 0, 1, or 2
 * Protocol
 * Addr
 * Comd
 * *Var
 * PECSupport
 *
 * ****************************************************************************
 */
BYTE bRWSMBus(
    BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,BYTE *Var,BYTE PECSupport)
{	
	BYTE error;
    BYTE status;
    BYTE resutl;

	#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel); 
	#endif //SMB_Channel_AUTO_detect   

	if( Channel >= SMbusCh4 )
	{
		
		if(FALSE == i2c_drv_wait_bus_idle(Channel - SMbusCh4) )
		{
				//TODO
				return FALSE;
				
		}
		/* Clear CRC variable */
        SMBCRC8_A = 0x00;
		//bITETestVar3 = 0;

		if( PECSupport )
		{
		 	
			

        	/* Write byte function */
	    	if(Protocol&0x80)
	    	{	
	       
                /* To calculate crc8 */
                CalcCRC8(Addr);
                /* To calculate crc8 */
                CalcCRC8(Comd);             
              	/* To calculate crc8 */
                CalcCRC8(*Var);
             	/* write word function */
		    	if(Protocol==SMbusWW)
		    	{
                    /* To calculate crc8 */
                    CalcCRC8(*(Var+0x01));
                }
				//bITETestVar3 = SMBCRC8_A;
	    	}
		}

		if ((Protocol == SMbusRW) || (Protocol == SMbusWW))
			resutl = SMBus_DEF_Transfer(Channel,Protocol,Addr,Comd,Var,sizeWORD,PECSupport,SMBCRC8_A);
		else 
			resutl = SMBus_DEF_Transfer(Channel,Protocol,Addr,Comd,Var,sizeBYTE,PECSupport,SMBCRC8_A);
		return resutl;
	}
	
    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==
        SMBus_CanNotUse)
    {
        /* SMBus interface can't be used. */
        resutl = FALSE;
    }
    else
    {
        /* Pre-set error */
        error = 0xEE;

        /* Pre-set result is fail */
        resutl = FALSE;

        /* Clear CRC variable */
        SMBCRC8_A=0x00;

        /* Write byte function */
	    if(Protocol&0x80)
	    {	
	        if(PECSupport)    
            {
                /* To calculate crc8 */
                CalcCRC8(Addr);
            }

            /* set address with writing bit */
		    *asSMBus[Channel].SMBusADR = Addr;
	        if(PECSupport)    
            {
                /* To calculate crc8 */
                CalcCRC8(Comd);
            }

            /* Set command */
	        *asSMBus[Channel].SMBusCMD=Comd;
		    Protocol&=(~0x80);
            if(PECSupport)    
            {
                /* To calculate crc8 */
                CalcCRC8(*Var);
            }

            /* set data1 */
		    *asSMBus[Channel].SMBusData0 = *Var;

            /* write word function */
		    if(Protocol==SMbusRW)
		    {
		        if(PECSupport)    
                {
                    /* To calculate crc8 */
                    CalcCRC8(*(Var+0x01));
                }
                /* set data2 */
			    *asSMBus[Channel].SMBusData1 = *(Var+0x01);
		    }
	    }
        /* Read function */
	    else
	    {
            /* set address with reading bit */
		    *asSMBus[Channel].SMBusADR=Addr|0x01;

	        /* Set command */
            *asSMBus[Channel].SMBusCMD=Comd;
	    }

	    /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
	    if(PECSupport)
        {
            *asSMBus[Channel].SMBusPEC=SMBCRC8_A;
            /* Start transaction */
	        *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT|HOCTL_PEC_EN);
        }
        else
        {
	        /* Start transaction */
	        *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);
        }

        /* To enable 26ms time-out timer */
        Enable_ETimer_T(26);
	    while(1)
	    {
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[Channel].SMBusSTA;
                if(IS_MASK_SET(status,
                    (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+
                    HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
                {
                    if(IS_MASK_SET(status,
                        (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(Channel);
                        }
                        error = 0xEE;
                    }
                    else
                    {
                        /* Only Finish Interrupt bit is set. */
                        error = 0x00;
                    }
                    break;
                }
            }
            else
            {
                ResetSMBus(Channel);
                error = 0xEE;
                break;
            }
	    }

        Stop_ETimer_T();

        /* Fail */
	    if(error == 0xEE)
	    {
		    resutl = FALSE;
	    }
        /* OK */
        else
        {
	        if (((*asSMBus[Channel].SMBusADR & 0x01) != 0x00)) 
	        {
                /* read data1 */
		        *Var = *asSMBus[Channel].SMBusData0;
		        if(Protocol==SMbusRW)
		        {
		            /* read data2 */
			        *(Var+0x01) = *asSMBus[Channel].SMBusData1;
		        }
	        }
            resutl = TRUE;
        }

        /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
    }
    
	return(resutl);
}

/**
 * ****************************************************************************
 * Read SMbus block function	 
 *
 * @return
 * 1, OK. 0, fail.
 *
 * @parameter
 * Channel, 0, 1, or 2
 * Protocol
 * Addr
 * Comd
 * *Var
 *
 * ****************************************************************************
 */
BYTE bRSMBusBlock(BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,BYTE *Var,BYTE i2cSize)
{
	BYTE ack;
    BYTE bcount;
    BYTE status;

	#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel); 
	#endif //SMB_Channel_AUTO_detect       

	if( Channel >= SMbusCh4 )
	{
		if(FALSE == i2c_drv_wait_bus_idle(Channel - SMbusCh4) )
		{
				//TODO
				return FALSE;
				
		}
     if(i2cSize != 0x00)
       {ack = SMBus_DEF_Transfer(Channel,Protocol,Addr,Comd,Var,i2cSize,FALSE,0xFF);}
     else
       {ack = SMBus_DEF_Transfer(Channel,Protocol,Addr,Comd,Var,sizeSMBRBK,FALSE,0xFF);}
		return ack;
	}

    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==
        SMBus_CanNotUse)
    {
        /* SMBus interface can't be used. */
        ack = FALSE;
    }
    else
    {
        /* pr-set flag */
	    ack = FALSE;

        /* clear byte counter */
	    bcount = 0x00;
	
        /* set address with reading bit */
	    *asSMBus[Channel].SMBusADR=Addr|0x01;

	    /* Set command */
	    *asSMBus[Channel].SMBusCMD=Comd;

	    /* clear bits */
        *asSMBus[Channel].SMBusSTA=0xFE;

        /* Start transaction */
        *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);

        /* To enable 26ms time-out timer */
        Enable_ETimer_T(26);

        while(1)
        {
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[Channel].SMBusSTA;
                if(IS_MASK_SET(status,
                    (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR )))
                {
                    if(IS_MASK_SET(status,
                        (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(Channel);
                        }
    	                ack = FALSE;
                        break;
                    }
                    /* Byte Done Status bit or finish flag is set. */
                    else
                    {
                        /* Byte Done Status bit is set. */
                        if(IS_MASK_SET(status, HOSTA_BDS))
                        {
                            /* get first data */
        	                *Var = *asSMBus[Channel].SMBusBData;
        	                if(bcount ==0x00)
        	                {
                                /* get the data of byte count */
        		                bcount = *asSMBus[Channel].SMBusData0;
        		                Hook_GetSMBusReadBlockByteCount(bcount);
								
								if(bcount==0x00)	// alex + for read battery some special cmd
								{
									ResetSMBus(Channel);
									ack = FALSE;
                        			break;
								}
								else if(bcount==0x01)
								{
									/* set last byte flag */
        		                	SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);
								}
								else if(bcount>32)
								{
									bcount=32;
								}

        	                }	
                    
        	                bcount--;
        	                Var++;
            
        	                if(bcount == 0x01)
        	                {
                                /* set last byte flag */
        		                SET_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);
        	                }

        	                if(bcount == 0x00)
        	                {

                            }

                            /* clear bits for next byte */
                            *asSMBus[Channel].SMBusSTA=0xFC;
                        }
                        else
                        {
                            /* finish */
                            ack = TRUE;

                            /* SMBus read block done. */
                            break;
                        }
                    }
                }
            }
            else
            {
    		    ResetSMBus(Channel);
    		    ack = FALSE;
                break;
            }
        }

        Stop_ETimer_T();

        /* clear last byte flag */
	    CLEAR_MASK(*asSMBus[Channel].SMBusCTL,HOCTL_LABY);

        /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
    }

	return(ack);	
}

/**
 * ****************************************************************************
 * Write SMbus block function	 
 *
 * @return
 * 1, OK. 0, fail.
 *
 * @parameter
 * Channel, 0, 1, or 2
 * Protocol
 * Addr
 * Comd
 * *Var
 * ByteCont
 * PECsupport
 *
 * ****************************************************************************
 */
BYTE bWSMBusBlock(
    BYTE Channel,
    BYTE Protocol,
    BYTE Addr,
    BYTE Comd,
    BYTE *Var,
    BYTE ByteCont,
    BYTE PECsupport)
{	
	BYTE ack;
    BYTE BCTemp;
    BYTE status;

	#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel); 
	#endif //SMB_Channel_AUTO_detect   

	SMBCRC8_A=0x00;

	if( Channel >= SMbusCh4 )
	{
		if(FALSE == i2c_drv_wait_bus_idle(Channel - SMbusCh4) )
		{
				//TODO
				return FALSE;
				
		}
		if(PECsupport)
        {   
        	//BCTemp=ByteCont;
            SMBCRC8_A=0x00;
            CalcCRC8(Addr);
            CalcCRC8(Comd);
            CalcCRC8(ByteCont);
			for(BCTemp = 0; BCTemp < ByteCont; BCTemp++)
			{
            	CalcCRC8(*(Var+BCTemp));
			}
        }
		ack = SMBus_DEF_Transfer(Channel,Protocol,Addr,Comd,Var,ByteCont,PECsupport,SMBCRC8_A);
		return ack;
	}

    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==
        SMBus_CanNotUse)
    {
        /* SMBus interface can't be used. */
        ack = FALSE;
    }
    else
    {
        /* pre-set flag */
	    ack = FALSE;

        /* clear byte count */
	    BCTemp = 0x00;

        if(PECsupport)
        {   
            SMBCRC8_A=0x00;
            CalcCRC8(Addr);
            CalcCRC8(Comd);
            CalcCRC8(ByteCont);
            CalcCRC8(*Var);
        }
    
	    Protocol&=(~0x80);

	    /* set address with writing flag */
	    *asSMBus[Channel].SMBusADR=Addr;

	    /* Set command */
        *asSMBus[Channel].SMBusCMD=Comd;

        /* set byte count */
        *asSMBus[Channel].SMBusData0=ByteCont;
        /* sync byte count */
        BCTemp=ByteCont;
	    /* set first data */
        *asSMBus[Channel].SMBusBData=*Var;

	    /* clear bits */
        *asSMBus[Channel].SMBusSTA=0xFE;
	    if(PECsupport)
        {   
            /* Start transaction */
            *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT|HOCTL_PEC_EN);
        }
        else
        {
            /* Start transaction */
            *asSMBus[Channel].SMBusCTL=(Protocol|HOCTL_SRT);
        }

        /* To enable 26ms time-out timer */
        Enable_ETimer_T(26);

        while(1)
        {
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[Channel].SMBusSTA;
                if(IS_MASK_SET(status,
                    (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR )))
                {
                    if(IS_MASK_SET(status,
                        (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(Channel);
                        }
    	                ack = FALSE;
                        break;
                    }
                    /* Byte Done Status bit or finish flag is set. */
                    else
                    {
                        /* byte done */
                        if(IS_MASK_SET(status, HOSTA_BDS))
                        {
        	                BCTemp--;
    					    /* point to next address of variable */
        	                Var++;

        	                if(BCTemp != 0x00)
        	                {
                                if(PECsupport)
                                {
                                    *asSMBus[Channel].SMBusBData=*Var;
                                    CalcCRC8(*Var);

                                    /* last byte of byte count */
                                    if(BCTemp==0x01)
                                    {
                                        *asSMBus[Channel].SMBusPEC = SMBCRC8_A;
                                    }
                                }
                                else
                                {
        		                    *asSMBus[Channel].SMBusBData=*Var;
                                }
        	                }
                            /* write block transaction done */
                            else
                            {

                            }
                            /* clear bits for next byte */
                            *asSMBus[Channel].SMBusSTA=0xFC;
                        }
                        else
                        {
                            /* finish */
        	                ack = TRUE;
        	                break;
                        }
                    }
                }
            }
            else
            {
		        ResetSMBus(Channel);
		        ack = FALSE;
                break;
            }
        }
        
        Stop_ETimer_T();
        /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
    }
    
	return(ack);
} 

/**
 * ****************************************************************************
 * The function of SMbus send byte	 
 *
 * @return
 * 1, OK. 0, fail.
 *
 * @parameter
 * Channel, 0, 1, or 2
 * Addr
 * SData
 *
 * ****************************************************************************
 */
BYTE bSMBusSendByte(BYTE Channel,BYTE Addr,BYTE SData)
{
	BYTE error;
    BYTE result;
    BYTE status;

	#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel); 
	#endif //SMB_Channel_AUTO_detect   

	if( Channel >= SMbusCh4 )
	{
		if(FALSE == i2c_drv_wait_bus_idle(Channel - SMbusCh4) )
		{
				//TODO
				return FALSE;
				
		}
		result = SMBus_DEF_Transfer(Channel,SMbusSBC,Addr,SData,NULL,0x00,FALSE,0xFF);
		return result;
	}

    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==
        SMBus_CanNotUse)
    {
        /* SMBus interface can't be used. */
        result = FALSE;
    }
    else
    {
        /* Pre-set error */
        error = 0xEE;

        /* Pre-set result is fail */
        result = FALSE;

        /* set address with writing bit */
	    *asSMBus[Channel].SMBusADR = Addr;

        /* Set command */
	    *asSMBus[Channel].SMBusCMD = SData;

        /* clear bits */
	    *asSMBus[Channel].SMBusSTA = 0xFE;

        /* Start transaction */
	    *asSMBus[Channel].SMBusCTL = (0x04|HOCTL_SRT);

        /* To enable 26ms time-out timer */
        Enable_ETimer_T(26);

        while(1)
	    { 
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[Channel].SMBusSTA;
                if(IS_MASK_SET(status,
                    (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+
                    HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
                {
                    if(IS_MASK_SET(status,
                        (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(Channel);
                        }
                        error = 0xEE;
                    }
                    /* Only Finish Interrupt bit is set. */
                    else
                    {
                        error = 0x00;
                    }
                    break;
                }
            }
            /* time-out */
            else
            {
                ResetSMBus(Channel);
                error = 0xEE;
                break;
            }
	    }

        Stop_ETimer_T();
    
	    if(error == 0xEE)
	    {
		    result = FALSE;
	    }
        else
        {
            result = TRUE;
        }

	    /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
    }
    
	return(result);
}

/**
 * ****************************************************************************
 * The function of SMbus receive byte	 
 *
 * @return
 * 1, OK. 0, fail.
 *
 * @parameter
 * Channel, 0, 1, or 2
 * Addr
 * *Var
 *
 * ****************************************************************************
 */
BYTE bSMBusReceiveByte(BYTE Channel,BYTE Addr,BYTE *Var)
{
	BYTE error;
    BYTE result;
    BYTE status;

	#if SMB_Channel_AUTO_detect    
    Channel = SMB_CH_DET(Addr, Channel);
	#endif //SMB_Channel_AUTO_detect     

	if( Channel >= SMbusCh4 )
	{
		if(FALSE == i2c_drv_wait_bus_idle(Channel - SMbusCh4) )
		{
				//TODO
				return FALSE;
				
		}
		result = SMBus_DEF_Transfer(Channel,SMbusRBC,Addr,0x00,Var,0x01,FALSE,0xFF);
		return result;
	}

    if(CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn)==
        SMBus_CanNotUse)
    {
        /* SMBus interface can't be used. */
        result = FALSE;
    }
    else
    {
        /* Pre-set error */
        error = 0xEE;

        /* Pre-set result is fail */
        result = FALSE;

        /* set address with writing bit */
	    *asSMBus[Channel].SMBusADR = (Addr|0x01);

        /* clear bits */
	    *asSMBus[Channel].SMBusSTA = 0xFE;

        /* Start transaction */
	    *asSMBus[Channel].SMBusCTL = (0x04|HOCTL_SRT);

        /* To enable 26ms time-out timer */
        Enable_ETimer_T(26);

        while(1)
	    { 
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                /* Read SMBus Host Status */
                status = *asSMBus[Channel].SMBusSTA;
                if(IS_MASK_SET(status,
                    (HOSTA_FINTR+HOSTA_DVER+HOSTA_BSER+
                    HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE )))
                {
                    if(IS_MASK_SET(status,
                        (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        HOSTA_NACK+HOSTA_TMOE)))
                    {
                        if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        {
                            ResetSMBus(Channel);
                        }
                        error = 0xEE;
                    }
                    /* Only Finish Interrupt bit is set. */
                    else
                    {
                        error = 0x00;
                    }
                    break;
                }
            }
            /* time-out */
            else
            {
                ResetSMBus(Channel);
                error = 0xEE;
                break;
            }
	    }
	
        Stop_ETimer_T();
     
	    if(error == 0xEE)
	    {
		    result = FALSE; 
	    }	
        else
        {
            /* read data */
	        *Var = *asSMBus[Channel].SMBusData0;
	        result = TRUE; 
        }
	    /* clear bits */
	    *asSMBus[Channel].SMBusSTA=0xFE;
    }
    
	return(result);
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
void CalcCRC8(BYTE sourcebyte)
{
    BYTE temp;
	SMBCRC8_A ^=sourcebyte;
    temp = SMBCRC8_A;

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        SMBCRC8_A ^= temp;
    }
    else
    {
        temp = temp<<1;
        SMBCRC8_A ^= 0x09;
        SMBCRC8_A ^= temp;
    }
    
    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        SMBCRC8_A ^= temp;
    }
    else
    {
        temp = temp<<1;
        SMBCRC8_A ^= 0x07;
        SMBCRC8_A ^= temp;
    }
}

/**
 * ****************************************************************************
 * The function of Checking SMbus clock and data pins are both high	
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE CheckSMBusFree(BYTE channel)
{
    BYTE pinstatus;

    pinstatus = *asResetSMBusS[channel].SMBusPinCTL;

    /* Check BIT0 and BIT1 */
    if((pinstatus&=0x03)==0x03)
    {
        return(SMBus_Free);
    }
    else
    {
        return(SMBus_Busy);
    }
}

/**
 * ****************************************************************************
 * The function of Checking SMbus pins are all alt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE CheckSMBusInterfaceIsALT(BYTE channel)
{
    /* SMCLK2 is located on GPC7 */
    if(CheckSMCLK2PinSwitch(channel)==0x01)
    {
        if(((GPCRC7 & 0xFB)!=0x00)||((GPCRF7 & 0xFB)!=0x00))
        {
            return(SMBusPin_GPIO);
        }
        else
        {
            return(SMBusPin_ALT);
        }
    }
    else
    {
        if(((*asResetSMBusS[channel].SMBusPin0 & 0xFB)!=0x00)||
            ((*asResetSMBusS[channel].SMBusPin1 & 0xFB)!=0x00))
        {
            return(SMBusPin_GPIO);
        }
        else
        {
            return(SMBusPin_ALT);
        }
    }
}

/**
 * ****************************************************************************
 * The function of Checking SMbus pins are all alt
 *
 * @return
 * SMBus_CanUse or SMBus_CanNotUse
 *
 * @parameter
 * channel
 * accesstype
 *
 * ****************************************************************************
 */
BYTE CheckSMBusInterfaceCanbeUse(BYTE channel, BYTE accesstype)
{
    BYTE checksmbus;

    /* Pre-set smbus interface can be used */
    checksmbus = SMBus_CanUse;

	#ifdef PECI_Support
    if(channel==SMbusCh3)                       // If PECI function enable, return.
    {
        checksmbus = SMBus_CanNotUse;           // Interface can't be used
        return(checksmbus);                     // return result
    }
    #endif

    /* SMBus interface is GPIO function */
    if(CheckSMBusInterfaceIsALT(channel)==SMBusPin_GPIO)
    {
        /* Interface can't be used */
        checksmbus = SMBus_CanNotUse;
    }
    /* SMBus interface is ALT function */
    else
    {
        if(accesstype==SMBus_AccessType_Fucn)
        {
            #ifdef SMBusServiceCenterFunc
            /* Check smbus is in used or not. */
            WaitSMBusCHxFree(channel);
            #endif
        }

        if(CheckSMBusFree(channel)==SMBus_Busy)
        {
            /* Interface can't be used */
            checksmbus = SMBus_CanNotUse;
            if( (*asResetSMBusS[channel].recoverCunt)++ >SMBus_BusyRecover)
            {
                ResetSMBus(channel);
            }
        }
        else
        {
            *asResetSMBusS[channel].recoverCunt=0x00;
            /* smbus interface can be used */
            checksmbus = SMBus_CanUse;
        }
    }

    #ifdef DEF_HOOK_FUNC_SMBUS_CAN_NOT_USE
    if (checksmbus == SMBus_CanNotUse)
    {
        hook_func_smbus_can_not_use(channel);
    }
    #endif

    /* return result */
    return(checksmbus);
}

/**
 * ****************************************************************************
 * To init. SMBus setting
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Core_Init_SMBus(void)
{
    /*
     * Reset SMBus interface if bus clock pin or data pin is low greater than
     * SMBus_BusyRecover times.
     */
    SMBus_BusyRecover = 20;
}

/**
 * ****************************************************************************
 * To check SMCLK2 pin switch
 *
 * @return
 * 00h, SMCLK2 is located on GPF6.
 * 01h, SMCLK2 is located on GPC7.
 *
 * @parameter
 * p_channel, SMBus channel 0, 1, 2
 *
 * ****************************************************************************
 */
BYTE CheckSMCLK2PinSwitch(BYTE p_channel)
{
    BYTE status;

    /* pre-set SMCLK2 is located on GPF6. */
    status = 0x00;

    /* SMCLK2 Pin Switch (SMCLK2PS) */
    if(IS_MASK_SET(GCR7, BIT7) && (p_channel==0x02))
    {
        /* SMCLK2 is located on GPC7 */
        status = 0x01;
    }
    else
    {
        /* SMCLK2 is located on GPF6 */
        status = 0x00;
    }

    return(status);
}

//============================================================================
// I2C protocol
//============================================================================

/**
 * ****************************************************************************
 * I2C-compatible read command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device address with r/w bit
 * p_length, read length
 * *p_data_buffer, data buffer address
 * P_first_word_is_read_length
 *
 * ****************************************************************************
 */
BYTE I2C_Read(BYTE p_channel, BYTE p_address, WORD p_length,
                BYTE *p_data_buffer, BYTE P_first_word_is_read_length)
{
    BYTE status;
    BYTE ack;
    BYTE l_read_length[2];
    BYTE l_read_length_index;

    /* pre-set fail. */
    ack = FALSE;

    if(P_first_word_is_read_length)
    {
        l_read_length_index = 0x02;
    }

    /* illegal parameter */
    if(p_length == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)
            ==SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* block read address */
            *asSMBus[p_channel].SMBusADR = (p_address|0x01);

            /* only read one byte */
            if(p_length==0x01)
            {
                /* set last byte flag */
                SET_MASK(*asSMBus[p_channel].SMBusCTL,HOCTL_LABY);
            }

            /* i2c read, Start transaction */
            *asSMBus[p_channel].SMBusCTL = 0x5C;

            
            /* To enable 26ms time-out timer */
        	Enable_ETimer_T(26);

        	while(1)
        	{
            	if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            	{
                	/* Read SMBus Host Status */
                	status = *asSMBus[p_channel].SMBusSTA;
                	/* any status bit */
                	if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+
                                HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR)))
                	{
                    	/* any error bit */
                    	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                                                HOSTA_NACK+HOSTA_TMOE)))
                    	{
                        	/* bus fail or error */
                        	if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        	{
                            	ResetSMBus(p_channel);
                        	}                    
                        	break;
                    	}
                    	/* Byte Done or finish Status bit is set */
                    	else
                    	{
                        	if(IS_MASK_SET(status,HOSTA_BDS))
                        	{
                            	/* read data */
                            	if(p_length)
                            	{
                                	*p_data_buffer = *asSMBus[p_channel].SMBusBData;
                                	p_data_buffer++;
                                	p_length--;

                                	if(P_first_word_is_read_length)
                                	{
                                    	if(l_read_length_index)
                                    	{
                                        	l_read_length_index--;
                                        	l_read_length[l_read_length_index] =
                                            *(p_data_buffer -1);
                                        	if(l_read_length_index==0)
                                        	{
                                            	p_length = l_read_length[1] +
                                                	(l_read_length[0]*256);
                                            	if(p_length>2)
                                            	{
                                                	p_length-=2;
                                            	}
                                        	}
                                    	}
                                	}
                            	}
                            	/* next cycle is last byte */
                            	if(p_length==0x01)
                            	{
                                	/* set last byte flag */
            	                	SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                    HOCTL_LABY);
                            	}
                            	/* clear status bits for next byte */
                            	*asSMBus[p_channel].SMBusSTA=0xFC; 

								/* reset timer */
                        	}
                        	/* finish */
                        	else
                        	{
        	                	if(p_length==0)
        	                	{
                                	/* i2c read success */
        	                    	ack = TRUE;
        	                	}
                            	break;
                        	}
                    	}
                	}
            	}
				else
				{
					/* 26ms time-out and no any status bit is set. */
					ResetSMBus(p_channel);
                	break;
				}
          	}


          	/* disable I2C mode */
          	*asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* w/c status */
          	*asSMBus[p_channel].SMBusSTA=0xFE;

            /* timer  stop */
            Stop_ETimer_T();
        }
    }
    return(ack);
}

/**
 * ****************************************************************************
 * I2C-compatible write command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device address with r/w bit
 * p_length, write length
 * *p_data_buffer, data buffer address
 *
 * ****************************************************************************
 */
BYTE I2C_Write(BYTE p_channel, BYTE p_address, BYTE p_length,
                BYTE *p_data_buffer)
{
    BYTE status;
    BYTE ack;

    /* pre-set fail. */
    ack = FALSE;

    /* illegal parameter */
    if(p_length == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)
            ==SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* device address */
            *asSMBus[p_channel].SMBusADR = p_address;

            /* first byte */
            *asSMBus[p_channel].SMBusBData = *p_data_buffer;

            /* Start transaction */
            *asSMBus[p_channel].SMBusCTL=0x5C;

            /* To enable 26ms time-out timer */
        	Enable_ETimer_T(26);

        	while(1)
        	{
            	if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            	{
                	/* Read SMBus Host Status */
                	status = *asSMBus[p_channel].SMBusSTA;

                	/* any status bit */
                	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    	HOSTA_NACK+HOSTA_TMOE+HOSTA_BDS+HOSTA_FINTR )))
                	{
                    	/* any error bit */
                    	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        	HOSTA_NACK+HOSTA_TMOE)))
                    	{
                        	/* bus fail or error */
                        	if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        	{
                            	ResetSMBus(p_channel);
                        	}
                        	ack = FALSE;
                        	break;
                    	}
                    	/* Finish or Byte Done bit is set. */
                    	else
                    	{
                        	/* finish bit */
                        	if(IS_MASK_SET(status,HOSTA_FINTR))
                        	{

								if(p_length==0)
        	                	{
                                	/* i2c write success */
        	                    	ack = TRUE;
        	                	}
                            	break;
                        	}

                        	/* Byte Done */
                        	else
                        	{
                            	if(p_length)
                            	{
                                	p_length--;

                                	if(p_length)
                                	{
                                    	/* to send next byte */
                                    	p_data_buffer++;
                                    	*asSMBus[p_channel].SMBusBData =
                                        	*p_data_buffer;
                                	}
                                	else
                                	{
                                    	/* i2c write success and wait finish bit */

                                    	/* disable I2C mode */
                                    	*asResetSMBusS[p_channel].SMBusCTL2 = 0x11;
                                	}
                                	*asSMBus[p_channel].SMBusSTA=0xFC;
                            	}
                            	/* reset timer */
                       		}
                    	}
                	}
    	    	}
				else
				{
					/* 26ms time-out and no any status bit is set. */
					ResetSMBus(p_channel);
                	break;
				}
        	}

            /* disable I2C mode */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* w/c status */
            *asSMBus[p_channel].SMBusSTA=0xFE;

            /* timer  stop */
            Stop_ETimer_T();
        }
    }

    return(ack);
}

/**
 * ****************************************************************************
 * I2C-compatible write to read command
 *
 * @return
 * 0, fail
 * 1, OK
 *
 * @parameter
 * p_channel, i2c channel selection        0 ~ 2
 * p_address, device addres
 * p_wlength, write length
 * *p_wdata_buffer, data buffer address for write
 * p_rlength, read length
 * *p_rdata_buffer, data buffer address for read
 *
 * ****************************************************************************
 */
BYTE I2C_Write_to_Read(BYTE p_channel, BYTE p_address, BYTE p_wlength,
    BYTE *p_wdata_buffer, WORD p_rlength, BYTE *p_rdata_buffer,
    BYTE P_first_word_is_read_length)
{
    BYTE status;
    BYTE ack;
    BYTE l_read_length[2];
    BYTE l_read_length_index;

    if(P_first_word_is_read_length)
    {
        l_read_length_index = 0x02;
    }

    /* pre-set fail. */
    ack = FALSE;

    /* illegal parameter */
    if(p_wlength == 0x00)
    {
        ack = FALSE;
    }
    else
    {
        /* to check interface can be used or not */
        if(CheckSMBusInterfaceCanbeUse(p_channel, SMBus_AccessType_Fucn)==
            SMBus_CanNotUse)
        {
            /* SMBus interface can't be used. */
            ack = FALSE;
        }
        else
        {
            /* I2C enable */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x13;

            /* device address (write) */
            *asSMBus[p_channel].SMBusADR = (p_address & 0xFE);

            /* first byte */
            *asSMBus[p_channel].SMBusBData = *p_wdata_buffer;

            /* Start transaction */
            *asSMBus[p_channel].SMBusCTL=0x5C;

            /* To enable 26ms time-out timer */
        	Enable_ETimer_T(26);

        	while(1)
        	{
            	if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            	{
                	/* Read SMBus Host Status */
                	status = *asSMBus[p_channel].SMBusSTA;

                	/* any status bit */
                	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                    	HOSTA_NACK+HOSTA_TMOE+HOSTA_BDS)))
                	{
                    	/* any error bit */
                    	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+HOSTA_FAIL+
                        	HOSTA_NACK+HOSTA_TMOE)))
                    	{
                        	/* bus fail or error */
                        	if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                        	{
                            	ResetSMBus(p_channel);
                        	}
                        	ack = FALSE;
                        	break;
                    	}
                    	/* Byte Done or finish bit is set. */
                    	else
                    	{
                        	/* finish bit */
                        	if(IS_MASK_SET(status,HOSTA_FINTR))
                        	{
                            	/* disable timer */
                            	Stop_ETimer_T();

                            	/* i2c write success */
    	                    	ack = FALSE;
                        	}
                        	else
                        	{
                            	if(p_wlength)
                            	{
                                	p_wlength--;

                                	if(p_wlength!=0x00)
                                	{
                                    	/* to send next byte */
                                    	p_wdata_buffer++;
                                    	*asSMBus[p_channel].SMBusBData =
                                        	*p_wdata_buffer;

                                    	/* clear status bits for next byte */
                                    	*asSMBus[p_channel].SMBusSTA=0xFC;
                                	}
                                	/* change direction. */
                                	else
                                	{
                                    	/* to set I2C_SW_EN and I2C_SW_WAIT */
                                    	*asResetSMBusS[p_channel].SMBusCTL2 |=
                                        	(BIT2+BIT3);

                                    	/* clear status bits */
                                    	*asSMBus[p_channel].SMBusSTA=0xFC;

                                    	if(p_rlength==0x01)
                                    	{
                                        	/* set last byte flag */
                    	                	SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                            	HOCTL_LABY);
                                    	}

                                    	/* to clear I2C_SW_WAIT */
                                    	*asResetSMBusS[p_channel].SMBusCTL2 &=
                                        	~(BIT2);

                                    	ack = TRUE;
                                    	break;
                                	}
                                	/* reset timer  */
                                	
                            	}
                        	}
                    	}
                	}
            	}
				else
				{
					/* 26ms time-out and no any status bit is set. */
					ResetSMBus(p_channel);
                	break;
				}
    	    }

            if(ack == TRUE)
            {
                /* pre-set fail. */
                ack = FALSE;

                /* reset timer  */
               	Enable_ETimer_T(26);

        		while(1)
        		{
            		if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            		{
                    	/* Read SMBus Host Status */
                    	status = *asSMBus[p_channel].SMBusSTA;

                    	/* any status bit */
                    	if(IS_MASK_SET(status, (HOSTA_BDS+HOSTA_DVER+HOSTA_BSER+
                        	HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE+HOSTA_FINTR )))
                    	{
                        	/* any error bit */
                        	if(IS_MASK_SET(status, (HOSTA_DVER+HOSTA_BSER+
                            	HOSTA_FAIL+HOSTA_NACK+HOSTA_TMOE)))
                        	{
                            	/* bus fail or error */
                            	if(IS_MASK_SET(status, (HOSTA_BSER+HOSTA_FAIL)))
                            	{
                                	ResetSMBus(p_channel);
                            	}
                            	break;
                        	}

                        	/* Only Byte Done Status bit is set (successful) */
                        	else
                        	{
                            	if(IS_MASK_SET(status,HOSTA_BDS))
                            	{
                                	/* read data */
                                	if(p_rlength)
                                	{
                                    	*p_rdata_buffer =
                                        	*asSMBus[p_channel].SMBusBData;
                                    	p_rdata_buffer++;

                                    	p_rlength--;

                                    	if(P_first_word_is_read_length)
                                    	{
                                        	if(l_read_length_index)
                                        	{
                                            	l_read_length_index--;
                                            	l_read_length[l_read_length_index] =
                                                	*(p_rdata_buffer -1);

                                            	if(l_read_length_index==0)
                                            	{
                                                	p_rlength = l_read_length[1] +
                                                    	(l_read_length[0]*256);
                                                	if(p_rlength>2)
                                                	{
                                                    	p_rlength-=2;
                                                	}
                                            	}
                                        	}
                                    	}
                                	}

                                	/* next cycle is last byte */
                                	if(p_rlength==0x01)
                                	{
                                    	/* set last byte flag */
                	                	SET_MASK(*asSMBus[p_channel].SMBusCTL,
                                        	HOCTL_LABY);
                                	}

                                	/* clear status bits for next byte */
                                	*asSMBus[p_channel].SMBusSTA=0xFC;

                                	/* reset timer */
                                	
                            	}
                            	/* finish */
                            	else
                            	{
                                	
            	                	if(p_rlength==0)
            	                	{
                                    	/* i2c write success */
            	                    	ack = TRUE;
            	                	}
                                	break;
                            	}
                        	}
                        }
                    }
					else
					{
						/* 26ms time-out and no any status bit is set. */
						ResetSMBus(p_channel);
                		break;
					}
                }

            }

            /* w/c status */
            *asSMBus[p_channel].SMBusSTA=0xFE;

            /* disable I2C mode */
            *asResetSMBusS[p_channel].SMBusCTL2 = 0x11;

            /* timer stop */
            Stop_ETimer_T();
        }
    }

    return(ack);
}



BOOL SMBus_DEF_read_reg(UINT8 channel,UINT8 slv_id, UINT8 addr, UINT8* p_u8Buf, UINT8 u8Length,BOOL pecSupport)
{

	BOOL rdata;
	IO_TRANS stTrans;
	memset(&stTrans, 0, sizeof(IO_TRANS));

	stTrans.enumTransType	= IO_TRANS_TYPE_PIO;
	stTrans.bIsEnd			= TRUE;
	stTrans.u8SlaveId		= slv_id;
	stTrans.u8Dev			= 0;

	//transmit cmd
	rdata = FALSE;
	i2c_set_trans_struc(&stTrans, &addr, 1, NULL, 0, p_u8Buf, u8Length);

	//while (rdata == FALSE)
	{
		rdata = i2c_trans(channel, &stTrans,pecSupport,0xFF);
	}
	return rdata;

}

BOOL SMBus_DEF_write_reg(UINT8 channel,UINT8 slv_id, UINT8 addr, UINT8* p_u8Buf, UINT8 u8Length, BOOL pecSupport,BYTE pecValue)
{

	BOOL rdata;
	IO_TRANS	stTrans;
	memset(&stTrans, 0, sizeof(IO_TRANS));

	stTrans.enumTransType	= IO_TRANS_TYPE_PIO;
	stTrans.bIsEnd			= TRUE;
	stTrans.u8SlaveId		= slv_id;
	stTrans.u8Dev			= 0;

	//transmit cmd
	rdata = FALSE;
	i2c_set_trans_struc(&stTrans, &addr, 1, p_u8Buf, u8Length,  NULL, 0 );

	//while (rdata == FALSE)
	{
		rdata = i2c_trans(channel, &stTrans,pecSupport,pecValue);
	}
	return rdata;

}


BYTE SMBus_DEF_Transfer(BYTE channel, BYTE prtcl, BYTE smbus_address, BYTE smbus_cmd,BYTE *buf,BYTE count,BOOL pecsupport,BYTE pecValue)
{
	UINT8 Ch,result;


	if((channel>=SMbusCh4) && (channel<=SMbusCh6))
	{
		Ch = channel-SMbusCh4;
		
		switch(prtcl)
		{
			case SMbusSBC:	//0x04: Send Byte
				//
				result = SMBus_DEF_write_reg(Ch,smbus_address,smbus_cmd,buf,count,pecsupport,pecValue);
				break;
			case SMbusRBC:	//0x05: Receive Byte
				//
				result = SMBus_DEF_read_reg(Ch,smbus_address,smbus_cmd,buf,count,pecsupport);
				break;
			case SMbusWB:	//0x06: Write Byte
			case SMbusWW:	//0x48: Write Word 
			case SMbusWBK:	//0x0A: Write Block
				result = SMBus_DEF_write_reg(Ch,smbus_address,smbus_cmd,buf,count,pecsupport,pecValue);
				break;
			case SMbusRB:	//0x07: Read Byte
			case SMbusRW:	//0x09: Read Word
			case SMbusRBK:	//0x0B: Read Block
				result = SMBus_DEF_read_reg(Ch,smbus_address,smbus_cmd,buf,count,pecsupport);
				break;
			//case 0x0e:	//0x0E: Write Block (I2C)
			//case 0x0f:	//0x0F: Read Block (I2C)
				//
				//break;
			default:
				//
				break;			
		}
	}
	return result;
}


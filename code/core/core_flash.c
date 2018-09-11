/*
 * ****************************************************************************
 *
 *  core_flash.c
 *
 *  Copyright (c) 2012- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/11/21
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

#define CORE_FLASH_OPT  __attribute__((optimize("O0")))

/**
 * ****************************************************************************
 * To reset immu
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void __attribute__ ((section (".core_flash_reset_immu")))
    Reset_Immu_Tag_Sram(void)
{
    SET_MASK(MCCR, BIT4);
    CLEAR_MASK(MCCR, BIT4);     /* Immu Tag Sram reset. */
}

#if __IT8386__ELPM__

CORE_FLASH_OPT
void CORE_FLASH_CODE FlashECCode(void)
{
	RamcodeCmd = 0x00;
	RamcodeSend = 0x00;

   // PM1DO = 0x33;		// ACK

   	if(RamCodePort==0x6C)
    	PM2DO = 0x33;               // ACK
    else
    	PM1DO = 0x33;               // ACK  

 	while(1) 
  	{	
  		if( IS_MASK_SET(KBHISR,IBF))
  		{
			RamcodeCmd = KBHIDIR;
			continue;
		}
		
		if (RamCodePort==0x6C)
        {
	       	if( IS_MASK_CLEAR(PM2STS,P_IBF) ) continue;
	        if( IS_MASK_CLEAR(PM2STS,P_C_D) ) continue;
	        RamcodeCmd = PM2DI;
        }
        else 
        {
         	if( IS_MASK_CLEAR(PM1STS,P_IBF) ) continue;
	        if( IS_MASK_CLEAR(PM1STS,P_C_D) ) continue;
	        RamcodeCmd = PM1DI;
        }
		
		if(RamcodeSend==1)
		{
			ECINDDR = RamcodeCmd;
			RamcodeSend = 0x00;
			continue;
		}

		if(RamcodeCmd==0x01)
		{
			ECINDAR3 = EC_Indirect_Selection;   // Enter follow mode
   			ECINDAR2 = 0xFF;
			ECINDAR1 = 0xFE;
   			ECINDAR0 = 0x00;   		// FFFFExx = 0xFF   
   			ECINDDR = 0x00;			// SCE# high level
		}
		else if(RamcodeCmd==0x02)
		{							// Send SPI command
			ECINDAR1 = 0xFD;
			RamcodeSend = 1;
		}
		else if(RamcodeCmd==0x03)
		{							// write byte to spi
			RamcodeSend = 1;
		}
		else if(RamcodeCmd==0x04)
		{	
			if(RamCodePort==0x6C)
            	PM2DO=ECINDDR;                 // Read byte from spi                     
            else
            	PM1DO=ECINDDR;                 // Read byte from spi
		}
		else if(RamcodeCmd==0x05)
		{	
			ECINDAR3 = 0x00;     	//Exit follow mode
			ECINDAR2 = 0x00;
		}
		else if(RamcodeCmd==0xFC)
		{	
			WinFlashMark = exitFlashMark1;

			#ifndef EC_FEATURE_ONLY
            SET_MASK(MCCR1, BIT7);
			#endif				/* #ifndef EC_FEATURE_ONLY */
			break;

		}
		else if(RamcodeCmd==0xFE)
		{	
            BRAM[62]=0xAA;
			BRAM[63]=0x55;
            ETWCFG=EWDKEYEN;  	    // enable external WDT key
			EWDKEYR=0xFF;           // external WDT reset   
       		while(1);				// Wait for watch dog time-out
		}
 	}

    Reset_Immu_Tag_Sram();          /* reset immu */

#ifdef	EC_FEATURE_ONLY
    main();                         /* return to main */
#else
    hal_warm_boot_os();
#endif /* #ifdef EC_FEATURE_ONLY */

}

#else

/**
 * ****************************************************************************
 * The function of EC flash
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE FlashECCode(void)
{
	RamcodeCmd = 0x00;
	RamcodeSend = 0x00;

    PM1DO = 0x33;		/* ACK */

 	while(1) 
  	{	
  		if( IS_MASK_SET(KBHISR,IBF))
  		{
			RamcodeCmd = KBHIDIR;
			continue;
		}
		if( IS_MASK_CLEAR(PM1STS,P_IBF) ) continue;
		if( IS_MASK_CLEAR(PM1STS,P_C_D) ) continue;
		RamcodeCmd = PM1DI;

		if(RamcodeSend==1)
		{
			ECINDDR = RamcodeCmd;
			RamcodeSend = 0x00;
			continue;
		}

		if(RamcodeCmd==0x01)
		{
			ECINDAR3 = EC_Indirect_Selection;   /* Enter follow mode */
   			ECINDAR2 = 0xFF;
			ECINDAR1 = 0xFE;
   			ECINDAR0 = 0x00;   		/* FFFFExx = 0xFF */
   			ECINDDR = 0x00;			/* SCE# high level */
		}
		else if(RamcodeCmd==0x02)
		{							/* Send SPI command */
			ECINDAR1 = 0xFD;
			RamcodeSend = 1;
		}
		else if(RamcodeCmd==0x03)
		{							/* write byte to spi */
			RamcodeSend = 1;
		}
		else if(RamcodeCmd==0x04)
		{	
			/* while(IS_MASK_SET(PM1STS,P_OBF)); */
			PM1DO=ECINDDR;			/* Read byte from spi */
		}
		else if(RamcodeCmd==0x05)
		{	
			ECINDAR3 = 0x00;     	/* Exit follow mode */
			ECINDAR2 = 0x00;
		}
		else if(RamcodeCmd==0xFC)
		{	
			WinFlashMark = exitFlashMark1;

            #ifndef EC_FEATURE_ONLY
            SET_MASK(MCCR1, BIT7);
            #endif  /* #ifndef EC_FEATURE_ONLY */
			break;
		}
		else if(RamcodeCmd==0xFE)
		{	
            ETWCFG=EWDKEYEN;  	    /* enable external WDT key */
			EWDKEYR=0xFF;           /* external WDT reset */
       		while(1);				/* Wait for watch dog time-out */
		}
 	}

    Reset_Immu_Tag_Sram();          /* reset immu */

    #ifdef  EC_FEATURE_ONLY
    main();                         /* return to main */
    #else
    hal_warm_boot_os();
    #endif /* #ifdef EC_FEATURE_ONLY */
}
#endif

#if !DEF_EC_RW_EFLASH_VIA_ROM_API
/**
 * ****************************************************************************
 * The function of reading SPI status ( command 0x05)
 * After this function, spi status will save to SPIReadStatus
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Read_Status(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read status command */

	SPIReadStatus = ECINDDR;		/* Save status Register to SPIReadStatus */
	
	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of Write SPI status ( command 0x01)
 * After this function, the value of SPIWriteStatus will write to spi status
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Write_Status(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			/* Write enable */
	
    if(SPIID==SSTID)
	{
		ECINDAR1 = 0xFE;
		ECINDDR = 0xFF;				/* SCE# high level */
		ECINDAR1 = 0xFD;
		ECINDDR = SPICmd_EWSR;		/* Enable Write Status Register */
	}    
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRSR;			/* Write status command */

	/* Write SPIWriteStatus to spi status register */
	ECINDDR = SPIWriteStatus;

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	
	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of reading SPI ID (command 0x9F)
 * After this function, spi id will save to array SPIIDBuf[] 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Read_ID(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_DeviceID;      /* Manufacture ID command */
	for(SPIIndex=0x00;SPIIndex<4;SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR;
	}
	SPIID = SPIIDBuf[0];	

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */

	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     	        /* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of reading SPI ID (command 0xAB)
 * After this function, spi id will save to array SPIIDBuf[] 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Read_ID_CmdAB(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_RDID;		    /* Read ID command */
    ECINDDR = 0x00;		            /* Read ID command addr2 */
    ECINDDR = 0x00;		            /* Read ID command addr1 */
    ECINDDR = 0x00;		            /* Read ID command addr0 */
    
	for(SPIIndex=0x00;SPIIndex<4;SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR; 
	}
	SPIID = SPIIDBuf[0];	

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */

	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     	        /* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of Erasing SPI
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Erase(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&(SPIStatus_BUSY+SPIStatus_WEL))==SPIStatus_WriteEnable)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPIAEraseCmd;			/* Send erase command */
	for(SPIIndex=0x00;SPIIndex<3;SPIIndex++)
	{
		ECINDDR = SPIAddrCycle[SPIIndex];
	}								/* Send address cycle */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */

	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */

	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}
	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of SPI write enable
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Write_Enable(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			/* Write enable */
	
    if(SPIID==SSTID)
    {
        ECINDAR1 = 0xFE;
        ECINDDR = 0xFF;				/* SCE# high level */
        ECINDAR1 = 0xFD;
        ECINDDR = SPICmd_EWSR;		/* Enable Write Status Register */
    }
    
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
        /* Check write enable and spi not busy */
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x02)
        {
            break;
        } 
	}

	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of SPI write disable
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Write_Disable(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			/* Write disable */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
        /* Check write disable and spi not busy */
		if((ECINDDR&(SPIStatus_WEL+SPIStatus_BUSY))==0x00)
        {
            break;
        } 
	}

	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of other SPI write
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SPI_Write_Byte(UINT8  u8IndirectSelection, 
                                    UINT32 u32SPIAddress, 
                                    UINT8* pBuffer, 
                                    UINT32 u32ByteCount)
{
    UINT32 u32SPIIndex;

    /* Follow Mode Enable */
    ECINDAR3 = u8IndirectSelection;
    ECINDAR2 = 0xFF;
    ECINDAR0 = 0x00; /*FFFFExx = 0xFF */

    /* Waiting SPI Free */
    ECINDAR1 = 0xFE;
    ECINDDR  = 0xFF; /*SCE# high level*/
    ECINDAR1 = 0xFD;
    ECINDDR  = SPICmd_ReadStatus;
    while((ECINDDR & SPIStatus_BUSY) != 0x00); 

    /* Send Program One Data Byte command */
    ECINDAR1 = 0xFE;
    ECINDDR  = 0xFF; /*SCE# high level*/
    ECINDAR1 = 0xFD;
    ECINDDR  = SPICmd_BYTEProgram;

    /* Send Address */
    ECINDDR = (UINT8)(((u32SPIAddress) >> 16) & 0xFF); /*Send Addr 23-16*/
	ECINDDR = (UINT8)(((u32SPIAddress) >> 8 ) & 0xFF); /*Send Addr 15-8 */
	ECINDDR = (UINT8)(((u32SPIAddress) >> 0 ) & 0xFF); /*Send Addr 7-0  */        

	u32SPIIndex = 0x00;
	do
	{
		ECINDDR = *pBuffer;
		u32SPIIndex++;
		pBuffer++;
	}
	while(u32SPIIndex != u32ByteCount);    

    /* Waiting SPI Free */
    ECINDAR1 = 0xFE;
    ECINDDR  = 0xFF; /*SCE# high level*/
    ECINDAR1 = 0xFD;
    ECINDDR  = SPICmd_ReadStatus;
    while((ECINDDR & SPIStatus_BUSY) != 0x00);     

    /* Exit Follow Mode */
    ECINDAR1 = 0xFE;
    ECINDDR  = 0xFF;  /*SCE# high level*/
    ECINDAR3 = 0x00;  /*Exit follow mode*/
    ECINDAR2 = 0x00;
}

/**
 * ****************************************************************************
 * The function of other SPI write 256 bytes
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE Other_SPI_Write_256Bytes(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    /* SCE# high level */
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_BYTEProgram;   /* Send Program One Data Byte command */

	ECINDDR = SPIAddrCycle[0];	    /* For 256 bytes function limite */
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointer++;
	}while(SPIIndex!=0x00);		    /* Send 256 bytes */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}	

	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;				/* For 256 bytes function limite */
}

/**
 * ****************************************************************************
 * The function of SST SPI write 256 bytes (for Word AAI command)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE SST_SPI_Write_256Bytes(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    /* SCE# high level */
	ECINDAR1 = 0xFD;
    /* Auto Address Increment Programming */
    ECINDDR = SPICmd_AAIWordProgram;

	ECINDDR = SPIAddrCycle[0];	    /* For 256 bytes function limite */
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{ 
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointer++;

	    if((SPIIndex%2)==0x00)
	    {
		    ECINDAR1 = 0xFE;
		    ECINDDR = 0xFF;					/* SCE# high level */
		    ECINDAR1 = 0xFD;
		    ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
		    while(1)						/* waiting spi free */
		    {
			    if((ECINDDR&SPIStatus_BUSY)==0x00)
        	    {
            	    break;
                } 
		    }	
                
            if(SPIIndex!=0x00)
            {
		        ECINDAR1 = 0xFE;
		        ECINDDR = 0xFF;			    /* SCE# high level */
		        ECINDAR1 = 0xFD;
                /* Auto Address Increment Programming */
		        ECINDDR = SPICmd_AAIWordProgram;
            }
	    }
	}while(SPIIndex!=0x00);	    /* Send 256 bytes */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    /* SCE# high level */
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_WRDI;          /* Write disable */

    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */

	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;				/* For 256 bytes function limite */
}

/**
 * ****************************************************************************
 * The function of old SST SPI write 256 bytes
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
CORE_FLASH_OPT
void CORE_FLASH_CODE Old_SST_SPI_Write_256Bytes(void)
{
	ECINDAR3 = EC_Indirect_Selection; 
 	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;   				/* FFFFExx = 0xFF */
	
	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					/* SCE# high level */
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	while(1)						/* waiting spi free */
	{
		if((ECINDDR&SPIStatus_BUSY)==0x00)
        {
            break;
        } 
	}								

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    /* SCE# high level */
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_AAIProgram;    /* Auto Address Increment Programming */

	ECINDDR = SPIAddrCycle[0];	    /* For 256 bytes function limite */
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	
	SPIIndex = 0x00;
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointer++;

	    ECINDAR1 = 0xFE;
	    ECINDDR = 0xFF;					/* SCE# high level */
	    ECINDAR1 = 0xFD;
	    ECINDDR = SPICmd_ReadStatus;	/* Read Status Register */
	    while(1)						/* waiting spi free */
	    {
		    if((ECINDDR&SPIStatus_BUSY)==0x00)
            {
                break;
            } 
	    }	

        if(SPIIndex!=0x00)
        {
	        ECINDAR1 = 0xFE;
	        ECINDDR = 0xFF;			    /* SCE# high level */
	        ECINDAR1 = 0xFD;
                /* Auto Address Increment Programming */
            ECINDDR = SPICmd_AAIProgram;
        }
	}while(SPIIndex!=0x00); /* Send 256 bytes */

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;				    /* SCE# high level */
	ECINDAR1 = 0xFD;
    ECINDDR = SPICmd_WRDI;          /* Write disable */


    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */
    WNCKR = 0x00;                   /* Delay 15.26 us */

    /*
	 * ECINDAR1 = 0xFE;
	 * ECINDDR = 0xFF;
	 * ECINDAR1 = 0xFD;
	 * ECINDDR = SPICmd_ReadStatus;
	 * while(1)
	 * {
	 *    if((ECINDDR&(SPIStatus_BUSY+SSTSPIStatus_AAI))==0x00)
     *    {
     *        break;
     *    } 
	 * }	
	 */

	ECINDAR3 = 0x00;     			/* Exit follow mode */
	ECINDAR2 = 0x00;				/* For 256 bytes function limite */
}
#endif  /* !DEF_EC_RW_EFLASH_VIA_ROM_API */

//-----------------------------------------------------------------------------
// Common functions for SPI control
//-----------------------------------------------------------------------------
static void CORE_FLASH_CODE FOLLOW_MODE_ENABLE(BYTE u8Data)
{
    ECINDAR3 = u8Data;
    ECINDAR2 = 0xFF;
    ECINDAR0 = 0x00; /*FFFFExx = 0xFF */
}

static void CORE_FLASH_CODE CS_HIGH(void)
{
    ECINDAR1 = 0xFE;
    ECINDDR  = 0xFF; /*SCE# high level*/
}

static void CORE_FLASH_CODE CS_LOW(void)
{
    ECINDAR1 = 0xFD;
}

static void CORE_FLASH_CODE SET_CMD(BYTE cmd)
{
    CS_HIGH();
    CS_LOW();
    ECINDDR = cmd;
}

static void CORE_FLASH_CODE SEND_ADDRESS(DWORD addr)
{
    ECINDDR = (BYTE)(((addr) >> 16) & 0xFF); /*Send Addr 23-16*/
	ECINDDR = (BYTE)(((addr) >> 8 ) & 0xFF); /*Send Addr 15-8 */
	ECINDDR = (BYTE)(((addr) >> 0 ) & 0xFF); /*Send Addr 7-0  */
}

static void CORE_FLASH_CODE WAIT_STATUS(BYTE m, BYTE s)
{
    SET_CMD(SPICmd_ReadStatus); /*Read Status Register*/
    while((ECINDDR & m) != s);
}

static void CORE_FLASH_CODE FOLLOW_MODE_DISABLE(void)
{
    CS_HIGH();
    ECINDAR3 = 0x00;  /*Exit follow mode*/
    ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of reading SPI status ( command 0x05)
//  After this function, spi status will save to SPIReadStatus
//-----------------------------------------------------------------------------
BYTE CORE_FLASH_CODE spi_read_status(BYTE u8IndirectSelection)
{
	BYTE u8SPIReadStatus;

	FOLLOW_MODE_ENABLE(u8IndirectSelection);

	SET_CMD(SPICmd_ReadStatus);     // Read status command

	u8SPIReadStatus  = ECINDDR;     // Save status Register to SPIReadStatus

	FOLLOW_MODE_DISABLE();          // Exit follow mode

	return u8SPIReadStatus;
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0x9F)
//  After this function, spi id will save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
void CORE_FLASH_CODE spi_read_id_ext(BYTE u8IndirectSelection, 
                                     BYTE pSPIIDBuf[], BYTE u8length)
{
	BYTE u8SPIIndex;

	FOLLOW_MODE_ENABLE(u8IndirectSelection);

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	SET_CMD(SPICmd_DeviceID);               // Manufacture ID command

	for(u8SPIIndex = 0x00; u8SPIIndex < u8length; u8SPIIndex++)
	{
		pSPIIDBuf[u8SPIIndex] = ECINDDR;
	}

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	FOLLOW_MODE_DISABLE();                  // Exit follow mode
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0xAB)
//  After this function, spi id will save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
void CORE_FLASH_CODE spi_read_id_cmd_ab_ext(BYTE u8IndirectSelection, 
                                            BYTE pSPIIDBuf[], BYTE u8length)
{
	BYTE u8SPIIndex;

	FOLLOW_MODE_ENABLE(u8IndirectSelection);

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	SET_CMD(SPICmd_RDID);            // Read ID command
	ECINDDR  = 0x00;                 // Read ID command addr2
	ECINDDR  = 0x00;                 // Read ID command addr1
	ECINDDR  = 0x00;                 // Read ID command addr0

	for(u8SPIIndex = 0x00; u8SPIIndex < u8length; u8SPIIndex++)
	{
		pSPIIDBuf[u8SPIIndex] = ECINDDR;
	}

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	FOLLOW_MODE_DISABLE();                  // Exit follow mode
}

//-----------------------------------------------------------------------------
// The function of other SPI write
//-----------------------------------------------------------------------------
void CORE_FLASH_CODE spi_write_byte(BYTE u8IndirectSelection, 
                                    DWORD u32SPIAddress, 
                                    BYTE *pBuffer, DWORD u32ByteCount)
{
	DWORD u32SPIIndex;

	FOLLOW_MODE_ENABLE(u8IndirectSelection);

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	SET_CMD(SPICmd_BYTEProgram);            // Send Program One Data Byte command

	SEND_ADDRESS(u32SPIAddress);

	u32SPIIndex = 0x00;
	do
	{
		ECINDDR = *pBuffer;
		u32SPIIndex++;
		pBuffer++;
	}
	while(u32SPIIndex != u32ByteCount);

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	FOLLOW_MODE_DISABLE();                  // Exit follow mode
}

//-----------------------------------------------------------------------------
// The function of old SST SPI (for AAI command)
//-----------------------------------------------------------------------------
void CORE_FLASH_CODE spi_write_aai(BYTE u8IndirectSelection, 
                                   DWORD u32SPIAddress, 
                                   BYTE *pBuffer, DWORD u32ByteCount)
{
	DWORD u32SPIIndex;

	FOLLOW_MODE_ENABLE(u8IndirectSelection);

	WAIT_STATUS(SPIStatus_BUSY, 0x00);      // waiting spi free

	SET_CMD(SPICmd_AAIProgram);             // Auto Address Increment Programming

	SEND_ADDRESS(u32SPIAddress);

	u32SPIIndex = 0x00;
	do
	{
		ECINDDR = *pBuffer;
		u32SPIIndex++;
		pBuffer++;

		WAIT_STATUS(SPIStatus_BUSY, 0x00);  // waiting spi free

		if(u32SPIIndex != 0x00)
		{
			SET_CMD(SPICmd_AAIProgram);     // Auto Address Increment Programming
		}
	}
	while(u32SPIIndex != u32ByteCount);      

	SET_CMD(SPICmd_WRDI);                   // Write disable

	WNCKR = 0x00;                   // Delay 15.26 us
	WNCKR = 0x00;                   // Delay 15.26 us
	WNCKR = 0x00;                   // Delay 15.26 us
	WNCKR = 0x00;                   // Delay 15.26 us

	//ECINDAR1 = 0xFE;
	//ECINDDR = 0xFF;               // SCE# high level
	//ECINDAR1 = 0xFD;
	//ECINDDR = SPICmd_ReadStatus;  // Read Status Register
	//while(1)                      // waiting spi free
	//{
	//  if((ECINDDR&(SPIStatus_BUSY+SSTSPIStatus_AAI))==0x00)
	//    {
	//        break;
	//    }
	//}

	FOLLOW_MODE_DISABLE();                  // Exit follow mode
	// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of EC-indirect fast read
//-----------------------------------------------------------------------------
void CORE_FLASH_CODE spi_ec_indirect_fast_read(BYTE u8IndirectSelection, 
                                               DWORD u32SPIAddress, 
                                               BYTE *pBuffer, 
                                               DWORD u32ByteCount)
{
	BYTE u8ECINDAR3_Store;
	BYTE u8FLHCTRL3R_Store;

	u8ECINDAR3_Store  = ECINDAR3;
	u8FLHCTRL3R_Store = FLHCTRL3R;

	if(u8IndirectSelection == SPI_selection_internal || 
       u8IndirectSelection == SPI_selection_external)
	{
		if(u8IndirectSelection == SPI_selection_internal) // internal
		{
			ECINDAR3 = 0x40;
		}
		else                                      // external
		{
			ECINDAR3 = 0x00;
			CLEAR_MASK(FLHCTRL3R, BIT0);
		}

		ECINDAR2 = (BYTE)((u32SPIAddress >> 16) & 0xFF);     // start address
		ECINDAR1 = (BYTE)((u32SPIAddress >>  8) & 0xFF);
		ECINDAR0 = (BYTE)((u32SPIAddress >>  0) & 0xFF);

		while(u32ByteCount)
		{
			ECINDAR2 = (BYTE)((u32SPIAddress >> 16) & 0xFF); // start address
			ECINDAR1 = (BYTE)((u32SPIAddress >> 8) & 0xFF);
			ECINDAR0 = (BYTE)((u32SPIAddress >> 0) & 0xFF);

			*pBuffer = ECINDDR;

			u32SPIAddress++;
			pBuffer++;                      // external ram pointer ++
			u32ByteCount--;
		}

		ECINDAR3  = u8ECINDAR3_Store;
		FLHCTRL3R = u8FLHCTRL3R_Store;
	}
}

/**
 * ****************************************************************************
 * The function of rescan signature
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void CORE_FLASH_CODE eflash_rescan_signature(void)
{
    HINSTC1 |= 0x40;            //rescan 16 byte signature    

	while( STCDMACR & 0x20)     //check bit5 = 0 
    {
        
    }
}

/**
 * ****************************************************************************
 * The function of SPI write 256 bytes
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SPI_Write_256Bytes(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API

    #else
    if(SPIID==SSTID)
    {
        if(SSTDeviceID==SSTID_Old) 
        {
            Old_SST_SPI_Write_256Bytes();
        }
        else
        {
	        SST_SPI_Write_256Bytes();
        }
    }
    else
    {
	    Other_SPI_Write_256Bytes();
    }  
    #endif
}

/**
 * ****************************************************************************
 * The function for flash utility
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ITE_Flash_Utility(void)
{
	DisableAllInterrupt();  /* Disable all interrupt */
    FlashECCode();
}

/**
 * ****************************************************************************
 * The function for oem code write spi status
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Do_SPI_Write_Status(BYTE statusvalue)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        SPIWriteStatus = statusvalue;
    
        if(SPIID==SSTID)
        {
            spi_write_status(EC_Indirect_Selection, SPIWriteStatus, 1);
        }
        else
        {
            spi_write_status(EC_Indirect_Selection, SPIWriteStatus, 0);
        }
    #else
        /* function direct map */
    	SPIWriteStatus = statusvalue;
        /* Disable all interrupt */
    	DisableAllInterrupt();
        SPI_Write_Status();
	    /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}

/**
 * ****************************************************************************
 * The function for oem code read spi status
 *
 * @return
 *
 * @parameter
 *
 * @note, SPI status always save to SPIReadStatus
 *
 * ****************************************************************************
 */
void Do_SPI_Read_Status(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        SPIReadStatus = spi_read_status(EC_Indirect_Selection);
    #else
        /* function direct map */
        /* Disable all interrupt */
    	DisableAllInterrupt();
    	SPI_Read_Status();
        /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}

/**
 * ****************************************************************************
 * The function for oem code read spi ID (command 0x9F)  
 *
 * @return
 *
 * @parameter
 *
 * @note, SPI ID always save to array SPIIDBuf[]
 *
 * ****************************************************************************
 */
void Do_SPI_Read_ID(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        spi_read_id_ext(EC_Indirect_Selection, &SPIIDBuf[0], 4);
        SPIID = SPIIDBuf[0];
    #else
        /* function direct map */
	    /* Disable all interrupt */
    	DisableAllInterrupt();
    	SPI_Read_ID();
        /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif

    if(SPIIDBuf[0]==0xFF && SPIIDBuf[1]==0xFF && SPIIDBuf[2]==0xFE)
    {
        /* force eflash type == sst */
        SPIID=SSTID;
    }
}

/**
 * ****************************************************************************
 * The function for oem code read spi ID (command 0xAB)  
 *
 * @return
 *
 * @parameter
 *
 * @note, SPI ID always save to array SPIIDBuf[]
 *
 * ****************************************************************************
 */
void Do_SPI_Read_ID_CmdAB(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        spi_read_id_cmd_ab_ext(EC_Indirect_Selection, &SPIIDBuf[0], 4);
        SPIID = SPIIDBuf[0];
    #else
        /* function direct map */
        /* Disable all interrupt */
	    DisableAllInterrupt();
	    SPI_Read_ID_CmdAB();
        /* Enable all interrupt */
	    EnableAllInterrupt();
	#endif
}

/**
 * ****************************************************************************
 * The function for oem code erase spi
 *
 * @return
 *
 * @parameter
 * 1 EraseCmd,Only support SPICmd_Erase4KByte, 
 *            SPICmd_Erase32KByte, and SPICmd_Erase64KByte.
 * 2 Addr2, A23 ~ A16
 * 3 Addr1, A15 ~ A8
 * 4 Addr0, A7  ~ A0
 *
 * @note
 *
 * ****************************************************************************
 */
void Do_SPI_Erase(BYTE EraseCmd, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        /* Disable all interrupt */
        //DisableAllInterrupt();
        if(SPIID==SSTID)
        {
            spi_write_enable(EC_Indirect_Selection, 1);
        }
        else
        {
            spi_write_enable(EC_Indirect_Selection, 0);
        } 
        spi_erase(EC_Indirect_Selection, EraseCmd,
            (unsigned long)((Addr2<<16)+(Addr1<<8)+Addr0));
        spi_write_disable(EC_Indirect_Selection);
        /* Enable all interrupt */
        //EnableAllInterrupt();
    #else
        /* function direct map */
    	SPIAEraseCmd = EraseCmd;
    	SPIAddrCycle[0] = Addr2;
    	SPIAddrCycle[1] = Addr1;
    	SPIAddrCycle[2] = Addr0;
		/* Disable all interrupt */
    	DisableAllInterrupt();
    	SPI_Erase();
	    /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}
#if 0
/**
 * ****************************************************************************
 * The function for oem code write spi 256 bytes
 *
 * @return
 *
 * @parameter
 * 1 DataPointer --> Pointe to external memory
 * 2 Addr2, A23 ~ A16
 * 3 Addr1, A15 ~ A8
 * 4 Addr0, A7  ~ A0
 *
 * @note
 *
 * ****************************************************************************
 */
void Do_SPI_Write_256Bytes(
    BYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
	    /* Disable all interrupt */
        DisableAllInterrupt();
        if(SPIID==SSTID)
        {
            spi_write_enable(EC_Indirect_Selection, 1);
        }
        else
        {
            spi_write_enable(EC_Indirect_Selection, 0);
        } 

        if(SPIID==SSTID)
        {
            if(SSTDeviceID==SSTID_Old) 
            {
                spi_write_aai(EC_Indirect_Selection,
                    (unsigned long)((Addr2<<16)+(Addr1<<8)+Addr0),
                    DataPointer, 256);
            }
            else
            {
    	        spi_write_aai_word(EC_Indirect_Selection,
                    (unsigned long)((Addr2<<16)+(Addr1<<8)+Addr0),
                    DataPointer, 256);
            }
        }
        else
        {
    	    spi_write_byte(EC_Indirect_Selection,
                (unsigned long)((Addr2<<16)+(Addr1<<8)+Addr0),
                DataPointer, 256);
        }  
        spi_write_disable(EC_Indirect_Selection);
	    /* Enable all interrupt */
        EnableAllInterrupt();
    #else
        /* function direct map */
    	SPIDataPointer = DataPointer;
    	SPIAddrCycle[0] = Addr2;
    	SPIAddrCycle[1] = Addr1;
    	SPIAddrCycle[2] = Addr0;
		/* Disable all interrupt */
    	DisableAllInterrupt();
        /* Load function to ram */
    	SPI_Write_256Bytes();
    	/* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}
#endif
/**
 * ****************************************************************************
 * he function for oem code spi write enable
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Do_SPI_Write_Enable(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        if(SPIID==SSTID)
        {
            spi_write_enable(EC_Indirect_Selection, 1);
        }
        else
        {
            spi_write_enable(EC_Indirect_Selection, 0);
        } 
    #else
        /* function direct map */
        /* Disable all interrupt */
    	DisableAllInterrupt();
    	SPI_Write_Enable();
        /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}

/**
 * ****************************************************************************
 * The function for oem code spi write disable
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Do_SPI_Write_Disable(void)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        spi_write_disable(EC_Indirect_Selection);
    #else
        /* function direct map */
        /* Disable all interrupt */
    	DisableAllInterrupt();
    	SPI_Write_Disable();
        /* Enable all interrupt */
    	EnableAllInterrupt();
	#endif
}

/**
 * ****************************************************************************
 * EC-indirect fast read.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ECIndirectFastRead(
    BYTE add2, BYTE add1, BYTE add0,
    BYTE selection, WORD length, BYTE *bufferindex)
{
    #if DEF_EC_RW_EFLASH_VIA_ROM_API
        /* rom api */
        spi_ec_indirect_fast_read(selection,
            (unsigned long)((add2<<16)+(add1<<8)+add0), bufferindex, length);
    #else
        /* function direct map */
        BYTE ECINDAR3_Store;
        BYTE FLHCTRL3R_Store;

        ECINDAR3_Store = ECINDAR3;
        FLHCTRL3R_Store = FLHCTRL3R;
        
        if(selection==SPI_selection_internal ||
            selection==SPI_selection_external)
        {
            if(selection==SPI_selection_internal)   /* internal */
            {
                ECINDAR3 = 0x40; 
            }
            else                                    /* external */
            {
                ECINDAR3 = 0x00; 
                CLEAR_MASK(FLHCTRL3R, BIT0);
            }

            ECINDAR2 = add2;                        /* start address */
            ECINDAR1 = add1;
            ECINDAR0 = add0;

            while(length)
            {
                ECINDAR2 = add2;                    /* start address */
                ECINDAR1 = add1;
                ECINDAR0 = add0;
                *bufferindex = ECINDDR;

                if(++add0 == 0x00)
                {
                    if(++add1 == 0x00)
                    {
                        add2++;
                    }
                }
                bufferindex++;                  /* external ram pointer ++ */
                length--;
            }

            ECINDAR3 = ECINDAR3_Store;
            FLHCTRL3R = FLHCTRL3R_Store;
        }
    #endif
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 * selection, SPI_selection_internal | SPI_selection_external
 *
 * @note
 *
 * ****************************************************************************
 */
void Init_EC_Indirect_Selection(BYTE selection)
{
    if(selection==SPI_selection_internal)   /* Internal */
    {
        EC_Indirect_Selection = 0x4F;
    }
    else                                    /* external */
    {
        EC_Indirect_Selection = 0x0F;
    }
}
#if 0
/**
 * ****************************************************************************
 * The function of direct map
 *
 * @return
 *
 * @parameter
 * sramnum, 0 ~ 12. ***Note*** : EC firmware only use [0].
 * addr, 4K boundary.
 * option, 1 valid, enabke. 0 invalid, disable.
 *
 * @note
 *
 * ****************************************************************************
 */
const sDMAScratchSRAM asDMAScratchSRAM[]=
{
    {	&SCRA0H,	&SCRA0M,	&SCRA0L	    },
	{	&SCRA1H,	&SCRA1M,	&SCRA1L	    },
	{	&SCRA2H,	&SCRA2M,	&SCRA2L	    },
	{	&SCRA3H,	&SCRA3M,	&SCRA3L	    },
	{	&SCRA4H,	&SCRA4M,	&SCRA4L	    },
    {	&SCRA5H,	&SCRA5M,	&SCRA5L	    },
	{	&SCRA6H,	&SCRA6M,	&SCRA6L	    },
	{	&SCRA7H,	&SCRA7M,	&SCRA7L	    },
	{	&SCRA8H,	&SCRA8M,	&SCRA8L	    },
	{	&SCRA9H,	&SCRA9M,	&SCRA9L	    },
    {	&SCRA10H,	&SCRA10M,	&SCRA10L	},
	{	&SCRA11H,	&SCRA11M,	&SCRA11L	},
	{	&SCRA12H,	&SCRA12M,	&SCRA12L	},
};

void CacheDma(BYTE sramnum,DWORD addr, BYTE option)
{
    if(option==0x01)
    {
        eflash_to_direct_map(addr, sramnum);
        dm_set_segment_valid(sramnum);
    }
    else
    {
        dm_set_segment_invalid(sramnum);
    }	
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
extern char __ramcode_begin;
#define GET_RAMCODE_START_ADDRESS     ((unsigned long)&__ramcode_begin)

void Direct_Map_Core_Flash(void)
{
    #if DEF_DIRECT_MAPPING_CORE_FLASH_SECTION
    CacheDma(0, GET_RAMCODE_START_ADDRESS, 1);
    #endif

    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    hal_init_os();
    #endif
}
#endif
/**
 * ****************************************************************************
 * To clear offset 00008Dh of signature to disable mirror function.
 *
 * @return
 *
 * @parameter
 *
 * @note, To make sure all interrupt is disabled before use this function.
 *
 * ****************************************************************************
 */
void Mirror_Code_WriteOnce(void)
{
    BYTE SPIBuffer[16];
    BYTE SPIWriteBuffer[1];
    /* internal eflash */
    Init_EC_Indirect_Selection(SPI_selection_internal);

    /* Read EFlash 0x80 ~ 0x8F to SRAM array SPIBuffer[] */
    ECIndirectFastRead(
        0x00, 0x00, 0x80, SPI_selection_internal, 16, &SPIBuffer[0]);

    /* cmp size not 0x00 */
    if(SPIBuffer[0x0D]!=0x00)
    {
        /* to clear 14th byte of signature */
        spi_write_enable(EC_Indirect_Selection, 1);
        SPIWriteBuffer[0] = 0x00;
        spi_write_byte(EC_Indirect_Selection, (unsigned long)(0x00008D),
            &SPIWriteBuffer[0], 1);
        spi_write_disable(EC_Indirect_Selection);
        /* Rescan 16 byte signature */
        eflash_rescan_signature();
    }
}

#if EX_FLASH_POWER_DOWN
void CORE_FLASH_CODE SetSPIFlashPwr(BYTE pwr_on_off)
{
	FLHCTRL3R=0x08;				// enable SPI Interface
		
	FOLLOW_MODE_ENABLE(SPI_selection_external);
	SET_CMD(pwr_on_off);
	WNCKR = 0x00;			/* Delay 15.26 us */
	FOLLOW_MODE_DISABLE();          // Exit follow mode
}
#endif

#if 1
//----------------------------------------------------------------------------
// Host send idle command to EC
//----------------------------------------------------------------------------
CORE_FLASH_OPT
void CORE_FLASH_CODE Eflash_CmdRamCode(void)
{
  //SCRA2H = 3;          //Disable SCRA2H
  RamcodeCmd = 0x00;
  DisableInternalWDT();			
  SET_MASK(SMECCS,HOSTWA);  // Host write allow
//[Project][CC20130423][Start] Add EC idle function for 68/6C port.
  if (RamCodePort == 0x00)
  {
    KBHIKDOR = 0xFA;
  }
  else if (RamCodePort == 0x01)
  {
    PM2DO = 0xFA;
  }
//[Project][CC20130423][End] Add EC idle function for 68/6C port.
   while(1)
    {
#if chPWSW1WDT
     SET_MASK(GCR9,PWSW1EN2);		// RESET PWSW1 Timer	
#endif
//      BRAM31++;
//[Project][CC20130423][Start] Add EC idle function for 68/6C port.
    if (RamCodePort == 0x00)
    {
      if( IS_MASK_CLEAR(KBHISR,IBF)) continue;
      if( IS_MASK_CLEAR(KBHISR,C_D)) continue;
      RamcodeCmd = KBHIDIR;
    }
    else if (RamCodePort == 0x01)
    {
      if( IS_MASK_CLEAR(PM2STS,P_IBF)) continue;
      if(!(PM2STS & P_C_D)) continue;
      RamcodeCmd = PM2DI;
    }
//[Project][CC20130423][End] Add EC idle function for 68/6C port.
        if (RamcodeCmd == 0x8F)// || (BRAM31==0x55))  // EC exit follow mode
       {
      WinFlashMark = exitFlashMark1;
            WinFlashMark2 = exitFlashMark2;
			//bITETestVar5 = 0x01;  //Debug flag David-
			//A285D00006:---> Start
      break;
      }
		else if(RamcodeCmd == 0x7F)
		{
			 WinFlashMark = exitFlashMark1;
            WinFlashMark2 = exitFlashMark2;
			afterFlashMark = afterFlashMode;
      		break;
		}
		//A285D00006:---> End
//    else if (RamcodeCmd == 0x9F)
//    {
//      //CKCON &= 0x3F;      // set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
//      //CKCON |= 0x10;      // set 26 bit counter // bit7-6 00=14ms,01=113ms, 10=911ms, 11(1/9.2MHz)*2'26=7.2S
//      //      WDTCON|=0x01;       /* WDTRST = 1 Reset watch dog timer.*/
//      //      WDTCON|=0x02;       /* WDTEN  = 1 Enable watch dog.     */	
//     	ETWCFG = EWDKEYEN;
//		EWDKEYR = 0x5A;						// External WDTreset
//	  	while(1);           /* Wait for watch dog time-out      */
//    }
  }
    CLEAR_MASK(SMECCS,HOSTWA);  // Host write not allow
    //bITETestVar5 = 0x02;  //Debug flag David-
  Reset_Immu_Tag_Sram();          /* reset immu */
  //bITETestVar5 = 0x03;  //Debug flag David-
  _nop_();
  //bITETestVar5 = 0x04;  //Debug flag David-
  main();
}
#endif



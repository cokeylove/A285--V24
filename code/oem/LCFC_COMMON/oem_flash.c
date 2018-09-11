//*****************************************************************************
//
//  oem_flash.c
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************



void Check_Mirror_Occurs(void)
{
	if(IS_MASK_SET(FLHCTRL3R, BIT4))			// disable WDT reset Mirror
	{
		CLEAR_MASK(FLHCTRL3R, BIT4);
	}
	
	if(IS_MASK_SET(FLHCTRL3R, BIT7))
	{
		SET_MASK(FLHCTRL3R, BIT7);
		SET_MASK(FLHCTRL3R, BIT0);
	}
}

void EC_Mirror_release(void)
{
	Check_Mirror_Occurs();
	if(FLHCTRL3R & 0x80)
	{
		FLHCTRL3R |= 0x80;		
	}
	
	FLHCTRL3R = 0x81;					// force tri state
	Mirror_Code_WriteOnce();
}


void SetLPCClockFreeRun(void)
{
  GPCRH0 = OUTPUT;
  CLEAR_MASK(GPDRH,BIT(0));
}

//----------------------------------------------------------------------------
// The function of Scratch rom
//----------------------------------------------------------------------------
void RamProgram(BYTE useport)
{
  RamCodePort = useport;
  DisableAllInterrupt();

  Eflash_CmdRamCode();

  //Tmp_XPntr = 0x700;
  //Tmp_code_pointer = (void *)Eflash_CmdRamCode;

//     for(ITempB01=0;ITempB01<255;ITempB01++)
//     {
//    *Tmp_XPntr = *Tmp_code_pointer;
//        Tmp_XPntr++;
//       Tmp_code_pointer++;
//     }
//  SET_MASK(FBCFG,SSMC);         // enable scatch ROM

//    _nop_();
//  _nop_();
//  _nop_();
//  _nop_();
//  _nop_();
//  _nop_();
//  _nop_();
//  _nop_();

//  RamCode();
}

void Write_Eflash_1Byte(BYTE FA2,BYTE FA1,BYTE FA0,BYTE OneByte)
{
    DisableAllInterrupt();
    
	spi_write_enable(SPI_selection_internal, 0);
    spi_write_byte(EC_Indirect_Selection, (unsigned long)((FA2<<16)+(FA1<<8)+FA0), &OneByte, 1);
	spi_write_disable(SPI_selection_internal);


    eFlashVarifyOK = 0x78;
}

//*************************************************************************
BYTE Erase_Eflash_1K(BYTE FA2,BYTE FA1,BYTE FA0)
{
  
   
   spi_write_enable(SPI_selection_internal, 0);
   Do_SPI_Erase(SPICmd_SectorErase, FA2, FA1, FA0);
   spi_write_disable(SPI_selection_internal);

   return (0x01);
}


BYTE Read_Eflash_Byte(BYTE FA2,BYTE FA1,BYTE FA0)
{
  
  BYTE tmpRead;

  
  ECIndirectFastRead(FA2, FA1, FA0, SPI_selection_internal, 1, &tmpRead);
  return tmpRead;
}






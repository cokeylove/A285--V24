//*****************************************************************************
//
//  oem_debug.c
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

//------------------------------------------------------------
// Debug function
//------------------------------------------------------------
void Save_All_RamandGPIO (void){
//save EC ram 0x000 ~ 0x3FF 
    DisableAllInterrupt();
    Erase_Eflash_1K(EEPROMA2,EEPROM_GPIO_DEGUG,0x00);
    Erase_Eflash_1K(EEPROMA2,EEPROM_ALL_RAM0,0x00);
    Erase_Eflash_1K(EEPROMA2,EEPROM_ALL_RAM4,0x00);
    Erase_Eflash_1K(EEPROMA2,EEPROM_ALL_RAM8,0x00);
    Erase_Eflash_1K(EEPROMA2,EEPROM_ALL_RAMC,0x00);
    SCRA2H = 3; //disable SCRA2H
    EnableAllInterrupt();    
//    Erase_Eflash_1K(EEPROMA2,EEPROM_ALL_RAM0,0x00);
    WRITE_DATA2EEPROM(SD_Ptr_uchar_8(EC_RAMBase), 0x400, EEPROM_ALL_RAM0, 0x00);     
//save EC ram 0x400 ~ 0x7FF 
    WRITE_DATA2EEPROM(SD_Ptr_uchar_8(OEMRAM4), 0x400, EEPROM_ALL_RAM4, 0x00);  
//save EC ram 0x800 ~ 0xBFF 
    WRITE_DATA2EEPROM(SD_Ptr_uchar_8(OEMRAM8), 0x400, EEPROM_ALL_RAM8, 0x00);  
//save EC ram 0xC00 ~ 0xFFF 
    WRITE_DATA2EEPROM(SD_Ptr_uchar_8(OEMRAMC), 0x400, EEPROM_ALL_RAMC, 0x00);     
//save GPIO 0x1600 ~ 0x167F to BANK3 0x00 to 0x7F
    WRITE_DATA2EEPROM((UINT8 *)&GCR, 0x80, EEPROM_GPIO_DEGUG, 0x00);
}




BOOL CheckDebugCardIN ( void){
  if( !bRWSMBus(chSMB_DebugCard,SMbusRB,DebugCard_Addr,ReadID,&checkCardStatus,SMBus_NoPEC) )
  {
   	CLEAR_MASK(checkCardStatus,checkCardExist);
    return FALSE;
  }
  else
  {
  	  SET_MASK(checkCardStatus,checkCardExist);
  	  bRWSMBus(chSMB_DebugCard,SMbusWB,DebugCard_Addr,SendBIOSCode,(BYTE *)&BRAM3F,SMBus_NoPEC);
    return TRUE;  
  }
}


BOOL CheckDebugCardOut ( void){
  if(IS_MASK_CLEAR(checkCardStatus,checkCardExist))
  {
    return FALSE;
  }
  else
  {
	  checkCardStatus = 0x00;
    return TRUE; 
  }
}


//-----------------------------------------------------------------------------
const BYTE SEG7_Table[]=
{
  /*            a
               ---------
               |       |
           f   |   g   |  b
              ---------
               |       |
           e   |       |  c
               ---------
                   d      . h
     */
  // COMMON ANODE SEVEN SEGMENT DISPLAY TABLE
    //  0    1     2     3     4     5     6     7
       0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    //  8     9     A     b     C     d     E     F
       0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,
};

//-----------------------------------------------------------------------------
// Out7SegLED
//-----------------------------------------------------------------------------
void Out7SegLED(BYTE bData)
{
  BYTE bIndex = 8;

  while(bIndex != 0)
  {
    bIndex--;
    if (bData & BIT(bIndex))
    { DEBUG_TxDAT_HI(); }
    else
    { DEBUG_TxDAT_LO(); }

    DEBUG_RxCLK_LO();
    DEBUG_RxCLK_HI();
  }
}

//-----------------------------------------------------------------------------
// Dark7SegLed
//-----------------------------------------------------------------------------
void Dark7SegLed(void)
{
  Out7SegLED(0xFF);  // dark LED
  Out7SegLED(0xFF);  // dark LED
}


//-----------------------------------------------------------------------------
// P80LedOut
//-----------------------------------------------------------------------------
void P80LedOut(BYTE bData)
{
  const BYTE * data_pntr;

//  if(FgDebugCarIn)              // if card dose not exist, return
  {
    // out upper byte
    data_pntr = SEG7_Table;          // get 7-seg table
    data_pntr +=((bData & 0xF0) >> 4);    // mask upper byte and shit 4 bytes  then get data of table

    BRAM3A = *data_pntr;
    Out7SegLED(*data_pntr & ~(GET_ECPORT80 << 7));          // display upper value.

    // out lower byte
    data_pntr = SEG7_Table;          // get 7-seg table
    data_pntr += (bData & 0x0F);       // mask lower byte then get data of table.

    BRAM3B = *data_pntr;

    Out7SegLED(*data_pntr);          // dispaly lower value.
  }
}


//-----------------------------------------------------------------------------
// PollingBIOS80Port
//-----------------------------------------------------------------------------
void PollingBIOS80Port(void)
{
  BRAM3D++;
  if(BRAM3F != BRAM3E)        // if no data in , return.
  {  // clear it.
    EC_TX_ALT;
    printf("\rP80h : 0x%02bX   \n\r",BRAM3F);
    EC_TX_INPUT;
    BRAM3E=  BRAM3F;          // clear it.
    DelayXms(1);
/*
    DelayXms(1);

    EC_RX_OUTPUT;
    EC_TX_OUTPUT;
        P80LedOut(BRAM3F);        // to show it
        EC_RX_ALT;
    EC_TX_ALT;
*/

    Port80_Data[Port80_Index] = BRAM3F;
    Port80_Index++;

//    if ( Port80_Index >= (sizeof(Port80_Data)/sizeof(Port80_Data[0])) ) // Range from 0 to 14
   if (Port80_Index > 14)
    { Port80_Index = 0; }

  }
  else
  {
    if( IS_MASK_CLEAR(CombineKeyStatus, b2FnRNL) )  // 20130607 Log
    {
      if(BRAM3F != BRAM3C)      // if no data in , return.
      {
        BRAM3C= BRAM3F;        // clear it.
            //GCR15 &= 0x3F;
        if ( (SystemNotS0&&IS_MASK_CLEAR(CombineKeyStatus, b2FnRNL)) || IS_MASK_CLEAR(pEdgeProjectFlag1, pEdgeProjectDebugIn) )  // 20130607 Log
        {
          EC_RX_INPUT;
          EC_TX_INPUT;
          //return;		// debug card
        } else {
          EC_RX_OUTPUT;
          EC_TX_OUTPUT;
          P80LedOut(BRAM3F);      // to show it
          //GCR15 |= 0xC0;
          EC_RX_INPUT;
          EC_TX_INPUT;
        }
		// debug card +
		if(IS_MASK_SET(checkCardStatus,checkCardExist))
			bRWSMBus(chSMB_DebugCard,SMbusWB,DebugCard_Addr,SendBIOSCode,(BYTE *)&BRAM3F,SMBus_NoPEC);
		// debug card -
      }
    }
  }
}

void RamDebug(unsigned char dbgcode)
{
#if 1
  	BYTE *ClearIndex;
 	BYTE *byte_register_pntr;
    BYTE index;
//  	BYTE i;
	
    byte_register_pntr = (BYTE *)(DebugRamAddr+DebugRamRange);          
    index = *byte_register_pntr;
    *byte_register_pntr +=1;
	
    if ( *byte_register_pntr == DebugRamRange )
   	{
   		*byte_register_pntr = 0;
       	ClearIndex= (BYTE *)DebugRamAddr;
/*
		for (i=0x00;i<DebugRamRange;i++)
     	{
         	*ClearIndex=0x00;
           	ClearIndex++;
     	}
 */    	
  	}

	byte_register_pntr = (BYTE *)(DebugRamAddr + index);
    *byte_register_pntr = dbgcode;
#endif
}

void RamDebugWord(unsigned int dbgword)
{
	BYTE	dgb_H;
	BYTE	dgb_L;

	dgb_H = (BYTE) (dbgword >> 8) ;
	dgb_L = (BYTE) (dbgword & 0x00FF ) ;

	RamDebug(dgb_H);
	RamDebug(dgb_L);
}

void RamDebugDword(unsigned long dbgdword)
{
	BYTE	dgb_4;
	BYTE	dgb_3;
	BYTE	dgb_2;
	BYTE	dgb_1;

	dgb_4 = (BYTE) (dbgdword >> 24) ;
	dgb_3 = (BYTE) (dbgdword >> 16) ;
	dgb_2 = (BYTE) (dbgdword >> 8) ;
	dgb_1 = (BYTE) (dbgdword & 0x00FF ) ;

	RamDebug(dgb_4);
	RamDebug(dgb_3);
	RamDebug(dgb_2);
	RamDebug(dgb_1);
}
  
void Ec2iWrite (
  BYTE  RegOffset,
  BYTE  RegValue
)
{
  SET_MASK(LSIOHA,LKCFG);
  SET_MASK(IBMAE,CFGAE);
  SET_MASK(IBCTL,CSAE);

  //
  // Set offset
  //
  IHIOA = 0;
  IHD = RegOffset;
  while(IS_MASK_SET(IBCTL,CWIB));

  //
  // Write data
  //
  IHIOA = 1;
  IHD = RegValue;
  while(IS_MASK_SET(IBCTL,CWIB));

 	CLEAR_MASK(LSIOHA,LKCFG);
  CLEAR_MASK(IBMAE,CFGAE);
  CLEAR_MASK(IBCTL,CSAE);
}



BYTE Ec2iRead (
  BYTE  RegOffset
)
{
  BYTE  RegValue;

  SET_MASK(LSIOHA,LKCFG);
  SET_MASK(IBMAE,CFGAE);
  SET_MASK(IBCTL,CSAE);

  //
  // Set offset
  //
  IHIOA = 0;
  IHD = RegOffset;
  while(IS_MASK_SET(IBCTL,CWIB));

  //
  // Read data
  //
  IHIOA = 1;
  SET_MASK(IBCTL,CRIB);
  while(IS_MASK_SET(IBCTL,CRIB));
  RegValue = IHD;

 	CLEAR_MASK(LSIOHA,LKCFG);
  CLEAR_MASK(IBMAE,CFGAE);
  CLEAR_MASK(IBCTL,CSAE);

  return RegValue;
}



void DumpLpcReg (
  unsigned char Ldn
)
{
  unsigned short  RegIndex;
  unsigned char   LpcValue;

  //
  // Select LDN
  //
  Ec2iWrite (0x07, Ldn);
  printf ("\n---------------         ");
  switch (Ldn)
  {
    case 0x01: // UART1
      printf ("UART1   ");
      break;

    case 0x02: // UART2
      printf ("UART2   ");
      break;

    case 0x04: // SWUC
      printf ("SWUC    ");
      break;

    case 0x05: // KBC_MS
      printf ("KBC_MS");
      break;

    case 0x06: // KBC_KB
      printf ("KBC_KB  ");
      break;

    case 0x0a: // CIR
      printf ("CIR         ");
      break;

    case 0x0f: // SMFI
      printf ("SMFI      ");
      break;

    case 0x10: // BRAM
      printf ("BRAM    ");
      break;

    case 0x11: // PMC1
      printf ("PMC1     ");
      break;

    case 0x12: // PMC2
      printf ("PMC2     ");
      break;

    case 0x13: // SPI
      printf ("SPI           ");
      break;

    case 0x14: // PECI
      printf ("PECI        ");
      break;

    case 0x17: // PMC3
      printf ("PMC3     ");
      break;

    case 0x18: // PMC4
      printf ("PMC4     ");
      break;

    case 0x19: // PMC5
      printf ("PMC5     ");
      break;

    default:
      break;

  }

  //
  // Header
  //
  printf ("       ----------------\n");
  printf ("\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n\n");

  for (RegIndex = 0; RegIndex <=0xFF; RegIndex++) {
    //
    // Address label
    //
    if ((RegIndex & 0x0F) == 0x00) {
      printf ("%02X      ", RegIndex);
    }

    //
    // Register
    //
    LpcValue = Ec2iRead (RegIndex);
    printf ("%02bx ", LpcValue);
    if ((RegIndex & 0x0F) == 0x0F) {
      printf ("\n");
    }
  }
}



void DumpReg (
  unsigned short RegBase
)
{
  unsigned short ContentIndex;

  //
  // Header
  //
  printf ("\n----------------------- 0x%04X ------------------------\n", (RegBase & BASE_MASK));
  printf ("\t00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n\n");

  //
  // Content
  //
  for (ContentIndex = 0; ContentIndex <= 0xFF; ContentIndex++) {
    //
    // Address label
    //
    if ((ContentIndex & 0x0F) == 0x00) {
      printf ("%04X    ", (RegBase + ContentIndex & LINE_MASK));
    }

    //
    // Register
    //
    printf ("%02bx ", (* (XBYTE * )( (RegBase & BASE_MASK) + ContentIndex)));

    if ((ContentIndex & 0x0F) == 0x0F) {
      printf ("\n");
    }
  }
}

void DumpAllReg (
  void
)
{
  unsigned short DumpAddress;
  //
  // Use UART to print out all EC registers.
  // Following D2EC's dump log format.
  //
  for (DumpAddress = 0x1000; DumpAddress <= 0x2900; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpReg (0x2A00);

  for (DumpAddress = 0x2C00; DumpAddress <= 0x3000; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpReg (0x8000);
  DumpReg (0xC000);

  for (DumpAddress = 0x0000; DumpAddress <= 0x0F00; DumpAddress += 0x100) {
    DumpReg (DumpAddress);
  }

  DumpLpcReg (0x01);  // UART1
  DumpLpcReg (0x02);  // UART2
  DumpLpcReg (0x04);  // SWUC
  DumpLpcReg (0x05);  // KBC_MS
  DumpLpcReg (0x06);  // KBC_KB
  DumpLpcReg (0x0A);  // CIR
  DumpLpcReg (0x0F);  // SMFI
  DumpLpcReg (0x10);  // RTCT
  DumpLpcReg (0x11);  // PMC1
  DumpLpcReg (0x12);  // PMC2
  DumpLpcReg (0x13);  // SSPI
  DumpLpcReg (0x14);  // PECI
  DumpLpcReg (0x17);  // PMC3
  DumpLpcReg (0x18);  // PMC4
  DumpLpcReg (0x19);  // PMC5
  printf("\nDone!\n");
}

//----------------------------------------------------------------------------
// INIT UART
//----------------------------------------------------------------------------
void Init_UART(void)
{
#ifdef UART_Debug
	EC_TX_ALT;    // Set EC Tx ALT.
	Uart_Init();
#endif
}




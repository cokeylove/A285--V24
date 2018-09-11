//*****************************************************************************
//
//  oem_mailbox.c
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



//----------------------------------------------------------------------------
// The function of Mailbox handler
//  Note :
//----------------------------------------------------------------------------
#if Support_COMPAL_MFG_TOOL
void Hook_MailBox(BYTE Cmd)
{
  BYTE i;

  switch(Cmd)
  {
    case 0x40:
      Cmd_40(MMIO_iDATA8C);
      break;
    case 0x41:
      if(MMIO_iDATA8C==0xA0) Cmd_41_A0(0xFE);
      if(MMIO_iDATA8C==0xA1) Cmd_41_A1(0xFE);
      if(MMIO_iDATA8C==0xA2) Cmd_41_A2(0xFE);
      if(MMIO_iDATA8C==0xA3) Cmd_41_A3(0xFE);
      break;
    case 0x42:
      Cmd_42(MMIO_iDATA8C);
      break;
    case 0x43:
      Cmd_43(0xFE,MMIO_iDATA8C);
      break;
    case 0x45:
      // Priam 20120204 ++
      Cmd_45(0xFE,MMIO_iDATA8C);
      break;
      // Priam --
    case 0x46:
      Cmd_46(0xFE,MMIO_iDATA8C);
      break;
    case 0x47:
      // msmart 20100208 ++
      Cmd_47(0xFE,MMIO_iDATA8C);
      break;
      // msmart
      break;
    case 0x49:
      break;
    case 0x4B:
      i           = MMIO_iDATA8C; //Mos: What it is??
      eEEPROMAddrsss   = MMIO_iDATA8D;
      eEEPROMBank    = MMIO_iDATA8E;
      eEEPROMData    = MMIO_iDATA8F;
      Update_EEPROMMark();
      Data2Port(0xFE, 0x00);
      break;

    case 0x4C:
      i = MMIO_iDATA8C;
      eEEPROMAddrsss = MMIO_iDATA8D;
      eEEPROMBank = MMIO_iDATA8E;
      Cmd_4E(0xFE,eEEPROMAddrsss);
      break;

    case 0x4D:
      eEEPROMAddrsss = MMIO_iDATA8C;
      eEEPROMData  = MMIO_iDATA8D;
      Update_EEPROMMark();
      Data2Port(0xFE, 0x00);
      break;
    case 0x4E:
      eEEPROMAddrsss = MMIO_iDATA8C;
      Cmd_4E(0xFE,eEEPROMAddrsss);
      break;
    case 0x4F: //reference from Hook_64Port
      Erase_EEPROMAll();
      break;
    case 0x51:
      Cmd_51(0xFE);
      break;
    case 0x52:
      if(MMIO_iDATA8C==0xA0) Cmd_52_A0(0xFE);
      if(MMIO_iDATA8C==0xA3) Cmd_52_A3(0xFE);
      if(MMIO_iDATA8C==0xA4) Cmd_52_A4(0xFE);
      if(MMIO_iDATA8C==0xA5) Cmd_52_A5(0xFE);
      break;
    case 0x53:
    break;
    case 0x56:
    break;
    case 0x58:
      Cmd_58(MMIO_iDATA8C);
      break;
    case 0x59:
      // Priam 20120204 ++
      Cmd_59(0xFE,MMIO_iDATA8C,0);
      // Priam --
      break;
    case 0x5C:
      Cmd_5C(0xFE);
      break;
    case 0x5D:
      Cmd_5D(MMIO_iDATA8C);
      break;
    case 0x97:
      Cmd_97(MMIO_iDATA8C);
      break;
    case 0xB0:
      Cmd_B0(0xFE,MMIO_iDATA8C);
      break;
    case 0xB4:
      Cmd_B4();
      break;
    case 0x77:
///      CacheDma(1,FSCEHighLevel);
            DisableAllInterrupt();
      Erase_Eflash_1K(0x01,0x20,0x00);
            EnableAllInterrupt();
      break;
    case 0x88:
//      CacheDma(3,FSCEHighLevel);
//      Copy_Eflash_1K(0x00,0x00,0x00, 0x01,0x20,0x00);
      break;
    default:
      break;
  }
}
#endif
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#if Support_COMPAL_MFG_TOOL
void H2RAMprocess(void)
{
  if( (MMIO_CMD_Ctrl & MMIO_Start_flage)==0 ) return;
  if( (MMIO_iCMD < 0x40) || (MMIO_iCMD >0x60) ) return;
  Clear_RAM_Range(MMIO_rDATA[0],0x07);  // Clear H2RAM return data space.
  //(H2RAM_Table[MMIO_iCMD>>4])();

  MMIO_rDATA_index = 0;
  Hook_MailBox(MMIO_iCMD);

  MMIO_iCMD = 0x00;

  MMIO_CMD_Ctrl &= 0xFD; // Stop Start
}
#endif

#if Support_COMPAL_MFG_TOOL
void ThermalMailBox(void)
{
  if(EXT_NAMESPACE_BANK)
  {
    switch(EXT_NAMESPACE_BANK)
    {
      case 0x81:
        if (EXT_NAMESPACE_INDEX==0x02)
          EXT_NAMESPACE_DATA = (FAN_PWM/2);
        if (EXT_NAMESPACE_INDEX==0x04)
          EXT_NAMESPACE_DATA = nAtmFanSpeed;
        else if (EXT_NAMESPACE_INDEX==0x07)
          EXT_NAMESPACE_DATA = Step0_H;
        else if (EXT_NAMESPACE_INDEX==0x08)
          EXT_NAMESPACE_DATA = Step0_L;
        else if (EXT_NAMESPACE_INDEX==0x09)
          EXT_NAMESPACE_DATA = Step0_DAC;
        else if (EXT_NAMESPACE_INDEX==0x0A)
          EXT_NAMESPACE_DATA = Step1_H;
        else if (EXT_NAMESPACE_INDEX==0x0B)
          EXT_NAMESPACE_DATA = Step1_L;
        else if (EXT_NAMESPACE_INDEX==0x0C)
          EXT_NAMESPACE_DATA = Step1_DAC;
        else if (EXT_NAMESPACE_INDEX==0x0D)
          EXT_NAMESPACE_DATA = Step2_H;
        else if (EXT_NAMESPACE_INDEX==0x0E)
          EXT_NAMESPACE_DATA = Step2_L;
        else if (EXT_NAMESPACE_INDEX==0x0F)
          EXT_NAMESPACE_DATA = Step2_DAC;
        else if (EXT_NAMESPACE_INDEX==0x10)
          EXT_NAMESPACE_DATA = Step3_H;
        else if (EXT_NAMESPACE_INDEX==0x11)
          EXT_NAMESPACE_DATA = Step3_L;
        else if (EXT_NAMESPACE_INDEX==0x12)
          EXT_NAMESPACE_DATA = Step3_DAC;
        else if (EXT_NAMESPACE_INDEX==0x13)
          EXT_NAMESPACE_DATA = Step4_H;
        else if (EXT_NAMESPACE_INDEX==0x14)
          EXT_NAMESPACE_DATA = Step4_L;
        else if (EXT_NAMESPACE_INDEX==0x15)
          EXT_NAMESPACE_DATA = Step4_DAC;
        break;

      case 0x82:
        if (EXT_NAMESPACE_INDEX==0x02)
        if (EXT_NAMESPACE_INDEX==0x04)
          EXT_NAMESPACE_DATA = FAN2PRM;
        break;

      case 0x84:
        if (EXT_NAMESPACE_INDEX==0x00)
          EXT_NAMESPACE_DATA = 0x00;
        else if (EXT_NAMESPACE_INDEX==0x01)
          EXT_NAMESPACE_DATA = nRamTemp;
        else if (EXT_NAMESPACE_INDEX==0x02)
          EXT_NAMESPACE_DATA = nVramTemp;
    }
        //RamDebug(EXT_NAMESPACE_DATA);
    EXT_NAMESPACE_INDEX = 0;
    EXT_NAMESPACE_BANK = 0;
  }
}
#endif   //Support_COMPAL_MFG_TOOL






/*-----------------------------------------------------------------------------
 * TITLE: OEM_VGA_AMD.C
 * TITLE: OEM_VGA_AMD.H
 *
 * Author : Ivan Chen
 *
 * Note : Initial VGA & read VGA temperature
 *        
*---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************


#if (PLATFORM_DGPU == AMD)

/*****************************************************************************/
// Procedure: VGA_Initial_Temp                                TimeDiv: 500mSec
// Description: Initial VGA temperature function for AMD.
// GPIO:
// Referrals:
/*****************************************************************************/
void VGA_Initial_Temp(void)
{
  // Check S0 and VGA into sleep mode and VGA delay initial temperature.
  if ( SystemNotS0 ||
       (!(CPU_TYPE & DGPU_EXIST)) ||
       ( VGA_TempDly != 0 ) ||
       (IS_MASK_CLEAR(SYS_MISC1, ACPI_OS))
     ) {
     CLEAR_MASK(ERR_THMSTS,b1VGATempEr);
     VGAErrCnt = 0;
     if (VGA_TEMP > 32) { // Decrease VGA_TEMP
       if (VGA_TEMP <= 100) {
         VGA_TEMP--;
       } else {
         VGA_TEMP = 100;
       }
     }

     VGA_TBuff3 = VGA_TBuff2;
     VGA_TBuff2 = VGA_TBuff1;
     VGA_TBuff1 = VGA_TEMP;
     VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;    // VGA average temperature.
     return;
  }

  MaxCPU_MCHTemp0 = 0x0F;
  MaxCPU_MCHTemp1 = 0x00;
  MaxCPU_MCHTemp2 = 0x01;
  MaxCPU_MCHTemp3 = 0xC5;

  if( !bWSMBusBlock(chSMB_VGA, SMbusWBK, DGPU_Addr, 0x01, &MaxCPU_MCHTemp0, 4, SMBus_NoPEC))
  {
    VGAErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
    {
      if ( VGAErrCnt > 3 )  // 3 Sec.
      {
        SET_MASK(ERR_THMSTS,b1VGATempEr);  // Set bit1 Thermal IC read temperature error.
      }
    }
    else
    {
      if ( VGAErrCnt > 34 )      // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(VGACOMMFAIL_ID);  // 0x20 shutdown ID.		
        VGAErrCnt = 00;        // Clear error count.
        RSMRST_shutdown();	
      }
    }
    ResetSMBus(chSMB_VGA);			
  }
  else
  {
//    VGAErrCnt = 00; // Clear error count.
    SET_MASK(SysStatus2,b4VGATemp_Init); // Start read GPU temperatuer.
    CLEAR_MASK(ERR_THMSTS,b1VGATempEr);     // Clear Error states.
    MaxCPU_MCHTemp0 = 0;
    MaxCPU_MCHTemp2 = 0;
    MaxCPU_MCHTemp3 = 0;
  }

}

/*****************************************************************************/
// Procedure: VGA_Temperature                TimeDiv: 500mSec
// Description: Read VGA temperature(M/B)
// GPIO: GPIOF6(118)
// Referrals:
/*****************************************************************************/
void VGA_Temperature(void)
{
  BYTE BsData;

  // Check S0 and VGA into and ACPI mode.
  if( SystemNotS0 || (!(CPU_TYPE & DGPU_EXIST)) || (IS_MASK_CLEAR(SYS_MISC1,ACPI_OS)) ) {
    CLEAR_MASK(ERR_THMSTS,b1VGATempEr);
    CLEAR_MASK(SysStatus2, b4VGATemp_Init); // Clear AMD dGPU init ok flag.
    VGAErrCnt = 0;
    if (VGA_TEMP > 32) { // Decrease VGA_TEMP
      if (VGA_TEMP <= 100) {
        VGA_TEMP--;
      } else {
        VGA_TEMP = 100;
      }
      if ( IS_MASK_CLEAR(Fan_Debug_Temp,b1VGA_Temp) )  // Debug VGA Temperature, Engineer myself control.
      {
        VGA_TBuff3 = VGA_TBuff2;
        VGA_TBuff2 = VGA_TBuff1;
        VGA_TBuff1 = VGA_TEMP;
        VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;    // VGA average temperature.
      }
    }
    return;
  }

  if(!bRSMBusBlock(chSMB_VGA, SMbusRBK, DGPU_Addr, 0x03, SMB3_DATA, bRead_I2C_NULL))
  {
    CLEAR_MASK(SysStatus2,b4VGATemp_Init); // Clear AMD dGPU init ok flag.
    VGAErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
    {
      if ( VGAErrCnt > 3 )  // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b1VGATempEr);  // Set bit1 Thermal IC read temperature error.
      }
    }
    else
    {
      if ( VGAErrCnt > 34 )      // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(VGACOMMFAIL_ID);  // 0x20 shutdown ID.			
        VGAErrCnt = 00;        // Clear error count.
        RSMRST_shutdown();						
      }
    }
    ResetSMBus(chSMB_VGA);
  }
  else
  {
    *(DWORD*)&MaxCPU_MCHTemp0 = *(DWORD*)&SMB3_DATA[0];
    if(MaxCPU_MCHTemp2 == 0x00)  // read 0'C
    {
      VGAErrCnt++;
      CLEAR_MASK(SysStatus2, b4VGATemp_Init); // Clear AMD dGPU init ok flag.
      if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
      {
        if ( VGAErrCnt > 3 )  // 3 Sec.
        {
          SET_MASK(ERR_THMSTS, b1VGATempEr);  // Set bit1 Thermal IC read temperature error.
        }
      }
      else
      {
        if ( VGAErrCnt > 34 )      // 30 Sec.
        {
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID(VGACOMMFAIL_ID | BIT7);  // 0x20 shutdown ID.		
          VGAErrCnt = 00;        // Clear error count.
        }
      }
    }
    else
    {
      BsData  = ( MaxCPU_MCHTemp2 >> 1 );
      VGA_TEMP = ( BsData & 0x7F );
      VGAErrCnt = 00;    // Clear error count.
      CLEAR_MASK(ERR_THMSTS,b1VGATempEr); // Clear bit1,bit5.

      if ( IS_MASK_CLEAR(Fan_Debug_Temp,b1VGA_Temp) )  // Debug VGA Temperature, Engineer myself control.
      {
        VGA_TBuff3 = VGA_TBuff2;
        VGA_TBuff2 = VGA_TBuff1;
        VGA_TBuff1 = VGA_TEMP;
        VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;    // VGA average temperature.
      }
    }
  }
}



/*****************************************************************************/
// Procedure: OEM_VGA_Hook                                     TimeDiv: 500mSec
// Description: Initial VGA temperature function for AMD.
// GPIO:
// Referrals:
/*****************************************************************************/
void OEM_VGA_Hook(void)
{

  if ( IS_MASK_CLEAR(SysStatus2,b4VGATemp_Init) )
  { VGA_Initial_Temp(); }
  else
  { VGA_Temperature(); }
}

#endif //(PLATFORM_DGPU == AMD)

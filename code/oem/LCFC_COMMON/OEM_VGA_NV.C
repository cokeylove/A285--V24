/*-----------------------------------------------------------------------------
 * TITLE: OEM_VGA_NV.C
 * TITLE: OEM_VGA_NV.H
 *
 * Author : Ivan Chen
 *
 * Note : Read VGA temperature
 *        
*---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************


#if (PLATFORM_DGPU == NVidia)

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

	if(!bRWSMBus(chSMB_VGA, SMbusRB, DGPU_Addr, TmlIC_Temp, &BsData, 0))
	{
		VGAErrCnt++;
		if( IS_MASK_CLEAR(ERR_THMSTS, b1VGATempEr) )// Check bit1 is Error.
		{
			if ( VGAErrCnt > 7 )	// 3 Sec.
			{
				SET_MASK(ERR_THMSTS, b1VGATempEr);	// Set bit1 Thermal IC read temperature error.
			}
		}
		else
		{
			if ( VGAErrCnt > 68 )			// 30 Sec.
			{
//				ProcessSID(VGACOMMFAIL_ID);	// 0x20 shutdown ID.
				VGAErrCnt = 00;				// Clear error count.
//				RSMRST_shutdown();
			}
		}
		ResetSMBus(chSMB_VGA);
	}
	else
	{
			VGA_TEMP = BsData;
			VGAErrCnt = 00;		// Clear error count.
			CLEAR_MASK(ERR_THMSTS,b1VGATempEr);	// Clear bit1,bit5.

			if ( IS_MASK_CLEAR(Fan_Debug_Temp,b1VGA_Temp) )	// Debug VGA Temperature, Engineer myself control.
			{
				VGA_TBuff3 = VGA_TBuff2;
				VGA_TBuff2 = VGA_TBuff1;
				VGA_TBuff1 = VGA_TEMP;
				VGA_TBuff3 = (VGA_TBuff1 + VGA_TBuff2 + VGA_TBuff3)/3;		// VGA average temperature.
			}
	}
}

/*****************************************************************************/
// Procedure: OEM_VGA_Hook                                     TimeDiv: 500mSec
// Description: Initial VGA temperature function for NVidia.
// GPIO:
// Referrals:
/*****************************************************************************/
void OEM_VGA_Hook(void)
{

  VGA_Temperature();
}

#endif //(PLATFORM_DGPU == NVidia)

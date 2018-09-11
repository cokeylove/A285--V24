//*****************************************************************************
//
//  oem_gpio.c
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

const sInitGPIOReg asInitGPIOReg[] = 
{
 	{ &GCR,		GCR_Init		},	// 
 	{ &GCR2,	GCR2_Init		},	//	
 	
	{ &GPDRA,	PortA_Init		},	// Port A Data port Init
	{ &GPDRB,	PortB_Init		},	// Port B Data port Init
	{ &GPDRC,	PortC_Init		},	// Port C Data port Init
	{ &GPDRD,	PortD_Init		},	// Port D Data port Init
	{ &GPDRE,	PortE_Init		},	// Port E Data port Init
	{ &GPDRF,	PortF_Init		},	// Port F Data port Init
	{ &GPDRG,	PortG_Init		},	// Port G Data port Init
	{ &GPDRH,	PortH_Init		},	// Port H Data port Init
	{ &GPDRI,	PortI_Init		},	// Port I Data port Init
	{ &GPDRJ,	PortJ_Init		},	// Port J Data port Init
	{ &GPDRK,	PortK_Init		},	// Port K Data port Init
	{ &GPDRL,	PortL_Init		},	// Port L Data port Init
//	{ &GPDRM,	PortM_Init		},	// Port M Data port Init


	{ &GPCRA0,	PortA0_Ctrl		},  // 
	{ &GPCRA1,	PortA1_Ctrl		},  // 
	{ &GPCRA2,	PortA2_Ctrl		},  // 
	{ &GPCRA3,	PortA3_Ctrl		},  // 
	{ &GPCRA4,	PortA4_Ctrl		},  // 
	{ &GPCRA5,	PortA5_Ctrl		},  // 
	{ &GPCRA6,	PortA6_Ctrl		},  // 
	{ &GPCRA7,	PortA7_Ctrl		},  //
	
	{ &GPCRB0,	PortB0_Ctrl		},
	{ &GPCRB1,	PortB1_Ctrl		},
	{ &GPCRB2,	PortB2_Ctrl		},
	{ &GPCRB3,	PortB3_Ctrl		},
	{ &GPCRB4,	PortB4_Ctrl		},
	{ &GPCRB5,	PortB5_Ctrl		},
	{ &GPCRB6,	PortB6_Ctrl		},
	{ &GPCRB7,	PortB7_Ctrl		},

	{ &GPCRC0,	PortC0_Ctrl		},
	{ &GPCRC1,	PortC1_Ctrl		},
	{ &GPCRC2,	PortC2_Ctrl		},
	{ &GPCRC3,	PortC3_Ctrl		},
	{ &GPCRC4,	PortC4_Ctrl		},
	{ &GPCRC5,	PortC5_Ctrl		},
	{ &GPCRC6,	PortC6_Ctrl		},
	{ &GPCRC7,	PortC7_Ctrl		},

	{ &GPCRD0,	PortD0_Ctrl		},
	{ &GPCRD1,	PortD1_Ctrl		},
	{ &GPCRD2,	PortD2_Ctrl		},
	{ &GPCRD3,	PortD3_Ctrl		},
	{ &GPCRD4,	PortD4_Ctrl		},
	{ &GPCRD5,	PortD5_Ctrl		},
	{ &GPCRD6,	PortD6_Ctrl		},
	{ &GPCRD7,	PortD7_Ctrl		},

	{ &GPCRE0,	PortE0_Ctrl		},
	{ &GPCRE1,	PortE1_Ctrl		},
	{ &GPCRE2,	PortE2_Ctrl		},
	{ &GPCRE3,	PortE3_Ctrl		},
	{ &GPCRE4,	PortE4_Ctrl  	},	
	{ &GPCRE5,	PortE5_Ctrl		},
	{ &GPCRE6,	PortE6_Ctrl		},
	{ &GPCRE7,	PortE7_Ctrl		},

	{ &GPCRF0,	PortF0_Ctrl		},
	{ &GPCRF1,	PortF1_Ctrl		},
	{ &GPCRF2,	PortF2_Ctrl		},
	{ &GPCRF3,	PortF3_Ctrl		},
	{ &GPCRF4,	PortF4_Ctrl		},
	{ &GPCRF5,	PortF5_Ctrl		},
	{ &GPCRF6,	PortF6_Ctrl		},
	{ &GPCRF7,	PortF7_Ctrl		},

	{ &GPCRG0,	PortG0_Ctrl		},
	{ &GPCRG1,	PortG1_Ctrl		},
	{ &GPCRG2,	PortG2_Ctrl		},
	{ &GPCRG3,	PortG3_Ctrl		},
	{ &GPCRG4,	PortG4_Ctrl		},
	{ &GPCRG5,	PortG5_Ctrl		},
	{ &GPCRG6,	PortG6_Ctrl		},
	{ &GPCRG7,	PortG7_Ctrl		},

	{ &GPCRH0,	PortH0_Ctrl		},
	{ &GPCRH1,	PortH1_Ctrl		},
	{ &GPCRH2,	PortH2_Ctrl		},
	{ &GPCRH3,	PortH3_Ctrl		},
	{ &GPCRH4,	PortH4_Ctrl		},
	{ &GPCRH5,	PortH5_Ctrl		},
	{ &GPCRH6,	PortH6_Ctrl		},

	{ &GPCRI0,	PortI0_Ctrl		},
	{ &GPCRI1,	PortI1_Ctrl		},
	{ &GPCRI2,	PortI2_Ctrl		},
	{ &GPCRI3,	PortI3_Ctrl		},
	{ &GPCRI4,	PortI4_Ctrl		},
	{ &GPCRI5,	PortI5_Ctrl		},
	{ &GPCRI6,	PortI6_Ctrl		},
	{ &GPCRI7,	PortI7_Ctrl		},

	{ &GPCRJ0,	PortJ0_Ctrl		},
	{ &GPCRJ1,	PortJ1_Ctrl		},
	{ &GPCRJ2,	PortJ2_Ctrl		},
	{ &GPCRJ3,	PortJ3_Ctrl		},
	{ &GPCRJ4,	PortJ4_Ctrl		},
	{ &GPCRJ5,	PortJ5_Ctrl		},
	{ &GPCRJ6,	PortJ6_Ctrl		},

	{ &GPCRK0,	PortK0_Ctrl		},
	{ &GPCRK1,	PortK1_Ctrl		},
	{ &GPCRK2,	PortK2_Ctrl		},
	{ &GPCRK3,	PortK3_Ctrl		},
	{ &GPCRK4,	PortK4_Ctrl		},
	{ &GPCRK5,	PortK5_Ctrl		},
	{ &GPCRK6,	PortK6_Ctrl		},
	{ &GPCRK7,	PortK7_Ctrl		},
	
	{ &GPCRL0,	PortL0_Ctrl		},
	{ &GPCRL1,	PortL1_Ctrl		},
	{ &GPCRL2,	PortL2_Ctrl		},
	{ &GPCRL3,	PortL3_Ctrl		},
	{ &GPCRL4,	PortL4_Ctrl		},
	{ &GPCRL5,	PortL5_Ctrl		},
	{ &GPCRL6,	PortL6_Ctrl		},
	{ &GPCRL7,	PortL7_Ctrl		},

	{ &GPCRM0,	PortM0_Ctrl		},
	{ &GPCRM1,	PortM1_Ctrl		},
	{ &GPCRM2,	PortM2_Ctrl		},
	{ &GPCRM3,	PortM3_Ctrl		},
	{ &GPCRM4,	PortM4_Ctrl		},
	{ &GPCRM5,	PortM5_Ctrl		},
	{ &GPCRM6,	PortM6_Ctrl		}

};


//-----------------------------------------------------------------------------
// The function of GPIO register init.
//-----------------------------------------------------------------------------
void Init_GPIO(void)
{
	BYTE index;
	index=0x00;
	while(index<(sizeof(asInitGPIOReg)/sizeof(sInitGPIOReg)))
	{
		*asInitGPIOReg[index].reg=asInitGPIOReg[index].value;
		index++;
	}
}

//----------------------------------------------------------------------------
// Hook function of SCI high
//----------------------------------------------------------------------------
void Hook_SCION(void)
{
  SCI_ON();
}

//----------------------------------------------------------------------------
// Hook function of SCI low
//----------------------------------------------------------------------------
void Hook_SCIOFF(void)
{
  SCI_OFF();
}

//----------------------------------------------------------------------------
// Hook function of SMI high
//----------------------------------------------------------------------------
void Hook_SMION(void)
{
  SMI_ON();
}

//----------------------------------------------------------------------------
// Hook function of SMI low
//----------------------------------------------------------------------------
void Hook_SMIOFF(void)
{
  SMI_OFF();
}

//----------------------------------------------------------------------------
// Hook function of A20 high
//----------------------------------------------------------------------------
void Hook_A20ON(void)
{
#if Support_GATEA20
   GATEA20_ON();
#endif
}

//----------------------------------------------------------------------------
// Hook function of A20 low
//----------------------------------------------------------------------------
void Hook_A20OFF(void)
{
#if Support_GATEA20
  GATEA20_OFF();
#endif
}

//----------------------------------------------------------------------------
// Hook function of keyboard reset low
//----------------------------------------------------------------------------
void Hook_KBRSTON(void)
{
  KBRST_ON();
}

//----------------------------------------------------------------------------
// Hook function of keyboard reset high
//----------------------------------------------------------------------------
void Hook_KBRSTOFF(void)
{
  KBRST_OFF();
}

//----------------------------------------------------------------------------
// Hook function of NUM LED ON
//----------------------------------------------------------------------------
void Hook_NUMLED_ON(void)
{
  //if (cOsLedCtrl.fbit.cOL_CtrlRight ==0 )
  //  NUMLED_ON();
}

//----------------------------------------------------------------------------
// Hook function of NUM LED OFF
//----------------------------------------------------------------------------
void Hook_NUMLED_OFF(void)
{
  //if (cOsLedCtrl.fbit.cOL_CtrlRight ==0 )
  //  NUMLED_OFF();
}

//----------------------------------------------------------------------------
// Hook function of CAP LED ON
//----------------------------------------------------------------------------
void Hook_CAPLED_ON(void)
{
#if Support_CAPSLOCKLED
  CAPS_LED_ON();
#endif //#if Support_CAPSLED
#if Support_CAPSLOCKLED_byBIOS
  SET_MASK(OEM_BIOS_LED_STATUS, CapsLockOn); // Set flag for CapsLock On
  ECSMI_SCIEvent(CAPSLOCK_LED_ON);
#endif //#if Support BIOS control CAPS LED
}

//----------------------------------------------------------------------------
// Hook function of CAP LED OFF
//----------------------------------------------------------------------------
void Hook_CAPLED_OFF(void)
{
#if Support_CAPSLOCKLED
    CAPS_LED_OFF();
#endif //#if Support_CAPSLED
#if Support_CAPSLOCKLED_byBIOS
  CLEAR_MASK(OEM_BIOS_LED_STATUS, CapsLockOn); // Set flag for CapsLock Off
  ECSMI_SCIEvent(CAPSLOCK_LED_OFF);
#endif //#if Support BIOS control CAPS LED
}



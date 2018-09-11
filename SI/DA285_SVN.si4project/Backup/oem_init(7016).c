//*****************************************************************************
//
//  oem_init.c
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
#include "..\..\api\i2c\i2c_drv.h"


//
//*****************************************************************************

void EnableHW8sReset(void)
{
 	GCR10 = 0x00;          		 					// set reset counter 10s
	SET_MASK(GCR8, BIT4);  						// enable 8s EC reset 
	RSTStatus1 = SPCTRL4;

	if(IS_MASK_SET(RSTStatus1, BIT1))
	{
		PWRBTN_WDTFLAG = TRUE;
	}
}

void Oem_StartUp(void)
{

}


//----------------------------------------------------------------------------
// Oem_StartUp
//----------------------------------------------------------------------------
void EC_Restart_System(void)
{
	if((BRAM[63]==0x55)&&(BRAM[62] == 0x5A))
	{
		BRAM[63]=0x00;
		BRAM[62]=0x00;
		CLEAR_MASK(KBHISR,SYSF);	
        Ccb42_SYS_FLAG=0;
		DelayDoPowerSeq=100;

	}
}

const sREG_INIT_DEF Init_SMBus_table[] = 
{
/*
	{ &SMB4P7USL		, 0x72		},	//100K (SMBus default clock frequency select PLL/2) Anthony modify
	{ &SMB4P0USH  		, 0x68      	},  	
	{ &SMB300NS     		, 0x08      	},  	
	{ &SMB250NS     		, 0x09      	},    //W021: change 0x08 to 0x09  	
	{ &SMB25MS      		, 0x19      	},   		
	{ &SMB45P3USL   		, 0xFF      	},		
	{ &SMB45P3USH   		, 0x03      	},   		
	{ &SMB4P7A4P0H		, 0x00      	},  	
	{ &SCLKTS_A			, 0x02      	},	// set clock 100khz
	*/
	{ &SMB4P7USL        	, 0x16          	},  //98K
	{ &SMB4P0USH        	, 0x26      	},  
	{ &SMB300NS     		, 0x03      	},  
	{ &SMB250NS     		, 0x14      	},  
	{ &SMB25MS      		, 0x19      	},          
	{ &SMB45P3USL   		, 0xB7      	},      
	{ &SMB45P3USH   		, 0x01      	},          
	{ &SMB4P7A4P0H      	, 0x00      	},  
	{ &SCLKTS_A         	, 0x10      	},
	{ &SCLKTS_B         	, 0x10      	},
	{ &SCLKTS_C         	, 0x10      	},
//	{ &SCDCR2           	, 0x15      	},  // set CLK_SMB_DIV_SEL = 0x05 
	{ &SCDCR2           	, 0x11      	},  // SHAWN20150416: SMB Clock = PLL 48 / 2 = 24M.

	{ &HOCTL2_A 		, 0x01      },  
	{ &HOCTL_A   		, 0x03      },  
	{ &HOCTL_A  		, 0x01      },  
	{ &HOSTA_A    		, 0xFF      },  
	{ &HOCTL2_B   		, 0x01      },  
	{ &HOCTL_B    		, 0x03      },	
	{ &HOCTL_B    		, 0x01      },	
	{ &HOSTA_B    		, 0xFF      },	
	{ &HOCTL2_C			, 0x01		},	 
	{ &HOCTL_C			, 0x03		},	 
	{ &HOCTL_C			, 0x01		},	 
	{ &HOSTA_C			, 0xFF		},
	{ &HINSTC2      , 0x30  },             
#ifdef SMBusChannel4Support
	{ &HOCTL2_D			, 0x01		},	 
	{ &HOCTL_D			, 0x03		},	 
	{ &HOCTL_D			, 0x01		},	 
	{ &HOSTA_D			, 0xFF		},
#endif

};
void Init_SMBus_Regs(void)
{
	BYTE index = 0x00;
	uchar_8 *Tmp_XPntr;
    while(index < (sizeof(Init_SMBus_table)/sizeof(sREG_INIT_DEF)))
    {
        Tmp_XPntr = Init_SMBus_table[index].address;
		*Tmp_XPntr = Init_SMBus_table[index].initdata;
        index ++;
    }
}

//----------------------------------------------------------------------------
// The function of init. registers
//----------------------------------------------------------------------------
//const struct REG_INIT_DEF code reg_init_table[] =
const sREG_INIT_DEF reg_init_table[] =
{
// DAC
    { &DACPWRDN    ,0x00                },   //

//PWM
    { &PWMODENR    ,0x00        },  // PWM output open-drain
    { &TSWCTRL    ,0x0A        },  // Enable TACH0A,TACH1A speed.
    { &GCR15     ,0xC0        },  // Fix the issue of black screen when plug out 80port.
    { &GCR29     ,0x01        },  // 
};

void Init_Regs(void)
{
  BYTE index = 0x00;  
  uchar_8 *Tmp_XPntr;
  
  while(index < (sizeof(reg_init_table)/sizeof(sREG_INIT_DEF)))
  {
    Tmp_XPntr = reg_init_table[index].address;
    *Tmp_XPntr = reg_init_table[index].initdata;
    index ++;
  }
  
}


//----------------------------------------------------------------------------
// Oem_Initialization - Initialize the registers, data variables, and oem 
// functions after kernel init. function.
//----------------------------------------------------------------------------
void Oem_Initialization(void)
{
	BYTE Temp8;

	#if Auto_Detect_ITE_chip
	Auto_Setting_ITE_CHIP();
	#endif //Auto_Init_ITE_CHIP  

	if( (pPWRRSTMark1==0x78) && (pPWRRSTMark2==0x87) )
	{
		pPWRRSTCnt = 1;
		OTP_RESET_HI();
	}

	 
	Init_GPIO();
	
	CLEAR_MASK(SSCR, BIT0);			//  for LPC and espi work normally
 #if 1
    SET_MASK(GCR9,PWSWDT2GPEN);
    GPCRI5= ALT;
 #endif 	
// EC add for mirror function 
	EC_Mirror_release( );
// EC add for restart system 
	//EC_Restart_System( );
//	SetUSBExTimer();

// --------------------------------------------------------------------------
//	init Regs
//---------------------------------------------------------------------------
	Init_Regs();

	// config ECPM Clock gate
	CGCTRL2R &= 0x8F;
  	CGCTRL3R &= 0x10;
  	CGCTRL4R &= 0x00;

// --------------------------------------------------------------------------
//	init PECI
//---------------------------------------------------------------------------
#if (PLATFORM_CHIP == INTEL)
	Init_PECI();
#endif

// --------------------------------------------------------------------------
//	init SMBus
//---------------------------------------------------------------------------

	Init_SMBus_Regs();	

	#if IS_SUPPORT_SMB4
	i2c_drv_channel_initial_handler(I2C_DEV_1);
	#endif

	#if IS_SUPPORT_SMB5
	i2c_drv_channel_initial_handler(I2C_DEV_2);
	#endif

	#if IS_SUPPORT_SMB6
	i2c_drv_channel_initial_handler(I2C_DEV_3);
	#endif

	

//---------------------------------------------------------------------------
// Init ADC Function
//---------------------------------------------------------------------------
	Init_ADC();
	//Init_VC(3);  // Init All VC Channel.	// CMW 20121022

//---------------------------------------------------------------------------
// Init PWM Function
//---------------------------------------------------------------------------
	OemPwmSetting(FAN_PWM_Channel, FAN_PWM_Prescaler, 23000, 0);	 // FAN PWM Initial
	PWM_Channelx_Disable(FAN_PWM_Channel);

	OemPwmSetting(LOGOLED_PWM_Channel,LOGOLED_PWM_Prescaler,500,0);
	PWM_Channelx_Disable(LOGOLED_PWM_Channel);
	
	#if Support_PWRBTN_LED
	OemPwmSetting(PWRLED_PWM_Channel,PWRLED_PWM_Prescaler,500,0);
	PWM_Channelx_Disable(PWRLED_PWM_Channel);
	#endif

//---------------------------------------------------------------------------
// MISC Init
//---------------------------------------------------------------------------
	//EnableHW8sReset( );	
	InitSysMemory2ECRam();	  // H2RAM function
	#if Support_RFID
	DisableAllInterrupt();
	eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,0xFB,0xFF);	//
	if(eEEPROMData != 0xFF)
	{
		if(eEEPROMData == 0x03)
		{
				Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N5,0x00);
				Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N1,0x00);
				Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N2,0x00);
				Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index,0x00);
		}
		if(eEEPROMData == 0x01)
		{
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N2,0x00);
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index,0x00);
		}
	}
	eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,0xDF,0xFF);	//
	if(eEEPROMData != 0xFF)
	{
		if(eEEPROMData == 0x01)
		{
			Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N7,0x00);
		  	Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_index2,0x00);
		}
	}
	EnableAllInterrupt();
	initRFIDProtect();	
	#endif //Support_RFID	

#if(uMBID_LoadRom)
    uMBID = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7 ,EEPROM_MBID);
#else
    uMBID = Current_MBID;
#endif

#if Support_DASH
  //i2c_swap_channel(0,3);  // Slave A interface is located at SMCLK3/SMDAT3
  Init_Dash();
#endif	  //DASH Enable 

  pWorkAround = 0x01;
  LED_Initial();
	Edge_Initial();
	Gsensor_Initial();
	Battery_Initial();
	ChargerIC_Initial();
  AC_DROP_COUNT = 0x00;
		  
//  Temp8 = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_DevStus);
//  if(IS_MASK_SET(Temp8,EEPROM_DevStus_USB))
//  { USB_PWREn; }
//  else
//  { USB_PWRDis;}
		
	#if chPWSW1WDT
	OEM_PWRSWWDTEn(); 
	#endif
	
	#if !Support_SMTADP
	SetPowerBatteryparameter();
	#endif // !Support_SMTADP
  
#if KB_Primary_Key
    SET_MASK(u4EcCapability1, Fn_primary);
#endif 
    pLastLPCstatusSET;

#if Support_TYPE_C
		OEM_TYPE_C_init_hook();
		UcsiInit();
#endif //Support_TYPE_C	

	
//---------------------------------------------------------------------------
// Init Variable State
//---------------------------------------------------------------------------	
	if( WinFlashMark == exitFollowMode )
	{
		WinFlashMark = 0x00;
	}
	AC_DROP_COUNT = 0x00;
		
	Temp8 = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7 ,EEPROM_DevStus);
	if(IS_MASK_SET(Temp8,EEPROM_DevStus_USB))
	{ USB_PWREn; }
	else
	{ USB_PWRDis;}
	if(IS_MASK_SET(Temp8,EEPROM_DevStus_AOU_DC_Dis))
	{ DisAOU_DC_En; }
	else
	{ DisAOU_DC_Dis;}	
	  
	LID_readytime = 11;
	SysPowState=SYSTEM_S5;
	pWorkAround = 0x01;

	FAN_ID_TYPE = Fan_Null;

  	tmpEvent = NULL_EVENT;
  	tmpSSKey2 = 0xFF;
   
#if KB_Primary_Key
	SET_MASK(u4EcCapability1, Fn_primary);
#endif 
	pLastLPCstatusSET;
	
	ADPI_MUL = ClassicAdaptor_ADPI_MUL; //Default setting the ADPI_MUL is 20V.

#if Support_Thermal_monitor
  Thermal_monitor_En;
#endif 

#if EnhancedPcBeepSupport
//  if(IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepActive))
//  {
//		SET_MASK(EnhancedPcBeep_Status,BeepActive);
//		SET_MASK(EnhancedPcBeep_Status,BeepBusy);
//  	EnhancedPcBeep_DATA[0] = 0x02;
//  	EnhancedPcBeep_DATA[1] = 0x82;
//  	EnhancedPcBeep_ID = 0x01;
//  }
  OEM_ENHANCED_PC_BEEP_Init_HOOK();
#endif

#if Support_BOTTOM_TAMPER_FUNC
   CheckBottomTamper();
#endif // SUPPORT_BOTTOM_TAMPER_FUNC

#ifdef OEM_Hook_Init_support
  OEM_HOOK_Init();
#endif
}


//----------------------------------------------------------------------------
// The function of clearing external ram for OEM code
// Clear external ram (0x0008 0000~0x0008 0FFF)
//----------------------------------------------------------------------------
void Init_ClearRam(void)
{
  uchar_8 * byte_register_pntr;
  BYTE temp0,temp1,temp2,temp3;

  temp0 = pPWRRSTMark1;
  temp1 = pPWRRSTMark2;
  temp2 = WinFlashMark;
  temp3 = WinFlashMark2;
  byte_register_pntr=(uchar_8 *)(EC_RAMBase+0x0000);
  while(byte_register_pntr<((uchar_8 *)(EC_RAMBase+0x2000)))// Clear external ram (0x100~0xFFF)
  {
      *byte_register_pntr=0;
      byte_register_pntr ++;
  }

  byte_register_pntr=(uchar_8 *)(EC_Register_Base_Address+0x2200);
  while(byte_register_pntr<((uchar_8 *)(EC_Register_Base_Address+0x2280)))// Clear Batt ram BANK0 (0x2200~0x227F)
  {
      *byte_register_pntr=0;
      byte_register_pntr ++;
  }

  pPWRRSTMark1 = temp0;
  pPWRRSTMark2 = temp1;
  WinFlashMark = temp2;
  WinFlashMark2 = temp3;
}

void Clear_RAM_Range(uchar_8 *nAddr, WORD nSize)
{
    uchar_8 * Tmp_XPntr = nAddr;
    for( ITempW01=0; ITempW01<=nSize; ITempW01++ )
    {
        *Tmp_XPntr = 0;
        Tmp_XPntr++;
    }
}


//----------------------------------------------------------------------------
// Hook_ECExitFollowMode
//----------------------------------------------------------------------------
void Hook_ECExitFollowMode(void)
{
  CLEAR_MASK(M_BATStopChgStatH, ECFLASH);
}

//----------------------------------------------------------------------------
// EC leaves follow mode or EC scatch ROM and jump to main function
//  Note:
//      1. Before jump to main function if want to keep EC setting [return(0x33);] is necessary.
//      2. If you don't understand the use of Hook_ECRetunrMainFuncKeepCondition function,
//          don't change anything.
//      3. Always at bank0 (0x9000)
//----------------------------------------------------------------------------
BYTE Hook_ECRetunrMainFuncKeepCondition(void)
{
    if(WinFlashMark==exitFlashMark1 && WinFlashMark2==exitFlashMark2)
    {
        return(0x33);
    }
    else
    {
        return(0x00);
    }
}


#if chPWSW1WDT
void OEM_PWRSWWDTEn()
{
	// Disable PWRSW WDT Timer2
	CLEAR_MASK(GCR8,PWSW2EN);

	// Enable PWRSW WDT Timer1
	GCR11 |= PWSW8s;				// set PWSW WDT Timer to 7 sec.
	GCR9  |= (PWSW1EN1|PWSW1EN2);
}

void OEM_RESETPWSWTIMER1()
{
	SET_MASK(GCR9,PWSW1EN2);
}
#endif


#if Auto_Detect_ITE_chip
void Auto_Setting_ITE_CHIP(void)
{
   WORD temp16;
   temp16 = (ECHIPID1 << 8) + ECHIPID2;
   if ((temp16 == IT8586) && (RSTS != IT8586_Init_RSTS)){
    RSTS = IT8586_Init_RSTS;
   } else if ((temp16 == IT8986) && (RSTS != IT8986_Init_RSTS)){ 
    RSTS = IT8986_Init_RSTS;
   }
}
#endif //Auto_detect_ITEEC_chip


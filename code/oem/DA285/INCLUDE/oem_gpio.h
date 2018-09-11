//*****************************************************************************
//
//  oem_gpio.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************
 
#ifndef OEM_GPIO_H
#define OEM_GPIO_H


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
#define HighActive      1
#define LowActive       0

//-----------------------------------------------------------------
// GPIO Registers define
//-----------------------------------------------------------------
/*  General Control Register (GCR)  */
// BIT 7     -  GPIOB5 HIGH Follow LPCRST# HIGH-to-LOW
// BIT 6     -  WUI7 Enabled
// BIT 5     -  WUI6 Enabled
// BIT [4:3] -  Reserved
// BIT [2:1] -  [00] Reserved
//              [01] LPCRST# in GPIOB7
//              [10] LPCRST# in GPIOD2
//              [11] LPCRST# is dsiabled
// BIT 0     -  GFLE Set (GFLES0)

#define GCR_Init      0x06             // LPCRST# is dsiabled

/*  General Control 2 Register (GCR2)  */
// BIT 7     -  TACH2Enable (TACH2E)
// BIT 6     -  Reserved
// BIT 5     -  SMBus Channel 3 Enable (SMB3E), GPIOH1(94) as SM_CLK, GPIOH2(95) as SM_DAT
// BIT 4     -  PECI Enable (PECIE)
// BIT 3     -  TMB1 Enable (TMB1EN)
// BIT 2     -  TMB0 Enable (TMB0EN)
// BIT 1     -  TMA1 Enable (TMA1EN)
// BIT 0     -  TMB0 Enable (TMA0EN)

#define GCR2_Init     0x00             

//-----------------------------------------------------------------
// OEM Phase Setting
//-----------------------------------------------------------------

#define B_phase       FALSE            // if B_phase == TRUE , then GPIO is for B_phase use only.
#define C_phase       FALSE            // if C_phase == TRUE , then GPIO is for C_phase use only.

//[-start-130918-T130124_001-modify]//
//-----------------------------------------------------------------
// GPIO Port A Registers define
//-----------------------------------------------------------------
#define PortA_Init    0x80              // PORT Data

#define PortA0_Ctrl   INPUT            // 24   LOGO_LED# // POWER LED,Low active
#define PortA1_Ctrl   INPUT             // 25   AOU_DET#
#define PortA2_Ctrl   OUTPUT            // 28   EC_FAN_PWM
#define PortA3_Ctrl   OUTPUT            // 29	 KBD_BL_PWM
#define PortA4_Ctrl   ALT            	  // 30   I2C_CLK_BT0
#define PortA5_Ctrl   ALT            	  // 31   I2C_DATA_BT0
#define PortA6_Ctrl   (INPUT | PULL_DW) // 32   BEEP# // Will be "Output" when using..
#define PortA7_Ctrl   INPUT             // 34   PWRBTN_LED#

//-----------------------------------------------------------------
// GPIO Port B Registers define
//-----------------------------------------------------------------
#define PortB_Init    0x00              // PORT Data

#define PortB0_Ctrl   INPUT             // 108  ACPRN(ACIN)
#define PortB1_Ctrl   INPUT             // 109  LID_SW#
#define PortB2_Ctrl   INPUT             // 123  GSENSE_INT
#define PortB3_Ctrl   INPUT             // 110  ON/OFFBTN#(PWRSWITCH#)
#define PortB4_Ctrl   (INPUT | PULL_DW) // 111  NC
#define PortB5_Ctrl   OUTPUT            // 126  GATEA20
#define PortB6_Ctrl   OUTPUT            // 4    KBRST#
#define PortB7_Ctrl   (INPUT | PULL_DW)// NC


//-----------------------------------------------------------------
// GPIO Port C Registers define
//-----------------------------------------------------------------
#define PortC_Init    0x09              // PORT Data

#define PortC0_Ctrl   OUTPUT            // 119  3V_LAN_EN#//ACOFF
#define PortC1_Ctrl   ALT               // 115  _EC_SMB_CK1
#define PortC2_Ctrl   ALT               // 116  _EC_SMB_DA1
#define PortC3_Ctrl   OUTPUT            // 56   M1_DRV
#define PortC4_Ctrl   OUTPUT            // 120  OTP_RESET // 14" only, high active
#define PortC5_Ctrl   OUTPUT            // 57   M2_DRV#
#define PortC6_Ctrl   OUTPUT            // 124  SUSP#
#define PortC7_Ctrl   INPUT            // 16   EC_RX

//-----------------------------------------------------------------
// GPIO Port D Registers define
//-----------------------------------------------------------------
#define PortD_Init    0x00              // PORT Data

#define PortD0_Ctrl   INPUT             // 18   PM_SLP_S3#
#define PortD1_Ctrl   INPUT             // 21   SYS_LAN_WAKE#//DCIN_ATTACHED2
#define PortD2_Ctrl   INPUT             // 22   PLT_RST#
#define PortD3_Ctrl   OUTPUT            // 23   EC_SCI#
#define PortD4_Ctrl   OUTPUT             // 15   EC_SMI#//+1.05VS_VDDPPG
#define PortD5_Ctrl   INPUT              // 33  //DOCK_USB2_BUS_EN# A485D00032+ BPWRG //PD_VBUS_C_CTRL1_EC
#define PortD6_Ctrl   (INPUT | PULL_DW) // 47   EC_FAN_SPEED
#define PortD7_Ctrl   OUTPUT            // 48   AOU_CTL3

//-----------------------------------------------------------------
// GPIO Port E Registers define
//-----------------------------------------------------------------
#define PortE_Init    0x84              // PORT Data    --default high-A285D00049: fix usb3.0 identify abnormal

#define PortE0_Ctrl   ALT             // 19   _EC_SMB_CK4
#define PortE1_Ctrl   INPUT             // 82   VDDPALW_PWRGD
#define PortE2_Ctrl   OUTPUT            // 83   EC_ON
#define PortE3_Ctrl   OUTPUT            // 84   AOU_CTL1
#define PortE4_Ctrl   OUTPUT             // Input->Output David modify for SCH Change 180408
#define PortE5_Ctrl   OUTPUT            // 35   CP_RESET#
#define PortE6_Ctrl   INPUT            // 17   EC_TX
#define PortE7_Ctrl   OUTPUT             // 20		_EC_SMB_DA4

//-----------------------------------------------------------------
// GPIO Port F Registers define
//-----------------------------------------------------------------
#define PortF_Init    0x02              // PORT Data

#define PortF0_Ctrl   OUTPUT            // 85   AOU_EN
#define PortF1_Ctrl   OUTPUT            // 86   PBTN_OUT#
#define PortF2_Ctrl   ALT               // 87   _EC_SMB_CK2
#define PortF3_Ctrl   ALT               // 88   _EC_SMB_DA2
#define PortF4_Ctrl   (INPUT | PULL_DW) // 89   CP_CLK // ALT when SetS0GPIO()
#define PortF5_Ctrl   (INPUT | PULL_DW) // 90   CP_DATA // ALT when SetS0GPIO()
#define PortF6_Ctrl   ALT               // 117  SMCLK2
#define PortF7_Ctrl   ALT               // 118  SMDAT2

//-----------------------------------------------------------------
// GPIO Port G Registers define
//-----------------------------------------------------------------
#define PortG_Init    0x00              // PORT Data

#define PortG0_Ctrl   OUTPUT            // 106  CP_BYPASS
#define PortG1_Ctrl   OUTPUT            // 107  SYSON
#define PortG2_Ctrl   OUTPUT            // 100  EC_MUTE#		//EC_BKOFF# for FVT-R
#define PortG3_Ctrl   INPUT             // 101  SPI_FSCE#
#define PortG4_Ctrl   INPUT             // 102  SPI_FMOSI#
#define PortG5_Ctrl   INPUT             // 103  SPI_FMISO#
#define PortG6_Ctrl   OUTPUT	            // 104  APUPWR_EN
#define PortG7_Ctrl   INPUT             // 105  SPI_FSCK

//-----------------------------------------------------------------
// GPIO Port H Registers define
//-----------------------------------------------------------------
#define PortH_Init    0x40              // PORT Data

#define PortH0_Ctrl   OUTPUT            // 93   EC_RSMRST#
#define PortH1_Ctrl   (INPUT | PULL_DW) // 94   EC_SMB_CK3 // ALT when SetS0GPIO()
#define PortH2_Ctrl   (INPUT | PULL_DW) // 95   EC_SMB_DA3 // ALT when SetS0GPIO()
#define PortH3_Ctrl   OUTPUT            // 96   PWR_GOOD_D
#define PortH4_Ctrl   OUTPUT            // 97   BATT_CHG_LED#
#define PortH5_Ctrl   OUTPUT            // 98   EC_BKOFF#
#define PortH6_Ctrl   INPUT            // 99   0.9VSPG//ADAPTER_ID_ON#
#define PortH7_Ctrl   INPUT				// 11   NC

//-----------------------------------------------------------------
// GPIO Port I Registers define
//-----------------------------------------------------------------
#define PortI_Init    0x00              // PORT Data

#define PortI0_Ctrl   INPUT            // 66   KBD_ID//EC_ON2 //A485D00021  ALT->Input
#define PortI1_Ctrl   INPUT		           // 67   _ADC1 for Main BATT_TEMP
#define PortI2_Ctrl   INPUT               // 68   _ADC2 for Second BATT_TEMP
#define PortI3_Ctrl   ALT               // 69   _ADC3 for Board_ID
#define PortI4_Ctrl   INPUT//ALT               // 70   _ADC4 for FAN_ID //A285D00008 Add fan table
#define PortI5_Ctrl   ALT               // 71   ADP_I
#define PortI6_Ctrl   ALT		            // 72   TEMBER_DETECT#
#define PortI7_Ctrl   ALT               // 73   PSYS

//-----------------------------------------------------------------
// GPIO Port J Registers define
//-----------------------------------------------------------------
#define PortJ_Init    0x00              // PORT Data

#define PortJ0_Ctrl   INPUT             // 76   INT#_TYPEC
#define PortJ1_Ctrl   INPUT             // 77   PM_SLP_S5#
#define PortJ2_Ctrl   INPUT             // 78   IMVPPOK
#define PortJ3_Ctrl   (INPUT | PULL_UP) // 79   MAINPWON for FVT phase, GPU_VR_HOT# for SDV phsae.
#define PortJ4_Ctrl   INPUT      //OUTPUT // A485D00001  for EE change    // 80   H_PROCHOT_EC
#define PortJ5_Ctrl   INPUT             // 81   APU_ENBKL
#define PortJ6_Ctrl   OUTPUT            // 128  AC_PRESENT_R
//[-end-130918-T130124_001-modify]//

//-----------------------------------------------------------------
// GPIO Port K Registers define
//-----------------------------------------------------------------
#define PortK_Init		0x00            // PORT Data  

#define PortK0_Ctrl		INPUT			//	REAR_EJECT_LEVER#
#define PortK1_Ctrl		INPUT			//	   KB_FN
#define PortK2_Ctrl		INPUT			//	APU_THERMTRIP#//SPI_PWR_DISABLE
#define PortK3_Ctrl		INPUT			//	DCOVER_SW_R#
#define PortK4_Ctrl		OUTPUT			//	EC_WAKE#
#define PortK5_Ctrl		OUTPUT			//	USB_ON#
#define PortK6_Ctrl		OUTPUT		//	PWR_STATUS_LED#
#define PortK7_Ctrl		INPUT			//	KBD_BL_DTCT#
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// GPIO Port L Registers define
//-----------------------------------------------------------------
#define PortL_Init		0x00//0x80            // PORT Data  //A285D00064 Enable WLan power when EC initile

#define PortL0_Ctrl		OUTPUT			////USBC_USB2_BUS_EN# //A485D00032+	EC_ON2//TP4RST
#define PortL1_Ctrl		INPUT			//	HP_JACK_IN
#define PortL2_Ctrl		OUTPUT			//	S1_DRV
#define PortL3_Ctrl		OUTPUT			//	S2_DRV#
#define PortL4_Ctrl		OUTPUT			//	M_TRCL
#define PortL5_Ctrl		OUTPUT			//	S_TRCL
#define PortL6_Ctrl		OUTPUT			//	TP4RST//OTG
#define PortL7_Ctrl		OUTPUT			//	3V_WLAN_EN#//PWRON_DOCK#
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// GPIO Port M Registers define
//-----------------------------------------------------------------
#define PortM_Init		0x00            // PORT Data  

#define PortM0_Ctrl		ALT			//	LPC_AD0_ESPI_IO0
#define PortM1_Ctrl		ALT			//	LPC_AD1_ESPI_IO1
#define PortM2_Ctrl		ALT			//	LPC_AD2_ESPI_IO2
#define PortM3_Ctrl		ALT			//	LPC_AD3_ESPI_IO3 
#define PortM4_Ctrl		ALT			//	CLK_PCI_LPC_ESPI_CLK
#define PortM5_Ctrl		ALT			//	LPC_FRAME_N_ESPI_CS_N
#define PortM6_Ctrl		ALT			//	INT_SERIRQ


//------------------------------------------------------------------------------
// S.B.
//------------------------------------------------------------------------------
#define SCI_ON()        CLEAR_MASK(GPDRD,BIT(3))
#define SCI_OFF()       SET_MASK(GPDRD,BIT(3))
#define SCI_LOW()       CLEAR_MASK(GPDRD,BIT(3))
#define SCI_HI()        SET_MASK(GPDRD,BIT(3))

#define SMI_ON()        CLEAR_MASK(GPDRD,BIT(4))
#define SMI_OFF()       SET_MASK(GPDRD,BIT(4))

#if Support_GATEA20
#define GATEA20_ON()    SET_MASK(GPDRB,BIT(5))
#define GATEA20_OFF()   CLEAR_MASK(GPDRB,BIT(5))
#define GATEA20_HI()    SET_MASK(GPDRB,BIT(5))
#define GATEA20_LOW()   CLEAR_MASK(GPDRB,BIT(5))
#endif

#define KBRST_ON()      CLEAR_MASK(GPDRB,BIT(6))
#define KBRST_OFF()     SET_MASK(GPDRB,BIT(6))

#define Read_LPC_RST()  IS_MASK_SET(GPDRD,BIT(2))
//------------------------------------------------------------------------------
// TYPE C
//------------------------------------------------------------------------------

#if Support_TYPE_C
#define Read_DCIN_ATTACHED2()    //IS_MASK_CLEAR(GPDRD,BIT(1))

#define PD_VBUS_CTRL1_EN_SDV()  //SET_MASK(GPDRD,BIT(5))
#define PD_VBUS_CTRL1_DIS_SDV() //CLEAR_MASK(GPDRD,BIT(5))
//#define PD_VBUS_CTRL1_EN()  	SET_MASK(GPDRD,BIT(4))
//#define PD_VBUS_CTRL1_DIS() 	CLEAR_MASK(GPDRD,BIT(4))

//#define INT_TYPEC_EN()      SET_MASK(GPDRJ,BIT(0))
//#define INT_TYPEC_DIS()     CLEAR_MASK(GPDRJ,BIT(0))
#define Read_TYPEC_INT()      	IS_MASK_CLEAR(GPDRJ,BIT(0))	//Low active, means event occur
#endif //Support_TYPE_C
//------------------------------------------------------------------------------
// Power sequnce
//------------------------------------------------------------------------------
#define PM_PWRBTN_LOW()   		CLEAR_MASK(GPDRF,BIT(1))
#define PM_PWRBTN_HI()    		SET_MASK(GPDRF,BIT(1))
#define Read_PM_PWRBTN()  		IS_MASK_SET(GPDRF,BIT(1))

#if (PLATFORM_CHIP == INTEL)
#define SUSACK_LOW()      		CLEAR_MASK(GPDRF,BIT(3))
#define SUSACK_HI()       		SET_MASK(GPDRF,BIT(3))
#endif // PLATFORM_CHIP
#define EC_RTCRST_ON()			CLEAR_MASK(GPDRB,BIT(5))
#define EC_RTCRST_OFF()			SET_MASK(GPDRB,BIT(5))

#define RSMRST_LOW()      		CLEAR_MASK(GPDRH,BIT(0))
#define RSMRST_HI()       		SET_MASK(GPDRH,BIT(0))
#define Read_RSMRST()     		IS_MASK_SET(GPDRH,BIT(0))

#if (PLATFORM_CHIP == INTEL)
//
// Intel +3V PCH
//
#define PCH_PWR_EN_OFF()  		CLEAR_MASK(GPDRH,BIT(4))
#define PCH_PWR_EN_ON()   		SET_MASK(GPDRH,BIT(4))
#define Read_PCH_PWR_EN() 		IS_MASK_SET(GPDRH,BIT(4))
#else // PLATFORM_CHIP


#endif // PLATFORM_CHIP

#define AC_PRESENT_LOW()  		CLEAR_MASK(GPDRJ,BIT(6))
#define AC_PRESENT_HI()   		SET_MASK(GPDRJ,BIT(6))

#if (PLATFORM_CHIP == INTEL)
#define PCH_PWROK_LOW()      	CLEAR_MASK(GPDRH,BIT(6))
#define PCH_PWROK_HI()       	SET_MASK(GPDRH,BIT(6))
#define Read_PCH_PWROK()     	IS_MASK_SET(GPDRH,BIT(6))
#else // PLATFORM_CHIP
#define FCH_PWR_GOOD_LOW()   //CLEAR_MASK(GPDRH,BIT(6))
#define FCH_PWR_GOOD_HI()    //SET_MASK(GPDRH,BIT(6))
#define Read_FCH_PWR_GOOD()  //IS_MASK_SET(GPDRH,BIT(6))

#endif // PLATFORM_CHIP

#define SYSON_LOW()       		CLEAR_MASK(GPDRG,BIT(1))
#define SYSON_HI()        		SET_MASK(GPDRG,BIT(1))

#define SUSP_OFF()        		CLEAR_MASK(GPDRC,BIT(6))
#define SUSP_ON()         		SET_MASK(GPDRC,BIT(6))

#define AOU_EN_OFF()      		CLEAR_MASK(GPDRF,BIT(0))
#define AOU_EN_ON()       		SET_MASK(GPDRF,BIT(0))
#define Read_AOU_EN()     		IS_MASK_SET(GPDRF,BIT(0))

#define AOU_CTL1_HI()     		SET_MASK(GPDRE,BIT(3))    // Clr SDP mode.
#define AOU_CTL1_LOW()    		CLEAR_MASK(GPDRE,BIT(3))    // Set SDP mode.

#define AOU_CTL3_HI()     		SET_MASK(GPDRD,BIT(7))    // Clr SDP mode.
#define AOU_CTL3_LOW()    		CLEAR_MASK(GPDRD,BIT(7))    // Set SDP mode.

#define USB_ON_INPUT      		GPCRK5 = INPUT            // SET USB_ON INPUT.
#define USB_ON_OUTPUT     		GPCRK5 = OUTPUT           // SET USB_ON OUTPUT.
#define USB_ON_HIGH()      		SET_MASK(GPDRK,BIT(5))
#define USB_ON_LOW()      		CLEAR_MASK(GPDRK,BIT(5))
//#define USB_ON_ON()      		USB_ON_OUTPUT;USB_ON_LOW();	// Turn On USB_ON.
//#define USB_ON_OFF()      	USB_ON_INPUT    // Turn Off USB_ON.
#define USB_ON_ON()      			USB_ON_HIGH()    // Turn On USB_ON.
#define USB_ON_OFF()      		USB_ON_LOW()    // Turn Off USB_ON.

#if (PLATFORM_CHIP == INTEL)
//
// CPU Vcore
//
#define VR_ON_OFF()       		CLEAR_MASK(GPDRE,BIT(4))
#define VR_ON_ON()        		SET_MASK(GPDRE,BIT(4))
#define Read_VR_ON()      		IS_MASK_SET(GPDRE,BIT(4))
#else
#define APUPWR_EN_OFF()   		CLEAR_MASK(GPDRG,BIT(6))
#define APUPWR_EN_ON()    		SET_MASK(GPDRG,BIT(6))
#define Read_APUPWR_EN()  		IS_MASK_SET(GPDRG,BIT(6))
#endif

#if (PLATFORM_CHIP == INTEL)
#define DPWROK_EC_LOW()   		CLEAR_MASK(GPDRE,BIT(5))
#define DPWROK_EC_HI()    		SET_MASK(GPDRE,BIT(5))
#define Read_DPWROK()     		IS_MASK_SET(GPDRE,BIT(5))

//#define PCH_APWROK_ON()   		SET_MASK(GPDRH,BIT(3))
//#define PCH_APWROK_OFF()  		CLEAR_MASK(GPDRH,BIT(3))

#define VM_PWRON_ON()     		SET_MASK(GPDRG,BIT(7))
#define VM_PWRON_OFF()    		CLEAR_MASK(GPDRG,BIT(7))
#define Read_VMPWR()      		IS_MASK_SET(GPDRG,BIT(7))
#endif

#define EC_ON_HI()        		SET_MASK(GPDRE,BIT(2))//SET_MASK(GPDRL,BIT(0));//A485D00032- for EE change
#define EC_ON_LOW()       		CLEAR_MASK(GPDRE,BIT(2)) //CLEAR_MASK(GPDRL,BIT(0));//A485D00032-for EE change
#define Read_EC_ON()      		IS_MASK_SET(GPDRE,BIT(2))
#define EC_ON2_HI()       		SET_MASK(GPDRE,BIT(4)) //A285D00043 modify for A285 SIT SCH change 180408//SET_MASK(GPDRL,BIT(0)) ////A485D00002 Delete for EE change
#define EC_ON2_LOW()      		CLEAR_MASK(GPDRE,BIT(4)) //A285D00043 modify for A285 SIT SCH change 180408//CLEAR_MASK(GPDRL,BIT(0)) //A485D00002 Delete for EE change

#if (PLATFORM_CHIP == INTEL)
#define Read_VDDQ_PGOOD() 		IS_MASK_SET(GPDRI,BIT(7))
#endif // PLATFORM_CHIP

#define Read_SLPS3()      		IS_MASK_SET(GPDRD,BIT(0))

#if (PLATFORM_CHIP == INTEL)
#define Read_SLPS4()      		IS_MASK_SET(GPDRD,BIT(1))
#endif // PLATFORM_CHIP
#if (PLATFORM_CHIP == AMD)
#define Read_EnBeep_Active()   IS_MASK_CLEAR(GPDRD,BIT(1)) //A485D00087:
#endif // PLATFORM_CHIP


#define Read_SLPS5()      		IS_MASK_SET(GPDRJ,BIT(1))

#if (PLATFORM_CHIP == INTEL)
#define Read_PM_SLP_A()   		IS_MASK_SET(GPDRJ,BIT(0))
#define Read_PM_SLP_SUS() 		IS_MASK_SET(GPDRF,BIT(2))
#endif // PLATFORM_CHIP

#define Read_EC_PWRBTN()  		IS_MASK_CLEAR(GPDRB,BIT(3))
//#define Read_EC_NOVO()    		IS_MASK_CLEAR(GPDRJ,BIT(0))

//#define Read_AC_IN()      		((IS_MASK_SET(GPDRB,BIT(0))&&IS_MASK_SET(Attach2status,StableLLevel))||(IS_MASK_SET(GPDRB,BIT(0))&&IS_MASK_CLEAR(Attach2status,StableLLevel)&&IS_MASK_SET(TypeCStatus,TypeCAdpExist))) //A485D00003
#define Read_AC_IN()      		IS_MASK_SET(GPDRB,BIT(0)) //A485D00003 Modify for EE change
#define Read_AC_IN_pin()      		IS_MASK_SET(GPDRB,BIT(0))
#define AC_IN_ALT         		GPCRB0 = ALT              // Set AC_IN Alternate.
#define AC_IN_INPUT       		GPCRB0 = INPUT            // Set AC_IN INPUT.

#define PWRON_DOCK_ON()      //CLEAR_MASK(GPDRL,BIT(7))
#define PWRON_DOCK_OFF()     //SET_MASK(GPDRL,BIT(7))
#define v3VWLAN_EN_ON()			CLEAR_MASK(GPDRL,BIT(7))
#define v3VWLAN_EN_OFF()		SET_MASK(GPDRL,BIT(7))

#define EC_MainPwr_ON()   		SET_MASK(GPDRJ,BIT(3))
#define EC_MainPwr_OFF()  		CLEAR_MASK(GPDRJ,BIT(3))
#define EC_MainPwr()      		IS_MASK_SET(GPDRJ,BIT(3))

#define EC_RX_ALT         		GPCRC7 = ALT              // Set EC_RX Alternate.
#define EC_RX_OUTPUT      		GPCRC7 = OUTPUT           // Set EC_RX OUTPUT.
#define EC_RX_INPUT       		GPCRC7 = INPUT            // Set EC_RX INPUT.
#define DEBUG_RxCLK_HI()  		SET_MASK(GPDRC,BIT(7))
#define DEBUG_RxCLK_LO()  		CLEAR_MASK(GPDRC,BIT(7))

#define EC_TX_ALT         		GPCRE6 = ALT              // Set EC_TX Alternate.
#define EC_TX_OUTPUT      		GPCRE6 = OUTPUT           // Set EC_TX OUTPUT.
#define EC_TX_INPUT       		GPCRE6 = INPUT            // Set EC_RX INPUT.
#define DEBUG_TxDAT_HI()  		SET_MASK(GPDRE,BIT(6))
#define DEBUG_TxDAT_LO()  		CLEAR_MASK(GPDRE,BIT(6))
#define Chk_DEBUG_IN()    		IS_MASK_SET(GPDRE,BIT(6))

#define USB_RE_Enable_OUTPUT      		GPCRE7 = OUTPUT           // A285D00054:
#define USB_RE_Enable_INPUT       		GPCRE7 = INPUT           //A285D00054:

#define USB_RE_Enable_HI()  	SET_MASK(GPDRE,BIT(7))   //A285D00054:
#define USB_RE_Enable_LO()  	CLEAR_MASK(GPDRE,BIT(7)) //A285D00054:


#define Read_AOU_DET()    		IS_MASK_SET(GPDRA,BIT(1))

#if Support_TP_RESET_control
#define TP_RESET_ON()     		SET_MASK(GPDRL,BIT(6)) //SET_MASK(GPDRL,BIT(0))   // A485D00012  // Modify for EE change
#define TP_RESET_OFF()    		CLEAR_MASK(GPDRL,BIT(6))// CLEAR_MASK(GPDRL,BIT(0)) // A485D00012
#endif //Support_TP_RESET_control

#if Support_BYPASS_PAD_control
#define BYPASS_PAD_ON()   		SET_MASK(GPDRG,BIT(0))
#define BYPASS_PAD_OFF()  		CLEAR_MASK(GPDRG,BIT(0))
#endif //Support_BYPASS_PAD_control

#if Support_CP_RESET_control
#define CP_RESET_ON()     		CLEAR_MASK(GPDRE,BIT(5))
#define CP_RESET_OFF()    		SET_MASK(GPDRE,BIT(5))
#endif //Support_CP_RESET_control

#define VGA_AC_BATT_ON()  		//SET_MASK(GPDRG,BIT(0))
#define VGA_AC_BATT_OFF() 		//CLEAR_MASK(GPDRG,BIT(0))

#if NV_GC6
#define FB_CLAMP_ON()     		SET_MASK(GPDRE,BIT(1))
#define FB_CLAMP_OFF()    		CLEAR_MASK(GPDRE,BIT(1))
#define Read_FB_CLAMP()   		IS_MASK_SET(GPDRE,BIT(1))

#define Read_GC6Event_ON() 		IS_MASK_CLEAR(GPDRE,BIT(2))
#endif // NV_GC6



#define Read_BAT_IN()     		IS_MASK_SET(GPDRI,BIT(2))
#define Read_BOARD_IN()   		IS_MASK_SET(GPDRI,BIT(3))
//A485D00014 Start  Change the BAT_IN detection to GPI low triggered  
#define Read_MAIN_BAT_IN()		IS_MASK_CLEAR(GPDRI,BIT(1))  
#define Read_SEC_BAT_IN()		IS_MASK_CLEAR(GPDRI,BIT(2))
//A485D00014 End
#if (PLATFORM_CHIP == INTEL)
#define Read_SUSWARN()    		IS_MASK_SET(GPDRJ,BIT(2))
#define Read_SUSACK()     		IS_MASK_SET(GPDRF,BIT(3))
#endif // PLATFORM_CHIP
#define Read_SUSP()       		IS_MASK_SET(GPDRC,BIT(6))

#if (PLATFORM_CHIP == AMD)
#define Read_VDDAPWROK()  		TRUE //IS_MASK_SET(GPDRF,BIT(6))
#define Read_IMVPPOK()    		IS_MASK_SET(GPDRJ,BIT(2))
#define Read_09VALWS_PWRGD()  	IS_MASK_SET(GPDRE,BIT(1))
#define Read_105VS_VDDPPG()  	IS_MASK_SET(GPDRH,BIT(6))//IS_MASK_SET(GPDRD,BIT(4))
#define Read_09VS_PG()			IS_MASK_SET(GPDRH,BIT(6))
#define PWR_GOOD_D_HI()      	SET_MASK(GPDRH,BIT(3))
#define PWR_GOOD_D_LOW()     	CLEAR_MASK(GPDRH,BIT(3))
#endif // PLATFORM_CHIP
#define SPI_PWR_DISABLE_HI()      	//SET_MASK(GPDRK,BIT(2))
#define SPI_PWR_DISABLE_LOW()     	//CLEAR_MASK(GPDRK,BIT(2))
#define CHK_095VS_PGD()            IS_MASK_SET(GPDRH,BIT(6))  //David+ for SCH Modify
//-----------------------------------------------------------------------------
// LED control
//-----------------------------------------------------------------------------
#if Support_FnLKLED
#define FN_LED_ON()       		CLEAR_MASK(GPDRK,BIT(2))
#define FN_LED_OFF()      		SET_MASK(GPDRK,BIT(2))
#endif

#if Support_MuteLED
#define Mute_LED_ON()     		CLEAR_MASK(GPDRI,BIT(6))
#define Mute_LED_OFF()    		SET_MASK(GPDRI,BIT(6))
#endif

#if Support_Microphone_Mute_LED
#define MICMute_LED_ON()  		CLEAR_MASK(GPDRJ,BIT(3))
#define MICMute_LED_OFF() 		SET_MASK(GPDRJ,BIT(3))
#endif

#if Support_BATTCHGLED         
#define BATTCHG_LED_ON()     	SET_MASK(GPDRH,BIT(4))
#define BATTCHG_LED_OFF()    	CLEAR_MASK(GPDRH,BIT(4))
#endif

#if Support_PWRSTATUSLED         
#define PWRSTATUS_LED_ON()   	SET_MASK(GPDRK,BIT(6))
#define PWRSTATUS_LED_OFF()  	CLEAR_MASK(GPDRK,BIT(6))
#endif

#if Support_CAPSLOCKLED
#define CAPS_LED_ON()       	CLEAR_MASK(GPDRE,BIT(0))
#define CAPS_LED_OFF()      	SET_MASK(GPDRE,BIT(0))
#endif

#if Support_KBD_BL_DETECT
#define Read_KBD_BL_DTCT()   		IS_MASK_CLEAR(GPDRK,BIT(7))
#endif
//-----------------------------------------------------------------------------
// Output control
//-----------------------------------------------------------------------------
#define GS_ST_HI()        		//SET_MASK(GPDRA,BIT(4))  // GS_SELFTEST
#define GS_ST_LOW()       		//CLEAR_MASK(GPDRA,BIT(4))

#define TEST_HI()        		SET_MASK(GPDRA,BIT(0))  // GS_SELFTEST
#define TEST_LOW()       		CLEAR_MASK(GPDRA,BIT(0))

#define v3VLAN_EN_ON()				CLEAR_MASK(GPDRC,BIT(0))
#define v3VLAN_EN_OFF()				SET_MASK(GPDRC,BIT(0))
#define Read_VLAN_PWR()                         IS_MASK_CLEAR(GPDRC,BIT(0))//A285D00041:Fix sysem cannot be WoLAN from S5 (G3 then plugin AC) with magic packet


//#if SW_ISCT
#define AOAC_PWRON()      		//CLEAR_MASK(GPDRG,BIT(6))
#define AOAC_PWROFF()     		//SET_MASK(GPDRG,BIT(6))
//#endif // SW_ISCT

//#define VGA_AC_DET_HI() SET_MASK(GPDRA,BIT(7))
//#define VGA_AC_DET_LO()   CLEAR_MASK(GPDRA,BIT(7))

//#define LAN_PWR_ON_HI()   SET_MASK(GPDRF,BIT(7))
//#define LAN_PWR_ON_LO()   CLEAR_MASK(GPDRF,BIT(7))

#define EC_MUTE_ACT()     		CLEAR_MASK(GPDRG,BIT(2))
#define EC_MUTE_INACT()   		SET_MASK(GPDRG,BIT(2))

#define BEEP_ENABLE_ON()     		CLEAR_MASK(GPDRK,BIT(3))
#define BEEP_ENABLE_OFF()   		SET_MASK(GPDRK,BIT(3))

//#define CMOS_Cam_ON()   CLEAR_MASK(GPDRG,BIT(7))// Camera ON.
//#define CMOS_Cam_OFF()  SET_MASK(GPDRG,BIT(7))  // Camera OFF.

#define TP_CLK_INDW       		GPCRF4 = (INPUT | PULL_DW)  // Set TP CLK Input+Pull down.
#define TP_CLK_ALT        		GPCRF4 = ALT                // Set TP CLK Alternate.
#define TP_DATA_INDW      		GPCRF5 = (INPUT | PULL_DW)  // Set TP DATA Input+Pull down.
#define TP_DATA_ALT       		GPCRF5 = ALT                // Set TP DATA Alternate.

#define SMBus3_CLK_INDW   		GPCRH1 = (INPUT | PULL_DW)  // Set SMBus3 CLK input+pull down.
#define SMBus3_CLK_OUTDW   		GPCRH1 = (OUTPUT | PULL_DW)

#define SMBus3_CLK_ALT    		GPCRH1 = ALT                // Set SMBus3 CLK Alternate.
#define SMBus3_DAT_OUTDW   		GPCRH2 = (OUTPUT | PULL_DW) 

#define SMBus3_DAT_INDW   		GPCRH2 = (INPUT | PULL_DW)  // Set SMBus3 DATA input+pull down.
#define SMBus3_DAT_ALT    		GPCRH2 = ALT                // Set SMBus3 DATA Alternate.

#if (PLATFORM_CHIP == INTEL)
#define PECI_INDW         		GPCRF6 = INPUT              // Set PECI INPUT.
#define PECI_ALT          		GPCRF6 = ALT                // Set PECI Alternate.
#endif // PLATFORM_CHIP

#if Support_EC_GS_ON    
#define EC_GS_ON()        		CLEAR_MASK(GPDRG,BIT(0))
#define EC_GS_OFF()       		{SET_MASK(GPDRG,BIT(0));  CLEAR_MASK(GS.Status, GS_Init);}
#else
#define EC_GS_OFF()       		CLEAR_MASK(GS.Status, GS_Init)
#endif //Support_EC_GS_ON    

#if Support_WLAN_WAKE
#define Read_WLAN_WAKE_IN() 	IS_MASK_CLEAR(GPDRG,BIT(6))
#endif //Support_WLAN_WAKE

#if Support_WLAN_ON_OFF_control
#define BTRF_OFF_ON()
#define BTRF_OFF_OFF()
#endif //Support_WLAN_ON_OFF_control

#if Support_BT_ON_OFF_control
#define BT_OFF_ON()
#define BT_OFF_OFF()
#endif //Support_BT_ON_OFF_control

#define FAN_PWM           		DCR2
#define FAN_PWM_INDW      		GPCRA2 = (INPUT | PULL_DW)  // Set FAN_PWM input+pull down.
#define FAN_PWM_ALT       		GPCRA2 = ALT                // Set FAN_PWM Alt.
#define FAN_PWM_OUT       		GPCRA2 = OUTPUT             // Set FAN_PWM OUTPUT.
#define EC_FAN_PWM_HI()   		SET_MASK(GPDRA,BIT(2))

#define Read_FANID_HIGH()		IS_MASK_SET(GPDRI,BIT(4))  //A285D00010+



#define FAN_SPEED_ALT     		GPCRD6 = ALT                // Set FAN Speed Alt.
#define FAN_SPEED_INDW    		GPCRD6 = (INPUT | PULL_DW)  // Set FAN Speed input+pull down.

#define H_PROCHOT_LOW()     GPCRJ4=INPUT//CLEAR_MASK(GPDRJ,BIT(4))  //A485D00001 Modify for EE change
#define H_PROCHOT_HI()      GPCRJ4=OUTPUT;CLEAR_MASK(GPDRJ,BIT(4))////A485D00001 Modify for EE change


#define BKOFF_OFF()       		CLEAR_MASK(GPDRH,BIT(5))
#define BKOFF_ON()        		SET_MASK(GPDRH,BIT(5))
#define Read_BKOFF()      		IS_MASK_SET(GPDRH,BIT(5))

#define EC_WAKE_OUT_LOW()  		CLEAR_MASK(GPDRK,BIT(4)) // This is EC_WAKE#, just for LID out event use only.
#define EC_WAKE_OUT_HI()   		SET_MASK(GPDRK,BIT(4))


#define Read_ENBKL_IN()   		IS_MASK_SET(GPDRJ,BIT(5))
#define Read_LID_SW_IN()  		IS_MASK_SET(GPDRB,BIT(1))
#define Read_LAN_WAKE_IN()  	IS_MASK_CLEAR(GPDRD,BIT(5))//A285D00043 Modify for A285 SIT SCH change 180408//IS_MASK_CLEAR(GPDRE,BIT(4))
#define Read_SYS_LAN_WAKE_IN()	IS_MASK_CLEAR(GPDRD,BIT(1))

#if (PLATFORM_CHIP == INTEL)
#define pchME_FLAS_HI()   		SET_MASK(GPDRG,BIT(6))        // ME Lock
#define pchME_FLAS_LOW()  		CLEAR_MASK(GPDRG,BIT(6))      // ME unlock
#define pchME_FLAS_INDW   		GPCRG6 = (INPUT | PULL_DW)    // SET ME Input+Pull down.
#define pchME_FLAS_OUT    		GPCRG6 = OUTPUT               // SET ME Output.
#endif // PLATFORM_CHIP
#define OTP_RESET_LOW()     	CLEAR_MASK(GPDRC,BIT(4))
#define OTP_RESET_HI()      	SET_MASK(GPDRC,BIT(4))
#define Read_OTP_RESET()     	IS_MASK_SET(GPDRC, BIT(4))

#define ACOFF_LOW()       		//CLEAR_MASK(GPDRC,BIT(0))
#define ACOFF_HI()        		//SET_MASK(GPDRC,BIT(0))

//#define Read_ACOFF()      		(IS_MASK_SET(GPDRC,BIT(0)) || IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn)) //A485D00005 
#define Read_ACOFF()      		IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn)  ////A485D00005 Modify for EE change

#if EC_Brightness_Ctrl
#define Read_EC_INVT_PWM()  	IS_MASK_SET(GPDRA,BIT(7))
#define EC_INVT_PWM_ALT     	GPCRA7 = ALT
#define EC_INVT_PWM_OUTPUT  	GPCRA7 = OUTPUT
#define EC_INVT_PWM_HI()    	SET_MASK(GPDRA,BIT(7))
#endif  // EC_Brightness_Ctrl

#define PWM_BEEP            	DCR6
#define BEEP_INIT           	(GPCRA6 = PortA6_Ctrl)    // init PWM Beep.
#define BEEP_ALT            	(GPCRA6 = ALT)            // Set PWM BEEP ALT.

#define EC_KBFNON()         	IS_MASK_CLEAR(GPDRK,BIT(1))

//#define ChkHDD_Detect_SDV()     IS_MASK_CLEAR(GPDRD,BIT(4)) // Check HDD detect.
//#define ChkHDD_Detect()     	//TRUE //IS_MASK_CLEAR(GPDRD,BIT(4)) // Check HDD detect.

//#define CHK_GPUVRHOT()      	IS_MASK_CLEAR(GPDRD,BIT(1))

#define ADPID_ON_EN()       	//CLEAR_MASK(GPDRH,BIT(6));ADC_Get_Channelx_Data_Valid(API_ID_Channel)
#define ADPID_ON_DIS()      	//SET_MASK(GPDRH,BIT(6))
#define ADPID_ON_Detect()   	//IS_MASK_CLEAR(GPDRH,BIT(6))

#if Support_TWO_BATT
#define M1_DRV_LOW()       //	CLEAR_MASK(GPDRC,BIT(3))  // A285D00015-
#define M1_DRV_HI()        // 	SET_MASK(GPDRC,BIT(3))    // A285D00015-
#define M2_DRV_LOW()        	CLEAR_MASK(GPDRC,BIT(5))
#define M2_DRV_HI()        	SET_MASK(GPDRC,BIT(5))
#define S1_DRV_LOW()       	CLEAR_MASK(GPDRL,BIT(2))
#define S1_DRV_HI()        	SET_MASK(GPDRL,BIT(2))
#define S2_DRV_LOW()       	CLEAR_MASK(GPDRL,BIT(3))
#define S2_DRV_HI()        	SET_MASK(GPDRL,BIT(3))
#define M_CTRL_LOW()       	CLEAR_MASK(GPDRL,BIT(4))
#define M_CTRL_HI()        	SET_MASK(GPDRL,BIT(4))
#define M_CTRL_GET()        	IS_MASK_SET(GPDRL,BIT(4))
#define S_CTRL_LOW()       	CLEAR_MASK(GPDRL,BIT(5))
#define S_CTRL_HI()        	SET_MASK(GPDRL,BIT(5))
#define S_CTRL_GET()        	IS_MASK_SET(GPDRL,BIT(5))
#define Eject_Enable()       IS_MASK_CLEAR(GPDRK,BIT(0))
#endif 


//-----------------------------------------------------------------------------
// Bottom tamper
//-----------------------------------------------------------------------------
#if Support_BOTTOM_TAMPER_FUNC
//
// D_Cover switch -- 0: Open, 1: Close
//
#define Read_D_Cover_SW() IS_MASK_CLEAR(GPDRK,BIT(3))
#endif

//-----------------------------------------------------------------------------
// DPGU
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Input control
//-----------------------------------------------------------------------------
extern void Init_GPIO(void);
extern void Hook_SCION(void);
extern void Hook_SCIOFF(void);
extern void Hook_SMION(void);
extern void Hook_SMIOFF(void);
extern void Hook_A20ON(void);
extern void Hook_A20OFF(void);
extern void Hook_KBRSTON(void);
extern void Hook_KBRSTOFF(void);
extern void Hook_NUMLED_ON(void);
extern void Hook_NUMLED_OFF(void);
extern void Hook_CAPLED_ON(void);
extern void Hook_CAPLED_OFF(void);


typedef struct InitGPIOReg
{
    uchar_8 *reg;	
	CBYTE	value;
} sInitGPIOReg;


#endif


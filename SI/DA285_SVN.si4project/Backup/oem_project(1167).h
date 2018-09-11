//*****************************************************************************
//
//  oem_project.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author:
//
//*****************************************************************************

#ifndef OEM_Project_H
#define OEM_Project_H

//#include "../../nds/include/config.h"

#define TRUE	1
#define FALSE 	0

//*****************************************************************************
// Kernel code feature
//*****************************************************************************


//-----------------------------------------------------------------------------
// Keyboard scan
//-----------------------------------------------------------------------------
#define KB_FnStickKey           				0       // True : for Fn stick key function



//*****************************************************************************
// HID and Fusion Interface
//*****************************************************************************
//
// HID Descriptor
//

#define ITE_PRODUCT_ID              0x8186

#define SHOW_HID_CUSTOM_SENSOR      1

#define HID_CUSTOM_MODEL_LENGTH     11
#define HID_CUSTOM_MODEL_STRING     {'L', 'e', 'n', 'o','v','o',' ', 'Y', 'o', 'g', 'a'}


//
// USB Descriptor
//
#define USB_STR_DEVICE_NAME_LENGTH  0x20
#define USB_STR_DEVICE_NAME         'I',0,'T',0,'E',0,' ',0,'D',0,'e',0,'v',0,'i',0,'c',0,'e',0,'(',0,'8',0,'3',0,'9',0,'6',0,')',0

//
// HID over USB or I2C
//
#define DEF_HID_OVER_I2C            			1
#define DEF_HID_OVER_USB            			(!DEF_HID_OVER_I2C)


//
// Setting for HID over I2C
//
#define HID_INT_GPDR                			GPDRJ// HID interrupt pin setting
#define HID_INT_GPCR                			GPCRJ1
#define HID_INT_GPDR_BIT           				BIT1
#define HID_INT_ACTIVE_HIGH 					1 		// Alex + 1 For HIGI ACTIVE , 0: FOR LOW ACTIVE

//	#define DEF_HID_OVER_I2C_INT_LOW_LEVEL_TRIGGER
//	#define DEF_I2C_HID_SLAVE_ADDRESS   		0xA0    	// HID Slave ID -> Shawn: EVB test
#define DEF_I2C_HID_SLAVE_ADDRESS   		    0x5C      	// HID Slave ID
#define DEF_HID_I2C_CH              			I2C_F           		// HID I2C Channel
#define DEF_SWAP_TO_CH0             			0

//
// Sensor Fusion Configure
//
#define SENSOR_DEVICE_TYPE          			2
#define DEF_SENSOR_I2C_CH           			I2C_E           // Sensor I2C Channel
#define DEF_SENSOR_ONE_SHOT_MODE                    // Define : Enable sensor one shot mode

#ifdef EC_FEATURE_ONLY
#undef  DEF_SENSOR_I2C_ERR_HANDLER
#else
//#define DEF_SENSOR_I2C_ERR_HANDLER                  // Define : Enable sensor i2c error handler
#endif

#ifdef DEF_SENSOR_I2C_ERR_HANDLER // {
#define ACC_MUST_INIT               				1
#define MAG_MUST_INIT               				1
#define GYO_MUST_INIT               				1
#define ALS_MUST_INIT               				0
#define ACC2_MUST_INIT              				0
#define PRS_MUST_INIT               				0
#endif  // } DEF_SENSOR_I2C_ERR_HANDLER

#define USB_ISR_IN_INTERRUPT       					1

//
// Fusion Library Configure
//
#define DEF_ANDROID         						0

// #define DEF_FUSION_6D       						0
#ifdef __ENABLE_AKM_PG_LIB__ // {
#define DEF_FUSION_6D       						1
#else
#define DEF_FUSION_6D       						1
#endif // } __ENABLE_AKM_PG_LIB__


//
// OS Configure
//
#define DEF_DUAL_OS         					0

//
// OS hot switch Configure
//
#define DEF_OS_HOT_SWITCH  				0

#define OS_HOT_SWITCH_PIN_GPCR      		GPCRB3
#define OS_HOT_SWITCH_PIN_GPDR      		GPDRB       
#define OS_HOT_SWITCH_PIN_GPDR_BIT  		BIT3
#define OS_HOT_SWITCH_MODE_ANDROID 			BIT3


//
// TRACE CONFIGURE
//
#define GPDR_FUSION_IDX             			0
#define GPDR_FUSION_PIN             			(1UL << (GPDR_FUSION_IDX))
#define GPDR_FUSION                 			GPDRA

#define GPDR_HID_IDX                			1
#define GPDR_HID_PIN                			(1UL << (GPDR_HID_IDX))
#define GPDR_HID                    			GPDRA

#define GPDR_EC_IDX                 			2
#define GPDR_EC_PIN               				(1UL << (GPDR_EC_IDX))
#define GPDR_EC                     			GPDRA

//
// always report 12 levels
//
//
#define SENSOR_ALS_CURVE_LENGTH     				(2*6)			// alex + from Vienna
#if 1 // {
#define SENSOR_ALS_CURVE    {   \
		83,      1,              \
		100,     32,             \
		115,     316,            \
		125,     1000,            \
		135,     3162,            \
		149,     10000,            \
	};
#else
#define SENSOR_ALS_CURVE    {   \
		90,      1,              \
		102,     32,             \
		110,     316,            \
		116,     1000,            \
		123,     3162,            \
		125,     10000,            \
	};
#endif // } 0

//*****************************************************************************
// OEM code feature
//*****************************************************************************
#define __IT8386__ELPM__						1   
#define P80DEBUG 								0
#define PWMTEST 								0
#define LenovoS0ILSupport 						0
#define Fan_Debug 								1

#define EC_USB_charger							1
#ifdef EC_USB_charger
#define EC_USB_BYPASS 							1
#endif


#define _FanIDCheck_ 							0

//#define SMBusServiceCenterFunc          		// Define : for SMBus service center

#define SaveBatteryStorageMode 					0

#define PowerSequenceOnly						0

#define EX_FLASH_POWER_DOWN 					0

#define _ITE_USB_PD_ENABLE_						0
#define _ITE_USB_PD_EVB_ 						0

#define TI_USB_TYPEC 							0

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function define
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define Support_EC_Sleep      TRUE   // Support EC sleep function.
#define Support_External_IO   FALSE  // Support EC External IO function.

#define WDT_Support           FALSE    // Disable for SDV's SMT phase.

// CMW 20121022 +
#define Support_Mirror_Code   TRUE//FALSE

#define Support_VC0_Function  FALSE  // Support Voltage Compare 0 Function.
#define Support_VC1_Function  FALSE  // Support Voltage Compare 1 Function.
#define Support_VC2_Function  FALSE  // Support Voltage Compare 2 Function.
// CMW 20121022 -

#define Support_EC_Port80_Debug TRUE
#define Support_SMB_debug_card  FALSE
#define Support_RAM_Debug       TRUE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC Function Setting and selection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CMW 20121022 +
#if Support_Mirror_Code
#define EC_Signature_Flag     0x95//0x14    // For 8518 crystal free
#else
#define EC_Signature_Flag     0xB5    // For 8518 crystal free
#endif

#if Support_Mirror_Code
#define EC_Signature_Addr        0xA7//0x67   //168K
#else
#define EC_Signature_Addr        0x7F//0xAA
#endif
// CMW 20121022 -






//-----------------------------------------------------------------------------
// PS2 interface
//-----------------------------------------------------------------------------
#define TouchPad_only         TRUE    // True : Only touch pad device in this project
                                      /* Note : If TouchPad_only is set, remeber to call
                                          				PowerOffClearTPPendingData() function in power off sequnce
                                         					 include (S3, S4, and S5)!C*/

/* Note : If TouchPad_only is set, remeber to call
PowerOffClearTPPendingData() function in power off sequnce
include (S3, S4, and S5)。*/

#if TouchPad_only
#define MultiplexedModes        				0       	// 0 : Doesn't support multiplexed modes.
#define PS2IFHotPlugHotSwap    				0       	// 1 : Support PS2 interface hot-plug, hot-swap
#define Hook_Func_DisableTP            				 	// Support "Hook_DisableInternalTB" for EC on/off touchpad
#else
#define MultiplexedModes        				1       	// 1 : Support multiplexed modes.
#define PS2IFHotPlugHotSwap     				0       	// 1 : Support PS2 interface hot-plug, hot-swap
#endif

//-----------------------------------------------------------------------------
// Power sequence only
//-----------------------------------------------------------------------------
#define EN_PwrSeqTest         FALSE  // For SDV's SMT phase used.
                                     // Disable other functions beside power sequence.

//-----------------------------------------------------------------------------
// Keyboard scan
//-----------------------------------------------------------------------------
#define Support_Keyboard      CS16KB    //support CS13KB/CS16KB
#define KB_FnFuncKey          TRUE
#define KB_FnSticky_Key       TRUE
#define KB_Primary_Key        TRUE

#define SUPPORT_KB_RAM_TABLE  TRUE   // KB matrix placed at RAM(TRUE) or ROM(FALSE).

#define ACTION                0x03

//*****************************************************************************
// UART debug interface
// Timer 2 baudrate = Fosc / 32 / (0x10000-RCAP2)
//*****************************************************************************
#define UART_Debug        // Define : enable Tx debug (GPIO)

#ifdef UART_Debug
// 9600 bps-------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xE2
// 9600 bps-------------------------

// 19200 bps------------------------
//#define R2HV                  0xFF
//#define R2LV                  0xF1
// 19200 bps------------------------

// 57600 bps------------------------
#define R2HV                    0xFF
#define R2LV                    0xFB
// 57600 bps------------------------
#endif

//*****************************************************************************
// OEM code feature
//*****************************************************************************
#define Init_FMSSR            0x1F  // 0x1F 2M Bytes Share Rom
#define Init_BADR             0x01  // Base address select 0b00:2E 2F 0b01:4E 4F 0b10:User define SWCBALR | SWCBAHR
#define Init_SWCBAHR          0x00  // I/O Port High Byte
#define Init_SWCBALR          0x4E  // I/O Port Low  Byte
#define Init_I2EC             0x43  // Enable I2EC R/W and log 80 port

#define PLATFORM_CHIP         AMD   // INTEL or AMD platform
#define PLATFORM_DGPU         FALSE   // NVidia or AMD platform
#define EC_CHIP               IT8186 // Support IT8586/IT8986
#define Auto_Detect_ITE_chip  FALSE  // Support IT8986, IT8586
#define SUPPORT_DSx           FALSE  // Support Deep Sx
#define EFLASH192KB			  0x02
#define EFLASH128KB			  0x01
#define EFLASHSIZE		  	  EFLASH192KB	// define the eflash size which is 192KB or 128KB

#if (PLATFORM_CHIP == AMD)
#define AMDPWRBTNWK           100
#define AMDPWRBTNPULSE        5
#endif // PLATFORM_CHIP


#define SUPPORT_CPUMAXPSTATE  TRUE  // Support CPU Max state obtained by BIOS
#if SUPPORT_CPUMAXPSTATE
#define CPUMAXPSTATE          THROTTLING_HW
#define GPUMAXPSTATE          4
#else
#define CPUMAXPSTATE          0x0F
#endif
//-----------------------------------------------------------------------------
// OEM define.
//-----------------------------------------------------------------------------
// VGA feature
#define NV_GC6                FALSE // Set TRUE to support nV VGA GC6 feature

// Platform feature
#define SW_ISCT               FALSE  // Support ISCT function. (AOAC/IOIC)
#define Support_IFFS          FALSE  // Support IFFS function. (INTEL RapidStart)

// EC feature
#define Support_EC_GS_ON      FALSE  // Support G-sensor.
#define SW_ChkTimeScale       TRUE   // Support Timer.

#define Support_WebCam        FALSE  // Support WebCam enable/disable by EC.
#define ACPI_Smbus            TRUE   // Support ACPI command
#define EC_Brightness_Ctrl    FALSE  // Support EC brightness control. (?)
#define Support_EC_LANWake    TRUE   // Support Lan Wake by EC control.

#define Support_EC_KBFN       TRUE   // Support Edge KB FN GPIO Pin.
#define Support_SMTADP        FALSE   // Support Smart Adaptor

#define Support_HUAshutdown	 FALSE  // shutdown function for hang on power sequence
#define Support_WLAN_WAKE	   FALSE  // Support WLAN wake

#define chPWSW1WDT			      TRUE	 // power button function for BIOS flashing protect.
#define CHARGER_IC             BQ25700//ChargerIC support BQ24780S : BQ25700


//-----------------------------------------------------------------------------
// Thermal define.
//-----------------------------------------------------------------------------
#define FAN_TABLE_Already        FALSE//TRUE    // FAN Table already. FALSE = fan full speed.
#define Support_Thermal_monitor  TRUE     // True : fan speed report Hexadecimal , False : report BCD
#define Support_FANLESSS         FALSE  // system with fanless
#if (PLATFORM_CHIP == INTEL)
#define Support_CPUCOMMFAIL_shutdown      TRUE  // for Intel platform, if set ture communication fail get temperature from BIOS
#endif

#define Support_STDNProct     FALSE//TRUE
#define Support_MAXPWR        FALSE//TRUE

// Battery
#define Support_TWO_BATT      TRUE
#define S3RSOCPercentage      0x05    // 5%
#define BatteryOTPSD          70
#define BatteryOTPHi          60      // 60 degree
#define BatteryOTPLow         55      // 55 degree
#define FastChgTimeOut        720     // 12 * 60 = 720min
#define Thinkpad_Battery_HW_Auth  TRUE  // Battery authencation by EC HW(CHIP).
#define Lenovo_Authentication TRUE
#define BATS3WAKEcondition    45        //default 0 : battery under 5% wake,  1 => Remain cap 10mW 2 = >Remain cap 20mW  ......
#define SupportCHKtrippoint   TRUE
#define BatLowVot_Rule         LowVot_3S4S //Battery low votlage shutdown protect rule
#define Support_SmartCharge   TRUE

#define Trickle_Control_By_CHGIC  FALSE   // FALSE:control by EE, TRUE: control by charge IC
#define Trickle_ChargeCurr     0x0100     // 256mA
#define Trickle_ChargeVolt     0x2E0E     // 12V
#define Tricklechargecurrent   300        // 300mA


#define OCPCapacity15           50
#define OCPCapacity             60    
#define OCPCapacityRelease      OCPCapacity * 0.9
#define OCPCapacityRelease15    OCPCapacity15 * 0.9

#define OCPCurrentShutDown      7200
#define OCPCurrentShutDown15    5200    


//[CC20131118][Project][Start] Add shutdown function for hang on power sequence.
#define PWRFAILCnt     0x08    //500ms * 8 = 4 Seconds
//[CC20131118][Project][End] Add shutdown function for hang on power sequence.

/*===========================================================================
 * TYPE-C          support chip CCG2 CCG3
 *===========================================================================*/
#define Support_TYPE_C       3

/*===========================================================================
 * LED control
 *===========================================================================*/
#define Support_MuteLED              FALSE
#define Support_FnLKLED              FALSE
#define Support_Microphone_Mute_LED  FALSE
#define Support_PWRBTN_LED           TRUE
#define Support_BATTCHGLED           TRUE
#define Support_BATTCHGLED_BLINK     FALSE  //ROSC 0% ~ 20% LED Blink, this is optional function. need check with SW PM
#define Support_PWRSTATUSLED         TRUE
#define Support_CAPSLOCKLED          TRUE
#define Support_CAPSLOCKLED_byBIOS   TRUE
#define Support_KBD_BL_LED           TRUE
#define Support_KBD_BL_DETECT        TRUE

/*===========================================================================
 * temp remove MFG tool
 *===========================================================================*/
#define Support_COMPAL_MFG_TOOL        FALSE

/*===========================================================================
 * GATEA20 support
 *===========================================================================*/
#define Support_GATEA20             FALSE//TRUE

/*===========================================================================
 *WLAN BT ON/OFF control by EC
 *===========================================================================*/
#define Support_WLAN_ON_OFF_control           FALSE
#define Support_BT_ON_OFF_control             FALSE

/*===========================================================================
 * RFID support
 *===========================================================================*/
#define Support_RFID                  TRUE

/*===========================================================================
 * AOU support
 *===========================================================================*/
#define Support_AOU5_V1_2                  TRUE      //1: support v1.2      0: support v1.0

/*===========================================================================
 * Gsensor support
 *===========================================================================*/
#define Support_LenovoAPS           TRUE && !EN_PwrSeqTest  // Support Lenovo APS
#define Support_GSNS_I2C            TRUE      //1: Gsensor => I2C       0: Gsensor => ADC
#define Support_BMA255           		TRUE

/*===========================================================================
 * SMBUS => SMCLK0/1/2 and SMDAT0/1/2 timing
 *===========================================================================*/
#define SMB4P7USL_REG    0x28 
#define SMB4P0USH_REG    0x25        
#define SMB300NS_REG     0x03         
#define SMB250NS_REG     0x05         
#define SMB25MS_REG      0x19          
#define SMB45P3USL_REG   0xA5       
#define SMBS4P3USH_REG   0x01       
#define SMB4P7A4P0H_REG  0x00      

/*===========================================================================
 * 
Read Board ID by ADC
 *===========================================================================*/
#define Support_ADC_BoardID    TRUE 

/*===========================================================================
 * Dash 
 *===========================================================================*/
#define Support_DASH    FALSE//TRUE 
#define Support_DASH_APU_Voltage   FALSE
#define Support_DASH_DDR_Voltage   FALSE
#define DASH_workAroundNofity  TRUE

/*===========================================================================
 * Tamper detection      
 *===========================================================================*/
#define Support_Tamper_function  Tamper_ADC    // FALSE / Tamper_ADC / Tamper_GPIO
#define Support_Chk_mSATA	   TRUE    // Support Check mSATA
#define Support_Chk_BayHDD  	 FALSE   // Support Check BayHDD
#define Support_Chk_WWAN    	 TRUE    // Support Check WWAN by ADC
#define Support_BOTTOM_TAMPER_FUNC  TRUE    // support Dcover check

/*===========================================================================
// Define SMBus device address
 *===========================================================================*/
#define IS_SUPPORT_SMB4	TRUE
#define IS_SUPPORT_SMB5	TRUE
#define IS_SUPPORT_SMB6	TRUE

#define EC_I2C_ADDR      0x02
#define SmBat_Addr       0x16    // smart battery address
#define SecSmBat_Addr    0x16    // 2nd smart battery address
#define Charger_Addr     0x12    // charger address
#define TmlIC_Addr       0x9A    // Thermal IC SMBus address.
#define NV_VGA_Addr      0x9E    // nVIDIA SMBus address.
#define Intel_PCH_Addr   0x96    // Intel PCH SMbus address.
#define AMD_SBTSI_Addr   0x98    // AMD SMBus address.
#define AMD_VGA_Addr     0x82    // AMD GPU SMBus address. 
#define DebugCard_Addr	 0x0E	// debug card address		// debug card
#define DGPU_Addr        AMD_VGA_Addr
#define TypeC_Addr       TPS65982_I2C_ADDR    // Type-C CCG3 I2C address

#define chSMB_BAT		  SMbusCh6
#define chSMB_2ndBAT  SMbusCh5
#define chSMB_THM		  SMbusCh4          //swap ch1 & ch4 for dash
#define chSMB_GS		  SMbusCh4			//swap ch1 & ch4 for dash	
#define chSMB_VGA		  SMbusCh4          //swap ch1 & ch4 for dash
#define chSMB_TYPEC		SMbusCh3
#define chSMB_CHG		  SMbusCh2
#define chSMB_INTPCH	SMbusCh4            //swap ch1 & ch4 for dash
#define chSMB_AMDTSI	SMbusCh4            //swap ch1 & ch4 for dash
#define chSMB_DebugCard		chSMB_2ndBAT		// debug card

#define SMB_Channel_AUTO_detect TRUE
#define SMB_BAT_Exception_Addr FALSE   //FALSE: dual bat and address different  , XX: battery address (same address)
#define SMB_AUTO_Channel {SmBat_Addr    , chSMB_BAT},\
                         {SecSmBat_Addr , chSMB_2ndBAT},\
                       	 {Charger_Addr  , chSMB_CHG},\
	                       {TmlIC_Addr    , chSMB_THM},\
	                       {Intel_PCH_Addr, chSMB_INTPCH},\
	                       {AMD_SBTSI_Addr, chSMB_AMDTSI},\
	                       {DGPU_Addr     , chSMB_VGA},\
	                       {TypeC_Addr    , chSMB_TYPEC},\
	                       {LIS3DH_ADDRESS, chSMB_GS},\
	                       {KX022_ADDRESS, chSMB_GS},\
	                       {KX023_ADDRESS , chSMB_GS}

/*===========================================================================
 * TP/CP control 
 *===========================================================================*/
#define Support_TP_RESET_control             TRUE   
#define Support_BYPASS_PAD_control           TRUE 
#define Support_CP_RESET_control             TRUE 

/*===========================================================================
 * Max power control setting
 *===========================================================================*/
#define A45W_CP                               41
#define A45W_CHARGER_TURBO_POINT              43.87 // CP * 1.07
#define A45W_CPU_TURBO_BOOST_OFF_POINT        70
#define A45W_CPU_TURBO_BOOST_ON_POINT         65
#define A45W_CPU_THROTTLING_ON_POINT          70
#define A45W_CPU_THROTTLING_OFF_POINT         65
#define A45W_GPU_THROTTLING_ON_POINT          65
#define A45W_GPU_THROTTLING_OFF_POINT         60

#define A45W_CPU_THROTTLING_ON_POINT_L        38
#define A45W_CPU_THROTTLING_OFF_POINT_L       33
#define A45W_GPU_THROTTLING_ON_POINT_L        33
#define A45W_GPU_THROTTLING_OFF_POINT_L       28

#define A45W_AC_CPU_TURBO_BOOST_OFF_POINT     45
#define A45W_AC_CPU_TURBO_BOOST_ON_POINT      40
#define A45W_AC_CPU_THROTTLING_ON_POINT       45
#define A45W_AC_CPU_THROTTLING_OFF_POINT      40
#define A45W_AC_GPU_THROTTLING_ON_POINT       40
#define A45W_AC_GPU_THROTTLING_OFF_POINT      35

#define A45W_ACOFF                            54

#define A65W_CP                               60
#define A65W_CHARGER_TURBO_POINT              64.2 // CP * 1.07
#define A65W_CPU_TURBO_BOOST_OFF_POINT        90
#define A65W_CPU_TURBO_BOOST_ON_POINT         85
#define A65W_CPU_THROTTLING_ON_POINT          90
#define A65W_CPU_THROTTLING_OFF_POINT         85
#define A65W_GPU_THROTTLING_ON_POINT          85
#define A65W_GPU_THROTTLING_OFF_POINT         80

#define A65W_CPU_THROTTLING_ON_POINT_L        57
#define A65W_CPU_THROTTLING_OFF_POINT_L       52
#define A65W_GPU_THROTTLING_ON_POINT_L        52
#define A65W_GPU_THROTTLING_OFF_POINT_L       47

#define A65W_AC_CPU_TURBO_BOOST_OFF_POINT     65
#define A65W_AC_CPU_TURBO_BOOST_ON_POINT      60
#define A65W_AC_CPU_THROTTLING_ON_POINT       65
#define A65W_AC_CPU_THROTTLING_OFF_POINT      60
#define A65W_AC_GPU_THROTTLING_ON_POINT       60
#define A65W_AC_GPU_THROTTLING_OFF_POINT      55

#define A65W_ACOFF                            78

#define A90W_CP                               75
#define A90W_CHARGER_TURBO_POINT              80.25 // CP * 1.07
#define A90W_CPU_TURBO_BOOST_OFF_POINT        105
#define A90W_CPU_TURBO_BOOST_ON_POINT         100
#define A90W_CPU_THROTTLING_ON_POINT          105
#define A90W_CPU_THROTTLING_OFF_POINT         100
#define A90W_GPU_THROTTLING_ON_POINT          105
#define A90W_GPU_THROTTLING_OFF_POINT         100

#define A90W_AC_CPU_TURBO_BOOST_OFF_POINT     90
#define A90W_AC_CPU_TURBO_BOOST_ON_POINT      85
#define A90W_AC_CPU_THROTTLING_ON_POINT       90
#define A90W_AC_CPU_THROTTLING_OFF_POINT      85
#define A90W_AC_GPU_THROTTLING_ON_POINT       90
#define A90W_AC_GPU_THROTTLING_OFF_POINT      85

#define A90W_ACOFF                            109

#if Support_TYPE_C
#define A45W_System_Limited_support_TypeC_Adapter		45
#define A65W_System_Limited_support_TypeC_Adapter		65
#define A90W_System_Limited_support_TypeC_Adapter		90

#define A45W_System_Notsupport_TypeC_Adapter		36
#define A65W_System_Notsupport_TypeC_Adapter		45
#define A90W_System_Notsupport_TypeC_Adapter		60

#define A45W_System_Unknow_TypeC_Adapter		 6
#define A65W_System_Unknow_TypeC_Adapter		15
#define A90W_System_Unknow_TypeC_Adapter		29

#endif //Support_TYPE_C

#define A45W_System_Limited_support_Adapter		36//AdapterID_36W
#define A65W_System_Limited_support_Adapter		45//AdapterID_45W
#define A90W_System_Limited_support_Adapter		65//AdapterID_65W
#define Limited_support_Adapter               AdapterID_45W //useless

#define A45W_System_Not_support_Adapter		36
#define A65W_System_Not_support_Adapter		45
#define A90W_System_Not_support_Adapter		65
#define ADPI_times                            3
#define ACcutoff_Ts                           9 // cut off times

///////////////////////////////////////////////////////////////////////////////
//PWM Function Channel&Prescaler Setting,
//////////////////////////////////////////////////////////////////////////////
#define FAN_PWM_Channel			PWM_Channel_2
#define FAN_PWM_Prescaler   	PWM_PrescalerC6

#define BEEP_PWM_Channel		PWM_Channel_6
#define BEEP_PWM_Prescaler		PWM_PrescalerC4

#define LOGOLED_PWM_Channel    	PWM_Channel_0
#define LOGOLED_PWM_Prescaler	PWM_PrescalerC7

#define PWRLED_PWM_Channel		PWM_Channel_7
#define PWRLED_PWM_Prescaler	PWM_PrescalerC7

#define PWMPolarity0  1
#define PWMPolarity1  0
#define PWMPolarity2  0
#define PWMPolarity3  0
#define PWMPolarity4  0 
#define PWMPolarity5  0
#define PWMPolarity6  0
#define PWMPolarity7  1
#define PWM_Polarity  (PWMPolarity7 << 7) + \
                      (PWMPolarity6 << 6) + \
                      (PWMPolarity5 << 5) + \
                      (PWMPolarity4 << 4) + \
                      (PWMPolarity3 << 3) + \
                      (PWMPolarity2 << 2) + \
                      (PWMPolarity1 << 1) + \
                      PWMPolarity0
////////////////////////////////////////////////////////////////////////////
//External Timer Setting(Timer2/5/6/7 can be use.
///////////////////////////////////////////////////////////////////////////
#define USBTimer	ExternalTimer_5
#define tUSBCount	1000	// 1sec


///////////////////////////////////////////////////////////////////////////////
//ITE Debug Flag
///////////////////////////////////////////////////////////////////////////////
#define EventDebug			FALSE
#define DEBUG_UCSI_CMD        FALSE    //UCSI UART2 Debug
#define DEBUG_UCSI        FALSE    //UCSI UART2 Debug

#define UCSI_UPDATE_VERSION              0x06

////////////////////////////////////////////////
#define uMBID_LoadRom       FALSE
#if(!uMBID_LoadRom)
#define Current_MBID         0x41
#endif

#endif


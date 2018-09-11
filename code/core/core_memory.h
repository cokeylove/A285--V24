/* 
 * ****************************************************************************
 *
 *  core_memory.h
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/14
 *
 *      Author: ITE00580, Dino Li
 *
 * ****************************************************************************
 */

#ifndef CORE_MEMORY_H_
#define CORE_MEMORY_H_

/*
 * ****************************************************************************
 * [Kernel Memory Rang] 0x00080000 ~ 0x000801FF 512 bytes
 *
 * 0x00080000-0x000800FF   kernel code RAM 0
 * 0x00080100-0x000801FF   kernel code RAM 1
 *
 * [OEM Memory Rang] 0x00080200 ~ 0x00080FFF 3.5K bytes
 *
 * 0x00080200-0x000802FF   OEM RAM 2
 * 0x00080300-0x000803FF   OEM RAM 3 
 * 0x00080400-0x000804FF   OEM RAM 4
 * 0x00080500-0x000805FF   OEM RAM 5
 * 0x00080600-0x000806FF   OEM RAM 6
 * 0x00080700-0x000807FF   OEM RAM 7
 * 0x00080800-0x000808FF   OEM RAM 8
 * 0x00080900-0x000809FF   OEM RAM 9
 * 0x00080A00-0x00080AFF   OEM RAM A
 * 0x00080B00-0x00080BFF   OEM RAM B
 * 0x00080C00-0x00080CFF   OEM RAM C
 * 0x00080D00-0x00080DFF   OEM RAM D 
 * 0x00080E00-0x00080EFF   OEM RAM E
 * 0x00080F00-0x00080FFF   OEM RAM F
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * global variables
 * ****************************************************************************
 */
extern const CBYTE *Scanner_Table_Pntr;
extern const CBYTE *Extendkey_Table_Pntr;
extern const CBYTE *Scanner_LOC_Pntr;
extern BYTE *SPIDataPointer;

/* 
 * ****************************************************************************
 * SRAM Data Base Adress for kernel code using
 * ****************************************************************************
 */
#define SRAM_DATA_CORE_BASE_ADDR        0x00080000
#define SRAM_DATA_CORE_BASE_ADDR_02     0x00080100

/* 
 * ****************************************************************************
 * Global variable 01 for kernel code
 * ****************************************************************************
 */
/* offset 0x00 ~ 0x1F */
#define F_Service_All 	        SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x00)

#define F_Service_0             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x00)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_PCI           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x00)
#define F_Service_Extend_Task   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x01)
#define F_Service_SEND          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x02)
#define F_Service_Send_PS2      SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x03)

#define F_Service_1             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x04)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_PS2           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x04)
#define F_Service_PCI2          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x05)
#define F_Service_MS_1          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x06)
#define F_Service_KEY           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x07)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_2             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x08)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_Low_LV        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x08)
#define F_Service_PCI3          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x09)
#define F_Service_CIR           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0A)
#define F_Service_PCI4          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0B)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_3             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x0C)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_OEM_1         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0C)
#define F_Service_OEM_2         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0D)
#define F_Service_OEM_3         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0E)
#define F_Service_OEM_4         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x0F)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_4             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x10)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_Reserved_16   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x10)
#define F_Service_Reserved_17   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x11)
#define F_Service_Reserved_18   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x12)
#define F_Service_Reserved_19   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x13)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_5             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x14)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_Reserved_20   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x14)
#define F_Service_Reserved_21   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x15)
#define F_Service_Reserved_22   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x16)
#define F_Service_Reserved_23   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x17)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_6             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x18)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_Reserved_24   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x18)
#define F_Service_Reserved_25   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x19)
#define F_Service_Reserved_26   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1A)
#define F_Service_Reserved_27   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1B)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define F_Service_7             SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x1C)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define F_Service_Reserved_28   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1C)
#define F_Service_Reserved_29   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1D)
#define F_Service_Reserved_30   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1E)
#define USB_ISR_SERVICE_FLAG    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x1F)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Ccb42                 (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x20)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Ccb42                   u_Ccb42.byte
#define Ccb42_INTR_KEY          u_Ccb42.fbit.bit0
#define Ccb42_INTR_AUX          u_Ccb42.fbit.bit1
#define Ccb42_SYS_FLAG          u_Ccb42.fbit.bit2
#define Ccb42_RV1               u_Ccb42.fbit.bit3
#define Ccb42_DISAB_KEY         u_Ccb42.fbit.bit4
#define Ccb42_DISAB_AUX         u_Ccb42.fbit.bit5
#define Ccb42_XLATE_PC          u_Ccb42.fbit.bit6
#define Ccb42_RV2               u_Ccb42.fbit.bit7

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// The Controller Command Byte.  Accessed through Commands 20h
// and 60h.  First byte in the 0 through 1F space in an 8042.
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define INTR_KEY                0x01	//bit0
#define INTR_AUX                0x02	//bit1
//#define SYS_FLAG              0x04	//bit2
#define DISAB_KEY               0x10	//bit4
#define DISAB_AUX               0x20	//bit5
#define XLATE_PC                0x40	//bit6

#define all 		            byte
#define SCROLL   	            bit0
#define NUM      	            bit1
#define CAPS      	            bit2
#define OVL     	            bit3
#define LED4    	            bit4
#define LED5     	            bit5
#define LED6    	            bit6
#define LED7    	            bit7

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_MULPX                 (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x21)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define MULPX                   u_MULPX.byte
#define MULPX_RV5               u_MULPX.fbit.bit0
#define MULPX_Mx_state2         u_MULPX.fbit.bit1
#define MULPX_Mx_state3         u_MULPX.fbit.bit2
#define MULPX_Multiplex         u_MULPX.fbit.bit3
#define MULPX_RV1               u_MULPX.fbit.bit4
#define MULPX_RV2               u_MULPX.fbit.bit5
#define MULPX_RV3               u_MULPX.fbit.bit6
#define MULPX_RV4               u_MULPX.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_AuxTestFlagS          (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x22)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define AuxTestFlagS            u_AuxTestFlagS.byte
//#define ---                   u_AuxTestFlagS.fbit.bit0
#define MouseDriverIn           u_AuxTestFlagS.fbit.bit1
#define KeyboardDriverIn        u_AuxTestFlagS.fbit.bit2
#define AUX2ByteCommand         u_AuxTestFlagS.fbit.bit3
#define AuxTestFlagS_RV1        u_AuxTestFlagS.fbit.bit4
#define AuxTestFlagS_RV2        u_AuxTestFlagS.fbit.bit5
#define AuxTestFlagS_RV3        u_AuxTestFlagS.fbit.bit6
#define AuxTestFlagS_RV4        u_AuxTestFlagS.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Led_Ctrl              (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x23)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Led_Ctrl                u_Led_Ctrl.byte
#define Led_Ctrl_SCROLL         u_Led_Ctrl.fbit.bit0
#define Led_Ctrl_NUM            u_Led_Ctrl.fbit.bit1
#define Led_Ctrl_CAPS           u_Led_Ctrl.fbit.bit2
#define Led_Ctrl_OVL            u_Led_Ctrl.fbit.bit3
#define Led_Ctrl_LED4           u_Led_Ctrl.fbit.bit4
#define Led_Ctrl_LED5           u_Led_Ctrl.fbit.bit5
#define Led_Ctrl_LED6           u_Led_Ctrl.fbit.bit6
#define Led_Ctrl_LED7           u_Led_Ctrl.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Led_Data              (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x24)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Led_Data                u_Led_Data.byte
#define Led_Data_SCROLL         u_Led_Data.fbit.bit0
#define Led_Data_NUM            u_Led_Data.fbit.bit1
#define Led_Data_CAPS           u_Led_Data.fbit.bit2
#define Led_Data_OVL            u_Led_Data.fbit.bit3
#define Led_Data_LED4           u_Led_Data.fbit.bit4
#define Led_Data_LED5           u_Led_Data.fbit.bit5
#define Led_Data_LED6           u_Led_Data.fbit.bit6
#define Led_Data_LED7           u_Led_Data.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Scanner_State         (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x25)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Scanner_State           u_Scanner_State.byte
#define Scanner_State_NUM_LOCK  u_Scanner_State.fbit.bit0
#define Scanner_State_LEFT      u_Scanner_State.fbit.bit1
#define Scanner_State_RIGHT     u_Scanner_State.fbit.bit2
#define Scanner_State_ALT       u_Scanner_State.fbit.bit3
#define Scanner_State_CONTROL   u_Scanner_State.fbit.bit4
#define Scanner_State_OVERLAY   u_Scanner_State.fbit.bit5
#define Scanner_State_FN        u_Scanner_State.fbit.bit6
#define Scanner_State_FN_SILENT u_Scanner_State.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Gen_Info              (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x26)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Gen_Info                u_Gen_Info.byte
#define Gen_Info_LED_CHANGE     u_Gen_Info.fbit.bit0
#define Gen_Info_TYPM_CHANGE    u_Gen_Info.fbit.bit1
#define Gen_Info_CODE_CHANGE    u_Gen_Info.fbit.bit2
#define Gen_Info_SCAN_LOCKED    u_Gen_Info.fbit.bit3
#define Gen_Info_PASS_ENABLE    u_Gen_Info.fbit.bit4
#define Gen_Info_BREAK_AUXD     u_Gen_Info.fbit.bit5
#define Gen_Info_BREAK_AUXK     u_Gen_Info.fbit.bit6
#define Gen_Info_BREAK_SCAN     u_Gen_Info.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Kbd_Response          (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x27)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Kbd_Response                u_Kbd_Response.byte
// Don't use bit0 ~ bit5
#define Kbd_Response_SECOND_ACK     u_Kbd_Response.fbit.bit6
#define Kbd_Response_CMD_RESPONSE   u_Kbd_Response.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_AuxScanFlagS          (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x28)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define AuxScanFlagS            u_AuxScanFlagS.byte

//#define ---                   u_AuxScanFlagS.fbit.bit0
//#define ---                   u_AuxScanFlagS.fbit.bit1
#define StartScanAUXDevice      u_AuxScanFlagS.fbit.bit2
#define StartENAUXDevice        u_AuxScanFlagS.fbit.bit3
#define AUXInterfaceBusy        u_AuxScanFlagS.fbit.bit4
#define AuxScanFlagS_RV3        u_AuxScanFlagS.fbit.bit5
#define AuxScanFlagS_RV1        u_AuxScanFlagS.fbit.bit6
#define AuxScanFlagS_RV2        u_AuxScanFlagS.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_CustomFlag            (*((BITS_8 *)(SRAM_DATA_CORE_BASE_ADDR+0x29)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define CustomFlag              u_CustomFlag.byte
#define KbdNeedResponseFlag     u_CustomFlag.fbit.bit0
#define ExtendMatrix            u_CustomFlag.fbit.bit1
//#define ---                   u_CustomFlag.fbit.bit2
#define FastA20                 u_CustomFlag.fbit.bit3
#define ChkAUXRespFlag2         u_CustomFlag.fbit.bit4
#define ECCheckBurstMode        u_CustomFlag.fbit.bit5
//#define ---                   u_CustomFlag.fbit.bit6
#define FnStickKey              u_CustomFlag.fbit.bit7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define PS2StartBit 	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x2A)
#define SendtoAUXFlag           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x2B)
#define ECSendtoAUXFlag         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x2C)
#define BurstModeInstructionDelay   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x2D)

/* Reserved offset 0x2E ~ 0x2F */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define KBHICmd 		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x30)
#define KBHIStep 			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x31)
#define KBHIData		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x32)
#define KBHIData1			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x33)
#define KBHIData2			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x34)
#define PM1Cmd				    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x35)
#define PM1Step			        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x36)
#define PM1Data				    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x37)
#define PM1Data1			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x38)
#define PM1Data2			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x39)
#define PM2Cmd				    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3A)
#define PM2DataCount		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3B)
#define PM2Data				    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3C)
#define PM2Data1			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3D)
#define PM2Data2		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3E)
#define Tmp_Load			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x3F)

#define u_Timer_A               (*((TIMER *)(SRAM_DATA_CORE_BASE_ADDR+0x40)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Timer_A                 u_Timer_A

#define HIF2_SAFETY             count   /* Secondary host interface timer. */
#define TMR_SCAN                bit4    /* Scanner interval enabled. */
#define TMR_GENTMO              bit5    /* General timeouts. */
//                              bit6 	/* */
//                              bit7    /* */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Timer_B               (*((TIMER *)(SRAM_DATA_CORE_BASE_ADDR+0x41)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Timer B. Timer A has a time base of 150 uS. */
#define Timer_B                 u_Timer_B
/* Internal device send delay count used with SEND_ENABLE. */
#define SEND_DELAY              count
/* Internal device send request. */
#define SEND_ENABLE             bit4
/* Delay to enable serial clock lines. */
#define INH_CLEAR               bit5
/* Delay for unlock request. */
#define UNLOCK_TMO              bit6
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Timer1msCnt			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x42)

/* Reserved offset 0x43 */
                                        
#define Service_Timer5msEvent   SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0x44)

/* Reserved offset 0x48 */

#define u_new_keyl          (*((UUnew_keyl *)(SRAM_DATA_CORE_BASE_ADDR+0x49)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define new_keyl            u_new_keyl
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_new_keyh          (*((uKEY *)(SRAM_DATA_CORE_BASE_ADDR+0x4A)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                            /*
                             * Holds new scan key address and
                             * debounce status.
                             */
#define new_keyh            u_new_keyh
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_typematic         (*((uKEY *)(SRAM_DATA_CORE_BASE_ADDR+0x4B)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
                            /* Holds new_keyh for typematic action. */
#define typematic 		    u_typematic
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_new_extendkey     (*((uKEY *)(SRAM_DATA_CORE_BASE_ADDR+0x4C)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define new_extendkey 	    u_new_extendkey /* For extend keys */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_extendstatus      (*((UUnew_keyl *)(SRAM_DATA_CORE_BASE_ADDR+0x4D)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define extendstatus        u_extendstatus
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_etkeytypematic    (*((uKEY *)(SRAM_DATA_CORE_BASE_ADDR+0x4E)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define etkeytypematic      u_etkeytypematic
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define ScanCode2_USBHID    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x4F)
/* offset 0x50 ~ 0x53 */
#define s_scan              (*((sSscan *)(SRAM_DATA_CORE_BASE_ADDR+0x50)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define scan                    s_scan          /* 3 BYTES, + 1 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* offset 0x54 ~ 0x57 */
#define s_extendscan            (*((sSscan *)(SRAM_DATA_CORE_BASE_ADDR+0x54)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define extendscan              s_extendscan    /* 3 BYTES, + 1 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define PM3Cmd			        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x59)
#define PM3DataCount	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5A)
#define PM3Data			        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5B)
#define PM3Data1		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5C)
#define PM3Data2		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5D)
#define MainFucnLow		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5E)
#define MainFucnHigh	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x5F)

#define ITempB01		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x64)
#define ITempB02			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x65)
#define ITempB03			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x66)
#define ITempB04		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x67)
#define ITempB05			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x68)
#define ITempB06			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x69)

#define bscan_matrix            SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x6D)

#define AUXInactiveCounter	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x80)
#define CheckAUXIndex		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x81)
#define TPResolution		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x82)
#define TPSamplingRate		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x83)
#define PS2_IRQ_Channel	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x84)
#define TPACKCounter		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x85)
#define AUXCmdFFCnt			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x86)
#define AUXPortNum			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x87)
/* array offset 0x88 ~ 0x8A */
#define AuxFlags 			    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x88)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define DEVICE_IS_MOUSE         BIT7
#define DEVICE_IS_KEYBOARD      BIT6
#define DEVICE_IS_MAIN_MOUSE    BIT5
#define DEVICE_IS_MAIN_KB       BIT4
#define DEVICE_NEED_CONFIG      BIT3
#define DEVICE_MS_DRIVER_NO_MATCH   BIT2
#define DEVICE_NEED_INIT        BIT1
#define DEVICE_IS_ATTACHED      BIT0
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define MousePendingBuf		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x8B)
#define ScanAUXStepIndex        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x8C)
/* array offset 0x8D ~ 0x8F */
#define AuxFlags2               SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x8D)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define DEVICE_POWER_DOWN       BIT7
#define DEVICE_ENABLED_TIME_OUT BIT6
#define DEVICE_IS_ENABLED       BIT5
#define DEVICE_NEED_ENABLED     BIT4

#define DEVICE_DISABLE_TIME_OUT BIT2
#define DEVICE_IS_DISABLED      BIT1
#define DEVICE_NEED_DISABLED    BIT0
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define MS_ID_CUNT              SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x90)
#define MS_DRIVER_TYPE          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x91)
#define KB_ACK_DELAY_CUNT       SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x92)
#define KB_CodeSet              SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x93)
#define PS2KB_Typematic         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x94)
#define PS2IFAck                SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x95)
#define Save_Typematic          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x96)
/* array offset 0x97 ~ 0x99 */
#define PS2PortxData 		    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x97)
#define KB_Typematic            SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9A)
#define Main_MOUSE_CHN          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9B)
#define Main_KB_CHN             SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9C)
#define Main_KB_ACK_CUNT        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9D)
#define AUX_SCAN_INDEX          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9E)
#define AUX_SCAN_INDEX_HS       SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0x9F)

#define AUX_SCAN_TIMEOUT_CUNT   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA0)
#define SKIP_AUX_SCAN_CUNT      SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA1)
/* array offset 0xA2 ~ 0xA4 */
#define AuxFlags_KB_CMD		    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA2)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define KB_CMD_0xF5             BIT0
#define KB_CMD_0xF4             BIT1
#define KB_CMD_0xF3             BIT2
#define KB_CMD_0xED             BIT3
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* array offset 0xA5 ~ 0xA7 */
#define AuxFlags_MS_CMD		    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA5)
#define AuxScanWDT			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA8)
#define SMBUS_STATUS            SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xA9)
#define PS2_SSIRQ_Channel	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAA)
#define PS2_KBCmd_BK		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAB)
#define KB_Command              SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAC)
#define MSPending               SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAD)
#define PS2_ReTryCunt           SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAE)
#define SMBCRC8_A               SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xAF)
/* offset 0xB0 ~ 0xB3 */
#define u_Flag                  (*((FLAGS *)(SRAM_DATA_CORE_BASE_ADDR+0xB0)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Flag                    u_Flag
#define SCAN_INH		        bit0
#define SENDING_SCAN_RC         bit1 
#define PASS_READY              bit2
#define NEW_PRI_K               bit3
#define NEW_PRI_M               bit4
#define VALID_SCAN              bit5
#define uLED_ON                 bit6
#define ENTER_LOW_POWER         bit7
#define ACPI_IN_PROGRESS        bit8
#define ACPI_SMB_ALARM          bit9
#define LEGACY_SMB_ALARM        bit10
#define ALT_PRESSED             bit11
#define CTRL_PRESSED            bit12
#define DETECT_EC_B0            bit13
#define DETECT_EC_B1            bit14
#define Aux_Not_RESP            bit15
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define u_Ext_Cb2               (*((EXT_CB2 *)(SRAM_DATA_CORE_BASE_ADDR+0xB4)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Ext_Cb2                 u_Ext_Cb2
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define Save_Kbd_State 		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xB5)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*
 * maskLEDS is used by Keyboard Command EDh to qualify the data.
 * If any bits outside of the mask are attempted to be modifed
 * by Keyboard Command EDh, the command is rejected.
 */
#define maskLEDS                0x07

/*
 * shiftCODESET is used to shift the bits for the saved Scan Code
 * Set number to the lower part of the byte.  maskCODESET is then
 * used to isolate those bits.
 */
#define shiftCODESET            3
#define maskCODESET             0x03

/*
 * The enabled state of the external keyboard
 * modified by Keyboard Command F4h/F5h.
 */
#define maskAUXENAB             (1 << 5)

/* Bit 0 through 5 used for code. */
#define maskKBD_RESPONSE_CODE   0x03F
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Reserved 0xB6, 0xB7 */

/* offset 0xB8 ~ 0xBB */
#define u_Int_Var               (*((INT_VAR *)(SRAM_DATA_CORE_BASE_ADDR+0xB8)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define Int_Var                 u_Int_Var
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define KBHIReponse 		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xBC)

#define u_temp_scanner_state \
    (*((key_state *)(SRAM_DATA_CORE_BASE_ADDR+0xBD)))
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define temp_scanner_state      u_temp_scanner_state
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Reserved offset 0xBE ~ 0xBF */
#define ScanCode2_PS2KB_LAST_SCANCODE \
    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xC0)
#define ScanCode2_PS2KB_F0_PREFIX \
    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xC3)
#define ScanCode2_PS2KB_E0_PREFIX \
    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xC6)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* for shub offset 0xC8 ~ 0xDF */
#define SHUB_CORE_BUF_START         (SRAM_DATA_CORE_BASE_ADDR+0xC8)

#define HW1_ISR_DBG_MSG             SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xC8) 
#define SENSOR_I2C_CHANNEL_WDT      SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xC9)  
#define USB_ISR_SERVICE             SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCA)                         
#define ONE_SHOT_CMD_WDT            SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCB)                                        
#define ONE_SHOT_CMD_INDEX          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCC)
#define ONE_SHOT_MAIN_SENSOR_INDEX  SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCD)							    
#define SENSOR_ISR_SERVICE_FLAG_OS  SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCE)		
#define EXTEND_EVENT_ISR_SERVICE_FLAG \
    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xCF)

//#define USB_ISR_SERVICE_FLAG      SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD4)
#define SENSOR_INIT_STEP            SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD5)
#define HID_INTERFACE_STEP          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD6)
#define HID_INTERFACE_INIT_STEP     SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD7)
#define USB_SLAVE_INIT_STEP         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD8)
#define SENSOR_DEVICE_INIT_RETRY    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xD9)
#define EC_HANDLE_TASK_INDEX        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDA)
#define SENSOR_ISR_SERVICE_FLAG     SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDB)
#define HID_OVER_I2C_ISR_SERVICE_FLAG \
    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDC)
#define SENSOR_INIT_STEP_FAIL_CAUSE SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDD)
#define EC_HANDLE_FUSION_TASK_STATUS \
    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDE)
#define EC_HANDLE_HID_TASK_STATUS   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xDF)

#define tmpEvent					SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xE0)
#define tmpSSKey2					SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xE1)
#define tmpSSKeyCnt					SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xE2)
#define nSSKeyTbl					SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xE3)
#define ntmpKeyTbl					SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xE4)


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define TIMER_1MS_TICKS     SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR+0xF0)

/* Reserved offset 0xF4 ~ 0xF9 */

#define KeyScanKSI		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFA)
#define KeyScanKSO		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFB)
#define KeyScanACK		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFC)
#define ExtKeyScanKSI	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFD)
#define ExtKeyScanKSO	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFE)
#define UtilityFlag		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR+0xFF)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define KBDebug   		    BIT6    /* 1: for keyboard matrix utility */
#define SMBusDebug		    BIT7    /* 1: for smbus utility */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* 
 * ****************************************************************************
 * Global variable 02 for kernel code
 * ****************************************************************************
 */
/* Scanned keyboard buffer storage. offset 0x00 ~ 0x0F */
#define bKEY_BUFF               SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x00)
/* array offset 0x10 ~ 0x17 */
#define Pass_Buff			    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x10)

#define Pass_Buff_Idx		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x18)
#define Pass_On			        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x19)
#define Pass_Off			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1A)
#define Pass_Make1			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1B)
#define Pass_Make2			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1C)
#define PS_Debug_ECToHost       SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1D)
#define PS_Debug_DeviceToEC     SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1E)
#define DebugTest               SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x1F)

#define KB_HW_ExtendKeyIndex        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x20)
#define KB_HW_ExtendKeyTypematic    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x21)

/* Reserved offset 0x22 */

#define bTMrepeat			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x23)
#define bTMdelay			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x24)
#define bTMcount			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x25)
#define diode_key			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x26)

/* Reserved offset 0x27 ~ 0x2F */

#define DBG_IPC                 SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0x30)
#define DBG_IPSW                SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0x34)
#define DBG_SP                  SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0x38)
#define DBG_ITYPE               SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0x3C)

/* Reserved offset 0x40 */
#define RamCodePort 			SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x40)
#define PS2BufHead			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x41)
#define PS2BufTail			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x42)
#define PS2DataPinStatusCounter SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x43)
#define PS2TypeCounter		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x44)
#define WinFlashMark		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x45)
#define WinFlashMark2		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x46)
/* array offset 0x47 ~ 0x4A */
#define KBDataPending           SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x47)
#define KBPendingRXCount        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x4B)
#define KBPendingTXCount        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x4C)
#define PS2DataPinStatus	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x4D)

/* Reserved offset 0x4E ~ 0x4F */
                                        
#define HID_INT_Event_In_Index  SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x50)
#define HID_INT_Event_Out_Index SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x51)
/* array offset 0x52 ~ 0x5F */
#define HID_INT_Event_Buffer    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x52)

/* Reserved 0x60 ~ 0x80 */
					                    
#define ExtendScanPin	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x81)
#define RSTStatus		        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x82)
#define RamcodeCmd			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x83)
#define RamcodeSend			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x84)

/* Reserved 0x85 ~ 0x88 */
                                        
#define SMBus1RecoverCunt	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x89)
#define SMBus2RecoverCunt	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8A)
#define SMBus3RecoverCunt	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8B)
#define SMBus4RecoverCunt	    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8C)
#define FN_Key_Break            SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8D)
#define FN_Key_Break_HS         SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8E)
#define SMBus_BusyRecover       SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x8F)

/* array offset 0x90 ~ 0x93 */
#define	SPIIDBuf			    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x90)
#define SPIIndex			    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x94)
/* array offset 0x95 ~ 0x97 */
#define	SPIAddrCycle		    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x95)
#define SPIAEraseCmd		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x98)
#define SPIReadStatus		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x99)
#define SPIWriteStatus	        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x9A)

#define SPIID				    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x9D)
#define SSTDeviceID  		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x9E)
#define EC_Indirect_Selection   SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0x9F)

#define Timer1msCounter		    SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xA0)
#define Service_Timer1MinEvent  SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xA4)
#define Service_Timer10msEventA SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xA8)
#define Service_Timer10msEventB SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xAC) 
#define Service_Timer50msEventA SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xB0)
#define Service_Timer50msEventB SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xB4)
#define Service_Timer50msEventC SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xB8)
#define Service_Timer100msEventA SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xBC)
#define Service_Timer100msEventB SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xC0)
#define Service_Timer100msEventC SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xC4)
#define Service_Timer500msEventA SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xC8)
#define Service_Timer500msEventB SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xCC)
#define Service_Timer500msEventC SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xD0)
#define Service_Timer1SecEventA SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xD4)
#define Service_Timer1SecEventB SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xD8)
#define Service_Timer1SecEventC SD_ulong_32(SRAM_DATA_CORE_BASE_ADDR_02+0xDC)

/* array offset 0xE0 ~ 0xEF, 16 bytes */
#define	FnKeyBundledFalg	    SD_Ptr_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0xE0)

#define KBCPortPending		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0xF0)
#define PMPortPending		    SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0xF1)

/* Reserved offset 0xF2 ~ 0xF7 */

#define Last_table_entry        SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0xF8)
#define table_entry_bk          SD_uchar_8(SRAM_DATA_CORE_BASE_ADDR_02+0xF9)

/* 
 * ****************************************************************************
 * I2C CommandQ buffer
 * ****************************************************************************
 */
#define I2C_CMDQ_BUFFER_BASE_ADDR   0x00080F00

/*   0 ~  63: Channel D    */
/*  64 ~ 127: Channel E    */
/* 128 ~ 191: Channel F    */
/* 192 ~ 255: fast command */
#define I2C_CMDQ_TX_CNT			0x40
#define I2C_FAST_CMD_CNT		0x40

#define I2C_CMDQ_TX_ADDR		(I2C_CMDQ_BUFFER_BASE_ADDR)
#define I2C_FAST_CMD_ADDR		(I2C_CMDQ_BUFFER_BASE_ADDR + I2C_CMDQ_TX_CNT*3)

#endif /*CORE_MEMORY_H_*/


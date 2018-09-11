//*****************************************************************************
//
//  oem_memory.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

/*-----------------------------------------------------------------------------
*
* [Kernel Memory Rang] 0x00080000 ~ 0x000801FF 512 bytes
*
* 0x00080000-0x000800FF   kernel code RAM 0
* 0x00080100-0x000801FF   kernel code RAM 1
*
* [OEM Memory Rang] 0x00080200 ~ 0x00080FFF 3.25K bytes
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
*
* [Kernel I2C Memory Rang] 0x00080F00 ~ 0x00080FFF 256 bytes
*
* 0x00080F00-0x00080FFF   kernal I2C RAM F
*
*---------------------------------------------------------------------------*/
 
#ifndef OEM_MEMORY_H
#define OEM_MEMORY_H

#define EC_RAMBase 		    			0x00080000

#define OEMRAM2 		    			EC_RAMBase+0x0200   						// EC_RAMBase+0x0200 
#define OEMRAM3 		    			EC_RAMBase+0x0300   						// EC_RAMBase+0x0300 
#define OEMRAM4 		    			EC_RAMBase+0x0400   						// EC_RAMBase+0x0400 
#define OEMRAM5 		    			EC_RAMBase+0x0500   						// EC_RAMBase+0x0500 
#define OEMRAM6		           EC_RAMBase+0x0600   						  // EC_RAMBase+0x0600 
#define OEMRAM7 		    			EC_RAMBase+0x0700   						// EC_RAMBase+0x0700 

#define OEMRAM8 		    			EC_RAMBase+0x0800   						// EC_RAMBase+0x0800 
#define OEMRAM9 		    			EC_RAMBase+0x0900   						// EC_RAMBase+0x0900 
#define OEMRAMA 		    			EC_RAMBase+0x0A00   						// EC_RAMBase+0x0A00 
#define OEMRAMB 		    			EC_RAMBase+0x0B00   						// EC_RAMBase+0x0B00 
#define OEMRAMC 		    			EC_RAMBase+0x0C00   						// EC_RAMBase+0x0C00 
#define OEMRAMD 		    			EC_RAMBase+0x0D00   						// EC_RAMBase+0x0D00 
#define OEMRAME 		    			EC_RAMBase+0x0E00   						// EC_RAMBase+0x0E00 
#define OEMRAMF 		    			EC_RAMBase+0x0F00   						// EC_RAMBase+0x0F00 


//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 2  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define	SysPowState							SD_uchar_8(OEMRAM2+0x00) 
#define   PWSeqStep            				SD_uchar_8(OEMRAM2+0x01)                       
#define   DelayDoPowerSeq					SD_uint_16(OEMRAM2+0x02)                 
#define   PowSeqDelay          				SD_uint_16(OEMRAM2+0x04) 
#define   DeepSleepCunt        				SD_uchar_8(OEMRAM2+0x06)                
#define   CombineKeyStatus					SD_uchar_8(OEMRAM2+0x08)             
 #define  HotkeyStoreROM   BIT0
 #define  b2FnRNL          BIT2  
 #define  b6Fn4Key4        BIT6
 #define  b7Fn4			       BIT7

#define   ComboKeyState						SD_uchar_8(OEMRAM2+0x09)              
 #define ComboKeyValid     BIT5
 #define CallingFunc       BIT6
 #define ECDebugMode       BIT7

#define   LastComboKey						SD_uchar_8(OEMRAM2+0x0A)            


#define   KB_OBF_count						SD_uchar_8(OEMRAM2+0x0B)                


#define   CmdData2							SD_uchar_8(OEMRAM2+0x0C)                 

#define   uVGATurboFun						SD_uchar_8(OEMRAM2+0x0D)                 
 #define  uEnableGPS           		BIT0
 #define  uDisPowerSteeringOK  		BIT1
 #define  uDisVGATurboOK       		BIT2

#define   eFlashVarifyOK					SD_uchar_8(OEMRAM2+0x0E)                   
#define   eFlash_r_data						SD_uchar_8(OEMRAM2+0x0F)                   
#define   eFlashA1							SD_uchar_8(OEMRAM2+0x10)                 
#define   eFlashA0							SD_uchar_8(OEMRAM2+0x11)                    
#define   eEEPROMBank						SD_uchar_8(OEMRAM2+0x12)                   
#define   eEEPROMAddrsss					SD_uchar_8(OEMRAM2+0x13)                   
#define   eEEPROMData						SD_uchar_8(OEMRAM2+0x14)                 
#define   eEEPROMMarkData					SD_uchar_8(OEMRAM2+0x15)                  
#define   e256ByteCnt						SD_uchar_8(OEMRAM2+0x16)                     
#define   eUpdateEEPROMCnt					SD_uchar_8(OEMRAM2+0x17)            
#define   eUpdatepDevStusCnt				SD_uchar_8(OEMRAM2+0x18)               
#define   WSMbusTemp01						SD_uchar_8(OEMRAM2+0x19)              
#define   WSMbusTemp02						SD_uchar_8(OEMRAM2+0x1A)          
#define   SMbusFailCnt2						SD_uchar_8(OEMRAM2+0x1B)              
#define   SMbusFailCnt3						SD_uchar_8(OEMRAM2+0x1C)         
#define   RSOC1_DEBUG  						SD_uchar_8(OEMRAM2+0x1D)
 #define  RSOC_DEBUG_EN             BIT7
 #define  RSOC1_DEBUG_GET            IS_MASK_SET(RSOC1_DEBUG, RSOC_DEBUG_EN)
#define   RSOC2_DEBUG						   SD_uchar_8(OEMRAM2+0x1E)
 #define  RSOC2_DEBUG_GET            IS_MASK_SET(RSOC2_DEBUG, RSOC_DEBUG_EN)
#define  BIOS_WDT_Cnt				       SD_uchar_8(OEMRAM2+0x1F)
#define   AD_ID_Detect_Delay				 SD_uchar_8(OEMRAM2+0x20) 
#define   ACPISMBch  						   SD_uchar_8(OEMRAM2+0x21)   
#define   BatLowCnt							   SD_uchar_8(OEMRAM2+0x2C)  
#define   RSMshutdownCnt					   SD_uchar_8(OEMRAM2+0x2D)   
#define   BATFWupdate_Delay			   SD_uchar_8(OEMRAM2+0x2E)
#define   AC_OUT_Prochot_CNT  			 SD_uchar_8(OEMRAM2+0x2F)

#define     Get_Batt_debounce_count			SD_uchar_8(OEMRAM2+0x34)                     
#define     Get_Batt_debounce_hash1			SD_uchar_8(OEMRAM2+0x35)		                 
#define     Get_Batt_debounce_hash2			SD_uchar_8(OEMRAM2+0x36)                   




#define   Bat0x0BFakeCnt					SD_uchar_8(OEMRAM2+0x37)                   

#define   BatteryAlarm						SD_uchar_8(OEMRAM2+0x38)                         
 #define  BATOCP          BIT0
 #define  BATOTP          BIT1
 #define  ACSTPP          BIT2
 #define  BATPercl_5      BIT3
 #define  HybridSTPP      BIT4
 #define  DISTURBO        BIT5
 #define  ADPOUT          BIT6

#define   BattTemp							SD_uchar_8(OEMRAM2+0x3B)


#define   inhibit2sec						SD_uchar_8(OEMRAM2+0x3C)                
                 

#define  PWRBTN_WDTFLAG								SD_uchar_8(OEMRAM2+0x42)                                       

typedef enum
{
  Chk_Wrong_ADP_Status_Null,
  Chk_Wrong_ADP_Status_wait_2sec,
  Chk_Wrong_ADP_Status_wait_10sec,
  Chk_Wrong_ADP_Status_ACOFF_Lock
}Chk_Wrong_ADP_Status_Type;
#define   Chk_Wrong_ADP_Status				SD_uchar_8(OEMRAM2+0x45)                           
#define   API_ID							SD_uint_16(OEMRAM2+0x46)               
#define   Chk_Wrong_10ms_Count				SD_uint_16(OEMRAM2+0x48) 
#define   AdapterID							SD_uchar_8(OEMRAM2+0x4A) 

#define   ACIN_FallINT_Count				SD_uchar_8(OEMRAM2+0x50)                     
#define   ACIN_BLINK3						SD_uchar_8(OEMRAM2+0x51)                       
typedef enum
{
  Chk_Hybrid_STPP_Status_SystemOverCP,
  Chk_Hybrid_STPP_Status_SystemOverChargerTurboPoint_NonStartTurbo,
  Chk_Hybrid_STPP_Status_SystemOverChargerTurboPoint_TurboStarted,
  Chk_Hybrid_STPP_Status_Disable_CPUTurbo,
  Chk_Hybrid_STPP_Status_CPU_throttling,
  Chk_Hybrid_STPP_Status_Recover_CPU_throttling,
  Chk_Hybrid_STPP_Status_Recover_Charger_Turbo
}Chk_Hybrid_STPP_Status_Type;

#define   MaxPwrCtrlFsm								SD_uchar_8(OEMRAM2+0x52)                                    

#define   Chk_Hybrid_STPP_min_BattGasgauge			SD_uchar_8(OEMRAM2+0x53)               
#define   Chk_Hybrid_STPP_Batt_Output_Power			SD_uchar_8(OEMRAM2+0x54)            
#define   Chk_Hybrid_STPP_Batt_Output_Power_Limit	SD_uchar_8(OEMRAM2+0x56)     
#define   uBattFUDStatus		SD_uchar_8(OEMRAM2+0x57)
 #define  uFUDUpdateTimes        				BIT3
 #define  uFUDWriteEN2       				BIT5
 #define  uFUdayClr2         				BIT7
#define   BackLight_En_Delay						SD_uchar_8(OEMRAM2+0x58)                     

#define   Batteryin_delay							SD_uchar_8(OEMRAM2+0x59)                      

#define   uProject0									SD_uchar_8(OEMRAM2+0x5A)
 #define  ECPORT80          				BIT0
 #define  NeedRecoveryNotSupportADP         BIT1
 #define  NeedRecoveryLimitedSupportADP     BIT2
 #define  uFUDUpdateTimes        				BIT3
 #define  uSysTimeOK        				BIT4
 #define  uFUDWriteEN       				BIT5
 #define  uFUdayFail        				BIT6
 #define  uFUdayClr         				BIT7
 #define  GET_ECPORT80      						IS_MASK_SET(uProject0,ECPORT80) 
 #define  EN_ECPORT80       						SET_MASK(uProject0,ECPORT80)
 #define  DIS_ECPORT80      						CLEAR_MASK(uProject0,ECPORT80) 


#define   CRCDelay									SD_uchar_8(OEMRAM2+0x5B)
#define   FAN_PCH_Max								SD_uchar_8(OEMRAM2+0x5C)
#define   RPM_PCH_Max								SD_uchar_8(OEMRAM2+0x5D)

#define   pPWRLEDS3_off_Count						SD_uchar_8(OEMRAM2+0x5F)
#define   ADPI_Data									SD_Ptr_uint_16(OEMRAM2+0x60)//0x60~0x67                 

#define   XWTemp1									SD_uint_16(OEMRAM2+0x68)                             

#define   MpcSystemTotalPower						SD_uchar_8(OEMRAM2+0x6A)                     
#define   ADPI2Sec									SD_uchar_8(OEMRAM2+0x6B)                 
#define   MpcCP										SD_uchar_8(OEMRAM2+0x6C)                 

#define   MpcCpuTurboDisablePoint					SD_uchar_8(OEMRAM2+0x6D)                  
#define   MpcCpuTurboRecoverPoint					SD_uchar_8(OEMRAM2+0x6E)                  
#define   MpcCpuThrottlingPoint						SD_uchar_8(OEMRAM2+0x6F)                 
#define   MpcCpuThrottlingRecoverPoint				SD_uchar_8(OEMRAM2+0x70)                  
#define   MpcGpuThrottlingPoint						SD_uchar_8(OEMRAM2+0x71)                   
#define   MpcGpuThrottlingRecoverPoint				SD_uchar_8(OEMRAM2+0x72)                 
#define   MpcAcCpuTurboDisablePoint					SD_uchar_8(OEMRAM2+0x73)               
#define   MpcAcCpuTurboRecoverPoint					SD_uchar_8(OEMRAM2+0x74)               
#define   MpcAcCpuThrottlingPoint					SD_uchar_8(OEMRAM2+0x75)               
#define   MpcAcCpuThrottlingRecoverPoint			SD_uchar_8(OEMRAM2+0x76)                
#define   MpcAcGpuThrottlingPoint					SD_uchar_8(OEMRAM2+0x77)            
#define   MpcAcGpuThrottlingRecoverPoint			SD_uchar_8(OEMRAM2+0x78) 

#define   pProject4									SD_uchar_8(OEMRAM2+0x79)
 #define  pPWSWdisable      				BIT0  //Disable Power switch function(PBTN_OUT and 7s power switch WDT).  
 #define  pLastLPCstatus    				BIT1  //record LPC last status
 #define  pComboKeyWin      				BIT5  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define  pComboKeyLShift   				BIT6  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define  pComboKeyRArrow   				BIT7  //Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
 #define pLastLPCstatusGET    						IS_MASK_SET(pProject4,pLastLPCstatus)
 #define pLastLPCstatusSET    						SET_MASK(pProject4,pLastLPCstatus)
 #define pLastLPCstatusCLR    						CLEAR_MASK(pProject4,pLastLPCstatus)


#define   ADPI_AvgData								SD_uint_16(OEMRAM2+0x7A)                         
 
 
#define   PSW_COUNTER								SD_uint_16(OEMRAM2+0x7C)              
#define   DSxPowState								SD_uchar_8(OEMRAM2+0x7E)                  
#define   AOU_status								SD_uchar_8(OEMRAM2+0x7F)
 #define AOUPort_Dev    					BIT0    // 1 = Exist USB Device in AOU port.
 #define AOUPort_DevSET    							SET_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevCLR    							CLEAR_MASK(AOU_status,AOUPort_Dev)
 #define AOUPort_DevGET    							IS_MASK_SET(AOU_status,AOUPort_Dev)
#if (PLATFORM_CHIP == INTEL)
#define   SUSACK_LOW_CNT							SD_uint_16(OEMRAM2+0x80)                              
#define   SUSACK_HI_CNT								SD_uchar_8(OEMRAM2+0x82)                            
#endif // PLATFORM_CHIP
#define   SLP_A_CNT									SD_uchar_8(OEMRAM2+0x83)                              
#define   S3S4DelayCnt								SD_uchar_8(OEMRAM2+0x84)                             
#define   cADPIDTemp								SD_uchar_8(OEMRAM2+0x85)  // Bit7~4: ID , BIT3~0:Count   
#define   TPID1										SD_uchar_8(OEMRAM2+0x86)
#define   TPID2										SD_uchar_8(OEMRAM2+0x87)
#define   TPID3										SD_uchar_8(OEMRAM2+0x88)
#define   TPID4										SD_uchar_8(OEMRAM2+0x89)
#define   p2BeepCnt1								SD_uchar_8(OEMRAM2+0x8A)
#define   p2BeepCnt2								SD_uchar_8(OEMRAM2+0x8B)
#define   p2BeepCnt3								SD_uchar_8(OEMRAM2+0x8C)
#define   p2BeepCtrl								SD_uchar_8(OEMRAM2+0x8D)
 #define  p2RptCnt          				BIT0
#define   FnStatus									SD_uchar_8(OEMRAM2+0x8E)
#define   SysStatus									SD_uchar_8(OEMRAM2+0x8F)
 #define  LidStatus          				BIT0
 #define  ERR_ShuntDownFlag  				BIT1
 #define  FnF3WinKeyDn        				BIT2
 #define  FnF3PKeyUp          				BIT3
 #define  CloseLid            				BIT5
 #define  LidKBIgnore         				BIT7

#define   LID_DEBOUNCE_CNT							SD_uchar_8(OEMRAM2+0x90)
#define   PWRBTN_pressCnt							SD_uchar_8(OEMRAM2+0x91)
#define   LID_readytime								SD_uchar_8(OEMRAM2+0x92)
#define   ExtTimeCnt								SD_uchar_8(OEMRAM2+0x93)
#define   SysStatus2								SD_uchar_8(OEMRAM2+0x94)
 #define  PWRBTN_press_retry 				BIT0
 #define  b2NOVOKey_Press    				BIT2
 #define  b3LCtrl_Pressed    				BIT3
 #define  b4VGATemp_Init     				BIT4      // For AMD(ATI) used. 1: GPU VGA initial finish.
 #define  LidCloseKBMSDis    				BIT5
#define   LEDcontrol_bak							SD_uchar_8(OEMRAM2+0x95)
#define   Led_status								SD_uchar_8(OEMRAM2+0x96)
 #define  PWRLED_ON           				BIT7      // 1: PWRLED ON.   0: PWRLED OFF
 #define  PWRLED_Blinking     				BIT6      // 1: PWRLED Blinking. 
 #define  PWRLED_Gradually    				BIT5      // 1: Gradually.
 #define  LOGOLED_ON          				BIT3      // 1: LOGOLED ON.   0: LOGOLED OFF
 #define  LOGOLED_Blinking    				BIT2      // 1: LOGOLED Blinking. 
 #define  LOGOLED_Gradually   				BIT1      // 1: LOGOLED Gradually.
 #define  BatLED_test         				BIT0      // 1: battery LED test mode.

#define   Led_PWM_step								SD_uchar_8(OEMRAM2+0x97)
 #define  LED_countupdown  					BIT7      // 0: count up   1:count down.
#define   PowerLed_Count							SD_uchar_8(OEMRAM2+0x98)               
#define   LENOVOPMFW_Temp							SD_uchar_8(OEMRAM2+0x99)
 #define  BATTERY_CYCLE_RUNNING   			BIT0   //battery Cycle mode running  // Protection Mode
 #define  BATTERY_CALIBRATION_OK  			BIT1   // Calibration Finish
 #define  BATTERY_FULLED_100      			BIT2   // 100% Fulled ,from nonFull.
 #define  b3EC_ONStatus           			BIT3   // EC not entry Sleep status.
 #define  EEPROM_Token            			BIT4
 #define  b5EC_ON_EN              			BIT5   // 1: EC turn on always electricity.

//
#define   cCmd										SD_uchar_8(OEMRAM2+0x9C)
 #define  cC_PCHPWR_Keep    				BIT0
 #define  b3BkOff           				BIT3
 #define  cC_BkOff          				BIT3
 #define  cC_TestBtnEn      				BIT6	// Send scancode for test button
 #define  cC_LidIgnore      				BIT7	// Ignore lid function

#define   cKBStatus									SD_uchar_8(OEMRAM2+0x9D)
 #define  cKBS_KBType0     					BIT0  // Keyboard selection Bit0 control by EC
 #define  cKBS_KBType1     					BIT1  // Keyboard selection Bit1 control by EC
 #define  cKBS_KBInit      					BIT2  // Keyboard Type initial
 #define  cKBS_TPInS3      					BIT3
 #define  cKBS_TPData      					BIT4
 #define  cKBS_KBPress     					BIT5
 #define  cKBS_KBWakeUp    					BIT6  // KB Wake up
 #define  cKBS_TPWakeUp    					BIT7  // TP Wake up

#define   cBattFlag0								SD_uchar_8(OEMRAM2+0x9E)
 #define  cBF0_Full          				BIT0
 #define  cBF0_FullToTarget  				BIT1
 #define  cBF0_GoTarget      				BIT2
 #define  cBF0_BLMode        				BIT3
 #define  cCmdAcOff          				BIT7

#define   cSPIEEPROM								SD_uchar_8(OEMRAM2+0x9F)    
 #define  cSPI_ROMCopyStart  				BIT2           //it's for debug

#define   Bat1_FPChgFlag							SD_uchar_8(OEMRAM2+0xA0)

#define   cTargetGauge								SD_uchar_8(OEMRAM2+0xA1)


#define   cPanelId									SD_uchar_8(OEMRAM2+0xA2)

#define   cBrightUpDelayCnt							SD_uchar_8(OEMRAM2+0xA3)
#define   cBrightDnDelayCnt							SD_uchar_8(OEMRAM2+0xA4)
#define   cRecoveryBrightValue						SD_uchar_8(OEMRAM2+0xA5)

#define   cSysActionFlag							SD_uchar_8(OEMRAM2+0xA6)

#define   cADPThrottling							SD_uchar_8(OEMRAM2+0xA7)
#define   cTHERMThrottling							SD_uchar_8(OEMRAM2+0xA8)
#define   cBATTThrottling							SD_uchar_8(OEMRAM2+0xA9)

#define   ISCT_Again								SD_uchar_8(OEMRAM2+0xAA)
#define   pBattLearn								SD_uchar_8(OEMRAM2+0xAB)
 #define  uACoffstchg      					BIT0
 #define  uRefreshmode     					BIT1
 #define  us4ResumeACOFF   					BIT2
 #define  uACoffdschg      					BIT3

#define   pFakeCHGCnt								SD_uchar_8(OEMRAM2+0xAC)
#define   S0S5PWRcodition							SD_uchar_8(OEMRAM2+0xAD)
 #define S0S5PWROFF    						BIT0

#define   pProject3									SD_uchar_8(OEMRAM2+0xAE)
 #define  pEntryTB                 			BIT0
 #define  pExitTB                  			BIT1
 #define  pUpdateCpuTempFromBios   			BIT4	//Work-around for Intel issue "Some time PECI will hangup".Update from BIOS.
 #define  pIsMeFlashing            			BIT5	//Work-around for avoid the PCH communication fail during ME flashing.
 #define  uReSendBat1ChgQevent     			BIT7
 
#define   AOURSTCNT									SD_uchar_8(OEMRAM2+0xAF)
  
#define   EC_AOU_DBG_FLAG							SD_uchar_8(OEMRAM2+0xB0)
  // BIT6:0 used for setting battery level for testing.
 #define AOU_DC_LEVEL_DBG   				BIT7
#define   EC_AOU_PWR_TYPE							SD_uchar_8(OEMRAM2+0xB1)  
  // BIT7:4 AOU port power mode
  // BIT3:0 Non-AOU port power mode

#define   EVT_STATUS1								SD_uchar_8(OEMRAM2+0xB2)  //Event status flag 1    
 #define  M_bat_in           BIT0            // 1: Battery 1 exist
 #define  S_bat_in           BIT1            // 1: Battery 1 exist
 #define  adapter_in         BIT2            // 1: AC adapter exist
 #define  wait_PSW_off       BIT3            // 1: waiting for PSW released
 #define  LAN_WAKE_Status    BIT4            //
 #define  KB_FN_Status       BIT5            // 1: Fn Key Pressed
 #define  WLAN_WAKE_Status   BIT6            //



#define   EVT_STATUS2								SD_uchar_8(OEMRAM2+0xB3)  //Event status flag 1  
 #define  BottomCoverFlag             BIT0
 #define  DummyFlag          				BIT7            

 
#define   uAdpWattTemp								SD_uchar_8(OEMRAM2+0xB4)
#define   uAdpWattCnt								SD_uchar_8(OEMRAM2+0xB6)

#define   initMark									SD_uchar_8(OEMRAM2+0xB7) // CMW 20121203 for PS8625 translator
#define   TEMP_Error								SD_uchar_8(OEMRAM2+0xB8) // CMW 20121203 for PS8625 translator
#define   RFID_eFlashA1								SD_uchar_8(OEMRAM2+0xB9)
#define   RFID_eFlashA0								SD_uchar_8(OEMRAM2+0xBA)
#define   RFID_eEEPROMBlock							SD_uchar_8(OEMRAM2+0xBB)
#define   RFID_eEEPROMAddrsss						SD_uchar_8(OEMRAM2+0xBC)
#define   RFID_eEEPROMData							SD_uchar_8(OEMRAM2+0xBD)
#define   cPWRONFail								SD_uchar_8(OEMRAM2+0xBE)
#define   SYS_MISC1									SD_uchar_8(OEMRAM2+0xBF)
  #define   G3Mode				BIT3
 #define  Crisis_On    						BIT6
 #define  ACPI_OS      						BIT7

#define   ADP_I										SD_uint_16(OEMRAM2+0xC0)

#define   AutoTimer									SD_uchar_8(OEMRAM2+0xC2)
 #define   b7AutoBootTm  					BIT7

#define   SCI_Event_In_Index						SD_uchar_8(OEMRAM2+0xC3)
#define   SCI_Event_Out_Index						SD_uchar_8(OEMRAM2+0xC4)
#define   SCI_Event_Buffer							SD_Ptr_uchar_8(OEMRAM2+0xC5)  //0x1C5 ~ 0xCC	//EVENT_BUFFER_SIZE


#define   PM1PendingTXCount							SD_uchar_8(OEMRAM2+0xCD)
#define   PM1PendingRXCount							SD_uchar_8(OEMRAM2+0xCE)
#define   PM1DataPending							SD_Ptr_uchar_8(OEMRAM2+0xCF)         //0x1CF ~ 0x1D4
#define   PM2PendingTXCount							SD_uchar_8(OEMRAM2+0xD5)
#define   PM2PendingRXCount							SD_uchar_8(OEMRAM2+0xD6)
#define   PM2DataPending							SD_Ptr_uchar_8(OEMRAM2+0xD7)         //0x1D7 ~ 0x1DC 

#define   uCritBattWakeThre							SD_uchar_8(OEMRAM2+0xDD)

#if Support_IFFS
#define   uIffsCnt									SD_uchar_8(OEMRAM2+0xDE)
 #define IFFS_Enable     					BIT0
#endif // Support_IFFS
#if SW_ISCT
#define   uISCT										SD_uchar_8(OEMRAM2+0xDF)
 #define  b0IRST_WAKE_Enable    			BIT0
 #define  b1IRST_WAKE_Finish    			BIT1
 #define  b2IRST_TimeRecord     			BIT2
 #define  b3ISCT_Permit         			BIT3
 #define  b4ISCT_FanDis         			BIT4        // ISCT 1:Disable Fan cotrol.
 #define  b5ISCT_BKOFF          			BIT5        // ISCT 1:Enable Backlight OFF.
 #define  b6ISCT_Wake           			BIT6
 #define  b7ISCT_ShiftKey       			BIT7

#define   uISCT_2									SD_uchar_8(OEMRAM2+0xE0)
 #define  b0ISCT_pwrBTNpres     			BIT0        // Check power button wake up status.
 #define  b1ISCT_LidWake        			BIT1        // Check Lid wake up status.
 #define  b2ISCT_WlanLED        			BIT2
 #define  b3ISCT_MUTE           			BIT3        // ISCT 1:Enable MUTE.
 #define  b4ISCT_Camera         			BIT4        // ISCT 1:Disable Camera.
 #define  b5ISCT_PwrLED         			BIT5        // ISCT 1:Disable Power LED.
#endif // SW_ISCT



#if NV_GC6
#define   GC6EventCnt								SD_uchar_8(OEMRAM2+0xE1)
#endif // NV_GC6
#define   pACDCCnt									SD_uchar_8(OEMRAM2+0xE2)
#define   pTempBank									SD_uchar_8(OEMRAM2+0xE3)
#define   pTempAddress								SD_uchar_8(OEMRAM2+0xE4)
#define   nBattTsVolt								SD_uchar_8(OEMRAM2+0xE5)
#define   cThrottlingSet							SD_uchar_8(OEMRAM2+0xE6) // 00: Nothing, 01: Thr 12.5%, 02: Thr 25%,..,08: Thr 100%
  #define cTHRM_SW           				BIT6
  
#define 	 pCPUBasePStage							SD_uchar_8(OEMRAM2+0xE7)
#define   PWRCONTROL								SD_uchar_8(OEMRAM2+0xE8)
  #define MAINPWRONTrig      				BIT0

#define   LED_PWM_Status							SD_uchar_8(OEMRAM2+0xEB)
#define   Cgr_LED_Count								SD_uchar_8(OEMRAM2+0xEC)
#define   IP_GHOSTBEEP								SD_uchar_8(OEMRAM2+0xED)
#define   IP_GBEEPCnt								SD_uchar_8(OEMRAM2+0xEE)


//RESERVE 0x1EC ~ 0x1EF
#define   DEBOUNCE_CONT1							SD_uchar_8(OEMRAM2+0xF0)                   
#define   DEBOUNCE_CONT2							SD_uchar_8(OEMRAM2+0xF1)                    
#define   DEBOUNCE_CONT3							SD_uchar_8(OEMRAM2+0xF2)             
#define   DEBOUNCE_CONT4							SD_uchar_8(OEMRAM2+0xF3)                 
#define   DEBOUNCE_CONT5							SD_uchar_8(OEMRAM2+0xF4)                    
#define   DEBOUNCE_CONT6							SD_uchar_8(OEMRAM2+0xF5)                    
#define   DEBOUNCE_CONT7							SD_uchar_8(OEMRAM2+0xF6)                      
#define   DEBOUNCE_CONT8							SD_uchar_8(OEMRAM2+0xF7)                    
#define   DEBOUNCE_CONT9							SD_uchar_8(OEMRAM2+0xF8)                    
#define   DEBOUNCE_CONT10							SD_uchar_8(OEMRAM2+0xF9)                        
#define   DEBOUNCE_CONT11							SD_uchar_8(OEMRAM2+0xFA)                         
#define   DEBOUNCE_CONT12							SD_uchar_8(OEMRAM2+0xFB)                      
#define   DEBOUNCE_CONT13							SD_uchar_8(OEMRAM2+0xFC)                      
#define   DEBOUNCE_CONT14							SD_uchar_8(OEMRAM2+0xFD)                      
#define   DEBOUNCE_CONT15							SD_uchar_8(OEMRAM2+0xFE)                      
#define   DEBOUNCE_CONT16							SD_uchar_8(OEMRAM2+0xFF)    



//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 3  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define    pProject0								SD_uchar_8(OEMRAM3+0x00)  
 #define  b0DispToggleEn      			BIT0
 #define  bHYBAPUTURBO        			BIT1  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  bAPUTURBO           			BIT2  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  bRSOCAPUTURBO       			BIT3  // 1:Disable CPU Turbo, 0:Enable CPU Turbo
 #define  b6PostHotKey        			BIT6
 #define  b7SWITCHBAK         			BIT7
 
#define   pPROCHOT									SD_uchar_8(OEMRAM3+0x01)  
 #define  b0Thermal_PRCOHOTon    		BIT0
 #define  b1ISCT_PROCHOTon    			BIT1
 
 
#define   GPU_THROTTLE_SRC							SD_uchar_8(OEMRAM3+0x04)  
 #define  BAT_HYBRID_OTP    			BIT0
 #define  BAT_OCP           			BIT1
 #define  ADP_LIMIT_SUPPORT 			BIT2
 #define  MAX_POWER_CTRL    			BIT3
 #define  THERMAL_POLICY    			BIT4
 #define  External_Sensor   			BIT5
 
#define   AC_DROP_COUNT								SD_uchar_8(OEMRAM3+0x05) 
#define   ADP_ID_REC								SD_uint_16(OEMRAM3+0x06) 
#define   p2FUDayTemp								SD_uint_16(OEMRAM3+0x08) 
#define   p2FUDay									SD_uint_16(OEMRAM3+0x0A) 
#define   p2FUD_Year								SD_uint_16(OEMRAM3+0x0C) 
#define   p2FUD_Month								SD_uchar_8(OEMRAM3+0x0E) 
#define   p2FUD_Day									SD_uchar_8(OEMRAM3+0x0F) 
#define   Port80_Data								SD_Ptr_uchar_8(OEMRAM3+0x10)   // 0x210~0x21E SIZE:15
#define   Port80_Index								SD_uchar_8(OEMRAM3+0x1F) 
#define   pLastSID									SD_uchar_8(OEMRAM3+0x20) 
#define   pLastSID2									SD_uchar_8(OEMRAM3+0x21) 
#define   pLastSID3									SD_uchar_8(OEMRAM3+0x22) 
#define   pLastSID4									SD_uchar_8(OEMRAM3+0x23) 
#define   KeepBattRemineCap							SD_uint_16(OEMRAM3+0x24) 
#define   cPacketPowerLimit1						SD_uchar_8(OEMRAM3+0x26) 
#define   ChkBattery_FCCchg_count					SD_uchar_8(OEMRAM3+0x27) 
#define   ChkBattery_FCCchg_lastFCCL				SD_uchar_8(OEMRAM3+0x28) 
#define   ChkBattery_FCCchg_lastFCCH				SD_uchar_8(OEMRAM3+0x29) 
#define   CountSecAfterPswPressed					SD_uchar_8(OEMRAM3+0x2A) 
#define   pVCompareFlag								SD_uchar_8(OEMRAM3+0x2B) 
 #define  VC0TriggerOn        			BIT0
 #define  VC1TriggerOn        			BIT1
 #define  VC2TriggerOn        			BIT2
#define   TURBO_V									SD_uint_16(OEMRAM3+0x2C) 

#define   pEdgeProjectFlag1							SD_uchar_8(OEMRAM3+0x2E) 
 #define  pEdgeProjectGhostKey    BIT0
 #define  pEdgeACINOUTTrigger     BIT1
 #define  pEdgeNonSUPPORTADP      BIT2
 #define  pEdgeProjectDebugIn     BIT3
 #define  pEdgeACCUTOFF           BIT4
 #define  pEdgeTPReset            BIT5
 #define  pEdgeADPIDReady         BIT6
#if NV_GC6
 #define  pEdgeGC6EventHI         BIT7
#endif // NV_GC6

#define   pWorkAround								SD_uchar_8(OEMRAM3+0x30) 
#define   pWakeCnt									SD_uchar_8(OEMRAM3+0x31) 

#define   pSMIEvent									SD_uchar_8(OEMRAM3+0x32)
#define   pEdgeBATStopCharge						SD_uchar_8(OEMRAM3+0x33)

#define   pInhitTimer								SD_uint_16(OEMRAM3+0x34)
// Battery charge inhibit timer has a word-width value in units of minute, and is used to cancel inhibition
// of battery charge. The timer will start when high byte value is written in interface space, therefore low
// byte of timer value should be written first. 
// Writing 0000h cancels battery charge inhibition immediately. 
// If FFFFh is written, the timer will not expire. Reading the timer value is not defined.

#define   pEdgeBATStartCharge						SD_uchar_8(OEMRAM3+0x36)

#define   ACACOFFEn									SD_uint_16(OEMRAM3+0x38)

#define   mPWRDelay									SD_uchar_8(OEMRAM3+0x3A)

#define   pPWRRSTMark1								SD_uchar_8(OEMRAM3+0x45)
#define   pPWRRSTMark2								SD_uchar_8(OEMRAM3+0x46)
#define   pPWRRSTCnt								SD_uchar_8(OEMRAM3+0x47)
#if (PLATFORM_CHIP == AMD)
#define     pAMDPWRBTNCnt							SD_uchar_8(OEMRAM3+0x48)
#endif // PLATFORM_CHIP
#define   AdpCUTCnt									SD_uchar_8(OEMRAM3+0x49)
#define   cCADP_ID									SD_uint_16(OEMRAM3+0x4A)
#define   cCADP_I									SD_uint_16(OEMRAM3+0x4C)
#define   cCheatAD									SD_uchar_8(OEMRAM3+0x4E)
 #define  cCAD_ADPID         				BIT0
 #define  cCAD_ADPI          				BIT1
 #define  cCAD_BATPWR        				BIT2
 #define  cCAD_EN            				BIT7

#define   PECI_TIMEOUT								SD_uchar_8(OEMRAM3+0x4F)
#define   PECI_ERROR								SD_uchar_8(OEMRAM3+0x50)
#define   PECIDelayCnt								SD_uchar_8(OEMRAM3+0x51)

// #define PECIBuffer      (* (XBYTE * )( 0x0200 + 0x0E))
//#define XWORD  PECIBuffer0;        //_at_ OEMRAM2 + 0x52;
//#define XWORD  PECIBuffer1;        //_at_ OEMRAM2 + 0x54;
//#define XWORD  PECIBuffer3;        //_at_ OEMRAM2 + 0x56;
//#define XWORD  PECIBuffer2;        //_at_ OEMRAM2 + 0x58;
//#define XWORD  PECIBuffer4;        //_at_ OEMRAM2 + 0x5A;
//#define XWORD  PECIBuffer5;        //_at_ OEMRAM2 + 0x5C;
//#define XWORD  PECIBuffer6;        //_at_ OEMRAM2 + 0x5E;
//#define XWORD  PECIBuffer7;        //_at_ OEMRAM2 + 0x60;

#define   TJMAX										SD_uchar_8(OEMRAM3+0x62)	
#define   PECI_Err_CNT								SD_uchar_8(OEMRAM3+0x63)
#define   VGA_TempDly								SD_uchar_8(OEMRAM3+0x64)
#define   PECI_GET_TEMPL							SD_uchar_8(OEMRAM3+0x65)
#define   PECI_GET_TEMPH							SD_uchar_8(OEMRAM3+0x66)
#define   ERR_THMSTS								SD_uchar_8(OEMRAM3+0x67)
#define  b0ThmICError    					BIT0    // Thermal IC read temperature error.
#define  b1VGATempEr      					BIT1    // VGA read temperature error.
#define  b3PECITempEr    					BIT3    // CPU read temperature error.
#define  b4PCHThmError    					BIT4    // PCH Thermal read temperature error.

#define   TMErrCnt									SD_uchar_8(OEMRAM3+0x68)
#define   VGAErrCnt									SD_uchar_8(OEMRAM3+0x69)
#define   EXTVGAErrCnt								SD_uchar_8(OEMRAM3+0x6A)
#define   TmlICStep									SD_uchar_8(OEMRAM3+0x6B)
#define   cWriteCurrentPL1							SD_uchar_8(OEMRAM3+0x6C)
#define   cWriteCurrentPL2							SD_uchar_8(OEMRAM3+0x6E)
#define   MaxCPU_MCHTemp0							SD_uchar_8(OEMRAM3+0x70)
#define   MaxCPU_MCHTemp1							SD_uchar_8(OEMRAM3+0x71)
#define   MaxCPU_MCHTemp2							SD_uchar_8(OEMRAM3+0x72)
#define   MaxCPU_MCHTemp3							SD_uchar_8(OEMRAM3+0x73)
#define   FAN_ID									SD_uint_16(OEMRAM3+0x74)
#define   PCHErrCnt									SD_uchar_8(OEMRAM3+0x76)
#define   FAN_ID_TYPE								SD_uchar_8(OEMRAM3+0x77)
#define   nAtmFanSpeed								SD_uchar_8(OEMRAM3+0x78)   // Fan speed. For Lenovo ATM
#define   TEMP_Buff_1								SD_uchar_8(OEMRAM3+0x79)
#define   TEMP_Buff_2								SD_uchar_8(OEMRAM3+0x7A)
#define   TEMP_Buff_3								SD_uchar_8(OEMRAM3+0x7B)
#define   EXT_Duty									SD_uchar_8(OEMRAM3+0x7C)
#define   Target_Duty								SD_uchar_8(OEMRAM3+0x7D)
#define   MBID_Reload								SD_uchar_8(OEMRAM3+0x7E)
 #define  b0MBID_High     					BIT0         // MBID reload bit.
 #define  b1MBID_LOW      					BIT1         // MBID reload bit.
 
#define   Fan_ON_Count								SD_uchar_8(OEMRAM3+0x7F)
#define   Fan_Debug_Temp							SD_uchar_8(OEMRAM3+0x80)
 #define  b0CPU_Temp       					BIT0        // CPU Temperature.
 #define  b1VGA_Temp       					BIT1        // VGA Temperature.
 #define  b2EXTVGA_Temp    					BIT2        // SLI VGA Temperature.
 
#define   VGA_TBuff1								SD_uchar_8(OEMRAM3+0x81)
#define   VGA_TBuff2								SD_uchar_8(OEMRAM3+0x82)
#define   VGA_TBuff3								SD_uchar_8(OEMRAM3+0x83)
#define   Average_Temp								SD_uchar_8(OEMRAM3+0x84) // Average temperature stage
#define   USB_Charger								SD_uchar_8(OEMRAM3+0x85)
 #define  b0USB_EN        					BIT0         // USB charger bit, 1:Enable.
 #define  b1USB_initOK    					BIT1         // USB charger bit, 1:initial finish.
 #define  b2USB_TmlDis    					BIT2         // USB charger bit, 1:Disable USB charger.
 #define  b3USB_IDdeounce 					BIT3         // USB charger bit, 1:SMSC charger IC Fail.
 
#define   MuteCount									SD_uchar_8(OEMRAM3+0x86)
#define   Thro_Status								SD_uchar_8(OEMRAM3+0x87)
 #define  b0ProCH_CPU      					BIT0
 #define  b1ProCH_VGA      					BIT1
 #define  b2ProCH_EXTVGA   					BIT2
 #define  b3Turbo_CPU     					BIT3
 #define  b4Turbo_VGA      					BIT4
 #define  b5Turbo_EXTVGA   					BIT5
 #define  b6ProCH_TURBO    					BIT6
 #define  b7PMAXD2_VGA     					BIT7
 
#define   Thro_Status2								SD_uchar_8(OEMRAM3+0x88)
 #define  b0Steer_CPU      					BIT0
 #define  b1Steer_VGA      					BIT1
 #define  b2Steer_EXTVGA   					BIT2
 #define  b4FAN1_FullOn    					BIT4
 #define  b5BAT_TooLow     					BIT5        // Battery under 8.5V must turn off ALW's power.
 #define  b7AC_Again       					BIT7
 
#define   CPU_Shut_Cnt								SD_uchar_8(OEMRAM3+0x89)
#define   VGA_Shut_Cnt								SD_uchar_8(OEMRAM3+0x8A)
#define   FAN2PRM									SD_uchar_8(OEMRAM3+0x8B)
#define   FAN_CpTarget								SD_uchar_8(OEMRAM3+0x8C)
#define   ManualFan2PRM								SD_uchar_8(OEMRAM3+0x8D)
#define   SHA1_SEED									SD_uint_16(OEMRAM3+0x8E)
#define   SHA1failCnt								SD_uchar_8(OEMRAM3+0x90)
#define   Service_Auth_Step							SD_uchar_8(OEMRAM3+0x91)
#define   FANCPUSeg									SD_uchar_8(OEMRAM3+0x92)
#define   FANVGASeg									SD_uchar_8(OEMRAM3+0x93)
#define   FANPCHSeg									SD_uchar_8(OEMRAM3+0x94)
#define   TargetRPM									SD_uchar_8(OEMRAM3+0x95)

 
#define   ECSleepCount								SD_uchar_8(OEMRAM3+0x97)
#define   TouchPadCount								SD_uchar_8(OEMRAM3+0x98)
#define   EEPROM_PwrSts								SD_uchar_8(OEMRAM3+0x99)
#define   FNKB_S3Dly								SD_uchar_8(OEMRAM3+0x9A)
#define   Target_Duty_BCD							SD_uchar_8(OEMRAM3+0x9B)  // Target_duty  BCD value
#define   FAN_PWM_BCD								SD_uchar_8(OEMRAM3+0x9C)  // FAN_PWM duty BCD value
#define   TargetRPM_BCD								SD_uchar_8(OEMRAM3+0x9D) // TargetRPM    BCD value
#define   Real_RPM_BCD								SD_uchar_8(OEMRAM3+0x9E) // Real_RPM     BCD value
#define   PWM_CHG_delay								SD_uchar_8(OEMRAM3+0x9F)
#define   ManualFanPRM								SD_uchar_8(OEMRAM3+0xA0)                 
#define   uReserve07								SD_uchar_8(OEMRAM3+0xA1)                    
 #define nFanManual        					BIT6
 #define b7Fan2Manual      					BIT7
 
#define   FAN_Tab_OFF								SD_uchar_8(OEMRAM3+0xA2)                     
#define   FAN_Tab_LOW								SD_uchar_8(OEMRAM3+0xA3)
#define   FAN_Tab_HI								SD_uchar_8(OEMRAM3+0xA4)
#define   FAN_Tab_DAC								SD_uchar_8(OEMRAM3+0xA5)
#define   VFAN_Tab_HI								SD_uchar_8(OEMRAM3+0xA6)
#define   VFAN_Tab_LOW								SD_uchar_8(OEMRAM3+0xA7)		
#define   VFAN_Tab_DAC								SD_uchar_8(OEMRAM3+0xA8)
#define   THR_PRO_OFF								SD_uchar_8(OEMRAM3+0xA9)
#define   THR_PRO_ON								SD_uchar_8(OEMRAM3+0xAA)
#define   THR_Turo_Rem								SD_uchar_8(OEMRAM3+0xAB)
#define   THR_Turo_OFF								SD_uchar_8(OEMRAM3+0xAC)
#define   THR_Tab_Shut								SD_uchar_8(OEMRAM3+0xAD)
#define   LENOVOPMFW								SD_uchar_8(OEMRAM3+0xAE)   //
 #define  BATTERY_CAL_END    				BIT0        //End the calibration cycle  //09111001 Isaac Start
 #define  AUTO_BRIGHTNESS    				BIT1         // 1 = Auto Brightness on
 #define  BATTERY_MAIN_CAL   				BIT2         //calibration cycle for the main battery
 #define  BATTERY_SEC_CAL    				BIT3         //calibration cycle for the second battery
 #define  BATTERY_STORAGE    				BIT4         //battery storage mode
 #define  BATTERY_CYCLE      				BIT5         //battery Cycle mode
 #define  BATTERY_LONGEST    				BIT6         //Battery longest mode
 #define  BATTERY_GBMD       				BIT7         //Clear bit of advise user to select battery mode

#define   SWITCHABLE								SD_uchar_8(OEMRAM3+0xAF)   //                                         
 #define SWITCHABLEBTN       				BIT0          // 1: discrete VGA  0: UMA VGA
 #define HDMI                				BIT1          // 1 = HDMI exist
 #define HybridGraphics      				BIT2        // 1: Hybrid Graphics 0: External Graphic Only
 #define Switchstate         				BIT3        // 1: Switch started  0: Switch complete
 #define EVENT_NOTIFY        				BIT4         //send input event notify 1:Send 0:Don't send  //09110502 Isaac
 #define DCRFunction         				BIT5        // 1: Enable  0: Disable
 #define ColorEngine         				BIT6        // 1: Enable  0: Disable
#if NV_GC6
 #define  GC6_CLAMP          				BIT7        // 1: CLAMP ON 0: CLAMP OFF
#endif // NV_GC6
 #define nSwitchableBTNGET  				IS_MASK_SET(SWITCHABLE,SWITCHABLEBTN)
 #define nSwitchableBTNEN   				SET_MASK(SWITCHABLE,SWITCHABLEBTN)
 #define nSwitchableBTNDIS  				CLEAR_MASK(SWITCHABLE,SWITCHABLEBTN)
 #define nHybridGraphicsGET 				IS_MASK_SET(SWITCHABLE,HybridGraphics)
 #define nHybridGraphicsEN  				SET_MASK(SWITCHABLE,HybridGraphics)
 #define nHybridGraphicsDIS 				CLEAR_MASK(SWITCHABLE,HybridGraphics)
 #define nSwitchstateGET    				IS_MASK_SET(SWITCHABLE,Switchstate)
#if NV_GC6
 #define  nGC6ClampOn       				SET_MASK(SWITCHABLE,GC6_CLAMP)
 #define  nGC6ClampOff      				CLEAR_MASK(SWITCHABLE,GC6_CLAMP)
#endif // NV_GC6

#define   SMB3_DATA							SD_Ptr_uchar_8(OEMRAM3+0xB0)  // 0xB0~0xCF // size 32 bytes
#define   nRamTemp							SD_uchar_8(OEMRAM3+0xD0)   // Ram temperature
#define   nVramTemp							SD_uchar_8(OEMRAM3+0xD1)    // V Ram temperature
//#define   BATTUPDATEFW						SD_uchar_8(OEMRAM3+0xD2)
// #define  PriBattInhib       		BIT0        // bit0 1-Disable reading information from primary battery for update battery firmware
// #define  SecBattInhib       		BIT1        // bit1 1-Disable reading information from second battery for update battery firmware.

/*
0x37 (Get Battery Information )   Read Block
byte 0,1: The battery maker ID
byte 2,3: Hardware ID
byte 4,5: Firmware version (0 if the firmware is corrupted
byte 6,7: Data version (0 if this is not supported or the firmware is corrupted.
*/
#define   ISCT_Timer						SD_uchar_8(OEMRAM3+0xD3)
#define   TP_Type							SD_uchar_8(OEMRAM3+0xD4)
#if SW_ISCT
#define   AOAC_STATUS						SD_uchar_8(OEMRAM3+0xD5)
 #define  ISCT_BAT_Temp      		BIT0        // ISCT BAT temperature limitaion.
 #define  ISCT_BAT_Capy      		BIT1        // ISCT BAT capacity limitaion.
 #define  ISCT_S0_ISCT       		BIT2        // ISCT S0_ISCT status.
 #define  RESERVEA1_BIT3     		BIT3        // Reserved
 #define  ISCT_Enable        		BIT4        // ISCT support enable.
 #define  RESERVEA1_BIT5     		BIT5        // Reserved
// bit6-7 ISCT wake up status. 01:EC time out, 02:PME(LAN) wake.
#endif // SW_ISCT

#define   ACPI_HOTKEY						SD_uchar_8(OEMRAM3+0xD6)
#if Support_IFFS
 #define  b5BIOS_IFFS        		BIT5        // BIOS enter IFFS status.
#endif // Support_IFFS
 #define  b6Cmd_NoShut       		BIT6        // For command use to cut power bit.
 #define  b7BIOS_NoShut      		BIT7        // For BIOS used.
 
#define   SYS_STATUS						SD_uchar_8(OEMRAM3+0xD7)
 #define OSTYPE0          			BIT0        // OSTYPE Bit2~Bit0 0x00:XP, 0x01:VISTA, 0x02:Linux, 0x03:WIN7, 0x04:WIN8, 0x05:WIN10
 #define OSTYPE1          			BIT1        //
 #define OSTYPE2          			BIT2        //
 #define RESERVEA3_BIT3   			BIT3        // Reserved
 #define RESERVEA3_BIT4   			BIT4        // Reserved
 #define RESERVEA3_BIT5   			BIT5        // Reserved
 #define AC_ADP           			BIT7        // (AC Adapter:0=OffLine, 1=OnLine)

#define   ACOFF_SOURCE						SD_uchar_8(OEMRAM3+0xD8)
 #define CHARGECURRENT    			BIT0        // 1 = AC_OFF for when stop charge but have charge current
 #define ADPOVP           			BIT1        // 1 = AC_OFF for adaptor improper
 #define BATTLEARN        			BIT2        // 1 = AC_OFF for battery at learning mode
 #define ACMD             			BIT3        // 1 = AC_OFF for Command
 #define BATTOVP          			BIT4        // 1 = AC_OFF for battery OVP
 #define LEAKCURRENT      			BIT5        // 1 = AC_OFF for Leakage current
 #define AIRADP           			BIT6        // 1 = AC_OFF for air adaptor
 #define ACOP             			BIT7        // 1 = AC_OFF for ACOP 
 #define nChargeCurrentEN     		SET_MASK(ACOFF_SOURCE,CHARGECURRENT)
 #define nChargeCurrentDIS    		CLEAR_MASK(ACOFF_SOURCE,CHARGECURRENT)
 #define nChargeCurrentGET    		IS_MASK_SET(ACOFF_SOURCE,CHARGECURRENT)
 #define nAdpOvpEN            		SET_MASK(ACOFF_SOURCE,ADPOVP)
 #define nAdpOVPDIS           		CLEAR_MASK(ACOFF_SOURCE,ADPOVP)
 #define nBattLearnEN         		SET_MASK(ACOFF_SOURCE,BATTLEARN)
 #define nBattLearnDIS        		CLEAR_MASK(ACOFF_SOURCE,BATTLEARN)
 #define nAcOffCmdEN          		SET_MASK(ACOFF_SOURCE,ACMD)
 #define nAcOffCmdDIS         		CLEAR_MASK(ACOFF_SOURCE,ACMD)
 #define nAcOffCmdGET         		IS_MASK_SET(ACOFF_SOURCE,ACMD)
 #define nAcOffBattOvpEN      		SET_MASK(ACOFF_SOURCE,BATTOVP)
 #define nAcOffBattOvpDIS     		CLEAR_MASK(ACOFF_SOURCE,BATTOVP)
 #define nLeakCurrentEN       		SET_MASK(ACOFF_SOURCE,LEAKCURRENT)
 #define nLeakCurrentDIS      		CLEAR_MASK(ACOFF_SOURCE,LEAKCURRENT)
 #define nLeakCurrentGET      		IS_MASK_SET(ACOFF_SOURCE,LEAKCURRENT)
 #define nDis65WOVPEN         		SET_MASK(ACOFF_SOURCE,AIRADP)
 #define nDis65WOVPDIS        		CLEAR_MASK(ACOFF_SOURCE,AIRADP)
 #define nDis65WOVPGET        		IS_MASK_SET(ACOFF_SOURCE,AIRADP)
 #define nACOPEN              		SET_MASK(ACOFF_SOURCE,ACOP)
 #define nACOPDIS             		CLEAR_MASK(ACOFF_SOURCE,ACOP)

#define   CURRENT_STATUS					SD_uchar_8(OEMRAM3+0xD9)                   
 #define ENTER_S3      				BIT0        // Entry S3 State flag
 #define RESUME_S3     				BIT1        // Resume S3 State flag
 #define ENTER_S4      				BIT2        // Entry S4 State flag
 #define RESUME_S4     				BIT3        // Resume S4 State flag
 #define ENTER_S5      				BIT4        // Entry S5 State flag
 #define RESUME_S5     				BIT5        // Resume S5 State flag
 
#define   nThrottlingAPSet					SD_uchar_8(OEMRAM3+0xDA)
#define   REAL_THROTTLING_INDEX				SD_uchar_8(OEMRAM3+0xDB)
 #define REAL_SW_THRM       		BIT6        // (1=SW Throttling active)
 #define REAL_HW_THRM_PIN  			BIT7        // (1=HW Throttling Pin active)
 #define nSWThermalPinEN    		SET_MASK(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nSWThermalPinDIS   		CLEAR_MASK(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nSWThermalPinGET   		IS_MASK_SET(REAL_THROTTLING_INDEX,REAL_SW_THRM)
 #define nRealThermalPinEN  		SET_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
 #define nRealThermalPinDIS 		CLEAR_MASK(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)
 #define nRealThermalPinGET 		IS_MASK_SET(REAL_THROTTLING_INDEX,REAL_HW_THRM_PIN)

#define   THROTTLING_HW						SD_uchar_8(OEMRAM3+0xDC)    //HW throttling define     P-MAX          
#define   nCpuTempLocal						SD_uchar_8(OEMRAM3+0xDD)   // CPU local temperature 
#define   PCH_Temp3							SD_uchar_8(OEMRAM3+0xDE)   // PCH Temperature.
#define   VGA_TEMP							SD_uchar_8(OEMRAM3+0xDF)   // VGA temperature
#define   EXTVGA_TEMP						SD_uchar_8(OEMRAM3+0xE0)    // External VGA temperature.
#define   ACPI_STAT							SD_uchar_8(OEMRAM3+0xE1)
#if NV_GC6
#define GC6ENTRY         			BIT4        // Reserved [L10110801] [L11051201]
#else
#define RESERVEB7_BIT4   			BIT4        // Reserved
#endif // NV_GC6

#define   nBrightValue						SD_uchar_8(OEMRAM3+0xE2)  // LCD Brightness value.
#define   nKbProjectID						SD_uchar_8(OEMRAM3+0xE3)
#define   CPU_TYPE							SD_uchar_8(OEMRAM3+0xE4)                          
                        // CPU_TYPE : bit0~bit2 0x00:Tj85,    0x01:Tj90,   0x02:Tj100,   0x03:Tj105
                        // CPU TDP : bit3~bit5 0x01:XE(55w), 0x02:QuadCore(45w), 0x03:DualCore(35w), 0x04: UL(25w), 0x00: ULV(17w)
 #define  IGPU_EXIST     			BIT6
 #define  DGPU_EXIST     			BIT7
// VGA Type : iGPU(bit6) 1:Exist 0:Non-exist
//            dGPU(bit7) 1:Exist 0:Non-exist

//            Bit[7:6] -- 0x01:UMA, 0x10:Discrete, 0x11:SG(Optimus/PX).
 
 #define   uMBID										SD_uchar_8(OEMRAM3+0xE8)  
//BIT6 1: 12" 0:14"
 #define  IS15Inch 						IS_MASK_SET(uMBID, BIT6)       
 #define  IS14Inch 						IS_MASK_CLEAR(uMBID, BIT6) 
 
#define   BoardID									SD_uchar_8(OEMRAM3+0xE9)  
 #define  BoardID_ready    				BIT7
 #define  BoardID_readyGET  			IS_MASK_SET(BoardID,BoardID_ready)
 #define  BoardID_readyEN   			SET_MASK(BoardID,BoardID_ready)
#define   BoardADCID             SD_uint_16(OEMRAM3+0xEA)     // word  
 
#define   rACCPAGE							SD_Ptr_uchar_8(OEMRAM3+0xEC) // OEMRAM3+0xEC ~ 0xF3;	
#define   cCSTPPBATPWR						SD_uint_16(OEMRAM3+0xF4)     // word  
#define   BIOS_SID							SD_uint_16(OEMRAM3+0xF6)    // word  
#define   BIOS_SID_REC						SD_uint_16(OEMRAM3+0xF8)    // word 
#define   SMBusRBDataCount					SD_uchar_8(OEMRAM3+0xFA)
#define   I2CRBDataCount					SD_uchar_8(OEMRAM3+0xFB)
#define   LimitedSupportAdpWatt				SD_uchar_8(OEMRAM3+0xFC)
#define   NotSupportAdpWatt					SD_uchar_8(OEMRAM3+0xFD)
#define   Ccb42_States						SD_uchar_8(OEMRAM3+0xFE)
#define   ADPI_MUL							SD_uchar_8(OEMRAM3+0xFF)


//*********************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 4  
//-------------------------------------------------------------------------------
//*********************************************************************
#define   uAPSLastX								SD_uint_16(OEMRAM4+0x00)
#define   uAPSLastY								SD_uint_16(OEMRAM4+0x02)
#define   uCountX								SD_uchar_8(OEMRAM4+0x04)
#define   uCountY								SD_uchar_8(OEMRAM4+0x05)
#define   uTotalX								SD_uint_16(OEMRAM4+0x06)
#define   uTotalY								SD_uint_16(OEMRAM4+0x08)
#define   uAPSThresholdX						SD_uchar_8(OEMRAM4+0x0A)
#define   uAPSThresholdY						SD_uchar_8(OEMRAM4+0x0B)
#define   uAPSTimeOut							SD_uchar_8(OEMRAM4+0x0C)
#define   uAPStickCount							SD_uchar_8(OEMRAM4+0x0D)
#define   AD_Xdiff								SD_uchar_8(OEMRAM4+0x0E)
#define   AD_Ydiff								SD_uchar_8(OEMRAM4+0x10)

#define  BUF1_SIZE 4
#define  BUF2_SIZE 4

typedef struct _GSensor
{
  BYTE Status;     // G-sensor Status
  BYTE Buf1X_S;    // Start point for buffer 1 of X-chanel
  BYTE Buf1X_E;    // End point for buffer 1 of X-chanel
  BYTE Buf1X_N;    // Store number for buffer 1 of X-chanel
  BYTE Buf1Y_S;    // Start point for buffer 1 of Y-chanel
  BYTE Buf1Y_E;    // End point for buffer 1 of Y-chanel
  BYTE Buf1Y_N;    // Store number for buffer 1 of Y-chanel


  BYTE Buf2X_S;    // Start point for buffer 2 of X-chanel
  BYTE Buf2X_E;    // End point for buffer 2 of X-chanel
  BYTE Buf2X_N;    // Store number for buffer 2 of X-chanel
  BYTE Buf2Y_S;    // Start point for buffer 2 of Y-chanel
  BYTE Buf2Y_E;    // End point for buffer 2 of Y-chanel
  BYTE Buf2Y_N;    // Store number for buffer 2 of Y-chanel


  //BYTE DIO[0x1C];
  //In APS spec. offset=10h~23h
  // Lenovo maybe use 0xF8B0 ~ 0xF8BF some day
  WORD SampleRate;  // Sampling Frequency (Hz)
  BYTE Factor;    // Moving average factor
  WORD Buf1X[BUF1_SIZE];  // ADC for X-chanel
  WORD Buf1Y[BUF1_SIZE];  // ADC for Y-chanel

  WORD Buf2X[BUF2_SIZE];  // Calculate AD for X-chanel
  WORD Buf2Y[BUF2_SIZE];  // Calculate AD for X-chanel

} GSensor;
// G-Monitor Status
#define GS        								(*((GSensor *)(OEMRAM4+0x12)))// _at_ OEMRAM3+0x12;    //0x312 ~ 0x342
	#define  GS_Enable     			BIT0  // 1: Enabled, 0: Disabled
	#define  GS_Request    			BIT1  // 1: Last Enable request pending, 0:No enable request
	#define  GS_Process    			BIT2  // 1:Under Data Processing, 0:Wait Raw Data
	#define  GS_DiagRun    			BIT3  // 1:Diag running, 0:Diag completed
	#define  GS_Init       			BIT7  // 1:GS init completed, 0:GS no init 

#define   ADTOTALHDDCount						SD_uchar_8(OEMRAM4+0x44)
#define   APSHDDFLAG							SD_uchar_8(OEMRAM4+0x45)
 #define  APSHDDPRT_STA    			BIT0  // 1 = Start APS HDD Protection
 #define  APSHDDPRT_CHK    			BIT1  // 1 = Check APS HDD Protection
 #define  APSHDDPRT_SEC    			BIT2  // 1 = APS HDD Protection count to 1Sec OK
 #define  APSHDDPRT_MIN    			BIT3  // 1 = APS HDD Protection count to 1Min OK
 #define  b4QuickPortect   			BIT4  // 1 = Quick Protection.
 #define  b5SlowPortect    			BIT5  // 1 = Slow Protection.
 #define  b6PMEWAKE        			BIT6  // 1 = PME WAKE.
 #define  b7RTCWAKE        			BIT7  // 1 = RTC WAKE.

#define   X_Max180								SD_uint_16(OEMRAM4+0x46)	
#define   X_Min180								SD_uint_16(OEMRAM4+0x48)
#define   Y_Max180								SD_uint_16(OEMRAM4+0x4A)
#define   Y_Min180								SD_uint_16(OEMRAM4+0x4C)
#define   X_Avg									SD_uint_16(OEMRAM4+0x4E)
#define   Y_Avg									SD_uint_16(OEMRAM4+0x50)
#define   GS_VOUTX								SD_uint_16(OEMRAM4+0x52)
#define   GS_VOUTY								SD_uint_16(OEMRAM4+0x54)
#define   GSSmbusFailCnt						SD_uchar_8(OEMRAM4+0x56)  //G-sensor communication fail count
#define   GSDeviceID							SD_uchar_8(OEMRAM4+0x57)  //G-sensor Device ID LIS3DH:0x33, LIS3DSH:0x3F, Kx023_1025:0x15
#define   APSHDDCount							SD_uchar_8(OEMRAM4+0x58)
#define   rI2C_DATA								SD_uchar_8(OEMRAM4+0x59)		
#define   rI2C_DATA1							SD_Ptr_uchar_8(OEMRAM4+0x5A)		// size:5 byites
#define   cGPUThrottlingSet						SD_uchar_8(OEMRAM4+0x5F)

#define   AOU_IFG_Debounce_Cnt					SD_uchar_8(OEMRAM4+0x64)
#define  InputCurrent_Watt						SD_uchar_8(OEMRAM4+0x65)
#define  ACCutoff_Watt							SD_uchar_8(OEMRAM4+0x66)
#define  MpcCpuThrottlingPoint_L				SD_uchar_8(OEMRAM4+0x67)
#define  MpcCpuThrottlingRecoverPoint_L			SD_uchar_8(OEMRAM4+0x68)
#define  MpcGpuThrottlingPoint_L				SD_uchar_8(OEMRAM4+0x69)
#define  MpcGpuThrottlingRecoverPoint_L			SD_uchar_8(OEMRAM4+0x6A)
#define   Recovery2Sec							SD_uchar_8(OEMRAM4+0x6B)
  #define RecoveryCPU         		BIT0
  #define RecoveryGPU         		BIT2
  
#define   GPUAlarm								SD_uchar_8(OEMRAM4+0x6C)
  #define GPUAlarm_GPUThrRe   BIT0
  #define GPUAlarm_BATOCP     BIT1
  #define GPUAlarm_ADPOUT     BIT3
  #define GPUAlarm_ADPLimRe   BIT5
  #define GPUAlarm_BATOTP     BIT6
  #define GPUAlarm_TubONOFF   BIT7
#define   ACTypeCAlarm							SD_uchar_8(OEMRAM4+0x6D)
#define   cGPUThrottling						SD_uchar_8(OEMRAM4+0x6E)
#define   GPU2Sec								SD_uchar_8(OEMRAM4+0x6F)

#define   TYPE_C_RAM0							SD_uchar_8(OEMRAM4+0x70)   //for type-c used 
#define   TYPE_C_RAM1							SD_uchar_8(OEMRAM4+0x71)   //for type-c used
#define   TYPE_C_RAM2							SD_uchar_8(OEMRAM4+0x72)   //for type-c used
#define   TYPE_C_RAM3							SD_uchar_8(OEMRAM4+0x73)  //for type-c used
#define   TYPE_C_RAM4							SD_uchar_8(OEMRAM4+0x74)   //for type-c used
#define   TYPE_C_RAM5							SD_uchar_8(OEMRAM4+0x75)    //for type-c used
#define   TYPE_C_RAM6							SD_uchar_8(OEMRAM4+0x76)   //for type-c used
#define   TYPE_C_RAM7							SD_uchar_8(OEMRAM4+0x77)    //for type-c used
#define   TYPE_C_RAM8							SD_uchar_8(OEMRAM4+0x78)    //for type-c used
#define   TYPE_C_RAM9							SD_uchar_8(OEMRAM4+0x79)    //for type-c used
#define   TYPE_C_RAM10							SD_uint_16(OEMRAM4+0x7A)   //for type-c used
#define   TYPE_C_RAM11							SD_uint_16(OEMRAM4+0x7C)    //for type-c used
#define   TYPE_C_RAM12							SD_uchar_8(OEMRAM4+0x7E)    //for type-c used
#define   TYPE_C_RAM13							SD_uchar_8(OEMRAM4+0x7F)    //for type-c used
#define   TYPE_C_RAM14							SD_uchar_8(OEMRAM4+0x80)   //for type-c used
#define   TYPE_C_RAM15							SD_uchar_8(OEMRAM4+0x81)    //for type-c used
#define   TYPE_C_DOCKING_RAM0							SD_uchar_8(OEMRAM4+0x82)   //for type-c used
#define   TYPE_C_DOCKING_RAM1							SD_uchar_8(OEMRAM4+0x83)    //for type-c used
#define   TYPE_C_DOCKING_RAM2							SD_Ptr_uchar_8(OEMRAM4+0x84)    //for type-c used 0x84~0x87
#define   TYPE_C_RAM19							SD_uchar_8(OEMRAM4+0x88)    //for type-c used
#define   TYPE_C_RAM20							SD_uchar_8(OEMRAM4+0x89)    //for type-c used

#define  CHGIC_RAM0               SD_uchar_8(OEMRAM4+0x90)
#define  CHGIC_RAM1               SD_uchar_8(OEMRAM4+0x91)
#define  CHGIC_RAM2               SD_uchar_8(OEMRAM4+0x92)
#define  CHGIC_RAM3               SD_uchar_8(OEMRAM4+0x93)
#define  CHGIC_RAM4               SD_uchar_8(OEMRAM4+0x94)
#define  CHGIC_RAM5               SD_uchar_8(OEMRAM4+0x95)
#define  CHGIC_RAM6               SD_uchar_8(OEMRAM4+0x96)
#define  CHGIC_RAM7               SD_uchar_8(OEMRAM4+0x97)
#define  CHGIC_RAM8               SD_uchar_8(OEMRAM4+0x98)
#define  CHGIC_RAM9               SD_uchar_8(OEMRAM4+0x99)
#define  CHGIC_RAMA               SD_uchar_8(OEMRAM4+0x9A)
#define  CHGIC_RAMB               SD_uchar_8(OEMRAM4+0x9B)
#define  CHGIC_RAMC               SD_uchar_8(OEMRAM4+0x9C)
#define  CHGIC_RAMD               SD_uchar_8(OEMRAM4+0x9D)
#define  CHGIC_RAME               SD_uchar_8(OEMRAM4+0x9E)
#define  CHGIC_RAMF               SD_uchar_8(OEMRAM4+0x9F)
#define  CHGIC_RAM10              SD_uchar_8(OEMRAM4+0xA0)
#define  CHGIC_RAM11              SD_uchar_8(OEMRAM4+0xA1)
#define  CHGIC_RAM12              SD_uchar_8(OEMRAM4+0xA2)
#define  CHGIC_RAM13              SD_uchar_8(OEMRAM4+0xA3)
#define  CHGIC_RAM14              SD_uchar_8(OEMRAM4+0xA4)
#define  CHGIC_RAM15              SD_uchar_8(OEMRAM4+0xA5)
#define  CHGIC_RAM16              SD_uchar_8(OEMRAM4+0xA6)
#define  CHGIC_RAM17              SD_uchar_8(OEMRAM4+0xA7)
#define  CHGIC_RAM18              SD_uchar_8(OEMRAM4+0xA8)
#define  CHGIC_RAM19              SD_uchar_8(OEMRAM4+0xA9)
#define  CHGIC_RAM1A              SD_uchar_8(OEMRAM4+0xAA)
#define  CHGIC_RAM1B              SD_uchar_8(OEMRAM4+0xAB)
#define  CHGIC_RAM1C              SD_uchar_8(OEMRAM4+0xAC)
#define  CHGIC_RAM1D              SD_uchar_8(OEMRAM4+0xAD)
#define  CHGIC_RAM1E              SD_uchar_8(OEMRAM4+0xAE)
#define  CHGIC_RAM1F              SD_uchar_8(OEMRAM4+0xAF)

#define  ENHANCED_PC_BEEP_RAM0	  SD_Ptr_uchar_8(OEMRAM4+0xB0)	//0x4B0~0x4A7
#define  ENHANCED_PC_BEEP_RAM1	  SD_uchar_8(OEMRAM4+0xB8)
#define  ENHANCED_PC_BEEP_RAM2	  SD_uchar_8(OEMRAM4+0xB9)
#define  ENHANCED_PC_BEEP_RAM3	  SD_uchar_8(OEMRAM4+0xBA)
#define  ENHANCED_PC_BEEP_RAM4	  SD_uchar_8(OEMRAM4+0xBB)
#define  ENHANCED_PC_BEEP_RAM5	  SD_uchar_8(OEMRAM4+0xBC)
#define  ENHANCED_PC_BEEP_RAM6	  SD_uchar_8(OEMRAM4+0xBD)
#define  ENHANCED_PC_BEEP_RAM7	  SD_uchar_8(OEMRAM4+0xBE)
#define  ENHANCED_PC_BEEP_RAM8	  SD_uchar_8(OEMRAM4+0xBF)


#define  Tamper_ram0							SD_uint_16(OEMRAM4+0xC0)
#define  Tamper_ram1							SD_uchar_8(OEMRAM4+0xC2)
#define  Tamper_ram2							SD_uchar_8(OEMRAM4+0xC3)
#define  Tamper_ram3							SD_uint_16(OEMRAM4+0xC4)

#define	 PSYS									SD_uint_16(OEMRAM4+0xC6)  //A485D00016+

// 0x4E0~04F7 reserved for BIOS usage   for Think engine
#define  BiosReserved3E2						SD_uchar_8(OEMRAM4+0xE2)


#define  Debug_Test_3F8							SD_uchar_8(OEMRAM4+0xF8)
#define  Debug_Test_3F9							SD_uchar_8(OEMRAM4+0xF9)
#define  Debug_Test_3FA							SD_uchar_8(OEMRAM4+0xFA)
#define  Debug_Test_3FB							SD_uchar_8(OEMRAM4+0xFB)  
#define  Debug_Test_3FC							SD_uchar_8(OEMRAM4+0xFC)  
#define  Debug_Test_3FD							SD_uchar_8(OEMRAM4+0xFD)  
#define  Debug_Test_3FE							SD_uchar_8(OEMRAM4+0xFE)   //for MAX power control
#define  Debug_Test_3FF							SD_uchar_8(OEMRAM4+0xFF)   //for MAX power c debug



//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 5  
//-------------------------------------------------------------------------------
//0x00 ~ 0x7F    Main battery 
#define  M_BATTEMP            SD_uint_16(OEMRAM5+0x00)                 
#define  M_BATpollstep        SD_uchar_8(OEMRAM5+0x02)               
#define  M_BATStopCharge      SD_uchar_8(OEMRAM5+0x03)
#define  M_BATStartCharge     SD_uchar_8(OEMRAM5+0x04)
#define  M_BATSMbusFailCount  SD_uchar_8(OEMRAM5+0x05)               
#define  M_BATAlarm           SD_uchar_8(OEMRAM5+0x06)                            
 #define  BATOCP          BIT0
 #define  BATOTP          BIT1
 #define  ACSTPP          BIT2
 #define  BATPercl_5      BIT3
 #define  HybridSTPP      BIT4
 #define  DISTURBO        BIT5
 #define  ADPOUT          BIT6
#define  M_BATStopChgStatL    SD_uchar_8(OEMRAM5+0x07)
 #define  ENSTOPCHG       BIT0         // Battery fast charging timeout
 #define  ENB0B3STOP      BIT1         // Battery name space B0/B3 start stop charge enable                           
 #define  ENCOMMFAIL      BIT2
 #define  ENCHARGESUSP    BIT3         // use for detect battery charging suspend
 #define  ENCMDSTOPCHG    BIT4         // Battery command stop charge
 #define  ENCUSTOMER      BIT5         // Battery stop charge for Customer use
 #define  ENOVERVOLTAGE   BIT6
 #define  ENOVERTEMP      BIT7         // Battery over temperature
#define  M_BATStopChgStatH    SD_uchar_8(OEMRAM5+0x08)
 #define  ENTRITIMEOUT      BIT0        // Battery trickle charging timeout
 #define  ENADPTHROTTLING   BIT1        // Stop charge of Adapter throttling
 #define  PMTHRESHOLD       BIT2        // Stop Charge Threshold of PM AP
 #define  HybridModeStopChg BIT3
 #define  ECFLASH           BIT4        // Stop charge of flash
 #define  NotLenovoBattery  BIT5        // Stop charge if it is not Lenovo battery
 #define  EmStopChgarg      BIT6        // Battery health function used.Flag will set high for stop chgarg when battery chgarg full.
 #define  ENBOOSTSTOPCHG    BIT7
#define  M_BATFlag1           SD_uchar_8(OEMRAM5+0x09)
 #define  pEdgeProjectBATFWUP      BIT0
 #define  pEdgeProjectBATIn        BIT1
 #define  pEdgeProjectBATChange    BIT2
 #define  pEdgeProjectBATWake      BIT3
 #define  pEdgeBATTOver20          BIT4
 #define  pEdgeBATTUnder20         BIT5
 #define  M_BATWake   							IS_MASK_SET(M_BATFlag1,pEdgeProjectBATWake)
#define  M_BATErrorCnt              SD_uchar_8(OEMRAM5+0x0A)  // (XBYTE) Communicatiion fail counter
#define  M_BATdata_ready            SD_uchar_8(OEMRAM5+0x0B) 
#define  M_BATin_delay              SD_uchar_8(OEMRAM5+0x0C)
#define  M_BATOvrTempCnt            SD_uchar_8(OEMRAM5+0x0D)  
#define  M_BATChk_FCCchg_count      SD_uchar_8(OEMRAM5+0x0E)  
#define  M_BATChk_FCCchg_lastFCCL   SD_uchar_8(OEMRAM5+0x0F)  
#define  M_BATChk_FCCchg_lastFCCH   SD_uchar_8(OEMRAM5+0x10)  
typedef enum
{
  ChkBattery_abnormal_status_normal,
  ChkBattery_abnormal_status_wait5sec,
  ChkBattery_abnormal_status_error
}ChkBattery_abnormal_status_type;
#define  M_BATChkBattery_abnormal_status	SD_uchar_8(OEMRAM5+0x11)		                  
#define  M_BATChkBattery_abnormal_count		SD_uchar_8(OEMRAM5+0x12) 
#define  M_BAT_Status1                    SD_uchar_8(OEMRAM5+0x13) 
 #define  M_Trickle_T           BIT0          //Main battery in tricle mode now
 #define  M_Fast_T              BIT1          //Main battery in Fast mode now
 #define  M_Trickle2Fast        BIT2          //Main battery from trickle to fast charge 
 #define  IS_M_Trickle                   IS_MASK_SET(M_BAT_Status1,M_Trickle_T)
 #define  M_Trickle2FastGet              IS_MASK_SET(M_BAT_Status1,M_Trickle2Fast)

#define	M_BATTrickleChgTimeOutCnt				SD_uint_16(OEMRAM5+0x14)                    
#define	M_BATFastChgTimeOutCnt					  SD_uint_16(OEMRAM5+0x16)   
#define  M_BAT_Trickle_switch_time        SD_uchar_8(OEMRAM5+0x18) 
#define  M_BAT_Trickle_switch_Voltage     SD_uchar_8(OEMRAM5+0x19)
#define	M_BatLowVoltage					  SD_uint_16(OEMRAM5+0x1A)
	#define M_BatLowVoltage_LB					SD_uchar_8(OEMRAM5+0x1A)  //A485D00017+
	#define M_BatLowVoltage_HB					SD_uchar_8(OEMRAM5+0x1B)  //A485D00017+
#define  M_BAT_OCP_Current   						  SD_uint_16(OEMRAM5+0x1C)	//For battery OCP protect
#define  M_BAT_Max_Discharge_Watt 		    SD_uchar_8(OEMRAM5+0x1E)	//For battery OCP protect



#if Lenovo_Authentication
#define   M_BAT_uAuthenRand1								SD_uchar_8(OEMRAM5+0x30)
#define   M_BAT_uAuthenRand2								SD_uchar_8(OEMRAM5+0x31)
#define   M_BAT_uAuthenRand3								SD_uchar_8(OEMRAM5+0x32)
#define   M_BAT_uAuthenRand4								SD_uchar_8(OEMRAM5+0x33)
#define   M_BAT_uAuthenRPCL								SD_uchar_8(OEMRAM5+0x34)
#define   M_BAT_uAuthenRPCH								SD_uchar_8(OEMRAM5+0x35)
#define   M_BAT_uAuthenRPackL							SD_uchar_8(OEMRAM5+0x36)
#define   M_BAT_uAuthenRPackH							SD_uchar_8(OEMRAM5+0x37)
#define   M_BAT_uAuthenNPackL							SD_uchar_8(OEMRAM5+0x38)
#define   M_BAT_uAuthenNPackH   						SD_uchar_8(OEMRAM5+0x39)

#define   M_BAT_uAuthenNPC								  SD_uint_16(OEMRAM5+0x3A)
#define   M_BAT_uAuthenFlag								SD_uchar_8(OEMRAM5+0x3C)
 #define  uDF_Write       				BIT0         // EC write InQueue OK
 #define  uDF_Read        				BIT1         // EC read InQueue OK
 #define  uDF_Return      				BIT2         // Battery return calculate result to EC is OK
 #define  uDF_ReadFail    				BIT3         // Battery return calculate result to EC is Fail (Communication failed)
 #define  uDF_ErrorReturn 				BIT4
 #define  uDF_Done        				BIT5
 #define  uDF_HWstart     				BIT6
 #define  uDF_SWstart     				BIT7
#define   M_BAT_uAuthenReadAfter1Sec				SD_uchar_8(OEMRAM5+0x3D) // EC write Challenge to battery and read after 1 sec
#define   M_BAT_uAuthenIdx3rd           		SD_uint_16(OEMRAM5+0x3E)
#define   M_BAT_uAuthenNPack3rd							SD_uint_16(OEMRAM5+0x40)
#define   M_BAT_uAuthenIdx3rdtemp						SD_uint_16(OEMRAM5+0x42)
#define   M_BAT_uAuthenIdx3rdtemp2					SD_ulong_32(OEMRAM5+0x44)
#define   M_BAT_uAuthenNPC3rd							SD_uint_16(OEMRAM5+0x48)
#define   M_BAT_uAuthenFailCnt							SD_uchar_8(OEMRAM5+0x4A)
#define   M_BAT_uAuthenFlag3rd							SD_uchar_8(OEMRAM5+0x4B)
 #define  uDF_Write        			BIT0        // EC write InQueue OK
 #define  uDF_Read         			BIT1        // EC read InQueue OK
 #define  uDF_Return       			BIT2        // Battery return calculate result to EC is OK
 #define  uDF_ReadFail     			BIT3        // Battery return calculate result to EC is Fail (Communication failed)
 #define  uDF_ErrorReturn  			BIT4
 #define  uDF_Done         			BIT5
 #define  uDF_3rdStart     			BIT6
 #define  uDF_bit7         			BIT7
#define   M_BAT_uAuthenFailCnt3rd						SD_uchar_8(OEMRAM5+0x4C)
#define   M_BAT_edgeProject4								SD_uchar_8(OEMRAM5+0x4D)
 #define uHWAuthTruePass    			BIT0
 #define uECAuthTruePass    			BIT1
 #define uAuthComplete    				BIT2
 #define uLENOVO_Error    				BIT3
 #define uLenovo_JapanError  		BIT4
 #define uAuthSWstatus    				BIT5
 #define uSWDone        				  BIT6
 #define usLenovo_Error    			BIT7

#define   M_BAT_LV_SW_SIGNERTURE						SD_Ptr_uchar_8(OEMRAM5+0x4E)   //0x54E~0x55D SIZE:16 BYTES
#define   M_BAT_p2AuthFailCnt							SD_uchar_8(OEMRAM5+0x5E)
#define   M_BAT_pAuthCommFailCnt						SD_uchar_8(OEMRAM5+0x5F)
#endif
#define   M_BAT_pFKTemp									  SD_uchar_8(OEMRAM5+0x60)
#define   M_BAT_Batt0x3CFailRetry5sTimer		SD_uchar_8(OEMRAM5+0x61)
// Batt0x3CFailRetry5sTimer: Timer counter 
// Unit: second.
// Range: 0x01 ~ 0xFE (1 ~ 254) sec. 0xFF = disable.




//0x80 ~ 0xFF    Second battery 
#define  S_BATTEMP                  SD_uint_16(OEMRAM5+0x80) 
#define  S_BATpollstep              SD_uchar_8(OEMRAM5+0x82) 
#define  S_BATStopCharge 		       SD_uchar_8(OEMRAM5+0x83)
#define  S_BATStartCharge 	         SD_uchar_8(OEMRAM5+0x84)
#define  S_BATSMbusFailCount        SD_uchar_8(OEMRAM5+0x85)	
#define  S_BATAlarm                 SD_uchar_8(OEMRAM5+0x86) 
#define  S_BATStopChgStatL          SD_uchar_8(OEMRAM5+0x87)
#define  S_BATStopChgStatH          SD_uchar_8(OEMRAM5+0x88)
#define  S_BATFlag1 			         SD_uchar_8(OEMRAM5+0x89)
 #define  pEdgeProject2ndBATFWUP      BIT0
 #define  pEdgeProject2ndBATIn        BIT1
 #define  pEdgeProject2ndBATChange    BIT2
 #define  pEdgeProject2ndBATWake      BIT3
 #define  pEdge2ndBATTOver20          BIT4
 #define  pEdge2ndBATTUnder20         BIT5
 #define  S_BATWake   							IS_MASK_SET(S_BATFlag1,pEdgeProject2ndBATWake) 
#define  S_BATErrorCnt              SD_uchar_8(OEMRAM5+0x8A)  // (XBYTE) Communicatiion fail counter
#define  S_BATdata_ready            SD_uchar_8(OEMRAM5+0x8B)
#define  S_BATin_delay              SD_uchar_8(OEMRAM5+0x8C)
#define  S_BATOvrTempCnt            SD_uchar_8(OEMRAM5+0x8D)  
#define  S_BATChk_FCCchg_count      SD_uchar_8(OEMRAM5+0x8E)  
#define  S_BATChk_FCCchg_lastFCCL   SD_uchar_8(OEMRAM5+0x8F)  
#define  S_BATChk_FCCchg_lastFCCH   SD_uchar_8(OEMRAM5+0x90)
#define  S_BATChkBattery_abnormal_status	SD_uchar_8(OEMRAM5+0x91)		                  
#define  S_BATChkBattery_abnormal_count		SD_uchar_8(OEMRAM5+0x92)
#define  S_BAT_Status1                    SD_uchar_8(OEMRAM5+0x93) 
 #define  IS_S_Trickle                    IS_MASK_SET(S_BAT_Status1,M_Trickle_T)
 #define  S_Trickle2FastGet               IS_MASK_SET(S_BAT_Status1,M_Trickle2Fast) 
#define	S_BATTrickleChgTimeOutCnt				SD_uint_16(OEMRAM5+0x94)                    
#define	S_BATFastChgTimeOutCnt					  SD_uint_16(OEMRAM5+0x96)   
#define  S_BAT_Trickle_switch_time        SD_uchar_8(OEMRAM5+0x98) 
#define  S_BAT_Trickle_switch_Voltage     SD_uchar_8(OEMRAM5+0x99)
#define	S_BatLowVoltage					  SD_uint_16(OEMRAM5+0x9A)
#define  S_BAT_OCP_Current   						  SD_uint_16(OEMRAM5+0x9C)	//For battery OCP protect
#define  S_BAT_Max_Discharge_Watt 		    SD_uchar_8(OEMRAM5+0x9E)	//For battery OCP protect



#if Lenovo_Authentication
#define   S_BAT_uAuthenRand1								SD_uchar_8(OEMRAM5+0xB0)
#define   S_BAT_uAuthenRand2								SD_uchar_8(OEMRAM5+0xB1)
#define   S_BAT_uAuthenRand3								SD_uchar_8(OEMRAM5+0xB2)
#define   S_BAT_uAuthenRand4								SD_uchar_8(OEMRAM5+0xB3)
#define   S_BAT_uAuthenRPCL								SD_uchar_8(OEMRAM5+0xB4)
#define   S_BAT_uAuthenRPCH								SD_uchar_8(OEMRAM5+0xB5)
#define   S_BAT_uAuthenRPackL							SD_uchar_8(OEMRAM5+0xB6)
#define   S_BAT_uAuthenRPackH							SD_uchar_8(OEMRAM5+0xB7)
#define   S_BAT_uAuthenNPackL							SD_uchar_8(OEMRAM5+0xB8)
#define   S_BAT_uAuthenNPackH							SD_uchar_8(OEMRAM5+0xB9)
#define   S_BAT_uAuthenNPC									SD_uint_16(OEMRAM5+0xBA)
#define   S_BAT_uAuthenFlag								SD_uchar_8(OEMRAM5+0xBC)
#define   S_BAT_uAuthenReadAfter1Sec				SD_uchar_8(OEMRAM5+0xBD) // EC write Challenge to battery and read after 1 sec
#define   S_BAT_uAuthenIdx3rd           		SD_uint_16(OEMRAM5+0xBE)
#define   S_BAT_uAuthenNPack3rd							SD_uint_16(OEMRAM5+0xC0)
#define   S_BAT_uAuthenIdx3rdtemp						SD_uint_16(OEMRAM5+0xC2)
#define   S_BAT_uAuthenIdx3rdtemp2					SD_ulong_32(OEMRAM5+0xC4)
#define   S_BAT_uAuthenNPC3rd							SD_uint_16(OEMRAM5+0xC8)
#define   S_BAT_uAuthenFailCnt							SD_uchar_8(OEMRAM5+0xCA)
#define   S_BAT_uAuthenFlag3rd							SD_uchar_8(OEMRAM5+0xCB)
#define   S_BAT_uAuthenFailCnt3rd						SD_uchar_8(OEMRAM5+0xCC)
#define   S_BAT_edgeProject4								SD_uchar_8(OEMRAM5+0xCD)
#define   S_BAT_LV_SW_SIGNERTURE						SD_Ptr_uchar_8(OEMRAM5+0xCE)   //0x5CE~0x5DD SIZE:16 BYTES
#define   S_BAT_p2AuthFailCnt							SD_uchar_8(OEMRAM5+0xDE)
#define   S_BAT_pAuthCommFailCnt						SD_uchar_8(OEMRAM5+0xDF)
#endif 
#define   S_BAT_pFKTemp										SD_uchar_8(OEMRAM5+0xE0)
#define   S_BAT_Batt0x3CFailRetry5sTimer		SD_uchar_8(OEMRAM5+0xE1)

#define  BAT_Average_curr_CNT                			SD_uchar_8(OEMRAM5+0xE2)
#define  BAT_Average_curr_sum                			SD_ulong_32(OEMRAM5+0xE4)
//A485D00027:Enable Battery CAP trip function.
#define	  BAT_CAPTripFlag								SD_uchar_8(OEMRAM5+0xE8)
	#define MBAT_triggered	BIT0
	#define SBAT_triggered	BIT4
//A485D00027->end
#define  BATUPDATEFW                SD_uchar_8(OEMRAM5+0xF0)
 #define  PriBattInhib       BIT0        // bit0 1-Disable reading information from primary battery for update battery firmware
 #define  SecBattInhib       BIT1        // bit1 1-Disable reading information from second battery for update battery firmware.
 #define  PriBattExistTemp   BIT2
 #define  SecBattExistTemp   BIT3
#define  nBatteryStatH              SD_uchar_8(OEMRAM5+0xF1)
 #define  PreBat_DISCHARGE    BIT0        // Discharging  1st battery
 #define  SecBat_DISCHARGE    BIT1        // Discharging  2nd battery
 #define  PreBat_CHARGE       BIT2        // Charging     1st battery
 #define  SecBat_CHARGE       BIT3        // Charging     2nd battery


#define  nBattAverTemp               SD_uchar_8(OEMRAM5+0xF2)  // (XBYTE) Battery Average Temp.
#define  nBattTempCnt                SD_uchar_8(OEMRAM5+0xF3)  // (XBYTE) Battery current Temp sample counter
#define  StatusKeeper							 SD_uchar_8(OEMRAM5+0xF4)
 #define  BatteryFwUpdate     BIT0
 #define  SecBatteryFwUpdate  BIT1
#define  LOWBATT_3TIMES					  SD_uchar_8(OEMRAM5+0xF5)          
#define  BAT_Status                SD_uchar_8(OEMRAM5+0xF6) 
 #define  BAT_switch_state_now    (BIT0 + BIT1 + BIT2)
 #define  BAT_switch_ready        BIT3
 #define  BAT_switch_state_target (BIT4 + BIT5 + BIT6) 
 #define  BAT_Charger_sts     BIT7     //0:discharge , 1:charge
 #define  BAT_switch_ready_SET     SET_MASK(BAT_Status,BAT_switch_ready)
 #define  BAT_switch_ready_CLR     CLEAR_MASK(BAT_Status,BAT_switch_ready) 
 #define  IS_BAT_switch_ready      IS_MASK_SET(BAT_Status,BAT_switch_ready)
 #define  BAT_switch_not_ready     IS_MASK_CLEAR(BAT_Status,BAT_switch_ready)

#define  BAT_Eject_Cnt            SD_uint_16(OEMRAM5+0xF8) 
 
#define  BAT_switch_delay_timer    SD_uchar_8(OEMRAM5+0xFA) 
#define  BAT_Trickle_Charge_step   SD_uchar_8(OEMRAM5+0xFB)
#define  BAT_Trickle_ACIN_timer    SD_uint_16(OEMRAM5+0xFC)
#define  BAT_Unship_mode           SD_uchar_8(OEMRAM5+0xFE)
 #define  M_unship_mode        BIT0
 #define  S_unship_mode        BIT1
#define  BAT_Unship_CNT           SD_uchar_8(OEMRAM5+0xFF)

#if Support_COMPAL_MFG_TOOL
#define   Step0_L									SD_uchar_8(OEMRAM5+0x03)
#define   Step0_H									SD_uchar_8(OEMRAM5+0x04)
#define   Step0_DAC									SD_uchar_8(OEMRAM5+0x05)
#define   Step1_L									SD_uchar_8(OEMRAM5+0x06)
#define   Step1_H									SD_uchar_8(OEMRAM5+0x07)
#define   Step1_DAC									SD_uchar_8(OEMRAM5+0x08)
#define   Step2_L									SD_uchar_8(OEMRAM5+0x09)
#define   Step2_H									SD_uchar_8(OEMRAM5+0x0A)
#define   Step2_DAC									SD_uchar_8(OEMRAM5+0x0B)
#define   Step3_L									SD_uchar_8(OEMRAM5+0x0C)
#define   Step3_H									SD_uchar_8(OEMRAM5+0x0D)
#define   Step3_DAC									SD_uchar_8(OEMRAM5+0x0E)
#define   Step4_L									SD_uchar_8(OEMRAM5+0x0F)
#define   Step4_H									SD_uchar_8(OEMRAM5+0x10)
#define   Step4_DAC									SD_uchar_8(OEMRAM5+0x11)                        
#endif
#if Support_COMPAL_MFG_TOOL
#define   EXT_NAMESPACE_INDEX						SD_uchar_8(OEMRAM5+0x12) // 0x5D
#define   EXT_NAMESPACE_BANK						SD_uchar_8(OEMRAM5+0x13)  // 0x5E
#define   EXT_NAMESPACE_DATA						SD_uchar_8(OEMRAM5+0x14) // 0x5F
#endif
#if Support_COMPAL_MFG_TOOL
#define  MMIO_rDATA_index							SD_uchar_8(OEMRAM5+0x15) 
#define  MMIO_CMD_Ctrl								SD_uchar_8(OEMRAM5+0x16) 
 #define MMIO_Processing_flage  		BIT0
 #define MMIO_Start_flage       		BIT1
 
#define  MMIO_rDATA									SD_Ptr_uchar_8(OEMRAM5+0x17) // 0x17~ox1E
#define  MMIO_iCMD									SD_uchar_8(OEMRAM5+0x1F)
#define  MMIO_iDATA8C								SD_uchar_8(OEMRAM5+0x20)
#define  MMIO_iDATA8D								SD_uchar_8(OEMRAM5+0x21) 
#define  MMIO_iDATA8E								SD_uchar_8(OEMRAM5+0x22)
#define  MMIO_iDATA8F								SD_uchar_8(OEMRAM5+0x23)
#define  MMIO_iDATA90								SD_uchar_8(OEMRAM5+0x24) 
#endif


//0x580 ~ 0x5FF reserved for project usage, please reference to LCFC OEM_HOOK as well


//*****************************************************************
//------------------------------------------------------------------------
//  OEM RAM 6 
//-------------------------------------------------------------------------
//****************************************************************
#define  KB_RAM_Table							SD_Ptr_uchar_8(OEMRAM6+0x00)  // 0x00~0x7F size 128
#define  ExKB_RAM_Table							SD_Ptr_uchar_8(OEMRAM6+0x80)  // 0x80~0x98 size 24

//0x6A0 ~ 0x6FF reserved for project usage


//-------------------------------------------------------------------------------
//  OEM RAM 7
//-------------------------------------------------------------------------------
#define  PWDSIZE  8
#define  PWDNUM  16

#define   edgePassword								SD_Ptr_uchar_8(OEMRAM7+0x00) //_at_ OEMRAM9 + 0x00;  // 0x0900 ~ 0x097F
#define   edgePasswordID							SD_uchar_8(OEMRAM7+0x80)
 #define  ePWDIDRead          			BIT6
 #define  ePWDIDSet           			BIT7


#if Thinkpad_Battery_HW_Auth
#define   LV_HW_BatteryReserved6					SD_uint_16(OEMRAM7+0xBC) // 0x03BC ~ 0x03BD
#define   LV_HW_BatteryResponseX					SD_Ptr_uint_16(OEMRAM7+0xBE) // 0x03BE ~ 0x03C5
#define   LV_HW_BatteryResponseZ					SD_Ptr_uint_16(OEMRAM7+0xC6) // 0x03C6 ~ 0x03D7
#define   LV_HW_BatteryControl						SD_uchar_8(OEMRAM7+0xD8)
 #define  LV_HW_BatteryAuthPassed    	BIT0
 #define  LV_HW_BatteryAuthFinished 	BIT2
 #define  LV_HW_BatteryDataReady     	BIT7
 
#define   LV_HW_BatteryChallengeKey					SD_Ptr_uchar_8(OEMRAM7+0xD9) // 0x03D9 ~ 0x03EA
#define   LV_HW_BatteryCheckValue					SD_Ptr_uint_16(OEMRAM7+0xEC) // 0x03EC ~ 0x03FD
#endif



//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 8  
//-------------------------------------------------------------------------------
//*******************************************************************************

#if Thinkpad_Battery_HW_Auth
#define   LV_HW_BatteryReserved1					SD_uchar_8(OEMRAM8+0x01)//_at_ ECRAMC+0x01;
#define   gf2n_t_a             						SD_Ptr_uint_16(OEMRAM8+0x02)//_at_ ECRAMC+0x02; // 0x0C02 ~ 0x0C13
#define   LV_HW_BatteryReserved2					SD_uint_16(OEMRAM8+0x14) // 0x0C14 ~ 0x0C15
#define   gf2n_temp									SD_Ptr_uint_16(OEMRAM8+0x16)//_at_ ECRAMC+0x16; // 0x0C16 ~ 0x0C37
#define   LV_HW_BatteryReserved3					SD_uint_16(OEMRAM8+0x38) //_at_ ECRAMC+0x38; // 0x0C38 ~ 0x0C39
#define   LV_HW_BatteryReserved4					SD_uint_16(OEMRAM8+0x4C) //_at_ ECRAMC+0x4C; // 0x0C4C ~ 0x0C4D
#define   LV_HW_BatteryReserved5					SD_uint_16(OEMRAM8+0x4E)//_at_ ECRAMC+0x4E; // 0x0C4E ~ 0x0C4F
#endif  // Thinkpad_Battery_HW_Auth


//Dash
#define  Dash_RAM0_8              SD_uchar_8(OEMRAM8+0x90)
//reserved  0x899~0x89F
#define  Dash_RAM0_48             SD_Ptr_uchar_8(OEMRAM8+0xA0)     //0x8A0~0xCF  //48 byte buffer
#define  Dash_RAM1_48             SD_Ptr_uchar_8(OEMRAM8+0xD0)     //0x8D0~0xFF  //48 byte buffer

//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM 9  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define RSTStatus1 									SD_uchar_8(OEMRAM9+0x00)	
#define DB_SMBusAddr								SD_uchar_8(OEMRAM9+0x01)
#define DB_SMBusCmd									SD_uchar_8(OEMRAM9+0x02)
#define DB_SMBusBCunt								SD_uchar_8(OEMRAM9+0x03)
#define DB_SMBusFlag1								SD_uchar_8(OEMRAM9+0x04)
#define DB_SMBusFlag2								SD_uchar_8(OEMRAM9+0x05)
#define DB_SMBusFlag3								SD_uchar_8(OEMRAM9+0x06)
	#define DB_S	                       	BIT0
	#define DB_R	                        BIT1
	#define DB_RB	                        BIT2
	#define DB_WB	                        BIT3
	#define DB_RW	                        BIT4
	#define DB_WW	                        BIT5
	#define DB_RBB	                        BIT6
	#define DB_WBB	                        BIT7

#define	DB_SMBusData                				SD_Ptr_uchar_8(OEMRAM9+0x07)    // array offset 0x5C ~ 0x26
#define DB_SMBusAck1								SD_uchar_8(OEMRAM9+0x27)

//====================================================================
// Offset 0x28 ~ 0x47   SMBus center variables
//====================================================================
#define	SMBus1Flag1									SD_uchar_8(OEMRAM9+0x28)
	#define B1_RSOC         				BIT0
	#define B1_C            				BIT1
	#define B1_V            				BIT2
	#define B1_RMcap        				BIT3
	#define B1_Status       				BIT4
	#define B1_FCC         	 				BIT5
	#define B1_Temp         				BIT6
	
#define	SMBus1Flag2									SD_uchar_8(OEMRAM9+0x29)
	#define B1_CC           				BIT0
	#define B1_CV           				BIT1
	#define B1_AVC							BIT2
	#define B1_AVTimeFull					BIT3
	#define B1_AVTimeEmpty					BIT4	
	#define ISL_INT_HT_LSB					BIT5
	#define ISL_INT_HT_MSB					BIT6
	#define B1_Cycle						BIT7

#define	SMBus1Flag3									SD_uchar_8(OEMRAM9+0x2A)
	#define Charging_CC						BIT0
	#define Charging_CV						BIT1
	#define Charging_CM						BIT2
	#define Charging_CL						BIT3
	#define Charging_OP						BIT4
	#define Charging_TS						BIT5
	#define Charging_IP						BIT6
	
#define	SMBus1Flag4									SD_uchar_8(OEMRAM9+0x2B)
#define	SMBus1Flag5									SD_uchar_8(OEMRAM9+0x2C)
	#define PollCPUThermal     				BIT0            								// Read thermal sensor remote temperature
	#define PollFanThermal       			BIT1          							  		// Read thermal sensor local temperature
	#define PollDIM0Thermal       			BIT2           									// Read thermal sensor local temperature
	#define PollPWRThermal       			BIT3           									// Read thermal sensor local temperature
	#define PollPCHThermal       			BIT4           									// Read thermal sensor local temperature
	#define PollKTFByte						BIT5											// Read KTF Byte
	
#define	SMBus2Flag1									SD_uchar_8(OEMRAM9+0x2D)
	#define PollRemote      				BIT1            								// Read thermal sensor remote temperature
	#define PollLocal       				BIT2           									// Read thermal sensor local temperature

#define	SMBus2Flag2									SD_uchar_8(OEMRAM9+0x2E)
#define	SMBus2Flag3									SD_uchar_8(OEMRAM9+0x2F)
#define	SMBus2Flag4									SD_uchar_8(OEMRAM9+0x30)
#define	SMBus2Flag5									SD_uchar_8(OEMRAM9+0x31)
#define	SMBus3Flag1									SD_uchar_8(OEMRAM9+0x32)
#define	SMBusSelection	   	 						SD_uchar_8(OEMRAM9+0x33)
#define	F_Service_SMBus1    						SD_uchar_8(OEMRAM9+0x34)
#define	F_Service_SMBus2    						SD_uchar_8(OEMRAM9+0x35)
#define	F_Service_SMBus3    						SD_uchar_8(OEMRAM9+0x36)
#define	F_Service_SMBus4     						SD_uchar_8(OEMRAM9+0x37)
#define	SMBus1ByteCunt	    						SD_uchar_8(OEMRAM9+0x38)
#define	SMBus2ByteCunt								SD_uchar_8(OEMRAM9+0x39)
#define	SMBus3ByteCunt								SD_uchar_8(OEMRAM9+0x3A)
#define	SMBus1counter								SD_uchar_8(OEMRAM9+0x3B)
#define	SMBus2counter								SD_uchar_8(OEMRAM9+0x3C)
#define	SMBus3counter								SD_uchar_8(OEMRAM9+0x3D)
#define	SMBus1index									SD_uchar_8(OEMRAM9+0x3E)
#define	SMBus2index									SD_uchar_8(OEMRAM9+0x3F)
#define	SMBus3index									SD_uchar_8(OEMRAM9+0x40)
#define SMBus1InUsing       						SD_uchar_8(OEMRAM9+0x41)
#define SMBus2InUsing       						SD_uchar_8(OEMRAM9+0x42)
#define SMBus3InUsing       						SD_uchar_8(OEMRAM9+0x43)
#define SMBus1TableSize     						SD_uchar_8(OEMRAM9+0x44)
#define SMBus2TableSize     						SD_uchar_8(OEMRAM9+0x45)
#define SMBus3TableSize     						SD_uchar_8(OEMRAM9+0x46)
#define SMBusXStatus        						SD_uchar_8(OEMRAM9+0x47)
	#define 	SMBus1_OK      			BIT0
	#define 	SMBus1_Fail     		BIT1
	#define 	SMBus2_OK       		BIT2
	#define 	SMBus2_Fail     		BIT3
	#define 	SMBus3_OK       		BIT4
	#define 	SMBus3_Fail     		BIT5
	#define 	SMBus4_OK       		BIT6
	#define 	SMBus4_Fail     		BIT7

#define ITempW01									SD_uint_16(OEMRAM9+0x48)
#define EventId										SD_uchar_8(OEMRAM9+0x4A)
#define PECI_CPU_temp      							SD_uchar_8(OEMRAM9+0x4B)   		// Use PECI interface to read CPU temperature
#define	CPUTjmax			    					SD_uchar_8(OEMRAM9+0x4C)
#define	ReadCPUTjmaxCUNT        					SD_uchar_8(OEMRAM9+0x4D)

#define PECIReadBuffer  							SD_Ptr_uchar_8(OEMRAM9+0x50)	// PECIReadBuffer[16]  0x950~0x95F
#define adcDyCounter								SD_uchar_8(OEMRAM9+0x61)
#define adcUniCounter								SD_uchar_8(OEMRAM9+0x62)

#define checkCardStatus								SD_uchar_8(OEMRAM9+0x63)
	#define idCard_0			BIT0
	#define idCard_1			BIT1	// idCard_0&idCard_1 1:checkpoint display 2:175 error key 3:175 error keywith check point
	#define checkCardExist		BIT7

#define tmpACPISMBBCnt								SD_uchar_8(OEMRAM9+0x64)



//-------------------------------------------------------------------------
// ITE Debug Variable 0xF0~0xFF
//-------------------------------------------------------------------------
#define   bITEDebug									SD_uchar_8(OEMRAM9+0xF0)
#define   bITETestVar								SD_uint_16(OEMRAM9+0xF2)
#define   bITETestVar2								SD_uint_16(OEMRAM9+0xF4)
#define   bITETestVar3								SD_uchar_8(OEMRAM9+0xF6)
#define   bITETestAddr								SD_Ptr_uchar_8(OEMRAM9+0xF8)
#define   bITETestAddr2								SD_Ptr_uchar_8(OEMRAM9+0xFC)
#define   bITETestVar4								SD_uchar_8(OEMRAM9+0xFE)
#define   bITETestVar5								SD_uchar_8(OEMRAM9+0xFF)




//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM A  
//-------------------------------------------------------------------------------
//*******************************************************************************

/***********   TPG Customer RAM   ****************/

#define   u4Cfgspace0								SD_uchar_8(OEMRAMA+0x00)
 #define  GlobalAtt      			BIT1  
 #define  FnFunc         			BIT2
 #define  FnSticky       			BIT3
 #define  Thermal_monitor      	BIT7 
 #define  GlobalAttGET   							IS_MASK_SET(u4Cfgspace0,GlobalAtt)  
 #define  FnFuncGET      							IS_MASK_SET(u4Cfgspace0,FnFunc)
 #define  FnFuncSET      							SET_MASK(u4Cfgspace0,FnFunc)
 #define  FnFuncCLR      							CLEAR_MASK(u4Cfgspace0,FnFunc)
 #define  FnStickyGET    							IS_MASK_SET(u4Cfgspace0,FnSticky)
 #define  Thermal_monitor_En 					SET_MASK(u4Cfgspace0,Thermal_monitor) 
 #define  Thermal_monitor_Get 					IS_MASK_SET(u4Cfgspace0,Thermal_monitor)  

#define   u4Cfgspace1								SD_uchar_8(OEMRAMA+0x01)  
 #define  LIDBKSYNC       			BIT0
 #define  CS1_POWER_ON_BY_AC		BIT1
 #define  CS1_DISABLE_KBD_LIGHT		BIT2
 #define  CS1_DISABLE_KBD_BACK_LIGHT        			BIT3
 #define  PBTNWARN        			BIT4
 #define  PalBKLidSyncGET 			IS_MASK_SET(u4Cfgspace1,LIDBKSYNC)
 #define  PowerOnByAcGET 				IS_MASK_SET(u4Cfgspace1,CS1_POWER_ON_BY_AC)
 #define  DisableKbdLightGET 				IS_MASK_SET(u4Cfgspace1,CS1_DISABLE_KBD_LIGHT)
 #define  DisableKbdBackLightGET 				IS_MASK_SET(u4Cfgspace1,CS1_DISABLE_KBD_BACK_LIGHT)
 #define  PwrBtnWarnGET   			IS_MASK_SET(u4Cfgspace1,PBTNWARN)

#define   u4Cfgspace2								SD_uchar_8(OEMRAMA+0x02)
 #define  USBALWOn        			BIT3
 #define  DockingWOLEn    BIT4
 #define  SMART_NumLock   			BIT5
 #define  USBALWONGet     			IS_MASK_SET(u4Cfgspace2,USBALWOn)	//disable AOU in DC mode.1:disable
 #define	DisAOU_DC_En					SET_MASK(u4Cfgspace2,USBALWOn)
 #define	DisAOU_DC_Dis					CLEAR_MASK(u4Cfgspace2,USBALWOn)
 #define  SrtNumLockGet   			IS_MASK_SET(u4Cfgspace2,SMART_NumLock)
 
#define   u4Cfgspace3								SD_uchar_8(OEMRAMA+0x03)
 #define PhantomKeyIgnor	  		BIT1
 #define AirplaneMode     			BIT3 // Set power control mode to Airplane mode
// When this bit is set, maximum charge current is 1.0A in normal state and is 2.2A in suspend and off state. 
// The capability of the adapter is showed correctly but set a 65W AC adapter mode to DC-DC.
 #define FN_STICKY_LED    			BIT4 // 1:Fn Lock LED turns on for Fn Sticky/Lock
 #define GET_FN_STICKY_LED  		IS_MASK_SET(u4Cfgspace3,FN_STICKY_LED)
 #define SET_FN_STICKY_LED  		SET_MASK(u4Cfgspace3,FN_STICKY_LED)
 #define CLR_FN_STICKY_LED  		CLEAR_MASK(u4Cfgspace3,FN_STICKY_LED)

#define   u4SoundMask1								SD_uchar_8(OEMRAMA+0x04)
 #define  Reserved      			BIT0    // Reserved
 #define  CriLowBatt    			BIT1    // Critical low
 #define  LowBatt       			BIT2    // Low batt
 #define  Suspend       			BIT3    // Suspend
 #define  VMSuspend     			BIT4    // VMSsupend
 #define  Resume        			BIT5    // Resume
 #define  DCInOut       			BIT6    // DC in/out
 #define  PowerOff      			BIT7    // Power off
 #define  BeepCriLowGET 			IS_MASK_SET(u4SoundMask1,CriLowBatt)
 #define  BeepLowGET    			IS_MASK_SET(u4SoundMask1,LowBatt)
 #define  SuspendGET    			IS_MASK_SET(u4SoundMask1,Suspend)
 #define  VMSuspendGET  			IS_MASK_SET(u4SoundMask1,VMSuspend)
 #define  ResumeGET     			IS_MASK_SET(u4SoundMask1,Resume)
 #define  DCInOutGET    			IS_MASK_SET(u4SoundMask1,DCInOut)
 #define  PowerOffGET   			IS_MASK_SET(u4SoundMask1,PowerOff)
 
#define   u4SoundMask2								SD_uchar_8(OEMRAMA+0x05)
 #define  PowerOffWarn  			BIT0    // Power off warn
 #define  NoHDDWarn     			BIT1    // No HDD warn
 #define  Dead          			BIT2    // Dead
 #define  F440Hz        			BIT3    // 440Hz
 #define  F315Hz        			BIT4    // 315Hz
 #define  F315Hz2Times  			BIT5    // 315Hz 2 times
 #define  F315Hz3Times  			BIT6    // 315Hz 2 times
 #define  InhibitSwap   			BIT7    // Inhibit swap
 #define  PowerOffWarnGET  			IS_MASK_SET(u4SoundMask2,PowerOffWarn)
 #define  NoHDDWarnGET     			IS_MASK_SET(u4SoundMask2,NoHDDWarn)
 #define  DeadGET          			IS_MASK_SET(u4SoundMask2,Dead)
 #define  F440HzGET        			IS_MASK_SET(u4SoundMask2,F440Hz)
 #define  F315HzGET        			IS_MASK_SET(u4SoundMask2,F315Hz)
 #define  F315Hz2TimesGET  			IS_MASK_SET(u4SoundMask2,F315Hz2Times)
 #define  F315Hz3TimesGET  			IS_MASK_SET(u4SoundMask2,F315Hz3Times)
 #define  InhibitSwapGET   			IS_MASK_SET(u4SoundMask2,InhibitSwap)
#define   u4BeepID									SD_uchar_8(OEMRAMA+0x06)
#define   u4BeepRepeat								SD_uchar_8(OEMRAMA+0x07)
#define   u4BeepMask3								SD_uchar_8(OEMRAMA+0x08)
#define   u4KBDID									SD_uchar_8(OEMRAMA+0x09)
 #define  KBD_WITH_NUMPAD           0x00           // Other than Japanese Keyboard
 #define  JP_KBD_WITH_NUMPAD        0x01           // Japanese Keyboard
 #define  CLASSIC_TOP_ROW           0x04           // Fn key as primary
 #define  NO_KBD                    0x07
 #define  MODERN_TOP_ROW            0x08           // Media key as primary
 
#define   u4Reserve0A								SD_uchar_8(OEMRAMA+0x0A)
#define   u4Reserve0B								SD_uchar_8(OEMRAMA+0x0B)
#define   u4LEDcontrol								SD_uchar_8(OEMRAMA+0x0C)
 #define  LED_TURN_ON          		BIT7
 #define  LED_Blinking         		BIT6
 #define  LED_Blink_gradually  		BIT5
 #define  LED_ID  					(BIT0 + BIT1 + BIT2 + BIT3)
 #define  u4LEDcontrolGet    		IS_MASK_SET(u4LEDcontrol,LED_TURN_ON)
#define   u4PeriCtrl3								SD_uchar_8(OEMRAMA+0x0D)
 #define  USB_PWR          			BIT0
 #define  RST_TRACK_POINT  			BIT2
 #define  BYPASS_CLICKPAD  			BIT5
 #define  PC3_TURN_ON_KEYBOARD_BACK_LIGHT  			(u4PeriCtrl3&(BIT6 + BIT7))
 #define  PC3_TURN_ON_KEYBOARD_BACK_LIGHT_MASK  			(BIT6 + BIT7)
 #define  USB_PWRGET           		IS_MASK_SET(u4PeriCtrl3,USB_PWR)
 #define  USB_PWREn            		SET_MASK(u4PeriCtrl3,USB_PWR)
 #define  USB_PWRDis           		CLEAR_MASK(u4PeriCtrl3,USB_PWR)
 #define  RST_TRACK_POINT_GET  		IS_MASK_SET(u4PeriCtrl3,RST_TRACK_POINT)
 #define  BYPASS_CLICKPAD_GET  		IS_MASK_SET(u4PeriCtrl3,BYPASS_CLICKPAD)

#define   u4PeriSTS4								SD_uchar_8(OEMRAMA+0x0E)
// BIT[1:0] 
//  Fn key status 
//  00: unlock
//  01: sticky
//  10: lock
//  11: reserved
 #define  STICKY_MODE    			BIT0
 #define  STICKY_LOCK    			BIT1
 #define  GSENSOR_ERROR  			BIT2           // BIT2 Gravity sensor diag error
 #define  Inhibit_charging  			BIT3  // BIT3 Inhibit charging (this is one while at least one battery is inhibit to charge)
 #define  PeakShiftDischargSTS  	BIT4  // Bit 4 Peak Shift Discharge Status. 1:Start Discharge/0:Stop Discharge
 #define  GSENSOR_DIAG_DONE  		BIT5  // BIT5 Gravity sensor diagnostic program is running now
 // BIT[7:6]
//  Gravity sensor ID
//  00: no sensor
//  01: STMicro
//  10: Memsic
//  11: ADI
 #define  STICKY_MODE_GET    		IS_MASK_SET(u4PeriSTS4,STICKY_MODE)
 #define  STICKY_MODE_SET    		SET_MASK(u4PeriSTS4,STICKY_MODE)
 #define  STICKY_MODE_CLR    		CLEAR_MASK(u4PeriSTS4,STICKY_MODE) 
 #define  STICKY_LOCK_GET    		IS_MASK_SET(u4PeriSTS4,STICKY_LOCK)
 #define  STICKY_LOCK_SET    		SET_MASK(u4PeriSTS4,STICKY_LOCK)
 #define  STICKY_LOCK_CLR    		CLEAR_MASK(u4PeriSTS4,STICKY_LOCK) 
 #define  GSENSOR_ERROR_SET  		SET_MASK(u4PeriSTS4,GSENSOR_ERROR)
 #define  GSENSOR_ERROR_CLR  		CLEAR_MASK(u4PeriSTS4,GSENSOR_ERROR)
 #define  GSENSOR_DIAG_DONE_SET  	SET_MASK(u4PeriSTS4,GSENSOR_DIAG_DONE)
 #define  GSENSOR_DIAG_DONE_CLR  	CLEAR_MASK(u4PeriSTS4,GSENSOR_DIAG_DONE)
 #define  Inhibit_chargingSET   	SET_MASK(u4PeriSTS4,Inhibit_charging)
 #define  Inhibit_chargingCLR   	CLEAR_MASK(u4PeriSTS4,Inhibit_charging)
 #define  Inhibit_chargingGet   	IS_MASK_SET(u4PeriSTS4,Inhibit_charging)
 #define  PeakShiftDischargSTSSET  SET_MASK(u4PeriSTS4,PeakShiftDischargSTS)
 #define  PeakShiftDischargSTSCLR  CLEAR_MASK(u4PeriSTS4,PeakShiftDischargSTS)
 #define  PeakShiftDischargSTSGet  IS_MASK_SET(u4PeriSTS4,PeakShiftDischargSTS)
 

#define   u4PeriSTS5								SD_uchar_8(OEMRAMA+0x0F)
 #define  PriBattinhibit    		BIT0       // The primary battery is inhibited to charge
 #define  SecBattinhibit    		BIT1       // The secondary battery is inhibited to charge
 #define  PriBattDisChg     		BIT2       // The primary battery is discharging to refresh itself.
 #define  SecBattDisChg     		BIT3       // The secondary battery is discharging to refresh itself.
 #define  b4HDDdetached     		BIT4       // HDD was detached in suspend.
 #define  BayHDDdetached    		BIT5       // Bay HDD was detached in suspend
 #define  NumLockSTS        		BIT6       // NumLock state of the embedded keyboad
 #define  PriBattDisChgEn   		SET_MASK(u4PeriSTS5,PriBattDisChg)
 #define  PriBattDisChgDis  		CLEAR_MASK(u4PeriSTS5,PriBattDisChg)
 #define  PriBattDisChgGet  		IS_MASK_SET(u4PeriSTS5,PriBattDisChg)
 #define  SecBattDisChgEn   		SET_MASK(u4PeriSTS5,SecBattDisChg)
 #define  SecBattDisChgDis  		CLEAR_MASK(u4PeriSTS5,SecBattDisChg)
 #define  SecBattDisChgGet  		IS_MASK_SET(u4PeriSTS5,SecBattDisChg)
 #define  PriBattinhibitEn  		SET_MASK(u4PeriSTS5,PriBattinhibit)
 #define  PriBattinhibitDis 		CLEAR_MASK(u4PeriSTS5,PriBattinhibit)
 #define  PriBattinhibitGet 		IS_MASK_SET(u4PeriSTS5,PriBattinhibit)
 #define  SecBattinhibitEn  		SET_MASK(u4PeriSTS5,SecBattinhibit)
 #define  SecBattinhibitDis 		CLEAR_MASK(u4PeriSTS5,SecBattinhibit)
 #define  SecBattinhibitGet 		IS_MASK_SET(u4PeriSTS5,SecBattinhibit) 
 #define  NumLockEn         		SET_MASK(u4PeriSTS5,NumLockSTS)
 #define  NumLockDis        		CLEAR_MASK(u4PeriSTS5,NumLockSTS)
 #define  NumLockGet        		IS_MASK_SET(u4PeriSTS5,NumLockSTS)
 
#define   AttentionOffset							SD_uchar_8(OEMRAMA+0x10)
#define   u4Reserve11								SD_uchar_8(OEMRAMA+0x11)
#define   u4Reserve12								SD_uchar_8(OEMRAMA+0x12)
#define   u4Reserve13								SD_uchar_8(OEMRAMA+0x13)
#define   u4Reserve14								SD_uchar_8(OEMRAMA+0x14)
#define   u4Reserve15								SD_uchar_8(OEMRAMA+0x15)
#define   u4Reserve16								SD_uchar_8(OEMRAMA+0x16)
#define   u4Reserve17								SD_uchar_8(OEMRAMA+0x17)
#define   u4Reserve18								SD_uchar_8(OEMRAMA+0x18)
#define   u4Reserve19								SD_uchar_8(OEMRAMA+0x19)
#define   u4Reserve1A								SD_uchar_8(OEMRAMA+0x1A)
#define   u4Reserve1B								SD_uchar_8(OEMRAMA+0x1B)
#define   u4Reserve1C								SD_uchar_8(OEMRAMA+0x1C)
#define   u4Reserve1D								SD_uchar_8(OEMRAMA+0x1D)
#define   u4Reserve1E								SD_uchar_8(OEMRAMA+0x1E)
#define   u4Reserve1F								SD_uchar_8(OEMRAMA+0x1F)
#define   u4Reserve20								SD_uchar_8(OEMRAMA+0x20)
#define   u4InhChgTimerL							SD_uchar_8(OEMRAMA+0x21)
#define   u4InhChgTimerH							SD_uchar_8(OEMRAMA+0x22)
#define   u4ExtCommand								SD_uchar_8(OEMRAMA+0x23)
#define   uThresholdSetting					SD_uchar_8(OEMRAMA+0x24) // Battery charge start/ stop threshold setting for the main battery
#define   uThreshold2ndSetting				SD_uchar_8(OEMRAMA+0x25) // Battery charge start/ stop threshold setting for the secondary battery
 #define  SET_CHARGE_CAPACITY_FLAG BIT7
// 0xA24 and 0xA25
// Battery Charge start/stop threshold setting of the primary(0x24)/seconday(0x25) battery.
// When bit 2 of configuration space 3 is 0, bit 0 - 6 represent the capacity to start to charge.
// Otherwise bit 0-6 represent the capacity to stop charging. 
// Bit 7 is needed to set to 1 when a value is written to this byte. 
// When the command is done, bit 7 is cleared.

#define   TamperSTS									SD_uchar_8(OEMRAMA+0x26)
 #define b0HDDReset        			BIT0
 #define b0mSataReset      			BIT1
 #define b0mSata2Reset     			BIT2
 #define b0BayHDDReset     			BIT3
 #define BottomTamper            BIT4
 
#define   u4Reserve27								SD_uchar_8(OEMRAMA+0x27)
#define   p2IBMChargeflag							SD_uchar_8(OEMRAMA+0x28)
 #define  p2SetInhChgTimer  		BIT0
 #define  p2BattExistTemp   		BIT1
 #define  p2PeakShiftFlag   		BIT2
 #define  p2Reserve28_bit3  		BIT3
 #define  p2Reserve28_bit4  		BIT4
 #define  p2Reserve28_bit5  		BIT5
 #define  p2Reserve28_bit6  		BIT6
 #define  p2Reserve28_bit7  		BIT7
 #define  p2BattExistTempGet  		IS_MASK_SET(p2IBMChargeflag,p2BattExistTemp)
 #define  p2BattExistTempEn   		SET_MASK(p2IBMChargeflag,p2BattExistTemp)
 #define  p2BattExistTempDis  		CLEAR_MASK(p2IBMChargeflag,p2BattExistTemp)
 #define  p2SetInhChgTimerGet 		IS_MASK_SET(p2IBMChargeflag,p2SetInhChgTimer)
 #define  p2SetInhChgTimerEn  		SET_MASK(p2IBMChargeflag,p2SetInhChgTimer)
 #define  p2SetInhChgTimerDis 		CLEAR_MASK(p2IBMChargeflag,p2SetInhChgTimer)
 
#define   p2InhChgTimer								SD_uchar_8(OEMRAMA+0x29)
#define   u4AttentionReq							SD_uchar_8(OEMRAMA+0x2A)
#define   u4TripPointCapacity0H						SD_uchar_8(OEMRAMA+0x2B)
#define   u4TripPointCapacity0L						SD_uchar_8(OEMRAMA+0x2C)
#define   u4TripPointCapacity1H						SD_uchar_8(OEMRAMA+0x2D)
#define   u4TripPointCapacity1L						SD_uchar_8(OEMRAMA+0x2E)
#define   u4Fanspeedcontrol							SD_uchar_8(OEMRAMA+0x2F)
 #define  FanAutomaticmode     		BIT7     //Automatic mode (Fan speed is controlled by the thermal level)
 #define  FanMaximumspeed      		BIT6     //Maximum speed (run at fastest speed)
 #define  Fanspeedlevel        		BIT0 + BIT1 + BIT2  //fan speed level (0: stop, 7: highest speed)
 #define  FanAutomaticmodeEn     	SET_MASK(u4Fanspeedcontrol,BIT7)
 #define  FanAutomaticmodeDis    	CLEAR_MASK(u4Fanspeedcontrol,BIT7)    
 #define  FanAutomaticmodeGet    	IS_MASK_SET(u4Fanspeedcontrol,BIT7) 
 #define  FanMaximumspeedSet     	SET_MASK(u4Fanspeedcontrol,BIT6)      
 #define  FanMaximumspeedCLR     	CLEAR_MASK(u4Fanspeedcontrol,BIT6)      
 #define  FanMaximumspeedGet     	IS_MASK_SET(u4Fanspeedcontrol,BIT6)  
 
#define   u4Audio									SD_uchar_8(OEMRAMA+0x30)
 #define  Mutevolume   				BIT6      //
 #define  MutevolumeEGet    		IS_MASK_SET(u4Audio,Mutevolume)
 
#define   u4Peripheralcontrol2						SD_uchar_8(OEMRAMA+0x31)
#define   EventMask0								SD_uchar_8(OEMRAMA+0x32)
 #define  EnvntWOLAC     			BIT0  // Wake On LAN. (AC)
 #define  CritLowBatt    			BIT1
 #define  LID39          			BIT2
 #define  EjectBtn       			BIT3
 #define  FnKeyPress     			BIT4
 #define  PortfWake      			BIT5
 #define  ModemRing      			BIT6  //AOAC
 #define  UltraBayUnlock 			BIT7
 #define  LidOpenGET     			IS_MASK_SET(EventMask0,LID39)
 #define  WOLACWakeEnGet 			IS_MASK_SET(EventMask0,EnvntWOLAC)
 #define  LidWakeEnGet   			IS_MASK_SET(EventMask0,LID39)
#define   EventMask1								SD_uchar_8(OEMRAMA+0x33)
 #define  LeftBayUnlock  			BIT0  // Left Bay Unlock
 #define  EnvntWOLDC     			BIT1  // Wake On LAN. (DC)
 #define  WOLDCWakeEnGet 			IS_MASK_SET(EventMask1,EnvntWOLDC)
 
#define   u4PeriSTS2								SD_uchar_8(OEMRAMA+0x34)
 #define  APS2_KBD_LIGHT        			BIT0
 #define  Beeping        			BIT1
 #define  SMBbusy        			BIT2
 #define  FanOK          			BIT4
 #define  GSexist        			BIT5
 #define  APS2_KBD_BACKLIGHT        			BIT6
 #define  PWRconWarn     			BIT7
 #define  KbdLightExistEN       			SET_MASK(u4PeriSTS2,APS2_KBD_LIGHT)
 #define  KbdLightExistDIS      			CLEAR_MASK(u4PeriSTS2,APS2_KBD_LIGHT)
 #define  BeepingEN      			SET_MASK(u4PeriSTS2,Beeping)
 #define  BeepingDIS     			CLEAR_MASK(u4PeriSTS2,Beeping)
 #define  FanExistEN     			SET_MASK(u4PeriSTS2,FanOK)
 #define  FanExistDIS    			CLEAR_MASK(u4PeriSTS2,FanOK)
 #define  FanExistGET    			IS_MASK_SET(u4PeriSTS2,FanOK)
 #define  GExistEN       			SET_MASK(u4PeriSTS2,GSexist)   
 #define  GExistDIS      			CLEAR_MASK(u4PeriSTS2,GSexist)
 #define  GExistGET      			IS_MASK_SET(u4PeriSTS2,GSexist)
 #define  KbdBackLightExistEN       			SET_MASK(u4PeriSTS2,APS2_KBD_BACKLIGHT)
 #define  KbdBackLightExistDIS      			CLEAR_MASK(u4PeriSTS2,APS2_KBD_BACKLIGHT)
 
#define   u4PeriSTS3								SD_uchar_8(OEMRAMA+0x35)
 #define  FanRstOK        			BIT2
 #define  AttTempOFF      			BIT3  // CMW 20121031 for attention function
 #define  FanErr          			BIT4
 #define  UNSUPPORTED_AC  			BIT5
 #define  ThermalErr      			BIT7
 #define  FanInitEN       			SET_MASK(u4PeriSTS3,FanRstOK)
 #define  FanInitDIS      			CLEAR_MASK(u4PeriSTS3,FanRstOK)
 #define  FanInitGET      			IS_MASK_SET(u4PeriSTS3,FanRstOK)
 #define  AttTempOFFEN    			SET_MASK(u4PeriSTS3,AttTempOFF)  // CMW 20121031 for attention function
 #define  AttTempOFFDIS   			CLEAR_MASK(u4PeriSTS3,AttTempOFF)
 #define  AttTempOFFGET   			IS_MASK_SET(u4PeriSTS3,AttTempOFF)
 #define  FanErrEN        			SET_MASK(u4PeriSTS3,FanErr)
 #define  FanErrDIS       			CLEAR_MASK(u4PeriSTS3,FanErr)
 #define  FanErrGET       			IS_MASK_SET(u4PeriSTS3,FanErr)
 #define  FintekErrEN     			SET_MASK(u4PeriSTS3,ThermalErr)
 #define  FintekErrDIS    			CLEAR_MASK(u4PeriSTS3,ThermalErr)
 #define  FintekErrGET    			IS_MASK_SET(u4PeriSTS3,ThermalErr)
 
#define   u4Reserve36								SD_uchar_8(OEMRAMA+0x36)
#define   u4Reserve37								SD_uchar_8(OEMRAMA+0x37)
#define   PriBattSTS								SD_uchar_8(OEMRAMA+0x38)
 #define  CapLevel1  				BIT0
 #define  CapLevel2  				BIT1
 #define  CapLevel3  				BIT2
 #define  NonGenBatt 				BIT3
 #define  BattError  				BIT4
 #define  Charging   				BIT5
 #define  DisChg     				BIT6
 #define  BattExist  				BIT7
 #define  NonGenuineBattEn  		SET_MASK(PriBattSTS,NonGenBatt)   //(TBD)
 #define  NonGenuineBattDis 		CLEAR_MASK(PriBattSTS,NonGenBatt)   //(TBD)
 #define  BattErrorEn       		SET_MASK(PriBattSTS,BattError)
 #define  BattErrorDis      		CLEAR_MASK(PriBattSTS,BattError)
 #define  ChargingEn        		SET_MASK(PriBattSTS,Charging)
 #define  ChargingDis       		CLEAR_MASK(PriBattSTS,Charging)
 #define  DisChgEn          		SET_MASK(PriBattSTS,DisChg)
 #define  DisChgDis         		CLEAR_MASK(PriBattSTS,DisChg)
 #define  BattExistEn       		SET_MASK(PriBattSTS,BattExist)
 #define  BattExistGet      		IS_MASK_SET(PriBattSTS,BattExist)
 #define  BattExistDis      		CLEAR_MASK(PriBattSTS,BattExist)
 
#define   SecBattSTS								SD_uchar_8(OEMRAMA+0x39)
 #define  SecBattExistEn       		SET_MASK(SecBattSTS,BattExist)
 #define  SecBattExistDis      		CLEAR_MASK(SecBattSTS,BattExist)
 #define  SecBattExistGet      		IS_MASK_SET(SecBattSTS,BattExist)
 
#define   u4PeriCtrl0								SD_uchar_8(OEMRAMA+0x3A)
 #define  BTsts      				BIT4
 #define  WLANsts    				BIT5
 #define  WWANsts    				BIT6
 #define  BTstsEn    				SET_MASK(u4PeriCtrl0,BTsts)
 #define  BTstsDis   				CLEAR_MASK(u4PeriCtrl0,BTsts)
 #define  BTstsGet   				IS_MASK_SET(u4PeriCtrl0,BTsts)
 #define  WLANstsEn  				SET_MASK(u4PeriCtrl0,WLANsts)
 #define  WLANstsDis 				CLEAR_MASK(u4PeriCtrl0,WLANsts)
 #define  WLANstsGet 				IS_MASK_SET(u4PeriCtrl0,WLANsts)
 #define  WWANstsEn  				SET_MASK(u4PeriCtrl0,WWANsts)
 #define  WWANstsDis 				CLEAR_MASK(u4PeriCtrl0,WWANsts)
 #define  WWANstsGet 				IS_MASK_SET(u4PeriCtrl0,WWANsts)
 
#define     u4PeriCtrl1								SD_uchar_8(OEMRAMA+0x3B)
 #define  b0Mute     				BIT0
 #define  KBlight    				BIT1
 #define  FNLK_STATE 				BIT3       // Set Fn+Esc lock state, lock if 1, unlock if 0
 #define  USBon      				BIT4
 #define  Edge_PriBattInhib  		BIT5
 #define  Edge_SecBattInhib  		BIT6
 #define  b0MuteGET     			IS_MASK_SET(u4PeriCtrl1,b0Mute)    
 #define  b0MuteCPL     			INVERSE_REG_MASK(u4PeriCtrl1,b0Mute)
 #define  b0MuteSET     			SET_MASK(u4PeriCtrl1,b0Mute)    
 #define  KBlightGET    			IS_MASK_SET(u4PeriCtrl1,KBlight)
 #define  KBlightCPL    			INVERSE_REG_MASK(u4PeriCtrl1,KBlight)
 #define  KBlightSET    			SET_MASK(u4PeriCtrl1,KBlight)
 #define  KBlightCLR    			CLEAR_MASK(u4PeriCtrl1,KBlight)
 #define  FNLKSTATEGET  			IS_MASK_SET(u4PeriCtrl1,FNLK_STATE)    
 #define  FNLKSTATECLR  			CLEAR_MASK(u4PeriCtrl1,FNLK_STATE)
 #define  FNLKSTATESET  			SET_MASK(u4PeriCtrl1,FNLK_STATE)   
 #define  PriBattInhibGET  	IS_MASK_SET(u4PeriCtrl1,Edge_PriBattInhib)
 #define  SecBattInhibGET  	IS_MASK_SET(u4PeriCtrl1,Edge_SecBattInhib)
 #define  TurnOnUSBGET  			IS_MASK_SET(u4PeriCtrl1,USBon)
 #define  TurnOnUSBEn   			SET_MASK(u4PeriCtrl1,USBon)
 #define  TurnOnUSBDis  			CLEAR_MASK(u4PeriCtrl1,USBon)
 
#define   u4Reserve3C								SD_uchar_8(OEMRAMA+0x3C)
#define   u4PassCtrl								SD_uchar_8(OEMRAMA+0x3D)
 #define  PassCtrlBusy  			BIT7
 #define  PassCtrlBusyDIS 			CLEAR_MASK(u4PassCtrl,PassCtrlBusy)
 #define  PassCtrlBusyGET 			IS_MASK_SET(u4PassCtrl,PassCtrlBusy)
 
#define   u4PassData1								SD_uchar_8(OEMRAMA+0x3E)
#define   u4PassData2								SD_uchar_8(OEMRAMA+0x3F)
#define   u4PassData3								SD_uchar_8(OEMRAMA+0x40)
#define   u4PassData4								SD_uchar_8(OEMRAMA+0x41)
#define   u4PassData5								SD_uchar_8(OEMRAMA+0x42)
#define   u4PassData6								SD_uchar_8(OEMRAMA+0x43)
#define   u4PassData7								SD_uchar_8(OEMRAMA+0x44)
#define   u4PassData8								SD_uchar_8(OEMRAMA+0x45)
#define   u4SenseSTS0								SD_uchar_8(OEMRAMA+0x46)
 #define  Edge_LidStatus  			BIT2
 #define  ACSts           			BIT4
 #define  LidStsEn        			SET_MASK(u4SenseSTS0,Edge_LidStatus)   // Lid is open
 #define  LidStsDis       			CLEAR_MASK(u4SenseSTS0,Edge_LidStatus)   // Lid is closed
 #define  ACEn            			SET_MASK(u4SenseSTS0,ACSts)
 #define  ACDis           			CLEAR_MASK(u4SenseSTS0,ACSts)
 #define  AC_MODE         			IS_MASK_SET(u4SenseSTS0,ACSts)
 
#define   u4SenseSTS1								SD_uchar_8(OEMRAMA+0x47)
 #define HDDisinthebay   			BIT3
 #define b6mSatadet      			BIT7

#define   u4SenseSTS2								SD_uchar_8(OEMRAMA+0x48)
 #define Kill_SW         			BIT1
 #define DISVGA          			BIT4
 #define b6HDDdet        			BIT6
 #define Kill_SWEn       			SET_MASK(u4SenseSTS2,Kill_SW)
 #define Kill_SWDis      			CLEAR_MASK(u4SenseSTS2,Kill_SW)
 #define DisVGAEn        			SET_MASK(u4SenseSTS2,DISVGA)
 #define DisVGADis       			CLEAR_MASK(u4SenseSTS2,DISVGA)
 
#define   u4SenseSTS3								SD_uchar_8(OEMRAMA+0x49)
 #define SomeAcAdapterIn 			BIT2
 
#define   u4Reserve4A								SD_uchar_8(OEMRAMA+0x4A)
#define   u4Reserve4B								SD_uchar_8(OEMRAMA+0x4B)
#define   u4Reserve4C								SD_uchar_8(OEMRAMA+0x4C)
#define   u4Reserve4D								SD_uchar_8(OEMRAMA+0x4D)
#define   EventSTS									SD_uchar_8(OEMRAMA+0x4E)
 #define EvPME          			BIT0
 #define EvCritLowBatt 			 	BIT1
 #define EvLID39        			BIT2
 #define EvEjectBtn     			BIT3
 #define EvFnKeyPress   			BIT4
 #define EvPortfWake    			BIT5
 #define EvModemRing    			BIT6    //AOAC
 #define EvUltraBayUnlock 			BIT7
 #define PMEWakeEn     				SET_MASK(EventSTS,EvPME)
 #define PMEWakeGet    				IS_MASK_SET(EventSTS,EvPME)
 #define LLBWakeEn     				SET_MASK(EventSTS,EvCritLowBatt)
 #define LidWakeEn     				SET_MASK(EventSTS,EvLID39)
 #define LidWakeGet    				IS_MASK_SET(EventSTS,EvLID39)
 #define KeyWakeEn     				SET_MASK(EventSTS,EvFnKeyPress)
 
#define   u4Reserve4F								SD_uchar_8(OEMRAMA+0x4F)
#define   uSMBusPrtcl								SD_uchar_8(OEMRAMA+0x50)
#define   uSMBusSts									SD_uchar_8(OEMRAMA+0x51)
 #define SMB_STS_ALARM	BIT6    //AOAC
 #define SMB_STS_DONE		BIT7
#define   uSMBusAddr								SD_uchar_8(OEMRAMA+0x52)
#define   uSMBusCmd									SD_uchar_8(OEMRAMA+0x53)
#define   uSMBusData0								SD_uchar_8(OEMRAMA+0x54)
#define   uSMBusData1								SD_uchar_8(OEMRAMA+0x55)
#define   uSMBusData2								SD_uchar_8(OEMRAMA+0x56)
#define   uSMBusData3								SD_uchar_8(OEMRAMA+0x57)
#define   uSMBusData4								SD_uchar_8(OEMRAMA+0x58)
#define   uSMBusData5								SD_uchar_8(OEMRAMA+0x59)
#define   uSMBusData6								SD_uchar_8(OEMRAMA+0x5A)
#define   uSMBusData7								SD_uchar_8(OEMRAMA+0x5B)
#define   uSMBusData8								SD_uchar_8(OEMRAMA+0x5C)
#define   uSMBusData9								SD_uchar_8(OEMRAMA+0x5D)
#define   uSMBusData10								SD_uchar_8(OEMRAMA+0x5E)
#define   uSMBusData11								SD_uchar_8(OEMRAMA+0x5F)
#define   uSMBusData12								SD_uchar_8(OEMRAMA+0x60)
#define   uSMBusData13								SD_uchar_8(OEMRAMA+0x61)
#define   uSMBusData14								SD_uchar_8(OEMRAMA+0x62)
#define   uSMBusData15								SD_uchar_8(OEMRAMA+0x63)
#define   uSMBusData16								SD_uchar_8(OEMRAMA+0x64)
#define   uSMBusData17								SD_uchar_8(OEMRAMA+0x65)
#define   uSMBusData18								SD_uchar_8(OEMRAMA+0x66)
#define   uSMBusData19								SD_uchar_8(OEMRAMA+0x67)
#define   uSMBusData20								SD_uchar_8(OEMRAMA+0x68)
#define   uSMBusData21								SD_uchar_8(OEMRAMA+0x69)
#define   uSMBusData22								SD_uchar_8(OEMRAMA+0x6A)
#define   uSMBusData23								SD_uchar_8(OEMRAMA+0x6B)
#define   uSMBusData24								SD_uchar_8(OEMRAMA+0x6C)
#define   uSMBusData25								SD_uchar_8(OEMRAMA+0x6D)
#define   uSMBusData26								SD_uchar_8(OEMRAMA+0x6E)
#define   uSMBusData27								SD_uchar_8(OEMRAMA+0x6F)
#define   uSMBusData28								SD_uchar_8(OEMRAMA+0x70)
#define   uSMBusData29								SD_uchar_8(OEMRAMA+0x71)
#define   uSMBusData30								SD_uchar_8(OEMRAMA+0x72)
#define   uSMBusData31								SD_uchar_8(OEMRAMA+0x73)
#define   uSMBusBcnt								SD_uchar_8(OEMRAMA+0x74)
#define   uSMBusAlarAddr							SD_uchar_8(OEMRAMA+0x75)
#define   uSMBusAlarData							SD_uchar_8(OEMRAMA+0x76)
#define   u4Reserve77								SD_uchar_8(OEMRAMA+0x77)
#define   u4CpuThermal								SD_uchar_8(OEMRAMA+0x78)
#define   u4Reserve79								SD_uchar_8(OEMRAMA+0x79)
#define   u4Reserve7A								SD_uchar_8(OEMRAMA+0x7A)
#define   u4Reserve7B								SD_uchar_8(OEMRAMA+0x7B)
#define   u4Reserve7C								SD_uchar_8(OEMRAMA+0x7C)
#define   u4Reserve7D								SD_uchar_8(OEMRAMA+0x7D)
#define   u4Reserve7E								SD_uchar_8(OEMRAMA+0x7E)
#define   u4Reserve7F								SD_uchar_8(OEMRAMA+0x7F)
#define   u4AttentionCtrl							SD_uchar_8(OEMRAMA+0x80)
#define   u4BattInfoID								SD_uchar_8(OEMRAMA+0x81)
#define   u4Reserve82								SD_uchar_8(OEMRAMA+0x82)
#define   u4Reserve83								SD_uchar_8(OEMRAMA+0x83)
#define   u4FanSpdL									SD_uchar_8(OEMRAMA+0x84)
#define   u4FanSpdH									SD_uchar_8(OEMRAMA+0x85)
#define   u4PasswordSts1							SD_uchar_8(OEMRAMA+0x86)
#define   u4PasswordSts2							SD_uchar_8(OEMRAMA+0x87)
#define   u4Reserve88								SD_uchar_8(OEMRAMA+0x88)
#define   u4Reserve89								SD_uchar_8(OEMRAMA+0x89)
#define   u4Reserve8A								SD_uchar_8(OEMRAMA+0x8A)
#define   u4Reserve8B								SD_uchar_8(OEMRAMA+0x8B)
#define   u4Reserve8C								SD_uchar_8(OEMRAMA+0x8C)
#define   u4Reserve8D								SD_uchar_8(OEMRAMA+0x8D)
#define   u4Reserve8E								SD_uchar_8(OEMRAMA+0x8E)
#define   u4ManualFanPRM							SD_uchar_8(OEMRAMA+0x8F)
#define   u4Reserve90								SD_uchar_8(OEMRAMA+0x90)
#define   u4Reserve91								SD_uchar_8(OEMRAMA+0x91)
#define   u4Reserve92								SD_uchar_8(OEMRAMA+0x92)
#define   u4Reserve93								SD_uchar_8(OEMRAMA+0x93)
#define   u4Reserve94								SD_uchar_8(OEMRAMA+0x94)
#define   u4Reserve95								SD_uchar_8(OEMRAMA+0x95)
#define   u4Reserve96								SD_uchar_8(OEMRAMA+0x96)
#define   u4Reserve97								SD_uchar_8(OEMRAMA+0x97)
#define   u4Reserve98								SD_uchar_8(OEMRAMA+0x98)
#define   u4Reserve99								SD_uchar_8(OEMRAMA+0x99)
#define   u4Reserve9A								SD_uchar_8(OEMRAMA+0x9A)
#define   u4Reserve9B								SD_uchar_8(OEMRAMA+0x9B)
#define   u4Reserve9C								SD_uchar_8(OEMRAMA+0x9C)
#define   u4Reserve9D								SD_uchar_8(OEMRAMA+0x9D)
#define   u4Reserve9E								SD_uchar_8(OEMRAMA+0x9E)
#define   u4Reserve9F								SD_uchar_8(OEMRAMA+0x9F)
#define   u4BattInfo1								SD_uchar_8(OEMRAMA+0xA0)
#define   u4BattInfo2								SD_uchar_8(OEMRAMA+0xA1)
#define   u4BattInfo3								SD_uchar_8(OEMRAMA+0xA2)
#define   u4BattInfo4								SD_uchar_8(OEMRAMA+0xA3)
#define   u4BattInfo5								SD_uchar_8(OEMRAMA+0xA4)
#define   u4BattInfo6								SD_uchar_8(OEMRAMA+0xA5)
#define   u4BattInfo7								SD_uchar_8(OEMRAMA+0xA6)
#define   u4BattInfo8								SD_uchar_8(OEMRAMA+0xA7)
#define   u4BattInfo9								SD_uchar_8(OEMRAMA+0xA8)
#define   u4BattInfo10								SD_uchar_8(OEMRAMA+0xA9)
#define   u4BattInfo11								SD_uchar_8(OEMRAMA+0xAA)
#define   u4BattInfo12								SD_uchar_8(OEMRAMA+0xAB)
#define   u4BattInfo13								SD_uchar_8(OEMRAMA+0xAC)
#define   u4BattInfo14								SD_uchar_8(OEMRAMA+0xAD)
#define   u4BattInfo15								SD_uchar_8(OEMRAMA+0xAE)
#define   u4BattInfo16								SD_uchar_8(OEMRAMA+0xAF)
#define   u4MainBattChgOnCap	   			SD_uchar_8(OEMRAMA+0xB0)
#define   u4MainBattChgOffCap				SD_uchar_8(OEMRAMA+0xB1)
#define   u42ndBattChgOnCap					SD_uchar_8(OEMRAMA+0xB2)
#define   u42ndBattChgOffCap					SD_uchar_8(OEMRAMA+0xB3)
#define   u4MainBattCtrl							SD_uchar_8(OEMRAMA+0xB4)
#define   u4ReserveB5								SD_uchar_8(OEMRAMA+0xB5)
#define   u4ReserveB6								SD_uchar_8(OEMRAMA+0xB6)
#define   u4ReserveB7								SD_uchar_8(OEMRAMA+0xB7)
#define   u4ReserveB8								SD_uchar_8(OEMRAMA+0xB8)
#define   u4ReserveB9								SD_uchar_8(OEMRAMA+0xB9)
#define   u4ReserveBA								SD_uchar_8(OEMRAMA+0xBA)
#define   u4ReserveBB								SD_uchar_8(OEMRAMA+0xBB)
#define   u4ReserveBC								SD_uchar_8(OEMRAMA+0xBC)
#define   u4ReserveBD								SD_uchar_8(OEMRAMA+0xBD)
#define   u4ReserveBE								SD_uchar_8(OEMRAMA+0xBE)
#define   u4ReserveBF								SD_uchar_8(OEMRAMA+0xBF)
#define   u4ReserveC0								SD_uchar_8(OEMRAMA+0xC0)
#define   u4ReserveC1								SD_uchar_8(OEMRAMA+0xC1)
#define   uEXTERNAL_POWER_SOURCE_ID	SD_uchar_8(OEMRAMA+0xC2)
#define   ACPI_POWER_ON_EVENT				SD_uchar_8(OEMRAMA+0xC3)
 #define POE_DOCK_WOL   BIT0
 #define EvDockingWOLOn	SET_MASK(ACPI_POWER_ON_EVENT,POE_DOCK_WOL)
#define   u4ReserveC4								SD_uchar_8(OEMRAMA+0xC4)
#define   u4ReserveC5								SD_uchar_8(OEMRAMA+0xC5)
#define   u4ReserveC6								SD_uchar_8(OEMRAMA+0xC6)
#define   u4ReserveC7								SD_uchar_8(OEMRAMA+0xC7)
#define   u4ReserveC8								SD_uchar_8(OEMRAMA+0xC8)
#define   u4ACAdaptorCap							SD_uchar_8(OEMRAMA+0xC9)
#define   u4ReserveCA								SD_uchar_8(OEMRAMA+0xCA)
#define   u4ReserveCB								SD_uchar_8(OEMRAMA+0xCB)
#define   u4AC_PwrH									SD_uchar_8(OEMRAMA+0xCC)
#define   u4AC_PwrL									SD_uchar_8(OEMRAMA+0xCD)
#define   u4Cfgspace4								SD_uchar_8(OEMRAMA+0xCE)
 #define  u4Cfgspace4FnCtrl  			BIT4 // 0: normal, 1: Fn/Ctrl swap
 #define  u4Cfgspace4BATseq  			(BIT5 + BIT6 + BIT7)// dual battery control
 #define  FnCtrlSWAPGet      			IS_MASK_SET(u4Cfgspace4,u4Cfgspace4FnCtrl)

 
#define   u4ReserveCF								SD_uchar_8(OEMRAMA+0xCF)
#define   u4ReserveD0								SD_uchar_8(OEMRAMA+0xD0)
#define   u4PriBattType								SD_uchar_8(OEMRAMA+0xD1)
//bit 0 - 3: reserved
#define  u4PriBattTypeHWauthenticated  	BIT4
#define  u4PriBattTypeAuthorizedBatt   	BIT5
#define  u4PriBattTypeGenuineBatt      	BIT6
#define  u4PriBattTypeValid            	BIT7
#define   u4SecBattType   						SD_uchar_8(OEMRAMA+0xD2)
#define   u4ReserveD3								SD_uchar_8(OEMRAMA+0xD3)
#define   u4ReserveD4								SD_uchar_8(OEMRAMA+0xD4)
#define   u4ReserveD5								SD_uchar_8(OEMRAMA+0xD5)
#define   u4ReserveD6								SD_uchar_8(OEMRAMA+0xD6)
#define   u4ReserveD7								SD_uchar_8(OEMRAMA+0xD7)
#define   u4ReserveD8								SD_uchar_8(OEMRAMA+0xD8)
#define   u4ReserveD9								SD_uchar_8(OEMRAMA+0xD9)
#define   u4ReserveDA								SD_uchar_8(OEMRAMA+0xDA)
#define   u4ReserveDB								SD_uchar_8(OEMRAMA+0xDB)
#define   u4ReserveDC								SD_uchar_8(OEMRAMA+0xDC)
#define   u4ReserveDD								SD_uchar_8(OEMRAMA+0xDD)
#define   u4ReserveDE								SD_uchar_8(OEMRAMA+0xDE)
#define   u4ReserveDF								SD_uchar_8(OEMRAMA+0xDF)
#define   u4ReserveE0								SD_uchar_8(OEMRAMA+0xE0)
#define   u4ReserveE1								SD_uchar_8(OEMRAMA+0xE1)
#define   u4ReserveE2								SD_uchar_8(OEMRAMA+0xE2)
#define   u4ReserveE3								SD_uchar_8(OEMRAMA+0xE3)
#define   u4ReserveE4								SD_uchar_8(OEMRAMA+0xE4)
#define   u4ReserveE5								SD_uchar_8(OEMRAMA+0xE5)
#define   u4ReserveE6								SD_uchar_8(OEMRAMA+0xE6)
#define   u4ReserveE7								SD_uchar_8(OEMRAMA+0xE7)
#define   u4VersionH								SD_uchar_8(OEMRAMA+0xE8)
#define   u4VersionL								SD_uchar_8(OEMRAMA+0xE9)
#define   u4VersionT								SD_uchar_8(OEMRAMA+0xEA)
#define   u4SpecversionL							SD_uchar_8(OEMRAMA+0xEB)
#define   u4EcCapability0							SD_uchar_8(OEMRAMA+0xEC)
 #define capSmartBatt    				BIT2          // True: Smart Battery Data V1.1 Support
 #define capTwoBatt      				BIT5          // True: Secondary Battery Support
 #define capRefFunc      				BIT6          // True: Discharge with AC battery function is supported.
 #define capSmartBattEn  				SET_MASK(u4EcCapability0, capSmartBatt)
 #define capSmartBattDis 				CLEAR_MASK(u4EcCapability0, capSmartBatt)
 #define capTwoBattEn    				SET_MASK(u4EcCapability0, capTwoBatt)
 #define capTwoBattDis   				CLEAR_MASK(u4EcCapability0, capTwoBatt)
 #define capRefFuncEn    				SET_MASK(u4EcCapability0, capRefFunc)
 #define capRefFuncDis   				CLEAR_MASK(u4EcCapability0,capRefFunc)
 
#define   u4EcCapability1							SD_uchar_8(OEMRAMA+0xED)
 #define capNoFAN      					BIT0          // this system has no fan.
 #define capBattLVL    					BIT1          // Highest battery level byte (0EEh) is valid.
 #define capFnDual     					BIT2          // Fn Key can function as another key.
 #define capEnhTWR     					BIT3          // version information of the interface is gotten through TWR/Access Bus interface.
 #define capATM        					BIT5          // adaptive thermal management functions are available.
 #define Fn_primary    					BIT7         // Fn key as primary support
 #define capNoFANEn    					SET_MASK(u4EcCapability1, capNoFAN)
 #define capNoFANDis   					CLEAR_MASK(u4EcCapability1, capNoFAN)
 #define capBattLVLEn  					SET_MASK(u4EcCapability1, capBattLVL)
 #define capBattLVLDis 					CLEAR_MASK(u4EcCapability1, capBattLVL)
 #define capFnDualEn   					SET_MASK(u4EcCapability1, capFnDual)
 #define capFnDualDis  					CLEAR_MASK(u4EcCapability1, capFnDual)
 #define capEnhTWREn   					SET_MASK(u4EcCapability1, capEnhTWR)
 #define capEnhTWRDis  					CLEAR_MASK(u4EcCapability1, capEnhTWR)
 #define capATMEn      					SET_MASK(u4EcCapability1, capATM)
 #define capATMDis     					CLEAR_MASK(u4EcCapability1, capATM)
 
#define   u4HighBattLV								SD_uchar_8(OEMRAMA+0xEE)
#define   u4SpecversionH							SD_uchar_8(OEMRAMA+0xEF)
#define   u4BuildID1								SD_uchar_8(OEMRAMA+0xF0)
#define   u4BuildID2								SD_uchar_8(OEMRAMA+0xF1)
#define   u4BuildID3								SD_uchar_8(OEMRAMA+0xF2)
#define   u4BuildID4								SD_uchar_8(OEMRAMA+0xF3)
#define   u4BuildID5								SD_uchar_8(OEMRAMA+0xF4)
#define   u4BuildID6								SD_uchar_8(OEMRAMA+0xF5)
#define   u4BuildID7								SD_uchar_8(OEMRAMA+0xF6)
#define   u4BuildID8								SD_uchar_8(OEMRAMA+0xF7)
#define   u4BuildDate1								SD_uchar_8(OEMRAMA+0xF8)
#define   u4BuildDate2								SD_uchar_8(OEMRAMA+0xF9)
#define   u4BuildTime1								SD_uchar_8(OEMRAMA+0xFA)
#define   u4BuildTime2								SD_uchar_8(OEMRAMA+0xFB)
#define   u4ReserveFC								SD_uchar_8(OEMRAMA+0xFC)
#define   u4ReserveFD								SD_uchar_8(OEMRAMA+0xFD)
#define   u4ReserveFE								SD_uchar_8(OEMRAMA+0xFE)
#define   u4ReserveFF								SD_uchar_8(OEMRAMA+0xFF)

//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM B  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define   rDebugData								SD_Ptr_uchar_8(OEMRAMB+0x00)
#define   rDebugIndex								SD_uchar_8(OEMRAMB+0xFF)

//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM C  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define    UCSI_RAM0								SD_uint_16(OEMRAMC+0x00)
#define    UCSI_RAM1								SD_uint_16(OEMRAMC+0x02)
#define    UCSI_RAM2								SD_Ptr_uchar_8(OEMRAMC+0x04)  // size 4 bytes
#define    UCSI_RAM3								SD_Ptr_uchar_8(OEMRAMC+0x08)  // size 8 bytes
#define    UCSI_RAM4								SD_Ptr_uchar_8(OEMRAMC+0x10)  // size 16 bytes
#define    UCSI_RAM5								SD_Ptr_uchar_8(OEMRAMC+0x20)  // size 16 bytes
#define    Usb_Pdc_Power_Status						(*((USB_POWER_STATUS *)(OEMRAMC+0x30)))
#define    Usb_Pdc_Status							(*((USB_STATUS *)(OEMRAMC+0x31)))
#define    UCSI_RDO1								SD_uchar_8(OEMRAMC+0x32)
#define    UCSI_RDO2								SD_uchar_8(OEMRAMC+0x33)
#define    UCSI_RDO3								SD_uchar_8(OEMRAMC+0x34)
#define    UCSI_RDO4								SD_uchar_8(OEMRAMC+0x35)
#define    UCSI_CONNECTOR_STATE						SD_uchar_8(OEMRAMC+0x36)
#define    UCSI_POWER_SOURCE						SD_uchar_8(OEMRAMC+0x37)
#define    UCSI_POWER_DATA_SOURCE_REJECT			SD_uchar_8(OEMRAMC+0x38)
#define    UCSI_POWER_ROLE_SWAP_COMPLETE			SD_uchar_8(OEMRAMC+0x39)

#define    UCSI_PDOs_Buffer   		                SD_Ptr_uchar_8(OEMRAMC+0x40)
#define    SMB3_DATA_TEMP							SD_Ptr_uchar_8(OEMRAMC+0x60)  // 0xB0~0xCF // size 32 bytes
#define    SMB3_DATA_TEMP1							SD_Ptr_uchar_8(OEMRAMC+0x80)  // 0xB0~0xCF // size 32 bytes
#define    UCSI_NEG_POWER_LEVEL_CHANGE				SD_uchar_8(OEMRAMC+0xA0)
#define    UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE				SD_uchar_8(OEMRAMC+0xA1)
#define    UCSI_POWER_DIRECTION_CHANGE				SD_uchar_8(OEMRAMC+0xA2)
#define    UCSI_CONNECTOR_PARTNER_CHANGE			SD_uchar_8(OEMRAMC+0xA3)
#define    UCSI_PDC_REQ_RETRY				        SD_uchar_8(OEMRAMC+0xA4)
#define    UCSI_4CC_CMD_COMPLETE				    SD_uchar_8(OEMRAMC+0xA5)
#define    UCSI_4CC_CMD_RETRY				        SD_uchar_8(OEMRAMC+0xA6)
#define    UCSI_DATA_ROLE_SWAP_COMPLETE				SD_uchar_8(OEMRAMC+0xA7)
#define    UCSI_CONNECTOR_RESET_WAIT				SD_uchar_8(OEMRAMC+0xA8)
#define    UCSI_CONNECTOR_RESET_WAIT_CNT			SD_uint_16(OEMRAMC+0xAA)




//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM D  
//-------------------------------------------------------------------------------
//*******************************************************************************
#define    KB_LOC_Table								SD_Ptr_uchar_8(OEMRAMD+0x00)  // 128Bytes 

#define    BAT_unlock_ship_mode_CNT				    SD_uchar_8(OEMRAMD+0xF0)
#define    BAT_ship_mode_CNT				           SD_uchar_8(OEMRAMD+0xF1)

//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM E  
//-------------------------------------------------------------------------------
//*******************************************************************************
/////////////////////////////////////////////////////////////////////////////
/***********   EDGE Customer RAM for Battery  ****************/
#define  TWR00										SD_uchar_8(OEMRAME+0x00) // EDGE_BATTERY_INFO+0x00
//#define TWR00       EDGE_BATTERY_INFO[0x00]
 #define  TWR_BIT0                    	BIT0
 #define  TWR_BIT1                    	BIT1
 #define  TWR_BIT2                     	BIT2
 #define  AttachedPowerNOTsupport     	BIT3
 #define  AuthenticationFailed        	BIT4
 #define  SecondaryBattAttached       	BIT5
 #define  MainBattAttached            	BIT6
 #define  ACattached                  	BIT7
 #define  AttachedPowerNOTsupportEn   	SET_MASK(TWR00, AttachedPowerNOTsupport)
 #define  AttachedPowerNOTsupportDis  	CLEAR_MASK(TWR00, AttachedPowerNOTsupport)
 #define  AttachedPowerNOTsupportGET  	IS_MASK_SET(TWR00, AttachedPowerNOTsupport)
 #define  AuthenticationFailedEn      	SET_MASK(TWR00, AuthenticationFailed)
 #define  AuthenticationFailedDis     	CLEAR_MASK(TWR00, AuthenticationFailed)
 #define  AuthenticationFailedGET     	IS_MASK_SET(TWR00, AuthenticationFailed)
 #define  SecondaryBattAttachedEn     	SET_MASK(TWR00, SecondaryBattAttached)
 #define  SecondaryBattAttachedDis    	CLEAR_MASK(TWR00, SecondaryBattAttached)
 #define  SecondaryBattAttachedGET    	IS_MASK_SET(TWR00, SecondaryBattAttached)
 #define  MainBattAttachedEn          	SET_MASK(TWR00, MainBattAttached)
 #define  MainBattAttachedDis         	CLEAR_MASK(TWR00, MainBattAttached)
 #define  MainBattAttachedGET         	IS_MASK_SET(TWR00, MainBattAttached)
 #define  ACattachedEn                	SET_MASK(TWR00, ACattached)
 #define  ACattachedDis               	CLEAR_MASK(TWR00, ACattached)
 #define  ACattachedGET               	IS_MASK_SET(TWR00, ACattached)
 
#define   TWR01										SD_uchar_8(OEMRAME+0x01)  // EDGE_BATTERY_INFO+0x01
//#define TWR01       EDGE_BATTERY_INFO[0x01]
 #define  BatteryCritical         		BIT0
 #define  Error                   		BIT1
 #define  CapacityMode            		BIT2
 #define  CriticalLow             		BIT3
 #define  Discharge               		BIT4
 #define  Charge                  		BIT5
 #define  Commestablished         		BIT6
 #define  TWRvalid                		BIT7
 #define  BatteryCriticalEn       		SET_MASK(TWR01, BatteryCritical)
 #define  BatteryCriticalDis      		SET_MASK(TWR01, BatteryCritical)
 #define  BatteryCriticalGET      		IS_MASK_SET(TWR01, BatteryCritical)
 #define  ErrorEn                 		SET_MASK(TWR01, Error)
 #define  ErrorDis                		CLEAR_MASK(TWR01, Error)
 #define  ErrorGET                		IS_MASK_SET(TWR01, Error)
 #define  CapacityModeEn          		SET_MASK(TWR01, CapacityMode)
 #define  CapacityModeDis         		SET_MASK(TWR01, CapacityMode)
 #define  CapacityModeGET         		IS_MASK_SET(TWR01, CapacityMode)
 #define  CriticalLowEn           		SET_MASK(TWR01, CriticalLow)
 #define  CriticalLowDis          		CLEAR_MASK(TWR01, CriticalLow)
 #define  CriticalLowGET          		IS_MASK_SET(TWR01, CriticalLow)
 #define  CommestablishedEn       		SET_MASK(TWR01, Commestablished)
 #define  CommestablishedDis      		CLEAR_MASK(TWR01, Commestablished)
 #define  CommestablishedGET      		IS_MASK_SET(TWR01, Commestablished)
 #define  TWRvalidEn              		SET_MASK(TWR01, TWRvalid)
 #define  TWRvalidDis             		SET_MASK(TWR01, TWRvalid)
 #define  TWRvalidGET             		IS_MASK_SET(TWR01, TWRvalid)
 
#define   MajorVer									SD_uchar_8(OEMRAME+0x02)  // EDGE_BATTERY_INFO+0x02
//#define MajorVer    EDGE_BATTERY_INFO[0x02]
#define   MinorVer									SD_uchar_8(OEMRAME+0x03) // EDGE_BATTERY_INFO+0x03
//#define MinorVer    EDGE_BATTERY_INFO[0x03]
#define   TWRInfo									SD_uchar_8(OEMRAME+0x04)  // EDGE_BATTERY_INFO+0x04
//#define TWRInfo     EDGE_BATTERY_INFO[0x04]
#define   AdpWatt									SD_uchar_8(OEMRAME+0x05)  // EDGE_BATTERY_INFO+0x05
////#define AdpWatt     EDGE_BATTERY_INFO[0x05]
#define   TWR06										SD_uchar_8(OEMRAME+0x06)  // EDGE_BATTERY_INFO+0x06
//#define TWR06       EDGE_BATTERY_INFO[0x06]
 #define  TWR06Reserve_BIT0    BIT0
 #define  TWR06Reserve_BIT1    BIT1
 #define  TWR06Reserve_BIT2    BIT2
 #define  TWR06Reserve_BIT3    BIT3
 #define  HWauthSupport        BIT4
 #define  TWR06Reserve_BIT5    BIT5
 #define  TWR06Reserve_BIT6    BIT6
 #define  TWR06Reserve_BIT7    BIT7
#define   u3Reserve07								SD_uchar_8(OEMRAME+0x07) // EDGE_BATTERY_INFO+0x07
//#define u3Reserve07 EDGE_BATTERY_INFO[0x07]
#define   u3Reserve08								SD_uchar_8(OEMRAME+0x08)  // EDGE_BATTERY_INFO+0x08
#define   u3Reserve09								SD_uchar_8(OEMRAME+0x09) // EDGE_BATTERY_INFO+0x09
#define   u3Reserve0A								SD_uchar_8(OEMRAME+0x0A)  // EDGE_BATTERY_INFO+0x0A
#define   u3Reserve0B								SD_uchar_8(OEMRAME+0x0B)  // EDGE_BATTERY_INFO+0x0B
#define   u3Reserve0C								SD_uchar_8(OEMRAME+0x0C) // EDGE_BATTERY_INFO+0x0C
#define   u3Reserve0D								SD_uchar_8(OEMRAME+0x0D)  // EDGE_BATTERY_INFO+0x0D
#define   u3Reserve0E								SD_uchar_8(OEMRAME+0x0E) // EDGE_BATTERY_INFO+0x0E
#define   u3Reserve0F								SD_uchar_8(OEMRAME+0x0F) // EDGE_BATTERY_INFO+0x0F
#define   TWR10										SD_uchar_8(OEMRAME+0x10)
//#define TWR10   EDGE_BATTERY_INFO[0x10]
#define   TWR11										SD_uchar_8(OEMRAME+0x11)
//#define TWR11   EDGE_BATTERY_INFO[0x11]
#define   BattModeL									SD_uchar_8(OEMRAME+0x12)
//#define BattModeL   EDGE_BATTERY_INFO[0x12]
#define   BattModeH									SD_uchar_8(OEMRAME+0x13)
//#define BattModeH   EDGE_BATTERY_INFO[0x13]
#define   TemperatureL								SD_uchar_8(OEMRAME+0x14)
#define   TemperatureH								SD_uchar_8(OEMRAME+0x15)  // EDGE_BATTERY_INFO+0x15
#define   VoltageL									SD_uchar_8(OEMRAME+0x16)  // EDGE_BATTERY_INFO+0x16
#define   VoltageH									SD_uchar_8(OEMRAME+0x17) // EDGE_BATTERY_INFO+0x17
#define   CurrentL									SD_uchar_8(OEMRAME+0x18) // EDGE_BATTERY_INFO+0x18
#define   CurrentH									SD_uchar_8(OEMRAME+0x19)  // EDGE_BATTERY_INFO+0x19
#define   AvgCurrentL								SD_uchar_8(OEMRAME+0x1A)  // EDGE_BATTERY_INFO+0x1A
#define   AvgCurrentH								SD_uchar_8(OEMRAME+0x1B) // EDGE_BATTERY_INFO+0x1B
#define   RelativeStateOfChgL						SD_uchar_8(OEMRAME+0x1C) // EDGE_BATTERY_INFO+0x1C
#define   RelativeStateOfChgH						SD_uchar_8(OEMRAME+0x1D)  // EDGE_BATTERY_INFO+0x1D
#define   RemainCapL								SD_uchar_8(OEMRAME+0x1E)  // EDGE_BATTERY_INFO+0x1E
#define   RemainCapH								SD_uchar_8(OEMRAME+0x1F)  // EDGE_BATTERY_INFO+0x1F 
#define   TWR20										SD_uchar_8(OEMRAME+0x20) // EDGE_BATTERY_INFO+0x20
//#define TWR20   EDGE_BATTERY_INFO[0x20]
#define   TWR21										SD_uchar_8(OEMRAME+0x21) // EDGE_BATTERY_INFO+0x21
//#define TWR21   EDGE_BATTERY_INFO[0x21]
#define   FullChgCapL								SD_uchar_8(OEMRAME+0x22) // EDGE_BATTERY_INFO+0x22
#define   FullChgCapH								SD_uchar_8(OEMRAME+0x23) // EDGE_BATTERY_INFO+0x23
#define   RunTimeToEmptyL							SD_uchar_8(OEMRAME+0x24) // EDGE_BATTERY_INFO+0x24
#define   RunTimeToEmptyH							SD_uchar_8(OEMRAME+0x25) // EDGE_BATTERY_INFO+0x25
#define   AvgTimeToEmptyL							SD_uchar_8(OEMRAME+0x26) // EDGE_BATTERY_INFO+0x26
#define   AvgTimeToEmptyH							SD_uchar_8(OEMRAME+0x27) // EDGE_BATTERY_INFO+0x27
#define   AvgTimeToFullL							SD_uchar_8(OEMRAME+0x28) // EDGE_BATTERY_INFO+0x28
#define   AvgTimeToFullH							SD_uchar_8(OEMRAME+0x29) // EDGE_BATTERY_INFO+0x29
#define   BattStatusL								SD_uchar_8(OEMRAME+0x2A) // EDGE_BATTERY_INFO+0x2A  
 #define SMBerror0    					BIT0          // error code bit0
 #define SMBerror1    					BIT1          // error code bit1
 #define SMBerror2    					BIT2          // error code bit2
 #define SMBerror3    					BIT3          // error code bit3
 #define FullyDsg     					BIT4          // statusfully discharged
 #define FullyChg     					BIT5          // statusfully charged
 #define Dsg          					BIT6          // statusdischarging
 #define Initialized  					BIT7          // statusgauge initialized
#define   BattStatusH								SD_uchar_8(OEMRAME+0x2B) // EDGE_BATTERY_INFO+0x2B
#define   CycleCountL								SD_uchar_8(OEMRAME+0x2C) // EDGE_BATTERY_INFO+0x2C
#define   CycleCountH								SD_uchar_8(OEMRAME+0x2D) // EDGE_BATTERY_INFO+0x2D
#define   FETTemperatureL							SD_uchar_8(OEMRAME+0x2E) // EDGE_BATTERY_INFO+0x2E
#define   FETTemperatureH							SD_uchar_8(OEMRAME+0x2F) // EDGE_BATTERY_INFO+0x2F
#define   TWR30										SD_uchar_8(OEMRAME+0x30) // EDGE_BATTERY_INFO+0x30
//#define TWR30   EDGE_BATTERY_INFO[0x30]
#define   TWR31										SD_uchar_8(OEMRAME+0x31) // EDGE_BATTERY_INFO+0x31
//#define TWR31   EDGE_BATTERY_INFO[0x31]
#define   DesignCapL								SD_uchar_8(OEMRAME+0x32) // EDGE_BATTERY_INFO+0x32
#define   DesignCapH								SD_uchar_8(OEMRAME+0x33) // EDGE_BATTERY_INFO+0x33
#define   DesignVoltL								SD_uchar_8(OEMRAME+0x34) // EDGE_BATTERY_INFO+0x34
#define   DesignVoltH								SD_uchar_8(OEMRAME+0x35) // EDGE_BATTERY_INFO+0x35
#define   SpecInfoL									SD_uchar_8(OEMRAME+0x36) // EDGE_BATTERY_INFO+0x36
#define   SpecInfoH									SD_uchar_8(OEMRAME+0x37) // EDGE_BATTERY_INFO+0x37
#define   ManufactureDateL							SD_uchar_8(OEMRAME+0x38) // EDGE_BATTERY_INFO+0x38
#define   ManufactureDateH							SD_uchar_8(OEMRAME+0x39) // EDGE_BATTERY_INFO+0x39
#define   SerialNumberL								SD_uchar_8(OEMRAME+0x3A) // EDGE_BATTERY_INFO+0x3A
#define   SerialNumberH								SD_uchar_8(OEMRAME+0x3B) // EDGE_BATTERY_INFO+0x3B
#define   WarrantyPeriodL							SD_uchar_8(OEMRAME+0x3C) // EDGE_BATTERY_INFO+0x3C
#define   WarrantyPeriodH							SD_uchar_8(OEMRAME+0x3D) // EDGE_BATTERY_INFO+0x3D
#define   u3Reserve2E								SD_uchar_8(OEMRAME+0x3E) // EDGE_BATTERY_INFO+0x3E
#define   u3Reserve2F								SD_uchar_8(OEMRAME+0x3F) // EDGE_BATTERY_INFO+0x3F
#define   TWR40										SD_uchar_8(OEMRAME+0x40) // EDGE_BATTERY_INFO+0x40
//#define TWR40   EDGE_BATTERY_INFO[0x40]
#define   TWR41										SD_uchar_8(OEMRAME+0x41) // EDGE_BATTERY_INFO+0x41
//#define TWR41   EDGE_BATTERY_INFO[0x41]
#define   ManufacturerName							SD_Ptr_uchar_8(OEMRAME+0x42) // EDGE_BATTERY_INFO+0x42
#define   TWR50										SD_uchar_8(OEMRAME+0x50) // EDGE_BATTERY_INFO+0x50
//#define TWR50   EDGE_BATTERY_INFO[0x50]
#define   TWR51										SD_uchar_8(OEMRAME+0x51) // EDGE_BATTERY_INFO+0x51
//#define TWR51   EDGE_BATTERY_INFO[0x51]
#define   DeviceName								SD_Ptr_uchar_8(OEMRAME+0x52) // EDGE_BATTERY_INFO+0x52

#define   TWR60										SD_uchar_8(OEMRAME+0x60) // EDGE_BATTERY_INFO+0x60
//#define TWR60   EDGE_BATTERY_INFO[0x60]
#define   TWR61										SD_uchar_8(OEMRAME+0x61)  // EDGE_BATTERY_INFO+0x61
//#define TWR61   EDGE_BATTERY_INFO[0x61]
#define   DeviceChemistry							SD_Ptr_uchar_8(OEMRAME+0x62) // EDGE_BATTERY_INFO+0x62

#define   TWR70										SD_uchar_8(OEMRAME+0x070) // EDGE_BATTERY_INFO+0x70
//#define TWR70   EDGE_BATTERY_INFO[0x70]
#define   TWR71										SD_uchar_8(OEMRAME+0x71)  // EDGE_BATTERY_INFO+0x71
//#define TWR71   EDGE_BATTERY_INFO[0x71]
#define   OptMfgFunction5							SD_Ptr_uchar_8(OEMRAME+0x72) //_at_ OEMRAME+0x72;    // EDGE_BATTERY_INFO+0x72
#define   TWR80										SD_uchar_8(OEMRAME+0x80) // EDGE_BATTERY_INFO+0x80
//#define TWR80   EDGE_BATTERY_INFO[0x80]
#define   TWR81										SD_uchar_8(OEMRAME+0x81) // EDGE_BATTERY_INFO+0x81
//#define TWR81   EDGE_BATTERY_INFO[0x81]

#define   OptMfgFunction1L							SD_uchar_8(OEMRAME+0x82) // EDGE_BATTERY_INFO+0x82
#define   OptMfgFunction1H							SD_uchar_8(OEMRAME+0x83) // EDGE_BATTERY_INFO+0x83
#define   OptMfgFunction2L							SD_uchar_8(OEMRAME+0x84) // EDGE_BATTERY_INFO+0x84
 #define  DUAL_MODE_CAPABILITY  		BIT5
 #define  DUAL_MODE_SELECT      		BIT6
// OptMfgFunction2L
// BIT0: CAPACITY_CORRECTION  1: enable 0: disable
// BIT1-2: Preservation (Open for system use)
// BIT3: EXPRESS_CHARGE
// BIT4: CLEAN_SHEET2       1: enable 0: disable
// BIT5: Dual Mode Capability (0: No Dual Mode Capability, 1:Dual Mode Capability, Read Only)
// BIT6: Dual Mode Select(0: Long runtime mode(Default), 1:Long Lifespan mode, 
// This bit must be kept until next write operation and must not be reset at OFF 
// State)
//
// Bit7: Non-replaceable battery
// 0 : Replaceable battery
// 1 : Non-replaceable battery
#define   OptMfgFunction2H							SD_uchar_8(OEMRAME+0x85) // EDGE_BATTERY_INFO+0x85
 #define COMMON_DUAL_MODE    			BIT0
// OptMfgFunction2H
// BIT0 : Common Dual Mode, 0 : No Common Dual Mode Capability, 1 : Common Dual Mode Capability)
// 
#define   FirmwareUpdateStatusL						SD_uchar_8(OEMRAME+0x86) // EDGE_BATTERY_INFO+0x86
#define   FirmwareUpdateStatusH						SD_uchar_8(OEMRAME+0x87) // EDGE_BATTERY_INFO+0x87
#define   BatterymakerIDL							SD_uchar_8(OEMRAME+0x88) // EDGE_BATTERY_INFO+0x88
#define   BatterymakerIDH							SD_uchar_8(OEMRAME+0x89) // EDGE_BATTERY_INFO+0x89
#define   HardwareIDL								SD_uchar_8(OEMRAME+0x8A) // EDGE_BATTERY_INFO+0x8A
#define   HardwareIDH								SD_uchar_8(OEMRAME+0x8B) // EDGE_BATTERY_INFO+0x8B
#define   FirmwareversionL							SD_uchar_8(OEMRAME+0x8C) // EDGE_BATTERY_INFO+0x8C
#define   FirmwareversionH							SD_uchar_8(OEMRAME+0x8D) // EDGE_BATTERY_INFO+0x8D
#define   DataVersionL								SD_uchar_8(OEMRAME+0x8E) // EDGE_BATTERY_INFO+0x8E
#define   DataVersionH								SD_uchar_8(OEMRAME+0x8F)  // EDGE_BATTERY_INFO+0x8F
#define   TWR90										SD_uchar_8(OEMRAME+0x90) // EDGE_BATTERY_INFO+0x90
//#define TWR90   EDGE_BATTERY_INFO[0x90]
#define   TWR91										SD_uchar_8(OEMRAME+0x91) // EDGE_BATTERY_INFO+0x91
//#define TWR91   EDGE_BATTERY_INFO[0x91]

#define   ManufactureAccessL						SD_uchar_8(OEMRAME+0x92) // EDGE_BATTERY_INFO+0x92
#define   ManufactureAccessH						SD_uchar_8(OEMRAME+0x93) // EDGE_BATTERY_INFO+0x93
#define   MaxErroL									SD_uchar_8(OEMRAME+0x94) // EDGE_BATTERY_INFO+0x94
#define   MaxErroH									SD_uchar_8(OEMRAME+0x95) // EDGE_BATTERY_INFO+0x95
#define   ChargeCurrL								SD_uchar_8(OEMRAME+0x96) // EDGE_BATTERY_INFO+0x96
#define   ChargeCurrH								SD_uchar_8(OEMRAME+0x97) // EDGE_BATTERY_INFO+0x97
#define   ChargeVoltL								SD_uchar_8(OEMRAME+0x98) // EDGE_BATTERY_INFO+0x98
#define   ChargeVoltH								SD_uchar_8(OEMRAME+0x99)  // EDGE_BATTERY_INFO+0x99
#define   u3Reserve9A								SD_uchar_8(OEMRAME+0x9A) // EDGE_BATTERY_INFO+0x9A
#define   u3Reserve9B								SD_uchar_8(OEMRAME+0x9B) // EDGE_BATTERY_INFO+0x9B
#define   u3Reserve9C								SD_uchar_8(OEMRAME+0x9C) // EDGE_BATTERY_INFO+0x9C
#define   u3Reserve9D								SD_uchar_8(OEMRAME+0x9D) // EDGE_BATTERY_INFO+0x9D
#define   u3Reserve9E								SD_uchar_8(OEMRAME+0x9E) // EDGE_BATTERY_INFO+0x9E
#define   u3Reserve9F								SD_uchar_8(OEMRAME+0x9F) // EDGE_BATTERY_INFO+0x9F
#define   TWRA0										SD_uchar_8(OEMRAME+0xA0) // EDGE_BATTERY_INFO+0xA0
//#define TWRA0   EDGE_BATTERY_INFO[0xA0]
#define   TWRA1										SD_uchar_8(OEMRAME+0xA1) // EDGE_BATTERY_INFO+0xA1
//#define TWRA1   EDGE_BATTERY_INFO[0xA1]
#define   ManufactureData							SD_Ptr_uchar_8(OEMRAME+0xA2)  // EDGE_BATTERY_INFO+0xA2
#define   Major										SD_uchar_8(OEMRAME+0xAE) // EDGE_BATTERY_INFO+0xAE
#define   Minor										SD_uchar_8(OEMRAME+0xAF) // EDGE_BATTERY_INFO+0xAF
#define   TWRB0										SD_uchar_8(OEMRAME+0xB0) // EDGE_BATTERY_INFO+0xB0
//#define TWRB0   EDGE_BATTERY_INFO[0xB0]
#define   TWRB1										SD_uchar_8(OEMRAME+0xB1) // EDGE_BATTERY_INFO+0xB1
//#define TWRB1   EDGE_BATTERY_INFO[0xB1]
#define   BattUsageCondition						SD_Ptr_uchar_8(OEMRAME+0xB2) //_at_ OEMRAME+0xB2;    // EDGE_BATTERY_INFO+0xB2
#define   u3ReserveBC								SD_uchar_8(OEMRAME+0xBC) // EDGE_BATTERY_INFO+0xBC
#define   u3ReserveBD								SD_uchar_8(OEMRAME+0xBD) // EDGE_BATTERY_INFO+0xBD
#define   u3ReserveBE								SD_uchar_8(OEMRAME+0xBE) // EDGE_BATTERY_INFO+0xBE
#define   u3ReserveBF								SD_uchar_8(OEMRAME+0xBF)  // EDGE_BATTERY_INFO+0xBF
#define   TWRC0										SD_uchar_8(OEMRAME+0xC0) // EDGE_BATTERY_INFO+0xC0
//#define TWRC0   EDGE_BATTERY_INFO[0xC0]
#define   TWRC1										SD_uchar_8(OEMRAME+0xC1) // EDGE_BATTERY_INFO+0xC1
//#define TWRC1   EDGE_BATTERY_INFO[0xC1]
#define   BattAuthStat								SD_uchar_8(OEMRAME+0xC2) // EDGE_BATTERY_INFO+0xC2
//#define BattAuthStat    EDGE_BATTERY_INFO[0xC2]
 #define  TWRC2Reserve_BIT0    BIT0
 #define  TWRC2Reserve_BIT1    BIT1
 #define  TWRC2Reserve_BIT2    BIT2
 #define  TWRC2Reserve_BIT3    BIT3
 #define  TWRC2Reserve_BIT4    BIT4
 #define  HWauthFail          BIT5
 #define  OEMauthPass          BIT6
 #define  SWauthPass          BIT7
#define   u3ReserveC3								SD_uchar_8(OEMRAME+0xC3) // EDGE_BATTERY_INFO+0xC3
#define   u3ReserveC4								SD_uchar_8(OEMRAME+0xC4) // EDGE_BATTERY_INFO+0xC4
#define   u3ReserveC5								SD_uchar_8(OEMRAME+0xC5) // EDGE_BATTERY_INFO+0xC5
#define   u3ReserveC6								SD_uchar_8(OEMRAME+0xC6) // EDGE_BATTERY_INFO+0xC6
#define   u3ReserveC7								SD_uchar_8(OEMRAME+0xC7) // EDGE_BATTERY_INFO+0xC7
#define   u3ReserveC8								SD_uchar_8(OEMRAME+0xC8) // EDGE_BATTERY_INFO+0xC8
#define   u3ReserveC9								SD_uchar_8(OEMRAME+0xC9) // EDGE_BATTERY_INFO+0xC9
#define   u3ReserveCA								SD_uchar_8(OEMRAME+0xCA) // EDGE_BATTERY_INFO+0xCA
#define   u3ReserveCB								SD_uchar_8(OEMRAME+0xCB) // EDGE_BATTERY_INFO+0xCB
#define   u3ReserveCC								SD_uchar_8(OEMRAME+0xCC) // EDGE_BATTERY_INFO+0xCC
#define   u3ReserveCD								SD_uchar_8(OEMRAME+0xCD) // EDGE_BATTERY_INFO+0xCD
#define   u3ReserveCE								SD_uchar_8(OEMRAME+0xCE) // EDGE_BATTERY_INFO+0xCE
#define   u3ReserveCF								SD_uchar_8(OEMRAME+0xCF) // EDGE_BATTERY_INFO+0xCF
#define   u3ReserveD0								SD_uchar_8(OEMRAME+0xD0) // EDGE_BATTERY_INFO+0xD0
#define   u3ReserveD1								SD_uchar_8(OEMRAME+0xD1) // EDGE_BATTERY_INFO+0xD1
#define   u3ReserveD2								SD_uchar_8(OEMRAME+0xD2) // EDGE_BATTERY_INFO+0xD2
#define   u3ReserveD3								SD_uchar_8(OEMRAME+0xD3)  // EDGE_BATTERY_INFO+0xD3
#define   u3ReserveD4								SD_uchar_8(OEMRAME+0xD4) // EDGE_BATTERY_INFO+0xD4
#define   u3ReserveD5								SD_uchar_8(OEMRAME+0xD5) // EDGE_BATTERY_INFO+0xD5
#define   u3ReserveD6								SD_uchar_8(OEMRAME+0xD6) // EDGE_BATTERY_INFO+0xD6
#define   u3ReserveD7								SD_uchar_8(OEMRAME+0xD7) // EDGE_BATTERY_INFO+0xD7
#define   u3ReserveD8								SD_uchar_8(OEMRAME+0xD8) // EDGE_BATTERY_INFO+0xD8
#define   u3ReserveD9								SD_uchar_8(OEMRAME+0xD9) // EDGE_BATTERY_INFO+0xD9
#define   u3ReserveDA								SD_uchar_8(OEMRAME+0xDA) // EDGE_BATTERY_INFO+0xDA
#define   u3ReserveDB								SD_uchar_8(OEMRAME+0xDB) // EDGE_BATTERY_INFO+0xDB
#define   u3ReserveDC								SD_uchar_8(OEMRAME+0xDC) // EDGE_BATTERY_INFO+0xDC
#define   u3ReserveDD								SD_uchar_8(OEMRAME+0xDD) // EDGE_BATTERY_INFO+0xDD
#define   u3ReserveDE								SD_uchar_8(OEMRAME+0xDE) // EDGE_BATTERY_INFO+0xDE
#define   u3ReserveDF								SD_uchar_8(OEMRAME+0xDF) // EDGE_BATTERY_INFO+0xDF
#define   u3ReserveE0								SD_uchar_8(OEMRAME+0xE0) // EDGE_BATTERY_INFO+0xE0
#define   u3ReserveE1								SD_uchar_8(OEMRAME+0xE1) // EDGE_BATTERY_INFO+0xE1
#define   u3ReserveE2								SD_uchar_8(OEMRAME+0xE2) // EDGE_BATTERY_INFO+0xE2
#define   u3ReserveE3								SD_uchar_8(OEMRAME+0xE3) // EDGE_BATTERY_INFO+0xE3
#define   u3ReserveE4								SD_uchar_8(OEMRAME+0xE4) // EDGE_BATTERY_INFO+0xE4
#define   u3ReserveE5								SD_uchar_8(OEMRAME+0xE5) // EDGE_BATTERY_INFO+0xE5
#define   u3ReserveE6								SD_uchar_8(OEMRAME+0xE6) // EDGE_BATTERY_INFO+0xE6
#define   u3ReserveE7								SD_uchar_8(OEMRAME+0xE7) // EDGE_BATTERY_INFO+0xE7
#define   u3ReserveE8								SD_uchar_8(OEMRAME+0xE8) // EDGE_BATTERY_INFO+0xE8
#define   u3ReserveE9								SD_uchar_8(OEMRAME+0xE9) // EDGE_BATTERY_INFO+0xE9
#define   u3ReserveEA								SD_uchar_8(OEMRAME+0xEA) // EDGE_BATTERY_INFO+0xEA
#define   u3ReserveEB								SD_uchar_8(OEMRAME+0xEB) // EDGE_BATTERY_INFO+0xEB
#define   u3ReserveEC								SD_uchar_8(OEMRAME+0xEC) // EDGE_BATTERY_INFO+0xEC
#define   u3ReserveED								SD_uchar_8(OEMRAME+0xED) // EDGE_BATTERY_INFO+0xED
#define   u3ReserveEE								SD_uchar_8(OEMRAME+0xEE) // EDGE_BATTERY_INFO+0xEE
#define   u3ReserveEF								SD_uchar_8(OEMRAME+0xEF) // EDGE_BATTERY_INFO+0xEF
#define   u3ReserveF0								SD_uchar_8(OEMRAME+0xF0) // EDGE_BATTERY_INFO+0xF0
#define   u3ReserveF1								SD_uchar_8(OEMRAME+0xF1) // EDGE_BATTERY_INFO+0xF1
#define   u3ReserveF2								SD_uchar_8(OEMRAME+0xF2) // EDGE_BATTERY_INFO+0xF2
#define   u3ReserveF3								SD_uchar_8(OEMRAME+0xF3) // EDGE_BATTERY_INFO+0xF3
#define   u3ReserveF4								SD_uchar_8(OEMRAME+0xF4) // EDGE_BATTERY_INFO+0xF4
#define   u3ReserveF5								SD_uchar_8(OEMRAME+0xF5) // EDGE_BATTERY_INFO+0xF5
#define   u3ReserveF6								SD_uchar_8(OEMRAME+0xF6) // EDGE_BATTERY_INFO+0xF6
#define   u3ReserveF7								SD_uchar_8(OEMRAME+0xF7) // EDGE_BATTERY_INFO+0xF7
#define   u3ReserveF8								SD_uchar_8(OEMRAME+0xF8) // EDGE_BATTERY_INFO+0xF8
#define   u3ReserveF9								SD_uchar_8(OEMRAME+0xF9) // EDGE_BATTERY_INFO+0xF9
#define   u3ReserveFA								SD_uchar_8(OEMRAME+0xFA) // EDGE_BATTERY_INFO+0xFA
#define   u3ReserveFB								SD_uchar_8(OEMRAME+0xFB) // EDGE_BATTERY_INFO+0xFB
#define   u3ReserveFC								SD_uchar_8(OEMRAME+0xFC) // EDGE_BATTERY_INFO+0xFC
#define   u3ReserveFD								SD_uchar_8(OEMRAME+0xFD) // EDGE_BATTERY_INFO+0xFD
#define   u3ReserveFE								SD_uchar_8(OEMRAME+0xFE) // EDGE_BATTERY_INFO+0xFE
#define   u3ReserveFF								SD_uchar_8(OEMRAME+0xFF) // EDGE_BATTERY_INFO+0xFF


//*******************************************************************************
//-------------------------------------------------------------------------------
//  OEM RAM F 
//-------------------------------------------------------------------------------
//*******************************************************************************
/***********  Customer RAM for 2nd Battery  ****************/                                              
#define  SecTWR00                       SD_uchar_8(OEMRAMF+0x00)  	// EDGE_BATTERY_INFO+0x00                                 #
#define  SecTWR01                       SD_uchar_8(OEMRAMF+0x01)  	// EDGE_BATTERY_INFO+0x01 
 #define  SecBatteryCritical         BIT0
 #define  SecError                   BIT1
 #define  SecCapacityMode            BIT2
 #define  SecCriticalLow             BIT3
 #define  SecDischarge               BIT4
 #define  SecCharge                  BIT5
 #define  SecCommestablished         BIT6
 #define  SecTWRvalid                BIT7
 #define  SecBatteryCriticalEn       SET_MASK(SecTWR01, SecBatteryCritical)
 #define  SecBatteryCriticalDis      CLEAR_MASK(SecTWR01, SecBatteryCritical)
 #define  SecBatteryCriticalGET      IS_MASK_SET(SecTWR01, SecBatteryCritical)
 #define  SecErrorEn                 SET_MASK(SecTWR01, SecError)
 #define  SecErrorDis                CLEAR_MASK(SecTWR01, SecError)
 #define  SecErrorGET                IS_MASK_SET(SecTWR01, SecError)
 #define  SecCapacityModeEn          SET_MASK(SecTWR01, SecCapacityMode)
 #define  SecCapacityModeDis         CLEAR_MASK(SecTWR01, SecCapacityMode)
 #define  SecCapacityModeGET         IS_MASK_SET(SecTWR01, SecCapacityMode)
 #define  SecCriticalLowEn           SET_MASK(SecTWR01, SecCriticalLow)
 #define  SecCriticalLowDis          CLEAR_MASK(SecTWR01, SecCriticalLow)
 #define  SecCriticalLowGET          IS_MASK_SET(SecTWR01, SecCriticalLow)
 #define  SecCommestablishedEn       SET_MASK(SecTWR01, SecCommestablished)
 #define  SecCommestablishedDis      CLEAR_MASK(SecTWR01, SecCommestablished)
 #define  SecCommestablishedGET      IS_MASK_SET(SecTWR01, SecCommestablished)
 #define  SecTWRvalidEn              SET_MASK(SecTWR01, SecTWRvalid)
 #define  SecTWRvalidDis             CLEAR_MASK(SecTWR01, SecTWRvalid)
 #define  SecTWRvalidGET             IS_MASK_SET(SecTWR01, SecTWRvalid)
#define  SecMajorVer                    SD_uchar_8(OEMRAMF+0x02)  	// EDGE_BATTERY_INFO+0x02                               
#define  SecMinorVer                    SD_uchar_8(OEMRAMF+0x03)  	// EDGE_BATTERY_INFO+0x03                               
#define  SecTWRInfo                     SD_uchar_8(OEMRAMF+0x04)  	// EDGE_BATTERY_INFO+0x04                               
#define  SecAdpWatt                     SD_uchar_8(OEMRAMF+0x05)  	// EDGE_BATTERY_INFO+0x05                               
#define  SecTWR06                       SD_uchar_8(OEMRAMF+0x06)  	// EDGE_BATTERY_INFO+0x06                               
#define  Secu3Reserve07                 SD_uchar_8(OEMRAMF+0x07)  	// EDGE_BATTERY_INFO+0x07                               
#define  Secu3Reserve08                 SD_uchar_8(OEMRAMF+0x08)  	// EDGE_BATTERY_INFO+0x08                                 
#define  Secu3Reserve09                 SD_uchar_8(OEMRAMF+0x09)  	// EDGE_BATTERY_INFO+0x09                                 
#define  Secu3Reserve0A                 SD_uchar_8(OEMRAMF+0x0A)  	// EDGE_BATTERY_INFO+0x0A                                 
#define  Secu3Reserve0B                 SD_uchar_8(OEMRAMF+0x0B)  	// EDGE_BATTERY_INFO+0x0B                                 
#define  Secu3Reserve0C                 SD_uchar_8(OEMRAMF+0x0C)  	// EDGE_BATTERY_INFO+0x0C                                 
#define  Secu3Reserve0D                 SD_uchar_8(OEMRAMF+0x0D)  	// EDGE_BATTERY_INFO+0x0D                                 
#define  Secu3Reserve0E                 SD_uchar_8(OEMRAMF+0x0E)  	// EDGE_BATTERY_INFO+0x0E                                 
#define  Secu3Reserve0F                 SD_uchar_8(OEMRAMF+0x0F)  	// EDGE_BATTERY_INFO+0x0F                                 
#define  SecTWR10                       SD_uchar_8(OEMRAMF+0x10)  	// EDGE_BATTERY_INFO+0x10                                 
#define  SecTWR11                       SD_uchar_8(OEMRAMF+0x11)  	// EDGE_BATTERY_INFO+0x11                                 
#define  SecBattModeL                   SD_uchar_8(OEMRAMF+0x12)  	// EDGE_BATTERY_INFO+0x12                                 
#define  SecBattModeH                   SD_uchar_8(OEMRAMF+0x13)  	// EDGE_BATTERY_INFO+0x13                                 
#define  SecTemperatureL                SD_uchar_8(OEMRAMF+0x14)  	// EDGE_BATTERY_INFO+0x14                                 
#define  SecTemperatureH                SD_uchar_8(OEMRAMF+0x15)  	// EDGE_BATTERY_INFO+0x15                                 
#define  SecVoltageL                    SD_uchar_8(OEMRAMF+0x16)  	// EDGE_BATTERY_INFO+0x16                                 
#define  SecVoltageH                    SD_uchar_8(OEMRAMF+0x17)  	// EDGE_BATTERY_INFO+0x17                                 
#define  SecCurrentL                    SD_uchar_8(OEMRAMF+0x18)  	// EDGE_BATTERY_INFO+0x18                                 
#define  SecCurrentH                    SD_uchar_8(OEMRAMF+0x19)  	// EDGE_BATTERY_INFO+0x19                                 
#define  SecAvgCurrentL                 SD_uchar_8(OEMRAMF+0x1A)  	// EDGE_BATTERY_INFO+0x1A                                 
#define  SecAvgCurrentH                 SD_uchar_8(OEMRAMF+0x1B)  	// EDGE_BATTERY_INFO+0x1B                                 
#define  SecRelativeStateOfChgL         SD_uchar_8(OEMRAMF+0x1C)  	// EDGE_BATTERY_INFO+0x1C                                 
#define  SecRelativeStateOfChgH         SD_uchar_8(OEMRAMF+0x1D)  	// EDGE_BATTERY_INFO+0x1D                                 
#define  SecRemainCapL                  SD_uchar_8(OEMRAMF+0x1E)  	// EDGE_BATTERY_INFO+0x1E                                 
#define  SecRemainCapH                  SD_uchar_8(OEMRAMF+0x1F)  	// EDGE_BATTERY_INFO+0x1F                                 
#define  SecTWR20                       SD_uchar_8(OEMRAMF+0x20)  	// EDGE_BATTERY_INFO+0x20                   
#define  SecTWR21                       SD_uchar_8(OEMRAMF+0x21)  	// EDGE_BATTERY_INFO+0x21                   
#define  SecFullChgCapL                 SD_uchar_8(OEMRAMF+0x22)  	// EDGE_BATTERY_INFO+0x22                   
#define  SecFullChgCapH                 SD_uchar_8(OEMRAMF+0x23)  	// EDGE_BATTERY_INFO+0x23                   
#define  SecRunTimeToEmptyL             SD_uchar_8(OEMRAMF+0x24)  	// EDGE_BATTERY_INFO+0x24                   
#define  SecRunTimeToEmptyH             SD_uchar_8(OEMRAMF+0x25)  	// EDGE_BATTERY_INFO+0x25                   
#define  SecAvgTimeToEmptyL             SD_uchar_8(OEMRAMF+0x26)  	// EDGE_BATTERY_INFO+0x26                   
#define  SecAvgTimeToEmptyH             SD_uchar_8(OEMRAMF+0x27)  	// EDGE_BATTERY_INFO+0x27                   
#define  SecAvgTimeToFullL              SD_uchar_8(OEMRAMF+0x28)  	// EDGE_BATTERY_INFO+0x28                   
#define  SecAvgTimeToFullH              SD_uchar_8(OEMRAMF+0x29)  	// EDGE_BATTERY_INFO+0x29                   
#define  SecBattStatusL                 SD_uchar_8(OEMRAMF+0x2A)  	// EDGE_BATTERY_INFO+0x2A                   
#define  SecBattStatusH                 SD_uchar_8(OEMRAMF+0x2B)  	// EDGE_BATTERY_INFO+0x2B                   
#define  SecCycleCountL                 SD_uchar_8(OEMRAMF+0x2C)  	// EDGE_BATTERY_INFO+0x2C                   
#define  SecCycleCountH                 SD_uchar_8(OEMRAMF+0x2D)  	// EDGE_BATTERY_INFO+0x2D                   
#define  SecFETTemperatureL             SD_uchar_8(OEMRAMF+0x2E)  	// EDGE_BATTERY_INFO+0x2E                   
#define  SecFETTemperatureH             SD_uchar_8(OEMRAMF+0x2F)  	// EDGE_BATTERY_INFO+0x2F                   
#define  SecTWR30                       SD_uchar_8(OEMRAMF+0x30)  	// EDGE_BATTERY_INFO+0x30                   
#define  SecTWR31                       SD_uchar_8(OEMRAMF+0x31)  	// EDGE_BATTERY_INFO+0x31                   
#define  SecDesignCapL                  SD_uchar_8(OEMRAMF+0x32)  	// EDGE_BATTERY_INFO+0x32                                 
#define  SecDesignCapH                  SD_uchar_8(OEMRAMF+0x33)  	// EDGE_BATTERY_INFO+0x33                                 
#define  SecDesignVoltL                 SD_uchar_8(OEMRAMF+0x34)  	// EDGE_BATTERY_INFO+0x34                                 
#define  SecDesignVoltH                 SD_uchar_8(OEMRAMF+0x35)  	// EDGE_BATTERY_INFO+0x35                 
#define  SecSpecInfoL                   SD_uchar_8(OEMRAMF+0x36)  	// EDGE_BATTERY_INFO+0x36                 
#define  SecSpecInfoH                   SD_uchar_8(OEMRAMF+0x37)  	// EDGE_BATTERY_INFO+0x37                 
#define  SecManufactureDateL            SD_uchar_8(OEMRAMF+0x38)  	// EDGE_BATTERY_INFO+0x38                 
#define  SecManufactureDateH            SD_uchar_8(OEMRAMF+0x39)  	// EDGE_BATTERY_INFO+0x39                 
#define  SecSerialNumberL               SD_uchar_8(OEMRAMF+0x3A)  	// EDGE_BATTERY_INFO+0x3A                 
#define  SecSerialNumberH               SD_uchar_8(OEMRAMF+0x3B)  	// EDGE_BATTERY_INFO+0x3B                 
#define  SecWarrantyPeriodL             SD_uchar_8(OEMRAMF+0x3C)  	// EDGE_BATTERY_INFO+0x3C                 
#define  SecWarrantyPeriodH             SD_uchar_8(OEMRAMF+0x3D)  	// EDGE_BATTERY_INFO+0x3D                 
#define  Secu3Reserve2E                 SD_uchar_8(OEMRAMF+0x3E)  	// EDGE_BATTERY_INFO+0x3E                 
#define  Secu3Reserve2F                 SD_uchar_8(OEMRAMF+0x3F)  	// EDGE_BATTERY_INFO+0x3F                 
#define  SecTWR40                       SD_uchar_8(OEMRAMF+0x40)  	// EDGE_BATTERY_INFO+0x40                 
#define  SecTWR41                       SD_uchar_8(OEMRAMF+0x41)  	// EDGE_BATTERY_INFO+0x41                                 
#define  SecManufacturerName            SD_Ptr_uchar_8(OEMRAMF+0x42)   // EDGE_BATTERY_INFO+0x42                                  
#define  SecTWR50                       SD_uchar_8(OEMRAMF+0x50)  	// EDGE_BATTERY_INFO+0x50                                 
#define  SecTWR51                       SD_uchar_8(OEMRAMF+0x51)  	// EDGE_BATTERY_INFO+0x51                                 
#define  SecDeviceName                  SD_Ptr_uchar_8(OEMRAMF+0x52)   // EDGE_BATTERY_INFO+0x42                                  
#define  SecTWR60                       SD_uchar_8(OEMRAMF+0x60)  	// EDGE_BATTERY_INFO+0x60                                 
#define  SecTWR61                       SD_uchar_8(OEMRAMF+0x61)  	// EDGE_BATTERY_INFO+0x61                                 
#define  SecDeviceChemistry             SD_Ptr_uchar_8(OEMRAMF+0x62)   // EDGE_BATTERY_INFO+0x42                                  
#define  SecTWR70                       SD_uchar_8(OEMRAMF+0x70)  	// EDGE_BATTERY_INFO+0x70                                 
#define  SecTWR71                       SD_uchar_8(OEMRAMF+0x71)  	// EDGE_BATTERY_INFO+0x71                                 
#define  SecOptMfgFunction5             SD_Ptr_uchar_8(OEMRAMF+0x72)  	// EDGE_BATTERY_INFO+0x72                                 
#define  SecTWR80                       SD_uchar_8(OEMRAMF+0x80)  	// EDGE_BATTERY_INFO+0x80                                 
#define  SecTWR81                       SD_uchar_8(OEMRAMF+0x81)  	// EDGE_BATTERY_INFO+0x81                                 
#define  SecOptMfgFunction1L            SD_uchar_8(OEMRAMF+0x82)  	// EDGE_BATTERY_INFO+0x82                  
#define  SecOptMfgFunction1H            SD_uchar_8(OEMRAMF+0x83)  	// EDGE_BATTERY_INFO+0x83                  
#define  SecOptMfgFunction2L            SD_uchar_8(OEMRAMF+0x84)  	// EDGE_BATTERY_INFO+0x84                
#define  SecOptMfgFunction2H            SD_uchar_8(OEMRAMF+0x85)  	// EDGE_BATTERY_INFO+0x85                 
#define  SecFirmwareUpdateStatusL       SD_uchar_8(OEMRAMF+0x86)  	// EDGE_BATTERY_INFO+0x86                                 
#define  SecFirmwareUpdateStatusH       SD_uchar_8(OEMRAMF+0x87)  	// EDGE_BATTERY_INFO+0x87                                 
#define  SecBatterymakerIDL             SD_uchar_8(OEMRAMF+0x88)  	// EDGE_BATTERY_INFO+0x88                                 
#define  SecBatterymakerIDH             SD_uchar_8(OEMRAMF+0x89)  	// EDGE_BATTERY_INFO+0x89                                 
#define  SecHardwareIDL                 SD_uchar_8(OEMRAMF+0x8A)  	// EDGE_BATTERY_INFO+0x8A                                 
#define  SecHardwareIDH                 SD_uchar_8(OEMRAMF+0x8B)  	// EDGE_BATTERY_INFO+0x8B                                 
#define  SecFirmwareversionL            SD_uchar_8(OEMRAMF+0x8C)  	// EDGE_BATTERY_INFO+0x8C                                 
#define  SecFirmwareversionH            SD_uchar_8(OEMRAMF+0x8D)  	// EDGE_BATTERY_INFO+0x8D                                 
#define  SecDataVersionL                SD_uchar_8(OEMRAMF+0x8E)  	// EDGE_BATTERY_INFO+0x8E                                 
#define  SecDataVersionH                SD_uchar_8(OEMRAMF+0x8F)  	// EDGE_BATTERY_INFO+0x8F                                 
#define  SecTWR90                       SD_uchar_8(OEMRAMF+0x90)  	// EDGE_BATTERY_INFO+0x90                                 
#define  SecTWR91                       SD_uchar_8(OEMRAMF+0x91)  	// EDGE_BATTERY_INFO+0x91                                 
#define  SecManufactureAccessL          SD_uchar_8(OEMRAMF+0x92)  	// EDGE_BATTERY_INFO+0x92                                 
#define  SecManufactureAccessH          SD_uchar_8(OEMRAMF+0x93)  	// EDGE_BATTERY_INFO+0x93                                 
#define  SecMaxErroL                    SD_uchar_8(OEMRAMF+0x94)  	// EDGE_BATTERY_INFO+0x94                   
#define  SecMaxErroH                    SD_uchar_8(OEMRAMF+0x95)  	// EDGE_BATTERY_INFO+0x95                  
#define  SecChargeCurrL                 SD_uchar_8(OEMRAMF+0x96)  	// EDGE_BATTERY_INFO+0x96                                 
#define  SecChargeCurrH                 SD_uchar_8(OEMRAMF+0x97)  	// EDGE_BATTERY_INFO+0x97                                                                                                             
#define  SecChargeVoltL                 SD_uchar_8(OEMRAMF+0x98)  	// EDGE_BATTERY_INFO+0x98                                 
#define  SecChargeVoltH                 SD_uchar_8(OEMRAMF+0x99)  	// EDGE_BATTERY_INFO+0x99                                 
#define  Secu3Reserve9A                 SD_uchar_8(OEMRAMF+0x9A)  	// EDGE_BATTERY_INFO+0x9A                                 
#define  Secu3Reserve9B                 SD_uchar_8(OEMRAMF+0x9B)  	// EDGE_BATTERY_INFO+0x9B                                 
#define  Secu3Reserve9C                 SD_uchar_8(OEMRAMF+0x9C)  	// EDGE_BATTERY_INFO+0x9C                                 
#define  Secu3Reserve9D                 SD_uchar_8(OEMRAMF+0x9D)  	// EDGE_BATTERY_INFO+0x9D                                 
#define  Secu3Reserve9E                 SD_uchar_8(OEMRAMF+0x9E)  	// EDGE_BATTERY_INFO+0x9E                                 
#define  Secu3Reserve9F                 SD_uchar_8(OEMRAMF+0x9F)  	// EDGE_BATTERY_INFO+0x9F                                 
#define  SecTWRA0                       SD_uchar_8(OEMRAMF+0xA0)  	// EDGE_BATTERY_INFO+0xA0                                 
#define  SecTWRA1                       SD_uchar_8(OEMRAMF+0xA1)  	// EDGE_BATTERY_INFO+0xA1                                 
#define  SecManufactureData             SD_Ptr_uchar_8(OEMRAMF+0xA2)  	// EDGE_BATTERY_INFO+0xA2                                 
#define  SecMajor                  	 	  SD_uchar_8(OEMRAMF+0xAE)  	// EDGE_BATTERY_INFO+0xAE                                 
#define  SecMinor                    		SD_uchar_8(OEMRAMF+0xAF)  	// EDGE_BATTERY_INFO+0xAF                                 
#define  SecTWRB0                       SD_uchar_8(OEMRAMF+0xB0)  	// EDGE_BATTERY_INFO+0xB0                                 
#define  SecTWRB1                       SD_uchar_8(OEMRAMF+0xB1)  	// EDGE_BATTERY_INFO+0xB1                                 
#define  SecBattUsageCondition          SD_Ptr_uchar_8(OEMRAMF+0xB2)  	// EDGE_BATTERY_INFO+0xB2                                 
#define  Secu3ReserveBC                 SD_uchar_8(OEMRAMF+0xBC)  	// EDGE_BATTERY_INFO+0xBC                                 
#define  Secu3ReserveBD                 SD_uchar_8(OEMRAMF+0xBD)  	// EDGE_BATTERY_INFO+0xBD                                 
#define  Secu3ReserveBE                 SD_uchar_8(OEMRAMF+0xBE)  	// EDGE_BATTERY_INFO+0xBE                                 
#define  Secu3ReserveBF                 SD_uchar_8(OEMRAMF+0xBF)  	// EDGE_BATTERY_INFO+0xBF                                 
#define  SecTWRC0                       SD_uchar_8(OEMRAMF+0xC0)  	// EDGE_BATTERY_INFO+0xC0                                 
#define  SecTWRC1                       SD_uchar_8(OEMRAMF+0xC1)  	// EDGE_BATTERY_INFO+0xC1                                 
#define  SecBattAuthStat                SD_uchar_8(OEMRAMF+0xC2)  	// EDGE_BATTERY_INFO+0xC2                                 
#define  Secu3ReserveC3                 SD_uchar_8(OEMRAMF+0xC3)  	// EDGE_BATTERY_INFO+0xC3                                 
#define  Secu3ReserveC4                 SD_uchar_8(OEMRAMF+0xC4)  	// EDGE_BATTERY_INFO+0xC4                                 
#define  Secu3ReserveC5                 SD_uchar_8(OEMRAMF+0xC5)  	// EDGE_BATTERY_INFO+0xC5                                 
#define  Secu3ReserveC6                 SD_uchar_8(OEMRAMF+0xC6)  	// EDGE_BATTERY_INFO+0xC6                                 
#define  Secu3ReserveC7                 SD_uchar_8(OEMRAMF+0xC7)  	// EDGE_BATTERY_INFO+0xC7                                 
#define  Secu3ReserveC8                 SD_uchar_8(OEMRAMF+0xC8)  	// EDGE_BATTERY_INFO+0xC8                                 
#define  Secu3ReserveC9                 SD_uchar_8(OEMRAMF+0xC9)  	// EDGE_BATTERY_INFO+0xC9                                 
#define  Secu3ReserveCA                 SD_uchar_8(OEMRAMF+0xCA)  	// EDGE_BATTERY_INFO+0xCA                                 
#define  Secu3ReserveCB                 SD_uchar_8(OEMRAMF+0xCB)  	// EDGE_BATTERY_INFO+0xCB                                 
#define  Secu3ReserveCC                 SD_uchar_8(OEMRAMF+0xCC)  	// EDGE_BATTERY_INFO+0xCC                                 
#define  Secu3ReserveCD                 SD_uchar_8(OEMRAMF+0xCD)  	// EDGE_BATTERY_INFO+0xCD                                 
#define  Secu3ReserveCE                 SD_uchar_8(OEMRAMF+0xCE)  	// EDGE_BATTERY_INFO+0xCE                                 
#define  Secu3ReserveCF                 SD_uchar_8(OEMRAMF+0xCF)  	// EDGE_BATTERY_INFO+0xCF                                 
#define  Secu3ReserveD0                 SD_uchar_8(OEMRAMF+0xD0)  	// EDGE_BATTERY_INFO+0xD0                                 
#define  Secu3ReserveD1                 SD_uchar_8(OEMRAMF+0xD1)  	// EDGE_BATTERY_INFO+0xD1                                 
#define  Secu3ReserveD2                 SD_uchar_8(OEMRAMF+0xD2)  	// EDGE_BATTERY_INFO+0xD2                                 
#define  Secu3ReserveD3                 SD_uchar_8(OEMRAMF+0xD3)  	// EDGE_BATTERY_INFO+0xD3                                 
#define  Secu3ReserveD4                 SD_uchar_8(OEMRAMF+0xD4)  	// EDGE_BATTERY_INFO+0xD4                                 
#define  Secu3ReserveD5                 SD_uchar_8(OEMRAMF+0xD5)  	// EDGE_BATTERY_INFO+0xD5                                 
#define  Secu3ReserveD6                 SD_uchar_8(OEMRAMF+0xD6)  	// EDGE_BATTERY_INFO+0xD6                                 
#define  Secu3ReserveD7                 SD_uchar_8(OEMRAMF+0xD7)  	// EDGE_BATTERY_INFO+0xD7                                 
#define  Secu3ReserveD8                 SD_uchar_8(OEMRAMF+0xD8)  	// EDGE_BATTERY_INFO+0xD8                                 
#define  Secu3ReserveD9                 SD_uchar_8(OEMRAMF+0xD9)  	// EDGE_BATTERY_INFO+0xD9                                 
#define  Secu3ReserveDA                 SD_uchar_8(OEMRAMF+0xDA)  	// EDGE_BATTERY_INFO+0xDA                                 
#define  Secu3ReserveDB                 SD_uchar_8(OEMRAMF+0xDB)  	// EDGE_BATTERY_INFO+0xDB                                 
#define  Secu3ReserveDC                 SD_uchar_8(OEMRAMF+0xDC)  	// EDGE_BATTERY_INFO+0xDC                                 
#define  Secu3ReserveDD                 SD_uchar_8(OEMRAMF+0xDD)  	// EDGE_BATTERY_INFO+0xDD                                 
#define  Secu3ReserveDE                 SD_uchar_8(OEMRAMF+0xDE)  	// EDGE_BATTERY_INFO+0xDE                                 
#define  Secu3ReserveDF                 SD_uchar_8(OEMRAMF+0xDF)  	// EDGE_BATTERY_INFO+0xDF                                 
#define  Secu3ReserveE0                 SD_uchar_8(OEMRAMF+0xE0)  	// EDGE_BATTERY_INFO+0xE0                                 
#define  Secu3ReserveE1                 SD_uchar_8(OEMRAMF+0xE1)  	// EDGE_BATTERY_INFO+0xE1                                 
#define  Secu3ReserveE2                 SD_uchar_8(OEMRAMF+0xE2)  	// EDGE_BATTERY_INFO+0xE2                                 
#define  Secu3ReserveE3                 SD_uchar_8(OEMRAMF+0xE3)  	// EDGE_BATTERY_INFO+0xE3                                 
#define  Secu3ReserveE4                 SD_uchar_8(OEMRAMF+0xE4)  	// EDGE_BATTERY_INFO+0xE4                                 
#define  Secu3ReserveE5                 SD_uchar_8(OEMRAMF+0xE5)  	// EDGE_BATTERY_INFO+0xE5                                 
#define  Secu3ReserveE6                 SD_uchar_8(OEMRAMF+0xE6)  	// EDGE_BATTERY_INFO+0xE6                                 
#define  Secu3ReserveE7                 SD_uchar_8(OEMRAMF+0xE7)  	// EDGE_BATTERY_INFO+0xE7                                 
#define  Secu3ReserveE8                 SD_uchar_8(OEMRAMF+0xE8)  	// EDGE_BATTERY_INFO+0xE8                                 
#define  Secu3ReserveE9                 SD_uchar_8(OEMRAMF+0xE9)  	// EDGE_BATTERY_INFO+0xE9                                 
#define  Secu3ReserveEA                 SD_uchar_8(OEMRAMF+0xEA)  	// EDGE_BATTERY_INFO+0xEA                                 
#define  Secu3ReserveEB                 SD_uchar_8(OEMRAMF+0xEB)  	// EDGE_BATTERY_INFO+0xEB                                 
#define  Secu3ReserveEC                 SD_uchar_8(OEMRAMF+0xEC)  	// EDGE_BATTERY_INFO+0xEC                                 
#define  Secu3ReserveED                 SD_uchar_8(OEMRAMF+0xED)  	// EDGE_BATTERY_INFO+0xED                                 
#define  Secu3ReserveEE                 SD_uchar_8(OEMRAMF+0xEE)  	// EDGE_BATTERY_INFO+0xEE                                 
#define  Secu3ReserveEF                 SD_uchar_8(OEMRAMF+0xEF)  	// EDGE_BATTERY_INFO+0xEF                                 
#define  Secu3ReserveF0                 SD_uchar_8(OEMRAMF+0xF0)  	// EDGE_BATTERY_INFO+0xF0                                 
#define  Secu3ReserveF1                 SD_uchar_8(OEMRAMF+0xF1)  	// EDGE_BATTERY_INFO+0xF1                                 
#define  Secu3ReserveF2                 SD_uchar_8(OEMRAMF+0xF2)  	// EDGE_BATTERY_INFO+0xF2                                 
#define  Secu3ReserveF3                 SD_uchar_8(OEMRAMF+0xF3)  	// EDGE_BATTERY_INFO+0xF3                                 
#define  Secu3ReserveF4                 SD_uchar_8(OEMRAMF+0xF4)  	// EDGE_BATTERY_INFO+0xF4                                 
#define  Secu3ReserveF5                 SD_uchar_8(OEMRAMF+0xF5)  	// EDGE_BATTERY_INFO+0xF5                                 
#define  Secu3ReserveF6                 SD_uchar_8(OEMRAMF+0xF6)  	// EDGE_BATTERY_INFO+0xF6                                 
#define  Secu3ReserveF7                 SD_uchar_8(OEMRAMF+0xF7)  	// EDGE_BATTERY_INFO+0xF7                                 
#define  Secu3ReserveF8                 SD_uchar_8(OEMRAMF+0xF8)  	// EDGE_BATTERY_INFO+0xF8                                 
#define  Secu3ReserveF9                 SD_uchar_8(OEMRAMF+0xF9)  	// EDGE_BATTERY_INFO+0xF9                                 
#define  Secu3ReserveFA                 SD_uchar_8(OEMRAMF+0xFA)  	// EDGE_BATTERY_INFO+0xFA                                 
#define  Secu3ReserveFB                 SD_uchar_8(OEMRAMF+0xFB)  	// EDGE_BATTERY_INFO+0xFB                                 
#define  Secu3ReserveFC                 SD_uchar_8(OEMRAMF+0xFC)  	// EDGE_BATTERY_INFO+0xFC                                 
#define  Secu3ReserveFD                 SD_uchar_8(OEMRAMF+0xFD)  	// EDGE_BATTERY_INFO+0xFD                                 
#define  Secu3ReserveFE                 SD_uchar_8(OEMRAMF+0xFE)  	// EDGE_BATTERY_INFO+0xFE                                 
#define  Secu3ReserveFF                 SD_uchar_8(OEMRAMF+0xFF)  	// EDGE_BATTERY_INFO+0xFF  



#endif 


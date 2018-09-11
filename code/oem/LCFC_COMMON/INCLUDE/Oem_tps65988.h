/*
********************************************************************************
* Lenovo Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LENOVO 2016 ALL RIGHTS RESERVED
*
* Filename: Oem_tps65988.h
* 
* Description:
*    definition of the unique address interface registers of TI TPS65988 USB-PD controller.
* 
* History:
*  12/30/2016    Isaac Lin
*      created as new
********************************************************************************
*/
 
#ifndef _OEM_TPS65988_H
#define _OEM_TPS65988_H

#if (Support_TYPE_C == TPS65988)

#define 	TypeC_Ini_Delay_time  10 	//Delay 100ms for type C stable     10 * 10mS 
#define 	TypeCLimitedSupport_BatteryRSOC  10 	//In AC+DC mode, when battery capacity < 5%, behavior is the same with AC only mode
#define 	TypeC_IntSetting_Delay_time  50 	//Wait for 500ms if power is supplied from USB PDC B port(Dead battery)     50 * 10mS 
#define 	ObjectPosition5V  1


#ifndef 	NUMBER_OF_USB_PDC
    #define NUMBER_OF_USB_PDC 2
#endif
#define     NUMBER_ALT_MODES		2

#define 	USB_PDC_RC_OK              0
#define 	USB_PDC_RC_NOT_SUPPORTED   1
#define 	USB_PDC_RC_BUSY            2



/*
*********************************************************
*     #EC ram define
*********************************************************
*/
#define  TypeCStatus          					TYPE_C_RAM0             //_at_ OEMRAM4+0x70;	//Bit2~bit0:Step
 #define     TypeCAdpAuthing   			BIT0
 #define     TypeCAdpAuthDone  			BIT1
 #define     TypeCAdpAuthPass  			BIT2
 #define     TypeCAdpExist     			BIT3
 #define     OldDcinAttached2  			BIT4
 #define     TypeCAdpReady     			BIT5
 #define     TypeCProcessOK    			BIT6
 #define     TypeCIniOK        			BIT7
 
#define  TypeCIniDelay         					TYPE_C_RAM1             //_at_ OEMRAM4+0x71;
#define  TypeCEventNum         					TYPE_C_RAM2             //_at_ OEMRAM4+0x72;
#define  TypeCCommFailCnt      					TYPE_C_RAM3             //_at_ OEMRAM4+0x73;
#define  TypeCAdpID            					TYPE_C_RAM4             //_at_ OEMRAM4+0x74;
#define  CurrentTypeCWatt      					TYPE_C_RAM5             //_at_ OEMRAM4+0x75;
#define  TypeCAdpPdStableCheck 					TYPE_C_RAM6             //_at_ OEMRAM4+0x76;
#define  TypeCProcessStep      					TYPE_C_RAM7             //_at_ OEMRAM4+0x77;
#define  PDStatus	             				TYPE_C_RAM8             //_at_ OEMRAM4+0x78;
// #define 		CurrentDEV_INTR		BIT0
// #define 		CurrentPORT0_INTR	BIT1
// #define 		CurrentPORT1_INTR	BIT2
 #define     TypeCFwUpdating   			BIT7
 #define    TypeC4CCCmd                         BIT5        //A285D00075+
 
#define  LimitedSupportAdpWattUpper				TYPE_C_RAM9              //_at_ OEMRAM4+0x79;
#define  CurrentTypeCV         					TYPE_C_RAM10             //_at_ OEMRAM4+0x7A;  word
#define  CurrentTypeCI         					TYPE_C_RAM11             //_at_ OEMRAM4+0x7C;  word
#define  TypeCStatus2							TYPE_C_RAM12             //_at_ OEMRAM4+0x7E;
 #define    ClassicAdpDetachShutdown   	BIT0
 #define    DockingEvent				BIT1
 #define    TypeC_Docking_Attached	   	BIT2
 #define    TypeC_ChipOff	   			BIT3
 #define    PdcRequestDone             	BIT4
 #define    TypeC_Dead_Battery_Mode	   	BIT5	//1:Dead battery mode;0:non-dead battery mode
#define    TypeC_Laptop_Power_Type_Target          BIT6    //1:Source,0:Sink
#define    TypeC_Laptop_Power_Charge_Status        BIT7    //1:Charge,0:Don't charge

#define  TypeCStatus3					TYPE_C_RAM14             //_at_ OEMRAM4+0x7E;
 #define    NeedUpdateToPartner   	BIT0
 #define    AllEventsNotClear	   	BIT1
 #define    GotPartnerInformation   	BIT2
 #define    P2PPowerSwapWait	   	BIT3
 #define    TypeC_Laptop_Attached             BIT4
 #define    TypeC_Laptop_Power_Charge_Target	   	BIT5	//1:Charge,0:Don't charge
 #define    TypeC_Laptop_Power_Type	   	        BIT6	//1:Dead battery mode;0:non-dead battery mode
 #define    Cmd_4CC_Completed	   	        BIT7

#define  UnknowAdpWatt					 		TYPE_C_RAM13             //_at_ OEMRAM4+0x7F;

#define  PowerSourceSwitchDelay	 				TYPE_C_RAM33	//Type-C power source switch command delay for three port
#define  AnoutherUnitInformation	    TYPE_C_RAM35    //_at_ OEMRAM4+0x19;0x19~0x1A
#define  AnoutherUnitInformation2	    TYPE_C_RAM36    //_at_ OEMRAM4+0x19;0x19~0x1A

#define  TypeCPort2Status          				TYPE_C_RAM17             //_at_ OEMRAM4+0x88;	//Bit2~bit0:Step
 #define     TypeCProcessOK    			BIT6
 #define     TypeCIniOK        			BIT7
 
#define	 TypeCPort2IniDelay         			TYPE_C_RAM18             //_at_ OEMRAM4+0x89;	
#define  TypeCPort2ProcessStep      			TYPE_C_RAM19             //_at_ OEMRAM4+0x8A;
#define  TypeCPort2CommFailCnt      			TYPE_C_RAM20             //_at_ OEMRAM4+0x8B;
#define  TypeCPort2EventNum         			TYPE_C_RAM21             //_at_ OEMRAM4+0x8C;
#define  TypeCPort2AdpPdStableCheck 			TYPE_C_RAM22             //_at_ OEMRAM4+0x8D;

#define  Port2CurrentTypeCV         			TYPE_C_RAM23             //_at_ OEMRAM4+0x7A;  word
#define  Port2CurrentTypeCI         			TYPE_C_RAM24             //_at_ OEMRAM4+0x7C;  word
#define  TypeCPort2Status2						TYPE_C_RAM25             //_at_ OEMRAM4+0x7E;
 #define    ClassicAdpDetachShutdown   	BIT0
 #define    DockingEvent				BIT1
 #define    TypeC_Docking_Attached	   	BIT2
 #define    TypeC_ChipOff	   			BIT3
 #define    PdcRequestDone             	BIT4
 #define    TypeC_Dead_Battery_Mode	   	BIT5	//1:Dead battery mode;0:non-dead battery mode
 
 #define  TypeCPort2Status3					TYPE_C_RAM34             //_at_ OEMRAM4+0x7E;

#define	 TypeCPort2AdpID						TYPE_C_RAM26
#define	 Port2CurrentTypeCWatt					TYPE_C_RAM27
////A485D00080
#define  TypeCAdpDetectDelay					TYPE_C_RAM29
#define  UnknowAdpRetry                         TYPE_C_RAM30
#define  TypeCPort2AdpDetectDelay					TYPE_C_RAM31
#define  Port2UnknowAdpRetry                         TYPE_C_RAM32
//A485D00080

#define  Port_Select							TYPE_C_RAM41
#define TypeCPort1AdpID				TYPE_C_RAM42
#define Port1CurrentTypeCWatt			TYPE_C_RAM43
#define Port1CurrentTypeCV			TYPE_C_RAM44
#define Port1CurrentTypeCI				TYPE_C_RAM45
#define UCSI_LastRelativeStateOfChg	TYPE_C_RAM46

#define PDOVoltageFlag				TYPE_C_RAM47

#define Port1PDOVoltage9V				BIT0
#define Port2PDOVoltage9V				BIT4

#define PD_SWITCH_BATTERY_CAPA_THERSHOLD		(7)


#define  OldDockingACKStatus					TYPE_C_DOCKING_RAM0             //_at_ OEMRAM5+0x85;
#define  OldDockingEvent						TYPE_C_DOCKING_RAM1             //_at_ OEMRAM5+0x86;
#define  OldVDO2								TYPE_C_DOCKING_RAM2             //_at_ OEMRAM5+0x87;0x87~0x8A
#define  OldInformation	                        TYPE_C_RAM37
#define  OldInformation2	                    TYPE_C_RAM38
#define DockingPSW_ActiveCnt					TYPE_C_RAM40             //_at_ OEMRAM4+0x60		// CMW20180323

/*
*********************************************************
*     #define Constants
*********************************************************
*/

#ifndef TPS65988_A_I2C_ADDR
    #define TPS65988_A_I2C_ADDR   0x70//0x46
#endif
#ifndef TPS65988_B_I2C_ADDR
    #define TPS65988_B_I2C_ADDR   0x7E//0x4E
#endif
#ifndef TPS65988_I2C_ADDR
    #define TPS65988_I2C_ADDR     TPS65988_B_I2C_ADDR
#endif

//   SYMBOL definition
#define	SET_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	SET_MASK(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	CLEAR_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	CLEAR_MASK(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	READ_SINGLE_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	IS_MASK_SET(*(&_reg_base_+_reg_offset_),_bit_number_)
#define	READ_MULTI_BIT_IN_BYTES(_reg_base_,_reg_offset_,_bit_number_)	((*(&_reg_base_+_reg_offset_)) & _bit_number_ )
//#define	SET_SINGLE_BIT_IN_BYTES(x,y,z)	SET_MASK(*(&x+y),z)
//#define	CLEAR_SINGLE_BIT_IN_BYTES(x,y,z)	CLEAR_MASK(*(&x+y),z)
//#define	READ_SINGLE_BIT_IN_BYTES(x,y,z)	IS_MASK_SET(*(&x+y),z)
//define	Fu_En(x)	Fs(x,1,BIT2)	//Set bit10
//define	Fu_Dis(x)	Fc(x,1,BIT2)	//Clear bit10

//     b29: Littel/Big endian, b28: Multiple bit/ Single bit, b24-b27: Type
//     b16-23: number of bit
//     b8-15:  offset or size
//     b0-7:   bit number or start bit number for multiple bit definition
#define TPS65988_LITTLE_ENDIAN 0x20000000l
#define TPS65988_BIG_ENDIAN    0
#define TPS65988_MULTI_BIT     0x10000000l
#define TPS65988_SINGLE_BIT    0
#define TPS65988_TYPE_NONE     0x0
#define TPS65988_TYPE_BYTE     0x01000000l
#define TPS65988_TYPE_WORD     0x02000000l
#define TPS65988_TYPE_DWORD    0x03000000l

#define TPS65988_TBT_VID                        0x00          // Intel assigned Thunderbolt Vender ID (4 bytes (RO))
#define TPS65988_DID                            0x01          // Vender specific Device ID (4 bytes (RO))
#define TPS65988_PROTO_VER                      0x02          // Thunderbolt Protocol Version (4 bytes (RO))
#define TPS65988_MODE                           0x03          // Indicate the ope. state of device (4 bytes (RO))
                                                              //  "BOOT", "APP" or "BIST"
#define TPS65988_TYPE                           0x04          // Respond 'I2C' (4 butes (RO))
#define TPS65988_UID                            0x05          // 128bit unique ID (16 butes (RO))
#define TPS65988_CUSTUSE                        0x06          // Customer use data configured by the tool (8 butes (RO))
#define TPS65988_CMD1                           0x08          // Command register used for primary command interface     
                                                              // '!CMD' is returned for unrecognized commands
                                                              //  (4 bytes (R/W)
#define TPS65988_DATA1                          0x09          // Data register used for the primary command interface    
                                                              //  (64 bytes (R/W))
#define TPS65988_VERSION                        0x0f          // BCD version number, boot loader/application code    
                                                              // represented VVVVMMRR (4bytes (RO))
#define TPS65988_CMD2                           0x10          // Command register used for the second command
                                                              //   interface     
                                                              // '!CMD' is returned for unrecognized commands
                                                              //   (4 bytes (R/W)
#define TPS65988_DATA2                          0x11          // Data register used for the secondary command interface    
                                                              //  (64 bytes (R/W))
/*
*    Interrupt event bit field for I2C_IRQ1 (7 bytes (RO))
*/
#define TPS65988_INT_NO_EVENT_Num               0xFF
#define TPS65988_INT_EVENT1                     0x14

#define TPS65988_INT_SIZE						11
                                                              // Bits: 59: a User VID structured non-Attention VDM or
                                                              //           unstructured VDM has been received
//    #define TPS65988_INT_USER_VID_ALT_MODE_OTHER_VDM   TPS65988_DEF_BIT_FIELD(59, 8)  
		#define	TPS65988_INT_USER_VID_ALT_MODE_OTHER_VDM(x)	READ_SINGLE_BIT_IN_BYTES(x,7,BIT3)	//Set bit59
		#define	TPS65988_INT_USER_VID_ALT_MODE_OTHER_VDM_Num	59
                                                              // Bits: 58: a User VID structured Attention VDM has been received
//    #define TPS65988_INT_USER_VID_ALT_MODE_ATTN_VDM    TPS65988_DEF_BIT_FIELD(58, 8)  
		#define	TPS65988_INT_USER_VID_ALT_MODE_ATTN_VDM(x)	READ_SINGLE_BIT_IN_BYTES(x,7,BIT2)	//Set bit58
		#define	TPS65988_INT_USER_VID_ALT_MODE_ATTN_VDM_Num	58
                                                              // Bits: 57: a User VID alternate mode has been exited
//    #define TPS65988_INT_USER_VID_ALT_MODE_EXITED      TPS65988_DEF_BIT_FIELD(57, 8)  
		#define	TPS65988_INT_USER_VID_ALT_MODE_EXITED(x)	READ_SINGLE_BIT_IN_BYTES(x,7,BIT1)	//Set bit57
		#define	TPS65988_INT_USER_VID_ALT_MODE_EXITED_Num	57
                                                              // Bits: 56: a User VID alternate mode has been entered
//    #define TPS65988_INT_USER_VID_ALT_MODE_ENTERED     TPS65988_DEF_BIT_FIELD(56, 8)  
		#define	TPS65988_INT_USER_VID_ALT_MODE_ENTERED(x)	READ_SINGLE_BIT_IN_BYTES(x,7,BIT0)	//Set bit56
		#define	TPS65988_INT_USER_VID_ALT_MODE_ENTERED_Num	56
                                                              // Bits: 55: The primary device is ready for
                                                              //           to receive a patch bundle from the host
//    #define TPS65988_INT_READY_FOR_PATCH               TPS65988_DEF_BIT_FIELD(55, 8)  
		#define	TPS65988_INT_READY_FOR_PATCH(x)	READ_SINGLE_BIT_IN_BYTES(x,6,BIT7)	//Set bit55
                                                              // Bits: 54: Patch was loaded
//    #define TPS65988_INT_PATCH_LOADED                  TPS65988_DEF_BIT_FIELD(54, 8)
		#define	TPS65988_INT_PATCH_LOADED(x)	READ_SINGLE_BIT_IN_BYTES(x,6,BIT6)	//Set bit54
                                                              // Bits: 52: Exit mode process is completed
//    #define TPS65988_INT_EXIT_MODE_COMPLETE            TPS65988_DEF_BIT_FIELD(52, 8)
		#define	TPS65988_INT_EXIT_MODE_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,6,BIT4)	//Set bit52
                                                              // Bits: 51: Discover mode process has completed
//    #define TPS65988_INT_DISCOVER_MODE_COMPLETE        TPS65988_DEF_BIT_FIELD(51, 8)
		#define	TPS65988_INT_DISCOVER_MODE_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,6,BIT3)	//Set bit51
                                                              // Bits: 49: set when any alternate mode is entered 
//    #define TPS65988_INT_VDM_ENTER_MODE                TPS65988_DEF_BIT_FIELD(49, 8)
		#define	TPS65988_INT_VDM_ENTER_MODE(x)	READ_SINGLE_BIT_IN_BYTES(x,6,BIT1)	//Set bit49
                                                              // Bits: 46: source was unable to increase voltage to
                                                              //         negotiated voltage of contract
//    #define TPS65988_INT_ERROR_UNABLE_TO_SOURCE        TPS65988_DEF_BIT_FIELD(46, 8)
		#define	TPS65988_INT_ERROR_UNABLE_TO_SOURCE(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT6)	//Set bit46
                                                              // Bits: 42: time for sink transition is expired  
//    #define TPS65988_INT_SNK_TRANSITION                TPS65988_DEF_BIT_FIELD(42, 8)
		#define	TPS65988_INT_SNK_TRANSITION(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT2)	//Set bit42
                                                              // Bits: 41: TPS65988 fails to discharge VBUSus 
//    #define TPS65988_INT_ERROR_DISCHARGE_FAILED        TPS65988_DEF_BIT_FIELD(41, 8)
		#define	TPS65988_INT_ERROR_DISCHARGE_FAILED(x)	READ_SINGLE_BIT_IN_BYTES(x,5,BIT1)	//Set bit41
                                                              // Bits: 39: Bad CRC of received message or data length unmatch
//    #define TPS65988_INT_ERROR_MESSAGE_DATA            TPS65988_DEF_BIT_FIELD(39, 8)
		#define	TPS65988_INT_ERROR_MESSAGE_DATA(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT7)	//Set bit39
                                                              // Bits: 38: unexpected message has been received
//    #define TPS65988_INT_ERROR_PROTOCOL_ERROR          TPS65988_DEF_BIT_FIELD(38, 8)
		#define	TPS65988_INT_ERROR_PROTOCOL_ERROR(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT6)	//Set bit38
                                                              // Bits: 36: no response for Get_Sink or Get_Source_Cap message
//    #define TPS65988_INT_ERROR_MISSING_GET_CAP_MESSAGE TPS65988_DEF_BIT_FIELD(36, 8)
		#define	TPS65988_INT_ERROR_MISSING_GET_CAP_MESSAGE(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT4)	//Set bit36
                                                              // Bits: 35: OVP or short circuit event occurred on VBus
//    #define TPS65988_INT_ERROR_POWER_EVENT_OCCURRED    TPS65988_DEF_BIT_FIELD(35, 8)
		#define	TPS65988_INT_ERROR_POWER_EVENT_OCCURRED(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT3)	//Set bit35
                                                              // Bits: 34: wait response for providing voltage and 
                                                              //         current was sent or received
//    #define TPS65988_INT_ERROR_CAN_PROVIDE_VOLTAGE     TPS65988_DEF_BIT_FIELD(34, 8)
		#define	TPS65988_INT_ERROR_CAN_PROVIDE_VOLTAGE(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT2)	//Set bit34
                                                              // Bits: 33: reject message was sent to the sink or
                                                              //         capability mismatch message was received
                                                              //         from the sink
//    #define TPS65988_INT_ERROR_CANNOT_PROVIDE_VOLTAGE  TPS65988_DEF_BIT_FIELD(33, 8)
		#define	TPS65988_INT_ERROR_CANNOT_PROVIDE_VOLTAGE(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT1)	//Set bit33
                                                              // Bits: 32: a USB device with an incompatible
                                                              //         specification version was connected
//    #define TPS65988_INT_ERROR_DEVICE_INCOMPTIBLE      TPS65988_DEF_BIT_FIELD(32, 8)
		#define	TPS65988_INT_ERROR_DEVICE_INCOMPTIBLE(x)	READ_SINGLE_BIT_IN_BYTES(x,4,BIT0)	//Set bit32
                                                              // Bits: 31: a non-zero value in CMD2 register is
                                                              //         set to zero or !CMD
//    #define TPS65988_INT_CMD2_COMPLETE                 TPS65988_DEF_BIT_FIELD(31, 8)
		#define	TPS65988_INT_CMD2_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT7)	//Set bit31
		#define	TPS65988_INT_CMD2_COMPLETE_Num	31
                                                              // Bits: 30: a non-zero value in CMD1 register is
                                                              //         set to zero or !CMD
//    #define TPS65988_INT_CMD1_COMPLETE                 TPS65988_DEF_BIT_FIELD(30, 8)
		#define	TPS65988_INT_CMD1_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT6)	//Set bit30
		#define	TPS65988_INT_CMD1_COMPLETE_Num	30
                                                              // Bits: 29: ADC high threshold
//    #define TPS65988_INT_ADC_HIGH_THRESHOLD            TPS65988_DEF_BIT_FIELD(29, 8)
		#define	TPS65988_INT_ADC_HIGH_THRESHOLD(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT5)	//Set bit29
                                                              // Bits: 28: ADC low threshold
//    #define TPS65988_INT_ADC_LOW_THRESHOLD             TPS65988_DEF_BIT_FIELD(28, 8)
		#define	TPS65988_INT_ADC_LOW_THRESHOLD(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT4)	//Set bit28
                                                              // Bits: 27: PD status register (0x40) is changed
//    #define TPS65988_INT_PD_STATUS_UPDATE              TPS65988_DEF_BIT_FIELD(27, 8)
		#define	TPS65988_INT_PD_STATUS_UPDATE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT3)	//Set bit27
		#define	TPS65988_INT_PD_STATUS_UPDATE_Num	27
                                                              // Bits: 26: Status register (0x1A) is changed
//    #define TPS65988_INT_STATUS_UPDATE                 TPS65988_DEF_BIT_FIELD(26, 8)
		#define	TPS65988_INT_STATUS_UPDATE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT2)	//Set bit26
		#define	TPS65988_INT_STATUS_UPDATE_Num	26
                                                              // Bits: 25: Data status register (0x5f) is changed
//    #define TPS65988_INT_DATA_STATUS_UPDATE            TPS65988_DEF_BIT_FIELD(25, 8)
		#define	TPS65988_INT_DATA_STATUS_UPDATE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT1)	//Set bit25
		#define	TPS65988_INT_DATA_STATUS_UPDATE_Num	25
                                                              // Bits: 24: Power status register (0x3f) is changed
//    #define TPS65988_INT_POWER_STATUS_UPDATE           TPS65988_DEF_BIT_FIELD(24, 8)
		#define	TPS65988_INT_POWER_STATUS_UPDATE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT0)	//Set bit24
		#define	TPS65988_INT_POWER_STATUS_UPDATE_Num	24
                                                              // Bits: 23: Status.PP switch registers changed
//    #define TPS65988_INT_PP_SWITCH_CHANGE              TPS65988_DEF_BIT_FIELD(23, 8)
		#define	TPS65988_INT_PP_SWITCH_CHANGE(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT7)	//Set bit23
                                                              // Bits: 22: Status.HighVoltageWarning transition
                                                              //         from 0 to 1.
//    #define TPS65988_INT_HIGH_VOLTAGE_WARNING          TPS65988_DEF_BIT_FIELD(22, 8)
		#define	TPS65988_INT_HIGH_VOLTAGE_WARNING(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT6)	//Set bit22
                                                              // Bits: 21: Status.UsbHostPresent transition from 1 to 0.
//    #define TPS65988_INT_USB_HOST_PRESENT_NO_LONGER    TPS65988_DEF_BIT_FIELD(21, 8)
		#define	TPS65988_INT_USB_HOST_PRESENT_NO_LONGER(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT5)	//Set bit21
                                                              // Bits: 20: Status.UsbHostPresent transition from 0 to 1.
//    #define TPS65988_INT_USB_HOST_PRESENT              TPS65988_DEF_BIT_FIELD(20, 8)
		#define	TPS65988_INT_USB_HOST_PRESENT(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT4)	//Set bit20
                                                              // Bits: 19: GotoMin message received
//    #define TPS65988_INT_GO_TO_MIN_RECEIVED            TPS65988_DEF_BIT_FIELD(19, 8)
		#define	TPS65988_INT_GO_TO_MIN_RECEIVED(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT3)	//Set bit19
                                                              // Bits: 17: A swap was requested
//    #define TPS65988_INT_SWAP_REQUESTED                TPS65988_DEF_BIT_FIELD(17, 8)
		#define	TPS65988_INT_SWAP_REQUESTED(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT1)	//Set bit17
		#define	TPS65988_INT_SWAP_REQUESTED_Num	17
		
                                                              // Bits: 15: Sink capabilities has been updated
//    #define TPS65988_INT_SINK_CAP_MSG_READY            TPS65988_DEF_BIT_FIELD(15, 8)
		#define	TPS65988_INT_SINK_CAP_MSG_READY(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT7)	//Set bit15
                                                              // Bits: 14: Source capabilities has been updated
//    #define TPS65988_INT_SOURCE_CAP_MSG_READY          TPS65988_DEF_BIT_FIELD(14, 8)
		#define	TPS65988_INT_SOURCE_CAP_MSG_READY(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT6)	//Set bit14
		#define	TPS65988_INT_SOURCE_CAP_MSG_READY_Num	14
                                                              // Bits: 13: RDO has been accepted and
                                                              //           the TPS65988 is a source
//    #define TPS65988_INT_NEW_CONTRACT_AS_PROV          TPS65988_DEF_BIT_FIELD(13, 8)
		#define	TPS65988_INT_NEW_CONTRACT_AS_PROV(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT5)	//Set bit13
		#define	TPS65988_INT_NEW_CONTRACT_AS_PROV_Num	13
                                                              // Bits: 12: RDO has been accepted and
                                                              //           the TPS65988 is a sink
//    #define TPS65988_INT_NEW_CONTRACT_AS_CONS          TPS65988_DEF_BIT_FIELD(12, 8)
		#define	TPS65988_INT_NEW_CONTRACT_AS_CONS(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT4)	//Set bit12
		#define	TPS65988_INT_NEW_CONTRACT_AS_CONS_Num	12
                                                              // Bits: 11: A VDM has been receieved
//    #define TPS65988_INT_VDM_RECEIVED                  TPS65988_DEF_BIT_FIELD(11, 8)
		#define	TPS65988_INT_VDM_RECEIVED(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT3)	//Set bit11
		#define	TPS65988_INT_VDM_RECEIVED_Num	11
                                                              // Bits: 10: An attention message has been received
//    #define TPS65988_INT_ATTENTION_RECEIVED            TPS65988_DEF_BIT_FIELD(10, 8)
		#define	TPS65988_INT_ATTENTION_RECEIVED(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT2)	//Set bit10
                                                              // Bits:  9: Status.OverCurrent is changed
//    #define TPS65988_INT_OVER_CURRENT                  TPS65988_DEF_BIT_FIELD( 9, 8)
		#define	TPS65988_INT_OVER_CURRENT(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT1)	//Set bit9
                                                              // Bits:  8: Status.BIST is changed
//    #define TPS65988_INT_BIST                          TPS65988_DEF_BIT_FIELD( 8, 8)
		#define	TPS65988_INT_BIST(x)	READ_SINGLE_BIT_IN_BYTES(x,1,BIT0)	//Set bit8
                                                              // Bits:  7: Source receives a request from the sink
//    #define TPS65988_INT_RDO_RECEIVED_FROM_SINK        TPS65988_DEF_BIT_FIELD( 7, 8)
		#define	TPS65988_INT_RDO_RECEIVED_FROM_SINK(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT7)	//Set bit7
                                                              // Bits:  5: Data role swap has completed
//    #define TPS65988_INT_DR_SWAP_COMPLETE              TPS65988_DEF_BIT_FIELD( 5, 8)
#define TPS65988_INT_DR_SWAP_Num        5
		#define	TPS65988_INT_DR_SWAP_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT5)	//Set bit5
                                                              // Bits:  4: Power role swap has completed
//    #define TPS65988_INT_PR_SWAP_COMPLETE              TPS65988_DEF_BIT_FIELD( 4, 8)
#define TPS65988_INT_PR_SWAP_Num        4
		#define	TPS65988_INT_PR_SWAP_COMPLETE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT4)	//Set bit4
                                                              // Bits:  3: USB plug status has changed
//    #define TPS65988_INT_PLUG_INSERT_OR_REMOVAL        TPS65988_DEF_BIT_FIELD( 3, 8)
		#define	TPS65988_INT_PLUG_INSERT_OR_REMOVAL(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT3)	//Set bit3
    #define TPS65988_INT_PLUG_INSERT_OR_REMOVAL_Num        3
                                                              // Bits:  1: PD Hard reset has been performed
//    #define TPS65988_INT_HARD_RESET                    TPS65988_DEF_BIT_FIELD( 1, 8)
		#define	TPS65988_INT_HARD_RESET(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT1)	//Set bit1
                                                              // Bits:  0: PD Soft reset has been performed
//    #define TPS65988_INT_SOFT_RESET                    TPS65988_DEF_BIT_FIELD( 0, 8)
		#define	TPS65988_INT_SOFT_RESET(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT0)	//Set bit0

#define TPS65988_INT_EVENT2                     0x15     // Interrupt event bit field for I2C_IRQ2 (7 bytes (RO))

#define TPS65988_INT_MASK1                      0x16     // Interrupt mask bit field corresponding to IntEevent1
                                                         //  (7 bytes (R/W))
#define TPS65988_INT_MASK2                      0x17     // Interrupt mask bit field corresponding to IntEevent1
                                                         //  (7 bytes (R/W))
#define TPS65988_INT_CLEAR1                     0x18     // Interrupt clear bit field for IntEvent1.
                                                         //  (7 bytes (R/W))
#define TPS65988_INT_CLEAR2                     0x19     // Interrupt clear bit field for IntEvent2.
                                                         //  (7 bytes (R/W))
/*
*      Status bit field for non-interrupt events (4 bytes R/O)
*/
#define TPS65988_STATUS                         0x1a
                                                              // Bits: 29: TPS65988 as Source and VBus voltage below
                                                              //           threshold
//    #define TPS65988_STATUS_LOW_VOLTAGE_WARNING       TPS65988_DEF_BIT_FIELD_IN_DWORD(29, 0)
		#define	TPS65988_STATUS_LOW_VOLTAGE_WARNING(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT5)	//Set bit29
                                                              // Bits: 28: TPS65988 as Source and VBus voltage above 
                                                              //           threshold
//    #define TPS65988_STATUS_HIGH_VOLTAGE_WARNING      TPS65988_DEF_BIT_FIELD_IN_DWORD(28, 0)
		#define	TPS65988_STATUS_HIGH_VOLTAGE_WARNING(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT4)	//Set bit28
                                                              // Bits: 27:
//    #define TPS65988_STATUS_BIST                      TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
		#define	TPS65988_STATUS_BIST(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT3)	//Set bit27
                                                              // Bits: 26: sent GotoMin as a source or
                                                              //           receive GotoMin as a sink
//    #define TPS65988_STATUS_GO_TO_MIN_ACTIVE          TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
		#define	TPS65988_STATUS_GO_TO_MIN_ACTIVE(x)	READ_SINGLE_BIT_IN_BYTES(x,3,BIT2)	//Set bit26
                                                              // Bits: 25..24: Mode of TPS65892 
//    #define TPS65988_STATUS_ACTING_AS_LEGACY_MASK     TPS65988_DEF_MBIT_FIELD_IN_DWORD(24, 2, 0)
		#define	TPS65988_STATUS_ACTING_AS_LEGACY_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,3,(BIT1+BIT0))	//Set bit25..24
        #define TPS65988_STATUS_ACTING_AS_NONE_PD         0     // TPS65988 is not in legacy (non PD mode)
        #define TPS65988_STATUS_ACTING_AS_LEGACY_SINK     1     // TPS65988 is acting like a legacy sink
        #define TPS65988_STATUS_ACTING_AS_LEGACY_SOURCE   2     // TPS65988 is acting like a legacy source
                                                              // Bits: 23..22: VBUS state of an attached device
//    #define TPS65988_STATUS_USB_HOST_PRESENT_MASK     TPS65988_DEF_MBIT_FIELD_IN_DWORD(22, 2, 0)
		#define	TPS65988_STATUS_USB_HOST_PRESENT_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,2,(BIT7+BIT6))>>6)	//Set bit23..22
    #define TPS65988_STATUS_USB_HOST_PRESENT_PD_NOT_USB 1            // VBus is being provided by an end device that 
                                                                     //   is a PD device not capable of USB
                                                                     //   communication
    #define TPS65988_STATUS_USB_HOST_PRESENT_NOT_PD   2              // VBus is being provided by an end device that 
                                                                     //   is not a PD device.
    #define TPS65988_STATUS_USB_HOST_PRESENT_PD       3              // VBus is being provided by an end device that 
                                                                     //   is a PD device capable of USB communications
                                                              // Bits: 21.20: VBus state
//    #define TPS65988_STATUS_VBUS_STATUS_MASK          TPS65988_DEF_MBIT_FIELD_IN_DWORD(20, 2, 0)
		#define	TPS65988_STATUS_VBUS_STATUS_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,2,(BIT5+BIT4))>>4)	//Set bit21..20
    #define TPS65988_STATUS_VBUS_STATUS_SAFE_0V       0              // VBUS is at vSafe0V (less than 0.8V)
    #define TPS65988_STATUS_VBUS_STATUS_SAFE_5V       1              // VBUS is at vSafe5V (4.75V to 5.5V)
    #define TPS65988_STATUS_VBUS_STATUS_NEGOCIATE_LEVEL 2            // VBUS is at negotiated power level
    #define TPS65988_STATUS_VBUS_STATUS_OTHER         3              // VBUS is not within any of previous rages given
                                                              // Bits: 19.18: Power source of TPS65988
//    #define TPS65988_STATUS_POWER_SOURCE_MASK         TPS65988_DEF_MBIT_FIELD_IN_DWORD(18, 2, 0)
		#define	TPS65988_STATUS_POWER_SOURCE_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,2,(BIT3+BIT2))>>2)	//Set bit19..18
    #define TPS65988_STATUS_POWER_SOURCE_VIN_3V3      1
    #define TPS65988_STATUS_POWER_SOURCE_PP_CABLE     2
    #define TPS65988_STATUS_POWER_SOURCE_VBUS         3
                                                              // Bits: 16: PP_CABLE and/or selected output switch
                                                              //           is in over current
//    #define TPS65988_STATUS_OVER_CURRENT              TPS65988_DEF_BIT_FIELD_IN_DWORD(16, 0)
		#define	TPS65988_STATUS_OVER_CURRENT(x)	READ_SINGLE_BIT_IN_BYTES(x,2,BIT0)	//Set bit16
                                                              // Bits: 15.14: State of PP_CABLE switch
//    #define TPS65988_STATUS_PP_CABLE_SWITCH_MASK      TPS65988_DEF_MBIT_FIELD_IN_DWORD(14, 2, 0)
		#define	TPS65988_STATUS_PP_CABLE_SWITCH_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,1,(BIT7+BIT6))>>6)	//Set bit15..14
    #define TPS65988_STATUS_PP_CABLE_SWITCH_OV        1              // PP_CABLE switch is disabled due to overcurrent
    #define TPS65988_STATUS_PP_CABLE_SWITCH_C_CC1     2              // PP_CABLE switch enabled on C_CC1
    #define TPS65988_STATUS_PP_CABLE_SWITCH_C_CC2     3              // PP_CABLE switch enabled on C_CC2
                                                              // Bits: 13.12: State of PP_EXT switch
//    #define TPS65988_STATUS_PP_EXT_SWITCH_MASK        TPS65988_DEF_MBIT_FIELD_IN_DWORD(12, 2, 0)
		#define	TPS65988_STATUS_PP_EXT_SWITCH_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,1,(BIT5+BIT4))>>4)	//Set bit13..12
    #define TPS65988_STATUS_PP_EXT_SWITCH_OV          1              // PP_EXT switch is disabled due to overcurrent
    #define TPS65988_STATUS_PP_EXT_SWITCH_ENALE_OUT   2              // PP_EXT switch enabled, reverse current from
                                                                     //    VBUS to PP_EXT blocked
    #define TPS65988_STATUS_PP_EXT_SWITCH_ENALE_IN    3              // PP_EXT switch enabled, reverse current from
                                                                     //    PP_EXT to VBUS blocked
                                                              // Bits: 11.10: State of PP_HV switch
//    #define TPS65988_STATUS_PP_HV_SWITCH_MASK         TPS65988_DEF_MBIT_FIELD_IN_DWORD(10, 2, 0)
		#define	TPS65988_STATUS_PP_HV_SWITCH_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,1,(BIT3+BIT2))>>2)	//Set bit11..10
    #define TPS65988_STATUS_PP_HV_SWITCH_OV           1              // PP_HV switch is disabled due to overcurrent
    #define TPS65988_STATUS_PP_HV_SWITCH_ENALE_OUT    2              // PP_HV switch enabled, reverse current from
                                                                     //    VBUS to PP_HV blocked
    #define TPS65988_STATUS_PP_HV_SWITCH_ENALE_IN     3              // PP_HV switch enabled, reverse current from
                                                                     //    PP_HV to VBUS blocked
                                                              // Bits: 9..8: State of PP_5V0 switch
//    #define TPS65988_STATUS_PP_5V0_SWITCH_MASK        TPS65988_DEF_MBIT_FIELD_IN_DWORD( 8, 2, 0)
		#define	TPS65988_STATUS_PP_5V0_SWITCH_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,1,(BIT1+BIT0))	//Set bit9..8
    #define TPS65988_STATUS_PP_5V0_SWITCH_OV          1              // PP_5V0 disabled due to overcurrent
    #define TPS65988_STATUS_PP_5V0_SWITCH_ENABLE      2              // PP_5V0 enabled, reverse from VBUS to
                                                                     //    PP_5V0 blocked
                                                              // Bits:  7: VCONN power enabled
//    #define TPS65988_STATUS_VCONN_ENABLE              TPS65988_DEF_BIT_FIELD_IN_DWORD( 7, 0)
		#define	TPS65988_STATUS_VCONN_ENABLE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT7)	//Set bit7
                                                              // Bits:  6: TPS65988 is DFP
//    #define TPS65988_STATUS_DATA_ROLE                 TPS65988_DEF_BIT_FIELD_IN_DWORD( 6, 0)
		#define	TPS65988_STATUS_DATA_ROLE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT6)	//Set bit6
                                                              // Bits:  5: C_CCx pull-up active
//    #define TPS65988_STATUS_PORT_ROLE                 TPS65988_DEF_BIT_FIELD_IN_DWORD( 5, 0)
		#define	TPS65988_STATUS_PORT_ROLE(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT5)	//Set bit5
                                                              // Bits:  4: Upside-down orientation (plug CC on C_CC2)
//    #define TPS65988_STATUS_PLUG_ORIENTATION          TPS65988_DEF_BIT_FIELD_IN_DWORD( 4, 0)
		#define	TPS65988_STATUS_PLUG_ORIENTATION(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT4)	//Set bit4
                                                              // Bits: 3..1: State of Connection
//    #define TPS65988_STATUS_CONN_STATE_MASK           TPS65988_DEF_MBIT_FIELD_IN_DWORD( 1, 3, 0)
		#define	TPS65988_STATUS_CONN_STATE_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,0,(BIT3+BIT2+BIT1))>>1)	//Set bit3..1
    #define TPS65988_STATUS_CONN_STATE_NOT_CONNECT    0              // No connection
    #define TPS65988_STATUS_CONN_STATE_PORT_DISABLED  1              // Port is disabled
    #define TPS65988_STATUS_CONN_STATE_AUDIO          2              // Audio connection (Ra/Ra)
    #define TPS65988_STATUS_CONN_STATE_DEBUG          3              // Debug connection (Rd/Rd)
    #define TPS65988_STATUS_CONN_STATE_NOT_CONNECT_RA 4              // No connection, Ra detected
    #define TPS65988_STATUS_CONN_STATE_CONNECT_NO_RA  6              // connection present, no Ra detected
                                                                     //   (Rd but no Ra)
    #define TPS65988_STATUS_CONN_STATE_CONNECT_RA     7              // connection present, Ra detected
                                                                     //   (Rd and Ra detected)
                                                              // Bits:  0: connection present
//    #define TPS65988_STATUS_PLUG_PRESENT              TPS65988_DEF_BIT_FIELD_IN_DWORD( 0, 0)
		#define	TPS65988_STATUS_PLUG_PRESENT(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT0)	//Set bit0

#define TPS65988_SYSTEM_POWER_STATE             0x20     // System power state, 0x00=S0, 0x03=S3, 0x04=S4, 0x05=S5
                                                         //  (1 byte (R/W))

#define TPS65988_GLOBAL_SYSTEM_CONFIG                  0x27
/*
*      Configuration bits depending on in the TPS65988
*/
#define TPS65988_SYSTEM_CONFIG                  0x28
                                                         //  Not expected to change during normal operation
                                                         //  (10 bytes (R/W))
//    #define TPS65988_SYSTEM_CONFIG_UART_DISABLED         TPS65988_DEF_BIT_FIELD(79, 10)
		#define	TPS65988_SYSTEM_CONFIG_UART_DISABLED(x)	READ_SINGLE_BIT_IN_BYTES(x,9,BIT7)	//Set bit79
                                                              //Bits: 72..70: Under voltage protection usage of HV
//    #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_MASK     TPS65988_DEF_MBIT_FIELD(70, 3, 10)
		#define	TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_MASK(x)	((READ_MULTI_BIT_IN_BYTES(x,9,BIT0)<<2)|(READ_MULTI_BIT_IN_BYTES(x,8,(BIT7+BIT6))>>6))	//Set bit72..70
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_5     0          // 5%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_10    1          // 10%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_15    2          // 15%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_20    3          // 20%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_25    4          // 25%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_30    5          // 30%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_40    6          // 40%
        #define TPS65988_SYSTEM_CONFIG_UVP_USAGE_HV_50    7          // 50%
                                                              // Bits: 69..67: Under voltage protection of VBUS
//    #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V     TPS65988_DEF_MBIT_FIELD(67, 3, 10)
		#define	TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V(x)	(READ_MULTI_BIT_IN_BYTES(x,8,(BIT5+BIT4+BIT3))>>3)	//Set bit69..67
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_5  0        // 5% 
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_10 1        // 10%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_15 2        // 15%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_20 3        // 20%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_25 4        // 25%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_30 5        // 30%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_40 6        // 40%
        #define TPS65988_SYSTEM_CONFIG_UVP_TRIP_POINT_5V_50 7        // 50%
                                                              // Bits: 66: Under-Voltage Protection is set to 4.5V (1)
                                                              //         UVP is determined by UvpTrip5V or UvpUsaveHV
//    #define TPS65988_SYSTEM_CONFIG_SET_UVP_TO_4P5V       TPS65988_DEF_BIT_FIELD(66, 10)
		#define	TPS65988_SYSTEM_CONFIG_SET_UVP_TO_4P5V(x)	READ_SINGLE_BIT_IN_BYTES(x,8,BIT2)	//Set bit66
                                                              // Bits: 62: Enable VOUT_3V3 automatically
//    #define TPS65988_SYSTEM_CONFIG_VOUT_3V3_ENABLE       TPS65988_DEF_BIT_FIELD(62, 10)
		#define	TPS65988_SYSTEM_CONFIG_VOUT_3V3_ENABLE(x)	READ_SINGLE_BIT_IN_BYTES(x,7,BIT6)	//Set bit62
                                                              // Bits: 61..59: RESETZ assert when VIN_3V3 is below
                                                              //            folloing voltage while VOUT_3V3 is enabled
//    #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH    TPS65988_DEF_MBIT_FIELD(59, 3, 10)
		#define	TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH(x)	(READ_MULTI_BIT_IN_BYTES(x,7,(BIT5+BIT4+BIT3))>>3)	//Set bit61..59
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_1125 0     // 1.125V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2250 1     // 2.25V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2375 2     // 2.375V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2500 3     // 2.5V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2625 4     // 2.625V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2750 5     // 2.75V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_2875 6     // 2.875V
        #define TPS65988_SYSTEM_CONFIG_VOUT3V3_SUP_THRESH_3000 7     // 3.0V
                                                              // Bits: 58..57: Clock of RESETZ Timer
//    #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK  TPS65988_DEF_MBIT_FIELD(57, 2, 10)
		#define	TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK(x)	(READ_MULTI_BIT_IN_BYTES(x,7,(BIT2+BIT1))>>1)	//Set bit58..57
        #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK_160      // 160us
        #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK_640      // 640us
        #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK_1280     // 1.28ms
        #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_CLOCK_5120     // 5.12ms
                                                              // Bits: 56..51: RESETZ Release Timing Clock Count
//    #define TPS65988_SYSTEM_CONFIG_RESETZ_TIMEOUT_COUNT  TPS65988_DEF_MBIT_FIELD(51, 6, 10)
                                                              // Bits: 50: Enable time out counter of PP_EXT
//    #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_ENABLE TPS65988_DEF_BIT_FIELD(50, 10)
                                                              // Bits: 49..46: Overcurrent timeout for PP_EXT
//    #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT     TPS65988_DEF_MBIT_FIELD(46, 4, 10)
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_10    0     // 10us
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_20    1     // 20us
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_80    2     // 80us
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_160   3     // 160us
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_640   4     // 640us
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_1280  5     // 1.28ms
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_5120  6     // 5.12ms
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_10240 7     // 10.24ms
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_40960 8     // 40.96ms
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_OC_TIMEOUT_81920 9     // 81.92ms
                                                              // Bits: 43: Billboard supported
//    #define TPS65988_SYSTEM_CONFIG_BILLBOARD_ALLOWED     TPS65988_DEF_BIT_FIELD(43, 10)
                                                              // Bits: 42: Type-C state machine supports Try.SRC
                                                              //        and Try.SNKWait
//    #define TPS65988_SYSTEM_CONFIG_TRY_SRC_SUPPORT       TPS65988_DEF_BIT_FIELD(42, 10)
                                                              // Bits: 41: External sense register value (5M/10ohm)
//    #define TPS65988_SYSTEM_CONFIG_RSENSE                TPS65988_DEF_BIT_FIELD(41, 10)
                                                              // Bits: 40: Support Powered Accessory
//    #define TPS65988_SYSTEM_CONFIG_POWERED_ACCESSORY_SUPOORT TPS65988_DEF_BIT_FIELD(40, 10)
                                                              // Bits: 39: Support Debug Accessory
//    #define TPS65988_SYSTEM_CONFIG_DEBUG_ACCESSORY_SUPOORT TPS65988_DEF_BIT_FIELD(39, 10)
                                                              // Bits: 38: Support Audio Accessory
//    #define TPS65988_SYSTEM_CONFIG_AUDIO_ACCESSORY_SUPOORT TPS65988_DEF_BIT_FIELD(38, 10)
                                                              // Bits: 36..35: Rate of USB 3
//    #define TPS65988_SYSTEM_CONFIG_USB_3_RATE            TPS65988_DEF_MBIT_FIELD(36, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_USB_3_RATE_NOT_SUPPORTED 0        // USB3 not supoorted
        #define TPS65988_SYSTEM_CONFIG_USB_3_RATE_GEN1    1              // USB3 Gen1 signaling rate supported
        #define TPS65988_SYSTEM_CONFIG_USB_3_RATE_GEN2    2              // USB3 Gen2 signaling rate supported
                                                              // Bits: 34..33: USB Path
//    #define TPS65988_SYSTEM_CONFIG_USB_PATH              TPS65988_DEF_MBIT_FIELD(33, 2, 10)
		#define	TPS65988_SYSTEM_CONFIG_USB_PATH(x)	(READ_MULTI_BIT_IN_BYTES(x,4,(BIT2+BIT1))>>1)	//Set bit34..33
        #define TPS65988_SYSTEM_CONFIG_USB_PATH_DISABLE   0              // USB RP and EP paths not enabled
        #define TPS65988_SYSTEM_CONFIG_USB_PATH_RP        1              // USB RP path enabled
        #define TPS65988_SYSTEM_CONFIG_USB_PATH_EP        2              // USB EP path enabled
        #define TPS65988_SYSTEM_CONFIG_USB_PATH_BOTH_ENABLE 3            // USB RP and EP path enabled
                                                              // Bits: 32: BC1.2 checks enabled
//    #define TPS65988_SYSTEM_CONFIG_BC12_ENABLE           TPS65988_DEF_BIT_FIELD(32, 10)
                                                              // Bits: 30..28: Configuration of PP_EXT switch
//    #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG         TPS65988_DEF_MBIT_FIELD(28, 3, 10)
		#define	TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG(x)	(READ_MULTI_BIT_IN_BYTES(x,3,(BIT6+BIT5+BIT4))>>4)	//Set bit30..28
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_DISABLE                    0 // PP_EXT is disabled
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_OUTPUT                     1 // PP_EXT as output
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_INPUT                      2 // PP_EXT as input
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_INPUT_WAIT_SRDY            3 // PP_EXT as input but wait for
                                                                                  //    SYS_RDY
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_INPUT_AND_OUTPUT           4 // PPEXT as input and output
        #define TPS65988_SYSTEM_CONFIG_PP_EXT_CONFIG_INPUT_AND_OUTPUT_WAIT_SRDY 5 // PPEXT as input and output but
                                                                                  //    wait for SYS_RDY
                                                              // Bits: 27..26: Configuration of PP_HV switch
//    #define TPS65988_SYSTEM_CONFIG_PP_HV_CONFIG          TPS65988_DEF_MBIT_FIELD(26, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_PP_HV_CONFIG_DISABLE         0   // PP_HV switch is diabled
        #define TPS65988_SYSTEM_CONFIG_PP_HV_CONFIG_OUTPUT          1   // PP_HV as output
        #define TPS65988_SYSTEM_CONFIG_PP_HV_CONFIG_INPUT           2   // PP HV as input
        #define TPS65988_SYSTEM_CONFIG_PP_HV_CONFIG_INPUT_WAIT_SRDY 3   // PP_HV as input but wait for SYS_RDY
                                                              // Bits: 25..24: Configuration of PP_5V0 switch
//    #define TPS65988_SYSTEM_CONFIG_PP_5V0_CONFIG         TPS65988_DEF_MBIT_FIELD(24, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_PP_5V0_CONFIG_DISABLE 0          // PP_5V0 not used
        #define TPS65988_SYSTEM_CONFIG_PP_5V0_CONFIG_OUTPUT  1          // PP_5V0 as output
                                                              // Bits: 23..22: Usage of Over Voltage Protection (OVP)
//    #define TPS65988_SYSTEM_CONFIG_OVP_USAGE             TPS65988_DEF_MBIT_FIELD(22, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_OVP_USAGE_OVP_TRIP_POINT 0       // disconnected if VBUS exceeds OvpTripPoint
        #define TPS65988_SYSTEM_CONFIG_OVP_USAGE_5              1       // disconnected if VBUs exceeds 105% of max. voltage
        #define TPS65988_SYSTEM_CONFIG_OVP_USAGE_10             2       // disconnected if VBUs exceeds 110% of max. voltage
        #define TPS65988_SYSTEM_CONFIG_OVP_USAGE_15             3       // disconnected if VBUs exceeds 115% of max. voltage
                                                              // Bits: 21..16: voltage of VBus exceeds this value and 
                                                              //            OvpUsage is 00, then VBUS will be
                                                              //            disconnected from the input rails.
                                                              //            000000b means disabled.
                                                              //            volatge is calculated as follows
                                                              //           this_value X 0.32V + 3.84V : 4.16V - 24V
//    #define TPS65988_SYSTEM_CONFIG_OVP_TRIP_POINT        TPS65988_DEF_MBIT_FIELD(21, 6, 10)
                                                              // Bits: 15: Status.LowVoltageWarning is set if voltage of
                                                              //        soutrce and VBus less than 80/90% of nominal one
//    #define TPS65988_SYSTEM_CONFIG_LOW_VOLTAGE_WARING_LEVEL TPS65988_DEF_BIT_FIELD(15, 10)
                                                              // Bits: 14: Status.HighVoltageWarning is set if voltage of
                                                              //        soutrce and VBus greater than 120/110% of nominal one
//    #define TPS65988_SYSTEM_CONFIG_HIGH_VOLTAGE_WARING_LEVEL TPS65988_DEF_BIT_FIELD(14, 10)
                                                              // Bits: 9..8: Configuration for VCONN switches.
//    #define TPS65988_SYSTEM_CONFIG_VCONN_SUPPORT         TPS65988_DEF_MBIT_FIELD( 8, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_VCONN_SUPPORT_DISABLE    0        // VCONN not supported
        #define TPS65988_SYSTEM_CONFIG_VCONN_SUPPORT_AS_DFP     2        // VCONN supported as DFP only
        #define TPS65988_SYSTEM_CONFIG_VCONN_SUPPORT_AS_DFP_UFP 3        // VCONN supported as DFP/UFP
                                                              // Bits: 7..6: Type-C current advertisement (pull down
                                                              //          enabled and active strength), don't care
                                                              //          if a source is not
//    #define TPS65988_SYSTEM_CONFIG_TYPE_C_CURRENT        TPS65988_DEF_MBIT_FIELD( 6, 2, 10)
        #define TPS65988_SYSTEM_CONFIG_TYPE_C_CURRENT_DEFAULT 0          // Default current (weakest pullup)
        #define TPS65988_SYSTEM_CONFIG_TYPE_C_CURRENT_1_5_A   1          // 1.5A (medium pullup)
        #define TPS65988_SYSTEM_CONFIG_TYPE_C_CURRENT_3A      2          // 3A (strongest pullup)
                                                              // Bits: 5..3: Specifies USB-C connection to TPS65988
//    #define TPS65988_SYSTEM_CONFIG_RECEPTABLE_TYPE       TPS65988_DEF_MBIT_FIELD( 3, 3, 10)
        #define TPS65988_SYSTEM_CONFIG_RECEPTABLE_TYPE_STANDARD_USB2_ONLY    0 // Stanrd USB2-only USB-C receptable
        #define TPS65988_SYSTEM_CONFIG_RECEPTABLE_TYPE_STANDARD_FULL_FEATURE 1 // Stanrd fully-featured USB-C receptable
        #define TPS65988_SYSTEM_CONFIG_RECEPTABLE_TYPE_TETHERED_USB2_ONLY    2 // Tethered USB2-only cable with USB-C plug
        #define TPS65988_SYSTEM_CONFIG_RECEPTABLE_TYPE_TETHERED_FULL_FEATURE 3 // Tethered fully-featured cable with USB-C plug
                                                              // Bits: 2..0: Defines the USB PD and Type-C roles of TPS65988
//    #define TPS65988_SYSTEM_CONFIG_PORT_INFO             TPS65988_DEF_MBIT_FIELD( 0, 3, 10)
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SINK_UFP            0   // Power role = Sink, Data roll = UFP
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SINK_WITH_ACCESSORY 1   // Power role = Sink, Data roll = UFP
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SINK_SOURCE_UFP     2   // Power role = Sink, Data roll = UFP,
                                                                         //    PR_Swap supported
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SINK_SOURCE_UFP_DFP 3   // Power role = Sink, Data roll = UFP,
                                                                         //    PR_Swap & DR_swap supported
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SOURCE_SINK_DFP     4   // Power role = Sink, Data roll = DFP,
                                                                         //    PR_Swap supported
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SOURCE_SINK_DFP_UFP 5   // Power role = Sink, Data roll = DFP,
                                                                         //    PR_Swap & DR_swap supported
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_SOURCE              6   // Power role = Source
        #define TPS65988_SYSTEM_CONFIG_PORT_INFO_DISABLE             7   // Port is disabled

/*
*       Configuration bits depending on current system policy (5 bytes R/W)
*/
#define TPS65988_CONTROL_CONFIG                 0x29
#define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SOURCE  0x80
#define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SINK  0x20
#define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_DFP  0x80
#define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_DFP  0x40
#define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_UFP  0x20
#define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_UFP  0x10
                                                              // Bits 32-34: Reset the I2C slave if the I2C timeout is exceeded
//    #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT         TPS65988_DEF_MBIT_FIELD_IN_BYTE(32, 3, 4)
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_25MS  0
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_50MS  1
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_75MS  2
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_100MS 3
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_125MS 4
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_150MS 5
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_175MS 6
        #define TPS65988_CONTROL_CONFIG_I2C_TIME_OUT_200MS 7
                                                              // Bits 31: Force G1 operation
//    #define TPS65988_CONTROL_CONFIG_FORCE_USB3_GEN1      TPS65988_DEF_BIT_FIELD_IN_DWORD(31, 0)
                                                              // Bits 25: Do not issue any Enter Mode Structured VDM
                                                              //       Commands automatically /
                                                              //       Do not operate in Alternate Mode intrusive mode.
//    #define TPS65988_CONTROL_CONFIG_AM_INTRUSIVE_MODE    TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
                                                              // Bits 24: TPS65988 wiil automatically issue Discover
                                                              //       Identify VDM when DFP, to SOP', SOP'', SOP and
                                                              //       SOP*_Debug when appropriate /                 
                                                              //       TPS65988 wiil not automatically issue Discover
                                                              //       Identify VDM when DFP
//    #define TPS65988_CONTROL_CONFIG_AUTOMATIC_ID_REQUEST TPS65988_DEF_BIT_FIELD_IN_DWORD(24, 0)
                                                              // Bits 15: TPS65988 automatically initiates and sends
                                                              //       DR_Swap request to devices if operate as UFP
//    #define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_DFP TPS65988_DEF_BIT_FIELD_IN_DWORD(15, 0)
                                                              // Bits 14: TPS65988 automatically accepts DR_Swap
                                                              //       request from devices if operate as UFP
//    #define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_DFP  TPS65988_DEF_BIT_FIELD_IN_DWORD(14, 0)
                                                              // Bits 13: TPS65988 automatically initiates and sends
                                                              //       DR_Swap request to devices if operate as DFP
//    #define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_UFP TPS65988_DEF_BIT_FIELD_IN_DWORD(13, 0)
                                                              // Bits 12: TPS65988 automatically accepts DR_Swap
                                                              //       request from devices if operate as DFP
//    #define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_UFP  TPS65988_DEF_BIT_FIELD_IN_DWORD(12, 0)
                                                              // Bits 11: TPS65988 automatically initiates and sends
                                                              //       VCONN_Swap request to devices
//    #define TPS65988_CONTROL_CONFIG_INITIATE_VCONN_SWAP  TPS65988_DEF_BIT_FIELD_IN_DWORD(11, 0)
                                                              // Bits 10: TPS65988 automatically accepts VCONN_Swap
                                                              //       request from devices
//    #define TPS65988_CONTROL_CONFIG_PROCESS_VCONN_SWAP   TPS65988_DEF_BIT_FIELD_IN_DWORD(10, 0)
                                                              // Bits 9: Do not automatically issue Request in
                                                              //      response to any Source Capabilities
//    #define TPS65988_CONTROL_CONFIG_PDO_INTRUSIVE_MODE   TPS65988_DEF_BIT_FIELD_IN_DWORD( 9, 0)
                                                              // Bits 8: Send a Wait in response to any RDO, unless
                                                              //      PRDO or ARDO command has been issued since
                                                              //      last time a Source Capabilities message has
                                                              //      been sent
//    #define TPS65988_CONTROL_CONFIG_RDO_INTRUSIVE_MODE   TPS65988_DEF_BIT_FIELD_IN_DWORD( 8, 0)
                                                              // Bits 7: TPS65988 automatically initiates and sends
                                                              //      PR_Swap request to devices if operate as
                                                              //      SINK/SOURCE
//    #define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SOURCE TPS65988_DEF_BIT_FIELD_IN_DWORD( 7, 0)
                                                              // Bits 6: TPS65988 automatically accepts PR_Swap
                                                              //      request from devices
//    #define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_SOURCE TPS65988_DEF_BIT_FIELD_IN_DWORD( 6, 0)
                                                              // Bits 5: TPS65988 automatically initiates and sends
                                                              //      PR_Swap request to devices if operate as
                                                              //      SOURCE/SINK
//    #define TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SINK TPS65988_DEF_BIT_FIELD_IN_DWORD( 5, 0)
                                                              // Bits 4: TPS65988 automatically accepts PR_Swap
                                                              //      request from devices if operates as Source/Sink
//    #define TPS65988_CONTROL_CONFIG_PROCESS_SWAP_TO_SINK TPS65988_DEF_BIT_FIELD_IN_DWORD( 4, 0)
                                                              // Bits 2: The system is recieving external power from
                                                              //      a source other than VBUS for this TPS65988
//    #define TPS65988_CONTROL_CONFIG_EXTERNALLY_POWERED   TPS65988_DEF_BIT_FIELD_IN_DWORD( 2, 0)
                                                              // Bits 1..0: Disable PD feature
//    #define TPS65988_CONTROL_CONFIG_DISABLE_PD           TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 2, 0)
        #define TPS65988_CONTROL_CONFIG_DISABLE_PD_NORMAL      0     // maintain normal USB PD behaivior  
        #define TPS65988_CONTROL_CONFIG_DISABLE_PD_STOP_SOURCE 1     // Stop USB PD activities and behave like
                                                                     //    a Legacy USB source
        #define TPS65988_CONTROL_CONFIG_DISABLE_PD_STOP_SINK   2     // Stop USB PD activities and behave like
                                                                     //    a Legacy USB sink.
/*
*       Byte 0-3: Boot Flags (32bit little-endian)       
*       Byte 4-7: OTP configuration (32bit little-endian)
*       Byte 8-11: Hardware Device ID                    
*        (12 bytes (R/O))                                
*/
#define TPS65988_BOOT_FLAG_OTP_CONFIG           0x2d
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_UART_TIMEOUT_ERR           TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(31, 0)
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_UART_RETRY_ERR             TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(30, 0)
                                                                     // Bits 29: PP_HV enabled during dead battery
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_INT_PHV_SWITCH             TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(29,0)
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_UART_OVER_FLOW_ERR         TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(28, 0)
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_UART_BOOT                  TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(27, 0)
                                                                     // Bits 26..24Device number based on I2C_ADDR
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEV_NUMBER_MASK            TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(24, 0)
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEBUG_CTL_BITS_MASK        TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(22, 0)
                                                                     // Bits 21..17: OTP portion of All-Call I2C
                                                                     //              address
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_ALL_CALL_I2C_OPT_BITS_MASK TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(17, 0)
                                                                     // Bits 15..16: OTP portion of Single-Call I2C
                                                                     //              address
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_ONE_CALL_I2C_OPT_BITS_MASK TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(15, 0)
                                                                     // Bits 14: customer OTP bits are valid
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_CUSTOMER_OTP_INVALID       TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(14, 0)
                                                                      // Bits 13: CRC of read data from Region 1 of
                                                                     //          SPI memory failed
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION1_CRC_FAIL           TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(13, 0)
                                                                     // Bits 12: CRC of read data from Region 0 of
                                                                     //          SPI memory failed
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION0_CRC_FAIL           TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(12, 0)
                                                                     // Bits 11:CRC of read data from UART failed
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_UART_CRC_FAIL              TPS65988_DEF_BIT_FIELD_IN_DWORD_LE(11, 0)
                                                                     // Bits 9: An error occurred attempting to read
                                                                     //         Region 1 of SPI memory. A retry may
                                                                     //         have been successfull
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION1_FLASH_ERR          TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 9, 0)
                                                                     // Bits 8: An error occurred attempting to read
                                                                     //         Region 0 of SPI memory. A retry may
                                                                     //         have been successfull
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION0_FLASH_ERR          TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 8, 0)
                                                                     // Bits 7: Region 1 of SPI memory was invalid
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION1_INVALID            TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 7, 0)
                                                                     // Bits 6: Region 0 of SPI memory was invalid
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION0_INVALID            TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 6, 0)
                                                                     // Bits 5: Region 1 of SPI memory was attempted
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION1                    TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 5, 0)
                                                                     // Bits 4: Region 0 of SPI memory was attempted
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_REGION0                    TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 4, 0)
                                                                     // Bits 3: a SPI memory is attached
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_SPI_FLASH_PRESENT          TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 3, 0)
                                                                     // Bits 2: Booted in dead battery condition
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEAD_BATTERY               TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 2, 0)
		#define	TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEAD_BATTERY(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT2)	//Set bit2
                                                                     // Bits 1: PP_EXT enabled during dead battery
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_EXT_PHV_SWITCH             TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 1, 0)
                                                                     // Bits 0: SRAM has been loaded and is valid
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_OK                         TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 0, 0)

                                                                     // Bits 22..18: Voltage at VOUT_3V3 above which
                                                                     //              RESETZ clears
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_VOUT_3V3_THREHOLD_MASK      TPS65988_DEF_MBIT_FIELD_IN_DWORD_LE(18, 5, 4)
                                                                     // Bits 9..8: set portion of Unique I2C address
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_ONE_CALL_I2C_OPT_BITS_MASK  TPS65988_DEF_MBIT_FIELD_IN_DWORD_LE( 8, 2, 4)
                                                                     // Bits 5: Wait for VIN3V3 before completing
                                                                     //         device initialization
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_WAIT_FOR_VIN_3V3            TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 5, 4)
                                                                     // Bits 4: VOUT_3V3 is turned on if attached to
                                                                     //         Flashmemory and App code load fail
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_VOUT_3V3_CTL                TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 4, 4)
                                                                     // Bits 2: Disable connection of SWD input to
                                                                     //         SBU pins on Type-C port
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_SWD_DISABLE                 TPS65988_DEF_BIT_FIELD_IN_DWORD_LE( 2, 4)
                                                                     // Bits 0: 10b indiacte valid OTP configuration.
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_OTP_VALID_MASK                  TPS65988_DEF_MBIT_FIELD_IN_DWORD_LE( 0, 2, 4)
//    #define TPS65988_BOOT_FLAG_OTP_CONFIG_HARDWARE_REVISION_ID_MASK       TPS65988_DEF_MBIT_FIELD_IN_DWORD_LE( 0, 8, 8)

#define TPS65988_BUILD_IDENTIFIER               0x2e     // ASCII string returns uniquely identifying custom build
                                                         // information. 40 hex characters representing the build + 1
                                                         // underscore character + MMDDYYYY (build data) + null
                                                         //  (49 bytes (RO))
#define TPS65988_DEVICE_INFO                    0x2f     // ASCII string
                                                         //  (36 bytes (RO))
#define TPS65988_RX_SOURCE_CAPABILITIES         0x30     // Latest source capabilities message received over BMC
                                                         //  (29 bytes (RO))
                                                                     // Bits 2..0: Number of valid PDOs in
                                                                     //            register (1-7)
//    #define TPS65988_RX_SOURCE_CAPABILITIES_NUM_VALID_PDOS_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
                                                                     // offset 1-28: Source Capability 
                                                                     //              (NumValidPDOs X 4)
#define TPS65988_RX_SINK_CAPABILITIES           0x31     // Latest sink capabilities message received over BMC
                                                         //  (29 bytes (RO))
                                                                     // Bits 2..0: Number of valid PDOs in
                                                                     //            register (1-7)
//    #define TPS65988_RX_SINK_CAPABILITIES_NUM_VALID_PDOS_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)

#define TPS65988_TX_SOURCE_CAPABILITIES         0x32     // Source capabilities contents to send over BMC
                                                         //  (31 bytes (RW))
                                                                     // Bits 2..0: Number of valid PDOs in
                                                                     //            register (1-7)
//    #define TPS65988_TX_SOURCE_CAPABILITIES_NUM_VALID_PDOS_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
		#define	TPS65988_TX_SOURCE_CAPABILITIES_NUM_VALID_PDOS_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,0,0x07)	//Set bit0..2
                                                                     // Bits 7..2: when bit is 1, corresponding PDO
                                                                     //         will be advertised when bit is 0,
                                                                     //         corresponding PDO will only be
                                                                     //         advertized when EXternally Powered bit
                                                                     //         is 1. The first 5-V PDO (PDO1) is
                                                                     //         always advertized. Therefore these
                                                                     //         bits apply to PDO2 through PDO7.
//    #define TPS65988_TX_SOURCE_CAPABILITIES_ADVERTISED_PDO_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 2, 6, 1)
		#define	TPS65988_TX_SOURCE_CAPABILITIES_ADVERTISED_PDO_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,1,0xFC)>>2)	//Set Byte1 bit2..7
                                                                     // Bits 7..2: When bit is 0, PP_HV source
                                                                     //         the correspondingPDO. when bit is 1,
                                                                     //         PP_EXT sourcecorresponding PDO (switch
                                                                     //         must be configuredas output in System
                                                                     //         Configuration.PP_HVconfig)
//    #define TPS65988_TX_SOURCE_CAPABILITIES_PDO_SOURCE_FIELD_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 2, 6, 2)
                                                                     // Bits 1: When PDO0NotSourcedbyPP5V0 is 1.
                                                                     //      behaves like bits 7..2 to determine which
                                                                     //      of PP_HV or PP_EXT sources PDO0
//    #define TPS65988_TX_SOURCE_CAPABILITIES_PDO0_HV_SOURCE TPS65988_DEF_BIT_FIELD_IN_BYTE( 1, 2)
                                                                     // Bits 0: When bit is 0, PP_5V0 sources PDO0,
                                                                     //      otherwisesourced by PP_*switch indicated
                                                                     //      in PDOHVSource
//    #define TPS65988_TX_SOURCE_CAPABILITIES_PDO0_NOT_SOURCEDBY_PP_5V0 TPS65988_DEF_BIT_FIELD_IN_BYTE( 0, 2)
#define TPS65988_TX_SINK_CAPABILITIES           0x33     // Sink capabilities contents to send over BMC
                                                         //  (29 bytes (RW))
                                                                     // Bits 2..0: Number of valid PDOs in
                                                                     //            register (1-7)
//    #define TPS65988_TX_SINK_CAPABILITIES_NUM_VALID_PDOS_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
		#define	TPS65988_TX_SINK_CAPABILITIES_NUM_VALID_PDOS_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,0,0x07)	//Set bit0..2
#define TPS65988_ACTIVE_CONTRACT_PDO            0x34     // Byte 1-4: PDO of current contract
                                                         // Byte 5-6: Contains bit 29..20 of the first PDO
                                                         //  (6 bytes (RO))
#define TPS65988_ACTIVE_CONTRACT_RDO            0x35     // Contains RDO of current contract
                                                         //  (4 bytes (RO))
#define TPS65988_SINK_REQUEST_RDO               0x36     // Recent RDO sent by SINK
                                                         //  (4 bytes (RO))
/*
*      Auto Negotiate Sink (17 bytes (R/W)
*          offset  0:    Auto-negotiate control and RDO flags
*          offset  1- 4: RDO power parameter
*          offset  5- 8: RDO current parameter
*          offset  9-12: Battery PDO parameter
*          offset 13-16: Non-Battery PDO parameters
*/
#define TPS65988_AUTO_NEGOTIATE_SINK            0x37     // (17 bytes (RW))
//    #define TPS65988_AUTO_NEGOTIATE_SINK_RDO_GIVE_BACK   TPS65988_DEF_BIT_FIELD_IN_BYTE( 7, 0)
//    #define TPS65988_AUTO_NEGOTIATE_SINK_RDO_NO_USB_SUSP TPS65988_DEF_BIT_FIELD_IN_BYTE( 6, 0)
                                                                     // Bits 5..4: Offer Priority when evaluating
                                                                     //            PDOs offered by source
//    #define TPS65988_AUTO_NEGOTIATE_SINK_OFFER_PRIORITY_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 4, 2, 0)
    #define TPS65988_AUTO_NEGOTIATE_SINK_OFFER_PRIORITY_HIGHER_CURRENT 0
    #define TPS65988_AUTO_NEGOTIATE_SINK_OFFER_PRIORITY_HIGHER_VOLTAGE 1
    #define TPS65988_AUTO_NEGOTIATE_SINK_OFFER_PRIORITY_HIGHER_POWER   2
//    #define TPS65988_AUTO_NEGOTIATE_SINK_RDO_USB_COMMON_CAPABILITY TPS65988_DEF_BIT_FIELD_IN_BYTE( 3, 0)
                                                                     // Bits 0: Auto negociate Fixed PDO below
//    #define TPS65988_AUTO_NEGOTIATE_SINK_AUTO_NGT        TPS65988_DEF_BIT_FIELD_IN_BYTE( 0, 0)
		#define	TPS65988_AUTO_NEGOTIATE_SINK_AUTO_NGT(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT0)	//Set bit0
                                                                     // Bits 19..10: Min Operating Power (250mW steps)
//    #define TPS65988_AUTO_NEGOTIATE_SINK_MIN_OPERATING_POWER_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD( 10, 10, 1)
                                                                     // Bits 19..10: Min Operating Current (10mA steps)
//    #define TPS65988_AUTO_NEGOTIATE_SINK_MIN_OPERATING_CURRENT_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD( 10, 10, 5)
                                                                     // Bits 9..0:  Maximum Power (250mW steps)
//    #define TPS65988_AUTO_NEGOTIATE_SINK_MAXIMUM_POWER_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 10, 9)
                                                                     // Bits 31..12: Peak Current
//    #define TPS65988_AUTO_NEGOTIATE_SINK_PEAK_CURRENT_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD(20, 12, 13)
                                                                     // Bits 9..0: Maximum Current (10mA steps)
//    #define TPS65988_AUTO_NEGOTIATE_SINK_MAXIMUM_CURRENT_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 10, 13)
 
/*
*       Alternate Mode Automatic Entry Sequence Register (12 bytes (R/W))
*            offset 0- 3: SVID/Mode 1
*            offset 4- 7: SVID/Mode 2
*            offset 8-11: SVID/Mode 3
*/
#define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE  0x38     // Alternate mode selection sequence (12bytes (RW))
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE1_OBJECT_POSITION_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD(16, 8, 0)
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE1_SVID_MASK            TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 16 , 0)
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE2_OBJECT_POSITION_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD(16, 8, 4)
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE2_SVID_MASK            TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 16, 4)
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE3_OBJECT_POSITION_MASK TPS65988_DEF_MBIT_FIELD_IN_DWORD(16, 16, 8)
//    #define TPS65988_ALTERNATE_MODE_ENTRY_SEQUENCE_MODE3_SVID_MASK            TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 8, 8)
/*
*       Power Status Register (2 bytes (R/O))
*/
#define TPS65988_POWER_STATUS                   0x3f     // data consumed by system policy manager (2 bytes (RO))
                                                                     // Bits 6..5: B12 Status
//    #define TPS65988_POWER_STATUS_BC12_STATUS_MASK       TPS65988_DEF_MBIT_FIELD_IN_WORD( 5, 2, 0)
		#define	TPS65988_POWER_STATUS_BC12_STATUS_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,0,0x60)>>5)	//Set bit6..5
        #define TPS65988_POWER_STATUS_BC12_STATUS_SDP   0            // SDP detected
        #define TPS65988_POWER_STATUS_BC12_STATUS_CDP   2            // CDP detected
        #define TPS65988_POWER_STATUS_BC12_STATUS_DCP   3            // DCP detected
//    #define TPS65988_POWER_STATUS_BC12_DETECTION        TPS65988_DEF_BIT_FIELD_IN_WORD( 4, 0)
		#define	TPS65988_POWER_STATUS_BC12_DETECTION(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT4)	//Set bit4
//    #define TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK   TPS65988_DEF_MBIT_FIELD_IN_WORD( 2, 2, 0)
		#define	TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,0,0x0C)>>2)	//Set bit3..2
        #define TPS65988_POWER_STATUS_TYPE_C_CURRENT_DEFAULT 0
        #define TPS65988_POWER_STATUS_TYPE_C_CURRENT_1_5_A   1
        #define TPS65988_POWER_STATUS_TYPE_C_CURRENT_3_0_A   2
        #define TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT 3
                                                                     // Connection provides power (TPS65988 as sink)
//    #define TPS65988_POWER_STATUS_SOURCE_SINK            TPS65988_DEF_BIT_FIELD_IN_WORD( 1, 0)
		#define	TPS65988_POWER_STATUS_SOURCE_SINK(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT1)	//Set bit1
                                                                     // Connection present
//    #define TPS65988_POWER_STATUS_POWER_CONNECTION       TPS65988_DEF_BIT_FIELD_IN_WORD( 0, 0)
		#define	TPS65988_POWER_STATUS_POWER_CONNECTION(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT0)	//Set bit0
        #define TPS65988_POWER_STATUS_TYPE_C_CONNECTION_NO_CONNECTION 0
        #define TPS65988_POWER_STATUS_TYPE_C_CONNECTION_CONNECTION   1
#define TPS65988_PD_STATUS                      0x40     // Status bit field for PD state
                                                         //  (4 bytes (RO))
//    #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_MASK   TPS65988_DEF_MBIT_FIELD_IN_DWORD(16, 6, 0)
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_REQUIRED_BY_POLICY_ENGINE  1
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_REQUESTD_BY_HOST           2
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_INVALID_DR_SWAP            3
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_DISCHARGE_FAILED           4
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_NO_RESPONSE_TIMEOUT        5
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SEND_SOFT_RESET            6
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SINK_SELECT_CAPABILITY     7
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SINK_TRANSITION_SINK       8
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SINK_WAIT_FOR_CAPABILITIES 9
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SOFT_RESET                 10
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SOURCE_ON_TIMEOUT          11
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SOURCE_CAPABILITY_RESPONSE 12
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SOURCE_SEND_CAPABILITIES   13
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_SOURCING_FAULT             14
        #define TPS65988_PD_STATUS_HARD_RESET_DETAILS_UNABLE_TO_SOURCE           15
//    #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_MASK     TPS65988_DEF_MBIT_FIELD_IN_DWORD( 8, 16, 0)
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_RECEIVED_FROM_DEVICE              1
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_NO_GOOD_CRC_FOR_SOFT_RESET        3
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_INVALID_SOURCE_CAPABILIES         4
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_SOFT_RESET_FAIL                   5
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_ACCEPTE_FOR_SOFT_RESET 6
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_GET_SINK_CAP           8
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_GET_SOURCE_CAP         9 
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_GOTO_MIN               10
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_PS_RDY                 11
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_PING                   12
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_REJECT                 13
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_REQUEST                14
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_SINK_CAPABILITIES      15
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_SOURCE_CAPABILITIES    16
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_SWAP                   17
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_WAIT_CAPABILITIES      18
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_UNKNOWN_CONTROL        19
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_UNEXPECTED_UNKNOWN_DATA           20
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_SOFT_RESET_TO_INIT_SOP1           21
        #define TPS65988_PD_STATUS_SOFT_RESET_TYPE_SOFT_RESET_TO_INIT_SOP2           22
//    #define TPS65988_PD_STATUS_PRESENT_ROLE_SOURCE      TPS65988_DEF_BIT_FIELD_IN_DWORD( 6, 0)
//    #define TPS65988_PD_STATUS_PORT_TYPE_MASK           TPS65988_DEF_MBIT_FIELD_IN_DWORD( 4, 2, 0)
        #define TPS65988_PD_STATUS_PORT_TYPE_SINK_SOURCE         0
        #define TPS65988_PD_STATUS_PORT_TYPE_SINK                1
        #define TPS65988_PD_STATUS_PORT_TYPE_SOURCE              2
        #define TPS65988_PD_STATUS_PORT_TYPE_SOURCE_SINK         3
//    #define TPS65988_PD_STATUS_CC_PULL_UP_MASK          TPS65988_DEF_MBIT_FIELD_IN_DWORD( 2, 2, 0)
        #define TPS65988_PD_STATUS_CC_PULL_UP_NONE               0
        #define TPS65988_PD_STATUS_CC_PULL_UP_DEFAULT            1
        #define TPS65988_PD_STATUS_CC_PULL_UP_1_5_A              2
        #define TPS65988_PD_STATUS_CC_PULL_UP_3_0_A              3
//    #define TPS65988_PD_STATUS_PLUG_DETAILS_MASK        TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0, 2, 0)
        #define TPS65988_PD_STATUS_PLUG_DETAILS_USB_TYPE_C_FULL_FEATURED 0
        #define TPS65988_PD_STATUS_PLUG_DETAILS_USB_2_TYPE_C             1

#define TPS65988_TX_IDENTITY                    0x47     // Data to send over BMC as a response to discover identity
                                                         //   (49 byte (RO)
                                                         // Byte 2-25 : SOP Response 
                                                         // Byte 26-49 :SOP' Response
//    #define TPS65988_TX_IDENTITY_NUM_VALID_DO_IN_SOP_DASH_RESPONSE_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 4, 3, 0)
                                                                     // Number of valid IDOs in register
                                                                     //   when 0, TPS65988 will send NAK for
                                                                     //   USB PD Discover Identufy message.
                                                                     //   when 1, TPS65988 will respond BUSY.
                                                                     //   2-7 indicates ACK for the message
//    #define TPS65988_TX_IDENTITY_NUM_VALID_DO_IN_SOP_RESPONSE_MASK      TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
                                                                     // Number of valid IDOs in register
                                                                     //   when 0, TPS65988 will send NAK for
                                                                     //   USB PD Discover Identufy message.
                                                                     //   when 1, TPS65988 will respond BUSY.
                                                                     //   2-7 indicates ACK for the message
#define TPS65988_RX_IDENTITY_SOP                0x48     // Discover identity response received over BMC from standard SOP
                                                         //  (25 byte (RO))
                                                         //  Byte 2-25: Latest IDOs received in response from SOP to  
                                                         //    a Discover Identity message
//    #define TPS65988_RX_IDENTITY_SOP_RESPONSE_MASK       TPS65988_DEF_MBIT_FIELD_IN_BYTE( 6, 2, 0)
        #define TPS65988_RX_IDENTITY_SOP_RESPONSE_ACK_RECEIVED      1
        #define TPS65988_RX_IDENTITY_SOP_RESPONSE_NACK_OR_TIMEOUT   2
        #define TPS65988_RX_IDENTITY_SOP_RESPONSE_BUSY_RECEIVED     3
//    #define TPS65988_RX_IDENTITY_SOP_NUM_VALID_DO_MASK   TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
                                                                     // Number of valid IDOs in register
#define TPS65988_RX_IDENTITY_SOP_DASH           0x49     // Discover IDENTITY response received over BMC from standard SOP'
                                                         //  (25 byte (RO))
//    #define TPS65988_RX_IDENTITY_SOP_DASH_RESPONSE_MASK  TPS65988_DEF_MBIT_FIELD_IN_BYTE( 6, 2, 0)
        #define TPS65988_RX_IDENTITY_SOP_DASH_RESPONSE_ACK_RECEIVED      1
        #define TPS65988_RX_IDENTITY_SOP_DASH_RESPONSE_NACK_OR_TIMEOUT   2
        #define TPS65988_RX_IDENTITY_SOP_DASH_RESPONSE_BUSY_RECEIVED     3
//    #define TPS65988_RX_IDENTITY_SOP_DASH_NUM_VALID_DO_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)

#define TPS65988_USER_VID_CONFIGURATION           0x4a     // User VID Configuration. Initialized by Application Customization.
                                                                     // Number of valid IDOs in register
#define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG     0x4b     // Discover identity response received over BMC from
                                                         // standard SOP'_Debug (DFP) or SOP''_Debug (UFP)
                                                         //  (25 byte (RO))
//    #define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG_RESPONSE_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 6, 2, 0)
        #define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG_RESPONSE_ACK_RECEIVED      1
        #define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG_RESPONSE_NACK_OR_TIMEOUT   2
        #define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG_RESPONSE_BUSY_RECEIVED     3
//    #define TPS65988_RX_IDENTITY_SOP_ASTA_DEBUG_NUM_VALID_DO_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
#define TPS65988_RX_ATTENTION                   0x4e     // Structured VDM attention message received over BMC
                                                         //  (29 byte (RO))
                                                         //  Byte 2-29 : SVDMReceived (Latest structured VDM Attention)
//    #define TPS65988_RX_ATTENTION_SEQUENCE_NUM_MASK      TPS65988_DEF_MBIT_FIELD_IN_BYTE( 5, 3, 0)
//    #define TPS65988_RX_ATTENTION_NUM_VALID_VDO_MASK     TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
#define TPS65988_RX_VDM                         0x4f     // VDM amessage received over BMC
                                                         //  (29 byte (RO))
//    #define TPS65988_RX_VDM_SEQUENCE_NUM_MASK            TPS65988_DEF_MBIT_FIELD_IN_BYTE( 5, 3, 0)
//    #define TPS65988_RX_VDM_SOP_TYPE_MASK                TPS65988_DEF_MBIT_FIELD_IN_BYTE( 3, 2, 0)
        #define TPS65988_RX_VDM_SOP_TYPE_SOP             0
        #define TPS65988_RX_VDM_SOP_TYPE_SOP_DASH        1
        #define TPS65988_RX_VDM_SOP_TYPE_SOP_DOUBLE_DASH 2
        #define TPS65988_RX_VDM_SOP_TYPE_SOP_ASTA_DEBUG  23
//    #define TPS65988_RX_VDM_NUM_VALID_VDO_MASK           TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
		#define	TPS65988_RX_VDM_NUM_VALID_VDO_MASK(x)	READ_MULTI_BIT_IN_BYTES(x,0,0x07)	//Set bit0..2
#define TPS65988_DATA_CONTROL                   0x50     // Bit field for data provided by an external controller
                                                         //  (4 byte (RO))
//    #define TPS65988_DATA_CONTROL_INTERRUPT_ACK          TPS65988_DEF_BIT_FIELD_IN_DWORD( 2, 0)
                                                                  // Copies IntMask1 to IntClear1
//    #define TPS65988_DATA_CONTROL_SOFT_RESET             TPS65988_DEF_BIT_FIELD_IN_DWORD( 1, 0)
                                                                  // Perform Soft-Reset
//    #define TPS65988_DATA_CONTROL_HOST_CONNECTED         TPS65988_DEF_BIT_FIELD_IN_DWORD( 0, 0)
#define TPS65988_DP_SID_CONFIGURATION           0x51     // DisplayPort alternate mode configuration
                                                         //  (5 byte (RW))
//    #define TPS65988_DP_SID_CONFIGURATION_MULTI_FUNCTION_PREFFERED         TPS65988_DEF_BIT_FIELD(32, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_UFP_D_PIN_ASSIGNMENT_A_SUPPORTED TPS65988_DEF_BIT_FIELD(24, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_UFP_D_PIN_ASSIGNMENT_B_SUPPORTED TPS65988_DEF_BIT_FIELD(25, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_UFP_D_PIN_ASSIGNMENT_C_SUPPORTED TPS65988_DEF_BIT_FIELD(26, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_UFP_D_PIN_ASSIGNMENT_D_SUPPORTED TPS65988_DEF_BIT_FIELD(27, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_UFP_D_PIN_ASSIGNMENT_E_SUPPORTED TPS65988_DEF_BIT_FIELD(28, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DFP_D_PIN_ASSIGNMENT_A_SUPPORTED TPS65988_DEF_BIT_FIELD(16, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DFP_D_PIN_ASSIGNMENT_B_SUPPORTED TPS65988_DEF_BIT_FIELD(17, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DFP_D_PIN_ASSIGNMENT_C_SUPPORTED TPS65988_DEF_BIT_FIELD(18, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DFP_D_PIN_ASSIGNMENT_D_SUPPORTED TPS65988_DEF_BIT_FIELD(19, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DFP_D_PIN_ASSIGNMENT_E_SUPPORTED TPS65988_DEF_BIT_FIELD(20, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_USB_2_0_SIGNAL_NOT_USED          TPS65988_DEF_BIT_FIELD(15, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DP_RECEPTACLE_INDICATION         TPS65988_DEF_BIT_FIELD(14, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DP_SIGNAL_USB_GEN_2              TPS65988_DEF_BIT_FIELD(11, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DP_SIGNAL_DP_V1_3                TPS65988_DEF_BIT_FIELD(10, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_DP_PORT_CAPABILITY_MASK          TPS65988_DEF_MBIT_FIELD( 8, 2, 5)
        #define TPS65988_DP_SID_CONFIGURATION_DP_PORT_CAPABILITY_UFP_D         1
        #define TPS65988_DP_SID_CONFIGURATION_DP_PORT_CAPABILITY_DFP_D         2
        #define TPS65988_DP_SID_CONFIGURATION_DP_PORT_CAPABILITY_DFP_D_UFP_D   3
//    #define TPS65988_DP_SID_CONFIGURATION_DP_MODE                          TPS65988_DEF_BIT_FIELD( 1, 5)
//    #define TPS65988_DP_SID_CONFIGURATION_ENABLE_DP_SID                    TPS65988_DEF_BIT_FIELD( 0, 5)
#define TPS65988_INTEL_VID_CONFIGURATION        0x52     // Intel alternate modes configuration (1 byte (RW))
                                                         //   Byte 5,6 : TBTModeDataTxSOPp
                                                         //   Byte 3,4 : TBTModeDataTxSOP
//    #define TPS65988_INTEL_VID_CONFIGURATION_EMAKER_OVERRIDE     TPS65988_DEF_BIT_FIELD_IN_BYTE(1, 1)
                                                              // 10Gb passive cable is assumed
//    #define TPS65988_INTEL_VID_CONFIGURATION_VOUT_3V3_REQUIRED   TPS65988_DEF_BIT_FIELD_IN_BYTE(0, 1)
                                                              // VOUT_3V3 is required for Thunderbolt Mode support
//    #define TPS65988_INTEL_VID_CONFIGURATION_THUNDERBOLT         TPS65988_DEF_BIT_FIELD_IN_BYTE(1, 0)
//    #define TPS65988_INTEL_VID_CONFIGURATION_INTEL_VID_ENABLED   TPS65988_DEF_BIT_FIELD_IN_BYTE(0, 0)
#define TPS65988_TI_VID_CONFIGURATION           0x54     // reserved for TI alternate modes configuration (1 byte (RO))
#define TPS65988_USER_SID_STATUS                  0x57     // User VID Status
#define TPS65988_DP_SID_STATUS                  0x58     // DisplayPort alternate mode status (17 bytes (RO))
//    #define TPS65988_DP_SID_STATUS_DP_MODE_DATA_MASK             TPS65988_DEF_MBIT_FIELD(104, 32, 17)
//    #define TPS65988_DP_SID_STATUS_DP_CONFIGURE_MASK             TPS65988_DEF_MBIT_FIELD( 72, 32, 17)
//    #define TPS65988_DP_SID_STATUS_DP_STATUS_RX_MASK             TPS65988_DEF_MBIT_FIELD( 40, 32, 17)
//    #define TPS65988_DP_SID_STATUS_DP_STATUS_TX_MASK             TPS65988_DEF_MBIT_FIELD(  8, 32, 17)
//    #define TPS65988_DP_SID_STATUS_DP_MODE_ACTIVE                TPS65988_DEF_BIT_FIELD(  1, 17)
//    #define TPS65988_DP_SID_STATUS_DP_SID_ACTIVE                 TPS65988_DEF_BIT_FIELD(  0, 17)
#define TPS65988_INTEL_VID_STATUS               0x59     // Intel alternate mode status (5 bytes (RO))
//    #define TPS65988_INTEL_VID_STATUS_THUNDERBOLT_ENTER_MODE_DATA_MASK TPS65988_DEF_MBIT_FIELD(  8, 32, 5)
//    #define TPS65988_INTEL_VID_STATUS_THUNDERBOLT_MODE_ACTIVE    TPS65988_DEF_BIT_FIELD(  1, 5)
//    #define TPS65988_INTEL_VID_STATUS_INTEL_VID_ACTIVE           TPS65988_DEF_BIT_FIELD(  0, 5)
#define TPS65988_TI_VID_STATUS               0x5B     // ITI VID Alternate Mode Status

#define TPS65988_DATA_STATUS                    0x5f     // status bit field of connection
                                                         //  (8 byte (RO))
#define	TPS65988_DATA_USB2_CONNECT(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT4)
#define	TPS65988_DATA_USB3_CONNECT(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT5)
#define	TPS65988_DATA_DP_CONNECTION(x)	READ_SINGLE_BIT_IN_BYTES(x,0,BIT0)
//    #define TPS65988_DATA_STATUS_TBT_CABLE_GEN_SUPPORT_MASK      TPS65988_DEF_MBIT_FIELD( 25, 7 , 8)
        #define TPS65988_DATA_STATUS_TBT_CABLE_GEN_SUPPORT_GEN2      0
        #define TPS65988_DATA_STATUS_TBT_CABLE_GEN_SUPPORT_GEN3_10GB 1
        #define TPS65988_DATA_STATUS_TBT_CABLE_GEN_SUPPORT_GEN3_20GB 2
//    #define TPS65988_DATA_STATUS_FORCE_LSX                       TPS65988_DEF_BIT_FIELD( 23, 8)
//    #define TPS65988_DATA_STATUS_TBT_TYPE_MASK                   TPS65988_DEF_MBIT_FIELD( 17, 3, 8)
        #define TPS65988_DATA_STATUS_TBT_TYPE_TYPE_C_TO_TYPE_C   0 
        #define TPS65988_DATA_STATUS_TBT_TYPE_TBT_LEGACY         1 
        #define TPS65988_DATA_STATUS_TBT_TYPE_OPTICAL_CABLE      2 
//    #define TPS65988_DATA_STATUS_TBT_CONNECTION                  TPS65988_DEF_BIT_FIELD( 16, 8)
//    #define TPS65988_DATA_STATUS_DP_PIN_ASSIGNMENT_MASK          TPS65988_DEF_MBIT_FIELD( 10, 2, 8)
        #define TPS65988_DATA_STATUS_DP_PIN_ASSIGNMENT_EF        0
        #define TPS65988_DATA_STATUS_DP_PIN_ASSIGNMENT_CD        1
        #define TPS65988_DATA_STATUS_DP_PIN_ASSIGNMENT_AB        2
//    #define TPS65988_DATA_STATUS_DP_SOURCE_SINK                  TPS65988_DEF_BIT_FIELD( 9, 8) // DP sink connection requested
//    #define TPS65988_DATA_STATUS_DP_CONNECTION                   TPS65988_DEF_BIT_FIELD( 8, 8)
//    #define TPS65988_DATA_STATUS_USB_DATA_ROLE_UFP_DFP           TPS65988_DEF_BIT_FIELD( 7, 8) // UFP/DFP
//    #define TPS65988_DATA_STATUS_USB3_SPEED_GEN2                 TPS65988_DEF_BIT_FIELD( 6, 8) // Gen1 (5Gb)/ Gen2 (10Gb)
//    #define TPS65988_DATA_STATUS_USB3_CONNECTION                 TPS65988_DEF_BIT_FIELD( 5, 8)
//    #define TPS65988_DATA_STATUS_USB2_CONNECTION                 TPS65988_DEF_BIT_FIELD( 4, 8)
//    #define TPS65988_DATA_STATUS_OVER_CURRENT                    TPS65988_DEF_BIT_FIELD( 3, 8)
//    #define TPS65988_DATA_STATUS_ACTIVE_CABLE                    TPS65988_DEF_BIT_FIELD( 2, 8) // Capble is active/passive
//    #define TPS65988_DATA_STATUS_CONNECTION_ORIENTATION          TPS65988_DEF_BIT_FIELD( 1, 8) // Upside-down/ upside-up
//    #define TPS65988_DATA_STATUS_DATA_CONNECTION_PRESENT         TPS65988_DEF_BIT_FIELD( 0, 8)
#define TPS65988_RX_USER_VID_ATTENTION_VDM      0x60     // Latest structured Attention Initiator message
                                                         // received for User VID. (29 bytes (RO))
//    #define TPS65988_RX_USER_VID_ATTENTION_VDM_NUM_VALID_VDO_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
//    #define TPS65988_RX_USER_VID_ATTENTION_VDM_SEQUENCE_NUM_MASK  TPS65988_DEF_MBIT_FIELD_IN_BYTE( 5, 3, 0)
#define TPS65988_RX_USER_VID_OTHER_VDM          0x61     // Latest unstructured VDM or a non-Attention 
                                                         // Structured VDM received for User VID. (29 bytes (RO))
//    #define TPS65988_RX_USER_VID_OTHER_VDM_NUM_VALID_VDO_MASK     TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
//    #define TPS65988_RX_USER_VID_OTHER_VDM_SOURCE_MASK            TPS65988_DEF_MBIT_FIELD_IN_BYTE( 3, 2, 0)
        #define TPS65988_RX_USER_VID_OTHER_VDM_SOURCE_SOP            0
        #define TPS65988_RX_USER_VID_OTHER_VDM_SOURCE_SOP_DASH       1
        #define TPS65988_RX_USER_VID_OTHER_VDM_SOURCE_SOP_DASH_DASH  2
        #define TPS65988_RX_USER_VID_OTHER_VDM_SOURCE_SOP_DASH_ASTA  3
//    #define TPS65988_RX_USER_VID_OTHER_VDM_SEQUENCE_NUM_MASK      TPS65988_DEF_MBIT_FIELD_IN_BYTE( 5, 3, 0)

#define TPS65988_C_CC_PIN_STATUS                0x69     // status of both CCn pins (4 bytes (RO))
                                                         //   Byte 1 : CCpinForPD (1: C_CC1 is CC, 2: C_CC2 is CC)
                                                         //   Byte 2 : CC1PinState
                                                         //     (1: RA detect (Source only))
                                                         //     (2: RD detect (Source only))
                                                         //     (3: STD Advertisement detected (Sink only))
                                                         //     (4: 1.5A Advertisement detected (Sink only))
                                                         //     (5: 3.0A Advertisement detected (Sink only))
                                                         //  Byte 3 : CC2PinState (same as CC1PinState)
                                                         //  Byte 4 : TypeCPortState
                                                         //   SRC states
                                                         //     0x00 : Disable 
                                                         //     0x01 : Unattached.SRC 
                                                         //     0x02 : AudioAccessaory
                                                         //     0x03 : DebugAccessory
                                                         //     0x04 : Attach.SRC
                                                         //     0x05 : ErrorRecovery
                                                         //     0x06 : AttachWait.SRC
                                                         //   SNK states
                                                         //     0x20 : Disabled
                                                         //     0x21 : Unattached.SNK
                                                         //     0x22 : Attached.SNK
                                                         //     0x24 : Unattached.Accessory
                                                         //     0x25 : Powered.Accessory
                                                         //     0x26 : Unsupported.Accessory
                                                         //     0x27 : AudioAccessory
                                                         //     0x28 : DebugAccessory
                                                         //     0x29 : ErrorRecovery
                                                         //     0x2a : AttachWait.SNK
                                                         //     0x2b : AttachWait.Accessory
                                                         //   DRP state
                                                         //     0x40 : Disabled
                                                         //     0x41 : Unattached.SRC
                                                         //     0x42 : Unattached.SNK
                                                         //     0x44 : Attached.SRC
                                                         //     0x45 : Try.SRC
                                                         //     0x47 : Attached.SNK
                                                         //     0x49 : AudioAccessory
                                                         //     0x4a : DebugAccessory
                                                         //     0x4b : ErrorRecovery
                                                         //     0x4c : AttachWait.SNK
                                                         //     0x4d : AttachWait.ARC
                                                         //     0x4e : TryWait.SNK

#define TPS65988_SLEEP_CONFIGURATION            0x70     // Sleep configurations (2 bytes (RW))
//    #define TPS65988_SLEEP_CONFIGURATION_SLEEP_AT_5V     TPS65988_DEF_BIT_FIELD_IN_BYTE( 3, 0)
//    #define TPS65988_SLEEP_CONFIGURATION_SLEEP_TIME_MASK TPS65988_DEF_MBIT_FIELD_IN_BYTE( 0, 3, 0)
        #define TPS65988_SLEEP_CONFIGURATION_SLEEP_TIME_NEVER          0
        #define TPS65988_SLEEP_CONFIGURATION_SLEEP_TIME_WHEN_POSSIBLE  1
        #define TPS65988_SLEEP_CONFIGURATION_SLEEP_TIME_WAIT_100MS     3
        #define TPS65988_SLEEP_CONFIGURATION_SLEEP_TIME_WAIT_1000MS    5
                                                         // Byte 2 : System Power State Threshold at or below which
                                                         //    I2C responsiveness is relaxed.
#define TPS65988_GPIO_STATUS                    0x72     // Captures status and settings of all GPIO pins. (8 bytes (RO))

/*
*      Power Data Object (PDO)
*/

//#define PDO_TYPE_MASK                                  TPS65988_DEF_MBIT_FIELD_IN_DWORD(30, 2, 0)     
		#define	PDO_TYPE_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,3,0xC0)>>6)	//Set bit31..30
    #define PDO_TYPE_FIXED_SUPPLY    0
    #define PDO_TYPE_BATTERY         1
    #define PDO_TYPE_VARIABLE_SUPPLY 2

//#define PDO_SOURCE_FIXED_SUPPLY_DUAL_ROLE_POWER        TPS65988_DEF_BIT_FIELD_IN_DWORD(29, 0)
//#define PDO_SOURCE_FIXED_SUPPLY_USB_SUSPEND_SUPPORTED  TPS65988_DEF_BIT_FIELD_IN_DWORD(28, 0)
//#define PDO_SOURCE_FIXED_SUPPLY_EXTERNALLY_POWERED     TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
//#define PDO_SOURCE_FIXED_SUPPLY_USB_COMM_CAPABLE       TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
//#define PDO_SOURCE_FIXED_SUPPLY_DATA_ROLE_SWAP         TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
//#define PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_MASK      TPS65988_DEF_MBIT_FIELD_IN_DWORD(20, 2, 0)
		#define	PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_MASK(x)	(READ_MULTI_BIT_IN_BYTES(x,2,0x30)>>4)	//Set bit21..20
    #define PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_IOC    0 // Overload capavilities
    #define PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_OLC_1  1 // 150% Ioc for 1ms @ 5% duty (low current is 97% of IOC)
                                                          // 125% Ioc for 2ms @ 10% duty (low current is 97% of IOC)
                                                          // 110% Ioc for 10ms @ 50% duty (low current is 90% of IOC)
    #define PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_OLC_2  2 // 200% Ioc for 1ms @ 5% duty (low currnet is 95% of IOC)
                                                          // 150% Ioc for 2ms @ 10% duty (low current is 94% of IOC) 
                                                          // 125% Ioc for 10ms @ 50% duty (low current is 75% of IOC)
    #define PDO_SOURCE_FIXED_SUPPLY_PEAK_CURRENT_OLC_3  3 // 200% Ioc for 1ms @ 5% duty (low currnet is 95% of IOC)
                                                          // 175% Ioc for 2ms @ 10% duty (low current is 92% of IOC) 
                                                          // 150% Ioc for 10ms @ 50% duty (low current is 50% of IOC)

//#define PDO_SOURCE_FIXED_SUPPLY_VOLTAGE_IN_50MV_MASK   TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
#define	PDO_SOURCE_FIXED_SUPPLY_VOLTAGE_IN_50MV_MASK(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,2,0x0F))<<6)+(READ_MULTI_BIT_IN_BYTES(x,1,0xFC)>>2))	//Set bit19..10
//#define PDO_SOURCE_FIXED_SUPPLY_MAX_CURRENT_IN_10MA    TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
#define	PDO_SOURCE_FIXED_SUPPLY_MAX_CURRENT_IN_10MA(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,1,0x03))<<8)+READ_MULTI_BIT_IN_BYTES(x,0,0xFF))	//Set bit9..10

//#define PDO_SOURCE_VARIABLE_SUPPLY_MAX_VOLTAGE_IN_50MV TPS65988_DEF_MBIT_FIELD_IN_DWORD(20,10, 0)
//#define PDO_SOURCE_VARIABLE_SUPPLY_MIN_VOLTAGE_IN_50MV TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
#define	PDO_SOURCE_VARIABLE_SUPPLY_MIN_VOLTAGE_IN_50MV(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,2,0x0F))<<6)+(READ_MULTI_BIT_IN_BYTES(x,1,0xFC)>>2))	//Set bit19..10
//#define PDO_SOURCE_VARIABLE_SUPPLY_MAX_CURRNET_IN_10MA TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
#define	PDO_SOURCE_VARIABLE_SUPPLY_MAX_CURRNET_IN_10MA(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,1,0x03))<<8)+READ_MULTI_BIT_IN_BYTES(x,0,0xFF))	//Set bit9..10

//#define PDO_SOURCE_BATTERY_MIN_VOLTAGE_IN_50MV         TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
#define	PDO_SOURCE_BATTERY_MIN_VOLTAGE_IN_50MV(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,2,0x0F))<<6)+(READ_MULTI_BIT_IN_BYTES(x,1,0xFC)>>2))	//Set bit19..10
//#define PDO_SOURCE_BATTERY_MAX_CURRNET_IN_10MA         TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
#define	PDO_SOURCE_BATTERY_MAX_CURRNET_IN_10MA(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,1,0x03))<<8)+READ_MULTI_BIT_IN_BYTES(x,0,0xFF))	//Set bit9..10

//#define PDO_SINK_FIXED_SUPPLY_DUAL_ROLE_POWER          TPS65988_DEF_BIT_FIELD_IN_DWORD(29, 0)
//#define PDO_SINK_FIXED_SUPPLY_HIGHER_CAPABILITY        TPS65988_DEF_BIT_FIELD_IN_DWORD(28, 0)
//#define PDO_SINK_FIXED_SUPPLY_EXTERNALLY_POWERED       TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
//#define PDO_SINK_FIXED_SUPPLY_USB_COMM_CAPABLE         TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
//#define PDO_SINK_FIXED_SUPPLY_DATA_ROLE_SWAP           TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
//#define PDO_SINK_FIXED_SUPPLY_VOLTAGE_IN_50MV          TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
#define	PDO_SINK_FIXED_SUPPLY_VOLTAGE_IN_50MV(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,2,0x0F))<<6)+(READ_MULTI_BIT_IN_BYTES(x,1,0xFC)>>2))	//Set bit19..10
//#define PDO_SINK_FIXED_SUPPLY_OPERATIONAL_CURRENT_IN_10MA TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
#define	PDO_SINK_FIXED_SUPPLY_OPERATIONAL_CURRENT_IN_10MA(x)	(((WORD)(READ_MULTI_BIT_IN_BYTES(x,1,0x03))<<8)+READ_MULTI_BIT_IN_BYTES(x,0,0xFF))	//Set bit9..10

//#define PDO_SINK_VARIABLE_SUPPLY_MAX_VOLTAGE_IN_50MV   TPS65988_DEF_MBIT_FIELD_IN_DWORD(20,10, 0)
//#define PDO_SINK_VARIABLE_SUPPLY_MIN_VOLTAGE_IN_50MV   TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
//#define PDO_SINK_VARIABLE_SUPPLY_OPERATIONAL_CURRENT_IN_10MA TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)

/*
*      Request Data Object
*/

//#define RDO_FIXED_SUPPLY_OBJECT_POSITION                    TPS65988_DEF_MBIT_FIELD_IN_DWORD(28, 3, 0)
                                                            // the SINK will respond to a GotoMin Message
                                                            // by reducing its load to the Minimum Operationg 
                                                            // current if GiveBackFlag is 1
//#define RDO_FIXED_SUPPLY_GIVE_BACK_FLAG                     TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
//#define RDO_FIXED_SUPPLY_CAPABILITY_MISMATCH                TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
//#define RDO_FIXED_SUPPLY_USB_COMM_CAPABLE                   TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
//#define RDO_FIXED_SUPPLY_NO_USB_SUSPEND                     TPS65988_DEF_BIT_FIELD_IN_DWORD(24, 0)
//#define RDO_FIXED_SUPPLY_OPERATIONAL_CURRENT_IN_10MA        TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
//#define RDO_FIXED_SUPPLY_MAX_OPERATIONAL_CURRENT_IN_10MA    TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
//#define RDO_FIXED_SUPPLY_MIN_OPERATIONAL_CURRENT_IN_10MA    TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)

//#define RDO_VARIABLE_SUPPLY_OBJECT_POSITION                 TPS65988_DEF_MBIT_FIELD_IN_DWORD(28, 3, 0)
                                                            // the SINK will respond to a GotoMin Message
                                                            // by reducing its load to the Minimum Operationg 
                                                            // current if GiveBackFlag is 1
//#define RDO_VARIABLE_SUPPLY_GIVE_BACK_FLAG                  TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
//#define RDO_VARIABLE_SUPPLY_CAPABILITY_MISMATCH             TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
//#define RDO_VARIABLE_SUPPLY_USB_COMM_CAPABLE                TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
//#define RDO_VARIABLE_SUPPLY_NO_USB_SUSPEND                  TPS65988_DEF_BIT_FIELD_IN_DWORD(24, 0)
//#define RDO_VARIABLE_SUPPLY_OPERATIONAL_CURRENT_IN_10MA     TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
//#define RDO_VARIABLE_SUPPLY_MAX_OPERATIONAL_CURRENT_IN_10MA TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
//#define RDO_VARIABLE_SUPPLY_MIN_OPERATIONAL_CURRENT_IN_10MA TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)

//#define RDO_BATTERY_OBJECT_POSITION                         TPS65988_DEF_MBIT_FIELD_IN_DWORD(28, 3, 0)
                                                            // the SINK will respond to a GotoMin Message
                                                            // by reducing its load to the Minimum Operationg 
                                                            // current if GiveBackFlag is 1
//#define RDO_BATTERY_GIVE_BACK_FLAG                          TPS65988_DEF_BIT_FIELD_IN_DWORD(27, 0)
//#define RDO_BATTERY_CAPABILITY_MISMATCH                     TPS65988_DEF_BIT_FIELD_IN_DWORD(26, 0)
//#define RDO_BATTERY_USB_COMM_CAPABLE                        TPS65988_DEF_BIT_FIELD_IN_DWORD(25, 0)
//#define RDO_BATTERY_NO_USB_SUSPEND                          TPS65988_DEF_BIT_FIELD_IN_DWORD(24, 0)
//#define RDO_BATTERY_OPERATIONAL_POWER_IN_250MW              TPS65988_DEF_MBIT_FIELD_IN_DWORD(10,10, 0)
//#define RDO_BATTERY_MAX_OPERATING_POWER_IN_250MW            TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)
//#define RDO_BATTERY_MIN_OPERATING_POWER_IN_250MW            TPS65988_DEF_MBIT_FIELD_IN_DWORD( 0,10, 0)


/*
*     Type-C docking
*/
 #define 		TYPE_C_DOCKING_EVENT							BIT0
 #define    TYPE_C_DOCKING_EVENT_BUTTON_WOL   BIT1
 #define 		TYPE_C_DOCKING_EVENT_BUTTON_MAKE	BIT2
 #define 		TYPE_C_DOCKING_EVENT_BUTTON_BREAK	BIT3
/*
*********************************************************
*     Global Macros
*********************************************************
*/


/*
*********************************************************
*     Global Data Types
*********************************************************
*/

 typedef enum {POWER_SOURCE_TYPE_NONE,
			   POWER_SOURCE_TYPE_FIXED_SUPPLY,
			   POWER_SOURCE_TYPE_BATTERY,
			   POWER_SOURCE_TYPE_VARIABLE_SUPPLY} POWER_SOURCE_TYPE;
 
 typedef enum {
	 USB_PDC_REQ_NONE,
	 USB_PDC_REQ_ENABLE_TO_SUPPLY_POWER,
	 USB_PDC_REQ_DISABLE_TO_SUPPLY_POWER,
	 USB_PDC_REQ_GET_CONFIGURATION,
 
	 USB_PDC_REQ_HARD_RESET,
	 USB_PDC_REQ_SOFT_RESET,
	 USB_PDC_REQ_GET_TX_SOURCE_PDO,
	 USB_PDC_REQ_GET_TX_SINK_PDO,
	 USB_PDC_REQ_GET_RX_SOURCE_PDO,
	 USB_PDC_REQ_GET_RX_SINK_PDO,
	 USB_PDC_REQ_SWAP_TO_SINK,
	 USB_PDC_REQ_SWAP_TO_SOURCE,
	 USB_PDC_REQ_SWAP_TO_UFP,
	 USB_PDC_REQ_SWAP_TO_DFP,
	 USB_PDC_REQ_OPERATE_AS_DFP,
	 USB_PDC_REQ_OPERATE_AS_UFP,
	 USB_PDC_REQ_OPERATE_AS_SOURCE,
	 USB_PDC_REQ_OPERATE_AS_SINK,
	 USB_PDC_REQ_GET_ALTERNATE_MODES,
	 USB_PDC_REQ_GET_CURRENT_CAM,
 
	 USB_PDC_REQ_SEND_VDM,
	 USB_PDC_REQ_READ_RECEIVED_VDM,
	 USB_PDC_REQ_READ_RECEIVED_USER_VID_ATTENTION_VDM,
	 USB_PDC_REQ_READ_RECEIVED_USER_VID_VDM,
 
	 USB_PDC_REQ_CHANGE_TO_LOWEST_SINK_CAPABILITY,
	 USB_PDC_REQ_RESTORE_SINK_CAPABILITY,
 
 } USB_PDC_REQUEST;
 
 typedef enum {
	 USB_PDC_EVENT_NONE,
	 USB_PDC_EVENT_VDM_RECEIVED,
	 USB_PDC_EVENT_USER_VID_VDM_ATTENTION_RECEIVED,
	 USB_PDC_EVENT_USER_VID_VDM_RECEIVED,
	 USB_PDC_EVENT_USER_VID_ALT_MODE_ENTERED,
	 USB_PDC_EVENT_USER_VID_ALT_MODE_EXITED,
 } USB_PDC_EVENT;
 
 typedef void (USB_PDC_NOTIFY_EVENT)(int usb_pdc_id, USB_PDC_EVENT event);
 
 struct _USB_REQ {
	 struct _USB_REQ	*next;
	 struct {
		 BOOL			done: 1;
		 BOOL			busy: 1;
	 } status;
	 BYTE				request;
	 WORD				result;
	 WORD				parameter[2];
	 void				*address;
	 BYTE				pdos_valid;
 //    USB_PDC_CALL_BACK  *call_back;
 };
 
 typedef struct _USB_REQ USB_PDC_REQ;
 
 enum _USB_BC12_STATUS {
	 USB_BC12_NONE,
	 USB_BC12_SDP,
	 USB_BC12_CDP,
	 USB_BC12_DCP,
 };
 
 typedef enum _USB_BC12_STATUS USB_BC12_STATUS;
 
 enum _USB_TYPE_C_CURRENT {
	 USB_TYPE_C_CURRENT_NONE,
	 USB_TYPE_C_CURRENT_DEFAULT,
	 USB_TYPE_C_CURRENT_1_5_A,
	 USB_TYPE_C_CURRENT_3_0_A,
	 USB_TYPE_C_CURRENT_PD_CONTRACT,
 };
 
 typedef enum _USB_TYPE_C_CURRENT USB_TYPE_C_CURRENT;
 
 struct _USB_POWER_STATUS {
	 BOOL				power_connection:  1;
	 USB_BC12_STATUS	bc12_status:	   3;
	 USB_TYPE_C_CURRENT type_c_current:    3;
	 BOOL				power_source:	   1;	// power role is source
 };
 
 typedef struct _USB_POWER_STATUS USB_POWER_STATUS;
 
 enum _USB_CONNECT {
	 USB_CONNECT_NONE,
	 USB_CONNECT_CONNECTED,
	 USB_CONNECT_AUDIO_ACCESSORY,
	 USB_CONNECT_DEBUG_ACCESSORY,
	 USB_CONNECT_DP,
 };
 
 typedef enum _USB_CONNECT USB_CONNECT;
 
 enum _USB_HOST {
	 USB_HOST_NONE, 					 // VBus is not provided
	 USB_HOST_PD,						 // VBus is provided by PD (with USB functions)
	 USB_HOST_PD_NO_USB,				 // VBus is provided by PD (no USB function)
	 USB_HOST_NONE_PD,					 // VBUs is provided by a device (no PD function)
 };
 
 typedef enum _USB_HOST USB_HOST;
 
 struct _USB_STATUS {
	 BOOL		 device_connected:	1;	 // a device is connected
	 BOOL		 dfp:				1;	 // Connector acts as DFP
	 USB_CONNECT connect_status:	3;	 // type of the device connected
	 USB_HOST	 usb_host:			3;	 // VBus state
 };
 
 typedef struct _USB_STATUS USB_STATUS;


/*
*********************************************************
*     Global Variables
*********************************************************
*/

/*
*********************************************************
*     Externals
*********************************************************
*/

/*
*********************************************************
*     Global Function Prototypes
*********************************************************
*/
extern void OEM_TYPE_C_ADPID_HOOK(void);
extern void ECResetTypecPD(void);
extern void InitialTypecPD(void);
extern BOOL TypecPDProcess(BOOL verify);
extern BOOL TypecPDPort2Process(BOOL verify);
extern void OEM_TYPE_C_init_hook(void);
extern void OEM_TYPE_C_HOOK(void);
extern void OEM_TYPE_C_S3_HOOK(void);
extern void OEM_TYPE_C_S3_S0_HOOK(void);
extern void OEM_TYPE_C_S0_S3_HOOK(void);
extern void OEM_TYPE_C_S5_HOOK(void);
extern void OEM_TYPE_C_S5_S0_HOOK(void);
extern void OEM_TYPE_C_S0_S5_HOOK(void);
extern void PowerSourceSwitch(void);
extern void Oem_Hook_Type_C_BeforeSleep(void);
extern void Send4ccCmd(BYTE DataCnt,BYTE PortID);
extern void SendPowerModeChangeToPdc(void);
extern void SendPdoSet(BYTE PDOSetNum,BYTE PortNum); //A485D00051
extern void UsbPdcGetPowerStatus(BYTE usb_pdc_id);
extern BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ *request_packet, BYTE cmd, WORD parm1, WORD parm2,void *address);
extern void UsbPdcGetDataRole(BYTE usb_pdc_id);
extern void UsbPdcGetPowerRole(BYTE usb_pdc_id);
extern void SendPDO20VTo9V(BYTE portNUM);
extern void P2P_Process_Port2(void);
extern void SendPDO9VTo20V(BYTE portNUM);
extern void SendUnitInformationToPdc_Port2(void);
extern void SinkPowerPathControl(BYTE OnOff,BYTE PortID);
extern void Enable_TwoPowerSourceSelect(void);
extern void TPS65988_SelectAdapterPort(void);
extern void SetMultiPortSNK(BYTE portNum);
extern BOOL isBatteryExist(void);
extern void TPS65988_UcsiRelativeInit(void);
extern void PDOPortSwitchToNormal(void);
extern void OEM_TYPE_C_S0_HOOK(void);

//extern void UsbPdcGetStatus(BYTE usb_pdc_id);
//extern BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ *request_packet, BYTE cmd, WORD parm1, WORD parm2,void *address);

#endif // (Support_TYPE_C == TPS65988)

#endif   //_OEM_TPS65988_H

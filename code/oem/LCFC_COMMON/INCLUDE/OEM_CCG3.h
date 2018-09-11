/*
********************************************************************************
* LCFC Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LCFC 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_CCG3.h
* 
* Description:
*    definition of (CCG3) Cypress Type-C controller register
* 
* History:
*   01/15/2016    Isaac Lin
*     created as new
********************************************************************************
*/

#ifndef _OEM_CCG3_H
#define _OEM_CCG3_H

//Support_TYPE_C
#define CCG3_Addr                  0x10    // Type-C CCG3 I2C address
#if (Support_TYPE_C == CCG3)

#define 	TypeC_Ini_Delay_time  8 	//Delay 80ms for type C stable     8 * 10mS 

/*
*********************************************************
*     #EC ram define
*********************************************************
*/
#define  TypeCStatus          TYPE_C_RAM0             //_at_ OEMRAM3+0x70;	//Bit2~bit0:Step
 #define     TypeCAdpAuthing   	BIT0
 #define     TypeCAdpAuthDone  BIT1
 #define     TypeCAdpAuthPass  	BIT2
 #define     TypeCAdpExist   	BIT3
 #define     OldDcinAttached2  BIT4
 #define     TypeCAdpReady     BIT5
 #define     TypeCProcessOK    BIT6
 #define     TypeCIniOK        BIT7
#define  TypeCIniDelay         TYPE_C_RAM1             //_at_ OEMRAM3+0x71;
#define  TypeCEventNum         TYPE_C_RAM2             //_at_ OEMRAM3+0x72;
#define  TypeCCommFailCnt      TYPE_C_RAM3             //_at_ OEMRAM3+0x73;
#define  TypeCAdpID            TYPE_C_RAM4             //_at_ OEMRAM3+0x74;
#define  CurrentTypeCWatt      TYPE_C_RAM5             //_at_ OEMRAM3+0x75;
#define  TypeCAdpPdStableCheck TYPE_C_RAM6             //_at_ OEMRAM3+0x76;
#define  TypeCProcessStep      TYPE_C_RAM7             //_at_ OEMRAM3+0x77;
#define  PDStatus	             TYPE_C_RAM8             //_at_ OEMRAM3+0x78;
 #define 		CurrentDEV_INTR		BIT0
 #define 		CurrentPORT0_INTR	BIT1
 #define 		CurrentPORT1_INTR	BIT2
 #define     TypeCFwUpdating   BIT7
#define  CurrentTypeCV         TYPE_C_RAM10             //_at_ OEMRAM3+0x7A;  word
#define  CurrentTypeCI         TYPE_C_RAM11             //_at_ OEMRAM3+0x7C;  word
/*
*********************************************************
*     #define Constants
*********************************************************
*/

/*
*     Commands(Offset address)
*/

#define PORT0_PORT1_COM_ADDRESS_HIGH		0x00	//CCG3/CCG4 common offset address high byte
#define PORT0_ADDRESS_HIGH							0x10	//CCG3/CCG4 PORT0 offset address high byte
#define PORT1_ADDRESS_HIGH							0x20	//CCG4 PORT1 offset address high byte

//CCG1/CCG2/CCG3/CCG4 Common commands
//CCG1/CCG2: 0xYY
//CCG3/CCG4: 0x00YY
#define DEVICE_MODE								0x00	//Byte	
   #define CURRENT_MODE0										BIT0	//0 ¡V Boot mode;1 ¡V Firmware 1;2 - Firmware 2
   #define CURRENT_MODE1										BIT1
   #define NUMBER_OF_PD_PORTS_SUPPORTED0		BIT2	//0:1 port,1:2 ports
   #define NUMBER_OF_PD_PORTS_SUPPORTED1		BIT3
   #define FLASH_ROW_SIZE0									BIT4	//0:128 bytes,1:256 bytes
   #define FLASH_ROW_SIZE1									BIT5
   #define FLASH_ROW_SIZE2									BIT6
   #define CURRENT_MODE3										BIT7	//0 ¡V Legacy boot mode, only bootloader can do flash read/write;1 ¡V Dual firmware mode, two copies of firmware which can do mutual updates.
#define BOOT_MODE_REASON					0x01	//Byte
   #define BOOT_MODE_REQUEST								BIT0
   #define CONFIGURATION_TABLE_STATUS				BIT1
   #define FIRMWARE_APPLICATION_1_STATUS		BIT2
   #define FIRMWARE_APPLICATION_2_STATUS		BIT3
#define READ_SILICON_ID						0x02	//2 Byte
#define BOOT_LOADER_LAST_ROW			0x04	//2 Byte Byte0~1:Number of the last flash row occupied by bootloader.
#define INTR_REG									0x06	//Byte
   #define DEV_INTR													BIT0	//This bit is used by EC to de-assert the DEV_INTR GPIO. Write 1 clear.
   #define PORT0_INTR												BIT1	//A new response is available in PORT0 specific PD_RESPONES register.
   #define PORT1_INTR												BIT2	//A new response is available in PORT1 specific PD_RESPONES register.
#define JUMP_TO_BOOT							0x07	//Byte	//J:JUMP_TO_BOOT;A:JUMP_TO_ALT_FW
#define TYPEC_RESET								0x08	//2 Byte Byte0:Allow reset if this byte is loaded with valid signature: ¡§R¡¨.;Byte1:b0:0 ¡V I2C reset;1 ¡V Device Reset
#define ENTER_FLASHING_MODE				0x0A	//Byte Handle command only if this byte is loaded with valid signature by EC: ¡§P¡¨.
#define VALIDATE_FW								0x0B	//Byte ID of the firmware to be validated. The value can be 0x01 or 0x02 based on whether FW1 or FW2 is to be validated
#define FLASH_ROW_READ_WRITE			0x0C	//4 Byte
//Byte 0:Handle command only if this field is loaded with valid signature by EC: ¡§F¡¨.
//Byte 1:0 ¡V Flash Row Read;1 ¡V Flash Row Write
//Byte 2~3:Row number of flash.
#define READ_ALL_VERSION					0x10	//16 Byte
//Byte0~7:BL Version, 8 bytes version information for the Bootloader.
//Byte8~15:FW Version, 8 bytes version information for the firmware application. In dual firmware applications, this field reports the version of FW1.
//Base Version Format:
//	Byte0~1:Build Number
//	Byte2:Patch Version
//	Byte3 bit0~3:Minor Version
//	Byte3 bit4~7:Major Version
//Application FW Version Format
//	Byte0~1:Application (Notebook) Name (0x6e62 : ASCII for "nb")
//	Byte2:External Circuit Specific Version
//	Byte3 bit0~3:Minor Version
//	Byte3 bit4~7:Major Version

//CCG3/CCG4 unique commands and CCG1/CCG2 do not support.
#define FW2_VERSION								0x20	//8 Byte
//Byte0~7:FW Version, 8 bytes version information for the firmware application. In dual firmware applications, this field reports the version of FW2.
#define FIRMWARE_LOCATION					0x28	//4 Byte
//Byte0~1:FW1_START, In Legacy Boot Mode, this shows the flash row where the configuration table is stored.In dual FW mode, this shows the flash row where FW1 is stored.
//Byte2~3:FW2_START, In Legacy Boot Mode, this shows the flash row where the configuration table is stored.In dual FW mode, this shows the flash row where FW2 is stored.
#define PDPORT_ENABLE							0x2C	//Byte
   #define PORT0_ENABLE											BIT0	//Port 0 enable
   #define PORT1_ENABLE											BIT1	//Port 1 enable
#define RESPONSE_REGISTER					0x7E	//2 Byte	Byte0 bit0~bit6 Response Codes,Byte1:Reply data count.
#define READ_DATA_AREA						0x80	//n Byte	n will reply in previous command(0x7E) High Byte

//Need distinguish commands for port0 and port1
//Port0:0x10xx
//Port1:0x20xx
#define VDM_CONTROL								0x00	//2 Byte
//Byte0:VDM mode, 0x00 ¡V SOP;0x01 ¡V SOP¡¦;0x02 ¡V SOP¡¨
//Byte1:Total length of VDM message in bytes. Should be a multiple of 4 and less than 28.
#define EFFECTIVE_SOURCE_PDO_MASK	0x02	//Byte 0:No be used;1:Being used
#define EFFECTIVE_SINK_PDO_MASK		0x03	//Byte 0:No be used;1:Being used
#define SELECT_SOURCE_PDO					0x04	//Byte
#define SELECT_SINK_PDO						0x05	//Byte
#define PD_CONTROL								0x06	//Byte
#define PD_STATUS									0x08	//4 Byte
#define TYPE_C_STATUS							0x0C	//Byte
#define CURRENT_PDO								0x10	//4 Byte
#define CURRENT_RDO								0x14	//4 Byte
#define CURRENT_CABLE_VDO					0x18	//4 Byte
#define EC_DP_HPD_CONTROL					0x20	//Byte
#define EC_DP_MUX_CONTROL					0x21	//Byte
#define TRIGGER_DP_MODE						0x22	//Byte
#define DP_CONFIGURE_MODE					0x23	//Byte
#define EVENT_MASK								0x24	//4 Byte
#define SWAP_RESPONSE							0x28	//Byte
#define ACTIVE_EC_MODES						0x29	//Byte
#define VDM_EC_CONTROL						0x2A	//Byte
#define PORT0_RESPONSE_REGISTER		0x1400	//4 Byte	Byte0 bit0~bit6 Response Codes,Byte2~3:Reply data count.
#define PORT1_RESPONSE_REGISTER		0x1800	//4 Byte	Byte0 bit0~bit6 Response Codes,Byte2~3:Reply data count.
#define PORT0_READ_DATA_AREA			0x1404	//(size:0x1404~0x1500)n Byte	n will reply in previous command(0x1400) Byte2~3
#define PORT0_WRITE_DATA_AREA			0x1800	//(size:0x1800~0x19FF)n Byte	n will reply in previous command(0x1400) Byte2~3
#define PORT1_READ_DATA_AREA			0x2404	//(size:0x2404~0x2500)n Byte	n will reply in previous command(0x1800) Byte2~3
#define PORT1_WRITE_DATA_AREA			0x2800	//(size:0x2800~0x29FF)n Byte	n will reply in previous command(0x1800) Byte2~3

/*
*     Response Codes
*/

#define NO_RESPONSE								0x00	//No outstanding command, event or asynchronous message in the system.
#define CMD_SUCCESS								0x02	//Command handled successfully.
#define FLASH_DATA_AVAILABLE			0x03	//Flash row data requested by EC is available in Data Memory.
#define INVALID_COMMAND						0x05	//Partial, unaligned register write or invalid command.
#define FLASH_UPDATE_FAILED				0x07	//Flash write operation failed.
#define INVALID_FW								0x08	//FW validity check failed. Please refer VALIDATE_FW command.
#define INVALID_ARGUMENTS					0x09	//Command handling failed due to invalid arguments.
#define NOT_SUPPORTED							0x0A	//Command not supported in the current mode.
#define TRANSACTION_FAILED				0x0C	//GOOD_CRC not received for the PD message transmitted.
#define PD_COMMAND_FAILED					0x0D	//CCG is not able to handle the command issued by EC.
#define UNDEFINED_ERROR						0x0F	//Undefined Error
#define READ_PDO_DATA							0x10	//Read PDO Data

/*
*     Event¡¦s opcode definitions
*/

/*
*     Device Specific Events
*/
#define RESET_COMPLETE						0x80	//Device underwent a reset and is back in operation mode.
#define MESSAGE_QUEUE_OVERFLOW		0x81	//Message queue overflow detected.	

/*
*     TYPE C Specific Events
*/
#define OVER_CURRENT_DETECTED			0x82	//Over Current Detected.
#define OVER_VOLTAGE_DETECTED			0x83	//Over Voltage Detected.
#define PORT_CONNECT_DETECTED			0x84	//Type C Port Connect Detected
#define PORT_DISCONNECT_DETECTED	0x85	//Type C Port Disconnect Detected

/*
*     PD Control Messages and Contract Specific Events
*/
#define CONTRACT_INFO							0x86	//8 Byte	PD Contract Negotiation Complete.
#define SWAP_STATUS								0x87	//
#define PS_RDY_MESSAGE_RECEIVED		0x8A	//
#define GOTOMIN_MESSAGE_RECEIVED	0x8B	//The GotoMin request is valid until the next contract re-negotiation or disconnect event.
#define ACCEPT_MESSAGE_RECEIVED		0x8C	//Accept Message Received.
#define REJECT_MESSAGE_RECEIVED		0x8D	//Reject Message Received.
#define WAIT_MESSAGE_RECEIVED			0x8E	//Wait Message Received.
#define HARD_RESET_RECEIVED				0x8F	//Hard Reset Received.

/*
*     PD Data Message Specific Events
*/
#define VDM_RECEIVED							0x90	//This event indicates that CCG received a VDM from Port Partner or EMCA.

/*
*     Capability Message Specific Events
*/
#define SOURCE_CAPABILITIES_MESSAGE_RECEIVED	0x91	//Source Capabilities Message Received
#define SINK_CAPABILITIES_MESSAGE_RECEIVED		0x92	//Sink Capabilities Message Received.

/*
*     DP and Alternate Mode Specific Events
*/
#define DISPLAY_PORT_ALTERNATE_MODE_ENTERED		0x93	//Display Port Alternate Mode entered.
#define DP_STATUS_UPDATE											0x94	//DP Status Update.
#define DISPLAY_PORT_SID_NOT_FOUND						0x96	//Display port SID not found in Discover SID process.
#define MULTIPLE_SVID_DISCOVERED							0x97	//Multiple SVIDs discovered along with Display port SID.
#define DP_FUNCTION_NOT_SUPPORTED_BY_CABLE		0x98	//DP Functionality not supported by Cable.
#define DISPLAY_PORT_NOT_SUPPORTED_BY_UFP			0x99	//Display Port Configuration not supported by UFP.

/*
*     Resets and Error Scenario Events
*/
#define HARD_RESET_SENT_TO_PORT_PARTNER				0x9A	//This event is reported when CCG sends HARD_RESET to port partner.
#define SOFT_RESET_SENT_TO_PORT_PARTNER				0x9B	//Soft Reset Sent to Port Partner.
#define CABLE_RESET_SENT_TO_EMCA							0x9C	//Cable Reset Sent to EMCA.
#define SOURCE_DISABLED_STATE_ENTERED					0x9D	//Source Disabled State Entered.
#define SENDER_RESPONSE_TIMER_TIMEOUT					0x9E	//Sender Response Timer Timeout.
#define NO_VDM_RESPONSE_RECEIVED							0x9F	//No VDM Response Received.
#define UNEXPECTED_VOLTAGE_ON_VBUS						0xA0	//CCG notifies EC with this event if CCG is DFP and unexpected voltage is detected on VBUS before CCG turns on VBUS.
#define TYPEC_ERROR_RECOVERY									0xA1	//CCG notifies EC with this event when CCG executes TYPE C ERROR Recovery.

/*
*     EMCA Related Events
*/
#define EMCA_DETECTED													0xA6	//CCG in DFP Mode discovers EMCA (SOP¡¦) by sending DISCOVER ID command.

/*
*     Miscellaneous Events
*/
#define RP_CHANGE_DETECTED										0xAA	//CCG notifies EC with this event when CCG detects a change in TYPE C ¡§Rp¡¨ resistor value in Sink Mode.

/*
*     Thunderbolt Related Events
*/
#define THUNDERBOLT_MODE_ENTERED							0xB0	//Thunderbolt mode entered.
#define THUNDERBOLT_MODE_EXITED								0xB1	//Thunderbolt mode exited.


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
extern void ECResetCCG3(void);
extern void InitialCCG3(void);
extern void CCG3Process(void);
extern void OEM_TYPE_C_init_hook(void);
extern void OEM_TYPE_C_HOOK(void);
extern void OEM_TYPE_C_S3_HOOK(void);
extern void OEM_TYPE_C_S5_HOOK(void);
extern void Oem_Hook_Type_C_BeforeSleep(void);

#endif  //Support_TYPE_C
  
#endif  //_OEM_CCG3_H
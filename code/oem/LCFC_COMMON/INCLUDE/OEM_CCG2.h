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
* Filename: OEM_CCG2.h
* 
* Description:
*    definition of CYPD2122(CCG2) Cypress Type-C controller register
* 
* History:
*   01/15/2016    Isaac Lin
*     created as new
********************************************************************************
*/

#ifndef _OEM_CCG2_H
#define _OEM_CCG2_H

//Support_TYPE_C
#define CCG2_Addr                  0x10    // Type-C CCG2 I2C address
#if (Support_TYPE_C == CCG2)

#define 	TypeC_Ini_Delay_time  8 	//Delay 300ms for type C stable     30 * 10mS 

/*
*********************************************************
*     #EC ram define
*********************************************************
*/
#define  TypeCStatus           TYPE_C_RAM0             //_at_ OEMRAM3+0x70;	//Bit2~bit0:Step
 #define   TypeCAdpAuthing   	 BIT0
 #define   TypeCAdpAuthDone     BIT1
 #define   TypeCAdpAuthPass  	 BIT2
 #define   TypeCAdpExist   	   BIT3
 #define   OldDcinAttached2     BIT4
 #define   TypeCAdpReady        BIT5
 #define   TypeCProcessOK       BIT6
 #define   TypeCIniOK           BIT7
#define  TypeCIniDelay         TYPE_C_RAM1             //_at_ OEMRAM3+0x71;
#define  TypeCEventNum         TYPE_C_RAM2             //_at_ OEMRAM3+0x72;
#define  TypeCCommFailCnt      TYPE_C_RAM3             //_at_ OEMRAM3+0x73;
#define  TypeCAdpID            TYPE_C_RAM4             //_at_ OEMRAM3+0x74;
#define  CurrentTypeCV         TYPE_C_RAM5             //_at_ OEMRAM3+0x75;
#define  CurrentTypeCI         TYPE_C_RAM6             //_at_ OEMRAM3+0x76;
#define  CurrentTypeCWatt      TYPE_C_RAM7             //_at_ OEMRAM3+0x77;
#define  TypeCAdpPdStableCheck TYPE_C_RAM8             //_at_ OEMRAM3+0x78;
#define  TypeCProcessStep      TYPE_C_RAM9             //_at_ OEMRAM3+0x79;
#define  CCG2Status            TYPE_C_RAM10            //_at_ OEMRAM3+0x7A;
 #define 	CurrentDEV_INTR		   BIT0
 #define 	CurrentPORT0_INTR	   BIT1
 #define 	CurrentPORT1_INTR	   BIT2

/*
*********************************************************
*     #define Constants
*********************************************************
*/

#define DEVICE_MODE								0x00	//Byte	0 ¡V Boot mode;1 ¡V Normal operation mode
#define BOOT_MODE_REASON					0x01	//Byte
   #define BOOT_MODE_REQUEST								BIT0
   #define CONFIGURATION_TABLE_STATUS				BIT1
   #define NORMAL_OPERATION_MODE_APP_STATUS	BIT2
#define READ_SILICON_ID						0x02	//2 Byte
#define BOOT_LOADER_LAST_ROW			0x04	//2 Byte
#define INTR_REG									0x06	//Byte
   #define INTR_ASSERT											BIT0	//This bit is used by EC to de-assert the INTR GPIO. Write 1 clear.
#define JUMP_TO_BOOT							0x07	//Byte
#define TYPEC_RESET								0x08	//2 Byte
#define ENTER_FLASHING_MODE				0x0A	//Byte
#define VALIDATE_FW								0x0B	//Byte
#define FLASH_ROW_READ_WRITE			0x0C	//4 Byte
#define READ_ALL_VERSION					0x10	//16 Byte
#define VDM_CONTROL								0x20	//2 Byte
#define EFFECTIVE_SOURCE_PDO_MASK	0x24	//Byte
#define EFFECTIVE_SINK_PDO_MASK		0x25	//Byte
#define SELECT_SOURCE_PDO					0x26	//Byte
#define SELECT_SINK_PDO						0x27	//Byte
#define PD_CONTROL								0x28	//Byte
#define PD_STATUS									0x2C	//4 Byte
#define TYPE_C_STATUS							0x30	//Byte
#define CURRENT_PDO								0x34	//4 Byte
#define CURRENT_RDO								0x38	//4 Byte
#define CURRENT_CABLE_VDO					0x3C	//4 Byte
#define EC_DP_HPD_CONTROL					0x44	//Byte
#define EC_DP_MUX_CONTROL					0x45	//Byte
#define TRIGGER_DP_MODE						0x46	//Byte
#define DP_CONFIGURE_MODE					0x47	//Byte
#define EVENT_MASK								0x48	//4 Byte
#define SWAP_RESPONSE							0x4C	//Byte
#define ACTIVE_EC_MODES						0x4D	//Byte
#define VDM_EC_CONTROL						0x4E	//Byte
#define RESPONSE_REGISTER					0x7E	//2 Byte	Byte0 bit0~bit6 Response Codes

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
extern void InitialCCG2(void);
extern void CCG2Process(void);
extern void OEM_TYPE_C_init_hook(void);
extern void OEM_TYPE_C_HOOK(void);
extern void Oem_Hook_Type_C_BeforeSleep(void);

#endif  //Support_TYPE_C
  
#endif  //_OEM_CCG2_H
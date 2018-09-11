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
* Filename: OEM_Ucsi.h
*
* Description:
*    definition of USB Type-C Connector System Software Interface (UCSI).
*
* History:
*   10/27/2016    Isaac Lin
*        created as new
********************************************************************************
*/

#ifndef OEM_UCSI_H
   #define OEM_UCSI_H

//#include <common.h>
//#include <BitFieldManipulate.h>

/*
*********************************************************
*     #EC ram define
*********************************************************
*/
#define  Ucsi_version          UCSI_RAM0             //_at_ OEMRAMC+0x00;				// PPM -> OPM
#define  Ucsi_reserved0        UCSI_RAM1             //_at_ OEMRAMC+0x02;
#define  Ucsi_cci              UCSI_RAM2             //_at_ OEMRAMC+0x04~0x07; 	// PPM -> OPM: USB Type-C Command Status and Connector Change Indication
#define UCSI_CCI_NOT_SUPPORTED_INDICATOR                                 			0x02	// bit25	IS_MASK_SET(Ucsi_cci[3],BIT1)
#define UCSI_CCI_CANCEL_COMPLETE_INDICATOR                               			0x04	// bit26	IS_MASK_SET(Ucsi_cci[3],BIT2)
#define UCSI_CCI_RESET_COMPLETE_INDICATOR                                			0x08	// bit27	IS_MASK_SET(Ucsi_cci[3],BIT3)
#define UCSI_CCI_BUSY_INDICATOR                                          			0x10	// bit28	IS_MASK_SET(Ucsi_cci[3],BIT4)
#define UCSI_CCI_ACKNOWLEDGE_INDICATOR                                   			0x20	// bit29	IS_MASK_SET(Ucsi_cci[3],BIT5)
#define UCSI_CCI_ERROR_INDICATOR                                         			0x40	// bit30	IS_MASK_SET(Ucsi_cci[3],BIT6)
#define UCSI_CCI_COMMAND_COMPLETE_INDICATOR                              			0x80	// bit31	IS_MASK_SET(Ucsi_cci[3],BIT7)
#define  Ucsi_control					 UCSI_RAM3             //_at_ OEMRAMC+0x08~0x0F;	// OPM -> PPM
#define			 UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE                        0x0001	//	IS_MASK_SET(Ucsi_control[0],BIT0)
#define			 UCSI_EXTERNAL_SUPPLY_CHANGE_NOTIFICATION_ENABLE                  0x0002	//	IS_MASK_SET(Ucsi_control[0],BIT1)
#define			 UCSI_POWER_OPERATION_MODE_CHANGE_NOTIFICATION_ENABLE             0x0004	//	IS_MASK_SET(Ucsi_control[0],BIT2)
#define			 UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE_NOTIFICATION_ENABLE  0x0020	//	IS_MASK_SET(Ucsi_control[0],BIT5)
#define			 UCSI_NEGOTIATED_POWER_LEVEL_CHANGE_NOTIFICATION_ENABLE           0x0040	//	IS_MASK_SET(Ucsi_control[0],BIT6)
#define			 UCSI_PD_RESET_COMPLETE_NOTIFICATION_ENABLE                       0x0080	//	IS_MASK_SET(Ucsi_control[0],BIT7)
#define			 UCSI_SUPPORTED_CAM_CHANGE_NOTIFICATION_ENABLE                    0x0100	//	IS_MASK_SET(Ucsi_control[1],BIT0) //bit8
#define			 UCSI_BATTERY_CHARGE_STATUS_CHANGE_NOTIFICATION_ENABLE            0x0200	//	IS_MASK_SET(Ucsi_control[1],BIT1) //bit9
#define			 UCSI_CONNECTOR_PARTNER_CHANGE_NOTIFICATION_ENABLE                0x0800	//	IS_MASK_SET(Ucsi_control[1],BIT3) //bit11
#define			 UCSI_POWER_DIRECTION_CHANGE_NOTIFICATION_ENABLE                  0x1000	//	IS_MASK_SET(Ucsi_control[1],BIT4) //bit12
#define			 UCSI_CONNECT_CHANGE_NOTIFICATION_ENABLE                          0x4000	//	IS_MASK_SET(Ucsi_control[1],BIT6) //bit14
#define			 UCSI_ERROR_NOTIFICATION_ENABLE                                   0x8000	//	IS_MASK_SET(Ucsi_control[1],BIT7) //bit15
#define  Ucsi_message_in       UCSI_RAM4             //_at_ OEMRAMC+0x10~0x1F;	// PPM -> OPM
#define  Ucsi_message_out      UCSI_RAM5             //_at_ OEMRAMC+0x20~0x2F;	// OPM -> PPM

#define CCI_Size               4
#define CCI_Control_Size       8
#define Message_in_Size        16
#define Message_out_Size       16

/*
*********************************************************
*     #define Constants
*********************************************************
*/

#define UCSI_COMMAND_PPM_RESET                                           0x01
#define UCSI_COMMAND_CANCEL                                              0x02
#define UCSI_COMMAND_CONNECTOR_RESET                                     0x03
#define UCSI_COMMAND_ACK_CC_CI                                           0x04
#define UCSI_COMMAND_SET_NOTIFICATION_ENABLE                             0x05
#define UCSI_COMMAND_GET_CAPABILITY                                      0x06
#define UCSI_COMMAND_GET_CONNECTOR_CAPABILITY                            0x07
#define UCSI_COMMAND_SET_UOM                                             0x08
#define UCSI_COMMAND_SET_UOR                                             0x09
#define UCSI_COMMAND_SET_PDM                                             0x0A
#define UCSI_COMMAND_SET_PDR                                             0x0B
#define UCSI_COMMAND_GET_ALTERNATE_MODES                                 0x0C
#define UCSI_COMMAND_GET_CAM_SUPPORTED                                   0x0D
#define UCSI_COMMAND_GET_CURRENT_CAM                                     0x0E
#define UCSI_COMMAND_SET_NEW_CAM                                         0x0F
#define UCSI_COMMAND_GET_PDOS                                            0x10
#define UCSI_COMMAND_GET_CABLE_PROPERTY                                  0x11
#define UCSI_COMMAND_GET_CONNECTOR_STATUS                                0x12
#define UCSI_COMMAND_GET_ERROR_STATUS                                    0x13

#define UCSI_PARM_MAX_DATA_LENGTH                                        0x10
#define UCSI_PARM_MAX_NUM_ALT_MODE                                       0x80
#define UCSI_PARM_MIN_TIME_TO_RESPOND_WITH_BUSY                          0x0A
#define UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH                           0x10

#define UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK                        (0x7F << 1)
#define UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT                       1
#define UCSI_CCI_DATA_LENGTH			                                       4
#define UCSI_CCI_DATA_LENGTH_MASK                                        0xff	//((WORD)0xff << 8)
#define UCSI_CCI_DATA_LENGTH_SHIFT                                       8

#define _SUPPORT_SET_UOM_					0
#define _SUPPORT_SET_PDM_					0
#define _SUPPORT_ALT_MODE_DETAILS_			1 //TODO
#define _SUPPORT_ALT_MODE_OVERRIDE_			0 //TODO
#define _SUPPORT_PDO_DETAIL_				1 //TODO
#define _SUPPORT_CABLE_DETAILS_				0 //TODO
#define _SUPPORT_EXT_SUPPLY_NOTIFICATION_	0 //
#define _SUPPORT_PD_RESET_NOTIFICATION_		0
#define UCSI_BM_OPTIONAL_FEATURES_SET_UOM_SUPPORTED                      (1 << 0)
#define UCSI_BM_OPTIONAL_FEATURES_SET_PDM_SUPPORTED                      (1 << 1)
#define UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_DETAILS_SUPPORTED       (1 << 2)
#define UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_OVERRIDE_SUPPORTED      (1 << 3)
#define UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED                  (1 << 4)
#define UCSI_BM_OPTIONAL_FEATURES_CABLE_DETAILS_SUPPORTED                (1 << 5)
#define UCSI_BM_OPTIONAL_FEATURES_EXTERNAL_SUPPLY_NOTIFICATION_SUPPORTED (1 << 6)
#define UCSI_BM_OPTIONAL_FEATURES_PD_RESET_NOTIFICATION_SUPPORTED        (1 << 7)

#define UCSI_BM_ATTRIBUTES_DISABLE_STATE_SUPPORT                         (1 << 0)
#define UCSI_BM_ATTRIBUTES_BATTERY_CHARGING                              (1 << 1)
#define UCSI_BM_ATTRIBUTES_USB_POWER_DELIVERY                            (1 << 2)
#define UCSI_BM_ATTRIBUTES_USB_TYPE_C_CURRENT                            (1 << 6)
#define UCSI_BM_ATTRIBUTES_POWER_SOURCE_MASK                             ((WORD)0xff << 8)
#define UCSI_BM_ATTRIBUTES_POWER_SOURCE_AC_SUPPLY                        (1 << 8)
#define UCSI_BM_ATTRIBUTES_POWER_SOURCE_OTHER                            (1 << 10)
#define UCSI_BM_ATTRIBUTES_POWER_SOURCE_USES_VBUS                        BIT6//(1 << 14)

#define UCSI_ERROR_STATUS_UNRECOGNIZED_COMMAND                           (1 << 0)
#define UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER                 (1 << 1)
#define UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS                    (1 << 2)
#define UCSI_ERROR_STATUS_INCOMPATIBLE_CONNECTOR_PARTNER                 (1 << 3)
#define UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR                         (1 << 4)
#define UCSI_ERROR_STATUS_COMMAND_UNSUCCESSFUL_DUE_TO_DEAD_BATTERY       (1 << 5)
#define UCSI_ERROR_STATUS_CONTRACT_NEGOTIATION_FAILURE                   (1 << 6)

#define UCSI_CONNECTOR_SUPPORTS_DFP_ONLY                                 (1 << 0)
#define UCSI_CONNECTOR_SUPPORTS_UFP_ONLY                                 (1 << 1)
#define UCSI_CONNECTOR_SUPPORTS_DRP                                      (1 << 2)
#define UCSI_CONNECTOR_SUPPORTS_AUDIO_ACCESSORY                          (1 << 3)
#define UCSI_CONNECTOR_SUPPORTS_DEBUG_ACCESSORY                          (1 << 4)
#define UCSI_CONNECTOR_SUPPORTS_USB_2                                    (1 << 5)
#define UCSI_CONNECTOR_SUPPORTS_USB_3                                    (1 << 6)
#define UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE                           (1 << 7)

#define UCSI_SOURCE_CAPABILITES_TYPE_CURRENT_SUPPORTED                   0
#define UCSI_SOURCE_CAPABILITES_TYPE_ADVERTISED                          1
#define UCSI_SOURCE_CAPABILITES_TYPE_MAXIMUM_SUPPORTED                   2

#define UCSI_CONNECTOR_STATUS_CHANGE_COMMAND_COMPLETED                   (1 << 0)
#define UCSI_CONNECTOR_STATUS_CHANGE_EXTERNAL_SUPPLY                     (1 << 1)
#define UCSI_CONNECTOR_STATUS_CHANGE_POWER_OPERATION_MODE                (1 << 2)
#define UCSI_CONNECTOR_STATUS_CHANGE_SUPPORTED_PROVIDER_CAPABILITIES     (1 << 5)
#define UCSI_CONNECTOR_STATUS_CHANGE_NEGOTIATED_POWER_LEVEL              (1 << 6)
#define UCSI_CONNECTOR_STATUS_CHANGE_PD_RESET_COMPLETE                   (1 << 7)
#define UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM                         (1 << 8)
#define UCSI_CONNECTOR_STATUS_CHANGE_BATTERY_CHARGE_STATUS               (1 << 9)
#define UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER                   (1 << 11)
#define UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION                     (1 << 12)
#define UCSI_CONNECTOR_STATUS_CHANGE_CONNECT                             (1 << 14)
#define UCSI_CONNECTOR_STATUS_CHANGE_ERROR                               (1 << 15)

#define UCSI_PROVIDER_CAPABILITY_LIMITED_REASON_POWER_BUDGET_LOWERED     (1 << 0)
#define UCSI_PROVIDER_CAPABILITY_LIMITED_REASON_REACHING_POWER_BUDGET_LIMIT (1 << 1)

#define UCSI_COMMAND_CONNECTOR_NUMBER           												((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22

#define UCSI_ACK_CC_CI_COMMAND_COMMAND                                   Ucsi_control[0]	//Control bit0~7)
#define UCSI_ACK_CC_CI_COMMAND_DATA_LENGTH                               	// bit8~15)
#define UCSI_ACK_CC_CI_COMMAND_CONNECTOR_CHANGE_ACKNOWLEDGE              IS_MASK_SET(Ucsi_control[2],BIT0)	//Control bit16
#define UCSI_ACK_CC_CI_COMMAND_COMMAND_COMPLETED_ACKNOWLEDGE             IS_MASK_SET(Ucsi_control[2],BIT1)	// bit17

#define UCSI_CONNECTOR_RESET_COMMAND_COMMAND                             	// bit0~7
#define UCSI_CONNECTOR_RESET_COMMAND_DATA_LENGTH                         	// bit8~15
#define UCSI_CONNECTOR_RESET_COMMAND_CONNECTOR_NUMBER                    ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_CONNECTOR_RESET_COMMAND_HARD_RESET                          IS_MASK_SET(Ucsi_control[2],BIT7)	//Control bit23

#define UCSI_SET_NOTIFICATION_ENABLE_COMMAND_COMMAND                     	// bit0~7
#define UCSI_SET_NOTIFICATION_ENABLE_COMMAND_DATA_LENGTH                 	// bit8~15
#define UCSI_SET_NOTIFICATION_ENABLE_COMMAND_NOTIFICATION_ENABLE         ((WORD)(Ucsi_control[3]<<8) + Ucsi_control[2])	//Control bit16~31

#define UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_COMMAND                    	// bit0~7
#define UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_DATA_LENGTH                	// bit8~15
#define UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER           ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_GET_CONNECTOR_CAPABILITY_DATA_OPERATION_MODE                Ucsi_message_in[0]	//Ucsi_message_in bit0~7
#define UCSI_GET_CONNECTOR_CAPABILITY_DATA_PROVIDER                      IS_MASK_SET(Ucsi_message_in[1],BIT0)	//Ucsi_message_in bit8
#define UCSI_GET_CONNECTOR_CAPABILITY_DATA_CONSUMER                      IS_MASK_SET(Ucsi_message_in[1],BIT1)	//Ucsi_message_in bit9

#define UCSI_SET_UOM_COMMAND_COMMAND                    	                // bit0~7
#define UCSI_SET_UOM_COMMAND_DATA_LENGTH                	                // bit8~15
#define UCSI_SET_UOM_COMMAND_CONNECTOR_NUMBER                           ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_SET_UOM_COMMAND_USB_OPERATION_MODE                          (((Ucsi_control[3]& 0x03)<<1)+(Ucsi_control[2]>>7))	//Control bit23~25
#define UCSI_SET_UOR_COMMAND_COMMAND                                     	// bit0~7
#define UCSI_SET_UOR_COMMAND_DATA_LENGTH                                 	// bit8~15
#define UCSI_SET_UOR_COMMAND_CONNECTOR_NUMBER                            ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_SET_UOR_COMMAND_USB_OPERATION_ROLE                          (((Ucsi_control[3]& 0x03)<<1)+(Ucsi_control[2]>>7))	//Control bit23~25

#define UCSI_SET_PDM_COMMAND_COMMAND                                     	// bit0~7
#define UCSI_SET_PDM_COMMAND_DATA_LENGTH                                 	// bit8~15
#define UCSI_SET_PDM_COMMAND_CONNECTOR_NUMBER                            ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_SET_PDM_COMMAND_POWER_DIRECTION_MODE                        (((Ucsi_control[3]& 0x03)<<1)+(Ucsi_control[2]>>7))	//Control bit23~25
#define UCSI_SET_PDR_COMMAND_COMMAND                                     	// bit0~7
#define UCSI_SET_PDR_COMMAND_DATA_LENGTH                                 	// bit8~15
#define UCSI_SET_PDR_COMMAND_CONNECTOR_NUMBER                            ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_SET_PDR_COMMAND_POWER_DIRECTION_ROLE                        (((Ucsi_control[3]& 0x03)<<1)+(Ucsi_control[2]>>7))	//Control bit23~25

#define UCSI_GET_ALTERNATE_MODES_COMMAND_COMMAND                         	// bit0~7
#define UCSI_GET_ALTERNATE_MODES_COMMAND_DATA_LENGTH                     	// bit8~15
#define UCSI_GET_ALTERNATE_MODES_COMMAND_RECIPIENT                       	(Ucsi_control[2] & 0x07)// bit16~18
#define UCSI_GET_ALTERNATE_MODES_COMMAND_CONNECTOR_NUMBER                	((Ucsi_control[3] & 0x7F) - 1)// bit24~30
#define UCSI_GET_ALTERNATE_MODES_COMMAND_ALTERNATE_MODE_OFFSET           	Ucsi_control[4]// bit32~39
#define UCSI_GET_ALTERNATE_MODES_COMMAND_NUMBER_OF_ALTERNATE_MODES       	(Ucsi_control[5] & 0x03)// bit40~41
#define UCSI_GET_ALTERNATE_MODES_DATA_SVID_0                             	// bit0~15
#define UCSI_GET_ALTERNATE_MODES_DATA_MID_0                              	// bit16~47
#define UCSI_GET_ALTERNATE_MODES_DATA_SVID_1                             	// bit48~63
#define UCSI_GET_ALTERNATE_MODES_DATA_MID_1                              	// bit64~95

#define UCSI_GET_CAM_SUPPORT_COMMAND_COMMAND                                     	// bit0~7
#define UCSI_GET_CAM_SUPPORT_COMMAND_DATA_LENGTH                                 	// bit8~15
#define UCSI_GET_CAM_SUPPORT_COMMAND_CONNECTOR_NUMBER                            ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_GET_CURRENT_CAM_COMMAND_COMMAND                                     	// bit0~7
#define UCSI_GET_CURRENT_CAM_COMMAND_DATA_LENGTH                                 	// bit8~15
#define UCSI_GET_CURRENT_CAM_COMMAND_CONNECTOR_NUMBER                            ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22


/*
*         SMBus Status code definition
*/
#define SMB_STS_STATUS_OK              0       //SMBus OK
#define SMB_STS_STATUS_CMD_ACC_DENIED  0x12    //SMBus Device Command Aceess
#define SMB_STS_STATUS_DATA_ACC_DENIED 0x17    //SMBus Device Access Denied

#define UCSI_GET_PDOS_COMMAND_COMMAND                                    	// bit0~7
#define UCSI_GET_PDOS_COMMAND_DATA_LENGTH                                	// bit8~15
#define UCSI_GET_PDOS_COMMAND_CONNECTOR_NUMBER                           ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_GET_PDOS_COMMAND_PARTNER_PDOS                               IS_MASK_SET(Ucsi_control[2],BIT7)	//Control bit23
#define UCSI_GET_PDOS_COMMAND_PDO_OFFSET                                 Ucsi_control[3]	//Control bit24~31
#define UCSI_GET_PDOS_COMMAND_NUMBER_OF_PDOS                             ((Ucsi_control[4] & 0x03) + 1)	//Control bit32~33
#define UCSI_GET_PDOS_COMMAND_SOURCE_OR_SINK_PDOS                        IS_MASK_SET(Ucsi_control[4],BIT2)	//Control bit34
#define UCSI_GET_PDOS_COMMAND_SOURCE_CAPABILITIES_TYPE                   ((Ucsi_control[4]>>3) & 0x03)	//Control bit35~36
#define UCSI_GET_PDOS_DATA_PDO_0                                         	// bit0~31
#define UCSI_GET_PDOS_DATA_PDO_1                                         	// bit32~63
#define UCSI_GET_PDOS_DATA_PDO_2                                         	// bit64~95
#define UCSI_GET_PDOS_DATA_PDO_3                                         	// bit96~127

#define UCSI_GET_CONNECTOR_STATUS_COMMAND_COMMAND                        	// bit0~7
#define UCSI_GET_CONNECTOR_STATUS_COMMAND_DATA_LENGTH                    	// bit8~15
#define UCSI_GET_CONNECTOR_STATUS_COMMAND_CONNECTOR_NUMBER               ((Ucsi_control[2] & 0x7F) - 1)	//Control bit16~22
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECTOR_STATUS_CHANGE           	// bit0~15
#define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE              ((Ucsi_control[2] & 0x7F) - 1)	//Ucsi_message_in bit16~18
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER  0
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_USB_DEFAULT  1
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_BC           2
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_PD           3
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_1_5_A 4
    #define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_3_0_A 5
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS                    BIT3//IS_MASK_SET(Ucsi_message_in[2],BIT3)	//Ucsi_message_in bit19
#define UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION                   BIT4//IS_MASK_SET(Ucsi_message_in[2],BIT4)	//Ucsi_message_in bit20
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS             ((Ucsi_message_in[3]<<3)+(Ucsi_message_in[2]>>5))	//Ucsi_message_in bit21~28
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB         BIT5//IS_MASK_SET(Ucsi_message_in[2],BIT5)	//Ucsi_message_in bit21
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_ALT         BIT6//IS_MASK_SET(Ucsi_message_in[2],BIT5)	//Ucsi_message_in bit21
#define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE              ((Ucsi_message_in[3] & 0xE0)>>5)	//Ucsi_message_in bit29~31
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP              1
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP              2
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_POWERED_NO_UFP   3
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_POWERED_UFP      4
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DEBUG_ACCESSORY  5
    #define UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_AUDIO_ACCESSORY  6
#define TI_CONNECTOR_STATUS_NO_CONNECTION                                       0
#define TI_CONNECTOR_STATUS_PORT_DISABLED                                       1
#define TI_CONNECTOR_STATUS_AUDIO_CONNECTION                                    2
#define TI_CONNECTOR_STATUS_DEBUG_CONNECTION                                    3
#define TI_CONNECTOR_STATUS_NO_CONNECTION_RA_NO_RD                              4
#define TI_CONNECTOR_STATUS_RESERVE                                             5
#define TI_CONNECTOR_STATUS_CONNECTION_RD_NO_RA                                 6
#define TI_CONNECTOR_STATUS_CONNECTION_RD_RA                                    7
#define UCSI_GET_CONNECTOR_STATUS_DATA_REQUEST_DATA_OBJECT               ((LWORD)(Ucsi_message_in[7]<<24) + (Ucsi_message_in[6]<<16) + (Ucsi_message_in[5]<<8) + Ucsi_message_in[4])	//Ucsi_message_in bit32~63
#define UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS           (Ucsi_message_in[8] & 0xFC)	//Ucsi_message_in bit64~65
    #define UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NO_CHARGING      0
    #define UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NOMINAL_CHARGING 1
    #define UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_SLOW_CHARGING    2
    #define UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_TRICKLE_CHARGING 3
#define UCSI_GET_CONNECTOR_STATUS_DATA_PROVIDER_CAPABILITY_LIMITED_REASON ((Ucsi_message_in[8] & 0x3C)>>2)	//Ucsi_message_in bit66~69
#define UCSI_GET_CONNECTOR_STATUS_DATA_RESERVED                          ((Ucsi_message_in[8] & 0xC0)>>6)	//Ucsi_message_in bit70~71


/*
*         SMBus Protocol definition
*/

#define SMB_PROTO_WRITE_QUICK 0x02
#define SMB_PROTO_READ_QUICK  0x03
#define SMB_PROTO_SEND_BYTE   0x04
#define SMB_PROTO_RCV_BYTE    0x05
#define SMB_PROTO_WRITE_BYTE  0x06
#define SMB_PROTO_READ_BYTE   0x07
#define SMB_PROTO_WRITE_WORD  0x08
#define SMB_PROTO_READ_WORD   0x09
#define SMB_PROTO_WRITE_BLOCK 0x0A
#define SMB_PROTO_READ_BLOCK  0x0B
#define SMB_PROTO_PROC_CALL   0x0C
//#define ;MB_PROTO_            0x0D
#define SMB_PROTO_WRITE_BYTES 0x0E
#define SMB_PROTO_READ_BYTES  0x0F

/*
*********************************************************
*     Global Data Types
*********************************************************
*/

//struct _UCSI_DATA_STRUCTURE {
//    unsigned short version;           // PPM -> OPM
//    unsigned short reserved0;
//    unsigned long  cci;               // PPM -> OPM: USB Type-C Command Status and Connector Change Indication
//    unsigned char  control[8];        // OPM -> PPM
//    unsigned char  message_in[16];    // PPM -> OPM
//    unsigned char  message_out[16];   // OPM -> PPM
//};

/*
*********************************************************
*     Global Function Prototypes
*********************************************************
*/

extern void UcsiInit(void);
extern WORD	UcsiGetVersion(void);
extern BYTE UcsiGetCci(BYTE *TempData, BYTE length);
extern void UcsiHousekeeper(BYTE task_id);
extern void	UcsiSetControl(BYTE *TempData, BYTE length);
extern BYTE	UcsiGetControl(BYTE *TempData, BYTE max_length);
extern BYTE	UcsiGetMessageIn(BYTE *TempData, BYTE max_length);
extern void	UcsiSetMessageOut(BYTE *TempData, BYTE length);
extern BYTE	UcsiGetMessageOut(BYTE *TempData, BYTE max_length);
extern void UcsiUsbPdcStatusChanged(USB_POWER_STATUS power_status[NUMBER_OF_USB_PDC], USB_STATUS status[NUMBER_OF_USB_PDC]);
extern void UcsiFuelStateChanged(BYTE sttae);
extern BYTE GetPdPortAddress(BYTE ReqConnector);
extern BYTE GetPdDeviceConnect(BYTE ReqConnector);


#endif

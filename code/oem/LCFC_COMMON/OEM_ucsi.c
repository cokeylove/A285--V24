/*
********************************************************************************
* Lenovo Confidential
*
* Microcode Source Materials
*
* ThinkPad Embedded Controller Microcode
*
* (C) COPYRIGHT LENOVO 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_UCSI.c
*
* Description:
*
*      USB Type-C System Interface
*
* History:
*    10/27/2016    Tetsuji Nakamura
*       created as new
********************************************************************************
*/

//#include <CORE_INCLUDE.H>
//#include <OEM_INCLUDE.H>
#include "..\..\include.h"
/*
*******************************************************************************
*       Local Macros
*******************************************************************************
*/

//#define UCSI_OPTIONAL_FEATURE (UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED) //(1 << 0)  // Isaac check
#define UCSI_ATTRIBUTE0 (UCSI_BM_ATTRIBUTES_USB_POWER_DELIVERY | UCSI_BM_ATTRIBUTES_USB_TYPE_C_CURRENT)

#define UCSI_ATTRIBUTE1 (UCSI_BM_ATTRIBUTES_POWER_SOURCE_USES_VBUS)
#define NUMBER_OF_ALT_MODES		2


/*
*******************************************************************************
*       Local Data Type
*******************************************************************************
*/

enum {
    UCSI_REQUEST_NONE,
    UCSI_REQUEST_HARD_RESET,
    UCSI_REQUEST_SOFT_RESET,
    UCSI_REQUEST_GET_TX_SOURCE_PDOS_CURRENT_SUPPORTED,
    UCSI_REQUEST_GET_TX_SOURCE_PDOS_ADVERTIZED,
    UCSI_REQUEST_GET_TX_SOURCE_PDOS_MAXIMUM_SUPPORTED,
    UCSI_REQUEST_GET_RX_SINK_PDOS,
    UCSI_REQUEST_GET_RX_SOURCE_PDOS,
    UCSI_REQUEST_GET_TX_SINK_PDOS,
    UCSI_REQUEST_SWAP_TO_SINK,
    UCSI_REQUEST_SWAP_TO_SOURCE,
    UCSI_REQUEST_SWAP_TO_UFP,
    UCSI_REQUEST_SWAP_TO_DFP,
    UCSI_REQUEST_OPERATE_AS_DFP,
    UCSI_REQUEST_OPERATE_AS_UFP,
    UCSI_REQUEST_OPERATE_AS_SOURCE,
    UCSI_REQUEST_OPERATE_AS_SINK,
    UCSI_REQUEST_GET_ALTERNATE_MODES,
    UCSI_REQUEST_GET_CURRENT_CAM,
};

enum {
    UCSI_INIT,
    UCSI_WAIT_REQ,
    UCSI_SEND_REQUEST,
    UCSI_WAIT_REQUEST_DONE,
    //UCSI_WAIT_EVENT,
};

struct _UCSI_FLAG {
    BYTE busy:        1;
    BYTE cancel:      1;
};

typedef struct _UCSI_FLAG UCSI_FLAG;

struct _UCSI_REQUEST {
    BYTE request:          8;
    BYTE connector:        8;
    BYTE offset:           8;
    BYTE number_of_pdo:    8;
};

typedef struct _UCSI_REQUEST UCSI_REQUEST;

struct _USB_PDC_CHARGE_STATUS {
    BOOL quick_charge:   1;
    BOOL trickle_charge: 1;
};

typedef struct _USB_PDC_CHARGE_STATUS USB_PDC_CHARGE_STATUS;

//typedef enum {POWER_SOURCE_TYPE_NONE,
//              POWER_SOURCE_TYPE_FIXED_SUPPLY,
//              POWER_SOURCE_TYPE_BATTERY,
//              POWER_SOURCE_TYPE_VARIABLE_SUPPLY} POWER_SOURCE_TYPE;

/*
*******************************************************************************
*       Local Variable
*******************************************************************************
*/

//static UCSI_DATA_STRUCTURE   Ucsi_Data;  // Isaac check
static UCSI_FLAG             Ucsi_Flag;
static UCSI_REQUEST            Ucsi_Request, Ucsi_Request_Executing;
static WORD                  Ucsi_Error_Status;
//static BYTE                  Ucsi_Connector_Number_Changed;
//static WORD                  Ucsi_Notification_Enable;
//static WORD                  Ucsi_Connector_Change_Status[NUMBER_OF_USB_PDC];
static BYTE                  Fsm;
//static USB_POWER_STATUS      Usb_Pdc_Power_Status[NUMBER_OF_USB_PDC];
//static USB_STATUS            Usb_Pdc_Status[NUMBER_OF_USB_PDC];
static USB_PDC_CHARGE_STATUS Usb_Pdc_Charge_Status[NUMBER_OF_USB_PDC];
//static BYTE                  UCSI_OPTIONAL_FEATURE;
//static BYTE                  Ucsi_Event_Changed;
//static BYTE                  Ucsi_Device_Connected;
static BYTE                  Ucsi_Error_Set;

/*
*******************************************************************************
*       Externals
*******************************************************************************
*/

/*
*******************************************************************************
*       Local Function Prototypes
*******************************************************************************
*/

//static void Ucsi_Usb_Pdc_Req_Done(BYTE result);

/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

//extern void UcsiInit(void);
//extern BOOL UcsiHousekeeper(BYTE task_id);

/*
*******************************************************************************
* Function name: UcsiInit
*
* Descriptoin:
*          This is invoked when the system is powered on
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void UcsiInit(void) {
    Ucsi_version        = 0x0100;
    Fsm                 = 0;
}

/*
*******************************************************************************
* Function name: UcsiHousekeeper
*
* Descriptoin:
*          This is used to control USB-C PD controllers.
*
* Arguments:
*       int task_id
* Return Values:
*       BOOL : TRUE if allow to enter deep sleep
*******************************************************************************
*/
static void CCI_Clear(void) {
    Ucsi_cci[0] = 0;
    Ucsi_cci[1] = 0;
    Ucsi_cci[2] = 0;
    Ucsi_cci[3] = 0;
    return ;
}

//static void CCI_SetConnectorChangeIndicator(const BYTE connector_number) {
    // Move connector number to bit 7~1.
//ivan    Ucsi_cci[0] = ((Ucsi_cci[0]&0x01)|((connector_number<<1)&0xFE));
//    Ucsi_cci[0] = ((Ucsi_cci[0]&0x01)|((connector_number<<1)&0xFE));
//    return ;
//}

void UcsiHousekeeper(BYTE task_id) {
    BOOL               rc;
    static USB_PDC_REQ pdc_req;
    BYTE              cmd, i;//, index;//, partner_type;
    //BYTE                counter;
    //WORD             current, voltage, Tempdata;
    //DWORD	capability;
		BYTE TempAddr,device_connected;

    rc = FALSE;
    switch (Fsm) {
        case UCSI_INIT:
             /*
             *   Initialize valiables and status of PDC
             */
             if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
             {
#if DEBUG_UCSI
RamDebug(0xC0);
#endif
                 CCI_Clear();
	             Ucsi_Request.request           = UCSI_REQUEST_NONE;
	             Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
	             for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
	                 UsbPdcGetPowerStatus(i);
	             }
	             Fsm++;
	           }
             break;
        case UCSI_WAIT_REQ:

            if(Read_TYPEC_INT())
            {
               for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
               	TempAddr = GetPdPortAddress(i);
                device_connected = GetPdDeviceConnect(i);
                if(device_connected == TRUE)
                {
                    if(!UCSI_DEVICE_CONNECTED[i])
                    {
#if 1
RamDebug(0xEA);
RamDebug(i);
#endif
                        //A285D00061+
                        UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECT|UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM);
                        UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                        UCSI_DEVICE_CONNECTED[i] = 1;
                        //for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
        	                 UsbPdcGetPowerStatus(i);
                             //UsbPdcGetPowerRole(0);
        	            //}
                        //CCI_SetConnectorChangeIndicator(i+1);
                        ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q0:AM_UCSI_EVENT]\n");
                    }
                    if(!UCSI_CONNECTOR_NUMBER_CHANGED[i])
                    {
                        if(UCSI_NEG_POWER_LEVEL_CHANGE)
                        {
                            // Negotiated power level change.
                            UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_NEGOTIATED_POWER_LEVEL | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                            UCSI_NEG_POWER_LEVEL_CHANGE = 0;
                            //for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
            	                 UsbPdcGetPowerStatus(i);
            	            //}
                            //CCI_SetConnectorChangeIndicator(i+1);
                            ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q1:AM_UCSI_EVENT]\n");
                        }
                        if(UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE)
                        {
                            // Supported provider capabilities change.
                            UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORTED_PROVIDER_CAPABILITIES | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                            UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE = 0;
                            //for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
            	                 UsbPdcGetPowerStatus(i);
            	            //}
                            //CCI_SetConnectorChangeIndicator(i+1);
                            ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q2:AM_UCSI_EVENT]\n");
                        }
                        if(UCSI_POWER_DIRECTION_CHANGE)
                        {
                            // Power direction change.
                            UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT); //printf ("("@A1\n");
                            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                            UCSI_POWER_DIRECTION_CHANGE = 0; //printf ("("@B7\n");
                            //CCI_SetConnectorChangeIndicator(i+1);
                            ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q3:AM_UCSI_EVENT]\n");
                        }
                        if(UCSI_CONNECTOR_PARTNER_CHANGE)
                        {
                            // Connector partner change.
                            UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                            UCSI_CONNECTOR_PARTNER_CHANGE = 0;
                            UsbPdcGetDataRole(i);
                            /*for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
            	                 UsbPdcGetPowerStatus(i);
            	            }*/
                            //CCI_SetConnectorChangeIndicator(i+1);
                            ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q4:AM_UCSI_EVENT]\n");
                        }
                    }

}
                else
                {
                    if(UCSI_DEVICE_CONNECTED[i])
                    {
#if 1
RamDebug(0xEB);
RamDebug(i);
#endif
                        UCSI_CONNECTOR_CHANGE_STATUS[i] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
                        UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;
                        UCSI_DEVICE_CONNECTED[i] = 0;
                        UCSI_4CC_CMD_COMPLETE = FALSE;
                        UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                        UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                        //CCI_SetConnectorChangeIndicator(i+1);
                        ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q5:AM_UCSI_EVENT]\n");
                    }
                }
              }
            }

             if (UCSI_REQUEST_NONE != Ucsi_Request.request) {
#if DEBUG_UCSI
RamDebug(0xC1);
RamDebug(Ucsi_Request.request);
#endif
                 /*
                 *   a request is set then start to execute
                 */
                 Ucsi_Request_Executing = Ucsi_Request;
                 Fsm++;
             } else {
                 rc = TRUE;
             }
             break;
        /*case UCSI_WAIT_EVENT:

                //if( (Ucsi_cci[0] == 0) && (Ucsi_cci[3] == 0) )
        		{
        			if( Ucsi_Notification_Enable & UCSI_CONNECTOR_STATUS_CHANGE_COMMAND_COMPLETED )
        			{
        			#if DEBUG_UCSI
        			RamDebug(0xCD);
                    #endif
        				Ucsi_Connector_Number_Changed = 1;
        			}
        		}
                Ucsi_Event_Changed = 0;
                Fsm = UCSI_WAIT_REQ;
            break;*/
        case UCSI_SEND_REQUEST:
             /*
             *    select a command of USB PD Controller module according to the request
             */
#if DEBUG_UCSI
RamDebug(0xC2);
RamDebug(Ucsi_Request_Executing.request);
#endif
             switch (Ucsi_Request_Executing.request) {
                 case UCSI_REQUEST_HARD_RESET:
                      cmd   = USB_PDC_REQ_HARD_RESET;
                      break;
                 case UCSI_REQUEST_SOFT_RESET:
                      cmd   = USB_PDC_REQ_SOFT_RESET;
                      break;
                 case UCSI_REQUEST_GET_RX_SOURCE_PDOS:
                      cmd   = USB_PDC_REQ_GET_RX_SOURCE_PDO;
                      break;
                 case UCSI_REQUEST_GET_RX_SINK_PDOS:
                      cmd   = USB_PDC_REQ_GET_RX_SINK_PDO;
                      break;
                 case UCSI_REQUEST_GET_TX_SOURCE_PDOS_CURRENT_SUPPORTED:
                 case UCSI_REQUEST_GET_TX_SOURCE_PDOS_ADVERTIZED:
                 case UCSI_REQUEST_GET_TX_SOURCE_PDOS_MAXIMUM_SUPPORTED:
                      cmd   = USB_PDC_REQ_GET_TX_SOURCE_PDO;
                      break;
                 case UCSI_REQUEST_GET_TX_SINK_PDOS:
                      cmd   = USB_PDC_REQ_GET_TX_SINK_PDO;
                      break;
                 case UCSI_REQUEST_SWAP_TO_SINK:
                      cmd   = USB_PDC_REQ_SWAP_TO_SINK;
                      break;
                 case UCSI_REQUEST_SWAP_TO_SOURCE:
                      cmd   = USB_PDC_REQ_SWAP_TO_SOURCE;
                      break;
                 case UCSI_REQUEST_SWAP_TO_UFP:
                      cmd   = USB_PDC_REQ_SWAP_TO_UFP;
                      break;
                 case UCSI_REQUEST_SWAP_TO_DFP:
                      cmd   = USB_PDC_REQ_SWAP_TO_DFP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_DFP:
                      cmd   = USB_PDC_REQ_OPERATE_AS_DFP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_UFP:
                      cmd   = USB_PDC_REQ_OPERATE_AS_UFP;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_SOURCE:
                      cmd   = USB_PDC_REQ_OPERATE_AS_SOURCE;
                      break;
                 case UCSI_REQUEST_OPERATE_AS_SINK:
                      cmd   = USB_PDC_REQ_OPERATE_AS_SINK;
                      break;
                 case UCSI_REQUEST_GET_ALTERNATE_MODES:
                      cmd   = USB_PDC_REQ_GET_ALTERNATE_MODES;
                      break;
                 case UCSI_REQUEST_GET_CURRENT_CAM:
                      cmd   = USB_PDC_REQ_GET_CURRENT_CAM;
                      break;
                 default:
                      /*
                      *    Request may be canceled
                      */
                      if ((Ucsi_Request.request   == Ucsi_Request_Executing.request)   &&
                          (Ucsi_Request.connector == Ucsi_Request_Executing.connector) &&
                          (Ucsi_Request.offset    == Ucsi_Request_Executing.offset)){
                          /*
                          *     New Request have not come
                          */
                          Ucsi_cci[3]          &= ~UCSI_CCI_BUSY_INDICATOR;
                          Ucsi_cci[3]          |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                          if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
#if DEBUG_UCSI
RamDebug(0xF2);
RamDebug(Ucsi_cci[0]);
RamDebug(Ucsi_cci[3]);
#endif
                              ECSMI_SCIEvent(AM_UCSI_EVENT);
                          }
                          Ucsi_Request.request   = UCSI_REQUEST_NONE;
                          Ucsi_Request_Executing = Ucsi_Request;
                      } else {
                          Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
                      }
             }
             if (UCSI_REQUEST_NONE != Ucsi_Request_Executing.request) {
                 /*
                 *    send a request to a USB-C PDC
                 */
#if DEBUG_UCSI
RamDebug(0xC3);
RamDebug(pdc_req.status.done);
RamDebug(pdc_req.request);
#endif
                 UsbPdcRequest(Ucsi_Request_Executing.connector, &pdc_req, cmd,Message_in_Size, 0, Ucsi_message_in);
#if DEBUG_UCSI
RamDebug(0x3C);
RamDebug(IS_MASK_SET(TypeCStatus2,PdcRequestDone));
#endif
                 /*if(!UsbPdcRequest(Ucsi_Request_Executing.connector, &pdc_req, cmd,Message_in_Size, 0, Ucsi_message_in))
                 {RamDebug(0x3C);
                     RamDebug(IS_MASK_SET(TypeCStatus2,PdcRequestDone));

                     Fsm++;
                 }*/
                 if(UCSI_CONNECTOR_RESET_WAIT == 0)
                    Fsm++;
             } else {
                 Fsm = UCSI_WAIT_REQ;
             }
             break;
        case UCSI_WAIT_REQUEST_DONE:
             //if (TRUE == pdc_req.status.done) {
             if (IS_MASK_SET(TypeCStatus2,PdcRequestDone)) {
#if DEBUG_UCSI
RamDebug(0xC4);
#endif
                 /*
                 *    the request has been done
                 */
                 if ((Ucsi_Request.request   == Ucsi_Request_Executing.request)   &&
                     (Ucsi_Request.connector == Ucsi_Request_Executing.connector) &&
                     (Ucsi_Request.offset    == Ucsi_Request_Executing.offset)){
                     /*
                     *     New Request have not come
                     */

             //RamDebug(0xC5);
                     if ((USB_PDC_RC_OK == pdc_req.result)&&(UCSI_POWER_DATA_SOURCE_REJECT == FALSE)) {
						             //Ucsi_cci[0] = 0;
						             //Ucsi_cci[1] = 0;
						             //Ucsi_cci[2] = 0;
						             Ucsi_cci[3] = 0;
#if DEBUG_UCSI
RamDebug(0xC6);
RamDebug(Ucsi_Request_Executing.request);
RamDebug(Ucsi_Request.number_of_pdo);
RamDebug(pdc_req.parameter[0]);
RamDebug(Ucsi_Request_Executing.offset);
#endif
                         switch (Ucsi_Request_Executing.request) {
                             case UCSI_REQUEST_GET_RX_SOURCE_PDOS:
                             case UCSI_REQUEST_GET_RX_SINK_PDOS:
                             case UCSI_REQUEST_GET_TX_SINK_PDOS:
                             case UCSI_REQUEST_GET_TX_SOURCE_PDOS_CURRENT_SUPPORTED:
                                  /*
                                  *   move PDOs to Message-In area
                                  */
                                 // if(Ucsi_Request.number_of_pdo > pdc_req.pdos_valid)
                                 //   Ucsi_Request.number_of_pdo = pdc_req.pdos_valid;
                                  //RamDebug(Ucsi_Request.number_of_pdo);
                                 /* counter = 0;
                                  for (i = 0; (4 > i) &&
                                        (Ucsi_Request.number_of_pdo > i) &&
                                        (pdc_req.parameter[0] > (i + Ucsi_Request_Executing.offset));
                                       i++) {
                                      Ucsi_message_in[i * 4] = Ucsi_message_in[(Ucsi_Request_Executing.offset + i) * 4];
                                      Ucsi_message_in[i * 4 + 1] = Ucsi_message_in[(Ucsi_Request_Executing.offset + i) * 4 + 1];
                                      Ucsi_message_in[i * 4 + 2] = Ucsi_message_in[(Ucsi_Request_Executing.offset + i) * 4 + 2];
                                      Ucsi_message_in[i * 4 + 3] = Ucsi_message_in[(Ucsi_Request_Executing.offset + i) * 4 + 3];
                                      counter++;
                                  }
                                  RamDebug(counter);
                                  Ucsi_cci[1] = (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK) | (counter * 4);
                                  RamDebug(Ucsi_cci[1]);*/
                                  break;
                             case UCSI_REQUEST_GET_TX_SOURCE_PDOS_ADVERTIZED:
                                  /*
                                  *   move advertised PDOs to Message-In area
                                  */
                                  /*counter = 0;
                                  for (i = 0; (pdc_req.parameter[0] > i); i++) {
                                      if (0 != (pdc_req.parameter[1] & (1 << i))) {
                                          if (counter > Ucsi_Request_Executing.offset) {*/
                                              /*
                                              *   move a PDO marked advertised
                                              */
                                /*              index =  counter - Ucsi_Request_Executing.offset;
                                              Ucsi_message_in[index * 4] = Ucsi_message_in[i * 4];
                                              Ucsi_message_in[index * 4 + 1] = Ucsi_message_in[i * 4 + 1];
                                              Ucsi_message_in[index * 4 + 2] = Ucsi_message_in[i * 4 + 2];
                                              Ucsi_message_in[index * 4 + 3] = Ucsi_message_in[i * 4 + 3];
                                          }
                                          counter++;
                                          if ((4 <= counter) || (Ucsi_Request.number_of_pdo < counter)) {
                                              break;
                                          }
                                      }
                                  }*/
                                  /*
                                  *   set data length
                                  */
                                /*  RamDebug(counter);
                                  Ucsi_cci[1] = (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK) | ((counter - Ucsi_Request_Executing.offset) * 4);
                                  RamDebug(Ucsi_cci[1]);*/
                                  break;
                             case UCSI_REQUEST_GET_TX_SOURCE_PDOS_MAXIMUM_SUPPORTED:
                                  //capability = 0;
                                  //counter = 0;

                                  /*
                                  *   find the PDO have biggest capability
                                  */
                                  //for (i = 0; pdc_req.parameter[0] > i; i++) {
                                      /*
                                      *   Get capability of the PDO
                                      */
                                   /*   Tempdata = (Ucsi_message_in[(i * 4) + 3] & 0xC0) >> 6;
                                      switch (Tempdata) {
                                      case PDO_TYPE_FIXED_SUPPLY:
                                          voltage = (Ucsi_message_in[(i * 4) + 2] * 64) + (Ucsi_message_in[(i * 4) + 1] >> 2);	//50mV
                                          current = (Ucsi_message_in[(i * 4) + 1] * 256) + Ucsi_message_in[i * 4];	//10mA
                                          break;
                                      case PDO_TYPE_BATTERY:
                                          voltage = (Ucsi_message_in[(i * 4) + 2] * 64) + (Ucsi_message_in[(i * 4) + 1] >> 2);	//50mV
                                          current = (Ucsi_message_in[(i * 4) + 1] * 256) + Ucsi_message_in[i * 4];	//10mA
                                          break;
                                      case PDO_TYPE_VARIABLE_SUPPLY:
                                          voltage = (Ucsi_message_in[(i * 4) + 2] * 64) + (Ucsi_message_in[(i * 4) + 1] >> 2);	//50mV
                                          current = (Ucsi_message_in[(i * 4) + 1] * 256) + Ucsi_message_in[i * 4];	//10mA
                                          break;
                                      default:
                                          voltage = 0;
                                          current = 0;
                                      }*/
                                      /*
                                      *   Update the position of max. capability PDO
                                      */
                                     /* if (capability < (voltage * current)) {
                                          counter    = i;
                                          capability = voltage * current;
                                      }
                                  }
                                  if (0 < capability) {*/
                                      /*
                                      *    move the PDO at the top
                                      */
                                     /* Ucsi_message_in[0] =  Ucsi_message_in[counter * 4];
                                      Ucsi_message_in[1] =  Ucsi_message_in[counter * 4 + 1];
                                      Ucsi_message_in[2] =  Ucsi_message_in[counter * 4 + 2];
                                      Ucsi_message_in[3] =  Ucsi_message_in[counter * 4 + 3];*/
//                                      Ucsi_cci[1] = (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK) | 4;
                                      /*Ucsi_cci[1] = 4;  // Isaac check

                                  } else {*/
                                      /*
                                      *    there is no PDO
                                      */
//                                      Ucsi_cci[1] = (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK);
                                     /* Ucsi_cci[1] = 0;  // Isaac check

                                  }*/
                                  break;
                         }
                     } else {
                         /*
                         *     Done with an error
                         */
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CONTRACT_NEGOTIATION_FAILURE;
                       //Ucsi_cci[0]     = 0;
                         Ucsi_cci[1]     = 0;
                       //Ucsi_cci[2]     = 0;
                         Ucsi_cci[3]     = (UCSI_CCI_ERROR_INDICATOR | UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
                         UCSI_POWER_DATA_SOURCE_REJECT = FALSE;
                     }
                     /*
                     *   set completed
                     */
                     Ucsi_cci[3]          &= ~UCSI_CCI_BUSY_INDICATOR;
                     Ucsi_cci[3]          |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                     if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
#if DEBUG_UCSI
RamDebug(0xF3);
RamDebug(Ucsi_cci[0]);
RamDebug(Ucsi_cci[3]);
#endif
                         ECSMI_SCIEvent(AM_UCSI_EVENT);
                     }
                     Ucsi_Request.request   = UCSI_REQUEST_NONE;
                     Ucsi_Request_Executing = Ucsi_Request;
                 } else {
                     /*
                     *     New Request have come then ignore the result
                     */
                     Ucsi_Request_Executing.request = UCSI_REQUEST_NONE;
				           //Ucsi_cci[0] = 0;
				             Ucsi_cci[1] = 0;
				           //Ucsi_cci[2] = 0;
				             Ucsi_cci[3] = 0;
                 }
                 Fsm = UCSI_WAIT_REQ;
             } else {
                 /*
                 *   the request is not done yet
                 *///RamDebug(0xC7);
                 Fsm--;
                 rc = TRUE;
             }
             break;
            default:
             break;
    }
    if (TRUE == rc) {
        ;//TskWaitTask(task_id);
    }
    //return rc;
}

/*
*******************************************************************************
* Function name: Ucsi_Usb_Pdc_Req_Done
*
* Descriptoin:
*          This is invoked when a request to USB-C PDC is done
*
* Arguments:
*       UCHAR result
* Return Values:
*       none
*******************************************************************************
*/

//static void Ucsi_Usb_Pdc_Req_Done(BYTE result) {
//
//    //TskActivateTask(TASK_UCSI);
//}

/*
*******************************************************************************
* Function name: UcsiGetVersion
*
* Descriptoin:
*          This is used to get contents of version area.
*
* Arguments:
*       none
* Return Values:
*       USHORT : version
*******************************************************************************
*/

WORD UcsiGetVersion(void) {
    return Ucsi_version;
}

/*
*******************************************************************************
* Function name: UcsiGetCci
*
* Descriptoin:
*          This is used to get contents of CCI area.
*
* Arguments:
*       none
* Return Values:
*       USHORT : version
*******************************************************************************
*/

BYTE                    UcsiGetCci(BYTE *TempData, BYTE max_length) {
BYTE connector_change;
    switch(Ucsi_control[0]) {
        case UCSI_COMMAND_PPM_RESET:                // 0x01
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = 0x08;
            Ucsi_cci[0]        = 0;
            Ucsi_cci[1]        = 0;
            Ucsi_cci[2]        = 0;
            Ucsi_cci[3]        = UCSI_CCI_RESET_COMPLETE_INDICATOR;
            break;
        case UCSI_COMMAND_CANCEL:                   // 0x02
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = 0x84;
            break;
        case UCSI_COMMAND_CONNECTOR_RESET:          // 0x03
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_ACK_CC_CI:                // 0x04
            Ucsi_cci[0] = 0x00;
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[2] = 0x00;
            Ucsi_cci[3] = 0x20;
            break ;
        case UCSI_COMMAND_SET_NOTIFICATION_ENABLE:  // 0x05
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xC0)|0x80);
            break;
        case UCSI_COMMAND_GET_CAPABILITY:           // 0x06
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:0x10;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_CONNECTOR_CAPABILITY: // 0x07
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:0x02;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_SET_UOM:                  // 0x08
                                                                // Note: NOT found in spec.
            break;
        case UCSI_COMMAND_SET_UOR:                  // 0x09
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_SET_PDM:                  // 0x0A
                                                                // Note: NOT found in spec.
            break;
        case UCSI_COMMAND_SET_PDR:                  // 0x0B
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_ALTERNATE_MODES:      // 0x0C
           //Ucsi_cci[1] = 0x00;
            //Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:Ucsi_cci[1];  // Note: If successful, set to the number of bytes returned in GET_ALTERNATE_MODES Data up to MAX_DATA_LENGTH. Else set to 0x00.
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_CAM_SUPPORTED:        // 0x0D
            Ucsi_cci[1] = 0x00;
            //Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:Ucsi_cci[1];  // Note: If successful set to ((Number of Alternate Modes Mod 8) + 1) else set to 0x00.
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break ;
        case UCSI_COMMAND_GET_CURRENT_CAM:          // 0x0E
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:0x01;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break ;
        case UCSI_COMMAND_SET_NEW_CAM:              // 0x0F
            Ucsi_cci[1] = 0x00;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break ;
        case UCSI_COMMAND_GET_PDOS:                 // 0x10
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:Ucsi_cci[1];  // Note: If successful set to four times the number of PDOs returned up to a maximum of MAX_DATA_LENGTH else set to 0x00.
            if((Ucsi_message_in[0]|Ucsi_message_in[1]|Ucsi_message_in[2]|Ucsi_message_in[3])==0x00) {
                Ucsi_cci[1] = 0x00;
            }
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_CABLE_PROPERTY:       // 0x11
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:0x05;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_CONNECTOR_STATUS:     // 0x12
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x09:0x09;
            Ucsi_cci[3] = ((Ucsi_cci[3]&0xD0)|0x80);
            break;
        case UCSI_COMMAND_GET_ERROR_STATUS:         // 0x13
            Ucsi_cci[1] = (Ucsi_cci[3]&0x40)?0x00:Ucsi_cci[1];  // Note: If successful set to GET_ERROR_STATUS_DATA_LENGTH else set to 0x00.
            Ucsi_cci[3] = ((Ucsi_cci[3]&0x90)|0x80);
            break;
         default:
            break;
    }
    if(Ucsi_cci[3]&0x10) {
        // BUSY.
        TempData[0] = 0x00;
        TempData[1] = 0x00;
        TempData[2] = 0x00;
        TempData[3] = 0x10;
    }
    else
    {
         if((Ucsi_control[0] == UCSI_COMMAND_PPM_RESET) || (Ucsi_control[0] == UCSI_COMMAND_ACK_CC_CI))
        {
            TempData[0] = Ucsi_cci[0] & 0xFE;   // The bit0 is reserved and shall be set to zero.
        }
        else
        {
            if(UCSI_CONNECTOR_NUMBER_CHANGED[0] == 1)
            {
                connector_change = 1;
            }
            else if(UCSI_CONNECTOR_NUMBER_CHANGED[1] == 1)
            {
                connector_change = 2;
            }
            else
            {
                connector_change = 0;
            }

            TempData[0] = (Ucsi_cci[0] & ~UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK) |
        ((connector_change << UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT) &
         UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK);
        }

        //TempData[0] = Ucsi_cci[0] & 0xFE;   // The bit0 is reserved and shall be set to zero.
        TempData[1] = Ucsi_cci[1];
        TempData[2] = Ucsi_cci[2] & 0x00;   // The bit16 to bit 23 are reserved and shall be set to zero.
        TempData[3] = Ucsi_cci[3] & 0xFE;   // The bit24 is reserved and shall be set to zero.
    //RamDebug(0xEE);
    //RamDebug(TempData[0]);

    }
    Ucsi_control[0] = 0;
    return (4);
}
/*
BYTE  UcsiGetCci(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
  //Ucsi_cci[0] = (Ucsi_cci[0] & ~UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK) |
  //    ((Ucsi_Connector_Number_Changed[UCSI_COMMAND_CONNECTOR_NUMBER] << UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT) &
  //     UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK);

    for (; (max_length > rc) && (CCI_Size > rc); rc++) {
        TempData[rc] = Ucsi_cci[rc];
    }
    return rc;
}
*/
/*
*******************************************************************************
* Function name: UcsiSetControl
*
* Descriptoin:
*          This is used to process data written into CONTROL area.
*
* Arguments:
*       UCHAR *data
*       int   length
* Return Values:
*       none
*******************************************************************************
*/

void UcsiSetControl(BYTE *TempData, BYTE length) {
    BOOL	command_complete = TRUE;
    WORD	connector;
    WORD	value;
    BYTE	i, ope_mode, charging_status,device_connected;
		BYTE TempAddr = TypeC_01_Addr;
        BYTE TempTypeData = TPS65988_DATA1;
        BYTE TempCmd = TPS65988_CMD1;

    if (0 < length) {
        /*
        *   move advertised PDOs to Message-In area
        */
        for (i = 0; (length > i) && (CCI_Control_Size  > i); i++) {
            Ucsi_control[i] = TempData[i];
        }

//        Ucsi_cci[0] = ((Ucsi_Connector_Number_Changed[0] << UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_SHIFT) &
 //                         UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK);
        Ucsi_cci[1] = 0;
        Ucsi_cci[3] = 0;
        switch (Ucsi_control[0]) {
            case UCSI_COMMAND_PPM_RESET:
                 /*
                 *     PPM RESET command
                 */
#if DEBUG_UCSI
RamDebug(0xD0);
#endif
//                 UcsiInit();

                 Ucsi_Flag.busy       = FALSE;
                 Ucsi_Request.request = UCSI_REQUEST_NONE;
//                 CCI_SetConnectorChangeIndicator(0x00);
                 Ucsi_cci[0]        = 0;
                 Ucsi_cci[1]        = 0;
                 Ucsi_cci[2]        = 0;
                 Ucsi_cci[3]        = UCSI_CCI_RESET_COMPLETE_INDICATOR;
//                Ucsi_cci[3] = 0x08;
                 command_complete     = FALSE;
                 UCSI_CONNECTOR_NUMBER_CHANGED[0] = 0;
                 UCSI_CONNECTOR_NUMBER_CHANGED[1] = 0;
                 Ucsi_Error_Set = 0;
                 UCSI_4CC_CMD_COMPLETE = FALSE;
                 UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                 UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                 //Ucsi_Event_Changed = 0;
                 break;
            case UCSI_COMMAND_CANCEL:
                 /*
                 *     CANCEL command
                 */
#if DEBUG_UCSI
RamDebug(0xD1);
#endif
                 Ucsi_Flag.busy       = FALSE;
                 Ucsi_Request.request = UCSI_REQUEST_NONE;
                 Ucsi_cci[3]        |= UCSI_CCI_CANCEL_COMPLETE_INDICATOR;
                 break;
            case UCSI_COMMAND_CONNECTOR_RESET:
                 /*
                 *     Connector reset command
                 */
#if DEBUG_UCSI
RamDebug(0xD2);
#endif
                 Ucsi_cci[0] &= ~UCSI_CCI_CONNECTOR_CHANGED_INDICATOR_MASK;
                 connector = UCSI_CONNECTOR_RESET_COMMAND_CONNECTOR_NUMBER;
#if DEBUG_UCSI
RamDebug(connector);
#endif
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector) {
                     if (((TRUE == BattExistGet) && (0 < RelativeStateOfChgL))
                         ||  ((TRUE == SecBattExistGet) && (0 < SecRelativeStateOfChgL))
                         )//|| (Read_AC_IN() && Read_DCIN_ATTACHED2()))
                         {
                         /*
                         *     not in DeadBattery State
                         */
#if DEBUG_UCSI
RamDebug(0xA0);
#endif
                         if (1 == UCSI_CONNECTOR_RESET_COMMAND_HARD_RESET) {
                         // unsupported hard reset
                             Ucsi_Request.request = UCSI_REQUEST_HARD_RESET;
                         } else {
                             Ucsi_Request.request = UCSI_REQUEST_SOFT_RESET;
                         }
#if DEBUG_UCSI
RamDebug(Ucsi_Request.request);
#endif
                         Ucsi_Request.connector = connector;
                         Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                         Ucsi_Flag.busy         = FALSE;
                         Ucsi_Flag.cancel       = FALSE;
                         command_complete       = FALSE;
                         //TskActivateTask(TASK_UCSI);
                     } else {
                         /*
                         *     in DeadBattery State
                         */
#if DEBUG_UCSI
RamDebug(0xA1);
#endif
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_COMMAND_UNSUCCESSFUL_DUE_TO_DEAD_BATTERY;
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_ACK_CC_CI:  // 0x04
                 /*
                 *     ACK CC CI command
                 */
#if 0
RamDebug(0xD3);
#endif
                 if (1 == UCSI_ACK_CC_CI_COMMAND_CONNECTOR_CHANGE_ACKNOWLEDGE) {
                     for (i = 0; NUMBER_OF_USB_PDC > i; i++ ) {
                         UCSI_CONNECTOR_CHANGE_STATUS[i] = 0;
                         UCSI_CONNECTOR_NUMBER_CHANGED[i] = 0;
                     }
#if DEBUG_UCSI
RamDebug(0xEC);
#endif
                 }
                 if (1 == UCSI_ACK_CC_CI_COMMAND_COMMAND_COMPLETED_ACKNOWLEDGE) {
                     //Ucsi_cci[3] &= ~UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
                 //}
                //CCI_SetConnectorChangeIndicator(0x00);
                 Ucsi_control[0] = 0;
                 Ucsi_cci[1]    = 0;
                 Ucsi_cci[3]    = UCSI_CCI_ACKNOWLEDGE_INDICATOR;
                 if(Ucsi_Error_Set)
                    Ucsi_Error_Set = 0;
                    }
                 //if(Ucsi_Event_Changed)
                 //   Fsm = UCSI_WAIT_EVENT;
                 command_complete = TRUE;
                 break;
            case UCSI_COMMAND_SET_NOTIFICATION_ENABLE: // 0x05
                 /*
                 *     SET NOTIFICATION ENABLE command
                 */
#if DEBUG_UCSI
RamDebug(0xD4);
// UCSI_SET_NOTIFICATION_ENABLE_COMMAND_NOTIFICATION_ENABLE         ((WORD)(Ucsi_control[3]<<8) + Ucsi_control[2])	//Control bit16~31
//RamDebug(Ucsi_control[3]);
//RamDebug(Ucsi_control[2]);
#endif

                UCSI_NOTIFICATION_ENABLE = (Ucsi_control[2] + (Ucsi_control[3] << 8));

                 //UCSI_NOTIFICATION_ENABLE = UCSI_SET_NOTIFICATION_ENABLE_COMMAND_NOTIFICATION_ENABLE;
                 #if !_SUPPORT_ALT_MODE_DETAILS_
            	UCSI_NOTIFICATION_ENABLE &= ~UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM;
            	#endif // _SUPPORT_ALT_MODE_DETAILS_

            	#if !_SUPPORT_PDO_DETAIL_
            	UCSI_NOTIFICATION_ENABLE &= ~(UCSI_CONNECTOR_STATUS_CHANGE_NEGOTIATED_POWER_LEVEL | UCSI_CONNECTOR_STATUS_CHANGE_SUPPORTED_PROVIDER_CAPABILITIES);
            	#endif // _SUPPORT_PDO_DETAIL_

            	#if !_SUPPORT_EXT_SUPPLY_NOTIFICATION_
            	UCSI_NOTIFICATION_ENABLE &= ~UCSI_CONNECTOR_STATUS_CHANGE_EXTERNAL_SUPPLY;
            	#endif // _SUPPORT_EXT_SUPPLY_NOTIFICATION_

            	#if !_SUPPORT_PD_RESET_NOTIFICATION_
            	UCSI_NOTIFICATION_ENABLE &= ~UCSI_CONNECTOR_STATUS_CHANGE_PD_RESET_COMPLETE;
            	#endif // _SUPPORT_PD_RESET_NOTIFICATION_
                //if(UCSI_NOTIFICATION_ENABLE)
                //    Ucsi_Event_Changed = 1;
                 break;
            case UCSI_COMMAND_GET_CAPABILITY:   // 0x06
                 /*
                 *     GET CAPABILITY command
                 */
#if DEBUG_UCSI
RamDebug(0xD5);
#endif
                 Ucsi_message_in[0] = 0;
                 Ucsi_message_in[0] |= (UCSI_BM_ATTRIBUTES_USB_POWER_DELIVERY | UCSI_BM_ATTRIBUTES_USB_TYPE_C_CURRENT);
                 Ucsi_message_in[1] = 0;
                 Ucsi_message_in[1] |= UCSI_BM_ATTRIBUTES_POWER_SOURCE_USES_VBUS;
                 Ucsi_message_in[2] = 0;
                 Ucsi_message_in[3] = 0;

                 Ucsi_message_in[4] &= 0x80;
                 Ucsi_message_in[4] |= NUMBER_OF_USB_PDC;

                 Ucsi_message_in[5] = (0x00
                                   #if _SUPPORT_SET_UOM_
                                   | UCSI_BM_OPTIONAL_FEATURES_SET_UOM_SUPPORTED
                                   #endif // _SUPPORT_SET_UOM_
                                   #if _SUPPORT_SET_PDM_
                                   | UCSI_BM_OPTIONAL_FEATURES_SET_PDM_SUPPORTED
                                   #endif // _SUPPORT_SET_PDM_
                                   #if _SUPPORT_ALT_MODE_DETAILS_
                                   | UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_DETAILS_SUPPORTED
                                   #endif // _SUPPORT_ALT_MODE_DETAILS_
                                   #if _SUPPORT_ALT_MODE_OVERRIDE_
                                   | UCSI_BM_OPTIONAL_FEATURES_ALTERNATE_MODE_OVERRIDE_SUPPORTED
                                   #endif // _SUPPORT_ALT_MODE_OVERRIDE_
                                   #if _SUPPORT_PDO_DETAIL_
                                   | UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED
                                   #endif // _SUPPORT_PDO_DETAIL_
                                   #if _SUPPORT_CABLE_DETAILS_
                                   | UCSI_BM_OPTIONAL_FEATURES_CABLE_DETAILS_SUPPORTED
                                   #endif // _SUPPORT_CABLE_DETAILS_
                                   #if _SUPPORT_EXT_SUPPLY_NOTIFICATION_
                                   | UCSI_BM_OPTIONAL_FEATURES_EXTERNAL_SUPPLY_NOTIFICATION_SUPPORTED
                                   #endif // _SUPPORT_EXT_SUPPLY_NOTIFICATION_
                                   #if _SUPPORT_PD_RESET_NOTIFICATION_
                                   | UCSI_BM_OPTIONAL_FEATURES_PD_RESET_NOTIFICATION_SUPPORTED
                                   #endif // _SUPPORT_RESET_NOTIFICATION_
                                   );
                 UCSI_OPTIONAL_FEATURE = Ucsi_message_in[5];
                 Ucsi_message_in[6] = 0;
                 Ucsi_message_in[7] = 0;
                 Ucsi_message_in[8] = NUMBER_ALT_MODES;
                 //BfmSetBitField(0,UCSI_GET_CAPABILITY_DATA_B_NUM_ALT_MODES,Ucsi_message_in);

                 Ucsi_message_in[10] = 0x20;
                 Ucsi_message_in[11] = 0x01;

                 Ucsi_message_in[12] = 0x00;
                 Ucsi_message_in[13] = 0x02;

                 Ucsi_message_in[14] = 0x10;
                 Ucsi_message_in[15] = 0x01;

                 Ucsi_cci[1] = 0x10;
                 break;
            case UCSI_COMMAND_GET_CONNECTOR_CAPABILITY:
                 /*
                 *     GET COPNNECTOR CAPABILITY command
                 */
#if DEBUG_UCSI
RamDebug(0xD6);
#endif
                 connector = UCSI_GET_CONNECTOR_CAPABILITY_COMMAND_CONNECTOR_NUMBER;
									TempAddr = GetPdPortAddress(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector){
                    bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_SYSTEM_CONFIG, SMB3_DATA_TEMP,bRead_I2C_NULL);
                    Ucsi_message_in[0] = 0;
                    Ucsi_message_in[1] = 0;
                  //if(((SMB3_DATA_TEMP[0] & 0x07) == 0x03) || (SMB3_DATA_TEMP[0] & 0x07) == 0x05)
                  //    Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_DRP;
                    //if((SMB3_DATA_TEMP[0] & 0x03) == 0x02)
                   // Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_DRP;   //A285D00038Yanis Modify for UCSI WHLK Test fail 180411
                   // else if(((SMB3_DATA_TEMP[0] & 0x07) == 0x04) || (SMB3_DATA_TEMP[0] & 0x07) == 0x06)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_DFP_ONLY;
                  //  else if(((SMB3_DATA_TEMP[0] & 0x07) == 0x00) || (SMB3_DATA_TEMP[0] & 0x07) == 0x01)
                     //   Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_UFP_ONLY;  //A285D00038Yanis Modify for UCSI WHLK Test fail 180411
                    if((SMB3_DATA_TEMP[0] & BIT6) == BIT6)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_AUDIO_ACCESSORY;
                    if((SMB3_DATA_TEMP[0] & BIT7) == BIT7)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_DEBUG_ACCESSORY;
                   // if((SMB3_DATA_TEMP[1] & BIT5) == BIT5)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_USB_2;

                  //if(((SMB3_DATA_TEMP[4] & 0x18) == 0x08) || ((SMB3_DATA_TEMP[4] & 0x18) == 0x10))
                  //    Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_USB_3;
                    if(((SMB3_DATA_TEMP[1] & 0x60) == 0x40) || ((SMB3_DATA_TEMP[1] & 0x60) == 0x20))
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_USB_3;
                    /*if(((SMB3_DATA_TEMP[0] & 0x07) >= 0x02) && ((SMB3_DATA_TEMP[0] & 0x07) <= 0x06))
                        Ucsi_message_in[1] |= 0x01;
                    if(((SMB3_DATA_TEMP[0] & 0x07) >= 0x00) && ((SMB3_DATA_TEMP[0] & 0x07) <= 0x05))
                        Ucsi_message_in[1] |= 0x02;*/
                        Ucsi_message_in[1] |= 0x33;
                    bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_USER_VID_CONFIGURATION, SMB3_DATA_TEMP,bRead_I2C_NULL);
                    if((SMB3_DATA_TEMP[0] & BIT0) == BIT0)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE;
                    bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_DP_SID_CONFIGURATION, SMB3_DATA_TEMP,bRead_I2C_NULL);
                    if((SMB3_DATA_TEMP[0] & BIT0) == BIT0)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE;
                    bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_INTEL_VID_CONFIGURATION, SMB3_DATA_TEMP,bRead_I2C_NULL);
                    if((SMB3_DATA_TEMP[0] & BIT0) == BIT0)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE;
                    bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TI_VID_CONFIGURATION, SMB3_DATA_TEMP,bRead_I2C_NULL);
                    if((SMB3_DATA_TEMP[0] & BIT0) == BIT0)
                        Ucsi_message_in[0] |= UCSI_CONNECTOR_SUPPORTS_ALTERNATE_MODE;
#if DEBUG_UCSI
RamDebug(Ucsi_message_in[0]);
RamDebug(Ucsi_message_in[1]);
#endif

                     Ucsi_cci[1] = 2;
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_SET_UOM:
            #if _SUPPORT_SET_UOM_
                connector = UCSI_SET_UOM_COMMAND_CONNECTOR_NUMBER;
                device_connected = GetPdDeviceConnect(connector);
                if (0 <= connector){
                    if (FALSE == device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR; //printf ("("[ERROR]2\n");
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                        value = UCSI_SET_UOM_COMMAND_USB_OPERATION_MODE;
                         switch (value) {
                             case 1:

                                    // Connector operate as a DFP

                                  Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_DFP;
                                  Ucsi_Request.connector = connector;
                                  Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                  Ucsi_Flag.busy         = FALSE;
                                  Ucsi_Flag.cancel       = FALSE;
                                  command_complete       = FALSE;
                                  //TskActivateTask(TASK_UCSI);
                                  break;
                             case 2:
                               //    Connector operate as a UFP

                                 Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_UFP;
                                 Ucsi_Request.connector = connector;
                                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                 Ucsi_Flag.busy         = FALSE;
                                 Ucsi_Flag.cancel       = FALSE;
                                 command_complete       = FALSE;
                                 break;
                             case 4:
                                 //    Accept operate as a DRP

                                 break;
                             default:
                                 Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                                 Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     }
                }
                     }else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                #else
                Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                #endif
                break;
            case UCSI_COMMAND_SET_PDM:
                #if _SUPPORT_SET_PDM_
                connector = UCSI_SET_PDM_COMMAND_CONNECTOR_NUMBER;
                 device_connected = GetPdDeviceConnect(connector);
                 if (0 <= connector){
                     if (FALSE == device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR; //printf ("("[ERROR]3\n");
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                         value = UCSI_SET_PDM_COMMAND_POWER_DIRECTION_MODE;
                         switch (value) {
                         case 1:
                             /*
                             *   Connector operate as a Source
                             */
                             Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_SOURCE;
                             Ucsi_Request.connector = connector;
                             Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                             Ucsi_Flag.busy         = FALSE;
                             Ucsi_Flag.cancel       = FALSE;
                             command_complete       = FALSE;
                             //TskActivateTask(TASK_UCSI);
                             break;
                        case 2:
                            /*
                            *    Connector operate as a Sink
                            */
                            Ucsi_Request.request = UCSI_REQUEST_OPERATE_AS_SINK;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;
                            break;
                        case 4:
                            /*
                            *    Connector operate as a provider/consumer if it is a DFP
                                or as a consumer/provider if it is a UFP.
                            */
                            break;
                        default:
                            Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                            Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;

                         }
                         Ucsi_Error_Status =  UCSI_ERROR_STATUS_INCOMPATIBLE_CONNECTOR_PARTNER;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                #endif
                 break;
            case UCSI_COMMAND_GET_ALTERNATE_MODES:
                 /*
                 *     unsupported commands, "SET UOM", "SET PDM" and "GET ALTERNATE MODES"
                 */
#if 0
RamDebug(0xD7);
#endif
                 #if _SUPPORT_ALT_MODE_DETAILS_
                 connector = UCSI_GET_ALTERNATE_MODES_COMMAND_CONNECTOR_NUMBER;
                 if (0 <= connector){
                    Ucsi_Request.request = UCSI_REQUEST_GET_ALTERNATE_MODES;
                    Ucsi_Request.connector = connector;
                    Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                    Ucsi_Flag.busy         = FALSE;
                    Ucsi_Flag.cancel       = FALSE;
                    command_complete       = FALSE;
                     } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 #else
                 //Ucsi_cci[1] = 0x10;
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_SET_UOR:
                 /*
                 *     "SET UOR" (USB Operation Role)
                 */
#if DEBUG_UCSI_CMD
RamDebug(0xD8);
#endif
                 connector = UCSI_SET_UOR_COMMAND_CONNECTOR_NUMBER;
									TempAddr = GetPdPortAddress(connector);
                 device_connected = GetPdDeviceConnect(connector);
                 if (0 <= connector){
//                     if (FALSE == Usb_Pdc_Status.device_connected) {


                     if (FALSE == device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR; //printf ("("[ERROR]4\n");
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                         value = UCSI_SET_UOR_COMMAND_USB_OPERATION_ROLE;

				    SMB3_DATA[0] =0xEF;
    				SMB3_DATA[1] = 0x17;
    				SMB3_DATA[2] = 0x01;
                    if(TempAddr == TypeC_01_Addr)
                    {
                        TempTypeData = TPS65988_DATA1;
                        TempCmd = TPS65988_CMD1;
                    }
                    else
                    {
                        TempTypeData = TPS65988_DATA2;
                        TempCmd = TPS65988_CMD2;
                    }

    		 if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TempTypeData, &SMB3_DATA[0], 3, SMBus_NoPEC))
              {
         			 SMB3_DATA[0] = 'A';
    				SMB3_DATA[1] = 'M';
    				SMB3_DATA[2] = 'E';
    				SMB3_DATA[3] = 'x';
              bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TempCmd, &SMB3_DATA[0], 4, SMBus_NoPEC);

              }
                         switch (value) {
                             case 1:
                                  /*
                                  *    Swap to DFP
                                  */
                                  Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_DFP;
                                  Ucsi_Request.connector = connector;
                                  Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                  Ucsi_Flag.busy         = FALSE;
                                  Ucsi_Flag.cancel       = FALSE;
                                  command_complete       = FALSE;
                                  //UsbPdcGetDataRole(0);
                                  //TskActivateTask(TASK_UCSI);
                                  break;
                             case 2:
                                 /*
                                 *    Swap to UFP
                                 */
                                 Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_UFP;
                                 Ucsi_Request.connector = connector;
                                 Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                                 Ucsi_Flag.busy         = FALSE;
                                 Ucsi_Flag.cancel       = FALSE;
                                 command_complete       = FALSE;
                                 //UsbPdcGetDataRole(0);
                                 break;
                             case 4:
                                 /*
                                 *    Accept role swap
                                 */
                                 break;
                             default:
                                 Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                                 Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                         }
                        }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_SET_PDR:
                 /*
                 *     "SET PDR" (Power Direction Role)
                 */
#if DEBUG_UCSI_CMD
RamDebug(0xD9);
#endif
                 connector = UCSI_SET_PDR_COMMAND_CONNECTOR_NUMBER;
									TempAddr = GetPdPortAddress(connector);
                 device_connected = GetPdDeviceConnect(connector);
                 if (0 <= connector){
                     if (FALSE == device_connected) {
                         Ucsi_Error_Status = UCSI_ERROR_STATUS_CC_COMMUNICATION_ERROR; //printf ("("[ERROR]5\n");
                         Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                     } else {
                         value = UCSI_SET_PDR_COMMAND_POWER_DIRECTION_ROLE;
#if DEBUG_UCSI_CMD
RamDebug(value);
#endif
                         switch (value) {
                         case 1:
                             /*
                             *    Swap to Source
                             */
                             Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_SOURCE;
                             Ucsi_Request.connector = connector;
                             Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                             Ucsi_Flag.busy         = FALSE;
                             Ucsi_Flag.cancel       = FALSE;
                             command_complete       = FALSE;

                             //TskActivateTask(TASK_UCSI);
                             break;
                        case 2:
                            /*
                            *    Swap to Sink
                            */
                            Ucsi_Request.request = UCSI_REQUEST_SWAP_TO_SINK;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;

                            break;
                        case 4:
                            /*
                            *    Accept power role swap
                            */
                            break;
                        default:
                            Ucsi_Error_Status = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
                            Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;

                         }
                         //Ucsi_Error_Status =  UCSI_ERROR_STATUS_INCOMPATIBLE_CONNECTOR_PARTNER;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            case UCSI_COMMAND_GET_CAM_SUPPORTED:
                 /*
                 *     GET CAM SUPPORT command
                 */
#if 1
RamDebug(0xDA);
#endif
                 #if _SUPPORT_ALT_MODE_DETAILS_
                    connector = UCSI_GET_CAM_SUPPORT_COMMAND_CONNECTOR_NUMBER;
                    if (0 <= connector){
                       Ucsi_message_in[0] = 0x03;
                       //Ucsi_message_in[1] = 0x00;
                       Ucsi_cci[1] = 0x01;
                    } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                    }
                 #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 //Ucsi_cci[1] = 0x01;
                 #endif
                 break;
            case UCSI_COMMAND_GET_CURRENT_CAM:
                 #if _SUPPORT_ALT_MODE_DETAILS_
                    connector = UCSI_GET_CURRENT_CAM_COMMAND_CONNECTOR_NUMBER;
                    if (0 <= connector){
                   RamDebug(0xDB);
                        Ucsi_Request.request = UCSI_REQUEST_GET_CURRENT_CAM;
                            Ucsi_Request.connector = connector;
                            Ucsi_cci[3]          |= UCSI_CCI_BUSY_INDICATOR;
                            Ucsi_Flag.busy         = FALSE;
                            Ucsi_Flag.cancel       = FALSE;
                            command_complete       = FALSE;
                    } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                    }
                 #else
                 //Ucsi_cci[1] = 0x01;
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_SET_NEW_CAM:
                 #if _SUPPORT_ALT_MODE_OVERRIDE_
                 #else
                 Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_GET_CABLE_PROPERTY:
                 /*
                 *     unsupported commands, "GET CURRENTCAM", "SET NEW CAM" and "GET GET CABLE PROPERTY"
                 */
#if DEBUG_UCSI
RamDebug(0xDB);
#endif
                 #if _SUPPORT_CABLE_DETAILS_
                 #else
                 //Ucsi_cci[1] = 0x05;
                Ucsi_cci[3] = UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                 #endif
                 break;
            case UCSI_COMMAND_GET_PDOS:
                 /*
                 *     GET PDOs command
                 */
#if 1
RamDebug(0xDC);
#endif
                 Ucsi_Flag.busy    = FALSE;
                 Ucsi_Flag.cancel  = FALSE;
                 RamDebug((Ucsi_control[2] & 0x7F));
                 connector = UCSI_GET_PDOS_COMMAND_CONNECTOR_NUMBER;
                 RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if (0 <= connector) {
                     if (0 != (UCSI_OPTIONAL_FEATURE & UCSI_BM_OPTIONAL_FEATURES_PDO_DETAILS_SUPPORTED)) {
	                     if (1 == UCSI_GET_PDOS_COMMAND_PARTNER_PDOS) {
	                         /*
	                         *     Get Source/Sink PDO of a partner
	                         */
	                         if (1 == UCSI_GET_PDOS_COMMAND_SOURCE_OR_SINK_PDOS) {
	                             Ucsi_Request.request = UCSI_REQUEST_GET_RX_SOURCE_PDOS;
	                         } else {
	                             Ucsi_Request.request = UCSI_REQUEST_GET_RX_SINK_PDOS;
	                         }
	                     } else {
	                         /*
	                         *     Get Source/Sink PDO of this
	                         */
	                         if (1 == UCSI_GET_PDOS_COMMAND_SOURCE_OR_SINK_PDOS) {
	                             value = UCSI_GET_PDOS_COMMAND_SOURCE_CAPABILITIES_TYPE;
	                             switch (value) {
	                                 case UCSI_SOURCE_CAPABILITES_TYPE_CURRENT_SUPPORTED:
	                                      Ucsi_Request.request = UCSI_REQUEST_GET_TX_SOURCE_PDOS_CURRENT_SUPPORTED;
	                                      break;
	                                 case UCSI_SOURCE_CAPABILITES_TYPE_ADVERTISED:
	                                      Ucsi_Request.request = UCSI_REQUEST_GET_TX_SOURCE_PDOS_ADVERTIZED;
	                                      break;
	                                 case UCSI_SOURCE_CAPABILITES_TYPE_MAXIMUM_SUPPORTED:
	                                      Ucsi_Request.request = UCSI_REQUEST_GET_TX_SOURCE_PDOS_MAXIMUM_SUPPORTED;
	                                      break;
	                                 default:
	                                      Ucsi_Error_Status    = UCSI_ERROR_STATUS_INVALID_SPECIFIC_PARAMETERS;
	                                      Ucsi_cci[3]        |= UCSI_CCI_ERROR_INDICATOR;
	                                      Ucsi_Request.request = UCSI_REQUEST_NONE;
	                             }
	                         } else {
	                             Ucsi_Request.request = UCSI_REQUEST_GET_TX_SINK_PDOS;
	                         }
	                     }

	                     if (UCSI_REQUEST_NONE != Ucsi_Request.request) {
	                         Ucsi_Request.connector     = connector;
	                         Ucsi_Request.offset        = UCSI_GET_PDOS_COMMAND_PDO_OFFSET;
	                         Ucsi_Request.number_of_pdo = UCSI_GET_PDOS_COMMAND_NUMBER_OF_PDOS;
                             Ucsi_cci[1]                = 0x04;
	                         //Ucsi_cci[3]              |= UCSI_CCI_BUSY_INDICATOR;
	                         command_complete           = FALSE;
	                         //TskActivateTask(TASK_UCSI);
#if DEBUG_UCSI
RamDebug(Ucsi_Request.offset);
RamDebug(Ucsi_Request.number_of_pdo);
#endif
	                     }
                     } else {
                          Ucsi_cci[3] |= UCSI_CCI_NOT_SUPPORTED_INDICATOR;
                     }
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                     Ucsi_cci[3]     |= UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            /*
            *************************************************************
            *     GET Connector Status
            *************************************************************
            */
            case UCSI_COMMAND_GET_CONNECTOR_STATUS:
#if 1
RamDebug(0xDD);
#endif
                 connector = UCSI_GET_CONNECTOR_STATUS_COMMAND_CONNECTOR_NUMBER;
                 RamDebug(connector);
                 //connector = SysConvertUsbConnectorIdToUsbPdcId(connector);
                 if ((0 <= connector) && (connector < NUMBER_OF_USB_PDC)) {
#if DEBUG_UCSI
RamDebug(UCSI_NOTIFICATION_ENABLE);
RamDebug((UCSI_NOTIFICATION_ENABLE >> 8));
RamDebug(UCSI_CONNECTOR_CHANGE_STATUS[connector]);
RamDebug((UCSI_CONNECTOR_CHANGE_STATUS[connector] >> 8));
#endif
                         if((UCSI_CONNECTOR_CHANGE_STATUS[connector] & UCSI_CONNECTOR_STATUS_CHANGE_ERROR) == UCSI_CONNECTOR_STATUS_CHANGE_ERROR)
                            Ucsi_cci[3]     = UCSI_CCI_ERROR_INDICATOR; ;
                         Ucsi_message_in[0] = UCSI_CONNECTOR_CHANGE_STATUS[connector] & 0xFF;
		                 Ucsi_message_in[1] = UCSI_CONNECTOR_CHANGE_STATUS[connector] >> 8;
#if 1
RamDebug(Ucsi_message_in[0]);
RamDebug(Ucsi_message_in[1]);
#endif

                     /*
                     *    Operation Mode
                     */


                     if(connector == 0)
                     {
                         if (TRUE == Usb_Pdc_Power_Status.power_connection) {
                             if (USB_BC12_NONE != Usb_Pdc_Power_Status.bc12_status) {
                                 /*
                                 *    BC 1.2 (SDP/CDP/DCP) operation
                                 */
                                 ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_BC;
                             } else {
                                 /*
                                 *    Type-C current
                                 */
                                 RamDebug(Usb_Pdc_Power_Status.type_c_current);
                                 switch (Usb_Pdc_Power_Status.type_c_current) {
                                     case USB_TYPE_C_CURRENT_DEFAULT:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_USB_DEFAULT;
                                          break;
                                     case USB_TYPE_C_CURRENT_1_5_A:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_1_5_A;
                                          break;
                                     case USB_TYPE_C_CURRENT_3_0_A:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_3_0_A;
                                          break;
                                     case USB_TYPE_C_CURRENT_PD_CONTRACT:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_PD;
                                          break;
                                     default:
                                         ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                                 }
                             }

                         }
                         else {
                             ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                         }
                     }
                     else
                     {
                        if (TRUE == Usb_Pdc_Power_Status2.power_connection) {
                             if (USB_BC12_NONE != Usb_Pdc_Power_Status2.bc12_status) {
                                 /*
                                 *    BC 1.2 (SDP/CDP/DCP) operation
                                 */
                                 ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_BC;
                             } else {
                                 /*
                                 *    Type-C current
                                 */
                                 RamDebug(Usb_Pdc_Power_Status2.type_c_current);
                                 switch (Usb_Pdc_Power_Status2.type_c_current) {
                                     case USB_TYPE_C_CURRENT_DEFAULT:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_USB_DEFAULT;
                                          break;
                                     case USB_TYPE_C_CURRENT_1_5_A:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_1_5_A;
                                          break;
                                     case USB_TYPE_C_CURRENT_3_0_A:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_TYPE_C_3_0_A;
                                          break;
                                     case USB_TYPE_C_CURRENT_PD_CONTRACT:
                                          ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_PD;
                                          break;
                                     default:
                                         ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                                 }
                             }

                         }
                         else {
                             ope_mode = UCSI_GET_CONNECTOR_STATUS_DATA_POWER_OPERATION_MODE_NO_CONSUMER;
                         }
                     }
                     /*   Power direction
                         */
                         if(connector == 0)
                         {
                             if (TRUE == UCSI_POWER_SOURCE)
                             //if(Usb_Pdc_Power_Status.power_source == TRUE)
                             {
                                 /*
                                 *   Operate as a Source
                                 */
                             		 SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                             } else {
                                 /*
                                 *   Operate as a Sink
                                 */
                             		 CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                             }
                         }
                         else
                         {
                            if (TRUE == UCSI_POWER_SOURCE2)
                             //if(Usb_Pdc_Power_Status.power_source == TRUE)
                             {
                                 /*
                                 *   Operate as a Source
                                 */
                             		 SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                             } else {
                                 /*
                                 *   Operate as a Sink
                                 */
                             		 CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_POWER_DIRECTION);
                             }
                         }

                     Ucsi_message_in[2] &= 0xF8;
                     Ucsi_message_in[2] |= ope_mode;

                     if(connector == 0)
                     {
                         if (TRUE == Usb_Pdc_Status.device_connected) {
                             SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                             } else {
                             CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                             //Ucsi_message_in[2] &= 0x1F;
                         }
                     }
                     else
                     {
                        if (TRUE == Usb_Pdc_Status2.device_connected) {
                             SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                             } else {
                             CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                             //Ucsi_message_in[2] &= 0x1F;
                         }
                     }

                         //Ucsi_message_in[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECT >> 8);
                         /*
                         *   Connected partner flags ?
                         */
                         //BfmSetBitField(0, UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS, Ucsi_message_in);
                         Ucsi_message_in[2] &= 0x1F;
                         Ucsi_message_in[3] &= 0x1F;
                         //SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB);
                         if(connector == 0)
                         {
                             if(Usb_Pdc_Status.connect_status == USB_CONNECT_CONNECTED)
                                SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB);
                             else if(Usb_Pdc_Status.connect_status == USB_CONNECT_DP)
                                SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_ALT);
                         }
                         else
                         {
                            if(Usb_Pdc_Status2.connect_status == USB_CONNECT_CONNECTED)
                                SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB);
                             else if(Usb_Pdc_Status2.connect_status == USB_CONNECT_DP)
                                SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_ALT);
                         }
                         /*if(IS_MASK_SET(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS))
                         SET_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB);
                         if(IS_MASK_SET(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_FLAGS_USB))
                            Usb_Pdc_Status.connect_status = USB_CONNECT_CONNECTED;*/
                         //Ucsi_message_in[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER >> 8);
                         /*
                         *   Connected partner type
                         */
#if DEBUG_UCSI
RamDebug(0xBF);
RamDebug(Ucsi_message_in[2]);
RamDebug(Usb_Pdc_Status.connect_status);
#endif
                         /*if(Usb_Pdc_Status.connect_status == USB_CONNECT_CONNECTED) {

                                UCSI_CONNECTOR_STATE = UCSI_CONNECTOR_STATE&0x07;
                                switch (UCSI_CONNECTOR_STATE) {
                                    case TI_CONNECTOR_STATUS_NO_CONNECTION:
                                    case TI_CONNECTOR_STATUS_PORT_DISABLED:
                                        break;
                                    case TI_CONNECTOR_STATUS_AUDIO_CONNECTION:
                                        i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_AUDIO_ACCESSORY;
                                        break;
                                    case TI_CONNECTOR_STATUS_DEBUG_CONNECTION:
                                        i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DEBUG_ACCESSORY;
                                        break;
                                    case TI_CONNECTOR_STATUS_CONNECTION_RD_NO_RA:
                                    case TI_CONNECTOR_STATUS_RESERVE:
                                        if(Usb_Pdc_Status.dfp == FALSE)
                                            i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP;
                                        else
                                            i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP;
                                        break;
                                    case TI_CONNECTOR_STATUS_NO_CONNECTION_RA_NO_RD:
                                    case TI_CONNECTOR_STATUS_CONNECTION_RD_RA:
                                        if(Usb_Pdc_Status.dfp == FALSE)
                                            i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_POWERED_UFP;
                                        else
                                            i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_POWERED_NO_UFP;
                                        break;
                                    }
                            }*/
                            if(connector == 0)
                            {
                                 if (Usb_Pdc_Status.dfp == FALSE) {
                                      i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP;
                                      } else {
                                      i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP;
                                      }
                            }
                            else
                            {
                                if (Usb_Pdc_Status2.dfp == FALSE) {
                                      i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP;
                                      } else {
                                      i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP;
                                      }
                            }

                          /*switch(Usb_Pdc_Status.connect_status) {
                             case USB_CONNECT_CONNECTED:
                                 if (Usb_Pdc_Status.dfp == FALSE) {
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DFP;
                                  } else {
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_UFP;
                                  }
                                  break;
                             case USB_CONNECT_AUDIO_ACCESSORY:
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_AUDIO_ACCESSORY;
                                  break;
                             case USB_CONNECT_DEBUG_ACCESSORY:
                                  i = UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE_DEBUG_ACCESSORY;
                                  break;
                         }*/
                         Ucsi_message_in[3] = ((UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_PARTNER_TYPE | i) << 5);
#if DEBUG_UCSI
RamDebug(0xBB);
RamDebug(Ucsi_message_in[3]);
#endif
                         /*
                         *     RDO ?
                         */
                         //BfmSetBitField(0, UCSI_GET_CONNECTOR_STATUS_DATA_REQUEST_DATA_OBJECT,Ucsi_message_in);
                         if(connector == 0)
                         {
                            Ucsi_message_in[4] = UCSI_RDO1;
                            Ucsi_message_in[5] = UCSI_RDO2;
                            Ucsi_message_in[6] = UCSI_RDO3;
                            Ucsi_message_in[7] = UCSI_RDO4;
                         }
                         else
                         {
                            Ucsi_message_in[4] = UCSI_RDO1_CONNECTOR2;
                            Ucsi_message_in[5] = UCSI_RDO2_CONNECTOR2;
                            Ucsi_message_in[6] = UCSI_RDO3_CONNECTOR2;
                            Ucsi_message_in[7] = UCSI_RDO4_CONNECTOR2;
                         }


                         /*
                         *    Charging status
                         */

                         if (TRUE == Usb_Pdc_Charge_Status[connector].quick_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NOMINAL_CHARGING;
                         } else if (TRUE == Usb_Pdc_Charge_Status[connector].trickle_charge) {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_TRICKLE_CHARGING;
                         } else {
                             charging_status = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS_NO_CHARGING;
                         }
                         RamDebug(0xAA);
                         RamDebug(charging_status);
                         Ucsi_message_in[8] = UCSI_GET_CONNECTOR_STATUS_DATA_BATTERY_CHARGING_STATUS | charging_status;
                         RamDebug(Ucsi_message_in[8]);
                    /* } else {
                         CLEAR_MASK(Ucsi_message_in[2],UCSI_GET_CONNECTOR_STATUS_DATA_CONNECT_STATUS);
                         Ucsi_message_in[2] &= 0x1F;
                     }*/
                     /*
                     *   fill 0 to unused bits
                     */
                     //BfmSetBitField(0, UCSI_GET_CONNECTOR_STATUS_DATA_PROVIDER_CAPABILITY_LIMITED_REASON,Ucsi_message_in);
                     Ucsi_message_in[8] &= 0xC3;
                     //BfmSetBitField(0, UCSI_GET_CONNECTOR_STATUS_DATA_RESERVED, Ucsi_message_in);
                     Ucsi_message_in[8] &= 0x3F;
#if DEBUG_UCSI
RamDebug(Ucsi_message_in[0]);
RamDebug(Ucsi_message_in[1]);
#endif
                     Ucsi_cci[1]     = 9;
                 } else {
                     Ucsi_Error_Status = UCSI_ERROR_STATUS_NON_EXISTTENT_CONNECTOR_NUMBER;
                   //UCSI_CONNECTOR_CHANGE_STATUS[connector-1] = UCSI_CONNECTOR_STATUS_CHANGE_ERROR;
                   //Ucsi_cci[1]     = 9;
                   //UCSI_CONNECTOR_NUMBER_CHANGED[connector-1] = 1;
                     Ucsi_cci[3]     = UCSI_CCI_ERROR_INDICATOR;
                 }
                 break;
            /*
            ********************************************
            *   Get Error Status
            ********************************************
            */
            case UCSI_COMMAND_GET_ERROR_STATUS:
#if DEBUG_UCSI_CMD
RamDebug(0xDE);
#endif
// Isaac check                 Ucsi_cci[1] =   (Ucsi_cci[1] & ~UCSI_CCI_DATA_LENGTH_MASK) | UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH;
                 Ucsi_cci[1] =  UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH;
                 Ucsi_message_in[0] = Ucsi_Error_Status & 0xff;
                 Ucsi_message_in[1] = (Ucsi_Error_Status >> 8) & 0xff;
                 Ucsi_Error_Set = 1;
                 #if DEBUG_UCSI
                 RamDebug(Ucsi_message_in[0]);
                 RamDebug(Ucsi_message_in[1]);
                 #endif
                 for (i = 2; UCSI_PARM_GET_ERROR_STATUS_DATA_LENGTH > i; i++) {
                     Ucsi_message_in[i] = 0;
                     //RamDebug(Ucsi_message_in[i]);
                 }
                 break;
            default:
                 //Ucsi_Error_Status = UCSI_ERROR_STATUS_UNRECOGNIZED_COMMAND;
               //Ucsi_cci[0]     = 0;
                 Ucsi_cci[1]     = 0;
               //Ucsi_cci[2]     = 0;
                 //Ucsi_cci[3]     = UCSI_CCI_ERROR_INDICATOR;
                 Ucsi_cci[3] = (UCSI_CCI_NOT_SUPPORTED_INDICATOR
| UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
        }
    } else {
      //Ucsi_cci[0] = 0;
        Ucsi_cci[1] = 0;
      //Ucsi_cci[2] = 0;
        Ucsi_cci[3] = (UCSI_CCI_NOT_SUPPORTED_INDICATOR
| UCSI_CCI_COMMAND_COMPLETE_INDICATOR);
    }
    if (TRUE == command_complete) {
    		if((Ucsi_cci[3] & UCSI_CCI_ACKNOWLEDGE_INDICATOR) == 0x00)
    		{
            /*
            *   not ACK_CC_CI
            */
        	Ucsi_cci[3] |= UCSI_CCI_COMMAND_COMPLETE_INDICATOR;
#if DEBUG_UCSI
RamDebug(0xF0);
RamDebug(Ucsi_cci[3]);
#endif
        }
        /*
        *    Issue the command complete notification
        */
        if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_COMMAND_COMPLETE_NOTIFICATION_ENABLE)) {
#if DEBUG_UCSI
RamDebug(0xF1);
RamDebug(Ucsi_cci[0]);
RamDebug(Ucsi_cci[3]);
#endif
            ECSMI_SCIEvent(AM_UCSI_EVENT);
        }
    }
}

/*
*******************************************************************************
* Function name: UcsiGetControl
*
* Descriptoin:
*          This is used to get data in CONTROL area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetControl(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    for (; (max_length > rc) && (CCI_Control_Size > rc); rc++) {
        TempData[rc] = Ucsi_control[rc];
    }
    return rc;
}

/*
*******************************************************************************
* Function name: UcsiGetMessageIn
*
* Descriptoin:
*          This is used to get data in MESSAGE-IN area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetMessageIn(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;
    for (; (max_length > rc) && (Message_in_Size> rc); rc++) {

        TempData[rc] = Ucsi_message_in[rc];
    }
    return rc;
}

/*
*******************************************************************************
* Function name: UcsiSetMessageOut
*
* Descriptoin:
*          This is used to set data into MESSAGE-OUT area.
*
* Arguments:
*       UCHAR *data
*       int   length
* Return Values:
*       none
*******************************************************************************
*/

void UcsiSetMessageOut(BYTE *TempData, BYTE length) {
BYTE	i;
    for (i = 0; (length > i) && (Message_out_Size > i); i++) {

        Ucsi_message_out[i] = TempData[i];
    }
}

/*
*******************************************************************************
* Function name: UcsiGetMessageOut
*
* Descriptoin:
*          This is used to get data in MESSAGE-OUT area.
*
* Arguments:
*       UCHAR *data
*       int   max_length
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE UcsiGetMessageOut(BYTE *TempData, BYTE max_length) {
    BYTE rc;

    rc = 0;

    for (; (max_length > rc) && (Message_out_Size > rc); rc++) {
        TempData[rc] = Ucsi_message_out[rc];
    }
    return rc;
}


/*
*******************************************************************************
* Function name: UcsiUsbPdcStatusChanged
*
* Descriptoin:
*          This is invoked when status of USB-C PDC is changed
*
* Arguments:
*       int   state
* Return Values:
*       none
*******************************************************************************
*/

void UcsiUsbPdcStatusChanged(USB_POWER_STATUS power_status[NUMBER_OF_USB_PDC], USB_STATUS status[NUMBER_OF_USB_PDC]) {
    BOOL             attention = FALSE;
    BOOL             changed;
		BYTE	i;
    USB_POWER_STATUS temp_power_status[NUMBER_OF_USB_PDC];
    USB_STATUS temp_status[NUMBER_OF_USB_PDC];

    temp_power_status[0] = Usb_Pdc_Power_Status;
    temp_power_status[1] = Usb_Pdc_Power_Status2;

    temp_status[0] = Usb_Pdc_Status;
    temp_status[1] = Usb_Pdc_Status2;

    for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
        changed = FALSE;

        //power_status = UsbPdcGetPowerStatus(i);
        if ((power_status[i].bc12_status      != temp_power_status[i].bc12_status)    ||
            (power_status[i].type_c_current   != temp_power_status[i].type_c_current) ||
            (power_status[i].power_connection != temp_power_status[i].power_connection)) {
            /*
            *   Operation mode is changed
            */
            UCSI_CONNECTOR_CHANGE_STATUS[i] |= UCSI_CONNECTOR_STATUS_CHANGE_POWER_OPERATION_MODE;
            //Usb_Pdc_Power_Status         = power_status;
            changed                         = TRUE;
            if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_POWER_OPERATION_MODE_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        if (power_status[i].power_source != temp_power_status[i].power_source) {
            /*
            *   power direction (Source/ Sink) is changed
            */
            UCSI_CONNECTOR_CHANGE_STATUS[i] |= UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION; //printf ("("@A3\n");
            //Usb_Pdc_Power_Status.power_source =  power_status[i].power_source;
            changed                         = TRUE;
            if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_POWER_DIRECTION_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        //status[i] = UsbPdcGetStatus(i);
        if (status[i].connect_status != temp_status[i].connect_status) {
            /*
            *   Connected partner is changed
            */
            UCSI_CONNECTOR_CHANGE_STATUS[i] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER;
            //Usb_Pdc_Status               = status;
            changed                         = TRUE;
            if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_CONNECTOR_PARTNER_CHANGE_NOTIFICATION_ENABLE)) {
                attention = TRUE;
            }
        }
        if (TRUE == changed) {
            /*
            *    Connector state is changed
            */
            //UCSI_CONNECTOR_CHANGE_STATUS[i] |= UCSI_CONNECTOR_STATUS_CHANGE_CONNECT;
            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 0;//SysConvertUsbPdcIdToUsbConnectorId(i);
        }
    }
    if (TRUE == attention) {

        ECSMI_SCIEvent(AM_UCSI_EVENT);
    }
}

/*
*******************************************************************************
* Function name: UcsiFuelStateChanged
*
* Descriptoin:
*          This is invoked when status of battery charging is changed
*
* Arguments:
*       int   state
* Return Values:
*       none
*******************************************************************************
*/

void UcsiFuelStateChanged(BYTE state) {
    BOOL                  attention = FALSE;
    USB_PDC_CHARGE_STATUS status[NUMBER_OF_USB_PDC];
		BYTE	i, TypeCStatus_temp;
		//WORD Temp;

    //if (0 != (UCSI_ATTRIBUTE & UCSI_BM_ATTRIBUTES_BATTERY_CHARGING))
    {
	    for (i = 0; NUMBER_OF_USB_PDC > i; i++) {
	        //i = 0;
	        /*
	        *    get charging status
	        */
	        if(i == 0)
            {
                TypeCStatus_temp = TypeCStatus;
            }
            else
            {
                TypeCStatus_temp = TypeCPort2Status;
            }
	        status[i].quick_charge   = FALSE;
	        status[i].trickle_charge = FALSE;
	        if (IS_MASK_SET(TypeCStatus_temp,TypeCAdpReady)) {
	        	//	Temp = ((WORD)(CurrentH & 0x7F) << 8) + CurrentL;
	           // if (IS_MASK_CLEAR(CurrentH, BIT7) && (Temp > 100)) {
	              //  if (Temp < 300) {
	                if(IS_M_Trickle || IS_S_Trickle)
                    {
	                	//Battery Charge Current < 0x01F4  (500mA) :Trickle charge
	                    status[i].quick_charge   = FALSE;
	                    status[i].trickle_charge = TRUE;
	                }
                    else if(IS_MASK_SET(M_BAT_Status1,M_Fast_T) || IS_MASK_SET(S_BAT_Status1,M_Fast_T))
	                {
	                    status[i].quick_charge   = TRUE;
	                    status[i].trickle_charge = FALSE;
	                }
	           // }
	        //}
	        if ((status[i].quick_charge   != Usb_Pdc_Charge_Status[i].quick_charge) ||
	            (status[i].trickle_charge != Usb_Pdc_Charge_Status[i].trickle_charge)){
	            /*
	            *   Charging status is changed
	            */
	            Usb_Pdc_Charge_Status[i]      = status[i];
              //UCSI_BATTERY_CHARGE_STATUS_CHANGE = 1;
	            if (0 != (UCSI_NOTIFICATION_ENABLE & UCSI_BATTERY_CHARGE_STATUS_CHANGE_NOTIFICATION_ENABLE)) {
			            UCSI_CONNECTOR_NUMBER_CHANGED[i] = 1;//SysConvertUsbPdcIdToUsbConnectorId(i);
			            //CCI_SetConnectorChangeIndicator(i+1);
			            UCSI_CONNECTOR_CHANGE_STATUS[i] |= (UCSI_CONNECTOR_STATUS_CHANGE_BATTERY_CHARGE_STATUS | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
	                attention = TRUE;
                    RamDebug(0xCC);
                    RamDebug(i);
                    RamDebug(Usb_Pdc_Charge_Status[i].quick_charge);
                    RamDebug(Usb_Pdc_Charge_Status[i].trickle_charge);
	            }
	        }
	  }
          else
          {
            Usb_Pdc_Charge_Status[i].quick_charge = FALSE;
            Usb_Pdc_Charge_Status[i].trickle_charge = FALSE;
          }
	  }
	    if (TRUE == attention) {
	        ECSMI_SCIEvent(AM_UCSI_EVENT); //printf ("("[Q10:AM_UCSI_EVENT]\n");
	    }
	  }
}
/*
*******************************************************************************
* Function name: GetPdPortAddress
*
* Descriptoin:
*          This is used to Type-C port address select.
*
* Arguments:
*       UCHAR OPM request connector
* Return Values:
*       int   number of data written into data area
*******************************************************************************
*/

BYTE GetPdPortAddress(BYTE ReqConnector) {
    BYTE rc;

    rc = 0;
				if(ReqConnector > 0)
				{
					rc = TypeC_02_Addr;
				}
				else
				{
					rc = TypeC_01_Addr;
				}
    return rc;
}

BYTE GetPdDeviceConnect(BYTE ReqConnector) {
    BYTE rc;

    rc = 0;
				if(ReqConnector > 0)
				{
					rc = Usb_Pdc_Status2.device_connected;
				}
				else
				{
					rc = Usb_Pdc_Status.device_connected;
				}
    return rc;
}


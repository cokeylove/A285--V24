#ifndef __USBSLAVE_H__  //{
#define __USBSLAVE_H__
//******************************************************************************
//
//  usb_slave.h
//
//  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
//
//  Created on: 2012/11/13
//
//******************************************************************************
#include "chip_type.h"

// *****************************************************************************
//
//  Macro Define
//
// *****************************************************************************
//#define __SC_DBG_ENABLE__
//#define __SC_ERR_DBG_ENABLE__
#define __ENABLE_USB_INTERRUPT__

// *****************************************************************************
//
//  Debug Message
//
// *****************************************************************************
#ifdef __SC_DBG_ENABLE__ // {
#define SC_DBG                      printf
#else
#define SC_DBG(a,...)
#endif // } _SC_DBG_ENABLE_

#ifdef __SC_ERR_DBG_ENABLE__ // {
#define SC_ERR_DBG                  printf
#else
#define SC_ERR_DBG(a,...)
#endif // } _SC_DBG_ENABLE_

// *****************************************************************************
//
//  USB error code
//
// *****************************************************************************
#define USB_ERROR_SUCCESS               (0)
#define MMP_ERROR_OFFSET                (16) // Error offset
#define MMP_MODULE_USB                  (1)  // module ID
#define ERROR_USB_BASE                  (MMP_MODULE_USB << MMP_ERROR_OFFSET)

#define ERROR_USB_HW_ASYN_SCHEDULE_ENABLE_TIMEOUT                   (ERROR_USB_BASE + 0x0001)
#define ERROR_USB_HW_ASYN_SCHEDULE_DISABLE_TIMEOUT                  (ERROR_USB_BASE + 0x0002)
#define ERROR_USB_HW_PERIODIC_SCHEDULE_ENABLE_TIMEOUT               (ERROR_USB_BASE + 0x0003)
#define ERROR_USB_HW_PERIODIC_SCHEDULE_DISABLE_TIMEOUT              (ERROR_USB_BASE + 0x0004)
#define ERROR_USB_HOST_RESET_FAIL                                   (ERROR_USB_BASE + 0x0005)
#define ERROR_USB_HOST_RUN_FAIL                                     (ERROR_USB_BASE + 0x0006)
#define ERROR_USB_HOST_STOP_FAIL                                    (ERROR_USB_BASE + 0x0007)
#define ERROR_USB_HOST_PORT_SUSPEND_FAIL                            (ERROR_USB_BASE + 0x0008)
#define ERROR_USB_HOST_FORCE_PORT_RESUME_FAIL                       (ERROR_USB_BASE + 0x0009)
#define ERROR_USB_ALLOCATE_QHD_BUF_FAIL                             (ERROR_USB_BASE + 0x000A)
#define ERROR_USB_RELEASE_QHD_BUF_FAIL                              (ERROR_USB_BASE + 0x000B)
#define ERROR_USB_ALLOCATE_QTD_BUF_FAIL                             (ERROR_USB_BASE + 0x000C)
#define ERROR_USB_RELEASE_QTD_BUF_FAIL                              (ERROR_USB_BASE + 0x000D)
#define ERROR_USB_GET_QTD_BUF_FAIL                                  (ERROR_USB_BASE + 0x000E)
#define ERROR_USB_GET_4K_PAGE_FAIL                                  (ERROR_USB_BASE + 0x000F)

#define ERROR_USB_ALLOCATE_4K_BUFFER_FAIL                           (ERROR_USB_BASE + 0x0010)
#define ERROR_USB_RELEASE_4K_BUFFER_FAIL                            (ERROR_USB_BASE + 0x0011)
#define ERROR_USB_INVALID_INPUT                                     (ERROR_USB_BASE + 0x0012)
#define ERROR_USB_HOST_DEVICE_CONNECT_TIMEOUT                       (ERROR_USB_BASE + 0x0013)
#define ERROR_USB_RELEASE_STRUCT_FAIL                               (ERROR_USB_BASE + 0x0014)
#define ERROR_USB_RELEASE_STRUCT_FAIL1                              (ERROR_USB_BASE + 0x0015)
#define ERROR_USB_BUS_RESET_FAIL                                    (ERROR_USB_BASE + 0x0016)
#define ERROR_USB_WAIT_QTD_SEND_STATUS_TIMEOUT                      (ERROR_USB_BASE + 0x0017)
#define ERROR_USB_EHCI_QTD_STATUS_HALT                              (ERROR_USB_BASE + 0x0018)
#define ERROR_USB_EHCI_QTD_STATUS_BUFFER_ERROR                      (ERROR_USB_BASE + 0x0019)
#define ERROR_USB_EHCI_QTD_STATUS_BABBLE                            (ERROR_USB_BASE + 0x001A)
#define ERROR_USB_EHCI_QTD_STATUS_TRANSACTION_ERROR                 (ERROR_USB_BASE + 0x001B)
#define ERROR_USB_EHCI_QTD_STATUS_MISS_MICROFRAME                   (ERROR_USB_BASE + 0x001C)
#define ERROR_USB_CONTROL_UNKNOWN_COMMAND                           (ERROR_USB_BASE + 0x001D)
#define ERROR_USB_CONFIGURATION_NUM_FAIL                            (ERROR_USB_BASE + 0x001E)
#define ERROR_USB_INTERFACE_NUM_FAIL                                (ERROR_USB_BASE + 0x001F)

#define ERROR_USB_ENDPOINT_NUM_FAIL                                 (ERROR_USB_BASE + 0x0020)
#define ERROR_USB_NOT_FIND_F_DRIVER_OBJECT                          (ERROR_USB_BASE + 0x0021)
#define ERROR_USB_F_DRIVER_OBJECT_NOT_INIT_OK                       (ERROR_USB_BASE + 0x0022)
#define ERROR_USB_GET_PHY_ERROR                                     (ERROR_USB_BASE + 0x0023)
#define ERROR_USB_DEVICE_NOT_CONNECTED                              (ERROR_USB_BASE + 0x0024)
#define ERROR_USB_EHCI_HOST_SUSPEND_FAIL                            (ERROR_USB_BASE + 0x0025)
#define ERROR_USB_OTG_WAIT_HOST_VBUS_VALID_TIMEOUT                  (ERROR_USB_BASE + 0x0026)
#define ERROR_USB_OTG_WAIT_HOST_VBUS_INVALID_TIMEOUT                (ERROR_USB_BASE + 0x0027)
#define ERROR_USB_OTG_WAIT_CHANGE_DEVICE_ROLE_TIMEOUT               (ERROR_USB_BASE + 0x0028)
#define ERROR_USB_EHCI_DEVICE_CLASS_NOT_SUPPORT                     (ERROR_USB_BASE + 0x0029)
#define ERROR_USB_EHCI_CONFIG_INTERFACE_NOT_SUPPORT                 (ERROR_USB_BASE + 0x002A)
#define ERROR_USB_EHCI_SEND_QTD_TIMEOUT                             (ERROR_USB_BASE + 0x002B)

#define ERROR_USB_MSCD_CSW_STATUS_STALL                             (ERROR_USB_BASE + 0x0040)
#define ERROR_USB_MSCD_CSW_STATUS_TAG_NOT_VALID                     (ERROR_USB_BASE + 0x0041)
#define ERROR_USB_MSCD_CSW_STATUS_SIGNATURE_NOT_VALID               (ERROR_USB_BASE + 0x0042)
#define ERROR_USB_MSCD_ISSUE_DATA_OVER_BUFFER                       (ERROR_USB_BASE + 0x0043)
#define ERROR_USB_MSCD_ISSUE_DATA_OUT_FAIL                          (ERROR_USB_BASE + 0x0044)
#define ERROR_USB_MSCD_ISSUE_DATA_IN_FAIL                           (ERROR_USB_BASE + 0x0045)
#define ERROR_USB_MSCD_ISSUE_CSW_FAIL                               (ERROR_USB_BASE + 0x0046)
#define ERROR_USB_MSCD_CLEAR_FEATURE_FAIL                           (ERROR_USB_BASE + 0x0047)

#define ERROR_USB_SCSI_SRB_ALLOCATE_FAIL                            (ERROR_USB_BASE + 0x0050)
#define ERROR_USB_SCSI_INQUIRY_ALLOCATE_FAIL                        (ERROR_USB_BASE + 0x0051)
#define ERROR_USB_SCSI_NOT_SUPPORT_DEVICE                           (ERROR_USB_BASE + 0x0052)
#define ERROR_USB_SCSI_ALLOCATE_READ_CAPACITY_MEM_FAIL              (ERROR_USB_BASE + 0x0053)
#define ERROR_USB_SCSI_BLOCK_SIZE_OVER_1024                         (ERROR_USB_BASE + 0x0054)
#define ERROR_USB_SCSI_DEVICE_IS_WRITE_PROTECTED                    (ERROR_USB_BASE + 0x0055)

#define ERROR_USB_DEVICE_CBW_SIGNATURE_INVALID                      (ERROR_USB_BASE + 0x0070)

#define ERROR_USB_DEVICE_SCSI_RECEIVE_UNSUPPORT_CMD                 (ERROR_USB_BASE + 0x0080)
#define ERROR_USB_DEVICE_SCSI_DEVICE_ERROR                          (ERROR_USB_BASE + 0x0081)
#define ERROR_USB_DEVICE_CREATE_THREAD_FAIL                         (ERROR_USB_BASE + 0x0082)
#define ERROR_OTG_INIT_ATTRIB_WRONG                                 (ERROR_USB_BASE + 0x0083)
#define ERROR_OTG_INIT_ATTRIB_WRONG1                                (ERROR_USB_BASE + 0x0084)
#define ERROR_USB_DEVICE_INVALID_REQUEST_TYPE                       (ERROR_USB_BASE + 0x0085)
#define ERROR_USB_DEVICE_INVALID_REQUEST_CODE                       (ERROR_USB_BASE + 0x0086)
#define ERROR_USB_DEVICE_INVALID_ENDPOINT_NUM                       (ERROR_USB_BASE + 0x0087)
#define ERROR_USB_DEVICE_INVALID_FIFO_NUM                           (ERROR_USB_BASE + 0x0088)
#define ERROR_USB_DEVICE_FIFO_NOT_ENABLED                           (ERROR_USB_BASE + 0x0089)
#define ERROR_USB_DEVICE_INVALID_RECIPIENT                          (ERROR_USB_BASE + 0x008A)
#define ERROR_USB_ALLOCATE_DMA_BUFFER_FAIL                          (ERROR_USB_BASE + 0x008B)
#define ERROR_USB_RELEASE_DMA_BUFFER_FAIL                           (ERROR_USB_BASE + 0x008C)
#define ERROR_USB_DEVICE_INVALID_FEATURE_SEL                        (ERROR_USB_BASE + 0x008D)
#define ERROR_USB_DEVICE_UNSUPPORT_FEATURE_SEL                      (ERROR_USB_BASE + 0x008E)
#define ERROR_USB_DEVICE_INVALID_TEST_SEL                           (ERROR_USB_BASE + 0x008F)

#define ERROR_USB_DEVICE_INVALID_ADDRESS                            (ERROR_USB_BASE + 0x0090)
#define ERROR_USB_DEVICE_INVALID_CONFIGURATION_NUM                  (ERROR_USB_BASE + 0x0091)
#define ERROR_USB_DEVICE_INVALID_STRING_NUM                         (ERROR_USB_BASE + 0x0092)
#define ERROR_USB_DEVICE_INVALID_DESCRIPTOR                         (ERROR_USB_BASE + 0x0093)
#define ERROR_USB_DEVICE_NOT_IN_CONFIGURATION_STATE                 (ERROR_USB_BASE + 0x0094)
#define ERROR_USB_DEVICE_INVALID_INTERFACE_NUM                      (ERROR_USB_BASE + 0x0095)
#define ERROR_USB_DEVICE_NOT_ISOCHRONOUS_TX_TYPE                    (ERROR_USB_BASE + 0x0096)
#define ERROR_USB_DEVICE_SCSI_TX_DIR_ERROR                          (ERROR_USB_BASE + 0x0097)
#define ERROR_USB_DEVICE_ILLEGAL_COMMAND                            (ERROR_USB_BASE + 0x0098)
#define ERROR_USB_DEVICE_ILLEGAL_PARAM_LIST                         (ERROR_USB_BASE + 0x0099)
#define ERROR_USB_DEVICE_GET_CAPACITY_FAIL                          (ERROR_USB_BASE + 0x009A)
#define ERROR_USB_DEVICE_CARD_NO_RESPONSE                           (ERROR_USB_BASE + 0x009B)
#define ERROR_USB_DEVICE_OPEN_FAIL                                  (ERROR_USB_BASE + 0x009C)
#define ERROR_USB_DEVICE_INQUIRY_FAIL                               (ERROR_USB_BASE + 0x009D)
#define ERROR_USB_DEVICE_INVALID_VALUE                              (ERROR_USB_BASE + 0x009E)
#define ERROR_USB_DEVICE_INVALID_LENGTH                             (ERROR_USB_BASE + 0x009F)

#define ERROR_USB_PTP_DEVICE_STATUS_NOT_OK                          (ERROR_USB_BASE + 0x00A0)
#define ERROR_USB_PTP_CLEAR_FEATURE                                 (ERROR_USB_BASE + 0x00A1)
#define ERROR_USB_PTP_DEVICE_RESET                                  (ERROR_USB_BASE + 0x00A2)

//  Flow Error
#define ERROR_USB_TX_SEND_FAIL                                      (ERROR_USB_BASE + 0x00A3)
#define ERROR_USB_TX_STATUS_FAIL                                    (ERROR_USB_BASE + 0x00A4) 
#define ERROR_USB_RX_RECEIVE_FAIL                                   (ERROR_USB_BASE + 0x00A5)
#define ERROR_USB_RX_STATUS_FAIL                                    (ERROR_USB_BASE + 0x00A6) 
#define ERROR_USB_RX_READFIFO_FAIL                                  (ERROR_USB_BASE + 0x00A7)

// *****************************************************************************
//
//
//
// *****************************************************************************
typedef enum _T_USB_RESUME_TYPE_
{
    USB_RESUME_RESET = 0,
    USB_RESUME_RESUME,
    USB_RESUME_REMOTEWAKEUP,
} USBResumeType;

// *****************************************************************************
//
//  External Variable
//
// *****************************************************************************
extern volatile BOOL gbUSBSuspended;

// *****************************************************************************
//
//  External Functions
//
// *****************************************************************************
inline BOOL usb_slave_is_shutdown(void);
inline BOOL usb_slave_is_require_resume(void);
extern BOOL usb_slave_is_busy(void);
extern BOOL usb_slave_resume(UINT8 u8Flag);
extern void usb_slave_check_suspend(void);
extern BOOL usb_slave_status_stage_in(void);

extern void usb_slave_init(void);
extern void usb_slave_ip_init(UINT8 p_action);
extern void usb_slave_main(void);

extern void usb_hid_isr(void);
extern void do_usb_slave_hid_init(void);

// *****************************************************************************
#endif //}__USBSLAVE_H__

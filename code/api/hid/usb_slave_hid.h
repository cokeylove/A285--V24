#ifndef __USB_SLAVE_HID_H__
#define __USB_SLAVE_HID_H__

#include "chip_type.h"


extern void usb_slave_hid_data_set(UINT8 *pBuf, void *pParam);

extern UINT32 usb_slave_hid_cmd_dispatch(UINT16 wReq, UINT16 wVal, UINT8 *pBufIn, UINT8 *pBufOut, UINT16 u16BufSize);

extern void usb_slave_hid_init();

#endif //__HID_MEMS_H__

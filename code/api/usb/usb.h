#ifndef __USB_H__	//{
#define __USB_H__

#include "config.h"
#include "chip_type.h"

#if 1//#ifdef __ENABLE_USB__



/* ************************************************************************
 *  Debug setting for printf
 *  
 * ********************************************************************* */

#define	DEB_ENABLE		0x00
#define	DEB_IN			(0x00 & DEB_ENABLE)
#define	DEB_OUT			(0x00 & DEB_ENABLE)
#define	DEB_SETUP		(0x00 & DEB_ENABLE)
#define	DEB_ERR			(0x01 & DEB_ENABLE)
#define	DEB_FUNCTION	(0x01 & DEB_ENABLE)



/* ************************************************************************
 *  Standard Request code(bRequest)
 *  USB spec. page248
 * ********************************************************************* */
#define	GET_STATUS			0x00
#define	CLEAR_FEATURE		0x01
#define	SET_FEATURE			0x03
#define	SET_ADDRESS			0x05
#define	GET_DESCRIPTOR		0x06
#define	GET_CONFIGURATION	0x08
#define	SET_CONFIGURATION	0x09
#define GET_MAX_LUN		0xFE
#define MSC_RESET		0xFF

/* ************************************************************************
 *  Standard Description Type(wValue)
 *  USB spec. page249
 * ********************************************************************* */
#define DEVICE			0x0100
#define CONFIGURATION	0x0200
#define INTERFACE		0x0300
#define ENDPOINT		0x0400

#define HUB_DESCRIPTOR	0x2900// USB spec  p414

#define REMOTE_WAKEUP	0x0001 //Page 250 
#define ENDPOINT_HALT	0x0000 //Page 250 

#define CONTROL_PIPE	0x00  // The control pipe are mostly Zero.


/* ************************************************************************
 *  HID Class Request code( bRequest)
 *  HID spec. page50
 * ********************************************************************* */
#define	GET_REPORT		0x01
#define	GET_IDLE    	0x02
#define	GET_PROTOCOL	0x03
#define	SET_REPORT		0x09
#define	SET_IDLE		0x0A
#define	SET_PROTOCOL	0x0B

#define	BOOT_PROTOCOL	0x0000
#define	REPORT_PROTOCOL	0x0001

/* ************************************************************************
 *  HUB Class Feature Selector( wValue)
 *  USB spec. page418
 * ********************************************************************* */
#define	C_HUB_LOCAL_POWER		0x0000
#define	C_HUB_OVER_CURRENT		0x0001
#define	PORT_CONNECTION			0x0000
#define	PORT_ENABLE				0x0001
#define	PORT_SUSPEND			0x0002
#define	PORT_OVER_CURRENT		0x0003
#define	PORT_RESET				0x0004
#define	PORT_POWER				0x0008
#define	PORT_LOW_SPEED			0x0009
#define	C_PORT_CONNECTION		0x0010
#define	C_PORT_ENABLE			0x0011
#define	C_PORT_SUSPEND			0x0012
#define	C_PORT_OVERCURRENT		0x0013
#define	C_PORT_RESET			0x0014
#define	PORT_TEST				0x0015
#define	PORT_INICATOR			0x0016

/* ************************************************************************
 *  HUB Class Feature Selector( wIndex)
 *  USB spec. page432
 * ********************************************************************* */
#define PORT1		0x0001
#define PORT2		0x0002
#define PORT3		0x0003

/* ************************************************************************
 *  HID Class code
 *  HID spec. page7
 * ********************************************************************* */
#define HID			0x03
#define HUB			0x09

/* ************************************************************************
 *  HID SubClass code
 *  HID spec. page8
 * ********************************************************************* */
#define BOOT_INTERFACE_SUBCLASS 0x01

/* ************************************************************************
 *  HID Class Protocol code
 *  HID spec. page9
 * ********************************************************************* */
#define NONE		0x00
#define KEYBOARD	0x01
#define MOUSE		0x02


/* ************************************************************************
 *  Keyboard Input report meaning define(byte0), Byte1 should be reserved(0x00).
 *  HID spec. page56
 * ********************************************************************* */
#define	L_CTRL			0x01
#define	L_SHIFT			0x02
#define	L_ALT			0x04
#define	L_GUI			0x08
#define	R_CTRL			0x10
#define	R_SHIFT			0x20
#define	R_ALT			0x40
#define	R_GUI			0x80

/* ************************************************************************
 *  Keyboard Input report meaning define (byte2 ~ byte7)
 *  HID Usage Table page53 ~ 59
 * ********************************************************************* */
#define	I_BENTER		0x28  // keyboard Enter usage ID
#define	I_PENTER		0x58  // keypad Enter usage ID
#define	I_BDELETE		0x4C  // keyboard delete usage ID
#define	I_PDELETE		0x63  // keypad delete(.) usage ID
#define	I_BACKSPACE		0x2A
#define	I_CAPLOCK		0x39
#define	I_NUMLOCK		0x53
#define	I_SCROLLLOCK	0x47
#define	I_SPACE			0x2C

/* ************************************************************************
 *  Parameter setting
 *  mode select for IN_Train, OUT_train
 * ********************************************************************* */
#define	IN_SETUP		0x00
#define	IN_HUB			0x01
#define	IN_KEY			0x02
#define	OUT_STATUS		0x00
#define	OUT_DATA		0x01
#define	OUT_MSC			0x02

#define DATA_TOGGLE0	0x00
#define DATA_TOGGLE1	0x01

/* ************************************************************************
 *  Parameter setting for Host Register
 *  Mode select for HOST_TX_TRANS_TYPE_REG
 * ********************************************************************* */
#define	SETUP		0x00
#define	IN			0x01
#define	OUTDATA0	0x02
#define	OUTDATA1	0x03

/* ************************************************************************
 *  Parameter setting for Host Register
 *  Mode select for HOST_RX_STATUS_REG
* ********************************************************************* */

#define	INDATA0		0x00
#define	INDATA1		0x01


/* ************************************************************************
 *  Bit mapping define for Bit select
 * ********************************************************************* */

/* ************************************************************************
 *  Bit mapping define for HOST_RX_STATUS_REG
 * ********************************************************************* */

#define	CRC_ERROR_BIT			BIT0
#define	BIT_STUFF_ERROR_BIT		BIT1
#define	RX_OVERFLOW_BIT			BIT2
#define	RX_TIME_OUT_BIT			BIT3
#define	NAK_RXED_BIT			BIT4
#define	STALL_RXED_BIT			BIT5
#define	ACK_RXED_BIT			BIT6
#define	DATA_SEQUENCE_BIT		BIT7


/* ************************************************************************
 *  Delay Setting, unit is ms
 * ********************************************************************* */

#define	SET_ADDR_DELAY		50 	// Delay after set address command
#define	RESET_LOW_T			50	// 
#define	RESET_RECOVER_T		10	// 

/* ************************************************************************
 *  idVender
 * ********************************************************************* */

#define	APPLE			0x05AC 	// Apple vender ID

/* ************************************************************************
 *  idProduct
 * ********************************************************************* */
#define	IPHONE_4		0x1297 	// Apple vender ID
#define	IPAD_2			0x129F 	// Apple vender ID
#define	IPHONE_4S		0x12A0 	// Apple vender ID
#define	NEW_IPAD		0x12A4 	// Apple vender ID


/* ************************************************************************
 *  Parameter setting for Error handle
 * ********************************************************************* */

#define	SETUP_RETRY_CNT		0x10
#define	IN_RETRY_CNT		0x10
#define	OUT_RETRY_CNT		0x10
#define	STALL_RETRY_CNT		0x10
#define READ10_PROBE_CNT	0x10

#define DEVICE_RESET_CNT 300 
#define CNT2SUSPEND	500
#define HUBCNT2SUSPEND	10
 
/* ************************************************************************
 *  Parameter setting for MISC Control Register
 * ********************************************************************* */
/*
#define	SETUP		0x00
#define	IN			0x01
#define	OUTDATA0	0x02
#define	OUTDATA1	0x03
*/

//#define KB_ADDR			0x02

//#define	KEYnum			0x06
//uchar	PASSKEY1[KEYnum]={0x04,0x05,0x06,0x07,0x08,0x09};	//PASSWork set1='a','b','c','d','e','f'





#endif //}__ENABLE_USB__
#endif //}__USB_H__
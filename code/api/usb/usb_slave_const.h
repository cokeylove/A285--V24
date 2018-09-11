#ifndef __USBSLAVE_CONST_H__	//{
#define __USBSLAVE_CONST_H__

#include "config.h"
#include "mem_layout.h"
#include "../oem/Project_H/INCLUDE/oem_project.h"



////// Define USB transfer test defines//////
#define Bulk_AP					0
#define Interrupt_AP			1   //Important:this setting only for cross-connection, NOT for PC test-bench
#define IsochronousIN_AP		2

#define OTG_AP_Satus			Interrupt_AP//Bulk_AP

#if(OTG_AP_Satus == Interrupt_AP)
#include "hid_common.h"
#endif

//==================================
// Constant for Device Descriptor
//==================================
// Table 9-2. - Request type
#define REQUEST_TYPE_STANDARD               0x00
#define REQUEST_TYPE_CLASS                  0x20
#define REQUEST_TYPE_VENDOR                 0x40
#define REQUEST_TYPE_RESERVED               0x60
#define REQUEST_TYPE_MASK                   0x60

// Table 9-2. - Recipient
#define RECIPIENT_TYPE_DEVICE               0
#define RECIPIENT_TYPE_INTERFACE            1
#define RECIPIENT_TYPE_ENDPOINT             2
#define RECIPIENT_TYPE_OTHERS               3
#define RECIPIENT_TYPE_MASK                 0x1F


#define INDEX_DIRECTION_IN                  1
#define INDEX_DIRECTION_OUT                 0


// Table 9-5. - Descriptor Types
#define DT_DEVICE							1
#define DT_CONFIGURATION					2
#define DT_STRING							3
#define DT_INTERFACE						4
#define DT_HID      						0x21
#define DT_ENDPOINT							5
#define DT_DEVICE_QUALIFIER					6
#define DT_OTHER_SPEED_CONFIGURATION		7
#define DT_INTERFACE_POWER					8
#define DT_OTG				                9

#define DT_REPORT_DESCRIPTOR      			0x22 // HID , get Reoprt descriptor


// Table 9-6. - Feature Selectors
#define FEATURE_SEL_ENDPOINT_HALT           0
#define FEATURE_SEL_DEVICE_REMOTE_WAKEUP    1
#define FEATURE_SEL_DEVICE_TEST_MODE        2


// Table 9-7. - Test Mode Selectors
#define TEST_MODE_TEST_J                    0x1
#define TEST_MODE_TEST_K                    0x2
#define TEST_MODE_TEST_SE0_NAK              0x3
#define TEST_MODE_TEST_PACKET               0x4
#define TEST_MODE_TEST_FORCE_ENABLE         0x5


// Descriptor Table uses the following parameters : fixed
#define DEVICE_LENGTH				0x12
#define CONFIG_LENGTH				0x09
#define INTERFACE_LENGTH			0x09
#define HID_LENGTH	        		0x09  // for HID Device
#define EP_LENGTH					0x07
#define DEVICE_QUALIFIER_LENGTH		0x0A
#define OTG_LENGTH                  0x03 //For OTG


#if(OTG_AP_Satus == Interrupt_AP)
#define FS_C1_I0_A0_EP_NUMBER          0X01
#else
#define FS_C1_I0_A0_EP_NUMBER          0X02
#endif
#define FS_C1_I0_A0_EP_LENGTH			(EP_LENGTH * FS_C1_I0_A0_EP_NUMBER)
#define FS_C1_I0_ALT_LENGTH				(FS_C1_I0_A0_EP_LENGTH)
#define FS_C1_INTERFACE_LENGTH				(FS_C1_I0_ALT_LENGTH)

#if(OTG_AP_Satus == Interrupt_AP)
    #define FS_C1_CONFIG_TOTAL_LENGTH               (CONFIG_LENGTH + INTERFACE_LENGTH +  FS_C1_INTERFACE_LENGTH + HID_LENGTH)
#elif (OTG_AP_Satus == IsochronousIN_AP)
    #define FS_C1_CONFIG_TOTAL_LENGTH               (39)// only one alternate setting //(279)
#else    
    #define FS_C1_CONFIG_TOTAL_LENGTH               (CONFIG_LENGTH + INTERFACE_LENGTH +  FS_C1_INTERFACE_LENGTH)
#endif




#define FS_MAX_CONFIG_TOTAL_LENGTH              (FS_C1_CONFIG_TOTAL_LENGTH)
#define CONFIG_LENGTH_EX                (FS_MAX_CONFIG_TOTAL_LENGTH)

#define STRING_00_LENGTH			0x04
#define STRING_10_LENGTH			0x1e
#define STRING_20_LENGTH			(USB_STR_DEVICE_NAME_LENGTH + 2)
#define STRING_30_LENGTH			0x2e
#define STRING_40_LENGTH			0x26
#define STRING_50_LENGTH			0x10
#define STRING_60_LENGTH			0x1e
#define STRING_70_LENGTH			0x10
#define STRING_80_LENGTH			0x0e
#define STRING_90_LENGTH			0x00
#define STRING_03_SERIAL_NUM_LENGTH	        0x20


#define mLowByte(u16)	((UINT8)(u16	 ))
#define mHighByte(u16)	((UINT8)(u16 >> 8))
// device configuration:
#define FS_bDeviceClass         0X00
#define FS_bDeviceSubClass      0X00
#define FS_bDeviceProtocol      0X00
#define FS_iManufacturer        0X01
#define FS_iProduct             0X02
#define FS_iSerialNumber        0X03
#define FS_CONFIGURATION_NUMBER 0X01

/** Only for Full Speed */
#define FS_USB_SPEC_VER        0x0200
//#define FS_USB_SPEC_VER        0x0110

#define FS_DEVICE_RELEASE_NO   ITE_FW_VERSION

// Configuration 0X01
#define FS_C1_INTERFACE_NUMBER  0X01
#define FS_C1                   0X01
#define FS_C1_iConfiguration    0x00 //0X30 by powei
#define FS_C1_iMaxPower         0x19 //0X00 by powei
#define FS_C1_I0_A0_bInterfaceNumber   0X00
#define FS_C1_I0_A0_bAlternateSetting  0X00


#if(OTG_AP_Satus == Interrupt_AP)

#define FS_PRODUCT_ID          ITE_PRODUCT_ID
#define FS_VENDOR_ID           ITE_VENDOR_ID

#define FS_C1_bmAttribute       0xA0//0X80 //0XC0 by powei

#define FS_C1_I0_A0_bInterfaceClass    0x03//0X00
#define FS_C1_I0_A0_bInterfaceSubClass 0X00
#define FS_C1_I0_A0_bInterfaceProtocol 0X00
#define FS_C1_I0_A0_iInterface         0x00//0X40

#else
#define FS_VENDOR_ID           0x1DB7
#define FS_PRODUCT_ID          0x0220 // move to usb_pid.h
#define FS_C1_bmAttribute       0x80//0XC0 //0XC0 by powei
#define FS_C1_I0_A0_bInterfaceClass    0X08 //0X00 by powei
#define FS_C1_I0_A0_bInterfaceSubClass 0X06 //0X00 by powei
#define FS_C1_I0_A0_bInterfaceProtocol 0X50 //0X00 by powei
#define FS_C1_I0_A0_iInterface         0X00 //0X40 by powei
#endif

// device configuration:
#define HS_bDeviceClass         0X00
#define HS_bDeviceSubClass      0X00
#define HS_bDeviceProtocol      0X00
#define HS_iManufacturer        0X01
#define HS_iProduct             0X02
#define HS_iSerialNumber        0X03
#define HS_CONFIGURATION_NUMBER 0X01

/** Only for High Speed */
#define HS_USB_SPEC_VER        0x0200
#define HS_VENDOR_ID           0x1DB7
#define HS_PRODUCT_ID          0x0220 // move to usb_pid.h
#define HS_DEVICE_RELEASE_NO   0x0100


//==================================
// Hardware related constant - for FIFO and endpoint
//==================================
// Block Size define
#define BLK64BYTE		1
#define BLK128BYTE		2

// Block toggle number define
#define SINGLE_BLK		1
#define DOUBLE_BLK		2
#define TRIBLE_BLK		3

// Endpoint transfer type
#define TF_TYPE_ISOCHRONOUS		1
#define TF_TYPE_BULK			2
#define TF_TYPE_INTERRUPT		3

// Endpoint or FIFO direction define
#define DIRECTION_IN		0
#define DIRECTION_OUT	1

// Endpoint number define
#define EP0        0x00
#define EP1        0x01
#define EP2        0x02


#if(OTG_AP_Satus == Interrupt_AP)
#if (FS_C1_I0_A0_EP_NUMBER >= 0X01)
    //EP0X01
    #define FS_C1_I0_A0_EP1_BLKSIZE    BLK64BYTE
    #define FS_C1_I0_A0_EP1_BLKNO      SINGLE_BLK
    #define FS_C1_I0_A0_EP1_DIRECTION  DIRECTION_IN
    #define FS_C1_I0_A0_EP1_TYPE       TF_TYPE_INTERRUPT
    #define FS_C1_I0_A0_EP1_MAX_PACKET 0x0040
    #define FS_C1_I0_A0_EP1_bInterval  0x01
#endif
#if (FS_C1_I0_A0_EP_NUMBER >= 0X02)
    //EP0X02
    #define FS_C1_I0_A0_EP2_BLKSIZE    BLK64BYTE
    #define FS_C1_I0_A0_EP2_BLKNO      SINGLE_BLK
    #define FS_C1_I0_A0_EP2_DIRECTION  DIRECTION_OUT
    #define FS_C1_I0_A0_EP2_TYPE       TF_TYPE_INTERRUPT
    #define FS_C1_I0_A0_EP2_MAX_PACKET 0x0040
    #define FS_C1_I0_A0_EP2_bInterval  01
#endif


#else
#define FS_C1_I0_A0_EP1_BLKSIZE    BLK64BYTE
#define FS_C1_I0_A0_EP1_BLKNO      DOUBLE_BLK
#define FS_C1_I0_A0_EP1_DIRECTION  DIRECTION_IN
#define FS_C1_I0_A0_EP1_TYPE       TF_TYPE_BULK
#define FS_C1_I0_A0_EP1_MAX_PACKET 0x0040
#define FS_C1_I0_A0_EP1_bInterval  00


#define FS_C1_I0_A0_EP2_BLKSIZE    BLK64BYTE
#define FS_C1_I0_A0_EP2_BLKNO      DOUBLE_BLK
#define FS_C1_I0_A0_EP2_DIRECTION  DIRECTION_OUT
#define FS_C1_I0_A0_EP2_TYPE       TF_TYPE_BULK
#define FS_C1_I0_A0_EP2_MAX_PACKET 0x0040
#define FS_C1_I0_A0_EP2_bInterval  00
#endif

//==================================
// Hardware related constant
//==================================
#define DEVICE_MAX_ENDPOINT_NUM		4
#define DEVICE_MAX_FIFO_NUM	        4
#define EP0MAXPACKETSIZE	        0x40

extern  const UINT8 HID_RODATA u8HSDeviceDescriptor[DEVICE_LENGTH];

extern const UINT8 HID_RODATA u8FSDeviceDescriptor[DEVICE_LENGTH];
extern const UINT8 HID_RODATA u8FSConfigDescriptor01[FS_C1_CONFIG_TOTAL_LENGTH];



extern  const UINT8 HID_RODATA u8String00Descriptor[STRING_00_LENGTH];
extern  const UINT8 HID_RODATA u8String10Descriptor[STRING_10_LENGTH];
extern  const UINT8 HID_RODATA u8String20Descriptor[STRING_20_LENGTH];

extern  const UINT8 HID_RODATA u8StringOSDescriptor[];
extern  const UINT8 HID_RODATA u8StringExtendPropertyDescriptor[];

#endif //}__USBSLAVE_CONST_H__

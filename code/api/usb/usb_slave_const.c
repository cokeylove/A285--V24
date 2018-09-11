#include "usb.h"
#include "usb_reg.h"
#include "usb_slave_const.h"

// Full speed Configuration
#if (OTG_AP_Satus == IsochronousIN_AP)
UINT8 u8OTGFSDeviceDescriptor[DEVICE_LENGTH] =
{
    0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40, 0x45, 0x0C,
    0x29, 0x60, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01 // iProduct = 0x01
};
#else
const UINT8 HID_RODATA u8FSDeviceDescriptor[DEVICE_LENGTH] =
{
	//	DEVICE descriptor : from 0
	DEVICE_LENGTH,					// bLength , 0x12
	DT_DEVICE,						// bDescriptorType , 0x01
	mLowByte(FS_USB_SPEC_VER),			// bcdUSB , 0x00
	mHighByte(FS_USB_SPEC_VER),      // 0x02
	FS_bDeviceClass,			    // bDeviceClass , 0x00
	FS_bDeviceSubClass,			    // bDeviceSubClass
	FS_bDeviceProtocol,			    // bDeviceProtocol
	EP0MAXPACKETSIZE,				// bMaxPacketSize0
	mLowByte(FS_VENDOR_ID),			// idVendor
	mHighByte(FS_VENDOR_ID),
	mLowByte(FS_PRODUCT_ID),			// idProduct
	mHighByte(FS_PRODUCT_ID),
	mLowByte(FS_DEVICE_RELEASE_NO),	// bcdDeviceReleaseNumber
	mHighByte(FS_DEVICE_RELEASE_NO),
	FS_iManufacturer,			    // iManufacturer
	FS_iProduct,				    // iProduct
	#if (OTG_AP_Satus == Interrupt_AP)
	0x00,
	#else
	FS_iSerialNumber, 			    // iSerialNumber
	#endif
	FS_CONFIGURATION_NUMBER			// bNumConfigurations
};
#endif
#if (OTG_AP_Satus == IsochronousIN_AP)
UINT8 u8FSConfigOTGDescriptor01[FS_C1_CONFIG_TOTAL_LENGTH] =
{

    0x09, 0x02, 0x27,0x00,/*0x17, 0x01,*/ 0x01, 0x01, 0x00, 0x80, 0xFA, //configuration
    0x09, 0x04, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0x00, //interface 0 , alternateSetting 0
    0x07, 0x05, 0x81, 0x01, 0x00,0x02,/*0xFF, 0x03,*/ 0x01, 
    0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00, 
    0x07, 0x05, 0x83, 0x03, 0x01, 0x00, 0x64             //endpoint3,IN,BULK

};
#else
const UINT8 HID_RODATA u8FSConfigDescriptor01[FS_C1_CONFIG_TOTAL_LENGTH] =
{
    //======================================
	//	CONFIGURATION descriptor
    //======================================
	CONFIG_LENGTH,					// bLength
	DT_CONFIGURATION,				// bDescriptorType CONFIGURATION
	mLowByte(FS_C1_CONFIG_TOTAL_LENGTH),	// wTotalLength, include all descriptors
	mHighByte(FS_C1_CONFIG_TOTAL_LENGTH),
	FS_C1_INTERFACE_NUMBER,			// bNumInterface
	FS_C1,							// bConfigurationValue
	FS_C1_iConfiguration,			// iConfiguration
	FS_C1_bmAttribute,				// bmAttribute
									// D7: Reserved(set to one), D6: Self-powered, D5: Remote Wakeup, D4..0: Reserved(reset to zero)
	#if (OTG_AP_Satus == Interrupt_AP)
	0x32,
	#else
	FS_C1_iMaxPower,				// iMaxPower (2mA units)
    #endif
    //======================================
	// Interface 0
    //======================================
	//#if (FS_C1_INTERFACE_NUMBER >= 1)
	//	#if (FS_C1_I0_ALT_NUMBER >= 1)
			// Alternate Setting 0
			INTERFACE_LENGTH,				// bLength
			DT_INTERFACE,					// bDescriptorType INTERFACE
			FS_C1_I0_A0_bInterfaceNumber,   // bInterfaceNumber
			FS_C1_I0_A0_bAlternateSetting,	// bAlternateSetting
			FS_C1_I0_A0_EP_NUMBER,			// bNumEndpoints(excluding endpoint zero)
			FS_C1_I0_A0_bInterfaceClass,	// bInterfaceClass
			FS_C1_I0_A0_bInterfaceSubClass, // bInterfaceSubClass
			FS_C1_I0_A0_bInterfaceProtocol, // bInterfaceProtocol
			FS_C1_I0_A0_iInterface,		    // iInterface

            #if (OTG_AP_Satus == Interrupt_AP) 
            //HID descriptor        
            HID_LENGTH,            //0x09 ,  bLength  
            DT_HID,                 //0x21 , bDescriptorType
            0x12,                  //bcdHID , low byte 
            0x01,                   ////bcdHID , high byte 
            0x00,                   //bCountryCode
            0x01,               //bNumDescriptor
            DT_REPORT_DESCRIPTOR,               //bDescriptorType (report)
            mLowByte(HID_REPORT_DES_TOTAL_LENGTH), // wDescriptorLen  low byte
            mHighByte(HID_REPORT_DES_TOTAL_LENGTH),// wDescriptorLen  high byte
            #endif
            
            
            //======================================
	        // endpoint 1
            //======================================
			#if (FS_C1_I0_A0_EP_NUMBER >= 1)
				// EP1
				EP_LENGTH,						                // bLength
				DT_ENDPOINT,					                // bDescriptorType ENDPOINT
				(((1 - FS_C1_I0_A0_EP1_DIRECTION) << 7) | EP1),	// bEndpointAddress
									                            // D7: Direction, 1=IN, 0=OUT
									                            // D6..4: Reserved(reset to zero), D3..0: The endpointer number
				FS_C1_I0_A0_EP1_TYPE,			                // bmAttributes
									                            // D1..0: Transfer Type 00=Control, 01=Isochronous, 10=Bulk, 11=Interrupt
									                            // if not an isochronous endpoint, D7..2 are Reserved
				mLowByte(FS_C1_I0_A0_EP1_MAX_PACKET),	        // wMaxPacketSize
				mHighByte(FS_C1_I0_A0_EP1_MAX_PACKET),
				FS_C1_I0_A0_EP1_bInterval,						// Interval for polling endpoint for data transfers.
			#endif
            //======================================
	        // endpoint 2
            //======================================
			#if (FS_C1_I0_A0_EP_NUMBER >= 2)
				// EP2
				EP_LENGTH,
				DT_ENDPOINT,
				(((1 - FS_C1_I0_A0_EP2_DIRECTION) << 7) | EP2),
				FS_C1_I0_A0_EP2_TYPE,	
				mLowByte(FS_C1_I0_A0_EP2_MAX_PACKET),
				mHighByte(FS_C1_I0_A0_EP2_MAX_PACKET),
				FS_C1_I0_A0_EP2_bInterval,
			#endif
};
#endif
// Full speed Configuration
#if (OTG_AP_Satus == IsochronousIN_AP)
UINT8 u8HSDeviceDescriptor[DEVICE_LENGTH] =
{
    0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40, 0x45, 0x0C,
    0x29, 0x60, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01 // iProduct = 0x01
};
#else
const UINT8 HID_RODATA u8HSDeviceDescriptor[DEVICE_LENGTH] =
{
	//	DEVICE descriptor
	DEVICE_LENGTH,					// bLength
	DT_DEVICE,						// bDescriptorType
	mLowByte(HS_USB_SPEC_VER),			// bcdUSB
	mHighByte(HS_USB_SPEC_VER),
	HS_bDeviceClass,			    // bDeviceClass
	HS_bDeviceSubClass,			    // bDeviceSubClass
	HS_bDeviceProtocol,			    // bDeviceProtocol
	EP0MAXPACKETSIZE,				// bMaxPacketSize0
	mLowByte(HS_VENDOR_ID),			// idVendor
	mHighByte(HS_VENDOR_ID),
	mLowByte(HS_PRODUCT_ID),			// idProduct
	mHighByte(HS_PRODUCT_ID),
	mLowByte(HS_DEVICE_RELEASE_NO),	// bcdDeviceReleaseNumber
	mHighByte(HS_DEVICE_RELEASE_NO),
	HS_iManufacturer,			    // iManufacturer
	HS_iProduct,				    // iProduct
#if (OTG_AP_Satus == Interrupt_AP)
	0x00,
#else
	HS_iSerialNumber, 			    // iSerialNumber
#endif
	HS_CONFIGURATION_NUMBER			// bNumConfigurations

};
#endif

const UINT8 HID_RODATA u8String00Descriptor[STRING_00_LENGTH] =
{//Language ID
   STRING_00_LENGTH,			// Size of this descriptor
   // STRING Descriptor type
   // Language ID, 0409: English, 0404: Chinese Taiwan
   0X03,     //bDescriptorType
   0X09,     //bLang
   0X04,
};

const UINT8 HID_RODATA u8String10Descriptor[STRING_10_LENGTH] =
{// iManufacturer
       STRING_10_LENGTH,            // Size of this descriptor
       DT_STRING,                   // STRING Descriptor type
       0X49, 0,  //I
       0X54, 0,  //T
       0X45, 0,  //E
       0X20, 0,  // 
       0X54, 0,  //T
       0X65, 0,  //e
       0X63, 0,  //c
       0X68, 0,  //h
       0x2E, 0,  //.
       0x20, 0,  //
       0X49, 0,  //I
       0X6E, 0,  //n
       0X63, 0,  //c
       0X2E, 0   //.
};

const UINT8 HID_RODATA u8String20Descriptor[STRING_20_LENGTH] =
{//iProduct
   STRING_20_LENGTH,			// Size of this descriptor
   DT_STRING,					// STRING Descriptor type   
   USB_STR_DEVICE_NAME
};


const UINT8 HID_RODATA u8StringOSDescriptor[] =
{
    0x12,
    0x03,
    0x4D,0x00,
    0x53,0x00,
    0x46,0x00,
    0x54,0x00,
    0x31,0x00,
    0x30,0x00,
    0x30,0x00,

    0x01, //"	bMS_VendorCode
    
    0x00,
   
};

const UINT8 HID_RODATA u8StringExtendPropertyDescriptor[] =
{
   //Header Section
   0x4C, 0, 0, 0,			// Size of this descriptor
   0, 0x01,					// BCD ver
   0x05, 0,                 //Extended property OS descriptor
   0x01, 0,                 //custom property count 
   
   //Custom Property Section
   0x42, 0, 0, 0,           //66 Bytes for this property
   0x04, 0, 0, 0,           //dwPropertyDataType (REG_DWORD_LITTLE_ENDIAN)
   0x30, 0,                 //wPropertyNameLength
   0x53, 0x00, 0x65, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x76, 0x00,
   0x65, 0x00, 0x53, 0x00, 0x75, 0x00, 0x73, 0x00, 0x70, 0x00, 0x65, 0x00, 0x6E, 0x00, 0x64, 0x00,
   0x45, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x62, 0x00, 0x6C, 0x00, 0x65, 0x00, 0x64, 0x00, 0x00, 0x00,
   
   0x04, 0, 0, 0,           //dwPropertyDataLength
   0x01, 0, 0, 0,           //bPropertyData
   
};



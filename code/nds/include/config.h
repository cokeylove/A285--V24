#ifndef __config_h
#define __config_h


/* lowlevel platform initialization if no bootcode.
 * Don't do this if there is bootcode on the platform, 
 * let bootcode do the work.*/
//#define BURN		0 	/* burn = 1, load = 0 */
//#define USE_LM_DMA
//#define CONFIG_AG101P_16MB 	/* In 16MB board*/
//#define AG101         1 /* For AG101 board, else for XC5 board */


//*******************************************************************************
#define ITE_VENDOR_ID   0x048D
#define ITE_FW_VERSION  0x1017 //0xYZZZ -> Y: ROM lib version(0~9), Z: release version(000~999)
#define ITE_FW_REVISION 8234


//*******************************************************************************
// 0 : EC firmware only
// 1 : RTOS ( EC+Fusion+HID )
// 2 : RTOS (Fusion+HID)
// 3 : Non-RTOS ( EC+Fusion+HID )
//
//*******************************************************************************
#if (Firmware_Feature_Selection == 0)
    #define EC_FEATURE_ONLY
    
#elif (Firmware_Feature_Selection == 1)
    #undef  EC_FEATURE_ONLY

#elif (Firmware_Feature_Selection == 2)
    #undef  EC_FEATURE_ONLY
    #define EC_FEATURE_NULL
    
#elif (Firmware_Feature_Selection == 3)
    #define EC_FEATURE_ONLY
    #define EC_FEATURE_ONLY_SUPPORT_FUSION

#endif

#endif  // __config_h
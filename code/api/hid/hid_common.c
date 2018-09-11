#include <stdlib.h>
#include <string.h>

#include "hid_common.h"
#include "hid_custom_cmd.h"
#include "hal.h"
#include "sensor_fusion.h"
#include "i2c_drv.h"
#include "core_memory.h"
#include "hid_hal.h"
#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
#include "core_asm.h"
#endif

//********************************************************************************
#define HID_PM_THRESHOLD                    2000

//	#define HID_SENSOR_REPORT_INTERVAL_SCALE    1
//	#define HID_SENSOR_REPORT_INTERVAL(t)       ((t) / HID_SENSOR_REPORT_INTERVAL_SCALE)

//min report interval(ms)
#define HID_MIN_REPORT_INVAL_ACC            5
#define HID_MIN_REPORT_INVAL_GYR	        5
#define HID_MIN_REPORT_INVAL_COM	        5
#define HID_MIN_REPORT_INVAL_INC	        5
#define HID_MIN_REPORT_INVAL_ORI	        5
#define HID_MIN_REPORT_INVAL_ALS            50
#define HID_MIN_REPORT_INVAL_GRA            5
#define HID_MIN_REPORT_INVAL_LACC	        5

#if __ENABLE_HID_SENSOR_STATE__ // {
#define HID_SENSOR_DEFAULT_EVENT  HID_USAGE_SENSOR_EVENT_DATA_UPDATED_ENUM
#define HID_SENSOR_DEFAULT_STATE  HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM

#define GET_SENSOR_STATE(f)        ((f).SensorState)
#define SET_SENSOR_STATE(f, s)     do{(f).SensorState = (s);}while(0)
#define SET_EVENT(f, e)            do{(f).Event = (e);}while(0)
#define CLEAR_EVENT(f)             do{(f).Event = HID_SENSOR_DEFAULT_EVENT;}while(0)
#else
#define HID_SENSOR_DEFAULT_EVENT HID_USAGE_SENSOR_EVENT_DATA_UPDATED_ENUM
#define HID_SENSOR_DEFAULT_STATE HID_USAGE_SENSOR_STATE_READY_ENUM

#define GET_SENSOR_STATE(f) ((f).PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM)?(HID_USAGE_SENSOR_STATE_READY_ENUM):(HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM)
#define SET_SENSOR_STATE(f, s) 
#define SET_EVENT(f, e)        
#define CLEAR_EVENT(f)
#endif // } __ENABLE_HID_SENSOR_STATE__

#define SET_REPORTING_STATE(f, s)  do{(f).ReportingState = (s);}while(0)
#define SET_POWER_STATE(f, s)      do{(f).PowerState = (s);}while(0)

#if 1
#define HID_SENSOR_DEFAULT_REPORTING_STATE  HID_USAGE_REPORTING_STATE_ON_ALL_ENUM
#define HID_SENSOR_DEFAULT_POWER_STATE      HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM
#else
#define HID_SENSOR_DEFAULT_REPORTING_STATE  HID_USAGE_REPORTING_STATE_ON_NONE_ENUM
#define HID_SENSOR_DEFAULT_POWER_STATE      HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM
#endif

#define IS_REPORTING_STATE_CHANGE(f, s) ((s) != (f).ReportingState)
#define IS_REPORTING_STATE_LEGAL(s)     (((s) >= HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_ENUM) &&   \
                                         ((s) <= HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_ENUM)) 

#define IS_POWER_STATE_CHANGE(f, s)     ((s) != (f).PowerState)
#define IS_POWER_STATE_LEGAL(s)         (((s) >= HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM) &&   \
                                         ((s) <= HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM))                                         

#define IS_POWER_STATE_ON(f) (((f).PowerState == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM) || \
                              ((f).PowerState == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_ENUM))

#define IS_FEATURE_SIZE_OK(x, y) (((x) == (y)) || ((x) == HID_READ_BUF_SIZE))

#define HID_REPORT_UNIT_ACC(out, in)    do{(out) = -((short)(in)) * 1000 / 256;}while(0)
#define HID_REPORT_UNIT_SHAKE(out, in)  do{(out) = (UINT8)(in);}while(0)
#define HID_REPORT_UNIT_GYR(out, in)    do{(out) = ((short)(in)) * 223811 / 10;}while(0)    // (180/pi)/256
#define HID_REPORT_UNIT_MAG(out, in)    do{(out) = ((short)(in)) * 1000000 / 256;}while(0)
#define HID_REPROT_UNIT_NORTH(out, in)  do{(out) = ((short)(in)) * 224 / 100;}while(0)      //0.1 unit (for Win8: /256 *180 /pai)
#define HID_REPORT_UNIT_INC(out, in)    do{(out) = ((short)(in)) * 224 / 100;}while(0)      //0.1 unit (for Win8: /256 *180 /pai)
#define HID_REPORT_UNIT_ORI(out, in)    do{(out) = (((short)(in)) * 1000000 / 256) * 10;}while(0) //0.0000001 unit (for Win8: /256)
#define HID_REPORT_UNIT_MATRIX(out, in) do{(out) = (SINT16)(in) * 1000 / 256;}while(0)
#define HID_REPORT_UNIT_ALS(out, in)    do{                                                 \
                                            (out) = (UINT32)((in) * ALS_Multiplier * 100);  \
                                            if(ALS_Divisor)                                 \
                                                (out) /= (UINT32)ALS_Divisor;               \
                                        }while(0)
#define HID_REPORT_UNIT_COLOR(out, in)  do{(out) = (in);}while(0)                                      
#define HID_REPORT_UNIT_CHROM(out, in)  do{(out) = (in);}while(0) // 10^(-4) -> do it in the driver
#define HID_REPORT_UNIT_ANGLE(out, in)  do{(out) = ((in) * 2238 / 10000) * 10;}while(0)                                        
#define HID_REPORT_UNIT_PRS(out, in)    do{(out) = (in);}while(0) // 1Bar = 10^5 Pa -> do it in the driver
#define HID_REPORT_UNIT_HYG(out, in)    do{(out) = (in) * 100;}while(0)
#define HID_REPORT_UNIT_TEMP(out, in)   do{(out) = ((short)(in)) * 100;}while(0)
#define HID_REPORT_UNIT_PROX(out, in)   do{(out) = (in) * 10;}while(0)
#define HID_REPORT_UNIT_UV(out, in)     do{(out) = (in) * 100;}while(0)

// [0]Set Interval, [1]Get Feature, [2]Set Feature, [3]Change Sensitivity, [4]Get Input Report
#define HID_FUNC_LIST(name)  {_s_i_##name, _g_f_##name, _s_f_##name, _c_s_##name, _g_i_##name}
#define SET_INTERVAL(name, time) do{_s_i_##name(time);}while(0)

// Debug Msg.
#define HID_DBG_MSG(...)     printk(__VA_ARGS__)
#define HID_DBG_STATE(...)   printk(__VA_ARGS__)
#define HID_DBG_PERIOD(...)  //printk(__VA_ARGS__)

// Debug GPIO
#define HID_DATA_SET_IN()     //do{GPDRA |= BIT7;}while(0)
#define HID_DATA_SET_OUT()    //do{GPDRA &= ~BIT7;}while(0)
#define HID_ACC_INPUT_IN()    //do{GPDRA |= BIT6;}while(0)
#define HID_ACC_INPUT_OUT()   //do{GPDRA &= ~BIT6;}while(0)
#define HID_GYO_INPUT_IN()    //do{GPDRA |= BIT5;}while(0)
#define HID_GYO_INPUT_OUT()   //do{GPDRA &= ~BIT5;}while(0)
#define HID_ORI_INPUT_IN()    //do{GPDRA |= BIT4;}while(0)
#define HID_ORI_INPUT_OUT()   //do{GPDRA &= ~BIT4;}while(0)

// Compile-Time Error Msg.
#define CCASSERT(predicate, msg) typedef char (msg)[2*((predicate))-1];

//********************************************************************************
typedef void (*P_SET_REPORT_INTERVAL)(UINT32 u32ReportInterval);
typedef  int (*P_GET_FEATURE)(UINT8 *pBuf);
typedef void (*P_SET_FEATURE)(UINT8 *pBuf, UINT16 u16BufSize);
typedef BOOL (*P_CHECK_SENSITIVITY)(void);
typedef  int (*P_GET_INPUT_REPORT)(UINT8 *pBuf);

typedef struct _T_HID_FUNC_
{
    P_SET_REPORT_INTERVAL pfnSetReportInterval;
    P_GET_FEATURE pfnGetFeature;
    P_SET_FEATURE pfnSetFeature;
    P_CHECK_SENSITIVITY pfnCheckSensitivity;
    P_GET_INPUT_REPORT pfnGetInputReport;
}T_HID_FUNC, *P_HID_FUNC;

//********************************************************************************
const char HID_VER_RODATA szHidVersion[32] = {'H', 'i', 'd', 'V', 'e', 'r', ':', 
                                              '0'+(ITE_FW_REVISION/1000), 
                                              '0'+((ITE_FW_REVISION/100)%10),
                                              '0'+((ITE_FW_REVISION/10)%10),
                                              '0'+(ITE_FW_REVISION%10),
                                              '-',
                                              
                                              #if _ENABLE_ACC_ // {
                                              'A',
                                              #endif // } _ENABLE_ACC_
                                              
                                              #if _ENABLE_GYO_ // {
                                              'G',
                                              #endif // } _ENABLE_GYO_
                                              
                                              #if _ENABLE_MAG_ // {
                                              'M',
                                              #endif // } _ENABLE_MAG_
                                              
                                              #if _ENABLE_ALS_ // {
                                              'L',
                                              #endif // } _ENABLE_ALS_
                                              
                                              #if _ENABLE_ACC2_ // {
                                              'A',
                                              #endif // } _ENABLE_ACC2_
                                              
                                              #if _ENABLE_PRS_ // {
                                              'P',
                                              #endif // } _ENABLE_PRS_
                                              
                                              #if _ENABLE_HYG_ // {
                                              'H',
                                              #endif // } _ENABLE_HYG_
                                              
                                              #if _ENABLE_TEMP_ // {
                                              'T',
                                              #endif // } _ENABLE_TEMP_
                                              
                                              #if _ENABLE_PROX_ // {
                                              'X',
                                              #endif // } _ENABLE_PROX_
                                              
                                              #if _ENABLE_UV_ // {
                                              'U',
                                              #endif // } _ENABLE_UV_
                                              
                                              #if DEF_DUAL_OS // {
                                              'D',
                                              #endif // } DEF_DUAL_OS
                                              
                                              '-', '0'+TOTAL_HID_DEVICE,
                                              
                                              #ifdef __ENABLE_HID_I2C_API__ // {
                                              'I',
                                              #endif // } __ENABLE_HID_I2C_API__

                                              #ifdef __ENABLE_HID_USB_API__ // {
                                              'U',
                                              #endif // } __ENABLE_HID_USB_API__

                                              #ifdef EC_FEATURE_ONLY // {
                                              '-', 'N',
                                              #else
                                              '-', 'R',
                                              #endif // } EC_FEATURE_ONLY
                                              
                                              '\0'
                                              };

const char HID_VER_RODATA szHidHotfixVersion[16] = "Hotfix:0004";
// r8234 Hotfix: 0001 -> HID/I2C R/W ¥Ñ polling IRQ §ï¬° polling status
// r8234 Hotfix: 0002 -> ALS³Ì¤p­È¬°1§ï¥Ñlib¸Ì±±¨î
// r8234 Hotfix: 0003 -> ­×¥¿HID/I2C report descriptor°e§¹«á·|¶}±ÒILMªºissue, HID report descriptor¥þ³¡©ñ¨ìRAM9
// r8234 Hotfix: 0004 -> HID report descriptor·h¨ìSRAM¤§«á¨Ã³]©w¬°DLM

const UINT8 HID_RODATA hid_report_descriptor[] =
{
    // *************************** Custom Device *************************** //
    0x06, 0x83, 0xFF,                   // USAGE_PAGE (Vender Defined Usage Page)
    0x09, 0x80,                         // USAGE (Vendor Usage 0x01)
    0xA1, 0x01,                         // COLLECTION (Application)
    0x85, HID_CUSTOM_REPORT_ID,         // REPORT_ID (90)
    0x09, 0x01,                         // USAGE (Vendor Usage 0x01)
    0x15, 0x00,                         // LOGICAL_MINIMUM(0)
    0x26, 0xff, 0x00,                   // LOGICAL_MAXIMUM(255)
    0x75, 0x08,                         // REPORT_SIZE (0x08)
    0x95, HID_CUSTOM_CMD_LENGTH,        // REPORT_COUNT (0x10)
    0xB1, 0x00,                         // FEATURE (Data,Ary,Abs)
    0xC0,                               // END_COLLECTION

    // ***************************  Sensor Hub  *************************** //
    #if (TOTAL_HID_DEVICE > 1)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_COLLECTION,
    HID_COLLECTION(Application),
    #endif

    #if __ENABLE_HID_ACC__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_ACC),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_MOTION_ACCELEROMETER_3D,
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_ACC),
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif
    
    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),     
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_X_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Y_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Z_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),

    //include the following datafield if required to support the shake event
    HID_USAGE_SENSOR_DATA_MOTION_STATE,
    HID_LOGICAL_MIN_8(0), // False = Still
    HID_LOGICAL_MAX_8(1), // True = In Motion
    HID_REPORT_SIZE(8), 
    HID_REPORT_COUNT(1), 
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_GYO__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_GYR),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_MOTION_GYROMETER_3D,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_MOTION_GYROMETER_3D,
    HID_COLLECTION(Application),
    HID_REPORT_ID(HID_SENSOR_GYR),    
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs), 
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    
    #if __ENABLE_HID_MIN_REPORT_INVAL__
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif
    
    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0xB),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0xB),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0x0B), // scale default unit to provide 5 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0x0B), // scale default unit to provide 5 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0x0B), // scale default unit to provide 5 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY_ROLL_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0xB), // scale default unit to provide 5 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY_PITCH_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0xB), // scale default unit to provide 5 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ANGULAR_VELOCITY_YAW_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES_PER_SECOND,
    HID_UNIT_EXPONENT(0xB), // scale default unit to provide 5 digits past decimal point
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_GYO__

    #if __ENABLE_HID_MAG__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_MAG),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ORIENTATION_COMPASS_3D,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ORIENTATION_COMPASS_3D,
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_MAG),    
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),    
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    
    #if __ENABLE_HID_MIN_REPORT_INVAL__
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif

    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH, HID_USAGE_SENSOR_DATA_MOD_RESOLUTION),
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH, HID_USAGE_SENSOR_DATA_MOD_ACCURACY),
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH, HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH, HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH, HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX, HID_USAGE_SENSOR_DATA_MOD_RESOLUTION),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x10, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX, HID_USAGE_SENSOR_DATA_MOD_ACCURACY),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x10, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX, HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX, HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x10, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX, HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x10, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ORIENTATION_COMPENSATED_MAGNETIC_NORTH,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digit past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX_X_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digit past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX_Y_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digit past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETIC_FLUX_Z_AXIS,
    HID_LOGICAL_MIN_32(0xFF, 0xFF, 0x01, 0x00),
    HID_LOGICAL_MAX_32(0xFF, 0x7F, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLIGAUSS,
    HID_UNIT_EXPONENT(0xD), // scale default unit to provide 3 digit past decimal point
    HID_INPUT(Data_Var_Abs),
    
    #if __ENABLE_HID_MAG_ACCURACY__ // {
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_8(2), 
    HID_REPORT_SIZE(8), 
    HID_REPORT_COUNT(1), 
    HID_COLLECTION(Logical), 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_LOW, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_MEDIUM, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_HIGH, 
        HID_INPUT(Data_Arr_Abs), 
        HID_END_COLLECTION,
    #else
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(3),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_MAG__

    #if __ENABLE_HID_FUSION_SENSOR__ // {
    //---------------------------------------------
    HID_REPORT_ID(HID_SENSOR_INC),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ORIENTATION_INCLINOMETER_3D,
    HID_COLLECTION(Physical),

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),    
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0xF),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_TILT,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_TILT,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_TILT,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ORIENTATION_TILT_X,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_ORIENTATION_TILT_Y,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_ORIENTATION_TILT_Z,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_DEGREES,
    HID_UNIT_EXPONENT(0x0F), // scale default unit to provide 1 digits past decimal point
    HID_INPUT(Data_Var_Abs),

    #if __ENABLE_HID_MAG_ACCURACY__ // {
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_8(2), 
    HID_REPORT_SIZE(8), 
    HID_REPORT_COUNT(1), 
    HID_COLLECTION(Logical), 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_LOW, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_MEDIUM, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_HIGH, 
        HID_INPUT(Data_Arr_Abs), 
        HID_END_COLLECTION,
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    HID_END_COLLECTION,

    //---------------------------------------------
    HID_REPORT_ID(HID_SENSOR_ORI),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ORIENTATION_DEVICE_ORIENTATION,
    HID_COLLECTION(Physical),

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0x00),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),    
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION, HID_USAGE_SENSOR_DATA_MOD_RESOLUTION),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION, HID_USAGE_SENSOR_DATA_MOD_ACCURACY),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION, HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9), // scale default unit to provide 7 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION, HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9), // scale default unit to provide 7 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION, HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9), // scale default unit to provide 7 digits past decimal point
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_ROTATION_MATRIX__ // {
    //include this if the values are calculated in firmware
    //otherwise, the driver will calculate these values from the Quaternion
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_ROTATION_MATRIX,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF), 
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0E), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_ROTATION_MATRIX,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ORIENTATION_ROTATION_MATRIX,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), 
    HID_FEATURE(Data_Var_Abs),
    #endif // }  __ENABLE_HID_ROTATION_MATRIX__
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ORIENTATION_QUATERNION,
    HID_LOGICAL_MIN_32(0x80, 0x69, 0x67, 0xFF),
    HID_LOGICAL_MAX_32(0x80, 0x96, 0x98, 0x00),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(4),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x9), // scale default unit to provide 7 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    
    #if __ENABLE_HID_ROTATION_MATRIX__ // {
    //include this if the values are calculated in firmware
    //otherwise, the driver will calculate these values from the Quaternion
    HID_USAGE_SENSOR_DATA_ORIENTATION_ROTATION_MATRIX,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(9), 
    HID_UNIT_EXPONENT(0x0D), 
    HID_INPUT(Data_Var_Abs),
    #endif // } __ENABLE_HID_ROTATION_MATRIX__

    #if __ENABLE_HID_MAG_ACCURACY__ // {
    HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_8(2), 
    HID_REPORT_SIZE(8), 
    HID_REPORT_COUNT(1), 
    HID_COLLECTION(Logical), 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_LOW, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_MEDIUM, 
        HID_USAGE_SENSOR_DATA_ORIENTATION_MAGNETOMETER_ACCURACY_HIGH, 
        HID_INPUT(Data_Arr_Abs), 
        HID_END_COLLECTION,
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_FUSION_SENSOR__

    #if __ENABLE_HID_ALS__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_ALS),
    HID_USAGE_PAGE_SENSOR,     // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_LIGHT_AMBIENTLIGHT, // USAGE (AmbientLight)
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,     // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_LIGHT_AMBIENTLIGHT, // USAGE (AmbientLight)
    HID_COLLECTION(Application),
    HID_REPORT_ID(HID_SENSOR_ALS),    
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0),
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_LIGHT_ILLUMINANCE, HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_REL_PCT),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0x10, 0x27), // 10000 = 0.00 to 100.00 percent with 2 digits past decimal point
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_LUX,
    HID_UNIT_EXPONENT(0x0E),  // scale default unit to provide 2 digits past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_LIGHT_ILLUMINANCE, HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_LUX,
    HID_UNIT_EXPONENT(0x00), // scale default unit to provide 0 digit past decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_LIGHT_ILLUMINANCE, HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_LUX,
    HID_UNIT_EXPONENT(0x00), // scale default unit to provide 0 digit past decimal point
    HID_FEATURE(Data_Var_Abs),

    #ifdef SENSOR_ALS_CURVE // {
    //add this definition if required by the specific application
    HID_USAGE_SENSOR_PROPERTY_RESPONSE_CURVE,
    HID_LOGICAL_MIN_16(0x01, 0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF, 0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(SENSOR_ALS_CURVE_LENGTH), //as required, here for n pair of values
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED, //define as required to match application 
    HID_UNIT_EXPONENT(0x0), // scale default unit to provide 0 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    #endif // }  SENSOR_ALS_CURVE

    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_LIGHT_ILLUMINANCE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF),
//	    HID_USAGE_SENSOR_UNITS_LUX,
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digit past decimal point
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_INPUT(Data_Var_Abs),

    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    HID_USAGE_SENSOR_DATA_LIGHT_COLOR_TEMPERATURE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_UNIT_EXPONENT(0), 
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_LIGHT_CHROMATICITY_X,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_UNIT_EXPONENT(0x0C), // scale default unit to provide 4 digits past decimal point
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_LIGHT_CHROMATICITY_Y,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_UNIT_EXPONENT(0x0C), // scale default unit to provide 4 digits past decimal point
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
    HID_INPUT(Data_Var_Abs),  
    #endif // } __ENABLE_ALS_COLOR_VALUE__

    #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xE),
    HID_INPUT(Data_Var_Abs),
    #endif // } __ENABLE_ALS_CUSTOM_VALUE__

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_ALS__

    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_CUSTOM),
    HID_USAGE_PAGE_SENSOR,         // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_OTHER_CUSTOM, // USAGE (Other Custom)
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,         // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_OTHER_CUSTOM, // USAGE (Other Custom)
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_CUSTOM),    
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),                   //LOGICAL_MINIMUM (0) 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), //LOGICAL_MAXIMUM (4294967295) 
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),          //LOGICAL_MINIMUM (0) 
    HID_LOGICAL_MAX_16(0xFF,0xFF), //LOGICAL_MAXIMUM (65535) 
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xF), 
    HID_FEATURE(Data_Var_Abs),

    #ifdef HID_CUSTOM_MODEL_STRING // {
    HID_USAGE_SENSOR_PROPERTY_SENSOR_MODEL, 
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(HID_CUSTOM_MODEL_LENGTH), 
    HID_FEATURE(Data_Var_Abs),
    #endif // } HID_CUSTOM_MODEL_STRING

    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    
    #if _ENABLE_ACC2_ // {
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xD),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_2,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xD),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_3,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xD),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_4,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xF), 
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_5,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xF), 
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_6,
    HID_LOGICAL_MIN_16(0x01,0x80),             //LOGICAL_MINIMUM (-32767) 
    HID_LOGICAL_MAX_16(0xFF,0x7F),             //LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xF), 
    HID_INPUT(Data_Var_Abs),
    #else
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_2,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_3,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_4,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_5,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_6,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_16(0xFF, 0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0x0),
    HID_INPUT(Data_Var_Abs),
    #endif // } _ENABLE_ACC2_

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__

    #if __ENABLE_HID_PRESSURE__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_PRS),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_PRS),        
    #endif
        
    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
	HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
	HID_REPORT_SIZE(8),
	HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
		HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
    	HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
    	HID_USAGE_SENSOR_STATE_READY_SEL,
    	HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
    	HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
    	HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
    	HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
    	HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0B), // scale default unit to provide 5 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,
    HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
	HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
	HID_REPORT_SIZE(8),
	HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
		HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
		HID_USAGE_SENSOR_STATE_READY_SEL,
		HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
		HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
		HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
		HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
		HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
	HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
	HID_REPORT_SIZE(8),
	HID_REPORT_COUNT(1),
	HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,   
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_ATMOSPHERIC_PRESSURE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0B), // scale default unit to provide 5 digits past the decimal point
    HID_INPUT(Data_Var_Abs),
    
    HID_END_COLLECTION,  
    #endif   // }  __ENABLE_HID_PRESSURE__

    #if __ENABLE_HID_HUMIDITY__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_HYG),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_HUMIDITY,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_HUMIDITY,
    HID_COLLECTION(Application),
    HID_REPORT_ID(HID_SENSOR_HYG),        
    #endif
    
    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_RELATIVE_HUMIDITY,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0x10,0x27), // 10000 = 0.00 to 100.00 percent with 2 digits past decimal point
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_RELATIVE_HUMIDITY,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0x10,0x27), // 10000 = 0.00 to 100.00 percent with 2 digits past decimal point
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_RELATIVE_HUMIDITY,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0x10,0x27), // 10000 = 0.00 to 100.00 percent with 2 digits past decimal point
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),

    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_RELATIVE_HUMIDITY,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0x10,0x27), // 10000 = 0.00 to 100.00 percent with 2 digits past decimal point
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit “percent?to provide 2 digits past the decimal point
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__ // {
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_TEMP),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_TEMP),    
    #endif
    
    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0E), // scale default unit "Celsius" to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit "Celsius" to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit "Celsius" to provide 2 digits past the decimal point
    HID_FEATURE(Data_Var_Abs),

    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(16),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0E), // scale default unit "Celsius" to provide 2 digits past the decimal point
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_PROX),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_BIOMETRIC_PROXIMITY,
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_BIOMETRIC_PROXIMITY,
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_PROX),    
    #endif
      
    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_BIOMETRIC_HUMAN_PROXIMITY_RANGE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0F), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_BIOMETRIC_HUMAN_PROXIMITY_RANGE,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0F), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_BIOMETRIC_HUMAN_PROXIMITY_RANGE,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0F), 
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_BIOMETRIC_HUMAN_PROXIMITY_OUT_OF_RANGE,
    HID_LOGICAL_MIN_8(0), // False
    HID_LOGICAL_MAX_8(1), // True
    HID_REPORT_SIZE(8), 
    HID_REPORT_COUNT(1),
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_BIOMETRIC_HUMAN_PROXIMITY_RANGE,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
    HID_UNIT_EXPONENT(0x0F),
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_PROXIMITY__

    #if __ENABLE_HID_UV__ // {
    #if (TOTAL_HID_DEVICE > 1)
    HID_REPORT_ID(HID_SENSOR_UV),
    HID_USAGE_PAGE_SENSOR,         // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_OTHER_CUSTOM, // USAGE (Other Custom)
    HID_COLLECTION(Physical),
    #else
    HID_USAGE_PAGE_SENSOR,         // USAGE_PAGE (Sensor)
    HID_USAGE_SENSOR_TYPE_OTHER_CUSTOM, // USAGE (Other Custom)
    HID_COLLECTION(Application),    
    HID_REPORT_ID(HID_SENSOR_UV),    
    #endif

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(50),                   //LOGICAL_MINIMUM (0) 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), //LOGICAL_MAXIMUM (4294967295) 
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_CUSTOM_VALUE,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),          //LOGICAL_MINIMUM (0) 
    HID_LOGICAL_MAX_16(0xFF,0xFF), //LOGICAL_MAXIMUM (65535) 
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xE), 
    HID_FEATURE(Data_Var_Abs),

    HID_USAGE_SENSOR_PROPERTY_SENSOR_MODEL, 
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(HID_UV_MODEL_LENGTH), 
    HID_FEATURE(Data_Var_Abs),

    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xE),
    HID_INPUT(Data_Var_Abs),

    #ifdef __ENABLE_UV_2ND_VALUE__ // {
    HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_2,
    HID_LOGICAL_MIN_8(0x00),
    HID_LOGICAL_MAX_32(0xFF, 0xFF, 0xFF, 0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_NOT_SPECIFIED,
    HID_UNIT_EXPONENT(0xE),
    HID_INPUT(Data_Var_Abs),    
    #endif // } __ENABLE_UV_2ND_VALUE__

    HID_END_COLLECTION,
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    //---------------------------------------------
    HID_REPORT_ID(HID_SENSOR_GRA),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ITE_GRAVITY,
    HID_COLLECTION(Physical),

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),     
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_X_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Y_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Z_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,
    
    //---------------------------------------------
    HID_REPORT_ID(HID_SENSOR_LACC),
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ITE_LINEAR_ACCELERATION,
    HID_COLLECTION(Physical),

    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  // NAry
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL, 
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,           
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,        
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL,  
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,        
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
//	    HID_USAGE_SENSOR_PROPERTY_SENSOR_STATUS,
//	    HID_LOGICAL_MIN_8(0),
//	    HID_LOGICAL_MAX_8(0xFF),
//	    HID_REPORT_SIZE(8),
//	    HID_REPORT_COUNT(1),
//	    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_8(10),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND,
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Data_Var_Abs),

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL, 
    HID_LOGICAL_MIN_8(0), 
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF), 
    HID_REPORT_SIZE(32), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_MILLISECOND, 
    HID_UNIT_EXPONENT(0), 
    HID_FEATURE(Const_Var_Abs),
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_16(0x01, 0x80),
    HID_LOGICAL_MAX_16(0xFF, 0x7F),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0xD),
    HID_FEATURE(Data_Var_Abs),     
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_CHANGE_SENSITIVITY_ABS),
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_16(0xFF,0xFF),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MAX),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION,HID_USAGE_SENSOR_DATA_MOD_MIN),
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), 
    HID_FEATURE(Data_Var_Abs),
    
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_X_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Y_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1),
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),
    HID_USAGE_SENSOR_DATA_MOTION_ACCELERATION_Z_AXIS,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16), 
    HID_REPORT_COUNT(1), 
//	    HID_USAGE_SENSOR_UNITS_G,
    HID_UNIT_EXPONENT(0x0D), // scale default unit to provide 3 digits past decimal point
    HID_INPUT(Data_Var_Abs),

    HID_END_COLLECTION,    
    #endif // } __ENABLE_HID_ANDROID__
    
    //---------------------------------------------
    #if (TOTAL_HID_DEVICE > 1)
    HID_END_COLLECTION
    #endif
};

//==============================================================================
HID_BSS UINT32 gpCurVal[6];

#if _ENABLE_ACC2_ // {
HID_BSS UINT16 Accelerometer2[3];
HID_BSS SINT32 LastCustomAngle;
HID_BSS SINT16 CustomAngle[3];
#endif // } _ENABLE_ACC2_

#if __ENABLE_HID_ACC__ // {
#define ACC_3D_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_ACC)])
#define ACC_3D_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_ACC])

HID_DATA UINT16 ACC_3D_Feature_ChangeSensitivityABS = 20;
HID_BSS UINT16 AccelerometerX, AccelerometerY, AccelerometerZ, Shake;
HID_BSS UINT16 LastShake, LastAcc_X, LastAcc_Y, LastAcc_Z;
#endif // } __ENABLE_HID_ACC__

#if __ENABLE_HID_GYO__ // {
#define GYR_3D_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_GYR)])
#define GYR_3D_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_GYR])

HID_DATA UINT32 GYR_3D_Feature_ChangeSensitivityABS = 4000000;
HID_BSS UINT16 GyrometerX, GyrometerY, GyrometerZ;
HID_BSS UINT32 LastGyr_X, LastGyr_Y, LastGyr_Z;

#ifdef __PSEUDO_GYRO_DBG__ // {
HID_BSS UINT16 GyrometerExt[3];
#endif // } __PSEUDO_GYRO_DBG__
#endif // } __ENABLE_HID_GYO__

#if __ENABLE_HID_MAG__ // {
#define COM_3D_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_MAG)])
#define COM_3D_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_MAG])

HID_DATA UINT16 COM_3D_Feature_NorthChangeSensitivityABS = 20;
HID_DATA UINT32 COM_3D_Feature_StrenthChangeSensitivityABS = 50000;
HID_BSS UINT16 MagnetometerX, MagnetometerY, MagnetometerZ, NorthDegrees;
HID_BSS UINT16 LastNorth;
HID_BSS UINT32 LastMag_X, LastMag_Y, LastMag_Z;
#endif // } __ENABLE_HID_MAG__

#if __ENABLE_HID_FUSION_SENSOR__ // {
#define INC_3D_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_INC)])
#define INC_3D_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_INC])

HID_DATA UINT16 INC_3D_Feature_ChangeSensitivityABS = 20;
HID_BSS UINT16 InclinometerX, InclinometerY, InclinometerZ;
HID_BSS UINT16 LastTil_X, LastTil_Y, LastTil_Z;

#define ORIENTATION_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_ORI)])
#define ORIENTATION_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_ORI])

HID_DATA UINT32 ORIENTATION_Feature_QuaChangeSensitivityABS = 10000;
HID_BSS UINT16 OrientationQuaternionX, OrientationQuaternionY, OrientationQuaternionZ, OrientationQuaternionW;
HID_BSS UINT32 LastQua_X, LastQua_Y, LastQua_Z, LastQua_W;

#if __ENABLE_HID_ROTATION_MATRIX__ // {
HID_BSS UINT16 RotationMatrix[9];
#endif // } __ENABLE_HID_ROTATION_MATRIX__
#endif // } __ENABLE_HID_FUSION_SENSOR__

#if __ENABLE_HID_ALS__ // {
#define ALS_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_ALS)])
#define ALS_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_ALS])

HID_DATA UINT16 ALS_Feature_ChangeSensitivityREL_PCT = 3000;
HID_BSS UINT16 LightIlluminace;
HID_BSS UINT32 LastLux;

#ifdef __ENABLE_ALS_COLOR_VALUE__ // {
HID_BSS UINT16 ColorTemp;
HID_BSS UINT16 ChromaticityX, ChromaticityY;
#endif // } __ENABLE_ALS_COLOR_VALUE__

#ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
HID_BSS UINT16 LightCustom;
#endif // } __ENABLE_ALS_CUSTOM_VALUE__
#endif // } __ENABLE_HID_ALS__

#if __ENABLE_HID_CUSTOM_SENSOR__ // {
#define CUSTOM_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_CUSTOM)])
#define CUSTOM_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_CUSTOM])

HID_DATA UINT16 CUSTOM_Feature_ChangeSensitivityABS = 10;
#endif // } __ENABLE_HID_CUSTOM_SENSOR__

#if __ENABLE_HID_PRESSURE__ // {
#define PRS_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_PRS)])
#define PRS_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_PRS])

HID_DATA UINT32 PRS_Feature_ChangeSensitivityABS = 1;
HID_BSS UINT32 PressureBar;
HID_BSS UINT32 LastPrs;
#endif // } __ENABLE_HID_PRESSURE__

#if __ENABLE_HID_HUMIDITY__ // {
#define HYG_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_HYG)])
#define HYG_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_HYG])

HID_DATA UINT16 HYG_Feature_ChangeSensitivityABS = 100;
HID_BSS UINT16 HygValue;
HID_BSS UINT16 LastHyg;
#endif // } __ENABLE_HID_HUMIDITY__

#if __ENABLE_HID_TEMPERATURE__ // {
#define TEMP_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_TEMP)])
#define TEMP_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_TEMP])

HID_DATA UINT16 TEMP_Feature_ChangeSensitivityABS = 100;
HID_BSS UINT16 TempValue;
HID_BSS UINT16 LastTemp;
#endif // } __ENABLE_HID_TEMPERATURE__

#if __ENABLE_HID_PROXIMITY__ // {
#define PROX_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_PROX)])
#define PROX_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_PROX])

HID_DATA UINT16 PROX_Feature_ChangeSensitivityABS = 10;
HID_BSS UINT16 ProxValue;
HID_BSS UINT16 LastProx;
HID_BSS UINT8  ProxOutOfRangeFlag;
#endif // } __ENABLE_HID_PROXIMITY__

#if __ENABLE_HID_UV__ // {
#define UV_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_UV)])
#define UV_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_UV])

HID_DATA UINT16 UV_Feature_ChangeSensitivityABS = 100;
HID_BSS UINT16 UVValue;
HID_BSS UINT32 LastUV;
HID_RODATA const UINT16 UVModel[HID_UV_MODEL_LENGTH] = HID_UV_MODEL_STRING;

#ifdef __ENABLE_UV_2ND_VALUE__ // {
HID_BSS UINT16 UVValue2;
#endif // } __ENABLE_UV_2ND_VALUE__
#endif // } __ENABLE_HID_UV__

#if __ENABLE_HID_ANDROID__ // {
#define GRA_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_GRA)])
#define GRA_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_GRA])

HID_DATA UINT16 GRA_Feature_ChangeSensitivityABS = 20;
HID_BSS UINT16 GravityX, GravityY, GravityZ;
HID_BSS UINT16 LastGra_X, LastGra_Y, LastGra_Z;

#define LACC_Feature (gstHIDFeature[HID_SENSOR_IDX(HID_SENSOR_LACC)])
#define LACC_Feature_ReportInterval (gu32HIDInterval[HID_SENSOR_LACC])

HID_DATA UINT16 LACC_Feature_ChangeSensitivityABS = 20;
HID_BSS UINT16 LinearAccX, LinearAccY, LinearAccZ;
HID_BSS UINT16 LastLAcc_X, LastLAcc_Y, LastLAcc_Z;
#endif // } __ENABLE_HID_ANDROID__

#ifdef __ENABLE_HID_PM_DIPO__
//HID_BSS UINT16 gusIdleThreshold = 0;
//HID_BSS UINT8  gusHIDIdleState = 0;
#endif

#ifdef __ENABLE_HID_FAKE_ROTATE_EVENT__ // {
HID_BSS UINT8 gu8AutoRotateEvent; //for simple device orientation sensor auto rotate event
#define RESET_AUTO_ROTATE_EVENT()   do{gu8AutoRotateEvent = 10;}while(0)
#define CLEAR_AUTO_ROTATE_EVENT()   do{gu8AutoRotateEvent = 0;}while(0)
#else
#define RESET_AUTO_ROTATE_EVENT()
#define CLEAR_AUTO_ROTATE_EVENT()
#endif // } __ENABLE_HID_FAKE_ROTATE_EVENT__

// USB & I2C share the same buffer, address must under 0x82000
// size should be 64 for USB and HID_READ_BUF_SIZE for I2C
HID_DATA UINT8 gpHIDReportBuffer[72] = {0}; 
CCASSERT((sizeof(gpHIDReportBuffer) >= HID_READ_BUF_SIZE), buffer_size_too_small)

HID_DATA volatile UINT8 gu8HIDBufferUpdating = 0;

HID_DATA volatile UINT8 g_u8HIDCurID = 0;
HID_DATA volatile UINT8 g_u8HIDNextCmdID = 0;

HID_BSS volatile UINT8 gu8HIDINTC[(TOTAL_HID_DEVICE + 1)];
HID_BSS volatile UINT32 gu32HIDInterval[(TOTAL_HID_DEVICE + 1)];
HID_BSS volatile int giHIDLastTimer[(TOTAL_HID_DEVICE + 1)];
HID_BSS volatile T_HID_POWER_STATE gstHIDPowerState;
HID_BSS T_HID_COMMON_FEATURE gstHIDFeature[TOTAL_HID_DEVICE];

HID_BSS hal_mutex_t hid_mutex;     // mutex for protecting resources

//********************************************************************************
#if (DEF_DUAL_OS && DEF_OS_HOT_SWITCH) // {
HID_BSS TFCInfo gstHidConfig[2];

void HID_CODE_L hid_save_config(UINT8 u8Idx)
{
    #if __ENABLE_HID_ACC__ // {
    gstHidConfig[u8Idx].Acc.u32ReportInterval = ACC_3D_Feature_ReportInterval;
    gstHidConfig[u8Idx].Acc.u16AccelChangeSensitivity = ACC_3D_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Acc.u8ReportingState = ACC_3D_Feature.ReportingState;
    gstHidConfig[u8Idx].Acc.u8PowerState = ACC_3D_Feature.PowerState;
    #endif // } __ENABLE_HID_ACC__
       
    #if __ENABLE_HID_GYO__ // {
    gstHidConfig[u8Idx].Gyo.u32ReportInterval = GYR_3D_Feature_ReportInterval;
    gstHidConfig[u8Idx].Gyo.u32GyroChangeSensitivity = GYR_3D_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Gyo.u8ReportingState = GYR_3D_Feature.ReportingState;
    gstHidConfig[u8Idx].Gyo.u8PowerState = GYR_3D_Feature.PowerState;
    #endif // } __ENABLE_HID_GYO__
        
    #if __ENABLE_HID_MAG__ // {
    gstHidConfig[u8Idx].Mag.u32ReportInterval = COM_3D_Feature_ReportInterval;
    gstHidConfig[u8Idx].Mag.u32FluxChangeSensitivity = COM_3D_Feature_StrenthChangeSensitivityABS;
    gstHidConfig[u8Idx].Mag.u16HeadingChangeSensitivity = COM_3D_Feature_NorthChangeSensitivityABS;
    gstHidConfig[u8Idx].Mag.u8ReportingState = COM_3D_Feature.ReportingState;
    gstHidConfig[u8Idx].Mag.u8PowerState = COM_3D_Feature.PowerState;
    #endif // } __ENABLE_HID_MAG__
        
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    gstHidConfig[u8Idx].Inc.u32ReportInterval = INC_3D_Feature_ReportInterval;
    gstHidConfig[u8Idx].Inc.u16IncChangeSensitivity = INC_3D_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Inc.u8ReportingState = INC_3D_Feature.ReportingState;
    gstHidConfig[u8Idx].Inc.u8PowerState = INC_3D_Feature.PowerState;

    gstHidConfig[u8Idx].Ori.u32ReportInterval = ORIENTATION_Feature_ReportInterval;
    gstHidConfig[u8Idx].Ori.u32QuaternionChangeSensitivity = ORIENTATION_Feature_QuaChangeSensitivityABS;
    gstHidConfig[u8Idx].Ori.u8ReportingState = ORIENTATION_Feature.ReportingState;
    gstHidConfig[u8Idx].Ori.u8PowerState = ORIENTATION_Feature.PowerState;
    #endif // } __ENABLE_HID_FUSION_SENSOR__
        
    #if __ENABLE_HID_ALS__ // {
    gstHidConfig[u8Idx].Als.u32ReportInterval = ALS_Feature_ReportInterval;
    gstHidConfig[u8Idx].Als.u16IlluminanceChangeSensitivity = ALS_Feature_ChangeSensitivityREL_PCT;
    gstHidConfig[u8Idx].Als.u8ReportingState = ALS_Feature.ReportingState;
    gstHidConfig[u8Idx].Als.u8PowerState = ALS_Feature.PowerState;
    #endif // } __ENABLE_HID_ALS__
        
    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    gstHidConfig[u8Idx].Custom.u32ReportInterval = CUSTOM_Feature_ReportInterval;
    gstHidConfig[u8Idx].Custom.u16CustomSensitivity = CUSTOM_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Custom.u8ReportingState = CUSTOM_Feature.ReportingState;
    gstHidConfig[u8Idx].Custom.u8PowerState = CUSTOM_Feature.PowerState;
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__
       
    #if __ENABLE_HID_PRESSURE__ // {
    gstHidConfig[u8Idx].Prs.u32ReportInterval = PRS_Feature_ReportInterval;
    gstHidConfig[u8Idx].Prs.u32BarChangeSensitivity = PRS_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Prs.u8ReportingState = PRS_Feature.ReportingState;
    gstHidConfig[u8Idx].Prs.u8PowerState = PRS_Feature.PowerState;
    #endif // } __ENABLE_HID_PRESSURE__
       
    #if __ENABLE_HID_HUMIDITY__ // {
    gstHidConfig[u8Idx].Hyg.u32ReportInterval = HYG_Feature_ReportInterval;
    gstHidConfig[u8Idx].Hyg.u16HygChangeSensitivity = HYG_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Hyg.u8ReportingState = HYG_Feature.ReportingState;
    gstHidConfig[u8Idx].Hyg.u8PowerState = HYG_Feature.PowerState;
    #endif // } __ENABLE_HID_HUMIDITY__
       
    #if __ENABLE_HID_TEMPERATURE__// {
    gstHidConfig[u8Idx].Temp.u32ReportInterval = TEMP_Feature_ReportInterval;
    gstHidConfig[u8Idx].Temp.u16TempChangeSensitivity = TEMP_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Temp.u8ReportingState = TEMP_Feature.ReportingState;
    gstHidConfig[u8Idx].Temp.u8PowerState = TEMP_Feature.PowerState;
    #endif // } __ENABLE_HID_TEMPERATURE__
       
    #if __ENABLE_HID_PROXIMITY__ // {
    gstHidConfig[u8Idx].Prox.u32ReportInterval = PROX_Feature_ReportInterval;
    gstHidConfig[u8Idx].Prox.u16ProximityChangeSensitivity = PROX_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Prox.u8ReportingState = PROX_Feature.ReportingState;
    gstHidConfig[u8Idx].Prox.u8PowerState = PROX_Feature.PowerState;    
    #endif // } __ENABLE_HID_PROXIMITIY__
       
    #if __ENABLE_HID_UV__ // {
    gstHidConfig[u8Idx].UV.u32ReportInterval = UV_Feature_ReportInterval;
    gstHidConfig[u8Idx].UV.u16UVSensitivity = UV_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].UV.u8ReportingState = UV_Feature.ReportingState;
    gstHidConfig[u8Idx].UV.u8PowerState = UV_Feature.PowerState;      
    #endif // } __ENABLE_HID_UV__
       
    #if __ENABLE_HID_ANDROID__ // {
    gstHidConfig[u8Idx].Gra.u32ReportInterval = GRA_Feature_ReportInterval;
    gstHidConfig[u8Idx].Gra.u16GraChangeSensitivity = GRA_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Gra.u8ReportingState = GRA_Feature.ReportingState;
    gstHidConfig[u8Idx].Gra.u8PowerState = GRA_Feature.PowerState; 

    gstHidConfig[u8Idx].Lacc.u32ReportInterval = LACC_Feature_ReportInterval;
    gstHidConfig[u8Idx].Lacc.u16LAccChangeSensitivity = LACC_Feature_ChangeSensitivityABS;
    gstHidConfig[u8Idx].Lacc.u8ReportingState = LACC_Feature.ReportingState;
    gstHidConfig[u8Idx].Lacc.u8PowerState = LACC_Feature.PowerState;     
    #endif // } __ENABLE_HID_ANDROID__

    gstHidConfig[u8Idx].u16Flag = 0x8380;

}
	
void HID_CODE_L hid_restore_config(UINT8 u8Idx)
{
    TFRInfo stReport;

    if(gstHidConfig[u8Idx].u16Flag == 0x8380)
    {
        #if __ENABLE_HID_ACC__ // {
        hid_get_feature_report(HID_SENSOR_ACC, (UINT8*)&stReport);
        stReport.ACC3DFeature.u32ReportInterval = gstHidConfig[u8Idx].Acc.u32ReportInterval;
        stReport.ACC3DFeature.u16AccelChangeSensitivity = gstHidConfig[u8Idx].Acc.u16AccelChangeSensitivity;
        stReport.ACC3DFeature.u8ReportingState = gstHidConfig[u8Idx].Acc.u8ReportingState;
        stReport.ACC3DFeature.u8PowerState = gstHidConfig[u8Idx].Acc.u8PowerState;
        hid_set_feature_report(HID_SENSOR_ACC, (UINT8*)&stReport, FEATURE_SIZE_ACC_3D);   
        #endif // } __ENABLE_HID_ACC__
           
        #if __ENABLE_HID_GYO__ // {
        hid_get_feature_report(HID_SENSOR_GYR, (UINT8*)&stReport);
        stReport.GYR3DFeature.u32ReportInterval = gstHidConfig[u8Idx].Gyo.u32ReportInterval;
        stReport.GYR3DFeature.u32GyroChangeSensitivity = gstHidConfig[u8Idx].Gyo.u32GyroChangeSensitivity;
        stReport.GYR3DFeature.u8ReportingState = gstHidConfig[u8Idx].Gyo.u8ReportingState;
        stReport.GYR3DFeature.u8PowerState = gstHidConfig[u8Idx].Gyo.u8PowerState;
        hid_set_feature_report(HID_SENSOR_GYR, (UINT8*)&stReport, FEATURE_SIZE_GYR_3D); 
        #endif // } __ENABLE_HID_GYO__
            
        #if __ENABLE_HID_MAG__ // {
        hid_get_feature_report(HID_SENSOR_MAG, (UINT8*)&stReport);
        stReport.COM3DFeature.u32ReportInterval = gstHidConfig[u8Idx].Mag.u32ReportInterval;
        stReport.COM3DFeature.u32FluxChangeSensitivity = gstHidConfig[u8Idx].Mag.u32FluxChangeSensitivity;
        stReport.COM3DFeature.u16HeadingChangeSensitivity = gstHidConfig[u8Idx].Mag.u16HeadingChangeSensitivity;
        stReport.COM3DFeature.u8ReportingState = gstHidConfig[u8Idx].Mag.u8ReportingState;
        stReport.COM3DFeature.u8PowerState = gstHidConfig[u8Idx].Mag.u8PowerState;
        hid_set_feature_report(HID_SENSOR_MAG, (UINT8*)&stReport, FEATURE_SIZE_COM_3D);
        #endif // } __ENABLE_HID_MAG__
            
        #if __ENABLE_HID_FUSION_SENSOR__ // {
        hid_get_feature_report(HID_SENSOR_INC, (UINT8*)&stReport);
        stReport.INC3DFeature.u32ReportInterval = gstHidConfig[u8Idx].Inc.u32ReportInterval;
        stReport.INC3DFeature.u16IncChangeSensitivity = gstHidConfig[u8Idx].Inc.u16IncChangeSensitivity;
        stReport.INC3DFeature.u8ReportingState = gstHidConfig[u8Idx].Inc.u8ReportingState;
        stReport.INC3DFeature.u8PowerState = gstHidConfig[u8Idx].Inc.u8PowerState;
        hid_set_feature_report(HID_SENSOR_INC, (UINT8*)&stReport, FEATURE_SIZE_INC_3D);

        hid_get_feature_report(HID_SENSOR_ORI, (UINT8*)&stReport);
        stReport.ORIENTATIONFeature.u32ReportInterval = gstHidConfig[u8Idx].Ori.u32ReportInterval;
        stReport.ORIENTATIONFeature.u32QuaternionChangeSensitivity = gstHidConfig[u8Idx].Ori.u32QuaternionChangeSensitivity;
        stReport.ORIENTATIONFeature.u8ReportingState = gstHidConfig[u8Idx].Ori.u8ReportingState;
        stReport.ORIENTATIONFeature.u8PowerState = gstHidConfig[u8Idx].Ori.u8PowerState;
        hid_set_feature_report(HID_SENSOR_ORI, (UINT8*)&stReport, FEATURE_SIZE_ORIENTATION);
        #endif // } __ENABLE_HID_FUSION_SENSOR__
            
        #if __ENABLE_HID_ALS__ // {
        hid_get_feature_report(HID_SENSOR_ALS, (UINT8*)&stReport);
        stReport.ALSFeature.u32ReportInterval = gstHidConfig[u8Idx].Als.u32ReportInterval;
        stReport.ALSFeature.u16IlluminanceChangeSensitivity = gstHidConfig[u8Idx].Als.u16IlluminanceChangeSensitivity;
        stReport.ALSFeature.u8ReportingState = gstHidConfig[u8Idx].Als.u8ReportingState;
        stReport.ALSFeature.u8PowerState = gstHidConfig[u8Idx].Als.u8PowerState;
        hid_set_feature_report(HID_SENSOR_ALS, (UINT8*)&stReport, FEATURE_SIZE_AMBIENTLIGHT);    
        #endif // } __ENABLE_HID_ALS__
            
        #if __ENABLE_HID_CUSTOM_SENSOR__ // {
        hid_get_feature_report(HID_SENSOR_CUSTOM, (UINT8*)&stReport);
        stReport.CUSTOMFeature.u32ReportInterval = gstHidConfig[u8Idx].Custom.u32ReportInterval;
        stReport.CUSTOMFeature.u16CustomSensitivity = gstHidConfig[u8Idx].Custom.u16CustomSensitivity;
        stReport.CUSTOMFeature.u8ReportingState = gstHidConfig[u8Idx].Custom.u8ReportingState;
        stReport.CUSTOMFeature.u8PowerState = gstHidConfig[u8Idx].Custom.u8PowerState;
        hid_set_feature_report(HID_SENSOR_CUSTOM, (UINT8*)&stReport, FEATURE_SIZE_CUSTOM);    
        #endif // } __ENABLE_HID_CUSTOM_SENSOR__
           
        #if __ENABLE_HID_PRESSURE__ // {
        hid_get_feature_report(HID_SENSOR_PRS, (UINT8*)&stReport);
        stReport.PRSFeature.u32ReportInterval = gstHidConfig[u8Idx].Prs.u32ReportInterval;
        stReport.PRSFeature.u32BarChangeSensitivity = gstHidConfig[u8Idx].Prs.u32BarChangeSensitivity;
        stReport.PRSFeature.u8ReportingState = gstHidConfig[u8Idx].Prs.u8ReportingState;
        stReport.PRSFeature.u8PowerState = gstHidConfig[u8Idx].Prs.u8PowerState;
        hid_set_feature_report(HID_SENSOR_PRS, (UINT8*)&stReport, FEATURE_SIZE_PRS);    
        #endif // } __ENABLE_HID_PRESSURE__
           
        #if __ENABLE_HID_HUMIDITY__ // {
        hid_get_feature_report(HID_SENSOR_HYG, (UINT8*)&stReport);
        stReport.HYGFeature.u32ReportInterval = gstHidConfig[u8Idx].Hyg.u32ReportInterval;
        stReport.HYGFeature.u16HygChangeSensitivity = gstHidConfig[u8Idx].Hyg.u16HygChangeSensitivity;
        stReport.HYGFeature.u8ReportingState = gstHidConfig[u8Idx].Hyg.u8ReportingState;
        stReport.HYGFeature.u8PowerState = gstHidConfig[u8Idx].Hyg.u8PowerState;
        hid_set_feature_report(HID_SENSOR_HYG, (UINT8*)&stReport, FEATURE_SIZE_HYG);    
        #endif // } __ENABLE_HID_HUMIDITY__
           
        #if __ENABLE_HID_TEMPERATURE__// {
        hid_get_feature_report(HID_SENSOR_TEMP, (UINT8*)&stReport);
        stReport.TEMPFeature.u32ReportInterval = gstHidConfig[u8Idx].Temp.u32ReportInterval;
        stReport.TEMPFeature.u16TempChangeSensitivity = gstHidConfig[u8Idx].Temp.u16TempChangeSensitivity;
        stReport.TEMPFeature.u8ReportingState = gstHidConfig[u8Idx].Temp.u8ReportingState;
        stReport.TEMPFeature.u8PowerState = gstHidConfig[u8Idx].Temp.u8PowerState;
        hid_set_feature_report(HID_SENSOR_TEMP, (UINT8*)&stReport, FEATURE_SIZE_TEMP);        
        #endif // } __ENABLE_HID_TEMPERATURE__
           
        #if __ENABLE_HID_PROXIMITY__ // {
        hid_get_feature_report(HID_SENSOR_PROX, (UINT8*)&stReport);
        stReport.PROXFeature.u32ReportInterval = gstHidConfig[u8Idx].Prox.u32ReportInterval;
        stReport.PROXFeature.u16ProximityChangeSensitivity = gstHidConfig[u8Idx].Prox.u16ProximityChangeSensitivity;
        stReport.PROXFeature.u8ReportingState = gstHidConfig[u8Idx].Prox.u8ReportingState;
        stReport.PROXFeature.u8PowerState = gstHidConfig[u8Idx].Prox.u8PowerState;    
        hid_set_feature_report(HID_SENSOR_PROX, (UINT8*)&stReport, FEATURE_SIZE_PROX);        
        #endif // } __ENABLE_HID_PROXIMITIY__
           
        #if __ENABLE_HID_UV__ // {
        hid_get_feature_report(HID_SENSOR_UV, (UINT8*)&stReport);
        stReport.UVFeature.u32ReportInterval = gstHidConfig[u8Idx].UV.u32ReportInterval;
        stReport.UVFeature.u16UVSensitivity = gstHidConfig[u8Idx].UV.u16UVSensitivity;
        stReport.UVFeature.u8ReportingState = gstHidConfig[u8Idx].UV.u8ReportingState;
        stReport.UVFeature.u8PowerState = gstHidConfig[u8Idx].UV.u8PowerState;      
        hid_set_feature_report(HID_SENSOR_UV, (UINT8*)&stReport, FEATURE_SIZE_UV);        
        #endif // } __ENABLE_HID_UV__
           
        #if __ENABLE_HID_ANDROID__ // {
        hid_get_feature_report(HID_SENSOR_GRA, (UINT8*)&stReport);
        stReport.GRAFeature.u32ReportInterval = gstHidConfig[u8Idx].Gra.u32ReportInterval;
        stReport.GRAFeature.u16GraChangeSensitivity = gstHidConfig[u8Idx].Gra.u16GraChangeSensitivity;
        stReport.GRAFeature.u8ReportingState = gstHidConfig[u8Idx].Gra.u8ReportingState;
        stReport.GRAFeature.u8PowerState = gstHidConfig[u8Idx].Gra.u8PowerState; 
        hid_set_feature_report(HID_SENSOR_GRA, (UINT8*)&stReport, FEATURE_SIZE_GRAVITY);        

        hid_get_feature_report(HID_SENSOR_LACC, (UINT8*)&stReport);
        stReport.LACCFeature.u32ReportInterval = gstHidConfig[u8Idx].Lacc.u32ReportInterval;
        stReport.LACCFeature.u16LAccChangeSensitivity = gstHidConfig[u8Idx].Lacc.u16LAccChangeSensitivity;
        stReport.LACCFeature.u8ReportingState = gstHidConfig[u8Idx].Lacc.u8ReportingState;
        stReport.LACCFeature.u8PowerState = gstHidConfig[u8Idx].Lacc.u8PowerState;     
        hid_set_feature_report(HID_SENSOR_LACC, (UINT8*)&stReport, FEATURE_SIZE_LINEAR_ACC);        
        #endif // } __ENABLE_HID_ANDROID__
    }
    
}
#endif // } (DEF_DUAL_OS && DEF_OS_HOT_SWITCH)

//********************************************************************************
//  main
//********************************************************************************
UINT8 HID_CODE_L hid_init(void)
{
    UINT8  i; 

    switch(HID_INTERFACE_INIT_STEP)
    {
        case HID_INTERFACE_INIT_STEP_START:
            // initial hid power state    
            #if (HID_SENSOR_DEFAULT_POWER_STATE == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM)
            *(LPUINT16)&gstHIDPowerState = 0x0000; // off        
            #else
            *(LPUINT16)&gstHIDPowerState = 0xFFFE; // on -> bit0 must always 0       
            #endif

            // initial hid feature parameter
            for(i=0; i<TOTAL_HID_DEVICE; i++)
            {
                gstHIDFeature[i].Event = HID_SENSOR_DEFAULT_EVENT;                   
                gstHIDFeature[i].SensorState = HID_SENSOR_DEFAULT_STATE;             
                gstHIDFeature[i].ReportingState = HID_SENSOR_DEFAULT_REPORTING_STATE;
                gstHIDFeature[i].PowerState = HID_SENSOR_DEFAULT_POWER_STATE;         
            }

            // initial hid timer parameter
            for(i=1; i<(TOTAL_HID_DEVICE+1); i++)
            {
                gu8HIDINTC[i]      =  0;    
                giHIDLastTimer[i]  = -1;
            }
            #if (defined(EC_FEATURE_ONLY) && defined(__ENABLE_HID_I2C_API__))
            HID_INT_Event_Reset();
            #endif

            hid_init_time_interval();

            RESET_AUTO_ROTATE_EVENT();

            HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_HID_IF_INIT;
            break;

        case HID_INTERFACE_INIT_STEP_HID_IF_INIT:
            // initial OS hot switch
            #if (DEF_DUAL_OS && DEF_OS_HOT_SWITCH) // {
            hid_init_os_hot_switch();
            #endif // } (DEF_DUAL_OS && DEF_OS_HOT_SWITCH)

            // initial I2C or USB interface
            if(DEF_I2C_HID)
            {
                #ifdef __ENABLE_HID_I2C_API__ // {
                SET_MASK_EXT(CGCTRL3R, BIT5); // Gating USB CLK
                
                i2c_hid_init();
                gu8HIDStatus = HID_STATUS_RESET;
                HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_HID_IF_READY;
                #endif // } __ENABLE_HID_I2C_API__
            }
            else if(DEF_USB_HID)
            {
                #ifdef __ENABLE_HID_USB_API__ // {
                switch(USB_SLAVE_INIT_STEP)
                {
                    case USB_SLAVE_INIT_STEP_START:
                        usb_slave_init();
                        //gu8HIDStatus = HID_STATUS_RESET;
                        break;

                    case USB_SLAVE_INIT_STEP_DETECT_SPEED:
                        usb_slave_ip_init(1);
                        do_usb_slave_hid_init();
                        break;

                    case USB_SLAVE_INIT_STEP_READY:
                        gu8HIDStatus = HID_STATUS_RESET;
                        HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_HID_IF_READY;
                        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_IDLE;
                        break;

                    case USB_SLAVE_INIT_STEP_IDLE:

                        break;

                    default :
                        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_START;
                        break;
                }
                #endif // } __ENABLE_HID_USB_API__
            }
            else
            {
                HID_CREATE_MUTEX();
                gu8HIDStatus = HID_STATUS_INIT;
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
                HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_HID_IF_IDLE;
            }
            break;

        case HID_INTERFACE_INIT_STEP_HID_IF_READY:
            HID_INTERFACE_STEP = HID_INTERFACE_STEP_SERVICE;
            HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_HID_IF_IDLE;
            
            #if 0 // {
            printk("Desc: %d\r\n", HID_REPORT_DES_TOTAL_LENGTH);
            printk("Max Feature Report: %d\r\n", sizeof(TFRInfo));
            printk("Max Input Report: %d\r\n", sizeof(TIRInfo)); 
            printk("Acc: %d %d\r\n", FEATURE_SIZE_ACC_3D, INPUT_SIZE_ACC_3D);
            printk("Gyo: %d %d\r\n", FEATURE_SIZE_GYR_3D, INPUT_SIZE_GYR_3D);
            printk("Mag: %d %d\r\n", FEATURE_SIZE_COM_3D, INPUT_SIZE_COM_3D);
            printk("Inc: %d %d\r\n", FEATURE_SIZE_INC_3D, INPUT_SIZE_INC_3D);
            printk("Ori: %d %d\r\n", FEATURE_SIZE_ORIENTATION, INPUT_SIZE_ORIENTATION);
            printk("Als: %d %d\r\n", FEATURE_SIZE_AMBIENTLIGHT, INPUT_SIZE_AMBIENTLIGHT);
            printk("Cus: %d %d\r\n", FEATURE_SIZE_CUSTOM, INPUT_SIZE_CUSTOM);
            #endif // }
            break;

        case HID_INTERFACE_INIT_STEP_HID_IF_IDLE:

            break;

        default:
            HID_INTERFACE_INIT_STEP = HID_INTERFACE_INIT_STEP_START;
            break;

    }
    return gu8HIDStatus;
}

//********************************************************************************
void HID_CODE hid_service(void)
{
    // check for OS hot switch
    #if (DEF_DUAL_OS && DEF_OS_HOT_SWITCH) // {
    hid_check_os_hot_switch();
    #endif // } (DEF_DUAL_OS && DEF_OS_HOT_SWITCH)

    // do hid service
    if(DEF_I2C_HID)
    {
        #ifdef __ENABLE_HID_I2C_API__ // {
    	i2c_hid_main();
        #endif // } __ENABLE_HID_I2C_API__
    }
    else if(DEF_USB_HID)
    {
        #ifdef __ENABLE_HID_USB_API__ // {
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        IER2 &= ~BIT7;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        #endif  // EC_FEATURE_ONLY_SUPPORT_FUSION
        
    	usb_slave_main();

        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        IER2 |= BIT7;
        #endif  // EC_FEATURE_ONLY_SUPPORT_FUSION
        #endif // } __ENABLE_HID_USB_API__
    }
    else
    {

    }
}

//********************************************************************************
#if __ENABLE_HID_ANDROID__ // {
static inline void HID_CODE hid_android_data_set(UINT8 *pBuf)
{
    P_SENSOR_BUFFER_ANDROID pAndroidDC = (P_SENSOR_BUFFER_ANDROID)pBuf;
    
    #if __ENABLE_HID_ACC__ // {
    if(gstHIDPowerState.Acc)
    {
        AccelerometerX = pAndroidDC->AccMeter[0];
        AccelerometerY = pAndroidDC->AccMeter[1];
        AccelerometerZ = pAndroidDC->AccMeter[2];
    }
    #endif // } __ENABLE_HID_ACC__
    
    #if __ENABLE_HID_MAG__ // {
    if(gstHIDPowerState.Mag)
    {
        MagnetometerX = pAndroidDC->MagCal[0];
        MagnetometerY = pAndroidDC->MagCal[1];
        MagnetometerZ = pAndroidDC->MagCal[2];
    } 
    #endif // } __ENABLE_HID_MAG__
    
    #if __ENABLE_HID_GYO__ // {
    if(gstHIDPowerState.Gyo)
    {
        GyrometerX = pAndroidDC->Gyo[0];
        GyrometerY = pAndroidDC->Gyo[1];
        GyrometerZ = pAndroidDC->Gyo[2];
    }
    #endif // } __ENABLE_HID_GYO__
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    if(gstHIDPowerState.Inc 
        || (gstHIDPowerState.Acc && gstHIDPowerState.Mag && gstHIDPowerState.Gyo))
    {
        InclinometerX = pAndroidDC->Eular[0];
        InclinometerY = pAndroidDC->Eular[1];
        InclinometerZ = pAndroidDC->Eular[2];
    }

    if(gstHIDPowerState.Ori
        || (gstHIDPowerState.Acc && gstHIDPowerState.Mag && gstHIDPowerState.Gyo))
    {
        OrientationQuaternionW = pAndroidDC->Quat[0];
        OrientationQuaternionX = pAndroidDC->Quat[1];
        OrientationQuaternionY = pAndroidDC->Quat[2];
        OrientationQuaternionZ = pAndroidDC->Quat[3];
    }  
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    if(gstHIDPowerState.Gra)
    {
        GravityX = pAndroidDC->Acc[0];
        GravityY = pAndroidDC->Acc[1];
        GravityZ = pAndroidDC->Acc[2];
    }

    if(gstHIDPowerState.Lacc)
    {
        LinearAccX = pAndroidDC->LinAcc[0];
        LinearAccY = pAndroidDC->LinAcc[1];
        LinearAccZ = pAndroidDC->LinAcc[2];
    }
}
#endif // } __ENABLE_HID_ANDROID__

#if __ENABLE_HID_WIN__ // {
static inline void HID_CODE hid_win_data_set(UINT8 *pBuf)
{
    #if (__ENABLE_HID_ACC__ || __ENABLE_HID_MAG__ || __ENABLE_HID_GYO__) // {
    P_SENSOR_BUFFER_WIN pWinDC = (P_SENSOR_BUFFER_WIN)pBuf;
    #endif // } 

    #if __ENABLE_HID_ACC__ // {
    if(gstHIDPowerState.Acc)
    {
        AccelerometerX = pWinDC->Acc[0];
        AccelerometerY = pWinDC->Acc[1];
        AccelerometerZ = pWinDC->Acc[2];
        Shake = pWinDC->Shake;
    }
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_MAG__ // {
    if(gstHIDPowerState.Mag)
    {
        MagnetometerX = pWinDC->Mag[0];
        MagnetometerY = pWinDC->Mag[1];
        MagnetometerZ = pWinDC->Mag[2];
        NorthDegrees = pWinDC->Heading;
    }
    #endif // } __ENABLE_HID_MAG__

    #if __ENABLE_HID_GYO__ // {
    if(gstHIDPowerState.Gyo)
    {
        GyrometerX = pWinDC->Gyo[0];
        GyrometerY = pWinDC->Gyo[1];
        GyrometerZ = pWinDC->Gyo[2];

        #ifdef __PSEUDO_GYRO_DBG__ // {
        GyrometerExt[0] = *(UINT16*)(pWinDC->Alt + 10);
        GyrometerExt[1] = *(UINT16*)(pWinDC->Alt + 12);
        GyrometerExt[2] = *(UINT16*)(pWinDC->Alt + 14);
        #endif // } __PSEUDO_GYRO_DBG__
    }
    #endif // } __ENABLE_HID_GYO__

    #if __ENABLE_HID_FUSION_SENSOR__ // {
    if(gstHIDPowerState.Inc 
        || (gstHIDPowerState.Acc && gstHIDPowerState.Mag && gstHIDPowerState.Gyo))
    {
        InclinometerX = pWinDC->Eular[0];
        InclinometerY = pWinDC->Eular[1];
        InclinometerZ = pWinDC->Eular[2];
    }

    if(gstHIDPowerState.Ori
        || (gstHIDPowerState.Acc && gstHIDPowerState.Mag && gstHIDPowerState.Gyo))
    {
        OrientationQuaternionW = pWinDC->Quat[0];
        OrientationQuaternionX = pWinDC->Quat[1];
        OrientationQuaternionY = pWinDC->Quat[2];
        OrientationQuaternionZ = pWinDC->Quat[3];

        #if __ENABLE_HID_ROTATION_MATRIX__ // {
        memcpy(RotationMatrix, pWinDC->RMatrix, sizeof(RotationMatrix));
        #endif // }  __ENABLE_HID_ROTATION_MATRIX__
    }
    #endif // } __ENABLE_HID_FUSION_SENSOR__

}
#endif // } __ENABLE_HID_WIN__

void HID_CODE hid_data_set(UINT8 *pBuf)
{
    HID_DATA_SET_IN();
    
    #if DEF_DUAL_OS // {
    if(gstFConfig.Enable_Android)
    #endif // } DEF_DUAL_OS    
    {
        #if __ENABLE_HID_ANDROID__ // {
        hid_android_data_set(pBuf);
        #endif // } __ENABLE_HID_ANDROID__
    }
    #if DEF_DUAL_OS // {
    else
    #endif // } DEF_DUAL_OS
    {
        #if __ENABLE_HID_WIN__ // {
        hid_win_data_set(pBuf);
        #endif // } __ENABLE_HID_WIN__
    }

    HID_DATA_SET_OUT();
}

void HID_CODE hid_update_last_data(void)
{
    /* Acc */
    #if __ENABLE_HID_ACC__ // {
    HID_REPORT_UNIT_ACC(gpCurVal[0], AccelerometerX);
    HID_REPORT_UNIT_ACC(gpCurVal[1], AccelerometerY);
    HID_REPORT_UNIT_ACC(gpCurVal[2], AccelerometerZ);
    HID_REPORT_UNIT_SHAKE(gpCurVal[3], Shake);

    LastAcc_X = (UINT16)gpCurVal[0];
    LastAcc_Y = (UINT16)gpCurVal[1];
    LastAcc_Z = (UINT16)gpCurVal[2];
    LastShake = (UINT16)gpCurVal[3];
    #endif // } __ENABLE_HID_ACC__

    /* Gyo */
    #if __ENABLE_HID_GYO__ // {
    HID_REPORT_UNIT_GYR(LastGyr_X, GyrometerX);
    HID_REPORT_UNIT_GYR(LastGyr_Y, GyrometerY);
    HID_REPORT_UNIT_GYR(LastGyr_Z, GyrometerZ);
    #endif // } __ENABLE_HID_GYO__

    /* Com */
    #if __ENABLE_HID_MAG__ // {
    HID_REPROT_UNIT_NORTH(LastNorth, NorthDegrees);
    HID_REPORT_UNIT_MAG(LastMag_X, MagnetometerX);
    HID_REPORT_UNIT_MAG(LastMag_Y, MagnetometerY);
    HID_REPORT_UNIT_MAG(LastMag_Z, MagnetometerZ);
    #endif // } __ENABLE_HID_MAG__

    /* Inc */
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    HID_REPORT_UNIT_INC(gpCurVal[0], InclinometerX);
    HID_REPORT_UNIT_INC(gpCurVal[1], InclinometerY);
    HID_REPORT_UNIT_INC(gpCurVal[2], InclinometerZ);

    LastTil_X = gpCurVal[0];
    LastTil_Y = gpCurVal[1];
    LastTil_Z = gpCurVal[2];
    #endif // } __ENABLE_HID_FUSION_SENSOR__

    /* Ori */
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    HID_REPORT_UNIT_ORI(LastQua_X, OrientationQuaternionX);
    HID_REPORT_UNIT_ORI(LastQua_Y, OrientationQuaternionY);
    HID_REPORT_UNIT_ORI(LastQua_Z, OrientationQuaternionZ);
    HID_REPORT_UNIT_ORI(LastQua_W, OrientationQuaternionW);
    #endif // } __ENABLE_HID_FUSION_SENSOR__

    /* ALS */
    #if __ENABLE_HID_ALS__ // {
    HID_REPORT_UNIT_ALS(LastLux, LightIlluminace);
    #endif // } __ENABLE_HID_ALS__

    /* Custom */
    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    HID_REPORT_UNIT_ANGLE(LastCustomAngle, CustomAngle[2]);
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__

    /* Pressure */
    #if __ENABLE_HID_PRESSURE__ // {
    HID_REPORT_UNIT_PRS(LastPrs, PressureBar);
    #endif // } __ENABLE_HID_PRESSURE__

    /* Humidity */
    #if __ENABLE_HID_HUMIDITY__ // {
    HID_REPORT_UNIT_HYG(LastHyg, HygValue);
    #endif // } __ENABLE_HID_HUMIDITY__

    /* Temperature */
    #if __ENABLE_HID_TEMPERATURE__ // {
    HID_REPORT_UNIT_TEMP(LastTemp, TempValue);
    #endif // } __ENABLE_HID_TEMPERATURE__

    /* Proximity */
    #if __ENABLE_HID_PROXIMITY__ // {
    HID_REPORT_UNIT_PROX(LastProx, ProxValue);
    #endif // } __ENABLE_HID_PROXIMITY__

    /* UV */
    #if __ENABLE_HID_UV__ // {
    HID_REPORT_UNIT_UV(LastUV, UVValue);
    #endif // } __ENABLE_HID_UV__
}

void HID_CODE_L hid_update_sensor_state(UINT8 u8ID, UINT8 u8State)
{
    #if __ENABLE_HID_SENSOR_STATE__ // {
    if((u8ID > HID_SENSOR_DEFAULT) && (u8ID < HID_SENSOR_MAX))
    {
        u8ID = HID_SENSOR_IDX(u8ID);
        
        if(gstHIDFeature[u8ID].SensorState != u8State)
        {                                     
            gstHIDFeature[u8ID].SensorState = u8State;
            gstHIDFeature[u8ID].Event = HID_USAGE_SENSOR_EVENT_STATE_CHANGED_ENUM;
        }   
    }
    else if(u8ID == HID_SENSOR_DEFAULT)
    {
        for(u8ID=0; u8ID<HID_SENSOR_MAX; u8ID++)
        {
            if(gstHIDFeature[u8ID].SensorState != u8State)
            {                                     
                gstHIDFeature[u8ID].SensorState = u8State;
                gstHIDFeature[u8ID].Event = HID_USAGE_SENSOR_EVENT_STATE_CHANGED_ENUM;
            }   
        }
    }
    #endif // } __ENABLE_HID_SENSOR_STATE__
}

void HID_CODE_L hid_reset_sensor_state(void)
{
    #if __ENABLE_HID_SENSOR_STATE__ // {
    int i;
    
    for(i=0; i<HID_SENSOR_MAX; i++)
    {
        if(gstHIDFeature[i].SensorState != HID_SENSOR_DEFAULT_STATE)
        {                                     
            gstHIDFeature[i].SensorState = HID_SENSOR_DEFAULT_STATE;
            gstHIDFeature[i].Event = HID_USAGE_SENSOR_EVENT_STATE_CHANGED_ENUM;
        }   
    }
    #endif // } __ENABLE_HID_SENSOR_STATE__
}

void HID_CODE hid_tiny_init(void)
{
    TFRInfo stReport;

    /* Acc */
    #if __ENABLE_HID_ACC__ // {
    hid_get_feature_report(HID_SENSOR_ACC, (UINT8*)&stReport);
    stReport.ACC3DFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.ACC3DFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.ACC3DFeature.u32ReportInterval = FUSION_PERIOD;
    hid_set_feature_report(HID_SENSOR_ACC, (UINT8*)&stReport, FEATURE_SIZE_ACC_3D);   
    #endif // } __ENABLE_HID_ACC__

    /* Gyo */
    #if __ENABLE_HID_GYO__ // {
    hid_get_feature_report(HID_SENSOR_GYR, (UINT8*)&stReport);
    stReport.GYR3DFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.GYR3DFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.GYR3DFeature.u32ReportInterval = FUSION_PERIOD;
    hid_set_feature_report(HID_SENSOR_GYR, (UINT8*)&stReport, FEATURE_SIZE_GYR_3D);    
    #endif // } __ENABLE_HID_GYO__

    /* Com */
    #if __ENABLE_HID_MAG__ // {
    hid_get_feature_report(HID_SENSOR_MAG, (UINT8*)&stReport);
    stReport.COM3DFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.COM3DFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.COM3DFeature.u32ReportInterval = FUSION_PERIOD;
    hid_set_feature_report(HID_SENSOR_MAG, (UINT8*)&stReport, FEATURE_SIZE_COM_3D);
    #endif // } __ENABLE_HID_MAG__

    /* ALS */
    #if __ENABLE_HID_ALS__ // {
    hid_get_feature_report(HID_SENSOR_ALS, (UINT8*)&stReport);
    stReport.ALSFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.ALSFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.ALSFeature.u32ReportInterval = 100;
    hid_set_feature_report(HID_SENSOR_ALS, (UINT8*)&stReport, FEATURE_SIZE_AMBIENTLIGHT);
    #endif // } __ENABLE_HID_ALS__
    
    /* Custom */
    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    hid_get_feature_report(HID_SENSOR_CUSTOM, (UINT8*)&stReport);
    stReport.CUSTOMFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.CUSTOMFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.CUSTOMFeature.u32ReportInterval = 100;  
    hid_set_feature_report(HID_SENSOR_CUSTOM, (UINT8*)&stReport, FEATURE_SIZE_CUSTOM);
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__

    /* Prs */
    #if __ENABLE_HID_PRESSURE__ // {
    hid_get_feature_report(HID_SENSOR_PRS, (UINT8*)&stReport);
    stReport.PRSFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.PRSFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.PRSFeature.u32ReportInterval = 1000;
    hid_set_feature_report(HID_SENSOR_PRS, (UINT8*)&stReport, FEATURE_SIZE_PRS);
    #endif // } __ENABLE_HID_PRESSURE__

    /* Hyg */
    #if __ENABLE_HID_HUMIDITY__ // {
    hid_get_feature_report(HID_SENSOR_HYG, (UINT8*)&stReport);
    stReport.HYGFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.HYGFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.HYGFeature.u32ReportInterval = 1000;
    hid_set_feature_report(HID_SENSOR_HYG, (UINT8*)&stReport, FEATURE_SIZE_HYG);
    #endif // } __ENABLE_HID_HUMIDITY__

    /* Temp */
    #if __ENABLE_HID_TEMPERATURE__ // {
    hid_get_feature_report(HID_SENSOR_TEMP, (UINT8*)&stReport);
    stReport.TEMPFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.TEMPFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.TEMPFeature.u32ReportInterval = 1000;
    hid_set_feature_report(HID_SENSOR_TEMP, (UINT8*)&stReport, FEATURE_SIZE_TEMP);
    #endif // } __ENABLE_HID_TEMPERATURE__    

    /* Prox */
    #if __ENABLE_HID_PROXIMITY__ // {
    hid_get_feature_report(HID_SENSOR_PROX, (UINT8*)&stReport);
    stReport.PROXFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.PROXFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.PROXFeature.u32ReportInterval = 1000;
    hid_set_feature_report(HID_SENSOR_PROX, (UINT8*)&stReport, FEATURE_SIZE_PROX);
    #endif // } __ENABLE_HID_PROXIMITY__    

    /* UV */
    #if __ENABLE_HID_UV__ // {
    hid_get_feature_report(HID_SENSOR_UV, (UINT8*)&stReport);
    stReport.UVFeature.u8PowerState = HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM;
    stReport.UVFeature.u8ReportingState = HID_USAGE_REPORTING_STATE_ON_ALL_ENUM;
    stReport.UVFeature.u32ReportInterval = 1000;
    hid_set_feature_report(HID_SENSOR_UV, (UINT8*)&stReport, FEATURE_SIZE_UV);
    #endif // } __ENABLE_HID_UV__    
}

//********************************************************************************
void HID_CODE hid_set_power_mode(BOOL bFusionAllOn)
{
    T_SENSOR_POWER_CONFIG stPower = {0};
        
    if(bFusionAllOn || hid_check_power_mode())
    {
        #if __ENABLE_HID_ACC__ // {
        stPower.Acc = 1;
        #endif // } __ENABLE_HID_ACC__
        
        #if __ENABLE_HID_MAG__ // {
        stPower.Mag = 1;
        #endif // } __ENABLE_HID_MAG__
        
        #if __ENABLE_HID_GYO__ // {
        stPower.Gyo = 1;
        #endif // } __ENABLE_HID_GYO__
    }
    else
    {
        #if __ENABLE_HID_ACC__ // {
        stPower.Acc = gstHIDPowerState.Acc;
        #endif // } __ENABLE_HID_ACC__
        
        #if __ENABLE_HID_MAG__ // {
        stPower.Mag = gstHIDPowerState.Mag;
        #endif // } __ENABLE_HID_MAG__
        
        #if __ENABLE_HID_GYO__ // {
        stPower.Gyo = gstHIDPowerState.Gyo;
        #endif // } __ENABLE_HID_GYO__
    }

    #if __ENABLE_HID_ALS__ // {
    stPower.Als = gstHIDPowerState.Als;
    #endif // } __ENABLE_HID_ALS__

    #if __ENABLE_HID_CUSTOM_SENSOR__// {
    stPower.Acc2 = gstHIDPowerState.Custom;
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__    

    #if __ENABLE_HID_PRESSURE__ // {
    stPower.Prs = gstHIDPowerState.Prs;
    #endif // } __ENABLE_HID_PRESSURE__    

    #if __ENABLE_HID_HUMIDITY__ // {
    stPower.Hyg = gstHIDPowerState.Hyg;
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__ // {
    stPower.Temp = gstHIDPowerState.Temp;
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    stPower.Prox = gstHIDPowerState.Prox;
    #endif // } __ENABLE_HID_PROXIMITY__

    #if __ENABLE_HID_UV__ // {
    stPower.UV = gstHIDPowerState.UV;
    #endif // } __ENABLE_HID_UV__

    sensor_power_mode_switch(stPower);
}

UINT8 HID_CODE hid_check_power_mode(void)
{
    #if __ENABLE_HID_MAG__ // {
    if((gstHIDPowerState.Mag == 1) 

        #if __ENABLE_HID_FUSION_SENSOR__ // {
        || (gstHIDPowerState.Inc == 1) || (gstHIDPowerState.Ori == 1)
        #endif // } __ENABLE_HID_FUSION_SENSOR__

        #if __ENABLE_HID_GYO__ // {
        || ((gstFConfig.Enable_6D == 1) && (gstHIDPowerState.Gyo == 1))
        #endif // } __ENABLE_HID_GYO__
       )
    {
        return 1;    
    }    
    else 
    #endif // } __ENABLE_HID_MAG__
    {
        return 0;    
    }
}

void HID_CODE hid_change_power_mode(void)
{
    if(hid_check_power_mode())
    {                       
    }
    else
    { 
        hid_set_power_mode(FALSE);
    }
}

/*-------------------------------------------------------------------------
    Accelerometer 
-------------------------------------------------------------------------*/
#if __ENABLE_HID_ACC__ // {
void HID_CODE _s_i_acc(UINT32 u32ReportInterval)
{
    ACC_3D_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//100;

    HID_DBG_PERIOD("A: %d\r\n", ACC_3D_Feature_ReportInterval);
}

int HID_CODE _g_f_acc(UINT8 *pBuf)
{
    PACCEL3_FEATURE_REPORT ACC3DFeature = (PACCEL3_FEATURE_REPORT)pBuf;
    
    ACC3DFeature->u8ReportId = HID_SENSOR_ACC;
    
    ACC3DFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    ACC3DFeature->u8ReportingState = ACC_3D_Feature.ReportingState;
    ACC3DFeature->u8PowerState = ACC_3D_Feature.PowerState;
    ACC3DFeature->u8SensorState = GET_SENSOR_STATE(ACC_3D_Feature);
//	    ACC3DFeature->u8SensorStatus = gu8HIDStatus;
    ACC3DFeature->u32ReportInterval = ACC_3D_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
    ACC3DFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_ACC;
    #endif
    
    ACC3DFeature->u16AccelResolution = 2;
    ACC3DFeature->u16AccelAccuracy = 2;
    ACC3DFeature->u16AccelChangeSensitivity = ACC_3D_Feature_ChangeSensitivityABS;
    ACC3DFeature->s16AccelMaximum = 4000;
    ACC3DFeature->s16AccelMinimum = -4000;

    return FEATURE_SIZE_ACC_3D;
}

void HID_CODE _s_f_acc(UINT8 *pBuf, UINT16 u16BufSize)
{
    PACCEL3_FEATURE_REPORT ACC3DFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_ACC_3D))
    {
        ACC3DFeature = (PACCEL3_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(ACC_3D_Feature, ACC3DFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(ACC3DFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(ACC_3D_Feature, ACC3DFeature->u8ReportingState);
                SET_EVENT(ACC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_ACC << 4) | (ACC3DFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(ACC_3D_Feature, ACC3DFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(ACC3DFeature->u8PowerState))
            {
                SET_POWER_STATE(ACC_3D_Feature, ACC3DFeature->u8PowerState);
                SET_EVENT(ACC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("A: %d, %d\r\n", ACC_3D_Feature.PowerState, ACC_3D_Feature.ReportingState);
                
                if(ACC_3D_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(ACC_3D_Feature))
                    {
                        if(gstHIDPowerState.Acc != 1)
                        {                    
                            gstHIDPowerState.Acc = 1;    
                            hid_change_power_mode();
                        }

                        SET_SENSOR_STATE(ACC_3D_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(ACC_3D_Feature.PowerState == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM)
                        {
                            RESET_AUTO_ROTATE_EVENT();
                        }
                        
                        if(gstHIDPowerState.Acc != 0)
                        {                    
                            gstHIDPowerState.Acc = 0;    
                            hid_change_power_mode();
                        }

                        SET_SENSOR_STATE(ACC_3D_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);

                        sensor_power_off_data_save();
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_ACC << 4) | (ACC3DFeature->u8PowerState));
            }
        }

        // set report interval
        if(ACC_3D_Feature_ReportInterval != ACC3DFeature->u32ReportInterval)
        {
            SET_INTERVAL(acc, ACC3DFeature->u32ReportInterval);
            
            SET_EVENT(ACC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            
            sensor_data_rate_switch(HID_SENSOR_ACC, ACC_3D_Feature_ReportInterval);                    
        }

        // set sensitivity
        if(ACC_3D_Feature_ChangeSensitivityABS != ACC3DFeature->u16AccelChangeSensitivity)
        {
            ACC_3D_Feature_ChangeSensitivityABS = ACC3DFeature->u16AccelChangeSensitivity;
            SET_EVENT(ACC_3D_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastAcc_X = LastAcc_Y = LastAcc_Z = 0xFFFF;
    }
}

BOOL HID_CODE _c_s_acc(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_ACC(gpCurVal[0], AccelerometerX);
    HID_REPORT_UNIT_ACC(gpCurVal[1], AccelerometerY);
    HID_REPORT_UNIT_ACC(gpCurVal[2], AccelerometerZ);

//	HID_REPORT_UNIT_ACC(gpCurVal[0], 0);
//	HID_REPORT_UNIT_ACC(gpCurVal[1], 1000);
//	HID_REPORT_UNIT_ACC(gpCurVal[2], 0);

    HID_REPORT_UNIT_SHAKE(gpCurVal[3], Shake);

    if(ACC_3D_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(ACC_3D_Feature.ReportingState != HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
        {
            if(!((LastAcc_X == 0xFFFF) && (LastAcc_Y == 0xFFFF) && (LastAcc_Z == 0xFFFF)))
            {
                #ifdef __ENABLE_HID_FAKE_ROTATE_EVENT__ // {
                if(gu8AutoRotateEvent == 0) 
                {
                #endif // } __ENABLE_HID_FAKE_ROTATE_EVENT__
                
                    if((abs((short)LastAcc_X - (short)gpCurVal[0]) < (short)ACC_3D_Feature_ChangeSensitivityABS) &&
                       (abs((short)LastAcc_Y - (short)gpCurVal[1]) < (short)ACC_3D_Feature_ChangeSensitivityABS) &&
                       (abs((short)LastAcc_Z - (short)gpCurVal[2]) < (short)ACC_3D_Feature_ChangeSensitivityABS) &&
                       (LastShake == (UINT16)gpCurVal[3]))
                    {
                        bRet = FALSE;
                    }
                    
                #ifdef __ENABLE_HID_FAKE_ROTATE_EVENT__ // {
                }
                else
                {
                    gu8AutoRotateEvent--;  
                    gpCurVal[0] = 0;
                    gpCurVal[1] = 0;
                    gpCurVal[2] = 1000;
                }
                #endif // } __ENABLE_HID_FAKE_ROTATE_EVENT__
            }
        }
        else
        {
            bRet = FALSE;
        }

        if(ACC_3D_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_acc(UINT8 *pBuf)
{
    PACCEL3_INPUT_REPORT ACC3DInput = (PACCEL3_INPUT_REPORT)pBuf;

    HID_ACC_INPUT_IN();

    ACC3DInput->u8ReportId = HID_SENSOR_ACC;
    
    ACC3DInput->u8SensorState = GET_SENSOR_STATE(ACC_3D_Feature);
    ACC3DInput->u8EventType = ACC_3D_Feature.Event;
    ACC3DInput->s16AccelXValue = (SINT16)gpCurVal[0];
    ACC3DInput->s16AccelYValue = (SINT16)gpCurVal[1];
    ACC3DInput->s16AccelZValue = (SINT16)gpCurVal[2];
    ACC3DInput->u8ShakeDetectState = (UINT8)gpCurVal[3];

    LastAcc_X = (UINT16)gpCurVal[0];
    LastAcc_Y = (UINT16)gpCurVal[1];
    LastAcc_Z = (UINT16)gpCurVal[2];
    LastShake = (UINT16)gpCurVal[3];
    CLEAR_EVENT(ACC_3D_Feature);

    //HID_DBG_MSG("A: [%d]%d %d %d\r\n", gu8AutoRotateEvent, gpCurVal[0], gpCurVal[1], gpCurVal[2]);
    HID_ACC_INPUT_OUT();

    return INPUT_SIZE_ACC_3D;
}
#endif // } __ENABLE_HID_ACC__

/*-------------------------------------------------------------------------
    Gyrometer
-------------------------------------------------------------------------*/
#if __ENABLE_HID_GYO__ // {
void HID_CODE _s_i_gyo(UINT32 u32ReportInterval)
{
    GYR_3D_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//50;

    HID_DBG_PERIOD("G: %d\r\n", GYR_3D_Feature_ReportInterval);
}

int HID_CODE _g_f_gyo(UINT8 *pBuf)
{
    PGYRO3_FEATURE_REPORT GYR3DFeature = (PGYRO3_FEATURE_REPORT)pBuf;

    GYR3DFeature->u8ReportId = HID_SENSOR_GYR;

    GYR3DFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    GYR3DFeature->u8ReportingState = GYR_3D_Feature.ReportingState;
    GYR3DFeature->u8PowerState = GYR_3D_Feature.PowerState;
    GYR3DFeature->u8SensorState = GET_SENSOR_STATE(GYR_3D_Feature);
//	    GYR3DFeature->u8SensorStatus = gu8HIDStatus;
    GYR3DFeature->u32ReportInterval = GYR_3D_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
	GYR3DFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_GYR;
    #endif
    
    GYR3DFeature->u32GyroResolution = 7000;
    GYR3DFeature->u32GyroAccuracy = 7500000;
    GYR3DFeature->u32GyroChangeSensitivity = GYR_3D_Feature_ChangeSensitivityABS;            
    GYR3DFeature->s32GyroMaximum = 200000000;
    GYR3DFeature->s32GyroMinimum = -200000000;

    return FEATURE_SIZE_GYR_3D;    
}

void HID_CODE _s_f_gyo(UINT8 *pBuf, UINT16 u16BufSize)
{
    PGYRO3_FEATURE_REPORT GYR3DFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_GYR_3D))
    {
        GYR3DFeature = (PGYRO3_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(GYR_3D_Feature, GYR3DFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(GYR3DFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(GYR_3D_Feature, GYR3DFeature->u8ReportingState);
                SET_EVENT(GYR_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_GYR << 4) | (GYR3DFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(GYR_3D_Feature, GYR3DFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(GYR3DFeature->u8PowerState))
            {
                SET_POWER_STATE(GYR_3D_Feature, GYR3DFeature->u8PowerState);
                SET_EVENT(GYR_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("G: %d, %d\r\n", GYR_3D_Feature.PowerState, GYR_3D_Feature.ReportingState);

                CLEAR_AUTO_ROTATE_EVENT();

                if(GYR_3D_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(GYR_3D_Feature))
                    {                    
                        if(gstFConfig.Enable_6D)
                        {
                            if(hid_check_power_mode() !=  1)
                            {      
                                hid_set_power_mode(TRUE);
                            }
                            gstHIDPowerState.Gyo = 1;                          
                        }
                        else
                        {
                            if(gstHIDPowerState.Gyo != 1)
                            {                    
                                gstHIDPowerState.Gyo = 1;     
                                hid_change_power_mode();               
                            }
                        }

                        SET_SENSOR_STATE(GYR_3D_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstFConfig.Enable_6D)
                        {
                            gstHIDPowerState.Gyo = 0;   
                            if(hid_check_power_mode() == 0)
                            {
                                hid_set_power_mode(FALSE);
                            }
                        }
                        else
                        {
                            if(gstHIDPowerState.Gyo != 0)
                            {                    
                                gstHIDPowerState.Gyo = 0;    
                                hid_change_power_mode();
                            }
                        }
                        
                        SET_SENSOR_STATE(GYR_3D_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_GYR << 4) | (GYR3DFeature->u8PowerState));
            }
        }
        
        // set report interval     
        if(GYR_3D_Feature_ReportInterval != GYR3DFeature->u32ReportInterval)
        {
            SET_INTERVAL(gyo, GYR3DFeature->u32ReportInterval);
            
            SET_EVENT(GYR_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            
            sensor_data_rate_switch(HID_SENSOR_GYR, GYR_3D_Feature_ReportInterval);                    
        }

        // set sensitivity
        if(GYR_3D_Feature_ChangeSensitivityABS != GYR3DFeature->u32GyroChangeSensitivity)
        {
            GYR_3D_Feature_ChangeSensitivityABS = GYR3DFeature->u32GyroChangeSensitivity;
            SET_EVENT(GYR_3D_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastGyr_X = LastGyr_Y = LastGyr_Z = 0xFFFFFFFF;
    }    
}

BOOL HID_CODE _c_s_gyo(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_GYR(gpCurVal[0], GyrometerX);
    HID_REPORT_UNIT_GYR(gpCurVal[1], GyrometerY);
    HID_REPORT_UNIT_GYR(gpCurVal[2], GyrometerZ);

    if(GYR_3D_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(GYR_3D_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;

        if(!((LastGyr_X == 0xFFFFFFFF) && (LastGyr_Y == 0xFFFFFFFF) && (LastGyr_Z == 0xFFFFFFFF)))
        {
            if(abs((long)LastGyr_X - (long)gpCurVal[0]) < (long)GYR_3D_Feature_ChangeSensitivityABS &&
               abs((long)LastGyr_Y - (long)gpCurVal[1]) < (long)GYR_3D_Feature_ChangeSensitivityABS &&
               abs((long)LastGyr_Z - (long)gpCurVal[2]) < (long)GYR_3D_Feature_ChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }

        if(GYR_3D_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_gyo(UINT8 *pBuf)
{
    PGYRO3_INPUT_REPORT GYR3DInput = (PGYRO3_INPUT_REPORT)pBuf;

    HID_GYO_INPUT_IN();

    GYR3DInput->u8ReportId = HID_SENSOR_GYR;  
    
    GYR3DInput->u8SensorState = GET_SENSOR_STATE(GYR_3D_Feature);
    GYR3DInput->u8EventType = GYR_3D_Feature.Event;
    GYR3DInput->s32GyroXValue = (SINT32)gpCurVal[0];
    GYR3DInput->s32GyroYValue = (SINT32)gpCurVal[1];
    GYR3DInput->s32GyroZValue = (SINT32)gpCurVal[2];

    LastGyr_X = gpCurVal[0];
    LastGyr_Y = gpCurVal[1];
    LastGyr_Z = gpCurVal[2];
    CLEAR_EVENT(GYR_3D_Feature);

    HID_GYO_INPUT_OUT();

    return INPUT_SIZE_GYR_3D;
}
#endif // } __ENABLE_HID_GYO__

/*-------------------------------------------------------------------------
    Magnetometer
-------------------------------------------------------------------------*/
#if __ENABLE_HID_MAG__ // {
void HID_CODE _s_i_mag(UINT32 u32ReportInterval)
{
    COM_3D_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//100;

    HID_DBG_PERIOD("M: %d\r\n", COM_3D_Feature_ReportInterval);
}

int HID_CODE _g_f_mag(UINT8 *pBuf)
{
    PCOMP3_FEATURE_REPORT COM3DFeature = (PCOMP3_FEATURE_REPORT)pBuf;

    COM3DFeature->u8ReportId = HID_SENSOR_MAG;

    COM3DFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    COM3DFeature->u8ReportingState = COM_3D_Feature.ReportingState;
    COM3DFeature->u8PowerState = COM_3D_Feature.PowerState;
    COM3DFeature->u8SensorState = GET_SENSOR_STATE(COM_3D_Feature);
//	    COM3DFeature->u8SensorStatus = gu8HIDStatus;
    COM3DFeature->u32ReportInterval = COM_3D_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
	COM3DFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_COM;
    #endif
    
    COM3DFeature->u16HeadingResolution = 1;
    COM3DFeature->u16HeadingAccuracy = 100;
    COM3DFeature->u16HeadingChangeSensitivity = COM_3D_Feature_NorthChangeSensitivityABS;
    COM3DFeature->s16HeadingMaximum = 3600;
    COM3DFeature->s16HeadingMinimum = 0;
    COM3DFeature->u32FluxResolution = 1576;
    COM3DFeature->u32FluxAccuracy = 1576;
    COM3DFeature->u32FluxChangeSensitivity = COM_3D_Feature_StrenthChangeSensitivityABS;
    COM3DFeature->s32FluxMaximum =  20000000;//  2500000;
    COM3DFeature->s32FluxMinimum = -20000000;//-2500000;

    return FEATURE_SIZE_COM_3D;    
}

void HID_CODE _s_f_mag(UINT8 *pBuf, UINT16 u16BufSize)
{
    PCOMP3_FEATURE_REPORT COM3DFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_COM_3D))
    {
        COM3DFeature = (PCOMP3_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(COM_3D_Feature, COM3DFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(COM3DFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(COM_3D_Feature, COM3DFeature->u8ReportingState);
                SET_EVENT(COM_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_MAG << 4) | (COM3DFeature->u8ReportingState));
            }
        }
        
        // set power state            
        if(IS_POWER_STATE_CHANGE(COM_3D_Feature, COM3DFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(COM3DFeature->u8PowerState))
            {
                SET_POWER_STATE(COM_3D_Feature, COM3DFeature->u8PowerState);
                SET_EVENT(COM_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("M: %d, %d\r\n", COM_3D_Feature.PowerState, COM_3D_Feature.ReportingState);

                CLEAR_AUTO_ROTATE_EVENT();

                if(COM_3D_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(COM_3D_Feature))
                    {                                    
                        if(hid_check_power_mode() !=  1)
                        {      
                            hid_set_power_mode(TRUE);
                        }
                        gstHIDPowerState.Mag = 1;      

                        SET_SENSOR_STATE(COM_3D_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {                
                        gstHIDPowerState.Mag = 0;   
                        if(hid_check_power_mode() == 0)
                        {
                            hid_set_power_mode(FALSE);
                        }   

                        SET_SENSOR_STATE(COM_3D_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_MAG << 4) | (COM3DFeature->u8PowerState));
            }
        }
                       
        // set report interval
        if(COM_3D_Feature_ReportInterval != COM3DFeature->u32ReportInterval)
        {
            SET_INTERVAL(mag, COM3DFeature->u32ReportInterval);

            SET_EVENT(COM_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

            sensor_data_rate_switch(HID_SENSOR_MAG, COM_3D_Feature_ReportInterval);                    
        }

        // set sensitivity
        if(COM_3D_Feature_NorthChangeSensitivityABS != COM3DFeature->u16HeadingChangeSensitivity)
        {
            COM_3D_Feature_NorthChangeSensitivityABS = COM3DFeature->u16HeadingChangeSensitivity;
            SET_EVENT(COM_3D_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }

        if(COM_3D_Feature_StrenthChangeSensitivityABS != COM3DFeature->u32FluxChangeSensitivity)
        {
            COM_3D_Feature_StrenthChangeSensitivityABS = COM3DFeature->u32FluxChangeSensitivity;
            SET_EVENT(COM_3D_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastNorth = 0xFFFF;        
        LastMag_X = LastMag_Y = LastMag_Z = 0xFFFFFFFF;        
    }    
}

BOOL HID_CODE _c_s_mag(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_MAG(gpCurVal[0], MagnetometerX);
    HID_REPORT_UNIT_MAG(gpCurVal[1], MagnetometerY);
    HID_REPORT_UNIT_MAG(gpCurVal[2], MagnetometerZ);
    HID_REPROT_UNIT_NORTH(gpCurVal[3], NorthDegrees);

    if(COM_3D_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(COM_3D_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!((LastMag_X == 0xFFFFFFFF) && (LastMag_Y == 0xFFFFFFFF) && (LastMag_Z == 0xFFFFFFFF)))
        {
            if(abs((long)LastMag_X - (long)gpCurVal[0]) < (long)COM_3D_Feature_StrenthChangeSensitivityABS &&
               abs((long)LastMag_Y - (long)gpCurVal[1]) < (long)COM_3D_Feature_StrenthChangeSensitivityABS &&
               abs((long)LastMag_Z - (long)gpCurVal[2]) < (long)COM_3D_Feature_StrenthChangeSensitivityABS &&
               abs((short)LastNorth - (short)gpCurVal[3]) < (short)COM_3D_Feature_NorthChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }

        if(COM_3D_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_mag(UINT8 *pBuf)
{
    UINT8 u8MagInfo[2];
    PCOMP3_INPUT_REPORT COM3DInput = (PCOMP3_INPUT_REPORT) pBuf;

    COM3DInput->u8ReportId = HID_SENSOR_MAG; 
    
    COM3DInput->u8SensorState = GET_SENSOR_STATE(COM_3D_Feature);
    COM3DInput->u8EventType = COM_3D_Feature.Event;
    COM3DInput->s16HeadingCompensatedMagneticNorthValue = (SINT16)gpCurVal[3];
    COM3DInput->s32FluxXValue = (SINT32)gpCurVal[0];
    COM3DInput->s32FluxYValue = (SINT32)gpCurVal[1];
    COM3DInput->s32FluxZValue = (SINT32)gpCurVal[2];
    
    do_get_calibrate_info_mag(u8MagInfo, u8MagInfo+1);
    COM3DInput->u8MagnetometerAccuracy = u8MagInfo[1] & 0x0F;
    #if __ENABLE_HID_MAG_ACCURACY__ // {
    if(COM3DInput->u8MagnetometerAccuracy == 0)
        COM3DInput->u8MagnetometerAccuracy = 1;
    #endif // } __ENABLE_HID_MAG_ACCURACY__
    
    LastMag_X = gpCurVal[0];
    LastMag_Y = gpCurVal[1];
    LastMag_Z = gpCurVal[2];
    LastNorth = (SINT16)gpCurVal[3];
    CLEAR_EVENT(COM_3D_Feature);
    
    //HID_DBG_MSG("M: [%d]%d %d %d %d\r\n", gu8HIDStatus, gpCurVal[0], gpCurVal[1], gpCurVal[2], gpCurVal[3]);

    return INPUT_SIZE_COM_3D;
}
#endif // } __ENABLE_HID_MAG__

/*-------------------------------------------------------------------------
    Inclinometer & Device Orientation
-------------------------------------------------------------------------*/
#if __ENABLE_HID_FUSION_SENSOR__ // {
void HID_CODE _s_i_inc(UINT32 u32ReportInterval)
{
    INC_3D_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//100;

    HID_DBG_PERIOD("I: %d\r\n", INC_3D_Feature_ReportInterval);
}

int HID_CODE _g_f_inc(UINT8 *pBuf)
{
    PINC3_FEATURE_REPORT INC3DFeature = (PINC3_FEATURE_REPORT)pBuf;

    INC3DFeature->u8ReportId = HID_SENSOR_INC;

    INC3DFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    INC3DFeature->u8ReportingState = INC_3D_Feature.ReportingState;
    INC3DFeature->u8PowerState = INC_3D_Feature.PowerState;
    INC3DFeature->u8SensorState = GET_SENSOR_STATE(INC_3D_Feature);
//	    INC3DFeature->u8SensorStatus = gu8HIDStatus;
    INC3DFeature->u32ReportInterval = INC_3D_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    INC3DFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_INC;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    INC3DFeature->u16IncResolution = 1;
    INC3DFeature->u16IncAccuracy = 50;
    INC3DFeature->u16IncChangeSensitivity = INC_3D_Feature_ChangeSensitivityABS;
    INC3DFeature->s16IncMaximum = 3650;
    INC3DFeature->s16IncMinimum = -3650;

    return FEATURE_SIZE_INC_3D;    
}

void HID_CODE _s_f_inc(UINT8 *pBuf, UINT16 u16BufSize)
{
    PINC3_FEATURE_REPORT INC3DFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_INC_3D))
    {
        INC3DFeature = (PINC3_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(INC_3D_Feature, INC3DFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(INC3DFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(INC_3D_Feature, INC3DFeature->u8ReportingState);
                SET_EVENT(INC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_INC << 4) | (INC3DFeature->u8ReportingState));
            }
        }
        
        // set power state     
        if(IS_POWER_STATE_CHANGE(INC_3D_Feature, INC3DFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(INC3DFeature->u8PowerState))
            {
                SET_POWER_STATE(INC_3D_Feature, INC3DFeature->u8PowerState);
                SET_EVENT(INC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("I: %d, %d\r\n", INC_3D_Feature.PowerState, INC_3D_Feature.ReportingState);

                CLEAR_AUTO_ROTATE_EVENT();

                if(INC_3D_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(INC_3D_Feature))
                    {
                        if(hid_check_power_mode() !=  1)
                        {      
                            hid_set_power_mode(TRUE);
                        }
                        gstHIDPowerState.Inc = 1;  

                        SET_SENSOR_STATE(INC_3D_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        gstHIDPowerState.Inc = 0;   
                        if(hid_check_power_mode() == 0)
                        {
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(INC_3D_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_INC << 4) | (INC3DFeature->u8PowerState));
            }
        }

        // set report interval
        if(INC_3D_Feature_ReportInterval != INC3DFeature->u32ReportInterval)
        {
            SET_INTERVAL(inc, INC3DFeature->u32ReportInterval);

            SET_EVENT(INC_3D_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
         
            sensor_data_rate_switch(HID_SENSOR_INC, INC_3D_Feature_ReportInterval);                    
        }

        // set sensitivity
        if(INC_3D_Feature_ChangeSensitivityABS != INC3DFeature->u16IncChangeSensitivity)
        {
            INC_3D_Feature_ChangeSensitivityABS = INC3DFeature->u16IncChangeSensitivity;
            SET_EVENT(INC_3D_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastTil_X = LastTil_Y = LastTil_Z = 0xFFFF;

        // -
        #ifdef __ENABLE_HID_PM_DIPO__
        gusIdleThreshold = (HID_PM_THRESHOLD / INC_3D_Feature_ReportInterval);
        #endif        
    }    
}

BOOL HID_CODE _c_s_inc(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_INC(gpCurVal[0], InclinometerX);
    HID_REPORT_UNIT_INC(gpCurVal[1], InclinometerY);
    HID_REPORT_UNIT_INC(gpCurVal[2], InclinometerZ);

    if(INC_3D_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(INC_3D_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;               

        if(!((LastTil_X == 0xFFFF) && (LastTil_Y == 0xFFFF) && (LastTil_Z == 0xFFFF)))
        {
            if(abs((short)LastTil_X - (short)gpCurVal[0]) < (short)INC_3D_Feature_ChangeSensitivityABS &&
               abs((short)LastTil_Y - (short)gpCurVal[1]) < (short)INC_3D_Feature_ChangeSensitivityABS &&
               abs((short)LastTil_Z - (short)gpCurVal[2]) < (short)INC_3D_Feature_ChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }

        if(INC_3D_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_inc(UINT8 *pBuf)
{
    #if __ENABLE_HID_MAG_ACCURACY__ // {
    UINT8 u8MagInfo[2];
    #endif // } __ENABLE_HID_MAG_ACCURACY__
    
    PINC3_INPUT_REPORT INC3DInput = (PINC3_INPUT_REPORT)pBuf;

    INC3DInput->u8ReportId = HID_SENSOR_INC;
    
    INC3DInput->u8SensorState = GET_SENSOR_STATE(INC_3D_Feature);
    INC3DInput->u8EventType = INC_3D_Feature.Event;
    INC3DInput->s16IncXValue = (SINT16)gpCurVal[0];
    INC3DInput->s16IncYValue = (SINT16)gpCurVal[1];
    INC3DInput->s16IncZValue = (SINT16)gpCurVal[2];

    #if __ENABLE_HID_MAG_ACCURACY__ // {
    do_get_calibrate_info_mag(u8MagInfo, u8MagInfo+1);
    INC3DInput->u8MagnetometerAccuracy = u8MagInfo[1] & 0x0F;
    if(INC3DInput->u8MagnetometerAccuracy == 0)
        INC3DInput->u8MagnetometerAccuracy = 1;
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    LastTil_X = gpCurVal[0];
    LastTil_Y = gpCurVal[1];
    LastTil_Z = gpCurVal[2];
    CLEAR_EVENT(INC_3D_Feature);

    return INPUT_SIZE_INC_3D;
}

void HID_CODE _s_i_ori(UINT32 u32ReportInterval)
{
    ORIENTATION_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//32;

    HID_DBG_PERIOD("O: %d\r\n", ORIENTATION_Feature_ReportInterval);
}

int HID_CODE _g_f_ori(UINT8 *pBuf)
{
    PDEVOR_FEATURE_REPORT ORIENTATIONFeature = (PDEVOR_FEATURE_REPORT)pBuf;

    ORIENTATIONFeature->u8ReportId = HID_SENSOR_ORI;

    ORIENTATIONFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    ORIENTATIONFeature->u8ReportingState = ORIENTATION_Feature.ReportingState;
    ORIENTATIONFeature->u8PowerState = ORIENTATION_Feature.PowerState;
    ORIENTATIONFeature->u8SensorState = GET_SENSOR_STATE(ORIENTATION_Feature);
//	    ORIENTATIONFeature->u8SensorStatus = gu8HIDStatus;
    ORIENTATIONFeature->u32ReportInterval = ORIENTATION_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    ORIENTATIONFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_ORI;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    ORIENTATIONFeature->u32QuaternionResolution = 1;
    ORIENTATIONFeature->u32QuaternionAccuracy = 100000;
    ORIENTATIONFeature->u32QuaternionChangeSensitivity = ORIENTATION_Feature_QuaChangeSensitivityABS;
    ORIENTATIONFeature->s32QuaternionMaximum = 10000000;
    ORIENTATIONFeature->s32QuaternionMinimum = -10000000;

    #if __ENABLE_HID_ROTATION_MATRIX__ // {
    ORIENTATIONFeature->u16RotationChangeSensitivity = 10;
    ORIENTATIONFeature->s16RotationMaximum = 120;
    ORIENTATIONFeature->s16RotationMinimum = -120;
    #endif // } __ENABLE_HID_ROTATION_MATRIX__

    return FEATURE_SIZE_ORIENTATION;    
}

void HID_CODE _s_f_ori(UINT8 *pBuf, UINT16 u16BufSize)
{
    PDEVOR_FEATURE_REPORT ORIENTATIONFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_ORIENTATION))
    {
        ORIENTATIONFeature = (PDEVOR_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(ORIENTATION_Feature, ORIENTATIONFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(ORIENTATIONFeature->u8ReportingState))            
            {
                SET_REPORTING_STATE(ORIENTATION_Feature, ORIENTATIONFeature->u8ReportingState);
                SET_EVENT(ORIENTATION_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_ORI << 4) | (ORIENTATIONFeature->u8ReportingState));
            }
        }

        // set power state            
        if(IS_POWER_STATE_CHANGE(ORIENTATION_Feature, ORIENTATIONFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(ORIENTATIONFeature->u8PowerState))
            {
                SET_POWER_STATE(ORIENTATION_Feature, ORIENTATIONFeature->u8PowerState);
                SET_EVENT(ORIENTATION_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("O: %d, %d\r\n", ORIENTATION_Feature.PowerState, ORIENTATION_Feature.ReportingState);

                CLEAR_AUTO_ROTATE_EVENT();

                if(ORIENTATION_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(ORIENTATION_Feature))
                    {
                        if(hid_check_power_mode() !=  1)
                        {                          
                            hid_set_power_mode(TRUE);            
                        }
                        gstHIDPowerState.Ori = 1;  

                        SET_SENSOR_STATE(ORIENTATION_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        gstHIDPowerState.Ori = 0;   
                        if(hid_check_power_mode() == 0)
                        {
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(ORIENTATION_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_ORI << 4) | (ORIENTATIONFeature->u8PowerState));
            }
        }
        
        // set report interval
        if(ORIENTATION_Feature_ReportInterval != ORIENTATIONFeature->u32ReportInterval)
        {
            SET_INTERVAL(ori, ORIENTATIONFeature->u32ReportInterval);

            SET_EVENT(ORIENTATION_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

            sensor_data_rate_switch(HID_SENSOR_ORI, ORIENTATION_Feature_ReportInterval);                    
        }

        // set sensitivity
        if(1000000 >= ORIENTATIONFeature->u32QuaternionChangeSensitivity)
        {
            if(ORIENTATION_Feature_QuaChangeSensitivityABS != ORIENTATIONFeature->u32QuaternionChangeSensitivity)
            {
                ORIENTATION_Feature_QuaChangeSensitivityABS = ORIENTATIONFeature->u32QuaternionChangeSensitivity;
                SET_EVENT(ORIENTATION_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
            }
        }
        
        LastQua_X = LastQua_Y = LastQua_Z = LastQua_W = 0xFFFFFFFF;        
    }    
}

BOOL HID_CODE _c_s_ori(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_ORI(gpCurVal[0], OrientationQuaternionX);
    HID_REPORT_UNIT_ORI(gpCurVal[1], OrientationQuaternionY);
    HID_REPORT_UNIT_ORI(gpCurVal[2], OrientationQuaternionZ);
    HID_REPORT_UNIT_ORI(gpCurVal[3], OrientationQuaternionW);

    if(ORIENTATION_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(ORIENTATION_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
           bRet = FALSE;

        if(!((LastQua_X == 0xFFFFFFFF) && (LastQua_Y == 0xFFFFFFFF) 
           && (LastQua_Z == 0xFFFFFFFF) && (LastQua_W == 0xFFFFFFFF)))
        {
           if(abs((long)LastQua_X - (long)gpCurVal[0]) < (long)ORIENTATION_Feature_QuaChangeSensitivityABS &&
              abs((long)LastQua_Y - (long)gpCurVal[1]) < (long)ORIENTATION_Feature_QuaChangeSensitivityABS &&
              abs((long)LastQua_Z - (long)gpCurVal[2]) < (long)ORIENTATION_Feature_QuaChangeSensitivityABS &&
              abs((long)LastQua_W - (long)gpCurVal[3]) < (long)ORIENTATION_Feature_QuaChangeSensitivityABS)
           {
               bRet = FALSE;
           }
        }

        if(ORIENTATION_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
           bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_ori(UINT8 *pBuf)
{
    #if __ENABLE_HID_MAG_ACCURACY__ // {
    UINT8 u8MagInfo[2];
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    PDEVOR_INPUT_REPORT  ORIENTATIONInput = (PDEVOR_INPUT_REPORT)pBuf;

    HID_ORI_INPUT_IN();

    ORIENTATIONInput->u8ReportId = HID_SENSOR_ORI;
    
    ORIENTATIONInput->u8SensorState = GET_SENSOR_STATE(ORIENTATION_Feature);
    ORIENTATIONInput->u8EventType = ORIENTATION_Feature.Event;
    ORIENTATIONInput->s32QuaternionXValue = (SINT32)gpCurVal[0];
    ORIENTATIONInput->s32QuaternionYValue = (SINT32)gpCurVal[1];
    ORIENTATIONInput->s32QuaternionZValue = (SINT32)gpCurVal[2];
    ORIENTATIONInput->s32QuaternionWValue = (SINT32)gpCurVal[3];

    #if __ENABLE_HID_ROTATION_MATRIX__ // {
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[0][0], RotationMatrix[0]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[1][0], RotationMatrix[1]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[2][0], RotationMatrix[2]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[0][1], RotationMatrix[3]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[1][1], RotationMatrix[4]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[2][1], RotationMatrix[5]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[0][2], RotationMatrix[6]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[1][2], RotationMatrix[7]);
    HID_REPORT_UNIT_MATRIX(ORIENTATIONInput->s16RotationValue[2][2], RotationMatrix[8]);
    #endif // } __ENABLE_HID_ROTATION_MATRIX__

    #if __ENABLE_HID_MAG_ACCURACY__ // {
    do_get_calibrate_info_mag(u8MagInfo, u8MagInfo+1);
    ORIENTATIONInput->u8MagnetometerAccuracy = u8MagInfo[1] & 0x0F;
    if(ORIENTATIONInput->u8MagnetometerAccuracy == 0)
        ORIENTATIONInput->u8MagnetometerAccuracy = 1;
    #endif // } __ENABLE_HID_MAG_ACCURACY__

    LastQua_X = gpCurVal[0];
    LastQua_Y = gpCurVal[1];
    LastQua_Z = gpCurVal[2];
    LastQua_W = gpCurVal[3];
    CLEAR_EVENT(ORIENTATION_Feature);

    HID_ORI_INPUT_OUT();

    return INPUT_SIZE_ORIENTATION;
}
#endif // } __ENABLE_HID_FUSION_SENSOR__

/*-------------------------------------------------------------------------
    Ambient Light
-------------------------------------------------------------------------*/
#if __ENABLE_HID_ALS__ // {
void HID_CODE _s_i_als(UINT32 u32ReportInterval)
{
    ALS_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;

    HID_DBG_PERIOD("Als: %d\r\n", ALS_Feature_ReportInterval);
}

int HID_CODE _g_f_als(UINT8 *pBuf)
{
    PALS_FEATURE_REPORT ALSFeature = (PALS_FEATURE_REPORT)pBuf;

    ALSFeature->u8ReportId = HID_SENSOR_ALS;

    ALSFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    ALSFeature->u8ReportingState = ALS_Feature.ReportingState;
    ALSFeature->u8PowerState = ALS_Feature.PowerState;
    ALSFeature->u8SensorState = GET_SENSOR_STATE(ALS_Feature);
    ALSFeature->u32ReportInterval = ALS_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    ALSFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_ALS;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    ALSFeature->u16IlluminanceChangeSensitivity = ALS_Feature_ChangeSensitivityREL_PCT;
    ALSFeature->u16IlluminanceMaximum = 0xFFFF;
    ALSFeature->u16IlluminanceMinimum = 0;
    
    #ifdef SENSOR_ALS_CURVE // {
    memcpy(ALSFeature->u16ResponseCurve, (UINT8*)aru16ALR_curve, sizeof(ALSFeature->u16ResponseCurve));
    #endif // } SENSOR_ALS_CURVE
    
    return FEATURE_SIZE_AMBIENTLIGHT;    
}

void HID_CODE _s_f_als(UINT8 *pBuf, UINT16 u16BufSize)
{
    PALS_FEATURE_REPORT ALSFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_AMBIENTLIGHT))
    {
        ALSFeature = (PALS_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(ALS_Feature, ALSFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(ALSFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(ALS_Feature, ALSFeature->u8ReportingState);
                SET_EVENT(ALS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_ALS << 4) | (ALSFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(ALS_Feature, ALSFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(ALSFeature->u8PowerState))
            {
                SET_POWER_STATE(ALS_Feature, ALSFeature->u8PowerState);
                SET_EVENT(ALS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("Als: %d, %d\r\n", ALS_Feature.PowerState, ALS_Feature.ReportingState);
                
                if(ALS_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(ALS_Feature))
                    {
                        if(gstHIDPowerState.Als != 1)
                        {                    
                            gstHIDPowerState.Als = 1;     
                            hid_set_power_mode(FALSE);               
                        }

                        SET_SENSOR_STATE(ALS_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.Als != 0)
                        {                    
                            gstHIDPowerState.Als = 0;    
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(ALS_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_ALS << 4) | (ALSFeature->u8PowerState));
            }
        }

        // set report interval               
        if(ALS_Feature_ReportInterval != ALSFeature->u32ReportInterval)
        {
            SET_INTERVAL(als, ALSFeature->u32ReportInterval);

            SET_EVENT(ALS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

            gu16SensorPeriod[SENSOR_I2C_IDX_ALS] = ALS_Feature_ReportInterval;                    
        }

        // set sensitivity
        if(ALS_Feature_ChangeSensitivityREL_PCT != ALSFeature->u16IlluminanceChangeSensitivity)
        {
            ALS_Feature_ChangeSensitivityREL_PCT = ALSFeature->u16IlluminanceChangeSensitivity;
            SET_EVENT(ALS_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastLux = 0xFFFFFFFF;        
    }

}

BOOL HID_CODE _c_s_als(void)
{
    BOOL bRet = TRUE;
    
    HID_REPORT_UNIT_ALS(gpCurVal[0], LightIlluminace);

    // +SHAWN20150424
    if(LightIlluminace == 0)
    {
        gpCurVal[0] = 100;
    }
    // -SHAWN20150424

    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    HID_REPORT_UNIT_COLOR(gpCurVal[1], ColorTemp);
    HID_REPORT_UNIT_CHROM(gpCurVal[2], ChromaticityX);
    HID_REPORT_UNIT_CHROM(gpCurVal[3], ChromaticityY);
    #endif // } __ENABLE_ALS_COLOR_VALUE__

    #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
    HID_REPORT_UNIT_ALS(gpCurVal[4], LightCustom);
    #endif // } __ENABLE_ALS_CUSTOM_VALUE__
        
    if(ALS_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(ALS_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;

        if(!(LastLux == 0xFFFFFFFF))
        {
            if(abs(LastLux - gpCurVal[0]) < ALS_Feature_ChangeSensitivityREL_PCT)
            {
                if(!((gpCurVal[0] == 0) && (LastLux != 0)))
                {
                    bRet = FALSE;
                }
            }
        }

        if(ALS_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_als(UINT8 *pBuf)
{
    PALS_INPUT_REPORT ALSInput = (PALS_INPUT_REPORT)pBuf;

    ALSInput->u8ReportId = HID_SENSOR_ALS;
    
    ALSInput->u8SensorState = GET_SENSOR_STATE(ALS_Feature);
    ALSInput->u8EventType = ALS_Feature.Event;
    ALSInput->u32IlluminanceValue = gpCurVal[0];

    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    ALSInput->u16ColorTempValue = (UINT16)gpCurVal[1];
    ALSInput->u16ChromaticityXValue = (UINT16)gpCurVal[2];
    ALSInput->u16ChromaticityYValue = (UINT16)gpCurVal[3];
    #endif // } __ENABLE_ALS_COLOR_VALUE__

    #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
    ALSInput->u32CustomValue = gpCurVal[4];
    #endif // } __ENABLE_ALS_CUSTOM_VALUE__

    LastLux = gpCurVal[0];
    CLEAR_EVENT(ALS_Feature);

    return INPUT_SIZE_AMBIENTLIGHT;
}
#endif // } __ENABLE_HID_ALS__

/*-------------------------------------------------------------------------
    CUSTOM
-------------------------------------------------------------------------*/
#if __ENABLE_HID_CUSTOM_SENSOR__ // {
void HID_CODE _s_i_custom(UINT32 u32ReportInterval)
{
    CUSTOM_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : ANGLE_PERIOD;

    HID_DBG_PERIOD("Cus: %d\r\n", CUSTOM_Feature_ReportInterval);
}

int HID_CODE _g_f_custom(UINT8 *pBuf)
{
    PCUS_FEATURE_REPORT CUSTOMFeature = (PCUS_FEATURE_REPORT)pBuf;
    
    CUSTOMFeature->u8ReportId = HID_SENSOR_CUSTOM;

    CUSTOMFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    CUSTOMFeature->u8ReportingState = CUSTOM_Feature.ReportingState;
    CUSTOMFeature->u8PowerState = CUSTOM_Feature.PowerState;
    CUSTOMFeature->u8SensorState = GET_SENSOR_STATE(CUSTOM_Feature);
    CUSTOMFeature->u32ReportInterval = CUSTOM_Feature_ReportInterval;
    CUSTOMFeature->u16CustomSensitivity = CUSTOM_Feature_ChangeSensitivityABS;

    #ifdef HID_CUSTOM_MODEL_STRING // {
    memcpy(CUSTOMFeature->u16Model, (UINT8*)CustomModel, sizeof(CUSTOMFeature->u16Model));
    #endif // } HID_CUSTOM_MODEL_STRING

    return FEATURE_SIZE_CUSTOM;
}

void HID_CODE _s_f_custom(UINT8 *pBuf, UINT16 u16BufSize)
{
    PCUS_FEATURE_REPORT CUSTOMFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_CUSTOM))
    {
        CUSTOMFeature = (PCUS_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(CUSTOM_Feature, CUSTOMFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(CUSTOMFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(CUSTOM_Feature, CUSTOMFeature->u8ReportingState);
                SET_EVENT(CUSTOM_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_CUSTOM << 4) | (CUSTOMFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(CUSTOM_Feature, CUSTOMFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(CUSTOMFeature->u8PowerState))
            {
                SET_POWER_STATE(CUSTOM_Feature, CUSTOMFeature->u8PowerState);
                SET_EVENT(CUSTOM_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("Cus: %d, %d\r\n", CUSTOM_Feature.PowerState, CUSTOM_Feature.ReportingState);
                
                if(CUSTOM_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(CUSTOM_Feature))
                    {
                        if(gstHIDPowerState.Custom != 1)
                        {
                            gstHIDPowerState.Custom = 1;
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(CUSTOM_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                        //printk("Custom On.\r\n");
                    }
                    else
                    {
                        if(gstHIDPowerState.Custom != 0)
                        {
                            gstHIDPowerState.Custom = 0;   
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(CUSTOM_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                        //printk("Custom Off.\r\n");
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_CUSTOM << 4) | (CUSTOMFeature->u8PowerState));
            }
        }

        // set report interval               
        if(CUSTOM_Feature_ReportInterval != CUSTOMFeature->u32ReportInterval)
        {
            SET_INTERVAL(custom, CUSTOMFeature->u32ReportInterval);

            SET_EVENT(CUSTOM_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                        
            sensor_data_rate_switch(HID_SENSOR_CUSTOM, CUSTOM_Feature_ReportInterval);
            
            gu16SensorPeriod[SENSOR_I2C_IDX_ACC2] = CUSTOM_Feature_ReportInterval;
        }

        // set sensitivity
        if(CUSTOM_Feature_ChangeSensitivityABS != CUSTOMFeature->u16CustomSensitivity)
        {
            CUSTOM_Feature_ChangeSensitivityABS = CUSTOMFeature->u16CustomSensitivity;
            SET_EVENT(CUSTOM_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }

        #if _ENABLE_ACC2_
        LastCustomAngle = 0xFFFFFFFF;
        #endif        
    }    
}

BOOL HID_CODE _c_s_custom(void)
{
    BOOL bRet = TRUE;

    #if _ENABLE_ACC2_ // {
    HID_REPORT_UNIT_ACC(gpCurVal[0], Accelerometer2[0]);
    HID_REPORT_UNIT_ACC(gpCurVal[1], Accelerometer2[1]);
    HID_REPORT_UNIT_ACC(gpCurVal[2], Accelerometer2[2]);
    HID_REPORT_UNIT_ANGLE(gpCurVal[3], CustomAngle[0]);
    HID_REPORT_UNIT_ANGLE(gpCurVal[4], CustomAngle[1]);
    HID_REPORT_UNIT_ANGLE(gpCurVal[5], CustomAngle[2]);
    #endif // } _ENABLE_ACC2_

    if(CUSTOM_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(CUSTOM_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        #if _ENABLE_ACC2_ // {
        if(!(LastCustomAngle == 0xFFFFFFFF))
        {
            if(abs(LastCustomAngle - (SINT16)gpCurVal[5]) < (CUSTOM_Feature_ChangeSensitivityABS + 10)) // Shawn: 1 degree margin
                bRet = FALSE;
        }
        #endif // } _ENABLE_ACC2_
        
        if(CUSTOM_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_custom(UINT8 *pBuf)
{
    PCUS_INPUT_REPORT CUSTOMInput = (PCUS_INPUT_REPORT)pBuf;

    CUSTOMInput->u8ReportId = HID_SENSOR_CUSTOM;  //Report_ID
    
    CUSTOMInput->u8SensorState = GET_SENSOR_STATE(CUSTOM_Feature);
    CUSTOMInput->u8EventType = CUSTOM_Feature.Event;

    #if _ENABLE_ACC2_
    CUSTOMInput->s16Val_1 = (SINT16)gpCurVal[0];
    CUSTOMInput->s16Val_2 = (SINT16)gpCurVal[1];
    CUSTOMInput->s16Val_3 = (SINT16)gpCurVal[2];
    CUSTOMInput->s16Val_4 = (SINT16)gpCurVal[3];
    CUSTOMInput->s16Val_5 = (SINT16)gpCurVal[4];
    CUSTOMInput->s16Val_6 = (SINT16)gpCurVal[5];

    LastCustomAngle = (SINT16)gpCurVal[5];
    #else
    CUSTOMInput->s16Val_1 = (SINT16)(AccelerometerX);
    CUSTOMInput->s16Val_2 = (SINT16)(AccelerometerY);
    CUSTOMInput->s16Val_3 = (SINT16)(AccelerometerZ);
    CUSTOMInput->s16Val_4 = (SINT16)(MagnetometerX);
    CUSTOMInput->s16Val_5 = (SINT16)(MagnetometerY);
    CUSTOMInput->s16Val_6 = (SINT16)(MagnetometerZ);
    #endif

    CLEAR_EVENT(CUSTOM_Feature);

    return INPUT_SIZE_CUSTOM;
}
#endif // } __ENABLE_HID_CUSTOM_SENSOR__

/*-------------------------------------------------------------------------
    Atmospheric Pressure
-------------------------------------------------------------------------*/
#if __ENABLE_HID_PRESSURE__ // {
void HID_CODE _s_i_prs(UINT32 u32ReportInterval)
{
    PRS_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;
}

int HID_CODE _g_f_prs(UINT8 *pBuf)
{
    PBAR_FEATURE_REPORT PRSFeature = (PBAR_FEATURE_REPORT)pBuf;

    PRSFeature->u8ReportId = HID_SENSOR_PRS;

    PRSFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    PRSFeature->u8ReportingState = PRS_Feature.ReportingState;
    PRSFeature->u8PowerState = PRS_Feature.PowerState;
    PRSFeature->u8SensorState = GET_SENSOR_STATE(PRS_Feature);
    PRSFeature->u32ReportInterval = PRS_Feature_ReportInterval;
    PRSFeature->u32BarChangeSensitivity = PRS_Feature_ChangeSensitivityABS;
    PRSFeature->u16BarMaximum = 5000; 
    PRSFeature->u16BarMinimum = 0;             

    return FEATURE_SIZE_PRS;    
}

void HID_CODE _s_f_prs(UINT8 *pBuf, UINT16 u16BufSize)
{
    PBAR_FEATURE_REPORT PRSFeature = NULL;
    
    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_PRS))
    {
        PRSFeature = (PBAR_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(PRS_Feature, PRSFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(PRSFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(PRS_Feature, PRSFeature->u8ReportingState);
                SET_EVENT(PRS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_PRS << 4) | (PRSFeature->u8ReportingState));
            }
        }
        
        // set power state
        if(IS_POWER_STATE_CHANGE(PRS_Feature, PRSFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(PRSFeature->u8PowerState))
            {
                SET_POWER_STATE(PRS_Feature, PRSFeature->u8PowerState);
                SET_EVENT(PRS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("P: %d, %d\r\n", PRS_Feature.PowerState, PRS_Feature.ReportingState);
               
                if(PRS_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(PRS_Feature))
                    {
                        if(gstHIDPowerState.Prs != 1)
                        {                    
                            gstHIDPowerState.Prs = 1;     
                            hid_set_power_mode(FALSE);               
                        }
     
                        SET_SENSOR_STATE(PRS_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.Prs != 0)
                        {                    
                            gstHIDPowerState.Prs = 0;    
                            hid_set_power_mode(FALSE);
                        }
     
                        SET_SENSOR_STATE(PRS_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_PRS << 4) | (PRSFeature->u8PowerState));
            }
        }

        // set report interval
        if(PRS_Feature_ReportInterval != PRSFeature->u32ReportInterval)
        {
            SET_INTERVAL(prs, PRSFeature->u32ReportInterval);

            SET_EVENT(PRS_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                
            gu16SensorPeriod[SENSOR_I2C_IDX_PRS] = PRS_Feature_ReportInterval;                    
        }    


        // set sensitivity
        if(PRS_Feature_ChangeSensitivityABS != PRSFeature->u32BarChangeSensitivity)
        {
            PRS_Feature_ChangeSensitivityABS = PRSFeature->u32BarChangeSensitivity;
            SET_EVENT(PRS_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastPrs = 0xFFFFFFFF;        
    }

}

BOOL HID_CODE _c_s_prs(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_PRS(gpCurVal[0], PressureBar);    

    if(PRS_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(PRS_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
             bRet = FALSE;
        
         if(!(LastPrs == 0xFFFFFFFF))
         {
             if(abs((long)LastPrs - (long)gpCurVal[0]) < (long)PRS_Feature_ChangeSensitivityABS)
             {
                 bRet = FALSE;
             }
         }
        
         if(PRS_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
             bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_prs(UINT8 *pBuf)
{
    PBAR_INPUT_REPORT PRSInput = (PBAR_INPUT_REPORT)pBuf;

    PRSInput->u8ReportId = HID_SENSOR_PRS;
    
    PRSInput->u8SensorState = GET_SENSOR_STATE(PRS_Feature);
    PRSInput->u8EventType = PRS_Feature.Event;
    PRSInput->u32BarValue = gpCurVal[0];
    
    LastPrs = gpCurVal[0];
    CLEAR_EVENT(PRS_Feature);

    return INPUT_SIZE_PRS;
}
#endif  //  } __ENABLE_HID_PRESSURE__

/*-------------------------------------------------------------------------
    Humidity
-------------------------------------------------------------------------*/
#if __ENABLE_HID_HUMIDITY__ // {
void HID_CODE _s_i_hyg(UINT32 u32ReportInterval)
{
    HYG_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;
}

int HID_CODE _g_f_hyg(UINT8 *pBuf)
{
    PHYG_FEATURE_REPORT HYGFeature = (PHYG_FEATURE_REPORT)pBuf;

    HYGFeature->u8ReportId = HID_SENSOR_HYG;

    HYGFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    HYGFeature->u8ReportingState = HYG_Feature.ReportingState;
    HYGFeature->u8PowerState = HYG_Feature.PowerState;
    HYGFeature->u8SensorState = GET_SENSOR_STATE(HYG_Feature);
    HYGFeature->u32ReportInterval = HYG_Feature_ReportInterval;
    HYGFeature->u16HygChangeSensitivity = HYG_Feature_ChangeSensitivityABS;
    HYGFeature->u16HygMaximum = 10000; 
    HYGFeature->u16HygMinimum = 0;             

    return FEATURE_SIZE_HYG;    
}

void HID_CODE _s_f_hyg(UINT8 *pBuf, UINT16 u16BufSize)
{
    PHYG_FEATURE_REPORT HYGFeature = NULL;
    
    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_HYG))
    {
        HYGFeature = (PHYG_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(HYG_Feature, HYGFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(HYGFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(HYG_Feature, HYGFeature->u8ReportingState);
                SET_EVENT(HYG_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_HYG << 4) | (HYGFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(HYG_Feature, HYGFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(HYGFeature->u8PowerState))
            {
                SET_POWER_STATE(HYG_Feature, HYGFeature->u8PowerState); 
                SET_EVENT(HYG_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
     
                HID_DBG_STATE("H: %d, %d\r\n", HYG_Feature.PowerState, HYG_Feature.ReportingState);
                
                if(HYG_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(HYG_Feature))
                    {
                        if(gstHIDPowerState.Hyg != 1)
                        {                    
                            gstHIDPowerState.Hyg = 1;     
                            hid_set_power_mode(FALSE);            
                        }
     
                        SET_SENSOR_STATE(HYG_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.Hyg != 0)
                        {                    
                            gstHIDPowerState.Hyg = 0;    
                            hid_set_power_mode(FALSE);
                        }
     
                        SET_SENSOR_STATE(HYG_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_HYG << 4) | (HYGFeature->u8PowerState));
            }
        }

        // set report interval
        if(HYG_Feature_ReportInterval != HYGFeature->u32ReportInterval)
        {
            SET_INTERVAL(hyg, HYGFeature->u32ReportInterval);

            SET_EVENT(HYG_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                
            gu16SensorPeriod[SENSOR_I2C_IDX_HYG] = HYG_Feature_ReportInterval;                    
        }    

        // set sensitivity
        if(HYG_Feature_ChangeSensitivityABS != HYGFeature->u16HygChangeSensitivity)
        {
            HYG_Feature_ChangeSensitivityABS = HYGFeature->u16HygChangeSensitivity;
            SET_EVENT(HYG_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastHyg = 0xFFFF;        
    }
}

BOOL HID_CODE _c_s_hyg(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_HYG(gpCurVal[0], HygValue);

    if(HYG_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(HYG_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!(LastHyg == 0xFFFF))
        {
            if(abs((short)LastHyg - (short)gpCurVal[0]) < (short)HYG_Feature_ChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }
      
        if(HYG_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_hyg(UINT8 *pBuf)
{
    PHYG_INPUT_REPORT HYGInput = (PHYG_INPUT_REPORT)pBuf;
    
    HYGInput->u8ReportId = HID_SENSOR_HYG;
    
    HYGInput->u8SensorState = GET_SENSOR_STATE(HYG_Feature);
    HYGInput->u8EventType = HYG_Feature.Event;
    HYGInput->u16HygValue = gpCurVal[0];
    
    LastHyg = gpCurVal[0];
    CLEAR_EVENT(HYG_Feature);

    return INPUT_SIZE_HYG;
}
#endif // } __ENABLE_HID_HUMIDITY__

/*-------------------------------------------------------------------------
    Temperature
-------------------------------------------------------------------------*/
#if __ENABLE_HID_TEMPERATURE__ // {
void HID_CODE _s_i_temp(UINT32 u32ReportInterval)
{
    TEMP_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;
}

int HID_CODE _g_f_temp(UINT8 *pBuf)
{
    PTEMP_FEATURE_REPORT TEMPFeature = (PTEMP_FEATURE_REPORT)pBuf;

    TEMPFeature->u8ReportId = HID_SENSOR_TEMP;

    TEMPFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    TEMPFeature->u8ReportingState = TEMP_Feature.ReportingState;
    TEMPFeature->u8PowerState = TEMP_Feature.PowerState;
    TEMPFeature->u8SensorState = GET_SENSOR_STATE(TEMP_Feature);
    TEMPFeature->u32ReportInterval = TEMP_Feature_ReportInterval;
    TEMPFeature->u16TempChangeSensitivity = TEMP_Feature_ChangeSensitivityABS;
    TEMPFeature->s16TempMaximum = 10000; 
    TEMPFeature->s16TempMinimum = -10000;             

    return FEATURE_SIZE_TEMP;    
}

void HID_CODE _s_f_temp(UINT8 *pBuf, UINT16 u16BufSize)
{
    PTEMP_FEATURE_REPORT TEMPFeature = NULL;
    
    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_TEMP))
    {
        TEMPFeature = (PTEMP_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(TEMP_Feature, TEMPFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(TEMPFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(TEMP_Feature, TEMPFeature->u8ReportingState);
                SET_EVENT(TEMP_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_TEMP << 4) | (TEMPFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(TEMP_Feature, TEMPFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(TEMPFeature->u8PowerState))
            {
                SET_POWER_STATE(TEMP_Feature, TEMPFeature->u8PowerState); 
                SET_EVENT(TEMP_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
     
                HID_DBG_STATE("T: %d, %d\r\n", TEMP_Feature.PowerState, TEMP_Feature.ReportingState);
                
                if(TEMP_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(TEMP_Feature))
                    {
                        if(gstHIDPowerState.Temp != 1)
                        {                    
                            gstHIDPowerState.Temp = 1;     
                            hid_set_power_mode(FALSE);              
                        }
     
                        SET_SENSOR_STATE(TEMP_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.Temp != 0)
                        {                    
                            gstHIDPowerState.Temp = 0;    
                            hid_set_power_mode(FALSE);
                        }
     
                        SET_SENSOR_STATE(TEMP_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_TEMP << 4) | (TEMPFeature->u8PowerState));
            }
        }

        // set report interval
        if(TEMP_Feature_ReportInterval != TEMPFeature->u32ReportInterval)
        {
            SET_INTERVAL(temp, TEMPFeature->u32ReportInterval);

            SET_EVENT(TEMP_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                
            gu16SensorPeriod[SENSOR_I2C_IDX_TEMP] = TEMP_Feature_ReportInterval;                    
        }    

        // set sensitivity
        if(TEMP_Feature_ChangeSensitivityABS != TEMPFeature->u16TempChangeSensitivity)
        {
            TEMP_Feature_ChangeSensitivityABS = TEMPFeature->u16TempChangeSensitivity;
            SET_EVENT(TEMP_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastTemp = 0xFFFF;        
    }
}

BOOL HID_CODE _c_s_temp(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_TEMP(gpCurVal[0], TempValue);

    if(TEMP_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(TEMP_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!(LastTemp== 0xFFFF))
        {
            if(abs((short)LastTemp - (short)gpCurVal[0]) < (short)TEMP_Feature_ChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }
      
        if(TEMP_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_temp(UINT8 *pBuf)
{
    PTEMP_INPUT_REPORT TEMPInput = (PTEMP_INPUT_REPORT)pBuf;
    
    TEMPInput->u8ReportId = HID_SENSOR_TEMP;
    
    TEMPInput->u8SensorState = GET_SENSOR_STATE(TEMP_Feature);
    TEMPInput->u8EventType = TEMP_Feature.Event;
    TEMPInput->s16TempValue = (short)gpCurVal[0];
    
    LastTemp = gpCurVal[0];
    CLEAR_EVENT(TEMP_Feature);

    return INPUT_SIZE_TEMP;
}
#endif // } __ENABLE_HID_TEMPERATURE__

/*-------------------------------------------------------------------------
    Proximity
-------------------------------------------------------------------------*/
#if __ENABLE_HID_PROXIMITY__ // {
void HID_CODE _s_i_prox(UINT32 u32ReportInterval)
{
    PROX_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;
}

int HID_CODE _g_f_prox(UINT8 *pBuf)
{
    PPROX_FEATURE_REPORT PROXFeature = (PPROX_FEATURE_REPORT)pBuf;

    PROXFeature->u8ReportId = HID_SENSOR_PROX;

    PROXFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    PROXFeature->u8ReportingState = PROX_Feature.ReportingState;
    PROXFeature->u8PowerState = PROX_Feature.PowerState;
    PROXFeature->u8SensorState = GET_SENSOR_STATE(PROX_Feature);
    PROXFeature->u32ReportInterval = PROX_Feature_ReportInterval;
    PROXFeature->u16ProximityChangeSensitivity = PROX_Feature_ChangeSensitivityABS;
    PROXFeature->s16ProximityMaximum = 20000; 
    PROXFeature->s16ProximityMinimum = 0;             

    return FEATURE_SIZE_PROX;    
}

void HID_CODE _s_f_prox(UINT8 *pBuf, UINT16 u16BufSize)
{
    PPROX_FEATURE_REPORT PROXFeature = NULL;
    
    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_PROX))
    {
        PROXFeature = (PPROX_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(PROX_Feature, PROXFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(PROXFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(PROX_Feature, PROXFeature->u8ReportingState);
                SET_EVENT(PROX_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_PROX << 4) | (PROXFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(PROX_Feature, PROXFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(PROXFeature->u8PowerState))
            {
                SET_POWER_STATE(PROX_Feature, PROXFeature->u8PowerState); 
                SET_EVENT(PROX_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
     
                HID_DBG_STATE("X: %d, %d\r\n", PROX_Feature.PowerState, PROX_Feature.ReportingState);
                
                if(PROX_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(PROX_Feature))
                    {
                        if(gstHIDPowerState.Prox != 1)
                        {                    
                            gstHIDPowerState.Prox = 1;     
                            hid_set_power_mode(FALSE);              
                        }
     
                        SET_SENSOR_STATE(PROX_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.Prox != 0)
                        {                    
                            gstHIDPowerState.Prox = 0;    
                            hid_set_power_mode(FALSE);
                        }
     
                        SET_SENSOR_STATE(PROX_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_PROX << 4) | (PROXFeature->u8PowerState));
            }
        }

        // set report interval
        if(PROX_Feature_ReportInterval != PROXFeature->u32ReportInterval)
        {
            SET_INTERVAL(prox, PROXFeature->u32ReportInterval);

            SET_EVENT(PROX_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                
            gu16SensorPeriod[SENSOR_I2C_IDX_PROX] = PROX_Feature_ReportInterval;                    
        }    

        // set sensitivity
        if(PROX_Feature_ChangeSensitivityABS != PROXFeature->u16ProximityChangeSensitivity)
        {
            PROX_Feature_ChangeSensitivityABS = PROXFeature->u16ProximityChangeSensitivity;
            SET_EVENT(PROX_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastProx = 0xFFFF;        
    }
}

BOOL HID_CODE _c_s_prox(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_PROX(gpCurVal[0], ProxValue);

    if(PROX_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(PROX_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!(LastProx == 0xFFFF))
        {
            if(abs((short)LastProx - (short)gpCurVal[0]) < (short)PROX_Feature_ChangeSensitivityABS)
            {
                bRet = FALSE;
            }
        }
      
        if(PROX_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_prox(UINT8 *pBuf)
{
    PPROX_INPUT_REPORT PROXInput = (PPROX_INPUT_REPORT)pBuf;
    
    PROXInput->u8ReportId = HID_SENSOR_PROX;
    
    PROXInput->u8SensorState = GET_SENSOR_STATE(PROX_Feature);
    PROXInput->u8EventType = PROX_Feature.Event;
    PROXInput->u8OutOfRange = ProxOutOfRangeFlag;
    PROXInput->s16ProximityValue = (short)gpCurVal[0];
    
    LastProx = (short)gpCurVal[0];
    CLEAR_EVENT(PROX_Feature);

    return INPUT_SIZE_PROX;
}
#endif // } __ENABLE_HID_PROXIMITY__

/*-------------------------------------------------------------------------
    UV
-------------------------------------------------------------------------*/
#if __ENABLE_HID_UV__ // {
void HID_CODE _s_i_uv(UINT32 u32ReportInterval)
{
    UV_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : 1000;
}

int HID_CODE _g_f_uv(UINT8 *pBuf)
{
    PUV_FEATURE_REPORT UVFeature = (PUV_FEATURE_REPORT)pBuf;
    
    UVFeature->u8ReportId = HID_SENSOR_UV;

    UVFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    UVFeature->u8ReportingState = UV_Feature.ReportingState;
    UVFeature->u8PowerState = UV_Feature.PowerState;
    UVFeature->u8SensorState = GET_SENSOR_STATE(UV_Feature);
    UVFeature->u32ReportInterval = UV_Feature_ReportInterval;
    UVFeature->u16UVSensitivity = UV_Feature_ChangeSensitivityABS;

    memcpy(UVFeature->u16Model, (UINT8*)UVModel, sizeof(UVFeature->u16Model));

    return FEATURE_SIZE_UV;
}

void HID_CODE _s_f_uv(UINT8 *pBuf, UINT16 u16BufSize)
{
    PUV_FEATURE_REPORT UVFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_UV))
    {
        UVFeature = (PUV_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(UV_Feature, UVFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(UVFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(UV_Feature, UVFeature->u8ReportingState);
                SET_EVENT(UV_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_UV << 4) | (UVFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(UV_Feature, UVFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(UVFeature->u8PowerState))
            {
                SET_POWER_STATE(UV_Feature, UVFeature->u8PowerState);
                SET_EVENT(UV_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);

                HID_DBG_STATE("UV: %d, %d\r\n", UV_Feature.PowerState, UV_Feature.ReportingState);
                
                if(UV_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(UV_Feature))
                    {
                        if(gstHIDPowerState.UV != 1)
                        {
                            gstHIDPowerState.UV = 1;
                            hid_set_power_mode(FALSE);
                        }

                         SET_SENSOR_STATE(UV_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        if(gstHIDPowerState.UV != 0)
                        {
                            gstHIDPowerState.UV = 0;   
                            hid_set_power_mode(FALSE);
                        }

                        SET_SENSOR_STATE(UV_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_UV << 4) | (UVFeature->u8PowerState));
            }
        }

        // set report interval               
        if(UV_Feature_ReportInterval != UVFeature->u32ReportInterval)
        {
            SET_INTERVAL(uv, UVFeature->u32ReportInterval);

            SET_EVENT(UV_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
                        
            gu16SensorPeriod[SENSOR_I2C_IDX_UV] = UV_Feature_ReportInterval;
        }

        // set sensitivity
        if(UV_Feature_ChangeSensitivityABS != UVFeature->u16UVSensitivity)
        {
            UV_Feature_ChangeSensitivityABS = UVFeature->u16UVSensitivity;
            SET_EVENT(UV_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }

        LastUV = 0xFFFFFFFF;        
    }    
}

BOOL HID_CODE _c_s_uv(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_UV(gpCurVal[0], UVValue);

    #ifdef __ENABLE_UV_2ND_VALUE__ // {
    HID_REPORT_UNIT_UV(gpCurVal[1], UVValue2);
    #endif // } __ENABLE_UV_2ND_VALUE__

    if(UV_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(UV_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!(LastUV == 0xFFFFFFFF))
        {
            if(abs(LastUV - gpCurVal[0]) < UV_Feature_ChangeSensitivityABS)
                bRet = FALSE;
        }
        
        if(UV_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_uv(UINT8 *pBuf)
{
    PUV_INPUT_REPORT UVInput = (PUV_INPUT_REPORT)pBuf;

    UVInput->u8ReportId = HID_SENSOR_UV;  //Report_ID
    
    UVInput->u8SensorState = GET_SENSOR_STATE(UV_Feature);
    UVInput->u8EventType = UV_Feature.Event;
    UVInput->u32UVValue = gpCurVal[0];

    #ifdef __ENABLE_UV_2ND_VALUE__ // {
    UVInput->u32UVValue2 = gpCurVal[1];
    #endif // } __ENABLE_UV_2ND_VALUE__

    LastUV = gpCurVal[0];
    CLEAR_EVENT(UV_Feature);

    return INPUT_SIZE_UV;
}
#endif // } __ENABLE_HID_UV__

/*-------------------------------------------------------------------------
    Android Sensor
-------------------------------------------------------------------------*/
#if __ENABLE_HID_ANDROID__ // {
UINT16 HID_CODE hid_get_mini_acc_interval(void)
{
    UINT16 mini_interval = 0xFFFF;

    if(ACC_3D_Feature_ReportInterval < mini_interval)
        mini_interval = ACC_3D_Feature_ReportInterval;

    if(GRA_Feature_ReportInterval < mini_interval)
        mini_interval = GRA_Feature_ReportInterval;

    if(LACC_Feature_ReportInterval < mini_interval)
        mini_interval = LACC_Feature_ReportInterval;

    return mini_interval;
}

void HID_CODE _s_i_gra(UINT32 u32ReportInterval)
{
    GRA_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//100;
}

int HID_CODE _g_f_gra(UINT8 *pBuf)
{
    PGRA_FEATURE_REPORT GRAFeature = (PGRA_FEATURE_REPORT)pBuf;

    GRAFeature->u8ReportId = HID_SENSOR_GRA;

    GRAFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    GRAFeature->u8ReportingState = GRA_Feature.ReportingState;
    GRAFeature->u8PowerState = GRA_Feature.PowerState;
    GRAFeature->u8SensorState = GET_SENSOR_STATE(GRA_Feature);
//	    GRAFeature->u8SensorStatus = gu8HIDStatus;
    GRAFeature->u32ReportInterval = GRA_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    GRAFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_GRA;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    GRAFeature->u16GraResolution = 2;
    GRAFeature->u16GraAccuracy = 2;
    GRAFeature->u16GraChangeSensitivity = GRA_Feature_ChangeSensitivityABS;
    GRAFeature->s16GraMaximum = 4000;
    GRAFeature->s16GraMinimum = -4000;

    return FEATURE_SIZE_GRAVITY;    
}

void HID_CODE _s_f_gra(UINT8 *pBuf, UINT16 u16BufSize)
{
    PGRA_FEATURE_REPORT GRAFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_GRAVITY))
    {
        GRAFeature = (PGRA_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(GRA_Feature, GRAFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(GRAFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(GRA_Feature, GRAFeature->u8ReportingState);
                SET_EVENT(GRA_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_GRA << 4) | (GRAFeature->u8ReportingState));
            }
        }

        // set power state
        if(IS_POWER_STATE_CHANGE(GRA_Feature, GRAFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(GRAFeature->u8PowerState))
            {
                SET_POWER_STATE(GRA_Feature, GRAFeature->u8PowerState);
                SET_EVENT(GRA_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
     
                HID_DBG_STATE("Gra: %d, %d\r\n", GRA_Feature.PowerState, GRA_Feature.ReportingState);
                
                if(GRA_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(GRA_Feature))
                    {
                        gstHIDPowerState.Gra = 1;  
     
                        // acc power on
                        if(gstHIDPowerState.Acc == 0)
                        {                    
                            gstHIDPowerState.Acc = 1;    
                            hid_change_power_mode();
                        }
     
                        SET_SENSOR_STATE(GRA_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        gstHIDPowerState.Gra = 0;    
     
                        // acc power off
                        if(ACC_3D_Feature.PowerState == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM)
                        {
                            if(gstHIDPowerState.Lacc == 0)
                            {
                                gstHIDPowerState.Acc = 0;
                                hid_change_power_mode();
                            }
                        }
     
                        SET_SENSOR_STATE(GRA_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_GRA << 4) | (GRAFeature->u8PowerState));
            }
        }
 
        // set report interval
        if(GRA_Feature_ReportInterval != GRAFeature->u32ReportInterval)
        {
            SET_INTERVAL(gra, GRAFeature->u32ReportInterval);
 
            SET_EVENT(GRA_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            
            if(gstHIDPowerState.Acc == 1)
            {
                sensor_data_rate_switch(HID_SENSOR_ACC, hid_get_mini_acc_interval());
            }                    
        }     
 
        // set sensitivity
        if(GRA_Feature_ChangeSensitivityABS != GRAFeature->u16GraChangeSensitivity)
        {
             GRA_Feature_ChangeSensitivityABS = GRAFeature->u16GraChangeSensitivity;
             SET_EVENT(GRA_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastGra_X = LastGra_Y = LastGra_Z = 0xFFFF;
    }

}

BOOL HID_CODE _c_s_gra(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_ACC(gpCurVal[0], GravityX);
    HID_REPORT_UNIT_ACC(gpCurVal[1], GravityY);
    HID_REPORT_UNIT_ACC(gpCurVal[2], GravityZ);

    if(GRA_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(GRA_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!((LastGra_X == 0xFFFF) && (LastGra_Y == 0xFFFF) && (LastGra_Z == 0xFFFF)))
        {
            if((abs((short)LastGra_X - (short)gpCurVal[0]) < (short)GRA_Feature_ChangeSensitivityABS) &&
               (abs((short)LastGra_Y - (short)gpCurVal[1]) < (short)GRA_Feature_ChangeSensitivityABS) &&
               (abs((short)LastGra_Z - (short)gpCurVal[2]) < (short)GRA_Feature_ChangeSensitivityABS))
            {
                bRet = FALSE;
            }
        }
        
        if(GRA_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_gra(UINT8 *pBuf)
{
    PGRA_INPUT_REPORT GRAInput = (PGRA_INPUT_REPORT)pBuf;
    
    GRAInput->u8ReportId = HID_SENSOR_GRA;

    GRAInput->u8SensorState = GET_SENSOR_STATE(GRA_Feature);
    GRAInput->u8EventType = GRA_Feature.Event;
    GRAInput->s16GraXValue = (SINT16)gpCurVal[0];
    GRAInput->s16GraYValue = (SINT16)gpCurVal[1];
    GRAInput->s16GraZValue = (SINT16)gpCurVal[2];

    LastGra_X = (UINT16)gpCurVal[0];
    LastGra_Y = (UINT16)gpCurVal[1];
    LastGra_Z = (UINT16)gpCurVal[2];
    CLEAR_EVENT(GRA_Feature);

    return INPUT_SIZE_GRAVITY;
}

void HID_CODE _s_i_lacc(UINT32 u32ReportInterval)
{
    LACC_Feature_ReportInterval = (u32ReportInterval != 0) ? u32ReportInterval : FUSION_PERIOD;//100;
}

int HID_CODE _g_f_lacc(UINT8 *pBuf)
{
    PLACC_FEATURE_REPORT LACCFeature = (PLACC_FEATURE_REPORT)pBuf;;

    LACCFeature->u8ReportId = HID_SENSOR_LACC;

    LACCFeature->u8ConnectionType = HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_ENUM;
    LACCFeature->u8ReportingState = LACC_Feature.ReportingState;
    LACCFeature->u8PowerState = LACC_Feature.PowerState;
    LACCFeature->u8SensorState = GET_SENSOR_STATE(LACC_Feature);
//	    LACCFeature->u8SensorStatus = gu8HIDStatus;
    LACCFeature->u32ReportInterval = LACC_Feature_ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    LACCFeature->u32MinimumReportInterval = HID_MIN_REPORT_INVAL_LACC;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__
    
    LACCFeature->u16LAccResolution = 2;
    LACCFeature->u16LAccAccuracy = 2;
    LACCFeature->u16LAccChangeSensitivity = LACC_Feature_ChangeSensitivityABS;
    LACCFeature->s16LAccMaximum = 4000;
    LACCFeature->s16LAccMinimum = -4000;

    return FEATURE_SIZE_LINEAR_ACC;    
}

void HID_CODE _s_f_lacc(UINT8 *pBuf, UINT16 u16BufSize)
{
    PLACC_FEATURE_REPORT    LACCFeature = NULL;

    if(IS_FEATURE_SIZE_OK(u16BufSize, FEATURE_SIZE_LINEAR_ACC))
    {
        LACCFeature = (PLACC_FEATURE_REPORT)pBuf;

        // set reporting state
        if(IS_REPORTING_STATE_CHANGE(LACC_Feature, LACCFeature->u8ReportingState))
        {
            if(IS_REPORTING_STATE_LEGAL(LACCFeature->u8ReportingState))
            {
                SET_REPORTING_STATE(LACC_Feature, LACCFeature->u8ReportingState);
                SET_EVENT(LACC_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            }
            else
            {
                HID_DEBUG_PORT4((HID_SENSOR_LACC << 4) | (LACCFeature->u8ReportingState));
            }
        }
        
        // set power state
        if(IS_POWER_STATE_CHANGE(LACC_Feature, LACCFeature->u8PowerState))
        {
            if(IS_POWER_STATE_LEGAL(LACCFeature->u8PowerState))
            {
                SET_POWER_STATE(LACC_Feature, LACCFeature->u8PowerState);
                SET_EVENT(LACC_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
     
                HID_DBG_STATE("LAcc: %d, %d\r\n", LACC_Feature.PowerState, LACC_Feature.ReportingState);
                
                if(LACC_Feature.SensorState != HID_USAGE_SENSOR_STATE_INITIALIZING_ENUM)
                {
                    if(IS_POWER_STATE_ON(LACC_Feature))
                    {
                        gstHIDPowerState.Lacc = 1;  
     
                        // acc power on
                        if(gstHIDPowerState.Acc == 0)
                        {                    
                            gstHIDPowerState.Acc = 1;    
                            hid_change_power_mode();
                        }
     
                        SET_SENSOR_STATE(LACC_Feature, HID_USAGE_SENSOR_STATE_READY_ENUM);
                    }
                    else
                    {
                        gstHIDPowerState.Lacc = 0;    
     
                        // acc power off
                        if(ACC_3D_Feature.PowerState == HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_ENUM)
                        {
                            if(gstHIDPowerState.Gra == 0)
                            {
                                gstHIDPowerState.Acc = 0;
                                hid_change_power_mode();
                            }
                        }
     
                        SET_SENSOR_STATE(LACC_Feature, HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_ENUM);
                    }
                }
            }
            else
            {
                HID_DEBUG_PORT5((HID_SENSOR_LACC << 4) | (LACCFeature->u8PowerState));
            }
        }
 
        // set report interval
        if(LACC_Feature_ReportInterval != LACCFeature->u32ReportInterval)
        {
            SET_INTERVAL(lacc, LACCFeature->u32ReportInterval);
 
            SET_EVENT(LACC_Feature, HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_ENUM);
            
            if(gstHIDPowerState.Acc == 1)
            {
                sensor_data_rate_switch(HID_SENSOR_ACC, hid_get_mini_acc_interval());                    
            }                    
        }          
 
        // set sensitivity
        if(LACC_Feature_ChangeSensitivityABS != LACCFeature->u16LAccChangeSensitivity)
        {
             LACC_Feature_ChangeSensitivityABS = LACCFeature->u16LAccChangeSensitivity;
             SET_EVENT(LACC_Feature, HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_ENUM);
        }
        
        LastLAcc_X = LastLAcc_Y = LastLAcc_Z = 0xFFFF;       
    }
}

BOOL HID_CODE _c_s_lacc(void)
{
    BOOL bRet = TRUE;

    HID_REPORT_UNIT_ACC(gpCurVal[0], LinearAccX);
    HID_REPORT_UNIT_ACC(gpCurVal[1], LinearAccY);
    HID_REPORT_UNIT_ACC(gpCurVal[2], LinearAccZ);

    if(LACC_Feature.Event == HID_SENSOR_DEFAULT_EVENT)
    {
        if(LACC_Feature.ReportingState == HID_USAGE_REPORTING_STATE_ON_NONE_ENUM)
            bRet = FALSE;
        
        if(!((LastLAcc_X == 0xFFFF) && (LastLAcc_Y == 0xFFFF) && (LastLAcc_Z == 0xFFFF)))
        {
            if((abs((short)LastLAcc_X - (short)gpCurVal[0]) < (short)LACC_Feature_ChangeSensitivityABS) &&
               (abs((short)LastLAcc_Y - (short)gpCurVal[1]) < (short)LACC_Feature_ChangeSensitivityABS) &&
               (abs((short)LastLAcc_Z - (short)gpCurVal[2]) < (short)LACC_Feature_ChangeSensitivityABS))
            {
                bRet = FALSE;
            }
        }
        
        if(LACC_Feature.PowerState != HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_ENUM)
            bRet = FALSE;
    }

    return bRet;
}

int HID_CODE _g_i_lacc(UINT8 *pBuf)
{
    PLACC_INPUT_REPORT LACCInput = (PLACC_INPUT_REPORT)pBuf;
    
    LACCInput->u8ReportId = HID_SENSOR_LACC;
    
    LACCInput->u8SensorState = GET_SENSOR_STATE(LACC_Feature);
    LACCInput->u8EventType = LACC_Feature.Event;
    LACCInput->s16LAccXValue = (SINT16)gpCurVal[0];
    LACCInput->s16LAccYValue = (SINT16)gpCurVal[1];
    LACCInput->s16LAccZValue = (SINT16)gpCurVal[2];

    LastLAcc_X = (UINT16)gpCurVal[0];
    LastLAcc_Y = (UINT16)gpCurVal[1];
    LastLAcc_Z = (UINT16)gpCurVal[2];
    CLEAR_EVENT(LACC_Feature);

    return INPUT_SIZE_LINEAR_ACC;
}
#endif // } __ENABLE_HID_ANDROID__

/*-------------------------------------------------------------------------
    HID Function List
-------------------------------------------------------------------------*/
HID_RODATA const T_HID_FUNC gstHidFunc[] = 
{
    #if __ENABLE_HID_ACC__ // {
    HID_FUNC_LIST(acc),
    #endif // } __ENABLE_HID_ACC__
        
    #if __ENABLE_HID_GYO__ // {
    HID_FUNC_LIST(gyo),
    #endif // } __ENABLE_HID_GYO__

    #if __ENABLE_HID_MAG__ // {
    HID_FUNC_LIST(mag),
    #endif // } __ENABLE_HID_MAG__

    #if __ENABLE_HID_FUSION_SENSOR__ // {
    HID_FUNC_LIST(inc),
    HID_FUNC_LIST(ori),
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    #if __ENABLE_HID_ALS__ // {
    HID_FUNC_LIST(als),
    #endif // } __ENABLE_HID_ALS__

    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    HID_FUNC_LIST(custom),
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__

    #if __ENABLE_HID_PRESSURE__ // {
    HID_FUNC_LIST(prs),
    #endif // } __ENABLE_HID_PRESSURE__
    
    #if __ENABLE_HID_HUMIDITY__ // {
    HID_FUNC_LIST(hyg),
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__ // {
    HID_FUNC_LIST(temp),
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    HID_FUNC_LIST(prox),
    #endif // } __ENABLE_HID_PROXIMITY__

    #if __ENABLE_HID_UV__ // {
    HID_FUNC_LIST(uv),
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    HID_FUNC_LIST(gra),
    HID_FUNC_LIST(lacc),
    #endif // } __ENABLE_HID_ANDROID__
};

//********************************************************************************
int HID_CODE hid_get_feature_report(UINT8 u8ID, UINT8 *pBuf)
{
    if((u8ID > 0) && (u8ID < HID_SENSOR_MAX))
    {
        return (gstHidFunc[HID_SENSOR_IDX(u8ID)].pfnGetFeature)(pBuf);
    }
    else if(u8ID == HID_CUSTOM_REPORT_ID)
    {
        return hcc_get_custom_data(pBuf);
    }
    else
    {
        
    }
    
    return 0;
}

void HID_CODE hid_set_feature_report(UINT8 u8ID, UINT8 *pBuf, UINT16 u16BufSize)
{
    if((u8ID > 0) && (u8ID < HID_SENSOR_MAX))
    {
        (gstHidFunc[HID_SENSOR_IDX(u8ID)].pfnSetFeature)(pBuf, u16BufSize);
    }
    else if(u8ID == HID_CUSTOM_REPORT_ID)
    {
        hcc_do_custom_cmd(pBuf);
    }
    else
    {
        
    }
}

BOOL HID_CODE hid_check_sensitivity(UINT8 u8ID)
{
    if((u8ID > 0) && (u8ID < HID_SENSOR_MAX))
    {
        return (gstHidFunc[HID_SENSOR_IDX(u8ID)].pfnCheckSensitivity)();
    }
    else
    {
        
    }

    return FALSE;
}

int HID_CODE hid_get_input_report(UINT8 u8ID, UINT8 *pBuf)
{
    if((u8ID > 0) && (u8ID < HID_SENSOR_MAX))
    {
        return (gstHidFunc[HID_SENSOR_IDX(u8ID)].pfnGetInputReport)(pBuf);
    }
    else
    {
        pBuf[0] = 0;  //Report_ID
        pBuf[1] = HID_USAGE_SENSOR_STATE_ERROR_ENUM;
    }
    
    return 2;
}

//********************************************************************************
UINT32 HID_CODE hid_get_min_report_interval(void)
{
#if 1
    UINT32 u32MiniPeriod = 0xFFFFFFFF;
    
    #if _ENABLE_ACC_ // {
    if(/*(gstHIDPowerState.Acc == 1) &&*/ (gu32HIDInterval[HID_SENSOR_ACC] != 0))
    {
        if(u32MiniPeriod > gu32HIDInterval[HID_SENSOR_ACC])
        {
            u32MiniPeriod = gu32HIDInterval[HID_SENSOR_ACC];
        }
    }
    #endif // } _ENABLE_ACC_
        
    #if _ENABLE_MAG_ // {
    if((gstHIDPowerState.Mag == 1) && (gu32HIDInterval[HID_SENSOR_MAG] != 0))
    {
        if(u32MiniPeriod > gu32HIDInterval[HID_SENSOR_MAG])
        {
            u32MiniPeriod = gu32HIDInterval[HID_SENSOR_MAG];
        }
    }
    #endif // } _ENABLE_MAG_
    
    #if _ENABLE_GYO_ // {
    if((gstHIDPowerState.Gyo == 1) && (gu32HIDInterval[HID_SENSOR_GYR] != 0))
    {
        if(u32MiniPeriod > gu32HIDInterval[HID_SENSOR_GYR])
        {
            u32MiniPeriod = gu32HIDInterval[HID_SENSOR_GYR];
        }
    }    
    #endif // } _ENABLE_GYO_

//	    #if DEF_FUSION_6D // {
//	    if(gstHIDPowerState.Gyo == 1)
//	    {
//	        u32MiniPeriod = 10; 
//	    }
//	    #endif
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    if((gstHIDPowerState.Inc == 1) && (gu32HIDInterval[HID_SENSOR_INC] != 0))
    {
        if(u32MiniPeriod > gu32HIDInterval[HID_SENSOR_INC])
        {
            u32MiniPeriod = gu32HIDInterval[HID_SENSOR_INC];
        }
    }

    if((gstHIDPowerState.Ori == 1) && (gu32HIDInterval[HID_SENSOR_ORI] != 0))
    {
        if(u32MiniPeriod > gu32HIDInterval[HID_SENSOR_ORI])
        {
            u32MiniPeriod = gu32HIDInterval[HID_SENSOR_ORI];
        }
    }        
    #endif // } __ENABLE_HID_FUSION_SENSOR__
#else
    int i;
    UINT32 u32MiniPeriod;

    for(u32MiniPeriod = 0xFFFFFFFF, i = 1; i < (TOTAL_HID_DEVICE + 1); i++)
    {
        if(u32MiniPeriod > gu32HIDInterval[i])
        {
            if(gu32HIDInterval[i] != 0)
            {
                u32MiniPeriod = gu32HIDInterval[i];
            }
        }
    }
#endif

    HID_DBG_PERIOD("MiniPeriod(%02Xh): %d\r\n", *(UINT8*)&gstHIDPowerState, u32MiniPeriod);
    
	return u32MiniPeriod;
}

void HID_CODE_L hid_init_time_interval(void)
{
    UINT8 u8Idx;

    for(u8Idx = 0; u8Idx < (HID_SENSOR_MAX-1); u8Idx++)
    {
        (gstHidFunc[u8Idx].pfnSetReportInterval)(0);
    }
}

//********************************************************************************
UINT8 HID_CODE_L hid_check_next_cmd(void)
{
    volatile UINT8 ucRet = 0;

    if(DEF_I2C_HID)
    { 
        ucRet = g_u8HIDNextCmdID;
    }
    
    return ucRet;    
}

//********************************************************************************
#ifdef __ENABLE_HID_I2C_API__ // {
#include "./i2c_hid.c"
#endif // } __ENABLE_HID_I2C_API__

#ifdef __ENABLE_HID_USB_API__ // {
#include "./usb_slave_hid.c"
#include "../usb/usb_slave.c"
#include "../usb/usb_slave_const.c"
#endif // } __ENABLE_HID_USB_API__

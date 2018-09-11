#ifndef __HID_COMMON_H__
#define __HID_COMMON_H__

#include "chip_type.h"
#include "mem_layout.h"
#include "hid_sensor_spec_macros.h"
#include "sensor_fusion.h"

//********************************************************************************
#include "../oem/Project_H/INCLUDE/oem_project.h"

// HID function
//  #define __ENABLE_HID_PM_DIPO__
#define __ENABLE_HID_I2C_API__
#define __ENABLE_HID_USB_API__
//	#define __ENABLE_HID_FAKE_ROTATE_EVENT__ //Shawn: Workaround for Win8 bug

// HID property
#define __ENABLE_HID_SENSOR_STATE__     1
#define __ENABLE_HID_ROTATION_MATRIX__  0
#define __ENABLE_HID_MAG_ACCURACY__     1 //Shawn: Minimum supported client -> Windows 8.1
#define __ENABLE_HID_MIN_REPORT_INVAL__	0

// HID device
#define __ENABLE_HID_FUSION_SENSOR__    (_ENABLE_ACC_ && _ENABLE_MAG_ && (_ENABLE_GYO_ || DEF_FUSION_6D)) // Orientation, Inclinometer
#define __ENABLE_HID_CUSTOM_SENSOR__    (_ENABLE_ACC2_ && SHOW_HID_CUSTOM_SENSOR)
#define __ENABLE_HID_ACC__              _ENABLE_ACC_
#define __ENABLE_HID_GYO__              (_ENABLE_GYO_ || DEF_FUSION_6D)
#define __ENABLE_HID_MAG__              _ENABLE_MAG_
#define __ENABLE_HID_ALS__              _ENABLE_ALS_
#define __ENABLE_HID_PRESSURE__         _ENABLE_PRS_
#define __ENABLE_HID_HUMIDITY__         _ENABLE_HYG_
#define __ENABLE_HID_TEMPERATURE__      _ENABLE_TEMP_
#define __ENABLE_HID_PROXIMITY__        _ENABLE_PROX_
#define __ENABLE_HID_UV__               _ENABLE_UV_
#define __ENABLE_HID_ANDROID__          (DEF_ANDROID || DEF_DUAL_OS)
#define __ENABLE_HID_WIN__              (!DEF_ANDROID || DEF_DUAL_OS)

//********************************************************************************
#ifdef __ENABLE_HID_I2C_API__ // {
#include "./i2c_hid.h"
#endif // } __ENABLE_HID_I2C_API__

#ifdef __ENABLE_HID_USB_API__ // {
#include "./usb_slave_hid.h"
#include "../usb/usb.h"
#include "../usb/usb_slave.h"
#include "../usb/usb_slave_const.h"
#endif // } __ENABLE_HID_USB_API__

#define TOTAL_HID_DEVICE    (__ENABLE_HID_ACC__ + __ENABLE_HID_GYO__ + __ENABLE_HID_MAG__ + __ENABLE_HID_ALS__ \
                             + (2*__ENABLE_HID_FUSION_SENSOR__) + __ENABLE_HID_CUSTOM_SENSOR__ \
                             + __ENABLE_HID_PRESSURE__ + __ENABLE_HID_HUMIDITY__ + __ENABLE_HID_TEMPERATURE__ \
                             + __ENABLE_HID_PROXIMITY__ + __ENABLE_HID_UV__ \
                             + (2*__ENABLE_HID_ANDROID__))

#if (TOTAL_HID_DEVICE >= SENSOR_PERIOD_CNT)
#warning "Too many HID devices."
#endif

#define HID_UV_MODEL_LENGTH     8
#define HID_UV_MODEL_STRING     {'U', 'V', '-', 'L', 'i', 'g', 'h', 't'}

#define HID_REPORT_DES_TOTAL_LENGTH (sizeof(hid_report_descriptor))

#define FEATURE_SIZE_ACC_3D         (sizeof(ACCEL3_FEATURE_REPORT))
#define FEATURE_SIZE_GYR_3D         (sizeof(GYRO3_FEATURE_REPORT))      
#define FEATURE_SIZE_COM_3D         (sizeof(COMP3_FEATURE_REPORT))      
#define FEATURE_SIZE_INC_3D         (sizeof(INC3_FEATURE_REPORT))      
#define FEATURE_SIZE_ORIENTATION    (sizeof(DEVOR_FEATURE_REPORT)) 
#define FEATURE_SIZE_AMBIENTLIGHT   (sizeof(ALS_FEATURE_REPORT))  
#define FEATURE_SIZE_CUSTOM         (sizeof(CUS_FEATURE_REPORT))      
#define FEATURE_SIZE_PRS            (sizeof(BAR_FEATURE_REPORT))
#define FEATURE_SIZE_HYG            (sizeof(HYG_FEATURE_REPORT))
#define FEATURE_SIZE_TEMP           (sizeof(TEMP_FEATURE_REPORT))
#define FEATURE_SIZE_PROX           (sizeof(PROX_FEATURE_REPORT))
#define FEATURE_SIZE_UV             (sizeof(UV_FEATURE_REPORT))
#define FEATURE_SIZE_GRAVITY        (sizeof(GRA_FEATURE_REPORT))  
#define FEATURE_SIZE_LINEAR_ACC     (sizeof(LACC_FEATURE_REPORT))  

#define INPUT_SIZE_ACC_3D           (sizeof(ACCEL3_INPUT_REPORT))
#define INPUT_SIZE_GYR_3D           (sizeof(GYRO3_INPUT_REPORT))       
#define INPUT_SIZE_COM_3D           (sizeof(COMP3_INPUT_REPORT))       
#define INPUT_SIZE_INC_3D           (sizeof(INC3_INPUT_REPORT))       
#define INPUT_SIZE_ORIENTATION      (sizeof(DEVOR_INPUT_REPORT))  
#define INPUT_SIZE_AMBIENTLIGHT     (sizeof(ALS_INPUT_REPORT))   
#define INPUT_SIZE_CUSTOM           (sizeof(CUS_INPUT_REPORT))       
#define INPUT_SIZE_PRS              (sizeof(BAR_INPUT_REPORT))   
#define INPUT_SIZE_HYG              (sizeof(HYG_INPUT_REPORT))
#define INPUT_SIZE_TEMP             (sizeof(TEMP_INPUT_REPORT))
#define INPUT_SIZE_PROX             (sizeof(PROX_INPUT_REPORT))
#define INPUT_SIZE_UV               (sizeof(UV_INPUT_REPORT))
#define INPUT_SIZE_GRAVITY          (sizeof(GRA_INPUT_REPORT))   
#define INPUT_SIZE_LINEAR_ACC       (sizeof(LACC_INPUT_REPORT))   

#define HID_INTERFACE_INIT_STEP_START           0x00
#define HID_INTERFACE_INIT_STEP_HID_IF_INIT     0x01
#define HID_INTERFACE_INIT_STEP_HID_IF_READY    0x02
#define HID_INTERFACE_INIT_STEP_HID_IF_IDLE     0x03

#define USB_SLAVE_INIT_STEP_START               0x00
#define USB_SLAVE_INIT_STEP_DETECT_SPEED        0x01
#define USB_SLAVE_INIT_STEP_READY               0x02 
#define USB_SLAVE_INIT_STEP_IDLE                0x03

#pragma pack(1)
//************************************
//  Acc
//************************************
typedef struct _ACCEL3_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16AccelChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} ACCEL3_FEATURE_CONFIG, *PACCEL3_FEATURE_CONFIG;

typedef struct _ACCEL3_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
    UINT32  u32MinimumReportInterval;
    #endif

    //properties specific to this sensor
    UINT16  u16AccelResolution;
    UINT16  u16AccelAccuracy;     
    UINT16  u16AccelChangeSensitivity;
    SINT16  s16AccelMaximum;
    SINT16  s16AccelMinimum;

} ACCEL3_FEATURE_REPORT, *PACCEL3_FEATURE_REPORT;

typedef struct _ACCEL3_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT16  s16AccelXValue;
    SINT16  s16AccelYValue;
    SINT16  s16AccelZValue;

    //include if required to support the "shake" event
    UINT8   u8ShakeDetectState;

} ACCEL3_INPUT_REPORT, *PACCEL3_INPUT_REPORT;

//************************************
//  Gyo
//************************************
typedef struct _GYRO3_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT32  u32GyroChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} GYRO3_FEATURE_CONFIG, *PGYRO3_FEATURE_CONFIG;

typedef struct _GYRO3_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
    UINT32  u32MinimumReportInterval;
    #endif

    //properties specific to this sensor
    UINT32  u32GyroResolution;
    UINT32  u32GyroAccuracy;    
    UINT32  u32GyroChangeSensitivity;
    SINT32  s32GyroMaximum;
    SINT32  s32GyroMinimum;

} GYRO3_FEATURE_REPORT, *PGYRO3_FEATURE_REPORT;

typedef struct _GYRO3_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT32  s32GyroXValue;
    SINT32  s32GyroYValue;
    SINT32  s32GyroZValue;

} GYRO3_INPUT_REPORT, *PGYRO3_INPUT_REPORT;

//************************************
//  Mag
//************************************
typedef struct _COMP3_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT32  u32FluxChangeSensitivity;
    UINT16  u16HeadingChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} COMP3_FEATURE_CONFIG, *PCOMP3_FEATURE_CONFIG;

typedef struct _COMP3_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__
    UINT32  u32MinimumReportInterval;
    #endif

    //properties specific to this sensor
    UINT16  u16HeadingResolution;
    UINT16  u16HeadingAccuracy;
    UINT16  u16HeadingChangeSensitivity;
    SINT16  s16HeadingMaximum;
    SINT16  s16HeadingMinimum;
    UINT32  u32FluxResolution;
    UINT32  u32FluxAccuracy;
    UINT32  u32FluxChangeSensitivity;
    SINT32  s32FluxMaximum;
    SINT32  s32FluxMinimum;

} COMP3_FEATURE_REPORT, *PCOMP3_FEATURE_REPORT;

typedef struct _COMP3_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT16  s16HeadingCompensatedMagneticNorthValue;
    SINT32  s32FluxXValue;
    SINT32  s32FluxYValue;
    SINT32  s32FluxZValue;
    UINT8   u8MagnetometerAccuracy;

} COMP3_INPUT_REPORT, *PCOMP3_INPUT_REPORT;

//************************************
//  Inc
//************************************
typedef struct _INC3_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16IncChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} INC3_FEATURE_CONFIG, *PINC3_FEATURE_CONFIG;

typedef struct _INC3_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    UINT32  u32MinimumReportInterval;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    //properties specific to this sensor
    UINT16  u16IncResolution;
	UINT16  u16IncAccuracy;
    UINT16  u16IncChangeSensitivity;
    SINT16  s16IncMaximum;
    SINT16  s16IncMinimum;

} INC3_FEATURE_REPORT, *PINC3_FEATURE_REPORT;

typedef struct _INC3_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT16  s16IncXValue;
    SINT16  s16IncYValue;
    SINT16  s16IncZValue;
    
    #if __ENABLE_HID_MAG_ACCURACY__ // {
    UINT8   u8MagnetometerAccuracy;
    #endif // } __ENABLE_HID_MAG_ACCURACY__

} INC3_INPUT_REPORT, *PINC3_INPUT_REPORT;

//************************************
//  Ori
//************************************
typedef struct _DEVOR_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT32  u32QuaternionChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} DEVOR_FEATURE_CONFIG, *PDEVOR_FEATURE_CONFIG;

typedef struct _DEVOR_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    UINT32  u32MinimumReportInterval;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    //properties specific to this sensor
    UINT32  u32QuaternionResolution;
	UINT32  u32QuaternionAccuracy;	
    UINT32  u32QuaternionChangeSensitivity;
    SINT32  s32QuaternionMaximum;
    SINT32  s32QuaternionMinimum;

    #if __ENABLE_HID_ROTATION_MATRIX__
    //include this if the values are calculated in firmware
    //otherwise, the driver will calculate these values from the Quaternion
    UINT16  u16RotationChangeSensitivity;
    SINT16  s16RotationMaximum;
    SINT16  s16RotationMinimum;
    #endif

} DEVOR_FEATURE_REPORT, *PDEVOR_FEATURE_REPORT;

typedef struct _DEVOR_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT32  s32QuaternionXValue;
    SINT32  s32QuaternionYValue;
    SINT32  s32QuaternionZValue;
    SINT32  s32QuaternionWValue;

    #if __ENABLE_HID_ROTATION_MATRIX__ // {
    //include this if the values are calculated in firmware
    //otherwise, the driver will calculate these values from the Quaternion
    SINT16  s16RotationValue[3][3];
    #endif // } __ENABLE_HID_ROTATION_MATRIX__

    #if __ENABLE_HID_MAG_ACCURACY__ // {
    UINT8   u8MagnetometerAccuracy;
    #endif // } __ENABLE_HID_MAG_ACCURACY__

} DEVOR_INPUT_REPORT, *PDEVOR_INPUT_REPORT;

//************************************
//  Als
//************************************
typedef struct _ALS_FEATURE_CONFIG 
{
    UINT32  u32ReportInterval;
    UINT16  u16IlluminanceChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;    

} ALS_FEATURE_CONFIG, *PALS_FEATURE_CONFIG;

typedef struct _ALS_FEATURE_REPORT 
{
    //common properties
    UINT8   u8ReportId;    
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;    
    UINT8   u8SensorState;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    UINT32  u32MinimumReportInterval;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    //properties specific to this sensor
    UINT16  u16IlluminanceChangeSensitivity;
    UINT16  u16IlluminanceMaximum;
    UINT16  u16IlluminanceMinimum;

    #ifdef SENSOR_ALS_CURVE // {
    //add this definition if required by the specific application
    UINT16  u16ResponseCurve[SENSOR_ALS_CURVE_LENGTH]; //12 elements matches descriptor
    #endif // } SENSOR_ALS_CURVE
    
} ALS_FEATURE_REPORT, *PALS_FEATURE_REPORT;

typedef struct _ALS_INPUT_REPORT 
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    UINT32 u32IlluminanceValue;

    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    UINT16  u16ColorTempValue;
    UINT16  u16ChromaticityXValue;
    UINT16  u16ChromaticityYValue;
    #endif // } __ENABLE_ALS_COLOR_VALUE__

    #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
    UINT32 u32CustomValue;
    #endif // } __ENABLE_ALS_CUSTOM_VALUE__

} ALS_INPUT_REPORT, *PALS_INPUT_REPORT;

//************************************
//  Custom
//************************************
typedef struct _CUS_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16CustomSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    
} CUS_FEATURE_CONFIG, *PCUS_FEATURE_CONFIG;

typedef struct _CUS_FEATURE_REPORT
{
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState; 
    UINT32  u32ReportInterval;
    UINT16  u16CustomSensitivity;

    #ifdef HID_CUSTOM_MODEL_STRING
    UINT16  u16Model[HID_CUSTOM_MODEL_LENGTH];
    #endif
    
} CUS_FEATURE_REPORT, *PCUS_FEATURE_REPORT;

typedef struct _CUS_INPUT_REPORT
{
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;
    SINT16  s16Val_1;
    SINT16  s16Val_2;
    SINT16  s16Val_3;
    SINT16  s16Val_4;
    SINT16  s16Val_5;
    SINT16  s16Val_6;

} CUS_INPUT_REPORT, *PCUS_INPUT_REPORT;

//************************************
//  Prs
//************************************
typedef struct _BAR_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT32  u32BarChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} BAR_FEATURE_CONFIG, *PBAR_FEATURE_CONFIG;

typedef struct _BAR_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
    UINT32  u32ReportInterval;

    //properties specific to this sensor
    UINT32  u32BarChangeSensitivity;
    UINT16  u16BarMaximum;
    UINT16  u16BarMinimum;

} BAR_FEATURE_REPORT, *PBAR_FEATURE_REPORT;

typedef struct _BAR_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    UINT32  u32BarValue;

} BAR_INPUT_REPORT, *PBAR_INPUT_REPORT;

//************************************
//  Hyg
//************************************
typedef struct _HYG_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16HygChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} HYG_FEATURE_CONFIG, *PHYG_FEATURE_CONFIG;

typedef struct _HYG_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
    UINT32  u32ReportInterval;

    //properties specific to this sensor
    UINT16  u16HygChangeSensitivity;
    UINT16  u16HygMaximum;
    UINT16  u16HygMinimum;

} HYG_FEATURE_REPORT, *PHYG_FEATURE_REPORT;

typedef struct _HYG_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    UINT16  u16HygValue;

} HYG_INPUT_REPORT, *PHYG_INPUT_REPORT;

//************************************
//  Temp
//************************************
typedef struct _TEMP_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16TempChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} TEMP_FEATURE_CONFIG, *PTEMP_FEATURE_CONFIG;

typedef struct _TEMP_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
    UINT32  u32ReportInterval;

    //properties specific to this sensor
    UINT16  u16TempChangeSensitivity;
    SINT16  s16TempMaximum;
    SINT16  s16TempMinimum;

} TEMP_FEATURE_REPORT, *PTEMP_FEATURE_REPORT;

typedef struct _TEMP_INPUT_REPORT
{
    //common values
    UINT8  u8ReportId;
    UINT8  u8SensorState;
    UINT8  u8EventType;

    //values specific to this sensor
    SINT16 s16TempValue;

} TEMP_INPUT_REPORT, *PTEMP_INPUT_REPORT;

//************************************
//  Prox
//************************************
typedef struct _PROX_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16ProximityChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    
} PROX_FEATURE_CONFIG, *PPROX_FEATURE_CONFIG;

typedef struct _PROX_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
    UINT32  u32ReportInterval;

    //properties specific to this sensor
    UINT16  u16ProximityChangeSensitivity;
    SINT16  s16ProximityMaximum;  
    SINT16  s16ProximityMinimum;  

} PROX_FEATURE_REPORT, *PPROX_FEATURE_REPORT;

typedef struct _PROX_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    UINT8   u8OutOfRange;
    SINT16  s16ProximityValue;

} PROX_INPUT_REPORT, *PPROX_INPUT_REPORT;

//************************************
//  UV
//************************************
typedef struct _UV_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16UVSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    
} UV_FEATURE_CONFIG, *PUV_FEATURE_CONFIG;

typedef struct _UV_FEATURE_REPORT
{
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState; 
    UINT32  u32ReportInterval;
    UINT16  u16UVSensitivity;

    UINT16  u16Model[HID_UV_MODEL_LENGTH];
    
} UV_FEATURE_REPORT, *PUV_FEATURE_REPORT;

typedef struct _UV_INPUT_REPORT
{
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;
    UINT32  u32UVValue;

    #ifdef __ENABLE_UV_2ND_VALUE__ // {
    UINT32  u32UVValue2;
    #endif // } __ENABLE_UV_2ND_VALUE__

} UV_INPUT_REPORT, *PUV_INPUT_REPORT;

//************************************
//  Gra
//************************************
typedef struct _GRA_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16GraChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} GRA_FEATURE_CONFIG, *PGRA_FEATURE_CONFIG;

typedef struct _GRA_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    UINT32  u32MinimumReportInterval;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    //properties specific to this sensor
    UINT16  u16GraResolution;
    UINT16  u16GraAccuracy;     
    UINT16  u16GraChangeSensitivity;
    SINT16  s16GraMaximum;
    SINT16  s16GraMinimum;

} GRA_FEATURE_REPORT, *PGRA_FEATURE_REPORT;

typedef struct _GRA_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT16  s16GraXValue;
    SINT16  s16GraYValue;
    SINT16  s16GraZValue;

} GRA_INPUT_REPORT, *PGRA_INPUT_REPORT;

//************************************
//  LAcc
//************************************
typedef struct _LACC_FEATURE_CONFIG
{
    UINT32  u32ReportInterval;
    UINT16  u16LAccChangeSensitivity;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;

} LACC_FEATURE_CONFIG, *PLACC_FEATURE_CONFIG;

typedef struct _LACC_FEATURE_REPORT
{
    //common properties
    UINT8   u8ReportId;
    UINT8   u8ConnectionType;
    UINT8   u8ReportingState;
    UINT8   u8PowerState;
    UINT8   u8SensorState;
//	    UINT8   u8SensorStatus;
    UINT32  u32ReportInterval;

    #if __ENABLE_HID_MIN_REPORT_INVAL__ // {
    UINT32  u32MinimumReportInterval;
    #endif // } __ENABLE_HID_MIN_REPORT_INVAL__

    //properties specific to this sensor
    UINT16  u16LAccResolution;
    UINT16  u16LAccAccuracy;     
    UINT16  u16LAccChangeSensitivity;
    SINT16  s16LAccMaximum;
    SINT16  s16LAccMinimum;

} LACC_FEATURE_REPORT, *PLACC_FEATURE_REPORT;

typedef struct _LACC_INPUT_REPORT
{
    //common values
    UINT8   u8ReportId;
    UINT8   u8SensorState;
    UINT8   u8EventType;

    //values specific to this sensor
    SINT16  s16LAccXValue;
    SINT16  s16LAccYValue;
    SINT16  s16LAccZValue;

} LACC_INPUT_REPORT, *PLACC_INPUT_REPORT;

//************************************
typedef struct _TFEATURE_CONFIG_INFO_
{
    UINT16 u16Flag;
    
    #if __ENABLE_HID_ACC__ // {
    ACCEL3_FEATURE_CONFIG  Acc;
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_GYO__ // {
    GYRO3_FEATURE_CONFIG   Gyo;
    #endif // } __ENABLE_HID_GYO__
    
    #if __ENABLE_HID_MAG__ // {
    COMP3_FEATURE_CONFIG   Mag;
    #endif // } __ENABLE_HID_MAG__
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    INC3_FEATURE_CONFIG    Inc;
    DEVOR_FEATURE_CONFIG   Ori;
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    #if __ENABLE_HID_ALS__ // {
    ALS_FEATURE_CONFIG     Als;
    #endif // } __ENABLE_HID_ALS__
    
    #if __ENABLE_HID_CUSTOM_SENSOR__    // {
    CUS_FEATURE_CONFIG     Custom;
    #endif  // } __ENABLE_HID_CUSTOM_SENSOR__

    #if __ENABLE_HID_PRESSURE__ // {
    BAR_FEATURE_CONFIG     Prs;
    #endif // } __ENABLE_HID_PRESSURE__

    #if __ENABLE_HID_HUMIDITY__ // {
    HYG_FEATURE_CONFIG     Hyg;
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__// {
    TEMP_FEATURE_CONFIG    Temp;
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    PROX_FEATURE_CONFIG    Prox;
    #endif // } __ENABLE_HID_PROXIMITIY__

    #if __ENABLE_HID_UV__ // {
    UV_FEATURE_CONFIG      UV;
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    GRA_FEATURE_CONFIG     Gra;
    LACC_FEATURE_CONFIG    Lacc;
    #endif // } __ENABLE_HID_ANDROID__
    
}TFEATURE_CONFIG_INFO;
typedef TFEATURE_CONFIG_INFO    TFCInfo;

typedef union _TFEATURE_REPORT_INFO_
{
    #if __ENABLE_HID_ACC__ // {
    ACCEL3_FEATURE_REPORT  ACC3DFeature;
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_GYO__ // {
    GYRO3_FEATURE_REPORT   GYR3DFeature;
    #endif // } __ENABLE_HID_GYO__
    
    #if __ENABLE_HID_MAG__ // {
    COMP3_FEATURE_REPORT   COM3DFeature;
    #endif // } __ENABLE_HID_MAG__
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    INC3_FEATURE_REPORT    INC3DFeature;
    DEVOR_FEATURE_REPORT   ORIENTATIONFeature;
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    #if __ENABLE_HID_ALS__ // {
    ALS_FEATURE_REPORT     ALSFeature;
    #endif // } __ENABLE_HID_ALS__
    
    #if __ENABLE_HID_CUSTOM_SENSOR__    // {
    CUS_FEATURE_REPORT     CUSTOMFeature;
    #endif  // } __ENABLE_HID_CUSTOM_SENSOR__

    #if __ENABLE_HID_PRESSURE__ // {
    BAR_FEATURE_REPORT     PRSFeature;
    #endif // } __ENABLE_HID_PRESSURE__

    #if __ENABLE_HID_HUMIDITY__ // {
    HYG_FEATURE_REPORT     HYGFeature;
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__// {
    TEMP_FEATURE_REPORT    TEMPFeature;
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    PROX_FEATURE_REPORT    PROXFeature;
    #endif // } __ENABLE_HID_PROXIMITIY__

    #if __ENABLE_HID_UV__ // {
    UV_FEATURE_REPORT      UVFeature;
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    GRA_FEATURE_REPORT     GRAFeature;
    LACC_FEATURE_REPORT    LACCFeature;
    #endif // } __ENABLE_HID_ANDROID__
    
}TFEATURE_REPORT_INFO;
typedef TFEATURE_REPORT_INFO    TFRInfo;

typedef union _TINPUT_REPORT_INFO_
{
    #if __ENABLE_HID_ACC__ // {
    ACCEL3_INPUT_REPORT  ACC3DInput;
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_GYO__ // {
    GYRO3_INPUT_REPORT   GYR3DInput;
    #endif // } __ENABLE_HID_GYO__
    
    #if __ENABLE_HID_MAG__ // {
    COMP3_INPUT_REPORT   COM3DInput;
    #endif // } __ENABLE_HID_MAG__
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    INC3_INPUT_REPORT    INC3DInput;
    DEVOR_INPUT_REPORT   ORIENTATIONInput;
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    #if __ENABLE_HID_ALS__ // {
    ALS_INPUT_REPORT     ALSInput;
    #endif // } __ENABLE_HID_ALS__
    
    #if __ENABLE_HID_CUSTOM_SENSOR__    // {
    CUS_INPUT_REPORT     CUSTOMInput;
    #endif  // } __ENABLE_HID_CUSTOM_SENSOR__

    #if __ENABLE_HID_PRESSURE__ // {
    BAR_INPUT_REPORT     PRSInput;
    #endif // } __ENABLE_HID_PRESSURE__

    #if __ENABLE_HID_HUMIDITY__ // {
    HYG_INPUT_REPORT     HYGInput;
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__ // {
    TEMP_INPUT_REPORT    TEMPInput;
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    PROX_INPUT_REPORT    PROXInput;
    #endif // } __ENABLE_HID_PROXIMITY__

    #if __ENABLE_HID_UV__ // {
    UV_INPUT_REPORT      UVInput;
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    GRA_INPUT_REPORT     GRAInput;
    LACC_INPUT_REPORT    LACCInput;
    #endif // } __ENABLE_HID_ANDROID__
    
}TINPUT_REPORT_INFO;
typedef TINPUT_REPORT_INFO    TIRInfo;

typedef struct _T_HID_POWER_STATE_
{
    FIELD Default: 1;
    FIELD Acc: 1;
    FIELD Gyo: 1; 
    FIELD Mag: 1;
    FIELD Inc: 1;
    FIELD Ori: 1;
    FIELD Als: 1;   
    FIELD Custom: 1;
    FIELD Prs: 1; 
    FIELD Hyg: 1;
    FIELD Temp: 1;
    FIELD Prox: 1;
    FIELD UV: 1;
    FIELD Gra: 1;
    FIELD Lacc: 1;
    FIELD Reserved: 1;
}T_HID_POWER_STATE, *P_HID_POWER_STATE;

typedef struct _T_HID_COMMON_FEATURE_
{
    FIELD Event: 4; // Shawn: we only use event1~event6
    FIELD SensorState: 4;
    FIELD ReportingState: 4;
    FIELD PowerState: 4;
}T_HID_COMMON_FEATURE, *P_HID_COMMON_FEATURE;
#pragma pack()

enum _HID_SENSOR_REPORT_ID_
{
    HID_SENSOR_DEFAULT = 0,
    
    #if __ENABLE_HID_ACC__ // {
    HID_SENSOR_ACC,
    #endif // } __ENABLE_HID_ACC__

    #if __ENABLE_HID_GYO__ // {
    HID_SENSOR_GYR,
    #endif // } __ENABLE_HID_GYO__

    #if __ENABLE_HID_MAG__ // {
    HID_SENSOR_MAG,
    #endif // } __ENABLE_HID_MAG__
    
    #if __ENABLE_HID_FUSION_SENSOR__ // {
    HID_SENSOR_INC,
    HID_SENSOR_ORI,
    #endif // } __ENABLE_HID_FUSION_SENSOR__
    
    #if __ENABLE_HID_ALS__ // {
    HID_SENSOR_ALS,   
    #endif // } __ENABLE_HID_ALS__
      
    #if __ENABLE_HID_CUSTOM_SENSOR__ // {
    HID_SENSOR_CUSTOM,
    #endif // } __ENABLE_HID_CUSTOM_SENSOR__
    
    #if __ENABLE_HID_PRESSURE__ // {
    HID_SENSOR_PRS, 
    #endif  // } __ENABLE_HID_PRESSURE__

    #if __ENABLE_HID_HUMIDITY__ // {
    HID_SENSOR_HYG,
    #endif // } __ENABLE_HID_HUMIDITY__

    #if __ENABLE_HID_TEMPERATURE__ // {
    HID_SENSOR_TEMP,
    #endif // } __ENABLE_HID_TEMPERATURE__

    #if __ENABLE_HID_PROXIMITY__ // {
    HID_SENSOR_PROX,
    #endif // } __ENABLE_HID_PROXIMITY__

    #if __ENABLE_HID_UV__ // {
    HID_SENSOR_UV,
    #endif // } __ENABLE_HID_UV__

    #if __ENABLE_HID_ANDROID__ // {
    HID_SENSOR_GRA,
    HID_SENSOR_LACC,
    #endif // } __ENABLE_HID_ANDROID__

    HID_SENSOR_MAX
};

// HID sensor index
#define HID_SENSOR_IDX(x)       ((x) - 1)  

// REPORT ID
#define HID_TP_POINTER          0x81
#define HID_TP_VENDER_USAGE_2   0x09
#define HID_TP_VENDER_USAGE_3   0x0A
#define HID_TP_VENDER_USAGE_4   0x0B
#define HID_TP_VENDER_USAGE_5   0x0C
#define HID_TP_VENDER_USAGE_6   0x0F

enum _HID_STATUS_FLAG_
{
    HID_DRIVER_INIT,
    HID_STATUS_RESET,
    HID_STATUS_INIT,
    HID_STATUS_NORMAL,
    HID_STATUS_STOP,
    HID_STATUS_HCC,
    HID_STATUS_SENSOR_TIMEOUT,
    HID_STATUS_SENSOR_ERROR,
    HID_STATUS_POWEROFF,
};

//********************************************************************************
extern hal_mutex_t hid_mutex; 

#define HID_CREATE_MUTEX()  hal_create_mutex(&hid_mutex, "hid")
#define HID_GET_MUTEX()     do{while(HAL_SUCCESS != hal_wait_for_mutex(&hid_mutex, HAL_SUSPEND)); gu8HIDBufferUpdating = 1;}while(0)
#define HID_RELEASE_MUTEX() do{gu8HIDBufferUpdating = 0; hal_release_mutex(&hid_mutex);}while(0)

extern const UINT8 hid_report_descriptor[];

extern UINT16 AccelerometerX, AccelerometerY, AccelerometerZ, GyrometerX, GyrometerY, GyrometerZ, MagnetometerX, MagnetometerY, MagnetometerZ;
extern UINT16 Shake, NorthDegrees, InclinometerX, InclinometerY, InclinometerZ, OrientationQuaternionX, OrientationQuaternionY, OrientationQuaternionZ, OrientationQuaternionW;
extern UINT16 LightIlluminace, ColorTemp, ChromaticityX, ChromaticityY, LightCustom, HygValue, TempValue, ProxValue, UVValue, UVValue2;
extern UINT32 PressureBar;
extern UINT8  ProxOutOfRangeFlag;

extern UINT16 RotationMatrix[9];

extern UINT16 Accelerometer2[3];
extern SINT16 CustomAngle[3];

extern UINT16 GyrometerExt[3];

//  extern UINT16 ALS_Multiplier, ALS_Divisor;
#define ALS_Multiplier  (gstSensorStoredData.u16AlsMultiplier)
#define ALS_Divisor     (gstSensorStoredData.u16AlsDivisor)

#define guiHIDTimer     (gpSHDInfo->iHIDTimer)
#define gu8HIDStatus    (gpSHDInfo->u8HIDStatus)

extern volatile UINT8  gu8HIDBufferUpdating;

extern volatile UINT8  g_u8HIDCurID;
extern volatile UINT8  g_u8HIDNextCmdID;

extern volatile UINT8  gu8HIDINTC[];
extern volatile UINT32 gu32HIDInterval[];
extern volatile int    giHIDLastTimer[];
extern volatile T_HID_POWER_STATE gstHIDPowerState;
extern T_HID_COMMON_FEATURE gstHIDFeature[];

#define gusIdleThreshold            (gpSHDInfo->u16IdleThreshold)
#define gusHIDIdleState             (gpSHDInfo->u8HIDIdleState)
#define DEF_I2C_HID                 (gpSHDInfo->bIsI2CHID)
#define DEF_USB_HID                 (gpSHDInfo->bIsUSBHID)
#define I2C_HID_SLAVE_ADDRESS       (gpSHDInfo->pHSlvAddr[0])
#define HID_I2C_INFO                (gpSHDInfo->u8HIDI2CInfo)
#define HID_I2C_CH                  (gpSHDInfo->u8HIDI2CInfo & 0x0F)
#define HID_I2C_SWAP                ((gpSHDInfo->u8HIDI2CInfo & 0xF0) >> 4)
#define SENSOR_I2C_CH               (gpSHDInfo->u8SensorI2CCh)

//************************************
// HID over I2C debug Info
//************************************

//HID_DEBUG_PORT1: HID main status
//   0x01: before parsing command
//   0x02: after parsing command

//HID_DEBUG_PORT2: ISR status
//   0x01: Enter ISR
//   0x02: Exit ISR
//   0x03: Stop interrupt
//   0x04: IDW interrupt and stop flag
//   0x05: IDW interrupt and no stop flag
//   0x06: IDR interrupt and stop flag
//   0x07: IDR interrupt and write length = 0
//   0x08: IDR interrupt and write length != 0
//   0x09: Enter do_write_command
//   0x0A: Set report command
//   0x0B: Set power
//   0x0C: Reset comamnd

//HID_DEBUG_PORT3: parsing command status
//   0x01: HID task resume in parsing_cmd
//   0x02: HID task suspend in parsing_cmd
//   0x03: HID descriptor command
//   0x04: Report descriptor command
//   0x05: Get report command
//   0x06: Input report command

//HID_DEBUG_PORT4: Set feature error status
//   HID report ID   (bit 4~7)
//   reporting state (bit 0~3)

//HID_DEBUG_PORT5: Set feature error status
//   HID report ID   (bit 4~7)
//   power state     (bit 0~3)

//HID_DEBUG_PORT6: reserved
//HID_DEBUG_PORT7: reserved

#define HID_DEBUG_PORT1(_X_)        (gpSHDInfo->pHDbgMsg[0] = (_X_))
#define HID_DEBUG_PORT2(_X_)        (gpSHDInfo->pHDbgMsg[1] = (_X_))
#define HID_DEBUG_PORT3(_X_)        (gpSHDInfo->pHDbgMsg[2] = (_X_))
#define HID_DEBUG_PORT4(_X_)        (gpSHDInfo->pHDbgMsg[3] = (_X_))
#define HID_DEBUG_PORT5(_X_)        (gpSHDInfo->pHDbgMsg[4] = (_X_))
#define HID_DEBUG_PORT6(_X_)        (gpSHDInfo->pHDbgMsg[5] = (_X_))
#define HID_DEBUG_PORT7(_X_)        (gpSHDInfo->pHDbgMsg[6] = (_X_))

#define HID_DEBUG_SET_FLAG()        do{(gpSHDInfo->pHDbgMsg[7])++;}while(0)

#define HID_DEBUG_CLEAR()           do{memset((UINT8*)(gpSHDInfo->pHDbgMsg), 0x00, 8);}while(0)

//************************************
//  Buffer 
//************************************
#define I2C_D_SENSOR_MAX_NUM        4
#define I2C_E_SENSOR_MAX_NUM        4    
#define I2C_F_SENSOR_MAX_NUM        4
#define I2C_SENSOR_MAX_NUM          (I2C_D_SENSOR_MAX_NUM + I2C_E_SENSOR_MAX_NUM + I2C_F_SENSOR_MAX_NUM)
#define SENSOR_BUFFER_SIZE          80
#define I2C_CMD_MAX_LEN             16
#define AUTO_MODE_BUF_SIZE          (I2C_CMD_MAX_LEN * I2C_SENSOR_MAX_NUM)

#define HID_CUSTOM_CMD_LENGTH       0x10

#define HID_WRITE_BUF_SIZE          16
#define HID_READ_BUF_SIZE           (sizeof(TFRInfo) + 2 + 7) // max feature report length among all sensor
#define HID_INPUT_REPORT_SIZE       (sizeof(TIRInfo) + 2)

//****************************************************************************
//  Structure Declaration
//****************************************************************************
typedef struct _T_SENSOR_HUB_DEBUG_INFO_
{
    //********************************** 0x0000
    UINT8   u8HIDStatus;                                // HID Init Status
    UINT8   u8HIDIdleState;                             // HID Idle State
    UINT16  u16IdleThreshold;                           // HID Idle Threshold
    int     iHIDTimer;                                  // HID Timer
    UINT8   bIsI2CHID;                                  // I2C over HID --> 1:Enable, 0:Disable
    UINT8   bIsUSBHID;                                  // USB over HID --> 1:Enable, 0:Disable
    UINT8   u8HIDI2CInfo;                               // HID SMBus Channel No.
    UINT8   u8SensorI2CCh;                              // Sensor SMBus Channel No.
    UINT32  u32FusionVer;                               // fusion version    
    //********************************** 0x0010
    UINT8   pHSlvAddr[4];                               // HID slave address
    UINT8   pSSlvAddr[I2C_SENSOR_MAX_NUM];              // sensor slave address
    //********************************** 0x0020
    UINT8   pHDbgMsg[8];                                // HID debug message
    UINT8   pSDbgMsg[8];                                // Sensor debug message
    UINT8   pUSBDbgMsg[8];                              // USB debug message
    UINT8   pI2CDbgMsg[8];                              // I2C debug message
    //********************************** 0x0040
    DWORD   u32Sensor_API_ID_Acc;                       // accelerometer
    DWORD   u32Sensor_API_ID_Acc2;                      // accelerometer2
    DWORD   u32Sensor_API_ID_Ambi;                      // ambientlight
    DWORD   u32Sensor_API_ID_Mag;                       // compass
    DWORD   u32Sensor_API_ID_Gyro;                      // gyrometer
    DWORD   u32Sensor_API_ID_Pres;                      // pressure
    DWORD   u32Sensor_API_ID_Hyg;                       // humidity
    DWORD   u32Sensor_API_ID_Temp;                      // temperature
    DWORD   u32Sensor_API_ID_Prox;                      // proximity
    DWORD   u32Sensor_API_ID_UV;                        // uv
    DWORD   u32Sensor_API_ID_Reserved[2];               // reserved
    //********************************** 0x0070    
    UINT8   u8DbgRTOS[16];                              // RTOS Debug message
    //********************************** 0x0080    
    UINT8   mSensorBuf[SENSOR_BUFFER_SIZE];             // seosor fusion buffer
    //********************************** 0x00d0
    UINT8   mCmdBuf[AUTO_MODE_BUF_SIZE];                // auto mode buffer
    //********************************** 0x0190
    UINT8   u8HccBuf[HID_CUSTOM_CMD_LENGTH];            // hid custom cmd buffer
    //********************************** 0x01a0
    //  No-fix memory location
    //**********************************
    UINT16  u16SensorPeriod[SENSOR_PERIOD_CNT];         // Sensor Period
}T_SENSOR_HUB_DEBUG_INFO;
typedef volatile T_SENSOR_HUB_DEBUG_INFO    TSHD_INFO;
typedef TSHD_INFO*                          LPSHD_INFO;
//TSHD_INFO   mCalSize;                   //  Calculation Size <= 0x200

#define gpSHDInfo                        ((LPSHD_INFO)HID_FUSION_BUFFER)

//********************************************************************************
//  external function
//********************************************************************************
extern UINT8 hid_init(void);
extern void hid_service(void);
extern void hid_data_set(UINT8 *pBuf);
extern int hid_get_feature_report(UINT8 u8ID, UINT8 *pBuf);
extern void hid_set_feature_report(UINT8 u8ID, UINT8 *pBuf, UINT16 u16BufSize);
extern BOOL hid_check_sensitivity(UINT8 u8ID);
extern int hid_get_input_report(UINT8 u8ID, UINT8 *pBuf);
extern UINT32 hid_get_min_report_interval(void);
extern void hid_init_time_interval(void);
extern UINT8 hid_check_power_mode(void);
extern UINT8 hid_check_bus_busy(void);

extern void hid_tiny_init(void);
extern void hid_update_last_data(void);
extern void hid_update_sensor_state(UINT8 u8ID, UINT8 u8State);
extern void hid_reset_sensor_state(void);
extern UINT8 hid_check_next_cmd(void);

extern void hid_restore_config(UINT8 u8Idx);
extern void hid_save_config(UINT8 u8Idx);

//********************************************************************************
#endif // __HID_COMMON_H__

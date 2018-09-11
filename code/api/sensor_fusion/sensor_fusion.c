#include "sensor_fusion.h"
#include "i2c.h"
#include "i2c_drv.h"
#include "ext_timer.h"
#include "hid_common.h"
#include "hid_custom_cmd.h"
#include "core_memory.h"
#include "hid_hal.h"
#include "core_timer.h"
#include "core_irq.h"


// *****************************************************************************
//  Sensor I2C Read Mode Config
// *****************************************************************************
#define _ENABLE_SPECIAL_READ_                                                  \
   (defined(_ENABLE_SPECIAL_READ_ACC_)  || defined(_ENABLE_SPECIAL_READ_MAG_)  \
 || defined(_ENABLE_SPECIAL_READ_GYO_)  || defined(_ENABLE_SPECIAL_READ_ALS_)  \
 || defined(_ENABLE_SPECIAL_READ_ACC2_) || defined(_ENABLE_SPECIAL_READ_PRS_ ) \
 || defined(_ENABLE_SPECIAL_READ_HYG_)  || defined(_ENABLE_SPECIAL_READ_TEMP_) \
 || defined(_ENABLE_SPECIAL_READ_PROX_) || defined(_ENABLE_SPECIAL_READ_UV_))

#ifdef _ENABLE_SPECIAL_READ_ACC_ // {
#define IF_I2C_MODE_MATCH_ACC(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_ACC(mode)    
#endif // } _ENABLE_SPECIAL_READ_ACC_

#ifdef _ENABLE_SPECIAL_READ_MAG_ // {
#define IF_I2C_MODE_MATCH_MAG(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_MAG(mode)    
#endif // } _ENABLE_SPECIAL_READ_MAG_

#ifdef _ENABLE_SPECIAL_READ_GYO_ // {
#define IF_I2C_MODE_MATCH_GYO(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_GYO(mode)    
#endif // } _ENABLE_SPECIAL_READ_GYO_

#ifdef _ENABLE_SPECIAL_READ_ALS_ // {
#define IF_I2C_MODE_MATCH_ALS(mode)     if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_ALS(mode)
#endif // } _ENABLE_SPECIAL_READ_ALS_

#ifdef _ENABLE_SPECIAL_READ_ACC2_ // {
#define IF_I2C_MODE_MATCH_ACC2(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_ACC2(mode)    
#endif // } _ENABLE_SPECIAL_READ_ACC2_

#ifdef _ENABLE_SPECIAL_READ_PRS_ // {
#define IF_I2C_MODE_MATCH_PRS(mode)     if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_PRS(mode)
#endif // } _ENABLE_SPECIAL_READ_PRS_

#ifdef _ENABLE_SPECIAL_READ_HYG_ // {
#define IF_I2C_MODE_MATCH_HYG(mode)     if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_HYG(mode)
#endif // } _ENABLE_SPECIAL_READ_HYG_

#ifdef _ENABLE_SPECIAL_READ_TEMP_ // {
#define IF_I2C_MODE_MATCH_TEMP(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_TEMP(mode)
#endif // } _ENABLE_SPECIAL_READ_TEMP_

#ifdef _ENABLE_SPECIAL_READ_PROX_ // {
#define IF_I2C_MODE_MATCH_PROX(mode)    if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_PROX(mode)
#endif // } _ENABLE_SPECIAL_READ_PROX_

#ifdef _ENABLE_SPECIAL_READ_UV_ // {
#define IF_I2C_MODE_MATCH_UV(mode)      if((mode) == 1)
#else
#define IF_I2C_MODE_MATCH_UV(mode)
#endif // } _ENABLE_SPECIAL_READ_UV_

// *****************************************************************************
//  Global Variable
// *****************************************************************************
#define Sensor_API_ID_Accelerometer     (gpSHDInfo->u32Sensor_API_ID_Acc)
#define Sensor_API_ID_Accelerometer2    (gpSHDInfo->u32Sensor_API_ID_Acc2)
#define Sensor_API_ID_Compass           (gpSHDInfo->u32Sensor_API_ID_Mag)
#define Sensor_API_ID_Gyrometer         (gpSHDInfo->u32Sensor_API_ID_Gyro)
#define Sensor_API_ID_Ambientlight      (gpSHDInfo->u32Sensor_API_ID_Ambi)
#define Sensor_API_ID_Pressure          (gpSHDInfo->u32Sensor_API_ID_Pres)
#define Sensor_API_ID_Hygrometer        (gpSHDInfo->u32Sensor_API_ID_Hyg)
#define Sensor_API_ID_Temperature       (gpSHDInfo->u32Sensor_API_ID_Temp)
#define Sensor_API_ID_Proximity         (gpSHDInfo->u32Sensor_API_ID_Prox)
#define Sensor_API_ID_Ultraviolet       (gpSHDInfo->u32Sensor_API_ID_UV)


const UINT8 SENSOR_RODATA gu8SensorParaDefault[] = 
{
//================================================================================================
// For Yoga4 Sensor default value
//================================================================================================
#if (defined(__ENABLE_AKM_PG_LIB__) || defined(__ENABLE_AKM2_LIB__))// {
#if _LISZT_
	0x80, 0x83, 0x78, 0x00, 0x05, 0x00, 0x01, 0x00, 0x27, 0x00, 0x21, 0x00, 0xFA, 0xFF, 0xFC, 0xFF, 
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x99, 0x54, 0x00, 0x20, 0x52, 0xBA, 0xAE, 
	0x31, 0xFF, 0xFF, 0x60, 0xD6, 0x37, 0xE7, 0x55, 0x26, 0xCE, 0xFF, 0xF2, 0xFF, 0xFF, 0x7F, 0x9A, 
	0xBF, 0xFC, 0xFF, 0xFF, 0x09, 0x26, 0xFF, 0x03, 0x00, 0xA5, 0x02, 0x29, 0x05, 0x00, 0x00, 0x00, 
	0x00, 0x27, 0x03, 0xA8, 0x05, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x0C, 0x59, 0x0B, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x01, 0xFD, 0xFF, 
	0xF9, 0xFF, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00
#else
	0x80, 0x83, 0x78, 0x00, 0x07, 0x00, 0x01, 0x00, 0x21, 0x05, 0x21, 0x00, 0x25, 0x00, 0x27, 0x00, 
	0xF8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x99, 0x54, 0x00, 0x20, 0x52, 0xBA, 0xAE, 
	0x31, 0xFF, 0xFF, 0x60, 0xD6, 0x37, 0xE7, 0x55, 0x26, 0xCE, 0xFF, 0xF2, 0xFF, 0xFF, 0x7F, 0x9A, 
	0xBF, 0xFC, 0xFF, 0xFF, 0x09, 0x26, 0xFF, 0x03, 0x00, 0x4E, 0x00, 0xDA, 0xFD, 0x00, 0x00, 0x00, 
	0x00, 0x0D, 0x01, 0x6C, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x54, 0xFB, 0x91, 0xF5, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0x07, 0x00, 
	0x0E, 0x00, 0xFC, 0xFF, 0x01, 0x00, 0x01, 0x00
#endif

#else

#if _LISZT_
	0x80, 0x83, 0x5C, 0x00, 0x05, 0x00, 0x01, 0x00, 0x27, 0x21, 0x21, 0x00, 0xFF, 0xFF, 0x10, 0x00, 
	0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x38, 0x00, 0xD6, 0x00, 0xFF, 0x00, 
	0xFD, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x62, 0x00, 0x86, 0x00, 0xD9, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x01, 0x51, 0x00, 
	0x27, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00
#else	// maksim
	0x80, 0x83, 0x5C, 0x00, 0x07, 0x00, 0x01, 0x00, 0x21, 0x05, 0x21, 0x00, 0x25, 0x00, 0x27, 0x00, 
	0xF8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x38, 0x00, 0x00, 0x00, 0x29, 0x00, 
	0x35, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0xFD, 0xFF, 0xCB, 0xFF, 0x25, 0xFE, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x00, 0x5C, 0x00, 
	0x03, 0x01, 0x07, 0x00, 0x0E, 0x00, 0xFC, 0xFF, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x1F, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xCC, 0xFF, 0xFF, 0xFF, 0x26, 0xFE, 0xFF, 0xFF, 
	0xFD, 0xFF, 0xFF, 0xFF, 0xCB


#endif

#endif // } __ENABLE_AKM_PG_LIB__
};


// *****************************************************************************
//  Macro
// *****************************************************************************
#define SENSOR_CREATE_MUTEX()  hal_create_mutex(&sensor_mutex, "sensor")
#define SENSOR_GET_MUTEX(_x)                                                   \
    do{                                                                        \
        SENSOR_MUTEX_WAIT = (_x);                                              \
        while(HAL_SUCCESS != hal_wait_for_mutex(&sensor_mutex, HAL_SUSPEND));  \
        SENSOR_MUTEX_WAIT = 0x00;                                              \
        SENSOR_MUTEX_CUR = (_x);                                               \
    }while(0)
#define SENSOR_RELEASE_MUTEX()                                                 \
    do{                                                                        \
        SENSOR_MUTEX_CUR = 0x00;                                               \
        hal_release_mutex(&sensor_mutex);                                      \
    }while(0)

#define I2C_READ(sa, addr, buf, cnt) \
    i2c_drv_data_read(ghI2CDrv, (sa), (addr), (buf), (cnt))
#define I2C_WRITE(sa, addr, buf, cnt) \
    i2c_drv_data_write(ghI2CDrv, (sa), (addr), (buf), (cnt))
#define I2C_BLOCK_READ(sa, buf_i, cnt_i, buf_o, cnt_o) \
    i2c_drv_data_block_read(ghI2CDrv, (sa), (buf_i), (cnt_i), (buf_o), (cnt_o))
#define I2C_BLOCK_WRITE(sa, buf, cnt) \
    i2c_drv_data_block_write(ghI2CDrv, (sa), (buf), (cnt))
#define I2C_CHECK_EXIST(sa) \
    i2c_check_slave_exist(SENSOR_I2C_CH, (sa))
#define I2C_RESET_CMD() \
    i2c_drv_reset_cmd(SENSOR_I2C_CH)

#ifdef DEF_SENSOR_I2C_ERR_HANDLER // {
#define I2C_GET_ONE_SHOT_DATA(cmd_idx, sensor_idx, cmd_flag)    \
    do{                                                 \
        SENSOR_FUSION_DBG_I2C((gppI2CCmd[cmd_idx]->u8SlaveAddr|0x01), \
                               gppI2CCmd[cmd_idx]->u8Addr, 0xCC);     \
        gucSensorStatusBusy = 1;                        \
        i2c_drv_fire_one_shot(SENSOR_I2C_CH, cmd_idx);  \
        gucSensorStatusBusy = 0;                        \
        if(gu8HIDStatus == HID_STATUS_SENSOR_TIMEOUT) { \
                SENSOR_FUSION_DBG_I2C_RTN(0xFF);        \
                SENSOR_RELEASE_MUTEX();                 \
                return;                                 \
        }                                               \
        if((gucI2CSensorErrStatus && gpSensorMustInit[sensor_idx]) \
            || sensor_error_status_check(sensor_idx)) { \
            cmd_flag = 0;                               \
        }                                               \
        else {                                          \
            i2c_drv_read_sram_data(gppI2CCmd[cmd_idx]); \
        }                                               \
        SENSOR_FUSION_DBG_I2C_RTN(0x00);                \
    }while(0)
#else
#define I2C_GET_ONE_SHOT_DATA(cmd_idx, sensor_idx, cmd_flag)    \
    do{                                                 \
        SENSOR_FUSION_DBG_I2C((gppI2CCmd[cmd_idx]->u8SlaveAddr|0x01), \
                               gppI2CCmd[cmd_idx]->u8Addr, 0xCC);     \
        gucSensorStatusBusy =  1;                       \
        i2c_drv_fire_one_shot(SENSOR_I2C_CH, cmd_idx);  \
        gucSensorStatusBusy = 0;                        \
        if(gu8HIDStatus == HID_STATUS_SENSOR_TIMEOUT) { \
                SENSOR_FUSION_DBG_I2C_RTN(0xFF);        \
                SENSOR_RELEASE_MUTEX();                 \
                return;                                 \
        }                                               \
        i2c_drv_read_sram_data(gppI2CCmd[cmd_idx]);     \
        SENSOR_FUSION_DBG_I2C_RTN(0x00);                \
    }while(0)
#endif  // }  DEF_SENSOR_I2C_ERR_HANDLER   

#ifndef EC_FEATURE_ONLY_SUPPORT_FUSION  // { RTOS only
#define SENSOR_INIT_FAIL_HANDLER(idx, FailCode, NextStep)   \
    do{                                                     \
        if(gpSensorMustInit[(idx)])                         \
        {                                                   \
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_FAIL;  \
            SENSOR_INIT_STEP_FAIL_CAUSE = (FailCode);       \
            I2C_DEBUG_PORT7((FailCode));                    \
        }                                                   \
        else                                                \
        {                                                   \
            SENSOR_INIT_STEP = (NextStep);                  \
        }                                                   \
    }while(0)
#else
#define SENSOR_INIT_FAIL_HANDLER(idx, FailCode, NextStep)   \
    do{                                                     \
        SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_FAIL;      \
        SENSOR_INIT_STEP_FAIL_CAUSE = (FailCode);           \
        I2C_DEBUG_PORT7((FailCode));                        \
    }while(0)
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION

#define RESET_FUSION()                                          \
    do{                                                         \
        gu8HIDStatus = HID_STATUS_INIT;                         \
        SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;    \
    }while(0)

#ifdef __HID_FUSION_FUN__ // {
#define RESUME_FUSION_TASK()    do{hal_resume_task(tthi_FUSION);}while(0)
#else
#define RESUME_FUSION_TASK()
#endif // } __HID_FUSION_FUN__

#define CLEAR_IMMU_CACHE()  \
    do{                     \
        DisableGlobalInt(); \
        immu_reset_ext();   \
        EnableGlobalInt();  \
    }while(0)  

#define UartPutChar(c)  debug_putc(c)
#define ClrScr()                        \
    do{                                 \
        UartPutChar(0x1b);  /*ESC*/     \
        UartPutChar(0x5b);  /*'['*/     \
        UartPutChar(0x32);  /*'2'*/     \
        UartPutChar(0x4a);  /*'J'*/     \
        UartPutChar(0x1b);  /*ESC*/     \
        UartPutChar(0x38);  /*'8'*/     \
    }while(0)

// *****************************************************************************
//  External Global Variable
// *****************************************************************************
SENSOR_BSS hal_mutex_t sensor_mutex;   // mutex for protecting resources
SENSOR_BSS T_SENSOR_STORED_DATA gstSensorStoredData;
SENSOR_DATA T_FUSION_CONFIG gstFConfig = {
                                          .Enable_Android = DEF_ANDROID, 
                                          .Enable_6D = DEF_FUSION_6D, 
                                          .Reserved = 0
                                          };
SENSOR_BSS T_SENSOR_INIT_DONE gstSensorInitDone;
SENSOR_DATA uchar_8 gucI2CSensorErrStatus = 0;

// *****************************************************************************
//  Internal Global Variable
// *****************************************************************************
#define SENSOR_TOTAL_CNT    \
    (_ENABLE_ACC_ + _ENABLE_MAG_ + _ENABLE_GYO_ + _ENABLE_ALS_ + _ENABLE_ACC2_ \
   + _ENABLE_PRS_ + _ENABLE_HYG_ + _ENABLE_TEMP_ + _ENABLE_PROX_ + _ENABLE_UV_)

#define SENSOR_I2C_CMD_CNT  \
    (_ENABLE_ACC_ + _ENABLE_MAG_ + _ENABLE_GYO_ +                      \
    (_ENABLE_ALS_  || _ENABLE_ACC2_ || _ENABLE_PRS_ || _ENABLE_HYG_ || \
     _ENABLE_TEMP_ || _ENABLE_PROX_ || _ENABLE_UV_))

#define SENSOR_I2C_CMD_ALT  (SENSOR_I2C_CMD_CNT - 1)

SENSOR_BSS static HANDLE ghI2CDrv;
SENSOR_BSS static UINT8  gu8I2CCmdCnt;
SENSOR_BSS static T_I2C_CMD gpI2CCmd[SENSOR_TOTAL_CNT];
SENSOR_BSS static P_I2C_CMD gppI2CCmd[SENSOR_I2C_CMD_CNT];

#ifndef EC_FEATURE_ONLY_SUPPORT_FUSION  // { RTOS only
SENSOR_RODATA const UINT8 gpSensorMustInit[] = {
    #if _ENABLE_ACC_
    ACC_MUST_INIT,
    #endif
    
    #if _ENABLE_MAG_
    MAG_MUST_INIT,
    #endif

    #if _ENABLE_GYO_
    GYO_MUST_INIT,
    #endif

    #if _ENABLE_ALS_
    ALS_MUST_INIT,
    #endif

    #if _ENABLE_ACC2_
    ACC2_MUST_INIT,
    #endif

    #if _ENABLE_PRS_
    PRS_MUST_INIT,
    #endif

    #if _ENABLE_HYG_
    HYG_MUST_INIT,
    #endif

    #if _ENABLE_TEMP_
    TEMP_MUST_INIT,
    #endif    

    #if _ENABLE_PROX_
    PROX_MUST_INIT,
    #endif     

    #if _ENABLE_UV_
    UV_MUST_INIT,
    #endif
    };
#endif  // } EC_FEATURE_ONLY_SUPPORT_FUSION

#if defined(_ENABLE_MULTI_ACC_) || defined(_ENABLE_MULTI_ACC2_) // {
SENSOR_DATA
#else
SENSOR_RODATA const 
#endif // } 
UINT8 gpSensorI2CID[] = {
    #if _ENABLE_ACC_
     #ifdef _ENABLE_MULTI_ACC_ // {
    0x00,
     #else
    SLAVE_ADDRESS_ACC,
     #endif // } _ENABLE_MULTI_ACC_
    #endif
    
    #if _ENABLE_MAG_
    SLAVE_ADDRESS_MAG,
    #endif

    #if _ENABLE_GYO_
    SLAVE_ADDRESS_GYO,
    #endif

    #if _ENABLE_ALS_
    SLAVE_ADDRESS_ALS,
    #endif

    #if _ENABLE_ACC2_
     #ifdef _ENABLE_MULTI_ACC2_ // {
    0x00,
     #else
    SLAVE_ADDRESS_ACC2,
     #endif // } _ENABLE_MULTI_ACC2_
    #endif

    #if _ENABLE_PRS_
    SLAVE_ADDRESS_PRS,
    #endif

    #if _ENABLE_HYG_
    SLAVE_ADDRESS_HYG,
    #endif

    #if _ENABLE_TEMP_
    SLAVE_ADDRESS_TEMP,
    #endif      

    #if _ENABLE_PROX_
    SLAVE_ADDRESS_PROX,
    #endif  

    #if _ENABLE_UV_
    SLAVE_ADDRESS_UV,
    #endif
    };    

#define gpSensorBuffer       ((LPUINT8)(gpSHDInfo->mSensorBuf))
#define gpSensorDC           ((P_SENSOR_BUFFER_WIN)(gpSHDInfo->mSensorBuf))

SENSOR_DATA static UINT16 gu16FusionFreq = FUSION_FREQUENCY;
SENSOR_DATA static uint_16 gusSensorErrStatus = 0;
SENSOR_DATA static UINT8 gu8SensorDataSaveFlag = 1;
SENSOR_DATA static UINT8 gu8SensorTimeoutCnt = 0;
SENSOR_DATA static UINT8 gu8SensorErrorCnt  = 0;	// alex copy here
SENSOR_DATA static UINT8 gu8I2CResetCnt = 0;

#if _ENABLE_ACC2_
SENSOR_BSS static UINT16 gu16AngleFreq;
SENSOR_DATA static char gcAngleDebounce = 0;
#endif

#define I2C_CMD_ACC          (gpI2CCmd[SENSOR_I2C_IDX_ACC])
#define I2C_CMD_MAG          (gpI2CCmd[SENSOR_I2C_IDX_MAG])
#define I2C_CMD_GYO          (gpI2CCmd[SENSOR_I2C_IDX_GYO])
#define I2C_CMD_ALS          (gpI2CCmd[SENSOR_I2C_IDX_ALS])
#define I2C_CMD_ACC2         (gpI2CCmd[SENSOR_I2C_IDX_ACC2])
#define I2C_CMD_PRS          (gpI2CCmd[SENSOR_I2C_IDX_PRS])
#define I2C_CMD_HYG          (gpI2CCmd[SENSOR_I2C_IDX_HYG])
#define I2C_CMD_TEMP         (gpI2CCmd[SENSOR_I2C_IDX_TEMP])
#define I2C_CMD_PROX         (gpI2CCmd[SENSOR_I2C_IDX_PROX])
#define I2C_CMD_UV           (gpI2CCmd[SENSOR_I2C_IDX_UV])

#define SENSOR_BUFFER_ACC    ((LPUINT8)(gpSensorDC->Acc))
#define SENSOR_BUFFER_MAG    ((LPUINT8)(gpSensorDC->Mag))
#define SENSOR_BUFFER_GYO    ((LPUINT8)(gpSensorDC->Gyo))
#define SENSOR_BUFFER_ALS    ((LPUINT8)(gpSensorDC->Alt))
#define SENSOR_BUFFER_ACC2   ((LPUINT8)(gpSensorDC->Alt))
#define SENSOR_BUFFER_PRS    ((LPUINT8)(gpSensorDC->Alt)) //need DWORD alignment
#define SENSOR_BUFFER_HYG    ((LPUINT8)(gpSensorDC->Alt))
#define SENSOR_BUFFER_TEMP   ((LPUINT8)(gpSensorDC->Alt))
#define SENSOR_BUFFER_PROX   ((LPUINT8)(gpSensorDC->Alt))
#define SENSOR_BUFFER_UV     ((LPUINT8)(gpSensorDC->Alt))

//******************************************************************************
// Function name:
//   _change_period()
// Description:
//   change fusion(I2C) and sensor period
// Arguments:
//   u16Period (in): target period (ms)
// Return Values:
//   n/a
// Note:
//   internal function
//******************************************************************************
static void SENSOR_CODE _change_period(UINT16 u16Period)
{
    UINT8 i, u8FreqIdx;

    if(u16Period)
    {
        u8FreqIdx = 0;

        if(u16Period > FUSION_PERIOD_MAX){
            u16Period = FUSION_PERIOD_MAX;
        }
        
        gu16FusionFreq = 1000 / u16Period;

        for(i = 0; i < SENSOR_I2C_CMD_CNT; i++){
            gpI2CCmd[i].u16Period = u16Period;
        }
        
        #if _ENABLE_ACC_ // {
        if(do_data_rate_check_acc(gu16FusionFreq, &u8FreqIdx) != 1)
        {
            do_data_rate_switch_acc(u8FreqIdx);

            SENSOR_FUSION_DBG_INFO("ACC data rate change.\r\n");
        }
        #endif // } _ENABLE_ACC_
        
        #if _ENABLE_MAG_ // {
        if(do_data_rate_check_mag(gu16FusionFreq, &u8FreqIdx) != 1)
        {
            do_data_rate_switch_mag(u8FreqIdx);
	
            SENSOR_FUSION_DBG_INFO("MAG data rate change.\r\n");
        }
        #endif // } _ENABLE_MAG_
        
        #if _ENABLE_GYO_ // {
        if(do_data_rate_check_gyo(gu16FusionFreq, &u8FreqIdx) != 1)
        {
            do_data_rate_switch_gyo(u8FreqIdx);
            
            SENSOR_FUSION_DBG_INFO("GYO data rate change.\r\n");
        }
        #endif // } _ENABLE_GYO_
    }
}

//******************************************************************************
// Function name:
//   sensor_log()
// Description:
//   log for debug
// Arguments:
//   u8MsgCode (in): message code
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************

#define SensorLog32Addr  0x00080D00
#define SensorLog32Range 0x7F

#define SensorLogAddr  0x00080D80
#define SensorLogRange 0x7F

void SENSOR_CODE_L sensor_log32(UINT32 u32MsgCode)
{
    #if defined(SensorLog32Addr) && defined(SensorLog32Range) // {
    UINT8 *pCurLogIdx;

    pCurLogIdx = (UINT8*)(SensorLog32Addr + SensorLog32Range);
    if(*pCurLogIdx == SensorLog32Range-3)
    {
        memset((UINT8*)SensorLog32Addr, 0x00, (SensorLog32Range-1));
        *pCurLogIdx = 0;
    }
    
    *((UINT32*)(SensorLog32Addr + (*pCurLogIdx))) = u32MsgCode;
    
    *pCurLogIdx += 4;

    #endif // }
}

#if defined(SensorLogAddr) && defined(SensorLogRange) // {
UINT8 u8LastMsgCode;
#endif // }    
void SENSOR_CODE_L sensor_log(UINT8 u8MsgCode)
{
    #if defined(SensorLogAddr) && defined(SensorLogRange) // {
    UINT8 *pCurLogIdx;

    #ifndef EC_FEATURE_ONLY // {
    vPortEnterCritical();
    #endif // } EC_FEATURE_ONLY
    
    pCurLogIdx = (UINT8*)(SensorLogAddr + SensorLogRange);

    if(u8LastMsgCode != u8MsgCode) 
    {
        u8LastMsgCode = u8MsgCode;
    }
    else 
    {
        return;
    }
    
    if(*pCurLogIdx == SensorLogRange)
    {
        //memset((UINT8*)SensorLogAddr, 0x00, (SensorLogRange-1));
        *pCurLogIdx = 0;
    }
    
    *((UINT8*)(SensorLogAddr + (*pCurLogIdx))) = u8MsgCode;
    
    *pCurLogIdx += 1;

    #ifndef EC_FEATURE_ONLY // {
    vPortExitCritical();
    #endif // } EC_FEATURE_ONLY
    
    #endif // }
}

//******************************************************************************
// Function name:
//   sensor_get_freq_idx()
// Description:
//   get the frequency index in "pFreqBuffer" which is >= "u16TargetFreq"
// Arguments:
//   pFreqBuffer   (in): array of frequency, 
//                       the value is from slowest to fastest, and the last is 0
//   u16TargetFreq (in): target frequency
// Return Values:
//   frequency index
// Note:
//   n/a
//******************************************************************************
int SENSOR_CODE sensor_get_freq_idx(const UINT16 *pFreqBuffer, 
                                    UINT16 u16TargetFreq)
{
    int i = 0;

    while(pFreqBuffer[i] != 0)
    {
        if(pFreqBuffer[i++] >= u16TargetFreq)
        {
            break;
        }
    }
    i--;

//	    SENSOR_FUSION_DBG_INFO("Sensor freq: [%d:%d]\r\n", 
//	                           u16TargetFreq, pFreqBuffer[i]);

    return (i);
}

//******************************************************************************
// Function name:
//   sensor_sleep()
// Description:
//   task sleep function
// Arguments:
//   nMilliSec (in): sleep time (ms)
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE_L sensor_sleep(int nMilliSec)
{    
    hal_sleep(nMilliSec);
}

//******************************************************************************
// Function name:
//   sensor_delay_ms()
// Description:
//   delay function
// Arguments:
//   u32MilliSec (in): delay time (ms)
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE_L sensor_delay_ms(UINT32 u32MilliSec)
{
    #if 0
    hal_sleep(u32MilliSec);
    #else
    timer_init_ms(EXT_TIMER_8, ET_PSR_8M, FALSE, FALSE, u32MilliSec);
    timer_delay(EXT_TIMER_8);
    #endif
}

//******************************************************************************
// Function name:
//   sensor_get_ticks()
// Description:
//   get system tick count
// Arguments:
//   n/a
// Return Values:
//   current tick count
// Note:
//   n/a
//******************************************************************************
UINT32 SENSOR_CODE_L sensor_get_ticks(void)
{
    return hal_get_sys_ticks();
}

#ifdef DEF_SENSOR_I2C_ERR_HANDLER  // {
//******************************************************************************
// Function name:
//   sensor_slaveID_to_idx()
// Description:
//   get sensor slave ID index of all driver
// Arguments:
//   ucSlaveID (in): sensor slave ID
// Return Values:
//   slave ID index
// Note:
//   n/a
//******************************************************************************
UINT8 SENSOR_CODE_L sensor_slaveID_to_idx(UINT8 ucSlaveID)
{
    int i;
    for(i = 1; i < SENSOR_I2C_IDX_TOTAL; i++)    
    {
        if(ucSlaveID == gpSensorI2CID[i])
        {
            return i;    
        }    
    }
    return 0;
}

//******************************************************************************
// Function name:
//   sensor_error_status_check()
// Description:
//   check sensor status
// Arguments:
//   u8SensorIdx (in): sensor index
// Return Values:
//   0: normal
//   1: error
// Note:
//   for one shot mode only
//******************************************************************************
UINT8 SENSOR_CODE sensor_error_status_check(UINT8 u8SensorIdx)
{
    if(gucI2CSensorErrStatus)  // I2C disconnected
    {
        gusSensorErrStatus |= (1 << u8SensorIdx);
    }
    else
    {
        /*if((gusSensorErrStatus & (1 << u8SensorIdx)))    
        {
            gu8HIDStatus = HID_STATUS_INIT;
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
            gusSensorErrStatus &= ~(1 << u8SensorIdx);
            return 1;
        }*/
    }
    return 0;
}
#endif // } DEF_SENSOR_I2C_ERR_HANDLER

//******************************************************************************
// Function name:
//   sensor_check()
// Description:
//   check device exist or not
// Arguments:
//   u8SlaveAddr  (in) : device slave ID, 0x00 means scan all
//   pTotalSensor (out): total sensor count
//   pSensorAddr  (out): buffer of sensor slave address
// Return Values:
//   0: no sensor
//   other: 1st sensor exist -> 0x0001, 2nd sensor exist -> 0x0002...
// Note:
//   n/a
//******************************************************************************
UINT16 SENSOR_CODE_L sensor_check(UINT8 u8SlaveAddr, 
                                  UINT8 *pTotalSensor, UINT8 pSensorAddr[10])
{
    int i;
    UINT16 rtn = 0;

    memset(pSensorAddr, 0x00, 10);

    if(u8SlaveAddr == 0x00)
    {
        *pTotalSensor = SENSOR_TOTAL_CNT;

        for(i=0; i<SENSOR_TOTAL_CNT; i++)
        {
            if(i < 10)
                *(pSensorAddr + i) = gpSensorI2CID[i];
            
            if(I2C_CHECK_EXIST(gpSensorI2CID[i]))
                rtn |= (1 << i);
        }
    }
    else
    {
        *pTotalSensor = 0;
        
        rtn = I2C_CHECK_EXIST(u8SlaveAddr);
    }

    return rtn;
}

//******************************************************************************
// Function name:
//   sensor_read()
// Description:
//   I2C read function for sensor
// Arguments:
//   u8SlaveAddr (in) : sensor slave address
//   u8Addr      (in) : sensor register address
//   pBuf        (out): data buffer
//   u8Cnt       (in) : byte count of data to read
// Return Values:
//    0: success
//   -1: fail
// Note:
//   n/a
//******************************************************************************
int SENSOR_CODE sensor_read(UINT8 u8SlaveAddr, UINT8 u8Addr, 
                            UINT8 *pBuf, UINT8 u8Cnt)
{
    int rtn;

    SENSOR_FUSION_DBG_I2C((u8SlaveAddr|0x01), u8Addr, 0xCC);

    rtn = I2C_READ(u8SlaveAddr, u8Addr, pBuf, u8Cnt);
    
    if(rtn)
    {
        if(gu8I2CResetCnt++ < MAX_I2C_RESET_CNT)
        {
            i2c_channel_reset(SENSOR_I2C_CH);
        }     
        SENSOR_FUSION_DBG_ERR("sensor_read error: %Xh, %Xh, %Xh...\r\n", 
                              u8SlaveAddr, u8Addr, pBuf[0]);   
    }
    else
    {
        gu8I2CResetCnt = 0;
    }
    
    #ifdef DEF_SENSOR_I2C_ERR_HANDLER  // {
    UINT8 ucI2CIdx = 0; 
    if(gucI2CSensorErrStatus)
    {
        ucI2CIdx = sensor_slaveID_to_idx(u8SlaveAddr);
        if(ucI2CIdx)
        {
            sensor_error_status_check(ucI2CIdx);    
        }
    }
    #endif // } DEF_SENSOR_I2C_ERR_HANDLER
    
    SENSOR_FUSION_DBG_I2C_RTN((UINT8)rtn);

    //SENSOR_FUSION_DBG_INFO("R: %02Xh, %02Xh, %02Xh\r\n", 
    //                       u8SlaveAddr, u8Addr, pBuf[0]);
    
    return rtn;
}

//******************************************************************************
// Function name:
//   sensor_write()
// Description:
//   I2C write function for sensor
// Arguments:
//   u8SlaveAddr (in): sensor slave address
//   u8Addr      (in): sensor register address
//   pBuf        (in): data buffer
//   u8Cnt       (in): byte count of data to write
// Return Values:
//    0: success
//   -1: fail
// Note:
//   n/a
//******************************************************************************
int SENSOR_CODE sensor_write(UINT8 u8SlaveAddr, UINT8 u8Addr, 
                             UINT8 *pBuf, UINT8 u8Cnt)
{
    int rtn;
    volatile int iDelay = 0x20;
    
    SENSOR_FUSION_DBG_I2C(u8SlaveAddr, u8Addr, 0xCC);

    //SENSOR_FUSION_DBG_INFO("W: %02Xh, %02Xh, %02Xh\r\n", 
    //                       u8SlaveAddr, u8Addr, pBuf[0]);

    rtn = I2C_WRITE(u8SlaveAddr, u8Addr, pBuf, u8Cnt);

    if(rtn)
    {
        if(gu8I2CResetCnt++ < MAX_I2C_RESET_CNT)
        {
            i2c_channel_reset(SENSOR_I2C_CH);
        }
        SENSOR_FUSION_DBG_ERR("sensor_write error: %Xh, %Xh, %Xh...\r\n", 
                              u8SlaveAddr, u8Addr, pBuf[0]);   
    }
    else
    {
        gu8I2CResetCnt = 0;
    }    
    
    #ifdef DEF_SENSOR_I2C_ERR_HANDLER  // {
    UINT8 ucI2CIdx = 0; 
    if(gucI2CSensorErrStatus)
    {
        ucI2CIdx = sensor_slaveID_to_idx(u8SlaveAddr);
        if(ucI2CIdx)
        {
            sensor_error_status_check(ucI2CIdx);    
        }
    }
    #endif // } DEF_SENSOR_I2C_ERR_HANDLER
    
    SENSOR_FUSION_DBG_I2C_RTN((UINT8)rtn);
    while(iDelay--);   // Delay
    
    return rtn;
}

//******************************************************************************
// Function name:
//   sensor_block_read()
// Description:
//   I2C block read function for sensor
// Arguments:
//   u8SlaveAddr (in) : sensor slave address
//   pBufIn      (in) : i2c write buffer
//   u8CntIn     (in) : byte count of data to write
//   pBufOut     (out): i2c read buffer
//   u8CntOut    (in) : byte count of data to read
// Return Values:
//    0: success
//   -1: fail
// Note:
//   n/a
//******************************************************************************
int SENSOR_CODE sensor_block_read(UINT8 u8SlaveAddr, 
                                  UINT8 *pBufIn, UINT8 u8CntIn, 
                                  UINT8 *pBufOut, UINT8 u8CntOut)
{
    int rtn;

    rtn = I2C_BLOCK_READ(u8SlaveAddr, pBufIn, u8CntIn, pBufOut, u8CntOut);   

    return rtn;
}

//******************************************************************************
// Function name:
//   sensor_block_write()
// Description:
//   I2C write function for sensor
// Arguments:
//   u8SlaveAddr (in): sensor slave address
//   pBuf        (in): data buffer
//   u8Cnt       (in): byte count of data to write
// Return Values:
//    0: success
//   -1: fail
// Note:
//   n/a
//******************************************************************************
int SENSOR_CODE sensor_block_write(UINT8 u8SlaveAddr, 
                                   UINT8 *pBuf, UINT8 u8Cnt)
{
    int rtn;
    volatile int iDelay = 0x20;   
    
    rtn = I2C_BLOCK_WRITE(u8SlaveAddr, pBuf, u8Cnt);

    while(iDelay--);   // Delay
    return rtn;
}

//*****************************************************************************
//
// sensor device write and read to check configuration is matched.
//
//  parameter :
//          sa      :
//          addr    :
//          param   :
//          val     :
//
//  return :
//          0 : fail
//          1 : match
//
//*****************************************************************************
UINT8 SENSOR_CODE SENSOR_WRITE_WITH_CHECK(UINT8 sa, UINT8 addr, 
                                          UINT8 param, UINT8 val)
{
    UINT8 l_data_match;

    l_data_match = 0;

    param = val;
    if(sensor_write(sa, addr, (UINT8 *)(&param), 1)==0)
    {
        param ^= 0xFF;
        sensor_delay_ms(3);
        
        if(sensor_read(sa, addr, (UINT8 *)(&param), 1)==0)
        {
            if(param == val)
            {
                l_data_match = 1;
            }
        }
    }
    
    return(l_data_match);
}

#if 0 // {
void SENSOR_CODE SENSOR_READ_WITH_CHECK(UINT8 sa, UINT8 addr, 
                                                        UINT8 param, UINT8 val)
{
    param = val ^ 0xFF;
    sensor_read(sa, addr, (UINT8 *)(&param), 1);
    _delay(0x10);

    if(param == val)
    {

    }
}
#endif // } 0

//******************************************************************************
// Function name:
//   sensor_get_data_start()
// Description:
//   set I2C cmd register and start
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_get_data_start(void)
{
    if(sensor_device_ready() == SENSOR_DEVICE_ALL_READY)
    {
        SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE);
        
        #ifndef DEF_SENSOR_ONE_SHOT_MODE // { 
        i2c_drv_cmd_read_stop(ghI2CDrv);
        #endif // } DEF_SENSOR_ONE_SHOT_MODE
        i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, gu8I2CCmdCnt, FALSE);
        gu8HIDStatus = HID_STATUS_NORMAL;
    }
}

//******************************************************************************
// Function name:
//   sensor_get_data_stop()
// Description:
//   stop I2C cmd
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_get_data_stop(void)
{
    if(gu8HIDStatus == HID_STATUS_NORMAL)
    {
        gu8HIDStatus = HID_STATUS_STOP;

        if(gucSensorStatusBusy == 1)
        {
            i2c_channel_reset(SENSOR_I2C_CH);
        }

        i2c_drv_cmd_read_stop(ghI2CDrv);

        sensor_delay_ms(1);
    }
}

#ifdef __ENABLE_AKM_PG_LIB__ // {
UINT8 dT = 0;
int CurT = 0;

//****************************************************************************************
// Function name:
//   sensor_do_akm_fusion()
// Description:
//   AKM pseudo Gyro. fusion function
// Arguments:
//   pDC  (in): sensor data buffer
//   freq (in): fusion frequency
// Return Values:
//   n/a
// Note:
//   n/a
//****************************************************************************************
void SENSOR_CODE sensor_do_akm_fusion(P_SENSOR_BUFFER_WIN pDC, UINT16 freq)
{
	SINT32 mag[3];
	/*SINT16 magBias[3];*/
	SINT32 acc[3];
	SINT32 correct_acc[3];
	SINT32 pg[3];
	SINT32 or[3];
	SINT32 rv[4];

    dT = guiHIDTimer - CurT;
    CurT = guiHIDTimer;

	acc[0] = (((SINT16)(pDC->Acc[0])) << 8);
	acc[1] = (((SINT16)(pDC->Acc[1])) << 8);
	acc[2] = (((SINT16)(pDC->Acc[2])) << 8);

	do_unit_correct_acc(acc, correct_acc);
    fusion_axis_switch(correct_acc, gstSensorStoredData.pAxisMode[0]);

	do_akm_fusion(mag, /*magBias,*/ correct_acc, pg, or, rv, freq);

	/* ACC Data */
    pDC->Acc[0] = (UINT16)correct_acc[0];
    pDC->Acc[1] = (UINT16)correct_acc[1];
    pDC->Acc[2] = (UINT16)correct_acc[2];

    pDC->Shake = 0;

	/* MAG Data */
    pDC->Mag[0] = (UINT16)mag[0];
    pDC->Mag[1] = (UINT16)mag[1];
    pDC->Mag[2] = (UINT16)mag[2];

    pDC->MagCal[0] = dT;//(UINT16)mag[0];
    pDC->MagCal[1] = (UINT16)mag[1];
    pDC->MagCal[2] = (UINT16)mag[2];

    pDC->Heading = (UINT16)or[0]; // yaw

	/* PG Data */
    pDC->Gyo[0] = (UINT16)pg[0];
    pDC->Gyo[1] = (UINT16)pg[1];
    pDC->Gyo[2] = (UINT16)pg[2];    

	/* Windows Inclinometer Data, Android OR Data */
    pDC->Eular[0] = (UINT16)or[1];
    pDC->Eular[1] = (UINT16)or[2];
    pDC->Eular[2] = (UINT16)or[0];

	/* Windows OR Data, Android RV Data */
    pDC->Quat[0] = (UINT16)rv[0];
    pDC->Quat[1] = (UINT16)rv[1];
    pDC->Quat[2] = (UINT16)rv[2];
    pDC->Quat[3] = (UINT16)rv[3];

	//RTL_PRINTF("gyro: %d %d %d \r\n", pg[0], pg[1], pg[2]);
}
#endif // } __ENABLE_AKM_PG_LIB__


//******************************************************************************
// Function name:
//   sensor_fusion_calc()
// Description:
//   fusion calculate
// Arguments:
//   pfnCallback (in): callback function
//   pParam      (in): parameter for callback function
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_fusion_calc(SENSOR_CALLBACK_FUNC pfnCallback, 
                                    void *pParam)
{   
    #ifndef DEF_SENSOR_ONE_SHOT_MODE // {  DEF_SENSOR_ONE_SHOT_MODE
    i2c_interrupt_mask(SENSOR_I2C_CH,0);    
    #endif // } DEF_SENSOR_ONE_SHOT_MODE

    if(gu16FusionFreq)
    {   
        
		#ifdef __ENABLE_AKM_PG_LIB__ // {
		sensor_do_akm_fusion((P_SENSOR_BUFFER_WIN)gpSensorBuffer, gu16FusionFreq);
		#else
        fusion_calc(gpSensorBuffer, gu16FusionFreq);
		#endif // } __ENABLE_AKM_PG_LIB__
    }
    
    #ifndef DEF_SENSOR_ONE_SHOT_MODE // {  DEF_SENSOR_ONE_SHOT_MODE
    i2c_interrupt_mask(SENSOR_I2C_CH,1);    
    #endif // } DEF_SENSOR_ONE_SHOT_MODE
    
    pfnCallback(gpSensorBuffer, pParam);
}

//******************************************************************************
// Function name:
//   sensor_power_mode_switch()
// Description:
//   switch sensor's power mode (0: off, 1: on)
// Arguments:
//   stPower (in): power mode configuration 
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_power_mode_switch(T_SENSOR_POWER_CONFIG stPower)
{
    UINT8 u8I2CCmdIdx = 0;

    if((gu8HIDStatus == HID_STATUS_NORMAL) || (gu8HIDStatus == HID_STATUS_STOP))
    {
        SENSOR_GET_MUTEX(MUTEX_ID_POWER_SWITCH);

        SENSOR_FUSION_DBG_INFO("Power: %04Xh\r\n", *(LPUINT16)&stPower);

        SENSOR_FUSION_DBG_SET_POWER(*(LPUINT8)&stPower);

        sensor_get_data_stop();

        #if _ENABLE_ACC_ // {
        do_power_mode_switch_acc(stPower.Acc);

        if(stPower.Acc)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_ACC;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE    
            u8I2CCmdIdx++;
        }
        #if _ENABLE_ACC2_ // {
        else
        {
            stPower.Acc2 = 0;
        }
        #endif // } _ENABLE_ACC2_        
        #endif // } _ENABLE_ACC_

        #if _ENABLE_MAG_ // {
        do_power_mode_switch_mag(stPower.Mag);
        
        if(stPower.Mag)
        {
            gu8SensorDataSaveFlag = 1;
            
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {    
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_MAG;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_MAG_

        #if _ENABLE_GYO_ // {
        do_power_mode_switch_gyo(stPower.Gyo);
        
        if(stPower.Gyo)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_GYO;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_GYO_

        #if _ENABLE_ALS_ // {
        do_power_mode_switch_als(stPower.Als);
        
        if(stPower.Als)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_ALS;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_ALS_

        #if _ENABLE_ACC2_ // {
        #if (defined(DEF_SENSOR_ACC2_ALWAYS_ON) || \
            (!__ENABLE_HID_CUSTOM_SENSOR__)) // {
        stPower.Acc2 = stPower.Acc;
        #endif // }
        
        do_power_mode_switch_acc2(stPower.Acc2);
        
        if(stPower.Acc2)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_ACC2;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_ACC2_
        
        #if _ENABLE_PRS_ // {
        do_power_mode_switch_prs(stPower.Prs);
        
        if(stPower.Prs)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_PRS;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_PRS_

        #if _ENABLE_HYG_ // {
        do_power_mode_switch_hyg(stPower.Hyg);
        
        if(stPower.Hyg)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_HYG;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_HYG_

        #if _ENABLE_TEMP_ // {
        do_power_mode_switch_temp(stPower.Temp);
        
        if(stPower.Temp)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_TEMP;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_TEMP_    

        #if _ENABLE_PROX_ // {
        do_power_mode_switch_prox(stPower.Prox);
        
        if(stPower.Prox)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_PROX;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_PROX_    

        #if _ENABLE_UV_ // {
        do_power_mode_switch_uv(stPower.UV);
        
        if(stPower.UV)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // {   
            gppI2CCmd[u8I2CCmdIdx] = &I2C_CMD_UV;
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            u8I2CCmdIdx++;
        }
        #endif // } _ENABLE_UV_    

        if((stPower.Acc + stPower.Mag + stPower.Gyo) == 0)
        {
            gu16FusionFreq = 0;
        }
        else
        {
            if(gpI2CCmd[0].u16Period)
            {
                gu16FusionFreq = 1000 / (gpI2CCmd[0].u16Period);
            }
        }
        
        #ifndef DEF_SENSOR_ONE_SHOT_MODE // { 
        gu8I2CCmdCnt = u8I2CCmdIdx;
        #endif // }  DEF_SENSOR_ONE_SHOT_MODE
        
        if(u8I2CCmdIdx > 0)
        {
            sensor_get_data_start();
        }
        else
        {
            // Clear all one shot flag
            gusSensorOneShotCmd = 0; 
        }

        //SENSOR_FUSION_DBG_INFO("Power: %04Xh\r\n", *(LPUINT16)&stPower);

        SENSOR_RELEASE_MUTEX();
    }
}
 
//******************************************************************************
// Function name:
//   sensor_data_rate_switch_5pick1()
// Description:
//   set sensor period to the fastest one among 5 sensors 
//   (Acc, Mag, Gyo, Inc, Ori)
// Arguments:
//   u16MiniPeriod (in): period
// Return Values:
//   always 1
// Note:
//   for one shot mode only
//******************************************************************************
UINT8 SENSOR_CODE sensor_data_rate_switch_5pick1(UINT16 u16MiniPeriod)
{
    _change_period(u16MiniPeriod);

    gu16SensorPeriod[SENSOR_FUSION_IDX] = u16MiniPeriod;

    #if _ENABLE_ACC_ // {
    gu16SensorPeriod[SENSOR_I2C_IDX_ACC] = u16MiniPeriod;
    #endif // } _ENABLE_ACC_

    #if _ENABLE_MAG_ // {
    gu16SensorPeriod[SENSOR_I2C_IDX_MAG] = u16MiniPeriod;
    #endif // } _ENABLE_MAG_
    
    #if _ENABLE_GYO_ // {
    gu16SensorPeriod[SENSOR_I2C_IDX_GYO] = u16MiniPeriod;
    #endif // } _ENABLE_GYO_
    
    return 1;
}

//******************************************************************************
// Function name:
//   sensor_data_rate_switch()
// Description:
//   switch sensor's data rate.
//   this function will set fusion(I2C) period to be the smallest one of 
//   all sensors' period.
// Arguments:
//   u8SensorID  (in): sensor HID report ID
//   u16MilliSec (in): data rate (ms)
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_data_rate_switch(UINT8 u8SensorID, UINT16 u16MilliSec)
{
    UINT16 u16MiniPeriod, u16LimitPeriod;

    if(u16MilliSec)
    {
        if(gu8HIDStatus == HID_STATUS_NORMAL)
        {
            SENSOR_GET_MUTEX(MUTEX_ID_RATE_SWITCH);
                
            #if __ENABLE_HID_CUSTOM_SENSOR__
            UINT8 u8FreqIdx;

            if(u8SensorID == HID_SENSOR_CUSTOM)
            {
                gu16AngleFreq = 1000 / u16MilliSec;
                if(do_data_rate_check_acc2(gu16AngleFreq, &u8FreqIdx) != 1)
                {
                    sensor_get_data_stop();
                    do_data_rate_switch_acc2(u8FreqIdx);
                    sensor_get_data_start();
                    SENSOR_FUSION_DBG_INFO("ACC2 data rate change.\r\n");
                }
            }
            else
            #endif //__ENABLE_HID_CUSTOM_SENSOR__
            {
                SENSOR_FUSION_DBG_INFO("Data rate switch(in): (%d)%d -> %d\r\n", 
                                       u8SensorID, gpI2CCmd[0].u16Period, 
                                       u16MilliSec);

                u16MiniPeriod = hid_get_min_report_interval();

                if(hid_check_power_mode()){ //Shawn: to improve calibration time
                    u16LimitPeriod = FUSION_PERIOD_LIMIT;
                }
                else{
                    u16LimitPeriod = FUSION_PERIOD_MAX;
                }
                
                if(u16MiniPeriod > u16LimitPeriod){
                    u16MiniPeriod = u16LimitPeriod;
                }
                
                sensor_get_data_stop();

                sensor_data_rate_switch_5pick1(u16MiniPeriod);

                gu16FusionFreq = (1000 / gu16SensorPeriod[SENSOR_FUSION_IDX]);

                sensor_get_data_start();

                SENSOR_FUSION_DBG_INFO("Data rate switch(out): %d\r\n\r\n", 
                                       gpI2CCmd[0].u16Period);                
            }      

            SENSOR_RELEASE_MUTEX();  
        }
    }
}

#ifdef __ENABLE_HID_PM_DIPO__  // {  __ENABLE_HID_PM_DIPO__
//******************************************************************************
// Function name:
//   sensor_work_mode_switch()
// Description:
//   switch all sensors' work mode.
//   this function will change frequency of fusion(I2C) and sensor itself.
// Arguments:
//   u8Mode (in): 0: slowest mode, 1: normal mode
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_work_mode_switch(UINT8 u8Mode)
{
    UINT8  i;
    UINT16 u16MiniPeriod;

    if(gu8HIDStatus == HID_STATUS_NORMAL)
    {
        sensor_get_data_stop();

        if(u8Mode == 0)
        {
            u16MiniPeriod = FUSION_PERIOD_MAX;
        }
        else
        {
            for(u16MiniPeriod = 0xFFFF, i = 0; i < SENSOR_PERIOD_CNT; i++)
            {
                if(u16MiniPeriod > gu16SensorPeriod[i])
                    u16MiniPeriod = gu16SensorPeriod[i];
            }
        }

        _change_period(u16MiniPeriod);

        sensor_get_data_start();
    }
}
#endif  // }   __ENABLE_HID_PM_DIPO__

//******************************************************************************
// Function name:
//   sensor_stored_data_save()
// Description:
//   save sensor parameter from RAM to eFlash
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_stored_data_save(void)
{
    //SENSOR_FUSION_DBG_INFO("SENSOR_STORED_DATA_SIZE: %d\r\n", 
    //                       SENSOR_STORED_DATA_SIZE);
        
    //  Disable INT
    hal_global_int_ctl(HAL_DISABLE_INTERRUPTS);

    spi_write_status(SPI_selection_internal, SPIStatus_UnlockAll, 0);

    spi_write_enable(SPI_selection_internal, 0);
    spi_erase(SPI_selection_internal, 
              SPICmd_SectorErase, 
              SENSOR_STORED_DATA_ADDRESS);
    spi_write_disable(SPI_selection_internal);

    sensor_delay_ms(1);

    spi_write_enable(SPI_selection_internal, 0);
    spi_write_aai_word(SPI_selection_internal, 
                       SENSOR_STORED_DATA_ADDRESS, 
                       (LPUINT8)&gstSensorStoredData, SENSOR_STORED_DATA_SIZE);
    spi_write_disable(SPI_selection_internal);
    
    //  Enable INT
    hal_global_int_ctl(HAL_ENABLE_INTERRUPTS);

    sensor_delay_ms(1);
}

//******************************************************************************
// Function name:
//   sensor_stored_data_load()
// Description:
//   load sensor parameter from eFlash to RAM
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_stored_data_load(void)
{
    SENSOR_FUSION_DBG_INFO("stored_data: %Xh(%d)\r\n", 
                           SENSOR_STORED_DATA_ADDRESS, 
                           sizeof(T_SENSOR_STORED_DATA));

    CLEAR_IMMU_CACHE();

    /* Check buffer flag */    
    if((gstSensorStoredData.u16ValidFlag != 0x8380) || 
       (gstSensorStoredData.u16TotalSize != sizeof(T_SENSOR_STORED_DATA)))
    {
        /* Check eFlash flag */
        memcpy(&gstSensorStoredData, (void*)SENSOR_STORED_DATA_ADDRESS, 
               sizeof(T_SENSOR_STORED_DATA));

        SENSOR_FUSION_DBG_DUMP((UINT8*)&gstSensorStoredData, 
                               sizeof(T_SENSOR_STORED_DATA));

        if((gstSensorStoredData.u16ValidFlag != 0x8380) || 
           (gstSensorStoredData.u16TotalSize != sizeof(T_SENSOR_STORED_DATA)))
        {
            /* Set default value */
			#if 1//
			memcpy(&gstSensorStoredData, (UINT8*)gu8SensorParaDefault, sizeof(T_SENSOR_STORED_DATA));
			#else
            memset(&gstSensorStoredData, 0x00, sizeof(T_SENSOR_STORED_DATA));
			
            gstSensorStoredData.u16ValidFlag = 0x8380;
            gstSensorStoredData.u16TotalSize = sizeof(T_SENSOR_STORED_DATA);

            gstSensorStoredData.u16AlsMultiplier = 1;
            gstSensorStoredData.u16AlsDivisor = 1;

            #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC2_ // {
            gstSensorStoredData.pAcc2GainBuffer[0] = 256;
            gstSensorStoredData.pAcc2GainBuffer[1] = 256;
            gstSensorStoredData.pAcc2GainBuffer[2] = 256;            
            #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC2_

            #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC_ // {
            gstSensorStoredData.pAccGainBuffer[0] = 256;
            gstSensorStoredData.pAccGainBuffer[1] = 256;
            gstSensorStoredData.pAccGainBuffer[2] = 256;              
            #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC_

            gstSensorStoredData.u16Version = 0x0001;
			#endif
        }
    }

    /* Initial fusion lib. & Set stored data info. */
    gpSHDInfo->u32FusionVer = fusion_init(&gstFConfig, 
                                          gstSensorStoredData.pAccBiasBuffer, 
                                          gstSensorStoredData.pGyroBiasBuffer, 
                                          gstSensorStoredData.pAxisMode);

    #if _ENABLE_ACC2_ // {
    init_custom_angle_param(gstSensorStoredData.u8Acc2AxisMode, 
                            gstSensorStoredData.pAcc2BiasBuffer);

    gstSensorStoredData.stStoredType.EnableAcc2 = 1;
    #endif // } _ENABLE_ACC2_

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC2_ // {
    // Define "Acc2_6AxisCalBuffer" in oem memory (24 bytes)
    init_acc2_6axis_cal_param(gstSensorStoredData.pAcc2GainBuffer, 
                              Acc2_6AxisCalBuffer);
    
    gstSensorStoredData.stStoredType.Enable6AxisCalAcc2 = 1;
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC2_

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC_ // {
    // Define "Acc_6AxisCalBuffer" in oem memory (24 bytes)
    init_acc_6axis_cal_param(gstSensorStoredData.pAccGainBuffer, 
                             Acc_6AxisCalBuffer);
    
    gstSensorStoredData.stStoredType.Enable6AxisCalAcc = 1;
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC_    
    
    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    gstSensorStoredData.stStoredType.EnableAlsColor = 1;
    #endif // } __ENABLE_ALS_COLOR_VALUE__
}

//******************************************************************************
// Function name:
//   sensor_power_off_data_save()
// Description:
//   save sensor parameter when OS power off
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   be called when Acc off
//******************************************************************************
void SENSOR_CODE sensor_power_off_data_save(void)
{
    P_SENSOR_STORED_DATA pDC = (P_SENSOR_STORED_DATA)SENSOR_STORED_DATA_ADDRESS;
    UINT32 u32Length;
    
    SENSOR_FUSION_DBG(0xFF);

    if(gu8SensorDataSaveFlag)
    {
        SENSOR_GET_MUTEX(MUTEX_ID_POWER_OFF);

        // Shawn: update pMagBuffer[] only
        if((pDC->u16ValidFlag == 0x8380) && 
           (pDC->u16TotalSize == sizeof(T_SENSOR_STORED_DATA)))
        {
            // Data before pMagBuffer[]
            u32Length = (UINT32)(((P_SENSOR_STORED_DATA)0)->pMagBuffer);
            memcpy(&gstSensorStoredData, (UINT8*)pDC, u32Length);

            // Data after pMagBuffer[]
            u32Length = (sizeof(T_SENSOR_STORED_DATA) - 
                        (u32Length + STORED_DATA_LENGTH_MAG));
            memcpy((gstSensorStoredData.pMagBuffer + STORED_DATA_LENGTH_MAG), 
                   (pDC->pMagBuffer + STORED_DATA_LENGTH_MAG), u32Length);
        }
        
        sensor_stored_data_save();
        SENSOR_RELEASE_MUTEX();

        gu8SensorDataSaveFlag = 0;
    }
}

#if _ENABLE_ACC_ // {
//******************************************************************************
// Function name:
//   sensor_update_acc_bias()
// Description:
//   update value of accelerometer bias
// Arguments:
//   calibration_phase_mode (in): 0: Z+, 1: Z-, 2: X+, 3: X-, 4: Y+, 5: Y-
//                                0xF0: start CMD for 6-axis/2-axis calibration
//                                0xF1: end CMD for 6-axis/2-axis calibration
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE_L sensor_update_acc_bias(UINT8 calibration_phase_mode)
{
    UINT8 acc_sum_cnt = 0;
    SINT32 acc_sum[3] = {0,0,0};
    SINT16 acc_avg[3] = {0,0,0};

    if((calibration_phase_mode >= 0) && (calibration_phase_mode <= 5))
    {
        do{
            sensor_read(SLAVE_ADDRESS_ACC, DATA_ADDRESS_ACC, 
                        SENSOR_BUFFER_ACC, DATA_LENGTH_ACC);
            do_data_correct_acc(SENSOR_BUFFER_ACC);
            
            acc_sum[0] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC + 0)));
            acc_sum[1] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC + 2)));
            acc_sum[2] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC + 4)));
            
            acc_sum_cnt++;
            
            sensor_delay_ms(FUSION_PERIOD);
        }while(acc_sum_cnt < 8);

        acc_avg[0] = (SINT16)(acc_sum[0] >> 3);
        acc_avg[1] = (SINT16)(acc_sum[1] >> 3);
        acc_avg[2] = (SINT16)(acc_sum[2] >> 3);
        memcpy(SENSOR_BUFFER_ACC, acc_avg, 6);        
    }

    fusion_update_acc_bias(SENSOR_BUFFER_ACC, calibration_phase_mode);

    SENSOR_FUSION_DBG_INFO("acc_bias: %d, %d, %d\r\n", 
                           gstSensorStoredData.pAccBiasBuffer[0],
                           gstSensorStoredData.pAccBiasBuffer[1], 
                           gstSensorStoredData.pAccBiasBuffer[2]);

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC_ // {
    SENSOR_FUSION_DBG_INFO("acc_gain: %d, %d, %d\r\n", 
                           gstSensorStoredData.pAccGainBuffer[0],
                           gstSensorStoredData.pAccGainBuffer[1], 
                           gstSensorStoredData.pAccGainBuffer[2]);       
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC_
}
#endif // } _ENABLE_ACC_

#if _ENABLE_ACC2_ // {
#if 0 // {
//******************************************************************************
// Function name:
//   sensor_get_acc2_info()
// Description:
//   get 2nd accelerometer information
// Arguments:
//   pAcc2  (in): pointer to 2nd accelerometer data buffer
//   pAngle (in): pointer to angle data buffer
// Return Values:
//   frequency of angle calculation
// Note:
//   n/a
//******************************************************************************
UINT16 SENSOR_CODE sensor_get_acc2_info(UINT16 pAcc2[3], SINT16 pAngle[3])
{
    HID_GET_MUTEX();
    pAcc2[0] = Accelerometer2[0];
    pAcc2[1] = Accelerometer2[1];
    pAcc2[2] = Accelerometer2[2];
    pAngle[0] = CustomAngle[0];
    pAngle[1] = CustomAngle[1];
    pAngle[2] = CustomAngle[2];
    HID_RELEASE_MUTEX();

    return gu16AngleFreq;
}
#endif // } 0

//******************************************************************************
// Function name:
//   sensor_update_acc2_bias()
// Description:
//   update value of 2nd accelerometer bias
// Arguments:
//   calibration_phase_mode (in): 0: Z+, 1: Z-, 2: X+, 3: X-, 4: Y+, 5: Y-
//                                0xF0: start CMD for 6-axis/2-axis calibration
//                                0xF1: end CMD for 6-axis/2-axis calibration
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE_L sensor_update_acc2_bias(UINT8 calibration_phase_mode)
{
    UINT8 acc2_sum_cnt = 0;
    SINT32 acc2_sum[3] = {0,0,0};
    SINT16 acc2_avg[3] = {0,0,0};

    if((calibration_phase_mode >= 0) && (calibration_phase_mode <= 5))
    {
        do{
            sensor_read(SLAVE_ADDRESS_ACC2, DATA_ADDRESS_ACC2, 
                        SENSOR_BUFFER_ACC2, DATA_LENGTH_ACC2);
            do_data_correct_acc2(SENSOR_BUFFER_ACC2);
            
            acc2_sum[0] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC2 + 0)));
            acc2_sum[1] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC2 + 2)));
            acc2_sum[2] += (SINT32)(*((SINT16*)(SENSOR_BUFFER_ACC2 + 4)));
            
            acc2_sum_cnt++;

            sensor_delay_ms(FUSION_PERIOD);
        }while(acc2_sum_cnt < 8);

        acc2_avg[0] = (SINT16)(acc2_sum[0] >> 3);
        acc2_avg[1] = (SINT16)(acc2_sum[1] >> 3);
        acc2_avg[2] = (SINT16)(acc2_sum[2] >> 3);
        memcpy(SENSOR_BUFFER_ACC2, acc2_avg, 6);
    }
    
    init_custom_angle_param(gstSensorStoredData.u8Acc2AxisMode, 
                            gstSensorStoredData.pAcc2BiasBuffer);

    fusion_update_acc2_bias(SENSOR_BUFFER_ACC2, calibration_phase_mode);

    SENSOR_FUSION_DBG_INFO("acc2_bias: %d, %d, %d\r\n", 
                           gstSensorStoredData.pAcc2BiasBuffer[0],
                           gstSensorStoredData.pAcc2BiasBuffer[1], 
                           gstSensorStoredData.pAcc2BiasBuffer[2]);

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC2_ // {
    SENSOR_FUSION_DBG_INFO("acc2_gain: %d, %d, %d\r\n", 
                           gstSensorStoredData.pAcc2GainBuffer[0],
                           gstSensorStoredData.pAcc2GainBuffer[1], 
                           gstSensorStoredData.pAcc2GainBuffer[2]);       
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC2_
}
#endif // } _ENABLE_ACC2_

#if _ENABLE_GYO_ // {
//******************************************************************************
// Function name:
//   sensor_update_gyro_bias()
// Description:
//   update value of gyrometer bias
// Arguments:
//   n/a
// Return Values:
//   0: success
//   1: timeout
// Note:
//   n/a
//******************************************************************************
UINT8 SENSOR_CODE_L sensor_update_gyro_bias(void)
{
    short retry_loop = 0;

    do
    {
        sensor_read(SLAVE_ADDRESS_GYO, DATA_ADDRESS_GYO, 
                    SENSOR_BUFFER_GYO, DATA_LENGTH_GYO);
        do_data_correct_gyo(SENSOR_BUFFER_GYO);

        sensor_delay_ms(FUSION_PERIOD);

        if(retry_loop++ >= 500)
        {
            SENSOR_FUSION_DBG_ERR("gyro calibration: retry timeout\r\n");
            return 1;
        }
    }while(!fusion_update_gyro_bias(SENSOR_BUFFER_GYO));

    SENSOR_FUSION_DBG_INFO("gyro_bias(%d): %d, %d, %d\r\n", 
                           retry_loop, 
                           gstSensorStoredData.pGyroBiasBuffer[0],
                           gstSensorStoredData.pGyroBiasBuffer[1], 
                           gstSensorStoredData.pGyroBiasBuffer[2]);

    return 0;
}
#endif // } _ENABLE_GYO_

//******************************************************************************
// Function name: 
//   send_packet()
// Description:
//   send sensor data via UART
// Arguments:
//   pData  (in): sensor buffer
//   pParam (in): buffer length
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void send_packet(UINT8 *pData, void *pParam)
{
#if (!DEF_HID_OVER_I2C && !DEF_HID_OVER_USB) // {
    UINT32 u32Len = (UINT32)pParam;
    UINT32 i = 0;

    #ifndef __SENSOR_DBG__
    UartPutChar(0xAA);
    UartPutChar(0xBB);
    for(i = 0 ; i < u32Len; i++)
    {
        UartPutChar(*pData++);
    }
    UartPutChar(0x5D);
    UartPutChar(0x5E);
    #else
    for(i = 0 ; i < u32Len; i++)
    {
        RTL_PRINTF("%02Xh ", *pData++);
    }
    RTL_PRINTF("\r\n--\r\n");
    sensor_delay_ms(FUSION_PERIOD);
    ClrScr();
    #endif  //__SENSOR_DBG__
#endif // } (!DEF_HID_OVER_I2C && !DEF_HID_OVER_USB)
}

//******************************************************************************
// Function name: 
//   diagnostics_sensor_test()   
// Description:
//   sensor diagnostics test.
//   Little Endian to PC
//   SOF
//   x_acc_L x_acc_H y_acc_L y_acc_H z_acc_H z_acc_H
//   x_mag_H x_mag_L y_mag_H y_mag_L z_mag_H z_mag_L
//   x_gyr_L x_gyr_H y_gyr_L y_gyr_H z_gyr_L z_gyr_H
//   EOF
//
//   Get sensor raw data and output to UART
//
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
#if 0
void diagnostics_sensor_test(void)
{
    RTL_PRINTF("%s()\r\n", __FUNCTION__);

    SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
    sensor_init();

    sensor_get_data_start();

    while(1)
    {
        sensor_fusion_calc(send_packet, (void*)64);
    }
}
#endif // }

//******************************************************************************
//  Sensor fusion interface
//******************************************************************************

//******************************************************************************
// Function name:
//   sensor_isr()
// Description:
//   sensor ISR
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_isr(void)
{
    UINT8 ucSTR;

    // 
    gucI2CSensorErrStatus = (I2C_GET_ERR_STAT(SENSOR_I2C_CH) & 0x0F); 
		
    // 
    ucSTR = I2C_GET_STAT(SENSOR_I2C_CH);

    if(gucI2CSensorErrStatus) // NAK
    {
        // +SHAWN20150416: clear error status
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_1);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_2);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_3);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_4);
        // -SHAWN20150416
        
        SENSOR_FUSION_DBG_ERR("sensor error: %02Xh\r\n", ucSTR); 
    }
    
    if((ucSTR & I2C_STAT_TIMEOUT))        // TIMEOUT
    {
        if(gu8HIDStatus != HID_STATUS_STOP)
        {
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // { 
            sensor_get_data_stop();
            #endif // } DEF_SENSOR_ONE_SHOT_MODE
            
            gu8HIDStatus = HID_STATUS_SENSOR_TIMEOUT;
			I2C_CLEAR_INTERRUPT(SENSOR_I2C_CH);
            i2c_interrupt_mask(SENSOR_I2C_CH,0);  
            
            #ifdef __HID_FUSION_FUN__ // {       
            hal_resume_task_in_isr(tthi_FUSION);     
            #endif  // } __HID_FUSION_FUN__
        }
        I2C_CLEAR_INTERRUPT(SENSOR_I2C_CH);

        // +SHAWN20150415: sensor timeout issue
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_1);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_2);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_3);
        i2c_get_err_status(SENSOR_I2C_CH, I2C_DEV_4);
        // -SHAWN20150415
        
        return;         
    }

    I2C_DEBUG_PORT3(0x01);
    #ifdef DEF_SENSOR_ONE_SHOT_MODE // {  
    #ifdef __HID_FUSION_FUN__ // {
    I2C_DEBUG_PORT4(0x02);
    hal_resume_task_in_isr(tthi_FUSION); 
    #endif  // } __HID_FUSION_FUN__
    I2C_CLEAR_INTERRUPT(SENSOR_I2C_CH);
    gucI2CINTFlag[SENSOR_I2C_CH] = 0;

    #ifdef EC_FEATURE_ONLY
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        if(gucI2CSensorErrStatus==0)
        {
            SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;

            //
            // To set extend task service flag
            //
            F_Service_Extend_Task = 1;
        }
        #endif
    #endif
    
    #else   // DEF_SENSOR_ONE_SHOT_MODE
    UINT8 ucCmdIdx = 0;
    int i = 0;

    I2C_CLEAR_INTERRUPT(SENSOR_I2C_CH);
    
    ucCmdIdx = ((ECReg(I2C_ERROR_MASK(SENSOR_I2C_CH)) & 0xF0) >> 4);
    for(i = 0; i < 4; i++)
    {
        if((ucCmdIdx & 0x01) == 1)
        {               
            i2c_drv_read_sram_data(gppI2CCmd[i]); 
            ECReg(I2C_ERROR_MASK(SENSOR_I2C_CH)) |= (1 <<(4+ i)); // Write Clear
            if(i == 2)
            {
                SET_MASK(gusSensorOneShotCmd,  BIT(SENSOR_FUSION_IDX));          
            }
            else if(i == 0 && (gu8HIDStatus == HID_STATUS_NORMAL))
            {             
                ECReg(I2C_CONTROL4(SENSOR_I2C_CH)) |= 
                    (I2C_CRTL4_ONE_SHOT_M3 | I2C_CRTL4_ONE_SHOT_M2);                 
            }
        }
        ucCmdIdx >>= 1;
    }
    #endif  // } DEF_SENSOR_ONE_SHOT_MODE
    I2C_DEBUG_PORT3(0x02);
}

//******************************************************************************
// Function name:
//   sensor_init()
// Description:
//   initial I2C driver and all sensors
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************

void SENSOR_CODE_L sensor_init(void)
{
    int i;

    switch(SENSOR_INIT_STEP)
    {
        case SENSOR_INIT_STEP_RESET_PARAMETER:
            SENSOR_FUSION_DBG_INFO("gpSensorBuffer: %Xh\r\n", gpSensorBuffer);
            SENSOR_FUSION_DBG_CLEAR();
            SENSOR_FUSION_DBG_SET_CNT(SENSOR_TOTAL_CNT);

            // Reset parameter
            memset(gppI2CCmd, 0x00, sizeof(gppI2CCmd));
            memset(gpI2CCmd, 0x00, sizeof(gpI2CCmd));
            memset(gpSensorBuffer, 0x00, SENSOR_BUFFER_SIZE);
            memset(&gstSensorInitDone, 0x00, sizeof(gstSensorInitDone));
            memset((UINT8*)&Sensor_API_ID_Accelerometer, 0xFF, 0x30);

            gu16FusionFreq = FUSION_FREQUENCY;
            gu8I2CResetCnt = 0;

            #if _ENABLE_ACC2_ // {
            gu16AngleFreq = ANGLE_FREQUENCY;
            #endif // } _ENABLE_ACC2_

            for(i = 0; i < SENSOR_PERIOD_CNT; i++)
            {
                gu16SensorPeriod[i] = 0x1000;
            }

            gu16SensorPeriod[SENSOR_FUSION_IDX] = FUSION_PERIOD;        

            // Initial Multi-Acc drv.
            #ifdef _ENABLE_MULTI_ACC_ // {
            do_init_drv_acc();
            
            gpSensorI2CID[SENSOR_I2C_IDX_ACC] = SLAVE_ADDRESS_ACC;
            #endif // } _ENABLE_MULTI_ACC_

            #ifdef _ENABLE_MULTI_ACC2_ // {
            do_init_drv_acc2();
            
            gpSensorI2CID[SENSOR_I2C_IDX_ACC2] = SLAVE_ADDRESS_ACC2;
            #endif // } _ENABLE_MULTI_ACC2_                  
            
            // Initial I2C drv.
            ghI2CDrv = i2c_drv_init(SENSOR_I2C_CH);

            // Reset sensor state
            hid_reset_sensor_state();

            SENSOR_DEVICE_INIT_RETRY = 0x00;
            SENSOR_INIT_STEP_FAIL_CAUSE = 0x00;
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ACC;
			hook_sensor_init_start();
            break;

        // Initial sensor & I2C Command
        case SENSOR_INIT_STEP_INIT_ACC:
            Sensor_API_ID_Accelerometer = ACC_SENSOR_API_ID;
            
            #if _ENABLE_ACC_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE |
                             ((SENSOR_I2C_IDX_ACC+1) << 4));
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_ACC, SLAVE_ADDRESS_ACC);

            gu16SensorPeriod[SENSOR_I2C_IDX_ACC] = FUSION_PERIOD;

            I2C_CMD_SET(I2C_CMD_ACC,
                        SLAVE_ADDRESS_ACC,
                        DATA_ADDRESS_ACC,
                        DATA_LENGTH_ACC,
                        FUSION_PERIOD,
                        SENSOR_BUFFER_ACC);

            gppI2CCmd[SENSOR_I2C_IDX_ACC] = &I2C_CMD_ACC;

            if(do_initial_acc(FUSION_FREQUENCY))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_ACC);

                gstSensorInitDone.Acc = 1;

                hid_update_sensor_state(HID_SENSOR_ACC, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);

                #if __ENABLE_HID_ANDROID__ // {
                hid_update_sensor_state(HID_SENSOR_GRA, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                hid_update_sensor_state(HID_SENSOR_LACC, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                #endif // } __ENABLE_HID_ANDROID__
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_MAG;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_ACC, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_ACC, 
                                             SENSOR_INIT_STEP_INIT_MAG);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_MAG;
            #endif // } _ENABLE_ACC_
            break;

        case SENSOR_INIT_STEP_INIT_MAG:
            Sensor_API_ID_Compass = MAG_SENSOR_API_ID;
            
            #if _ENABLE_MAG_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_MAG+1) << 4));
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_MAG, SLAVE_ADDRESS_MAG);
            SENSOR_FUSION_DBG_SET_MAG_ALG();

            gu16SensorPeriod[SENSOR_I2C_IDX_MAG] = FUSION_PERIOD;

            I2C_CMD_SET(I2C_CMD_MAG,
                        SLAVE_ADDRESS_MAG,
                        DATA_ADDRESS_MAG,
                        DATA_LENGTH_MAG,
                        FUSION_PERIOD,
                        SENSOR_BUFFER_MAG);

            gppI2CCmd[SENSOR_I2C_IDX_MAG] = &I2C_CMD_MAG;

            if(do_initial_mag(FUSION_FREQUENCY))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_MAG);

                gstSensorInitDone.Mag = 1;

                hid_update_sensor_state(HID_SENSOR_MAG, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);

                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_GYO;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_MAG, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_MAG, 
                                             SENSOR_INIT_STEP_INIT_GYO);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_GYO;
            #endif // } _ENABLE_MAG_
            break;

        case SENSOR_INIT_STEP_INIT_GYO:
            Sensor_API_ID_Gyrometer = GYO_SENSOR_API_ID;    
            
            #if _ENABLE_GYO_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_GYO+1) << 4));
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_GYO, SLAVE_ADDRESS_GYO);

            gu16SensorPeriod[SENSOR_I2C_IDX_GYO] = FUSION_PERIOD;

            I2C_CMD_SET(I2C_CMD_GYO,
                        SLAVE_ADDRESS_GYO,
                        DATA_ADDRESS_GYO,
                        DATA_LENGTH_GYO,
                        FUSION_PERIOD,
                        SENSOR_BUFFER_GYO);

            gppI2CCmd[SENSOR_I2C_IDX_GYO] = &I2C_CMD_GYO;  

            if(do_initial_gyo(FUSION_FREQUENCY))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_GYO);

                gstSensorInitDone.Gyo = 1;

                hid_update_sensor_state(HID_SENSOR_GYR, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ALS;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_GYO, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_GYO, 
                                             SENSOR_INIT_STEP_INIT_ALS);
                }
            }
            #else
            #if DEF_FUSION_6D // {
            gstSensorInitDone.Gyo = 1;

            hid_update_sensor_state(HID_SENSOR_GYR, 
                                    HID_USAGE_SENSOR_STATE_READY_ENUM);
            #endif // } DEF_FUSION_6D
            
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ALS;
            #endif // } _ENABLE_GYO_

            #if __ENABLE_HID_FUSION_SENSOR__ // {
            if(gstSensorInitDone.Acc && gstSensorInitDone.Mag && 
              (gstSensorInitDone.Gyo || gstFConfig.Enable_6D))
            {
                hid_update_sensor_state(HID_SENSOR_INC, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                hid_update_sensor_state(HID_SENSOR_ORI, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
            }
            #endif // } __ENABLE_HID_FUSION_SENSOR__
            break;

        case SENSOR_INIT_STEP_INIT_ALS:
            Sensor_API_ID_Ambientlight = ALS_SENSOR_API_ID;

            #if _ENABLE_ALS_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_ALS+1) << 4));
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_ALS, SLAVE_ADDRESS_ALS);

            gu16SensorPeriod[SENSOR_I2C_IDX_ALS] = 1000;

            I2C_CMD_SET(I2C_CMD_ALS,
                        SLAVE_ADDRESS_ALS,
                        DATA_ADDRESS_ALS,
                        DATA_LENGTH_ALS,
                        1000,
                        SENSOR_BUFFER_ALS);
            
            #ifndef DEF_SENSOR_ONE_SHOT_MODE  //{
            gppI2CCmd[SENSOR_I2C_IDX_ALS] = &I2C_CMD_ALS;
            #endif  //} DEF_SENSOR_ONE_SHOT_MODE
            
            if(do_initial_als(1))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_ALS);

                gstSensorInitDone.Als = 1;

                hid_update_sensor_state(HID_SENSOR_ALS, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ACC2;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_ALS, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_ALS, 
                                             SENSOR_INIT_STEP_INIT_ACC2);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ACC2;
            #endif // } _ENABLE_ALS_
            break;

        case SENSOR_INIT_STEP_INIT_ACC2:
            Sensor_API_ID_Accelerometer2 = ACC2_SENSOR_API_ID;

            #if _ENABLE_ACC2_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_ACC2+1) << 4));
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_ACC2, SLAVE_ADDRESS_ACC2);

            gu16SensorPeriod[SENSOR_I2C_IDX_ACC2] = ANGLE_PERIOD;

            I2C_CMD_SET(I2C_CMD_ACC2,
                        SLAVE_ADDRESS_ACC2,
                        DATA_ADDRESS_ACC2,
                        DATA_LENGTH_ACC2,
                        ANGLE_PERIOD,
                        SENSOR_BUFFER_ACC2);
                
//                  gppI2CCmd[SENSOR_I2C_IDX_ACC2] = &I2C_CMD_ACC2;   

            if(do_initial_acc2(ANGLE_FREQUENCY))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_ACC2);

                gstSensorInitDone.Acc2 = 1;

                #if __ENABLE_HID_CUSTOM_SENSOR__ // {
                hid_update_sensor_state(HID_SENSOR_CUSTOM, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                #endif // } __ENABLE_HID_CUSTOM_SENSOR__
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_PRS;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_ACC2, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_ACC2, 
                                             SENSOR_INIT_STEP_INIT_PRS);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_PRS;
            #endif // } _ENABLE_ACC2_
            break;
            
        case SENSOR_INIT_STEP_INIT_PRS:
            Sensor_API_ID_Pressure = PRS_SENSOR_API_ID;

            #if _ENABLE_PRS_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_PRS+1) << 4)); 
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_PRS, SLAVE_ADDRESS_PRS);

            gu16SensorPeriod[SENSOR_I2C_IDX_PRS] = 1000;

            I2C_CMD_SET(I2C_CMD_PRS,
                        SLAVE_ADDRESS_PRS,
                        DATA_ADDRESS_PRS,
                        DATA_LENGTH_PRS,
                        1000,
                        SENSOR_BUFFER_PRS);         

            if(do_initial_prs(1))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_PRS);

                gstSensorInitDone.Prs = 1;

                hid_update_sensor_state(HID_SENSOR_PRS, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_HYG;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_PRS, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_PRS, 
                                             SENSOR_INIT_STEP_INIT_HYG);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_HYG;
            #endif // } _ENABLE_PRS_
            break;

        case SENSOR_INIT_STEP_INIT_HYG:
            Sensor_API_ID_Hygrometer = HYG_SENSOR_API_ID;

            #if _ENABLE_HYG_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_HYG+1) << 4)); 
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_HYG, SLAVE_ADDRESS_HYG);

            gu16SensorPeriod[SENSOR_I2C_IDX_HYG] = 1000;

            I2C_CMD_SET(I2C_CMD_HYG,
                        SLAVE_ADDRESS_HYG,
                        DATA_ADDRESS_HYG,
                        DATA_LENGTH_HYG,
                        1000,
                        SENSOR_BUFFER_HYG);            

            if(do_initial_hyg(1))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_HYG);

                gstSensorInitDone.Hyg = 1;

                hid_update_sensor_state(HID_SENSOR_HYG, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_TEMP;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_HYG, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_HYG, 
                                             SENSOR_INIT_STEP_INIT_TEMP);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_TEMP;
            #endif // } _ENABLE_HYG_
            break;

        case SENSOR_INIT_STEP_INIT_TEMP:
            Sensor_API_ID_Temperature = TEMP_SENSOR_API_ID;

            #if _ENABLE_TEMP_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_TEMP+1) << 4)); 
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_TEMP, SLAVE_ADDRESS_TEMP);

            gu16SensorPeriod[SENSOR_I2C_IDX_TEMP] = 1000;

            I2C_CMD_SET(I2C_CMD_TEMP,
                        SLAVE_ADDRESS_TEMP,
                        DATA_ADDRESS_TEMP,
                        DATA_LENGTH_TEMP,
                        1000,
                        SENSOR_BUFFER_TEMP);    

            if(do_initial_temp(1))
            {
                SENSOR_FUSION_DBG_SET_INIT(SENSOR_I2C_IDX_TEMP);

                gstSensorInitDone.Temp = 1;

                hid_update_sensor_state(HID_SENSOR_TEMP, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_PROX;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_TEMP, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_TEMP, 
                                             SENSOR_INIT_STEP_INIT_PROX);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_PROX;
            #endif // } _ENABLE_TEMP_
            break;

        case SENSOR_INIT_STEP_INIT_PROX:
            Sensor_API_ID_Proximity = PROX_SENSOR_API_ID;

            #if _ENABLE_PROX_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_PROX+1) << 4)); 
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_PROX, SLAVE_ADDRESS_PROX);

            gu16SensorPeriod[SENSOR_I2C_IDX_PROX] = 1000;

            I2C_CMD_SET(I2C_CMD_PROX,
                        SLAVE_ADDRESS_PROX,
                        DATA_ADDRESS_PROX,
                        DATA_LENGTH_PROX,
                        1000,
                        SENSOR_BUFFER_PROX);    

            if(do_initial_prox(1))
            {
                gstSensorInitDone.Prox = 1;

                hid_update_sensor_state(HID_SENSOR_PROX, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_UV;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_PROX, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_PROX, 
                                             SENSOR_INIT_STEP_INIT_UV);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_UV;
            #endif // } _ENABLE_PROX_
            break;            

        case SENSOR_INIT_STEP_INIT_UV:
            Sensor_API_ID_Ultraviolet = UV_SENSOR_API_ID;

            #if _ENABLE_UV_ // {
            SENSOR_FUSION_DBG(SENSOR_DEVICE_TYPE | 
                             ((SENSOR_I2C_IDX_UV+1) << 4)); 
            SENSOR_FUSION_DBG_ID(SENSOR_DBG_ID_UV, SLAVE_ADDRESS_UV);

            gu16SensorPeriod[SENSOR_I2C_IDX_UV] = 1000;

            I2C_CMD_SET(I2C_CMD_UV,
                        SLAVE_ADDRESS_UV,
                        DATA_ADDRESS_UV,
                        DATA_LENGTH_UV,
                        1000,
                        SENSOR_BUFFER_UV);    

            if(do_initial_uv(1))
            {
                gstSensorInitDone.UV= 1;

                hid_update_sensor_state(HID_SENSOR_UV, 
                                        HID_USAGE_SENSOR_STATE_READY_ENUM);
                
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ENABLE_IRQ;
            }
            else
            {
                if(sensor_device_retry())
                {
                    SENSOR_INIT_FAIL_HANDLER(SENSOR_I2C_IDX_UV, 
                                             SENSOR_INIT_STEP_FAIL_CAUSE_UV, 
                                             SENSOR_INIT_STEP_INIT_ENABLE_IRQ);
                }
            }
            #else
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ENABLE_IRQ;
            #endif // } _ENABLE_UV_

			hook_sensor_init_end();
            break;

        case SENSOR_INIT_STEP_INIT_ENABLE_IRQ:
            //  Enable irq
            i2c_drv_irq_init(SENSOR_I2C_CH);

            gu8I2CCmdCnt = SENSOR_I2C_CMD_CNT;

            SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_READY;
            SENSOR_INIT_STEP_FAIL_CAUSE = SENSOR_INIT_STEP_FAIL_CAUSE_NO;
            break;

        case SENSOR_INIT_STEP_INIT_READY:
            #ifndef DEF_SENSOR_ONE_SHOT_MODE // { 
            i2c_drv_cmd_read_stop(ghI2CDrv);
            i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, gu8I2CCmdCnt, FALSE);   
            #endif  // } DEF_SENSOR_ONE_SHOT_MODE
            break;

        case SENSOR_INIT_STEP_INIT_FAIL:                        
            // Error handles of sensor initial fail 
            //I2C_RESET_CMD();
            
            #ifndef EC_FEATURE_ONLY_SUPPORT_FUSION  // { RTOS only
            if((SENSOR_INIT_STEP_FAIL_CAUSE >= SENSOR_INIT_STEP_FAIL_CAUSE_ACC) 
             && (SENSOR_INIT_STEP_FAIL_CAUSE <= SENSOR_INIT_STEP_FAIL_CAUSE_UV))
            {
	          SENSOR_INIT_STEP = SENSOR_INIT_STEP_INIT_ACC + 
                (SENSOR_INIT_STEP_FAIL_CAUSE - SENSOR_INIT_STEP_FAIL_CAUSE_ACC);
            }
            #endif  // } EC_FEATURE_ONLY_SUPPORT_FUSION
            break;
            
        default :
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
            break;
    }
}

//******************************************************************************
// Function name:
//   sensor_one_shot_main()
// Description:
//   get sensor data and do fusion calibrate
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   for one shot mode only
//******************************************************************************
void SENSOR_CODE sensor_one_shot_main(void)
{
    UINT8 u8I2CReadMode = 0; // 0: normal, 1: other
    UINT16 u16CmdFlag = gusSensorOneShotCmd;
    UINT16 u16CmdMask;

    #if _ENABLE_ALS_ // {
    UINT32 u32Lux;
    #endif // } _ENABLE_ALS_

    #if _ENABLE_ACC2_ // {
    int  angle;
    #endif // } _ENABLE_ACC2_

    if(u16CmdFlag)
    {   
        u16CmdMask = u16CmdFlag;
        
        SENSOR_GET_MUTEX(MUTEX_ID_MAIN);

        if(gu8HIDStatus != HID_STATUS_NORMAL){
            SENSOR_RELEASE_MUTEX();
            return;
        }
        
        //GPDRA = u16CmdFlag;
        do{
            #ifdef DEF_SENSOR_ONE_SHOT_MODE // {  

            #if _ENABLE_SPECIAL_READ_ // {
            if(u8I2CReadMode == 0){
               i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, gu8I2CCmdCnt, FALSE);          
            }
            #endif // } _ENABLE_SPECIAL_READ_

            #if _ENABLE_ACC_ // {
            IF_I2C_MODE_MATCH_ACC(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_ACC)) && 
                  (gstSensorInitDone.Acc))
                {    
                    // Do get acc    
                    #ifndef _ENABLE_SPECIAL_READ_ACC_ // {
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_IDX_ACC, 
                                          SENSOR_I2C_IDX_ACC, u16CmdFlag);
                    #else
                    do_get_data_acc(SENSOR_BUFFER_ACC);
                    #endif // } _ENABLE_SPECIAL_READ_ACC_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_ACC));

                    do_data_correct_acc(SENSOR_BUFFER_ACC);
                }
            }
            #endif // } _ENABLE_ACC_

            #if _ENABLE_MAG_ // {
            IF_I2C_MODE_MATCH_MAG(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_MAG)) && 
                  (gstSensorInitDone.Mag))
                {
                    // Do get mag  
                    #ifndef _ENABLE_SPECIAL_READ_MAG_ // {
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_IDX_MAG, 
                                          SENSOR_I2C_IDX_MAG, u16CmdFlag);
                    #else
                    do_get_data_mag(SENSOR_BUFFER_MAG);
                    #endif // } _ENABLE_SPECIAL_READ_MAG_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_MAG));

                    do_data_correct_mag(SENSOR_BUFFER_MAG);   
                }
            }
            #endif // } _ENABLE_MAG_

            #if _ENABLE_GYO_ // {
            IF_I2C_MODE_MATCH_GYO(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_GYO)) && 
                  (gstSensorInitDone.Gyo))
                {
                    // Do get gyr   
                    #ifndef _ENABLE_SPECIAL_READ_GYO_ // {
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_IDX_GYO, 
                                          SENSOR_I2C_IDX_GYO, u16CmdFlag);
                    #else
                    do_get_data_gyo(SENSOR_BUFFER_GYO);
                    #endif // } _ENABLE_SPECIAL_READ_GYO_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_GYO));

                    do_data_correct_gyo(SENSOR_BUFFER_GYO);
                }
            }
            #endif // }  _ENABLE_GYO_

            #if _ENABLE_ALS_ // {
            IF_I2C_MODE_MATCH_ALS(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_ALS)) && 
                  (gstSensorInitDone.Als))
                {   
                    #ifndef _ENABLE_SPECIAL_READ_ALS_ // {
                    // switch cmd to Als
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_ALS;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    
                    // Do get als       
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_ALS, u16CmdFlag);
                    #else
                    do_get_data_als(SENSOR_BUFFER_ALS);
                    #endif // } _ENABLE_SPECIAL_READ_ALS_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_ALS));

                    // update als data
                    do_data_correct_als(SENSOR_BUFFER_ALS); 

                    u32Lux = do_unit_correct_als(*(LPUINT16)SENSOR_BUFFER_ALS);
                    
                    HID_GET_MUTEX();
                    if(u32Lux < 0xFFFF){
                        LightIlluminace = u32Lux;
                    }
                    else{
                        LightIlluminace = 0xFFFF;
                    }

                    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
                    do_get_color_info_als(&ColorTemp, 
                                          &ChromaticityX, &ChromaticityY);
                    #endif // } __ENABLE_ALS_COLOR_VALUE__
                    
                    #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
                    LightCustom = 
                      do_unit_correct_als(*(LPUINT16)(SENSOR_BUFFER_ALS + 2));
                    #endif // } __ENABLE_ALS_CUSTOM_VALUE__
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_ALS_

            #if _ENABLE_ACC2_ // {
            IF_I2C_MODE_MATCH_ACC2(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_ACC2)) && 
                  (gstSensorInitDone.Acc2))
                {
                    #ifndef _ENABLE_SPECIAL_READ_ACC2_ // {
                    // switch cmd to 2nd Acc
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_ACC2;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);

                    // Do get acc2           
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_ACC2, u16CmdFlag);
                    #else
                    do_get_data_acc2(SENSOR_BUFFER_ACC2);
                    #endif // } _ENABLE_SPECIAL_READ_ACC2_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_ACC2));

                    do_data_correct_acc2(SENSOR_BUFFER_ACC2);

                    // sync 1st Acc data
                    if(IS_MASK_CLEAR(u16CmdMask, BIT(SENSOR_I2C_IDX_ACC)))
                    {
                        // Shawn: acc1 & acc2 i2c read mode must be the same
                        #ifndef _ENABLE_SPECIAL_READ_ACC_ // {
                        I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_IDX_ACC, 
                                              SENSOR_I2C_IDX_ACC, u16CmdFlag);
                        #else
                        do_get_data_acc(SENSOR_BUFFER_ACC);
                        #endif // } _ENABLE_SPECIAL_READ_ACC_

                        do_data_correct_acc(SENSOR_BUFFER_ACC);
                        
                        //SENSOR_FUSION_DBG_INFO("Sync 1st Acc\r\n");
                    }

                    // update angle & acc2 data
                    HID_GET_MUTEX();
                    get_custom_angle(SENSOR_BUFFER_ACC, SENSOR_BUFFER_ACC2, 
                                     CustomAngle, Accelerometer2, 
                                     gu16AngleFreq);
                    HID_RELEASE_MUTEX();

                    // switch formation
                    angle = UNIT_DEGREE(CustomAngle[2]);
                    //SENSOR_FUSION_DBG_INFO("Angle: %3d\r\n", angle);
                    if((angle > COMPASS_FORMATION_THRESHOLD) && 
                       (angle < (360-COMPASS_FORMATION_THRESHOLD))) 
                    {
                        if(++gcAngleDebounce >= COMPASS_FORMATION_DEBOUNCE){
                            gcAngleDebounce = COMPASS_FORMATION_DEBOUNCE;
                        }

                        if(gcAngleDebounce == COMPASS_FORMATION_DEBOUNCE){
                            do_switch_calibrate_formation(0);
                        }
                    }
                    else
                    {
                        if(--gcAngleDebounce <= -COMPASS_FORMATION_DEBOUNCE){
                            gcAngleDebounce = -COMPASS_FORMATION_DEBOUNCE;   
                        }

                        if(gcAngleDebounce == -COMPASS_FORMATION_DEBOUNCE){
                            do_switch_calibrate_formation(1);
                        }
                    }
                }
            }
            #endif // }  _ENABLE_ACC2_

            #if _ENABLE_PRS_ // {
            IF_I2C_MODE_MATCH_PRS(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_PRS)) && 
                  (gstSensorInitDone.Prs))
                {              
                    #ifndef _ENABLE_SPECIAL_READ_PRS_ // {
                    // switch cmd to pressure
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_PRS;            
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);     

                    // Do get pressure                       
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_PRS, u16CmdFlag);
                    #else
                    do_get_data_prs(SENSOR_BUFFER_PRS);
                    #endif // } _ENABLE_SPECIAL_READ_PRS_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_PRS));

                    // update prs data
                    do_data_correct_prs(SENSOR_BUFFER_PRS);         
                    HID_GET_MUTEX();
                    PressureBar = 
                        do_unit_correct_prs(*((LPUINT32)(SENSOR_BUFFER_PRS)));
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_PRS_

            #if _ENABLE_HYG_ // {
            IF_I2C_MODE_MATCH_HYG(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_HYG)) && 
                  (gstSensorInitDone.Hyg))
                {              
                    #ifndef _ENABLE_SPECIAL_READ_HYG_ // {
                    // switch cmd to humidity
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_HYG;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);     

                    // Do get humidity                       
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_HYG, u16CmdFlag);
                    #else
                    do_get_data_hyg(SENSOR_BUFFER_HYG);
                    #endif // } _ENABLE_SPECIAL_READ_HYG_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_HYG));
                    
                    // update hyg data
                    do_data_correct_hyg(SENSOR_BUFFER_HYG);         
                    HID_GET_MUTEX();
                    HygValue = 
                        do_unit_correct_hyg(*((LPUINT16)(SENSOR_BUFFER_HYG)));
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_HYG_

            #if _ENABLE_TEMP_ // {
            IF_I2C_MODE_MATCH_TEMP(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_TEMP)) && 
                  (gstSensorInitDone.Temp))
                {              
                    #ifndef _ENABLE_SPECIAL_READ_TEMP_ // {
                    // switch cmd to temperature
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_TEMP;            
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);     

                    // Do get temperature                       
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_TEMP, u16CmdFlag);
                    #else
                    do_get_data_temp(SENSOR_BUFFER_TEMP);
                    #endif // } _ENABLE_SPECIAL_READ_TEMP_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_TEMP));

                    // update temp data
                    do_data_correct_temp(SENSOR_BUFFER_TEMP);         
                    HID_GET_MUTEX();
                    TempValue = 
                        do_unit_correct_temp(*((LPUINT16)(SENSOR_BUFFER_TEMP)));
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_TEMP_   

            #if _ENABLE_PROX_ // {
            IF_I2C_MODE_MATCH_PROX(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_PROX)) && 
                  (gstSensorInitDone.Prox))
                {              
                    #ifndef _ENABLE_SPECIAL_READ_PROX_ // {
                    // switch cmd to proximity
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_PROX;            
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);     

                    // Do get proximity                       
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_PROX, u16CmdFlag);
                    #else
                    do_get_data_prox(SENSOR_BUFFER_PROX);
                    #endif // } _ENABLE_SPECIAL_READ_PROX_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_PROX));

                    // update temp data
                    do_data_correct_prox(SENSOR_BUFFER_PROX);
                    HID_GET_MUTEX();
                    ProxOutOfRangeFlag = *(SENSOR_BUFFER_PROX + 2);
                    ProxValue = 
                        do_unit_correct_prox(*((LPUINT16)(SENSOR_BUFFER_PROX)));
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_PROX_                 

            #if _ENABLE_UV_ // {
            IF_I2C_MODE_MATCH_UV(u8I2CReadMode)
            {
                if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_I2C_IDX_UV)) && 
                  (gstSensorInitDone.UV))
                {              
                    #ifndef _ENABLE_SPECIAL_READ_UV_ // {
                    // switch cmd to uv
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_UV;            
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);     

                    // Do get uv                     
                    I2C_GET_ONE_SHOT_DATA(SENSOR_I2C_CMD_ALT, 
                                          SENSOR_I2C_IDX_UV, u16CmdFlag);
                    #else
                    do_get_data_uv(SENSOR_BUFFER_UV);
                    #endif // } _ENABLE_SPECIAL_READ_UV_
                    CLEAR_MASK(u16CmdFlag, BIT(SENSOR_I2C_IDX_UV));

                    // update uv data
                    do_data_correct_uv(SENSOR_BUFFER_UV);         
                    HID_GET_MUTEX();
                    UVValue = 
                        do_unit_correct_uv(*((LPUINT16)(SENSOR_BUFFER_UV)));
                    #ifdef __ENABLE_UV_2ND_VALUE__ // {
                    UVValue2 = 
                        do_unit_correct_uv(*((LPUINT16)(SENSOR_BUFFER_UV + 2)));
                    #endif // } __ENABLE_UV_2ND_VALUE__
                    HID_RELEASE_MUTEX();
                }
            }
            #endif // }  _ENABLE_UV_     
            #else
            HID_GET_MUTEX();
            LightIlluminace = 
                do_unit_correct_als(*((LPUINT16)(SENSOR_BUFFER_ALS)));
            #ifdef __ENABLE_ALS_CUSTOM_VALUE__ // {
            LightCustom = 
                do_unit_correct_als(*((LPUINT16)(SENSOR_BUFFER_ALS + 2)));
            #endif // } __ENABLE_ALS_CUSTOM_VALUE__
            HID_RELEASE_MUTEX();            
            #endif  // }  DEF_SENSOR_ONE_SHOT_MODE
            
            #if _ENABLE_SPECIAL_READ_ // {
            if(u8I2CReadMode == 0){
                i2c_drv_cmd_read_stop(ghI2CDrv);
            }
            u8I2CReadMode++;
            #else
            break;
            #endif // } _ENABLE_SPECIAL_READ_
        }while(u8I2CReadMode < 2);

        if(IS_MASK_SET(u16CmdFlag, BIT(SENSOR_FUSION_IDX)) && 
          ((gstSensorInitDone.Acc) || 
           (gstSensorInitDone.Mag) || 
           (gstSensorInitDone.Gyo)))
        {    
            if(DEF_I2C_HID)
            {
                #ifdef __ENABLE_HID_I2C_API__ // {
                sensor_fusion_calc(i2c_hid_data_set, NULL);
                #endif // } __ENABLE_HID_I2C_API__
            }    
            else if(DEF_USB_HID)
            {
                #ifdef __ENABLE_HID_USB_API__ // {
                sensor_fusion_calc(usb_slave_hid_data_set, NULL);
                #endif // } __ENABLE_HID_USB_API__
            }
            else
            {
                sensor_fusion_calc(send_packet, (void*)64);
            }         
            CLEAR_MASK(u16CmdFlag, BIT(SENSOR_FUSION_IDX));
        }        
        SENSOR_RELEASE_MUTEX();   

        CLEAR_MASK_EXT(gusSensorOneShotCmd, u16CmdMask);

        I2C_DEBUG_PORT5((UINT8)(gusSensorErrStatus & 0x00FF));
        I2C_DEBUG_PORT6((UINT8)((gusSensorErrStatus & 0xFF00) >> 8));
    }

    #ifdef __HID_FUSION_FUN__ // {
    if(gu8HIDStatus == HID_STATUS_NORMAL){
        hal_suspend_task(tthi_FUSION); 
    }
    #endif // } __HID_FUSION_FUN__
}

//******************************************************************************
// Function name:
//    sensor_wait_done_and_stop
// Description:
//    wait fusion task done and stop it
// Arguments:
//    n/a
// Return Values:
//    n/a
// Note:
//    n/a
//******************************************************************************
void SENSOR_CODE_L sensor_wait_done_and_stop(void)
{
    SENSOR_GET_MUTEX(MUTEX_ID_STOP);
    sensor_get_data_stop();  
    gu8HIDStatus = HID_STATUS_STOP;
    SENSOR_RELEASE_MUTEX();
}

//******************************************************************************
// Function name:
//    sensor_restart
// Description:
//    restart fusion task
// Arguments:
//    n/a
// Return Values:
//    n/a
// Note:
//    n/a
//******************************************************************************
void SENSOR_CODE_L sensor_restart(void)
{
    SENSOR_GET_MUTEX(MUTEX_ID_RESTART);
    RESET_FUSION(); 
    SENSOR_RELEASE_MUTEX();
    RESUME_FUSION_TASK();
}

//******************************************************************************
// Function name:
//   sensor_do_hcc_service()
// Description:
//   set sensor task to do hcc_service()
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE sensor_do_hcc_service(void)
{
    SENSOR_GET_MUTEX(MUTEX_ID_HID_HCC);
    gu8HIDStatus = HID_STATUS_HCC;
    SENSOR_RELEASE_MUTEX();
}

//******************************************************************************
// Function name:
//   fusion_main()
// Description:
//   fusion task main function
//   HID_STATUS_INIT   -> initial parameter, I2C and sensor
//   HID_STATUS_NORMAL -> get sensor data and do fution calibrate
//   other             -> sleep 1 ms
// Arguments:
//   n/a
// Return Values:
//   n/a
// Note:
//   n/a
//******************************************************************************
void SENSOR_CODE fusion_main(void)
{
    //
    //
    //
    SENSOR_I2C_CH = DEF_SENSOR_I2C_CH;
    SENSOR_CREATE_MUTEX();
    #ifndef EC_FEATURE_ONLY_SUPPORT_FUSION
    while(1)
    #endif
    {
        SENSOR_FUSION_DBG_SET_FLAG();
        if(gu8HIDStatus == HID_STATUS_INIT)
        {  
            SENSOR_GET_MUTEX(MUTEX_ID_INIT_SENSOR);
            pure_ec_init_clear_fusion_variables();
            if(SENSOR_INIT_STEP == SENSOR_INIT_STEP_RESET_PARAMETER)
            {
                sensor_stored_data_load();
            }
            sensor_init();
            sensor_get_data_start();
            SENSOR_RELEASE_MUTEX();
            hal_yield_thread();
        }
        else if(gu8HIDStatus == HID_STATUS_NORMAL)
        {
            #ifndef EC_FEATURE_ONLY
            sensor_one_shot_main();      
            #endif  // ndef EC_FEATURE_ONLY
        }
        else if(gu8HIDStatus == HID_STATUS_HCC)
        {
            SENSOR_GET_MUTEX(MUTEX_ID_HCC);
            hcc_service();
            RESET_FUSION();
            SENSOR_RELEASE_MUTEX();
        }
        else if(gu8HIDStatus == HID_STATUS_SENSOR_TIMEOUT)
        {
            SENSOR_GET_MUTEX(MUTEX_ID_TIMEOUT);
            i2c_channel_reset(SENSOR_I2C_CH);

            if(gu8SensorTimeoutCnt++ < MAX_SENSOR_TIMEOUT_CNT)
            {
                gu8HIDStatus = HID_STATUS_INIT;
                SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;    
            }
            else
            {
                gu8SensorTimeoutCnt = 0;
                gu8HIDStatus = HID_STATUS_SENSOR_ERROR;
            }   
            SENSOR_RELEASE_MUTEX();
        }
		#if 0
        else if(gu8HIDStatus == HID_STATUS_SENSOR_ERROR)
        {
            hid_update_sensor_state(HID_SENSOR_DEFAULT, 
                                    HID_USAGE_SENSOR_STATE_ERROR_ENUM);
            hal_sleep(1);
        }
		#else
		else if(gu8HIDStatus == HID_STATUS_SENSOR_ERROR)
		{
//<<
			if (((gu8SensorErrorCnt++) % 10) == 0)
			{
				gu8HIDStatus = HID_STATUS_INIT;
				SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;  
			}
//>>
			hid_update_sensor_state(HID_SENSOR_DEFAULT, 
									HID_USAGE_SENSOR_STATE_ERROR_ENUM);
			hal_sleep(1);
		}
		#endif
        else if(gu8HIDStatus == HID_STATUS_POWEROFF)
        {
            gu8SensorTimeoutCnt = 0;
            i2c_drv_poweroff_handler(SENSOR_I2C_CH);
            gu8HIDStatus  = HID_STATUS_STOP;     
        }
        else
        {
            Sensor_API_ID_Accelerometer = ACC_SENSOR_API_ID;
            Sensor_API_ID_Compass = MAG_SENSOR_API_ID;
            Sensor_API_ID_Gyrometer = GYO_SENSOR_API_ID;
            Sensor_API_ID_Ambientlight = ALS_SENSOR_API_ID;
            Sensor_API_ID_Accelerometer2 = ACC2_SENSOR_API_ID;
            Sensor_API_ID_Pressure = PRS_SENSOR_API_ID;     
            Sensor_API_ID_Hygrometer = HYG_SENSOR_API_ID;
            Sensor_API_ID_Temperature = TEMP_SENSOR_API_ID;
            Sensor_API_ID_Proximity = PROX_SENSOR_API_ID;
            Sensor_API_ID_Ultraviolet = UV_SENSOR_API_ID;

            #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION

            #else
            //debug_putc((gu8HIDStatus + '0'));
            hal_sleep(1);
            #endif
        }
    }
}

//******************************************************************************
// Function name:
//   sensor_device_ready()
// Description:
//   To check all sensor devices init OK or not
// Arguments:
//   n/a
// Return Values:
//   1: OK
//   0: not ready
// Note:
//   n/a
//******************************************************************************
UINT8 SENSOR_CODE sensor_device_ready(void)
{
    UINT8 l_ready;
    if(SENSOR_INIT_STEP == SENSOR_INIT_STEP_INIT_READY)
    {
        l_ready = SENSOR_DEVICE_ALL_READY;
    }
    else
    {
        l_ready = SENSOR_DEVICE_NOT_ALL_READY;
    }

    return(l_ready);
}

//******************************************************************************
// Function name:
//   sensor_device_retry()
// Description:
//   To check allow re-init sensor device or not.
// Arguments:
//   n/a
// Return Values:
//   1: retry time-out
//   0: allow retry
// Note:
//   n/a
//******************************************************************************
UINT8 SENSOR_CODE sensor_device_retry(void)
{
    if(SENSOR_DEVICE_INIT_RETRY < SENSOR_DEVICE_INIT_RETRY_TIME)
    {
        SENSOR_DEVICE_INIT_RETRY++;
        return(0);
    }
    else
    {
        return(1);
    }
}

#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION // {
//*****************************************************************************
//
// fusion calculate.
//
//  parameter :
//
//
//  return :
//      none
//
//*****************************************************************************
void SENSOR_CODE pure_ec_sensor_fusion_calc(SENSOR_CALLBACK_FUNC pfnCallback, 
                                            void *pParam)
{
    #ifndef DEF_SENSOR_ONE_SHOT_MODE  // {  
    memset(gpSensorBuffer, 0x00, 18);
    if(pure_ec_i2c_drv_cmd_read(ghI2CDrv, gppI2CCmd, gu8I2CCmdCnt) == 0x00)
    #endif // } DEF_SENSOR_ONE_SHOT_MODE
    {
        #if defined(FusionLogBuffer) // {
        *(FusionLogBuffer+1) = 0;
        (*FusionLogBuffer)++;
        memcpy(FusionLogBuffer+2, gpSensorBuffer, 18);
        *(FusionLogBuffer+1) = (UINT8)gu16FusionFreq;
        #endif // } FusionLogBuffer

        if(gu16FusionFreq)
        {
			#ifdef __ENABLE_AKM_PG_LIB__ // {
            sensor_do_akm_fusion((P_SENSOR_BUFFER_WIN)gpSensorBuffer, gu16FusionFreq);
			#else
            fusion_calc(gpSensorBuffer, gu16FusionFreq);
			#endif // } __ENABLE_AKM_PG_LIB__
        }
        pfnCallback(gpSensorBuffer, pParam);
    }
}

int SENSOR_CODE pure_ec_sensor_poweroff_done(void)
{
    return (gu8HIDStatus != HID_STATUS_POWEROFF);
}

#ifdef DEF_SENSOR_ONE_SHOT_MODE  // {  

static inline void SENSOR_CODE pure_ec_sensor_one_shot_cmd_next_step(void)
{
    ONE_SHOT_CMD_INDEX++;
}

void SENSOR_CODE_L pure_ec_sensor_one_shot_cmd_timeout(void)
{
    SENSOR_ISR_SERVICE_FLAG_OS = 0;
    ONE_SHOT_CMD_INDEX++;

    // SW reset
    if(++SENSOR_I2C_CHANNEL_WDT > 100)
    {
        SENSOR_I2C_CHANNEL_WDT = 0x00;
        i2c_ch_reset(SENSOR_I2C_CH);     
        gu8HIDStatus = HID_STATUS_INIT;
        SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
    }
}

//*****************************************************************************
//
// sensor one shot main function for pure ec firmware.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void SENSOR_CODE pure_ec_sensor_one_shot_main(void)
{ 
    if(SENSOR_ISR_SERVICE_FLAG_OS==0)
    {
        ONE_SHOT_CMD_WDT = 0x00;
        
        switch(ONE_SHOT_CMD_INDEX)
        {
            case ONE_SHOT_CMD_INDEX_READ_ACC:
            #if _ENABLE_ACC_
            if((gstHIDPowerState.Acc == 1) || (hid_check_power_mode() == 1))
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ACC)))
                {    
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ACC));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get acc    
                    #ifndef _ENABLE_SPECIAL_READ_ACC_ // {
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_IDX_ACC);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_acc(SENSOR_BUFFER_ACC);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;
                    #endif // } _ENABLE_SPECIAL_READ_ACC_
                    
                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_ACC;
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif // _ENABLE_ACC_
            break;

            case ONE_SHOT_CMD_INDEX_READ_MAG:
            #if _ENABLE_MAG_
            if((gstHIDPowerState.Mag == 1) || (hid_check_power_mode() == 1))
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_MAG)))
                {
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_MAG));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get mag  
                    #ifndef _ENABLE_SPECIAL_READ_MAG_ // {
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_IDX_MAG);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_mag(SENSOR_BUFFER_MAG);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;
                    #endif // } _ENABLE_SPECIAL_READ_MAG_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_MAG;
                } 
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif // _ENABLE_MAG_
            break;

            case ONE_SHOT_CMD_INDEX_READ_GYO:
            #if _ENABLE_GYO_
            if((gstHIDPowerState.Gyo == 1) || (hid_check_power_mode() == 1))
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_GYO)))
                {
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_GYO));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get gyr    
                    #ifndef _ENABLE_SPECIAL_READ_GYO_ // {
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_IDX_GYO);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_gyo(SENSOR_BUFFER_GYO);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;  
                    #endif // } _ENABLE_SPECIAL_READ_GYO_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_GYO;
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif // _ENABLE_GYO_
            break;

            case ONE_SHOT_CMD_INDEX_READ_ALS:
            #if _ENABLE_ALS_
            if(gstHIDPowerState.Als == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ALS)))
                {    
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ALS));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    #ifndef _ENABLE_SPECIAL_READ_ALS_ // {
                    // switch cmd to Als
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_ALS;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    
                    // Do get als
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_als(SENSOR_BUFFER_ALS);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;  
                    #endif // } _ENABLE_SPECIAL_READ_ALS_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_ALS;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif // _ENABLE_ALS_
            break;

            case ONE_SHOT_CMD_INDEX_READ_ACC2:
            #if _ENABLE_ACC2_
            if(((gstHIDPowerState.Acc == 1) || (hid_check_power_mode() == 1))
                #ifndef DEF_SENSOR_ACC2_ALWAYS_ON // {
                && (gstHIDPowerState.Custom == 1) 
                #endif // } DEF_SENSOR_ACC2_ALWAYS_ON
                )
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ACC2)))
                {
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_ACC2));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    #ifndef _ENABLE_SPECIAL_READ_ACC2_ // {
                    // switch cmd to 2nd Acc
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_ACC2;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);

                    // Do get acc2
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_acc2(SENSOR_BUFFER_ACC2);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE; 
                    #endif // } _ENABLE_SPECIAL_READ_ACC2_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_ACC2;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif // _ENABLE_ACC2_
            break;

            case ONE_SHOT_CMD_INDEX_READ_PRS:
            #if _ENABLE_PRS_
            if(gstHIDPowerState.Prs == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_PRS)))
                {  
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_PRS));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get pressure                       
                    #ifndef _ENABLE_SPECIAL_READ_PRS_ // {
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_PRS;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_prs(SENSOR_BUFFER_PRS);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;   
                    #endif // } _ENABLE_SPECIAL_READ_PRS_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_PRS;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif //  _ENABLE_PRS_
            break;

            case ONE_SHOT_CMD_INDEX_READ_HYG:
            #if _ENABLE_HYG_
            if(gstHIDPowerState.Hyg == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_HYG)))
                {  
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_HYG));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get humidity                       
                    #ifndef _ENABLE_SPECIAL_READ_HYG_ // {
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_HYG;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_hyg(SENSOR_BUFFER_HYG);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;  
                    #endif // } _ENABLE_SPECIAL_READ_HYG_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_HYG;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif //  _ENABLE_HYG_
            break;

            case ONE_SHOT_CMD_INDEX_READ_TEMP:
            #if _ENABLE_TEMP_
            if(gstHIDPowerState.Temp == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_TEMP)))
                {  
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_TEMP));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get temperature                       
                    #ifndef _ENABLE_SPECIAL_READ_TEMP_ // {
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_TEMP;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_temp(SENSOR_BUFFER_TEMP);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE; 
                    #endif // } _ENABLE_SPECIAL_READ_TEMP_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_TEMP;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }                
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif //  _ENABLE_TEMP_
            break;

           case ONE_SHOT_CMD_INDEX_READ_PROX:
            #if _ENABLE_PROX_
            if(gstHIDPowerState.Prox == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_PROX)))
                {  
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_PROX));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get proximity                       
                    #ifndef _ENABLE_SPECIAL_READ_PROX_ // {
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_PROX;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_prox(SENSOR_BUFFER_PROX);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;   
                    #endif // } _ENABLE_SPECIAL_READ_PROX_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_PROX;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }                
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif //  _ENABLE_PROX_
            break;            

            case ONE_SHOT_CMD_INDEX_READ_UV:
            #if _ENABLE_UV_
            if(gstHIDPowerState.UV == 1)
            {
                if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_UV)))
                {  
                    CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_I2C_IDX_UV));

                    SENSOR_ISR_SERVICE_FLAG_OS = 1;
                    
                    // Do get uv                       
                    #ifndef _ENABLE_SPECIAL_READ_UV_ // {
                    gppI2CCmd[SENSOR_I2C_CMD_ALT] = &I2C_CMD_UV;
                    i2c_drv_set_cmd(ghI2CDrv, gppI2CCmd, SENSOR_I2C_CMD_ALT);
                    i2c_drv_fire_one_shot(SENSOR_I2C_CH, SENSOR_I2C_CMD_ALT);
                    #else
                    i2c_drv_cmd_read_stop(ghI2CDrv);
                    do_get_data_uv(SENSOR_BUFFER_UV);
                    i2c_drv_cmd_read_start(ghI2CDrv, gppI2CCmd, 
                                           gu8I2CCmdCnt, FALSE); 

                    SENSOR_ISR_SERVICE_FLAG_OS = 0xFE;  
                    #endif // } _ENABLE_SPECIAL_READ_UV_

                    ONE_SHOT_MAIN_SENSOR_INDEX = SENSOR_I2C_IDX_UV;
                }
                else
                {
                    pure_ec_sensor_one_shot_cmd_next_step();
                }                
            }
            else
            {
                pure_ec_sensor_one_shot_cmd_next_step();
            }
            #else
            pure_ec_sensor_one_shot_cmd_next_step();
            #endif //  _ENABLE_UV_
            break;            

            case ONE_SHOT_CMD_INDEX_FUSION:
            if(IS_MASK_SET(gusSensorOneShotCmd, BIT(SENSOR_FUSION_IDX)))
            {
                CLEAR_MASK(gusSensorOneShotCmd, BIT(SENSOR_FUSION_IDX));
                
                if(DEF_I2C_HID)
                {
                    #ifdef __ENABLE_HID_I2C_API__ // {
                    pure_ec_sensor_fusion_calc(i2c_hid_data_set, NULL);
                    #endif // } __ENABLE_HID_I2C_API__
                }    
                else if(DEF_USB_HID)
                {
                    #ifdef __ENABLE_HID_USB_API__ // {
                    pure_ec_sensor_fusion_calc(usb_slave_hid_data_set, NULL);
                    #endif // } __ENABLE_HID_USB_API__
                }
                else
                {
                    sensor_fusion_calc(send_packet, (void*)64);
                }
            }

            ONE_SHOT_CMD_INDEX = ONE_SHOT_CMD_INDEX_READ_ACC;
            break;

            default :
                ONE_SHOT_CMD_INDEX = ONE_SHOT_CMD_INDEX_READ_ACC;
                break;
        }

        if(ONE_SHOT_CMD_INDEX>ONE_SHOT_CMD_INDEX_FUSION)
        {
            ONE_SHOT_CMD_INDEX = ONE_SHOT_CMD_INDEX_READ_ACC;
        }      

        SENSOR_FUSION_DBG_I2C(
            (gpI2CCmd[ONE_SHOT_MAIN_SENSOR_INDEX].u8SlaveAddr|0x01),
             gpI2CCmd[ONE_SHOT_MAIN_SENSOR_INDEX].u8Addr, 0xCC);
    }
    else
    {
        if(++ONE_SHOT_CMD_WDT>30)
        {
            ONE_SHOT_CMD_WDT = 0x00;
            pure_ec_sensor_one_shot_cmd_timeout();

            SENSOR_FUSION_DBG_I2C_RTN(0xFF); 
        }
    }
}

//*****************************************************************************
//
// sensor one shot isr function for pure ec firmware.
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void SENSOR_CODE pure_ec_sensor_one_shot_isr(void)
{
    #if _ENABLE_ALS_ // {
    UINT32 u32Lux;
    #endif // } _ENABLE_ALS_

	#if _ENABLE_ACC2_
     #define PAD_LID()      IS_MASK_CLEAR(GPDRD,BIT(1))
    int  angle, angle_flag;
#endif


    #if _ENABLE_ACC_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_ACC)
    {
        #ifndef _ENABLE_SPECIAL_READ_ACC_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_IDX_ACC]);
        #endif // } _ENABLE_SPECIAL_READ_ACC_

        do_data_correct_acc(SENSOR_BUFFER_ACC);
    }
    #endif // _ENABLE_ACC_

    #if _ENABLE_MAG_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_MAG)
    {
        #ifndef _ENABLE_SPECIAL_READ_MAG_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_IDX_MAG]);
        #endif // } _ENABLE_SPECIAL_READ_MAG_

        do_data_correct_mag(SENSOR_BUFFER_MAG);        
    }        
    #endif // _ENABLE_MAG_

    #if _ENABLE_GYO_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_GYO)
    {
        #ifndef _ENABLE_SPECIAL_READ_GYO_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_IDX_GYO]);  
        #endif // } _ENABLE_SPECIAL_READ_GYO_

        do_data_correct_gyo(SENSOR_BUFFER_GYO);        
    }
    #endif // _ENABLE_GYO_

    #if _ENABLE_ALS_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_ALS)
    {
        #ifndef _ENABLE_SPECIAL_READ_ALS_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_ALS_

        do_data_correct_als(SENSOR_BUFFER_ALS);  

        // update als data
        u32Lux = do_unit_correct_als(*((LPUINT16)(SENSOR_BUFFER_ALS)));
        
        if(u32Lux < 0xFFFF){
            LightIlluminace = u32Lux;
        }
        else{
            LightIlluminace = 0xFFFF;
        } 
    }
    #endif // _ENABLE_ALS_

    #if _ENABLE_ACC2_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_ACC2)
    {
        #ifndef _ENABLE_SPECIAL_READ_ACC2_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_ACC2_

        do_data_correct_acc2(SENSOR_BUFFER_ACC2);

        // update angle & acc2 data
        angle_flag= get_custom_angle(SENSOR_BUFFER_ACC, SENSOR_BUFFER_ACC2, 
                         CustomAngle, Accelerometer2, 
                         gu16AngleFreq);

	  // switch formation
        if ((angle_flag !=0) && PAD_LID())                        
        {    
        //     CustomAngle[0] = 0x324;
        //     CustomAngle[1] = 0x320;
               CustomAngle[2] = 0;
        } 


        // switch formation
        angle = UNIT_DEGREE(CustomAngle[2]);

        if((angle > COMPASS_FORMATION_THRESHOLD) && 
           (angle < (360-COMPASS_FORMATION_THRESHOLD))) 
        {
            if(++gcAngleDebounce >= COMPASS_FORMATION_DEBOUNCE){
                gcAngleDebounce = COMPASS_FORMATION_DEBOUNCE;
            }

            if(gcAngleDebounce == COMPASS_FORMATION_DEBOUNCE){
                set_compass_calibrate_formation(0);
            }
        }
        else
        {
            if(--gcAngleDebounce <= -COMPASS_FORMATION_DEBOUNCE){
                gcAngleDebounce = -COMPASS_FORMATION_DEBOUNCE;   
            }

            if(gcAngleDebounce == -COMPASS_FORMATION_DEBOUNCE){
                set_compass_calibrate_formation(1);
            }
        }
    }
    #endif // _ENABLE_ACC2_

    #if _ENABLE_PRS_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_PRS)
    {
        #ifndef _ENABLE_SPECIAL_READ_PRS_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_PRS_

        do_data_correct_prs(SENSOR_BUFFER_PRS);         

        PressureBar = do_unit_correct_prs(*((LPUINT32)(SENSOR_BUFFER_PRS)));
    }
    #endif //  _ENABLE_PRS_ 

    #if _ENABLE_HYG_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_HYG)
    {
        #ifndef _ENABLE_SPECIAL_READ_HYG_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_HYG_

        do_data_correct_hyg(SENSOR_BUFFER_HYG);         

        HygValue = do_unit_correct_hyg(*((LPUINT16)(SENSOR_BUFFER_HYG)));
    }
    #endif //  _ENABLE_HYG_     

    #if _ENABLE_TEMP_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_TEMP)
    {
        #ifndef _ENABLE_SPECIAL_READ_TEMP_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_TEMP_

        do_data_correct_temp(SENSOR_BUFFER_TEMP);         

        TempValue = do_unit_correct_temp(*((LPUINT16)(SENSOR_BUFFER_TEMP)));
    }
    #endif //  _ENABLE_TEMP_     

    #if _ENABLE_PROX_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_PROX)
    {
        #ifndef _ENABLE_SPECIAL_READ_PROX_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_PROX_

        do_data_correct_prox(SENSOR_BUFFER_PROX);         
        ProxOutOfRangeFlag = *(SENSOR_BUFFER_PROX + 2);
        ProxValue = do_unit_correct_prox(*((LPUINT16)(SENSOR_BUFFER_PROX)));
    }
    #endif //  _ENABLE_PROX_     

    #if _ENABLE_UV_
    if(ONE_SHOT_MAIN_SENSOR_INDEX == SENSOR_I2C_IDX_UV)
    {
        #ifndef _ENABLE_SPECIAL_READ_UV_ // {
        i2c_drv_read_sram_data(gppI2CCmd[SENSOR_I2C_CMD_ALT]);
        #endif // } _ENABLE_SPECIAL_READ_UV_

        do_data_correct_uv(SENSOR_BUFFER_UV);         

        UVValue = do_unit_correct_uv(*((LPUINT16)(SENSOR_BUFFER_UV)));
    }
    #endif //  _ENABLE_UV_       

    pure_ec_sensor_one_shot_cmd_next_step();

    #ifdef EC_FEATURE_ONLY
    gucSensorBusStatus = 0;
    #endif

    SENSOR_I2C_CHANNEL_WDT = 0x00;

    SENSOR_FUSION_DBG_I2C_RTN(0x00); 
}

#endif  // }  DEF_SENSOR_ONE_SHOT_MODE
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION
//*****************************************************************************
//
//
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void SENSOR_CODE_L pure_ec_init_clear_fusion_variables(void)
{
    ONE_SHOT_CMD_WDT = 0x00;                                     
    ONE_SHOT_CMD_INDEX = 0x00;
    ONE_SHOT_MAIN_SENSOR_INDEX = 0x00;                          
    SENSOR_ISR_SERVICE_FLAG_OS = 0x00;
    SENSOR_ISR_SERVICE_FLAG = 0x00;
    SENSOR_I2C_CHANNEL_WDT = 0x00;
}

UINT8 CheckSensorHubStatus(void)
{
	return gu8HIDStatus;
}



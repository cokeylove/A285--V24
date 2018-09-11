#ifndef __SENSOR_FUSION_H__ //{
#define __SENSOR_FUSION_H__

#include "rom.h"
#include "hal.h"
#include "chip_type.h"
#include "chip_chipregs.h"
#include "debug_print.h"
#include "./sensor/sensor.h"
#include "mem_layout.h"
#include "../oem/Project_H/INCLUDE/oem_project.h"
#include "config.h"

// **************************************************************************
// Configuration
// **************************************************************************
#define __ENABLE_FUSION_API__   (_ENABLE_ACC_ || _ENABLE_MAG_ || _ENABLE_GYO_)

//	#define __PSEUDO_GYRO_DBG__
//	#define __SENSOR_DBG__    // print sensor raw data

/* Default value for fusion */
#define FUSION_FREQUENCY    (50) // Hz
#define FUSION_PERIOD       (1000/FUSION_FREQUENCY) // ms
#define FUSION_PERIOD_MAX   (500)
#define FUSION_PERIOD_LIMIT (30)

#ifndef STORED_DATA_LENGTH_MAG
#define STORED_DATA_LENGTH_MAG  56
#endif

#define SENSOR_PERIOD_CNT   12
#define SENSOR_FUSION_IDX  (SENSOR_PERIOD_CNT - 1)

/* For angle calculation */
#define ANGLE_FREQUENCY  10 // Hz
#define ANGLE_PERIOD    (1000/ANGLE_FREQUENCY) // ms
#define UNIT_DEGREE(x)  (((x)*2238)/10000)

#define COMPASS_FORMATION_THRESHOLD  30 //degree
#define COMPASS_FORMATION_DEBOUNCE   10

/* For control flow */
#define SENSOR_INIT_STEP_RESET_PARAMETER    0x00
#define SENSOR_INIT_STEP_INIT_ACC           0x01
#define SENSOR_INIT_STEP_INIT_MAG           0x02
#define SENSOR_INIT_STEP_INIT_GYO           0x03
#define SENSOR_INIT_STEP_INIT_ALS           0x04
#define SENSOR_INIT_STEP_INIT_ACC2          0x05
#define SENSOR_INIT_STEP_INIT_PRS           0x06
#define SENSOR_INIT_STEP_INIT_HYG           0x07
#define SENSOR_INIT_STEP_INIT_TEMP          0x08
#define SENSOR_INIT_STEP_INIT_PROX          0x09
#define SENSOR_INIT_STEP_INIT_UV            0x0A
#define SENSOR_INIT_STEP_INIT_ENABLE_IRQ    0x0B
#define SENSOR_INIT_STEP_INIT_READY         0x10
#define SENSOR_INIT_STEP_INIT_FAIL          0x11
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SENSOR_INIT_STEP_FAIL_CAUSE_ACC     0x01
#define SENSOR_INIT_STEP_FAIL_CAUSE_MAG     0x02
#define SENSOR_INIT_STEP_FAIL_CAUSE_GYO     0x03
#define SENSOR_INIT_STEP_FAIL_CAUSE_ALS     0x04
#define SENSOR_INIT_STEP_FAIL_CAUSE_ACC2    0x05
#define SENSOR_INIT_STEP_FAIL_CAUSE_PRS     0x06
#define SENSOR_INIT_STEP_FAIL_CAUSE_HYG     0x07
#define SENSOR_INIT_STEP_FAIL_CAUSE_TEMP    0x08
#define SENSOR_INIT_STEP_FAIL_CAUSE_PROX    0x09
#define SENSOR_INIT_STEP_FAIL_CAUSE_UV      0x0A
#define SENSOR_INIT_STEP_FAIL_CAUSE_NO      0x10
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ONE_SHOT_CMD_INDEX_READ_ACC         0x00
#define ONE_SHOT_CMD_INDEX_READ_MAG         0x01
#define ONE_SHOT_CMD_INDEX_READ_GYO         0x02
#define ONE_SHOT_CMD_INDEX_READ_ALS         0x03
#define ONE_SHOT_CMD_INDEX_READ_ACC2        0x04
#define ONE_SHOT_CMD_INDEX_READ_PRS         0x05
#define ONE_SHOT_CMD_INDEX_READ_HYG         0x06
#define ONE_SHOT_CMD_INDEX_READ_TEMP        0x07
#define ONE_SHOT_CMD_INDEX_READ_PROX        0x08
#define ONE_SHOT_CMD_INDEX_READ_UV          0x09
#define ONE_SHOT_CMD_INDEX_FUSION           0x0A

#define SENSOR_DEVICE_NOT_ALL_READY         0x00
#define SENSOR_DEVICE_ALL_READY             0x01

#define SENSOR_DEVICE_INIT_RETRY_TIME       20

/* For error handle */
#define MAX_SENSOR_TIMEOUT_CNT  10
#define MAX_I2C_RESET_CNT       3

// **************************************************************************
// Macro define
// **************************************************************************
#define _delay(i)   do{volatile int j = (i << 0x4); while(j--);}while(0)

#if 0
#define SENSOR_WRITE_WITH_CHECK(sa, addr, param, val)       \
    /*printk("Init: %02Xh\r\n", addr);*/                    \
    do{                                                     \
        param = val;                                        \
        sensor_write(sa, addr, (UINT8 *)(&param), 1);       \
        _delay(0x10);                                       \
        param ^= 0xFF;                                      \
        sensor_read(sa, addr, (UINT8 *)(&param), 1);        \
        _delay(0x10);                                       \
        /*printk("%02Xh -> %02Xh\r\n", val, param);*/       \
    }while(param != val)

#define SENSOR_READ_WITH_CHECK(sa, addr, param, val)        \
    do{                                                     \
        param = val^0xFF;                                   \
        sensor_read(sa, addr, (UINT8 *)(&param), 1);        \
        _delay(0x10);                                       \
        /*printk("%02Xh -> %02Xh\r\n", val, param);*/       \
    }while(param != val)
#endif

// **************************************************************************
// Structure define
// **************************************************************************
/* Callback function for fusion calculation */
typedef void (*SENSOR_CALLBACK_FUNC)(UINT8 *pDataBuffer, void *pParam);

#pragma pack(1)
/* Fusion algorithm configuration */
typedef struct _T_FUSION_CONFIG_
{
    FIELD Enable_Android: 1;
    FIELD Enable_6D: 1;
    FIELD Reserved: 6;
}T_FUSION_CONFIG, *P_FUSION_CONFIG;

/* Sensor initial done flag */
typedef struct _T_SENSOR_INIT_DONE_
{
    FIELD Acc : 1;
    FIELD Mag : 1;
    FIELD Gyo : 1;
    FIELD Als : 1;
    FIELD Acc2: 1;
    FIELD Prs : 1;
    FIELD Hyg : 1;
    FIELD Temp: 1;
    FIELD Prox: 1;
    FIELD UV  : 1;
    FIELD Reserved: 6;
}T_SENSOR_INIT_DONE, *P_SENSOR_INIT_DONE;

/* Sensor power configuration */
typedef struct _T_SENSOR_POWER_CONFIG_
{
    FIELD Acc : 1;
    FIELD Mag : 1;
    FIELD Gyo : 1;
    FIELD Als : 1;
    FIELD Acc2: 1;
    FIELD Prs : 1;
    FIELD Hyg : 1;
    FIELD Temp: 1;
    FIELD Prox: 1;
    FIELD UV  : 1;
    FIELD Reserved: 6;
}T_SENSOR_POWER_CONFIG, *P_SENSOR_POWER_CONFIG;
#pragma pack()

/* Sensor buffer for windows */
typedef struct _T_SENSOR_BUFFER_WIN_
{
    /* Fusion (in) */
    UINT16 Acc[3];      //00~05   [x, y, z]
    UINT16 Gyo[3];      //06~11   [x, y, z]
    UINT16 Mag[3];      //12~17   [x, y, z]
    UINT8  MagTmp[4];   //18~21

    /* Fusion (out) */
    UINT16 MagCal[3];   //22~27   [x, y, z]    
    UINT16 Eular[3];    //28~33   [pitch, roll, yaw]
    UINT16 Quat[4];     //34~41   [w, x, y, z]
    UINT16 Shake;       //42~43
    UINT16 Heading;     //44~45
    UINT16 RMatrix[9];  //46~63   [0,0][1,0][2,0][0,1][1,1][2,1][0,2][1,2][2,2]

    /* Alternative */
    UINT8 Alt[16];      //64~79
}T_SENSOR_BUFFER_WIN, *P_SENSOR_BUFFER_WIN;

/* Sensor buffer for android */
typedef struct _T_SENSOR_BUFFER_ANDROID_
{
    /* Fusion (in) */
    UINT16 Acc[3];      //00~05     [x, y, z]
    UINT16 Gyo[3];      //06~11     [x, y, z]
    UINT16 Mag[3];      //12~17     [x, y, z]
    UINT8  MagTmp[4];   //18~21

    /* Fusion (out) */
    UINT16 MagCal[3];   //22~27     [x, y, z]    
    UINT16 Eular[3];    //28~33     [pitch, roll, yaw]
    UINT16 Quat[4];     //34~41     [w, x, y, z]
    UINT16 AccMeter[3]; //42~47     [x, y, z]
    UINT16 LinAcc[3];   //48~53     [x, y, z]
    UINT8  Reserved[10];

    /* Alternative */
    UINT8 Alt[16];      //64~79
}T_SENSOR_BUFFER_ANDROID, *P_SENSOR_BUFFER_ANDROID;

/* Check driver data length */
#ifndef _ENABLE_MULTI_ACC_ // {
#if (DATA_LENGTH_ACC > 6)
#error "DATA_LENGTH_ACC error."
#endif
#endif // } _ENABLE_MULTI_ACC_

#if (DATA_LENGTH_MAG > 10) 
#error "DATA_LENGTH_MAG error."
#endif

#if (DATA_LENGTH_GYO > 6)
#error "DATA_LENGTH_GYO error."
#endif

#if (DATA_LENGTH_ALS > 16)
#error "DATA_LENGTH_ALS error."
#endif

#ifndef _ENABLE_MULTI_ACC2_ // {
#if (DATA_LENGTH_ACC2 > 16)
#error "DATA_LENGTH_ACC2 error."
#endif
#endif // } _ENABLE_MULTI_ACC2_

#if (DATA_LENGTH_PRS > 16)
#error "DATA_LENGTH_PRS error."
#endif

#if (DATA_LENGTH_HYG > 16)
#error "DATA_LENGTH_HYG error."
#endif

#if (DATA_LENGTH_TEMP > 16)
#error "DATA_LENGTH_TEMP error."
#endif

#if (DATA_LENGTH_PROX > 16)
#error "DATA_LENGTH_PROX error."
#endif

#if (DATA_LENGTH_UV > 16)
#error "DATA_LENGTH_UV error."
#endif

/* I2C sensor index */
enum _I2C_SENSOR_IDX_
{
    #if _ENABLE_ACC_
    SENSOR_I2C_IDX_ACC,
    #endif

    #if _ENABLE_MAG_
    SENSOR_I2C_IDX_MAG,
    #endif

    #if _ENABLE_GYO_
    SENSOR_I2C_IDX_GYO,
    #endif

    #if _ENABLE_ALS_
    SENSOR_I2C_IDX_ALS,
    #endif

    #if _ENABLE_ACC2_
    SENSOR_I2C_IDX_ACC2,
    #endif

    #if _ENABLE_PRS_
    SENSOR_I2C_IDX_PRS,
    #endif

    #if _ENABLE_HYG_
    SENSOR_I2C_IDX_HYG,
    #endif

    #if _ENABLE_TEMP_
    SENSOR_I2C_IDX_TEMP,
    #endif

    #if _ENABLE_PROX_
    SENSOR_I2C_IDX_PROX,
    #endif

    #if _ENABLE_UV_
    SENSOR_I2C_IDX_UV,
    #endif

    SENSOR_I2C_IDX_TOTAL
};

/* Sensor stored data (2 bytes alignment) */
#pragma pack(2)
typedef struct _T_SENSOR_STORED_DATA_
{
    /* Flag */
    UINT16 u16ValidFlag;
    UINT16 u16TotalSize;

    /* Basic */
    UINT16 u16AlsMultiplier;
    UINT16 u16AlsDivisor;

    UINT8  pAxisMode[3];
    UINT8  u8AccCalPhaseMode;

    SINT16 pAccBiasBuffer[3];
    SINT16 pGyroBiasBuffer[3];

    UINT8  pMagBuffer[STORED_DATA_LENGTH_MAG];

    /* Extend */
    #if _ENABLE_ACC2_ // {
    UINT8  u8Acc2AxisMode;
    UINT8  u8Acc2CalPhaseMode;    
    SINT16 pAcc2BiasBuffer[3];

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC2_ // {
    SINT16 pAcc2GainBuffer[3];
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC2_
    
    #endif // } _ENABLE_ACC2_

    #ifdef _ENABLE_6_AXIS_CALIBRATION_ACC_ // {
    SINT16 pAccGainBuffer[3];
    #endif // } _ENABLE_6_AXIS_CALIBRATION_ACC_    

    #ifdef __ENABLE_ALS_COLOR_VALUE__ // {
    UINT8  pColorBuffer[STORED_DATA_LENGTH_COLOR];
    #endif // } __ENABLE_ALS_COLOR_VALUE__

    /* Info. */
    struct 
    {
        FIELD EnableAcc2: 1;
        FIELD Enable6AxisCalAcc2: 1;
        FIELD Enable6AxisCalAcc: 1;
        FIELD EnableAlsColor: 1;
        FIELD Reserved0: 4;
        UINT8 Reserved1;
    }stStoredType;

    UINT16 u16Version;
}T_SENSOR_STORED_DATA, *P_SENSOR_STORED_DATA;
#pragma pack()

/* Stored data length (128 byte alignment) */
#define SENSOR_STORED_DATA_SIZE   \
    (((sizeof(T_SENSOR_STORED_DATA)) + 127) & ~(127))

// **************************************************************************
// Debug Message Rule define
// **************************************************************************
/* Sensor */
enum _SFD_IDX_
{
    SFD_IDX_TASK_FLAG = 0,
    SFD_IDX_STATUS,
    SFD_IDX_TOTAL_SENSOR,
    SFD_IDX_INIT_STATUS,
    SFD_IDX_I2C_0,
    SFD_IDX_I2C_1,
    SFD_IDX_I2C_2,
    SFD_IDX_POWER_STATUS,
    SFD_IDX_MAX
};

enum _SENSOR_DBG_ID_
{
    SENSOR_DBG_ID_ACC = 0,
    SENSOR_DBG_ID_MAG,
    SENSOR_DBG_ID_GYO,
    SENSOR_DBG_ID_ALS,
    SENSOR_DBG_ID_ACC2,
    SENSOR_DBG_ID_PRS,
    SENSOR_DBG_ID_HYG,
    SENSOR_DBG_ID_TEMP,
    SENSOR_DBG_ID_PROX,
    SENSOR_DBG_ID_UV
};

#define SENSOR_FUSION_DBG_SET_FLAG()       \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_TASK_FLAG])++;}while(0)
#define SENSOR_FUSION_DBG_SET_STATUS(_s)   \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_STATUS]) = (_s);}while(0)
#define SENSOR_FUSION_DBG_SET_CNT(_c)      \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_TOTAL_SENSOR]) = (_c);}while(0)
#define SENSOR_FUSION_DBG_SET_INIT(_idx)   \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_INIT_STATUS]) |= (1 << (_idx));}while(0)
#define SENSOR_FUSION_DBG_SET_POWER(_p)    \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_POWER_STATUS]) = (_p);}while(0)
#define SENSOR_FUSION_DBG_ID(_idx, _id)    \
    do{gpSHDInfo->pSSlvAddr[(_idx)] = (_id);}while(0)
#define SENSOR_FUSION_DBG(_X_)             \
    do{DEBUG_PORT(_X_); SENSOR_FUSION_DBG_SET_STATUS((_X_));}while(0)
#define SENSOR_FUSION_DBG_SET()            \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_STATUS])++;}while(0)
#define SENSOR_FUSION_DBG_I2C_RTN(_ret)    \
    do{(gpSHDInfo->pSDbgMsg[SFD_IDX_I2C_2]) = (_ret);}while(0)
#define SENSOR_FUSION_DBG_I2C(_c0,_c1,_c2)            \
    do{                                               \
        (gpSHDInfo->pSDbgMsg[SFD_IDX_I2C_0]) = (_c0); \
        (gpSHDInfo->pSDbgMsg[SFD_IDX_I2C_1]) = (_c1); \
        (gpSHDInfo->pSDbgMsg[SFD_IDX_I2C_2]) = (_c2); \
    }while(0)
    
#define SENSOR_FUSION_DBG_CLEAR()   \
    do{                             \
        memset((UINT8*)(gpSHDInfo->pSSlvAddr), 0x00, I2C_SENSOR_MAX_NUM);   \
        memset((UINT8*)(gpSHDInfo->pSDbgMsg), 0x00, SFD_IDX_POWER_STATUS);  \
    }while(0)

/* Mutex */
#define SENSOR_MUTEX_WAIT    (gpSHDInfo->u8DbgRTOS[0x06])
#define SENSOR_MUTEX_CUR     (gpSHDInfo->u8DbgRTOS[0x07])

enum _SENSOR_MUTEX_ID_
{
    MUTEX_ID_POWER_SWITCH = 1,
    MUTEX_ID_RATE_SWITCH,
    MUTEX_ID_POWER_OFF,
    MUTEX_ID_MAIN,
    MUTEX_ID_STOP,
    MUTEX_ID_RESTART,
    MUTEX_ID_HID_HCC,
    MUTEX_ID_INIT_SENSOR,
    MUTEX_ID_HCC,
    MUTEX_ID_TIMEOUT,
};

/* Mag. Lib. */
#define MAG_LIB_VER (gpSHDInfo->u8DbgRTOS[0x0C])

#if defined(__ENABLE_AKM_LIB__) // {
// AK8963
#define SENSOR_FUSION_DBG_SET_MAG_ALG()   do{MAG_LIB_VER = 1;}while(0)
#elif defined(__ENABLE_AKM2_LIB__)
// AK09911
#define SENSOR_FUSION_DBG_SET_MAG_ALG()   do{MAG_LIB_VER = 2;}while(0)
#elif defined(__ENABLE_AKM_PG_LIB__)
// AK09911 Pseudo Gyro.
#define SENSOR_FUSION_DBG_SET_MAG_ALG()   do{MAG_LIB_VER = 3;}while(0)
#elif defined(__ENABLE_MAGNA_LIB__)
// MXG1300
#define SENSOR_FUSION_DBG_SET_MAG_ALG()   do{MAG_LIB_VER = 4;}while(0)
#else
// ITE
#define SENSOR_FUSION_DBG_SET_MAG_ALG()   do{MAG_LIB_VER = 0;}while(0)
#endif // }

#define SENSOR_FUSION_DBG_ERR(...)         printk(__VA_ARGS__)
#define SENSOR_FUSION_DBG_INFO(...)        printk(__VA_ARGS__)
#define SENSOR_FUSION_DBG_DUMP(...)        DLDumpMem(__VA_ARGS__)

//************************************
// I2C debug Info
//************************************
//   I2C_DEBUG_PORT3:
//   0x01: Enter sensor_isr
//   0x02: Exit sensor_i2r

//   I2C_DEBUG_PORT4:
//   0x01: HW Timeout in ISR, do SW reset, and re-initial sensor. (Error case)
//   0x02: Do one shot command, read sensor data done. (Normal case)
//   0x03: I2C FIFO mode R/W , timeout happens. (Error case)
//   0x04: Do one shot command, susspend fusion task, 
//         wait stop condition.(Error case)


// (I2C_DEBUG_PORT5 |  (I2C_DEBUG_PORT6 << 8))
//  Sensor status(0: connected, 1: disconnected):
//  BIT n:  _I2C_SENSOR_IDX_

//  I2C_DEBUG_PORT7:
//  0x01: ACC initial fail
//  0x02: MAG initial fail
//  0x03: GYO initial fail
//  0x04: ALS initial fail
//  0x05: ACC2 initial fail
//  0x06: PRS initial fail



#define I2C_DEBUG_PORT3(_X_)         (gpSHDInfo->pI2CDbgMsg[2] = (_X_))
#define I2C_DEBUG_PORT4(_X_)         (gpSHDInfo->pI2CDbgMsg[3] = (_X_))
#define I2C_DEBUG_PORT5(_X_)         (gpSHDInfo->pI2CDbgMsg[4] = (_X_))
#define I2C_DEBUG_PORT6(_X_)         (gpSHDInfo->pI2CDbgMsg[5] = (_X_))
#define I2C_DEBUG_PORT7(_X_)         (gpSHDInfo->pI2CDbgMsg[6] = (_X_))


// **************************************************************************
// extern variable & function
// **************************************************************************
#define gu16SensorPeriod             ((UINT16*)(gpSHDInfo->u16SensorPeriod))

extern T_SENSOR_STORED_DATA gstSensorStoredData;
extern T_FUSION_CONFIG gstFConfig;
extern volatile UINT8 gucI2CSensorErrStatus ;

extern void sensor_log(UINT8 u8MsgCode);
extern void sensor_log32(UINT32 u32MsgCode);

extern int sensor_get_freq_idx(const UINT16 *pFreqBuffer, UINT16 u16TargetFreq);
extern void sensor_sleep(int nMilliSec);
extern void sensor_delay_ms(UINT32 u32MilliSec);
extern UINT32 sensor_get_ticks(void);
extern UINT16 sensor_check(UINT8 u8SlaveAddr, 
                           UINT8 *pTotalSensor, UINT8 pSensorAddr[10]);
extern int sensor_read(UINT8 u8SlaveAddr, UINT8 u8Addr, 
                       UINT8 *pBuf, UINT8 u8Cnt);
extern int sensor_write(UINT8 u8SlaveAddr, UINT8 u8Addr, 
                        UINT8 *pBuf, UINT8 u8Cnt);
extern int sensor_block_write(UINT8 u8SlaveAddr, UINT8 *pBuf, UINT8 u8Cnt);
extern int sensor_block_read(UINT8 u8SlaveAddr, UINT8 *pBufIn, UINT8 u8CntIn, 
                             UINT8 *pBufOut, UINT8 u8CntOut);
extern UINT8 SENSOR_WRITE_WITH_CHECK(UINT8 sa,UINT8 addr,
                                     UINT8 param, UINT8 val);
extern void SENSOR_READ_WITH_CHECK(UINT8 sa,UINT8 addr,UINT8 param, UINT8 val);
extern void sensor_get_data_start(void);
extern void sensor_get_data_stop(void);
extern void sensor_fusion_calc(SENSOR_CALLBACK_FUNC pfnCallback, void *pParam);
extern void sensor_power_mode_switch(T_SENSOR_POWER_CONFIG stPower);
extern void sensor_data_rate_switch(UINT8 u8SensorID, UINT16 u16MilliSec);

#ifdef __ENABLE_HID_PM_DIPO__ // { __ENABLE_HID_PM_DIPO__
extern void sensor_work_mode_switch(UINT8 u8Mode);
#endif // } __ENABLE_HID_PM_DIPO__

extern void sensor_stored_data_save(void);
extern void sensor_stored_data_load(void);
extern void sensor_power_off_data_save(void);
extern void sensor_update_acc_bias(UINT8 calibration_phase_mode);
extern void sensor_update_acc2_bias(UINT8 calibration_phase_mode);
extern UINT8 sensor_update_gyro_bias(void);
extern UINT8 sensor_error_status_check(UINT8 u8SensorIdx);
extern UINT16 sensor_get_acc2_info(UINT16 pAcc2[3], SINT16 pAngle[3]);

extern void diagnostics_sensor_test(void);
extern void send_packet(UINT8 *pData, void *pParam);

// ITE fusion library
#if __ENABLE_FUSION_API__ // {
extern UINT32 fusion_init(P_FUSION_CONFIG pFConfig, 
                          SINT16 pAccBiasBuffer[3], 
                          SINT16 pGyroBiasBuffer[3], 
                          UINT8 pAxisMode[3]);
extern void fusion_calc(UINT8 frame_data[], UINT16 freq);

extern void fusion_axis_switch(LPSINT32 rdata, UINT8 axis_mode);


extern void fusion_update_acc_bias(UINT8 *pRawData, 
                                   UINT8 CalibrationPhaseMode);
extern void fusion_update_acc2_bias(UINT8 *pRawData, 
                                    UINT8 CalibrationPhaseMode);

extern BOOL fusion_update_gyro_bias(UINT8 *pRawData);

extern void fusion_set_op_mode(UINT8 u8Mode);

extern int compass_calibrate(SINT32 pDataBuffer[3], SINT16 pBiasBuffer[3], 
                             UINT8 *pMagInfrFlag, UINT16 u16Freq);
extern void get_compass_calibrate_info(UINT8 *pState, UINT8 *pMagInfrFlag);
extern void init_compass_calibrate_data(UINT8 *pBuf);
extern void reset_compass_calibrate_data(UINT8 u8Formation);
extern UINT8 set_compass_calibrate_formation(UINT8 u8Formation);

extern void init_custom_angle_param(UINT8 u8AxisMode, SINT16 pBiasBuffer[3]);
extern int get_custom_angle(UINT8 pInAcc1[6], UINT8 pInAcc2[6], 
                            SINT16 pOutAngle[3], UINT16 pOutAccData[3], 
                            UINT16 u16Freq);

extern void init_acc_6axis_cal_param(SINT16 pGainBuffer[3], 
                                     UINT8 pCalBuffer[24]);
extern void init_acc2_6axis_cal_param(SINT16 pGainBuffer[3], 
                                      UINT8 pCalBuffer[24]);

#else
#define fusion_init(a,b,c,d)    (0xFFFFFFFF)
#define fusion_calc(a,b)
#endif // }  __ENABLE_FUSION_API__
    
// sensor fusion main api
extern void sensor_isr(void);
extern void sensor_init(void);
extern void fusion_main(void);
extern UINT8 sensor_device_ready(void);
extern UINT8 sensor_device_retry(void);
extern void sensor_one_shot_main(void);
extern void sensor_wait_done_and_stop(void);
extern void sensor_restart(void);
extern void sensor_do_hcc_service(void);

#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION // {
extern void pure_ec_sensor_fusion_calc(SENSOR_CALLBACK_FUNC pfnCallback, 
                                       void *pParam);
extern void pure_ec_sensor_one_shot_main(void);
extern void pure_ec_sensor_one_shot_isr(void);
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION
extern void pure_ec_init_clear_fusion_variables(void);

extern void RamDebug(unsigned char dbgcode);
extern void hook_sensor_init_start(void);		// alex + for debug
extern void hook_sensor_init_end(void);			// alex + for debug


#endif //} __SENSOR_FUSION_H__

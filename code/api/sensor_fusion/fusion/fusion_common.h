/*******************************************************************************
* Copyright (C), 2000-2013, ITE Tech. Inc.
* FileName: fusion_common.h
* Author  : Shawn Wang (ite01347)
* Version : 
* Date    : 2013-7-10
* Brief   : 
*******************************************************************************/
#ifndef _FUSION_COMMON_H_
#define _FUSION_COMMON_H_ // {

#include <stdlib.h>
#include <string.h>

#include "chip_type.h"
#include "debug_print.h"
#include "mem_layout.h"
#include "sensor_fusion.h"
#include "fusion.h"

#if __ENABLE_FUSION_API__ // {
#define DELTA_T     (300/FUSION_FREQUENCY)

#define PI          804
#define FIX_90      402
#define FIX_180     804
#define FIX_270     1206
#define FIX_360     1607

// Sensor Multi-rate
#define  ACC_MAG_EN     1
#define  GYRO_EN        1

// Linear Acceleration Thr
#define	 LINR_ACC_THR_U	320
#define	 LINR_ACC_THR_D	180
#define  LINR_ALPHA		204 //0.8

// Shake Detection
#define	 SHAKE_EN_THR_S	   470
#define	 SHAKE_EN_THR_8    1000
#define	 SHAKE_EN_THR_16   2500
#define  SHAKE_MODEL_TAP   16
#define  __SHAKE_FUNCTION_SIMPLE__

// Data Change Percentage
#define	 CHANGE_PERCENTAGE  5
#define	 TAP_DET_THR  3

// Rotation Matrix Setting
#define __YXZ_ORDER__

// Check Inv Fail Or Not (Compass Calibration)
#define	CHECK_INV_MAX_BOUND			272
#define	CHECK_INV_MIN_BOUND 		240

// Two-Acc Angle Detection
#if 1 // {
#define X_VERTICAL_VALUE			180 // 0.7*256
#define TWOACC_LENGTH_MAX			87000 // ~(1.15*256)^2
#define TWOACC_LENGTH_MIN			47000 // ~(0.85*256)^2
#else
#define X_VERTICAL_VALUE			230 // 0.9*256
#define TWOACC_LENGTH_MAX			147456 // ~(1.5*256)^2
#define TWOACC_LENGTH_MIN			16384  // ~(0.5*256)^2
#endif // } 0

// **************************************************************************
// Structure define
// **************************************************************************
// Sensor raw data structure define
typedef struct          // 17x2 UINT8s
{
	SINT32                s32Sensor_rawdata0;                  // 0000 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata1;                  // 0002 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata2;                  // 0004 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata3;                  // 0006 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata4;                  // 0008 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata5;                  // 000A : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata6;                  // 000C : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata7;                  // 000E : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata8;                  // 0010 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata9;                  // 0012 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata10;                 // 0014 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata11;                 // 0016 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata12;                 // 0018 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata13;                 // 001A : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata14;                 // 001C : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata15;                 // 001E : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata16;                 // 0020 : 0002 : sensor raw data
} SENSOR_RAW_DATA_17;

typedef struct          // 5x2 UINT8s
{
	SINT32                s32Sensor_rawdata0;                  // 0000 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata1;                  // 0002 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata2;                  // 0004 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata3;                  // 0006 : 0002 : sensor raw data
	SINT32                s32Sensor_rawdata4;                  // 0008 : 0002 : sensor raw data
} SENSOR_RAW_DATA_5;

// **************************************************************************
#define CMPCAL_WHOLE_NUM        (8)
#define CMPCAL_PART_NUM         (8)
#define CMPCAL_SKIP_NUM         (15)

typedef struct _TCOMPASS_CAL_COMB_INFO_
{
	SINT32  cal_offset[6];

	SINT32  mag_x_whole[CMPCAL_WHOLE_NUM];
	SINT32  mag_y_whole[CMPCAL_WHOLE_NUM];
	SINT32  mag_z_whole[CMPCAL_WHOLE_NUM];

	SINT32  mag_x_part[CMPCAL_PART_NUM];
	SINT32  mag_y_part[CMPCAL_PART_NUM];
	SINT32  mag_z_part[CMPCAL_PART_NUM];

} TCOMPASS_CAL_COMB_INFO;
typedef TCOMPASS_CAL_COMB_INFO      TCmpCalInf;
typedef TCmpCalInf*                 LPCmpCalInf;

typedef struct _TSENSOR_AXIS_VECTOR_
{
    SINT32 x;
    SINT32 y;
    SINT32 z;
} TSENSOR_AXIS_VECTOR;
typedef TSENSOR_AXIS_VECTOR      TSenAxisVec;
typedef TSenAxisVec*             LPSenAxisVec;

typedef struct _TACC_CAL_6AXIS_INFO_
{
	SINT16 	sum[3];
	SINT16	max_x;
	SINT16	min_x;
	SINT16	max_y;
	SINT16	min_y;
	SINT16	max_z;
	SINT16	min_z;
    SINT16 	sum_2; // for 2-axis mode
    UINT8   cnt[3];
} TACC_CAL_6AXIS_INFO;
typedef TACC_CAL_6AXIS_INFO      	TAccCal6AxisInf;
typedef TAccCal6AxisInf*  			LPAccCal6AxisInf;

typedef struct _T_FUSION_COMMON_FLAG_
{
    unsigned char _mag_op_mode: 1;        // Mag. calibration operating mode, 0: normal, 1: always do rt_calibrate
    unsigned char _compass_cal_flag: 1;   // Compass Calibration Flag
    unsigned char _pad_NB_mode_flag: 1;   // Pad<->NB Flag
    unsigned char _rdata_chag_flag: 1;    // Raw data Change Flag
    unsigned char _gyro_cal_done_flag: 1; // Gyro. calibration done flag
    unsigned char _reserved: 3;
}T_FUSION_COMMON_FLAG, *P_FUSION_COMMON_FLAG;

typedef void (*QUATERN_TO_ANGLE_FUNC)(LPSINT32 euler_angle_post, LPSINT32 euler_angle_pre, LPSINT32 quat_vec);
typedef void (*ANGLE_TO_MATRIX_FUNC)(LPSINT32 rotate_matrix, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
typedef void (*ANGLE_TO_QUATERN_FUNC)(LPSINT32 quat_vec, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
typedef void (*MAG_HORIZONTAL_FUNC)(LPSINT32 mag_cal_x, LPSINT32 mag_cal_y, LPSINT32 mag_data, LPSINT32 roll_ang, LPSINT32 pitch_ang);
typedef void (*ROLL_PITCH_FUNC)(LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 data_in, LPSINT32 roll_ang_crct, LPSINT32 pitch_ang_crct);

/*-------------------------------------------------------------------------
    common variable
-------------------------------------------------------------------------*/
extern const SINT16 sin_lut[];

extern SENSOR_RAW_DATA_17 acc_x_rdata;
extern SENSOR_RAW_DATA_17 acc_y_rdata;
extern SENSOR_RAW_DATA_17 acc_z_rdata;
extern SENSOR_RAW_DATA_17 mag_x_rdata;
extern SENSOR_RAW_DATA_17 mag_y_rdata;
extern SENSOR_RAW_DATA_17 mag_z_rdata;
extern SENSOR_RAW_DATA_17 gyo_x_rdata;
extern SENSOR_RAW_DATA_17 gyo_y_rdata;
extern SENSOR_RAW_DATA_17 gyo_z_rdata;

extern UINT8 *p_axis_mode;
#define acc_axis_mode   p_axis_mode[0]
#define gyro_axis_mode  p_axis_mode[1]
#define mag_axis_mode   p_axis_mode[2]

extern T_FUSION_COMMON_FLAG gtFusComFlag;
#define mag_op_mode        (gtFusComFlag._mag_op_mode)
#define compass_cal_flag   (gtFusComFlag._compass_cal_flag)
#define pad_NB_mode_flag   (gtFusComFlag._pad_NB_mode_flag)
#define rdata_chag_flag    (gtFusComFlag._rdata_chag_flag)
#define gyro_cal_done_flag (gtFusComFlag._gyro_cal_done_flag)

extern SINT32 ardata_chag_value[3];
extern SINT32 mrdata_chag_value[3];

extern SINT32 acc_data[3];
extern SINT32 mag_data[3];
extern SINT32 gyro_data[3];

extern SINT32 acc_raw_data[3];

extern SINT16 *acc_gain;

extern SINT16 acc_raw_x_vec_pre[16];
extern SINT16 acc_raw_y_vec_pre[16];
extern SINT16 acc_raw_z_vec_pre[16];

extern const SINT16 shake_model_up_down[16];
extern const SINT16 shake_model_down_up[16];

extern SINT16 acc_raw_x_vec_post[16];
extern SINT16 acc_raw_y_vec_post[16];
extern SINT16 acc_raw_z_vec_post[16];

extern TSenFusParam mSenFuParam;

extern QUATERN_TO_ANGLE_FUNC quatern2angle;
extern ANGLE_TO_MATRIX_FUNC angle2matrix;
extern ANGLE_TO_QUATERN_FUNC angle2quatern;
extern MAG_HORIZONTAL_FUNC mag_horizontal_func;
extern ROLL_PITCH_FUNC roll_pitch_func;

/*-------------------------------------------------------------------------
    common function
-------------------------------------------------------------------------*/
SINT32 inv_func(LPSINT32 inv_matrix_out, LPSINT32 matrix_in);
SINT32 check_inv_func(LPSINT32 matrix_inv, LPSINT32 matrix);
SINT32 det_3x3_func(LPSINT32 matrix_in);
SINT32 mag_norm_length(LPSINT32 mag_data);
void compass_cal_portion(LPSINT32 cal_offset_post, LPSINT32 mag_x_matrix, LPSINT32 mag_y_matrix, LPSINT32 mag_z_matrix, LPSINT32 cal_offset_pre);
void compass_cal_whole(LPSINT32 cal_offset_post, LPSINT32 mag_x_matrix, LPSINT32 mag_y_matrix, LPSINT32 mag_z_matrix, LPSINT32 cal_offset_pre);
void compass_cal_comb(TCmpCalInf* pMag_pre, TCmpCalInf* pMag_post, LPSINT32 mag_raw_data);
void soft_iron_calibration(LPSINT32 mag_raw_data);
void compass_infr_detect(LPSINT32 mag_rdata, LPSINT16 mag_offset, LPUINT8 mag_infr_flag);

SINT32 fir_filter(UINT8 fir_tap, UINT8 st_pt, LPSINT16 ptrCoeff, LPUINT32 ptrData);
UINT8 rdata_chag_prtg(LPSINT32 rdata_chag_value, LPSINT32 rdata_in);
int two_acc_angle(LPSINT32 acc_in1, LPSINT32 acc_in2, SINT16 angle[3], UINT16 u16Freq);
int acc_roll_pitch_6d(LPSINT32 acc_roll_ang_post, LPSINT32 acc_pitch_ang_post, LPSINT32 acc_data, LPSINT32 acc_roll_ang_pre, LPSINT32 acc_pitch_ang_pre);
void mag_yaw_6d(LPSINT32 yaw_change_en, LPSINT32 mag_yaw_ang_post, LPSINT32 mag_data, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 mag_yaw_ang_pre);
int compass_heading(LPSINT32 rmatrix);
void kg_value_set(LPSINT16 kalman_matrix, LPSINT16 kalman_value);
void quatmatrix(LPSINT16 quatern_matrix, LPSINT32 quat_vec);
void qua2ang_yxz(LPSINT32 euler_angle_post, LPSINT32 euler_angle_pre, LPSINT32 quat_vec);
void qua2ang_xyz(LPSINT32 euler_angle_post, LPSINT32 euler_angle_pre, LPSINT32 quat_vec);
void quatern2matrix(LPSINT32 rota_matrix, LPSINT32 quat_vec);
void ang2mat_yxz(LPSINT32 rotate_matrix, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
void ang2mat_xyz(LPSINT32 rotate_matrix, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
void ang2qua_yxz(LPSINT32 quat_vec, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
void ang2qua_xyz(LPSINT32 quat_vec, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang);
void matrix2quatern(LPSINT32 quat_vec, LPSINT32 rota_matrix);
void norm_func(LPSINT32 norm_data, LPSINT32 norm_value, LPSINT32 quat_data_en);
#ifdef __ENABLE_CODE__ //{
void sqrt_func1(char* File, int Line, LPSINT32 out, LPSINT32 in);
#define sqrt_func(x1, x2)   sqrt_func1(__FILE__, __LINE__, x1, x2)
#else
void sqrt_func(LPSINT32 out, LPSINT32 in);
#endif //} __ENABLE_CODE__
void mag_horiz_yxz(LPSINT32 mag_cal_x, LPSINT32 mag_cal_y, LPSINT32 mag_data, LPSINT32 roll_ang, LPSINT32 pitch_ang);
void mag_horiz_xyz(LPSINT32 mag_cal_x, LPSINT32 mag_cal_y, LPSINT32 mag_data, LPSINT32 roll_ang, LPSINT32 pitch_ang);
void yaw_func(LPSINT32 yaw_change_en, LPSINT32 yaw_ang, LPSINT32 data_in, SINT32 flag_9d, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang_cs);
void roll_pitch_yxz(LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 data_in, LPSINT32 roll_ang_crct, LPSINT32 pitch_ang_crct);
void roll_pitch_xyz(LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 data_in, LPSINT32 roll_ang_crct, LPSINT32 pitch_ang_crct);
void sin_cos_value(LPSINT32 sin_value, LPSINT32 cos_value, LPSINT32 angle);
void tan_cordic(LPSINT32 ang_rad, LPSINT32 atan_numr, LPSINT32 atan_deno);
int sign_func(LPSINT32 value);
void rshift_func(LPSINT32 out, LPSINT32 in, SINT32 shift_bit);
void quatern_equal(LPSINT32 out, LPSINT32 in);

#ifdef __SHAKE_FUNCTION_SIMPLE__ // {
void shake_detect_simple(LPSINT32 acc_raw_data, LPUINT8 shake_status);
#else
void shake_detect(
            // Input Data
            LPSINT32 acc_raw_data, LPSINT16 acc_raw_x_vec_pre, LPSINT16 acc_raw_y_vec_pre, LPSINT16 acc_raw_z_vec_pre,

            // Output Data
            LPSINT16 acc_raw_x_vec_post, LPSINT16 acc_raw_y_vec_post, LPSINT16 acc_raw_z_vec_post, LPUINT8 shake_status);
#endif // }  __SHAKE_FUNCTION_SIMPLE__

void acc_calibrate(SINT32 *pData, SINT16 *pBias, SINT16 *pGain);

void fusion_data_copy(LPUINT16 pBufOut, LPSINT32 pBufIn, UINT8 u8CntOfBufIn);
void fusion_axis_switch(LPSINT32 rdata, UINT8 axis_mode);

#endif // } __ENABLE_FUSION_API__
#endif // } _FUSION_COMMON_H_


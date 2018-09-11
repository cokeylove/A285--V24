#ifndef __FUSION_API_H__ //{
#define __FUSION_API_H__

#include "chip_type.h"

#if __ENABLE_FUSION_API__ // {
// **************************************************************************
// macro define
// **************************************************************************
//#define  __ANDROID_DEF__

// **************************************************************************
// Structure define
// **************************************************************************
typedef struct _TSENSOR_FUSION_PARAM_
{
	SINT16          *acc_bias;
	SINT16          *gyr_bias;
	SINT16          mag_bias[3];
	UINT8           acc_mag_enflag;
	UINT8           mag_infr_flag;
	UINT8           gyro_enflag;
	UINT8           delta_t;
	UINT8           initial_con;
} TSENSOR_FUSION_PARAM;
typedef TSENSOR_FUSION_PARAM        TSenFusParam;
typedef TSenFusParam*               LPSenFusParam;

typedef struct _TSENSOR_FUSION_INFO_
{
	SINT32			pgyro_data[3];		// Soft-Gyro Data 
	SINT32          ea6d[3];        	// Euler Angle 6d
	SINT32          ea[3];      		// Euler Angle 9d
	SINT32          heading;			// Heading
	SINT32          q2dcm[9];    		// Quaternion to DCM
	SINT32          qest9d[4];      	// Quaternion 9d
	SINT32          q_pgyo_6d[4];		// Soft-Gyro Quaternion
	SINT32			q_pgyo_9d[4];		// Soft-Gyro Quaternion
	SINT32			ea_android[3];		// Android Setting
	SINT32			qest_android[4];	// Android Setting
} TSENSOR_FUSION_INFO;
typedef TSENSOR_FUSION_INFO         TSenFusInfo;
typedef TSenFusInfo*                LPSenFusInfo;

// **************************************************************************
// extern variable
// **************************************************************************
extern P_FUSION_CONFIG gpFConfig;

// **************************************************************************
// extern function
// **************************************************************************
void acc_meanvar(LPSINT32 acc_data);
void fusion_acc_mag_gyro_9d(LPSINT32 qest_post_9d, LPUINT8 initial_con, LPUINT8 acc_infr, LPUINT8 mag_infr, LPSINT32 gyro_data, LPSINT16 gyro_bias, LPUINT8 delta_t, LPSINT32 q_vec_6d, LPSINT32 qest_pre_9d);
void fusion_acc_mag_pgyro_6d(LPSINT32 q_post_pgyro_6d, LPUINT8 initial_con, LPUINT8 acc_infr, LPUINT8 mag_infr, LPSINT32 pgyro_data, LPUINT8 delta_t, LPSINT32 q_vec_6d, LPSINT32 q_pre_pgyro_6d);
BOOL gyro_bias_from_rawdata(LPSINT16 gyro_bias, LPSINT32 gyro_raw_data, LPSINT32 gyro_bias_sum, LPSINT16 gyro_bias_ctrl);
UINT8 gyro_data_from_6d(LPSINT32 gyro_data_6d, LPUINT8 delta_t, LPSINT32 quat_6d_pre, LPSINT32 quat_6d_post);
void fusion_function_9d(LPSenFusParam pParam,
                     	LPSINT32 acc_data, LPSINT32 mag_data, LPSINT32 gyro_data,
                     	LPSenFusInfo pPre, LPSenFusInfo pPost);
void fusion_function_pgyo_6d(LPSenFusParam pParam, 
							 LPSINT32 acc_data, LPSINT32 mag_data,
							 LPSenFusInfo pPre, LPSenFusInfo pPost);

//*****************************************************
#endif // } __ENABLE_FUSION_API__

#endif //} __FUSION_API_H__


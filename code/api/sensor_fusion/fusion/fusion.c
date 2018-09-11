#include "fusion_common.h"

#if __ENABLE_FUSION_API__ // {
// **************************************************************************
//  Fusion Library Version
// **************************************************************************
#define FUSION_VER_BYTE0  0x00 // major version
#define FUSION_VER_BYTE1  0x26 // minor version

#define FUSION_VER_NUM    ((FUSION_VER_BYTE1<<24) | (FUSION_VER_BYTE0<<16))

FUSION_VER_RODATA const char szFusionVersion[16] = 
{
    'F', 'u', 's', 'i', 'o', 'n', 'V', 'e', 'r', ':',
    '0'+(FUSION_VER_BYTE0>>4), '0'+(FUSION_VER_BYTE0 & 0x0F), '.',
    '0'+(FUSION_VER_BYTE1>>4), '0'+(FUSION_VER_BYTE1 & 0x0F), '\0'
};

FUSION_VER_RODATA const char szFusionHotfixVersion[16] = "Hotfix:0010";
// V0.25, Hotfix:0001 -> support Gyro constant rate
// V0.26, Hotfix:0001 -> support Gyro constant rate
// V0.26, Hotfix:0002 -> increase PGYRO_FIR_TAP
// V0.26, Hotfix:0003 -> modify for pseudo Gyro
// V0.26, Hotfix:0004 -> fixed boundary issue (pseudo Gyro)
// V0.26, Hotfix:0005 -> pseudo Gyro not always do calibration
// V0.26, Hotfix:0006 -> modify lp_coeff_mag_pgyo_6d[]
// V0.26, Hotfix:0007 -> modify gyro_data_from_6d(): Check W Sign, add return value
// V0.26, Hotfix:0008 -> pseudo Gyro always do calibration
// V0.26, Hotfix:0009 -> pseudo Gyro 9D feedback
// V0.26, Hotfix:0010 -> change condition for Two-Acc Angle calculation

// **************************************************************************
//  Fusion Library Feature
// **************************************************************************
#define _SUPPORT_CONSTANT_RATE_   1

// **************************************************************************
//  Global Variable (Soft-Gyro, 6D)
// **************************************************************************
#define PGYRO_FIR_TAP       4
#define PGYRO_FIR_TAP_SKIP  8
#define FIR_TAP_ACC_PGYO_6D     8
#define FIR_TAP_MAG_PGYO_6D     16

SENSOR_DATA UINT8 st_pt_acc_6d = 0;
SENSOR_DATA UINT8 st_pt_mag_6d = 0;

const SINT16 SENSOR_RODATA lp_coeff_acc_pgyo_6d[] =
{
	// 4-Tap
	//659, 7565, 16320, 7565, 659

	// 8-Tap
	166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166

	// 16-Tap
	//301, 433, 807, 1368, 2030, 2691, 3252, 3627, 3758, 3627, 3252, 2691, 2030, 1368, 807, 433, 301
};

const SINT16 SENSOR_RODATA lp_coeff_mag_pgyo_6d[] =
{
	// 4-Tap
	//659, 7565, 16320, 7565, 659

	// 8-Tap
	//166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166

	// 16-Tap
//		84, 189, 482, 1031, 1818, 2735, 3601, 4223, 4449, 4223, 3601, 2735, 1818, 1031, 482, 189, 84
    934, 925, 1298, 1690, 2073, 2415, 2685, 2858, 2917, 2858, 2685, 2415, 2073, 1690, 1298, 925, 934
};

const SINT16 SENSOR_RODATA lp_coeff_pgyo[] = { 659, 7565, 16320, 7565, 659};
//const SINT16 SENSOR_RODATA lp_coeff_pgyo[] = { 166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166 };

//const SINT16 SENSOR_RODATA lp_coeff_pgyo_skip[] = { 331, 7220, 17666, 7220, 331};
const SINT16 SENSOR_RODATA lp_coeff_pgyo_skip[] = { 166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166 };

SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_x_rdata = {0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_y_rdata = {0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_z_rdata = {0, 0, 0, 0, 0};
/*
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_x_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_y_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_z_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
*/
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_x_rdata_skip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_y_rdata_skip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 pgyo_z_rdata_skip = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/*
SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_x_rdata_skip = {0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_y_rdata_skip = {0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_5 pgyo_z_rdata_skip = {0, 0, 0, 0, 0};
*/

SENSOR_DATA SINT32 pgyro_data[3] = {0, 0, 0};
SENSOR_DATA UINT8 motion_flag = 1;
SENSOR_DATA UINT16 motion_cnt = 0;

// Pseudo Gyro
SENSOR_DATA UINT8 pgyo_cnt = 0;
SENSOR_DATA UINT8 pgyo_st_pt = 0;
SENSOR_DATA UINT8 pgyo_st_pt_skip = 0;
SENSOR_BSS  UINT8 pgyro_skip_delta_t;
SENSOR_DATA SINT32 pgyro_qvec_6d_pre[4] = {0, 0, 0, 0};
SENSOR_DATA SINT32 pgyro_qvec_6d_post[4] = {0, 0, 0, 0};
SENSOR_DATA SINT32 gyro_data_6d[3] = {0, 0, 0};
SENSOR_DATA SINT32 gyro_data_6d_check[3] = {0, 0, 0};

// **************************************************************************
//  Global Variable (Real-Gyro, 9D)
// **************************************************************************
#define FIR_TAP_ACC_GYO_9D     4
#define FIR_TAP_MAG_GYO_9D     8

SENSOR_DATA UINT8 st_pt_acc_9d = 0;
SENSOR_DATA UINT8 st_pt_mag_9d = 0;

const SINT16 SENSOR_RODATA lp_coeff_acc_9d[] =
{
	// 4-Tap
	331, 7220, 17666, 7220, 331
	
	// 8-Tap
	//166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166
};

const SINT16 SENSOR_RODATA lp_coeff_mag_9d[] =
{
	// 4-Tap
	//331, 7220, 17666, 7220, 331
	
	// 8-Tap
	166, 962, 3629, 7187, 8879, 7187, 3629, 962, 166
};

SENSOR_DATA SINT8 linr_acc_cnt = 0;
SENSOR_DATA SINT8 linr_acc_num = 0;

SENSOR_DATA SINT16 gyro_bias_ctrl[3] = {0, 256, 0};
SENSOR_DATA SINT32 gyro_bias_sum[3] = {0, 0, 0};

SENSOR_DATA SINT8 gyro_cal_en = 1;
SENSOR_DATA SINT8 gyo_stop_cnt = 0;
SENSOR_DATA UINT8 fusion_cal_en = 1;
	
SENSOR_DATA UINT8 shake_i;
SENSOR_DATA SINT32 acc_length_van = 0;
SENSOR_DATA SINT16 acc_length_matrix[8] = {0};

SENSOR_DATA SINT16 gyro_vec[3];
SENSOR_DATA SINT32 gyro_mul_t[3];
SENSOR_DATA SINT16 abs_gyro_mul_t[3];
SENSOR_DATA SINT32 gyro_xdata_matrix[8] = {0};
SENSOR_DATA SINT32 gyro_ydata_matrix[8] = {0};
SENSOR_DATA SINT32 gyro_zdata_matrix[8] = {0};
SENSOR_DATA UINT8 static_flag_cnt = 0;

SENSOR_DATA UINT8 pad_nb_excnt = 0;

// **************************************************************************
//  Global Variable (Fusion Parameter)
// **************************************************************************
SENSOR_DATA TSenFusParam mSenFuParam =
{
	NULL,
	NULL,
	{ 0, 0, 0 },
	ACC_MAG_EN, 0, GYRO_EN,
	DELTA_T,
	0,
};
SENSOR_BSS TSenFusInfo  mSFInfoPre;
SENSOR_BSS TSenFusInfo  mSFInfoPost;

SENSOR_BSS P_FUSION_CONFIG gpFConfig = NULL;

SENSOR_DATA SINT16 gravity[3] = {0};
//------------------------------------
//  Accelerometer Mean & Var(mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
void SENSOR_CODE acc_meanvar(LPSINT32 acc_data)
{
	int i, j;
	
	// ACC Mean & Van
	SINT32 acc_length_mean = 0;
	SINT32 shift_in;
	SINT32 norm_total, acc_length_2;
	
	// Accelerometer Magnitude
	acc_length_2 = (*(acc_data + 0)) * (*(acc_data + 0)) + 
				   (*(acc_data + 1)) * (*(acc_data + 1)) + 
				   (*(acc_data + 2)) * (*(acc_data + 2));
	sqrt_func(&norm_total, &acc_length_2);
	
	// Data Length Window
	for(i = 0 ; i < 8 ; i++)
	{
		j = i + 1;
		if(i < 7)
			acc_length_matrix[i] = acc_length_matrix[j];
		else
			acc_length_matrix[i] = norm_total;
	}
	
	// Mean
	for(i = 0 ; i < 8 ; i++)
	{
		acc_length_mean += acc_length_matrix[i];
	}
	shift_in = acc_length_mean;
	rshift_func(&acc_length_mean, &shift_in, 3);
	
	// Var
	acc_length_van = 0;
	for(i = 0 ; i < 8 ; i++)
	{
		acc_length_van += (acc_length_matrix[i] - acc_length_mean) * 
                          (acc_length_matrix[i] - acc_length_mean);
	}
	acc_length_van = acc_length_van >> 3;
	//RTL_PRINTF("%d.\r\n", acc_length_van);
	
	return;
}

//------------------------------------
//  Fusion Function For 9D (mul: , div: )
//  Based On Accelerometer, Compass And Gyrometer
//  Quaternion
//  tan:   sin/cos:   mul:   div:
//------------------------------------
void SENSOR_CODE fusion_acc_mag_gyro_9d(LPSINT32 qest_post_9d, LPUINT8 initial_con, LPUINT8 acc_infr, LPUINT8 mag_infr,
                                        LPSINT32 gyro_data, LPSINT16 gyro_bias, LPUINT8 delta_t, LPSINT32 q_vec_6d, LPSINT32 qest_pre_9d)
{
	// Shift Function
	SINT32 shift_in, shift_out, shift_bit;

	// Mul
	int i, j, entry_index;
	SINT32 mul_tmp_1, mul_tmp_2, mul_tmp_3;

	// For Data Normalized (1/0 -> Quaternion/Raw Data)
	SINT32 norm_total, quat_data_en;

	// State Equation
	SINT32 q_Osserv[4] = {256, 0, 0, 0};

	// Quaternion Matrix For Process Equation
	SINT16 q_matrix[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SINT32 q_Predict[4] = {256, 0, 0, 0};

	// Kalman Gain
	int static_flag = 0;
	int ang_no_change_flag = 0;
	SINT32 dis_1, dis_2;
	SINT16 dis_6d_9d = 0;
	SINT16 kg_value = 0;
	SINT32 dis_qpdict_osv = 0;
	SINT32 sum_kg_data = 0;
	SINT16 kalman_gain[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SINT16 diff_qpdict_osv[4] = {0, 0, 0, 0};

	// ---------- State Equation ----------
	quatern_equal(&q_Osserv[0], q_vec_6d);

	// --------- Process Equation ---------
	// Process Equation Initial Value Based On 6D
	if((*initial_con == 0))
		quatern_equal(&q_Predict[0], q_vec_6d);
	else
		quatern_equal(&q_Predict[0], qest_pre_9d);

	// Quaternion Matrix For Gyroscope
	quatmatrix(&q_matrix[0], &q_Predict[0]);

	for(i = 0 ; i < 4 ; i++)
	{
		entry_index = i + 4;
		mul_tmp_1 = q_matrix[entry_index] * gyro_vec[0];
		entry_index = i + 8;
		mul_tmp_2 = q_matrix[entry_index] * gyro_vec[1];
		entry_index = i + 12;
		mul_tmp_3 = q_matrix[entry_index] * gyro_vec[2];
		shift_in = mul_tmp_1 + mul_tmp_2 + mul_tmp_3;
		for(j = 0 ; j < 2 ; j++)
		{
			shift_bit = 17 - j;
		rshift_func(&shift_out, &shift_in, shift_bit);
			if(shift_out != 0)
				break;
		}
		q_Predict[i] = q_Predict[i] + shift_out;
	}

	// Normalized
	quat_data_en = 1;
	norm_func(&q_Predict[0], &norm_total, &quat_data_en);

	// -------------Kalman Filter Process---------------
	// Distance Between qPredicted And qOsserv
	dis_1 = 0;
	dis_2 = 0;
    shift_bit = 8;
	for(i = 0 ; i < 4 ; i++)
	{
		// Distance Between qOsserv And q_Predict
		dis_qpdict_osv = q_Osserv[i] - q_Predict[i];
		shift_in = dis_qpdict_osv * dis_qpdict_osv;
		rshift_func(&shift_out, &shift_in, shift_bit);
		dis_1 = dis_1 + shift_out;

		// Distance Between qOsserv And -q_Predict
		dis_qpdict_osv = q_Osserv[i] + q_Predict[i];
		shift_in = dis_qpdict_osv * dis_qpdict_osv;
		rshift_func(&shift_out, &shift_in, shift_bit);
		dis_2 = dis_2 + shift_out;
	}

	if(dis_1 >= dis_2)
	{
		dis_6d_9d = dis_2;
		for(i = 0 ; i < 4 ; i++)
			diff_qpdict_osv[i] = (-q_Osserv[i]) - q_Predict[i];
	}
	else
	{
		dis_6d_9d = dis_1;
		for(i = 0 ; i < 4 ; i++)
			diff_qpdict_osv[i] = q_Osserv[i] - q_Predict[i];
	}
	
	// Kalman Gain Setting
	if(pad_NB_mode_flag == 1)
	{
		pad_nb_excnt++;
		if(pad_nb_excnt == 4)
		{
			pad_NB_mode_flag = 0;
			pad_nb_excnt = 0;
		}
			
		ang_no_change_flag = 0;
		kg_value = 150;
		kg_value_set(&kalman_gain[0], &kg_value);
		//RTL_PRINTF("---NB<->PAD---.\r\n");
	}
	else if((*acc_infr == 1) || (linr_acc_cnt != 0))
	{
		if((linr_acc_cnt != linr_acc_num) && (*acc_infr == 1))
			linr_acc_cnt = linr_acc_num;
		else if(linr_acc_cnt > 0)
			linr_acc_cnt--;
		
		ang_no_change_flag = 0;
		kg_value = 0;
		kg_value_set(&kalman_gain[0], &kg_value);
		//RTL_PRINTF("---ACC INFR---.\r\n");
	}
	else if(((abs_gyro_mul_t[0] > 10) || (abs_gyro_mul_t[1] > 10) || (abs_gyro_mul_t[2] > 10)) && (acc_length_van != 0))
	{
		if(dis_6d_9d > 100)
		{
			kg_value = 35;
			//RTL_PRINTF("---2-A---.\r\n");
		}
		else if(dis_6d_9d > 50)
		{
			kg_value = 15;
			//RTL_PRINTF("---2-B---.\r\n");
		}
		else if(dis_6d_9d > 20)
		{
			kg_value = 5;
			//RTL_PRINTF("---2-C---.\r\n");
		}
		else
		{
			kg_value = 0;
			//RTL_PRINTF("---2-D---.\r\n");
		}
		ang_no_change_flag = 0;
		kg_value_set(&kalman_gain[0], &kg_value);
	}
	else if((abs_gyro_mul_t[0] > 5) || (abs_gyro_mul_t[1] > 5) || (abs_gyro_mul_t[2] > 5))
	{
		if(dis_6d_9d > 10)
		{
			kg_value = 0;
			//RTL_PRINTF("---3-A---.\r\n");

		}
		else
		{
			kg_value = 15;
			//RTL_PRINTF("---3-B---.\r\n");
		}
		ang_no_change_flag = 0;
		kg_value_set(&kalman_gain[0], &kg_value);
	}
	else
	{
        #if 0
        if((abs(abs_gyro_mul_t[0]) < 1) && (abs(abs_gyro_mul_t[1]) < 1) && (abs(abs_gyro_mul_t[2])  < 1))
        #else
		if((abs(gyro_vec[0]) < 5) && (abs(gyro_vec[1]) < 5) && (abs(gyro_vec[2])  < 5))
        #endif
		{
			static_flag = 1;
			kg_value = 50;
			if(static_flag_cnt == 8)
			{
				ang_no_change_flag = 1;
				kg_value = 0;
				//RTL_PRINTF("---STOP---.\r\n");
			}
		}
		else
		{
			ang_no_change_flag = 0;
			kg_value = 80;
			//RTL_PRINTF("---4-A---.\r\n");
		}
		kg_value_set(&kalman_gain[0], &kg_value);
	}
	//RTL_PRINTF("DIS = %d.\r\n", dis_6d_9d);

	if((static_flag == 1) && (static_flag_cnt < 8))
		static_flag_cnt = static_flag_cnt + 1;
	else if(static_flag == 0)
		static_flag_cnt = 0;
	//RTL_PRINTF("%d.\r\n",static_flag_cnt);

	// Updata Estimate Quaternion Equation
	if(ang_no_change_flag == 1)
		quatern_equal(qest_post_9d, qest_pre_9d);
	else if((*initial_con == 0) || (kg_value == 0))
		quatern_equal(qest_post_9d, &q_Predict[0]);
	else
	{
		for(i = 0 ; i < 4 ; i++)
		{
			sum_kg_data = 0;
			shift_bit = 8;
			for(j = 0 ; j < 4 ; j++)
			{
				entry_index = i + (j << 2);
				sum_kg_data = sum_kg_data + (kalman_gain[entry_index] * diff_qpdict_osv[j]);
			}
			shift_in = sum_kg_data;
				rshift_func(&shift_out, &shift_in, shift_bit);
			sum_kg_data = shift_out;
			*(qest_post_9d + i) = q_Predict[i] + sum_kg_data;
		}

		// Normalized
		quat_data_en = 1;
		norm_func(qest_post_9d, &norm_total, &quat_data_en);
	}

	// Quaternion (w,x,y,z, w > 0)
	if(*(qest_post_9d + 0) < 0)
	{
		*(qest_post_9d + 0) = -(*(qest_post_9d + 0));
		*(qest_post_9d + 1) = -(*(qest_post_9d + 1));
		*(qest_post_9d + 2) = -(*(qest_post_9d + 2));
		*(qest_post_9d + 3) = -(*(qest_post_9d + 3));
	}


	return;
}

//------------------------------------
//  Fusion Function For 6D (mul: , div: )
//  Based On Accelerometer, Compass And Software-Gyrometer
//  Quaternion
//  tan:   sin/cos:   mul:   div:
//------------------------------------
void SENSOR_CODE fusion_acc_mag_pgyro_6d(LPSINT32 q_post_pgyro_6d, LPUINT8 initial_con, LPUINT8 acc_infr, LPUINT8 mag_infr,
        								 LPSINT32 pgyro_data, LPUINT8 delta_t, LPSINT32 q_vec_6d, LPSINT32 q_pre_pgyro_6d)
{
	// Shift Function
	SINT32 shift_in, shift_out, shift_bit;

	// Mul
	int i, j, entry_index;
	SINT32 mul_tmp_1, mul_tmp_2, mul_tmp_3;

	// For Data Normalized (1/0 -> Quaternion/Raw Data)
	SINT32 norm_total, quat_data_en;

	// State Equation
	SINT32 q_Osserv[4] = {256, 0, 0, 0};

	// Quaternion Matrix For Process Equation
	SINT16 q_matrix[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SINT32 q_Predict[4] = {256, 0, 0, 0};

	// Kalman Gain
	SINT32 pgyro_mul_t[3];
	SINT32 dis_1, dis_2;
	SINT16 kg_value = 0;
	SINT32 dis_qpdict_osv = 0;
	SINT32 sum_kg_data = 0;
	SINT16 kalman_gain[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SINT16 diff_qpdict_osv[4] = {0, 0, 0, 0};

	// ---------- State Equation ----------
	quatern_equal(&q_Osserv[0], q_vec_6d);

	// --------- Process Equation ---------
	// Process Equation Initial Value Based On 6D
	if((*initial_con == 0))
		quatern_equal(&q_Predict[0], q_vec_6d);
	else
		quatern_equal(&q_Predict[0], q_pre_pgyro_6d);

	// Quaternion Matrix For Gyroscope
	quatmatrix(&q_matrix[0], &q_Predict[0]);

	shift_bit = 8;
	for(i = 0 ; i < 3 ; i++)
	{
		shift_in = *(pgyro_data + i) * (*delta_t);
		rshift_func(&pgyro_mul_t[i], &shift_in, shift_bit);
	}

	shift_bit = 9;
	for(i = 0 ; i < 4 ; i++)
	{
		entry_index = i + 4;
		mul_tmp_1 = q_matrix[entry_index] * (pgyro_mul_t[0]);
		entry_index = i + 8;
		mul_tmp_2 = q_matrix[entry_index] * (pgyro_mul_t[1]);
		entry_index = i + 12;
		mul_tmp_3 = q_matrix[entry_index] * (pgyro_mul_t[2]);
		shift_in = mul_tmp_1 + mul_tmp_2 + mul_tmp_3;
		rshift_func(&shift_out, &shift_in, shift_bit);
		q_Predict[i] = q_Predict[i] + shift_out;
	}

	// Normalized
	quat_data_en = 1;
	norm_func(&q_Predict[0], &norm_total, &quat_data_en);

	// -------------Kalman Filter Process---------------
	// Distance Between qPredicted And qOsserv
	shift_bit = 8;
	dis_1 = 0;
	dis_2 = 0;
	for(i = 0 ; i < 4 ; i++)
	{
		// Distance Between qOsserv And q_Predict
		dis_qpdict_osv = q_Osserv[i] - q_Predict[i];
		shift_in = dis_qpdict_osv * dis_qpdict_osv;
		rshift_func(&shift_out, &shift_in, shift_bit);
		dis_1 = dis_1 + shift_out;

		// Distance Between qOsserv And -q_Predict
		dis_qpdict_osv = q_Osserv[i] + q_Predict[i];
		shift_in = dis_qpdict_osv * dis_qpdict_osv;
		rshift_func(&shift_out, &shift_in, shift_bit);
		dis_2 = dis_2 + shift_out;
	}

	if(dis_1 >= dis_2)
	{
		for(i = 0 ; i < 4 ; i++)
			diff_qpdict_osv[i] = (-q_Osserv[i]) - q_Predict[i];
	}
	else
	{
		for(i = 0 ; i < 4 ; i++)
			diff_qpdict_osv[i] = q_Osserv[i] - q_Predict[i];
	}

	// Kalman Gain Setting
	if((*mag_infr == 1))
	{
		kg_value = 0;
		kg_value_set(&kalman_gain[0], &kg_value);
		//RTL_PRINTF("---I---.\r\n");
	}
	else
	{
		kg_value = 125;
		kg_value_set(&kalman_gain[0], &kg_value);
		//RTL_PRINTF("---II---.\r\n");
	}

	// Updata Estimate Quaternion Equation
	if((*initial_con == 0) || (kg_value == 0))
		quatern_equal(q_post_pgyro_6d, &q_Predict[0]);
	else
	{
		for(i = 0 ; i < 4 ; i++)
		{
			sum_kg_data = 0;
			shift_bit = 8;
			for(j = 0 ; j < 4 ; j++)
			{
				entry_index = i + (j << 2);
				sum_kg_data = sum_kg_data + (kalman_gain[entry_index] * diff_qpdict_osv[j]);
			}
			shift_in = sum_kg_data;
				rshift_func(&shift_out, &shift_in, shift_bit);
			sum_kg_data = shift_out;
			*(q_post_pgyro_6d + i) = q_Predict[i] + sum_kg_data;
		}

		// Normalized
		quat_data_en = 1;
		norm_func(q_post_pgyro_6d, &norm_total, &quat_data_en);
	}

	// (w,x,y,z) (w must > 0)
	if(*(q_post_pgyro_6d + 0) < 0)
	{
		*(q_post_pgyro_6d + 0) = -(*(q_post_pgyro_6d + 0));
		*(q_post_pgyro_6d + 1) = -(*(q_post_pgyro_6d + 1));
		*(q_post_pgyro_6d + 2) = -(*(q_post_pgyro_6d + 2));
		*(q_post_pgyro_6d + 3) = -(*(q_post_pgyro_6d + 3));
	}

	return;
}

//------------------------------------
//  Gyro Data From Quaternion Of 6D (mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
UINT8 SENSOR_CODE gyro_data_from_6d(LPSINT32 gyro_data_6d, LPUINT8 delta_t, LPSINT32 quat_6d_pre, LPSINT32 quat_6d_post)
{
	// Quaternion Data (6D/3D)
	SINT32 check_q;
	SINT32 q_dot_sum_1, q_dot_sum_2;
	SINT32 quat_6d_tmp[4], q_dot_6d_1[4], q_dot_6d_2[4], q_dot_6d[4];
	SINT8 sign_qpre_w, sign_qpost_w;
	
	// Check W Sign
	sign_qpre_w = sign_func((quat_6d_pre + 0));
	sign_qpost_w = sign_func((quat_6d_post + 0));
	if(sign_qpre_w != sign_qpost_w)
	{
		return 1;
	}

	quat_6d_tmp[0] = *(quat_6d_pre + 0);
	quat_6d_tmp[1] = *(quat_6d_pre + 1);
	quat_6d_tmp[2] = *(quat_6d_pre + 2);
	quat_6d_tmp[3] = *(quat_6d_pre + 3);

	q_dot_6d_1[0] = *(quat_6d_post + 0) - quat_6d_tmp[0];
	q_dot_6d_1[1] = *(quat_6d_post + 1) - quat_6d_tmp[1];
	q_dot_6d_1[2] = *(quat_6d_post + 2) - quat_6d_tmp[2];
	q_dot_6d_1[3] = *(quat_6d_post + 3) - quat_6d_tmp[3];
	q_dot_sum_1 = q_dot_6d_1[0]*q_dot_6d_1[0] + q_dot_6d_1[1]*q_dot_6d_1[1] + 
                  q_dot_6d_1[2]*q_dot_6d_1[2] + q_dot_6d_1[3]*q_dot_6d_1[3];

	q_dot_6d_2[0] = -(*(quat_6d_post + 0)) - quat_6d_tmp[0];
	q_dot_6d_2[1] = -(*(quat_6d_post + 1)) - quat_6d_tmp[1];
	q_dot_6d_2[2] = -(*(quat_6d_post + 2)) - quat_6d_tmp[2];
	q_dot_6d_2[3] = -(*(quat_6d_post + 3)) - quat_6d_tmp[3];
	q_dot_sum_2 = q_dot_6d_2[0]*q_dot_6d_2[0] + q_dot_6d_2[1]*q_dot_6d_2[1] + 
                  q_dot_6d_2[2]*q_dot_6d_2[2] + q_dot_6d_2[3]*q_dot_6d_2[3];

	if(q_dot_sum_1 > q_dot_sum_2)
	{
		q_dot_6d[0] = q_dot_6d_2[0];
		q_dot_6d[1] = q_dot_6d_2[1];
		q_dot_6d[2] = q_dot_6d_2[2];
		q_dot_6d[3] = q_dot_6d_2[3];
	}
	else
	{
		q_dot_6d[0] = q_dot_6d_1[0];
		q_dot_6d[1] = q_dot_6d_1[1];
		q_dot_6d[2] = q_dot_6d_1[2];
		q_dot_6d[3] = q_dot_6d_1[3];
	}
	//RTL_PRINTF("(%d:%d:%d:%d).\r\n", q_dot_6d[0], q_dot_6d[1], q_dot_6d[2], q_dot_6d[3]);

	check_q    	   = ((quat_6d_tmp[0] * q_dot_6d[0] + quat_6d_tmp[1] * q_dot_6d[1] + quat_6d_tmp[2] * q_dot_6d[2] + quat_6d_tmp[3] * q_dot_6d[3]) << 1) / (*delta_t);
	*(gyro_data_6d + 0) = ((-quat_6d_tmp[1] * q_dot_6d[0] + quat_6d_tmp[0] * q_dot_6d[1] + quat_6d_tmp[3] * q_dot_6d[2] - quat_6d_tmp[2] * q_dot_6d[3]) << 1) / (*delta_t);
	*(gyro_data_6d + 1) = ((-quat_6d_tmp[2] * q_dot_6d[0] - quat_6d_tmp[3] * q_dot_6d[1] + quat_6d_tmp[0] * q_dot_6d[2] + quat_6d_tmp[1] * q_dot_6d[3]) << 1) / (*delta_t);
	*(gyro_data_6d + 2) = ((-quat_6d_tmp[3] * q_dot_6d[0] + quat_6d_tmp[2] * q_dot_6d[1] - quat_6d_tmp[1] * q_dot_6d[2] + quat_6d_tmp[0] * q_dot_6d[3]) << 1) / (*delta_t);
	//RTL_PRINTF("(%d:%d:%d:%d), (%d:%d:%d:%d).\r\n", q_dot_6d[0], q_dot_6d[1], q_dot_6d[2], q_dot_6d[3], check_q, *(gyro_data_6d + 0), *(gyro_data_6d + 1), *(gyro_data_6d + 2));

	return 0;
}

//------------------------------------
//  Gyro Bias Estimate From Raw Data(mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
BOOL SENSOR_CODE gyro_bias_from_rawdata(LPSINT16 gyro_bias, LPSINT32 gyro_raw_data, LPSINT32 gyro_bias_sum, LPSINT16 gyro_bias_ctrl)
{
	// Operate Param
	int shift_bit = 8;
	SINT32 shift_in, shift_out;
	
	// Gyro Mean & Van
	SINT32 gyro_rate_det[3] = {0, 0, 0};
	SINT32 gyro_var[3] = {0, 0, 0};
	SINT32 gyro_mean[3] = {0, 0, 0};
	
	// Gyro Bias Control Information
	int i, j;
	SINT16 static_cnt, static_num, gyro_delta_t;
	BOOL ret = FALSE;

	// Bias Control Information
	static_cnt = *(gyro_bias_ctrl + 0);
	static_num = *(gyro_bias_ctrl + 1);
	gyro_delta_t = *(gyro_bias_ctrl + 2);

	// Data Length Window
	for(i = 0 ; i < 8 ; i++)
	{
		if(i < 7)
		{
			j = i + 1;
			gyro_xdata_matrix[i] = gyro_xdata_matrix[j];
			gyro_ydata_matrix[i] = gyro_ydata_matrix[j];
			gyro_zdata_matrix[i] = gyro_zdata_matrix[j];
		}
		else
		{
			// Unit:DPS/256 
			gyro_xdata_matrix[i] = (*(gyro_raw_data + 0)*2238)/10000;
			gyro_ydata_matrix[i] = (*(gyro_raw_data + 1)*2238)/10000;
			gyro_zdata_matrix[i] = (*(gyro_raw_data + 2)*2238)/10000;
		}
	}

	// Variance
	for(i = 0 ; i < 8 ; i++)
	{
		gyro_mean[0] += gyro_xdata_matrix[i];
		gyro_mean[1] += gyro_ydata_matrix[i];
		gyro_mean[2] += gyro_zdata_matrix[i];
	}

	shift_bit = 3;
	shift_in = gyro_mean[0];
	rshift_func(&shift_out, &shift_in, shift_bit);
	gyro_mean[0] = shift_out;
	shift_in = gyro_mean[1];
	rshift_func(&shift_out, &shift_in, shift_bit);
	gyro_mean[1] = shift_out;
	shift_in = gyro_mean[2];
	rshift_func(&shift_out, &shift_in, shift_bit);
	gyro_mean[2] = shift_out;

	for(i = 0 ; i < 8 ; i++)
	{		
		gyro_var[0] += (gyro_xdata_matrix[i]-gyro_mean[0]) * (gyro_xdata_matrix[i]-gyro_mean[0]);
		gyro_var[1] += (gyro_ydata_matrix[i]-gyro_mean[1]) * (gyro_ydata_matrix[i]-gyro_mean[1]);
		gyro_var[2] += (gyro_zdata_matrix[i]-gyro_mean[2]) * (gyro_zdata_matrix[i]-gyro_mean[2]);
		
		gyro_rate_det[0] += gyro_xdata_matrix[i]*gyro_xdata_matrix[i];
		gyro_rate_det[1] += gyro_ydata_matrix[i]*gyro_ydata_matrix[i];
		gyro_rate_det[2] += gyro_zdata_matrix[i]*gyro_zdata_matrix[i];
	}
	gyro_var[0] = gyro_var[0] >> 3;
	gyro_var[1] = gyro_var[1] >> 3;
	gyro_var[2] = gyro_var[2] >> 3;

	gyro_rate_det[0] = gyro_rate_det[0] >> 3;
	gyro_rate_det[1] = gyro_rate_det[1] >> 3;
	gyro_rate_det[2] = gyro_rate_det[2] >> 3;

	if(gyro_var[0] == 0)
		gyro_rate_det[0] = 0;
	else
		gyro_rate_det[0] = (gyro_rate_det[0]/gyro_var[0])/8;

	if(gyro_var[1] == 0)
		gyro_rate_det[1] = 0;
	else
		gyro_rate_det[1] = (gyro_rate_det[1]/gyro_var[1])/8;

	if(gyro_var[2] == 0)
		gyro_rate_det[2] = 0;
	else
		gyro_rate_det[2] = (gyro_rate_det[2]/gyro_var[2])/8;
	//RTL_PRINTF("(%d:%d:%d)(%d:%d:%d).\r\n", gyro_var[0], gyro_var[1], gyro_var[2], gyro_rate_det[0], gyro_rate_det[1], gyro_rate_det[2]);

	//RTL_PRINTF("%d.\r\n", static_cnt);
	if((gyro_rate_det[0] == 0) && (gyro_rate_det[1] == 0) && (gyro_rate_det[2] == 0)
#if _SUPPORT_CONSTANT_RATE_ // {
        && (abs(*(gyro_raw_data + 0)) < 20) && (abs(*(gyro_raw_data + 1)) < 20) && (abs(*(gyro_raw_data + 2)) < 20) // < 5dps
#endif // } _SUPPORT_CONSTANT_RATE_
       )
	{
		static_cnt = static_cnt + 1;
		*(gyro_bias_sum + 0) += *(gyro_raw_data + 0);
		*(gyro_bias_sum + 1) += *(gyro_raw_data + 1);
		*(gyro_bias_sum + 2) += *(gyro_raw_data + 2);

		if(static_cnt == static_num)
		{
			*(gyro_bias + 0) = *(gyro_bias_sum + 0)/static_num;
			*(gyro_bias + 1) = *(gyro_bias_sum + 1)/static_num;
			*(gyro_bias + 2) = *(gyro_bias_sum + 2)/static_num;
						
			static_cnt = 0;
			*(gyro_bias_sum + 0) = 0;
			*(gyro_bias_sum + 1) = 0;
			*(gyro_bias_sum + 2) = 0;

            gyro_cal_done_flag = 1;

			ret = TRUE;
			//RTL_PRINTF("%d:%d:%d.\r\n", *(gyro_bias + 0), *(gyro_bias + 1), *(gyro_bias + 2));
		}
	}
	else
	{
		static_cnt = 0;
		*(gyro_bias_sum + 0) = 0;
		*(gyro_bias_sum + 1) = 0;
		*(gyro_bias_sum + 2) = 0;
	}
	*(gyro_bias_ctrl + 0) = static_cnt;

	// Compass Calibration Enable (With Gyro Detect)
	shift_bit = 8;
	for(i = 0 ; i < 3 ; i++)
	{
		gyro_vec[i] = (*(gyro_raw_data + i) - *(gyro_bias + i)) * gyro_delta_t;
		shift_in = gyro_vec[i];
		rshift_func(&shift_out, &shift_in, shift_bit);
		gyro_mul_t[i] = shift_out;
		abs_gyro_mul_t[i] = abs(gyro_mul_t[i]);
	}
	//RTL_PRINTF("(%d:%d:%d)->(%d:%d:%d).\r\n", gyro_vec[0]*2238/10000, gyro_vec[1]*2238/10000, gyro_vec[2]*2238/10000, gyro_mul_t[0], gyro_mul_t[1], gyro_mul_t[2]);

	if(((abs_gyro_mul_t[0] > 10) || (abs_gyro_mul_t[1] > 10) || (abs_gyro_mul_t[2] > 10)) && (acc_length_van != 0))
		compass_cal_flag = 1;
	else
		compass_cal_flag = 0;

	return ret;
}

//------------------------------------
//  Sensor Fusion Algorithm (main)
//------------------------------------
void SENSOR_CODE fusion_function_9d(LPSenFusParam pParam,
                                    LPSINT32 acc_data, LPSINT32 mag_data, LPSINT32 gyro_data,
                                    LPSenFusInfo pPre, LPSenFusInfo pPost)
{
	// For Accelerometer
	UINT8 acc_infr;

	// For Magnetometer
	SINT32 yaw_change_en;

	// DCM/Quaternion For 6D
	UINT8 angle_update = 0;
//      SINT32 dcm_6d[9];
	SINT32 quat_vec_6d[4];

	// -------------- Only Accelerometer And Magnetometer ---------------------
	// ------------------------------------------------------------------------
	if(((pParam->acc_mag_enflag == 1) || (pParam->initial_con == 0)) && (pParam->mag_infr_flag == 0))
	{
		// ----------- Calculate Roll And Pitch Angle -------------
		acc_infr = acc_roll_pitch_6d(&pPost->ea6d[0], &pPost->ea6d[1], acc_data, &pPre->ea6d[0], &pPre->ea6d[1]);

		// ---------------- Calculate Yaw Angle -------------------
		if(acc_infr == 0)
		{
			mag_yaw_6d(&yaw_change_en, &pPost->ea6d[2], mag_data, &pPost->ea6d[0], &pPost->ea6d[1], &pPre->ea6d[2]);

			if((yaw_change_en == 0))
			{
				pPost->ea6d[0] = pPre->ea6d[0];
				pPost->ea6d[1] = pPre->ea6d[1];
			}
		}
		else
		{
			pPost->ea6d[0] = pPre->ea6d[0];
			pPost->ea6d[1] = pPre->ea6d[1];
			pPost->ea6d[2] = pPre->ea6d[2];
		}
	}
	else
	{
		pPost->ea6d[0] = pPre->ea6d[0];
		pPost->ea6d[1] = pPre->ea6d[1];
		pPost->ea6d[2] = pPre->ea6d[2];
	}
	// Euler Angle --> Rotation Matrix (DCM , Transpose)
	//angle2matrix(&dcm_6d[0], (euler_angle_post_6d+0), (euler_angle_post_6d+1), (euler_angle_post_6d+2));

	// Euler Angle --> Quaternion
	angle2quatern(&quat_vec_6d[0], &pPost->ea6d[0], &pPost->ea6d[1], &pPost->ea6d[2]);

	// -------------- Accelerometer, Magnetometer And Gyroscope ---------------
	// ------------------------------------------------------------------------
	if((pParam->gyro_enflag == 1) || (pParam->initial_con == 0))
	{
		angle_update = 1;
		fusion_acc_mag_gyro_9d(pPost->qest9d, &pParam->initial_con, &acc_infr,
		                       &pParam->mag_infr_flag, gyro_data, pParam->gyr_bias,
		                       &pParam->delta_t, &quat_vec_6d[0], pPre->qest9d);
	}
	else if((pParam->gyro_enflag == 0))
	{
		angle_update = 2;
		quatern_equal(pPost->qest9d, &quat_vec_6d[0]);
	}
	else
	{
		angle_update = 0;
		quatern_equal(pPost->qest9d, pPre->qest9d);
	}

	// Quaternion --> Rotation Matrix
	quatern2matrix(pPost->q2dcm, pPost->qest9d);

	// Quaternion --> Euler Angle(9D)
	if(angle_update == 1)
	{
		// Fusion 9D
		quatern2angle(pPost->ea, pPre->ea, pPost->qest9d);
	}
	else if(angle_update == 2)
	{
		// Fusion 6D
		pPost->ea[0] = pPost->ea6d[0];
		pPost->ea[1] = pPost->ea6d[1];
		pPost->ea[2] = pPost->ea6d[2];
	}
	else
	{
		pPost->ea[0] = pPre->ea[0];
		pPost->ea[1] = pPre->ea[1];
		pPost->ea[2] = pPre->ea[2];
	}

	// Heading
	pPost->heading = compass_heading(pPost->q2dcm);
	
	if(gpFConfig->Enable_Android)
	{
		// Android System
		*(pPost->qest_android + 0) = *(pPost->qest9d + 0);
		*(pPost->qest_android + 1) = -(*(pPost->qest9d + 1));
		*(pPost->qest_android + 2) = -(*(pPost->qest9d + 2));
		*(pPost->qest_android + 3) = -(*(pPost->qest9d + 3));

		quatern2angle(pPost->ea_android, pPre->ea_android, pPost->qest_android);
	}

	return;
}

//------------------------------------
//  Sensor Fusion Algorithm (main)
//------------------------------------
void SENSOR_CODE fusion_function_pgyo_6d(
    // Input Data
    LPSenFusParam pParam, LPSINT32 acc_data, LPSINT32 mag_data,

    // Output Data
    LPSenFusInfo pPre, LPSenFusInfo pPost)
{

	int shift_bit = 15;
	SINT32 shift_in, shift_out;
	
	UINT32 *ptrData32;

	// For Accelerometer
	UINT8 acc_infr;

	// For Magnetometer
	SINT32 yaw_change_en;

	UINT8 pgyro_n_valid;
		
	// -------------- Accelerometer And Magnetometer ---------------------
	// -------------------------------------------------------------------
	if(((pParam->acc_mag_enflag == 1) || (pParam->initial_con == 0)) && (pParam->mag_infr_flag == 0))
	{
		// ----------- Calculate Roll And Pitch Angle -------------
		acc_infr = acc_roll_pitch_6d(&pPost->ea6d[0], &pPost->ea6d[1], acc_data, &pPre->ea6d[0], &pPre->ea6d[1]);

		// ---------------- Calculate Yaw Angle -------------------
		mag_yaw_6d(&yaw_change_en, &pPost->ea6d[2], mag_data, &pPost->ea6d[0], &pPost->ea6d[1], &pPre->ea6d[2]);

		if((yaw_change_en == 0))
		{
			pPost->ea6d[0] = pPre->ea6d[0];
			pPost->ea6d[1] = pPre->ea6d[1];
		}
	}
	else
	{
		pPost->ea6d[0] = pPre->ea6d[0];
		pPost->ea6d[1] = pPre->ea6d[1];
		pPost->ea6d[2] = pPre->ea6d[2];
	}

	// Euler Angle --> Quaternion (For Windows 8) (w,x,y,z) (w must > 0)
	angle2quatern(pPost->q_pgyo_6d, &pPost->ea6d[0], &pPost->ea6d[1], &pPost->ea6d[2]);
/*
	if(pPost->q_pgyo_6d[0] < 0)
	{
		pPost->q_pgyo_6d[0] = -(pPost->q_pgyo_6d[0]);
		pPost->q_pgyo_6d[1] = -(pPost->q_pgyo_6d[1]);
		pPost->q_pgyo_6d[2] = -(pPost->q_pgyo_6d[2]);
		pPost->q_pgyo_6d[3] = -(pPost->q_pgyo_6d[3]);
	}
*/
	// Psuedo-Gyro
	pgyo_cnt++;
	if(pgyo_cnt == 1)
	{
		if(pParam->initial_con == 0)
			quatern_equal(pgyro_qvec_6d_pre, pPost->q_pgyo_6d);
		else
			quatern_equal(pgyro_qvec_6d_pre, pgyro_qvec_6d_post);
	}
	if(pgyo_cnt == 7)
	{
        //+SHAWN20150522
//			quatern_equal(pgyro_qvec_6d_post, pPost->q_pgyo_6d);
        quatern_equal(pgyro_qvec_6d_post, pPost->q_pgyo_9d);
        //-SHAWN20150522

		pgyro_n_valid = gyro_data_from_6d(gyro_data_6d, &pgyro_skip_delta_t, pgyro_qvec_6d_pre, pgyro_qvec_6d_post);
		pgyo_cnt = 0;

		if(pgyro_n_valid)
		{
			gyro_data_6d[0] = gyro_data_6d_check[0];
			gyro_data_6d[1] = gyro_data_6d_check[1];
			gyro_data_6d[2] = gyro_data_6d_check[2];
		}
		
		gyro_data_6d_check[0] = gyro_data_6d[0];
		gyro_data_6d_check[1] = gyro_data_6d[1];
		gyro_data_6d_check[2] = gyro_data_6d[2];

		// FIR OUT
		// Psuedo-Gyro X Part
		ptrData32 = (LPUINT32) &pgyo_x_rdata;
		*(ptrData32 + pgyo_st_pt) = gyro_data_6d[0];
		gyro_data_6d[0] = fir_filter(PGYRO_FIR_TAP, (pgyo_st_pt + 1) % PGYRO_FIR_TAP, (LPSINT16)lp_coeff_pgyo, (LPUINT32)&pgyo_x_rdata);
		shift_in = gyro_data_6d[0];
		rshift_func(&shift_out, &shift_in, shift_bit);
		gyro_data_6d[0] = shift_out;
		
		// Psuedo-Gyro Y Part
		ptrData32 = (LPUINT32) &pgyo_y_rdata;
		*(ptrData32 + pgyo_st_pt) = gyro_data_6d[1];
		gyro_data_6d[1] = fir_filter(PGYRO_FIR_TAP, (pgyo_st_pt + 1) % PGYRO_FIR_TAP, (LPSINT16)lp_coeff_pgyo, (LPUINT32)&pgyo_y_rdata);
		shift_in = gyro_data_6d[1];
		rshift_func(&shift_out, &shift_in, shift_bit);
		gyro_data_6d[1] = shift_out;
		
		// Psuedo-Gyro Z Part
		ptrData32 = (LPUINT32) &pgyo_z_rdata;
		*(ptrData32 + pgyo_st_pt) = gyro_data_6d[2];
		gyro_data_6d[2] = fir_filter(PGYRO_FIR_TAP, (pgyo_st_pt + 1) % PGYRO_FIR_TAP, (LPSINT16)lp_coeff_pgyo, (LPUINT32)&pgyo_z_rdata);
		shift_in = gyro_data_6d[2];
		rshift_func(&shift_out, &shift_in, shift_bit);
		gyro_data_6d[2] = shift_out;
		
		pgyo_st_pt = (pgyo_st_pt + 1) % PGYRO_FIR_TAP;

		#if 0 // Static condition(For AP Test)
		if((gyro_data_6d_check[0] == 0) && (gyro_data_6d_check[1] == 0) && (gyro_data_6d_check[2] == 0))
		{
			gyro_data_6d[0] = 0;
			gyro_data_6d[1] = 0;
			gyro_data_6d[2] = 0;
		}
		#endif
	}

	// FIR OUT
	// Psuedo-Gyro X Part
	ptrData32 = (LPUINT32) &pgyo_x_rdata_skip;
	*(ptrData32 + pgyo_st_pt_skip) = gyro_data_6d[0];
	pPost->pgyro_data[0] = fir_filter(PGYRO_FIR_TAP_SKIP, (pgyo_st_pt_skip + 1) % PGYRO_FIR_TAP_SKIP, (LPSINT16)lp_coeff_pgyo_skip, (LPUINT32)&pgyo_x_rdata_skip);
	shift_in = pPost->pgyro_data[0];
	rshift_func(&shift_out, &shift_in, shift_bit);
	pPost->pgyro_data[0] = shift_out;
		
	// Psuedo-Gyro Y Part
	ptrData32 = (LPUINT32) &pgyo_y_rdata_skip;
	*(ptrData32 + pgyo_st_pt_skip) = gyro_data_6d[1];
	pPost->pgyro_data[1] = fir_filter(PGYRO_FIR_TAP_SKIP, (pgyo_st_pt_skip + 1) % PGYRO_FIR_TAP_SKIP, (LPSINT16)lp_coeff_pgyo_skip, (LPUINT32)&pgyo_y_rdata_skip);
	shift_in = pPost->pgyro_data[1];
	rshift_func(&shift_out, &shift_in, shift_bit);
	pPost->pgyro_data[1] = shift_out;
	
	// Psuedo-Gyro Z Part
	ptrData32 = (LPUINT32) &pgyo_z_rdata_skip;
	*(ptrData32 + pgyo_st_pt_skip) = gyro_data_6d[2];
	pPost->pgyro_data[2] = fir_filter(PGYRO_FIR_TAP_SKIP, (pgyo_st_pt_skip + 1) % PGYRO_FIR_TAP_SKIP, (LPSINT16)lp_coeff_pgyo_skip, (LPUINT32)&pgyo_z_rdata_skip);
	shift_in = pPost->pgyro_data[2];
	rshift_func(&shift_out, &shift_in, shift_bit);
	pPost->pgyro_data[2] = shift_out;
	
	pgyo_st_pt_skip = (pgyo_st_pt_skip + 1) % PGYRO_FIR_TAP_SKIP;

	// Fusion With Pseudo-Gyro
	fusion_acc_mag_pgyro_6d(pPost->q_pgyo_9d, &pParam->initial_con, &acc_infr,
	                        &pParam->mag_infr_flag, gyro_data_6d,
	                        &pParam->delta_t, pPost->q_pgyo_6d, pPre->q_pgyo_9d);

	// Quaternion
	quatern_equal(pPost->q_pgyo_6d, pPost->q_pgyo_9d);

	// Quaternion --> Rotation Matrix
	quatern2matrix(pPost->q2dcm, pPost->q_pgyo_6d);

	// Quaternion --> Euler Angle(6D)
	quatern2angle(pPost->ea, pPre->ea, pPost->q_pgyo_6d);

	// Heading
	pPost->heading = compass_heading(pPost->q2dcm);

	if(gpFConfig->Enable_Android)
	{
		// Android System
		*(pPost->qest_android + 0) = *(pPost->q_pgyo_6d + 0);
		*(pPost->qest_android + 1) = -(*(pPost->q_pgyo_6d + 1));
		*(pPost->qest_android + 2) = -(*(pPost->q_pgyo_6d + 2));
		*(pPost->qest_android + 3) = -(*(pPost->q_pgyo_6d + 3));

		quatern2angle(pPost->ea_android, pPre->ea_android, pPost->qest_android);
	}
	return;
}

// **************************************************************************
//  9D Fusion (A+M+G)
// **************************************************************************
void SENSOR_CODE fusion_calc_9d(UINT8 frame_data[], UINT16 freq)
{
	P_SENSOR_BUFFER_WIN pDC = (P_SENSOR_BUFFER_WIN)frame_data;
	P_SENSOR_BUFFER_ANDROID pADC = (P_SENSOR_BUFFER_ANDROID)frame_data;
	SINT32 fir_result_array[9];
	SINT32 mag_raw_data[3];
	UINT32 *ptrData32;
	UINT8 gyo_cal_succ;
    UINT8 shake_status;
    SINT32 acc_linear[3] = {0, 0, 0};
	
	// For Shake Detect
	acc_data[0] = (((SINT16)(pDC->Acc[0])) << 8);
	acc_data[1] = (((SINT16)(pDC->Acc[1])) << 8);
	acc_data[2] = (((SINT16)(pDC->Acc[2])) << 8);
	do_unit_correct_acc(acc_data, acc_raw_data);
	fusion_axis_switch(acc_raw_data, acc_axis_mode);

	// accelerometer x part
	ptrData32 = (UINT32*) &acc_x_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Acc[0]);
	fir_result_array[0] = fir_filter(FIR_TAP_ACC_GYO_9D, (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D, (LPSINT16)lp_coeff_acc_9d, (UINT32*)&acc_x_rdata);
	
	// accelerometer y part
	ptrData32 = (UINT32*) &acc_y_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Acc[1]);
	fir_result_array[1] = fir_filter(FIR_TAP_ACC_GYO_9D, (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D, (LPSINT16)lp_coeff_acc_9d, (UINT32*)&acc_y_rdata);

	// accelerometer z part
	ptrData32 = (UINT32*) &acc_z_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Acc[2]);
	fir_result_array[2] = fir_filter(FIR_TAP_ACC_GYO_9D, (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D, (LPSINT16)lp_coeff_acc_9d, (UINT32*)&acc_z_rdata);

	// magnetometer x part
	ptrData32 = (UINT32*) &mag_x_rdata;
	*(ptrData32 + st_pt_mag_9d) = (SINT16)(pDC->Mag[0]);
	fir_result_array[3] = fir_filter(FIR_TAP_MAG_GYO_9D, (st_pt_mag_9d + 1) % FIR_TAP_MAG_GYO_9D, (LPSINT16)lp_coeff_mag_9d, (UINT32*)&mag_x_rdata);

	// magnetometer y part
	ptrData32 = (UINT32*) &mag_y_rdata;
	*(ptrData32 + st_pt_mag_9d) = (SINT16)(pDC->Mag[1]);
	fir_result_array[4] = fir_filter(FIR_TAP_MAG_GYO_9D, (st_pt_mag_9d + 1) % FIR_TAP_MAG_GYO_9D, (LPSINT16)lp_coeff_mag_9d, (UINT32*)&mag_y_rdata);

	// magnetometer z part
	ptrData32 = (UINT32*) &mag_z_rdata;
	*(ptrData32 + st_pt_mag_9d) = (SINT16)(pDC->Mag[2]);
	fir_result_array[5] = fir_filter(FIR_TAP_MAG_GYO_9D, (st_pt_mag_9d + 1) % FIR_TAP_MAG_GYO_9D, (LPSINT16)lp_coeff_mag_9d, (UINT32*)&mag_z_rdata);

	// gyroscope x part
	ptrData32 = (UINT32*) &gyo_x_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Gyo[0]);
	fir_result_array[6] = *(ptrData32 + (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D);

	// gyroscope y part
	ptrData32 = (UINT32*) &gyo_y_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Gyo[1]);
	fir_result_array[7] = *(ptrData32 + (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D);

	// gyroscope z part
	ptrData32 = (UINT32*) &gyo_z_rdata;
	*(ptrData32 + st_pt_acc_9d) = (SINT16)(pDC->Gyo[2]);
	fir_result_array[8] = *(ptrData32 + (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D);

	//RTL_PRINTF("IACC x,y,z = %X:%X:%X.\r\n", fir_result_array[0], fir_result_array[1], fir_result_array[2]);
	//RTL_PRINTF("IMAG x,y,z = %X:%X:%X.\r\n", fir_result_array[3], fir_result_array[4], fir_result_array[5]);
	//RTL_PRINTF("IGYR x,y,z = %X:%X:%X.\r\n", fir_result_array[6], fir_result_array[7], fir_result_array[8]);

	do_unit_correct_acc(&fir_result_array[0], acc_data);
	acc_data[0] >>= 7;
	acc_data[1] >>= 7;
	acc_data[2] >>= 7;
	fusion_axis_switch(acc_data, acc_axis_mode);

	do_unit_correct_mag(&fir_result_array[3], mag_data);
	mag_data[0] >>= 7;
	mag_data[1] >>= 7;
	mag_data[2] >>= 7;

	mag_raw_data[0] = mag_data[0];
	mag_raw_data[1] = mag_data[1];
	mag_raw_data[2] = mag_data[2];
	fusion_axis_switch(mag_raw_data, mag_axis_mode);

	do_unit_correct_gyo(&fir_result_array[6], gyro_data);
	fusion_axis_switch(gyro_data, gyro_axis_mode);

	//RTL_PRINTF("ACC x,y,z = %d:%d:%d.\r\n", acc_data[0], acc_data[1], acc_data[2]);
	//RTL_PRINTF("MAG x,y,z = %d:%d:%d.\r\n", mag_data[0], mag_data[1], mag_data[2]);
	//RTL_PRINTF("GYR x,y,z = %d:%d:%d.\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
	
	// ----------------------------------
	// Timing Infor
	// ----------------------------------
	mSenFuParam.delta_t = 300 / freq;		
	gyro_bias_ctrl[1] = (freq << 1);
	gyro_bias_ctrl[2] = mSenFuParam.delta_t;
	linr_acc_num = (freq >> 3);
		
	// ----------------------------------
	// Acc Magnitude Detector Function
	// ----------------------------------
	acc_calibrate(acc_data, mSenFuParam.acc_bias, acc_gain);
	acc_meanvar(&acc_data[0]);
	
	// ----------------------------------
	// Gyro Bias Function
	// ----------------------------------
	if(gyro_cal_en == 1)
		gyo_cal_succ = gyro_bias_from_rawdata(&mSenFuParam.gyr_bias[0], &gyro_data[0], &gyro_bias_sum[0], &gyro_bias_ctrl[0]);
	
	if(rdata_chag_flag == 1)
		gyo_stop_cnt = 0;
	else if((gyo_cal_succ == 1) && (gyo_stop_cnt >= 0))
		gyo_stop_cnt++;
	else if(gyo_stop_cnt == 2)
	{
		gyo_stop_cnt = -1;
		gyro_cal_en = 0;
	}
//	RTL_PRINTF("cnt:(%d, %d).\r\n",gyo_stop_cnt, gyro_bias_ctrl[0]);

	// Raw data Change Flag
	rdata_chag_flag = rdata_chag_prtg(&ardata_chag_value[0], &acc_data[0]);

	if((rdata_chag_flag == 1) && (gyo_stop_cnt == -1))
	{
		gyo_stop_cnt = 0;
		gyro_cal_en = 1;
	}
	
	// Fusion Calculate Flag
	if(gyro_bias_ctrl[0] == freq)
	{
		fusion_cal_en = 0;
	}
	else if(rdata_chag_flag == 1)
	{
		fusion_cal_en = 1;
	}
	
	// ----------------------------------
	// Compass Calibration Function
	// ----------------------------------
	if((fusion_cal_en == 1) || (mag_op_mode == 1))
	{
		do_rt_calibrate_mag(mag_data, mSenFuParam.mag_bias, &mSenFuParam.mag_infr_flag, freq);
	}
	else
		mSenFuParam.mag_infr_flag = 0;
	//RTL_PRINTF("%d:%d:%d.\r\n", mSenFuParam.mag_bias[0], mSenFuParam.mag_bias[1], mSenFuParam.mag_bias[2]);

	// -
	fusion_axis_switch(mag_data, mag_axis_mode);

	// ----------------------------------
	// call shake detection
	// ----------------------------------
	//RTL_PRINTF("ACC x,y,z = %d:%d:%d.\r\n\n\n", acc_raw_data[0], acc_raw_data[1], acc_raw_data[2]);
	//RTL_PRINTF("ACC shake_en_thr, shake_model_tap = %d:%d.\r\n", shake_en_thr, shake_model_tap);

#ifdef __SHAKE_FUNCTION_SIMPLE__ //{
	shake_detect_simple(acc_raw_data, &shake_status);
#else
	shake_detect(
	    // Input Data
	    acc_raw_data, acc_raw_x_vec_pre, acc_raw_y_vec_pre, acc_raw_z_vec_pre,

	    // Output Data
	    acc_raw_x_vec_post, acc_raw_y_vec_post, acc_raw_z_vec_post, &shake_status);
	for(shake_i = 0; shake_i < SHAKE_MODEL_TAP; shake_i++)
	{
		acc_raw_x_vec_pre[shake_i] = acc_raw_x_vec_post[shake_i];
		acc_raw_y_vec_pre[shake_i] = acc_raw_y_vec_post[shake_i];
		acc_raw_z_vec_pre[shake_i] = acc_raw_z_vec_post[shake_i];
	}
	/*
	if(shake_status != 0)
	{
		RTL_PRINTF("SHAKE: shake_status = %d.\r\n", shake_status);
	}
	*/	
#endif //}__SHAKE_FUNCTION_SIMPLE__

	// ----------------------------------
	// call 9D sensor fusion
	// ----------------------------------
	if(fusion_cal_en == 1)
	{
		fusion_function_9d(&mSenFuParam, acc_data, mag_data, gyro_data, &mSFInfoPre, &mSFInfoPost);
	}

	mSenFuParam.initial_con = 1;
	memcpy((char*)&mSFInfoPre, (char*)&mSFInfoPost, sizeof(TSenFusInfo));

	// ----------------------------------
	// reorder output data
	// ----------------------------------
	// Raw Data With Drift-Free
	gyro_data[0] = gyro_data[0] - mSenFuParam.gyr_bias[0];
	gyro_data[1] = gyro_data[1] - mSenFuParam.gyr_bias[1];
	gyro_data[2] = gyro_data[2] - mSenFuParam.gyr_bias[2];

	if(gpFConfig->Enable_Android)
	{
        // Accelerometer = Gravity + Linear Acceleration (Android Define)
        acc_calibrate(acc_raw_data, mSenFuParam.acc_bias, acc_gain);
        
        gravity[0] = ((LINR_ALPHA*gravity[0]) + ((256-LINR_ALPHA)*acc_raw_data[0])) >> 8;
  		gravity[1] = ((LINR_ALPHA*gravity[1]) + ((256-LINR_ALPHA)*acc_raw_data[1])) >> 8;
  		gravity[2] = ((LINR_ALPHA*gravity[2]) + ((256-LINR_ALPHA)*acc_raw_data[2])) >> 8;
  		acc_linear[0] = acc_raw_data[0] - gravity[0];
  		acc_linear[1] = acc_raw_data[1] - gravity[1];
  		acc_linear[2] = acc_raw_data[2] - gravity[2];
  		
		fusion_data_copy(pADC->Acc     , acc_data                , 3);
		fusion_data_copy(pADC->Mag     , mag_raw_data            , 3);
		fusion_data_copy(pADC->Gyo     , gyro_data  			 , 3);

		fusion_data_copy(pADC->AccMeter, acc_raw_data            , 3);
		fusion_data_copy(pADC->LinAcc  , acc_linear              , 3);

        if(fusion_cal_en == 1)
        {
		    fusion_data_copy(pADC->MagCal  , mag_data                , 3);
    		fusion_data_copy(pADC->Eular   , mSFInfoPost.ea_android  , 3);
    		fusion_data_copy(pADC->Quat    , mSFInfoPost.qest_android, 4);
        }        
	}
	else
	{
		fusion_data_copy(pDC->Acc    , acc_data          , 3);
		fusion_data_copy(pDC->Mag    , mag_raw_data      , 3);
		fusion_data_copy(pDC->Gyo    , gyro_data         , 3);

        if(fusion_cal_en == 1)
        {
		    fusion_data_copy(pDC->MagCal , mag_data          , 3);
    		fusion_data_copy(pDC->Eular  , mSFInfoPost.ea    , 3);
    		fusion_data_copy(pDC->Quat   , mSFInfoPost.qest9d, 4);
    		fusion_data_copy(pDC->RMatrix, mSFInfoPost.q2dcm , 9);
    		pDC->Heading = (UINT16)(mSFInfoPost.heading);
        }
        
		pDC->Shake = shake_status;
	}

	// start point inc 1
	st_pt_acc_9d = (st_pt_acc_9d + 1) % FIR_TAP_ACC_GYO_9D;
	st_pt_mag_9d = (st_pt_mag_9d + 1) % FIR_TAP_MAG_GYO_9D;
}

// **************************************************************************
//  6D Fusion (A+M, soft-gyro)
// **************************************************************************
void SENSOR_CODE fusion_calc_6d(UINT8 frame_data[], UINT16 freq)
{
	P_SENSOR_BUFFER_WIN pDC = (P_SENSOR_BUFFER_WIN)frame_data;
	P_SENSOR_BUFFER_ANDROID pADC = (P_SENSOR_BUFFER_ANDROID)frame_data;
    #ifdef __PSEUDO_GYRO_DBG__ // {
    SINT32 fir_result_array[9];
    #else
	SINT32 fir_result_array[6];
    #endif // } __PSEUDO_GYRO_DBG__
	SINT32 mag_raw_data[3];
	UINT8 rdata_chag_flag_m;
	UINT32 *ptrData32;
    UINT8 shake_status;
    SINT32 acc_linear[3] = {0, 0, 0};
    
	// For Shake Detect
	acc_data[0] = (((SINT16)(pDC->Acc[0])) << 8);
	acc_data[1] = (((SINT16)(pDC->Acc[1])) << 8);
	acc_data[2] = (((SINT16)(pDC->Acc[2])) << 8);
	do_unit_correct_acc(acc_data, acc_raw_data);
	fusion_axis_switch(acc_raw_data, acc_axis_mode);

	// accelerometer x part
	ptrData32 = (UINT32*) &acc_x_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Acc[0]);
	fir_result_array[0] = fir_filter(FIR_TAP_ACC_PGYO_6D, (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D, (LPSINT16)lp_coeff_acc_pgyo_6d, (UINT32*)&acc_x_rdata);

	// accelerometer y part
	ptrData32 = (UINT32*) &acc_y_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Acc[1]);
	fir_result_array[1] = fir_filter(FIR_TAP_ACC_PGYO_6D, (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D, (LPSINT16)lp_coeff_acc_pgyo_6d, (UINT32*)&acc_y_rdata);

	// accelerometer z part
	ptrData32 = (UINT32*) &acc_z_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Acc[2]);
	fir_result_array[2] = fir_filter(FIR_TAP_ACC_PGYO_6D, (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D, (LPSINT16)lp_coeff_acc_pgyo_6d, (UINT32*)&acc_z_rdata);

	// magnetometer x part
	ptrData32 = (UINT32*) &mag_x_rdata;
	*(ptrData32 + st_pt_mag_6d) = (SINT16)(pDC->Mag[0]);
	fir_result_array[3] = fir_filter(FIR_TAP_MAG_PGYO_6D, (st_pt_mag_6d + 1) % FIR_TAP_MAG_PGYO_6D, (LPSINT16)lp_coeff_mag_pgyo_6d, (UINT32*)&mag_x_rdata);

	// magnetometer y part
	ptrData32 = (UINT32*) &mag_y_rdata;
	*(ptrData32 + st_pt_mag_6d) = (SINT16)(pDC->Mag[1]);
	fir_result_array[4] = fir_filter(FIR_TAP_MAG_PGYO_6D, (st_pt_mag_6d + 1) % FIR_TAP_MAG_PGYO_6D, (LPSINT16)lp_coeff_mag_pgyo_6d, (UINT32*)&mag_y_rdata);

	// magnetometer z part
	ptrData32 = (UINT32*) &mag_z_rdata;
	*(ptrData32 + st_pt_mag_6d) = (SINT16)(pDC->Mag[2]);
	fir_result_array[5] = fir_filter(FIR_TAP_MAG_PGYO_6D, (st_pt_mag_6d + 1) % FIR_TAP_MAG_PGYO_6D, (LPSINT16)lp_coeff_mag_pgyo_6d, (UINT32*)&mag_z_rdata);

	//RTL_PRINTF("IACC x,y,z = %X:%X:%X.\r\n", fir_result_array[0], fir_result_array[1], fir_result_array[2]);
	//RTL_PRINTF("IMAG x,y,z = %X:%X:%X.\r\n", fir_result_array[3], fir_result_array[4], fir_result_array[5]);

    #ifdef __PSEUDO_GYRO_DBG__ // {
	// gyroscope x part
	ptrData32 = (UINT32*) &gyo_x_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Gyo[0]);
	fir_result_array[6] = *(ptrData32 + (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D);

	// gyroscope y part
	ptrData32 = (UINT32*) &gyo_y_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Gyo[1]);
	fir_result_array[7] = *(ptrData32 + (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D);

	// gyroscope z part
	ptrData32 = (UINT32*) &gyo_z_rdata;
	*(ptrData32 + st_pt_acc_6d) = (SINT16)(pDC->Gyo[2]);
	fir_result_array[8] = *(ptrData32 + (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D);

	do_unit_correct_gyo(&fir_result_array[6], gyro_data);
	fusion_axis_switch(gyro_data, gyro_axis_mode);

    fusion_data_copy((UINT16*)(pDC->Alt+10), gyro_data         , 3);
    #endif // } __PSEUDO_GYRO_DBG__

	do_unit_correct_acc(&fir_result_array[0], acc_data);
	acc_data[0] >>= 7;
	acc_data[1] >>= 7;
	acc_data[2] >>= 7;
	fusion_axis_switch(acc_data, acc_axis_mode);

	do_unit_correct_mag(&fir_result_array[3], mag_data);
	mag_data[0] >>= 7;
	mag_data[1] >>= 7;
	mag_data[2] >>= 7;

	mag_raw_data[0] = mag_data[0];
	mag_raw_data[1] = mag_data[1];
	mag_raw_data[2] = mag_data[2];
	fusion_axis_switch(mag_raw_data, mag_axis_mode);

	//RTL_PRINTF("ACC x,y,z = %d:%d:%d.\r\n", acc_data[0], acc_data[1], acc_data[2]);
	//RTL_PRINTF("MAG x,y,z = %d:%d:%d.\r\n", mag_data[0], mag_data[1], mag_data[2]);

	// Raw Data With Drift-Free
	acc_calibrate(acc_data, mSenFuParam.acc_bias, acc_gain);    

	// ----------------------------------
	// Power Control Function Based on Acc Data
	// ----------------------------------
	// Raw data Change Flag
	rdata_chag_flag = rdata_chag_prtg(&ardata_chag_value[0], &acc_data[0]);
	rdata_chag_flag_m = rdata_chag_prtg(&mrdata_chag_value[0], &mag_data[0]);

	if(((rdata_chag_flag == 0) && (rdata_chag_flag_m == 0)) && (motion_flag == 1) && (motion_cnt < (freq << 2)))
		motion_cnt = motion_cnt + 1;
	else
		motion_cnt = 0;
		
	// Motion Detect
	if(motion_cnt == (freq << 2))
	{
		motion_flag = 0;
		//RTL_PRINTF("------- STOP -------.\r\n");
	}

	if(((rdata_chag_flag == 1) || (rdata_chag_flag_m == 1)) && (motion_flag == 0))
	{
		motion_flag = 1;
		//RTL_PRINTF("------- MOTION -------.\r\n");
	}

	// ----------------------------------
	// Compass Calibration Function
	// ----------------------------------
	if((motion_flag == 1) || (mag_op_mode == 1))
		do_rt_calibrate_mag(mag_data, mSenFuParam.mag_bias, &mSenFuParam.mag_infr_flag, freq);
	else
		mSenFuParam.mag_infr_flag = 0;

	// -
	fusion_axis_switch(mag_data, mag_axis_mode);

	// ----------------------------------
	// call shake detection
	// ----------------------------------
	//RTL_PRINTF("ACC x,y,z = %d:%d:%d.\r\n\n\n", acc_raw_data[0], acc_raw_data[1], acc_raw_data[2]);
	//RTL_PRINTF("ACC shake_en_thr, shake_model_tap = %d:%d.\r\n", shake_en_thr, shake_model_tap);

#ifdef __SHAKE_FUNCTION_SIMPLE__ //{
	shake_detect_simple(acc_raw_data, &shake_status);
#else
	shake_detect(
	    // Input Data
	    acc_raw_data, acc_raw_x_vec_pre, acc_raw_y_vec_pre, acc_raw_z_vec_pre,

	    // Output Data
	    acc_raw_x_vec_post, acc_raw_y_vec_post, acc_raw_z_vec_post, &shake_status);
	for(shake_i = 0; shake_i < SHAKE_MODEL_TAP; shake_i++)
	{
		acc_raw_x_vec_pre[shake_i] = acc_raw_x_vec_post[shake_i];
		acc_raw_y_vec_pre[shake_i] = acc_raw_y_vec_post[shake_i];
		acc_raw_z_vec_pre[shake_i] = acc_raw_z_vec_post[shake_i];
	}

	//RTL_PRINTF("SHAKE: shake_status = %d.\r\n", shake_status);
#endif //}__SHAKE_FUNCTION_SIMPLE__

	// ----------------------------------
	// call 6D sensor fusion
	// ----------------------------------
	//pgyro_skip_delta_t = ((13 * 100) / freq);
	pgyro_skip_delta_t = ((1536) / freq);
			
	if((motion_flag == 1) && (mSenFuParam.mag_infr_flag == 0))
	{
		fusion_function_pgyo_6d(&mSenFuParam, acc_data, mag_data, &mSFInfoPre, &mSFInfoPost);
	}

	if((mSenFuParam.mag_infr_flag == 1) || (motion_flag == 0))
	{
		mSFInfoPost.pgyro_data[0] = 0;
		mSFInfoPost.pgyro_data[1] = 0;
		mSFInfoPost.pgyro_data[2] = 0;
	}

	mSenFuParam.initial_con = 1;
	memcpy((char*)&mSFInfoPre, (char*)&mSFInfoPost, sizeof(TSenFusInfo));

	// ----------------------------------
	// reorder output data
	// ----------------------------------
	if(gpFConfig->Enable_Android)
	{
    	// Accelerometer = Gravity + Linear Acceleration (Android Define)
		acc_calibrate(acc_raw_data, mSenFuParam.acc_bias, acc_gain);

		gravity[0] = ((LINR_ALPHA*gravity[0]) + ((256-LINR_ALPHA)*acc_raw_data[0])) >> 8;
  		gravity[1] = ((LINR_ALPHA*gravity[1]) + ((256-LINR_ALPHA)*acc_raw_data[1])) >> 8;
  		gravity[2] = ((LINR_ALPHA*gravity[2]) + ((256-LINR_ALPHA)*acc_raw_data[2])) >> 8;
  		acc_linear[0] = acc_raw_data[0] - gravity[0];
  		acc_linear[1] = acc_raw_data[1] - gravity[1];
  		acc_linear[2] = acc_raw_data[2] - gravity[2];
        
		fusion_data_copy(pADC->Acc     , acc_data                , 3);
		fusion_data_copy(pADC->Mag     , mag_raw_data            , 3);
        fusion_data_copy(pADC->Gyo     , mSFInfoPost.pgyro_data  , 3);

		fusion_data_copy(pADC->AccMeter, acc_raw_data            , 3);
		fusion_data_copy(pADC->LinAcc  , acc_linear              , 3);
        
        if(motion_flag == 1)
        {
       		fusion_data_copy(pADC->MagCal  , mag_data                , 3);
      		fusion_data_copy(pADC->Eular   , mSFInfoPost.ea_android  , 3);
    		fusion_data_copy(pADC->Quat    , mSFInfoPost.qest_android, 4);
        }
	}
	else
	{
		fusion_data_copy(pDC->Acc    , acc_data              , 3);
		fusion_data_copy(pDC->Mag    , mag_raw_data          , 3);
		fusion_data_copy(pDC->Gyo    , mSFInfoPost.pgyro_data, 3);

        if(motion_flag == 1)
        {
    		fusion_data_copy(pDC->MagCal , mag_data              , 3);
    		fusion_data_copy(pDC->Eular  , mSFInfoPost.ea        , 3);
    		fusion_data_copy(pDC->Quat   , mSFInfoPost.q_pgyo_6d , 4);
    		fusion_data_copy(pDC->RMatrix, mSFInfoPost.q2dcm     , 9);
    		pDC->Heading = (UINT16)(mSFInfoPost.heading);
        }
        
		pDC->Shake = shake_status;
	}

	// start point inc 1
	st_pt_acc_6d = (st_pt_acc_6d + 1) % FIR_TAP_ACC_PGYO_6D;
	st_pt_mag_6d = (st_pt_mag_6d + 1) % FIR_TAP_MAG_PGYO_6D;
}

void SENSOR_CODE fusion_calc(UINT8 frame_data[], UINT16 freq)
{
	if(gpFConfig)
	{
		if(gpFConfig->Enable_6D)
			fusion_calc_6d(frame_data, freq);
		else
			fusion_calc_9d(frame_data, freq);
	}
}

UINT32 SENSOR_CODE_L fusion_init(P_FUSION_CONFIG pFConfig, SINT16 pAccBiasBuffer[3], SINT16 pGyroBiasBuffer[3], UINT8 pAxisMode[3])
{
	UINT32 lib_info = FUSION_VER_NUM;

	gpFConfig = pFConfig;

	if(gpFConfig)
	{
        mSenFuParam.initial_con = 0;
            
		mSenFuParam.acc_bias = pAccBiasBuffer;
		mSenFuParam.gyr_bias = pGyroBiasBuffer;

		p_axis_mode = pAxisMode;

		if(gpFConfig->Enable_Android)
        {
            quatern2angle = qua2ang_xyz;
            //angle2matrix = ang2mat_xyz;
            angle2quatern = ang2qua_xyz;
            mag_horizontal_func = mag_horiz_xyz;            
            roll_pitch_func = roll_pitch_xyz;  
            
			lib_info |= 0x000000FA;
		}
		else
        {
            quatern2angle = qua2ang_yxz;
            //angle2matrix = ang2mat_yxz;
            angle2quatern = ang2qua_yxz;
            mag_horizontal_func = mag_horiz_yxz;            
            roll_pitch_func = roll_pitch_yxz;            
            
			lib_info |= 0x000000F0;
		}

		if(gpFConfig->Enable_6D)
		{
            compass_cal_flag = 1;
            
			lib_info |= 0x00006D00;
		}
		else
		{
			lib_info |= 0x00009D00;
		}
	}

	return lib_info;
}

BOOL SENSOR_CODE_L fusion_update_gyro_bias(UINT8 *pRawData)
{
	if(gpFConfig)
	{
		if(gpFConfig->Enable_6D == 0)
		{
			gyro_data[0] = (*((LPSINT16)(pRawData + 0)));
			gyro_data[1] = (*((LPSINT16)(pRawData + 2)));
			gyro_data[2] = (*((LPSINT16)(pRawData + 4)));
            
			do_unit_correct_gyo(gyro_data, gyro_data);
			fusion_axis_switch(gyro_data, gyro_axis_mode);

            gyro_bias_from_rawdata(&mSenFuParam.gyr_bias[0], &gyro_data[0], &gyro_bias_sum[0], &gyro_bias_ctrl[0]);
			
			return gyro_cal_done_flag;
		}
	}

	return TRUE;
}
#endif // } __ENABLE_FUSION_API__


#include "fusion_common.h"

#if __ENABLE_FUSION_API__ // {
// **************************************************************************
#define MAX_FORMATION_NUM  2

typedef struct _T_FORMATION_DATA_
{
	SINT16 SoftHardIronOffset[3];
	SINT16 SoftIronMatrix[9];
}T_FORMATION_DATA, *P_FORMATION_DATA;

typedef struct _T_CAL_STORED_DATA_
{
	UINT16 ValidFlag;
	UINT16 TotalSize;
    T_FORMATION_DATA FData[MAX_FORMATION_NUM];
    SINT16 AvgLength[MAX_FORMATION_NUM];
} T_CAL_STORED_DATA, *P_CAL_STORED_DATA;

#define IS_CAL_DATA_VALID(p)    (((p)->ValidFlag == 0x8380) && ((p)->TotalSize == sizeof(T_CAL_STORED_DATA)))
#define IS_MATRIX_VALID(p)      ((((p)->ValidFlag) & 0xFF00) == 0x8300)

#define SET_CAL_DATA_VALID(p)                       \
    do{                                             \
        (p)->ValidFlag = 0x8380;                    \
        (p)->TotalSize = sizeof(T_CAL_STORED_DATA); \
    }while(0)

#define SET_MATRIX_VALID(p)                         \
    do{                                             \
        (p)->ValidFlag = 0x8300;                    \
    }while(0)    
    
#define RESET_CAL_DATA(p, i)       \
    do{                            \
        (p)->ValidFlag &= ~0x00FF; \
        (p)->TotalSize = 0;        \
        memset(((p)->FData[i].SoftHardIronOffset), 0x00, (UINT32)(((P_FORMATION_DATA)0)->SoftIronMatrix)); \
        (p)->AvgLength[i] = 0;     \
    }while(0)

#define SET_DEFAULT_MATRIX(p) \
    do{                         \
		memcpy((LPUINT8)((p)->FData[0].SoftIronMatrix), (LPUINT8)DefaultSoftIronMatrix, sizeof(DefaultSoftIronMatrix)); \
		memcpy((LPUINT8)((p)->FData[1].SoftIronMatrix), (LPUINT8)DefaultSoftIronMatrix, sizeof(DefaultSoftIronMatrix)); \
    }while(0)
    
// **************************************************************************
// For Compass Calibration
//	const SINT16 SENSOR_RODATA DefaultSoftIronMatrix[9] = {672, -2, 6, -2, 686, 1, 6, 1, 703}; //Bosch
//	const SINT16 SENSOR_RODATA DefaultSoftIronMatrix[9] = {732, -10, 4, -10, 733, 39, 4, 39, 747}; //AKM
const SINT16 SENSOR_RODATA DefaultSoftIronMatrix[9] = {256, 0, 0, 0, 256, 0, 0, 0, 256};

//(whole_num/partial_num/skip_num/0/0)
SENSOR_BSS static TCmpCalInf mag_cal_pre;
SENSOR_BSS static TCmpCalInf mag_cal_post;

// Calibration Control Information
SENSOR_DATA static UINT8 skip_num = CMPCAL_SKIP_NUM;
SENSOR_DATA static UINT8 whole_cnt = 0, sys_cnt = 0;

SENSOR_DATA static UINT8 mag_fdata_idx = 0;

SENSOR_DATA static P_CAL_STORED_DATA pCalData = NULL;

SENSOR_DATA static UINT8 u8MagInfrFlag = 0;
SENSOR_DATA static BOOL bCalibrationStateReset = FALSE;

#define soft_hard_iron_offset  (pCalData->FData[mag_fdata_idx].SoftHardIronOffset)
#define soft_iron_matrix       (pCalData->FData[mag_fdata_idx].SoftIronMatrix)
#define mag_length_avg         (pCalData->AvgLength[mag_fdata_idx])

SENSOR_DATA static UINT8 compass_infr_cnt = 0;
SENSOR_DATA static UINT8 mag_HID_cal_flag = 0;
SENSOR_DATA static UINT8 mag_cal_success = 0;

// **************************************************************************

void SENSOR_CODE skip_num_update(UINT16 u16Freq)
{
    skip_num = u16Freq >> 2;
}

//------------------------------------
//  Det Matrix Function 3x3 (mul:0 , div:0)
//------------------------------------
SINT32 SENSOR_CODE det_3x3_func(LPSINT32 matrix_in)
{

	//
	SINT32 shift_in, shift_out;
	SINT32 det_matrix_value, det_tmp_1, det_tmp_2, det_tmp_3, det_tmp_4, det_tmp_5, det_tmp_6;

	shift_in = (*(matrix_in + 0)) * (*(matrix_in + 4));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_1 = shift_out * (*(matrix_in + 8));
	//RTL_PRINTF ("det_tmp_1 = %d, (%d:%d:%d).\r\n", det_tmp_1, *(matrix_in+0), *(matrix_in+4), *(matrix_in+8));

	shift_in = (*(matrix_in + 3)) * (*(matrix_in + 7));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_2 = shift_out * (*(matrix_in + 2));
	//RTL_PRINTF ("det_tmp_2 = %d, (%d:%d:%d).\r\n", det_tmp_2, *(matrix_in+3), *(matrix_in+7), *(matrix_in+2));

	shift_in = (*(matrix_in + 6)) * (*(matrix_in + 1));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_3 = shift_out * (*(matrix_in + 5));
	//RTL_PRINTF ("det_tmp_3 = %d, (%d:%d:%d).\r\n", det_tmp_3, *(matrix_in+6), *(matrix_in+1), *(matrix_in+5));

	shift_in = (*(matrix_in + 6)) * (*(matrix_in + 4));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_4 = shift_out * (*(matrix_in + 2));
	//RTL_PRINTF ("det_tmp_4 = %d, (%d:%d:%d).\r\n", det_tmp_4, *(matrix_in+6), *(matrix_in+4), *(matrix_in+2));

	shift_in = (*(matrix_in + 7)) * (*(matrix_in + 5));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_5 = shift_out * (*(matrix_in + 0));
	//RTL_PRINTF ("det_tmp_5 = %d, (%d:%d:%d).\r\n", det_tmp_5, *(matrix_in+7), *(matrix_in+5), *(matrix_in+0));

	shift_in = (*(matrix_in + 8)) * (*(matrix_in + 3));
	rshift_func(&shift_out, &shift_in, 8);
	det_tmp_6 = shift_out * (*(matrix_in + 1));
	//RTL_PRINTF ("det_tmp_6 = %d, (%d:%d:%d).\r\n", det_tmp_6, *(matrix_in+8), *(matrix_in+3), *(matrix_in+1));

	shift_in = (det_tmp_1 + det_tmp_2 + det_tmp_3) - (det_tmp_4 + det_tmp_5 + det_tmp_6);
	rshift_func(&det_matrix_value, &shift_in, 8);
	//RTL_PRINTF ("det_matrix_value = %d.\r\n", det_matrix_value);

	return det_matrix_value;
}

//------------------------------------
//  Check Inv Function 4x4 (mul:0 , div:0)
//------------------------------------
SINT32 SENSOR_CODE check_inv_func(LPSINT32 matrix_inv, LPSINT32 matrix)
{

	//
	SINT32 shift_in;
	UINT8 inv_fail, p1_fail, p2_fail, p3_fail, p4_fail;
	SINT32 entry_1, entry_2, entry_3, entry_4;

	// Check Diagonal 1
	shift_in = (*(matrix_inv + 0)) * (*(matrix + 0)) + 
			   (*(matrix_inv + 4)) * (*(matrix + 1)) + 
			   (*(matrix_inv + 8)) * (*(matrix + 2)) + 
			   (*(matrix_inv + 12)) * (*(matrix + 3));
	rshift_func(&entry_1, &shift_in, 8);
	/*
	RTL_PRINTF ("1(%d:%d:%d).\r\n", *(matrix_inv+0), *(matrix+0), mul_tmp_1);
	RTL_PRINTF ("1(%d:%d:%d).\r\n", *(matrix_inv+4), *(matrix+1), mul_tmp_2);
	RTL_PRINTF ("1(%d:%d:%d).\r\n", *(matrix_inv+8), *(matrix+2), mul_tmp_3);
	RTL_PRINTF ("1(%d:%d:%d).\r\n", *(matrix_inv+12), *(matrix+3), mul_tmp_4);
	*/
	// Check Diagonal 2
	shift_in = (*(matrix_inv + 1)) * (*(matrix + 4)) + 
			   (*(matrix_inv + 5)) * (*(matrix + 5)) + 
			   (*(matrix_inv + 9)) * (*(matrix + 6)) + 
			   (*(matrix_inv + 13)) * (*(matrix + 7));
	rshift_func(&entry_2, &shift_in, 8);
	/*
	RTL_PRINTF ("2(%d:%d:%d).\r\n", *(matrix_inv+1), *(matrix+4), mul_tmp_1);
	RTL_PRINTF ("2(%d:%d:%d).\r\n", *(matrix_inv+5), *(matrix+5), mul_tmp_2);
	RTL_PRINTF ("2(%d:%d:%d).\r\n", *(matrix_inv+9), *(matrix+6), mul_tmp_3);
	RTL_PRINTF ("2(%d:%d:%d).\r\n", *(matrix_inv+13), *(matrix+7), mul_tmp_4);
	*/
	// Check Diagonal 3
	shift_in = (*(matrix_inv + 2)) * (*(matrix + 8)) + 
			   (*(matrix_inv + 6)) * (*(matrix + 9)) + 
			   (*(matrix_inv + 10)) * (*(matrix + 10)) + 
			   (*(matrix_inv + 14)) * (*(matrix + 11));
	rshift_func(&entry_3, &shift_in, 8);
	/*
	RTL_PRINTF ("3(%d:%d:%d).\r\n", *(matrix_inv+2), *(matrix+8), mul_tmp_1);
	RTL_PRINTF ("3(%d:%d:%d).\r\n", *(matrix_inv+6), *(matrix+9), mul_tmp_2);
	RTL_PRINTF ("3(%d:%d:%d).\r\n", *(matrix_inv+10), *(matrix+10), mul_tmp_3);
	RTL_PRINTF ("3(%d:%d:%d).\r\n", *(matrix_inv+14), *(matrix+11), mul_tmp_4);
	*/
	// Check Diagonal 4
	shift_in = (*(matrix_inv + 3)) * (*(matrix + 12)) + 
			   (*(matrix_inv + 7)) * (*(matrix + 13)) + 
			   (*(matrix_inv + 11)) * (*(matrix + 14))+ 
			   (*(matrix_inv + 15)) * (*(matrix + 15));
	rshift_func(&entry_4, &shift_in, 8);
	/*
	RTL_PRINTF ("4(%d:%d:%d).\r\n", *(matrix_inv+3), *(matrix+12), mul_tmp_1);
	RTL_PRINTF ("4(%d:%d:%d).\r\n", *(matrix_inv+7), *(matrix+13), mul_tmp_2);
	RTL_PRINTF ("4(%d:%d:%d).\r\n", *(matrix_inv+11), *(matrix+14), mul_tmp_3);
	RTL_PRINTF ("4(%d:%d:%d).\r\n", *(matrix_inv+15), *(matrix+15), mul_tmp_4);
	*/
	if((entry_1 > CHECK_INV_MAX_BOUND) || (entry_1 < CHECK_INV_MIN_BOUND))
		p1_fail = 1;
	else
		p1_fail = 0;

	if((entry_2 > CHECK_INV_MAX_BOUND) || (entry_2 < CHECK_INV_MIN_BOUND))
		p2_fail = 1;
	else
		p2_fail = 0;

	if((entry_3 > CHECK_INV_MAX_BOUND) || (entry_3 < CHECK_INV_MIN_BOUND))
		p3_fail = 1;
	else
		p3_fail = 0;

	if((entry_4 > CHECK_INV_MAX_BOUND) || (entry_4 < CHECK_INV_MIN_BOUND))
		p4_fail = 1;
	else
		p4_fail = 0;

	if((p1_fail == 1) || (p2_fail == 1) || (p3_fail == 1) || (p4_fail == 1))
		inv_fail = 1;
	else
		inv_fail = 0;

	//RTL_PRINTF ("inv_fail = %d, (%d:%d:%d:%d).\r\n", inv_fail, entry_1, entry_2, entry_3, entry_4);

	return inv_fail;
}

//------------------------------------
//  Inverse Matrix Function 4x4 (mul:0 , div:0)
//------------------------------------
SINT32 SENSOR_CODE inv_func(LPSINT32 inv_matrix_out, LPSINT32 matrix_in)
{

	//
	UINT8 i, inv_fail;
	SINT32 shift_in;
	SINT32 det_4x4_tmp_1, det_4x4_tmp_2, det_4x4_tmp_3, det_4x4_tmp_4, det_4x4;
	SINT32 inv_entry, inv_entry_1, inv_entry_2, inv_entry_3, inv_entry_4;
	SINT32 matrix_det_in[9] = {0};

	// Det Matrix 4x4
	matrix_det_in[0] = *(matrix_in + 5);
	matrix_det_in[1] = *(matrix_in + 9);
	matrix_det_in[2] = *(matrix_in + 13);
	matrix_det_in[3] = *(matrix_in + 6);
	matrix_det_in[4] = *(matrix_in + 10);
	matrix_det_in[5] = *(matrix_in + 14);
	matrix_det_in[6] = *(matrix_in + 7);
	matrix_det_in[7] = *(matrix_in + 11);
	matrix_det_in[8] = *(matrix_in + 15);
	inv_entry_1 = det_3x3_func(matrix_det_in);

	matrix_det_in[0] = *(matrix_in + 1);
	matrix_det_in[1] = *(matrix_in + 9);
	matrix_det_in[2] = *(matrix_in + 13);
	matrix_det_in[3] = *(matrix_in + 2);
	matrix_det_in[4] = *(matrix_in + 10);
	matrix_det_in[5] = *(matrix_in + 14);
	matrix_det_in[6] = *(matrix_in + 3);
	matrix_det_in[7] = *(matrix_in + 11);
	matrix_det_in[8] = *(matrix_in + 15);
	inv_entry_2 = det_3x3_func(matrix_det_in);

	matrix_det_in[0] = *(matrix_in + 1);
	matrix_det_in[1] = *(matrix_in + 5);
	matrix_det_in[2] = *(matrix_in + 13);
	matrix_det_in[3] = *(matrix_in + 2);
	matrix_det_in[4] = *(matrix_in + 6);
	matrix_det_in[5] = *(matrix_in + 14);
	matrix_det_in[6] = *(matrix_in + 3);
	matrix_det_in[7] = *(matrix_in + 7);
	matrix_det_in[8] = *(matrix_in + 15);
	inv_entry_3 = det_3x3_func(matrix_det_in);

	matrix_det_in[0] = *(matrix_in + 1);
	matrix_det_in[1] = *(matrix_in + 5);
	matrix_det_in[2] = *(matrix_in + 9);
	matrix_det_in[3] = *(matrix_in + 2);
	matrix_det_in[4] = *(matrix_in + 6);
	matrix_det_in[5] = *(matrix_in + 10);
	matrix_det_in[6] = *(matrix_in + 3);
	matrix_det_in[7] = *(matrix_in + 7);
	matrix_det_in[8] = *(matrix_in + 11);
	inv_entry_4 = det_3x3_func(matrix_det_in);

	shift_in = (*(matrix_in + 0)) * inv_entry_1;
	rshift_func(&det_4x4_tmp_1, &shift_in, 8);

	shift_in = (*(matrix_in + 4)) * inv_entry_2;
	rshift_func(&det_4x4_tmp_2, &shift_in, 8);

	shift_in = (*(matrix_in + 8)) * inv_entry_3;
	rshift_func(&det_4x4_tmp_3, &shift_in, 8);

	shift_in = (*(matrix_in + 12)) * inv_entry_4;
	rshift_func(&det_4x4_tmp_4, &shift_in, 8);

	det_4x4 = (det_4x4_tmp_1 + det_4x4_tmp_3) - (det_4x4_tmp_2 + det_4x4_tmp_4);
	//RTL_PRINTF ("%d, (%d:%d:%d:%d), (%d:%d:%d:%d).\r\n", det_4x4, *(matrix_in+0), *(matrix_in+4), *(matrix_in+8), *(matrix_in+12), inv_entry_1, inv_entry_2, inv_entry_3, inv_entry_4);

	if(det_4x4 != 0)
	{
		*(inv_matrix_out + 0) = inv_entry_1;
		*(inv_matrix_out + 1) = -inv_entry_2;
		*(inv_matrix_out + 2) = inv_entry_3;
		*(inv_matrix_out + 3) = -inv_entry_4;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 8);
		matrix_det_in[2] = *(matrix_in + 12);
		matrix_det_in[3] = *(matrix_in + 2);
		matrix_det_in[4] = *(matrix_in + 10);
		matrix_det_in[5] = *(matrix_in + 14);
		matrix_det_in[6] = *(matrix_in + 3);
		matrix_det_in[7] = *(matrix_in + 11);
		matrix_det_in[8] = *(matrix_in + 15);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 5) = inv_entry;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 4);
		matrix_det_in[2] = *(matrix_in + 12);
		matrix_det_in[3] = *(matrix_in + 2);
		matrix_det_in[4] = *(matrix_in + 6);
		matrix_det_in[5] = *(matrix_in + 14);
		matrix_det_in[6] = *(matrix_in + 3);
		matrix_det_in[7] = *(matrix_in + 7);
		matrix_det_in[8] = *(matrix_in + 15);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 6) = -inv_entry;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 4);
		matrix_det_in[2] = *(matrix_in + 8);
		matrix_det_in[3] = *(matrix_in + 2);
		matrix_det_in[4] = *(matrix_in + 6);
		matrix_det_in[5] = *(matrix_in + 10);
		matrix_det_in[6] = *(matrix_in + 3);
		matrix_det_in[7] = *(matrix_in + 7);
		matrix_det_in[8] = *(matrix_in + 11);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 7) = inv_entry;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 4);
		matrix_det_in[2] = *(matrix_in + 12);
		matrix_det_in[3] = *(matrix_in + 1);
		matrix_det_in[4] = *(matrix_in + 5);
		matrix_det_in[5] = *(matrix_in + 13);
		matrix_det_in[6] = *(matrix_in + 3);
		matrix_det_in[7] = *(matrix_in + 7);
		matrix_det_in[8] = *(matrix_in + 15);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 10) = inv_entry;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 4);
		matrix_det_in[2] = *(matrix_in + 8);
		matrix_det_in[3] = *(matrix_in + 1);
		matrix_det_in[4] = *(matrix_in + 5);
		matrix_det_in[5] = *(matrix_in + 9);
		matrix_det_in[6] = *(matrix_in + 3);
		matrix_det_in[7] = *(matrix_in + 7);
		matrix_det_in[8] = *(matrix_in + 11);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 11) = -inv_entry;

		matrix_det_in[0] = *(matrix_in + 0);
		matrix_det_in[1] = *(matrix_in + 4);
		matrix_det_in[2] = *(matrix_in + 8);
		matrix_det_in[3] = *(matrix_in + 1);
		matrix_det_in[4] = *(matrix_in + 5);
		matrix_det_in[5] = *(matrix_in + 9);
		matrix_det_in[6] = *(matrix_in + 2);
		matrix_det_in[7] = *(matrix_in + 6);
		matrix_det_in[8] = *(matrix_in + 10);
		inv_entry = det_3x3_func(matrix_det_in);
		*(inv_matrix_out + 15) = inv_entry;

		*(inv_matrix_out + 4) = *(inv_matrix_out + 1);
		*(inv_matrix_out + 8) = *(inv_matrix_out + 2);
		*(inv_matrix_out + 9) = *(inv_matrix_out + 6);
		*(inv_matrix_out + 12) = *(inv_matrix_out + 3);
		*(inv_matrix_out + 13) = *(inv_matrix_out + 7);
		*(inv_matrix_out + 14) = *(inv_matrix_out + 11);

		for(i = 0 ; i < 16 ; i++)
			*(inv_matrix_out + i) = (*(inv_matrix_out + i) << 8) / det_4x4;

		// Check Inv
		inv_fail = check_inv_func(inv_matrix_out, matrix_in);
	}
	else
	{
		inv_fail = 1;
	}

	return inv_fail;
}

//------------------------------------
//  Compass Normailized Value(mul: , div: )
//  tan:   sin/cos:   mul:   div:   sqrt:
//------------------------------------
SINT32 SENSOR_CODE mag_norm_length(LPSINT32 mag_data)
{

	//
	SINT32 mag_norm_value, mag_norm_value_2;

	// (Data)^2
	mag_norm_value_2 = *(mag_data + 0)*(*(mag_data + 0)) + 
					   *(mag_data + 1)*(*(mag_data + 1)) + 
					   *(mag_data + 2)*(*(mag_data + 2));

	// sqrt(data_total);
	sqrt_func(&mag_norm_value, &mag_norm_value_2);

	return mag_norm_value;
}

//------------------------------------
//  Compass Calibration (Partial) (mul: , div:)
//  tan: 0  sin/cos:  mul:  div: 0
//------------------------------------
void SENSOR_CODE compass_cal_portion(LPSINT32 cal_offset_post, LPSINT32 mag_x_matrix, LPSINT32 mag_y_matrix, LPSINT32 mag_z_matrix, LPSINT32 cal_offset_pre)
{

	//
	SINT32 shift_in, shift_out;
	UINT8 i;
	SINT32 diff_mag_data_offset[3] = {0};
	SINT32 diff_mag_offset_norm;
	SINT32 mean_mag_x, mean_mag_y, mean_mag_z;
	SINT32 r_x0_sum_tmp, r_y0_sum_tmp, r_z0_sum_tmp, r_sum_tmp;
	SINT32 mean_r_x0, mean_r_y0, mean_r_z0, mean_r_matrix;

	// -------------- Compass Offset Calculate ---------------------
	mean_mag_x = 0;
	mean_mag_y = 0;
	mean_mag_z = 0;
	mean_r_x0 = 0;
	mean_r_y0 = 0;
	mean_r_z0 = 0;
	mean_r_matrix = 0;

	for(i = 0 ; i < CMPCAL_PART_NUM ; i++)
	{
		// Calculate SQRT((xi-x0)^2 + (yi-y0)^2 + (zi-z0)^2)
		diff_mag_data_offset[0] = *(mag_x_matrix + i) - *(cal_offset_pre + 0);
		diff_mag_data_offset[1] = *(mag_y_matrix + i) - *(cal_offset_pre + 1);
		diff_mag_data_offset[2] = *(mag_z_matrix + i) - *(cal_offset_pre + 2);
		diff_mag_offset_norm = mag_norm_length(diff_mag_data_offset);
		r_sum_tmp = diff_mag_offset_norm;

		// Calculate R(x0,y0,z0)
		if(r_sum_tmp == 0)
		{
			r_x0_sum_tmp = 0;
			r_y0_sum_tmp = 0;
			r_z0_sum_tmp = 0;
		}
		else
		{
			r_x0_sum_tmp = ((-diff_mag_data_offset[0]) << 8) / r_sum_tmp;
			r_y0_sum_tmp = ((-diff_mag_data_offset[1]) << 8) / r_sum_tmp;
			r_z0_sum_tmp = ((-diff_mag_data_offset[2]) << 8) / r_sum_tmp;
		}

		mean_mag_x = mean_mag_x + *(mag_x_matrix + i);
		mean_mag_y = mean_mag_y + *(mag_y_matrix + i);
		mean_mag_z = mean_mag_z + *(mag_z_matrix + i);
		mean_r_x0 = mean_r_x0 + r_x0_sum_tmp;
		mean_r_y0 = mean_r_y0 + r_y0_sum_tmp;
		mean_r_z0 = mean_r_z0 + r_z0_sum_tmp;
		mean_r_matrix = mean_r_matrix + r_sum_tmp;
	}
			mean_mag_x = mean_mag_x / CMPCAL_PART_NUM;
			mean_mag_y = mean_mag_y / CMPCAL_PART_NUM;
			mean_mag_z = mean_mag_z / CMPCAL_PART_NUM;
			mean_r_x0 = mean_r_x0 / CMPCAL_PART_NUM;
			mean_r_y0 = mean_r_y0 / CMPCAL_PART_NUM;
			mean_r_z0 = mean_r_z0 / CMPCAL_PART_NUM;
			mean_r_matrix = mean_r_matrix / CMPCAL_PART_NUM;

	// Calculate Partial Offset And Shift Offset Value
	shift_in = mean_r_x0 * mean_r_matrix;
	rshift_func(&shift_out, &shift_in, 8);
	*(cal_offset_post + 0) = mean_mag_x + shift_out;

	shift_in = mean_r_y0 * mean_r_matrix;
	rshift_func(&shift_out, &shift_in, 8);
	*(cal_offset_post + 1) = mean_mag_y + shift_out;

	shift_in = mean_r_z0 * mean_r_matrix;
	rshift_func(&shift_out, &shift_in, 8);
	*(cal_offset_post + 2) = mean_mag_z + shift_out;

	return;
}

//------------------------------------
//  Compass Calibration Part I (Total) (mul: , div:)
//  tan: 0  sin/cos:  mul:  div: 0
//------------------------------------
void SENSOR_CODE compass_cal_whole(LPSINT32 cal_offset_post, LPSINT32 mag_x_matrix, LPSINT32 mag_y_matrix, LPSINT32 mag_z_matrix, LPSINT32 cal_offset_pre)
{

	//
	SINT32 shift_in, shift_out;
	SINT32 mag_dmatrix[3];
	SINT32 mag_x_2, mag_y_2, mag_z_2;
	UINT8 i, entry_index, inv_fail;
	SINT32 atxa[16] = {0};
	SINT32 inv_atxa[16] = {0};
	SINT32 atxb[4] = {0};
	SINT32 mul_tmp, mul_tmp_1, mul_tmp_2, mul_tmp_3, mul_tmp_4;
	SINT32 diff_mag_offset_norm = 0;
	SINT32 diff_mag_offset_norm_avg = 0;
	SINT32 diff_mag_data_offset[3] = {0};
	SINT32 mag_whole_max[3] = {0};
	SINT32 mag_whole_min[3] = {0};
	SINT32 init_offset[3] = {0};
	SINT32 mag_r_max = 0;
	SINT32 mag_r_min = 0;
	
	// Find Whole Part Matrix Max. & Min.
	mag_whole_max[0] = *(mag_x_matrix + 0);
	mag_whole_max[1] = *(mag_y_matrix + 0);
	mag_whole_max[2] = *(mag_z_matrix + 0);
	mag_whole_min[0] = *(mag_x_matrix + 0);
	mag_whole_min[1] = *(mag_y_matrix + 0);
	mag_whole_min[2] = *(mag_z_matrix + 0);

	for(i = 0 ; i < CMPCAL_WHOLE_NUM; i++)
	{
		// X-axis Max. & Min.
		if(mag_whole_max[0] < * (mag_x_matrix + i))
			mag_whole_max[0] = *(mag_x_matrix + i);
		if(*(mag_x_matrix + i) < mag_whole_min[0])
			mag_whole_min[0] = *(mag_x_matrix + i);

		// Y-axis Max. & Min.
		if(mag_whole_max[1] < * (mag_y_matrix + i))
			mag_whole_max[1] = *(mag_y_matrix + i);
		if(*(mag_y_matrix + i) < mag_whole_min[1])
			mag_whole_min[1] = *(mag_y_matrix + i);

		// Z-axis Max. & Min.
		if(mag_whole_max[2] < * (mag_z_matrix + i))
			mag_whole_max[2] = *(mag_z_matrix + i);
		if(*(mag_z_matrix + i) < mag_whole_min[2])
			mag_whole_min[2] = *(mag_z_matrix + i);
		//RTL_PRINTF ("Max:(%d:%d:%d), Min:(%d:%d:%d), In:(%d:%d:%d).\r\n", mag_whole_max[0], mag_whole_max[1], mag_whole_max[2], mag_whole_min[0], mag_whole_min[1], mag_whole_min[2], *(mag_x_matrix + i), *(mag_y_matrix + i), *(mag_z_matrix + i));
	}
	shift_in = mag_whole_max[0] + mag_whole_min[0];
	rshift_func(&init_offset[0], &shift_in, 1);
	shift_in = mag_whole_max[1] + mag_whole_min[1];
	rshift_func(&init_offset[1], &shift_in, 1);
	shift_in = mag_whole_max[2] + mag_whole_min[2];
	rshift_func(&init_offset[2], &shift_in, 1);
	//RTL_PRINTF ("OFFSET:(%d:%d:%d).\r\n", init_offset[0], init_offset[1], init_offset[2]);

	// Calculate Whole Calibration Matrix
	for(i = 0 ; i < CMPCAL_WHOLE_NUM; i++)
	{
		// Determine Compass Data Range
		mag_dmatrix[0] = *(mag_x_matrix + i) - init_offset[0];
		mag_dmatrix[1] = *(mag_y_matrix + i) - init_offset[1];
		mag_dmatrix[2] = *(mag_z_matrix + i) - init_offset[2];

		// At*A
		mag_x_2 = mag_dmatrix[0] * mag_dmatrix[0];
		mag_y_2 = mag_dmatrix[1] * mag_dmatrix[1];
		mag_z_2 = mag_dmatrix[2] * mag_dmatrix[2];

		atxa[0] = atxa[0] + (mag_x_2 << 2);
		atxa[5] = atxa[5] + (mag_y_2 << 2);
		atxa[10] = atxa[10] + (mag_z_2 << 2);
		atxa[15] = atxa[15] + (1 << 8);

		mul_tmp = (mag_dmatrix[0] << 1) * (mag_dmatrix[1] << 1);
		atxa[4] = atxa[4] + mul_tmp;

		mul_tmp = (mag_dmatrix[0] << 1) * (mag_dmatrix[2] << 1);
		atxa[8] = atxa[8] + mul_tmp;

		mul_tmp = (mag_dmatrix[1] << 1) * (mag_dmatrix[2] << 1);
		atxa[9] = atxa[9] + mul_tmp;

		atxa[12] = atxa[12] + (mag_dmatrix[0] << 1);
		atxa[13] = atxa[13] + (mag_dmatrix[1] << 1);
		atxa[14] = atxa[14] + (mag_dmatrix[2] << 1);

		// A = At
		atxa[1] = atxa[4];
		atxa[2] = atxa[8];
		atxa[3] = atxa[12];
		atxa[6] = atxa[9];
		atxa[7] = atxa[13];
		atxa[11] = atxa[14];

		// At*B
		shift_in = mag_x_2 + mag_y_2 + mag_z_2;
		rshift_func(&shift_out, &shift_in, 8);
		
		mul_tmp = (mag_dmatrix[0] << 1) * shift_out;
		atxb[0] = atxb[0] + mul_tmp;

		mul_tmp = (mag_dmatrix[1] << 1) * shift_out;
		atxb[1] = atxb[1] + mul_tmp;

		mul_tmp = (mag_dmatrix[2] << 1) * shift_out;
		atxb[2] = atxb[2] + mul_tmp;

		atxb[3] = atxb[3] + shift_out;
	}

			shift_in = atxa[0];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[0] = shift_out;
			shift_in = atxa[5];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[5] = shift_out;
			shift_in = atxa[10];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[10] = shift_out;

			shift_in = atxa[4];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[4] = shift_out;
			atxa[1] = atxa[4];

			shift_in = atxa[8];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[8] = shift_out;
			atxa[2] = atxa[8];

			shift_in = atxa[9];
	rshift_func(&shift_out, &shift_in, 8);
			atxa[9] = shift_out;
			atxa[6] = atxa[9];

			shift_in = atxb[0];
	rshift_func(&shift_out, &shift_in, 8);
			atxb[0] = shift_out;
			shift_in = atxb[1];
	rshift_func(&shift_out, &shift_in, 8);
			atxb[1] = shift_out;
			shift_in = atxb[2];
	rshift_func(&shift_out, &shift_in, 8);
			atxb[2] = shift_out;
			//RTL_PRINTF ("(%d:%d:%d), (%d:%d:%d), (%d:%d:%d).\r\n", atxa[0], atxa[5], atxa[10], atxa[4], atxa[8], atxa[9], atxb[0], atxb[1], atxb[2]);

	inv_fail = inv_func(inv_atxa, atxa);
	/*
	RTL_PRINTF ("mag_scale_factor = %d.\r\n", mag_scale_factor);
	RTL_PRINTF ("----- INV INPUT ----.\r\n");
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", atxa[0], atxa[4], atxa[8], atxa[12]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", atxa[1], atxa[5], atxa[9], atxa[13]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", atxa[2], atxa[6], atxa[10], atxa[14]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", atxa[3], atxa[7], atxa[11], atxa[15]);

	RTL_PRINTF ("----- INV DONE -----.\r\n");
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", inv_atxa[0], inv_atxa[4], inv_atxa[8], inv_atxa[12]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", inv_atxa[1], inv_atxa[5], inv_atxa[9], inv_atxa[13]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", inv_atxa[2], inv_atxa[6], inv_atxa[10], inv_atxa[14]);
	RTL_PRINTF ("(%d:%d:%d:%d).\r\n", inv_atxa[3], inv_atxa[7], inv_atxa[11], inv_atxa[15]);
	RTL_PRINTF ("atxb = (%d:%d:%d:%d).\r\n", atxb[0], atxb[1], atxb[2], atxb[3]);
	*/
	
	if(inv_fail == 0)
	{
		for(i = 0 ; i < 3 ; i++)
		{
			entry_index = i;
			mul_tmp_1 = inv_atxa[entry_index] * (atxb[0]);
			entry_index = i + 4;
			mul_tmp_2 = inv_atxa[entry_index] * (atxb[1]);
			entry_index = i + 8;
			mul_tmp_3 = inv_atxa[entry_index] * (atxb[2]);
			entry_index = i + 12;
			mul_tmp_4 = inv_atxa[entry_index] * (atxb[3]);
			shift_in = mul_tmp_1 + mul_tmp_2 + mul_tmp_3 + mul_tmp_4;
			rshift_func(&shift_out, &shift_in, 8);
			*(cal_offset_post + i) = shift_out + init_offset[i];
		}
		
		// Mag Length Avg. & Check Mag Point and Mag r
		for(i = 0 ; i < CMPCAL_WHOLE_NUM; i++)
		{
			diff_mag_data_offset[0] = *(mag_x_matrix + i) - *(cal_offset_post + 0);
			diff_mag_data_offset[1] = *(mag_y_matrix + i) - *(cal_offset_post + 1);
			diff_mag_data_offset[2] = *(mag_z_matrix + i) - *(cal_offset_post + 2);
			diff_mag_offset_norm = mag_norm_length(diff_mag_data_offset);
			diff_mag_offset_norm_avg = diff_mag_offset_norm_avg + diff_mag_offset_norm;
			
			if(i == 0)
			{
				mag_r_max = diff_mag_offset_norm;
				mag_r_min = mag_r_max;
			}
			else
			{
				if((diff_mag_offset_norm > mag_r_max))
					mag_r_max = diff_mag_offset_norm;
				if((diff_mag_offset_norm < mag_r_min))
					mag_r_min = diff_mag_offset_norm;
			}
		}
		diff_mag_offset_norm_avg = diff_mag_offset_norm_avg/CMPCAL_WHOLE_NUM;
		
		if((mag_r_max - mag_r_min) < (diff_mag_offset_norm_avg/10))
		{
			*(cal_offset_post + 3) = diff_mag_offset_norm_avg;
			//RTL_PRINTF ("Seccess->S(%d:%d).\r\n", mag_r_max, mag_r_min);
			mag_cal_success = 1;
		}
		else
		{
			*(cal_offset_post + 0) = *(cal_offset_pre + 0);
			*(cal_offset_post + 1) = *(cal_offset_pre + 1);
			*(cal_offset_post + 2) = *(cal_offset_pre + 2);
			*(cal_offset_post + 3) = *(cal_offset_pre + 3);
			//RTL_PRINTF ("Seccess->F(%d:%d).\r\n", mag_r_max, mag_r_min);
		}
	}
	else
	{
		*(cal_offset_post + 0) = *(cal_offset_pre + 0);
		*(cal_offset_post + 1) = *(cal_offset_pre + 1);
		*(cal_offset_post + 2) = *(cal_offset_pre + 2);
		*(cal_offset_post + 3) = *(cal_offset_pre + 3);
		//RTL_PRINTF ("Fail.\r\n");
	}

	return;
}

//------------------------------------
//  Compass Calibration Comb. With Part I/II (Whole/Partial) (mul: , div:)
//  tan: 0  sin/cos:  mul:  div: 0
//------------------------------------
void SENSOR_CODE compass_cal_comb(TCmpCalInf* pMag_pre, TCmpCalInf* pMag_post,
                                  LPSINT32 mag_raw_data)
{
	
	//
	UINT8 i, j;
	SINT32 cal_offset_whole[4] = {0};
	SINT32 cal_offset_whole_pre[4] = {0};
	SINT32 cal_offset_partial[3] = {0};

	// -------------- Compass Offset Calculate ---------------------
	// Whole/Partial Offset
	cal_offset_whole[0] = pMag_pre->cal_offset[3];
	cal_offset_whole[1] = pMag_pre->cal_offset[4];
	cal_offset_whole[2] = pMag_pre->cal_offset[5];
	cal_offset_whole_pre[0] = pMag_pre->cal_offset[3];
	cal_offset_whole_pre[1] = pMag_pre->cal_offset[4];
	cal_offset_whole_pre[2] = pMag_pre->cal_offset[5];
	cal_offset_partial[0] = pMag_pre->cal_offset[0];
	cal_offset_partial[1] = pMag_pre->cal_offset[1];
	cal_offset_partial[2] = pMag_pre->cal_offset[2];

	// Mag Length Avg.
	cal_offset_whole_pre[3] = mag_length_avg;

	// Shift Calibration Data (Whole)
	sys_cnt = sys_cnt + 1;

	if((skip_num <= sys_cnt))
	{
		for(i = 0 ; i < CMPCAL_WHOLE_NUM ; i++)
		{
			if(i < (CMPCAL_WHOLE_NUM - 1))
			{
				j = i + 1;
				pMag_post->mag_x_whole[i] = pMag_pre->mag_x_whole[j];
				pMag_post->mag_y_whole[i] = pMag_pre->mag_y_whole[j];
				pMag_post->mag_z_whole[i] = pMag_pre->mag_z_whole[j];
			}
			else
			{
				pMag_post->mag_x_whole[i] = *(mag_raw_data + 0);
				pMag_post->mag_y_whole[i] = *(mag_raw_data + 1);
				pMag_post->mag_z_whole[i] = *(mag_raw_data + 2);
			}
		}
		whole_cnt = whole_cnt + 1;
		sys_cnt = 0;
	}
	else
	{
		for(i = 0 ; i < CMPCAL_WHOLE_NUM ; i++)
		{
			pMag_post->mag_x_whole[i] = pMag_pre->mag_x_whole[i];
			pMag_post->mag_y_whole[i] = pMag_pre->mag_y_whole[i];
			pMag_post->mag_z_whole[i] = pMag_pre->mag_z_whole[i];
		}
	}

	// Shift Calibration Data (Partial)
	for(i = 0 ; i < CMPCAL_PART_NUM ; i++)
	{
		if(i < (CMPCAL_PART_NUM - 1))
		{
			j = i + 1;
			pMag_post->mag_x_part[i] = pMag_pre->mag_x_part[j];
			pMag_post->mag_y_part[i] = pMag_pre->mag_y_part[j];
			pMag_post->mag_z_part[i] = pMag_pre->mag_z_part[j];
		}
		else
		{
			pMag_post->mag_x_part[i] = *(mag_raw_data + 0);
			pMag_post->mag_y_part[i] = *(mag_raw_data + 1);
			pMag_post->mag_z_part[i] = *(mag_raw_data + 2);
		}
	}

	// Calibration Whole/Partial
	if((CMPCAL_WHOLE_NUM == whole_cnt))
	{
		compass_cal_whole(cal_offset_whole,  pMag_post->mag_x_whole,  pMag_post->mag_y_whole,  pMag_post->mag_z_whole, cal_offset_whole_pre);
		whole_cnt = 0;
		
		cal_offset_partial[0] = cal_offset_whole[0];
		cal_offset_partial[1] = cal_offset_whole[1];
		cal_offset_partial[2] = cal_offset_whole[2];

		mag_length_avg = (SINT16)cal_offset_whole[3];
		//RTL_PRINTF ("Avg = %d, (%d:%d:%d).\r\n",cal_offset_whole[3], cal_offset_partial[0], cal_offset_partial[1], cal_offset_partial[2]);
	}
	else
	{
		compass_cal_portion(cal_offset_partial, pMag_post->mag_x_part, pMag_post->mag_y_part, pMag_post->mag_z_part, cal_offset_whole);
		//RTL_PRINTF ("INFR:%d->(%d:%d:%d).\r\n", *mag_infr_flag, cal_offset_partial[0], cal_offset_partial[1], cal_offset_partial[2]);
	}

	// Whole/Partial Offset
	pMag_post->cal_offset[3] = cal_offset_whole[0];
	pMag_post->cal_offset[4] = cal_offset_whole[1];
	pMag_post->cal_offset[5] = cal_offset_whole[2];
	pMag_post->cal_offset[0] = cal_offset_partial[0];
	pMag_post->cal_offset[1] = cal_offset_partial[1];
	pMag_post->cal_offset[2] = cal_offset_partial[2];

	return;
}

//------------------------------------
//  Compass Soft-Iron Calibration (mul: , div:)
//  tan: 0  sin/cos:  mul:  div: 0
//------------------------------------
void SENSOR_CODE soft_iron_calibration(LPSINT32 mag_raw_data)
{
	
	// 
	SINT32 mul_tmp;
	SINT32 mag_rdata_offset[3];

	// Soft Iron Offset
	mag_rdata_offset[0] = *(mag_raw_data + 0);
	mag_rdata_offset[1] = *(mag_raw_data + 1);
	mag_rdata_offset[2] = *(mag_raw_data + 2);

	// Soft Iron Calibration Matrix
	mul_tmp = soft_iron_matrix[0] * mag_rdata_offset[0] + soft_iron_matrix[3] * mag_rdata_offset[1] + soft_iron_matrix[6] * mag_rdata_offset[2];
	rshift_func((mag_raw_data + 0), &mul_tmp, 8);
	mul_tmp = soft_iron_matrix[1] * mag_rdata_offset[0] + soft_iron_matrix[4] * mag_rdata_offset[1] + soft_iron_matrix[7] * mag_rdata_offset[2];
	rshift_func((mag_raw_data + 1), &mul_tmp, 8);
	mul_tmp = soft_iron_matrix[2] * mag_rdata_offset[0] + soft_iron_matrix[5] * mag_rdata_offset[1] + soft_iron_matrix[8] * mag_rdata_offset[2];
	rshift_func((mag_raw_data + 2), &mul_tmp, 8);

	return;
}

//------------------------------------
//  Compass Interference Detect (mul: , div:)
//  tan: 0  sin/cos:  mul:  div: 0
//------------------------------------
#if 1 //{ 
void SENSOR_CODE compass_infr_detect(LPSINT32 mag_rdata, LPSINT16 mag_offset, LPUINT8 mag_infr_flag)
{
	
	//
	SINT32 diff_mag_offset_norm = 0;
	SINT32 diff_mag_data_offset[3] = {0};
	SINT32 infr_max_bound, infr_min_bound;

	// Compass Interference Detect
	diff_mag_data_offset[0] = *(mag_rdata + 0) - *(mag_offset + 0);
	diff_mag_data_offset[1] = *(mag_rdata + 1) - *(mag_offset + 1);
	diff_mag_data_offset[2] = *(mag_rdata + 2) - *(mag_offset + 2);
	diff_mag_offset_norm = mag_norm_length(diff_mag_data_offset);
	
	infr_max_bound = (mag_length_avg) + (mag_length_avg >> 1); // 1.5*mag_length_avg
	infr_min_bound = (mag_length_avg >> 1); // 0.5*mag_length_avg
	
	if((diff_mag_offset_norm > infr_max_bound) || (diff_mag_offset_norm < infr_min_bound))
		*mag_infr_flag = 1;
	else
		*mag_infr_flag = 0;
		
	//RTL_PRINTF ("(%d:%d).\r\n", diff_mag_offset_norm_2, diff_mag_offset_norm);
	
	return;
}
#endif // } compass_infr_detect()

// **************************************************************************
int SENSOR_CODE compass_calibrate(SINT32 pDataBuffer[3], SINT16 pBiasBuffer[3], UINT8 *pMagInfrFlag, UINT16 u16Freq)
{
	if(bCalibrationStateReset)
	{
		memset((LPUINT8)&mag_cal_pre, 0x00, sizeof(TCmpCalInf));
		memset((LPUINT8)&mag_cal_post, 0x00, sizeof(TCmpCalInf));

        if(pCalData)
			RESET_CAL_DATA(pCalData, mag_fdata_idx);
        
		whole_cnt = 0;
		sys_cnt = 0;

		mag_HID_cal_flag = 0;
        mag_length_avg = 0;
		compass_infr_cnt = 0;
		
		bCalibrationStateReset = FALSE;

		//RTL_PRINTF("Reset.\r\n");
	}
	
	// Update Skip count
    skip_num_update(u16Freq);

	// Soft-Iron Calibration
	soft_iron_calibration(pDataBuffer);
	
	// Hard-Iron Calibration(Move/Rotate)
	mag_cal_success = 0;
	if(compass_cal_flag == 1)
		compass_cal_comb(&mag_cal_pre, &mag_cal_post, pDataBuffer);
	
	// Detect Calibration Success or Not
	if(mag_cal_success == 1)
	{
		soft_hard_iron_offset[0] = mag_cal_post.cal_offset[3];
		soft_hard_iron_offset[1] = mag_cal_post.cal_offset[4];
		soft_hard_iron_offset[2] = mag_cal_post.cal_offset[5];
	}
	
	memcpy((UINT8*)&mag_cal_pre, (UINT8*)&mag_cal_post, sizeof(TCmpCalInf));

	pBiasBuffer[0] = (SINT16)(mag_cal_post.cal_offset[0]);
	pBiasBuffer[1] = (SINT16)(mag_cal_post.cal_offset[1]);
	pBiasBuffer[2] = (SINT16)(mag_cal_post.cal_offset[2]);
	
	// Magnetometer HID Calibration Flag
	compass_infr_cnt++;
	if((mag_length_avg > 0) && ((compass_infr_cnt >= skip_num) || (mag_cal_success == 1)))
	{
		// Compass Interference Detect
		compass_infr_detect(pDataBuffer, pBiasBuffer, pMagInfrFlag);
		
		if((*pMagInfrFlag == 0) && (mag_HID_cal_flag < 3))
			mag_HID_cal_flag++;
		else if((*pMagInfrFlag == 1) && (mag_HID_cal_flag > 0))
			mag_HID_cal_flag--;
		
		compass_infr_cnt = 0;
	}
	//RTL_PRINTF ("%d:%d:%d.\r\n", mag_cal_success, mag_length_avg, mag_HID_cal_flag);
	
	if((mag_HID_cal_flag == 3) && (!IS_CAL_DATA_VALID(pCalData)))
		SET_CAL_DATA_VALID(pCalData);	
	
	*pMagInfrFlag = 0;
	u8MagInfrFlag = (UINT8) *pMagInfrFlag;	

    pDataBuffer[0] -= pBiasBuffer[0];
	pDataBuffer[1] -= pBiasBuffer[1];
	pDataBuffer[2] -= pBiasBuffer[2];

	return mag_HID_cal_flag;
}

void SENSOR_CODE_L init_calibrate_parameter(void)
{
    whole_cnt = 0;
    sys_cnt = 0;
    
    mag_HID_cal_flag = 2;
    compass_infr_cnt = 0;
    
	mag_cal_post.cal_offset[0] = soft_hard_iron_offset[0];
	mag_cal_post.cal_offset[1] = soft_hard_iron_offset[1];
	mag_cal_post.cal_offset[2] = soft_hard_iron_offset[2];
	mag_cal_post.cal_offset[3] = soft_hard_iron_offset[0];
	mag_cal_post.cal_offset[4] = soft_hard_iron_offset[1];
	mag_cal_post.cal_offset[5] = soft_hard_iron_offset[2];
    memcpy((UINT8*)&mag_cal_pre, (UINT8*)&mag_cal_post, sizeof(TCmpCalInf));
}

// **************************************************************************
void SENSOR_CODE get_compass_calibrate_info(UINT8 *pState, UINT8 *pMagInfrFlag)
{
	*pState = ((mag_fdata_idx << 4) | mag_HID_cal_flag);
	*pMagInfrFlag = ((compass_cal_flag << 4) | mag_HID_cal_flag);//u8MagInfrFlag;
}

void SENSOR_CODE_L init_compass_calibrate_data(UINT8 *pBuf)
{
	pCalData = (P_CAL_STORED_DATA)pBuf;

	if(!bCalibrationStateReset)
	{
		if(IS_CAL_DATA_VALID(pCalData))
		{
            init_calibrate_parameter();
		}
		else
			bCalibrationStateReset = TRUE;
	}

	if(!IS_MATRIX_VALID(pCalData))
	{
        SET_DEFAULT_MATRIX(pCalData);
        SET_MATRIX_VALID(pCalData);
//	        printk("Set SoftIronMatrix to default.\r\n");
	}

//	    printk("pCalData: %X(%d)\r\n", pBuf, sizeof(T_CAL_STORED_DATA));
//	    printk("%d %d %d %d\r\n", soft_hard_iron_offset[0], soft_hard_iron_offset[1], soft_hard_iron_offset[2], mag_length_avg);
}

void SENSOR_CODE_L reset_compass_calibrate_data(UINT8 u8Formation)
{
    set_compass_calibrate_formation(u8Formation);

	bCalibrationStateReset = TRUE;

//	    printk("pCalData reset\r\n");
}

UINT8 SENSOR_CODE_L set_compass_calibrate_formation(UINT8 u8Formation)
{
	if(mag_fdata_idx != u8Formation)
	{
        if(u8Formation < MAX_FORMATION_NUM)
        {
            mag_fdata_idx = u8Formation;

			init_calibrate_parameter();
            
            // Pad-NB Change Flag
            pad_NB_mode_flag = 1;
        }
	}

    return mag_fdata_idx;
}
#endif // } __ENABLE_FUSION_API__


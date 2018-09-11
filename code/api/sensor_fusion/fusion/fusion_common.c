#include "fusion_common.h"

#if __ENABLE_FUSION_API__ // {
/*-------------------------------------------------------------------------
    Global Variable
-------------------------------------------------------------------------*/
// Sin/Cos LUT
const SINT16 SENSOR_RODATA sin_lut[] = { 0, 1, 3, 4, 6, 7, 9, 10, 12, 14, 15, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37,
                                       39, 40, 42, 43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 60, 62, 63, 65, 66, 68, 69, 71, 72, 74, 75,
                                       77, 78, 80, 81, 83, 84, 86, 87, 89, 90, 92, 93, 95, 96, 97, 99, 100, 102, 103, 105, 106, 108, 109, 110, 112,
                                       113, 115, 116, 117, 119, 120, 122, 123, 124, 126, 127, 128, 130, 131, 132, 134, 135, 136, 138, 139, 140, 142, 143, 144, 146,
                                       147, 148, 149, 151, 152, 153, 155, 156, 157, 158, 159, 161, 162, 163, 164, 166, 167, 168, 169, 170, 171, 173, 174, 175, 176,
                                       177, 178, 179, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
                                       203, 204, 205, 206, 207, 208, 209, 210, 211, 211, 212, 213, 214, 215, 216, 217, 217, 218, 219, 220, 221, 221, 222, 223, 224,
                                       225, 225, 226, 227, 227, 228, 229, 230, 230, 231, 232, 232, 233, 234, 234, 235, 235, 236, 237, 237, 238, 238, 239, 239, 240,
                                       241, 241, 242, 242, 243, 243, 244, 244, 244, 245, 245, 246, 246, 247, 247, 247, 248, 248, 249, 249, 249, 250, 250, 250, 251,
                                       251, 251, 251, 252, 252, 252, 252, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255,
                                       255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 254, 255, 160, 169, 161, 161, 167, 163, 160, 162, 169, 166
                                       };

// Initial
SENSOR_DATA SENSOR_RAW_DATA_17 acc_x_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 acc_y_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 acc_z_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 mag_x_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 mag_y_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 mag_z_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 gyo_x_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 gyo_y_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SENSOR_RAW_DATA_17 gyo_z_rdata = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Raw data Change
SENSOR_DATA UINT8 *p_axis_mode = NULL;

SENSOR_DATA SINT32 ardata_chag_value[3] = {0, 0, 0};
SENSOR_DATA SINT32 mrdata_chag_value[3] = {0, 0, 0};

SENSOR_DATA SINT32 acc_data[3] = {0, 0, 0};
SENSOR_DATA SINT32 mag_data[3] = {0, 0, 0};
SENSOR_DATA SINT32 gyro_data[3] = {0, 0, 0};

// Shake Detection
SENSOR_DATA SINT32 acc_raw_data[3] = {0, 0, 0};
#ifdef __SHAKE_FUNCTION_SIMPLE__ // {
#define SHAKE_EN_THR    SHAKE_EN_THR_S
#else
#define SHAKE_EN_THR    SHAKE_EN_THR_16
SENSOR_DATA SINT16 acc_raw_x_vec_pre[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SINT16 acc_raw_y_vec_pre[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
SENSOR_DATA SINT16 acc_raw_z_vec_pre[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const SINT16 SENSOR_RODATA shake_model_up_down[16] = { -4, 3, 39, 187, 415, 510, 463, -14, -510, -510, -385, -188, -55, 6, 25, 23};
const SINT16 SENSOR_RODATA shake_model_down_up[16] = { -11, -18, -35, -187, -510, -510, -312, 166, 510, 510, 407, 162, 16, -22, -11, 10};

SENSOR_BSS SINT16 acc_raw_x_vec_post[16];
SENSOR_BSS SINT16 acc_raw_y_vec_post[16];
SENSOR_BSS SINT16 acc_raw_z_vec_post[16];

SENSOR_DATA UINT8 shake_status_tmp = 0;
#endif // } __SHAKE_FUNCTION_SIMPLE__

// 2nd Acc
SENSOR_BSS UINT8 acc2_axis_mode;
SENSOR_DATA SINT16 *acc2_bias = NULL;
SENSOR_DATA UINT8 lnr_acc_cnt = 0;

// Acc 6-axis Calibration
SENSOR_DATA SINT16 *acc_gain = NULL;
SENSOR_DATA TAccCal6AxisInf *p_acc_cal_buf = NULL;
SENSOR_DATA SINT16 *acc2_gain = NULL;
SENSOR_DATA TAccCal6AxisInf *p_acc2_cal_buf = NULL;

// -
SENSOR_DATA T_FUSION_COMMON_FLAG gtFusComFlag = {
                                                 ._mag_op_mode = 0,
                                                 ._compass_cal_flag = 1,
                                                 ._pad_NB_mode_flag = 0,
                                                 ._rdata_chag_flag = 0,
                                                 ._gyro_cal_done_flag = 0,
                                                 };

SENSOR_DATA QUATERN_TO_ANGLE_FUNC quatern2angle = qua2ang_yxz;
//	SENSOR_DATA ANGLE_TO_MATRIX_FUNC angle2matrix = ang2mat_yxz;
SENSOR_DATA ANGLE_TO_QUATERN_FUNC angle2quatern = ang2qua_yxz;
SENSOR_DATA MAG_HORIZONTAL_FUNC mag_horizontal_func = mag_horiz_yxz;
SENSOR_DATA ROLL_PITCH_FUNC roll_pitch_func = roll_pitch_yxz;

/*-------------------------------------------------------------------------
    Internal function for fusion
-------------------------------------------------------------------------*/
//------------------------------------
//  Sensor Raw Data Change (Percentage)
//------------------------------------
UINT8 SENSOR_CODE rdata_chag_prtg(LPSINT32 rdata_chag_value, LPSINT32 rdata_in)
{
	int i;

	for(i = 0; i < 3; i++)
	{
		if(abs(*(rdata_in + i) - * (rdata_chag_value + i)) > CHANGE_PERCENTAGE)
		{
			*(rdata_chag_value + 0) = *(rdata_in + 0);
			*(rdata_chag_value + 1) = *(rdata_in + 1);
			*(rdata_chag_value + 2) = *(rdata_in + 2);

			return 1;
		}
	}

	return 0;
}

//------------------------------------
//  Two Accelerometer Vector Angle
//------------------------------------
int SENSOR_CODE two_acc_angle(LPSINT32 acc_in1, LPSINT32 acc_in2, 
                              SINT16 angle[3], UINT16 u16Freq)
{
	// Acc1/Acc2 Vector
	LPSenAxisVec screen_side = (LPSenAxisVec)acc_in1;
    LPSenAxisVec base_side = (LPSenAxisVec)acc_in2;

	// Two Acc Control Value
	SINT32 base_side_angle, screen_side_angle, acc_in1_in2_angle;
	SINT32 g_2_length_screen, g_2_length_base;

	// Calculate Acc Length
	g_2_length_screen = (screen_side->x*screen_side->x) + 
	                    (screen_side->y*screen_side->y) + 
	                    (screen_side->z*screen_side->z);
	g_2_length_base = (base_side->x*base_side->x) + 
                      (base_side->y*base_side->y) + 
                      (base_side->z*base_side->z);
	
	// Calculate condition -> |x| <= 0.7, g-0.15 < |ACC|^2 < g+0.15
    if(((abs(screen_side->x) < X_VERTICAL_VALUE) && (abs(base_side->x) < X_VERTICAL_VALUE)) && 
       ((abs(g_2_length_screen) < TWOACC_LENGTH_MAX) && (abs(g_2_length_screen) > TWOACC_LENGTH_MIN)) && 
       ((abs(g_2_length_base) < TWOACC_LENGTH_MAX) && (abs(g_2_length_base) > TWOACC_LENGTH_MIN)) && 
       (lnr_acc_cnt == 0))
    {
    	tan_cordic(&base_side_angle, &base_side->y, &base_side->z);
    	tan_cordic(&screen_side_angle, &screen_side->y, &screen_side->z);

    	// Base Side Angle
    	if((base_side->y == 0) && (base_side->z > 0))
    		base_side_angle = FIX_180;
    	else if((base_side->y == 0) && (base_side->z < 0))
    		base_side_angle = 0;
    	else if((base_side->y > 0) && (base_side->z == 0))
    		base_side_angle = FIX_90;
    	else if((base_side->y < 0) && (base_side->z == 0))
    		base_side_angle = FIX_270;
    	else if((base_side->y > 0) && (base_side->z > 0))
    		base_side_angle = FIX_180 - base_side_angle;
    	else if((base_side->y > 0) && (base_side->z < 0))
    		base_side_angle = - base_side_angle;
    	else if((base_side->y < 0) && (base_side->z < 0))
    		base_side_angle = FIX_360 - base_side_angle;
    	else if((base_side->y < 0) && (base_side->z > 0))
    		base_side_angle = FIX_180 - base_side_angle;

    	base_side_angle = base_side_angle + FIX_180;
    	if(base_side_angle > FIX_360)
    		base_side_angle = base_side_angle - FIX_360;

    	// Screen Side Angle
    	if((screen_side->y == 0) && (screen_side->z > 0))
    		screen_side_angle = FIX_180;
    	else if((screen_side->y == 0) && (screen_side->z < 0))
    		screen_side_angle = 0;
    	else if((screen_side->y > 0) && (screen_side->z == 0))
    		screen_side_angle = FIX_90;
    	else if((screen_side->y < 0) && (screen_side->z == 0))
    		screen_side_angle = FIX_270;
    	else if((screen_side->y > 0) && (screen_side->z > 0))
    		screen_side_angle = FIX_180 - screen_side_angle;
    	else if((screen_side->y > 0) && (screen_side->z < 0))
    		screen_side_angle = - screen_side_angle;
    	else if((screen_side->y < 0) && (screen_side->z < 0))
    		screen_side_angle = FIX_360 - screen_side_angle;
    	else if((screen_side->y < 0) && (screen_side->z > 0))
    		screen_side_angle = FIX_180 - screen_side_angle;

    	// Between Screen And Base Side Angle
    	acc_in1_in2_angle = screen_side_angle - base_side_angle;
    	if(acc_in1_in2_angle < 0)
    		acc_in1_in2_angle = FIX_360 + acc_in1_in2_angle;

    	angle[0] = (SINT16)base_side_angle;
    	angle[1] = (SINT16)screen_side_angle;
    	angle[2] = (SINT16)acc_in1_in2_angle;

        return 0;
    }
    else
    {
    	lnr_acc_cnt++;
    	if(((abs(screen_side->x) > X_VERTICAL_VALUE) || 
            (abs(base_side->x) > X_VERTICAL_VALUE)) ||
       	   ((abs(g_2_length_screen) > TWOACC_LENGTH_MAX) || 
       	    (abs(g_2_length_screen) < TWOACC_LENGTH_MIN)) ||
       	   ((abs(g_2_length_base) > TWOACC_LENGTH_MAX) || 
       	    (abs(g_2_length_base) < TWOACC_LENGTH_MIN)))
       	   lnr_acc_cnt = 1;
       	   
    	if(lnr_acc_cnt == u16Freq)
    		lnr_acc_cnt = 0;
    }

	return -1;
}

//****************************************************************************************
// Function name: fir_filter()
//
// Description:
//   Do FIR filter
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
SINT32 SENSOR_CODE fir_filter(UINT8 fir_tap, UINT8 st_pt, LPSINT16 ptrCoeff, LPUINT32 ptrData)
{
	UINT8 i;
	UINT8 j;
	SINT32 b32_fir_result;

	b32_fir_result = 0;
	j = 0;

	for(i = 0; i < fir_tap; i++)
	{
		j = (i + st_pt) % fir_tap;
		b32_fir_result = b32_fir_result + (*ptrCoeff) * (*(ptrData + j));
		ptrCoeff++;
	}

	return b32_fir_result;
}

//------------------------------------
//  Fusion Function For 6D (mul: , div: )
//  Based On Accelerometer
//  Roll And Pitch Angle
//  tan:   sin/cos:   mul:   div:
//------------------------------------
int SENSOR_CODE acc_roll_pitch_6d(LPSINT32 acc_roll_ang_post, 
                                  LPSINT32 acc_pitch_ang_post, 
                                  LPSINT32 acc_data,
                                  LPSINT32 acc_roll_ang_pre, 
                                  LPSINT32 acc_pitch_ang_pre)
{
    // Accelerometer Param
    SINT8 acc_infr;
    SINT32 norm_total_in, norm_total_out;
    
    //  Bias And Normalized
    norm_total_in = acc_data[0]*acc_data[0] + 
                    acc_data[1]*acc_data[1] + 
                    acc_data[2]*acc_data[2];
    sqrt_func(&norm_total_out, &norm_total_in);
    
    if(norm_total_out > ((SINT32)LINR_ACC_THR_U))
        acc_infr = 1;
    else if(norm_total_out < ((SINT32)LINR_ACC_THR_D))
        acc_infr = 1;
    else
        acc_infr = 0;

    //+SHAWN20150513
//	    if(gpFConfig->Enable_6D)
//	    {
//	        compass_cal_flag = acc_infr;
//	    }
    //-SHAWN20150513

    // Calculate Roll And Pitch Angle
    if(acc_infr == 0)
    {
        roll_pitch_func(acc_roll_ang_post, acc_pitch_ang_post, 
                        acc_data, 
                        acc_roll_ang_pre, acc_pitch_ang_pre);
    }
    else
    {
        *acc_roll_ang_post  = *acc_roll_ang_pre;
        *acc_pitch_ang_post = *acc_pitch_ang_pre;
    }
    return acc_infr;
}

//------------------------------------
//  Fusion Function For 6D (mul: , div: )
//  Based On Compass
//  Yaw Angle
//  tan:   sin/cos:   mul:   div:
//------------------------------------
void SENSOR_CODE mag_yaw_6d(LPSINT32 yaw_change_en, LPSINT32 mag_yaw_ang_post, LPSINT32 mag_data,
                            LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 mag_yaw_ang_pre)
{
	//
	SINT8 quat2dcm_flag = 0;
	SINT32 mag_cal_x, mag_cal_y;
	SINT32 yaw_data[2];

	// Magnet Calibration (Dynamic)
	mag_horizontal_func(&mag_cal_x, &mag_cal_y, mag_data, roll_ang, pitch_ang);

	// Calulate Yaw Angle
	yaw_data[0] = mag_cal_x;
	yaw_data[1] = mag_cal_y;

	yaw_func(yaw_change_en, mag_yaw_ang_post, &yaw_data[0], quat2dcm_flag, roll_ang, pitch_ang, mag_yaw_ang_pre);

	return;
}

//------------------------------------
//  Compass Heading (mul: , div: )
//  tan:   sin/cos:   mul:   div:
//------------------------------------
int SENSOR_CODE compass_heading(LPSINT32 rmatrix)
{
	UINT8 maping_flag;
	SINT32 point_length, point_length_s;
	SINT32 point_vec[2];
	SINT32 cos_angle, sin_angle, vec_angle;
	SINT16 heading;

	if(abs(*(rmatrix+5)) < 230)
	{
		// XY-Plane
		maping_flag = 0;
		point_vec[0] = *(rmatrix+3);
		point_vec[1] = *(rmatrix+4);
	}
	else
	{
		// XZ-Plane
		maping_flag = 1;
		point_vec[0] = *(rmatrix+6);
		point_vec[1] = *(rmatrix+7);
	}
	point_length_s = point_vec[0]*point_vec[0] + point_vec[1]*point_vec[1];
	sqrt_func(&point_length, &point_length_s);

	// Scale Product
	if(point_length != 0)
	{
		cos_angle = (point_vec[1] << 8)/point_length;

		// Vector Product
		sin_angle = abs(point_vec[0]);
		tan_cordic(&vec_angle, &sin_angle, &cos_angle);

		if((point_vec[0] < 0) && (point_vec[1] > 0))
			heading = FIX_360 - vec_angle;
		else if((point_vec[0] < 0) && (point_vec[1] == 0))
			heading = FIX_270;
		else if((point_vec[0] < 0) && (point_vec[1] < 0))
			heading = FIX_180 - vec_angle;
		else if((point_vec[0] == 0) && (point_vec[1] < 0))
			heading = FIX_180;
		else if((point_vec[0] > 0) && (point_vec[1] < 0))
			heading = FIX_180 + vec_angle;
		else if((point_vec[0] > 0) && (point_vec[1] == 0))
			heading = FIX_90;
		else
			heading = vec_angle;
			
		if(maping_flag == 1)		
		{
			// Map to Y-Point
			heading = FIX_180 + heading;
			if(heading > FIX_360)
				heading = heading - FIX_360;
		}
	}
	//RTL_PRINTF("(%d).\r\n", ((heading*3667) >> 14));
		
	return heading;
}

//------------------------------------
//  Kalman Gain Entry (mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
void SENSOR_CODE kg_value_set(LPSINT16 kalman_matrix, LPSINT16 kalman_value)
{
	int i;

	for(i = 0 ; i < 16 ; i++)
	{
		if(i == 0 || i == 5 || i == 10 || i == 15)
			*(kalman_matrix + i) = *kalman_value;
		else
			*(kalman_matrix + i) = 0;
	}
	return;
}

//------------------------------------
//  Quaternion Matrix (mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
void SENSOR_CODE quatmatrix(LPSINT16 quatern_matrix, LPSINT32 quat_vec)
{
	// Quaternion Matrix
	*(quatern_matrix + 0)  =  *(quat_vec + 0);
	*(quatern_matrix + 1)  =  *(quat_vec + 1);
	*(quatern_matrix + 2)  =  *(quat_vec + 2);
	*(quatern_matrix + 3)  =  *(quat_vec + 3);
	*(quatern_matrix + 4)  = -*(quat_vec + 1);
	*(quatern_matrix + 5)  =  *(quat_vec + 0);
	*(quatern_matrix + 6)  =  *(quat_vec + 3);
	*(quatern_matrix + 7)  = -*(quat_vec + 2);
	*(quatern_matrix + 8)  = -*(quat_vec + 2);
	*(quatern_matrix + 9)  = -*(quat_vec + 3);
	*(quatern_matrix + 10) =  *(quat_vec + 0);
	*(quatern_matrix + 11) =  *(quat_vec + 1);
	*(quatern_matrix + 12) = -*(quat_vec + 3);
	*(quatern_matrix + 13) =  *(quat_vec + 2);
	*(quatern_matrix + 14) = -*(quat_vec + 1);
	*(quatern_matrix + 15) =  *(quat_vec + 0);
	return;
}

//------------------------------------
//  Quaternion --> Euler Angle (mul: , div:)
//  tan:   sin/cos:   mul:   div:
//------------------------------------
void SENSOR_CODE qua2ang_yxz(LPSINT32 euler_angle_post, LPSINT32 euler_angle_pre, LPSINT32 quat_vec)
{
	// Quaternion Entry
	SINT32 q0, q1, q2, q3;

	// Right Shift Function
	SINT32 shift_out, shift_in, shift_bit;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;
	SINT32 sin_pitch_value, cos_pitch_value;

	// Roll/Pitch/Yaw Function Input
	int quat2dcm_flag = 1;
	SINT32 yaw_change_en;
	SINT32 roll_pitch_data[3];
	SINT32 yaw_data_x_t1, yaw_data_x_t2;
	SINT32 yaw_data_y_t1, yaw_data_y_t2;
	SINT32 yaw_data[2];

	//
	q0 = *(quat_vec + 0);
	q1 = *(quat_vec + 1);
	q2 = *(quat_vec + 2);
	q3 = *(quat_vec + 3);

	// ------ Roll And Pitch Angle Based On Quaternion ------
	// roll_pitch_data_x = 2*(q1*q3-q0*q2);
	shift_bit = 7;
	shift_in = q1 * q3 - q0 * q2;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[0] = shift_out;

	// roll_pitch_data_y = 2*(q2*q3+q0*q1);
	shift_bit = 7;
	shift_in = q2 * q3 + q0 * q1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[1] = shift_out;

	// roll_pitch_data_z = q0^2 - q1^2 - q2^2 + q3^2;
	shift_bit = 8;
	shift_in = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[2] = shift_out;

	roll_pitch_func((euler_angle_post + 0), (euler_angle_post + 1), &roll_pitch_data[0], (euler_angle_pre + 0), (euler_angle_pre + 1));

    #if 1 // Boundary Test
    if(!gpFConfig->Enable_Android)
    {
        if(((abs(*(euler_angle_post + 0)) > 379) || (abs(*(euler_angle_post + 0)) < 424)) && 
    	   (abs(acc_data[0]) < 26) && 
    	   (abs(acc_data[2]) < 26) && 
    	   (abs(acc_data[1]) > 230)
    	  )
    	{
    		*(euler_angle_post + 1) = 0;
    	}
    }
    #endif
	
	// ------ Yaw Angle Based On Quaternion ------
	if(*(euler_angle_post + 1) < 0)
		sin_cos_ang = FIX_360 + *(euler_angle_post + 1);
	else
		sin_cos_ang = *(euler_angle_post + 1);

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch_value = sin_value;
	cos_pitch_value = cos_value;

	// (q0^2 + q1^2 - q2^2 - q3^2)*cos_pitch_value
	shift_bit = 8;
	shift_in = (q0 * q0) + (q1 * q1) - (q2 * q2) - (q3 * q3);
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_in = shift_out * cos_pitch_value;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data_x_t1 = shift_out;

	// (2*(q1*q3+q0*q2))*sin_pitch_value
	shift_bit = 7;
	shift_in = q1 * q3 + q0 * q2;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_bit = 8;
	shift_in = shift_out * sin_pitch_value;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data_x_t2 = shift_out;
	yaw_data[0] = yaw_data_x_t1 + yaw_data_x_t2;

	// (2*(q1*q2+q0*q3))*cos_pitch_value
	shift_bit = 7;
	shift_in = q1 * q2 + q0 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_bit = 8;
	shift_in = shift_out * cos_pitch_value;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data_y_t1 = shift_out;

	// (2*(q2*q3-q0*q1))*sin_pitch_value
	shift_bit = 7;
	shift_in = q2 * q3 - q0 * q1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_bit = 8;
	shift_in = shift_out * sin_pitch_value;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data_y_t2 = shift_out;
	yaw_data[1] = yaw_data_y_t1 + yaw_data_y_t2;

	yaw_func(&yaw_change_en, (euler_angle_post + 2), &yaw_data[0], quat2dcm_flag, (euler_angle_post + 0), (euler_angle_post + 1), (euler_angle_pre + 2));

	if((yaw_change_en == 0))
	{
		*(euler_angle_post + 0) = *(euler_angle_pre + 0);
		*(euler_angle_post + 1) = *(euler_angle_pre + 1);
	}

	return;
}

void SENSOR_CODE qua2ang_xyz(LPSINT32 euler_angle_post, LPSINT32 euler_angle_pre, LPSINT32 quat_vec)
{
	// Quaternion Entry
	SINT32 q0, q1, q2, q3;

	// Right Shift Function
	SINT32 shift_out, shift_in, shift_bit;

	// Roll/Pitch/Yaw Function Input
	int quat2dcm_flag = 1;
	SINT32 yaw_change_en;
	SINT32 roll_pitch_data[3];
	SINT32 yaw_data[2];

	//
	q0 = *(quat_vec + 0);
	q1 = *(quat_vec + 1);
	q2 = *(quat_vec + 2);
	q3 = *(quat_vec + 3);

	// ------ Roll And Pitch Angle Based On Quaternion ------
	// roll_pitch_data_x = 2*(q1*q3-q0*q2);
	shift_bit = 7;
	shift_in = q1 * q3 - q0 * q2;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[0] = shift_out;

	// roll_pitch_data_y = 2*(q2*q3+q0*q1);
	shift_bit = 7;
	shift_in = q2 * q3 + q0 * q1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[1] = shift_out;

	// roll_pitch_data_z = q0^2 - q1^2 - q2^2 + q3^2;
	shift_bit = 8;
	shift_in = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	roll_pitch_data[2] = shift_out;

	roll_pitch_func((euler_angle_post + 0), (euler_angle_post + 1), &roll_pitch_data[0], (euler_angle_pre + 0), (euler_angle_pre + 1));

	// ------ Yaw Angle Based On Quaternion ------
	//yaw_data_x = q0^2 + q1^2 - q2^2 - q3^2;
	shift_bit = 8;
	shift_in = q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data[0] = shift_out;

	//yaw_data_y = 2*(q1*q2+q0*q3);
	shift_bit = 7;
	shift_in = q1 * q2 + q0 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	yaw_data[1] = shift_out;

	yaw_func(&yaw_change_en, (euler_angle_post + 2), &yaw_data[0], quat2dcm_flag, (euler_angle_post + 0), (euler_angle_post + 1), (euler_angle_pre + 2));

	if((yaw_change_en == 0))
	{
		*(euler_angle_post + 0) = *(euler_angle_pre + 0);
		*(euler_angle_post + 1) = *(euler_angle_pre + 1);
	}

	return;
}

//------------------------------------
//  Quaternion --> Rotation Matrix (mul:24 , div:0)
//  tan: 0  sin/cos: 0  mul: 24  div: 0
//------------------------------------
void SENSOR_CODE quatern2matrix(LPSINT32 rota_matrix, LPSINT32 quat_vec)
{
	// Quaternion Entry
	SINT32 q0, q1, q2, q3;

	// Right Shift Function
	SINT32 shift_out, shift_in, shift_bit;

	// Quaternion Value
	q0 = *(quat_vec + 0);
	q1 = *(quat_vec + 1);
	q2 = *(quat_vec + 2);
	q3 = *(quat_vec + 3);

	// q0^2 + q1^2 - q2^2 - q3^2
	shift_bit = 8;
	shift_in = (q0 * q0) + (q1 * q1) - (q2 * q2) - (q3 * q3);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 0) = shift_out;

	// 2*(q1*q2+q0*q3)
	shift_bit = 7;
	shift_in = q1 * q2 + q0 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 1) = shift_out;

	// 2*(q1*q3-q0*q2)
	shift_bit = 7;
	shift_in = q1 * q3 - q0 * q2;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 2) = shift_out;

	// 2*(q1*q2-q0*q3)
	shift_bit = 7;
	shift_in = q1 * q2 - q0 * q3;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 3) = shift_out;

	// q0^2 - q1^2 + q2^2 - q3^2
	shift_bit = 8;
	shift_in = (q0 * q0) - (q1 * q1) + (q2 * q2) - (q3 * q3);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 4) = shift_out;

	// 2*(q2*q3+q0*q1)
	shift_bit = 7;
	shift_in = q2 * q3 + q0 * q1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 5) = shift_out;

	// 2*(q1*q3+q0*q2)
	shift_bit = 7;
	shift_in = q1 * q3 + q0 * q2;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 6) = shift_out;

	// 2*(q2*q3-q0*q1)
	shift_bit = 7;
	shift_in = q2 * q3 - q0 * q1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 7) = shift_out;

	// q0^2 - q1^2 - q2^2 + q3^2
	shift_bit = 8;
	shift_in = (q0 * q0) - (q1 * q1) - (q2 * q2) + (q3 * q3);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rota_matrix + 8) = shift_out;
	return;
}

//------------------------------------
//  Euler Angle --> Rotation Matrix (DCM) (mul:16 , div:3)
//  tan: 0  sin/cos: 3  mul: 16  div: 0
//------------------------------------
#if 0 // {
void SENSOR_CODE ang2mat_yxz(LPSINT32 rotate_matrix, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang)
{
	// Right Shift
	SINT32 shift_in, shift_out, shift_bit;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// Roll, Pitch And Yaw Sin/Cos Value
	SINT32 sin_roll, cos_roll;
	SINT32 sin_pitch, cos_pitch;
	SINT32 sin_yaw, cos_yaw;

	// Rotation Matrix Entry Tmp
	SINT32 rm_11_tmp1, rm_11_tmp2, rm_11_tmp3;
	SINT32 rm_21_tmp1, rm_21_tmp2, rm_21_tmp3;
	SINT32 rm_13_tmp1, rm_13_tmp2, rm_13_tmp3;
	SINT32 rm_23_tmp1, rm_23_tmp2, rm_23_tmp3;

	// Right Shift Bit
	shift_bit = 8;

	// Sin/Cos(Roll Angle)
	if(*roll_ang < 0)
		sin_cos_ang = FIX_360 + *roll_ang;
	else
		sin_cos_ang = *roll_ang;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll = sin_value;
	cos_roll = cos_value;

	// Sin/Cos(Pitch Angle)
	if(*pitch_ang < 0)
		sin_cos_ang = FIX_360 + *pitch_ang;
	else
		sin_cos_ang = *pitch_ang;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch = sin_value;
	cos_pitch = cos_value;

	// Sin/Cos(Yaw Angle)
	sin_cos_ang = *yaw_ang;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_yaw = sin_value;
	cos_yaw = cos_value;

	// Ratation Matrix (1,1) (cos_yaw*cos_pitch)-(sin_pitch*sin_roll*sin_yaw)
	shift_in = cos_yaw * cos_pitch;
	rshift_func(&rm_11_tmp1, &shift_in, shift_bit);

	shift_in = sin_pitch * sin_roll;
	rshift_func(&rm_11_tmp2, &shift_in, shift_bit);
	shift_in = rm_11_tmp2 * sin_yaw;
	rshift_func(&rm_11_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 0) = rm_11_tmp1 - rm_11_tmp3;

	// Ratation Matrix (2,1) (sin_yaw*cos_pitch)+(sin_pitch*sin_roll*cos_yaw)
	shift_in = sin_yaw * cos_pitch;
	rshift_func(&rm_21_tmp1, &shift_in, shift_bit);

	shift_in = sin_pitch * sin_roll;
	rshift_func(&rm_21_tmp2, &shift_in, shift_bit);
	shift_in = rm_21_tmp2 * cos_yaw;
	rshift_func(&rm_21_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 1) = rm_21_tmp1 + rm_21_tmp3;

	// Ratation Matrix (3,1) -(sin_pitch*cos_roll)
	shift_in = -(sin_pitch * cos_roll);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 2) = shift_out;

	// Ratation Matrix (1,2) -(cos_roll*sin_yaw)
	shift_in = -(cos_roll * sin_yaw);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 3) = shift_out;

	// Ratation Matrix (2,2) (cos_roll*cos_yaw)
	shift_in = cos_roll * cos_yaw;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 4) = shift_out;

	// Ratation Matrix (3,2) sin_roll
	*(rotate_matrix + 5) = sin_roll;

	// Ratation Matrix (1,3) (cos_pitch*sin_roll*sin_yaw)+(sin_pitch*cos_yaw);
	shift_in = cos_pitch * sin_roll;
	rshift_func(&rm_13_tmp1, &shift_in, shift_bit);
	shift_in = rm_13_tmp1 * sin_yaw;
	rshift_func(&rm_13_tmp2, &shift_in, shift_bit);

	shift_in = sin_pitch * cos_yaw;
	rshift_func(&rm_13_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 6) = rm_13_tmp2 + rm_13_tmp3;

	// Ratation Matrix (2,3) -(cos_yaw*cos_pitch*sin_roll)+(sin_yaw*sin_pitch);
	shift_in = cos_yaw * cos_pitch;
	rshift_func(&rm_23_tmp1, &shift_in, shift_bit);
	shift_in = rm_23_tmp1 * sin_roll;
	rshift_func(&rm_23_tmp2, &shift_in, shift_bit);

	shift_in = sin_yaw * sin_pitch;
	rshift_func(&rm_23_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 7) = -rm_23_tmp2 + rm_23_tmp3;

	// Ratation Matrix (3,3)
	shift_in = cos_pitch * cos_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 8) = shift_out;

	return;
}

void SENSOR_CODE ang2mat_xyz(LPSINT32 rotate_matrix, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang)
{
	// Right Shift
	SINT32 shift_in, shift_out, shift_bit;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// Roll, Pitch And Yaw Sin/Cos Value
	SINT32 sin_roll, cos_roll;
	SINT32 sin_pitch, cos_pitch;
	SINT32 sin_yaw, cos_yaw;

	// Rotation Matrix Entry Tmp
	SINT32 rm_12_tmp1, rm_12_tmp2, rm_12_tmp3;
	SINT32 rm_22_tmp1, rm_22_tmp2, rm_22_tmp3;
	SINT32 rm_13_tmp1, rm_13_tmp2, rm_13_tmp3;
	SINT32 rm_23_tmp1, rm_23_tmp2, rm_23_tmp3;

	// Right Shift Bit
	shift_bit = 8;

	// Sin/Cos(Roll Angle)
	if(*roll_ang < 0)
		sin_cos_ang = FIX_360 + *roll_ang;
	else
		sin_cos_ang = *roll_ang;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll = sin_value;
	cos_roll = cos_value;

	// Sin/Cos(Pitch Angle)
	if(*pitch_ang < 0)
		sin_cos_ang = FIX_360 + *pitch_ang;
	else
		sin_cos_ang = *pitch_ang;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch = sin_value;
	cos_pitch = cos_value;

	// Sin/Cos(Yaw Angle)
	sin_cos_ang = *yaw_ang;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_yaw = sin_value;
	cos_yaw = cos_value;

	// Ratation Matrix (1,1)
	shift_in = cos_pitch * cos_yaw;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 0) = shift_out;

	// Ratation Matrix (2,1)
	shift_in = cos_pitch * sin_yaw;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 1) = shift_out;

	// Ratation Matrix (3,1)
	*(rotate_matrix + 2) = -sin_pitch;

	// Ratation Matrix (1,2) (cos_yaw*sin_pitch*sin_roll)-(cos_roll*sin_yaw)
	shift_in = cos_yaw * sin_pitch;
	rshift_func(&rm_12_tmp1, &shift_in, shift_bit);
	shift_in = rm_12_tmp1 * sin_roll;
	rshift_func(&rm_12_tmp2, &shift_in, shift_bit);
	shift_in = cos_roll * sin_yaw;
	rshift_func(&rm_12_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 3) = rm_12_tmp2 - rm_12_tmp3;

	// Ratation Matrix (2,2) (cos_roll*cos_yaw)+(sin_pitch*sin_roll*sin_yaw)
	shift_in = cos_roll * cos_yaw;
	rshift_func(&rm_22_tmp1, &shift_in, shift_bit);
	shift_in = sin_pitch * sin_roll;
	rshift_func(&rm_22_tmp2, &shift_in, shift_bit);
	shift_in = rm_22_tmp2 * sin_yaw;
	rshift_func(&rm_22_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 4) = rm_22_tmp1 + rm_22_tmp3;

	// Ratation Matrix (3,2)
	shift_in = cos_pitch * sin_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 5) = shift_out;

	// Ratation Matrix (1,3) (cos_yaw*sin_pitch*cos_roll)+(sin_roll*sin_yaw);
	shift_in = cos_yaw * sin_pitch;
	rshift_func(&rm_13_tmp1, &shift_in, shift_bit);
	shift_in = rm_13_tmp1 * cos_roll;
	rshift_func(&rm_13_tmp2, &shift_in, shift_bit);
	shift_in = sin_roll * sin_yaw;
	rshift_func(&rm_13_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 6) = rm_13_tmp2 + rm_13_tmp3;

	// Ratation Matrix (2,3) (sin_pitch*cos_roll*sin_yaw)-(sin_roll*cos_yaw);
	shift_in = sin_roll * cos_yaw;
	rshift_func(&rm_23_tmp1, &shift_in, shift_bit);
	shift_in = sin_pitch * cos_roll;
	rshift_func(&rm_23_tmp2, &shift_in, shift_bit);
	shift_in = rm_23_tmp2 * sin_yaw;
	rshift_func(&rm_23_tmp3, &shift_in, shift_bit);
	*(rotate_matrix + 7) = rm_23_tmp3 - rm_23_tmp1;

	// Ratation Matrix (3,3)
	shift_in = cos_pitch * cos_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	*(rotate_matrix + 8) = shift_out;

	return;
}
#endif // } 0

//------------------------------------
//  Euler Angle --> Quaternion (mul:16 , div:3)
//  tan: 0  sin/cos: 3  mul: 16  div: 0
//------------------------------------
void SENSOR_CODE ang2qua_yxz(LPSINT32 quat_vec, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang)
{
	// Right Shift
	SINT32 shift_in, /*shift_out,*/ shift_bit;

	// For Data Normalized (1/0 -> Quaternion/Raw Data)
	SINT32 norm_total, quat_data_en;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// Roll, Pitch And Yaw Sin/Cos Value
	SINT32 /*roll_ang_half,*/ sin_roll_2, cos_roll_2;
	SINT32 /*pitch_ang_half,*/ sin_pitch_2, cos_pitch_2;
	SINT32 /*yaw_ang_half,*/ sin_yaw_2, cos_yaw_2;

	// Rotation Matrix Entry Tmp
	SINT32 q0_tmp, quat_vec_0_tmp1, quat_vec_0_tmp2;
	SINT32 q1_tmp, quat_vec_1_tmp1, quat_vec_1_tmp2;
	SINT32 q2_tmp, quat_vec_2_tmp1, quat_vec_2_tmp2;
	SINT32 q3_tmp, quat_vec_3_tmp1, quat_vec_3_tmp2;

	// Right Shift Bit
	shift_bit = 8;

	// Sin(Roll_Angle/2), Cos(Roll_Angle/2)
	if(*roll_ang < 0)
		sin_cos_ang = FIX_360 + (*roll_ang);
	else
		sin_cos_ang = *roll_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll_2 = sin_value;
	cos_roll_2 = cos_value;

	// Sin(Pitch_Angle/2), Cos(Pitch_Angle/2)
	if(*pitch_ang < 0)
		sin_cos_ang = FIX_360 + (*pitch_ang);
	else
		sin_cos_ang = *pitch_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch_2 = sin_value;
	cos_pitch_2 = cos_value;

	// Sin(Yaw_Angle/2), Cos(Yaw_Angle/2)
	if(*yaw_ang < 0)
		sin_cos_ang = FIX_360 + (*yaw_ang);
	else if(*yaw_ang > FIX_360)
		sin_cos_ang = *yaw_ang - FIX_360;
	else
		sin_cos_ang = *yaw_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_yaw_2 = sin_value;
	cos_yaw_2 = cos_value;

	// Quaternion Q0 (cos(yaw/2)*cos(roll/2)*cos(pitch/2) - sin(yaw/2)*sin(roll/2)*sin(pitch/2))
	shift_in = cos_yaw_2 * cos_roll_2;
	rshift_func(&q0_tmp, &shift_in, shift_bit);
	shift_in = q0_tmp * cos_pitch_2;
	rshift_func(&quat_vec_0_tmp1, &shift_in, shift_bit);

	shift_in = sin_yaw_2 * sin_roll_2;
	rshift_func(&q0_tmp, &shift_in, shift_bit);
	shift_in = q0_tmp * sin_pitch_2;
	rshift_func(&quat_vec_0_tmp2, &shift_in, shift_bit);
	*(quat_vec + 0) = quat_vec_0_tmp1 - quat_vec_0_tmp2;

	// Quaternion Q1 (cos(yaw/2)*sin(roll/2)*cos(pitch/2) - sin(yaw/2)*cos(roll/2)*sin(pitch/2))
	shift_in = cos_yaw_2 * sin_roll_2;
	rshift_func(&q1_tmp, &shift_in, shift_bit);
	shift_in = q1_tmp * cos_pitch_2;
	rshift_func(&quat_vec_1_tmp1, &shift_in, shift_bit);

	shift_in = sin_yaw_2 * cos_roll_2;
	rshift_func(&q1_tmp, &shift_in, shift_bit);
	shift_in = q1_tmp * sin_pitch_2;
	rshift_func(&quat_vec_1_tmp2, &shift_in, shift_bit);
	*(quat_vec + 1) = quat_vec_1_tmp1 - quat_vec_1_tmp2;

	// Quaternion Q2 (sin(yaw/2)*sin(roll/2)*cos(pitch/2) + cos(yaw/2)*cos(roll/2)*sin(pitch/2))
	shift_in = sin_yaw_2 * sin_roll_2;
	rshift_func(&q2_tmp, &shift_in, shift_bit);
	shift_in = q2_tmp * cos_pitch_2;
	rshift_func(&quat_vec_2_tmp1, &shift_in, shift_bit);

	shift_in = cos_yaw_2 * cos_roll_2;
	rshift_func(&q2_tmp, &shift_in, shift_bit);
	shift_in = q2_tmp * sin_pitch_2;
	rshift_func(&quat_vec_2_tmp2, &shift_in, shift_bit);
	*(quat_vec + 2) = quat_vec_2_tmp1 + quat_vec_2_tmp2;

	// Quaternion Q3 (sin(yaw/2)*cos(roll/2)*cos(pitch/2) + cos(yaw/2)*sin(roll/2)*sin(pitch/2))
	shift_in = sin_yaw_2 * cos_roll_2;
	rshift_func(&q3_tmp, &shift_in, shift_bit);
	shift_in = q3_tmp * cos_pitch_2;
	rshift_func(&quat_vec_3_tmp1, &shift_in, shift_bit);

	shift_in = cos_yaw_2 * sin_roll_2;
	rshift_func(&q3_tmp, &shift_in, shift_bit);
	shift_in = q3_tmp * sin_pitch_2;
	rshift_func(&quat_vec_3_tmp2, &shift_in, shift_bit);
	*(quat_vec + 3) = quat_vec_3_tmp1 + quat_vec_3_tmp2;

	// Normalized
	quat_data_en = 1;
	norm_func(quat_vec, &norm_total, &quat_data_en);

	return;
}

void SENSOR_CODE ang2qua_xyz(LPSINT32 quat_vec, LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang)
{
	// Right Shift
	SINT32 shift_in, /*shift_out,*/ shift_bit;

	// For Data Normalized (1/0 -> Quaternion/Raw Data)
	SINT32 norm_total, quat_data_en;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// Roll, Pitch And Yaw Sin/Cos Value
	SINT32 /*roll_ang_half,*/ sin_roll_2, cos_roll_2;
	SINT32 /*pitch_ang_half,*/ sin_pitch_2, cos_pitch_2;
	SINT32 /*yaw_ang_half,*/ sin_yaw_2, cos_yaw_2;

	// Rotation Matrix Entry Tmp
	SINT32 q0_tmp, quat_vec_0_tmp1, quat_vec_0_tmp2;
	SINT32 q1_tmp, quat_vec_1_tmp1, quat_vec_1_tmp2;
	SINT32 q2_tmp, quat_vec_2_tmp1, quat_vec_2_tmp2;
	SINT32 q3_tmp, quat_vec_3_tmp1, quat_vec_3_tmp2;

	// Right Shift Bit
	shift_bit = 8;

	// Sin(Roll_Angle/2), Cos(Roll_Angle/2)
	if(*roll_ang < 0)
		sin_cos_ang = FIX_360 + (*roll_ang);
	else
		sin_cos_ang = *roll_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll_2 = sin_value;
	cos_roll_2 = cos_value;

	// Sin(Pitch_Angle/2), Cos(Pitch_Angle/2)
	if(*pitch_ang < 0)
		sin_cos_ang = FIX_360 + (*pitch_ang);
	else
		sin_cos_ang = *pitch_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch_2 = sin_value;
	cos_pitch_2 = cos_value;

	// Sin(Yaw_Angle/2), Cos(Yaw_Angle/2)
	if(*yaw_ang < 0)
		sin_cos_ang = FIX_360 + (*yaw_ang);
	else if(*yaw_ang > FIX_360)
		sin_cos_ang = *yaw_ang - FIX_360;
	else
		sin_cos_ang = *yaw_ang;
	sin_cos_ang = sin_cos_ang >> 1;

	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_yaw_2 = sin_value;
	cos_yaw_2 = cos_value;

	// Quaternion Q0 (cos(yaw/2)*cos(pitch/2)*cos(roll/2) + sin(yaw/2)*sin(pitch/2)*sin(roll/2))
	shift_in = cos_yaw_2 * cos_pitch_2;
	rshift_func(&q0_tmp, &shift_in, shift_bit);
	shift_in = q0_tmp * cos_roll_2;
	rshift_func(&quat_vec_0_tmp1, &shift_in, shift_bit);

	shift_in = sin_yaw_2 * sin_pitch_2;
	rshift_func(&q0_tmp, &shift_in, shift_bit);
	shift_in = q0_tmp * sin_roll_2;
	rshift_func(&quat_vec_0_tmp2, &shift_in, shift_bit);
	*(quat_vec + 0) = quat_vec_0_tmp1 + quat_vec_0_tmp2;

	// Quaternion Q1 (cos(yaw/2)*cos(pitch/2)*sin(roll/2) - sin(yaw/2)*sin(pitch/2)*cos(roll/2))
	shift_in = cos_yaw_2 * cos_pitch_2;
	rshift_func(&q1_tmp, &shift_in, shift_bit);
	shift_in = q1_tmp * sin_roll_2;
	rshift_func(&quat_vec_1_tmp1, &shift_in, shift_bit);

	shift_in = sin_yaw_2 * sin_pitch_2;
	rshift_func(&q1_tmp, &shift_in, shift_bit);
	shift_in = q1_tmp * cos_roll_2;
	rshift_func(&quat_vec_1_tmp2, &shift_in, shift_bit);
	*(quat_vec + 1) = quat_vec_1_tmp1 - quat_vec_1_tmp2;

	// Quaternion Q2 (cos(yaw/2)*sin(pitch/2)*cos(roll/2) + sin(yaw/2)*cos(pitch/2)*sin(roll/2))
	shift_in = cos_yaw_2 * sin_pitch_2;
	rshift_func(&q2_tmp, &shift_in, shift_bit);
	shift_in = q2_tmp * cos_roll_2;
	rshift_func(&quat_vec_2_tmp1, &shift_in, shift_bit);

	shift_in = sin_yaw_2 * cos_pitch_2;
	rshift_func(&q2_tmp, &shift_in, shift_bit);
	shift_in = q2_tmp * sin_roll_2;
	rshift_func(&quat_vec_2_tmp2, &shift_in, shift_bit);
	*(quat_vec + 2) = quat_vec_2_tmp1 + quat_vec_2_tmp2;

	// Quaternion Q3 (sin(yaw/2)*cos(pitch/2)*cos(roll/2) - cos(yaw/2)*sin(pitch/2)*sin(roll/2))
	shift_in = sin_yaw_2 * cos_pitch_2;
	rshift_func(&q3_tmp, &shift_in, shift_bit);
	shift_in = q3_tmp * cos_roll_2;
	rshift_func(&quat_vec_3_tmp1, &shift_in, shift_bit);

	shift_in = cos_yaw_2 * sin_pitch_2;
	rshift_func(&q3_tmp, &shift_in, shift_bit);
	shift_in = q3_tmp * sin_roll_2;
	rshift_func(&quat_vec_3_tmp2, &shift_in, shift_bit);
	*(quat_vec + 3) = quat_vec_3_tmp1 - quat_vec_3_tmp2;

	// Normalized
	quat_data_en = 1;
	norm_func(quat_vec, &norm_total, &quat_data_en);

	return;
}

//------------------------------------
//  Rotation Matrix --> Quaternion (mul:10 , div:3)
//  tan: 0  sin/cos: 0  mul: 0  div: 3  sqrt: 1
//------------------------------------
#if 0 //{ 
void SENSOR_CODE matrix2quatern(LPSINT32 quat_vec, LPSINT32 rota_matrix)
{
	//
	SINT32 matrix_trace, S;

	// For Data Normalized (1/0 -> Quaternion/Raw Data)
	SINT32 norm_total, quat_data_en;

	// For Sqrt Function
	SINT32 sqrt_in, sqrt_out;

	// Rotation Matrix Trace
	matrix_trace = (SINT32)(*(rota_matrix + 0)) + (SINT32)(*(rota_matrix + 4)) + (SINT32)(*(rota_matrix + 8));

	if(matrix_trace > 0)
	{
		// S = sqrt(matrix_trace+1.0)*2;
		sqrt_in = matrix_trace + (1 << 8);
		sqrt_func(&sqrt_out, &sqrt_in);
		S = sqrt_out << 1;

		*(quat_vec + 0) = S >> 2;
		*(quat_vec + 1) = (((SINT32)(*(rota_matrix + 5)) - (SINT32)(*(rota_matrix + 7))) << 8) / S;
		*(quat_vec + 2) = (((SINT32)(*(rota_matrix + 6)) - (SINT32)(*(rota_matrix + 2))) << 8) / S;
		*(quat_vec + 3) = (((SINT32)(*(rota_matrix + 1)) - (SINT32)(*(rota_matrix + 3))) << 8) / S;
	}
	else if((*(rota_matrix + 0) > *(rota_matrix + 4)) && (*(rota_matrix + 0) > *(rota_matrix + 8)))
	{
		// S = sqrt(1.0 + *(rota_matrix+0) - *(rota_matrix+4) - *(rota_matrix+8))*2;
		sqrt_in = (1 << 8) + (SINT32)(*(rota_matrix + 0)) - (SINT32)(*(rota_matrix + 4)) - (SINT32)(*(rota_matrix + 8));
		sqrt_func(&sqrt_out, &sqrt_in);
		S = sqrt_out << 1;

		*(quat_vec + 0) = (((SINT32)(*(rota_matrix + 5)) - (SINT32)(*(rota_matrix + 7))) << 8) / S;
		*(quat_vec + 1) = S >> 2;
		*(quat_vec + 2) = (((SINT32)(*(rota_matrix + 1)) + (SINT32)(*(rota_matrix + 3))) << 8) / S;
		*(quat_vec + 3) = (((SINT32)(*(rota_matrix + 2)) + (SINT32)(*(rota_matrix + 6))) << 8) / S;
	}
	else if(*(rota_matrix + 4) > *(rota_matrix + 8))
	{
		// S = sqrt(1.0 + *(rota_matrix+4) - *(rota_matrix+0) - *(rota_matrix+8))*2;
		sqrt_in = (1 << 8) + (SINT32)(*(rota_matrix + 4)) - (SINT32)(*(rota_matrix + 0)) - (SINT32)(*(rota_matrix + 8));
		sqrt_func(&sqrt_out, &sqrt_in);
		S = sqrt_out << 1;

		*(quat_vec + 0) = (((SINT32)(*(rota_matrix + 6)) - (SINT32)(*(rota_matrix + 2))) << 8) / S;
		*(quat_vec + 1) = (((SINT32)(*(rota_matrix + 1)) + (SINT32)(*(rota_matrix + 3))) << 8) / S;
		*(quat_vec + 2) = S >> 2;
		*(quat_vec + 3) = (((SINT32)(*(rota_matrix + 5)) + (SINT32)(*(rota_matrix + 7))) << 8) / S;
	}
	else
	{
		// S = sqrt(1.0 + *(rota_matrix+8) - *(rota_matrix+0) - *(rota_matrix+4))*2;
		sqrt_in = (1 << 8) + (SINT32)(*(rota_matrix + 8)) - (SINT32)(*(rota_matrix + 0)) - (SINT32)(*(rota_matrix + 4));
		sqrt_func(&sqrt_out, &sqrt_in);
		S = sqrt_out << 1;

		*(quat_vec + 0) = (((SINT32)(*(rota_matrix + 1)) - (SINT32)(*(rota_matrix + 3))) << 8) / S;
		*(quat_vec + 1) = (((SINT32)(*(rota_matrix + 2)) + (SINT32)(*(rota_matrix + 6))) << 8) / S;
		*(quat_vec + 2) = (((SINT32)(*(rota_matrix + 5)) + (SINT32)(*(rota_matrix + 7))) << 8) / S;
		*(quat_vec + 3) = S >> 2;
	}

	// Normalized
	quat_data_en = 1;
	norm_func(quat_vec, &norm_total, &quat_data_en);

	return;
}
#endif // } matrix2quatern()

//------------------------------------
//  Data Normailized Function(mul:14 , div:4)
//  tan: 0  sin/cos: 0  mul: 4  div: 4  sqrt: 1
//------------------------------------
void SENSOR_CODE norm_func(LPSINT32 norm_data, LPSINT32 norm_value, LPSINT32 quat_data_en)
{
	SINT32 data_0_2, data_1_2, data_2_2, data_3_2, data_total;

	// (Data)^2
	data_0_2 = ((SINT32)(*(norm_data + 0)) * (SINT32)(*(norm_data + 0)));
	data_1_2 = ((SINT32)(*(norm_data + 1)) * (SINT32)(*(norm_data + 1)));
	data_2_2 = ((SINT32)(*(norm_data + 2)) * (SINT32)(*(norm_data + 2)));

	// Quaternion/Raw Data -> 4/3
	if((*quat_data_en == 1))
		data_3_2 = ((SINT32)(*(norm_data + 3)) * (SINT32)(*(norm_data + 3)));
	else
		data_3_2 = 0;

	data_total = data_0_2 + data_1_2 + data_2_2 + data_3_2;

	// sqrt(data_total);
	sqrt_func(norm_value, &data_total);

	// Normalized
	if(*norm_value == 0)
	{
		*(norm_data + 0) = 0;
		*(norm_data + 1) = 0;
		*(norm_data + 2) = 0;
		*(norm_data + 3) = 0;
	}
	else
	{
		*(norm_data + 0) = (((SINT32)(*(norm_data + 0)) << 8) / (*norm_value));
		*(norm_data + 1) = (((SINT32)(*(norm_data + 1)) << 8) / (*norm_value));
		*(norm_data + 2) = (((SINT32)(*(norm_data + 2)) << 8) / (*norm_value));

		if((*quat_data_en == 1))
			*(norm_data + 3) = (((SINT32)(*(norm_data + 3)) << 8) / (*norm_value));
		else
			*(norm_data + 3) = 0;
	}
	return;
}

//------------------------------------
//  Sqrt Function (mul: 1~10 , div:0)
//  tan: 0  sin/cos: 0  mul: 1~15  div: 0
//------------------------------------
#ifdef __ENABLE_CODE__ //{
void SENSOR_CODE sqrt_func1(char* File, int Line, LPSINT32 out, LPSINT32 in)
#else
void SENSOR_CODE sqrt_func(LPSINT32 out, LPSINT32 in)
#endif //} __ENABLE_CODE__
{
	SINT32 range_low, range_high, mid, mid_2;
	SINT32 small = 1;

	if(*in < 0)
	{
#ifdef __ENABLE_CODE__ //{
		RTL_PRINTF("Error Format_%s_%d.\r\n", File, Line);
#else
		RTL_PRINTF("Error Format\r\n");
#endif //}  __ENABLE_CODE__
	}
	else if(*in == 0)
	{
		*out = 0;
	}
	else
	{
		range_low = 1;
		range_high = *in;

		while(range_high - range_low > small)
		{
			mid = (range_low + range_high) >> 1;
			if(mid >= 32767)
			{
				mid_2 = *in;
				range_high = mid;
			}
			else
			{
				mid_2 = (mid * mid);
			if(mid_2 > *in)
				range_high = mid;
			else
				range_low = mid;
		}
		}
		*out = range_low;
	}
	return;
}

//------------------------------------
//  Magnetometer Calibration Function
//------------------------------------
void SENSOR_CODE mag_horiz_yxz(LPSINT32 mag_cal_x, LPSINT32 mag_cal_y, LPSINT32 mag_data, /*LPSINT16 mag_bias,*/
                                     LPSINT32 roll_ang, LPSINT32 pitch_ang)
{
	// Right Shift
	SINT32 shift_out, shift_in, shift_bit;

	// Data/Bias
	SINT32 mag_x, mag_y, mag_z;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;
	SINT32 roll_ang_360, pitch_ang_360;
	SINT32 sin_roll, cos_roll, sin_pitch, cos_pitch;

	// Magnetometer Horizontal Data
	SINT32 mag_y_h_tmp1, mag_y_h_tmp2, mag_y_h_tmp3;

	// Sin(Roll Angle) And Cos(Roll Angle)
	if(*roll_ang < 0)
		roll_ang_360 = FIX_360 + *roll_ang;
	else
		roll_ang_360 = *roll_ang;

	sin_cos_ang = roll_ang_360;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll = sin_value;
	cos_roll = cos_value;

	// Sin(Pitch Angle) And Cos(Pitch Angle)
	if(*pitch_ang < 0)
		pitch_ang_360 = FIX_360 + *pitch_ang;
	else
		pitch_ang_360 = *pitch_ang;

	sin_cos_ang = pitch_ang_360;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch = sin_value;
	cos_pitch = cos_value;

	// Mag Data-Bais
	mag_x = (*(mag_data + 0)) /*- (*(mag_bias + 0))*/;
	mag_y = (*(mag_data + 1)) /*- (*(mag_bias + 1))*/;
	mag_z = (*(mag_data + 2)) /*- (*(mag_bias + 2))*/;

	// mag_x(Horizontal) --> cos_pitch*mag_x + sin_pitch*mag_z
	shift_bit = 8;
	shift_in = (mag_x * cos_pitch) + (mag_z * sin_pitch);
	rshift_func(&shift_out, &shift_in, shift_bit);
	*mag_cal_x = shift_out;

	// mag_y(Horizontal) --> sin_roll*sin_pitch*mag_x + cos_roll*mag_y - sin_roll*cos_pitch*mag_z
	shift_in = mag_x * sin_pitch;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_in = sin_roll * shift_out;
	rshift_func(&shift_out, &shift_in, shift_bit);
	mag_y_h_tmp1 = shift_out;

	shift_in = mag_y * cos_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	mag_y_h_tmp2 = shift_out;

	shift_in = mag_z * cos_pitch;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_in = sin_roll * shift_out;
	rshift_func(&shift_out, &shift_in, shift_bit);
	mag_y_h_tmp3 = shift_out;
	*mag_cal_y = (mag_y_h_tmp1 + mag_y_h_tmp2 - mag_y_h_tmp3);

	//RTL_PRINTF("(%d:%d:%d:%d), (%d:%d:%d), (%d:%d).\r\n", sin_roll, cos_roll, sin_pitch, cos_pitch, mag_x, mag_y, mag_z, *mag_cal_x, *mag_cal_y);

	return;
}

void SENSOR_CODE mag_horiz_xyz(LPSINT32 mag_cal_x, LPSINT32 mag_cal_y, LPSINT32 mag_data, /*LPSINT16 mag_bias,*/
                                     LPSINT32 roll_ang, LPSINT32 pitch_ang)
{
	// Right Shift
	SINT32 shift_out, shift_in, shift_bit;

	// Data/Bias
	SINT32 mag_x, mag_y, mag_z;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;
	SINT32 roll_ang_360, pitch_ang_360;
	SINT32 sin_roll, cos_roll, sin_pitch, cos_pitch;

	// Magnetometer Horizontal Data
	SINT32 mag_x_h_tmp1, mag_x_h_tmp2, mag_x_h_tmp3;

	// Sin(Roll Angle) And Cos(Roll Angle)
	if(*roll_ang < 0)
		roll_ang_360 = FIX_360 + *roll_ang;
	else
		roll_ang_360 = *roll_ang;

	sin_cos_ang = roll_ang_360;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_roll = sin_value;
	cos_roll = cos_value;

	// Sin(Pitch Angle) And Cos(Pitch Angle)
	if(*pitch_ang < 0)
		pitch_ang_360 = FIX_360 + *pitch_ang;
	else
		pitch_ang_360 = *pitch_ang;

	sin_cos_ang = pitch_ang_360;
	sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
	sin_pitch = sin_value;
	cos_pitch = cos_value;

	// Mag Data-Bais
	mag_x = (*(mag_data + 0))/* - (*(mag_bias + 0))*/;
	mag_y = (*(mag_data + 1))/* - (*(mag_bias + 1))*/;
	mag_z = (*(mag_data + 2))/* - (*(mag_bias + 2))*/;

	// mag_x(Horizontal) --> cos_pitch*mag_x + sin_pitch*sin_roll*mag_y + sin_pitch*cos_roll*mag_z
	shift_bit = 8;
	shift_in = mag_x * cos_pitch;
	rshift_func(&mag_x_h_tmp1, &shift_in, shift_bit);

	shift_in = sin_pitch * sin_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_in = mag_y * shift_out;
	rshift_func(&mag_x_h_tmp2, &shift_in, shift_bit);

	shift_in = sin_pitch * cos_roll;
	rshift_func(&shift_out, &shift_in, shift_bit);
	shift_in = mag_z * shift_out;
	rshift_func(&mag_x_h_tmp3, &shift_in, shift_bit);

	*mag_cal_x = mag_x_h_tmp1 + mag_x_h_tmp2 + mag_x_h_tmp3;

	// mag_y(Horizontal) --> cos_roll*mag_y - sin_roll*mag_z
	shift_in = (mag_y * cos_roll) - (mag_z * sin_roll);
	rshift_func(&shift_out, &shift_in, shift_bit);

	*mag_cal_y = shift_out;

	return;
}

//------------------------------------
//  Yaw Angle Function (mul:10 , div:3)
//  tan: 1  sin/cos: 2  mul: 7  div: 0
//------------------------------------
void SENSOR_CODE yaw_func(LPSINT32 yaw_change_en, LPSINT32 yaw_ang, LPSINT32 data_in, SINT32 quat2dcm_flag,
                          LPSINT32 roll_ang, LPSINT32 pitch_ang, LPSINT32 yaw_ang_cs)
{
	// Tan Function
	SINT32 atan_numr, atan_deno, tan_ang, tan_ang_tmp;

	//
	int q2d_sign;
	SINT32 data_x, data_y;
	SINT32 abs_roll_ang, abs_pitch_ang;

	//
	int fix90_flag;

	// Data Value
	data_x = *(data_in + 0);
	data_y = *(data_in + 1);

	// Yaw Angle Based On Quaternion-2-DCM Signal
	if(quat2dcm_flag == 0)
		q2d_sign = 1;
	else
		q2d_sign = -1;

	// For Win8 Spec.
	if(quat2dcm_flag == 0)
		fix90_flag = 1;
	else
		fix90_flag = 0;

	// ----------- Calculate Yaw Angle ------------------
	atan_numr = data_y;
	atan_deno = data_x;

	// Arc-Tan(Value) --> Yaw Angle (-90~90)
	tan_cordic(&tan_ang, &atan_numr, &atan_deno);

	//
	*yaw_change_en = 1;
	abs_roll_ang = abs(*roll_ang);
	abs_pitch_ang = abs(*pitch_ang);

	if(atan_deno == 0 || atan_numr == 0)
	{
		if((abs_roll_ang > 350) && (abs_roll_ang < 420))
			*yaw_change_en = 0;
		else if((abs_pitch_ang > 350) && (abs_pitch_ang < 420))
			*yaw_change_en = 0;
	}

	// Yaw Angle Map To 0~360
	if(atan_deno == 0 || atan_numr == 0)
		tan_ang_tmp = FIX_360 - (*yaw_ang_cs); // For Windows8
	else if(atan_deno > 0 && atan_numr > 0)
		tan_ang_tmp = (tan_ang - FIX_90 * fix90_flag) * q2d_sign;
	else if(atan_deno < 0)
		tan_ang_tmp = (tan_ang + FIX_180 - FIX_90 * fix90_flag) * q2d_sign;
	else if(atan_deno > 0 && atan_numr < 0)
		tan_ang_tmp = (tan_ang + FIX_360 - FIX_90 * fix90_flag) * q2d_sign;
	else
		tan_ang_tmp = tan_ang;

	// 0~360
	if(tan_ang_tmp < 0)
		tan_ang_tmp = FIX_360 + tan_ang_tmp;

	// For Win8 Spec.
	tan_ang_tmp = FIX_360 - tan_ang_tmp;

	if(tan_ang_tmp < 0)
		*yaw_ang = FIX_360 + tan_ang_tmp;
	else
		*yaw_ang = tan_ang_tmp;
	//RTL_PRINTF("(%d:%d).\r\n", tan_ang, *yaw_ang);

	return;
}

//------------------------------------
//  Roll And Pitch Angle Function (mul:13 , div:7)
//  tan: 3  sin/cos: 4  mul: 4  div: 0
//------------------------------------
void SENSOR_CODE roll_pitch_yxz(LPSINT32 roll_ang, LPSINT32 pitch_ang, 
                                LPSINT32 data_in,
                                LPSINT32 roll_ang_crct, LPSINT32 pitch_ang_crct)
{
	// Data Value
	LPSenAxisVec data = (LPSenAxisVec)data_in;

	// Tan Function
	SINT32 atan_numr, atan_deno, tan_ang;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// For Roll Angle
	SINT32 roll_ang_t1 = 0, roll_ang_t2 = 0, roll_ang_t1_360, roll_ang_t2_360;
	SINT32 sin_roll_value_t1 = 0, cos_roll_value_t1 = 0, sin_roll_value_t2 = 0, cos_roll_value_t2 = 0;

	// For Pitch Angle
	SINT32 pitch_ang_t = 0;
	SINT32 sin_pitch_value = 0, cos_pitch_value = 0;

	// Sign Value
	SINT32 roll_ang_no_change, pitch_ang_no_change;
	SINT32 sign_cos_pitch, sign_sin_pitch;
	SINT32 sign_cos_roll_t1, sign_sin_roll_t1, sign_cos_roll_t2, sign_sin_roll_t2;
	SINT32 angle_val_data_x, angle_val_data_y, angle_val_data_z;
	SINT32 sign_x, sign_y, sign_z;

	// ----------- Calculate Roll Angle ------------------
	// Numerator And Denomonator of Arc Tan For Angle Roll
	atan_numr = data->y;
	atan_deno = data->x * data->x + data->z * data->z;
	sqrt_func(&atan_deno, &atan_deno);

	if((atan_deno == 0))
	{
		roll_ang_no_change = 1;
	}
	else
	{
		roll_ang_no_change = 0;

		// Arc-Tan(Value) --> Roll Angle (-90~90)
		tan_cordic(&tan_ang, &atan_numr, &atan_deno);

		// Roll Angle (-180~180)
		if(tan_ang > 0)
		{
			roll_ang_t1 = tan_ang;
			roll_ang_t2 = (FIX_180 - tan_ang);
		}
		else
		{
			roll_ang_t1 = tan_ang;
			roll_ang_t2 = -(FIX_180 + tan_ang);
		}

		// Mapping to 0~360
		if(roll_ang_t1 < 0)
			roll_ang_t1_360 = FIX_360 + roll_ang_t1;
		else
			roll_ang_t1_360 = roll_ang_t1;


		if(roll_ang_t2 < 0)
			roll_ang_t2_360 = FIX_360 + roll_ang_t2;
		else
			roll_ang_t2_360 = roll_ang_t2;

		// Sin(roll_ang_t1_360), Cos(roll_ang_t1_360)
		sin_cos_ang = roll_ang_t1_360;
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_roll_value_t1 = sin_value;
		cos_roll_value_t1 = cos_value;

		// Sin(roll_ang_t2_360), Cos(roll_ang_t2_360)
		sin_cos_ang = roll_ang_t2_360;
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_roll_value_t2 = sin_value;
		cos_roll_value_t2 = cos_value;
	}

	// ----------- Calculate Pitch Angle ------------------
	// Calculate Pitch Angle
	atan_numr = -(data->x);
	atan_deno = data->z;

	if((atan_deno == 0))
	{
		pitch_ang_no_change = 1;
	}
	else
	{
		pitch_ang_no_change = 0;

		// Arc-Tan(Value) --> Pitch Angle (-90~90)
		tan_cordic(&tan_ang, &atan_numr, &atan_deno);
		pitch_ang_t = tan_ang;

		if(pitch_ang_t < 0)
			sin_cos_ang = FIX_360 + pitch_ang_t;
		else
			sin_cos_ang = pitch_ang_t;

		// Calculate Sin(pitch_ang) Cos(pitch_ang)
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_pitch_value = sin_value;
		cos_pitch_value = cos_value;
	}

	// Find Final Roll Angle
	// For Solution t1/t2
	sign_cos_pitch = sign_func(&cos_pitch_value);
	sign_sin_pitch = sign_func(&sin_pitch_value);
	sign_cos_roll_t1 = sign_func(&cos_roll_value_t1);
	sign_sin_roll_t1 = sign_func(&sin_roll_value_t1);
	sign_cos_roll_t2 = sign_func(&cos_roll_value_t2);
	sign_sin_roll_t2 = sign_func(&sin_roll_value_t2);

	angle_val_data_x = data->x;
	angle_val_data_y = data->y;
	angle_val_data_z = data->z;
	sign_x = sign_func(&angle_val_data_x);
	sign_y = sign_func(&angle_val_data_y);
	sign_z = sign_func(&angle_val_data_z);

	if((roll_ang_no_change == 1) || (pitch_ang_no_change == 1))
	{
		*roll_ang = *roll_ang_crct;
		*pitch_ang = *pitch_ang_crct;
	}
	else if(sign_sin_pitch == 0)
	{
		if((sign_sin_roll_t1 == sign_y) && (sign_cos_pitch * sign_cos_roll_t1 == sign_z))
		{
			*roll_ang = roll_ang_t1;
			*pitch_ang = pitch_ang_t;
		}
		else if((sign_sin_roll_t2 == sign_y) && (sign_cos_pitch * sign_cos_roll_t2 == sign_z))
		{
			*roll_ang = roll_ang_t2;
			*pitch_ang = pitch_ang_t;
		}
		else
		{
			*roll_ang = *roll_ang_crct;
			*pitch_ang = *pitch_ang_crct;
		}
	}
	else
	{
		if(((-1)*sign_sin_pitch * sign_cos_roll_t1 == sign_x) && (sign_cos_pitch * sign_cos_roll_t1 == sign_z))
		{
			*roll_ang = roll_ang_t1;
			*pitch_ang = pitch_ang_t;
		}
		else if(((-1)*sign_sin_pitch * sign_cos_roll_t2 == sign_x) && (sign_cos_pitch * sign_cos_roll_t2 == sign_z))
		{
			*roll_ang = roll_ang_t2;
			*pitch_ang = pitch_ang_t;
		}
		else
		{
			*roll_ang = *roll_ang_crct;
			*pitch_ang = *pitch_ang_crct;
		}
	}
	return;
}

void SENSOR_CODE roll_pitch_xyz(LPSINT32 roll_ang, LPSINT32 pitch_ang, 
                                LPSINT32 data_in,
                                LPSINT32 roll_ang_crct, LPSINT32 pitch_ang_crct)
{
	// Right Shift
	SINT32 shift_in, shift_out, shift_bit;

	// Data Value
	LPSenAxisVec data = (LPSenAxisVec)data_in;

	// Tan Function
	SINT32 atan_numr, atan_deno, tan_ang;

	// Sin And Cos Function
	SINT32 sin_value, cos_value, sin_cos_ang;

	// For Roll Angle
	SINT32 roll_ang_t1, roll_ang_t2, roll_ang_t1_360, roll_ang_t2_360;
	SINT32 sin_roll_value_t1, cos_roll_value_t1, sin_roll_value_t2, cos_roll_value_t2;

	// For Pitch Angle
	SINT32 pitch_ang_t1, pitch_ang_t2;
	SINT32 sin_pitch_value_t1, cos_pitch_value_t1, sin_pitch_value_t2, cos_pitch_value_t2;

	// Sign Value
	SINT32 roll_ang_no_change, pitch_ang_no_change;
	SINT32 sign_sin_roll_t1, sign_cos_roll_t1, sign_sin_pitch_t1, sign_cos_pitch_t1;
	SINT32 sign_sin_roll_t2, sign_cos_roll_t2, sign_sin_pitch_t2, sign_cos_pitch_t2;
	SINT32 angle_val_data_x, angle_val_data_y, angle_val_data_z;
	SINT32 sign_x, sign_y, sign_z;

	// ----------- Calculate Roll Angle ------------------
	// Numerator And Denomonator of Arc Tan For Angle Roll
	atan_numr = data->y;
	atan_deno = data->z;

	if((atan_deno == 0))
	{
		roll_ang_no_change = 1;
	}
	else
	{
		roll_ang_no_change = 0;

		// Arc-Tan(Value) --> Roll Angle (-90~90)
		tan_cordic(&tan_ang, &atan_numr, &atan_deno);

		// Roll Angle (-180~180)
		if(tan_ang > 0)
		{
			roll_ang_t1 = tan_ang;
			roll_ang_t2 = -(FIX_180 - tan_ang);
		}
		else
		{
			roll_ang_t1 = tan_ang;
			roll_ang_t2 = (FIX_180 + tan_ang);
		}

		// Mapping to 0~360
		if(roll_ang_t1 < 0)
			roll_ang_t1_360 = FIX_360 + roll_ang_t1;
		else
			roll_ang_t1_360 = roll_ang_t1;


		if(roll_ang_t2 < 0)
			roll_ang_t2_360 = FIX_360 + roll_ang_t2;
		else
			roll_ang_t2_360 = roll_ang_t2;

		// Sin(roll_ang_t1_360), Cos(roll_ang_t1_360)
		sin_cos_ang = roll_ang_t1_360;
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_roll_value_t1 = sin_value;
		cos_roll_value_t1 = cos_value;

		// Sin(roll_ang_t2_360), Cos(roll_ang_t2_360)
		sin_cos_ang = roll_ang_t2_360;
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_roll_value_t2 = sin_value;
		cos_roll_value_t2 = cos_value;
	}

	// ----------- Calculate Pitch Angle ------------------
	// Calculate Pitch Angle Based On Roll Angle (roll_ang_t1)
	atan_numr = -(data->x);

	shift_bit = 8;
	shift_in = data->y * sin_roll_value_t1 + data->z * cos_roll_value_t1;
	rshift_func(&shift_out, &shift_in, shift_bit);
	atan_deno = shift_out;

	if((atan_deno == 0))
	{
		pitch_ang_no_change = 1;
	}
	else
	{
		pitch_ang_no_change = 0;

		// Arc-Tan(Value) --> Pitch Angle (-90~90)
		tan_cordic(&tan_ang, &atan_numr, &atan_deno);
		pitch_ang_t1 = tan_ang;

		if(pitch_ang_t1 < 0)
			sin_cos_ang = FIX_360 + pitch_ang_t1;
		else
			sin_cos_ang = pitch_ang_t1;

		// Calculate Sin(pitch_ang_t1) Cos(pitch_ang_t1)
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_pitch_value_t1 = sin_value;
		cos_pitch_value_t1 = cos_value;

		// Calculate Pitch Angle Based On Roll Angle (roll_ang_t2)
		atan_numr = -(data->x);

		shift_bit = 8;
		shift_in = data->y * sin_roll_value_t2 + data->z * cos_roll_value_t2;
		rshift_func(&shift_out, &shift_in, shift_bit);
		atan_deno = shift_out;

		// Arc-Tan(Value) --> Pitch Angle (-90~90)
		tan_cordic(&tan_ang, &atan_numr, &atan_deno);
		pitch_ang_t2 = tan_ang;

		if(pitch_ang_t2 < 0)
			sin_cos_ang = FIX_360 + pitch_ang_t2;
		else
			sin_cos_ang = pitch_ang_t2;

		// Calculate Sin(pitch_ang_t2) Cos(pitch_ang_t2)
		sin_cos_value(&sin_value, &cos_value, &sin_cos_ang);
		sin_pitch_value_t2 = sin_value;
		cos_pitch_value_t2 = cos_value;
	}

	// Find Final Pitch Angle (Only one)
	// For Solution t1
	sign_sin_roll_t1 = sign_func(&sin_roll_value_t1);
	sign_cos_roll_t1 = sign_func(&cos_roll_value_t1);
	sign_sin_pitch_t1 = sign_func(&sin_pitch_value_t1);
	sign_cos_pitch_t1 = sign_func(&cos_pitch_value_t1);

	// For Solution t2
	sign_sin_roll_t2 = sign_func(&sin_roll_value_t2);
	sign_cos_roll_t2 = sign_func(&cos_roll_value_t2);
	sign_sin_pitch_t2 = sign_func(&sin_pitch_value_t2);
	sign_cos_pitch_t2 = sign_func(&cos_pitch_value_t2);

	angle_val_data_x = data->x;
	angle_val_data_y = data->y;
	angle_val_data_z = data->z;
	sign_x = sign_func(&angle_val_data_x);
	sign_y = sign_func(&angle_val_data_y);
	sign_z = sign_func(&angle_val_data_z);

	if((roll_ang_no_change == 1) || (pitch_ang_no_change == 1))
	{
		*roll_ang = *roll_ang_crct;
		*pitch_ang = *pitch_ang_crct;
	}
	else if((sign_sin_roll_t1 == 0) || (sign_sin_roll_t2 == 0))
	{
		if(((-1)*sign_sin_pitch_t1 == sign_x) && (sign_cos_pitch_t1 * sign_cos_roll_t1 == sign_z))
		{
			*roll_ang = roll_ang_t1;
			*pitch_ang = pitch_ang_t1;
		}
		else if(((-1)*sign_sin_pitch_t2 == sign_x) && (sign_cos_pitch_t2 * sign_cos_roll_t2 == sign_z))
		{
			*roll_ang = roll_ang_t2;
			*pitch_ang = pitch_ang_t2;
		}
		else
		{
			*roll_ang = *roll_ang_crct;
			*pitch_ang = *pitch_ang_crct;
		}
	}
	else
	{
		if((sign_cos_pitch_t1 * sign_sin_roll_t1 == sign_y) && (sign_cos_pitch_t1 * sign_cos_roll_t1 == sign_z))
		{
			*roll_ang = roll_ang_t1;
			*pitch_ang = pitch_ang_t1;
		}
		else if((sign_cos_pitch_t2 * sign_sin_roll_t2 == sign_y) && (sign_cos_pitch_t2 * sign_cos_roll_t2 == sign_z))
		{
			*roll_ang = roll_ang_t2;
			*pitch_ang = pitch_ang_t2;
		}
		else
		{
			*roll_ang = *roll_ang_crct;
			*pitch_ang = *pitch_ang_crct;
		}
	}
	return;
}

//------------------------------------
//  Sin And Cos Look-Up Table (mul:0 , div:1)
//------------------------------------
void SENSOR_CODE sin_cos_value(LPSINT32 sin_value, LPSINT32 cos_value, LPSINT32 angle)
{
	// Angle 0~90
	SINT32 angle_0_2_90, angle_table, cos_table_index;

	//
	int cal_ang_flag = 0;

	// +/- Sin Or Cos Signal
	int minus_sig_sin, minus_sig_cos;

	//
	if(*angle == FIX_90)
	{
		*sin_value = (1 << 8);
		*cos_value = 0;
	}
	else if(*angle == FIX_180)
	{
		*sin_value = 0;
		*cos_value = -(1 << 8);
	}
	else if(*angle == FIX_270)
	{
		*sin_value = -(1 << 8);
		*cos_value = 0;
	}
	else if(*angle == 0 || *angle == FIX_360)
	{
		*sin_value = 0;
		*cos_value = (1 << 8);
	}
	else
	{
		cal_ang_flag = 1;
	}

	if(cal_ang_flag == 1)
	{
		// Sin And Cos Table Quadrant(Angle Value Is Between 0 and 90)
		if(*angle > 0 && *angle < FIX_90)
		{
			angle_0_2_90 = *angle;
			minus_sig_sin = 0;
			minus_sig_cos = 0;
		}
		else if(*angle > FIX_90 && *angle < FIX_180)
		{
			angle_0_2_90 = FIX_180 - *angle;
			minus_sig_sin = 0;
			minus_sig_cos = 1;
		}
		else if(*angle > FIX_180 && *angle < FIX_270)
		{
			angle_0_2_90 = *angle - FIX_180;
			minus_sig_sin = 1;
			minus_sig_cos = 1;
		}
		else if(*angle > FIX_270 && *angle < FIX_360)
		{
			angle_0_2_90 = FIX_360 - *angle;
			minus_sig_sin = 1;
			minus_sig_cos = 0;
		}

		else
		{
			angle_0_2_90 = *angle;
			minus_sig_sin = 0;
			minus_sig_cos = 0;
		}

		angle_table = (SINT32)((angle_0_2_90 << 8) / FIX_90);

		// Sin(Angle) Result
		if(angle_table == 0)
			*sin_value = 0;
		else if(minus_sig_sin == 1)
			*sin_value = -(*(sin_lut + angle_table));
		else
			*sin_value = (*(sin_lut + angle_table));

		// Cos(Angle) Result
		if(angle_table == 0)
		{
			if(minus_sig_cos == 1)
				*cos_value = -(1 << 8);
			else
				*cos_value = (1 << 8);
		}
		else
		{
			cos_table_index = (256 - angle_table);
			if(minus_sig_cos == 1)
				*cos_value = -(*(sin_lut + cos_table_index));
			else
				*cos_value = *(sin_lut + cos_table_index);
		}
	}
	return;
}

//------------------------------------
//  Tan Value (mul:3 , div:1)
//------------------------------------
void SENSOR_CODE tan_cordic(LPSINT32 ang_rad, LPSINT32 atan_numr, LPSINT32 atan_deno)
{
	// Tan --> 0~45 And Co-Tan --> 45~90
	int cot_en;

	// Abs(atan_numr) And Abs(atan_deno)
	SINT32 atan_numr_tmp, atan_deno_tmp;

	//
	SINT32 atan_value, atan_tmp1, atan_tmp2, ang_tmp;

	// Abs(atan_numr) And Abs(atan_deno)
	if(*atan_numr < 0)
		atan_numr_tmp = -(*atan_numr);
	else
		atan_numr_tmp = *atan_numr;

	if(*atan_deno < 0)
		atan_deno_tmp = -(*atan_deno);
	else
		atan_deno_tmp = *atan_deno;

	// Tan --> 0~45 And Co-Tan --> 45~90
	if(((atan_numr_tmp == atan_deno_tmp) && (atan_numr_tmp == 0)) || (atan_numr_tmp == 0))
	{
		atan_value = 0;
		cot_en = 0;
	}
	else if(atan_numr_tmp <= atan_deno_tmp)
	{
		atan_value = ((SINT32)(atan_numr_tmp) << 8) / (SINT32)(atan_deno_tmp);
		cot_en = 0;
	}
	else
	{
		atan_value = ((SINT32)(atan_deno_tmp) << 8) / (SINT32)(atan_numr_tmp);
		cot_en = 1;
	}

	// Tan Function Approximation
	atan_tmp1 = (70 * atan_value) >> 8; // 0.273*x
	atan_tmp2 = ((1 << 8) - atan_value); // 1-x

	// Atan(x) ~= (pi/4)*x + 0.273*x*(1-abs(x))
	ang_tmp = (SINT32)(((PI >> 2) * atan_value) >> 8) + (SINT32)((atan_tmp1 * atan_tmp2) >> 8);

	// Co-Tan Function Translate
	if(cot_en == 1)
		ang_tmp = FIX_90 - ang_tmp;

	// Final Angle Output
	if(*atan_numr == 0 || *atan_deno == 0)
		*ang_rad = 0;
	else
	{
		if((*atan_numr > 0 && *atan_deno > 0) || (*atan_numr < 0 && *atan_deno < 0))
			*ang_rad = ang_tmp;
		else
			*ang_rad = -ang_tmp;
	}
	return;
}

//------------------------------------
//  Right Shift Function (mul:0 , div:0)
//------------------------------------
void SENSOR_CODE rshift_func(LPSINT32 out, LPSINT32 in, SINT32 shift_bit)
{
	SINT32 in_tmp, out_tmp;

	if(*in < 0)
	{
		in_tmp = -(*in);
		out_tmp = (in_tmp >> shift_bit);
		*out = -out_tmp;
	}
	else
	{
		*out = *in >> shift_bit;
	}
	return;
}

//------------------------------------
//  Sign Function (mul:0 , div:0)
//------------------------------------
int SENSOR_CODE sign_func(LPSINT32 value)
{
	if(*value < 0)
		return -1;
	else if(*value > 0)
		return 1;
	else
		return 0;
}

//------------------------------------
//  Quaternion Equal (mul:0 , div:0)
//  tan: 0  sin/cos: 0  mul: 0  div: 0
//------------------------------------
void SENSOR_CODE quatern_equal(LPSINT32 out, LPSINT32 in)
{
	int i;
	for(i = 0 ; i < 4 ; i++)
		*out++ = *in++;
}

#ifdef __SHAKE_FUNCTION_SIMPLE__
//------------------------------------
//  Shaking Detection (Simple)
//------------------------------------
void SENSOR_CODE shake_detect_simple(LPSINT32 acc_raw_data, LPUINT8 shake_status)
{
	// Accelerometer Data Function
	SINT32 acc_x_2, acc_y_2, acc_z_2, acc_2_total, acc_total;

	// Accelerometer Data^2
	acc_x_2 = ((SINT32)(*(acc_raw_data + 0)) * (SINT32)(*(acc_raw_data + 0)));
	acc_y_2 = ((SINT32)(*(acc_raw_data + 1)) * (SINT32)(*(acc_raw_data + 1)));
	acc_z_2 = ((SINT32)(*(acc_raw_data + 2)) * (SINT32)(*(acc_raw_data + 2)));
	acc_2_total = acc_x_2 + acc_y_2 + acc_z_2;

	// sqrt(acc_2_total);
	sqrt_func(&acc_total, &acc_2_total);

	if(acc_total > SHAKE_EN_THR)
		*shake_status = 1;
	else
		*shake_status = 0;

	return;
}
#else
//------------------------------------
//  Shaking Detection
//------------------------------------
void SENSOR_CODE shake_detect(
    // Input Data
    LPSINT32 acc_raw_data, LPSINT16 acc_raw_x_vec_pre, LPSINT16 acc_raw_y_vec_pre, LPSINT16 acc_raw_z_vec_pre,

    // Output Data
    LPSINT16 acc_raw_x_vec_post, LPSINT16 acc_raw_y_vec_post, LPSINT16 acc_raw_z_vec_post, LPUINT8 shake_status)
{
	// Shift Function
	SINT32 shift_in, shift_out, shift_bit;

	//
	int i, j;
	SINT32 matrix_size;
	SINT32 shake_x_updown, shake_x_downup;
	SINT32 shake_y_updown, shake_y_downup;
	SINT32 shake_z_updown, shake_z_downup;

	// -------------- Detect Shake Direction ---------------------
	matrix_size = (SINT32)SHAKE_MODEL_TAP;

	// Shift Shake Data
	for(i = 0 ; i < matrix_size ; i++)
	{
		if(i < (matrix_size - 1))
		{
			j = i + 1;
			*(acc_raw_x_vec_post + i) = *(acc_raw_x_vec_pre + j);
			*(acc_raw_y_vec_post + i) = *(acc_raw_y_vec_pre + j);
			*(acc_raw_z_vec_post + i) = *(acc_raw_z_vec_pre + j);
		}
		else
		{
			*(acc_raw_x_vec_post + i) = *(acc_raw_data + 0);
			*(acc_raw_y_vec_post + i) = *(acc_raw_data + 1);
			*(acc_raw_z_vec_post + i) = *(acc_raw_data + 2);
		}
	}

	// Detect X, Y And Z Direction
	shift_bit = 8;
	shake_x_updown = 0;
	shake_x_downup = 0;
	shake_y_updown = 0;
	shake_y_downup = 0;
	shake_z_updown = 0;
	shake_z_downup = 0;

	for(i = 0 ; i < matrix_size ; i++)
	{
		// Detect X Up-2-Down (Right)
		shift_in = *(acc_raw_x_vec_post + i) * (shake_model_up_down[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_x_updown = shift_out + shake_x_updown;

		// Detect X Down-2-Up (Left)
		shift_in = *(acc_raw_x_vec_post + i) * (shake_model_down_up[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_x_downup = shift_out + shake_x_downup;

		// Detect Y Up-2-Down (Front)
		shift_in = *(acc_raw_y_vec_post + i) * (shake_model_up_down[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_y_updown = shift_out + shake_y_updown;

		// Detect Y Down-2-Up (Back
		shift_in = *(acc_raw_y_vec_post + i) * (shake_model_down_up[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_y_downup = shift_out + shake_y_downup;

		// Detect Z Up-2-Down (Up)
		shift_in = *(acc_raw_z_vec_post + i) * (shake_model_up_down[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_z_updown = shift_out + shake_z_updown;

		// Detect Z Down-2-Up (Down)
		shift_in = *(acc_raw_z_vec_post + i) * (shake_model_down_up[i]);
		rshift_func(&shift_out, &shift_in, shift_bit);
		shake_z_downup = shift_out + shake_z_downup;
	}

	// Shake Status Initial
	*shake_status = 0;

	if(shake_x_updown > SHAKE_EN_THR)
	{
		*shake_status = 65; // 01000001
		//RTL_PRINTF("Right\r\n\n");
	}
	else if(shake_x_downup > SHAKE_EN_THR)
	{
		*shake_status = 66; // 01000010
		//RTL_PRINTF("Left\r\n\n");
	}
	else if(shake_y_updown > SHAKE_EN_THR)
	{
		*shake_status = 68; // 01000100
		//RTL_PRINTF("Front\r\n\n");
	}
	else if(shake_y_downup > SHAKE_EN_THR)
	{
		*shake_status = 72; // 01001000
		//RTL_PRINTF("Back\r\n\n");
	}
	else if(shake_z_updown > SHAKE_EN_THR)
	{
		*shake_status = 80; // 01010000
		//RTL_PRINTF("Up\r\n\n");
	}
	else if(shake_z_downup > SHAKE_EN_THR)
	{
		*shake_status = 96; // 01100000
		//RTL_PRINTF("Down\r\n\n");
	}

	if((*shake_status != 0))
	{
		for(i = 0 ; i < matrix_size ; i++)
		{
			*(acc_raw_x_vec_post + i) = 0;
			*(acc_raw_y_vec_post + i) = 0;
			*(acc_raw_z_vec_post + i) = 0;
	}
	}

	if(*shake_status != 0)
	{
		shake_status_tmp = *shake_status;
	}
	*shake_status = shake_status_tmp;

	return;
}
#endif

void SENSOR_CODE fusion_data_copy(LPUINT16 pBufOut, LPSINT32 pBufIn, UINT8 u8CntOfBufIn)
{
    while(u8CntOfBufIn--)
    {
        *pBufOut = (UINT16)(*pBufIn);

        pBufOut++;
        pBufIn++;
    }
}

/*-------------------------------------------------------------------------
    Internal function for sensor calibration
-------------------------------------------------------------------------*/
void SENSOR_CODE fusion_axis_switch(LPSINT32 rdata, UINT8 axis_mode)
{
	UINT8 axis_order;
	SINT32 rdata_tmp[3];

	if(axis_mode)
	{
		// Axis Order
		axis_order = (axis_mode >> 4);
		switch(axis_order)
		{
				// {x,y,z}
			case 0 :
				rdata_tmp[0] = *(rdata + 0);
				rdata_tmp[1] = *(rdata + 1);
				rdata_tmp[2] = *(rdata + 2);
				break;

				// {x,z,y}
			case 1 :
				rdata_tmp[0] = *(rdata + 0);
				rdata_tmp[1] = *(rdata + 2);
				rdata_tmp[2] = *(rdata + 1);
				break;

				// {y,x,z}
			case 2 :
				rdata_tmp[0] = *(rdata + 1);
				rdata_tmp[1] = *(rdata + 0);
				rdata_tmp[2] = *(rdata + 2);
				break;

				// {y,z,x}
			case 3 :
				rdata_tmp[0] = *(rdata + 1);
				rdata_tmp[1] = *(rdata + 2);
				rdata_tmp[2] = *(rdata + 0);
				break;

				// {z,x,y}
			case 4 :
				rdata_tmp[0] = *(rdata + 2);
				rdata_tmp[1] = *(rdata + 0);
				rdata_tmp[2] = *(rdata + 1);
				break;

				// {z,y,x}
			case 5 :
				rdata_tmp[0] = *(rdata + 2);
				rdata_tmp[1] = *(rdata + 1);
				rdata_tmp[2] = *(rdata + 0);
				break;

			default :
				rdata_tmp[0] = *(rdata + 0);
				rdata_tmp[1] = *(rdata + 1);
				rdata_tmp[2] = *(rdata + 2);
				break;
		}

		// Sign Order
		// X-Axis
		*(rdata + 0) = (axis_mode & 0x04) ? (-rdata_tmp[0]) : (rdata_tmp[0]);

		// Y-Axis
		*(rdata + 1) = (axis_mode & 0x02) ? (-rdata_tmp[1]) : (rdata_tmp[1]);

		// Z-Axis
		*(rdata + 2) = (axis_mode & 0x01) ? (-rdata_tmp[2]) : (rdata_tmp[2]);
	}
}

void SENSOR_CODE acc_calibrate(SINT32 *pData, SINT16 *pBias, SINT16 *pGain)
{
    int i;
    
    if(pGain != NULL)
    {
        for(i=0; i<3; i++)
        {
            if(*pGain != 0)
            {
                *pData = (((*pData) - (*pBias)) << 8) / (*pGain);
            }
            else
            {
                *pData -= (*pBias);
            }

            pData++;
            pBias++;
            pGain++;
        }
    }
    else
    {
        for(i=0; i<3; i++)
        {
            *pData -= (*pBias);

            pData++;
            pBias++;
        }
    }
}

void SENSOR_CODE_L _update_acc_bias(SINT16 *pBias, 
                                    SINT32 *pData, UINT8 CalibrationPhaseMode)
{
	switch(CalibrationPhaseMode)
	{
			// {x,y,z} = {0,0,g}
		case 0 :
			pBias[0] = pData[0] - 0;
			pBias[1] = pData[1] - 0;
			pBias[2] = pData[2] - 256;
			break;

			// {x,y,z} = {0,0,-g}
		case 1 :
			pBias[0] = pData[0] - 0;
			pBias[1] = pData[1] - 0;
			pBias[2] = pData[2] + 256;
			break;

			// {x,y,z} = {g,0,0}
		case 2 :
			pBias[0] = pData[0] - 256;
			pBias[1] = pData[1] - 0;
			pBias[2] = pData[2] - 0;
			break;

			// {x,y,z} = {-g,0,0}
		case 3 :
			pBias[0] = pData[0] + 256;
			pBias[1] = pData[1] - 0;
			pBias[2] = pData[2] - 0;
			break;

			// {x,y,z} = {0,g,0}
		case 4 :
			pBias[0] = pData[0] - 0;
			pBias[1] = pData[1] - 256;
			pBias[2] = pData[2] - 0;
			break;

			// {x,y,z} = {0,-g,0}
		case 5 :
			pBias[0] = pData[0] - 0;
			pBias[1] = pData[1] + 256;
			pBias[2] = pData[2] - 0;
			break;

		default :
			pBias[0] = 0;
			pBias[1] = 0;
			pBias[2] = 0;
			break;
	}

	//printk("[%d, %d, %d][%Xh]\r\n", 
    //       pData[0], pData[1], pData[2], CalibrationPhaseMode);
}

void SENSOR_CODE_L acc_cali_6axis(TAccCal6AxisInf *pCtx, 
                                  SINT16 *pGain, SINT16 *pBias, 
                                  SINT32 *pData, UINT8 CalibrationPhaseMode)
{
	switch(CalibrationPhaseMode)
	{
			// {x,y,z} = {0,0,g}
		case 0 :
			pCtx->sum[0] += pData[0];
            pCtx->cnt[0]++;

            pCtx->sum[1] += pData[1];
            pCtx->cnt[1]++;

            pCtx->sum_2 += pData[2];

            pCtx->max_z = pData[2];
			break;

			// {x,y,z} = {0,0,-g}
		case 1 :
			pCtx->sum[0] += pData[0];
            pCtx->cnt[0]++;
            
			pCtx->sum[1] += pData[1];
            pCtx->cnt[1]++;

            pCtx->sum_2 += pData[2];
            
			pCtx->min_z = pData[2];
			break;

			// {x,y,z} = {g,0,0}
		case 2 :
			pCtx->max_x = pData[0];
            
			pCtx->sum[1] += pData[1];
            pCtx->cnt[1]++;
            
			pCtx->sum[2] += pData[2];
            pCtx->cnt[2]++;
			break;

			// {x,y,z} = {-g,0,0}
		case 3 :
			pCtx->min_x = pData[0];
            
			pCtx->sum[1] += pData[1];
            pCtx->cnt[1]++;
            
			pCtx->sum[2] += pData[2];
            pCtx->cnt[2]++;
			break;

			// {x,y,z} = {0,g,0}
		case 4 :
			pCtx->sum[0] += pData[0];
            pCtx->cnt[0]++;
            
			pCtx->max_y = pData[1];
            
			pCtx->sum[2] += pData[2];
            pCtx->cnt[2]++;
			break;

			// {x,y,z} = {0,-g,0}
		case 5 :
			pCtx->sum[0] += pData[0];
            pCtx->cnt[0]++;
            
			pCtx->min_y = pData[1];
            
			pCtx->sum[2] += pData[2];
            pCtx->cnt[2]++;
			break;

			// Initial Reset
        case 0xF0:
			printk("[reset acc calibration buffer]\r\n");
			memset((LPUINT8)pCtx, 0x00, sizeof(TAccCal6AxisInf));            
            break;

    		// Final Result
        case 0xF1:
            // BIAS
            if(pCtx->cnt[0]){                
    			pBias[0] = (pCtx->sum[0] / pCtx->cnt[0]);
            }

            if(pCtx->cnt[1]){
			    pBias[1] = (pCtx->sum[1] / pCtx->cnt[1]);
            }

            if(pCtx->cnt[2]){
			    pBias[2] = (pCtx->sum[2] / pCtx->cnt[2]);
            }
            else{
                pBias[2] = pCtx->sum_2 / 2;
            }

            // GAIN
            if((pCtx->max_x != 0) && (pCtx->min_x != 0)){
			    pGain[0] = (pCtx->max_x - pCtx->min_x) >> 1;
            }
            else{
                pGain[0] = 256;
            }

            if((pCtx->max_y != 0) && (pCtx->min_y != 0)){
			    pGain[1] = (pCtx->max_y - pCtx->min_y) >> 1;
            }
            else{
                pGain[1] = 256;
            }

            if((pCtx->max_z != 0) && (pCtx->min_z != 0)){
    			pGain[2] = (pCtx->max_z - pCtx->min_z) >> 1;
            }
            else{
                pGain[2] = 256;
            }
            printk("[acc calibration finish]\r\n");            
            printk(" sum: %d(%d), %d(%d), %d(%d)\r\n", 
                   pCtx->sum[0], pCtx->cnt[0],
                   pCtx->sum[1], pCtx->cnt[1],
                   pCtx->sum[2], pCtx->cnt[2]);
            printk(" x: %d ~ %d\r\n", pCtx->max_x, pCtx->min_x);
            printk(" y: %d ~ %d\r\n", pCtx->max_y, pCtx->min_y);
            printk(" z: %d ~ %d\r\n", pCtx->max_z, pCtx->min_z);
            printk(" sum_2: %d\r\n", pCtx->sum_2);
            break;

		default :
			pBias[0] = 0;
			pBias[1] = 0;
			pBias[2] = 0;
			
			pGain[0] = 256;
			pGain[1] = 256;
			pGain[2] = 256;
			break;
	}
}

/*-------------------------------------------------------------------------
    External function for sensor calibration
-------------------------------------------------------------------------*/
void SENSOR_CODE_L init_acc_6axis_cal_param(SINT16 pGainBuffer[3], 
                                            UINT8 pCalBuffer[24])
{
    acc_gain = pGainBuffer;
    p_acc_cal_buf = (TAccCal6AxisInf*)pCalBuffer;
}

void SENSOR_CODE_L init_acc2_6axis_cal_param(SINT16 pGainBuffer[3], 
                                             UINT8 pCalBuffer[24])
{
    acc2_gain = pGainBuffer;
    p_acc2_cal_buf = (TAccCal6AxisInf*)pCalBuffer;
}

void SENSOR_CODE_L fusion_update_acc_bias(UINT8 *pRawData, 
                                          UINT8 CalibrationPhaseMode)
{
    if((CalibrationPhaseMode >= 0) && (CalibrationPhaseMode <= 5))
    {
    	acc_raw_data[0] = ((SINT32)(*((LPSINT16)(pRawData + 0))) << 8);
    	acc_raw_data[1] = ((SINT32)(*((LPSINT16)(pRawData + 2))) << 8);
    	acc_raw_data[2] = ((SINT32)(*((LPSINT16)(pRawData + 4))) << 8);
    	do_unit_correct_acc(acc_raw_data, acc_data);

    	fusion_axis_switch(acc_data, acc_axis_mode);

        if(acc_gain == NULL)
        {
    	    _update_acc_bias(mSenFuParam.acc_bias, 
                             acc_data, CalibrationPhaseMode);

            return;
        }
    }

    if(acc_gain != NULL)
    {
        acc_cali_6axis(p_acc_cal_buf, 
                       acc_gain, mSenFuParam.acc_bias, 
                       acc_data, CalibrationPhaseMode);
    }
    
}

void SENSOR_CODE_L fusion_update_acc2_bias(UINT8 *pRawData, 
                                           UINT8 CalibrationPhaseMode)
{
    if((CalibrationPhaseMode >= 0) && (CalibrationPhaseMode <= 5))
    {
    	acc_raw_data[0] = ((SINT32)(*((LPSINT16)(pRawData + 0))) << 8);
    	acc_raw_data[1] = ((SINT32)(*((LPSINT16)(pRawData + 2))) << 8);
    	acc_raw_data[2] = ((SINT32)(*((LPSINT16)(pRawData + 4))) << 8);
    	do_unit_correct_acc2(acc_raw_data, acc_data);

    	fusion_axis_switch(acc_data, acc2_axis_mode);

        if(acc2_gain == NULL)
        {
        	_update_acc_bias(acc2_bias, acc_data, CalibrationPhaseMode);

            return;
        }
    }

    if(acc2_gain != NULL)
    {
        acc_cali_6axis(p_acc2_cal_buf, 
                       acc2_gain, acc2_bias, 
                       acc_data, CalibrationPhaseMode);
    }
}

void SENSOR_CODE_L fusion_set_op_mode(UINT8 u8Mode)
{
	mag_op_mode = u8Mode;
}

/*-------------------------------------------------------------------------
    External function for 2nd Acc
-------------------------------------------------------------------------*/
void SENSOR_CODE_L init_custom_angle_param(UINT8 u8AxisMode, 
                                           SINT16 pBiasBuffer[3])
{
	acc2_axis_mode = u8AxisMode;
	acc2_bias = pBiasBuffer;
}

int SENSOR_CODE get_custom_angle(UINT8 pInAcc1[6], UINT8 pInAcc2[6], 
                                 SINT16 pOutAngle[3], UINT16 pOutAccData[3], 
                                 UINT16 u16Freq)
{
	SINT32 acc2_data[3];
	int rtn;

    // Acc1 Data 
	acc_raw_data[0] = ((SINT32)(*((LPSINT16)(pInAcc1 + 0))) << 8);
	acc_raw_data[1] = ((SINT32)(*((LPSINT16)(pInAcc1 + 2))) << 8);
	acc_raw_data[2] = ((SINT32)(*((LPSINT16)(pInAcc1 + 4))) << 8);
	do_unit_correct_acc(acc_raw_data, acc_data);
	fusion_axis_switch(acc_data, acc_axis_mode);

    acc_calibrate(acc_data, mSenFuParam.acc_bias, acc_gain);

    // Acc2 Data
	acc_raw_data[0] = ((SINT32)(*((LPSINT16)(pInAcc2 + 0))) << 8);
	acc_raw_data[1] = ((SINT32)(*((LPSINT16)(pInAcc2 + 2))) << 8);
	acc_raw_data[2] = ((SINT32)(*((LPSINT16)(pInAcc2 + 4))) << 8);
	do_unit_correct_acc2(acc_raw_data, acc2_data);
	fusion_axis_switch(acc2_data, acc2_axis_mode);

    acc_calibrate(acc2_data, acc2_bias, acc2_gain);

    // Output Data
	pOutAccData[0] = (UINT16)(acc2_data[0]);
	pOutAccData[1] = (UINT16)(acc2_data[1]);
	pOutAccData[2] = (UINT16)(acc2_data[2]);

	rtn = two_acc_angle(acc_data, acc2_data, pOutAngle, u16Freq);

    return rtn;
}
#endif // } __ENABLE_FUSION_API__


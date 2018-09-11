#include "sensor_fusion.h"

// Initial Function
UINT8 do_initial_gyo(UINT16 u16Freq)
{
	return 1;
}

// Initial Calibrate Function
UINT8 do_init_calibrate_gyo(UINT8 *pParam)
{
	return 0xFF;
}

// Self-Test Function
UINT32 do_self_test_gyo(void)
{
	return 0xFFFFFFFF;
}

// Data Correction Function
void do_data_correct_gyo(UINT8 *pBuf)
{

}

// Unit: DEGREE_PER_SECOND
void do_unit_correct_gyo(SINT32 pBufIn[3], SINT32 pBufOut[3])
{

}

// Power Mode Control Function
void do_power_mode_switch_gyo(UINT8 u8Mode)
{

}

// Data Rate Switch Function
BOOL do_data_rate_check_gyo(UINT16 u16Freq, UINT8 *pu8FreqIdx)
{
	return 1;
}

UINT16 do_data_rate_switch_gyo(UINT8 u8FreqIdx)
{
	return 0;
}


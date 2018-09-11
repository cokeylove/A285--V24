#include "sensor_fusion.h"

// Initial Function
UINT8 do_initial_mag(UINT16 u16Freq)
{
	return 1;
}

// Initial Calibrate Function
UINT8 do_init_calibrate_mag(UINT8 *pParam)
{
    return 0xFF;
}

// Runtime Calibrate Function
int do_rt_calibrate_mag(SINT32 pDataBuffer[3], SINT16 pBiasBuffer[3], UINT8 *pMagInfrFlag, UINT16 u16Freq)
{
	return 0;
}

void do_get_calibrate_info_mag(UINT8 *pState, UINT8 *pMagInfrFlag)
{

}

void do_switch_calibrate_formation(UINT8 u8Formation)
{

}

// Self-Test Function
UINT32 do_self_test_mag(void)
{
	return 0xFFFFFFFF;
}

// Data Correction Function
void do_data_correct_mag(UINT8 *pBuf)
{

}

// Unit: GAUSS
void do_unit_correct_mag(SINT32 pBufIn[3], SINT32 pBufOut[3])
{

}

// Power Mode Control Function
void do_power_mode_switch_mag(UINT8 u8Mode)
{

}

// Data Rate Switch Function
BOOL do_data_rate_check_mag(UINT16 u16Freq, UINT8 *pu8FreqIdx)
{
	return 1;
}

UINT16 do_data_rate_switch_mag(UINT8 u8FreqIdx)
{
	return 0;
}

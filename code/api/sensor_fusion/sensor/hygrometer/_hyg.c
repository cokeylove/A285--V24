#include "sensor_fusion.h"

// Initial Function
UINT8 do_initial_hyg(UINT16 u16Freq)
{
	return 1;
}

// Self-Test Function
UINT32 do_self_test_hyg(void)
{
    return 0xFFFFFFFF;
}

// Data Correction Function
void do_data_correct_hyg(UINT8 *pBuf)
{

}

// Unit: %
UINT32 do_unit_correct_hyg(UINT32 u32Data)
{
    return u32Data;
}

// Power Mode Control Function
void do_power_mode_switch_hyg(UINT8 u8Mode)
{

}


#include "sensor_fusion.h"

// Initial Function
UINT8 do_initial_prs(UINT16 u16Freq)
{
	return 1;
}

// Data Correction Function
void do_data_correct_prs(UINT8 *pBuf)
{

}

// Unit: Pa
UINT32 do_unit_correct_prs(UINT32 u32Data)
{
    return u32Data;
}

// Power Mode Control Function
void do_power_mode_switch_prs(UINT8 u8Mode)
{

}

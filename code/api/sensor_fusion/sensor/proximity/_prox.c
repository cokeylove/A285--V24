#include "sensor_fusion.h"


//****************************************************************************************
// extern parameter & function for general use
//****************************************************************************************

// Initial Function
UINT8 do_initial_prox(UINT16 u16Freq)
{
	return 1;
}

// Data Correction Function
void do_data_correct_prox(UINT8 *pBuf)
{

}

// Unit: 
UINT32 do_unit_correct_prox(UINT32 u32Data)
{
    return u32Data;
}

// Power Mode Control Function
void do_power_mode_switch_prox(UINT8 u8Mode)
{

}

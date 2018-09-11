#ifndef _NULL_TEMP_H_
#define _NULL_TEMP_H_

#include "chip_type.h"

#define _ENABLE_TEMP_  0

#define TEMP_SENSOR_API_ID   0xFF070001

//extern parameter & function for general use
extern UINT8 do_initial_temp(UINT16 u16Freq);
extern UINT32 do_self_test_temp(void);
extern void do_data_correct_temp(UINT8 *pBuf);
extern UINT32 do_unit_correct_temp(UINT32 u32Data);
extern void do_power_mode_switch_temp(UINT8 u8Mode);

#endif // _NULL_TEMP_H_

#ifndef _NULL_HYG_H_
#define _NULL_HYG_H_

#include "chip_type.h"

#define _ENABLE_HYG_  0

#define HYG_SENSOR_API_ID   0xFF060001

//extern parameter & function for general use
extern UINT8 do_initial_hyg(UINT16 u16Freq);
extern UINT32 do_self_test_hyg(void);
extern void do_data_correct_hyg(UINT8 *pBuf);
extern UINT32 do_unit_correct_hyg(UINT32 u32Data);
extern void do_power_mode_switch_hyg(UINT8 u8Mode);

#endif // _NULL_HYG_H_
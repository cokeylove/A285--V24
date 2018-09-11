#ifndef _NULL_GYO_H_
#define _NULL_GYO_H_

#include "chip_type.h"

#define _ENABLE_GYO_  0

#define GYO_SENSOR_API_ID   0xFF030001

//extern parameter & function for general use
extern UINT8 do_initial_gyo(UINT16 u16Freq);
extern UINT8 do_init_calibrate_gyo(UINT8 *pParam);
extern UINT32 do_self_test_gyo(void);
extern void do_data_correct_gyo(UINT8 *pBuf);
extern void do_unit_correct_gyo(SINT32 pBufIn[3], SINT32 pBufOut[3]);
extern void do_power_mode_switch_gyo(UINT8 u8Mode);
extern BOOL do_data_rate_check_gyo(UINT16 u16Freq, UINT8 *pu8FreqIdx);
extern UINT16 do_data_rate_switch_gyo(UINT8 u8FreqIdx);

#endif //_NULL_GYO_H_


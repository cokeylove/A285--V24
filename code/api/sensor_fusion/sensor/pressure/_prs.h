#ifndef _NULL_PRS_H_
#define _NULL_PRS_H_

#include "chip_type.h"

#define _ENABLE_PRS_  0

#define PRS_SENSOR_API_ID   0xFF050001

//extern parameter & function for general use
extern UINT8 do_initial_prs(UINT16 u16Freq);
extern void do_data_correct_prs(UINT8 *pBuf);
extern UINT32 do_unit_correct_prs(UINT32 u32Data);
extern void do_power_mode_switch_prs(UINT8 u8Mode);
 
#endif //_NULL_PRS_H_


#ifndef _NULL_PROX_H_
#define _NULL_PROX_H_

#define _ENABLE_PROX_  0

#define PROX_SENSOR_API_ID   0xFF090001

extern UINT8 do_initial_prox(UINT16 u16Freq);
extern void do_data_correct_prox(UINT8 *pBuf);
extern UINT32 do_unit_correct_prox(UINT32 u32Data);
extern void do_power_mode_switch_prox(UINT8 u8Mode);

#endif // _NULL_PROX_H_
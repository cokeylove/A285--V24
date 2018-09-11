#ifndef _NULL_UV_H_
#define _NULL_UV_H_

#define _ENABLE_UV_  0

#define UV_SENSOR_API_ID    0xFF080001

extern UINT8 do_initial_uv(UINT16 u16Freq);
extern UINT32 do_self_test_uv(void);
extern void do_data_correct_uv(UINT8 *pBuf);
extern UINT32 do_unit_correct_uv(UINT32 u32Data);
extern void do_power_mode_switch_uv(UINT8 u8Mode);

#endif // _NULL_UV_H_
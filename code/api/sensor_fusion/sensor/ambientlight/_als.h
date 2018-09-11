#ifndef _DI5112_ALS_H_
#define _DI5112_ALS_H_

#define _ENABLE_ALS_  1

#define ALS_SENSOR_API_ID   0x12040002

#define DI5112_ALS_ID		0x38

//extern parameter & function for general use
#define SLAVE_ADDRESS_ALS   (DI5112_ALS_ID)
#define DATA_ADDRESS_ALS    (0x22)
#define DATA_LENGTH_ALS     (2)

extern UINT8 do_initial_als(UINT16 u16Freq);
extern void do_data_correct_als(UINT8 *pBuf);
extern UINT32 do_unit_correct_als(UINT32 u32Data);
extern void do_power_mode_switch_als(UINT8 u8Mode);

#endif // _DI5112_ALS_H_
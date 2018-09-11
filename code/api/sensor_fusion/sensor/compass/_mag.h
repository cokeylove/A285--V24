#ifndef _NULL_MAG_H_
#define _NULL_MAG_H_

#include "chip_type.h"

#define _ENABLE_MAG_  0

#define MAG_SENSOR_API_ID       0xFF020001

//extern parameter & function for general use
#define STORED_DATA_LENGTH_MAG  0

extern UINT8 do_initial_mag(UINT16 u16Freq);
extern UINT8 do_init_calibrate_mag(UINT8 *pParam);
extern int do_rt_calibrate_mag(SINT32 pDataBuffer[3], SINT16 pBiasBuffer[3], UINT8 *pMagInfrFlag, UINT16 u16Freq);
extern void do_get_calibrate_info_mag(UINT8 *pState, UINT8 *pMagInfrFlag);
extern void do_switch_calibrate_formation(UINT8 u8Formation);
extern UINT32 do_self_test_mag(void);
extern void do_data_correct_mag(UINT8 *pBuf);
extern void do_unit_correct_mag(SINT32 pBufIn[3], SINT32 pBufOut[3]);
extern void do_power_mode_switch_mag(UINT8 u8Mode);
extern BOOL do_data_rate_check_mag(UINT16 u16Freq, UINT8 *pu8FreqIdx);
extern UINT16 do_data_rate_switch_mag(UINT8 u8FreqIdx);
 
#endif //_NULL_MAG_H_


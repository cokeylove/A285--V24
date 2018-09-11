#ifndef _DI5112_ACC_H_
#define _DI5112_ACC_H_  

#include "chip_type.h"  

#define _ENABLE_ACC_  1

#define ACC_SENSOR_API_ID   0x12010001
       
#define DI5112_ACC_ID		(0x4C<<1) 		//0x98
 
//extern parameter & function for general use
#define SLAVE_ADDRESS_ACC   (DI5112_ACC_ID)
#define DATA_ADDRESS_ACC    (0x0D)
#define DATA_LENGTH_ACC     (6)

extern UINT8 do_initial_acc(UINT16 u16Freq);
extern UINT8 do_init_calibrate_acc(UINT8 *pParam);
extern UINT32 do_self_test_acc(void);
extern void do_unit_correct_acc(SINT32 pBufIn[3], SINT32 pBufOut[3]);
extern void do_data_correct_acc(UINT8 *pBuf);
extern void do_power_mode_switch_acc(UINT8 u8Mode);
extern BOOL do_data_rate_check_acc(UINT16 u16Freq, UINT8 *pu8FreqIdx);
extern UINT16 do_data_rate_switch_acc(UINT8 u8FreqIdx);

#endif // _DI5112_ACC_H_

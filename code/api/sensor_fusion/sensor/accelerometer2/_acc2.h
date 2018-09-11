#ifndef _BMA255_ACC2_H_
#define _BMA255_ACC2_H_  

#include "chip_type.h"  

#define _ENABLE_ACC2_  1

#define ACC2_SENSOR_API_ID   0x01010002
       
#define BMA255_ACC2_ID		 0x32 //0x30
 
//extern parameter & function for general use
#define SLAVE_ADDRESS_ACC2   (BMA255_ACC2_ID)
#define DATA_ADDRESS_ACC2    (0x02)
#define DATA_LENGTH_ACC2     (6)

extern UINT8 do_initial_acc2(UINT16 u16Freq);
extern UINT8 do_init_calibrate_acc2(UINT8 *pParam);
extern UINT32 do_self_test_acc2(void);
extern void do_unit_correct_acc2(SINT32 pBufIn[3], SINT32 pBufOut[3]);
extern void do_data_correct_acc2(UINT8 *pBuf);
extern void do_power_mode_switch_acc2(UINT8 u8Mode);
extern BOOL do_data_rate_check_acc2(UINT16 u16Freq, UINT8 *pu8FreqIdx);
extern UINT16 do_data_rate_switch_acc2(UINT8 u8FreqIdx);

#endif // _BMA255_ACC2_H_

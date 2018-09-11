#ifndef __EXT_TIMER_H__
#define __EXT_TIMER_H__
//****************************************************************************
#include "chip_type.h"

//****************************************************************************
//
//****************************************************************************
enum _EXT_TIMER_PRESCALE_TYPE_  {
    ET_PSR_32K,
    ET_PSR_1K,
    ET_PSR_32,
    ET_PSR_8M,
};

enum _EXT_TIMER_IDX_{
    EXT_TIMER_3 = 0,    // EC timer
    EXT_TIMER_4,        // EC delay
    EXT_TIMER_5,
    EXT_TIMER_6,
    EXT_TIMER_7,        // FreeRTOS
    EXT_TIMER_8         // Sensor Fusion delay
};

enum _UNIT_TIME_ {
    UNIT_SECOND = 1,
    UNIT_MILLISECOND = 1000,
    UNIT_MICROSECOND = 1000000
};

//****************************************************************************
//  External function
//****************************************************************************
extern int timer_init(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32Cnt);
extern int timer_init_ms(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32MilliSec);
extern int timer_init_us(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32MicroSec);

extern void timer_count_restart(UINT8 u8Idx);
extern int timer_count_wait(UINT8 u8Idx);
extern int timer_delay(UINT8 u8Idx);

extern UINT32 timer_get_count(UINT8 u8Idx);

extern int timer_combine_init(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32Timeout, UINT32 u32TimeUnit);
extern int timer_combine_deinit(UINT8 u8Idx);
extern void timer_combine_count_restart(UINT8 u8Idx);
extern int timer_combine_count_wait(UINT8 u8Idx);
extern int timer_combine_delay(UINT8 u8Idx);
extern UINT64 timer_combine_get_count(UINT8 u8Idx);

//****************************************************************************
#endif //__EXT_TIMER_H__

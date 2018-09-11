//*****************************************************************************
//
//  oem_timer.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************
#ifndef OEM_TIMER_H
#define OEM_TIMER_H

/*===========================================================================
 * Timer count setup
 * ======================================================================== */
#define NOTIME          0    //Indicated Zero
#define Timer_50       50    //Indicated 50  count
#define Timer_40       40    //Indicated 40  count
#define Timer_30       30    //Indicated 30  count
#define Timer_20    20    //Indicated 20  count
#define Timer_10    10    //Indicated 10  count
#define Timer_9      9     //Indicated 9   count
#define Timer_8      8     //Indicated 8   count
#define Timer_7      7     //Indicated 7   count
#define Timer_6      6     //Indicated 6   count
#define Timer_5      5     //Indicated 5   count
#define Timer_4      4     //Indicated 4   count
#define Timer_3      3     //Indicated 3   count
#define Timer_2      2     //Indicated 2   count
#define Timer_1      1    //Indicated 1   count

extern void SystemAutoOn(void);
extern void InternalWDTNow(void);
extern void EnableInternalWDT(void);
extern void DisableInternalWDT(void);
extern void ResetInternalWDT(void);
extern void ExternalWDTNow(void);
extern void InitClearExTimerHighByte3(void);



#endif

//*****************************************************************************
//
//  oem_event.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_INPUT_H
#define OEM_INPUT_H

struct sDebounce
{
	BYTE			*REG_Event;
	BYTE			Flag_Event;
	BYTE			*Cunter_Event;
	BYTE			Time_Event;
	FUNCT_PTR_V_V	press;
	FUNCT_PTR_V_V	release;
	FUNCT_PTR_V_V	nochange;	
};


//-----------------------------------------------------------------------------
// OEM_Event.C Debounce time setting
//-----------------------------------------------------------------------------
#define T_AC_DEBOUNCE          1       // 10 ms (timebase: 10 ms)
#define T_PSW_DEBOUNCE         5      // 50 ms (timebase: 10 ms)
#define T_BAT_DEBOUNCE         15      // 150 ms (timebase: 10 ms)
#define T_PSWOFF               500   // 5 secs (timebase: 10ms) EC 5sec override turn off system power
#define T_EXTEVT_DEBOUNCE      10      // 100 ms (timebase: 10 ms)
#define T_LAN_WAKE_DEBOUNCE    30      // 300 ms (timebase: 10 ms)
#define T_KBFN_DEBOUNCE        1      // 10 ms (timebase: 10 ms)
#if Support_BOTTOM_TAMPER_FUNC
#define T_BOTTOM_COVER_DEBOUNCE  2      // 20 ms (timebase: 10 ms)
#endif

#define max_EVENTSIZE			10

extern void EventManager(BYTE device_id);
extern void BatteryIn(int battery_id);
extern void BatteryOut(int battery_id);
extern void LanWakeLow(void);
extern void PSWPressed(void);
extern void PSWReleased(void);
extern void WLanWakeLow(void);

#endif

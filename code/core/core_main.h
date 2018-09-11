/* 
 * ****************************************************************************
 * TITLE: CORE_MAIN.H
 * 
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 * update
 *
 *  core_main.h
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/13
 *
 *      Author: ITE00580, Dino Li
 *
 * ****************************************************************************
 */
 
#ifndef CORE_MAIN_H
#define CORE_MAIN_H 

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define Only_Timer1msEvent  0x01
#define All_TimerEvent      0x00
#define Normal_MainService  0xFF

#define exitFlashMark1		0x33	
#define exitFlashMark2		0x88	
#define autoOnMark1			0x53	
#define autoOnMark2			0x35		
#define exitFollowMode		0xFC	
#define afterFlashMode		0xFD  //A285D00006


/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
int main(void);
extern void main_service(void);
void service_unlock(void);
void service_Low_LVEvent(void);
void service_1mS(void); 
void ISR_Timer1msEvent(void);
void ISR_Timer5msEvent(void);
void ISR_Timer10msEvent(void);
void Timer1msEvent(void);
void Timer5msEvent(void);
void Timer10msEventA(void);
void Timer10msEventB(void);
void Timer50msEventA(void);
void Timer50msEventB(void);
void Timer50msEventC(void);
void Timer100msEventA(void);
void Timer100msEventB(void);
void Timer100msEventC(void);
void Timer500msEventA(void);
void Timer500msEventB(void);
void Timer500msEventC(void);
void Timer1SecEventA(void);
void Timer1SecEventB(void);
void Timer1SecEventC(void);
void Timer1MinEvent(void);
extern BYTE CheckEventPending(void);
extern void service_extend_task(void);

extern BYTE CheckCrisisMode(void);
extern BYTE CheckCanEnterDeepSleep(void);
extern void InitEnterDeepSleep(void);
extern void InitWakeFromDeepSleep(void);
extern void InitEnterDeepSleepForAC(void);  //A485D00090
extern void InitWakeFromDeepSleepForAC(void);  //A485D00090

extern void Sleep_Now(); 

extern void Direct_Map_Core_Flash(void);
extern void CacheDma(BYTE sramnum,DWORD addr, BYTE option);

#endif  


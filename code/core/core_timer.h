/* 
 * ****************************************************************************
 * core_timer.h
 * ****************************************************************************
 */

#ifndef CORE_TIMERS_H
#define CORE_TIMERS_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
/* 2ms */
#define T_ACPI_BurstTime    2
#define ExTimerOverflow     0x01
#define ExTimerNoOverflow   0x02

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Init_Timers(void);
extern void Enable_ETimer_T(DWORD time_out_ms);
extern void Stop_ETimer_T(void);
extern BYTE Check_ETimer_T_Overflow(void);
extern void Delay1MS(DWORD bMS);
extern void DelayXms(DWORD bMS);
extern void Core_Disable_Period_External_Timer_Interrupt(void);
extern void Core_Enable_Period_External_Timer_Interrupt(void);
extern void InstructionDelayXms(int ms);
#endif 


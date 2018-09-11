/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_etwd.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_ETWD_H
#define API_ETWD_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
/* For watchdog timer (WDT) */
#define ExternalTimer_1             0xFF
#define ExternalTimer_2             0
#define ExternalTimer_3             1
#define ExternalTimer_4             2
#define ExternalTimer_5             3
#define ExternalTimer_6             4
#define ExternalTimer_7             5
#define ExternalTimer_8             6

/* do not neet control register */
#define ETWD_ctrl_none              (uchar_8 *) 0xFFFF

/* The external timer has counted down to zero */
#define ExternalTimer_TC            1

/* The external timer counting */
#define ExternalTimer_NoTC          0

/* External WDT is re-started (touched) */
#define ExternalWDTKeyRestart       0x5C

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Enable_External_Timer_x(BYTE p_et_selection, DWORD p_et_time);
extern void Disable_External_Timer_x(BYTE p_et_selection);
extern BYTE Check_External_Timer_x_Terminal_Count(BYTE p_et_selection);
extern void Enable_External_WDT(DWORD p_wdt_time);
extern void Reset_External_WDT(void);
extern void Disable_External_WDT(void);
extern void Enable_External_Timer_Int_x(BYTE p_et_selection);

/* 
 * ****************************************************************************
 * ETWD registers struct
 * ****************************************************************************
 */
typedef struct ETWD_Reg
{
    /* external timer x control register */
	uchar_8	*ETWD_ctrl;
    /* external timer x prescaler register */
	uchar_8	*ETWD_psr;
    /* external timer x counter low byte */
	uchar_8	*ETWD_cntllr;
    /* external timer x counter high byte */
	uchar_8	*ETWD_cntlhr;
    /* external timer x counter high byte2 */
	uchar_8	*ETWD_cntlh2r;
    /* external timer interrupt enable register. */
	uchar_8 *ETWD_intc_ier;
    /* external timer interrupt status register. */
	uchar_8 *ETWD_intc_isr;
	/* external timer interrupt Edge register. */
	uchar_8 *ETWD_intc_edge;
    /* external timer interrupt control bit of register. */
	BYTE    ETWD_intc_ctrl;
} sETWD_Reg;

#endif


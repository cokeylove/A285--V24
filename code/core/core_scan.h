/* 
 * ****************************************************************************
 * TITLE: CORE_SCAN.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 * ****************************************************************************
 */

#ifndef CORE_SCAN_H
#define CORE_SCAN_H

#include "../oem/Project_H/INCLUDE/oem_project.h"

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define KSO16CtrlReg	GPCRC3
#define KSO17CtrlReg	GPCRC5

/* Firmware scan keyboard */
#ifndef KB_HW_KeyScan
/* 
 * ****************************************************************************
 * Function prototype
 * ****************************************************************************
 */
void debounce_key(void);
void change_valid(void);
void check_scan(BYTE changes, BYTE scan_address);
void cscfnd(BYTE bit_num, BYTE scan_address);
void setup_debounce(BYTE bit_num, BYTE scan_address, BYTE event);
FLAG find_paths(BYTE change_make_key, BYTE scan_address);
void Setup_Diode_Key(void);
extern BYTE scan_keys(void);
void Write_Strobe(BYTE scan_line_num);
BYTE Read_Scan_Lines(void);
extern void Setup_Ext_Hotkey_Pntr(void);
/* For extend keys */
BYTE bScanExtendKeys(void);
void SetExtendScanLines(BYTE scan_line);
void CheckExtendKeys(BYTE bit_num, BYTE scan_address);
void ExtendKeyDebounce(BYTE KSI_bit_num, BYTE scan_address,
                                    BYTE event);
void DebounceExtendkey(void);
void CheckExtendKeyValid(void);
void Send_EtScan2(union KEY key, BYTE event);
void ClearExtendKeys(void);
void CheckEtKeystm(union KEY key);
/* Hardware scan keyboard */
#else
extern void HW_KB_Check_TM(void);
extern void Clear_AllKeysBreak(void);
/* #ifndef KB_HW_KeyScan */
#endif

extern FLAG Buffer_Key(BYTE row_column);
extern BYTE Buffer_String(const BYTE *pntr);
extern void Buffer_Mark(void);
extern void service_scan(void);
extern BYTE Get_Buffer(void);
extern void Init_Kbd(void);
extern void Clear_Key(void);
extern void Clear_Typematic(void);
extern void CheckKSO1617Support(void);
extern void Scan_Init(void);
extern void Enable_Any_Key_Irq(void);
extern BYTE PS2_Scan_Code_To_USB_HID(void);
extern BYTE check_keyscan_buffer_half_full(void);
extern FLAG Get_Kbd_Type(void);
extern void Unlock_Scan(void);
extern void Lock_Scan(void);
extern void Set_Typematic(WORD type_rate);
extern void check_tm(uKEY key);
#endif


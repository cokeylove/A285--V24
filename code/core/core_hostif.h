/* 
 * ****************************************************************************
 * TITLE: CORE_HOSTIF.H - Header File for Host Interface Handler
 *
 * The manufacturer identification (uses Translated key codes) and the
 * controller type.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 * ****************************************************************************
 */
 
#ifndef CORE_HOSTIF_H 
#define CORE_HOSTIF_H

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Data_To_Host(BYTE data_byte);
extern void Data_To_Host_nWait(BYTE data_byte);
extern void Aux_Data_To_Host(BYTE data_byte);
extern void SetServiceSendFlag(void);
extern void service_send(void);
extern BYTE get_response(void);
BYTE send_to_pc(BYTE data_word, BYTE break_prefix_flag);
BYTE common_send_to_pc(BYTE data_word, BYTE break_prefix_flag);
BYTE translate_to_pc(BYTE data_word, BYTE break_prefix_flag);
extern void KBC_DataPending(BYTE nPending);
extern BYTE GetKB_PendingData(void);
extern void KBC_DataToHost(BYTE nKBData);
extern void Core_InitSio(void);

#endif  


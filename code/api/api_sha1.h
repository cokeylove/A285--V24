/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_sha1.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_SHA1_H
#define API_SHA1_H

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void SHA1_Set_W_H_K(BYTE *sha1_buffer_addr);
extern BYTE * FW_SHA1(BYTE *sha1_buffer_addr);

#endif


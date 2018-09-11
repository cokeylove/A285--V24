#ifndef __DEBUG_PRINT__ // {
#define __DEBUG_PRINT__
// **************************************************************************
// debug_print.h
// 
// Implementation debug print
//
// Copyright (c) ITE, All Rights Reserved
//
// **************************************************************************
//	#include <stdio.h>
#include "debug_ns16550.h"

// **************************************************************************
// External Function
// **************************************************************************
extern void InitDebugPrint(void);

#ifdef __ENABLE_DBG_MSG__ //{
    extern int  rtl_printf(const char *format, ...);
    extern int  rtl_sprintf(char *buf, const char *format, ...);
    extern void DLDumpMem(unsigned char* Addr, unsigned long Len);    
    extern void DEBUG_PORT(unsigned char u8Data);    
#else   //  __ENABLE_DBG_MSG__
    extern int  rtl_sprintf(char *buf, const char *format, ...);

    #define rtl_printf(...)
    #define DLDumpMem(_x_,_y_)              
    #define DEBUG_PORT(_Data_)              
#endif // } __ENABLE_DBG_MSG__

// ***************************************************************
//  Debug API
// ***************************************************************
#define RTL_PRINTF(fmt, arg...)             rtl_printf(fmt, ##arg)
#define printk                              RTL_PRINTF

#ifndef _STDIO_H_ // {
#define printf                              RTL_PRINTF
#define sprintf(buf, fmt, arg...)           rtl_sprintf(buf, fmt, ##arg)
#endif // } _STDIO_H_

// **************************************************************************
#endif // } __DEBUG_PRINT__

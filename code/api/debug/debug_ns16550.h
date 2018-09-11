#ifndef __DEBUG_NS16550_H__ //{
#define __DEBUG_NS16550_H__
/*
 * NS16550 Serial Port
 * originally from linux source (arch/ppc/boot/ns16550.h)
 * modified slightly to
 * have addresses as offsets from CFG_ISA_BASE
 * added a few more definitions
 * added prototypes for ns16550.c
 * reduced no of com ports to 2
 * modifications (c) Rob Taylor, Flying Pig Systems. 2000.
 * further modified to support the 8245 duart
 * modifications (c) Paul Jimenez, Musenki, Inc. 2001.
 */

#ifdef  __ENABLE_DBG_MSG__  // {
extern void Uart_Init(void);
extern void debug_putc(unsigned char c);
#else
#define Uart_Init()         ((void)0)
#define debug_putc(_c_)     ((void)0)
#endif  // } __ENABLE_DBG_MSG__

#endif //} __DEBUG_NS16550_H__


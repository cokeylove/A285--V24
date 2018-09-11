#ifndef OS_CPU_H
#define OS_CPU_H
//*************************************************************************************************
//
//*************************************************************************************************
#include "n12_def.h"

#ifndef __ASSEMBLY__    // {
/* ***********************************************************************************************
 * *
 * *                                              DATA TYPES
 * *                                         (Compiler Specific)
 * *
 * ***********************************************************************************************/
typedef unsigned char   BOOLEAN;
typedef unsigned char   INT8U;                  // Unsigned  8 bit quantity
typedef signed   char   INT8S;                  // Signed    8 bit quantity 
typedef unsigned short  INT16U;                 // Unsigned 16 bit quantity
typedef signed   short  INT16S;                 // Signed   16 bit quantity 
typedef unsigned int    INT32U;                 // Unsigned 32 bit quantity
typedef signed   int    INT32S;                 // Signed   32 bit quantity 
typedef float           FP32;                   // Single precision floating point
typedef double          FP64;                   // Double precision floating point

typedef unsigned long   OS_STK;                 // Each stack entry is 32-bit wide
typedef unsigned long   OS_CPU_SR;             // Define size of CPU status register (PSR = 32 bits)

//*************************************************************************************************
//  External Function
//*************************************************************************************************
extern void _code_start(void);
extern void OSCtxSw(void);
extern void CtxSave(void);
extern void CtxRestore(void);

//*************************************************************************************************
#else

    .macro gie_disable
        setgie.d
        dsb
    .endm

    .macro gie_enable
        setgie.e
        dsb
    .endm

    .macro push_d0d1
    #if (defined(NDS32_BASELINE_V2) && defined(NDS32_DX_REGS)) || (!defined(NDS32_BASELINE_V2) && (defined(NDS32_DIV)||defined(NDS32_MAC)))
    	mfusr   $r1, $d1.lo
    	mfusr   $r2, $d1.hi
    	mfusr   $r3, $d0.lo
    	mfusr   $r4, $d0.hi
    	pushm	$r1, $r4
    #endif
    .endm

    .macro pop_d0d1
    #if (defined(NDS32_BASELINE_V2) && defined(NDS32_DX_REGS)) || (!defined(NDS32_BASELINE_V2) && (defined(NDS32_DIV)||defined(NDS32_MAC)))
    	popm	$r1, $r4
    	mtusr   $r1, $d1.lo
    	mtusr   $r2, $d1.hi
    	mtusr   $r3, $d0.lo
    	mtusr   $r4, $d0.hi
    #endif
    .endm

    .macro  SAVE_ALL_HW		!should not touch $r0
    	/* push caller-saved gpr */
    	pushm	$r1, $r5
    	pushm	$r15, $r30
    	push_d0d1
    	
    #if !(defined(NDS32_BASELINE_V3M) || defined(NDS32_BASELINE_V3))
    	/* push $INT_MASK */
    	mfsr	$r1, $INT_MASK
    	push	$r1		!push $INT_MASK
    
    	/* disable low priority and 
    	 * enable high priority interrupt */
    	ori	$r1, $r1, 0x3f	! r1= {mask[31:6],000000}
    	li	$r2, 1
    	sll	$r2, $r2, $r0	! 1<<r0
    	subri	$r2, $r2, 64	! 64 - (1 << r0)
    	sub	$r1, $r1, $r2	! {mask[31:6],000000} - (64- (1<<r0))
    	
    	mtsr	$r1, $INT_MASK
    
    #endif
    	/* push $PSW, $IPC, $IPSW */
    	mfsr    $r1, $PSW
    	mfsr    $r2, $IPC
    	mfsr    $r3, $IPSW
    	pushm	$r1, $r3
    	
    	/* You can use -1 if you want to 
    	 * descend interrupt level and enable gie or
    	 * you can enable gie when you finish your top
    	 * half isr. */
    	/* Descend interrupt level */
    	addi	$r1, $r1, #-2
    	mtsr    $r1, $PSW
    	
    	/* align $sp to 8B boundary */	
    	move	$r2, $sp		!keep original $sp to be pushed
    #if !(defined(NDS32_BASELINE_V3M) || defined(NDS32_BASELINE_V3))
    	andi	$r3, $sp, #4
    	subri   $r3, $r3, #4            ! bit2==0? 4 : 0
    	sub	$sp, $sp, $r3		! bit2==0? (sp-=4) : (sp-=0)
    #endif
    	push	$r2
    	!push_fpu
    .endm

    .macro  RESTORE_ALL_HW
    	/* pop fpu*/
    	!pop_fpu
    	lwi	$sp, [$sp]
    
    	/* pop $PSW, $IPC, $IPSW*/
    	popm	$r0, $r2
    	gie_disable
    	mtsr    $r0, $PSW
    	mtsr    $r1, $IPC
    	mtsr    $r2, $IPSW
    
    #if !(defined(NDS32_BASELINE_V3M) || defined(NDS32_BASELINE_V3))
    	/* pop $INT_MASK */
    	pop	$r0		! get $INT_MASK
    	mtsr	$r0, $INT_MASK	! restore $INT_MASK
    #endif
    	/* pop d0d1 */
    	pop_d0d1
    
    	/* pop gpr */
    	popm	$r15,$r30
    	popm	$r1, $r5
    	pop	$r0
   .endm
#endif // __ASSEMBLY__
//*************************************************************************************************   
#endif // OS_CPU_H
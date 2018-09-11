
#include "../include/config.h"

#ifndef EC_FEATURE_ONLY
##-----------------------------------------------------------------------------
## RTOS
##-----------------------------------------------------------------------------

##-----------------------------------------------------------------------------
##	INCLUDE ASSEMBLY CONSTANTS
##-----------------------------------------------------------------------------
#include "../../api/free_rtos/rtos-freertos/freertos/FreeRTOSConfig.h"
#include "../../oem/oem_project.h"

##-----------------------------------------------------------------------------
##
##	Interruption Vector Table
##
##-----------------------------------------------------------------------------
	.section .ite8380_isr, "ax"

    .align  2
ISR_TABLE:
    .long   Interrupt_HW0
    .long   Interrupt_HW1
    .long   Interrupt_HW2
    .long   Interrupt_HW3
    .long   Interrupt_HW4
    .long   Interrupt_HW5
    .long   Interrupt_HW6
    .long   Interrupt_HW7
    .long   Interrupt_HW8
    .long   Interrupt_HW9
    .long   Interrupt_HW10
    .long   Interrupt_HW11
    .long   Interrupt_HW12
    .long   Interrupt_HW13
    .long   Interrupt_HW14
    .long   Interrupt_HW15
    .long   Interrupt_SW0

    .global __OS_Int_Stack
    .global OSCtxSw
    .global CtxSave
    .global CtxRestore  
    .global OS_TLB_Fill
    .global OS_PTE_Not_Present
    .global OS_TLB_Misc
    .global OS_TLB_VLPT_Miss
    .global OS_Machine_Error
    .global OS_Debug_Related
    .global OS_General_Exception
    .global OS_Syscall
    .global OS_Interrupt_HW0
    .global OS_Interrupt_HW1
    .global OS_Interrupt_HW2
    .global OS_Interrupt_HW3
    .global OS_Interrupt_HW4
    .global OS_Interrupt_HW5
    .global OS_Interrupt_HW6
    .global OS_Interrupt_HW7
    .global OS_Interrupt_HW8
    .global OS_Interrupt_HW9
    .global OS_Interrupt_HW10
    .global OS_Interrupt_HW11
    .global OS_Interrupt_HW12
    .global OS_Interrupt_HW13
    .global OS_Interrupt_HW14
    .global OS_Interrupt_HW15
    .global OS_Interrupt_SW0

    .equ MEMDUMP,   0x0081000

##-----------------------------------------------------------------------------
##	Switch to interruption level \lv
##-----------------------------------------------------------------------------
    .macro IntlSwitch lv
        mfsr    $a0, $PSW
        li      $a1, #~(PSW_mskINTL|PSW_mskHSS)
        and     $a0, $a0, $a1
        ori     $a0, $a0, #(\lv << PSW_offINTL )
        mtsr    $a0, $PSW
        isb
    .endm
##-----------------------------------------------------------------------------
##  Exception Dump
##-----------------------------------------------------------------------------
    .macro MemDump val
        la      $r0, MEMDUMP
        li      $r1, #(\val)
        swi     $r1, [$r0]
    .endm
##-----------------------------------------------------------------------------
##	Save current task's context
##-----------------------------------------------------------------------------
    .macro CtxSaveMacro
        ! assembler will distinguish whether it uses 
        ! NDS32_REDUCE_REGS or not when use this pseudo instruction
        ! push caller-saved gpr
        pushm   $r1, $r30

        ! adjust sp and push sp
        move    $r2, $sp
        andi    $r1, $sp, #7
        bnez    $r1, 1f
        push    $r2
1:
		push    $r2

        ! push $PSW, $IPC, $IPSW 
        mfsr    $r1, $PSW
        mfsr    $r2, $IPC
        mfsr    $r3, $IPSW
        pushm   $r1, $r3

		move    $r1, $sp
		push    $r1
		
        ! Store the new top of stack for the task.  
		la      $r2, pxCurrentTCB	! pxCurrentTCB->pxTopOfStack = $SP
		lwi     $r1, [$r2]
		swi     $sp, [$r1]
    .endm
##-----------------------------------------------------------------------------
##	Restore to new task's context
##-----------------------------------------------------------------------------	
    .macro CtxRestoreMacro
        ! use new pxCurrentTCB to get new stack pointer
		la      $r2, pxCurrentTCB
		lwi     $r1, [$r2]
		lwi     $sp, [$r1]		! $SP = pxCurrentTCB->pxTopOfStack
        ! pop sp
		pop	    $r1
		move    $sp, $r1

        ! pop $PSW, $IPC, $IPSW
        popm    $r1, $r3
        mtsr    $r1, $PSW
        mtsr    $r2, $IPC
        mtsr    $r3, $IPSW
		
        ! pop sp
		pop     $r1
		move    $sp, $r1

        ! pop gpr
        popm    $r1, $r30
        pop     $r0
        iret
	.endm

##-----------------------------------------------------------------------------
##   Exception Event
##-----------------------------------------------------------------------------
OS_TLB_Fill:
    MemDump 1
    b   OS_TLB_Fill

OS_PTE_Not_Present:
    MemDump 2
    b   OS_PTE_Not_Present

OS_TLB_Misc:
    MemDump 3
    b   OS_TLB_Misc

OS_TLB_VLPT_Miss:
    MemDump 4
    b   OS_TLB_VLPT_Miss

OS_Machine_Error:
    MemDump 5
    b   OS_Machine_Error

OS_Debug_Related:
    MemDump 6
    b   OS_Debug_Related

OS_General_Exception:
    #ifndef  ENABLE_IVT_MAP_TO_FLASH
    MemDump 7
    b   OS_General_Exception
    #else
    bal General_Exception
    iret
    #endif

OS_Syscall:
    MemDump 8
    b   OS_Syscall

##-----------------------------------------------------------------------------
##   Interrupt Event
##-----------------------------------------------------------------------------
OS_Interrupt_HW0:
    push 	$r0
    li      $r0, 0x00
    b       IRQ_ISR_HANDLE
    
OS_Interrupt_HW1:
    push 	$r0
    li      $r0, 0x01
    b       IRQ_ISR_HANDLE
	
OS_Interrupt_HW2:
    push 	$r0
    li      $r0, 0x02
    b       IRQ_ISR_HANDLE
	
OS_Interrupt_HW3:
    push 	$r0
    li      $r0, 0x03
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW4:
    push 	$r0
    li      $r0, 0x04
    b       IRQ_ISR_HANDLE
	
OS_Interrupt_HW5:
    push 	$r0
    li      $r0, 0x05
    b       IRQ_ISR_HANDLE
	
OS_Interrupt_HW6:
    push 	$r0
    li      $r0, 0x06
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW7:
    push 	$r0
    li      $r0, 0x07
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW8:
    push 	$r0
    li      $r0, 0x08
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW9:
    push 	$r0
    li      $r0, 0x09
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW10:
    push    $r0
    li      $r0, 0x0A
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW11:
    push    $r0
    li      $r0, 0x0B
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW12:
    push    $r0
    li      $r0, 0x0C
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW13:
    push    $r0
    li      $r0, 0x0D
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW14:
    push    $r0
    li      $r0, 0x0E
    b       IRQ_ISR_HANDLE

OS_Interrupt_HW15:
    push    $r0
    li      $r0, 0x0F
    b       IRQ_ISR_HANDLE

OS_Interrupt_SW0:
    push    $r0
    li      $r0, 0x10
    b       IRQ_ISR_HANDLE

##-----------------------------------------------------------------------------
## IRQ Interrupt Service Routine
##-----------------------------------------------------------------------------
    .section ._IRQ_ISR_HND,  "ax"
    
##-----------------------------------------------------------------------------
IRQ_ISR_HANDLE:
    CtxSaveMacro                ! Save current task's context
    la      $sp, __OS_Int_Stack ! INT_Stack
    la      $r1, ISR_TABLE
    lw      $r1, [$r1+$r0<<2]
    jral    $r1                 ! Call ISR
    CtxRestoreMacro             ! Restore to origianl task's context

##-----------------------------------------------------------------------------
##
##	RTOS Kernel and Context Switch
##
##-----------------------------------------------------------------------------
    .section .RTOS_CODE,  "ax"

##-----------------------------------------------------------------------------
##                   PERFORM A CONTEXT SWITCH - OSCtxSw()
##
## Note(s) : 1) OSIntCtxSw() is called with IRQ DISABLED
##
##           2) The pseudo-code for OSCtxSw() is:
##              a) $IPSW <- $PSW, $IPC <- $lp
##              c) CtxSaveMacro         : Save current task's context
##              d) vTaskSwitchContext   : get the highest priority of Task
##              e) IntlSwitch           : Switch to interruption level 1
##              f) CtxRestoreMacro      : Restore to the highest priority task's context 
##
##-----------------------------------------------------------------------------
OSCtxSw:
    push    $r0
    mfsr    $r0, $PSW
    mtsr    $r0, $IPSW          ! $IPSW <- $PSW
    mtsr    $lp, $IPC           ! $IPC  <- $lp
    CtxSaveMacro                ! Save current task's context
    la      $sp, __OS_Int_Stack ! INT_Stack
    bal     vTaskSwitchContext  ! get the highest priority of Task
    CtxRestoreMacro             ! Restore to origianl task's context
    
##-----------------------------------------------------------------------------
##  Context Save
##-----------------------------------------------------------------------------
CtxSave:
    push 	$r0
    mfsr    $r0, $PSW
    mtsr    $r0, $IPSW
    mtsr    $lp, $IPC
    CtxSaveMacro
    ret

##-----------------------------------------------------------------------------
##  Context Restore
##-----------------------------------------------------------------------------
CtxRestore:
    CtxRestoreMacro

##-----------------------------------------------------------------------------
## POINTERS TO VARIABLES
##-----------------------------------------------------------------------------
    .data
    .align 3
    .skip configIRQ_STACK_SIZE
__OS_Int_Stack:

    .end
##-----------------------------------------------------------------------------

#else
##-----------------------------------------------------------------------------
## EC only
##-----------------------------------------------------------------------------

##-----------------------------------------------------------------------------
## Assamble Macro
##-----------------------------------------------------------------------------
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
    pushm $r1, $r30

    /* push d0d1 */
	push_d0d1

	/* push $PSW, $IPC, $IPSW */
	mfsr    $r1, $PSW
	mfsr    $r2, $IPC
	mfsr    $r3, $IPSW
	pushm	$r1, $r3
	
	/* align $sp to 8B boundary */	
	!## move	$r2, $sp		!keep original $sp to be pushed
	!## push	$r2
    .endm

    .macro  RESTORE_ALL_HW
	!## lwi	$sp, [$sp]

	/* pop $PSW, $IPC, $IPSW*/
	popm	$r0, $r2

	mtsr    $r0, $PSW
	mtsr    $r1, $IPC
	mtsr    $r2, $IPSW

	/* pop d0d1 */
	pop_d0d1

	/* pop gpr */	
	popm	$r1,  $r30
	pop	    $r0
   .endm

##-----------------------------------------------------------------------------
## ISR Table
##-----------------------------------------------------------------------------
    .section .ite8380_isr, "ax"
    .align  2
ISR_TABLE:
    .long   Interrupt_HW0
    .long   Interrupt_HW1
    .long   Interrupt_HW2
    .long   Interrupt_HW3
    .long   Interrupt_HW4
    .long   Interrupt_HW5
    .long   Interrupt_HW6
    .long   Interrupt_HW7
    .long   Interrupt_HW8
    .long   Interrupt_HW9
    .long   Interrupt_HW10
    .long   Interrupt_HW11
    .long   Interrupt_HW12
    .long   Interrupt_HW13
    .long   Interrupt_HW14
    .long   Interrupt_HW15
    .long   Interrupt_SW0
	
    .global OS_TLB_Fill
    .global OS_PTE_Not_Present
    .global OS_TLB_Misc
    .global OS_TLB_VLPT_Miss
    .global OS_Machine_Error
    .global OS_Debug_Related
    .global OS_Syscall
    .global OS_General_Exception
    .global OS_Interrupt_HW0
    .global OS_Interrupt_HW1
    .global OS_Interrupt_HW2
    .global OS_Interrupt_HW3
    .global OS_Interrupt_HW4
    .global OS_Interrupt_HW5
    .global OS_Interrupt_HW6
    .global OS_Interrupt_HW7
    .global OS_Interrupt_HW8
    .global OS_Interrupt_HW9
    .global OS_Interrupt_HW10
    .global OS_Interrupt_HW11
    .global OS_Interrupt_HW12
    .global OS_Interrupt_HW13
    .global OS_Interrupt_HW14
    .global OS_Interrupt_HW15
    .global OS_Interrupt_SW0

##-----------------------------------------------------------------------------
## ISR
##-----------------------------------------------------------------------------
    .align 3
ISR_start:
common_ISR_wrapper:
    SAVE_ALL_HW
	la	    $r1, ISR_TABLE
	lw	    $r1, [$r1+$r0<<2]
	jral	$r1
	RESTORE_ALL_HW
	iret

##-----------------------------------------------------------------------------
!   Exception Event
##-----------------------------------------------------------------------------
OS_TLB_Fill:
    b   OS_TLB_Fill

OS_PTE_Not_Present:
    b   OS_PTE_Not_Present

OS_TLB_Misc:
    b   OS_TLB_Misc

OS_TLB_VLPT_Miss:
    b   OS_TLB_VLPT_Miss

OS_Machine_Error:
    b   OS_Machine_Error

OS_Debug_Related:
    b   OS_Debug_Related

OS_General_Exception:  
    #ifndef  ENABLE_IVT_MAP_TO_FLASH
    b   OS_General_Exception
    #else
    bal General_Exception
    iret
    #endif

OS_Syscall:
    b   OS_Syscall

##-----------------------------------------------------------------------------
!   Interrupt Event
##-----------------------------------------------------------------------------
OS_Interrupt_HW0:
    push    $r0
    li      $r0, 0x00
    b       common_ISR_wrapper

OS_Interrupt_HW1:
    push    $r0
    li      $r0, 0x01
    b       common_ISR_wrapper

OS_Interrupt_HW2:
    push    $r0
    li      $r0, 0x02
    b       common_ISR_wrapper

OS_Interrupt_HW3:
    push    $r0
    li      $r0, 0x03
    b       common_ISR_wrapper

OS_Interrupt_HW4:
    push    $r0
    li      $r0, 0x04
    b       common_ISR_wrapper

OS_Interrupt_HW5:
    push    $r0
    li      $r0, 0x05
    b       common_ISR_wrapper

OS_Interrupt_HW6:
    push    $r0
    li      $r0, 0x06
    b       common_ISR_wrapper

OS_Interrupt_HW7:
    push    $r0
    li      $r0, 0x07
    b       common_ISR_wrapper

OS_Interrupt_HW8:
    push    $r0
    li      $r0, 0x08
    b       common_ISR_wrapper

OS_Interrupt_HW9:
    push    $r0
    li      $r0, 0x09
    b       common_ISR_wrapper

OS_Interrupt_HW10:
    push    $r0
    li      $r0, 0x0A
    b       common_ISR_wrapper

OS_Interrupt_HW11:
    push    $r0
    li      $r0, 0x0B
    b       common_ISR_wrapper

OS_Interrupt_HW12:
    push    $r0
    li      $r0, 0x0C
    b       common_ISR_wrapper

OS_Interrupt_HW13:
    push    $r0
    li      $r0, 0x0D
    b       common_ISR_wrapper

OS_Interrupt_HW14:
    push    $r0
    li      $r0, 0x0E
    b       common_ISR_wrapper

OS_Interrupt_HW15:
    push    $r0
    li      $r0, 0x0F
    b   common_ISR_wrapper

OS_Interrupt_SW0:
    push    $r0
    li      $r0, 0x10
    b       common_ISR_wrapper
ISR_end:

    .end
#endif

##==============================================================================
##
##	crt0.S
##
##	nds32 startup code
##
##==============================================================================
##
## Copyright (c) 1995, 1996, 1997, 1998 Cygnus Solutions
## 
## The authors hereby grant permission to use, copy, modify, distribute,
## and license this software and its documentation for any purpose, provided
## that existing copyright notices are retained in all copies and that this
## notice is included verbatim in any distributions. No written agreement,
## license, or royalty fee is required for any of the authorized uses.
## Modifications to this software may be copyrighted by their authors
## and need not follow the licensing terms described here, provided that
## the new terms are clearly indicated on the first page of each file where
## they apply.
##
	
#include "../include/config.h"
#include "../include/nds32_defs.h"
#include "../include/nds32_regs.inc"
#include "../../oem/Project_H/INCLUDE/oem_project.h"
##------------------------------------------------------------------------------
	
##*****************************************************************************
##
##	startup.s
##
##	ITE-8380 startup code
##
##*****************************************************************************
    .file   "crt0.S"
##-----------------------------------------------------------------------------
##
## Key to Flags:
## W (write), A (alloc), X (execute), M (merge), S (strings)
## I (info), L (link order), G (group), x (unknown)
## O (extra OS processing required) o (OS specific), p (processor specific)
##
##-----------------------------------------------------------------------------
    .section .ite8380_init,  "ax"

##-----------------------------------------------------------------------------
## .align 3' advances the location counter until it a multiple of 8. 
## If the location counter is already a multiple of 8, no change is needed. 
##-----------------------------------------------------------------------------
    .align 3
##-----------------------------------------------------------------------------
## interruption vector table start ++
##-----------------------------------------------------------------------------
Interruption_Vector_Table:  ! Interruption Vectored Entry Point
    j   _code_start	                    !  (0) Trap Reset
    j   OS_TLB_Fill                     !  (1) Trap TLB fill
    j   OS_PTE_Not_Present              !  (2) Trap PTE not present
    j   OS_TLB_Misc                     !  (3) Trap TLB misc
    j   OS_TLB_VLPT_Miss                !  (4) Trap TLB VLPT miss
    j   OS_Machine_Error                !  (5) Trap Machine error
    j   OS_Debug_Related                !  (6) Trap Debug Related
    j   OS_General_Exception            !  (7) Trap General Exception
    j   OS_Syscall                      !  (8) Trap System Call
    j   OS_Interrupt_HW0                !  (9) Interrupt HW0
    j   OS_Interrupt_HW1                ! (10) Interrupt HW1
    j   OS_Interrupt_HW2                ! (11) Interrupt HW2
    j   OS_Interrupt_HW3                ! (12) Interrupt HW3
    j   OS_Interrupt_HW4                ! (13) Interrupt HW4
    j   OS_Interrupt_HW5                ! (14) Interrupt HW5
    j   OS_Interrupt_HW6                ! (15) Interrupt HW6
    j   OS_Interrupt_HW7                ! (16) Interrupt HW7
    j   OS_Interrupt_HW8                ! (17) Interrupt HW8
    j   OS_Interrupt_HW9                ! (18) Interrupt HW9
    j   OS_Interrupt_HW10               ! (19) Interrupt HW10
    j   OS_Interrupt_HW11               ! (20) Interrupt HW11
    j   OS_Interrupt_HW12               ! (21) Interrupt HW12
    j   OS_Interrupt_HW13               ! (22) Interrupt HW13
    j   OS_Interrupt_HW14               ! (23) Interrupt HW14
    j   OS_Interrupt_HW15               ! (24) Interrupt HW15
    j   OS_Interrupt_SW0                ! (25) Interrupt SW0
    
    .size   Interruption_Vector_Table, .-Interruption_Vector_Table    
##-----------------------------------------------------------------------------
## interrupt vector table end --
##-----------------------------------------------------------------------------

##-----------------------------------------------------------------------------
## 16 bytes signature
##-----------------------------------------------------------------------------
    .org 0x80   !# 0    1    2    3    4    5    6                 7
    .byte       0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,EC_Signature_Flag
                !# 8    9   10   11   12                13   14   15
    .byte       0x85,0x12,0x5A,0x5A,0xAA,EC_Signature_Addr,0x55,0x55
##-----------------------------------------------------------------------------
## EC firmware version
##-----------------------------------------------------------------------------
    .byte       'I', 'T', 'E', '8', '3', '8', '0', '-', 'E', 'C', '-', 'V', '1', '.', '4', '3'
    .org 0xA0
##-----------------------------------------------------------------------------
## startup code
##-----------------------------------------------------------------------------
    .section ._code_start
    .weak   _SDA_BASE_
    .weak   _FP_BASE_
    .global _code_start
	
    ! ---------------------------------------------------
    .equ __DLM_BASE_ADDR,   0x00080000
    .equ __DLM_SIZE,        0x00000000
    .equ GCTRL_MCCR,        0x00F02030
    .equ GCTRL_MCCR1,       0x00F0203E
    .equ MASK_MCCR1,        0x80
    .equ GCTRL_IVTBADR,     0x00F02041
    .equ VAR_CPU_ILMB,      0x0000000F
    ! ---------------------------------------------------

    .align	2
    .extern Oem_StartUp
    !##.extern Core_Init_ClearRam
    .extern Init_ClearRam
    .extern CheckResetSource
##-----------------------------------------------------------------------------
##  code entry point
##-----------------------------------------------------------------------------	
_code_start:
    ! ---------------------------------------------------
    ! Set IVT Base Addr to 0x00, for exception
    ! ---------------------------------------------------
    #ifdef  ENABLE_IVT_MAP_TO_FLASH
    !la    $r1, GCTRL_IVTBADR
    li    $r1, GCTRL_IVTBADR
    li    $r0, 0
    sbi   $r0, [$r1]
    #endif

    ! ---------------------------------------------------
    ! Set ILM Base to 0x00, for AICE
    ! ---------------------------------------------------
    !la    $r0, VAR_CPU_ILMB
    li    $r0, VAR_CPU_ILMB
    mtsr  $r0, $ILMB

    ! ---------------------------------------------------
    ! Set DSSIM Disable, for DBGR
    ! ---------------------------------------------------
    mfsr  $r0, $INT_MASK
    !la    $r1, (~INT_MASK_mskDSSIM)
    li    $r1, (~INT_MASK_mskDSSIM)
    and   $r0, $r0, $r1
    mtsr  $r0, $INT_MASK

    ! ---------------------------------------------------
    ! Set IVB.ESZ = 0 (vector table entry size = 4 bytes)
    ! ---------------------------------------------------
    mfsr  $r1, $IVB
    li	  $r0, 0xc000
    or	  $r1, $r1, $r0
    xor	  $r1, $r1, $r0
    mtsr  $r1, $IVB
    isb

    ! ---------------------------------------------------
    ! Set IMMU and DLM - (IMMU cache size 8k byte, burst length 16byte)
    ! ---------------------------------------------------
    !la    $r1, GCTRL_MCCR
    li    $r1, GCTRL_MCCR
    lbi   $r0, [$r1]
    ori   $r0, $r0, 0x0C		! Enable DLM 8k~12K(bit2) and DLM 12k~16k(bit3)
    sbi   $r0, [$r1]

    ! ---------------------------------------------------
    ! Set $PSW.INTL to 0
    ! ---------------------------------------------------
    mfsr  $r0, $PSW
    ori   $r0, $r0, 0x6
    xori  $r0, $r0, 0x6
    mtsr  $r0, $PSW
    ori	  $r0, $r0, 1	        ! Enable IPSW GIE
    mtsr  $r0, $IPSW
    isb

    ! ---------------------------------------------------
    ! Initial DLMB(mr7) address and enable DLMB
    ! ---------------------------------------------------
    !la    $r0, __DLM_BASE_ADDR
    li    $r0, __DLM_BASE_ADDR
    ori   $r0, $r0, 0x01        ! set DEN to 1
   	mtsr  $r0, $DLMB
   	dsb

    ! ---------------------------------------------------
    ! Init $gp, $fp, $sp
    ! ---------------------------------------------------
    la    $gp, _SDA_BASE_       ! init GP for small data access
    la    $fp, _FP_BASE_        ! init FP
 
    
    #ifndef EC_FEATURE_ONLY
    ! ---------------------------------------------------
    ! RTOS
    ! ---------------------------------------------------
    !la	  $sp, _stack           ! init SP 
    li	  $sp, _stack           ! init SP 
    #else
    ! ---------------------------------------------------
    ! EC only
    ! ---------------------------------------------------
    la    $sp, _stack_ec        ! init SP 
    #endif

    ! ---------------------------------------------------
    ! Check Warm boot
    ! ---------------------------------------------------	
    #if 0 
    !TODO
   ! la    $r1, GCTRL_MCCR1
    li    $r1, GCTRL_MCCR1
    lbi   $r0, [$r1]
    andi  $r15, $r0, MASK_MCCR1
    bnez  $r15, 6f
    #endif

    ! ---------------------------------------------------
    ! Init clear sram (Core and OEM SRAM) -- [0x00080000 ~ 0x00081FFF]
    ! ---------------------------------------------------	
   ! la    $r1, __DLM_SIZE       ! Check Size != 0
    li    $r1, __DLM_SIZE       ! Check Size != 0
    beqz  $r1, 6f
   ! la    $r0, __DLM_BASE_ADDR  ! To clear SRAM 0x00080000 ~ 0x00081FFF
    li    $r0, __DLM_BASE_ADDR  ! To clear SRAM 0x00080000 ~ 0x00081FFF
    li    $r2, #0
_clear_sram:
    sbi   $r2, [$r0]
    addi  $r0, $r0, #1
    addi  $r1, $r1, #-1
    bnez  $r1, _clear_sram
6:
    ! ---------------------------------------------------
    ! Init DATA Section
    ! ---------------------------------------------------
    la    $r1, __data_size      ! Check Size != 0
    beqz  $r1, 5f
    la    $r0, __lma_data_start ! eFLASH
    la    $r2, __vma_data_start ! SRAM
2:
    lbi   $r3, [$r0]
    sbi	  $r3, [$r2]
    addi  $r1, $r1, #-1
    addi  $r0, $r0, #1
    addi  $r2, $r2, #1
    bnez  $r1, 2b
5:
    ! ---------------------------------------------------
    ! Init BSS Section
    ! ---------------------------------------------------
    la    $r1, __bss_size       ! Check Size != 0
    beqz  $r1, 4f
    la    $r0, __vma_bss_start  ! SRAM
   ! la    $r2, #0
    li    $r2, #0
3:	
    sbi   $r2, [$r0]
    addi  $r0, $r0, #1
    addi  $r1, $r1, #-1
    bnez  $r1, 3b
4:    
    ! ---------------------------------------------------
    ! For OME EC start up function
    ! ---------------------------------------------------
    bal Init_ClearRam
    bnez  $r15, 8f
    bal   Oem_StartUp		    ! For OME EC start up function
    !bal Core_Init_ClearRam
    !bal Init_ClearRam
    bal   CheckResetSource
8:
    #ifndef EC_FEATURE_ONLY
    ! ---------------------------------------------------
    ! RTOS
    ! ---------------------------------------------------
    bal   task_main
    #else
    ! ---------------------------------------------------
    ! EC only
    ! ---------------------------------------------------
    bal   main
    #endif
1:
    b     1b
    
    .size _code_start, .-_code_start
    !--------------------------------------------------------------------------
    .end
##==============================================================================
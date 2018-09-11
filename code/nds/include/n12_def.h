#ifndef __N12_DEF_H__
#define __N12_DEF_H__

#include "nds32_defs.h"
#include "Config.h"

#ifndef __ASSEMBLY__

#include "nds32_regs.h"
#include <nds32_intrinsic.h>

#define TLBOP(op)		    __asm__ volatile ("tlbop "#op);
#define MSYNC(subtype)		__asm__ volatile ("msync "#subtype);
#define STANDBY(cond)		__asm__ volatile ("standby "#cond);

static inline unsigned short Word_Swap(unsigned short in)
{
    unsigned short val;
	__asm__ volatile ("wsbh %0, %1" : "=&r" (val) : "r" (in) : "memory");
	return  val;
}

static inline unsigned long DWord_Swap(unsigned long in)
{
    unsigned long val, tmp;
	__asm__ volatile ("\n\twsbh %1, %2"
                      "\n\trotri %0, %1, 16"
                      : "=&r" (val), "=&r" (tmp): "r" (in) : "memory");
	return  val;
}

static inline void Syscall(unsigned short Idx)
{
    __nds32__syscall((Idx & 0x7FFF));
}

static inline void ISB(void)
{
	__asm__ volatile ("isb");
}

static inline void DSB(void)
{
	__asm__ volatile ("dsb");
}

static inline void GIE_ENABLE(void)
{
    __nds32__gie_en();
}

static inline void GIE_DISABLE(void)
{
    __nds32__gie_dis();
}

static inline void GIE_SAVE(unsigned long *var)
{
    *var = GET_PSW();
    //GIE_DISABLE();
}

static inline void GIE_RESTORE(unsigned long var)
{
    SET_PSW(var);
    //GIE_ENABLE();
}

void vPortEnterCritical(void);
void vPortExitCritical(void);

//
// rtos
//
#ifndef  EC_FEATURE_ONLY
#define SET_MASK_EXT(reg, bit_mask)                                     \
    do{                                                                 \
        vPortEnterCritical();                                           \
        /* Enter Critical Section */                                    \
        (reg)|=(bit_mask);                                              \
        /* Exit Critical Section */                                     \
        vPortExitCritical();                                            \
    }while(0)

#define CLEAR_MASK_EXT(reg, bit_mask)                                   \
    do{                                                                 \
        vPortEnterCritical();                                           \
        /* Enter Critical Section */                                    \
        (reg)&=(~(bit_mask));                                           \
        /* Exit Critical Section */                                     \
        vPortExitCritical();                                            \
    }while(0)
#else
//
// non-rtos
//
#define SET_MASK_EXT(reg, bit_mask)         \
    do{                                     \
        (reg)|=(bit_mask);                  \
    }while(0)

#define CLEAR_MASK_EXT(reg, bit_mask)       \
    do{                                     \
        (reg)&=(~(bit_mask));               \
    }while(0)

#endif

#endif /* __ASSEMBLY__ */

/***********************************
 * SYSTEM CONSTANTS
 ***********************************/
#define VECTOR_BASE         (0x00070000)

#define IRQ_STACK_SIZE		(512)	/* Define the IRQ stack size */
#define SYS_STACK_SIZE		(256)	/* Define the system stack size */

/***********************************
 * TARGET INITIALIZATION CONSTANTS
 ***********************************/

#define PSW_INIT					\
	(0x0UL << PSW_offGIE				\
	 | 0x3UL << PSW_offINTL				\
	 | 0x1UL << PSW_offPOM				\
	 | 0x0UL << PSW_offBE				\
	 | 0x0UL << PSW_offIT				\
	 | 0x0UL << PSW_offDT				\
	 | 0x0UL << PSW_offIME				\
	 | 0x0UL << PSW_offDME				\
	 | 0x0UL << PSW_offDEX				\
	 | 0x0UL << PSW_offHSS)

#define PSW_MSK						\
	(PSW_mskGIE					\
	  | PSW_mskINTL					\
	  | PSW_mskPOM					\
	  | PSW_mskBE					\
	  | PSW_mskIT					\
	  | PSW_mskDT					\
	  | PSW_mskIME					\
	  | PSW_mskDME					\
	  | PSW_mskDEX					\
	  | PSW_mskHSS)

#define INT_MASK_INIT					\
	(0x0UL << INT_MASK_offH0IM			\
	  | 0x1UL << INT_MASK_offH1IM			\
	  | 0x0UL << INT_MASK_offH2IM			\
	  | 0x0UL << INT_MASK_offH3IM			\
	  | 0x0UL << INT_MASK_offH4IM			\
	  | 0x0UL << INT_MASK_offH5IM			\
	  | 0x0UL << INT_MASK_offH6IM			\
	  | 0x0UL << INT_MASK_offH7IM			\
	  | 0x0UL << INT_MASK_offH8IM			\
	  | 0x0UL << INT_MASK_offH9IM			\
	  | 0x0UL << INT_MASK_offH10IM			\
	  | 0x0UL << INT_MASK_offH11IM			\
	  | 0x0UL << INT_MASK_offH12IM			\
	  | 0x0UL << INT_MASK_offH13IM			\
	  | 0x0UL << INT_MASK_offH14IM			\
	  | 0x0UL << INT_MASK_offH15IM			\
	  | 0x1UL << INT_MASK_offSIM			\
	  | 0x1UL << INT_MASK_offIDIVZE)

#define INT_MASK_MSK					\
	(INT_MASK_mskH0IM				\
	  | INT_MASK_mskH1IM				\
	  | INT_MASK_mskH2IM				\
	  | INT_MASK_mskH3IM				\
	  | INT_MASK_mskH4IM				\
	  | INT_MASK_mskH5IM				\
	  | INT_MASK_mskH6IM				\
 	  | INT_MASK_mskH7IM				\
 	  | INT_MASK_mskH8IM				\
 	  | INT_MASK_mskH9IM				\
 	  | INT_MASK_mskH10IM				\
 	  | INT_MASK_mskH11IM				\
 	  | INT_MASK_mskH12IM				\
 	  | INT_MASK_mskH13IM				\
 	  | INT_MASK_mskH14IM				\
 	  | INT_MASK_mskH15IM				\
	  | INT_MASK_mskSIM				\
	  | INT_MASK_mskIDIVZE)

#define IVB_MSK						\
	(IVB_mskEVIC					\
	  | IVB_mskESZ					\
	  | IVB_mskIVBASE)

#define IVB_INIT					\
	(0x0UL << IVB_offIVBASE			\
		| 0x0UL << IVB_offESZ			\
		| 0x0UL << IVB_offEVIC)

#define MMU_CTL_MSK					\
	(MMU_CTL_mskD					\
	  | MMU_CTL_mskNTC0				\
	  | MMU_CTL_mskNTC1				\
	  | MMU_CTL_mskNTC2				\
	  | MMU_CTL_mskNTC3				\
	  | MMU_CTL_mskTBALCK				\
	  | MMU_CTL_mskMPZIU				\
	  | MMU_CTL_mskNTM0				\
	  | MMU_CTL_mskNTM1				\
	  | MMU_CTL_mskNTM2				\
	  | MMU_CTL_mskNTM3)

#define MMU_CTL_INIT					\
	(0x0UL << MMU_CTL_offD				\
	  | 0x0UL << MMU_CTL_offNTC0			\
	  | 0x0UL << MMU_CTL_offNTC1			\
	  | 0x0UL << MMU_CTL_offNTC2			\
	  | 0x0UL << MMU_CTL_offNTC3			\
	  | 0x0UL << MMU_CTL_offTBALCK			\
	  | 0x0UL << MMU_CTL_offMPZIU			\
	  | 0x0UL << MMU_CTL_offNTM0			\
	  | 0x1UL << MMU_CTL_offNTM1			\
	  | 0x2UL << MMU_CTL_offNTM2			\
	  | 0x3UL << MMU_CTL_offNTM3)
#ifdef CONFIG_CPU_DCACHE_ENABLE
#ifdef CONFIG_CPU_DCACHE_WRITETHROUGH
	/* Write through */
	#define CACHE_ABILITY	0x3UL
#else
	/* Write back */
	#define CACHE_ABILITY	0x2UL
#endif
#else
	/* Device */
	#define CACHE_ABILITY	0x0UL
#endif

#define MMU_CTL_INIT_CACHE_ON				\
	(0x0UL << MMU_CTL_offD				    \
	  | CACHE_ABILITY << MMU_CTL_offNTC0	\
	  | 0x0UL << MMU_CTL_offNTC1			\
	  | 0x0UL << MMU_CTL_offNTC2			\
	  | 0x0UL << MMU_CTL_offNTC3			\
	  | 0x0UL << MMU_CTL_offTBALCK			\
	  | 0x0UL << MMU_CTL_offMPZIU			\
	  | 0x0UL << MMU_CTL_offNTM0			\
	  | 0x1UL << MMU_CTL_offNTM1			\
	  | 0x2UL << MMU_CTL_offNTM2			\
	  | 0x3UL << MMU_CTL_offNTM3)

#define CACHE_CTL_MSK					\
	(CACHE_CTL_mskIC_EN				    \
	  | CACHE_CTL_mskDC_EN				\
	  | CACHE_CTL_mskICALCK				\
	  | CACHE_CTL_mskDCALCK				\
	  | CACHE_CTL_mskDCCWF				\
	  | CACHE_CTL_mskDCPMW)

#define CACHE_CTL_INIT					    \
	(0x0UL << CACHE_CTL_offIC_EN			\
	  | 0x0UL << CACHE_CTL_offDC_EN			\
	  | 0x0UL << CACHE_CTL_offICALCK		\
	  | 0x0UL << CACHE_CTL_offDCALCK		\
	  | 0x1UL << CACHE_CTL_offDCCWF			\
	  | 0x1UL << CACHE_CTL_offDCPMW)

#define CACHE_CTL_INIT_CACHE_ON				\
	(0x1UL << CACHE_CTL_offIC_EN			\
	  | 0x1UL << CACHE_CTL_offDC_EN			\
	  | 0x0UL << CACHE_CTL_offICALCK		\
	  | 0x0UL << CACHE_CTL_offDCALCK		\
	  | 0x1UL << CACHE_CTL_offDCCWF			\
	  | 0x1UL << CACHE_CTL_offDCPMW)

#define DLMB_MSK					\
	(DLMB_mskDEN				    \
	  | DLMB_mskDLMSZ     		    \
	  | DLMB_mskDBM       		    \
	  | DLMB_mskDBB				    \
	  | DLMB_mskDBPA)

#define DLMB_INIT					\
	(0x0UL << DLMB_offDEN			\
	  | 0x0UL << DLMB_offDLMSZ		\
	  | 0x0UL << DLMB_offDBM		\
	  | 0x0UL << DLMB_offDBB		\
	  | 0x1UL << DLMB_offDBPA)

#endif /* __N12_DEF_H__ */

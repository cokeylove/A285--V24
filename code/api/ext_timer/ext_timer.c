#include "ext_timer.h"
#include "chip_chipregs.h"
#include "mem_layout.h"
#include "hal.h"

//****************************************************************************
//  Macro Define
//****************************************************************************
#define EXT_CTL_B           (EC_Register_Base_Address+0x1F10)
#define EXT_PSC_B           (EC_Register_Base_Address+0x1F11)
#define EXT_LLR_B           (EC_Register_Base_Address+0x1F14)
#define EXT_LHR_B           (EC_Register_Base_Address+0x1F15)
#define EXT_LH2R_B          (EC_Register_Base_Address+0x1F16)
#define EXT_LH3R_B          (EC_Register_Base_Address+0x1F17)

#define EXT_CNTOLR_B        (EC_Register_Base_Address+0x1F48)
#define EXT_CNTOHR_B        (EC_Register_Base_Address+0x1F49)
#define EXT_CNTOH2R_B       (EC_Register_Base_Address+0x1F4A)
#define EXT_CNTOH3R_B       (EC_Register_Base_Address+0x1F4B)

//****************************************************************************
#define MILLI_SEC_TO_COUNT(hz, ms) ((hz)*(ms)/1000)
#define MICRO_SEC_TO_COUNT(hz, us) ((hz)*(us)/1000000) // be careful of overflow issue

#define TIMER_STOP(Idx)          ECReg(EXT_CTL_B + ((Idx)*8)) &= ~BIT0
#define TIMER_START(Idx)         ECReg(EXT_CTL_B + ((Idx)*8)) |= BIT0
#define TIMER_RESTART(Idx)  \
    do{                     \
        TIMER_STOP(Idx);    \
        TIMER_START(Idx);   \
    }while(0)
#define TIMER_RELOAD(Idx, Cnt)                                      \
    do{                                                             \
        TIMER_START((Idx));                                         \
        ECReg(EXT_LH3R_B + ((Idx)*8)) = (((Cnt) >> 24) & 0xFF);   \
        ECReg(EXT_LH2R_B + ((Idx)*8)) = (((Cnt) >> 16) & 0xFF);   \
        ECReg(EXT_LHR_B  + ((Idx)*8)) = (((Cnt) >>  8) & 0xFF);   \
        ECReg(EXT_LLR_B  + ((Idx)*8)) = ( (Cnt)        & 0xFF);   \
    }while(0)
// The macro can be used only for Timer #3 ~ #7
#define TIMER_DISABLE(Idx)                          \
    do{                                             \
        /*REG_UC(IGER19) &= ~(BIT(3+(Idx)));*/      \
        /*IER19  &= ~(BIT(3+(Idx)));*/              \
        CLEAR_MASK_EXT(IER19, (BIT(3+(Idx))));    \
    }while(0)
#define TIMER_ENABLE(Idx)                           \
    do{                                             \
        /*REG_UC(IGER19) |= BIT(3+(Idx));*/         \
        /*IER19  |= BIT(3+(Idx));*/                 \
        SET_MASK_EXT(IER19, (BIT(3+(Idx))));      \
    }while(0)
#define TIMER_CLEAR_FLAG(Idx)       ISR19 = BIT(3+(Idx))
#define TIMER_CHECK_FLAG(Idx)       ISR19 & BIT(3+(Idx))
#define TIMER_WAIT(Idx)         (!(ISR19 & BIT(3+(Idx))))

//****************************************************************************
//
//
//
//****************************************************************************
int RTOS_CODE_L timer_init(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32Cnt)
{
  	// Setup Triggered Mode -> Rising-Edge Trig.
  	if(u8Idx != EXT_TIMER_8)
  	{
        IELMR19 |= BIT(3+u8Idx);
        IPOLR19 &= (~(BIT(3+u8Idx)));
  	}
    else
    {
        IELMR10 |= BIT0;
        IPOLR10 &= (~(BIT0));
    }

    // Setup prescaler
    ECReg(EXT_PSC_B + (8*u8Idx)) = u8PSR;

    // Reload counter
    TIMER_RELOAD(u8Idx, u32Cnt);

    // Start counting or not
    if(bInitialState)
        //TIMER_START(u8Idx);
        TIMER_RESTART(u8Idx);
    else
        TIMER_STOP(u8Idx);

    // Enable ISR or not & Clear flag
    if(u8Idx != EXT_TIMER_8)
    {        
        if(bEnableISR)
            TIMER_ENABLE(u8Idx);
        else
            TIMER_DISABLE(u8Idx);

        TIMER_CLEAR_FLAG(u8Idx);
    }
    else
    {
        if(bEnableISR)
            SET_MASK_EXT(IER10, BIT0);
        else
            CLEAR_MASK_EXT(IER10, BIT0);

        ISR10 = BIT0;
    }

    return 0;
}

int RTOS_CODE_L timer_init_ms(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32MilliSec)
{
    UINT32 u32Cnt;

    if(u8PSR == ET_PSR_32K)
        u32Cnt = MILLI_SEC_TO_COUNT(32768, u32MilliSec);
    else if(u8PSR == ET_PSR_1K)
        u32Cnt = MILLI_SEC_TO_COUNT(1024, u32MilliSec);
    else if(u8PSR == ET_PSR_32)
        u32Cnt = MILLI_SEC_TO_COUNT(32, u32MilliSec);
    else if(u8PSR == ET_PSR_8M)
        u32Cnt = u32MilliSec * 8000; // fixed overflow issue
    else 
        return -1;

    // 24-bits only
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            if (u32Cnt >> 24) return -2;
        default:
            break;
    }

    return timer_init(u8Idx, u8PSR, bInitialState, bEnableISR, u32Cnt);
}

int RTOS_CODE_L timer_init_us(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32MicroSec)
{
    UINT32 u32Cnt;

    if(u8PSR == ET_PSR_32K)
        u32Cnt = MICRO_SEC_TO_COUNT(32768, u32MicroSec);
    else if(u8PSR == ET_PSR_1K)
        u32Cnt = MICRO_SEC_TO_COUNT(1024, u32MicroSec);
    else if(u8PSR == ET_PSR_32)
        u32Cnt = MICRO_SEC_TO_COUNT(32, u32MicroSec);
    else if(u8PSR == ET_PSR_8M)
        u32Cnt = u32MicroSec * 8; // fixed overflow issue
    else 
        return -1;

    // 24-bits only
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            if (u32Cnt >> 24) return -2;
        default:
            break;
    }

    return timer_init(u8Idx, u8PSR, bInitialState, bEnableISR, u32Cnt);
}

void RTOS_CODE_L timer_count_restart(UINT8 u8Idx)
{
	TIMER_STOP(u8Idx);
    
    if(u8Idx != EXT_TIMER_8)
    	TIMER_CLEAR_FLAG(u8Idx);
    else
        ISR10 = BIT0;    

	TIMER_START(u8Idx);
}

int RTOS_CODE_L timer_count_wait(UINT8 u8Idx)
{
    if(u8Idx != EXT_TIMER_8)
	    while(TIMER_WAIT(u8Idx));
    else
        while(!(ISR10 & BIT0));

	return 0;
}

int RTOS_CODE_L timer_delay(UINT8 u8Idx)
{
    timer_count_restart(u8Idx);

    return timer_count_wait(u8Idx);
}

UINT32 RTOS_CODE_L timer_get_count(UINT8 u8Idx)
{
    UINT32 u32Cnt;

    u32Cnt = ECReg(EXT_CNTOLR_B + (u8Idx*4)); // for latching current counter

    u32Cnt = ECReg(EXT_CNTOLR_B + (u8Idx*4)) 
        | (((UINT32)ECReg(EXT_CNTOHR_B + (u8Idx*4))) << 8)
        | (((UINT32)ECReg(EXT_CNTOH2R_B + (u8Idx*4))) << 16)
        | (((UINT32)ECReg(EXT_CNTOH3R_B + (u8Idx*4))) << 24);

    // 24-bits only
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            u32Cnt &= 0x00FFFFFF;
        default:
            break;
    }

    return u32Cnt;
}

int RTOS_CODE_L timer_combine_init(UINT8 u8Idx, UINT8 u8PSR, BOOL bInitialState, BOOL bEnableISR, UINT32 u32Timeout, UINT32 u32TimeUnit)
{
    UINT64 u64Cnt = 0;

    if(u8PSR == ET_PSR_32K)
        u64Cnt = ((UINT64) u32Timeout * 32768) / u32TimeUnit;
    else if(u8PSR == ET_PSR_1K)
        u64Cnt = ((UINT64) u32Timeout * 1024) / u32TimeUnit;
    else if(u8PSR == ET_PSR_32)
        u64Cnt = ((UINT64) u32Timeout * 32) / u32TimeUnit;
    else if(u8PSR == ET_PSR_8M)
        u64Cnt = ((UINT64) u32Timeout * 8000000) / u32TimeUnit;
    else 
        return -1;

    // 24-bits only
    if (u64Cnt >> 24)
        return -2;

    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            break;
        default:
            return -3;
    }

    // Combine Timer(u8Idx) & Timer(u8Idx+1) to 56-bits timer
    ECReg(EXT_CTL_B + u8Idx * 8) |= BIT3;

  	// Setup Triggered Mode -> Rising-Edge Trig.
    IELMR19 |= BIT(3 + u8Idx);
    IPOLR19 &= ~BIT(3 + u8Idx);

    // Setup prescaler
    ECReg(EXT_PSC_B + 8 * u8Idx) = u8PSR;

    // Reload counter
    TIMER_RELOAD(u8Idx, u64Cnt);

    // Start counting or not
    if(bInitialState)
    {
        TIMER_START(u8Idx);
    }
    else
    {
        TIMER_STOP(u8Idx);
    }

    // Enable ISR or not & Clear flag
    if(bEnableISR)
    {
        TIMER_ENABLE(u8Idx);
    }
    else
    {
        TIMER_DISABLE(u8Idx);
    }

    TIMER_CLEAR_FLAG(u8Idx);

    return 0;
}

int RTOS_CODE_L timer_combine_deinit(UINT8 u8Idx)
{
    // Combine Mode only support EXT_TIMER_3, EXT_TIMER_5, EXT_TIMER_7
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            break;
        default:
            return -1;
    }

    // Stop
    TIMER_STOP(u8Idx);

    // Disable ISR & Clear flag
    TIMER_DISABLE(u8Idx);

    TIMER_CLEAR_FLAG(u8Idx);

    // Disable 56-bits timer
    ECReg (EXT_CTL_B + u8Idx * 8) &= ~BIT3;

    return 0;
}

void RTOS_CODE_L timer_combine_count_restart(UINT8 u8Idx)
{
    TIMER_STOP(u8Idx);
    
	TIMER_CLEAR_FLAG(u8Idx);

    ECReg(EXT_CTL_B + ((u8Idx + 1) * 8)) |= BIT1;
    TIMER_START(u8Idx);
}

int RTOS_CODE_L timer_combine_count_wait(UINT8 u8Idx)
{
    while(TIMER_WAIT(u8Idx));

	return 0;
}

int RTOS_CODE_L timer_combine_delay(UINT8 u8Idx)
{
    // Combine Mode only support EXT_TIMER_3, EXT_TIMER_5, EXT_TIMER_7
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            break;
        default:
            return -1;
    }

    timer_combine_count_restart(u8Idx);

    return timer_combine_count_wait(u8Idx);
}

UINT64 RTOS_CODE_L timer_combine_get_count(UINT8 u8Idx)
{
    UINT64 u64Cnt;

    // Combine Mode only support EXT_TIMER_3, EXT_TIMER_5, EXT_TIMER_7
    switch (u8Idx) {
        case EXT_TIMER_3:
        case EXT_TIMER_5:
        case EXT_TIMER_7:
            break;
        default:
            return -1;
    }

    u64Cnt = ECReg(EXT_CNTOLR_B + (u8Idx*4)); // for latching current counter

    u64Cnt = ECReg(EXT_CNTOLR_B + (u8Idx*4)) 
        | (((UINT64)ECReg(EXT_CNTOHR_B + (u8Idx*4))) << 8)
        | (((UINT64)ECReg(EXT_CNTOH2R_B + (u8Idx*4))) << 16)
        | (((UINT64)ECReg(EXT_CNTOLR_B + ((u8Idx+1)*4))) << 32)
        | (((UINT64)ECReg(EXT_CNTOHR_B + ((u8Idx+1)*4))) << 40)
        | (((UINT64)ECReg(EXT_CNTOH2R_B + ((u8Idx+1)*4))) << 48)
        | (((UINT64)ECReg(EXT_CNTOH3R_B + ((u8Idx+1)*4))) << 56);

    return u64Cnt;
}
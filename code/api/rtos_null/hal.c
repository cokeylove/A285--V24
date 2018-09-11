
#include "Mem_layout.h"
#include "hal.h"
//#include "hid_hal.h"
#include "sensor_fusion.h"
#include "core_memory.h"
#include "i2c.h"
#include "I2c_drv.h"
#include "hid_common.h"
#include "chip_type.h"
#include "chip_chipregs.h"
#include "..\include.h"
#include "config.h"

#define SELF_TEST_BUF_SIZE      (0x200)
static BYTE gu8Buffer[SELF_TEST_BUF_SIZE];

//****************************************************************************
inline void RTOS_CODE_H hal_hw_module_int(void)
{

}

//****************************************************************************
inline int RTOS_CODE_H hal_wait_for_mutex(hal_mutex_t *mutex, unsigned int timeout)
{
    return HAL_SUCCESS;
}

//****************************************************************************
inline int RTOS_CODE_H hal_release_mutex(hal_mutex_t *mutex)
{
    return HAL_SUCCESS;
}

//****************************************************************************
inline int RTOS_CODE_L hal_create_mutex(hal_mutex_t *mutex, const void *param)
{
    return HAL_SUCCESS;
}

//****************************************************************************
inline int RTOS_CODE_H hal_suspend_all_thread(void)
{
    return HAL_SUCCESS;
}

//****************************************************************************
inline int RTOS_CODE_H hal_resume_all_thread(void)
{
    return HAL_SUCCESS;
}

//****************************************************************************
inline int RTOS_CODE_H hal_sleep(int ms)
{
    DWORD l_1ms_counter;
    
    while(ms)
    {
        for(l_1ms_counter=0x00; l_1ms_counter<70; l_1ms_counter++)
        {
            //
            // Delay 15.26 us
            //
            WNCKR = 0x00;
        }
        ms--;
    }
    return HAL_SUCCESS;
}

//****************************************************************************
//inline void RTOS_CODE_H hal_hid_irq(void)
//{
//    hid_isr();
//}

/* **************************************************************************** 
 * FUNCTION NAME: hal_SMbusD_irq
 * DESCRIPTION:
 * 		SMBus-D interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_SMbusD_irq(void)
{
    if(SENSOR_I2C_CH == I2C_D)
    {
        sensor_isr();
    }
    else if((DEF_I2C_HID) && (HID_I2C_CH == I2C_D))
    {
        hid_isr();
    }
    else
    {

    }
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_SMbusE_irq
 * DESCRIPTION:
 * 		SMBus-E interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_SMbusE_irq(void)
{
    if(SENSOR_I2C_CH == I2C_E)
    {
        sensor_isr();
    }
    else if((DEF_I2C_HID) && (HID_I2C_CH == I2C_E))
    {
        hid_isr();
    }
    else
    {

    }
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_SMbusF_irq
 * DESCRIPTION:
 * 		SMBus-F interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_SMbusF_irq(void)
{
    if(SENSOR_I2C_CH == I2C_F)
    {
        sensor_isr();
    }
    else if((DEF_I2C_HID) && (HID_I2C_CH == I2C_F))
    {
        hid_isr();
    }
    else
    {

    }
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_SmbD_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using SMBus-D interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_SmbD_irq(void)
{
    WUESR6 = BIT2;              // clear interrupt
    CLEAR_MASK(IER6, BIT2);     // INT50 GPCRH1 WUI interrupt mask enable
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_SmbE_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using SMBus-E interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_SmbE_irq(void)
{
    if((WUESR4 & BIT6))
    {
        WUESR4 = BIT6;              // clear interrupt
        CLEAR_MASK(IER0, BIT5);     // INT0 GPCRE7 WUI interrupt mask enable
    }
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_SmbF_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using SMBus-F interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_SmbF_irq(void)
{
    WUESR8 = BIT2;              // clear interrupt      
    CLEAR_MASK(IER11, BIT2);    // INT90 GPCRA5 WUI interrupt mask enable 
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_SmbF_swap2ch0_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using SMBus-F interrupt service routine. (swap to ch0)
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_SmbF_swap2ch0_irq(void)
{
    if((HID_I2C_CH == I2C_F) && (HID_I2C_SWAP == 1))
    {  
        WUESR9 = BIT6;              // clear interrupt      
        CLEAR_MASK(IER12, BIT3);    // INT99 GPCRB4 WUI interrupt mask enable 
    }
}


/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_USBDminus_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using USB D- interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_USBDminus_irq(void)
{
    #ifdef __ENABLE_HID_USB_API__ // {
    WUESR9 = BIT1;              // clear interrupt
    CLEAR_MASK(IER10, BIT6);    // INT86 GPDRH5 WUI interrupt mask disable
    
    if(DEF_USB_HID)   
    {
        CLEAR_MASK(CGCTRL3R, BIT5);   // Enable USB CLK
        if(usb_slave_is_shutdown())
        {
            if(!usb_slave_resume(USB_RESUME_RESUME))
                SET_MASK(IER10, BIT6);
        }
    }    
    #endif // } __ENABLE_HID_USB_API__
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_USBDplus_irq
 * DESCRIPTION:
 * 		Wake-up interrupt using USB D+ interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_USBDplus_irq(void)
{     
    #ifdef __ENABLE_HID_USB_API__ // {
    WUESR9 = BIT2;              // clear interrupt 
    CLEAR_MASK(IER10, BIT7);    // INT87 GPDRH5 WUI interrupt mask disable
    
    if(DEF_USB_HID )
    {
        CLEAR_MASK(CGCTRL3R, BIT5);   // Enable USB CLK
        #if 0
        if(usb_slave_is_shutdown())
        {    
            if(!usb_slave_resume(USB_RESUME_RESET))
                SET_MASK(IER10, BIT7);
        }
        #endif
    }
    #endif // } __ENABLE_HID_USB_API__
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_WUI_enable_mask
 * DESCRIPTION:
 * 		Enable wake-up interrupt mask.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_WUI_enable_mask(void)
{
    #ifdef __ENABLE_HID_I2C_API__ // {
#if 0 // { TODO
    if(DEF_I2C_HID)
    {
        if(HID_I2C_CH == I2C_D)
        {
            SET_MASK(IER6, BIT2);           // INT50 GPCRH2 WUI interrupt mask enable        
        }
        if(HID_I2C_CH == I2C_E)
        {
            SET_MASK(IER0, BIT5);           // INT50 GPCRA5 WUI interrupt mask enable
        }
        if(HID_I2C_CH == I2C_F)
        {
            if(HID_I2C_SWAP)
            {
                SET_MASK(IER12, BIT3);      // INT99 GPCRB4 WUI interrupt mask enable    
            }
            else
            {
                SET_MASK(IER11, BIT2);      // INT90 GPCRA5 WUI interrupt mask enable
            }
        }
    }
#endif // } 0
    #endif // } __ENABLE_HID_I2C_API__
    
    #ifdef __ENABLE_HID_USB_API__ // {
    if(DEF_USB_HID)
    {
        SET_MASK(IER10, BIT6);              // INT86 GPDRH5 WUI interrupt mask enable
        SET_MASK(IER10, BIT7);              // INT87 GPDRH6 WUI interrupt mask enable
    }
    #endif // } __ENABLE_HID_USB_API__

}

/* **************************************************************************** 
 * FUNCTION NAME: hal_usb_irq
 * DESCRIPTION:
 * 		USB interrupt service routine.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_usb_irq(void)
{
    #ifdef __ENABLE_HID_USB_API__ // {
    if(DEF_USB_HID)
    {
        hid_isr();
    }
    #endif // } __ENABLE_HID_USB_API__
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_get_sys_ticks
 * DESCRIPTION:
 * 		Get rtos tick count.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS:
 *      [OUT] tick count.
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline unsigned long RTOS_CODE_H hal_get_sys_ticks(void)
{
    //return  xTaskGetTickCount();
    return (TIMER_1MS_TICKS);
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_get_free_heep_size
 * DESCRIPTION:
 * 		Get the free heap size
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline unsigned long hal_get_free_heep_size(void)
{
    return  SELF_TEST_BUF_SIZE;
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_malloc
 * DESCRIPTION:
 * 		Dynamic allocate memory
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 *      Just only call once.
 *
 ******************************************************************************/
inline void* hal_malloc(unsigned long size)
{
    return  (void*)gu8Buffer;
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_free
 * DESCRIPTION:
 * 		free memory
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void  hal_free(void *ptr)
{

}

/* **************************************************************************** 
 * FUNCTION NAME: hal_yield_thread
 * DESCRIPTION:
 * 		Call the scheduler.
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 
 ******************************************************************************/
inline void RTOS_CODE_H hal_yield_thread(void)
{	

}

/* **************************************************************************** 
 * FUNCTION NAME: hal_sensor_hub_off
 * DESCRIPTION:
 * 		sensor hub power off
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: In HID/I2C case, make sure calling this function before 
 *       HID_I2C_INFO & I2C_HID_SLAVE_ADDRESS initialization
 ******************************************************************************/
inline void RTOS_CODE_L hal_sensor_hub_off(void)
{
    #if DEF_HID_OVER_I2C // {
    i2c_hid_power_off();
    #endif // } DEF_HID_OVER_I2C
    
    i2c_drv_sensor_power_off_handler();
}

/* **************************************************************************** 
 * FUNCTION NAME: hal_sensor_hub_on
 * DESCRIPTION:
 * 		sensor hub power on
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: In HID/I2C case, make sure calling this function before 
 *       HID_I2C_INFO & I2C_HID_SLAVE_ADDRESS initialization
 ******************************************************************************/
inline void RTOS_CODE_L hal_sensor_hub_on(void)
{
    i2c_drv_sensor_power_on_handler();

    #if DEF_HID_OVER_I2C // {
    i2c_hid_power_on();
    #endif // } DEF_HID_OVER_I2C
}

//*****************************************************************************
//
// handle fusion_main() and hid_main()
//
//  parameter :
//      none
//
//  return :
//      none
//
//  note :
//      1 ms time based
//
//*****************************************************************************
void RTOS_CODE_H EC_Handle_Task(void)
{
    #ifdef  EC_FEATURE_ONLY_SUPPORT_FUSION  //  {
    
    BYTE l_enable_sensor_interrupt;
    BYTE l_skip_handle_usb_interface;

    l_enable_sensor_interrupt = 0;
    l_skip_handle_usb_interface = 0;

    //
    // handle sensor devices data
    //
    #ifdef DEF_SENSOR_ONE_SHOT_MODE
    if(SENSOR_ISR_SERVICE_FLAG_OS==0xFE)
    {
        SENSOR_ISR_SERVICE_FLAG_OS = 0;
        if(gu8HIDStatus == HID_STATUS_NORMAL)
        {
            pure_ec_sensor_one_shot_isr();
        }
    }
    #else
    if(SENSOR_ISR_SERVICE_FLAG)
    {
        SENSOR_ISR_SERVICE_FLAG = 0;
        
        if(DEF_I2C_HID)
        {
            #ifdef __ENABLE_HID_I2C_API__ // {
            pure_ec_sensor_fusion_calc(i2c_hid_data_set, NULL);
            #endif // } __ENABLE_HID_I2C_API__
        }
        else if(DEF_USB_HID)
        {
            #ifdef __ENABLE_HID_USB_API__ // {
    	    pure_ec_sensor_fusion_calc(usb_slave_hid_data_set, NULL);
            #endif // } __ENABLE_HID_USB_API__
        }
        
        l_enable_sensor_interrupt = 1;
    }
    #endif

    //
    // 1ms time based
    //
    if(EXTEND_EVENT_ISR_SERVICE_FLAG)
    {
        EXTEND_EVENT_ISR_SERVICE_FLAG = 0;

        vApplicationTickHook();
        
        if((EC_HANDLE_TASK_INDEX%2)==0x00)
        {
            EC_HANDLE_FUSION_TASK_STATUS = 0x01;
            fusion_main();
            EC_HANDLE_FUSION_TASK_STATUS = 0x00;
        }
        else
        {
            EC_HANDLE_HID_TASK_STATUS = 0x01;
            if(l_skip_handle_usb_interface==0)
            {
                hid_main();
            }
            EC_HANDLE_HID_TASK_STATUS = 0x00;
        }

        #ifdef DEF_SENSOR_ONE_SHOT_MODE
            if(gu8HIDStatus == HID_STATUS_NORMAL)
            {
                pure_ec_sensor_one_shot_main();
            }
        #endif  // DEF_SENSOR_ONE_SHOT_MODE

        EC_HANDLE_TASK_INDEX++;
    }

    if(l_enable_sensor_interrupt)
    {
        //
        // To enable I2C interrupt of sensor channel.
        //
        i2c_interrupt_mask(SENSOR_I2C_CH, 1);
    }

    #endif  // } EC_FEATURE_ONLY_SUPPORT_FUSION
}

//*****************************************************************************
//
//
//
//  parameter :
//      none
//
//  return :
//      none
//
//  note :
//      1 ms time based
//
//*****************************************************************************
void RTOS_CODE_H vApplicationTickHook(void)
{
    #ifdef __ENABLE_HID_USB_API__ // {
    if(DEF_USB_HID)
    {
        IER2 &= ~BIT7;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        
        usb_slave_check_suspend();

        IER2 |= BIT7;
    }
    #endif // } __ENABLE_HID_USB_API__
    
	hid_time_sensor_interval();

    if( hid_time_report_interval() )
    {
        if(DEF_I2C_HID)
        {
            //i2c_hid_check_and_assert_interrupt();
        }
        else if(DEF_USB_HID)
        {
            #ifdef __ENABLE_HID_USB_API__ // {
            IER2 &= ~BIT7;
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            
            if(!usb_slave_is_shutdown())  
            {
                //hal_resume_task_in_isr(tthi_HID); 
            }
            else          
            {
                usb_slave_resume(USB_RESUME_REMOTEWAKEUP); // for wakeup host    
            }

            IER2 |= BIT7;
            #endif // } __ENABLE_HID_USB_API__
        }
    }

    //
    // HID over I2C only
    //
    #ifdef __ENABLE_HID_I2C_API__ // {
    if(DEF_I2C_HID)
    {
        if(gu8HIDStatus==HID_STATUS_NORMAL)
        {
            if(hid_check_bus_busy()==0)
            {
                if(HID_OVER_I2C_ISR_SERVICE_FLAG==0)
                {
                    i2c_hid_check_and_assert_interrupt();
                }
            }
        }
        else
        {
            HID_INT_Event_Reset();
        }
    }
    #endif // } __ENABLE_HID_I2C_API__
}

//****************************************************************************
inline void RTOS_CODE_L hal_init_os(void)
{	
    #if DEF_DIRECT_MAP_CODE // {
    UINT32  u32Addr  = GET_DM_S_LMA;
    UINT8   u8Cnt    = (0 != (GET_DM_SIZE % SRAM_SEG_SIZE)) ? ((GET_DM_SIZE >> SRAM_SEG_ALIGN) + 1): (GET_DM_SIZE >> SRAM_SEG_ALIGN);
    UINT8   u8Index  = 0;

    //  Check Direct map index size
    u8Cnt = (DM_MAX_NUM < u8Cnt) ? DM_MAX_NUM : u8Cnt;
    
    //  Load Direct Map code to SRAM
    for(u8Index = 0; u8Index < u8Cnt; u8Index++)
    {
        eflash_to_direct_map(u32Addr + u8Index*SRAM_SEG_SIZE, DM_S_IDX + u8Index);
        dm_set_segment_valid(DM_S_IDX + u8Index);
    }
    #endif  // } DEF_DIRECT_MAP_CODE

    i2c_drv_module_init();   
}

//****************************************************************************
#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION // {
void pure_ec_sensor_device_power_off(void)
{
    pure_ec_init_clear_fusion_variables();
    //gu8HIDStatus = HID_STATUS_RESET;
    #ifdef __ENABLE_HID_I2C_API__ // {
    HID_INT_Event_Reset();
    #endif // } __ENABLE_HID_I2C_API__
}
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION

//*****************************************************************************
//
// usb isr service flag for EC feature only use.
//
//  parameter :
//      none
//
//  return :
//      none
//
//
//*****************************************************************************
void usb_isr_service_flag(void)
{
    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION

    //F_Service_Extend_Task = 1;

    //
    // service flag fro "usb_slave_main();" function
    //
    USB_ISR_SERVICE_FLAG = 1;

    #endif
}

//*****************************************************************************
//
// handle usb interrupt service routine
//
//  parameter :
//      none
//
//  return :
//      none
//
//*****************************************************************************
void service_usb_isr(void)
{
    #ifdef __ENABLE_HID_USB_API__ // {
    if(USB_ISR_SERVICE_FLAG)
    {
        USB_ISR_SERVICE_FLAG = 0;
        usb_slave_main();
    }
    #endif // } __ENABLE_HID_USB_API__
}

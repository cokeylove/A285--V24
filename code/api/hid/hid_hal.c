#include "chip_type.h"
#include "chip_chipregs.h"
#include "mem_layout.h"
#include "rom.h"

#include "hal.h"
#include "hid_hal.h"
#include "hid_common.h"
#include "i2c_drv.h"
#include "core_gpio.h"
#include "core_memory.h"

/* *******************************************************************************

    ---- Ch3 ----
    H1 CLK
    H2 DAT
    
    ---- Ch4 ----
    E0 CLK
    E7 DAT
    
    ---- Ch5 ----
    A4 CLK
    A5 DAT

* *******************************************************************************/
 
//********************************************************************************
//  Macro Define
//********************************************************************************
#include "oem_project.h"

#define I2C_DEBUG_PORT8(_X_)         (gpSHDInfo->pI2CDbgMsg[7] = (_X_))

#if !defined(HID_INT_GPDR) // {
#define HID_INT_GPDR                GPDRH
#warning    "No define >> HID_INT_GPDR <<"
#endif  // } 

#if !defined(HID_INT_GPCR) // {
#define HID_INT_GPCR                GPCRH5
#warning    "No define >> HID_INT_GPCR <<"
#endif  // }

#if !defined(HID_INT_GPDR_BIT)
#define HID_INT_GPDR_BIT            BIT5 
#warning    "No define >> HID_INT_GPDR_BIT <<"
#endif  // }

// extern value
HID_DATA volatile UINT16 gusSensorOneShotCmd = 0;
HID_DATA volatile UINT8  gucSensorStatusBusy = 0;

#ifdef SENSOR_ALS_CURVE // {
HID_RODATA const UINT16 aru16ALR_curve[SENSOR_ALS_CURVE_LENGTH] = SENSOR_ALS_CURVE;
#endif // } SENSOR_ALS_CURVE

#ifdef HID_CUSTOM_MODEL_STRING // {
HID_RODATA const UINT16 CustomModel[HID_CUSTOM_MODEL_LENGTH] = HID_CUSTOM_MODEL_STRING;
#endif // } HID_CUSTOM_MODEL_STRING

#if  (DEF_DUAL_OS && DEF_OS_HOT_SWITCH) // {
HID_BSS UINT8 gu8LastOSFlag;
#endif // }  (DEF_DUAL_OS && DEF_OS_HOT_SWITCH)

extern HID_DATA volatile UINT8  gucHIDHandleReportID;

#define IS_SENSOR_TIME(s) ((guiHIDTimer % gu16SensorPeriod[(s)]) == 0)
#define IS_FUSION_TIME()  ((guiHIDTimer % gu16SensorPeriod[SENSOR_FUSION_IDX]) == 0)

// GPIO control
#ifdef DEF_HID_OVER_I2C_INT_LOW_LEVEL_TRIGGER
#define _HID_INT_H()			    CLEAR_MASK(HID_INT_GPDR, HID_INT_GPDR_BIT)
#define _HID_INT_L()			    SET_MASK(HID_INT_GPDR, HID_INT_GPDR_BIT)
#else
#define _HID_INT_H()			    SET_MASK(HID_INT_GPDR, HID_INT_GPDR_BIT)
#define _HID_INT_L()			    CLEAR_MASK(HID_INT_GPDR, HID_INT_GPDR_BIT)
#endif
#define	_AssertHidIntH()			_HID_INT_H()
#define	_AssertHidIntL()			_HID_INT_L()


//********************************************************************************
//  Function Declaration
//********************************************************************************
void HID_CODE_L hid_gpio_cfg(void)
{
    #ifdef __ENABLE_HID_I2C_API__ // {
    if(DEF_I2C_HID)
    {
        //  Init i2c GPIO function
        //i2c_drv_init(HID_I2C_CH);
    
        //  Init INT to host
        HID_INT_GPCR = OUTPUT;
        //  Default low
        _AssertHidIntL();
    }
    #endif // } __ENABLE_HID_I2C_API__
}

void HID_CODE_L hid_gpio_reset(void)
{
    #ifdef __ENABLE_HID_I2C_API__ // {
    // +SHAWN20150416: set INT low
    //  Set INT INPUT
//	    HID_INT_GPCR = INPUT;

    _AssertHidIntL();
    // -SHAWN20150416
    #endif // } __ENABLE_HID_I2C_API__
}

//********************************************************************************
void HID_CODE hid_assert_interrupt(void)
{  
    _AssertHidIntH();
}
//********************************************************************************
void HID_CODE hid_deassert_interrupt(void)
{
    _AssertHidIntL();
}
//********************************************************************************
void HID_CODE hid_isr(void)
{
    if(DEF_I2C_HID)
    {
        #ifdef __ENABLE_HID_I2C_API__ // {
        i2c_hid_isr();
        #endif // } __ENABLE_HID_I2C_API__
    }
    else if(DEF_USB_HID)
    {
        #ifdef __ENABLE_HID_USB_API__ // {
        usb_hid_isr();
        #endif // } __ENABLE_HID_USB_API__
    }
}

//********************************************************************************
UINT8 HID_CODE hid_check_bus_busy(void)
{          
    if(DEF_I2C_HID)
    {
        #ifdef __ENABLE_HID_I2C_API__ // {
        //return (UINT8)( (HID_INT_GPCR & HID_INT_GPDR_BIT) |(I2C_GET_STAT(HID_I2C_CH) & I2C_STAT_BUS_BUSY));
        #ifdef DEF_HID_OVER_I2C_INT_LOW_LEVEL_TRIGGER
        return (UINT8)( (IS_MASK_CLEAR(HID_INT_GPDR, HID_INT_GPDR_BIT)) |(I2C_GET_STAT(HID_I2C_CH) & I2C_STAT_BUS_BUSY));
        #else
        return (UINT8)( (IS_MASK_SET(HID_INT_GPDR, HID_INT_GPDR_BIT)) |(I2C_GET_STAT(HID_I2C_CH) & I2C_STAT_BUS_BUSY));
        #endif
        #endif // } __ENABLE_HID_I2C_API__
    }
    else if(DEF_USB_HID)
    {
        #ifdef __ENABLE_HID_USB_API__ // {
        return(0);
        #endif // } __ENABLE_HID_USB_API__
    }
    else
    {
        
    }

    return(1);
}

//********************************************************************************
void HID_CODE_L hid_extend_data_get(UINT8 pBuffer[13])
{
    #ifdef __ENABLE_HCC_EXTEND_CMD__ // {
    do_extend_data_get(pBuffer);
    #endif // } __ENABLE_HCC_EXTEND_CMD__
}

void HID_CODE_L hid_extend_data_set(UINT8 pBuffer[13])
{
    #ifdef __ENABLE_HCC_EXTEND_CMD__ // {
    do_extend_data_set(pBuffer);
    #endif // } __ENABLE_HCC_EXTEND_CMD__
}

//********************************************************************************
#if (DEF_DUAL_OS && DEF_OS_HOT_SWITCH) // {
void HID_CODE_L hid_init_os_hot_switch(void)
{
    OS_HOT_SWITCH_PIN_GPCR = INPUT;

    gu8LastOSFlag = DEF_ANDROID ? OS_HOT_SWITCH_MODE_ANDROID : (OS_HOT_SWITCH_MODE_ANDROID^OS_HOT_SWITCH_PIN_GPDR_BIT);
}

void HID_CODE hid_check_os_hot_switch(void)
{
    UINT8 u8OSFlag;

    u8OSFlag = OS_HOT_SWITCH_PIN_GPDR & OS_HOT_SWITCH_PIN_GPDR_BIT;

    if(u8OSFlag != gu8LastOSFlag)
    {
        if(u8OSFlag == OS_HOT_SWITCH_MODE_ANDROID)
        {
            printk("Android Mode.\r\n");
            hid_save_config(0);

            sensor_wait_done_and_stop();
            gstFConfig.Enable_Android = 1;
            sensor_restart();
            
            hid_restore_config(1);
        }
        else
        {
            printk("Windows Mode.\r\n");
            hid_save_config(1);
            
            sensor_wait_done_and_stop();
            gstFConfig.Enable_Android = 0;
            sensor_restart();
            
            hid_restore_config(0);
        }

        gu8LastOSFlag = u8OSFlag;
    }
}
#endif // } (DEF_DUAL_OS && DEF_OS_HOT_SWITCH)

//********************************************************************************
void HID_CODE hid_upgrade_flow(void)
{
#ifdef  eflash_upgrade_flow // {
    eflash_upgrade_flow();
#else
//	    MCCR1 |= (0x2A << 2);   /* set flag(main code to bbk), only for Bx/Cx IC */
    __asm__ volatile ("j __bbk_begin");
#endif  // } eflash_upgrade_flow
}

//********************************************************************************
void HID_CODE hid_main(void)
{
    //
    // rtos only
    //
    #ifndef  EC_FEATURE_ONLY
    #ifndef __ENABLE_EC_MAIN__  // { 
    //  Enable HW module Int    
    hal_hw_module_int();

    //  Enable INT, INTC
    hal_global_int_ctl(HAL_ENABLE_INTERRUPTS);
    #endif  // } __ENABLE_EC_MAIN__
    #endif

    //
    // HID interface selection
    //
    DEF_I2C_HID = DEF_HID_OVER_I2C;
    DEF_USB_HID = DEF_HID_OVER_USB;
    HID_I2C_INFO = ((DEF_HID_I2C_CH) |  (DEF_SWAP_TO_CH0 << 4));

    #ifdef DEF_I2C_HID_SLAVE_ADDRESS
    I2C_HID_SLAVE_ADDRESS = (DEF_I2C_HID_SLAVE_ADDRESS<<1);;
    #else
    I2C_HID_SLAVE_ADDRESS = (0x5C << 1);
    #endif

    #ifndef EC_FEATURE_ONLY_SUPPORT_FUSION
    while(1)
    #endif
    {
        switch(HID_INTERFACE_STEP)
        {
            case HID_INTERFACE_STEP_INIT:
                //  Init HID
                hid_init();
				#if EC_USB_charger
                CGCTRL3R &= 0xDF; 
				USB_SC_ADDRESS=0xFF;
                #endif
                break;

            case HID_INTERFACE_STEP_SERVICE:
                //#ifndef EC_FEATURE_ONLY
                hid_service();
                //#endif  // ndef EC_FEATURE_ONLY
                break;

            default:
                HID_INTERFACE_STEP = HID_INTERFACE_STEP_INIT;
                break;
        }
        HID_DEBUG_SET_FLAG();
    }
}
    

//****************************************************************************************
        #ifndef EC_FEATURE_ONLY
void HID_CODE hid_time_check_report_interval(void)
        {
	if( hid_time_report_interval() )
        {
	    if( DEF_I2C_HID )
            {                 
	        #ifdef __ENABLE_HID_I2C_API__ // {
	        i2c_hid_check_and_assert_interrupt();
	        #endif // } __ENABLE_HID_I2C_API__
            } 
	    else if( DEF_USB_HID )
                {   
	        #ifdef __ENABLE_HID_USB_API__ // {
	        if( !usb_slave_is_shutdown() )
                    {
	            hal_resume_task_in_isr(tthi_HID);
                    }    
	        else
                        {
	            usb_slave_resume(USB_RESUME_REMOTEWAKEUP); // for wakeup host
                        }
	        #endif // } __ENABLE_HID_USB_API__
                }
            }
        }
#endif

//****************************************************************************************
int HID_CODE hid_time_report_interval(void)
        {
    UINT8 i				= 0;   
    UINT8 iFlag			= 0;
    UINT8 ucReportID	= 0;
        
	//Check report interval for HID
	if(gu8HIDStatus == HID_STATUS_NORMAL)
        {
		ucReportID	= hid_check_next_cmd();
		guiHIDTimer = hal_get_sys_ticks();

		// Check I2C-HID report interval                                              
		for( i = 1; i < (TOTAL_HID_DEVICE + 1); i++ )
            {
			//Check interrupt status
			if(gu8HIDINTC[i] == 1)		iFlag = 1;

			//Check next command
			#ifndef EC_FEATURE_ONLY
			if(i == ucReportID){
				if(gu8HIDINTC[i] == 0){
					giHIDLastTimer[i]	= guiHIDTimer;
					gu8HIDINTC[i]		= 1;
				}

                iFlag = 1;    
            }  
			#endif// } EC_FEATURE_ONLY

			//Check time
			if(gu32HIDInterval[i] != 0)
			{							
				if(	((guiHIDTimer - giHIDLastTimer[i]) >= gu32HIDInterval[i])								||
					(gstHIDFeature[HID_SENSOR_IDX(i)].Event != HID_USAGE_SENSOR_EVENT_DATA_UPDATED_ENUM)				
																												)
            {
					giHIDLastTimer[i]	= guiHIDTimer;
				gu8HIDINTC[i] = 1;
				iFlag = 1;

                //
					// Non-RTOS
					//
					#ifdef EC_FEATURE_ONLY    
                // HID over I2C only
                //
                #ifdef __ENABLE_HID_I2C_API__ // {
                if(DEF_I2C_HID)
                {
                    HID_INT_Event(i);
                }
                #endif // } __ENABLE_HID_I2C_API__
					#endif // } EC_FEATURE_ONLY
			}
		}
		}
	}
        
	//Check status
	if( (gucHIDHandleReportID == 0) && (gu8HIDBufferUpdating == 0) ){
		return  iFlag;		
	}
	else{
		return 0;
	}
}

        

//****************************************************************************************
#ifndef DEF_SENSOR_ONE_SHOT_MODE // {  DEF_SENSOR_ONE_SHOT_MODE
extern UINT8 gucHIDDoWCmd;
#endif  // } DEF_SENSOR_ONE_SHOT_MODE
void HID_CODE hid_time_sensor_interval(void)
{
    #ifdef DEF_SENSOR_ONE_SHOT_MODE // {  DEF_SENSOR_ONE_SHOT_MODE
    UINT16	usCmdFlag			= 0;
    #endif // } DEF_SENSOR_ONE_SHOT_MODE

	guiHIDTimer = hal_get_sys_ticks();

	//Check sensor
    if ((gu8HIDStatus == HID_STATUS_NORMAL) )
    {
        #ifdef DEF_SENSOR_ONE_SHOT_MODE  // {       
        #if _ENABLE_ACC_ // {  
        if((gstHIDPowerState.Acc == 1) || (hid_check_power_mode() == 1))
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_ACC))
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_ACC));
            }     
        }
        #endif // } _ENABLE_ACC_
     
        #if _ENABLE_MAG_ // {
        if((gstHIDPowerState.Mag == 1) || (hid_check_power_mode() == 1))
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_MAG))
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_MAG));
            } 
        }       
        #endif // } _ENABLE_MAG_
         
        #if _ENABLE_GYO_ // {
        if((gstHIDPowerState.Gyo == 1) ||(hid_check_power_mode() == 1))
        {
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_GYO))
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_GYO));
            }
        }    
        #endif // } _ENABLE_GYO_

        #if _ENABLE_ALS_ // {
        if(gstHIDPowerState.Als == 1)
        {
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_ALS))
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_ALS));
            }    
        }
        #endif // } _ENABLE_ALS_
        
        #if _ENABLE_ACC2_ // {
        if(((gstHIDPowerState.Acc == 1) || (hid_check_power_mode() == 1))
            #ifndef DEF_SENSOR_ACC2_ALWAYS_ON // {
            && (gstHIDPowerState.Custom == 1)
            #endif // } DEF_SENSOR_ACC2_ALWAYS_ON
           )
        {
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_ACC2))
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_ACC2));
            }
        }
        #endif // } _ENABLE_ACC2_
        
        #if _ENABLE_PRS_ // {
        if(gstHIDPowerState.Prs == 1)
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_PRS))
            {   
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_PRS));
            }
        }    
        #endif // } _ENABLE_PRS_

        #if _ENABLE_HYG_ // {
        if(gstHIDPowerState.Hyg == 1)
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_HYG))
            {   
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_HYG));
            }
        }    
        #endif // } _ENABLE_HYG_ 

        #if _ENABLE_TEMP_ // {
        if(gstHIDPowerState.Temp == 1)
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_TEMP))
            {   
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_TEMP));
            }
        }    
        #endif // } _ENABLE_TEMP_

        #if _ENABLE_PROX_ // {
        if(gstHIDPowerState.Prox == 1)
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_PROX))
            {   
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_PROX));
            }
        }    
        #endif // } _ENABLE_PROX_

        #if _ENABLE_UV_ // {
        if(gstHIDPowerState.UV == 1)
        {           
            if(IS_SENSOR_TIME(SENSOR_I2C_IDX_UV))
            {   
                SET_MASK(usCmdFlag, BIT(SENSOR_I2C_IDX_UV));
            }
        }    
        #endif // } _ENABLE_UV_        
        #endif // } DEF_SENSOR_ONE_SHOT_MODE 

        // DO Fusion
        if(( gstHIDPowerState.Default == 1) // Shawn: must always false, just for compile ok in no Acc case

            #if __ENABLE_HID_ACC__ // {
            || ( gstHIDPowerState.Acc == 1)
            #endif // } __ENABLE_HID_ACC__
            
            #if __ENABLE_HID_MAG__ // {
            || ( gstHIDPowerState.Mag == 1) 
            #endif // } __ENABLE_HID_MAG__
            
            #if __ENABLE_HID_GYO__ // {
            || ( gstHIDPowerState.Gyo == 1) 
            #endif // } __ENABLE_HID_GYO__
            
            #if __ENABLE_HID_FUSION_SENSOR__ // {
            || ( gstHIDPowerState.Inc == 1) 
            || ( gstHIDPowerState.Ori == 1) 
            #endif // } __ENABLE_HID_FUSION_SENSOR__
           )
        #ifdef DEF_SENSOR_ONE_SHOT_MODE // {  DEF_SENSOR_ONE_SHOT_MODE
        {
            if(IS_FUSION_TIME())
            {
                SET_MASK(usCmdFlag, BIT(SENSOR_FUSION_IDX));
            }
        }

        if(usCmdFlag)
        {
            SET_MASK(gusSensorOneShotCmd, usCmdFlag);
            #ifdef __HID_FUSION_FUN__ // {
            hal_resume_task_in_isr(tthi_FUSION);
            #endif // } __HID_FUSION_FUN__
        }
        #else        
        if(gusSensorOneShotCmd && (gucHIDDoWCmd == 0))
        {
            #ifdef __HID_FUSION_FUN__ // {
            hal_resume_task_in_isr(tthi_FUSION);
            #endif  // } __HID_FUSION_FUN__
        }
        #endif // }  DEF_SENSOR_ONE_SHOT_MODE
    }
}

//********************************************************************************
//#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION // {
//void HID_CODE pure_ec_isr_hid_service(void)
//{
//    hid_service();
//}
//#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION
//********************************************************************************

//*****************************************************************************
//
//  include.h
//
//  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
//
//  Created on: 2012/11/14
//
//      Author: ITE00580, Dino Li
//
//*****************************************************************************

#ifndef INCLUDE_H_
#define INCLUDE_H_

//*****************************************************************************
//
// To include RTOS/EC configuration file.
//
//*****************************************************************************
#include "nds/include/config.h"

//*****************************************************************************
//
// To include chip header files.
//
//*****************************************************************************
#include "chip/chip_type.h"
#include "chip/chip_chipregs.h"
#include "api/usbpd/usbpd_reg.h"


//*****************************************************************************
//
// To include core header files.
//
//*****************************************************************************
#ifndef EC_FEATURE_NULL
#include "core/core_acpi.h"
#include "core/core_asm.h"
#include "core/core_flash.h"
#include "core/core_gpio.h"
#include "core/core_hostif.h"
#include "core/core_init.h"
#include "core/core_irq.h"
#include "core/core_main.h"
#include "core/core_memory.h"
#include "core/core_port6064.h"
#include "core/core_ps2.h"
#include "core/core_scan.h"
#include "core/core_smbus.h"
#include "core/core_timer.h"
#include "core/core_xlt.h"
#include "core/core_power.h"
#include "core/core_config.h"
#else 
#include "api/ec_null/core_power.h"
#include "api/ec_null/core_memory.h"
#include "api/ec_null/core_gpio.h"
#include "api/ec_null/core_timer.h"
#include "api/ec_null/core_irq.h"
#include "api/ec_null/core_sync.h"
#endif

//*****************************************************************************
//
// To include api header files.
//
//*****************************************************************************
#include "api/api_gpio.h"
#include "api/api_peci.h"
#include "api/api_wuc.h"
#include "api/api_sha1.h"
#include "api/api_ow.h"
#include "api/api_sspi.h"
#include "api/api_intc.h"
#include "api/api_etwd.h"
#include "api/api_adc.h"
#include "api/api_dac.h"
#include "api/api_pwm.h"
#include "api/api_i2c_slave.h"
#include "api/i2c/i2c_module.h"
#include "api/i2c/i2c.h"
#include "api/i2c/i2c_drv.h"
//#include "api/rtos_null/hal.h"






#ifndef EC_FEATURE_ONLY
//
// rtos only
//
#include "api/debug/debug_print.h"
#include "api/free_rtos/rtos-freertos/freertos/hal/hal.h"
#include "api/hid/hid_hal.h"
#else
//
// ec feature only
//
    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    #include "api/rtos_null/hal.h"
    #include "api/hid/hid_hal.h"
    #endif
#endif  // #ifndef EC_FEATURE_ONLY


#include "api/debug/debug_ns16550.h"		
#include "api/debug/debug_print.h" 			

#if _ITE_USB_PD_ENABLE_
#include "api/usbpd/power.h"				
#include "api/usbpd/usbpd_hal.h"			
#include "api/usbpd/usbpd_proto.h"			
#include "api/usbpd/usbpd_vdm.h"			
#include "api/usbpd/usbpd_reg.h"			
#include "api/usbpd/usbpd_dp.h"			
#endif


//*****************************************************************************
//
// To include oem header files.
//
//*****************************************************************************
#ifndef EC_FEATURE_NULL
#include "oem/LCFC_COMMON/INCLUDE/oem_define.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_6064.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_acpi.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_adc.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_asm.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_battery.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_cir.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_debug.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_ecpdm.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_event.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_fan.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_flash.h"
#include "oem/Project_H/INCLUDE/oem_gpio.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_hostif.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_hspi.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_init.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_irq.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_lcd.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_led.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_lpc.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_mailbox.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_main.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_memory.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_mmb.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_peci.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_pm2.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_pm3.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_port686c.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_power.h"
#include "oem/Project_H/INCLUDE/oem_project.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_ps2.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_scantabs.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_smbus.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_spi.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_timer.h"
#include "oem/Project_H/INCLUDE/oem_ver.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_tamper.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_ccg2.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_ccg3.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_gsns.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_SHUTDOWN_ID.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_LCFC.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_BEEP.h" 
#include "oem/LCFC_COMMON/INCLUDE/OEM_CHGIC_25700.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_QEVENT.h"
#include "oem/Project_H/INCLUDE/OEM_FANTBL.h"
#include "oem/Project_H/INCLUDE/OEM_POWERSEQ.h"
#include "oem/Project_H/INCLUDE/OEM_HOOK.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_EEPROM.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_VGA_AMD.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_VGA_NV.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_AOU.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_EDGE.h"
#include "oem/LCFC_COMMON/INCLUDE/Origa_DataTypes.h"
#include "oem/LCFC_COMMON/INCLUDE/Origa_Ecc.h"
#include "oem/LCFC_COMMON/INCLUDE/Origa_Gf2nMath.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_rfid.h"
//#include "oem/LCFC_COMMON/INCLUDE/Oem_tps65982.h"
#include "oem/LCFC_COMMON/INCLUDE/Oem_tps65988.h"
#include "oem/LCFC_COMMON/INCLUDE/Oem_Dash.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_EcCmd.h"
#include "oem/LCFC_COMMON/INCLUDE/Oem_Ucsi.h"
#include "oem/LCFC_COMMON/INCLUDE/Oem_bma255.h"
#include "oem/LCFC_COMMON/INCLUDE/OEM_EnhancedPcBeep.h"
#include "oem/LCFC_COMMON/INCLUDE/oem_BottomTamper.h"
#else
#include "oem/oem_project.h"
#include "oem/oem_ver.h"
#endif

//*****************************************************************************
//
//  To include nds header files.
//
//*****************************************************************************
#include "nds/include/nds32_intrinsic.h"
#include "nds/include/n12_def.h"
#include "nds/include/rom.h"
#include "nds/include/mem_layout.h"

//*****************************************************************************
//
//
//
//*****************************************************************************

#endif /* INCLUDE_H_ */


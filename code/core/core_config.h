/* 
 * ****************************************************************************
 *
 *  core_config.h
 *
 *  Copyright (c) 2013- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/01/14
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */
 
#include "../nds/include/config.h"
 
#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

/* 
 * ****************************************************************************
 * Default : 0
 * ****************************************************************************
 */
#define DEF_TIMER_OVERFLOW_FLAG_USE_ISRx        1
#define DEF_HOOK_FUNCTION_WITH_SERVICE_1MS      0

/* 
 * ****************************************************************************
 * Default : 1
 * ****************************************************************************
 */
#define DEF_MAIN_SERVICE_WITH_PRIORITY          1
#ifdef EC_FEATURE_ONLY
#define DEF_EC_FIRMWARE_DOZE_SUPPORT            0
#else
#define DEF_EC_FIRMWARE_DOZE_SUPPORT            1
#endif
#define DEF_EC_RW_EFLASH_VIA_ROM_API            1

/* 
 * ****************************************************************************
 * option
 * ****************************************************************************
 */
#ifdef EC_FEATURE_ONLY
/* ec only */
#define DEF_DIRECT_MAPPING_CORE_FLASH_SECTION   1
#else
/* rtos */
#define DEF_DIRECT_MAPPING_CORE_FLASH_SECTION   1
#define DEF_HID_FUSION_SLEEP_MODE               0
#endif

#endif


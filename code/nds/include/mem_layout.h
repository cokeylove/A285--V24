#ifndef __MEM_LAYOUT_H__ // {
#define __MEM_LAYOUT_H__
// **************************************************************************
// mem_layout.h
// 
// Define memory layout in *.lds.
//
// Copyright (c) ITE, All Rights Reserved
//
// Author      : Jyunlin, Chen
// Created on  : 2012/10/19
// **************************************************************************

// **************************************************************************
//  External Symbol
// **************************************************************************
extern char __rom_code_start, __vma_data_start, _vma_dm_s, _dm_size, _storage, _hid_fusion_buf;

// **************************************************************************
//  Macro define
// **************************************************************************
#define GET_ROM_ADDR                    ((unsigned long)&__rom_code_start)
#define GET_DATA_VMA_ADDR               ((unsigned long)&__vma_data_start)

#define GET_DM_S_VMA                    ((unsigned long)&_vma_dm_s)
#define GET_DM_S_LMA                    ((unsigned long)&_vma_dm_s)
#define GET_DM_SIZE                     ((unsigned long)&_dm_size)

#define HID_FUSION_BUFFER               ((unsigned long)&_hid_fusion_buf)
#define SENSOR_STORED_DATA_ADDRESS      ((unsigned long)&_storage)

// **************************************************************************
//  Direct Map Section Define
// **************************************************************************
// RTOS API Section
#define RTOS_CODE_H             __attribute__ ((section (".RTOS_CODE")))
#define RTOS_CODE_L             __attribute__ ((section (".RTOS_CODE_L")))
#define RTOS_DATA               __attribute__ ((section (".RTOS_DATA")))        // Read-Write Data
#define RTOS_DATA_H             __attribute__ ((section (".RTOS_DATA_H")))
                                
// OSC API Section
#define OSC_CODE                __attribute__ ((section (".OSC_CODE")))

// Sensor API Section
#define SENSOR_CODE             __attribute__ ((section (".SENSOR_CODE")))
#define SENSOR_CODE_L           __attribute__ ((section (".SENSOR_CODE_L")))
#define SENSOR_RODATA           __attribute__ ((section (".SENSOR_RODATA")))    //  Read-Only Data
#define SENSOR_DATA             __attribute__ ((section (".SENSOR_DATA")))      //  Read-Write Data
#define SENSOR_BSS              __attribute__ ((section (".SENSOR_BSS")))       //  Read-Write Data

// HID API Section
#define HID_CODE                __attribute__ ((section (".HID_CODE")))
#define HID_CODE_L              __attribute__ ((section (".HID_CODE_L")))
#define HID_RODATA              __attribute__ ((section (".HID_RODATA_L")))     //  Read-Only Data
#define HID_RODATA_L            __attribute__ ((section (".HID_RODATA_L")))     //  Read-Only Data
#define HID_DATA                __attribute__ ((section (".HID_DATA")))         //  Read-Write Data
#define HID_BSS                 __attribute__ ((section (".HID_BSS")))          //  Read-Write Data

// USB API Section
#define USB_CODE                __attribute__ ((section (".USB_CODE")))
#define USB_CODE_L              __attribute__ ((section (".USB_CODE_L")))
#define USB_DATA                __attribute__ ((section (".USB_DATA")))         //  Read-Write Data
#define USB_BSS                 __attribute__ ((section (".USB_BSS")))          //  Read-Write Data

// I2C API Section
#define I2C_CODE                __attribute__ ((section (".I2C_CODE")))
#define I2C_CODE_L              __attribute__ ((section (".I2C_CODE_L")))
#define I2C_DATA                __attribute__ ((section (".I2C_DATA")))         //  Read-Write Data
#define I2C_BSS                 __attribute__ ((section (".I2C_BSS")))          //  Read-Write Data

// Version Section
#define HID_VER_RODATA          __attribute__ ((section (".HID_VER_RODATA")))    //  Read-Only Data
#define FUSION_VER_RODATA       __attribute__ ((section (".FUSION_VER_RODATA"))) //  Read-Only Data

// Upgrade RAM Code
#define CORE_FLASH_CODE         __attribute__ ((section (".core_flash")))

// for oem ec version and hid fusion lib version.
#define OEM_VERSION              __attribute__ ((section (".OEM_VERSION"))) 


// **************************************************************************
#endif	// __MEM_LAYOUT_H__

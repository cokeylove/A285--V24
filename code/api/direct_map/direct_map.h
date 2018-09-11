/*******************************************************************************
* Copyright (C), 2000-2014, ITE Tech. Inc.
* FileName: direct_map.h
* Author  : Shawn Wang (ite01347)
* Version : 
* Date    : 2014-9-15
* Brief   : 
*******************************************************************************/
#ifndef _DIRECT_MAP_H_
#define _DIRECT_MAP_H_ // {

extern void dm_set_vma_address(unsigned long ulAddress, 
                               unsigned char ucSRAMIndex);
extern void dm_set_segment_valid(unsigned char ucSRAMIndex);
extern void dm_set_segment_invalid(unsigned char ucSRAMIndex);
extern unsigned long dm_get_segment_status(unsigned char u8SRAMIndex);

extern void immu_cache_dma_burst_length_select(unsigned char u8Param);
extern void immu_cache_size_select(unsigned char u8Param);

extern void dlm_config(unsigned char u8Index, unsigned char bEnable);

extern unsigned short dm_get_dma_map(unsigned long u32Addr);

#endif // } _DIRECT_MAP_H_


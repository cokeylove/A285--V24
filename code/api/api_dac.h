/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_dac.h
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_DAC_H
#define API_DAC_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define DAC_Channel_2    0
#define DAC_Channel_3    1
#define DAC_Channel_4    2
#define DAC_Channel_5    3

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void DAC_Channelx_Enable(BYTE p_dac_enable_channel_x);
extern void DAC_Channelx_Disable(BYTE p_dac_disable_channel_x);
extern void DAC_Channelx_Output(
    BYTE p_dac_channel_x, BYTE p_dac_channel_x_output);
extern BYTE DAC_Channelx_Read(BYTE p_dac_channel_x);

/* 
 * ****************************************************************************
 * dac registers struct
 * ****************************************************************************
 */
typedef struct DACCtrlStruct
{
    uchar_8     *dac_output_reg;	
	uchar_8 	*dac_pin_ctrl;
    uchar_8     dac_pd_ctrl;
} sDACCtrlStruct;

#endif


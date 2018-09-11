/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_adc.h
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_ADC_H
#define API_ADC_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define ADC_Channel_0    BIT0
#define ADC_Channel_1    BIT1
#define ADC_Channel_2    BIT2
#define ADC_Channel_3    BIT3
#define ADC_Channel_4    BIT4
#define ADC_Channel_5    BIT5
#define ADC_Channel_6    BIT6
#define ADC_Channel_7    BIT7

#define ADC_REGET_COUNT		5

/* 
 * ****************************************************************************
 * global variable for adc data
 * ****************************************************************************
 */
extern WORD   g_ADC_Variable_00;
extern WORD   g_ADC_Variable_01;
extern WORD   g_ADC_Variable_02;
extern WORD   g_ADC_Variable_03;
extern WORD   g_ADC_Variable_04;
extern WORD   g_ADC_Variable_05;
extern WORD   g_ADC_Variable_06;
extern WORD   g_ADC_Variable_07;

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void ADC_Initialization(void);
extern void ADC_Module_Enable(void);
extern void ADC_Module_Disable(void);
extern void ADC_Channelx_Enable(BYTE p_adc_enable_channel_x);
extern void ADC_Channelx_Disable(BYTE p_adc_disable_channel_x);
extern void ADC_Check_Channelx_Data_Valid(void);
extern WORD ADC_Get_Channelx_ADC_Data(BYTE p_adc_channel_x);
extern void ADC_Get_Channelx_Data_Valid(BYTE channelX);

/* 
 * ****************************************************************************
 * structure
 * ****************************************************************************
 */
typedef struct ADCCtrlStruct
{
    uchar_8     *adc_ctrl;
    uchar_8 	*adc_datm;
    uchar_8     *adc_datl;	
	uchar_8 	*adc_pin_ctrl;
	WORD        *adc_value;
} sADCCtrlStruct;

extern const sADCCtrlStruct asADCCtrlStruct[];

#endif


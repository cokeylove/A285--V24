/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_wuc.h
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_WUC_H
#define API_WUC_H

/* 
 * ****************************************************************************
 * WUC Input Assignments
 * ****************************************************************************
 */
/* 
 * ****************************************************************************
 * Group 1    Index       Source      INTC
 * ****************************************************************************
 */
#define WU10  0x00    //  PS2CLK0     INT5
#define WU11  0x01    //  PS2DAT0     INT5
#define WU12  0x02    //  //PS2CLK1     INT5 -> Reserved  None
#define WU13  0x03    //  //PS2DAT1     INT5 -> Reserved  None
#define WU14  0x04    //  PS2CLK2     INT5
#define WU15  0x05    //  PS2DAT2     INT5
#define WU16  0x06    //  Reserved    None
#define WU17  0x07    //  Reserved    None

/* 
 * ****************************************************************************
 * Group 2    Index       Source      INTC
 * ****************************************************************************
 */
#define WU20  0x08    //  WUI0_GPD0   INT1
#define WU21  0x09    //  WUI1_GPD1   INT31
#define WU22  0x0A    //  WUI2_GPC4   INT21
#define WU23  0x0B    //  WUI3_GPC6   INT6
#define WU24  0x0C    //  WUI4_GPD2   INT17
#define WU25  0x0D    //  PWRSW_GPE4  INT14
#define WU26  0x0E    //  SWUC Wake   INT12
#define WU27  0x0F    //  Reserved    None

/* 
 * ****************************************************************************
 * Group 3    Index       Source      INTC
 * ****************************************************************************
 */
#define WU30  0x10    //  KSI[0]      INT13
#define WU31  0x11    //  KSI[1]      INT13
#define WU32  0x12    //  KSI[2]      INT13
#define WU33  0x13    //  KSI[3]      INT13
#define WU34  0x14    //  KSI[4]      INT13
#define WU35  0x15    //  KSI[5]      INT13
#define WU36  0x16    //  KSI[6]      INT13
#define WU37  0x17    //  KSI[7]      INT13

/* 
 * ****************************************************************************
 * Group 4    Index       Source      INTC
 * ****************************************************************************
 */
#define WU40  0x18    //  WUI5_GPE5   INT5
#define WU41  0x19    //  CRX0/CRX1   INT5
#define WU42  0x1A    //  LPC Access  INT5
#define WU43  0x1B    //  SMDAT0      INT5
#define WU44  0x1C    //  SMDAT1      INT5
#define WU45  0x1D    //  WUI6_GPE6   INT5
#define WU46  0x1E    //  WUI7_GPE7   INT5
#define WU47  0x1F    //  SMDAT2      INT5

/* 
 * ****************************************************************************
 * Group 5    Index       Source      INTC
 * ****************************************************************************
 */
#define WU50  0x20    //  WUI8_GPK0	INT40
#define WU51  0x21    //  WUI9_GPK1	INT41
#define WU52  0x22    //  WUI10_GPK2	INT42
#define WU53  0x23    //  WUI11_GPK3	INT43
#define WU54  0x24    //  WUI12_GPK4	INT44
#define WU55  0x25    //  WUI13_GPK5	INT45
#define WU56  0x26    //  WUI14_GPK6	INT46
#define WU57  0x27    //  WUI15_GPK7	INT47

/* 
 * ****************************************************************************
 * Group 6    Index       Source      INTC
 * ****************************************************************************
 */
#define WU60  0x28    //  WUI16_GPH0  INT48
#define WU61  0x29    //  WUI17_GPH1  INT49
#define WU62  0x2A    //  WUI18_GPH2  INT50
#define WU63  0x2B    //  WUI19_GPH3  INT51
#define WU64  0x2C    //  WUI20_GPF4  INT52
#define WU65  0x2D    //  WUI21_GPF5  INT53
#define WU66  0x2E    //  WUI22_GPF6  INT54
#define WU67  0x2F    //  WUI23_GPF7  INT55

/* 
 * ****************************************************************************
 * Group 7    Index       Source      INTC
 * ****************************************************************************
 */
#define WU70  0x30    //  WUI24_GPE0  INT72
#define WU71  0x31    //  WUI25_GPE1  INT73
#define WU72  0x32    //  WUI26_GPE2  INT74
#define WU73  0x33    //  WUI27_GPE3  INT75
#define WU74  0x34    //  WUI28_GPI4  INT76
#define WU75  0x35    //  WUI29_GPI5  INT77
#define WU76  0x36    //  WUI30_GPI6  INT78
#define WU77  0x37    //  WUI31_GPI7  INT79

/* 
 * ****************************************************************************
 * Group 8    Index       Source      INTC
 * ****************************************************************************
 */
#define WU80  0x38    //  WUI32_GPA3  INT88
#define WU81  0x39    //  WUI33_GPA4  INT89
#define WU82  0x3A    //  WUI34_GPA5  INT90
#define WU83  0x3B    //  WUI35_GPA6  INT91
#define WU84  0x3C    //  WUI36_GPB2  INT92
#define WU85  0x3D    //  WUI37_GPC0  INT93
#define WU86  0x3E    //  WUI38_GPC7  INT94
#define WU87  0x3F    //  WUI39_GPD7  INT95

/* 
 * ****************************************************************************
 * Group 9    Index       Source      INTC
 * ****************************************************************************
 */
#define WU88  0x40    //  WUI40_GPH4  INT85
#define WU89  0x41    //  WUI41_GPH5  INT86
#define WU90  0x42    //  WUI42_GPH6  INT87
#define WU91  0x43    //  WUI43_GPA0  INT96
#define WU92  0x44    //  WUI44_GPA1  INT97
#define WU93  0x45    //  WUI45_GPA2  INT98
#define WU94  0x46    //  //WUI46_GPB4  INT99	-> Reserved None
#define WU95  0x47    //  WUI47_GPC2  INT100

/* 
 * ****************************************************************************
 * Group 10   Index       Source      INTC
 * ****************************************************************************
 */
#define WU96  0x48    //  WUI48_GPF0  INT101
#define WU97  0x49    //  WUI49_GPF1  INT102
#define WU98  0x4A    //  WUI50_GPF2  INT103
#define WU99  0x4B    //  WUI51_GPF3  INT104
#define WU100 0x4C    //  WUI52_GPA7  INT105
#define WU101 0x4D    //  WUI53_GPB0  INT106
#define WU102 0x4E    //  WUI54_GPB1  INT107
#define WU103 0x4F    //  WUI55_GPB3  INT108

/* 
 * ****************************************************************************
 * Group 11   Index       Source      INTC
 * ****************************************************************************
 */
#define WU104 0x50    //  WUI56_GPB5  INT109
#define WU105 0x51    //  WUI57_GPB6  INT110
#define WU106 0x52    //  WUI58_GPB7  INT111	-> Reserved None
#define WU107 0x53    //  WUI59_GPC1  INT112
#define WU108 0x54    //  WUI60_GPC3  INT113
#define WU109 0x55    //  WUI61_GPC5  INT114
#define WU110 0x56    //  WUI62_GPD3  INT115
#define WU111 0x57    //  WUI63_GPD4  INT116

/* 
 * ****************************************************************************
 * Group 12   Index       Source      INTC
 * ****************************************************************************
 */
#define WU112 0x58    //  WUI64_GPD5  INT117
#define WU113 0x59    //  WUI65_GPD6  INT118
#define WU114 0x5A    //  WUI66_GPE4  INT119
#define WU115 0x5B    //  WUI67_GPG0  INT120
#define WU116 0x5C    //  WUI68_GPG1  INT121
#define WU117 0x5D    //  WUI69_GPG2  INT122
#define WU118 0x5E    //  WUI70_GPG6  INT123
#define WU119 0x5F    //  WUI71_GPI0  INT124

/* 
 * ****************************************************************************
 * Group 13   Index       Source      INTC
 * ****************************************************************************
 */
#define WU120 0x60    //  WUI72_GPI1  INT125
#define WU121 0x61    //  WUI73_GPI2  INT126
#define WU122 0x62    //  WUI74_GPI3  INT127
#define WU123 0x63    //  Reserved    None
#define WU124 0x64    //  Reserved    None
#define WU125 0x65    //  Reserved    None
#define WU126 0x66    //  Reserved    None
#define WU127 0x67    //  Reserved    None

/* 
 * ****************************************************************************
 * Group 14   Index       Source      INTC
 * ****************************************************************************
 */
#define WU128 0x68    //  WUI80_GPJ0  INT128
#define WU129 0x69    //  WUI81_GPJ1  INT129
#define WU130 0x6A    //  WUI82_GPJ2  INT130
#define WU131 0x6B    //  WUI83_GPJ3  INT131
#define WU132 0x6C    //  WUI84_GPJ4  INT132
#define WU133 0x6D    //  WUI85_GPJ5  INT133
#define WU134 0x6E    //  WUI86_GPJ6  INT134
#define WU135 0x6F    //  Reserved    None

/* 
 * ****************************************************************************
 * Group 15   Index       Source      INTC
 * ****************************************************************************
 */
#define WU136 0x70    //  WUI88_GPL0  INT136
#define WU137 0x71    //  WUI89_GPL1  INT137
#define WU138 0x72    //  WUI90_GPL2  INT138
#define WU139 0x73    //  WUI91_GPL3  INT139
#define WU140 0x74    //  WUI92_GPL4  INT140
#define WU141 0x75    //  WUI93_GPL5  INT141
#define WU142 0x76    //  WUI94_GPL6  INT142
#define WU143 0x77    //  WUI95_GPL7  INT143


/* 
 * ****************************************************************************
 *
 * ****************************************************************************
 */
/* Reserved */
#define WUC_Reserved    (uchar_8 *) 0xFFFF

/* WUC input reserved and INTC none */
#define WUC_INTC_None   (uchar_8 *) 0xFFFF
#define WUC_INTC_Ctrl   0x00

/* Rising edge trigger */
#define WUC_Rising      0

/* Falling edge or either edge trigger, either edge only for group 7, 10, 12 */
#define WUC_Falling     1

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void WUC_Enable_WUx_Interrupt(BYTE wuc_input, BYTE interrupt_mode);
extern void WUC_Disable_WUx_Interrupt(BYTE wuc_input);
extern void WUC_WUx_ISR(BYTE wuc_input);
extern void WUC_Enable_WUx_Interrupt_IERx_Only(BYTE wuc_input);

/* 
 * ****************************************************************************
 * WUC Control registers struct
 * ****************************************************************************
 */
typedef struct WUCControlReg
{
    CBYTE   WUC_Input_Index;
	uchar_8	*WUC_WUEMRx;
    uchar_8	*WUC_WUESRx;	
    uchar_8	*WUC_WUENRx;
	CBYTE	WUC_Ctrl_BIT;
	uchar_8	*INTC_IERx;
    uchar_8	*INTC_ISRx;
    CBYTE   INTC_Ctrl_BIT;
} sWUCControlReg;

#endif


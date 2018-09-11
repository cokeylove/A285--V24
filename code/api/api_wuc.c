/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_wuc.c
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/* 
 * ****************************************************************************
 * WUI Input Controlling Table
 * ****************************************************************************
 */
const sWUCControlReg asWUCControlReg[] = 
{
/*
 * Index, WUEMR[x], WUESR[[x], WUENR[x], BIT[x],
 * IER[x], ISR[x], BIT[x] of IER[x] and ISR[x]
 */
/* Group 1 */
{ WU10, &WUEMR1, &WUESR1, &WUENR1, BIT0, &IER0, &ISR0, Int_WKINTAD },
{ WU11, &WUEMR1, &WUESR1, &WUENR1, BIT1, &IER0, &ISR0, Int_WKINTAD },
{ WU12, &WUEMR1, &WUESR1, &WUENR1, BIT2, &IER0, &ISR0, Int_WKINTAD },
{ WU13, &WUEMR1, &WUESR1, &WUENR1, BIT3, &IER0, &ISR0, Int_WKINTAD },
{ WU14, &WUEMR1, &WUESR1, &WUENR1, BIT4, &IER0, &ISR0, Int_WKINTAD },
{ WU15, &WUEMR1, &WUESR1, &WUENR1, BIT5, &IER0, &ISR0, Int_WKINTAD },
{ WU16, &WUEMR1, &WUESR1, &WUENR1, BIT6, &IER0, &ISR0, Int_WKINTAD },
{ WU17, &WUEMR1, &WUESR1, &WUENR1, BIT7, &IER0, &ISR0, Int_WKINTAD },

/* Group 2 */
{ WU20, &WUEMR2, &WUESR2, WUC_Reserved, BIT0, &IER0, &ISR0, Int_WKO20 },
{ WU21, &WUEMR2, &WUESR2, WUC_Reserved, BIT1, &IER3, &ISR3, Int_WKO21 },
{ WU22, &WUEMR2, &WUESR2, WUC_Reserved, BIT2, &IER2, &ISR2, Int_WKO22 },
{ WU23, &WUEMR2, &WUESR2, WUC_Reserved, BIT3, &IER0, &ISR0, Int_WKO23 },
{ WU24, &WUEMR2, &WUESR2, WUC_Reserved, BIT4, &IER2, &ISR2, Int_WKO24 },
{ WU25, &WUEMR2, &WUESR2, WUC_Reserved, BIT5, &IER1, &ISR1, Int_WKO25 },
{ WU26, &WUEMR2, &WUESR2, WUC_Reserved, BIT6, &IER1, &ISR1, Int_WKO26 },
{ WU27, &WUEMR2, &WUESR2, WUC_Reserved, BIT7, WUC_INTC_None, WUC_INTC_None,
    WUC_INTC_Ctrl },

/* Group 3 */
{ WU30, &WUEMR3, &WUESR3, &WUENR3, BIT0, &IER1, &ISR1, Int_WKINTC },
{ WU31, &WUEMR3, &WUESR3, &WUENR3, BIT1, &IER1, &ISR1, Int_WKINTC },
{ WU32, &WUEMR3, &WUESR3, &WUENR3, BIT2, &IER1, &ISR1, Int_WKINTC },
{ WU33, &WUEMR3, &WUESR3, &WUENR3, BIT3, &IER1, &ISR1, Int_WKINTC },
{ WU34, &WUEMR3, &WUESR3, &WUENR3, BIT4, &IER1, &ISR1, Int_WKINTC },
{ WU35, &WUEMR3, &WUESR3, &WUENR3, BIT5, &IER1, &ISR1, Int_WKINTC },
{ WU36, &WUEMR3, &WUESR3, &WUENR3, BIT6, &IER1, &ISR1, Int_WKINTC },
{ WU37, &WUEMR3, &WUESR3, &WUENR3, BIT7, &IER1, &ISR1, Int_WKINTC },

/* Group 4 */
{ WU40, &WUEMR4, &WUESR4, &WUENR4, BIT0, &IER0, &ISR0, Int_WKINTAD },
{ WU41, &WUEMR4, &WUESR4, &WUENR4, BIT1, &IER0, &ISR0, Int_WKINTAD },
{ WU42, &WUEMR4, &WUESR4, &WUENR4, BIT2, &IER0, &ISR0, Int_WKINTAD },
{ WU43, &WUEMR4, &WUESR4, &WUENR4, BIT3, &IER0, &ISR0, Int_WKINTAD },
{ WU44, &WUEMR4, &WUESR4, &WUENR4, BIT4, &IER0, &ISR0, Int_WKINTAD },
{ WU45, &WUEMR4, &WUESR4, &WUENR4, BIT5, &IER0, &ISR0, Int_WKINTAD },
{ WU46, &WUEMR4, &WUESR4, &WUENR4, BIT6, &IER0, &ISR0, Int_WKINTAD },
{ WU47, &WUEMR4, &WUESR4, &WUENR4, BIT7, &IER0, &ISR0, Int_WKINTAD },

/* Group 5 */
{ WU50, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT0, &IER5, &ISR5, Int_WKO50 },
{ WU51, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT1, &IER5, &ISR5, Int_WKO51 },
{ WU52, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT2, &IER5, &ISR5, Int_WKO52 },
{ WU53, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT3, &IER5, &ISR5, Int_WKO53 },
{ WU54, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT4, &IER5, &ISR5, Int_WKO54 },
{ WU55, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT5, &IER5, &ISR5, Int_WKO55 },
{ WU56, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT6, &IER5, &ISR5, Int_WKO56 },
{ WU57, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT7, &IER5, &ISR5, Int_WKO57 },

/* Group 6 */
{ WU60, &WUEMR6, &WUESR6, WUC_Reserved, BIT0, &IER6, &ISR6, Int_WKO60 },
{ WU61, &WUEMR6, &WUESR6, WUC_Reserved, BIT1, &IER6, &ISR6, Int_WKO61 },
{ WU62, &WUEMR6, &WUESR6, WUC_Reserved, BIT2, &IER6, &ISR6, Int_WKO62 },
{ WU63, &WUEMR6, &WUESR6, WUC_Reserved, BIT3, &IER6, &ISR6, Int_WKO63 },
{ WU64, &WUEMR6, &WUESR6, WUC_Reserved, BIT4, &IER6, &ISR6, Int_WKO64 },
{ WU65, &WUEMR6, &WUESR6, WUC_Reserved, BIT5, &IER6, &ISR6, Int_WKO65 },
{ WU66, &WUEMR6, &WUESR6, WUC_Reserved, BIT6, &IER6, &ISR6, Int_WKO66 },
{ WU67, &WUEMR6, &WUESR6, WUC_Reserved, BIT7, &IER6, &ISR6, Int_WKO67 },

/* Group 7 */
{ WU70, &WUEMR7, &WUESR7, WUC_Reserved, BIT0, &IER9, &ISR9, Int_WKO70 },
{ WU71, &WUEMR7, &WUESR7, WUC_Reserved, BIT1, &IER9, &ISR9, Int_WKO71 },
{ WU72, &WUEMR7, &WUESR7, WUC_Reserved, BIT2, &IER9, &ISR9, Int_WKO72 },
{ WU73, &WUEMR7, &WUESR7, WUC_Reserved, BIT3, &IER9, &ISR9, Int_WKO73 },
{ WU74, &WUEMR7, &WUESR7, WUC_Reserved, BIT4, &IER9, &ISR9, Int_WKO74 },
{ WU75, &WUEMR7, &WUESR7, WUC_Reserved, BIT5, &IER9, &ISR9, Int_WKO75 },
{ WU76, &WUEMR7, &WUESR7, WUC_Reserved, BIT6, &IER9, &ISR9, Int_WKO76 },
{ WU77, &WUEMR7, &WUESR7, WUC_Reserved, BIT7, &IER9, &ISR9, Int_WKO77 },

/* Group 8 */
{ WU80, &WUEMR8, &WUESR8, WUC_Reserved, BIT0, &IER11, &ISR11, Int_WKO80 },
{ WU81, &WUEMR8, &WUESR8, WUC_Reserved, BIT1, &IER11, &ISR11, Int_WKO81 },
{ WU82, &WUEMR8, &WUESR8, WUC_Reserved, BIT2, &IER11, &ISR11, Int_WKO82 },
{ WU83, &WUEMR8, &WUESR8, WUC_Reserved, BIT3, &IER11, &ISR11, Int_WKO83 },
{ WU84, &WUEMR8, &WUESR8, WUC_Reserved, BIT4, &IER11, &ISR11, Int_WKO84 },
{ WU85, &WUEMR8, &WUESR8, WUC_Reserved, BIT5, &IER11, &ISR11, Int_WKO85 },
{ WU86, &WUEMR8, &WUESR8, WUC_Reserved, BIT6, &IER11, &ISR11, Int_WKO86 },
{ WU87, &WUEMR8, &WUESR8, WUC_Reserved, BIT7, &IER11, &ISR11, Int_WKO87 },

/* Group 9 */
{ WU88, &WUEMR9, &WUESR9, WUC_Reserved, BIT0, &IER10, &ISR10, Int_WKO88 },
{ WU89, &WUEMR9, &WUESR9, WUC_Reserved, BIT1, &IER10, &ISR10, Int_WKO89 },
{ WU90, &WUEMR9, &WUESR9, WUC_Reserved, BIT2, &IER10, &ISR10, Int_WKO90 },
{ WU91, &WUEMR9, &WUESR9, WUC_Reserved, BIT3, &IER12, &ISR12, Int_WKO91 },
{ WU92, &WUEMR9, &WUESR9, WUC_Reserved, BIT4, &IER12, &ISR12, Int_WKO92 },
{ WU93, &WUEMR9, &WUESR9, WUC_Reserved, BIT5, &IER12, &ISR12, Int_WKO93 },
{ WU94, &WUEMR9, &WUESR9, WUC_Reserved, BIT6, &IER12, &ISR12, Int_WKO94 },
{ WU95, &WUEMR9, &WUESR9, WUC_Reserved, BIT7, &IER12, &ISR12, Int_WKO95 },

/* Group 10 */
{ WU96, &WUEMR10, &WUESR10, WUC_Reserved, BIT0, &IER12, &ISR12, Int_WKO96 },
{ WU97, &WUEMR10, &WUESR10, WUC_Reserved, BIT1, &IER12, &ISR12, Int_WKO97 },
{ WU98, &WUEMR10, &WUESR10, WUC_Reserved, BIT2, &IER12, &ISR12, Int_WKO98 },
{ WU99, &WUEMR10, &WUESR10, WUC_Reserved, BIT3, &IER13, &ISR13, Int_WKO99 },
{ WU100, &WUEMR10, &WUESR10, WUC_Reserved, BIT4, &IER13, &ISR13, Int_WKO100 },
{ WU101, &WUEMR10, &WUESR10, WUC_Reserved, BIT5, &IER13, &ISR13, Int_WKO101 },
{ WU102, &WUEMR10, &WUESR10, WUC_Reserved, BIT6, &IER13, &ISR13, Int_WKO102 },
{ WU103, &WUEMR10, &WUESR10, WUC_Reserved, BIT7, &IER13, &ISR13, Int_WKO103 },

/* Group 11 */
{ WU104, &WUEMR11, &WUESR11, WUC_Reserved, BIT0, &IER13, &ISR13, Int_WKO104 },
{ WU105, &WUEMR11, &WUESR11, WUC_Reserved, BIT1, &IER13, &ISR13, Int_WKO105 },
//{ WU106, &WUEMR11, &WUESR11, WUC_Reserved, BIT2, &IER13, &ISR13, Int_WKO106 },
{ WU106, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT7, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },

{ WU107, &WUEMR11, &WUESR11, WUC_Reserved, BIT3, &IER14, &ISR14, Int_WKO107 },
{ WU108, &WUEMR11, &WUESR11, WUC_Reserved, BIT4, &IER14, &ISR14, Int_WKO108 },
{ WU109, &WUEMR11, &WUESR11, WUC_Reserved, BIT5, &IER14, &ISR14, Int_WKO109 },
{ WU110, &WUEMR11, &WUESR11, WUC_Reserved, BIT6, &IER14, &ISR14, Int_WKO110 },
{ WU111, &WUEMR11, &WUESR11, WUC_Reserved, BIT7, &IER14, &ISR14, Int_WKO111 },

/* Group 12 */
{ WU112, &WUEMR12, &WUESR12, WUC_Reserved, BIT0, &IER14, &ISR14, Int_WKO112 },
{ WU113, &WUEMR12, &WUESR12, WUC_Reserved, BIT1, &IER14, &ISR14, Int_WKO113 },
{ WU114, &WUEMR12, &WUESR12, WUC_Reserved, BIT2, &IER14, &ISR14, Int_WKO114 },
{ WU115, &WUEMR12, &WUESR12, WUC_Reserved, BIT3, &IER15, &ISR15, Int_WKO115 },
{ WU116, &WUEMR12, &WUESR12, WUC_Reserved, BIT4, &IER15, &ISR15, Int_WKO116 },
{ WU117, &WUEMR12, &WUESR12, WUC_Reserved, BIT5, &IER15, &ISR15, Int_WKO117 },
{ WU118, &WUEMR12, &WUESR12, WUC_Reserved, BIT6, &IER15, &ISR15, Int_WKO118 },
{ WU119, &WUEMR12, &WUESR12, WUC_Reserved, BIT7, &IER15, &ISR15, Int_WKO119 },

/* Group 13 */
{ WU120, &WUEMR13, &WUESR13, WUC_Reserved, BIT0, &IER15, &ISR15, Int_WKO120 },
{ WU121, &WUEMR13, &WUESR13, WUC_Reserved, BIT1, &IER15, &ISR15, Int_WKO121 },
{ WU122, &WUEMR13, &WUESR13, WUC_Reserved, BIT2, &IER15, &ISR15, Int_WKO122 },
{ WU123, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT3, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },
{ WU124, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT4, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },
{ WU125, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT5, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },
{ WU126, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT6, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },
{ WU127, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT7, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },

/* Group 14 */
{ WU128, &WUEMR14, &WUESR14, WUC_Reserved, BIT0, &IER16, &ISR16, Int_WKO128 },
{ WU129, &WUEMR14, &WUESR14, WUC_Reserved, BIT1, &IER16, &ISR16, Int_WKO129 },
{ WU130, &WUEMR14, &WUESR14, WUC_Reserved, BIT2, &IER16, &ISR16, Int_WKO130 },
{ WU131, &WUEMR14, &WUESR14, WUC_Reserved, BIT3, &IER16, &ISR16, Int_WKO131 },
{ WU132, &WUEMR14, &WUESR14, WUC_Reserved, BIT4, &IER16, &ISR16, Int_WKO132 },
{ WU133, &WUEMR14, &WUESR14, WUC_Reserved, BIT5, &IER16, &ISR16, Int_WKO133 },
{ WU134, &WUEMR14, &WUESR14, WUC_Reserved, BIT6, &IER16, &ISR16, Int_WKO134 },
{ WU135, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT7, WUC_INTC_None,
    WUC_INTC_None, WUC_INTC_Ctrl },

///* Group 15 */
//{ WU136, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT0, &IER17, &ISR16, Int_WKO128 },
//{ WU137, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT1, &IER17, &ISR16, Int_WKO129 },
//{ WU138, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT2, &IER17, &ISR16, Int_WKO130 },
//{ WU139, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT3, &IER17, &ISR16, Int_WKO131 },
//{ WU140, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT4, &IER17, &ISR16, Int_WKO132 },
//{ WU141, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT5, &IER17, &ISR16, Int_WKO133 },
//{ WU142, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT6, &IER17, &ISR16, Int_WKO134 },
//{ WU143, WUC_Reserved, WUC_Reserved, WUC_Reserved, BIT7, &IER17, WUC_INTC_None, WUC_INTC_Ctrl },

};

/**
 * ****************************************************************************
 * The function for enabling interrupt of WUC input.
 *
 * @return
 *
 * @parameter
 * wuc_input,
 * WU10  || WU11  || WU12  || WU13  || WU14  || WU15  || WU16  || WU17 ||
 * WU20  || WU21  || WU22  || WU23  || WU24  || WU25  || WU26  || WU27 ||
 * WU30  || WU31  || WU32  || WU33  || WU34  || WU35  || WU36  || WU37 ||
 * WU40  || WU41  || WU42  || WU43  || WU44  || WU45  || WU46  || WU47 ||
 * WU50  || WU51  || WU52  || WU53  || WU54  || WU55  || WU56  || WU57 ||
 * WU60  || WU61  || WU62  || WU63  || WU64  || WU65  || WU66  || WU67 ||
 * WU70  || WU71  || WU72  || WU73  || WU74  || WU75  || WU76  || WU77 ||
 * WU80  || WU81  || WU82  || WU83  || WU84  || WU85  || WU86  || WU87 ||
 * WU88  || WU89  || WU90  || WU91  || WU92  || WU93  || WU94  || WU95 ||
 * WU96  || WU97  || WU98  || WU99  || WU100 || WU101 || WU102 || WU103 ||
 * WU104 || WU105 || WU106 || WU107 || WU108 || WU109 || WU110 || WU111 ||
 * WU112 || WU113 || WU114 || WU115 || WU116 || WU117 || WU118 || WU119 ||
 * WU120 || WU121 || WU122 || WU123 || WU124 || WU125 || WU126 || WU127 ||
 * WU128 || WU129 || WU130 || WU131 || WU132 || WU133 || WU134 || WU135 ||
 * interrupt_mode,
 * WUC_Rising || WUC_Falling
 *
 * ****************************************************************************
 */
void WUC_Enable_WUx_Interrupt(BYTE wuc_input, BYTE interrupt_mode)
{
    BYTE l_wuc_ctrl;
    BYTE l_intc_ctrl;

    l_wuc_ctrl = asWUCControlReg[wuc_input].WUC_Ctrl_BIT;
    l_intc_ctrl = asWUCControlReg[wuc_input].INTC_Ctrl_BIT;

    /* WUC input trigger mode setting */
    if(interrupt_mode == WUC_Falling)
    {
        /*
         * WUC falling edge or either edge trigger
         * either edge only for group 7, 10, 12
         */
        (*asWUCControlReg[wuc_input].WUC_WUEMRx) |= l_wuc_ctrl;
    }
    else
    {
        /* WUC rising trigger */
        (*asWUCControlReg[wuc_input].WUC_WUEMRx) &= ~l_wuc_ctrl;
    }

    /* W/C WUESRx corresponding bit */
    (*asWUCControlReg[wuc_input].WUC_WUESRx) = l_wuc_ctrl;

    /* Need enabling WUENRx */
    if(asWUCControlReg[wuc_input].WUC_WUENRx != WUC_Reserved)
    {
        // Enable wake-up interrupt
        (*asWUCControlReg[wuc_input].WUC_WUENRx) |= l_wuc_ctrl;
    }
    
    /* IERx : keep high level trigger */

    /* W/C ISRx corresponding bit */
    (*asWUCControlReg[wuc_input].INTC_ISRx) = l_intc_ctrl;

    /* Enable INTC */
    (*asWUCControlReg[wuc_input].INTC_IERx) |= l_intc_ctrl;
}

/**
 * ****************************************************************************
 * The function for disabling interrupt of WUC input.
 *
 * @return
 *
 * @parameter
 * wuc_input,
 * WU10  || WU11  || WU12  || WU13  || WU14  || WU15  || WU16  || WU17 ||
 * WU20  || WU21  || WU22  || WU23  || WU24  || WU25  || WU26  || WU27 ||
 * WU30  || WU31  || WU32  || WU33  || WU34  || WU35  || WU36  || WU37 ||
 * WU40  || WU41  || WU42  || WU43  || WU44  || WU45  || WU46  || WU47 ||
 * WU50  || WU51  || WU52  || WU53  || WU54  || WU55  || WU56  || WU57 ||
 * WU60  || WU61  || WU62  || WU63  || WU64  || WU65  || WU66  || WU67 ||
 * WU70  || WU71  || WU72  || WU73  || WU74  || WU75  || WU76  || WU77 ||
 * WU80  || WU81  || WU82  || WU83  || WU84  || WU85  || WU86  || WU87 ||
 * WU88  || WU89  || WU90  || WU91  || WU92  || WU93  || WU94  || WU95 ||
 * WU96  || WU97  || WU98  || WU99  || WU100 || WU101 || WU102 || WU103 ||
 * WU104 || WU105 || WU106 || WU107 || WU108 || WU109 || WU110 || WU111 ||
 * WU112 || WU113 || WU114 || WU115 || WU116 || WU117 || WU118 || WU119 ||
 * WU120 || WU121 || WU122 || WU123 || WU124 || WU125 || WU126 || WU127 ||
 * WU128 || WU129 || WU130 || WU131 || WU132 || WU133 || WU134 || WU135 ||
 *
 * ****************************************************************************
 */
void WUC_Disable_WUx_Interrupt(BYTE wuc_input)
{
    BYTE l_wuc_ctrl;
    BYTE l_intc_ctrl;

    l_wuc_ctrl = asWUCControlReg[wuc_input].WUC_Ctrl_BIT;
    l_intc_ctrl = asWUCControlReg[wuc_input].INTC_Ctrl_BIT;

    /* Disable INTC */
    (*asWUCControlReg[wuc_input].INTC_IERx) &= ~l_intc_ctrl;

    /* W/C WUESRx corresponding bit */
    (*asWUCControlReg[wuc_input].WUC_WUESRx) = l_wuc_ctrl;

    /* W/C ISRx corresponding bit */
    (*asWUCControlReg[wuc_input].INTC_ISRx) = l_intc_ctrl;
    
}

/**
 * ****************************************************************************
 * ISR of WUC input.
 *
 * @return
 *
 * @parameter
 * wuc_input,
 * WU10  || WU11  || WU12  || WU13  || WU14  || WU15  || WU16  || WU17 ||
 * WU20  || WU21  || WU22  || WU23  || WU24  || WU25  || WU26  || WU27 ||
 * WU30  || WU31  || WU32  || WU33  || WU34  || WU35  || WU36  || WU37 ||
 * WU40  || WU41  || WU42  || WU43  || WU44  || WU45  || WU46  || WU47 ||
 * WU50  || WU51  || WU52  || WU53  || WU54  || WU55  || WU56  || WU57 ||
 * WU60  || WU61  || WU62  || WU63  || WU64  || WU65  || WU66  || WU67 ||
 * WU70  || WU71  || WU72  || WU73  || WU74  || WU75  || WU76  || WU77 ||
 * WU80  || WU81  || WU82  || WU83  || WU84  || WU85  || WU86  || WU87 ||
 * WU88  || WU89  || WU90  || WU91  || WU92  || WU93  || WU94  || WU95 ||
 * WU96  || WU97  || WU98  || WU99  || WU100 || WU101 || WU102 || WU103 ||
 * WU104 || WU105 || WU106 || WU107 || WU108 || WU109 || WU110 || WU111 ||
 * WU112 || WU113 || WU114 || WU115 || WU116 || WU117 || WU118 || WU119 ||
 * WU120 || WU121 || WU122 || WU123 || WU124 || WU125 || WU126 || WU127 ||
 * WU128 || WU129 || WU130 || WU131 || WU132 || WU133 || WU134 || WU135 ||
 *
 * ****************************************************************************
 */
void WUC_WUx_ISR(BYTE wuc_input)
{
    BYTE l_wuc_ctrl;
    BYTE l_intc_ctrl;

    l_wuc_ctrl = asWUCControlReg[wuc_input].WUC_Ctrl_BIT;
    l_intc_ctrl = asWUCControlReg[wuc_input].INTC_Ctrl_BIT;

    /* Disable INTC */
    (*asWUCControlReg[wuc_input].INTC_IERx) &= ~l_intc_ctrl;

    /* W/C WUESRx corresponding bit */
    (*asWUCControlReg[wuc_input].WUC_WUESRx) = l_wuc_ctrl;

    /* W/C ISRx corresponding bit */
    (*asWUCControlReg[wuc_input].INTC_ISRx) = l_intc_ctrl;

    /* Enable INTC again */
    (*asWUCControlReg[wuc_input].INTC_IERx) |= l_intc_ctrl;

}

/**
 * ****************************************************************************
 * The function for enabling IERx of WUC only
 *
 * @return
 *
 * @parameter
 * wuc_input,
 * WU10  || WU11  || WU12  || WU13  || WU14  || WU15  || WU16  || WU17 ||
 * WU20  || WU21  || WU22  || WU23  || WU24  || WU25  || WU26  || WU27 ||
 * WU30  || WU31  || WU32  || WU33  || WU34  || WU35  || WU36  || WU37 ||
 * WU40  || WU41  || WU42  || WU43  || WU44  || WU45  || WU46  || WU47 ||
 * WU50  || WU51  || WU52  || WU53  || WU54  || WU55  || WU56  || WU57 ||
 * WU60  || WU61  || WU62  || WU63  || WU64  || WU65  || WU66  || WU67 ||
 * WU70  || WU71  || WU72  || WU73  || WU74  || WU75  || WU76  || WU77 ||
 * WU80  || WU81  || WU82  || WU83  || WU84  || WU85  || WU86  || WU87 ||
 * WU88  || WU89  || WU90  || WU91  || WU92  || WU93  || WU94  || WU95 ||
 * WU96  || WU97  || WU98  || WU99  || WU100 || WU101 || WU102 || WU103 ||
 * WU104 || WU105 || WU106 || WU107 || WU108 || WU109 || WU110 || WU111 ||
 * WU112 || WU113 || WU114 || WU115 || WU116 || WU117 || WU118 || WU119 ||
 * WU120 || WU121 || WU122 || WU123 || WU124 || WU125 || WU126 || WU127 ||
 * WU128 || WU129 || WU130 || WU131 || WU132 || WU133 || WU134 || WU135 ||
 *
 * @note, To enable IERx only, no w/c ISRx
 *
 * ****************************************************************************
 */
void WUC_Enable_WUx_Interrupt_IERx_Only(BYTE wuc_input)
{
    BYTE l_intc_ctrl;

    l_intc_ctrl = asWUCControlReg[wuc_input].INTC_Ctrl_BIT;

    /* Enable INTC */
    (*asWUCControlReg[wuc_input].INTC_IERx) |= l_intc_ctrl;
}


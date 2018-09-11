/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_intc.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_INTC_H
#define API_INTC_H

/* 
 * ****************************************************************************
 * INTC Interrupt Assignments
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 *      [Group 0]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT0      0	        // -
#define	Interrupt_INT1	    1           // WKO[20]
#define	Interrupt_INT2	    2	        // KBC Output Buffer Empty Interrupt
#define	Interrupt_INT3	    3           // PMC Output Buffer Empty Interrupt
#define	Interrupt_INT4	    4           // SMBUS D Interrupt	
#define Interrupt_INT5	    5           // WKINTAD (WKINTA or WKINTD)
#define Interrupt_INT6	    6           // WKO[23]
#define Interrupt_INT7	    7           // PWM Interrupt

/* 
 * ****************************************************************************
 *      [Group 1]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT8      8           // ADC Interrupt
#define	Interrupt_INT9      9           // SMBUS A Interrupt
#define	Interrupt_INT10	    10          // SMBUS B Interrupt
#define	Interrupt_INT11	    11          // KB Matrix Scan Interrupt
#define	Interrupt_INT12	    12          // WKO[26]
#define Interrupt_INT13	    13          // WKINTC
#define Interrupt_INT14	    14          // WKO[25]
#define Interrupt_INT15	    15          // CIR Interrupt

/* 
 * ****************************************************************************
 *      [Group 2]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT16	    16          // SMBUS C Interrupt
#define Interrupt_INT17	    17          // WKO[24]
#define	Interrupt_INT18     18          // PS/2 Interrupt 2
#define	Interrupt_INT19	    19          // PS/2 Interrupt 1
#define	Interrupt_INT20	    20          // PS/2 Interrupt 0
#define	Interrupt_INT21	    21          // WKO[22]
#define	Interrupt_INT22	    22          // SMFI Semaphore Interrupt
#define	Interrupt_INT23	    23          // -

/* 
 * ****************************************************************************
 *      [Group 3]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT24	    24          // KBC Input Buffer Full Interrupt
#define Interrupt_INT25	    25          // PMC Input Buffer Full Interrupt
#define Interrupt_INT26     26          // PMC2 Output Buffer Empty Intr.
#define Interrupt_INT27	    27          // PMC2 Input Buffer Full Intr.
#define	Interrupt_INT28     28          // GINT from function 1 of GPD5
#define	Interrupt_INT29     29          // EGPC Interrupt
#define	Interrupt_INT30     30          // External Timer Interrupt
#define	Interrupt_INT31     31          // WKO[21]

/* 
 * ****************************************************************************
 *      [Group 4]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT32     32          // GPINT0
#define	Interrupt_INT33     33          // GPINT1
#define	Interrupt_INT34     34          // GPINT2
#define	Interrupt_INT35     35          // GPINT3
#define	Interrupt_INT36     36          // CIR GPINT
#define	Interrupt_INT37     37          // SSPI Interrupt
#define	Interrupt_INT38     38          // UART1 Interrupt
#define Interrupt_INT39     39          // UART2 Interrupt

/* 
 * ****************************************************************************
 *      [Group 5]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT40     40          // -
#define Interrupt_INT41     41          // -
#define Interrupt_INT42     42          // -
#define Interrupt_INT43     43          // -
#define Interrupt_INT44     44          // -
#define Interrupt_INT45     45          // -
#define Interrupt_INT46     46          // -
#define Interrupt_INT47     47          // -

/* 
 * ****************************************************************************
 *      [Group 6]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT48	    48          // WKO[60]
#define	Interrupt_INT49	    49          // WKO[61]
#define	Interrupt_INT50	    50          // WKO[62]
#define	Interrupt_INT51	    51          // WKO[63]
#define	Interrupt_INT52	    52          // WKO[64]
#define	Interrupt_INT53	    53          // WKO[65]
#define	Interrupt_INT54	    54          // WKO[66]
#define	Interrupt_INT55	    55          // WKO[67]

/* 
 * ****************************************************************************
 *      [Group 7]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT56     56          // RTCT Alarm 1
#define	Interrupt_INT57     57          // RTCT Alarm 2
#define Interrupt_INT58	    58          // External Timer 2 Interrupt
#define Interrupt_INT59     59          // -
#define Interrupt_INT60     60          // TMRINTA0
#define Interrupt_INT61     61          // TMRINTA1
#define Interrupt_INT62     62          // TMRINTB0
#define	Interrupt_INT63     63          // TMRINTB1

/* 
 * ****************************************************************************
 *      [Group 8]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT64     64          // PMC2EX Output Buffer Empty Intr.
#define	Interrupt_INT65     65          // PMC2EX Input Buffer Full Intr.
#define	Interrupt_INT66     66          // PMC3 Output Buffer Empty Intr.
#define Interrupt_INT67     67          // PMC3 Input Buffer Full Interrupt
#define Interrupt_INT68     68          // PMC4 Output Buffer Empty Intr.
#define	Interrupt_INT69     69          // PMC4 Input Buffer Full Intr.
#define	Interrupt_INT70	    70          //
#define	Interrupt_INT71     71          // I2BRAM Interrupt

/* 
 * ****************************************************************************
 *      [Group 9]           [Interrupt] [Description]
 * ****************************************************************************
 */
#define	Interrupt_INT72	    72          // WKO[70]
#define	Interrupt_INT73	    73          // WKO[71]
#define	Interrupt_INT74     74          // WKO[72]
#define	Interrupt_INT75	    75          // WKO[73]
#define	Interrupt_INT76	    76          // WKO[74]
#define	Interrupt_INT77	    77          // WKO[75]
#define	Interrupt_INT78	    78          // WKO[76]
#define	Interrupt_INT79	    79          // WKO[77]

/* 
 * ****************************************************************************
 *      [Group 10]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT80     80          // External Timer 8 Interrupt
#define Interrupt_INT81     81          // SMbus Clock Held intr
#define Interrupt_INT82     82          // CEC Interrupt
#define Interrupt_INT83     83          // H2RAM LPC Trigger
#define Interrupt_INT84     84          // KB Scan Data Valid Interrupt
#define Interrupt_INT85     85          // WKO[88]
#define Interrupt_INT86     86          // WKO[89]
#define Interrupt_INT87     87          // WKO[90]

/* 
 * ****************************************************************************
 *      [Group 11]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT88     88          // WKO[80]
#define Interrupt_INT89     89          // WKO[81]
#define Interrupt_INT90     90          // WKO[82]
#define Interrupt_INT91     91          // WKO[83]
#define Interrupt_INT92     92          // WKO[84]
#define Interrupt_INT93     93          // WKO[85]
#define Interrupt_INT94     94          // WKO[86]
#define Interrupt_INT95     95          // WKO[87]

/* 
 * ****************************************************************************
 *      [Group 12]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT96     96          // WKO[91]
#define Interrupt_INT97     97          // WKO[92]
#define Interrupt_INT98     98          // WKO[93]
#define Interrupt_INT99     99          // WKO[94]
#define Interrupt_INT100    100         // WKO[95]
#define Interrupt_INT101    101         // WKO[96]
#define Interrupt_INT102    102         // WKO[97]
#define Interrupt_INT103    103         // WKO[98]

/* 
 * ****************************************************************************
 *      [Group 13]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT104    104         // WKO[99]
#define Interrupt_INT105    105         // WKO[100]
#define Interrupt_INT106    106         // WKO[101]
#define Interrupt_INT107    107         // WKO[102]
#define Interrupt_INT108    108         // WKO[103]
#define Interrupt_INT109    109         // WKO[104]
#define Interrupt_INT110    110         // WKO[105]
#define Interrupt_INT111    111         // WKO[106]

/* 
 * ****************************************************************************
 *      [Group 14]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT112    112         // WKO[107]
#define Interrupt_INT113    113         // WKO[108]
#define Interrupt_INT114    114         // WKO[109]
#define Interrupt_INT115    115         // WKO[110]
#define Interrupt_INT116    116         // WKO[111]
#define Interrupt_INT117    117         // WKO[112]
#define Interrupt_INT118    118         // WKO[113]
#define Interrupt_INT119    119         // WKO[114]

/* 
 * ****************************************************************************
 *      [Group 15]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT120    120         // WKO[115]
#define Interrupt_INT121    121         // WKO[116]
#define Interrupt_INT122    122         // WKO[117]
#define Interrupt_INT123    123         // WKO[118]
#define Interrupt_INT124    124         // WKO[119]
#define Interrupt_INT125    125         // WKO[120]
#define Interrupt_INT126    126         // WKO[121]
#define Interrupt_INT127    127         // WKO[122]

/* 
 * ****************************************************************************
 *      [Group 16]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT128    128         // WKO[128]
#define Interrupt_INT129    129         // WKO[129]
#define Interrupt_INT130    130         // WKO[130]
#define Interrupt_INT131    131         // WKO[131]
#define Interrupt_INT132    132         // WKO[132]
#define Interrupt_INT133    133         // WKO[133]
#define Interrupt_INT134    134         // WKO[134]
#define Interrupt_INT135    135         // -

/* 
 * ****************************************************************************
 *      [Group 17]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT136    136         // -
#define	Interrupt_INT137    137         // -
#define	Interrupt_INT138    138         // -
#define	Interrupt_INT139    139         // -
#define	Interrupt_INT140    140         // -
#define	Interrupt_INT141    141         // -
#define	Interrupt_INT142    142         // -
#define	Interrupt_INT143    143         // -

/* 
 * ****************************************************************************
 *      [Group 18]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT144    144         // -
#define Interrupt_INT145    145         // -
#define Interrupt_INT146    146         // -
#define Interrupt_INT147    147         // -
#define Interrupt_INT148    148         // -
#define Interrupt_INT149    149         // PMC5 Output Buffer Empty Intr.
#define Interrupt_INT150    150         // PMC5 Input Buffer Full Intr.
#define Interrupt_INT151    151         // Voltage Compare Interrupt

/* 
 * ****************************************************************************
 *      [Group 19]          [Interrupt] [Description]
 * ****************************************************************************
 */
#define Interrupt_INT152    152         // SMBus E Interrupt
#define	Interrupt_INT153    153         // SMBus F Interrupt
#define	Interrupt_INT154    154         // OSC DMA Interrupt
#define	Interrupt_INT155    155         // External Timer 3 Interrupt
#define	Interrupt_INT156    156         // External Timer 4 Interrupt
#define	Interrupt_INT157    157         // External Timer 5 Interrupt
#define	Interrupt_INT158    158         // External Timer 6 Interrupt
#define	Interrupt_INT159    159         // External Timer 7 Interrupt

/*
 * BIT7:0 level trigger, BIT7:1 edge trigger
 * BIT0:0 level-high-triggered or rising-edge-triggered,
 * BIT0:1 level-low-triggered or falling-edge-triggered
 */
#define INT_Level_Trigger                   0x00
#define INT_Edge_Trigger                    BIT7
#define INT_Polarity0                       0x00
#define INT_Polarity1                       BIT0

#define INT_Trigger_Mode_Set_LevelHigh      (INT_Level_Trigger+INT_Polarity0)
#define INT_Trigger_Mode_Set_LevelLow       (INT_Level_Trigger+INT_Polarity1)
#define INT_Trigger_Mode_Set_RisingEdge     (INT_Edge_Trigger+INT_Polarity0)
#define INT_Trigger_Mode_Set_FallingEdge    (INT_Edge_Trigger+INT_Polarity1)

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void INTC_Enable_INTx(BYTE interrup_index, BYTE interrupt_trigger_mode);
extern void INTC_Disable_INTx(BYTE interrup_index);
extern void INTC_Clear_ISRx(BYTE interrup_index);
extern void INTC_Disable_IERx(BYTE interrup_index);

/* 
 * ****************************************************************************
 * INTC registers struct
 * ****************************************************************************
 */
typedef struct INTC_Reg
{
    /*
     * Each bit determines whether its corresponding interrupt channel (INT7-0)
     * is masked or enabled.
     */
    uchar_8	*INTC_IERx;

    /* Interrupt input status of INTx. */
    uchar_8	*INTC_ISRx;

    /*
     * It determines whether its corresponding interrupt channel is
     * level-triggered or edge-triggered.
     * 0: level-triggered, 1: edge-triggered.
     */
    uchar_8	*INTC_IELMRx;

    /*
     * Each bit determines the active high/low of the corresponding interrupt
     * channel (INT7-0).
     * 0: level-high-triggered or rising-edge-triggered
     * 1: level-low-triggered or falling-edge-triggered
     */
    uchar_8	*INTC_IPOLRx;
} sINTC_Reg;

#endif


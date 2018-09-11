/* 
 * ****************************************************************************
 * core_irq.h
 * ****************************************************************************
 */

#ifndef CORE_IRQ_H
#define CORE_IRQ_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
//*****************************************************************************
//      [Group 0]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT0      0	        // -
#define	IVECT_INT1	    1           // WKO[20]
#define	IVECT_INT2	    2	        // KBC Output Buffer Empty Interrupt
#define	IVECT_INT3	    3           // PMC Output Buffer Empty Interrupt
#define	IVECT_INT4	    4           // SMBUS D Interrupt	
#define IVECT_INT5	    5           // WKINTAD (WKINTA or WKINTD)
#define IVECT_INT6	    6           // WKO[23]
#define IVECT_INT7	    7           // PWM Interrupt

//*****************************************************************************
//      [Group 1]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT8      8           // ADC Interrupt
#define	IVECT_INT9      9           // SMBUS A Interrupt
#define	IVECT_INT10	    10          // SMBUS B Interrupt
#define	IVECT_INT11	    11          // KB Matrix Scan Interrupt
#define	IVECT_INT12	    12          // WKO[26]
#define IVECT_INT13	    13          // WKINTC
#define IVECT_INT14	    14          // WKO[25]
#define IVECT_INT15	    15          // CIR Interrupt

//*****************************************************************************
//      [Group 2]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT16	    16          // SMBUS C Interrupt
#define IVECT_INT17	    17          // WKO[24]
#define	IVECT_INT18     18          // PS/2 Interrupt 2
#define	IVECT_INT19	    19          // PS/2 Interrupt 1
#define	IVECT_INT20	    20          // PS/2 Interrupt 0
#define	IVECT_INT21	    21          // WKO[22]
#define	IVECT_INT22	    22          // SMFI Semaphore Interrupt
#define	IVECT_INT23	    23          // -

//*****************************************************************************
//      [Group 3]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT24	    24          // KBC Input Buffer Full Interrupt
#define IVECT_INT25	    25          // PMC Input Buffer Full Interrupt
#define IVECT_INT26     26          // PMC2 Output Buffer Empty Intr.
#define IVECT_INT27	    27          // PMC2 Input Buffer Full Intr.
#define	IVECT_INT28     28          // GINT from function 1 of GPD5
#define	IVECT_INT29     29          // EGPC Interrupt
#define	IVECT_INT30     30          // External Timer Interrupt
#define	IVECT_INT31     31          // WKO[21]

//*****************************************************************************
//      [Group 4]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT32     32          // GPINT0
#define	IVECT_INT33     33          // GPINT1
#define	IVECT_INT34     34          // GPINT2
#define	IVECT_INT35     35          // GPINT3
#define	IVECT_INT36     36          // CIR GPINT
#define	IVECT_INT37     37          // SSPI Interrupt
#define	IVECT_INT38     38          // UART1 Interrupt
#define IVECT_INT39     39          // UART2 Interrupt

//*****************************************************************************
//      [Group 5]       [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT40     40          // -
#define IVECT_INT41     41          // -
#define IVECT_INT42     42          // -
#define IVECT_INT43     43          // -
#define IVECT_INT44     44          // -
#define IVECT_INT45     45          // -
#define IVECT_INT46     46          // -
#define IVECT_INT47     47          // -

//*****************************************************************************
//      [Group 6]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT48	    48          // WKO[60]
#define	IVECT_INT49	    49          // WKO[61]
#define	IVECT_INT50	    50          // WKO[62]
#define	IVECT_INT51	    51          // WKO[63]
#define	IVECT_INT52	    52          // WKO[64]
#define	IVECT_INT53	    53          // WKO[65]
#define	IVECT_INT54	    54          // WKO[66]
#define	IVECT_INT55	    55          // WKO[67]

//*****************************************************************************
//      [Group 7]       [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT56     56          // RTCT Alarm 1
#define	IVECT_INT57     57          // RTCT Alarm 2
#define IVECT_INT58	    58          // External Timer 2 Interrupt
#define IVECT_INT59     59          // -
#define IVECT_INT60     60          // TMRINTA0
#define IVECT_INT61     61          // TMRINTA1
#define IVECT_INT62     62          // TMRINTB0
#define	IVECT_INT63     63          // TMRINTB1

//*****************************************************************************
//      [Group 8]       [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT64     64          // PMC2EX Output Buffer Empty Intr.
#define	IVECT_INT65     65          // PMC2EX Input Buffer Full Intr.
#define	IVECT_INT66     66          // PMC3 Output Buffer Empty Intr.
#define IVECT_INT67     67          // PMC3 Input Buffer Full Interrupt
#define IVECT_INT68     68          // PMC4 Output Buffer Empty Intr.
#define	IVECT_INT69     69          // PMC4 Input Buffer Full Intr.
#define	IVECT_INT70	    70          //
#define	IVECT_INT71     71          // I2BRAM Interrupt

//*****************************************************************************
//      [Group 9]       [Interrupt] [Description]
//*****************************************************************************
#define	IVECT_INT72	    72          // WKO[70]
#define	IVECT_INT73	    73          // WKO[71]
#define	IVECT_INT74     74          // WKO[72]
#define	IVECT_INT75	    75          // WKO[73]
#define	IVECT_INT76	    76          // WKO[74]
#define	IVECT_INT77	    77          // WKO[75]
#define	IVECT_INT78	    78          // WKO[76]
#define	IVECT_INT79	    79          // WKO[77]

//*****************************************************************************
//      [Group 10]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT80     80          // External Timer 8 Interrupt
#define IVECT_INT81     81          // SMbus Clock Held intr
#define IVECT_INT82     82          // CEC Interrupt
#define IVECT_INT83     83          // H2RAM LPC Trigger
#define IVECT_INT84     84          // KB Scan Data Valid Interrupt
#define IVECT_INT85     85          // WKO[88]
#define IVECT_INT86     86          // WKO[89]
#define IVECT_INT87     87          // WKO[90]

//*****************************************************************************
//      [Group 11]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT88     88          // WKO[80]
#define IVECT_INT89     89          // WKO[81]
#define IVECT_INT90     90          // WKO[82]
#define IVECT_INT91     91          // WKO[83]
#define IVECT_INT92     92          // WKO[84]
#define IVECT_INT93     93          // WKO[85]
#define IVECT_INT94     94          // WKO[86]
#define IVECT_INT95     95          // WKO[87]

//*****************************************************************************
//      [Group 12]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT96     96          // WKO[91]
#define IVECT_INT97     97          // WKO[92]
#define IVECT_INT98     98          // WKO[93]
#define IVECT_INT99     99          // WKO[94]
#define IVECT_INT100    100         // WKO[95]
#define IVECT_INT101    101         // WKO[96]
#define IVECT_INT102    102         // WKO[97]
#define IVECT_INT103    103         // WKO[98]

//*****************************************************************************
//      [Group 13]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT104    104         // WKO[99]
#define IVECT_INT105    105         // WKO[100]
#define IVECT_INT106    106         // WKO[101]
#define IVECT_INT107    107         // WKO[102]
#define IVECT_INT108    108         // WKO[103]
#define IVECT_INT109    109         // WKO[104]
#define IVECT_INT110    110         // WKO[105]
#define IVECT_INT111    111         // WKO[106]

//*****************************************************************************
//      [Group 14]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT112    112         // WKO[107]
#define IVECT_INT113    113         // WKO[108]
#define IVECT_INT114    114         // WKO[109]
#define IVECT_INT115    115         // WKO[110]
#define IVECT_INT116    116         // WKO[111]
#define IVECT_INT117    117         // WKO[112]
#define IVECT_INT118    118         // WKO[113]
#define IVECT_INT119    119         // WKO[114]

//*****************************************************************************
//      [Group 15]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT120    120         // WKO[115]
#define IVECT_INT121    121         // WKO[116]
#define IVECT_INT122    122         // WKO[117]
#define IVECT_INT123    123         // WKO[118]
#define IVECT_INT124    124         // WKO[119]
#define IVECT_INT125    125         // WKO[120]
#define IVECT_INT126    126         // WKO[121]
#define IVECT_INT127    127         // WKO[122]

//*****************************************************************************
//      [Group 16]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT128    128         // WKO[128]
#define IVECT_INT129    129         // WKO[129]
#define IVECT_INT130    130         // WKO[130]
#define IVECT_INT131    131         // WKO[131]
#define IVECT_INT132    132         // WKO[132]
#define IVECT_INT133    133         // WKO[133]
#define IVECT_INT134    134         // WKO[134]
#define IVECT_INT135    135         // -

//*****************************************************************************
//      [Group 17]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT136    136         // -
#define	IVECT_INT137    137         // -
#define	IVECT_INT138    138         // -
#define	IVECT_INT139    139         // -
#define	IVECT_INT140    140         // -
#define	IVECT_INT141    141         // -
#define	IVECT_INT142    142         // -
#define	IVECT_INT143    143         // -

//*****************************************************************************
//      [Group 18]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT144    144         // -
#define IVECT_INT145    145         // -
#define IVECT_INT146    146         // -
#define IVECT_INT147    147         // -
#define IVECT_INT148    148         // -
#define IVECT_INT149    149         // PMC5 Output Buffer Empty Intr.
#define IVECT_INT150    150         // PMC5 Input Buffer Full Intr.
#define IVECT_INT151    151         // Voltage Compare Interrupt

//*****************************************************************************
//      [Group 19]      [Interrupt] [Description]
//*****************************************************************************
#define IVECT_INT152    152         // SMBus E Interrupt
#define	IVECT_INT153    153         // SMBus F Interrupt
#define	IVECT_INT154    154         // OSC DMA Interrupt
#define	IVECT_INT155    155         // External Timer 3 Interrupt
#define	IVECT_INT156    156         // External Timer 4 Interrupt
#define	IVECT_INT157    157         // External Timer 5 Interrupt
#define	IVECT_INT158    158         // External Timer 6 Interrupt
#define	IVECT_INT159    159         // External Timer 7 Interrupt

/* 
 * ****************************************************************************
 * section for LDS
 * ****************************************************************************
 */
#define Section_General_Exception __attribute__ \
                                        ((section ("._General_Exception")))
#define Section_Interrupt_HW0 __attribute__ ((section ("._Interrupt_HW0")))
#define Section_Interrupt_HW1 __attribute__ ((section ("._Interrupt_HW1")))
#define Section_Interrupt_HW2 __attribute__ ((section ("._Interrupt_HW2")))
#define Section_Interrupt_HW3 __attribute__ ((section ("._Interrupt_HW3")))
#define Section_Interrupt_HW4 __attribute__ ((section ("._Interrupt_HW4")))
#define Section_Interrupt_HW5 __attribute__ ((section ("._Interrupt_HW5")))
#define Section_Interrupt_HW6 __attribute__ ((section ("._Interrupt_HW6")))
#define Section_Interrupt_HW7 __attribute__ ((section ("._Interrupt_HW7")))
#define Section_Interrupt_HW8 __attribute__ ((section ("._Interrupt_HW8")))
#define Section_Interrupt_HW9 __attribute__ ((section ("._Interrupt_HW9")))
#define Section_Interrupt_HW10 __attribute__ ((section ("._Interrupt_HW10")))
#define Section_Interrupt_HW11 __attribute__ ((section ("._Interrupt_HW11")))
#define Section_Interrupt_HW12 __attribute__ ((section ("._Interrupt_HW12")))
#define Section_Interrupt_HW13 __attribute__ ((section ("._Interrupt_HW13")))
#define Section_Interrupt_HW14 __attribute__ ((section ("._Interrupt_HW14")))
#define Section_Interrupt_HW15 __attribute__ ((section ("._Interrupt_HW15")))
#define Section_Interrupt_SW0 __attribute__ ((section ("._Interrupt_SW0")))
#define Section_Isr_Int1 __attribute__ ((section ("._Isr_Int1")))
#define ISR_RODATA __attribute__ ((section ("._ISR_RODATA")))
#define ISR_CODE __attribute__ ((section ("._ISR_CODE")))
#define ISR_CODE_OEM __attribute__ ((section ("._ISR_CODE_OEM")))  

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void DisableAllInterrupt(void);
extern void EnableAllInterrupt(void);
extern void EnableKBCIBFInt(void);
extern void EnablePMCIBFInt(void);
extern void IRQ_INT18_Null(void);
extern void IRQ_INT19_Null(void);
extern void IRQ_INT20_Null(void);
extern void IRQ_INT18_PS2Interrupt2(void);
extern void IRQ_INT19_PS2Interrupt1(void);
extern void IRQ_INT20_PS2Interrupt0(void);
extern void InitEnableInterrupt(void);
extern void EnableModuleInterrupt(void);
extern void EnableGlobalInt(void);
extern void DisableGlobalInt(void);
extern void EnableExternalInt(void);

#endif


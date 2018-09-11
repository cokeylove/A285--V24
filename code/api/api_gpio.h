/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_gpio.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_GPIO_H
#define API_GPIO_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
/* GPIO ctrl define */

/* Alternate function */
#define ALT                     0x00

/* Input */
#define INPUT                   0x80

/* Output */
#define OUTPUT                  0x40

/* Port pin pull up */
#define PULL_UP                 0x04

/* Port pin pull down */
#define PULL_DW                 0x02

/* No output type control register */
#define OutputType_None         0x00

/* Push-pull output */
#define OutputType_Push_Pull    0x01

/* Open-drain output */
#define OutputType_Open_Drain   0x02

/* No output type register with this pin */
#define NoCtrlReg    (uchar_8 *) 0xFFFF

/* GPIO pin output toggle */
#define OutputMode_Inverse      0x02

/* GPIO pin output high */
#define OutputMode_High         0x01

/* GPIO pin output low */
#define OutputMode_Low          0x00

/* GPIO pin status high */
#define InputMode_High          0x01

/* GPIO pin status low */
#define InputMode_Low           0x00

/* GPIO port data mirror register high */
#define Mirror_High             0x01

/* GPIO port data mirror register low */
#define Mirror_Low              0x00

/* 
 * ****************************************************************************
 * GPIO Group & Pin Assignments
 * ****************************************************************************
 */
/*
 * ****************************************************************************
 *      Group A Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOA0 0x00 //  24  PWM0    WUI43   None    16mA        Up/Dn       GPI
#define GPIOA1 0x01 //  25  PWM1    WUI44   None    16mA        Up/Dn       GPI
#define GPIOA2 0x02 //  28  PWM2    WUI45   None    16mA        Up/Dn       GPI
#define GPIOA3 0x03 //  29  PWM3    WUI32   None    16mA        Up/Dn       GPI
#define GPIOA4 0x04 //  30  PWM4    WUI33   None    8mA         Up/Dn       GPI
#define GPIOA5 0x05 //  31  PWM5    WUI34   None    8mA         Up/Dn       GPI
#define GPIOA6 0x06 //  32  PWM6    WUI35   SSCK    8mA         Up/Dn       GPI
#define GPIOA7 0x07 //  34  PWM7    WUI52   RIG1#   8mA         Up/Dn       GPI

/*
 * ****************************************************************************
 *      Group B Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOB0 0x08 //  108 RXD     WUI53   SIN0    16mA        Up/Dn       GPI
#define GPIOB1 0x09 //  109 TXD     WUI54   SOUT0   16mA        Up/Dn       GPI
#define GPIOB2 0x0A //  123 CTX0    WUI36   TMA0    8mA         Up/Dn       GPI
#define GPIOB3 0x0B //  110 SMCLK0  WUI55   None    4mA         Up/Dn       GPI
#define GPIOB4 0x0C //  111 SMDAT0  WUI46   None    4mA         Up/Dn       GPI
#define GPIOB5 0x0D //  126 GA20    WUI56   None    2mA         Up/Dn       GPI
#define GPIOB6 0x0E //  4   KBRST#  WUI57   None    2mA         Up/Dn       GPI
#define GPIOB7 0x0F //  112 RING#   PWRFAIL CK32K   16mA        Up/Dn       GPI
                    //              LPCRST
                    //              WUI58

/*
 * ****************************************************************************
 *      Group C Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOC0 0x10 //  119 CRX0    WUI37   None    2mA         Up/Dn       GPI
#define GPIOC1 0x11 //  115 SMCLK1  WUI59   None    4mA         Up/Dn       GPI
#define GPIOC2 0x12 //  116 SMDAT1  WUI47   None    4mA         Up/Dn       GPI
#define GPIOC3 0x13 //  56  KSO16   WUI60   SMOSI   8mA         Up/Dn       GPI
#define GPIOC4 0x14 //  120 TMRI0   WUI2    None    2mA         Up/Dn       GPI
#define GPIOC5 0x15 //  57  KSO17   WUI61   SMISO   8mA         Up/Dn       GPI
#define GPIOC6 0x16 //  124 TMRI1   WUI3    None    2mA         Up/Dn       GPI
#define GPIOC7 0x17 //  16  PWUREQ# WUI38   BBO     16mA        Up/Dn       GPI
                    //                      SMCLK2ALT

/*
 * ****************************************************************************
 *      Group D Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOD0 0x18 //  18  RI1#    WUI0    None    4mA         Up/Dn       GPI
#define GPIOD1 0x19 //  21  RI2#    WUI1    None    4mA         Up/Dn       GPI
#define GPIOD2 0x1A //  22  LPCRST# WUI4    None    8mA         Up/Dn     Func1
#define GPIOD3 0x1B //  23  ECSCI#  WUI62   None    8mA         Up/Dn       GPI
#define GPIOD4 0x1C //  15  ECSMI#  WUI63   None    8mA         Up/Dn       GPI
#define GPIOD5 0x1D //  33  GINT    WUI64   CTS0#   8mA         Up/Dn       GPI
#define GPIOD6 0x1E //  47  TACH0A  WUI65   None    2mA         Up/Dn       GPI
#define GPIOD7 0x1F //  48  TACH1A  WUI39   TMA1    2mA         Up/Dn       GPI

/*
 * ****************************************************************************
 *      Group E Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOE0 0x20 //  19  L80HLAT WUI24   BAO     16mA        Up/Dn       GPI
#define GPIOE1 0x21 //  82  EGAD    WUI25   None    8mA         Up/Dn       GPI
#define GPIOE2 0x22 //  83  EGCS#   WUI26   None    8mA         Up/Dn       GPI
#define GPIOE3 0x23 //  84  EGCLK   WUI27   None    16mA        Up/Dn       GPI
#define GPIOE4 0x24 //  125 PWRSW   WUI66   None    2mA         Up/Dn       GPI
#define GPIOE5 0x25 //  35  None    WUI5    RTS1#   2mA         Up/Dn       GPI
#define GPIOE6 0x26 //  17  LPCPD#  WUI6    None    2mA         Up/Dn       GPI
#define GPIOE7 0x27 //  20  L80LLAT WUI7    None    16mA        Up/Dn       GPI

/*
 * ****************************************************************************
 *      Group F Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOF0 0x28 //  85  PS2CLK0 CEC     TMB0    8mA         Up/Dn       GPI
                    //              WUI48
#define GPIOF1 0x29 //  86  PS2DAT0 WUI49   TMB1    8mA         Up/Dn       GPI
#define GPIOF2 0x2A //  87  PS2CLK1 WUI50   DTR0#   8mA         Up/Dn       GPI
#define GPIOF3 0x2B //  88  PS2DAT1 WUI51   RTS0#   8mA         Up/Dn       GPI
#define GPIOF4 0x2C //  89  PS2CLK2 WUI20   None    8mA         Up/Dn       GPI
#define GPIOF5 0x2D //  90  PS2DAT2 WUI21   None    8mA         Up/Dn       GPI
#define GPIOF6 0x2E //  117 SMCLK2  WUI22   PECI    4mA         Up/Dn       GPI
#define GPIOF7 0x2F //  118 SMDAT2  WUI23   PECIRQT 4mA         Up/Dn       GPI

/*
 * ****************************************************************************
 *      Group G Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOG0 0x30 //  106 None    WUI67   SSCE1#  8mA         Up/Dn     GPO,H
#define GPIOG1 0x31 //  107 FDIO2   WUI68   DTR1#   16mA        Up/Dn     GPO,L
                    //                      SBUSY                           ID7
#define GPIOG2 0x32 //  100 None    WUI69   SSCE0#  4mA         Up/Dn       GPI
#define GPIOG3 0x33 //  101 FSCE#   None    None    8mA (output only)       GPI
#define GPIOG4 0x34 //  102 FMOSI   None    None    8mA (output only)       GPI
#define GPIOG5 0x35 //  103 FMISO   None    None        (input only)        GPI
#define GPIOG6 0x36 //  104 FDIO3   WUI70   DSR0#   4mA         Up/Dn       GPI
#define GPIOG7 0x37 //  105 FSCK    None    None    8mA (output only)       GPI

/*
 * ****************************************************************************
 *      Group H Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOH0 0x38 //  93  CLKRUN# WUI16   None    16mA        Up/Dn   GPI/ID0
#define GPIOH1 0x39 //  94  CRX1    WUI17   SIN1    16mA        Up/Dn   GPI/ID1
                    //                      SMCLK3
#define GPIOH2 0x3A //  95  CTX1    WUI18   SOUT1   16mA        Up/Dn   GPI/ID2
                    //                      SMDAT3
#define GPIOH3 0x3B //  96  HSCE#   WUI19   None    16mA        Up/Dn   GPI/ID3
#define GPIOH4 0x3C //  97  HSCK    WUI40   None    16mA        Up/Dn   GPI/ID4
#define GPIOH5 0x3D //  98  HMISO   WUI41   None    16mA        Up/Dn   GPI/ID5
#define GPIOH6 0x3E //  99  HMOSI   WUI42   None    16mA        Up/Dn   GPI/ID6
#define GPIOH7 0x3F //  xx  None    None    None    None        None     None

/*
 * ****************************************************************************
 *      Group I Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOI0 0x40 //  66  ADC0    WUI71   None    2mA         None        GPI
#define GPIOI1 0x41 //  67  ADC1    WUI72   None    2mA         None        GPI
#define GPIOI2 0x42 //  68  ADC2    WUI73   None    2mA         None        GPI
#define GPIOI3 0x43 //  69  ADC3    WUI74   None    2mA         None        GPI
#define GPIOI4 0x44 //  70  ADC4    WUI28   None    2mA         None        GPI
#define GPIOI5 0x45 //  71  ADC5    WUI29   DCD1#   2mA         None        GPI
#define GPIOI6 0x46 //  72  ADC6    WUI30   DSR1#   2mA         None        GPI
#define GPIOI7 0x47 //  73  ADC7    WUI31   CTS1#   2mA         None        GPI

/*
 * ****************************************************************************
 *      Group J Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOJ0 0x48 //  76  HDIO2   WUI80   TACH2   4mA         Up/Dn       GPI
#define GPIOJ1 0x49 //  77  HDIO3   WUI81   None    4mA         Up/Dn       GPI
#define GPIOJ2 0x4A //  78  DAC2    WUI82   TACH0B  4mA         Up/Dn       GPI
#define GPIOJ3 0x4B //  79  DAC3    WUI83   TACH1B  4mA         Up/Dn       GPI
#define GPIOJ4 0x4C //  80  DAC4    WUI84   DCD0#   4mA         Up/Dn       GPI
#define GPIOJ5 0x4D //  81  DAC5    WUI85   RIG0#   4mA         Up/Dn       GPI
#define GPIOJ6 0x4E //  128 CK32K   WUI86   None    2mA         None      Func1
#define GPIOJ7 0x4F //  2   CK32KE  None    None    2mA         None      Func1

/*
 * ****************************************************************************
 *      Group M Index   Pin Func1   Func2   Func3   Output      Pull_Cap    Def
 *                                                              Driving
 * ****************************************************************************
 */
#define GPIOM0 0x50 //  10  LAD0    None    None    12mA        Up/Dn     Func1
#define GPIOM1 0x51 //  9   LAD1    None    None    12mA        Up/Dn     Func1
#define GPIOM2 0x52 //  8   LAD2    None    None    12mA        Up/Dn     Func1
#define GPIOM3 0x53 //  7   LAD3    None    None    12mA        Up/Dn     Func1
#define GPIOM4 0x54 //  13  LPCCLK  None    None    2mA         Up/Dn     Func1
#define GPIOM5 0x55 //  6   LFRAME# None    None    2mA         Up/Dn     Func1
#define GPIOM6 0x56 //  5   SERIRQ  None    None    16mA        Up/Dn     Func1
#define GPIOM7 0x57 //  xx  None    None    None    None        None       None

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void GPIO_Operation_Mode(BYTE pin_index, BYTE pin_mode,
    BYTE pin_output_type);
extern void GPIO_Output_Ctrl(BYTE pin_index, BYTE pin_output_mode);
extern BYTE GPIO_Input_Status_Get(BYTE pin_index);
extern BYTE GPIO_Mirror_Status_Get(BYTE pin_index);

/* 
 * ****************************************************************************
 * GPIO configuration registers struct
 * ****************************************************************************
 */
typedef struct GPIOConfReg
{
    CBYTE    GPIO_Index;
	uchar_8	*GPIO_DataReg;
    uchar_8	*GPIO_CtrlReg;	
    uchar_8	*GPIO_MirrorReg;
    uchar_8	*GPIO_OutputTypeReg;
    CBYTE    GPIO_Ctrl_BIT;
} sGPIOConfReg;

#endif

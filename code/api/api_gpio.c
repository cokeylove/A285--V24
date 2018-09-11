/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_gpio.c
 * Dino Li
 * Version, 1.00
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
 * GPIO Configuration Table
 * ****************************************************************************
 */
const sGPIOConfReg asGPIOConfReg[] = 
{
    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group A */
 	{ GPIOA0,   &GPDRA,     &GPCRA0,	    &GPDMRA,    &GPOTA,	    BIT0    },
    { GPIOA1,   &GPDRA,     &GPCRA1,	    &GPDMRA,    &GPOTA,	    BIT1    },
    { GPIOA2,   &GPDRA,     &GPCRA2,	    &GPDMRA,    &GPOTA,	    BIT2    },
    { GPIOA3,   &GPDRA,     &GPCRA3,	    &GPDMRA,    &GPOTA,	    BIT3    },
    { GPIOA4,   &GPDRA,     &GPCRA4,        &GPDMRA,    NoCtrlReg,  BIT4    },
    { GPIOA5,   &GPDRA,     &GPCRA5,	    &GPDMRA,    NoCtrlReg,  BIT5    },
    { GPIOA6,   &GPDRA,     &GPCRA6,	    &GPDMRA,    NoCtrlReg,  BIT6    },
    { GPIOA7,   &GPDRA,     &GPCRA7,	    &GPDMRA,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group B */
 	{ GPIOB0,   &GPDRB,     &GPCRB0,	    &GPDMRB,    &GPOTB,	    BIT0    },
    { GPIOB1,   &GPDRB,     &GPCRB1,	    &GPDMRB,    &GPOTB,	    BIT1    },
    { GPIOB2,   &GPDRB,     &GPCRB2,	    &GPDMRB,    &GPOTB,	    BIT2    },
    { GPIOB3,   &GPDRB,     &GPCRB3,	    &GPDMRB,    &GPOTB,	    BIT3    },
    { GPIOB4,   &GPDRB,     &GPCRB4,        &GPDMRB,    &GPOTB,	    BIT4    },
    { GPIOB5,   &GPDRB,     &GPCRB5,	    &GPDMRB,    &GPOTB,	    BIT5    },
    { GPIOB6,   &GPDRB,     &GPCRB6,	    &GPDMRB,    &GPOTB,	    BIT6    },
    { GPIOB7,   &GPDRB,     &GPCRB7,	    &GPDMRB,    &GPOTB,	    BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group C */
 	{ GPIOC0,   &GPDRC,     &GPCRC0,	    &GPDMRC,    NoCtrlReg,  BIT0    },
    { GPIOC1,   &GPDRC,     &GPCRC1,	    &GPDMRC,    NoCtrlReg,  BIT1    },
    { GPIOC2,   &GPDRC,     &GPCRC2,	    &GPDMRC,    NoCtrlReg,  BIT2    },
    { GPIOC3,   &GPDRC,     &GPCRC3,	    &GPDMRC,    NoCtrlReg,  BIT3    },
    { GPIOC4,   &GPDRC,     &GPCRC4,        &GPDMRC,    NoCtrlReg,  BIT4    },
    { GPIOC5,   &GPDRC,     &GPCRC5,	    &GPDMRC,    NoCtrlReg,  BIT5    },
    { GPIOC6,   &GPDRC,     &GPCRC6,	    &GPDMRC,    NoCtrlReg,  BIT6    },
    { GPIOC7,   &GPDRC,     &GPCRC7,	    &GPDMRC,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group D */
 	{ GPIOD0,   &GPDRD,     &GPCRD0,	    &GPDMRD,    &GPOTD,     BIT0    },
    { GPIOD1,   &GPDRD,     &GPCRD1,	    &GPDMRD,    &GPOTD,     BIT1    },
    { GPIOD2,   &GPDRD,     &GPCRD2,	    &GPDMRD,    &GPOTD,     BIT2    },
    { GPIOD3,   &GPDRD,     &GPCRD3,	    &GPDMRD,    &GPOTD,     BIT3    },
    { GPIOD4,   &GPDRD,     &GPCRD4,        &GPDMRD,    &GPOTD,     BIT4    },
    { GPIOD5,   &GPDRD,     &GPCRD5,	    &GPDMRD,    &GPOTD,     BIT5    },
    { GPIOD6,   &GPDRD,     &GPCRD6,	    &GPDMRD,    &GPOTD,     BIT6    },
    { GPIOD7,   &GPDRD,     &GPCRD7,	    &GPDMRD,    &GPOTD,     BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group E */
 	{ GPIOE0,   &GPDRE,     &GPCRE0,	    &GPDMRE,    &GPOTE,     BIT0    },
    { GPIOE1,   &GPDRE,     &GPCRE1,	    &GPDMRE,    &GPOTE,     BIT1    },
    { GPIOE2,   &GPDRE,     &GPCRE2,	    &GPDMRE,    &GPOTE,     BIT2    },
    { GPIOE3,   &GPDRE,     &GPCRE3,	    &GPDMRE,    &GPOTE,     BIT3    },
    { GPIOE4,   &GPDRE,     &GPCRE4,        &GPDMRE,    &GPOTE,     BIT4    },
    { GPIOE5,   &GPDRE,     &GPCRE5,	    &GPDMRE,    &GPOTE,     BIT5    },
    { GPIOE6,   &GPDRE,     &GPCRE6,	    &GPDMRE,    &GPOTE,     BIT6    },
    { GPIOE7,   &GPDRE,     &GPCRE7,	    &GPDMRE,    &GPOTE,     BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group F */
 	{ GPIOF0,   &GPDRF,     &GPCRF0,	    &GPDMRF,    &GPOTF,     BIT0    },
    { GPIOF1,   &GPDRF,     &GPCRF1,	    &GPDMRF,    &GPOTF,     BIT1    },
    { GPIOF2,   &GPDRF,     &GPCRF2,	    &GPDMRF,    &GPOTF,     BIT2    },
    { GPIOF3,   &GPDRF,     &GPCRF3,	    &GPDMRF,    &GPOTF,     BIT3    },
    { GPIOF4,   &GPDRF,     &GPCRF4,        &GPDMRF,    &GPOTF,     BIT4    },
    { GPIOF5,   &GPDRF,     &GPCRF5,	    &GPDMRF,    &GPOTF,     BIT5    },
    { GPIOF6,   &GPDRF,     &GPCRF6,	    &GPDMRF,    &GPOTF,     BIT6    },
    { GPIOF7,   &GPDRF,     &GPCRF7,	    &GPDMRF,    &GPOTF,     BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group G */
 	{ GPIOG0,   &GPDRG,     &GPCRG0,	    &GPDMRG,    NoCtrlReg,  BIT0    },
    { GPIOG1,   &GPDRG,     &GPCRG1,	    &GPDMRG,    NoCtrlReg,  BIT1    },
    { GPIOG2,   &GPDRG,     &GPCRG2,	    &GPDMRG,    NoCtrlReg,  BIT2    },
    { GPIOG3,   &GPDRG,     &GPCRG3,	    &GPDMRG,    NoCtrlReg,  BIT3    },
    { GPIOG4,   &GPDRG,     &GPCRG4,        &GPDMRG,    NoCtrlReg,  BIT4    },
    { GPIOG5,   &GPDRG,     &GPCRG5,	    &GPDMRG,    NoCtrlReg,  BIT5    },
    { GPIOG6,   &GPDRG,     &GPCRG6,	    &GPDMRG,    NoCtrlReg,  BIT6    },
    { GPIOG7,   &GPDRG,     &GPCRG7,	    &GPDMRG,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group H */
 	{ GPIOH0,   &GPDRH,     &GPCRH0,	    &GPDMRH,    &GPOTH,     BIT0    },
    { GPIOH1,   &GPDRH,     &GPCRH1,	    &GPDMRH,    &GPOTH,     BIT1    },
    { GPIOH2,   &GPDRH,     &GPCRH2,	    &GPDMRH,    &GPOTH,     BIT2    },
    { GPIOH3,   &GPDRH,     &GPCRH3,	    &GPDMRH,    &GPOTH,     BIT3    },
    { GPIOH4,   &GPDRH,     &GPCRH4,        &GPDMRH,    &GPOTH,     BIT4    },
    { GPIOH5,   &GPDRH,     &GPCRH5,	    &GPDMRH,    &GPOTH,     BIT5    },
    { GPIOH6,   &GPDRH,     &GPCRH6,	    &GPDMRH,    &GPOTH,     BIT6    },
    { GPIOH7,   &GPDRH,     NoCtrlReg,      &GPDMRH,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group I */
 	{ GPIOI0,   &GPDRI,     &GPCRI0,	    &GPDMRI,    NoCtrlReg,  BIT0    },
    { GPIOI1,   &GPDRI,     &GPCRI1,	    &GPDMRI,    NoCtrlReg,  BIT1    },
    { GPIOI2,   &GPDRI,     &GPCRI2,	    &GPDMRI,    NoCtrlReg,  BIT2    },
    { GPIOI3,   &GPDRI,     &GPCRI3,	    &GPDMRI,    NoCtrlReg,  BIT3    },
    { GPIOI4,   &GPDRI,     &GPCRI4,        &GPDMRI,    NoCtrlReg,  BIT4    },
    { GPIOI5,   &GPDRI,     &GPCRI5,	    &GPDMRI,    NoCtrlReg,  BIT5    },
    { GPIOI6,   &GPDRI,     &GPCRI6,	    &GPDMRI,    NoCtrlReg,  BIT6    },
    { GPIOI7,   &GPDRI,     &GPCRI7,	    &GPDMRI,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group J */
 	{ GPIOJ0,   &GPDRJ,     &GPCRJ0,	    &GPDMRJ,    &GPOTJ,     BIT0    },
    { GPIOJ1,   &GPDRJ,     &GPCRJ1,	    &GPDMRJ,    &GPOTJ,     BIT1    },
    { GPIOJ2,   &GPDRJ,     &GPCRJ2,	    &GPDMRJ,    &GPOTJ,     BIT2    },
    { GPIOJ3,   &GPDRJ,     &GPCRJ3,	    &GPDMRJ,    &GPOTJ,     BIT3    },
    { GPIOJ4,   &GPDRJ,     &GPCRJ4,        &GPDMRJ,    &GPOTJ,     BIT4    },
    { GPIOJ5,   &GPDRJ,     &GPCRJ5,	    &GPDMRJ,    &GPOTJ,     BIT5    },
    { GPIOJ6,   &GPDRJ,     &GPCRJ6,	    &GPDMRJ,    NoCtrlReg,  BIT6    },
    { GPIOJ7,   &GPDRJ,     &GPCRJ7,	    &GPDMRJ,    NoCtrlReg,  BIT7    },

    /* Index,   Data Reg,   Control Reg,    Mirror Reg, Output Type BIT[x] */
    /* Group M */
 	{ GPIOM0,   &GPDRM,     &GPCRM0,	    &GPDMRM,    NoCtrlReg,   BIT0   },
    { GPIOM1,   &GPDRM,     &GPCRM1,	    &GPDMRM,    NoCtrlReg,   BIT1   },
    { GPIOM2,   &GPDRM,     &GPCRM2,	    &GPDMRM,    NoCtrlReg,   BIT2   },
    { GPIOM3,   &GPDRM,     &GPCRM3,	    &GPDMRM,    NoCtrlReg,   BIT3   },
    { GPIOM4,   &GPDRM,     &GPCRM4,        &GPDMRM,    NoCtrlReg,   BIT4   },
    { GPIOM5,   &GPDRM,     &GPCRM5,	    &GPDMRM,    NoCtrlReg,   BIT5   },
    { GPIOM6,   &GPDRM,     &GPCRM6,	    &GPDMRM,    NoCtrlReg,   BIT6   },
    { GPIOM7,   &GPDRM,     NoCtrlReg,	    &GPDMRM,    NoCtrlReg,   BIT7   },
};

/**
 * ****************************************************************************
 * The function for selecting GPIO operation mode.
 *
 * @return
 *
 * @parameter
 * pin_index,
 * GPIOA0 || GPIOA1 || GPIOA2 || GPIOA3 || GPIOA4 || GPIOA5 || GPIOA6 || GPIOA7
 * GPIOB0 || GPIOB1 || GPIOB2 || GPIOB3 || GPIOB4 || GPIOB5 || GPIOB6 || GPIOB7
 * GPIOC0 || GPIOC1 || GPIOC2 || GPIOC3 || GPIOC4 || GPIOC5 || GPIOC6 || GPIOC7
 * GPIOD0 || GPIOD1 || GPIOD2 || GPIOD3 || GPIOD4 || GPIOD5 || GPIOD6 || GPIOD7
 * GPIOE0 || GPIOE1 || GPIOE2 || GPIOE3 || GPIOE4 || GPIOE5 || GPIOE6 || GPIOE7
 * GPIOF0 || GPIOF1 || GPIOF2 || GPIOF3 || GPIOF4 || GPIOF5 || GPIOF6 || GPIOF7
 * GPIOG0 || GPIOG1 || GPIOG2 || GPIOG3 || GPIOG4 || GPIOG5 || GPIOG6 || GPIOG7
 * GPIOH0 || GPIOH1 || GPIOH2 || GPIOH3 || GPIOH4 || GPIOH5 || GPIOH6 || GPIOH7
 * GPIOI0 || GPIOI1 || GPIOI2 || GPIOI3 || GPIOI4 || GPIOI5 || GPIOI6 || GPIOI7
 * GPIOJ0 || GPIOJ1 || GPIOJ2 || GPIOJ3 || GPIOJ4 || GPIOJ5 || GPIOJ6 || GPIOJ7
 * GPIOM0 || GPIOM1 || GPIOM2 || GPIOM3 || GPIOM4 || GPIOM5 || GPIOM6 || GPIOM7
 * pin_mode, [ ALT || INPUT || OUTPUT ] + [ PULL_UP || PULL_DW ]
 * pin_output_type,
 * OutputType_None || OutputType_Push_Pull || OutputType_Open_Drain
 *
 * ****************************************************************************
 */
void GPIO_Operation_Mode(BYTE pin_index, BYTE pin_mode, BYTE pin_output_type)
{
    /* To set GPIO operation mode. */
    *asGPIOConfReg[pin_index].GPIO_CtrlReg = pin_mode;

    /* If GPIO operation mode is output. */
    if((pin_mode & OUTPUT) == OUTPUT)
    {
        /* Open-drain output */
        if(pin_output_type == OutputType_Open_Drain)
        {
            *asGPIOConfReg[pin_index].GPIO_OutputTypeReg |=
                asGPIOConfReg[pin_index].GPIO_Ctrl_BIT;
        }
        /* Push-pull output */
        else if(pin_output_type == OutputType_Push_Pull)
        {
            *asGPIOConfReg[pin_index].GPIO_OutputTypeReg &=
                ~asGPIOConfReg[pin_index].GPIO_Ctrl_BIT;
        }
        /* No output type control register */
        else
        {
            /* Do nothing */
        }
    }
}

/**
 * ****************************************************************************
 * The function for GPIO output control.
 *
 * @return
 *
 * @parameter
 * pin_index,
 * GPIOA0 || GPIOA1 || GPIOA2 || GPIOA3 || GPIOA4 || GPIOA5 || GPIOA6 || GPIOA7
 * GPIOB0 || GPIOB1 || GPIOB2 || GPIOB3 || GPIOB4 || GPIOB5 || GPIOB6 || GPIOB7
 * GPIOC0 || GPIOC1 || GPIOC2 || GPIOC3 || GPIOC4 || GPIOC5 || GPIOC6 || GPIOC7
 * GPIOD0 || GPIOD1 || GPIOD2 || GPIOD3 || GPIOD4 || GPIOD5 || GPIOD6 || GPIOD7
 * GPIOE0 || GPIOE1 || GPIOE2 || GPIOE3 || GPIOE4 || GPIOE5 || GPIOE6 || GPIOE7
 * GPIOF0 || GPIOF1 || GPIOF2 || GPIOF3 || GPIOF4 || GPIOF5 || GPIOF6 || GPIOF7
 * GPIOG0 || GPIOG1 || GPIOG2 || GPIOG3 || GPIOG4 || GPIOG5 || GPIOG6 || GPIOG7
 * GPIOH0 || GPIOH1 || GPIOH2 || GPIOH3 || GPIOH4 || GPIOH5 || GPIOH6 || GPIOH7
 * GPIOI0 || GPIOI1 || GPIOI2 || GPIOI3 || GPIOI4 || GPIOI5 || GPIOI6 || GPIOI7
 * GPIOJ0 || GPIOJ1 || GPIOJ2 || GPIOJ3 || GPIOJ4 || GPIOJ5 || GPIOJ6 || GPIOJ7
 * GPIOM0 || GPIOM1 || GPIOM2 || GPIOM3 || GPIOM4 || GPIOM5 || GPIOM6 || GPIOM7
 * pin_output_mode,
 * OutputMode_Inverse || OutputMode_High || OutputMode_Low
 *
 * ****************************************************************************
 */
void GPIO_Output_Ctrl(BYTE pin_index, BYTE pin_output_mode)
{
    /* Output high */
    if(pin_output_mode == OutputMode_High)
    {
        *asGPIOConfReg[pin_index].GPIO_DataReg |=
            asGPIOConfReg[pin_index].GPIO_Ctrl_BIT;
    }
    /* Output toggle */
    else if(pin_output_mode == OutputMode_Inverse)
    {
         *asGPIOConfReg[pin_index].GPIO_DataReg ^=
            asGPIOConfReg[pin_index].GPIO_Ctrl_BIT;
    }
    /* Output low */
    else
    {
        *asGPIOConfReg[pin_index].GPIO_DataReg &=
            ~asGPIOConfReg[pin_index].GPIO_Ctrl_BIT;
    }
}

/**
 * ****************************************************************************
 * The function for reading GPIO pin status.
 *
 * @return,
 * InputMode_High : high
 * InputMode_Low : low
 *
 * @parameter
 * pin_index,
 * GPIOA0 || GPIOA1 || GPIOA2 || GPIOA3 || GPIOA4 || GPIOA5 || GPIOA6 || GPIOA7
 * GPIOB0 || GPIOB1 || GPIOB2 || GPIOB3 || GPIOB4 || GPIOB5 || GPIOB6 || GPIOB7
 * GPIOC0 || GPIOC1 || GPIOC2 || GPIOC3 || GPIOC4 || GPIOC5 || GPIOC6 || GPIOC7
 * GPIOD0 || GPIOD1 || GPIOD2 || GPIOD3 || GPIOD4 || GPIOD5 || GPIOD6 || GPIOD7
 * GPIOE0 || GPIOE1 || GPIOE2 || GPIOE3 || GPIOE4 || GPIOE5 || GPIOE6 || GPIOE7
 * GPIOF0 || GPIOF1 || GPIOF2 || GPIOF3 || GPIOF4 || GPIOF5 || GPIOF6 || GPIOF7
 * GPIOG0 || GPIOG1 || GPIOG2 || GPIOG3 || GPIOG4 || GPIOG5 || GPIOG6 || GPIOG7
 * GPIOH0 || GPIOH1 || GPIOH2 || GPIOH3 || GPIOH4 || GPIOH5 || GPIOH6 || GPIOH7
 * GPIOI0 || GPIOI1 || GPIOI2 || GPIOI3 || GPIOI4 || GPIOI5 || GPIOI6 || GPIOI7
 * GPIOJ0 || GPIOJ1 || GPIOJ2 || GPIOJ3 || GPIOJ4 || GPIOJ5 || GPIOJ6 || GPIOJ7
 * GPIOM0 || GPIOM1 || GPIOM2 || GPIOM3 || GPIOM4 || GPIOM5 || GPIOM6 || GPIOM7
 *
 * ****************************************************************************
 */
BYTE GPIO_Input_Status_Get(BYTE pin_index)
{
    if(IS_MASK_SET(*asGPIOConfReg[pin_index].GPIO_DataReg,
        asGPIOConfReg[pin_index].GPIO_Ctrl_BIT))
    {
        return(InputMode_High);
    }
    else
    {
        return(InputMode_Low);
    }
}

/**
 * ****************************************************************************
 * The function for reading GPIO port data mirror register.
 *
 * @return,
 * Mirror_High : high
 * Mirror_Low : low
 *
 * @parameter
 * pin_index,
 * GPIOA0 || GPIOA1 || GPIOA2 || GPIOA3 || GPIOA4 || GPIOA5 || GPIOA6 || GPIOA7
 * GPIOB0 || GPIOB1 || GPIOB2 || GPIOB3 || GPIOB4 || GPIOB5 || GPIOB6 || GPIOB7
 * GPIOC0 || GPIOC1 || GPIOC2 || GPIOC3 || GPIOC4 || GPIOC5 || GPIOC6 || GPIOC7
 * GPIOD0 || GPIOD1 || GPIOD2 || GPIOD3 || GPIOD4 || GPIOD5 || GPIOD6 || GPIOD7
 * GPIOE0 || GPIOE1 || GPIOE2 || GPIOE3 || GPIOE4 || GPIOE5 || GPIOE6 || GPIOE7
 * GPIOF0 || GPIOF1 || GPIOF2 || GPIOF3 || GPIOF4 || GPIOF5 || GPIOF6 || GPIOF7
 * GPIOG0 || GPIOG1 || GPIOG2 || GPIOG3 || GPIOG4 || GPIOG5 || GPIOG6 || GPIOG7
 * GPIOH0 || GPIOH1 || GPIOH2 || GPIOH3 || GPIOH4 || GPIOH5 || GPIOH6 || GPIOH7
 * GPIOI0 || GPIOI1 || GPIOI2 || GPIOI3 || GPIOI4 || GPIOI5 || GPIOI6 || GPIOI7
 * GPIOJ0 || GPIOJ1 || GPIOJ2 || GPIOJ3 || GPIOJ4 || GPIOJ5 || GPIOJ6 || GPIOJ7
 * GPIOM0 || GPIOM1 || GPIOM2 || GPIOM3 || GPIOM4 || GPIOM5 || GPIOM6 || GPIOM7
 *
 * ****************************************************************************
 */
BYTE GPIO_Mirror_Status_Get(BYTE pin_index)
{
    if(IS_MASK_SET(*asGPIOConfReg[pin_index].GPIO_MirrorReg,
        asGPIOConfReg[pin_index].GPIO_Ctrl_BIT))
    {
        return(Mirror_High);
    }
    else
    {
        return(Mirror_Low);
    }
}


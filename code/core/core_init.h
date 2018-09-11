/* 
 * ****************************************************************************
 *
 *  core_init.h
 *
 *  Copyright (c) 2012- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/11/21
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */
 
#ifndef CORE_INIT_H
#define CORE_INIT_H

/*
 * ****************************************************************************
 * definition
 * Base address select 0x00:2E/2F 0x01:4E/4F 0x02:User define  
 * ****************************************************************************
 */
/*
 * Base address select 0b00:2E 2F 0b01:4E 4F
 * 0b10:User define SWCBALR | SWCBAHR
 */
#define Core_Init_BADR          0x01
/* I/O Port High Byte */
#define Core_Init_SWCBAHR       0x00
/* I/O Port Low  Byte */
#define Core_Init_SWCBALR       0x4E
/* Enable I2EC R/W and log 80 port */
#define Core_Init_I2EC          0x43

#define LED_CTRL_INIT 		(MASK(SCROLL) | MASK(NUM) | MASK(CAPS) | MASK(OVL))
#define SAVE_KBD_STATE_INIT (maskAUXENAB + (2 << shiftCODESET))
#define CCB42_INIT			DISAB_KEY
#define EXT_CB2_INIT    	0x22		
#define SAVE_TYPEMATIC_INIT 0x2A

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Core_Initialization(void);
extern void Core_Init_Regs(void);

/* 
 * ****************************************************************************
 * structure 
 * ****************************************************************************
 */
typedef struct REG_INIT
{
    uchar_8 	*address;
    uchar_8  	initdata;
} sREG_INIT;

#endif


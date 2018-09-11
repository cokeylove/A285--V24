/*
 * ****************************************************************************
 *
 *  core_gpio.h
 *
 *  Copyright (c) 2012- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/11/21
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */

#ifndef CORE_GPIO_H
#define CORE_GPIO_H

/*
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define ALT       		0x00
#define INPUT        	0x80
#define OUTPUT     		0x40
#define PULL_UP       	0x04
#define PULL_DW        	0x02
#define MaxD    		0x38

#define NullPin         INPUT

/*
 * ****************************************************************************
 * Function prototype
 * ****************************************************************************
 */
extern void gpio_fw_default_voltage_selection(void);

#endif


/*
 * ****************************************************************************
 *
 *  core_gpio.c
 *
 *  Copyright (c) 2012- ITE INC. All Rights Reserved.
 *
 *  Created on: 2012/11/21
 *
 *      Author: Dino
 *
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/**
 * ****************************************************************************
 * GPIO voltage selection
 *
 * @return
 *
 * @parameter
 *
 * @note, Do not enable internal pull-up if voltage selection is 1.8v
 *
 * ****************************************************************************
 */
void gpio_fw_default_voltage_selection(void)
{
    /* GCR19 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPB5_1P8V__
    SET_MASK(GCR19, GPB5VS);
    #else
    CLEAR_MASK(GCR19, GPB5VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPB6_1P8V__
    SET_MASK(GCR19, GPB6VS);
    #else
    CLEAR_MASK(GCR19, GPB6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC1_1P8V__
    SET_MASK(GCR19, GPC1VS);
    #else
    CLEAR_MASK(GCR19, GPC1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC2_1P8V__
    SET_MASK(GCR19, GPC2VS);
    #else
    CLEAR_MASK(GCR19, GPC2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPC7_1P8V__
    SET_MASK(GCR19, GPC7VS);
    #else
    CLEAR_MASK(GCR19, GPC7VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD0_1P8V__
    SET_MASK(GCR19, GPD0VS);
    #else
    CLEAR_MASK(GCR19, GPD0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD1_1P8V__
    SET_MASK(GCR19, GPD1VS);
    #else
    CLEAR_MASK(GCR19, GPD1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD2_1P8V__
    SET_MASK(GCR19, GPD2VS);
    #else
    CLEAR_MASK(GCR19, GPD2VS);
    #endif

    /* GCR20 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPD3_1P8V__
    SET_MASK(GCR20, GPD3VS);
    #else
    CLEAR_MASK(GCR20, GPD3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPD4_1P8V__
    SET_MASK(GCR20, GPD4VS);
    #else
    CLEAR_MASK(GCR20, GPD4VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE0_1P8V__
    SET_MASK(GCR20, GPE0VS);
    #else
    CLEAR_MASK(GCR20, GPE0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE6_1P8V__
    SET_MASK(GCR20, GPE6VS);
    #else
    CLEAR_MASK(GCR20, GPE6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPE7_1P8V__
    SET_MASK(GCR20, GPE7VS);
    #else
    CLEAR_MASK(GCR20, GPE7VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF2_1P8V__
    SET_MASK(GCR20, GPF2VS);
    #else
    CLEAR_MASK(GCR20, GPF2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF3_1P8V__
    SET_MASK(GCR20, GPF3VS);
    #else
    CLEAR_MASK(GCR20, GPF3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF4_1P8V__
    SET_MASK(GCR20, GPF4VS);
    #else
    CLEAR_MASK(GCR20, GPF4VS);
    #endif

    /* GCR21 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPF5_1P8V__
    SET_MASK(GCR21, GPF5VS);
    #else
    CLEAR_MASK(GCR21, GPF5VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF6_1P8V__
    SET_MASK(GCR21, GPF6VS);
    #else
    CLEAR_MASK(GCR21, GPF6VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPF7_1P8V__
    SET_MASK(GCR21, GPF7VS);
    #else
    CLEAR_MASK(GCR21, GPF7VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH0_1P8V__
    SET_MASK(GCR21, GPH0VS);
    #else
    CLEAR_MASK(GCR21, GPH0VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH1_1P8V__
    SET_MASK(GCR21, GPH1VS);
    #else
    CLEAR_MASK(GCR21, GPH1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPH2_1P8V__
    SET_MASK(GCR21, GPH2VS);
    #else
    CLEAR_MASK(GCR21, GPH2VS);
    #endif

    /* GCR22 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPB3_1P8V__
    SET_MASK(GCR22, GPB3VS);
    #else
    CLEAR_MASK(GCR22, GPB3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPB4_1P8V__
    SET_MASK(GCR22, GPB4VS);
    #else
    CLEAR_MASK(GCR22, GPB4VS);
    #endif

    /* GCR23 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPI4_1P8V__
    SET_MASK(GCR23, GPI4VS);
    #else
    CLEAR_MASK(GCR23, GPI4VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPI3_1P8V__
    SET_MASK(GCR23, GPI3VS);
    #else
    CLEAR_MASK(GCR23, GPI3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPI2_1P8V__
    SET_MASK(GCR23, GPI2VS);
    #else
    CLEAR_MASK(GCR23, GPI2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPI1_1P8V__
    SET_MASK(GCR23, GPI1VS);
    #else
    CLEAR_MASK(GCR23, GPI1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPJ3_1P8V__
    SET_MASK(GCR23, GPJ3VS);
    #else
    CLEAR_MASK(GCR23, GPJ3VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPJ2_1P8V__
    SET_MASK(GCR23, GPJ2VS);
    #else
    CLEAR_MASK(GCR23, GPJ2VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPJ1_1P8V__
    SET_MASK(GCR23, GPJ1VS);
    #else
    CLEAR_MASK(GCR23, GPJ1VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPJ0_1P8V__
    SET_MASK(GCR23, GPJ0VS);
    #else
    CLEAR_MASK(GCR23, GPJ0VS);
    #endif

    /* GCR24 */
    #ifdef __DEF_VOLTAGE_SELECTION_GPA5_1P8V__
    SET_MASK(GCR24, GPA5VS);
    #else
    CLEAR_MASK(GCR24, GPA5VS);
    #endif

    #ifdef __DEF_VOLTAGE_SELECTION_GPA4_1P8V__
    SET_MASK(GCR24, GPA4VS);
    #else
    CLEAR_MASK(GCR24, GPA4VS);
    #endif
}


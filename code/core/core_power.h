/* 
 * ****************************************************************************
 *
 *  core_power.h
 *
 *  Copyright (c) 2013-
 *
 *  Created on: 2013/01/10
 *
 *      Author: 
 *
 * ****************************************************************************
 */

#ifndef CORE_POWER_H
#define CORE_POWER_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
enum TPOWER_M_TYPE
{
    tpmDoze,
    tpmDeepDoze,
    tpmSleep,
    tpmMaxNo,
};

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void CheckResetSource(void);
extern void PowerM_Doze(void);
extern void PowerM_DeepDoze(void);
extern void PowerM_Sleep(void);

#endif


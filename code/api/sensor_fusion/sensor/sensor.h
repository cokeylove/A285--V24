#ifndef _SENSOR_H_ // {
#define _SENSOR_H_

#include "./accelerometer/_acc.h"
#include "./accelerometer2/_acc2.h"

//#include "./compass/_mag.h"
//#include "./gyrometer/_gyo.h"

#ifdef __ENABLE_AKM_PG_LIB__ // {
#include "./compass/AK09911_AKMApi_PG/_mag.h"
#include "./gyrometer/Null/_gyo.h"
#elif defined(__ENABLE_AKM2_LIB__)
#include "./compass/AK09911_AKMApi/_mag.h"
#include "./gyrometer/_gyo.h"
#else
#include "./compass/_mag.h"
//	#include "./gyrometer/_gyo.h"
#include "./gyrometer/_gyo.h"
#endif // } __ENABLE_AKM_PG_LIB__

#include "./ambientlight/_als.h"
#include "./pressure/_prs.h" 
#include "./hygrometer/_hyg.h"
#include "./temperature/_temp.h"
#include "./proximity/_prox.h"
#include "./ultraviolet/_uv.h"
#endif // } _SENSOR_H_
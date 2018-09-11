#include "sensor.h"

/*-------------------------------------------------------------------------
    Accelerometer
-------------------------------------------------------------------------*/
#include "./accelerometer/_acc.c"    

/*-------------------------------------------------------------------------
    Accelerometer2
-------------------------------------------------------------------------*/
#include "./accelerometer2/_acc2.c"

/*-------------------------------------------------------------------------
    Compass
-------------------------------------------------------------------------*/         
// #include "./compass/_mag.c"            
#ifdef __ENABLE_AKM_PG_LIB__
#include "./compass/AK09911_AKMApi_PG/_mag.c"
#elif defined(__ENABLE_AKM2_LIB__)
#include "./compass/AK09911_AKMApi/_mag.c"
#else
#include "./compass/_mag.c"
#endif

/*-------------------------------------------------------------------------
    Gyrometer
-------------------------------------------------------------------------*/        
//#include "./gyrometer/_gyo.c"          

#ifdef __ENABLE_AKM_PG_LIB__ // {
#include "./gyrometer/Null/_gyo.c"    
#else
//	#include "./gyrometer/_gyo.c" 
#include "./gyrometer/_gyo.c"
#endif // } __ENABLE_AKM_PG_LIB__


/*-------------------------------------------------------------------------
    Ambientlight
-------------------------------------------------------------------------*/
#include "./ambientlight/_als.c" 

/*-------------------------------------------------------------------------
     Pressure
-------------------------------------------------------------------------*/
#include "./pressure/_prs.c" 

/*-------------------------------------------------------------------------
     Hygrometer
-------------------------------------------------------------------------*/
#include "./hygrometer/_hyg.c" 

/*-------------------------------------------------------------------------
     Temperature
-------------------------------------------------------------------------*/
#include "./temperature/_temp.c" 

/*-------------------------------------------------------------------------
    Proximity
-------------------------------------------------------------------------*/
#include "./proximity/_prox.c"

/*-------------------------------------------------------------------------
    Ultraviolet
-------------------------------------------------------------------------*/
#include "./ultraviolet/_uv.c"


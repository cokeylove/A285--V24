#ifndef ORIGA_DATATYPES_H
#define ORIGA_DATATYPES_H
#define KEIL_ARM

#include "stdlib.h"
//#include "OCTYPES.H"



/* ****************************************************************************
   *** v0.90; 2008-11-10: 1st version.
   *** v0.93; 2009-03-30: check of TRUE and FALSE definition added.
   ***        2009-03-30: compile switch for KEIL and TASKING added.
   *** v1.00; 2011-07-09: KEIL_ARM definition for SRM9 system added.
   *** v1.01; 2011-07-21: const definitions added.
   *** v1.02; 2011-07-21: const definitions added.
   ************************************************************************* */



#ifdef TASKING
typedef __bit BIT;
#elif defined KEIL
typedef bit BIT;
#elif defined KEIL_ARM
#ifdef STM_STR912
#include <91x_map.h>
#endif
typedef unsigned char BIT;
#else
#error "Add compiler specific bit definition!"
#endif



//#ifndef BYTE
//typedef signed char    BYTE;
//#endif
#ifndef UBYTE
typedef unsigned char  UBYTE;
#endif
#ifndef UBYTE_MAX
#define UBYTE_MAX (0xFFu)
#endif

#ifndef UWORD
typedef unsigned short UWORD;
#endif
#ifndef SWORD
typedef signed short   SWORD;
#endif
#ifndef UWORD_MAX
#define UWORD_MAX (0xFFFFu)
#endif

#ifndef ULONG
typedef unsigned long  ULONG;
#endif
#ifndef LONG
typedef signed long    LONG;
#endif
#ifndef ULONG_MAX
#define ULONG_MAX (0xFFFFFFFFu)
#endif


//#ifndef BOOL
//typedef UBYTE BOOL;
//#endif

#ifndef gf2n_t
typedef UWORD gf2n_t[9];
#endif



#ifndef TRUE
#define TRUE    (1u)
#endif
#ifndef FALSE
#define FALSE   (0u)
#endif
#ifndef NULL
#define NULL    (0u)
#endif



#endif

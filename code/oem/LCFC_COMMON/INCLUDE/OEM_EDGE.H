/*-----------------------------------------------------------------------------
 * TITLE: OEM_Edge.H
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_EDGE_H
#define OEM_EDGE_H

/* __DATE__ format: Mmm DD YYYY*/
/* __TIME__ format: HH:MM:SS */

/* __TIME__ Data extracts */
#define  BUILD_SECONDS   ((__TIME__[6] - '0')*10 + (__TIME__[7] - '0'))
#define  BUILD_MINUTES   ((__TIME__[3] - '0')*10 + (__TIME__[4] - '0'))
#define  BUILD_HOURS    ((__TIME__[0] - '0')*10 + (__TIME__[1] - '0'))

/* __DATE__ Data extracts */
#define  BUILD_YEAR    ((__DATE__[9] - '0')*10 + (__DATE__[10] - '0'))
#define  BUILD_DATE    ((__DATE__[4] - '0')*10 + (__DATE__[5]  - '0'))

enum months  {JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};

/* Look at the last letter in the month to determine it */
#define  BUILD_MONTH    (__DATE__[2] == 'n' ? (__DATE__[1] == 'a' ? JAN : JUN) :\
                __DATE__[2] == 'b' ? FEB :  \
                __DATE__[2] == 'r' ? (__DATE__[0] == 'M' ? MAR : APR) :\
                __DATE__[2] == 'y' ? MAY :\
                __DATE__[2] == 'l' ? JUL :\
                __DATE__[2] == 'g' ? AUG :\
                __DATE__[2] == 'p' ? SEP :\
                __DATE__[2] == 't' ? OCT :\
                __DATE__[2] == 'v' ? NOV : DEC)

#define ACPOWER_ON ( Read_AC_IN() && !Read_ACOFF() ) //(JSN)

//  Edge Battery Definition
#define  BATMAXLVL  0x06
/* (From TPG sbcomm.c Update_Battery_Status_In_Interface_Space() )
*/
enum BATTERY_LEVEL {
    SBC_BATTERY_LEVEL_EMPTY = 0,    
    SBC_BATTERY_LEVEL_CRITICAL_LOW,
    SBC_BATTERY_LEVEL_WARNING,    
    SBC_BATTERY_LEVEL_LOW,        
    SBC_BATTERY_LEVEL_MIDDLE,     
    SBC_BATTERY_LEVEL_MIDDLE_HIGH,
    SBC_BATTERY_LEVEL_HIGH 
};

enum BM_LEVEL {
    BM_LEVEL_CRITICAL_LOW = 0,
    BM_LEVEL_LOW,        
    BM_LEVEL_MIDDLE,     
    BM_LEVEL_MIDDLE_HIGH,
    BM_MAX_LEVEL 
};
// static const BATTERY_LEVEL_TABLE Battery_Level_Table[] = {
//     { 0   ,-1 },     // empty
//     { 20  ,-1 },     // crtical low (in 10mW.h)
//     { 30  ,-1 },     // low battery warning (in 10mW.h)
//     { -1  ,5  },     // low
//     { -1  ,20 },     // middle
//     { -1  ,80 },     // middle - high
//     { -1  ,100}      // high
// };


extern void EdgePasswordProcess(void);
extern void Edge_Initial(void);
extern void PowerMonitor(void);

#endif

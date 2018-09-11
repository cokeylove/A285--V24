/*
--------------------------------------------------------------------
  Filename: OEM_DEFINE.H

  Purpose : LCFC define

  Author  : Ivan Chen 2016/03/14
--------------------------------------------------------------------
*/

#ifndef OEM_DEFINE_H
#define OEM_DEFINE_H
//------------------------------------------------------------------

//Typc-C chip
#define CCG2   1
#define CCG3   2
#define TPS65982   3
#define TPS65988	4  ////A485D00038

//charger IC chip
#define BQ24780S FALSE
#define BQ25700  TRUE

//Battery low votlage shutdown protect rule
#define LowVot_LGNotLG  0
#define LowVot_3S4S     1

//platform chip
#define INTEL           1
#define AMD             2
#define NVidia          3

//ITE Chip
#define IT8186     0x8186  //chip id
#define IT8586     0x8586  //chip id
#define IT8986     0x8986  //chip id

//Keyboard 
#define CS13KB     0    //CS13 keyboard define 
#define CS16KB     1    //CS16 keyboard define 
//Battery
#define M_BAT     0
#define S_BAT     1

#define AC_IN     0xAC
#define DC_IN     0xDC
#define Polling   0x91

#define Enable 1
#define Disable 0

//------------------------------------------------------------------
#endif  //OEM_DEFINE_H
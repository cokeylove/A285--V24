/*
********************************************************************************
* Lenovo Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LENOVO 2007 ALL RIGHTS RESERVED
*
* Filename: eccmd.h
* 
* Description:
*    declaration of correction of EC command handlers
* 
* History:
*   9/03/2007    Tetsuji Nakamura
*     created as new
********************************************************************************
*/

#ifndef OEM_ECCMD_H
   #define OEM_ECCMD_H
//   #undef EXTERN
//   #ifdef __ECCMD_S
//      #define EXTERN
//   #else
//      #define EXTERN EXTERN_C
//   #endif
/*
*********************************************************
*     #define Constants
*********************************************************
*/

/*
*********************************************************
*     Global Macros
*********************************************************
*/


/*
*********************************************************
*     Global Data Types
*********************************************************
*/

//struct _EC_MEM_MANIPULATE_DATA {
//    UCHAR cmd;
//    UCHAR addr_highest;
//    UCHAR addr_high;
//    UCHAR addr_low;
//    UCHAR data[12];
//};
//
//typedef struct _EC_MEM_MANIPULATE_DATA EC_MEM_MANIPULATE_DATA;

/*
*********************************************************
*     Global Variables
*********************************************************
*/

/*
*********************************************************
*     Externals
*********************************************************
*/


/*
*********************************************************
*     Global Function Prototypes
*********************************************************
*/

extern BYTE EcCmd(BYTE protocol, BYTE command, BYTE length, BYTE *Tempdata, BYTE *return_length);
extern void Set_Smbus_Communication_Done(BYTE length, BYTE status);
extern void Update_Smbus_Status(BYTE status);
//EXTERN void  EcCmdMemoryManipulate(EC_MEM_MANIPULATE_DATA *ec_mem);

#endif

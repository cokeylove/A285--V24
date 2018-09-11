/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOOK.C
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#include "..\..\include.h"



//
// Fan Table Struct
//
//For DA275:
//FAN_VENDOR_1(3.3v):AVC/SUNON
//FAN_VENDOR_2(1.6v):Null
//FAN_VENDOR_3(0v):DELTA

//(Updated by WV4_AMD_FAN_Control_Table_ver20_20170724.xls)

/*****************************************************************************/
//  Main FAN_VENDOR_1 FAN Control for project1.
/*****************************************************************************/
//-----------------------------------------------------------------------------

//thermal sensor table for avc performace mode -avc  //
const FanControlstruct Fan1TSFanCtlTable_OS_100_Project1[] = 
{
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {47,   41,    43,     30},  // 1
    {50,   44,    47,     58},  // 2
    {53,   47,    51,     62},  // 3
    {55,   50,    55,     70},  // 4
    {68,   53,    59,     80},  // 5
};


//cpu table for performance table -avc
const FanControlstruct Fan1CPUFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {70,   00,    00,     00},  // 0
    {87,   65,    43,     30},  // 1
    {90,   75,    51,     50},  // 2
    {95,   81,    59,     60},  // 3
    {99,   85,    59,     60}   // 4	//A485D00072+
   
};

const FanControlstruct Fan1VGAFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},  // 0
    {70,   50,    28,     52},  // 1
    {83,   56,    32,     64},  // 2
    {85,   70,    35,     74}   // 3
};

const FanControlstruct Fan1CPUFanCtlTable_DOS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {54,   45,    43,     30},  // 1
    {60,   50,    47,     58},  // 2
    {68,   55,    51,     62},  // 3
    {90,   65,    59,     70},  // 4
    {95,   75,    59,     80},  // 5
    {99,   85,    65,     100}  // 6
};
/*****************************************************************************/
//  Main FAN_VENDOR_1 THR Control for project1. performance mode
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct Fan1CPUTHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {85,    99,    76,        85,          0,       0,       100}  // 0
};

const CPUTHRstruct Fan1VGATHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {80,    86,    78,        85,          80,          86,         105}  // 0
};

const CPUTHRstruct Fan1CPUTHRTable_DOS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    99,    0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  Main FAN_VENDOR_1 FAN Control for project2. // Cool mode
/*****************************************************************************/
//-----------------------------------------------------------------------------

//thermal sensor table for avc cool mode -avc

const FanControlstruct Fan1TSFanCtlTable_OS_100_Project2[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   41,    41,     30},  // 1
    {51,   46,    43,     44},  // 2
    {54,   48,    47,     58},  // 3
    {65,   52,    51,     62}  // 4
};


const FanControlstruct Fan1CPUFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {65,   00,    00,     00},  // 0
    {77,   65,    41,     30},  // 1
    {85,   75,    47,     44},  // 2
    {95,   81,    51,     58},  // 3
    {99,   85,    55,     58}  // 4
};

const FanControlstruct Fan1VGAFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},  // 0
    {70,   50,    32,     47},  // 1
    {75,   53,    35,     55},  // 2
    {80,   55,    38,     62}   // 3
};

const FanControlstruct Fan1CPUFanCtlTable_DOS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {54,   45,    43,     30},  // 1
    {60,   50,    47,     58},  // 2
    {68,   55,    51,     62},  // 3
    {90,   65,    59,     70},  // 4
    {95,   75,    59,     80},  // 5
    {99,   85,    65,     100}  // 6
};
/*****************************************************************************/
//  Main FAN_VENDOR_1 THR Control for project2.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct Fan1CPUTHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {85,    99,    75,        85,          0,       0,       99}  // 0
};

const CPUTHRstruct Fan1VGATHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {70,    95,    60,        80,          70,          95,         105}  // 0
};

const CPUTHRstruct Fan1CPUTHRTable_DOS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    99,    0,         0,          0,      0,     99}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  Main FAN_VENDOR_2 FAN Control for project1. Performance mode
/*****************************************************************************/
//-----------------------------------------------------------------------------

const FanControlstruct Fan2TSFanCtlTable_OS_100_Project1[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {47,   41,    19,     30},  // 1
    {49,   45,    22,     44},  // 2
    {53,   47,    28,     58},  // 3
    {73,   51,    31,     62}  // 4
};


const FanControlstruct Fan2CPUFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   5,     19,     30},   // 1
    {60,   48,    28,     58},   // 2
    {65,   51,    31,     62},   // 3
    {68,   56,    33,     70},   // 4
    {85,   61,    35,     80}    // 5
};

const FanControlstruct Fan2VGAFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    35,     53},   // 1
    {75,   65,    36,     58},   // 2
    {85,   70,    40,     62}   // 3
};

const FanControlstruct Fan2CPUFanCtlTable_DOS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   5,     19,     30},   // 1
    {60,   48,    28,     58},   // 2
    {65,   51,    31,     62},   // 3
    {68,   56,    33,     70},   // 4
    {85,   61,    35,     80},   // 5
    {93,   75,    40,     100}   // 6
};

/*****************************************************************************/
//  Main FAN_VENDOR_2 THR Control for Project1. Performance mode
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct Fan2CPUTHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {82,    93,    75,        85,           0,        0,       100}  // 0
};

const CPUTHRstruct Fan2VGATHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    90,    80,        85,          85,          90,         105}  // 0
};

const CPUTHRstruct Fan2CPUTHRTable_DOS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {82,    93,    0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  Main FAN_VENDOR_2 FAN Control for project2. Cool Mode
/*****************************************************************************/
//-----------------------------------------------------------------------------

const FanControlstruct Fan2TSFanCtlTable_OS_100_Project2[] = 
{
	
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {47,   41,    19,     30},  // 1
    {49,   45,    22,     44},  // 2
    {53,   47,    28,     58},  // 3
    {73,   51,    31,     62}  // 4
};


const FanControlstruct Fan2CPUFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   45,    19,     30},   // 1
    {60,   51,    28,     58},   // 2
    {65,   56,    31,     62},   // 3
    {75,   61,    33,     70},   // 4
    {66,   85,    35,     80}    // 5
};

const FanControlstruct Fan2VGAFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    31,     51},   // 1
    {75,   65,    33,     57},   // 2
    {85,   70,    35,     60}    // 3
};

const FanControlstruct Fan2CPUFanTable_DOS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},   // 0
    {55,   5,     19,     30},   // 1
    {60,   48,    28,     58},   // 2
    {65,   51,    31,     62},   // 3
    {68,   56,    33,     70},   // 4
    {85,   61,    35,     80},   // 5
    {93,   75,    40,     100}   // 6
};

/*****************************************************************************/
//  Main FAN_VENDOR_2 THR Control for project2. Cool Mode
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct Fan2CPUTHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     TCCoff    TCCon    Shut  Step
    {82,    93,    76,        85,           0,        0,       100}  // 0
};

const CPUTHRstruct Fan2VGATHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {85,    90,    80,        85,           85,          90,        105}  // 0
};

const CPUTHRstruct Fan2CPUTHRTable_DOS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {82,    93,    0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  Main FAN_VENDOR_3 FAN Control for project1.
/*****************************************************************************/
//-----------------------------------------------------------------------------

const FanControlstruct Fan3TSFanCtlTable_OS_100_Project1[] = 
{
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {47,   41,    39,     30},  // 1
    {50,   44,    43,     42},  // 2
    {53,   47,    47,     50},  // 3
    {55,   50,    50,     55},  // 4
    {68,   53,    54,     60},  // 5
    {76,   65,    60,     60}   // 6 
};


const FanControlstruct Fan3CPUFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {70,   00,    00,     00},   // 0
    {87,   65,    39,     30},   // 1
    {90,   75,    47,     50},   // 2
    {95,   81,    54,     60},  // 3
    {99,   85,    54,     60}  // 4
};

const FanControlstruct Fan3VGAFanCtlTable_OS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    30,     43},   // 1
    {83,   56,    34,     53},   // 2
    {85,   70,    36,     58}    // 3
};

const FanControlstruct Fan3CPUFanCtlTable_DOS_100_Project1[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {54,   42,    39,     30},  // 1
    {60,   50,    43,     42},  // 2
    {68,   55,    47,     50},  // 3
    {90,   65,    54,     60},  // 4
    {95,   75,    60,     60},  // 5
    {99,   85,    60,     100}  // 6
};

/*****************************************************************************/
//  Main FAN_VENDOR_3 THR Control for project1.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct  Fan3CPUTHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {85,    99,    75,        85,          0,       0,       100}  // 0
};

const CPUTHRstruct  Fan3VGATHRTable_OS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {80,    86,    78,        85,           80,          86,         105}  // 0
};

const CPUTHRstruct  Fan3CPUTHRTable_DOS_100_Project1[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    99,    0,         0,          0,      0,     100}  // 0
};
//-----------------------------------------------------------------------------
/*****************************************************************************/
//  Main FAN_VENDOR_3 FAN Control for project2.
/*****************************************************************************/
//-----------------------------------------------------------------------------

const FanControlstruct Fan3TSFanCtlTable_OS_100_Project2[] = 
{
//  On    Off    RPM     PWM    Step
    {45,   00,    00,     00},  // 0
    {48,   41,    36,     30},  // 1
    {51,   46,    39,     44},  // 2
    {54,   48,    43,     58},  // 3
    {65,   52,    47,     62},  // 4
    {76,   60,    54,     62},  // 5
};


const FanControlstruct  Fan3CPUFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {65,   00,    00,     00},   // 0
    {77,   65,    36,     30},   // 1
    {85,   75,    43,     58},   // 2
    {95,   81,    47,     62},  // 3
    {99,   85,    50,     62}   // 4
};

const FanControlstruct Fan3VGAFanCtlTable_OS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {55,   00,    00,     00},   // 0
    {70,   50,    28,     48},   // 1
    {75,   53,    31,     56},   // 2
    {80,   55,    33,     61}    // 3
};

const FanControlstruct Fan3CPUFanCtlTable_DOS_100_Project2[]=
{//  On    Off    RPM     PWM    Step
    {50,   00,    00,     00},  // 0
    {54,   42,    39,     30},  // 1
    {60,   50,    43,     42},  // 2
    {68,   55,    47,     50},  // 3
    {90,   65,    54,     60},  // 4
    {95,   75,    60,     60},  // 5
    {99,   85,    60,     100}  // 6
};

/*****************************************************************************/
//  Main FAN_VENDOR_3 THR Control for project2.
/*****************************************************************************/
//-----------------------------------------------------------------------------
const CPUTHRstruct  Fan3CPUTHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off    TCCoff    TCCon    Shut  Step
    {85,    99,    75,        85,          0,       0,       100}  // 0
};

const CPUTHRstruct  Fan3VGATHRTable_OS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off     PMAX_D2_off  PMAX_D2_on  Shut  Step
    {70,    95,    60,        80,           70,          95,        105}  // 0
};

const CPUTHRstruct  Fan3CPUTHRTable_DOS_100_Project2[]=
{// THROff  THROn  Turbo_Rem  Turbo_Off   TCCoff  TCCon  Shut  Step
    {85,    99,    0,        0,          0,       0,       100}  // 0
};
//-----------------------------------------------------------------------------
const FanTableList  TjMax100_Project1[3] = {
  {
    Fan1TSFanCtlTable_OS_100_Project1,
    sizeof(Fan1TSFanCtlTable_OS_100_Project1) / sizeof(FanControlstruct),
    Fan1CPUFanCtlTable_OS_100_Project1,
    sizeof (Fan1CPUFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan1VGAFanCtlTable_OS_100_Project1,
    sizeof (Fan1VGAFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan1CPUFanCtlTable_DOS_100_Project1,
    sizeof (Fan1CPUFanCtlTable_DOS_100_Project1) / sizeof (FanControlstruct),
    Fan1CPUTHRTable_OS_100_Project1,
    Fan1VGATHRTable_OS_100_Project1,
    Fan1CPUTHRTable_DOS_100_Project1
  },
  {
    Fan2TSFanCtlTable_OS_100_Project1,
    sizeof(Fan2TSFanCtlTable_OS_100_Project1) / sizeof(FanControlstruct), 
    Fan2CPUFanCtlTable_OS_100_Project1,
    sizeof (Fan2CPUFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan2VGAFanCtlTable_OS_100_Project1,
    sizeof (Fan2VGAFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan2CPUFanCtlTable_DOS_100_Project1,
    sizeof (Fan2CPUFanCtlTable_DOS_100_Project1) / sizeof (FanControlstruct),
    Fan2CPUTHRTable_OS_100_Project1,
    Fan2VGATHRTable_OS_100_Project1,
    Fan2CPUTHRTable_DOS_100_Project1
  },
  {
    Fan3TSFanCtlTable_OS_100_Project1,
    sizeof(Fan3TSFanCtlTable_OS_100_Project1) / sizeof(FanControlstruct),
    Fan3CPUFanCtlTable_OS_100_Project1,
    sizeof (Fan3CPUFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan3VGAFanCtlTable_OS_100_Project1,
    sizeof (Fan3VGAFanCtlTable_OS_100_Project1) / sizeof (FanControlstruct),
    Fan3CPUFanCtlTable_DOS_100_Project1,
    sizeof (Fan3CPUFanCtlTable_DOS_100_Project1) / sizeof (FanControlstruct),
    Fan3CPUTHRTable_OS_100_Project1,
    Fan3VGATHRTable_OS_100_Project1,
    Fan3CPUTHRTable_DOS_100_Project1
  },
};
//-----------------------------------------------------------------------------
const FanTableList  TjMax100_Project2[3] = {
  {
    Fan1TSFanCtlTable_OS_100_Project2,
    sizeof(Fan1TSFanCtlTable_OS_100_Project2) / sizeof(FanControlstruct),
    Fan1CPUFanCtlTable_OS_100_Project2,
    sizeof (Fan1CPUFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan1VGAFanCtlTable_OS_100_Project2,
    sizeof (Fan1VGAFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan1CPUFanCtlTable_DOS_100_Project2,
    sizeof (Fan1CPUFanCtlTable_DOS_100_Project2) / sizeof (FanControlstruct),
    Fan1CPUTHRTable_OS_100_Project2,
    Fan1VGATHRTable_OS_100_Project2,
    Fan1CPUTHRTable_DOS_100_Project2
  },
  {
    Fan2TSFanCtlTable_OS_100_Project2,
    sizeof(Fan2TSFanCtlTable_OS_100_Project2) / sizeof(FanControlstruct),
    Fan2CPUFanCtlTable_OS_100_Project2,
    sizeof (Fan2CPUFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan2VGAFanCtlTable_OS_100_Project2,
    sizeof (Fan2VGAFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan2CPUFanTable_DOS_100_Project2,
    sizeof (Fan2CPUFanTable_DOS_100_Project2) / sizeof (FanControlstruct),
    Fan2CPUTHRTable_OS_100_Project2,
    Fan2VGATHRTable_OS_100_Project2,
    Fan2CPUTHRTable_DOS_100_Project2
  },
  {
    Fan3TSFanCtlTable_OS_100_Project2,
    sizeof(Fan3TSFanCtlTable_OS_100_Project2) / sizeof(FanControlstruct),
    Fan3CPUFanCtlTable_OS_100_Project2,
    sizeof (Fan3CPUFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan3VGAFanCtlTable_OS_100_Project2,
    sizeof (Fan3VGAFanCtlTable_OS_100_Project2) / sizeof (FanControlstruct),
    Fan3CPUFanCtlTable_DOS_100_Project2,
    sizeof (Fan3CPUFanCtlTable_DOS_100_Project2) / sizeof (FanControlstruct),
    Fan3CPUTHRTable_OS_100_Project2,
    Fan3VGATHRTable_OS_100_Project2,
    Fan3CPUTHRTable_DOS_100_Project2
  },
};

const FanTableList  * FanTblPtr;

void oem_FanTblPtrHook (void)
{
  //
  // This hook is using to decide which table should be loaded.
  ////A285D00008 Add fan table--->Start
  if(thermalTBlMode == PFM_Mode)
  	FanTblPtr = TjMax100_Project1;
  else if(thermalTBlMode == COOL_Mode)
  	FanTblPtr = TjMax100_Project2;
 //A285D00008 Add fan table--->End
// Sample1 --
// MBID
// Notice: If MBID has not be written on product line, its default value would be '0xFF'.
//         Whice means 15" will be selected.
  //if (IS15Inch) {  //A485D00039- 
  //  // 15" 
  //  FanTblPtr = TjMax100_Project2;
  //} else {
  //  // 14" 
  //  FanTblPtr = TjMax100_Project1;
  //}
//   // Sample2 --
//   // TjMax of CPU
//   //
//
//   switch((CPU_TYPE & 0x03)) // 0x00:Tj85,    0x01:Tj90,   0x02:Tj100,   0x03:Tj105
//   {
//
//     case 3:    // Tj105
// //      FanTblPtr = TjMax105;
// //      break;
//
//     case 2:    // Tj100
//       FanTblPtr = TjMax100;
//       break;
//
//     case 1:    // Tj90
// //      FanTblPtr = TjMax90;
// //      break;
//
//     case 0:    // Tj85
// //      FanTblPtr = TjMax85;
// //      break;
//
//     default:   // Tj85 Default.
//       FanTblPtr = TjMax85;
//       break;
//   }  
}

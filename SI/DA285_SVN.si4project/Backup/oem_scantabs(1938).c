/*-----------------------------------------------------------------
 * TITLE: OEM_SCANTABS.C - RC Table(s) for Scan Matix
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------*/
 
//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

const CBYTE Rc_ROM_Tables[] =
{  // 15" US,UK and JP
   ////A485D00007 Modify KB Matrix for A485 
    // Scanner Table 1  - 16 x 8 Scan Table
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  //0xEC,0xB0,0x61,0x00,0x34,0x33,0xB2,0x64,0x52,0x00,0xB1,0x00,0x8A,0x00,0xE4,0x00, // 00
  //0x0D,0xAF,0x58,0x00,0x2C,0x35,0x5B,0xB3,0x54,0x88,0x66,0x00,0x00,0x82,0x00,0x00, // 01
  //0x15,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0x4D,0x00,0x6A,0x00,0xF1,0x00,0x00,0x00, // 02
  //0x0E,0xAE,0xAD,0x8C,0x2E,0x36,0x55,0xB4,0x4E,0x00,0xB5,0xE8,0x00,0xF2,0xF3,0xEB, // 03
  //0x1C,0x23,0x1B,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0xE0, // 04
  //0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xB6,0xB7,0xF0,0xB8,0xE2,0xE8, // 05
  //0x1A,0x21,0x22,0x8D,0x2A,0x3A,0x41,0x49,0x5D,0x89,0xEE,0x00,0x00,0x04,0xEB,0x96, // 06
  //0x67,0x00,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0x00,0xED,0xE5,0x8B,0xE7,0xE6,0x97, // 07

	// Scanner Table 1  - 16 x 8 Scan Table
	//A485D00026:Fix PGUP/PGDN/ALT(R)/PrSC key no work.
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  0x0E,0xAD,0xAE,0x2E,0x36,0x55,0xB4,0x4E,0xB5,0xE3,0xE2,0x96,0xE8,0x00,0x00,0x8C, // 00
  0x16,0x1E,0x26,0x25,0x3D,0x3E,0x46,0x45,0xB6,0xB8,0xB7,0x97,0xEB,0xE0,0x00,0x00, // 01
  0x15,0x1D,0x24,0x2D,0x3C,0x43,0x44,0x4D,0x6A,0x04,0x00,0x00,0x00,0xF1,0x00,0x00, // 02
  0x0D,0x58,0xAF,0x2C,0x35,0x5B,0xB3,0x54,0x66,0x82,0x00,0x00,0x00,0x00,0x88,0x00, // 03
  0x1C,0x1B,0x23,0x2B,0x3B,0x42,0x4B,0x4C,0x5D,0x00,0x00,0xE0,0x8E,0x00,0x00,0x00, // 04
  0xEC,0x61,0xB0,0x34,0x33,0xB2,0x64,0x52,0xB1,0x00,0x00,0x00,0xE4,0x8A,0x00,0x00, // 05
  0x1A,0x22,0x21,0x2A,0x3A,0x41,0x49,0x5D,0xEE,0x00,0x00,0x96,0xEB,0x00,0x89,0x8D, // 06
  0x67,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0xED,0xE7,0xE5,0x97,0xE6,0x8B,0x00,0x00, // 07
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- 110  115   45   x    35   36  117  132   41   x   116   x    60   x    83   x
//1-  16  114   30   x    21   22   28  118   27   44   15   x    x    90   x    x
//2-  17   19   18   x    20   23   24   25   26   x    14   x   125   x    x   127
//3-  1   113  112   58   6    7    13  119   12   x   120   76   x    75   80   85
//4-  31   33   32   x    34   37   38   39   40   x    29   x    x    x    59  129
//5-  2    4    3    x    5    8    9    10   11   x   121  122  124  123   81   86
//6-  46   48   47   64   49   52   53   54   42   57   43   x    x    x   126   x
//7- 131   x    x    x    50   51   56  133   55   x    61   84   62   89   79   x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- Esc   F4   \|   x    G    H    F6  132   '"   x    F5   x   LAlt  x   UpAr  x
//1- Tab   F3  Caps  x    T    Y    ]}   F7   [{  LShf Back  x    x   Nums   x    x
//2-  Q    E    W    x    R    U    I    O    P    x   �FD   x    x    x    x   LWin
//3-  `~   F2   F1  LCtr  5%   6^   +=   F8   -_   x    F9  Del   x   Ins  Home PgUp
//4-  A    D    S    x    F    J    K    L    ;:   x    \|   x    x    x    Fn  RAPP
//5-  1!   3#   2@   x    4$   7&   8*   9(   0)   x   F10  F11  PrSc F12  End  PgDn
//6-  Z    C    X   RCtr  V    M    ,<   .>   \|  RShf Entr  x    x    x   PuBr  x
//7- 131   x    x    x    B    N    56  133   /?   x   Spce DnAr RAlt RiAr LeAr  x
//------------------------------------------------------------------------------------
};


const CBYTE Rc_LOC_Tables[] =
{  // 14" US, UK and JP.
    // Scanner Table 1  - 16 x 8 Scan Table
//00   01   02    03    04   05   06    07    08   09   10    11   12    13   14    15
  0  ,120,0  ,116,114,112,113,123,0  ,121,119,117,115,16 ,1  ,0  , // 00
  0  ,60 ,44 ,0  ,58 ,17 ,2  ,0  ,0  ,0  ,46 ,32 ,31 ,18 ,3  ,0  , // 01
  0  ,48 ,47 ,33 ,19 ,5  ,4  ,0  ,0  ,61 ,49 ,34 ,21 ,20 ,6  ,0  , // 02
  0  ,51 ,50 ,36 ,35 ,22 ,7  ,0  ,0  ,0  ,52 ,37 ,23 ,8  ,9  ,0  , // 03
  0  ,53 ,38 ,24 ,25 ,11 ,10 ,0  ,0  ,54 ,55 ,39 ,40 ,26 ,12 ,0  , // 04
  0  ,56 ,41 ,0  ,27 ,13 ,0  ,0  ,30 ,57 ,43 ,28 ,0  ,42 ,0  ,0  , // 05
  0  ,45 ,0  ,0  ,0  ,0  ,15 ,0  ,0  ,93 ,0  ,92 ,91 ,107,0  ,0  , // 06
  99 ,104,98 ,77 ,102,96 ,110,90 ,122,106,103,105,100,101,125,0  , // 07
};

const CBYTE Rc_ROM_Tables_15BR[] =
{  // 15" Brazil
    // Scanner Table 1  - 16 x 8 Scan Table
	//A485D00026:Fix PGUP/PGDN/ALT(R)/PrSC key no work.
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  //0xEC,0xB0,0x61,0x00,0x34,0x33,0xB2,0x64,0x52,0x00,0xB1,0x00,0x8A,0x00,0xE4,0x00, // 00
  //0x0D,0xAF,0x58,0x00,0x2C,0x35,0x5B,0xB3,0x54,0x88,0x66,0x00,0x00,0x82,0x00,0x00, // 01
  //0x15,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0x4D,0x00,0x6A,0x00,0xF1,0x00,0x00,0x00, // 02
  //0x0E,0xAE,0xAD,0x8C,0x2E,0x36,0x55,0xB4,0x4E,0x00,0xB5,0xE8,0x00,0xF2,0xF3,0xEB, // 03
  //0x1C,0x23,0x1B,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0xE0, // 04
  //0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xB6,0xB7,0xF0,0xB8,0xE2,0xE8, // 05
  //0x1A,0x21,0x22,0x8D,0x2A,0x3A,0x41,0x49,0x5D,0x89,0xEE,0x00,0x00,0x04,0xEB,0x96, // 06
  //0x67,0x00,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0x00,0xED,0xE5,0x8B,0xE7,0xE6,0x97, // 07
  // Scanner Table 1  - 16 x 8 Scan Table
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  0x0E,0xAD,0xAE,0x2E,0x36,0x55,0xB4,0x4E,0xB5,0xE3,0xE2,0x96,0xE8,0x00,0x00,0x8C, // 00
  0x16,0x1E,0x26,0x25,0x3D,0x3E,0x46,0x45,0xB6,0xB8,0xB7,0x97,0xEB,0xE0,0x00,0x00, // 01
  0x15,0x1D,0x24,0x2D,0x3C,0x43,0x44,0x4D,0x6A,0x04,0x00,0x00,0x00,0xF1,0x00,0x00, // 02
  0x0D,0x58,0xAF,0x2C,0x35,0x5B,0xB3,0x54,0x66,0x82,0x00,0x00,0x00,0x00,0x88,0x00, // 03
  0x1C,0x1B,0x23,0x2B,0x3B,0x42,0x4B,0x4C,0x5D,0x00,0x00,0xE0,0x8E,0x00,0x00,0x00, // 04
  0xEC,0x61,0xB0,0x34,0x33,0xB2,0x64,0x52,0xB1,0x00,0x00,0x00,0xE4,0x8A,0x00,0x00, // 05
  0x1A,0x22,0x21,0x2A,0x3A,0x41,0x49,0x5D,0xEE,0x00,0x00,0x96,0xEB,0x00,0x89,0x8D, // 06
  0x67,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0xED,0xE7,0xE5,0x97,0xE6,0x8B,0x00,0x00, // 07
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- 110  115   45   x    35   36  117  132   41   x   116   x    60   x    83   x
//1-  16  114   30   x    21   22   28  118   27   44   15   x    x    90    x    x
//2-  17   19   18   x    20   23   24   25   26   x    14   x   125   x    x   127
//3-  1   113  112   58   6    7    13  119   12   x   120   76   x    75   80   85
//4-  31   33   32   x    34   37   38   39   40   x    29   x    x    x    59  129
//5-  2    4    3    x    5    8    9    10   11   x   121  122  124  123   81   86
//6-  46   48   47   56   49   52   53   54   42   57   43   x    x    x   126   x
//7- 131   x    x    x    50   51   x   133   55   x    61   84   62   89   79   x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- Esc   F4   \|   x    G    H    F6  132   '"   x    F5   x   LAlt  x   UpAr  x
//1- Tab   F3  Caps  x    T    Y    ]}   F7   [{  LShf Back  x    x   Nums   x    x
//2-  Q    E    W    x    R    U    I    O    P    x   �FD   x    x    x    x   LWin
//3-  `~   F2   F1  LCtr  5%   6^   +=   F8   -_   x    F9  Del   x   Ins  Home PgUp
//4-  A    D    S    x    F    J    K    L    ;:   x    \|   x    x    x    Fn  RAPP
//5-  1!   3#   2@   x    4$   7&   8*   9(   0)   x   F10  F11  PrSc F12  End  PgDn
//6-  Z    C    X   RCtr  V    M    ,<   .>   \|  RShf Entr  x    x    x   PuBr  x
//7- 131   x    x    x    B    N    56  133   /?   x   Spce DnAr RAlt RiAr LeAr  x
//------------------------------------------------------------------------------------
};

const CBYTE EtKey_ROM_Tables[] =
{  //16   17   18    // 19X8 Scan Table
  0x77,0x6B,0x00,
  0x90,0x73,0x00,
  0x7C,0x74,0x00,
  0x7B,0x69,0x00,
  0x6C,0x72,0x00,
  0x75,0x7A,0x00,
  0x7D,0x81,0x00,
  0x79,0x70,0x00,
//------------------
//    16   17   18
//------------------
//0-  91   95   x
//1-  92   96   x
//2- 101  105   x
//3- 102  106   x
//4-  93   97   x
//5-  99   98   x
//6- 100  108   x
//7- 103  104   x
//------------------
//------------------
//    16   17   18
//------------------
//0-  7    /    x
//1-  4    8    x
//2-  9    -    x
//3-  6    +    x
//4-  1    5    x
//5-  0    2    x
//6-  *   Gent  x
//7-  3    .    x
//------------------
};

const CBYTE Rc_ROM_Tables_14[] =
{  // 14" US, UK and JP.
  //A485D00007 Modify KB Matrix for A485 
    // Scanner Table 1  - 16 x 8 Scan Table
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  //0xEC,0xB0,0x61,0x00,0x34,0x33,0xB2,0x64,0x52,0x00,0xB1,0x00,0x8A,0x00,0xE4,0x00, // 00
  //0x0D,0xAF,0x58,0x00,0x2C,0x35,0x5B,0xB3,0x54,0x88,0x66,0x00,0x00,0x82,0x00,0x00, // 01
  //0x15,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0x4D,0x00,0x6A,0x00,0x00,0x00,0x00,0x00, // 02
  //0x0E,0xAE,0xAD,0x8C,0x2E,0x36,0x55,0xB4,0x4E,0x00,0xB5,0xE2,0x00,0xE3,0xE8,0x96, // 03
  //0x1C,0x23,0x1B,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0xE0, // 04
  //0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xB6,0xB7,0xE0,0xB8,0x95,0x97, // 05
  //0x1A,0x21,0x22,0x8D,0x2A,0x3A,0x41,0x49,0x5D,0x89,0xEE,0x00,0x00,0x04,0xE2,0x00, // 06
  //0x67,0x00,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0x00,0xED,0xE5,0x8B,0xE7,0xE6,0x00, // 07

	// Scanner Table 1  - 16 x 8 Scan Table
	//A485D00026:Fix PGUP/PGDN/ALT(R)/PrSC key no work.
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  0x0E,0xAD,0xAE,0x2E,0x36,0x55,0xB4,0x4E,0xB5,0xE3,0xE2,0x96,0xE8,0x00,0x00,0x8C, // 00
  0x16,0x1E,0x26,0x25,0x3D,0x3E,0x46,0x45,0xB6,0xB8,0xB7,0x97,0xEB,0xE0,0x00,0x00, // 01
  0x15,0x1D,0x24,0x2D,0x3C,0x43,0x44,0x4D,0x6A,0x04,0x00,0x00,0x00,0xF1,0x00,0x00, // 02
  0x0D,0x58,0xAF,0x2C,0x35,0x5B,0xB3,0x54,0x66,0x82,0x00,0x00,0x00,0x00,0x88,0x00, // 03
  0x1C,0x1B,0x23,0x2B,0x3B,0x42,0x4B,0x4C,0x5D,0x00,0x00,0xE0,0x8E,0x00,0x00,0x00, // 04
  0xEC,0x61,0xB0,0x34,0x33,0xB2,0x64,0x52,0xB1,0x00,0x00,0x00,0xE4,0x8A,0x00,0x00, // 05
  0x1A,0x22,0x21,0x2A,0x3A,0x41,0x49,0x5D,0xEE,0x00,0x00,0x96,0xEB,0x00,0x89,0x8D, // 06
  0x67,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0xED,0xE7,0xE5,0x97,0xE6,0x8B,0x00,0x00, // 07
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- 110  115   45   x    35   36  117  132   41   x   116   x    60   x    83   x
//1-  16  114   30   x    21   22   28  118   27   44   15   x    x    90    x    x
//2-  17   19   18   x    20   23   24   25   26   x    14   x   125   x    x   127
//3-  1   113  112   58   6    7    13  119   12   x   120  126   x    75   80   81
//4-  31   33   32   x    34   37   38   39   40   x    29   x    x    x    59  129
//5-  2    4    3    x    5    8    9    10   11   x   121  122  124  123   86   85
//6-  46   48   47   56   49   52   53   54   42   57   43   x    x    x    76   x
//7- 131   x    x    x    50   51   x   133   55   x    61   84   62   89   79   x
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F
//------------------------------------------------------------------------------------
//0- Esc   F4   \|   x    G    H    F6  132   '"   x    F5   x   LAlt  x   UpAr  x
//1- Tab   F3  Caps  x    T    Y    ]}   F7   [{  LShf Back  x    x   Nums   x    x
//2-  Q    E    W    x    R    U    I    O    P    x   �FD   x    x    x    x   LWin
//3-  `~   F2   F1  LCtr  5%   6^   +=   F8   -_   x    F9  PuBr  x   Ins  Home End
//4-  A    D    S    x    F    J    K    L    ;:   x    \|   x    x    x    Fn  RAPP
//5-  1!   3#   2@   x    4$   7&   8*   9(   0)   x   F10  F11  PrSc F12  PgDn PgUp
//6-  Z    C    X   RCtr  V    M    ,<   .>   \|  RShf Entr  x    x    x   Del   x
//7- 131   x    x    x    B    N    56  133   /?   x   Spce DnAr RAlt RiAr LeAr  x
//------------------------------------------------------------------------------------
};

const CBYTE Rc_ROM_Tables_14BR[] =
{  // 14" Brazil
    // Scanner Table 1  - 16 x 8 Scan Table
	//A485D00026:Fix PGUP/PGDN/ALT(R)/PrSC key no work.
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  //0xEC,0xB0,0x61,0x00,0x34,0x33,0xB2,0x64,0x52,0x00,0xB1,0x00,0x8A,0x00,0xE4,0x00, // 00
  //0x0D,0xAF,0x58,0x00,0x2C,0x35,0x5B,0xB3,0x54,0x88,0x66,0x00,0x00,0x82,0x00,0x00, // 01
  //0x15,0x24,0x1D,0x00,0x2D,0x3C,0x43,0x44,0x4D,0x00,0x6A,0x00,0x00,0x00,0x00,0x00, // 02
  //0x0E,0xAE,0xAD,0x8C,0x2E,0x36,0x55,0xB4,0x4E,0x00,0xB5,0xE2,0x00,0xE3,0xE8,0x96, // 03
  //0x1C,0x23,0x1B,0x00,0x2B,0x3B,0x42,0x4B,0x4C,0x00,0x5D,0x00,0x00,0x00,0x8E,0xE0, // 04
  //0x16,0x26,0x1E,0x00,0x25,0x3D,0x3E,0x46,0x45,0x00,0xB6,0xB7,0xE0,0xB8,0x95,0x97, // 05
  //0x1A,0x21,0x22,0x51,0x2A,0x3A,0x41,0x49,0x5D,0x89,0xEE,0x00,0x00,0x04,0xE2,0x00, // 06
  //0x67,0x00,0x00,0x00,0x32,0x31,0x00,0x13,0x4A,0x00,0xED,0xE5,0x8B,0xE7,0xE6,0x00, // 07
  // Scanner Table 1  - 16 x 8 Scan Table
    //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  0x0E,0xAD,0xAE,0x2E,0x36,0x55,0xB4,0x4E,0xB5,0xE3,0xE2,0x96,0xE8,0x00,0x00,0x8C, // 00
  0x16,0x1E,0x26,0x25,0x3D,0x3E,0x46,0x45,0xB6,0xB8,0xB7,0x97,0xEB,0xE0,0x00,0x00, // 01
  0x15,0x1D,0x24,0x2D,0x3C,0x43,0x44,0x4D,0x6A,0x04,0x00,0x00,0x00,0xF1,0x00,0x00, // 02
  0x0D,0x58,0xAF,0x2C,0x35,0x5B,0xB3,0x54,0x66,0x82,0x00,0x00,0x00,0x00,0x88,0x00, // 03
  0x1C,0x1B,0x23,0x2B,0x3B,0x42,0x4B,0x4C,0x5D,0x00,0x00,0xE0,0x8E,0x00,0x00,0x00, // 04
  0xEC,0x61,0xB0,0x34,0x33,0xB2,0x64,0x52,0xB1,0x00,0x00,0x00,0xE4,0x8A,0x00,0x00, // 05
  0x1A,0x22,0x21,0x2A,0x3A,0x41,0x49,0x5D,0xEE,0x00,0x00,0x96,0xEB,0x00,0x89,0x8D, // 06
  0x67,0x00,0x00,0x32,0x31,0x51,0x13,0x4A,0xED,0xE7,0xE5,0x97,0xE6,0x8B,0x00,0x00, // 07
};

const CBYTE EtKey_ROM_Tables_1[] =
{  //16   17   18    // 19X8 Scan Table
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
};


const CBYTE KB_S3WakeUP_Tables[] =
{
    // Scanner Table 1
  //00   01   02   03   04   05   06   07   08   09   10   11   12   13   14   15
  0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
   0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
     0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
   0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55, //
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55  //
};

const CBYTE EtKey_S3WakeUP_Tables[] =
{  //16   17   18    // 19X8 Scan Table
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
  0x55,0x55,0x55,
};

//----------------------------------------------------------------------------
// The function of hotkey Fn + F1 Volume Mute
//----------------------------------------------------------------------------
void HotKey_Fn_F1(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      if( event == MAKE_EVENT )
      {
        e0_prefix_code(0x23,event);    // [A0h] Mute event
        if(Ccb42_DISAB_KEY == 0) {
          ECSMI_SCIEvent(MUTE_EVENT_43);    // 0x43 Mute.
        }
      }

      if ( event == BREAK_EVENT )
      {
        e0_prefix_code(0x23,event);    // [A0h] Mute event
      }
    }else{
      if( event == MAKE_EVENT )
      {
          ECSMI_SCIEvent(MUTE_EVENT_43);    // 0x43 Mute.
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x05,event); }    // [05H] F1 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{
  if( event == MAKE_EVENT )
  {
    e0_prefix_code(0x23,event);   // [A0h] Mute event
    if(Ccb42_DISAB_KEY == 0) {
      ECQEvent(MUTE_EVENT_43);    // 0x43 Mute.
    }
  }

  if ( event == BREAK_EVENT )
  {
    e0_prefix_code(0x23,event);   // [A0h] Mute event
  }
}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F2 Decrease Volume
//----------------------------------------------------------------------------
void HotKey_Fn_F2(BYTE event)
{
#if  KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      //if((event == MAKE_EVENT)||(event == BREAK_EVENT))
      //{
        e0_prefix_code(0x21,event);    // [A2h] Volume Down event
      //}
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x06,event); }    // [06H] F2 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{
  if((event == MAKE_EVENT)||(event == BREAK_EVENT))
  {
    e0_prefix_code(0x21,event);   // [A2h] Volume Down event
  }
}
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F3 Increase Volume
//----------------------------------------------------------------------------
void HotKey_Fn_F3(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      //if((event == MAKE_EVENT)||(event == BREAK_EVENT))
      {
        e0_prefix_code(0x32,event);    // [A1h] Volume Up event
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x04,event); }    // [04H] F3 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{
  if((event == MAKE_EVENT)||(event == BREAK_EVENT))
  {
    e0_prefix_code(0x32,event);   // [A1h] Volume Up event
  }
}
#endif

}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F4 Mic Mute
//----------------------------------------------------------------------------
void HotKey_Fn_F4(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    if(event == MAKE_EVENT)
    {
      if(Ccb42_DISAB_KEY == 0) {
        ECQEvent(MIC_MUTE_EVENT);    // 0x6A Mic Mute.
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x0C,event); }    // [0CH] F4 key.
  }
#else
//if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
//{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(MIC_MUTE_EVENT);    // 0x6A Mic Mute.
  }
//}
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F5 Decrease Brightness
//----------------------------------------------------------------------------
void HotKey_Fn_F5(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    if(event != BREAK_EVENT)
    {
      if( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECQEvent(LCD_BRIG_DEC_EVENT);  // 0x15 inform bios.
        }
      }
      else
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECSMIEvent(0x15);
        }
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x03,event); }    // [03H] F5 key.
  }
#else
//if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
//{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(CAMERA_MUTE_EVENT);  // 0x65 Camera Mute.
  }
//}

#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F6 increase Brightness
//----------------------------------------------------------------------------
void HotKey_Fn_F6(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    if(event != BREAK_EVENT)
    {
      if( IS_MASK_SET(SYS_MISC1,ACPI_OS) )
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECQEvent(LCD_BRIG_INC_EVENT);  // 0x14 inform bios.
        }
      }
      else
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECSMIEvent(0x14);
        }
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x0B,event); }    // [0BH] F6 key.
  }
#else
//if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
//{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(PRESENT_DIR_EVENT);  // 0x16 Presentation Direct.
  }
//}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F7 Display Device Selection
//----------------------------------------------------------------------------
void HotKey_Fn_F7(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if( (SYS_STATUS & 0x07) != 0 )    // Check DOS mode.
    if(event != BREAK_EVENT)
    {
      if(Ccb42_DISAB_KEY == 0) {
        ECQEvent(PRESENT_DIR_EVENT);  // 0x15 inform bios.
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x83,event); }    // [80H] F7 key.
  }
#else
if( (SYS_STATUS & 0x07) != 0 )    // Check DOS mode.
{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(LCD_BRIG_DEC_EVENT);  // 0x15 inform bios.
  }
}
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F8 Wireless Radio
//----------------------------------------------------------------------------
void HotKey_Fn_F8(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if( (SYS_STATUS & 0x07) != 0 )    // Check DOS mode.
    if(event == MAKE_EVENT)
    {
      if(Ccb42_DISAB_KEY == 0) {
        ECQEvent(WIRELESS_RADIO_EVENT);    // 0x64 Wirless Radio notify.
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x0A,event); }    // [0AH] F8 key.
  }
#else
if( (SYS_STATUS & 0x07) != 0 )    // Check DOS mode.
{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(LCD_BRIG_INC_EVENT);  // 0x14 inform bios.
  }
}
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F9 OS Setting
//----------------------------------------------------------------------------
void HotKey_Fn_F9(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    //if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    //{
    if(event == MAKE_EVENT)
    {
      if(Ccb42_DISAB_KEY == 0) {
        ECQEvent(OSSETTING_EVENT);    // 0x64 Wirless Radio notify.
      }
    }
    //}
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x01,event); }    // [01H] F9 key.
  }
#else
//if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
//{
  if(Ccb42_DISAB_KEY == 0) {
    ECQEvent(WIRELESS_RADIO_EVENT);   // 0x64 Wirless Radio notify.
  }
//}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F10 OS Search
//----------------------------------------------------------------------------
void HotKey_Fn_F10(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      if( event == MAKE_EVENT )
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECQEvent(OSSEARCH_EVENT);    // 0x67 OS Search
        }
      }

    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x09,event); }    // [09H] F10 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{

    if((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
      e0_prefix_code(0x15,event);     // [9Dh] Prev Track event
    }

}

#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F11 View Open APP
//----------------------------------------------------------------------------
void HotKey_Fn_F11(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      if(event == MAKE_EVENT)
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECQEvent(OSVEAROPENAPP_EVENT);    // 0x68 OS Vear Open APP
        }
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x78,event); }    // [78H] F11 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{
    if((SYS_STATUS & 0x07) == 0x02) // linpus
    { e0_prefix_code(0x10,event); } // ScanCode 0x65.
    else
    {
      if((event == MAKE_EVENT)||(event == BREAK_EVENT))
      { e0_prefix_code(0x34,event); } // [9Fh] Play/Pause event
    }
}
#endif

}

//----------------------------------------------------------------------------
// The function of hotkey Fn + F12 View ALL APP
//----------------------------------------------------------------------------
void HotKey_Fn_F12(BYTE event)
{
#if KB_FnFuncKey
  if ( Oem_CheckFxkeyMode() )
  {  // Enable HotKey
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
    {
      if( event == MAKE_EVENT )
      {
        if(Ccb42_DISAB_KEY == 0) {
          ECQEvent(OSVEARALLAPP_EVENT);    // 0x69 OS View All APP
        }
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x07,event); }    // [07H] F12 key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{
    if((SYS_STATUS & 0x07) == 0x02) // linpus
    { e0_prefix_code(0x20,event); } // ScanCode 0x67.
    else
    {
      if((event == MAKE_EVENT)||(event == BREAK_EVENT))
      { e0_prefix_code(0x4D,event); } // [9Ch] Next Track event
    }
}
#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + ESC
//----------------------------------------------------------------------------
void HotKey_Fn_ESC(BYTE event)
{
#if KB_FnFuncKey
  if( temp_scanner_state.field.tFN || (IS_MASK_SET(EVT_STATUS1,KB_FN_Status)&&(!FnCtrlSWAPGet)))
  {  // Enable HotKey
#if KB_FnSticky_Key      
    if(!FnStickyGET ){
#endif //KB_FnSticky_Key      
      if( event == MAKE_EVENT )
      {
        if(Ccb42_DISAB_KEY == 0) {
          if (FNLKSTATEGET)
            {FNLKSTATECLR;}
          else
            {FNLKSTATESET;}
            ECSMI_SCIEvent(FN4SEC_EVENT_74);		
        }
      }
#if KB_FnSticky_Key  
    }
#endif
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { simple_code(0x76,event); }    // [76H] ESC key.
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{

}

#endif
}

//----------------------------------------------------------------------------
// The function of hotkey Fn + Space
//----------------------------------------------------------------------------
void HotKey_Fn_SPACE(BYTE event)
{
#if KB_FnFuncKey
  if( temp_scanner_state.field.tFN || (IS_MASK_SET(EVT_STATUS1,KB_FN_Status)&&(!FnCtrlSWAPGet)))
  {  // Enable HotKey
#if Support_KBD_BL_LED
    if( IS_MASK_CLEAR(KBL_Status,PowerOnCheck) )
#endif
    {
      if( event == MAKE_EVENT )
      {
        if(Ccb42_DISAB_KEY == 0) 
        	{
#if Support_KBD_BL_LED
#if Support_KBD_BL_DETECT
					if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
					{
						if(DisableKbdBackLightGET)
						{
							KBD_BL_LED = LED_OFF;
							KBL_Target_Level_Duty = 0;
							KBL_Current_Level_Duty = 0;
			    		u4PeriCtrl3 &= ~PC3_TURN_ON_KEYBOARD_BACK_LIGHT_MASK;
						}
						else
						{
		          if(KBL_Target_Level_Duty < KBD_BL_LED_LOW)
		          {
		        		KBL_Target_Level_Duty = KBD_BL_LED_LOW;
		          }
		          else if(KBL_Target_Level_Duty < KBD_BL_LED_HIGH)
		          {
		        		KBL_Target_Level_Duty = KBD_BL_LED_HIGH;
		          }
		          else
		          {
		        		KBL_Target_Level_Duty = 0;
		          }
		          ChangeKbdBacklightLedIntervalTime();
		        }
					}
#endif //Support keyboard backlight
        }
      }
    }
  }
  else
  {
    if((event == MAKE_EVENT)||(event == BREAK_EVENT)||(event == REPEAT_EVENT))
    { ; }    //
  }
#else
if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
{

}

#endif
}
#if (Support_Keyboard == CS13KB)
//                             WinKey + key L  +BREAK L  + BREAK Winkey
const CBYTE windows_lcok[] = {0xE0,0x1F, 0x4B,   0xF0,0x4B,    0xE0,0xF0,0x1F,  0x00};

#endif //CS13KB

#if (Support_Keyboard == CS16KB)
// "(" 
const CBYTE Keypad_hotkey2[] = {0x12,0x46,  0xF0,0x46, 0xF0,0x12,  0x00};

// ")" 
const CBYTE Keypad_hotkey3[] = {0x12,0x45,  0xF0,0x45, 0xF0,0x12,  0x00};
#endif //CS13KB

//----------------------------------------------------------------------------
// The function of hotkey 1
//----------------------------------------------------------------------------
void HotKey_1(BYTE event)
{
#if (Support_Keyboard == CS13KB)
//Win Lock
  if ( IS_MASK_SET(SYS_MISC1, ACPI_OS))
  {
      Buffer_String(windows_lcok);        /* Buffer pre-string. */
      
  }
#endif //CS13KB

#if (Support_Keyboard == CS16KB)
//keypad hotkey1 "="
  if (temp_scanner_state.field.tFN) {
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS)){
      ECQEvent(HOTKEY_calculator);    // Sleep mode(S3).
    }
  }else{
    simple_code(0x55 ,event);
  }
#endif //CS16KB
}


//----------------------------------------------------------------------------
// The function of hotkey 2
//----------------------------------------------------------------------------
void HotKey_2(BYTE event)
{

#if (Support_Keyboard == CS16KB)
//keypad hotkey2 "("
  if ((event == MAKE_EVENT) || (event == REPEAT_EVENT)) {
      Buffer_String(Keypad_hotkey2);        /* Buffer pre-string. */
  }       
#endif //CS16KB
    
}

//----------------------------------------------------------------------------
// The function of hotkey 3
//----------------------------------------------------------------------------
void HotKey_3(BYTE event)
{
#if (Support_Keyboard == CS16KB)
//keypad hotkey3 ")"
  if ((event == MAKE_EVENT) || (event == REPEAT_EVENT)) {
      Buffer_String(Keypad_hotkey3);        /* Buffer pre-string. */
  }    
#endif //CS16KB
}

//----------------------------------------------------------------------------
// The function of hotkey 4
//----------------------------------------------------------------------------
void HotKey_4(BYTE event)
{
#if (Support_Keyboard == CS16KB)
//keypad hotkey4 "Backspace"
  if (temp_scanner_state.field.tFN) {
    if ( IS_MASK_SET(SYS_MISC1, ACPI_OS)){
      ECQEvent(H4_Suspend);    // Sleep mode(S3).
    }
  }else{
    simple_code(0x66 ,event);
  }
#endif //CS16KB       
}

//----------------------------------------------------------------------------
// The function of hotkey 5
//---------------------------------------------------------------------------
void HotKey_5(BYTE event)
{


}

const FUNCT_PTR_V_B  HotKey_Fn_Fx[] =
{
  HotKey_Fn_F1,      // Fn + F1
  HotKey_Fn_F2,      // Fn + F2
  HotKey_Fn_F3,      // Fn + F3
  HotKey_Fn_F4,      // Fn + F4
  HotKey_Fn_F5,      // Fn + F5
  HotKey_Fn_F6,      // Fn + F6
  HotKey_Fn_F7,      // Fn + F7
  HotKey_Fn_F8,      // Fn + F8
  HotKey_Fn_F9,      // Fn + F9
  HotKey_Fn_F10,     // Fn + F10
  HotKey_Fn_F11,     // Fn + F11
  HotKey_Fn_F12,     // Fn + F12
  HotKey_Fn_ESC,     // Fn + ESC

  HotKey_1,           // Hotkey1
  HotKey_2,           // Hotkey2
  HotKey_3,           // Hotkey3
  HotKey_4,           // Hotkey4
//  HotKey_5,           // Hotkey5
  HotKey_Fn_SPACE,    // Fn + Space
};

//---------------------------------------------------------------------------
//  Numpad 7 Home  [B0h]
//  Numpad 8 UP    [B1h]
//  Numpad 9 PGUp  [B2h]
//  Numpad *       [B3h]
//  Numpad 4 Left  [B4h]
//  Numpad 5       [B5h]
//  Numpad 6 Right [B6h]
//  Numpad -       [B7h]
//  Numpad 1 End   [B8h]
//  Numpad 2 Down  [B9h]
//  Numpad 3 PGDn  [BAh]
//  Numpad +       [BBh]
//  Numpad 0 Ins   [BCh]
//  Numpad . Del   [BDh]
//  Numpad /       [BEh]
//---------------------------------------------------------------------------

const CBYTE ss2_9C[] =  // Numpad 7
{
    0x3D,   // Normal     // 7
    0xB0,   // Numlock
    0xB0,   // Fn
    0xB0    // Fn+Numlock
};

const CBYTE ss2_9D[] =  // Numpad 8
{
    0x3E,   // Normal     // 8
    0xB1,   // Numlock
    0xB1,   // Fn
    0xB1    // Fn+Numlock
};

const CBYTE ss2_9E[] =   // Numpad 9
{
    0x46,   // Normal     // 9
    0xB2,   // Numlock
    0xB2,   // Fn
    0xB2    // Fn+Numlock
};

const CBYTE ss2_9F[] =   // Numpad 0
{
    0x45,   // Normal     // 0
    0xBE,   // Numlock
    0xBE,   // Fn
    0xBE    // Fn+Numlock
};

const CBYTE ss2_A0[] =   // Numpad U
{
    0x3C,   // Normal     // U
    0xB4,   // Numlock
    0xB4,   // Fn
    0xB4    // Fn+Numlock
};

const CBYTE ss2_A1[] =   // Numpad I
{
    0x43,   // Normal     // I
    0xB5,   // Numlock
    0xB5,   // Fn
    0xB5    // Fn+Numlock
};

const CBYTE ss2_A2[] =  // Numpad O
{
    0x44,   // Normal     // O
    0xB6,   // Numlock
    0xB6,   // Fn
    0xB6    // Fn+Numlock
};

const CBYTE ss2_A3[] =  // Numpad P
{
    0x4D,   // Normal     // P
    0xB3,   // Numlock
    0xB3,   // Fn
    0xB3    // Fn+Numlock
};

const CBYTE ss2_A4[] =   // Numpad J
{
    0x3B,   // Normal     // J
    0xB8,   // Numlock
    0xB8,   // Fn
    0xB8    // Fn+Numlock
};

const CBYTE ss2_A5[] =   // Numpad K
{
    0x42,   // Normal     // K
    0xB9,   // Numlock
    0xB9,   // Fn
    0xB9    // Fn+Numlock
};

const CBYTE ss2_A6[] =  // Numpad L
{
    0x4B,   // Normal     // L
    0xBA,   // Numlock
    0xBA,   // Fn
    0xBA    // Fn+Numlock
};

const CBYTE ss2_A7[] =   // Numpad ;:
{
    0x4C,   // Normal     // ;:
    0xB7,   // Numlock
    0xB7,   // Fn
    0xB7    // Fn+Numlock
};

const CBYTE ss2_A8[] =  // Numpad M
{
    0x3A,   // Normal     // M
    0xBC,   // Numlock
    0xBC,   // Fn
    0xBC    // Fn+Numlock
};

const CBYTE ss2_A9[] =   // Numpad ,<
{
    0x41,   // Normal     // ,<
    0x41,   // Numlock
    0x41,   // Fn
    0x41    // Fn+Numlock
};

const CBYTE ss2_AA[] =   // Numpad .>
{
    0x49,   // Normal     // .>
    0xBD,   // Numlock
    0xBD,   // Fn
    0xBD    // Fn+Numlock
};

const CBYTE ss2_AB[] =   // Numpad /?
{
    0x4A,   // Normal     // /?
    0xBB,   // Numlock
    0xBB,   // Fn
    0xBB    // Fn+Numlock
};

const CBYTE ss2_AC[] =
{
    0x5A,   // Normal
    0x81,   // Fn
};
//----------------------------------------------------------------
// Function key F1 ~ F12
//----------------------------------------------------------------
#if KB_FnFuncKey
// Force set2 to send combination key code to determine FnFunc in Hotkey function
              //Fn    //Fn
const CBYTE ss2_AD[] = {0xD0,  0xD0};  // F1
const CBYTE ss2_AE[] = {0xD1,  0xD1};  // F2
const CBYTE ss2_AF[] = {0xD2,  0xD2};  // F3
const CBYTE ss2_B0[] = {0xD3,  0xD3};  // F4
const CBYTE ss2_B1[] = {0xD4,  0xD4};  // F5
const CBYTE ss2_B2[] = {0xD5,  0xD5};  // F6
const CBYTE ss2_B3[] = {0xD6,  0xD6};  // F7
const CBYTE ss2_B4[] = {0xD7,  0xD7};  // F8
const CBYTE ss2_B5[] = {0xD8,  0xD8};  // F9
const CBYTE ss2_B6[] = {0xD9,  0xD9};  // F10
const CBYTE ss2_B7[] = {0xDA,  0xDA};  // F11
const CBYTE ss2_B8[] = {0xDB,  0xDB};  // F12
#else
            //Normal  //Fn
const CBYTE ss2_AD[] = {0x05,  0xD0};  // F1
const CBYTE ss2_AE[] = {0x06,  0xD1};  // F2
const CBYTE ss2_AF[] = {0x04,  0xD2};  // F3
const CBYTE ss2_B0[] = {0x0C,  0xD3};  // F4
const CBYTE ss2_B1[] = {0x03,  0xD4};  // F5
const CBYTE ss2_B2[] = {0x0B,  0xD5};  // F6
const CBYTE ss2_B3[] = {0x80,  0xD6};  // F7
const CBYTE ss2_B4[] = {0x0A,  0x77};  // F8
const CBYTE ss2_B5[] = {0x01,  0xD8};  // F9
const CBYTE ss2_B6[] = {0x09,  0xD9};  // F10
const CBYTE ss2_B7[] = {0x78,  0xDA};  // F11
const CBYTE ss2_B8[] = {0x07,  0xDB};  // F12
#endif

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Hook_calc_index_comb_BIT7
 *             6    Hook_calc_index_comb_BIT6
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
const A2_TABLE sskey2_A2_table[] =
{                      // Keyboard matrix index
  // Keypad -->
    {0x30, ss2_9C },    /* index = 00h */  // 0x9C
    {0x30, ss2_9D },    /* index = 01h */  // 0x9D
    {0x30, ss2_9E },    /* index = 02h */  // 0x9E
    {0x30, ss2_9F },    /* index = 03h */  // 0x9F
    {0x30, ss2_A0 },    /* index = 04h */  // 0xA0
    {0x30, ss2_A1 },    /* index = 05h */  // 0xA1
    {0x30, ss2_A2 },    /* index = 06h */  // 0xA2
    {0x30, ss2_A3 },    /* index = 07h */  // 0xA3
    {0x30, ss2_A4 },    /* index = 08h */  // 0xA4
    {0x30, ss2_A5 },    /* index = 09h */  // 0xA5
    {0x30, ss2_A6 },    /* index = 0Ah */  // 0xA6
    {0x30, ss2_A7 },    /* index = 0Bh */  // 0xA7
    {0x30, ss2_A8 },    /* index = 0Ch */  // 0xA8
    {0x30, ss2_A9 },    /* index = 0Dh */  // 0xA9
    {0x30, ss2_AA },    /* index = 0Eh */  // 0xAA
    {0x30, ss2_AB },    /* index = 0Fh */  // 0xAB
    {0x20, ss2_AC },    /* index = 10h */  // 0xAC
  // Keypad <--

  // F1~F12 hotkeys -->
    {0x20, ss2_AD },    /* index = 11h */  // 0xAD
    {0x20, ss2_AE },    /* index = 12h */  // 0xAE
    {0x20, ss2_AF },    /* index = 13h */  // 0xAF
    {0x20, ss2_B0 },    /* index = 14h */  // 0xB0
    {0x20, ss2_B1 },    /* index = 15h */  // 0xB1
    {0x20, ss2_B2 },    /* index = 16h */  // 0xB2
    {0x20, ss2_B3 },    /* index = 17h */  // 0xB3
    {0x20, ss2_B4 },    /* index = 18h */  // 0xB4
    {0x20, ss2_B5 },    /* index = 19h */  // 0xB5
    {0x20, ss2_B6 },    /* index = 1Ah */  // 0xB6
    {0x20, ss2_B7 },    /* index = 1Bh */  // 0xB7
    {0x20, ss2_B8 }     /* index = 1Ch */  // 0xB8
    // F1~F12 hotkeys <--
};
                      // Reserved 0xB9 ~ 0xDF

/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_overlay_table
 * ------------------------------------------------------------------------- */
const CBYTE sskey2_overlay_table[] =
{               //  Normal     Fn         // Keyboard matrix index
  0xC3, 0xC3,   //  Print Scr   Print Scr     // 0xE0
  0x77, 0x77,   //  Num Lk      Num LK       // 0xE1
  0xC0, 0xC0,   //  Del       Del            // 0xE2
  0xC2, 0xC2,   //  Insert     Insert        // 0xE3

  0x98, 0x98,   //  Up arrow   Function key (HotKey_Fn_Up)    // 0xE4
  0x99, 0x99,   //  Down arrow   Function key (HotKey_Fn_Down)   // 0xE5
  0x9A, 0x9A,   //  Left arrow  Volume Down event    // 0xE6
  0x9B, 0x9B,   //  Right arrow  Volume Up event    // 0xE7

  0x94, 0x94,   //  Home     Home      // 0xE8
  0x96, 0x96,   //  PgUp     PgUp       // 0xE9
  0x97, 0x97,   //  PgDn     PgDn      // 0xEA
  0x95, 0xC2,   //  End      End        // 0xEB

  0xDC, 0xDC,   //  ESC      Function key (HotKey_Fn_ESC)  // 0xEC
  0x29, 0xE1,   //  SPACE    Function key (HotKey_Fn_Space)  // 0xED

  0x5A, 0x5A,   //  Enter    Num-Enter    // 0xEE
  0x32, 0x32,   //  B        Break        // 0xEF
#if (Support_Keyboard == CS13KB)
  0xAB, 0xAB,   //  Caluator     Caluator      // 0xF0
  0xDD, 0xDD,   //  Lock       Lock         // 0xF1
  0xA5, 0xA5,   //  IE      IE        // 0xF2
  0xAC, 0xAC,   //  MyComputer  MyComputer    // 0xF3
#endif //CS13KB

#if (Support_Keyboard == CS16KB)
  0xDD, 0xDD,   //  Caluator     Caluator      // 0xF0
  0xDE, 0xDE,   //  Lock       Lock         // 0xF1
  0xDF, 0xDF,   //  IE      IE        // 0xF2
  0xE0, 0xE0,   //  MyComputer  MyComputer    // 0xF3
#endif //CSs16KB  

};
                        // Reserved 0xF4 ~ 0xFF

/* ----------------------------------------------------------------------------
 * Insyde Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 * ------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey3_80_table - Insyde Software Key Number 2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * ------------------------------------------------------------------------- */
const CBYTE sskey3_80_table[] =
{
//-----------------------------------------------------------------
// Index from keyboard matrix
//-----------------------------------------------------------------
                  // Keyboard matrix index
  0x83,   DO_SIMPLE_CODE,         // [80h] F7 (118)
  0x5A,   DO_E0_PREFIX_CODE,      // [81h] Numpad Enter (108)
  0x1F,   DO_E0_PREFIX_CODE,      // [82h] Windows Left
  0x27,   DO_E0_PREFIX_CODE,      // [83h] Windows Right
  0x2F,   DO_E0_PREFIX_CODE,      // [84h] Application
  0x37,   DO_E0_PREFIX_CODE,      // [85h] Power event
  0x3F,   DO_E0_PREFIX_CODE,      // [86h] Sleep event
  0x5E,   DO_E0_PREFIX_CODE,      // [87h] Wake event
  LShift, DO_CONTROL_EFFECT,      // [88h] 12 Shift(L)
  RShift, DO_CONTROL_EFFECT,      // [89h] 59 Shift(R)
  LAlt,   DO_CONTROL_EFFECT,      // [8Ah] 11 Alt(L)
  RAlt,   DO_CONTROL_EFFECT_E0,   // [8Bh] E0 11 Alt(R)
  LCtrl,  DO_CONTROL_EFFECT,      // [8Ch] 14 Crtl(L)
  RCtrl,  DO_CONTROL_EFFECT_E0,   // [8Dh] E0 14 Ctrl(R)
  FN,     DO_CONTROL_EFFECT,      // [8Eh] Fn
  OVLAY,  DO_CONTROL_EFFECT,      // [8Fh] Overlay
  0x4A,   DO_E0_PREFIX_CODE,      // [90h] Reserved msmart "key pad /"
  0x00,   DO_SPECIAL_CODE,        // [91h] Pause (126)
  0x01,   DO_SPECIAL_CODE,        // [92h] Break
  0x84,   DO_SIMPLE_CODE,         // [93h] (124) Alt-Case (SysRq)
  0x6C,   DO_E0_PREFIX_CODE,      // [94h] Home
  0x69,   DO_E0_PREFIX_CODE,      // [95h] End
  0x7D,   DO_E0_PREFIX_CODE,      // [96h] Page up
  0x7A,   DO_E0_PREFIX_CODE,      // [97h] Page down
  0x75,   DO_E0_PREFIX_CODE,      // [98h] UP arrow
  0x72,   DO_E0_PREFIX_CODE,      // [99h] Down arrow
  0x6B,   DO_E0_PREFIX_CODE,      // [9Ah] Left arrow
  0x74,   DO_E0_PREFIX_CODE,      // [9Bh] Right arrow

//-----------------------------------------------------------------
// For MicroSoft enhance keyboard feature.
//-----------------------------------------------------------------
  0x4D, DO_E0_PREFIX_CODE,        // [9Ch] Next Track event
  0x15, DO_E0_PREFIX_CODE,        // [9Dh] Prev Track event
  0x3B, DO_E0_PREFIX_CODE,        // [9Eh] Stop event
  0x34, DO_E0_PREFIX_CODE,        // [9Fh] Play/Pause event
  0x23, DO_E0_PREFIX_CODE,        // [A0h] Mute event
  0x32, DO_E0_PREFIX_CODE,        // [A1h] Volume Up event
  0x21, DO_E0_PREFIX_CODE,        // [A2h] Volume Down event
  0x48, DO_E0_PREFIX_CODE,        // [A3h] Mail event
  0x10, DO_E0_PREFIX_CODE,        // [A4h] Search event
  0x3A, DO_E0_PREFIX_CODE,        // [A5h] Web/Home event
  0x38, DO_E0_PREFIX_CODE,        // [A6h] Back event
  0x30, DO_E0_PREFIX_CODE,        // [A7h] Forward event
  0x28, DO_E0_PREFIX_CODE,        // [A8h] Stop event
  0x20, DO_E0_PREFIX_CODE,        // [A9h] Refresh event
  0x18, DO_E0_PREFIX_CODE,        // [AAh] Favorites event
  0x2B, DO_E0_PREFIX_CODE,        // [ABh] Caluator event
  0x40, DO_E0_PREFIX_CODE,        // [ACh] My Computer event
  0x50, DO_E0_PREFIX_CODE,        // [ADh] Media event
  0x00, DO_SIMPLE_CODE,           // [AEh] Reserved
  0x00, DO_SIMPLE_CODE,           // [AFh] Reserved
//-----------------------------------------------------------------
// Index from sskey2_A2_table
//-----------------------------------------------------------------
  0x6C, DO_SIMPLE_CODE,           // Numpad 7 Home  [B0h]
  0x75, DO_SIMPLE_CODE,           // Numpad 8 UP    [B1h]
  0x7D, DO_SIMPLE_CODE,           // Numpad 9 PGUp  [B2h]
  0x7C, DO_SIMPLE_CODE,           // Numpad *       [B3h]
  0x6B, DO_SIMPLE_CODE,           // Numpad 4 Left  [B4h]
  0x73, DO_SIMPLE_CODE,           // Numpad 5       [B5h]
  0x74, DO_SIMPLE_CODE,           // Numpad 6 Right [B6h]
  0x7B, DO_SIMPLE_CODE,           // Numpad -       [B7h]
  0x69, DO_SIMPLE_CODE,           // Numpad 1 End   [B8h]
  0x72, DO_SIMPLE_CODE,           // Numpad 2 Down  [B9h]
  0x7A, DO_SIMPLE_CODE,           // Numpad 3 PGDn  [BAh]
  0x79, DO_SIMPLE_CODE,           // Numpad +       [BBh]
  0x70, DO_SIMPLE_CODE,           // Numpad 0 Ins   [BCh]
  0x71, DO_SIMPLE_CODE,           // Numpad . Del   [BDh]
  0x4A, DO_E0_PREFIX_CODE,        // Numpad /       [BEh]

  0x00, DO_SIMPLE_CODE,           // Reserved     [BFh]

  0x71, DO_E0_PREFIX_CODE,        // Delete       [C0h]
  0x84, DO_SIMPLE_CODE,           // SysRq        [C1h]
  0x70, DO_E0_PREFIX_CODE,        // Insert       [C2h]
  0x7C, DO_E0_PREFIX_CODE,        // Print Screen [C3h]
  0x41, DO_E0_PREFIX_CODE,        // Euro Sign    [C4h]
  0x49, DO_E0_PREFIX_CODE,        // Dollar Sign  [C5h]
  0x00, DO_SIMPLE_CODE,           // Reserved     [C6h]
  0x00, DO_SIMPLE_CODE,           // Reserved     [C7h]
  0x00, DO_SIMPLE_CODE,           // Reserved     [C8h]
  0x00, DO_SIMPLE_CODE,           // Reserved     [C9h]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CAh]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CBh]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CCh]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CDh]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CEh]
  0x00, DO_SIMPLE_CODE,           // Reserved     [CFh] 

// Function key F1 ~ F12
  0  , DO_COSTOMER_FUNCTION,      // Function key F1  [D0h]
  1  , DO_COSTOMER_FUNCTION,      // Function key F2  [D1h]
  2  , DO_COSTOMER_FUNCTION,      // Function key F3  [D2h]
  3  , DO_COSTOMER_FUNCTION,      // Function key F4  [D3h]
  4  , DO_COSTOMER_FUNCTION,      // Function key F5  [D4h]
  5  , DO_COSTOMER_FUNCTION,      // Function key F6  [D5h]
  6  , DO_COSTOMER_FUNCTION,      // Function key F7  [D6h]
  7  , DO_COSTOMER_FUNCTION,      // Function key F8  [D7h]
  8  , DO_COSTOMER_FUNCTION,      // Function key F9  [D8h]
  9  , DO_COSTOMER_FUNCTION,      // Function key F10 [D9h]
  10  , DO_COSTOMER_FUNCTION,     // Function key F11 [DAh]
  11  , DO_COSTOMER_FUNCTION,     // Function key F12 [DBh]
  12  , DO_COSTOMER_FUNCTION,     // Function key Esc [DCh]
  13  , DO_COSTOMER_FUNCTION,     // Function key Up arrow    [DDh]
  14  , DO_COSTOMER_FUNCTION,     // Function key Down arrow  [DEh]
  15  , DO_COSTOMER_FUNCTION,     // Function key Left arrow  [DFh]
  16  , DO_COSTOMER_FUNCTION,     // Function key Right arrow [E0h]
  17  , DO_COSTOMER_FUNCTION,     // Function key Right arrow [E1h]
};
                  // Reserved 0xE2 ~ 0xFF

//-----------------------------------------------------------------
// The function of checking boot hotkey Dino 1024
//-----------------------------------------------------------------
#define DebugkeyFn     0x8E  // Fn
#define DebugKeyEqu    0x55  // =
#define DebugKeyS      0x1B  // S
#define DebugKeyA      0x1C  // A
#define DebugKeyR      0x2D  // R
#define DebugKeyN      0x31  // N
#define DebugKeyI      0x43  // I
#define DebugKeyO      0x44  // O
#define DebugKeyL      0x4B  // L  
#define DebugKeyE      0x24  // E
#define DebugKeyC      0x21  // C
#define DebugKeyM      0x3A  // M
#define DebugKeyB      0x32  // B


/* ----------------------------------------------------------------------------
 * FUNCTION: LcfcEdgeKeyOverrideTable
 * ------------------------------------------------------------------------- */

const FnKeyOverrideStruct LcfcEdgeKeyOverrideTable[] =
{ 
//  Normal      Fn         // Keyboard matrix index
  { 0x32,       0x92 },    // B,      Break
  { 0x1B,       0xC1 },    // S,      SysRq
  { 0x42,       0x7E },    // K,      Scr LK
  { 0x4D,       0x91 },    // P,      Pause
  { 0xE6,       0x94 },    // Left arrow ,   Home
  { 0xE7,       0x95 },    // Right arrow,   End
//  { 0x43,       0xC2 },    // I,      Insert
};

/* ----------------------------------------------------------------------------
 * FUNCTION: FN function Table
 * ------------------------------------------------------------------------- */
#if KB_FnSticky_Key
const CBYTE  FNFunctionTable[] =
{ 
  0x92,    //0x32,    //Fn+B,      Break
//  0x1B,    //Fn+S,      SysRq  no scancode
  0x7E,    //0x42,    //Fn+K,      Scr LK
  0x91,    //0x4D,    //Fn+P,      Pause
  0xAD,    //Fn+F1
  0xAE,    //Fn+F2
  0xAF,    //Fn+F3
  0xB0,    //Fn+F4
  0xB1,    //Fn+F5
  0xB2,    //Fn+F6
  0xB3,    //Fn+F7
  0xB4,    //Fn+F8
  0xB5,    //Fn+F9
  0xB6,    //Fn+F10
  0xB7,    //Fn+F11
  0xB8,    //Fn+F12
  0XEC,    //Fn+ESC 
//  0XE0,    //Fn+PrtSc        Qevent
//  0x25,    //Fn+4     sleep  Qevent
  0XF0,    //Fn+keypad hotkey1 
  0XF3,    //Fn+keypad hotkey4 
  0x82,    //Lwin keypad
  0x94,    // Home
  0x95,    // End
};
#endif //KB_FnSticky_Key

void FnEquECFunc (
  BYTE event
)
{
  SET_MASK(ComboKeyState, ECDebugMode);
}

void FnEquRAMFunc (
  BYTE event
)
{
  BYTE  OrigEcDbg;

  //
  // Check if entered EC_Debug_Mode to avoid end user mis-trigger.
  //
  if (IS_MASK_CLEAR(ComboKeyState, ECDebugMode)) {
    return;
  }

  //
  // Check if Port80 debug is pluged.
  //
  OrigEcDbg = GPCRE6;
  EC_TX_INPUT;
  if (Chk_DEBUG_IN()) {
    // Set EC pin16 and pin17 to alternate function for UART.
    SET_MASK(pEdgeProjectFlag1, pEdgeProjectDebugIn);
    EC_TX_ALT;
    EC_RX_ALT;
    // Dump all EC registers including LPC reg.

    DisableInternalWDT();
    DumpAllReg();
#if WDT_Support
    EnableInternalWDT();
#endif

    // Set EC pin16 and pin17 to output for Port80.
    EC_TX_OUTPUT;
    EC_RX_OUTPUT;
  }
  GPCRE6 = OrigEcDbg;

  CLEAR_MASK(ComboKeyState, ECDebugMode);
}

void FnEquSAFunc(BYTE event)
{

  BYTE CountIndex;
  BYTE Port80SaveIndex;
  BYTE EepromAddressToSavePort80History;
  BYTE Port80Count;

  
  Port80SaveIndex = Port80_Index; // The oldest Port80 records index. value would be 0x00 ~ 0x0E
  
  EepromAddressToSavePort80History = EEPROM_PORT80_HISTORY; // Save Port80 history from offset 0xC0 ~ (0xC0+CountIndex)
  Port80Count = 15; //sizeof(Port80_Data)/sizeof(Port80_Data[0]); // 15;

  if(IS_MASK_CLEAR(cSPIEEPROM,cSPI_ROMCopyStart)) {
    SET_MASK(CombineKeyStatus,HotkeyStoreROM);
    WRITE_DATA2EEPROM((UINT8 *)&BRAM3F, 1 ,EEPROM_Bank7, EEPROM_SAVE_PORT80);
    for (CountIndex = 0; CountIndex <= Port80Count - 1; CountIndex++) { 
      Port80SaveIndex = Port80_Index + CountIndex;
      if (Port80SaveIndex > (Port80Count - 1)) {
        Port80SaveIndex = Port80SaveIndex - (Port80Count - 1) - 1;
      }
      SET_MASK(CombineKeyStatus,HotkeyStoreROM);
      WRITE_DATA2EEPROM(&Port80_Data[Port80SaveIndex], 1 ,EEPROM_Bank7, (EepromAddressToSavePort80History + CountIndex));
    }
    Save_All_RamandGPIO();    
  }
}

void FnEquRNFunc(BYTE event)
{

}

void FnEquRNLFunc(BYTE event)
{
  SET_MASK(CombineKeyStatus, b2FnRNL);
}


void FnEquBLFunc(BYTE event)
{
  // (With PS/2 keyboards, you must enable the keyboard-initiated crash in the registry.
  // In the registry key HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\i8042prt\Parameters,
  // create a value named CrashOnCtrlScroll, and set it equal to a REG_DWORD value of 0x01.)

  //
  // Scroll scancode 0x46(Set1), 0x7E(Set2)
  //
  control_effect_e0 (RCtrl, MAKE_EVENT);  // Ctrl key pressed and hold
  simple_code (0x7E, MAKE_EVENT);         // Scroll key pressed
  simple_code (0x7E, BREAK_EVENT);        // Scroll key released
  simple_code (0x7E, MAKE_EVENT);         // Scroll key pressed
  simple_code (0x7E, BREAK_EVENT);        // Scroll key released
  //
  // It should become BSoD here, if registry be set correctly.
  //

  //
  // Release right Ctrl key if nothing happened.
  //
  control_effect_e0 (RCtrl, BREAK_EVENT);  // Ctrl key released
}

//----------------------------------------------------------------------------
// table_entry : value of keyboard matrix table. for example Rc_ROM_Tables[]
// event : key MAKE_EVENT or BREAK_EVENT or REPEAT_EVENT
//----------------------------------------------------------------------------
void OEM_Hook_Send_Key(BYTE *key_entry, BYTE event)
{
  BYTE  FnKeyOverrideIndex;
  BYTE  table_entry;
  table_entry = *key_entry;
//   if(SystemIsS0)
//   {
//     CheckBootHotKey(table_entry,event);
//     CheckRNHotKey(table_entry,event);
//     CheckIOHotKey(table_entry,event);
//   }

// --------------------------------
//               Fn
//               |
//               =
//               |
//         |   |    |   |  |
//         S   R    I   E  B
//         |   /\   |   |  |
//         A  A  N  O   C  /
//            |  |        /
//            M  L  -----
// --------------------------------
// ==== Top-down ====
// Fn	-> =
// =	-> S, R, I, E, B
// S	-> A
// A	-> M, (SA func)
// R	-> N, A
// N	-> L, (RN func)
// I	-> O
// O	-> (IO func)
// L	-> (RNL func)
// E	-> C
// C	-> (EC func)
// M	-> (RAM func)
// B  -> L, (BL func)
//
// ==== Bottom-up ====
// A	-> S, R
// M	-> A
// L	-> N, B
// O	-> I
// C	-> E
// N	-> R
// S	-> =
// R	-> =
// I	-> =
// E	-> =
// B  -> =
// =	-> Fn
  if ((ComboKeyState & 0x1F) && (event == BREAK_EVENT)) { // 0x1F means bit-mask 0:4 [00011111]b
    switch (table_entry) {

      case DebugKeyEqu: // =
        if (LastComboKey == DebugkeyFn) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyS: // S
        if (LastComboKey == DebugKeyEqu) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyA: // A
        if (LastComboKey == DebugKeyS) {
          ComboKeyState++;
          LastComboKey = table_entry;
          SET_MASK(ComboKeyState, CallingFunc);
          FnEquSAFunc (event);
        } else if(LastComboKey == DebugKeyR) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyR: // R
        if (LastComboKey == DebugKeyEqu) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyN: // N
        if (LastComboKey == DebugKeyR) {
          ComboKeyState++;
          LastComboKey = table_entry;
          if (SystemIsS0) {
            SET_MASK(ComboKeyState, CallingFunc);
            FnEquRNFunc (event);
          }
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyI: // I
        if (LastComboKey == DebugKeyEqu) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyO: // O
        if (LastComboKey == DebugKeyI) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyL: // L
        if (LastComboKey == DebugKeyN) {
          ComboKeyState++;
          LastComboKey = table_entry;
          if (SystemIsS0) {
            SET_MASK(ComboKeyState, CallingFunc);
            FnEquRNLFunc (event);
          }
        } else if (LastComboKey == DebugKeyB) {
          ComboKeyState++;
          LastComboKey = table_entry;
          if (SystemIsS0 && IS_MASK_SET(SYS_MISC1, ACPI_OS)) {
            SET_MASK(ComboKeyState, CallingFunc);
            FnEquBLFunc (event);
          }
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyE: // E
        if (LastComboKey == DebugKeyEqu) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyC: // C
        if (LastComboKey == DebugKeyE) {
          ComboKeyState++;
          LastComboKey = table_entry;
          SET_MASK(ComboKeyState, CallingFunc);
          FnEquECFunc (event);
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyM: // M
        if (LastComboKey == DebugKeyA) {
          ComboKeyState++;
          LastComboKey = table_entry;
          SET_MASK(ComboKeyState, CallingFunc);
          FnEquRAMFunc (event);
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      case DebugKeyB: // B
        if (LastComboKey == DebugKeyEqu) {
          ComboKeyState++;
          LastComboKey = table_entry;
        } else {
          ComboKeyState &= 0x80;
        }
        break;

      default:
        ComboKeyState &= 0x80;
        LastComboKey = 0xFF;
        break;
    }

    EC_TX_INPUT;
    if (Chk_DEBUG_IN()) {
      EC_TX_ALT;
      printf ("\rkey = 0x%02bX, ", table_entry);
      printf ("ComboKeyState = 0x%02bX   \n\r", ComboKeyState);
      EC_TX_OUTPUT;
    }
    CLEAR_MASK(ComboKeyState, CallingFunc);
  }
  if ((temp_scanner_state.field.tFN)|| (Test109 == 0x01)) { // Fn key + any key
    for (FnKeyOverrideIndex = 0; 
         FnKeyOverrideIndex < (sizeof(LcfcEdgeKeyOverrideTable)/sizeof(LcfcEdgeKeyOverrideTable[0])); 
         FnKeyOverrideIndex++) {
      //
      // Search override table
      //
      if (*key_entry == LcfcEdgeKeyOverrideTable[FnKeyOverrideIndex].NormalKey) {
        //
        // Once the pressed key is matched, replace to override key.
        //
        *key_entry = LcfcEdgeKeyOverrideTable[FnKeyOverrideIndex].OverrideKey;
		if(event == MAKE_EVENT ){
//			if((table_entry == 0x1B)||(table_entry == 0x42)||(table_entry == 0x4D)||(table_entry == 0x32)){
//				Test109 = 0x00;
//  			}else{
				Test109 = 0x01;
//  			}
		}else if(event == BREAK_EVENT){ 
			Test109 = 0x00;
		}
        break;
      }
    }

  //
  // A special prcoess for Fn + S to send SysRq.
  // According to AdditionalRequirementForEdgePlusKBD.ppt, 
  // SysRq needs to send "11 84" for make event and "F0 84 F0 11" for break event.
  //

    if ((table_entry == 0x1B) && (*key_entry == 0xC1)) {
      //
      // Make sure we are processing Fn + S -> SysRq.
      // 0x1B => S, 0xC1 => SysRq
      //
      switch (event) {
        case MAKE_EVENT:
          simple_code (0x11, MAKE_EVENT); // send 0x11
          simple_code (0x84, MAKE_EVENT); // send 0x84
          
          simple_code (0x84, BREAK_EVENT); // send F0 84
          simple_code (0x11, BREAK_EVENT); // send F0 11
          break;
        case BREAK_EVENT:

        default:
          break;
      }
      *key_entry = 0; // The scan code is done, clean key to NULL.
    }
    if (table_entry == 0x25)	//Fn+4  Sleep
    {
     	if(event == MAKE_EVENT)
    	  {
    		  Buffer_Key(0xE0);
	     	Buffer_Key(0xF0);          //     
	     	Buffer_Key(0x5E);
	     	F_Service_SEND = 1; 
      		ECQEvent(H4_Suspend);    // 0x13 Sleep mode(S3).
        	SET_MASK(CombineKeyStatus, b7Fn4);
			  SET_MASK(CombineKeyStatus, b6Fn4Key4);
    	  }
      	*key_entry = 0; // The scan code is done, clean key to NULL.
    }    

#if (Support_Keyboard == CS16KB)
    if ((table_entry == 0xE0) && ( event == MAKE_EVENT ))	//Fn+PrtSc  
    {
      ECQEvent(HOTKEY_PrtSc);    // 0x13 Sleep mode(S3).
    }  
#endif //(Support_Keyboard == CS16KB)     
   
  }
}

//----------------------------------------------------------------------------
// Return : 0xFF --> Send_Key function will be break; (no any scan code to host)
//----------------------------------------------------------------------------
BYTE OEM_Hook_Skip_Send_Key(BYTE *key_entry, BYTE event)
{
#if KB_FnSticky_Key
  BYTE  FNFunctionIndex;
  if (STICKY_MODE_GET | STICKY_LOCK_GET) { 
    for (FNFunctionIndex = 0; 
         FNFunctionIndex < (sizeof(FNFunctionTable)); 
         FNFunctionIndex++) {
      //
      // Search FNFunctionTable
      //
      if (*key_entry == FNFunctionTable[FNFunctionIndex]) {
        return(0x00);
      } 
    }
    return(0xFF);     
  }
#endif  //KB_FnSticky_Key  
  return(0x00);
}

//-----------------------------------------------------------------------------
// Keyboard hook function (KSO0 - KSO15)
// KSIValus : keyboard KSI
// KSOValus : keyboard KSO
//-----------------------------------------------------------------------------
void Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{

}

void Hook_keyboard_Oem(uKEY *key,  BYTE event)
{
  if( FnCtrlSWAPGet )
  {
    if((key->field.input== 0x03)&&(key->field.output == 0x03))// LCtrl
    {
      // Replaced by Fn key
      key->field.input = 0x04;
      key->field.output = 0x0E; // Fn
    } else if((key->field.input == 0x04)&&(key->field.output == 0x0E)) { // Fn
      // Replaced by LCtrl key
      key->field.input = 0x03;
      key->field.output = 0x03; // LCtrl
    }
  }

#if KB_FnSticky_Key

//  if(IS_MASK_SET(SpecialCodeState, SpecialCodeActive) && IS_MASK_SET(SYS_MISC1, ACPI_OS) && FnStickyGET && ((key->field.input == 0x04)&&(key->field.output == 0x0E)) && (event==BREAK_EVENT)){
//    CLEAR_MASK(SpecialCodeState, SpecialCodeActive);
//    return;
//  }

  //if (IS_MASK_SET(SYS_MISC1, ACPI_OS) && FnStickyGET && ((key->field.input == 0x04)&&(key->field.output == 0x0E)) && (event==BREAK_EVENT)){  //sticky support & Fn press //A485D00015-
//A485D00015: Modify FN Key Loc for Fn key no function
if (IS_MASK_SET(SYS_MISC1, ACPI_OS) && FnStickyGET && ((key->field.input == 0x04)&&(key->field.output == 0x0C)) && (event==BREAK_EVENT)){  //sticky support & Fn press  //A485D00015+
//  Fn key status 
//  00: unlock
//  01: sticky
//  10: lock  
    switch ( u4PeriSTS4 & (STICKY_MODE+STICKY_LOCK)){
      case 0:   //unlock -> sticky
         STICKY_MODE_SET;
         ECSMI_SCIEvent(FN_PRESS_EVENT_70);	
         break;
      case 1:   //sticky -> lock
         FnStickKey = 0;
         STICKY_MODE_CLR;
         STICKY_LOCK_SET;
         ECSMI_SCIEvent(STICKY_ON_EVENT_72);  
         break;
      case 2:   //lock -> unlock
         FnStickKey = 0;
         STICKY_LOCK_CLR;
         ECSMI_SCIEvent(STICKY_OFF_EVENT_73); 
         break;
      default:
         break;
    }    
  }
#endif //KB_FnSticky_Key

}

//-----------------------------------------------------------------------------
// Keyboard hook function (KSO16 - KSO17 and GPIO KSO)
// KSIValus : keyboard KSI
// KSOValus : keyboard KSO
//-----------------------------------------------------------------------------
void Et_Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{

}

/* ---------------------------------------------------------------------------
 * FUNCTION: Hook_Setup_Scanner_Pntr - to define the possible scanner tables
 *
 *     0   Rc_RAM_Table
 *     1   ROM Table 1
 *     ...
 * ------------------------------------------------------------------------ */
void Hook_Setup_Scanner_Pntr(void)
{
  #if SUPPORT_KB_RAM_TABLE
  Scanner_Table_Pntr = KB_RAM_Table;
  Extendkey_Table_Pntr = ExKB_RAM_Table;
  Scanner_LOC_Pntr   = KB_LOC_Table;  
  LoadKBMatrixToRAM();
  #else
  Scanner_Table_Pntr = &Rc_ROM_Tables;
  Extendkey_Table_Pntr = &EtKey_ROM_Tables;
  #endif
}

/* ----------------------------------------------------------------------------
 * FUNCTION: LoadKBMatrixToRAM
 * ------------------------------------------------------------------------- */
void LoadKBMatrixToRAM(void)
{
    if ( IS15Inch )
    {
    if ( (cKBStatus & 0x03) != 0x03 )
    {
          for( ITempB06=0; ITempB06<0x80; ITempB06++ )  //15" US,UK and JP.
          {
             // Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
             // *Tmp_XPntr = Rc_ROM_Tables[ITempB06];
              *( KB_RAM_Table + ITempB06 ) = Rc_ROM_Tables[ITempB06];
          }
    }
    else
    {
      for( ITempB06=0; ITempB06<0x80; ITempB06++ )  //15" Brazil.
          {
              //Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
              //*Tmp_XPntr = Rc_ROM_Tables_15BR[ITempB06];
              *( KB_RAM_Table + ITempB06 ) = Rc_ROM_Tables_15BR[ITempB06];
          }
    }

        for( ITempB06=0; ITempB06<0x18; ITempB06++ )
        {
           // Tmp_XPntr = ( ExKB_RAM_Table + ITempB06 );
           // *Tmp_XPntr = EtKey_ROM_Tables[ITempB06];
           *( ExKB_RAM_Table + ITempB06 ) = EtKey_ROM_Tables[ITempB06];

           
        }
    }
    else
    {
    if ( (cKBStatus & 0x03) != 0x03 )
    {
          for( ITempB06=0; ITempB06<0x80; ITempB06++ )  //14" US,UK and JP.
          {
             // Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
             // *Tmp_XPntr = Rc_ROM_Tables_14[ITempB06];
              *( KB_RAM_Table + ITempB06 ) = Rc_ROM_Tables_14[ITempB06];
          }
    }
    else
    {
      for( ITempB06=0; ITempB06<0x80; ITempB06++ )  //14" Brazil.
          {
              //Tmp_XPntr = ( KB_RAM_Table + ITempB06 );
              //*Tmp_XPntr = Rc_ROM_Tables_14BR[ITempB06];
               *( KB_RAM_Table + ITempB06 ) = Rc_ROM_Tables_14BR[ITempB06];
          }
    }

        for( ITempB06=0; ITempB06<0x18; ITempB06++ )
        {
            //Tmp_XPntr = ( ExKB_RAM_Table + ITempB06 );
            //*Tmp_XPntr = EtKey_ROM_Tables_1[ITempB06];
             *( ExKB_RAM_Table + ITempB06 ) = EtKey_ROM_Tables_1[ITempB06];
        }
    }
   	for( ITempB06=0; ITempB06<0x80; ITempB06++ )
    {    
      *( KB_LOC_Table + ITempB06 ) = Rc_LOC_Tables[ITempB06];
   	}
	
}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin to high
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinH(void)
{

}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin to low
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinL(void)
{

}

//-----------------------------------------------------------------------------
// The function of setting GPIO scan pin output
//-----------------------------------------------------------------------------
void Hook_SetGPIOScanPinCtrl(void)
{

}

//-----------------------------------------------------------------------------
// Fn key make
//-----------------------------------------------------------------------------
void Hook_Fn_Key_Make(void)
{
  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
       Buffer_Key(0xE0);                   //
         Buffer_Key(0x16);                  // page down make
       F_Service_SEND = 1;
  }
  else {
    Buffer_Key(0xE0);
    Buffer_Key(0x5E);
    F_Service_SEND = 1;
  }
  FnStatus = 1;              // for factory

}

//-----------------------------------------------------------------------------
// Fn key break
//-----------------------------------------------------------------------------
void Hook_Fn_Key_Break(void)
{
  if (IS_MASK_SET(cCmd,cC_TestBtnEn))
  {
    Buffer_Key(0xE0);          //
    Buffer_Key(0xF0);          //
    Buffer_Key(0x16);          // page down make
    F_Service_SEND = 1;
  }
  else {
  	  if(IS_MASK_SET(CombineKeyStatus, b7Fn4))
	  {
		  CLEAR_MASK(CombineKeyStatus, b7Fn4);
	  }
  	  else {
     	Buffer_Key(0xE0);
     	Buffer_Key(0xF0);          //     
     	Buffer_Key(0x5E);
     	F_Service_SEND = 1;
  	  }
  }
  FnStatus = 0;              // for factory

  if ( IS_MASK_SET(SysStatus, FnF3PKeyUp) )
  {
    SysStatus &= 0xF3;           // Clear Win+P Key.
    Buffer_Key(0xE0);          //
    Buffer_Key(0xF0);          //
    Buffer_Key(0x1F);          // Win Key
    F_Service_SEND = 1;
  }
}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit6
//-----------------------------------------------------------------------------
BYTE Hook_calc_index_comb_BIT6(void)
{
    return(0x00);
}

//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit7
//-----------------------------------------------------------------------------
BYTE Hook_calc_index_comb_BIT7(void)
{
    return(0x00);
}

#if KB_FnFuncKey
BYTE Oem_CheckFxkeyMode(void)
{
#if KB_FnSticky_Key 
    if (FnStickyGET){
      if (u4KBDID == MODERN_TOP_ROW)   // Fn key as primary
        {return (!(STICKY_MODE_GET || STICKY_LOCK_GET));}
      else
        return (STICKY_MODE_GET || STICKY_LOCK_GET);
    }
#endif //KB_FnSticky_Key
    if ((temp_scanner_state.field.tFN || (IS_MASK_SET(EVT_STATUS1,KB_FN_Status)&&(!FnCtrlSWAPGet)))){
      if (u4KBDID == MODERN_TOP_ROW){
        return(FNLKSTATEGET);
      }else {
        return(!FNLKSTATEGET);
      }
    }else {
      if (u4KBDID == MODERN_TOP_ROW){
        return(!FNLKSTATEGET);
      }else {
        return(FNLKSTATEGET);
      }
    }
}
#endif



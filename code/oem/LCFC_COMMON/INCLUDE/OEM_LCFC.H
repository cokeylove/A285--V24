/*
--------------------------------------------------------------------
  Filename: OEM_LCFC.H

  Purpose : LCFC define

  Author  : Ivan Chen 2015/10/01
--------------------------------------------------------------------
*/

#ifndef OEM_LCFC_H
#define OEM_LCFC_H
//------------------------------------------------------------------

//for debug 
#define MAX_power_state  Debug_Test_3FF
#define MAX_power_Debug  Debug_Test_3FB


//TPG spec version
#define TPGPM_Major    0x04  // TPG PM Major version
#define TPGPM_Minor    0x50  // TPG PM Minor version

//Specification version is in a word, 0EFh and 0EBh. Major version is in 0EFh and Minor version is in 0EBh.
#define OEM_SPECVERSION  0x0320    //V3.20

//Cmd_41_A1   report Platform ID  "COMPAL"     Don't change, if is not "Compal" the system onoff can't run
#define OEM_PLATFORMID "COMPAL"   

//SMBUS setting
#define  HOCTL2_A_REG 0x03  // 0x1C10   Host Control2 A
#define  HOCTL_A_REG 0x01   // 0x1C01   Host Control A
#define  HOSTA_A_REG 0xFF   // 0x1C00   Host Status A
#define  HOCTL2_B_REG 0x01  // 0x1C21   Host Control2 B
#define  HOCTL_B_REG 0x01   // 0x1C12   Host Control B
#define  HOSTA_B_REG 0xFF   // 0x1C11   Host Status B
#define  HOCTL2_C_REG 0x01  // 0x1C32   Host Control2 C
#define  HOCTL_C_REG 0x01   // 0x1C2A   Host Control C
#define  HOSTA_C_REG 0xFF   // 0x1C29   Host Status C
#define  SCLKTS_A_REG 0x03  // 0x1C40   SMCLK Timing Setting Register A


//ITE chip Reset status(RSTS)
#define IT8586_Init_RSTS 0x84
#define IT8986_Init_RSTS 0x44
#if (EC_CHIP == IT8586)
#define Core_Init_RSTS   IT8586_Init_RSTS
#endif 
#if (EC_CHIP == IT8986)
#define Core_Init_RSTS   IT8986_Init_RSTS
#endif 

//------------------------------------------------------------------
#endif

/*----------------------------------------------------------------------------
 * Filename: OEM_Power.H  For Chipset: ITE.IT85XX
 *
 * Function: Power sequnce control function example
 *
 * Author  : Dino
 *
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef OEM_POWERSEQ_H
#define OEM_POWERSEQ_H

extern void Oem_SysPowerContrl(void);
extern void TrigRsmrstInDcMode (void);
extern void Oem_S5S0Sequence(void);
extern void Oem_S0S5Sequence(void);
extern void Oem_S3S0Sequence(void);
extern void Oem_S0S3Sequence(void);
extern void CheckAutoPowerOn(void);
extern void S34toDSx(void);
extern void DSxtoS34(void);
extern void S0S3_VR_ON(void);
extern void Check_EC_ON();
extern void ChipSettingBeforeSleep();
extern BYTE Sequence_Null();

extern void Oem_PowerSequence(BYTE SxSeq);

typedef struct PowerSEQ
{
  FUNCT_PTR_B_V  func;  // Do step function
  WORD  delay;          // delay time (ms)
  BYTE  checkstatus;    // step function is check pin status (EX: slp_s3 HWPG ...)
} sPowerSEQ;



typedef struct _PowerSEQTableList {
  const sPowerSEQ       *PowerSEQTable;
  BYTE            PowerSEQCount;
} PowerSEQTableList;

enum {
  POWERSEQS5toS0 = 0,  // 0 -- S5toS0
  POWERSEQS0toS5,      // 1 -- S0toS5
  POWERSEQS3toS0,      // 2 -- S3toS0
  POWERSEQS0toS3       // 3 -- S0toS3
};
#endif

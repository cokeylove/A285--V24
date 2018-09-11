//*****************************************************************************
//
//  oem_fan.c
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************


extern const FanTableList  *  FanTblPtr;

/*****************************************************************************/
// Procedure: ThmIC_Temperature                TimeDiv: 500mSec
// Description: Read RAM temperature
// GPIO:
// Referrals:
/*****************************************************************************/
void ThmIC_Temperature(void)
{
  BYTE BsData,BTmlCmd;

  if( SystemNotS0 )
  { return; }

  switch( TmlICStep & 0x03 )
  {
    case 0:
      BTmlCmd = VRAM_Temp;
      break;
    case 1:
      BTmlCmd = DRAM_Temp;
      break;
    case 2:
      BTmlCmd = CPUcore_Temp;
      break;
    case 3:
#if (PLATFORM_CHIP == INTEL)
      if(IS_MASK_CLEAR(pProject3,pIsMeFlashing)){ //Enable ME	//To avoid the PCH communication fail during ME flashing.      	
        Tml_SMLink();
      }else{    //Disable ME
        TmlICStep = 0;
      }
#else
      TmlICStep = 0;
#endif // PLATFORM_CHIP
      return;
    default:
      TmlICStep = 0;
      return;
  }

  if(!bRWSMBus(chSMB_THM, SMbusRB, TmlIC_Addr, BTmlCmd, &BsData, 0))			
  {
    TMErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b0ThmICError) )// Check bit0 is Error.
    {
      if ( TMErrCnt > 7 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b0ThmICError);  // Set bit0 Thermal IC read temperature error.
      }
    }
    else
    {
      if ( TMErrCnt > 68 )  // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(THERMALCOMMFAIL_ID);  // 0x0B shutdown ID.				
        TMErrCnt = 0;          // Clear error count.
        RSMRST_shutdown();													
      }
    }
    ResetSMBus(chSMB_THM);													
  }
  else
  {
      switch( TmlICStep & 0x03 )
      {
        case 0:
          nVramTemp = BsData;  // Save VRAM temperature.
          TMErrCnt = 00;    // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;    // next step 1.
          break;
        case 1:
          nRamTemp = BsData;  // Save DRAM temperature.
          TMErrCnt = 00;    // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;    // next step 2.
          break;
        case 2:
	   nCpuTempLocal = TEMP_TS_2; //A285D00051: new fan table spec implementation
	   TEMP_TS_2 = TEMP_TS_1;
	   TEMP_TS_1 = BsData;
	   // 3s calculate per times - A285D00051: new fan table spec implementation
          nCpuTempLocal = (nCpuTempLocal + TEMP_TS_2 + TEMP_TS_1) / 3;  // Save Cpu Local temperature.
          TMErrCnt = 00;      // Clear error count.
          CLEAR_MASK(ERR_THMSTS,b0ThmICError);  // Clear bit0.
          TmlICStep++;      // next step 3.
          //break;
        default:
          break;
      }
  }

}
//-----------------------------------------------------------------------------
// Service short time PWM output, base on 50ms.
//-----------------------------------------------------------------------------
#if (PLATFORM_CHIP == INTEL)
void Tml_SMLink(void)
{
  if( !bRSMBusBlock(chSMB_THM, SMbusRBK, Intel_PCH_Addr, 0x40, &SMB3_DATA, bRead_I2C_NULL))//, 0x14, TRUE))			
  {
    PCHErrCnt++;
    if( IS_MASK_CLEAR(ERR_THMSTS, b4PCHThmError) )// Check bit4 is Error.
    {
      if ( PCHErrCnt > 7 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b4PCHThmError);  // Set bit4 PCH Thermal read temperature error.
      }
    }
    else
    {
      if ( PCHErrCnt > 68 )  // 30 Sec.
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(THERMALCOMMFAIL_ID);  // 0x0B shutdown ID.									
        PCHErrCnt = 0;          // Clear error count.
        RSMRST_shutdown();																	
      }
    }
    ResetSMBus(chSMB_THM);								
  }
  else
  {
  	*(DWORD*)&MaxCPU_MCHTemp0 = *(DWORD*)&SMB3_DATA[0];
    PCH_Temp3 = MaxCPU_MCHTemp0;        // Save PCH temperature.
    CLEAR_MASK(ERR_THMSTS,b4PCHThmError);  // Clear bit4.
    PCHErrCnt = 0;              // Clear error count.
    TmlICStep = 0;              // Reset Step.
  }
}
#endif // PLATFORM_CHIP

// ----------------------------------------------------------------------------
// The function of PECI3.0 CPU temperature read
// ----------------------------------------------------------------------------
void SBTSI_ReadCPUTemp(void)
{
  BYTE cputemp;

  if(SystemNotS0) {
    return;
  }
  if(!bRWSMBus(chSMB_AMDTSI, SMbusRB, AMD_SBTSI_Addr, 0x01, &cputemp, 0))				
  {
    PECI_Err_CNT++;  // increase read fail count
    if( IS_MASK_CLEAR(ERR_THMSTS, b3PECITempEr) ) // Check bit3 is Error.
    {
      if ( PECI_Err_CNT > 3 )    // 3 Sec.
      {
        SET_MASK(ERR_THMSTS, b3PECITempEr);  // Set bit3 CPU read temperature error.
      }
    }
    else
    {
      if ( PECI_Err_CNT > 34 )    // 30 Sec.
      {
#if !EN_PwrSeqTest  //A285D00057:
        #if FAN_CTRL_CPUFAIL_ALERT
        FAN_PWM_OUT;      // Set FAN_PWM OUTPUT.
        EC_FAN_PWM_HI();
        BEEP_315HZ;
        PECI_Err_CNT = 35;//always beep but would not over
        #else//A285D00057:
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(CPUCOMMFAIL_ID);  // 0x22 shutdown ID.				
        PECI_Err_CNT = 0;      // Clear error count.
        RSMRST_shutdown();				
        #endif
#endif  // EN_PwrSeqTest
      }
    }
    ResetSMBus(chSMB_AMDTSI);					
  }
  else
  {
    u4CpuThermal = cputemp;   // Get CPU temperature.
    PECI_Err_CNT = 0;   // Clear error count.
    CLEAR_MASK(ERR_THMSTS, b3PECITempEr); // Clear bit3 CPU read temperature error.
    if ( IS_MASK_CLEAR(Fan_Debug_Temp,b0CPU_Temp) )  // Debug CPU Temperature, Engineer myself control.
    {
        //A285D00051: new fan table spec implementation
      TEMP_Buff_3 = TEMP_Buff_5;
      TEMP_Buff_5 = TEMP_Buff_4;
      TEMP_Buff_4 = TEMP_Buff_2;
      TEMP_Buff_2 = TEMP_Buff_1;
      TEMP_Buff_1 = u4CpuThermal;
     //5s read calculate per times  - A285D00051: new fan table spec implementation
      TEMP_Buff_3 = (TEMP_Buff_1 + TEMP_Buff_2 + TEMP_Buff_3 + TEMP_Buff_4 + TEMP_Buff_5)/5;  // CPU average temperature.
    }
  }
}
/*****************************************************************************/
// Procedure: Oem_Thermal_Control                TimeDiv: 1 Sec
// Description: Read CPU temperature
// GPIO: GPIOF6(118)
// Referrals:
/*****************************************************************************/
void Oem_Thermal_Control(void)
{
  if(SystemIsS0)
  {
#if (PLATFORM_CHIP == INTEL)
//
// Intel PECI
//
    if(TJMAX == 0x00)
    {
      PECI_ReadCPUTJMAX();
      CPU_TYPE = CPU_TYPE & 0xFC; //
      if (TJMAX == 105)      //105
        CPU_TYPE |= 0x03;
      else if (TJMAX == 100)    //100
        CPU_TYPE |= 0x02;
      else if (TJMAX == 90)    //90
        CPU_TYPE |= 0x01;
    }
    else
    { PECI_ReadCPUTemp(); }
#else // PLATFORM_CHIP
//
// AMD SB-TSI
//
    if(TJMAX == 0x00) {
      //
      // TBD -- To determine TJMAX
      // Current, set default TJMAX = 100 for AMD Kaveri AAxE1 projects.
      //
      TJMAX = 100;
//       CPU_TYPE = CPU_TYPE & 0xFC;
//       if (TJMAX == 105)      //105
//         CPU_TYPE |= 0x03;
//       else if (TJMAX == 100)    //100
//         CPU_TYPE |= 0x02;
//       else if (TJMAX == 90)    //90
//         CPU_TYPE |= 0x01;
    } else {
      SBTSI_ReadCPUTemp();
    }
#endif // PLATFORM_CHIP

  }
   //A285D00052: add stp(skin temperature protection) function for thermal
    ThermalSTBProcess();    //process once per-second.
}

/*****************************************************************************/
// Procedure: Show Real PWM duty,Target PWM duty (BCD),
//                 Real FAN RPM , Target FAN RPM (BCD)
/*****************************************************************************/
//unsigned char hex2bcd (unsigned char x)
//{
//   unsigned char y;
//    y = (x / 10) << 4;
//    y = y | (x % 10);
//    return (y);
//}

BYTE hex2bcd (BYTE x)
{
    BYTE y,z;
    y = x / 10 ;
  z = x - (y*10);
    x = (y << 4) | z;
    return (x);
}

void Show_realPWM(void)
{
  WORD Temp16;
  FAN_PWM_BCD = FAN_PWM >>1;              // duty = PWM/2
//  FAN_PWM_BCD = ((WORD)((FAN_PWM+1) *100))/CTR2;    // duty = PWM/185 *100
  FAN_PWM_BCD = hex2bcd(FAN_PWM_BCD);
//  if (Thermal_monitor_Get){
    Temp16 = nAtmFanSpeed*100;     
    u4FanSpdL = Temp16 & 0xFF;
    u4FanSpdH = ((Temp16 & 0xFF00) >> 8);
//  }else{
//    u4FanSpdH = hex2bcd(nAtmFanSpeed);
//    u4FanSpdL = 0;    
//  }
}

void Show_targetduty(void)
{
  Target_Duty_BCD = Target_Duty >>1;              // Target_Duty/2
//  Target_Duty_BCD = ((WORD)((Target_Duty+1) *100))/CTR2;    // ((Target_Duty+1)*100)/200
  Target_Duty_BCD = hex2bcd(Target_Duty_BCD);
  TargetRPM_BCD= hex2bcd(TargetRPM);
}

/*****************************************************************************/
// Procedure: Oem_Fan_Speed                    TimeDiv: 1 Sec
// Description: Read Fan RPM
// GPIO: GPIOD6(47)
// Referrals:
/*****************************************************************************/
void Oem_Fan_Speed(void)
{
  WORD Curr_Fan1Tachometer;
  BYTE tach_H;
  BYTE tach_L;
 
  tach_H = F1TMRR;
  tach_L = F1TLRR;
  
  if(SystemIsS0)
  {
    if ( (tach_H == 0) && (tach_L == 0) )
    {
      nAtmFanSpeed = 0;
    }
    else
    {
//      Curr_Fan1Tachometer = (36000/((WORD)(F1TMRR<<8) + F1TLRR))*60;
      Curr_Fan1Tachometer = (FANSPEEDPOLEFac/((WORD)(tach_H<<8) + tach_L));
      nAtmFanSpeed = ( (Curr_Fan1Tachometer+50) / 100 );          // 2013.4.1 CAC. update
    }
    if( (Fan_ON_Count!=0) && (FAN_PWM!=0) )
    {
      if( nAtmFanSpeed > 0 )
      {
        FanExistEN;
        //[Project][CC20130503][Start]Fix abnormal show fan error message on power on status.
        FanInitEN;
        //[Project][CC20130503][End]Fix abnormal show fan error message on power on status.
        if( nAtmFanSpeed < 6 )
        {
          FanErrEN;
        }
        else
        {
          FanErrDIS;
        }
      }
      else
      {
        FanExistDIS;
      }
    }
    if ( IS_MASK_CLEAR(uReserve07,nFanManual))                  // 2013.4.18 CAC. update
    {
      if  (PWM_CHG_delay==0)
      {
        if ((nAtmFanSpeed != TargetRPM)&&(Fan_ON_Count == 0))
        {
          if( nAtmFanSpeed > TargetRPM )
          {
            if( FAN_PWM > 0 )
            { FAN_PWM--; }
          }
          else
          {
            if(FAN_PWM < FAN_PWM_Max)
            { FAN_PWM++; }
            else
            { FAN_PWM = FAN_PWM_Max; }
          }
        }
      }
      else
      {
        PWM_CHG_delay--;
      }
    }
      Show_realPWM();                              // 2013.4.18 CAC. just for test
      Show_targetduty();
  }
}

//-----------------------------------------------------------------------------
// Fan Table, Check Duty offset 1% or 2%.
//-----------------------------------------------------------------------------
void Fan_Offset_Chk(BYTE Duty_Offset, uchar_8 * BDCRx)
{
  if ( Duty_Offset != *BDCRx )
  {
    *BDCRx = Duty_Offset;
  }
}


//-----------------------------------------------------------------------------
// FAN table save to ram.
//-----------------------------------------------------------------------------
void Clr_Thermal_Tab(void)
{
  unsigned char * Thermal_pntr;

  Thermal_pntr = &FAN_Tab_OFF;  // Base on address 0x0160.
  for( ITempW01=0; ITempW01<=9; ITempW01++ )  // Clear 0160~0169.
  {
    *Thermal_pntr=0;
        Thermal_pntr++;
  }

#if Support_COMPAL_MFG_TOOL
  Thermal_pntr = &Step0_L;    // Base on address 0x0170.
  for( ITempW01=0; ITempW01<=9; ITempW01++ )  // Clear 0170~0179.
  {
    *Thermal_pntr=0;
        Thermal_pntr++;
  }
#endif  
}


/*****************************************************************************/
// Procedure: ThrottlingControl                TimeDiv: 100mSec
// Description: Chekc all throttling status.
// GPIO:
// Referrals:
/*****************************************************************************/
void ThrottlingControl(void)
{
  if ( SystemIsS0 )
  {
    cThrottlingSet &= (BIT6 + BIT7);  // Clear Throttling status.

    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cADPThrottling )  // Compare current and ADP Throttling
    {
      cThrottlingSet |= cADPThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cTHERMThrottling )  // Compare current and Thermal throttling.
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= cTHERMThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < cBATTThrottling )  // Compare current and BAT throttling.
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= cBATTThrottling;
    }
    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) < nThrottlingAPSet )  // For AP or debug
    {
      cThrottlingSet &= (BIT6 + BIT7);
      cThrottlingSet |= nThrottlingAPSet;
    }

    if( ( cThrottlingSet & ~(BIT6 + BIT7) ) != ( REAL_THROTTLING_INDEX & ~(BIT6 + BIT7) ) )
    {  // Cpu throttling for power status change
      if( ( (nRealThermalPinGET) && (( cThrottlingSet & ~(BIT6 + BIT7) ) >= 0x01 ))
      || (!nRealThermalPinGET) )
      {
        SET_MASK(cThrottlingSet,cTHRM_SW);
        REAL_THROTTLING_INDEX = (REAL_THROTTLING_INDEX & (BIT6 + BIT7)) | (cThrottlingSet & ~(BIT6 + BIT7));
        if( (REAL_THROTTLING_INDEX & ~(BIT6 + BIT7) ) == 0x00 )
        {
          nSWThermalPinDIS;
        }
        else
        {
          nSWThermalPinEN;
        }
      }
    }
    if( IS_MASK_SET(cThrottlingSet,cTHRM_SW) )
    {
      CLEAR_MASK(cThrottlingSet,cTHRM_SW);
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )  // Check OS mode.
      {
        ECQEvent(CPU_SLOW_AD);   // 0x7B inform bios.
      }
    }
    
    if( ( cGPUThrottlingSet & ~(BIT6 + BIT7) ) != cGPUThrottling )  // Compare current and GPU Throttling
    {
      cGPUThrottlingSet &= (BIT6 + BIT7);
      cGPUThrottlingSet = cGPUThrottling;
      SET_MASK(cGPUThrottlingSet,BIT6);
    }
    if( IS_MASK_SET(cGPUThrottlingSet,BIT6)){
      CLEAR_MASK(cGPUThrottlingSet,BIT6);
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )  // Check OS mode.
      {
		  	if(nSwitchableBTNGET)
		  	{
        	ECQEvent(GPU_pwrlmt_ON);   // 0x88 inform bios.
        }
      }
    }
    
  }
}

/*****************************************************************************/
// Procedure: Chk_FAN_RPM_Control              TimeDiv: 50mSec
// Description: Check FAN is AP control.
// GPIO:
// Referrals:
/*****************************************************************************/
void Chk_FAN_RPM_Control(void)
{

  if (IS_MASK_SET(uReserve07,nFanManual))
  {
    if (nAtmFanSpeed != ManualFanPRM)
    {
      if( nAtmFanSpeed > ManualFanPRM )
      {
        if( FAN_PWM > 0 )
        { FAN_PWM--; }
      }
      else
      {
        if(FAN_PWM < FAN_PWM_Max)
        {
          if(FAN_PWM == 0)
            {FAN_PWM = FAN_PWM_Min;}          
          else
            { FAN_PWM++; }
        }
        else
        { FAN_PWM = FAN_PWM_Max; }
      }
    }

  }

}


void Fan_control_update(void)
{
   BYTE Fanlevel;

    if( FAN_ID_TYPE == Fan_Null)
  	return;
//for fan1
   if (FanAutomaticmodeGet){                  //auto mode
      CLEAR_MASK(uReserve07,nFanManual);
      CLEAR_MASK(Thro_Status2, b4FAN1_FullOn);
   } else {                                   //manual mode  
      if (FanMaximumspeedGet){
        CLEAR_MASK(uReserve07,nFanManual);
        SET_MASK(Thro_Status2, b4FAN1_FullOn);
      }
      else{
        SET_MASK(uReserve07,nFanManual);
        CLEAR_MASK(Thro_Status2, b4FAN1_FullOn);
        Fanlevel = u4Fanspeedcontrol & (Fanspeedlevel);
        if (Fanlevel == 0){         //fan speed level 0: stop, control by 8Fh
          ManualFanPRM = u4ManualFanPRM;
        }
        else {    //fan speed level 1~7: control by thermal table
          if( IS_MASK_SET(SYS_MISC1, ACPI_OS) ){     //WIN
             if (Fanlevel >= FanTblPtr[FAN_ID_TYPE].WinTblCount){
               Fanlevel = FanTblPtr[FAN_ID_TYPE].WinTblCount - 1;
             }
             ManualFanPRM = FanTblPtr[FAN_ID_TYPE].WinTbl[Fanlevel].RPMValue;
          }
          else {                              //DOS
             if (Fanlevel >= FanTblPtr[FAN_ID_TYPE].DosTblCount){
               Fanlevel = FanTblPtr[FAN_ID_TYPE].DosTblCount - 1;
             }            
             ManualFanPRM = FanTblPtr[FAN_ID_TYPE].DosTbl[Fanlevel].RPMValue;
          }
        }
      }
   } 
}

//A285D00052: add stp(skin temperature protection) function for thermal
void ThermalSTBProcess(void)
{
    if(SystemIsS0 && IS_MASK_SET(SYS_MISC1, ACPI_OS))
    {
        if(nRamTemp >= THERMAL_STB_ENABLE_TEMP_THRESHOLD)
        {
            if(IS_MASK_CLEAR(Thro_Status2, b6Thermal_Stb))
            {
                ECQEvent(Thermal_STP_Enable);
                SET_MASK(Thro_Status2, b6Thermal_Stb);
            }
        }
        else if(nRamTemp <= THERMAL_STB_DISABLE_TEMP_THRESHOLD)
        {
            if(IS_MASK_SET(Thro_Status2, b6Thermal_Stb))
            {
                ECQEvent(Thermal_STP_Disable);
                CLEAR_MASK(Thro_Status2, b6Thermal_Stb);
            }
        }
    }
    else
    {
        CLEAR_MASK(Thro_Status2, b6Thermal_Stb);
    }
}


/*****************************************************************************/
// Procedure: Oem_Fan_Contro                  TimeDiv: 1 Sec
// Description: Main FAN Control Center
// GPIO:
// Referrals:  CPU/GPU Temperature
/*****************************************************************************/
//-----------------------------------------------------------------------------
void Oem_Fan_Control(void)
{

  // Check VGA and CPU Temperature error bit and full on bit.
  if ( (IS_MASK_SET(ERR_THMSTS, (b1VGATempEr+b3PECITempEr))) ||
      (IS_MASK_SET(Thro_Status2, b4FAN1_FullOn)) )
  {
    FAN_PWM = Target_Duty = FAN_PWM_Max;  // Fan turn on max.
    return;
  }

  if ( Fan_ON_Count != 0 )          // Turn on Fan x sec.
  {
    Fan_ON_Count--;
    return;
  }

  //A285D00010---start
	#if Support_IFC
	if(nCpuTempLocal < IFC_TEMP )
	{
		FAN_PWM = 0x00;
		return;
	}
	#endif	// end of Support_IFC//A285D00010---end

  if( SystemNotS0 )  // Not S0 status.
  {
    FAN_PWM = Target_Duty = 0;  // Turn off FAN.
    FanMaximumspeedCLR;         //clean Fan Maximum speed 
    return;
  }
#if Support_COMPAL_MFG_TOOL
  FANTabletoSTPM();
#endif
//  FanInitEN;                  // remove by CAC. 2013.05.06

    if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
    { FanCtrlDos(); }  // DOS mode.
    else
    { FanCtrlWin(); }  // OS mode.
}

void Chk_RPM_for_load_PWM(void)
{

  if( nAtmFanSpeed == TargetRPM )
    return;
  if( nAtmFanSpeed > TargetRPM )
  {
    if ((nAtmFanSpeed - TargetRPM) >= 3)
    {
      Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      PWM_CHG_delay = 5;
    }
  }
  else
  {
    if ((TargetRPM - nAtmFanSpeed) >= 3)
    {
      Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      PWM_CHG_delay = 5;
    }
  }
}

/*****************************************************************************/
// Procedure: OEM_Throttling_Ctrl                TimeDiv: 1 Sec
// Description: System CPU/GPU Throttling Data
// GPIO:
// Referrals:  CPU/GPU Temperature
/*****************************************************************************/
//-----------------------------------------------------------------------------
void OEM_Throttling_Ctrl(void)
{
  if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) )
  { ThrottlingDos(); }  // DOS mode.
  else
  { ThrottlingWin(); }  // OS mode.

//-----------------------------------------------------------------------------
// Throttling: Check throttling status.
//-----------------------------------------------------------------------------
  if ( IS_MASK_CLEAR(Thro_Status, (b0ProCH_CPU+b1ProCH_VGA+b2ProCH_EXTVGA)) && IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeACCUTOFF) )
  {  // turn off prochot.
    if (AC_OUT_Prochot_CNT == 0){ //WA : DAx75 prochot fixed 100mS
      H_PROCHOT_LOW();
      nRealThermalPinDIS;
      CLEAR_MASK(pPROCHOT, b0Thermal_PRCOHOTon);  // for AP display.
    }    //WA : DAx75 prochot fixed 100mS
  }
  else
   {  // turn on prochot.
    H_PROCHOT_HI();
    nRealThermalPinEN;
    SET_MASK(pPROCHOT, b0Thermal_PRCOHOTon);  // for AP display.
  }
//-----------------------------------------------------------------------------
// Throttling: Check CPU/VGA Turbo status.
//-----------------------------------------------------------------------------
  if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
  {

    //
    //  Behavior updated by Lonestar_Gordon_FAN_Control_Table_PWM_AVC_v04_0314 and 
    //                      Lonestar_Gordon_FAN_Control_Table_PWM_Delta_v04_0314
    //
    //  Changed from P1 to Pmax/2.

    if ( IS_MASK_CLEAR(Thro_Status, (b3Turbo_CPU+b4Turbo_VGA+b5Turbo_EXTVGA+b6ProCH_TURBO)) )
    { cTHERMThrottling &= (BIT6 + BIT7); }  // Not need throttling.
#if (PLATFORM_CHIP == INTEL)
    else
    {
      cTHERMThrottling &= 0xC0;  // Clear thermal throttling status.
      cTHERMThrottling |= CPUturboDisPstage;  // Set P-State level.
    }
#endif    
    if ( IS_MASK_SET(Thro_Status, b7PMAXD2_VGA) || IS_MASK_SET(Thro_Status, b3Turbo_CPU))
    {
      if  (CPUMAXPSTATE != 0)
      {
#if (PLATFORM_CHIP == INTEL)
        if ((cTHERMThrottling & !(BIT6 + BIT7)) < (((CPUMAXPSTATE+1-pCPUBasePStage)/2)+pCPUBasePStage) )
        {
          cTHERMThrottling &= (BIT6 + BIT7);
          cTHERMThrottling |= ((CPUMAXPSTATE+1-pCPUBasePStage)/2)+pCPUBasePStage;  // Set (1/2 (P-State MAX - P-State Base)) + P-State Base.
        }
#else
        if ((cTHERMThrottling & ~(BIT6 + BIT7)) < ((CPUMAXPSTATE+1)/2) )
        {
          cTHERMThrottling &= (BIT6 + BIT7);
          cTHERMThrottling |= ((CPUMAXPSTATE+1)/2);  // Set 1/2 P-State MAX.
        }
#endif        
      }
      else
      {
        cTHERMThrottling &= (BIT6 + BIT7);            // if BIOS no write P-stage MAX
        cTHERMThrottling |= 4;              // Set 1/2 P-State MAX = 4 .
      }
    }

    if ( IS_MASK_CLEAR(Thro_Status, (b4Turbo_VGA+b5Turbo_EXTVGA)) )
    {
      if ( IS_MASK_SET(uVGATurboFun,uDisVGATurboOK) )    // bit2.
      {
        CLEAR_MASK(uVGATurboFun, uDisVGATurboOK);      // Set bit2.
        //ECQEvent(DIS_TURBO_63);             // 0x63 VGA Disable turbo.
//        VGA_AC_BATT_ON(); // GPU throtle off
      }
    }
    else
    {
       if ( IS_MASK_CLEAR(uVGATurboFun,uDisVGATurboOK) )  // bit2.
       {
        SET_MASK(uVGATurboFun, uDisVGATurboOK);      // Set bit2.
        //ECQEvent(EN_TURBO_62);               // 0x62 VGA Enable turbo.
//        VGA_AC_BATT_OFF(); // GPU throtle on
       }
    }
  }
}

void DetectFanId (void)
{
	#if Support_FANID_ADC  //A485D00039
  	if ( FAN_ID > FAN_VEN_BOUNDARY_1 ) {         // Delta Fan.
    	FAN_ID_TYPE = FAN_VENDOR_1;
  	} else if ( FAN_ID > FAN_VEN_BOUNDARY_2 ) {  // Sunon Fan.
    	FAN_ID_TYPE = FAN_VENDOR_2;
  	} else {                                     // AVC Fan.
    	FAN_ID_TYPE = FAN_VENDOR_3;
  	}
  	#else  //A285D00010 --->S
	if(Read_FANID_HIGH())
		FAN_ID_TYPE = FAN_VENDOR_1;	//A485D00072			// Toshiba
	else
		FAN_ID_TYPE = FAN_VENDOR_3;	//A485D00072			// Delta
	#endif
  	//
  	// Hook for fan table.
  	//
  	oem_FanTblPtrHook();
}

void FanCtrlWin(void)
{
	BYTE BCPULevel,Level_chg, BTSLevel;
  	#if Support_VGA_FANTABLE  //A285D00010 --->S
  	BYTE	BVGALevel;
  	#endif	// end of Support_VGA_FANTABLE //A285D00010 --->E
  // Level_chg bit0: BCPULevel change flag
  // Level_chg bit1: BVGALevel change flag
  // Level_chg bit3: TSLevel change flag

   if( FAN_ID_TYPE == Fan_Null)
  	return;

  Level_chg = 0;
    do
    {
	BTSLevel = (FANTSSeg & 0x0F);

	if(BTSLevel <= 0)
	{
		BTSLevel = 0;

		if(nCpuTempLocal >=  FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn) { BTSLevel++; }
	}
	else if (BTSLevel >= (FanTblPtr[FAN_ID_TYPE].TsTblCount - 1))
	{
		BTSLevel = (FanTblPtr[FAN_ID_TYPE].TsTblCount - 1);
		if(nCpuTempLocal <= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff) { BTSLevel--; }
	}
	else
	{
		if(nCpuTempLocal >= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn) { BTSLevel++; }
		if(nCpuTempLocal <= FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff) { BTSLevel--; }
	}
	if (FANTSSeg != BTSLevel) { SET_MASK(Level_chg,BIT3); }     // BCPULevel change flag
	FANTSSeg = BTSLevel;
	//if thermal sensor fan run,need not detect cpu temperature
	if(BTSLevel > 0)
	{
		FANCPUSeg = 0;
		FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOn;
		FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].FanOff;
		FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue;
		break;
	}
	
  BCPULevel = (FANCPUSeg & 0x0F);  // Now CPU Fan Step

  if(BCPULevel <= 0)
  {
    BCPULevel = 0;
    if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn) { BCPULevel++; }
  }
  else if (BCPULevel >= (FanTblPtr[FAN_ID_TYPE].WinTblCount - 1))
     {
      BCPULevel = (FanTblPtr[FAN_ID_TYPE].WinTblCount - 1);
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
     else
     {
      if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn) { BCPULevel++; }
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
  if (FANCPUSeg != BCPULevel) { SET_MASK(Level_chg,BIT0); }     // BCPULevel change flag
  FANCPUSeg = BCPULevel;
  FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOn;
  FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].FanOff;
  FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
    }while(0);

	#if Support_VGA_FANTABLE //A285D00010
  BVGALevel = (FANVGASeg & 0x0F);  // Now CPU Fan Step
  if(BVGALevel <= 0)
  {
    BVGALevel = 0;
    if(VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn) { BVGALevel++; }
  }
  else if (BVGALevel >= (FanTblPtr[FAN_ID_TYPE].WinVgaTblCount - 1))
     {
      BVGALevel = (FanTblPtr[FAN_ID_TYPE].WinVgaTblCount - 1);
      if(VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff) { BVGALevel--; }
     }
     else
     {
      if(VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn) { BVGALevel++; }
      if(VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff) { BVGALevel--; }
     }
  if (FANVGASeg != BVGALevel) { SET_MASK(Level_chg,BIT1); }    // BVGALevel change flag
  FANVGASeg = BVGALevel;
  VFAN_Tab_HI   = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOn;
  VFAN_Tab_LOW  = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].FanOff;
  VFAN_Tab_DAC  = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue;
	#endif	// end of Support_VGA_FANTABLE  //A285D00010

#if (PLATFORM_CHIP == INTEL)
  if (( PCH_Temp3 >= 85 )&&(FANPCHSeg ==0))
  {
    if (TargetRPM < RPM_PCH_Max)
    {
      if ((RPM_PCH_Max - TargetRPM) > 4 )
      {
        Target_Duty = FAN_PCH_Max;
        Fan_Offset_Chk(Target_Duty, &FAN_PWM);
      }
    }
    TargetRPM = RPM_PCH_Max;
    FANPCHSeg = 1;
    return;
  }
  if ( FANPCHSeg !=0 )
  {
    if (PCH_Temp3 <= 70)
    {
      FANPCHSeg = 0;
      SET_MASK(Level_chg,BIT0);    // BCPULevel change flag
      SET_MASK(Level_chg,BIT1);    // BVGALevel change flag
    }
    else
    {
      TargetRPM = RPM_PCH_Max;
      return;
    }
  }
#endif // PLATFORM_CHIP

	#if Support_VGA_FANTABLE  //A285D00010
  if ( (FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue) > (FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue) )
  {
    TargetRPM = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue;
    if (IS_MASK_SET(Level_chg,BIT3))    // Level change flag be set, then load the FAN PWM initial duty
    {
      Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].TsTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
      Chk_RPM_for_load_PWM();
    }
  }
  else if ( (FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue) > (FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue) )
  {
    TargetRPM = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
    if (IS_MASK_SET(Level_chg,BIT0))    // Level change flag be set, then load the FAN PWM initial duty
    {
      Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
      Chk_RPM_for_load_PWM();
    }
  }
  else
  {
    TargetRPM = FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].RPMValue;
    if (IS_MASK_SET(Level_chg,BIT1))    // Level change flag be set, then load the FAN PWM initial duty
    {
      Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].VgaTbl[BVGALevel].PWMDuty,FAN_PWM_Max);
      Chk_RPM_for_load_PWM();
    }
  }
	#else  //A285D00010
	if(BTSLevel)
	{
		TargetRPM = FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].RPMValue;
	}
	else
	{
	TargetRPM = FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].RPMValue;
	}

	if(nCpuTempLocal >= STP_THOT_ON )
	{
		if( FANEXTSeg != 1)
		{
			FANEXTSeg = 1;
			SET_MASK(Level_chg,BIT2);
		}
	}
	else 
	{
		if( FANEXTSeg != 0)
		{
			FANEXTSeg = 0;
			SET_MASK(Level_chg,BIT2);
		}

	}
	if(FANEXTSeg == 1)
	{
		if(TargetRPM < STP_FAN_RPM)
		{
			if(FAN_ID_TYPE == FAN_VENDOR_1)
				TargetRPM = VEN1_STP_FAN_RPM;
			else if(FAN_ID_TYPE == FAN_VENDOR_2)
				TargetRPM = VEN2_STP_FAN_RPM;
			else if(FAN_ID_TYPE == FAN_VENDOR_3)  //A285D00040: Add Fan vender 3
				TargetRPM = VEN3_STP_FAN_RPM;
				
			if(IS_MASK_SET(Level_chg,BIT2))
			{
				if(FAN_ID_TYPE == FAN_VENDOR_1)
					Target_Duty = Cal_PWM_Duty(VEN1_STP_FAN_DUTY,FAN_PWM_Max);
				else if(FAN_ID_TYPE == FAN_VENDOR_2)
					Target_Duty = Cal_PWM_Duty(VEN2_STP_FAN_DUTY,FAN_PWM_Max);
				else if(FAN_ID_TYPE == FAN_VENDOR_3)  //A285D00040: Add fan vender 3
					Target_Duty = Cal_PWM_Duty(VEN3_STP_FAN_DUTY,FAN_PWM_Max);
			}
			return;
		}
	}
	
	if(IS_MASK_SET(Level_chg, BIT3))
	{
		Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].TsTbl[BTSLevel].PWMDuty,FAN_PWM_Max);
		Chk_RPM_for_load_PWM();
	}
	else if (IS_MASK_SET(Level_chg,BIT0))    // Level change flag be set, then load the FAN PWM initial duty
    {
    	Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].WinTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
    	Chk_RPM_for_load_PWM();
    }
	
	#endif	// end of Support_VGA_FANTABLE //A285D00010
}


void FanCtrlDos(void)
{
  BYTE BCPULevel,Level_chg;
  // Level_chg bit0: BCPULevel change flag
  // Level_chg bit1: BVGALevel change flag

   if( FAN_ID_TYPE == Fan_Null)
  	return;

  Level_chg = 0;
  BCPULevel = (FANCPUSeg & 0x0F);  // Now VGA Fan Step
  if(BCPULevel <= 0)
  {
    BCPULevel = 0;
    if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn) { BCPULevel++; }
  }
  else if (BCPULevel >= (FanTblPtr[FAN_ID_TYPE].DosTblCount - 1))
     {
      BCPULevel = (FanTblPtr[FAN_ID_TYPE].DosTblCount - 1);
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
     else
     {
      if(TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn) { BCPULevel++; }
      if(TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff) { BCPULevel--; }
     }
  if (FANCPUSeg != BCPULevel) { SET_MASK(Level_chg,BIT0); }    // BCPULevel change flag
  FANCPUSeg = BCPULevel;
  FAN_Tab_LOW   = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOn;
  FAN_Tab_HI    = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].FanOff;
  FAN_Tab_DAC    = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].RPMValue;

  TargetRPM = FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].RPMValue;
  if (IS_MASK_SET(Level_chg,BIT0))    // Level change flag be set, then load the FAN PWM initial duty
  {
    Target_Duty = Cal_PWM_Duty(FanTblPtr[FAN_ID_TYPE].DosTbl[BCPULevel].PWMDuty,FAN_PWM_Max);
    Chk_RPM_for_load_PWM();
  }
}


void ThrottlingWin(void)
{
	BYTE TurboOff_External_Sensor,TurboOn_External_Sensor;
   if( FAN_ID_TYPE == Fan_Null)
  	return;
  THR_PRO_OFF   = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Off;
  THR_PRO_ON    = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_On;
  THR_Turo_Rem  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Rem;
  THR_Turo_OFF  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Off;
  THR_Tab_Shut  = FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Shut;

  	// Set Internal VGA.
  	#if Support_VGA_FANTABLE  //A285D00010
  	if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].PMAX_D2_on )    // Check P_MAX/2 turn ON.
    { 
      SET_MASK(Thro_Status, b7PMAXD2_VGA);
    }
  else if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].PMAX_D2_off )  // Check P-MAX/2 turn OFF.
    { 
      CLEAR_MASK(Thro_Status, b7PMAXD2_VGA);
    }

  if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_On )  // Check prochot turn ON.
    {
#if (PLATFORM_CHIP == INTEL)
      SET_MASK(Thro_Status, b1ProCH_VGA);
#endif
      SET_MASK(GPU_THROTTLE_SRC, THERMAL_POLICY);
    }
  else if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_Off )    // Check prochot turn OFF.
    {
#if (PLATFORM_CHIP == INTEL)
      CLEAR_MASK(Thro_Status, b1ProCH_VGA); 
#endif
      CLEAR_MASK(GPU_THROTTLE_SRC, THERMAL_POLICY);
    }
  	#endif	// end of Support_VGA_FANTABLE  //A285D00010
   //A485D00039-
  	//if(IS14Inch)
  	//{
  	//	TurboOff_External_Sensor = 52;
  	//	TurboOn_External_Sensor = 50;
  	//}
  	//else
  	//{
  	//	TurboOff_External_Sensor = 255;
  	//	TurboOn_External_Sensor = 255;
  	//}
  	TurboOff_External_Sensor = STP_THOT_ON; //A485D00039+
  	TurboOn_External_Sensor = STP_THOT_OFF;  //A485D00039+
#if (PLATFORM_CHIP == INTEL)
  if(nCpuTempLocal >= 65 || nVramTemp >= 77)
#else
  if ( nCpuTempLocal >= TurboOff_External_Sensor)  // CPU turbo turn OFF.
#endif
    { 
      SET_MASK(GPU_THROTTLE_SRC, External_Sensor);
      if ((cTHERMThrottling & ~(BIT6 + BIT7)) < 0x01 )
      {
        cTHERMThrottling &= (BIT6 + BIT7);
        cTHERMThrottling |= ((CPUMAXPSTATE+1)/2);  // Set P-State 1.
      	SET_MASK(Thro_Status, b6ProCH_TURBO);
      	//SET_MASK(Thro_Status, b3Turbo_CPU);
      }
    }
#if (PLATFORM_CHIP == INTEL)
  else if (nCpuTempLocal <= 60 || nVramTemp <= 60)
#else
  else if ( nCpuTempLocal <= TurboOn_External_Sensor )    // CPU turbo turn ON.
#endif
    { 
      CLEAR_MASK(GPU_THROTTLE_SRC, External_Sensor);
      if ((cTHERMThrottling & ~(BIT6 + BIT7)) > 0x00 )
      {
      	CLEAR_MASK(Thro_Status, b6ProCH_TURBO);
      	//CLEAR_MASK(Thro_Status, b3Turbo_CPU);
      }
    }

	// external CPU Temp Shutdown Protection
	if(nCpuTempLocal >= STP_SHDN)  //A285D00010
		Thro_Count_Shut(&Ext_CPU_Shut_Cnt,TEMP_TYPE_EXT);  //A285D00010

	#if Support_VGA_FANTABLE  //A285D00010
  	if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Thr_Shut )    // Check shutdown status.
  	{ Thro_Count_Shut(&VGA_Shut_Cnt,TEMP_TYPE_VGA); }  // Check x times for shutdown protection.
  	else
  	{ VGA_Shut_Cnt = 0; }
	#endif	// end of Support_VGA_FANTABLE  //A285D00010

  // Set Internal CPU.
  if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Off )    // Check prochot turn OFF.
  { CLEAR_MASK(Thro_Status, b0ProCH_CPU); }
  else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_On )  // Check prochot turn ON.
  { SET_MASK(Thro_Status, b0ProCH_CPU); }

  if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Thr_Shut )    // Check shutdown status.
  { Thro_Count_Shut(&CPU_Shut_Cnt,TEMP_TYPE_CPU); }    // Check x times for shutdown protection.
  else
  { CPU_Shut_Cnt = 0; }

  if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
  {
    // Set Internal VGA Turbo.
    	#if Support_VGA_FANTABLE  //A285D00010
    if ( VGA_TBuff3 <= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Turbo_Rem )    // Check turbo resume.
    { CLEAR_MASK(Thro_Status, b4Turbo_VGA); }
//     else if ( VGA_TBuff3 >= FanTblPtr[FAN_ID_TYPE].VgaThrTbl[0].Turbo_Off )  // Check turbo Off.
//     { SET_MASK(Thro_Status, b4Turbo_VGA); }
		#endif // end of Support_VGA_FANTABLE //A285D00010

    // Set Internal CPU Turbo.
    if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Rem )    // Check turbo resume.
    { CLEAR_MASK(Thro_Status, b3Turbo_CPU); }
    else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrTbl[0].Turbo_Off )// Check turbo Off.
    { SET_MASK(Thro_Status, b3Turbo_CPU); }
  }

}

void ThrottlingDos(void)
{
   if( FAN_ID_TYPE == Fan_Null)
  	return;
  THR_PRO_OFF   = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Off;
  THR_PRO_ON    = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_On;
  THR_Turo_Rem  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Turbo_Rem;
  THR_Turo_OFF  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Turbo_Off;
  THR_Tab_Shut  = FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Shut;

  // Set Internal CPU.
  if ( TEMP_Buff_3 <= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Off )    // Check prochot turn OFF.
  { CLEAR_MASK(Thro_Status, b0ProCH_CPU); }
  else if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_On )  // Check prochot turn ON.
  { SET_MASK(Thro_Status, b0ProCH_CPU); }

  if ( TEMP_Buff_3 >= FanTblPtr[FAN_ID_TYPE].CpuThrDosTbl[0].Thr_Shut )    // Check shutdown status.
  { Thro_Count_Shut(&CPU_Shut_Cnt,TEMP_TYPE_CPU); }      // Check x times for shutdown protection.
  else
  { CPU_Shut_Cnt = 0; }
}


//-----------------------------------------------------------------------------
// Throttling: Shutdown count used.
//-----------------------------------------------------------------------------
void Thro_Count_Shut(BYTE *BShut_Cnt, BYTE TempType)
{
  if ( *BShut_Cnt >= Thro_Shut_Cnt )  // Read 3 times.
  {
    if ( TempType == 1 )
    { ProcessSID(VGAOVERTEMP_ID); }  // VGA Shutdown ID 0x12.	
    else if( TempType == 2)  //A285D00010
	{ ProcessSID(EXTCPUOVERTEMP_ID); }  // Ext CPU Shutdown ID 0x12. //	 //A285D00010	
    else
    { ProcessSID(DTSOVERTEMP_ID); }  // CPU Shutdown ID 0x11.

    SET_MASK(SysStatus,ERR_ShuntDownFlag);

//[-start-131212-T130124_8-remove]//
//     PWSeqStep = 1;
//     PowSeqDelay = 0x00;
//     SysPowState = SYSTEM_S0_S5;      // EC force Shutdown.
//[-end-131212-T130124_8-remove]//
    *BShut_Cnt = 0;
    RSMRST_shutdown();				

    //
    // AOU -- Unexpected power shutdown by thermal issue.
    // (The user needs to take off all power of the system for recovery from the thermal issue)
    //
    SET_MASK(USB_Charger, b2USB_TmlDis);  // Disable USB charger.

    //
    //   Dear Dustin,
    //     If we has embedded battery ,when happen unexpected shoutdow by thermal issue., the AOU function works also. 
    //   DavidMa (ThinkPad China Department Lab)
    //
    //   2014.06.23
    if (OneOfTwoBattExistGet) {
      if (IS_MASK_SET(OptMfgFunction2L, BIT7)) { // Replaceable(0)/Non-replaceable(1)
        CLEAR_MASK(USB_Charger, b2USB_TmlDis);  // Enable USB charger.
      }
    }
  }
  else
  { (*BShut_Cnt)++; }
}
#if Support_COMPAL_MFG_TOOL
void FANTabletoSTPM(void)
{
   if( FAN_ID_TYPE == Fan_Null)
  	return;
  if ( IS_MASK_CLEAR(SYS_MISC1, ACPI_OS) ) {
    //
    // DOS mode.
    //
    Step0_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[0].FanOn;
    Step0_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[0].FanOff;
    Step0_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[0].RPMValue;
    Step1_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[1].FanOn;;
    Step1_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[1].FanOff;;
    Step1_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[1].RPMValue;
    Step2_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[2].FanOn;;
    Step2_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[2].FanOff;;
    Step2_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[2].RPMValue;
    Step3_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[3].FanOn;;
    Step3_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[3].FanOff;;
    Step3_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[3].RPMValue;
    Step4_L   = FanTblPtr[FAN_ID_TYPE].DosTbl[4].FanOn;;
    Step4_H   = FanTblPtr[FAN_ID_TYPE].DosTbl[4].FanOff;;
    Step4_DAC = FanTblPtr[FAN_ID_TYPE].DosTbl[4].RPMValue;
  } else {
    //
    // OS mode.
    //
    Step0_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[0].FanOn;
    Step0_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[0].FanOff;
    Step0_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[0].RPMValue;
    Step1_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[1].FanOn;;
    Step1_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[1].FanOff;;
    Step1_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[1].RPMValue;
    Step2_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[2].FanOn;;
    Step2_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[2].FanOff;;
    Step2_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[2].RPMValue;
    Step3_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[3].FanOn;;
    Step3_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[3].FanOff;;
    Step3_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[3].RPMValue;
    Step4_L   = FanTblPtr[FAN_ID_TYPE].WinTbl[4].FanOn;;
    Step4_H   = FanTblPtr[FAN_ID_TYPE].WinTbl[4].FanOff;;
    Step4_DAC = FanTblPtr[FAN_ID_TYPE].WinTbl[4].RPMValue;
  }
}
#endif





   

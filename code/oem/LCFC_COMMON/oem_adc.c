//*****************************************************************************
//
//  oem_adc.c
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

void Init_ADC(void)
{
	ADC_Initialization();
#if (ADC_Channel_SUPPORT & ADC_Channel_0)  
  ADC_Channelx_Enable(ADC_Channel_0);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_1)  
  ADC_Channelx_Enable(ADC_Channel_1);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_2)  
  ADC_Channelx_Enable(ADC_Channel_2);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_3)  
  ADC_Channelx_Enable(ADC_Channel_3);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_4)  
  ADC_Channelx_Enable(ADC_Channel_4);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_5)  
  ADC_Channelx_Enable(ADC_Channel_5);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_6)  
  ADC_Channelx_Enable(ADC_Channel_6);	
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_7)  
  ADC_Channelx_Enable(ADC_Channel_7);	
#endif 
}


//----------------------------------------------------------------------------
// The function of scaning ADC channel
//----------------------------------------------------------------------------
void ScanADCChannel(void)
{

#if (ADC_Channel_SUPPORT & ADC_Channel_0)
	ADC_Data0 = ADC_Get_Channelx_ADC_Data(0);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_1)
  	ADC_Data1 = ADC_Get_Channelx_ADC_Data(1);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_2)
	ADC_Data2 = ADC_Get_Channelx_ADC_Data(2);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_3)
	ADC_Data3 = ADC_Get_Channelx_ADC_Data(3);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_4)
	ADC_Data4 = ADC_Get_Channelx_ADC_Data(4);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_5)
	ADC_Data5 = ADC_Get_Channelx_ADC_Data(5);
#endif 
#if (ADC_Channel_SUPPORT & ADC_Channel_6)
//A485D00025:Ignore device_detect values update in EC_ON off.
#if (Support_Tamper_function == Tamper_ADC)
	if(Read_EC_ON())
#endif
//A485D00025->end
		ADC_Data6 = ADC_Get_Channelx_ADC_Data(6);
#endif
#if (ADC_Channel_SUPPORT & ADC_Channel_7)
  ADC_Data7 = ADC_Get_Channelx_ADC_Data(7);
#endif 
  
    if( IS_MASK_SET(cCheatAD,cCAD_EN) )
    {
        if( IS_MASK_SET(cCheatAD,cCAD_ADPID) )
        {
            API_ID = cCADP_ID;
        }
    }
#if  Support_SMTADP
    CovertADPID();
#endif  // Suport_SMTADP
    

    if( IS_MASK_SET(cCheatAD,cCAD_EN) )
    {
        if( IS_MASK_SET(cCheatAD,cCAD_ADPI) )
        {
            ADP_I = cCADP_I;
        }
    }
    if (SysPowState == SYSTEM_S0)
    {
     	ADPI_AvgData = ADP_I;
      	if(ADPI_MUL == 0)
      	{
      		ADPI_MUL = ClassicAdaptor_ADPI_MUL;
      	}     
       if (OneOfTwoBattExistGet){
         Chk_Hybrid_STPP();
       }else{
         Chk_AC_STPP(0);
       }
    }
#if Support_ADC_BoardID
  ScanADC_BoardID();
#endif    
}



// CMW 20121022 +
//---------------------------------------------------------------------------
// Init Voltage Compare Function
// Input: 1 Group. choice the init VC channel  0: VC0 1: VC1 2:VC2 3:All VC Channel
//---------------------------------------------------------------------------
void Init_VC(BYTE Group)
{
  
  #if Support_VC0_Function
  int tGroup = (Group&0x03);
  // Set the CPU Thermal VC Setting
  if( (tGroup == 0x00) || (tGroup == 0x03) )
  {
    VCMP0CTL &= 0x3F;  // Disable VC0 & Interrupt.
    CMP0THRDATM = (VC0TriggerValue>>8);    // threshold value buffer
    CMP0THRDATL = VC0TriggerValue;
    VCMP0CTL &= 0xF8;  // Clear ADC Channel & set to Channel 0.
    VCMP0CTL &= 0xDF;  // Set the Less Then Voltage trigger.
    VCMP0CTL |= 0x08;  // Set Active-Low.
    GCR15 |= 0x01;    // Enable GPJ3 to be VC1 GPO.
    GPCRJ3 = 0x00;
    VCMP0CTL |= 0x40;  // Enable Interrupt.
    VCMP0CTL |= 0x80;
  }
  #endif

  #if Support_VC1_Function
  int tGroup = (Group&0x03);
  // Set the ADP_I VC setting
  if( (tGroup == 0x01) || (tGroup == 0x03) )
  {
    VCMP1CTL &= 0x3F;  // Disable VC1 & Interrupt.
    //CMP1THRDATM = (VC1TriggerValue>>8);  // threshold value buffer
    //CMP1THRDATL = VC1TriggerValue;
    SetTurbo_V_Thro();  // Set Turbo_V trigger point.
    VCMP1CTL &= 0xF8;  // Clear ADC Channel
    VCMP1CTL |= 0x01;  // Connect to ADC Channel 1.
    VCMP1CTL |= 0x20;  // Set the Over Voltage trigger.
    VCMP1CTL &= 0xF7;  // Set Active-High.
    GCR15 |= 0x02;    // Enable GPJ4 to be VC1 GPO.
    GPCRJ4 = 0x00;
    VCMP1CTL |= 0x40;  // Enable Interrupt.
    VCMP1CTL |= 0x80;  // Enable VC1
  }
  #endif

  #if Support_VC2_Function
  int tGroup = (Group&0x03);
  // Set the xxx VC setting.
  if( (tGroup == 0x02) || (tGroup == 0x03) )
  {
  }
  #endif

  if( (VCMP0CTL&0x80) || (VCMP1CTL&0x80) || (VCMP2CTL&0x80) )
  {
    IER18 |= 0x80;
  }
}
// CMW 20121022 -


void Chk_Wrong_ADP(void)
{
  if( (SysPowState == SYSTEM_S0) || (SystemNotS0 && (Chk_Wrong_ADP_Status != 0)) )
  {

    if((ACIN_FallINT_Count > 0) && (Chk_Wrong_ADP_Status == 0))
    {
      Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_wait_2sec;
    }

    switch(Chk_Wrong_ADP_Status)
    {
    case Chk_Wrong_ADP_Status_wait_2sec:
      Chk_Wrong_10ms_Count++;
      if (Chk_Wrong_10ms_Count  > 200)
      {
        CLEAR_MASK(ACOFF_SOURCE, ADPOVP);
        ACOFF_LOW();
        CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
        //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
        //WriteSmartChgIC();
        Chk_Wrong_10ms_Count = 0;    //
        ACIN_FallINT_Count = 0;
        Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_wait_10sec;
      }
      else
      {
        SET_MASK(cBattFlag0,cCmdAcOff);       //Stop charger
        //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
        //WriteSmartChgIC();
        SET_MASK(ACOFF_SOURCE, ADPOVP);
//        ACOFF_HI();
      }
      break;
    case Chk_Wrong_ADP_Status_wait_10sec:
      if (Chk_Wrong_10ms_Count > 1000)  //Over 10s not detect 5 times ACIN drop.
      {
        Chk_Wrong_10ms_Count = 0;     //Reset all counter
        ACIN_FallINT_Count = 0;
        Chk_Wrong_ADP_Status=0;       //Reset Status
      }
      else
      {
        if (ACIN_FallINT_Count >= 5)      //falling 5 times
        {
          Chk_Wrong_ADP_Status = Chk_Wrong_ADP_Status_ACOFF_Lock;
          ACIN_FallINT_Count = 0;
          Chk_Wrong_10ms_Count = 0;
          SET_MASK(cBattFlag0,cCmdAcOff);     //Stop charger
          //CHGIC_ptr = 2;          //reminder: call to write to SmartChg
          //WriteSmartChgIC();
          SET_MASK(ACOFF_SOURCE, ADPOVP);
//          ACOFF_HI();            //Pull High AC off until reset

        }
        Chk_Wrong_10ms_Count++;
      }
      break;
    case Chk_Wrong_ADP_Status_ACOFF_Lock:
      //Reset AC OFF Lock after Plug out ACIN 20ms
      if (!Read_AC_IN())
      {
        if (Chk_Wrong_10ms_Count >2)
        {
          Chk_Wrong_ADP_Status = 0;       //Reset Status and counter
          Chk_Wrong_10ms_Count = 0;
          ACIN_FallINT_Count =0;
          CLEAR_MASK(ACOFF_SOURCE, ADPOVP);
          ACOFF_LOW();
          CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
          //CHGIC_ptr = 2;            //reminder: call to write to SmartChg
          //WriteSmartChgIC();
        }
        Chk_Wrong_10ms_Count ++;
      } else {
        if (pLastSID != AC_WRONG_ADP_ID) {
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID (AC_WRONG_ADP_ID);		
        }
      }
      break;
    default:
      Chk_Wrong_ADP_Status = 0;
      Chk_Wrong_10ms_Count = 0;
      ACIN_FallINT_Count = 0;
      break;
    }
  }
  else
  {
    Chk_Wrong_ADP_Status = 0;
    Chk_Wrong_10ms_Count = 0;
    ACIN_FallINT_Count = 0;
  }
}



void BatteryCharge (
  BYTE OnOff
)
{
  //
  // 0: Stop charge, 1: Start charge
  //
  if (OnOff) {
    CLEAR_MASK(M_BATStopChgStatH, HybridModeStopChg);
    CLEAR_MASK(cBattFlag0,cCmdAcOff);       //Resume Charger
    CLEAR_MASK(M_BATStopChgStatH, ENADPTHROTTLING);
  } else {
    SET_MASK(M_BATStopChgStatH, HybridModeStopChg);
    SET_MASK(cBattFlag0,cCmdAcOff);       //Stop Charger
    SET_MASK(M_BATStopChgStatH, ENADPTHROTTLING);
  }
}

void CpuTurbo (
  BYTE OnOff
)
{ 
	BYTE BAT_CPU_TURBO_DIS_Temp,BAT_CPU_TURBO_EN_Temp; 
  if (OneOfTwoBattExistGet )//&& !Read_DCIN_ATTACHED2()) 
  {
    //
    // Battery condition
    //
    if(AllBattExistGet)  //&& (Current_Battery == S_BAT)) ////A285D00001 modify for only one battery
    {
			BAT_CPU_TURBO_DIS_Temp = BOTH_BAT_CPU_TURBO_DIS;	// Battery RSOC <= 5%, CPU Turbo should be turned off.
			BAT_CPU_TURBO_EN_Temp = BOTH_BAT_CPU_TURBO_EN;	// Battery RSOC > 5%, CPU Turbo can be turned on.
		}
		else
		{
			BAT_CPU_TURBO_DIS_Temp = BAT_CPU_TURBO_DIS;	// Battery RSOC <= 10%, CPU Turbo should be turned off.
			BAT_CPU_TURBO_EN_Temp = BAT_CPU_TURBO_EN;	// Battery RSOC > 10%, CPU Turbo can be turned on.
		}
		if(WorkingBattRSOC <= BAT_CPU_TURBO_DIS_Temp){
      if (IS_MASK_CLEAR(pProject0, bRSOCAPUTURBO)) {
        SET_MASK(pProject0, bRSOCAPUTURBO);
	      SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
      }
        UpGPUPState();			
    } else if (WorkingBattRSOC > BAT_CPU_TURBO_EN_Temp) {
      if (OnOff)
      {
        if ( IS_MASK_SET(pProject0, (bHYBAPUTURBO|bRSOCAPUTURBO)) ) {
          CLEAR_MASK(pProject0,bRSOCAPUTURBO);
          CLEAR_MASK(pProject0,bHYBAPUTURBO);
          CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
          CPUtuborONOFF(Enable);
        }
      }
    }

    if (OnOff == MPC_OFF) 
    {
      if (IS_MASK_CLEAR(pProject0, bHYBAPUTURBO)) {
        SET_MASK(pProject0,bHYBAPUTURBO);
        SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
      }
        UpGPUPState();		
    }
  } else {
    //
    // Adapter condition
    //
    if (IS_MASK_SET(pProject0,bAPUTURBO)) {
      return;
    } else {

      if(( AdapterID != LimitedSupportAdpWatt ) || ( AdapterID != AdapterID_NON_SUPPORT )){
        if (OnOff)
        {
          if ( IS_MASK_SET(pProject0, (bHYBAPUTURBO|bRSOCAPUTURBO)) ) {
            CLEAR_MASK(pProject0,bRSOCAPUTURBO);
            CLEAR_MASK(pProject0,bHYBAPUTURBO);
            CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
            CPUtuborONOFF(Enable);
            CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
          }
        } else {
          if (IS_MASK_CLEAR(pProject0, bHYBAPUTURBO)) {
            SET_MASK(pProject0,bHYBAPUTURBO);
            SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
            CPUtuborONOFF(Disable);
            SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
          }
           UpGPUPState();		
        }
      }else{
        if(OneOfTwoBattExistGet){
            if (WorkingBattRSOC <= BAT_CPU_TURBO_DIS) {
      // Battery RSOC <= 10%, CPU Turbo should be turned off.
        SET_MASK(pProject0, bRSOCAPUTURBO);
	    SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CPUtuborONOFF(Disable);
        SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
       	UpGPUPState();		
    } else if (WorkingBattRSOC > BAT_CPU_TURBO_EN) {
      // Battery RSOC > 10%, CPU Turbo can be turned on.
          CLEAR_MASK(pProject0,bRSOCAPUTURBO);
          CLEAR_MASK(pProject0,bHYBAPUTURBO);
          CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
          CPUtuborONOFF(Enable);
            CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
        }
        }
      }
      
    }
  }
}

void Chk_Hybrid_STPP(void)
{
  DWORD BattPresentVolt;
  DWORD BattNowCurrent;
  DWORD Batt_Output_Power;

  #if !Support_MAXPWR
  return;
  #endif

  if (inhibit2sec > 0)
  {
    CurrentL = 0;
    CurrentH = 0;
  }

  if (ADPI2Sec != 0){
    ADPI2Sec--;
  }

  if (GPU2Sec!= 0){
    GPU2Sec--;
  }
  CLEAR_MASK(M_BATAlarm,ADPOUT);
  CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);
//
// Adapter in + Battery in mode
//

//   if(Read_AC_IN() && IS_MASK_SET(CHGIC_ReadCmd0x12L,TurboBoost))//(RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
  if(Read_AC_IN() && !Read_ACOFF() && (WorkingBattRSOC > Chk_Hybrid_STPP_min_BattGasgauge)) //Check Battery over 20%
  {

    //if(Current_Battery == M_BAT){ //A285D00001-
        BattPresentVolt = (WORD)((VoltageH << 8) + VoltageL);
        if (IS_MASK_SET(CurrentH, BIT7))
        {
          BattNowCurrent = 0xFFFF - (WORD)((CurrentH << 8) + CurrentL);
        }
        else
        {
          BattNowCurrent = 0;
        }
		/*//A285D00001---->Start
    }else{
        BattPresentVolt = (WORD)((SecVoltageH << 8) + SecVoltageL);
        if (IS_MASK_SET(SecCurrentH, BIT7))
        {
          BattNowCurrent = 0xFFFF - (WORD)((SecCurrentH << 8) + SecCurrentL);
        }
        else
        {
          BattNowCurrent = 0;
        }
    }
    *///A285D00001---->End
    Batt_Output_Power = (BattPresentVolt * BattNowCurrent); // Ex. 12,000 mV * 2,500 mA = 30,000,000uW = 30W
    if( IS_MASK_SET(cCheatAD,cCAD_EN) && IS_MASK_SET(cCheatAD,cCAD_BATPWR))  //for debug
    {
       Chk_Hybrid_STPP_Batt_Output_Power = cCSTPPBATPWR;
    }else
    {
       Chk_Hybrid_STPP_Batt_Output_Power = (Batt_Output_Power * 0.00006826667) / ADPI_MUL; // The result almost equals 'ADC' value.
    }
    
    // Ex. 30,000,000uW = 30W, (30W / 20V) = 1.5A -> 1.5*0.2=0.3v(ADP_I), 0.3v / 3v * 1024(step) = 102.4(ADC value)
    // Aadc = 30W * 68.2666667 / Vsys -> if Vsys=20V -> Aadc = 30 *68.26667 /20 = 102.4

    // System total power=ADP_I/0.4*20 + Battery total power
    if (MAX_power_Debug){
      MpcSystemTotalPower = (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power) * 0.0146484375 *ADPI_MUL;//  (/ 1024 * 3 / 0.2 *Vsys);	//For Debug.
    }

    switch (MaxPwrCtrlFsm) {

      case SystemOverCP:
        if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcCP)) {
          MAX_power_state = 0;
          //
          // System under CP:
          //
          // 1. Set CPU turbo on.
          // 2. Set GPU throttling off.
          // 3. Set CPU throttling off.
          // 4. Set charger turbo off.
          // 5. Resume battery charge.
          //
        
          CpuTurbo(MPC_ON);
//          GpuThrottling(MPC_OFF);
         DownGPUPState();					
         DownBatteryPState();				
         ChargerTurbo(MPC_OFF);
          BatteryCharge(MPC_ON);
          //break;
        } else {
          MAX_power_state = 1;
          // Step1. System is overing CP(charge current limit)
          // System is overing CP and battery charge is off, but not reached charger turbo point.
          // -> No case break here, go to next check point.
        }

      case SystemOverChargerTurboPoint_ChargerTurboOff:

        if ( ADPI_AvgData > WATT_TO_ADC_BASE((MpcCP * 1.07)) ) {
          MAX_power_state = 2;
          // Step2. System is over charger turbo point when turbo boost(AC+DC) is not actived.
        
        }

      case SystemOverChargerTurboPoint_ChargerTurboOn:

        //if ( (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power) >= WATT_TO_ADC_BASE((MpcCP * 1.07)) ) {
          MAX_power_state = 3;
          // Step3. Already started charger turbo, active some actions to reduce system power consumption.

          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power <= WATT_TO_ADC_BASE(MpcGpuThrottlingRecoverPoint)) {
            MAX_power_state = 5;
            // AC + DC < GPU throttling recover point
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();		
//              GpuThrottling(MPC_OFF);
          }

          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power <= WATT_TO_ADC_BASE(MpcCpuThrottlingRecoverPoint)) {
            MAX_power_state = 6;
            // AC + DC < CPU throttling recover point
              CpuThrottling(MPC_OFF);
          }

          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power >= WATT_TO_ADC_BASE(MpcGpuThrottlingPoint)) {
            MAX_power_state = 7;
            // AC + DC > GPU throttling enable
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();			
//              GpuThrottling(MPC_ON);
          }

          if (ADPI_AvgData + Chk_Hybrid_STPP_Batt_Output_Power >= WATT_TO_ADC_BASE(MpcCpuThrottlingPoint)) {
            MAX_power_state = 9;
            // AC + DC > CPU throttling enable
            CpuThrottling(MPC_ON); // If system loading still over, then transform P1 to P2 or P3 or P4... etc.
          }
        //}

      default:
        MaxPwrCtrlFsm = SystemOverCP;
        break;

    } // switch (MaxPwrCtrlFsm)
  }
  else // if (Read_AC_IN() && !Read_ACOFF() && (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge))
  {
//
// Apapter in + Battery(<=20%) in mode
//
    // Battery RSOC <= 20%
    // Clean all flag and status

//     Chk_Hybrid_STPP_Status = Chk_Hybrid_STPP_Status_CP;
    MaxPwrCtrlFsm = SystemOverCP;

//     CLEAR_MASK(M_BATStopChgStatH, HybridModeStopChg);
//     cBattFlag0.fbit.cCmdAcOff=0;       //Resume Charger
//     CLEAR_MASK(M_BATStopChgStatH, ENADPTHROTTLING);
    BatteryCharge (MPC_ON);

//     CLEAR_MASK(CHGIC_ReadCmd0x12L, TurboBoost);
    ChargerTurbo (MPC_OFF);

//     CLEAR_MASK(BatteryAlarm,DISTURBO);
//    CpuTurbo (MPC_ON);

    AdpCUTCnt = 0;
    // Consider Battery gasguage less then 20%
    if( Read_AC_IN() && !Read_ACOFF() )
    {
      // AC + DC(<=20%)
      Chk_AC_STPP(1);
    }
  } // end else // if (Read_AC_IN() && !Read_ACOFF() && (RelativeStateOfChgL > Chk_Hybrid_STPP_min_BattGasgauge))


//
// Battery only mode
//
  if(OneOfTwoBattExistGet && (!Read_AC_IN() || Read_ACOFF())) {

    //
    // Battery RSOC condition is in function.
    //
    CpuTurbo(MPC_ON);

    // CPU/GPU throttling is polling by ChkBattery_OCP() in OEM_BATTERY.C
  }

//
// Code as below is doing battery RSOC process. We already processed it in CpuTurbo().
//

//   if( Read_AC_IN() && !Read_ACOFF() )
//   {
//     if( RelativeStateOfChgL > BATTURBOTEN )
//     {
// #if (PLATFORM_CHIP == INTEL)
//       CLEAR_MASK(BatteryAlarm,RSOCDISTURBO);
// #else
//       if( IS_MASK_CLEAR(pProject0,bRSOCAPUTURBO) )
//       {
//         ECSMI_SCIEvent(APU_DISTURBO);
//         SET_MASK(pProject0,bRSOCAPUTURBO);
//       }
// #endif
//     }
//     else if( RelativeStateOfChgL <= BATTRUBODIS )
//     {
// #if (PLATFORM_CHIP == INTEL)
//       SET_MASK(BatteryAlarm,RSOCDISTURBO);
//       cBATTThrottling |= 0x01;
// #else
//       if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
//       {
//         ECSMI_SCIEvent(APU_ENTURBO);
//         CLEAR_MASK(pProject0,bRSOCAPUTURBO);
//       }
// #endif
//     }
//   }
//   else
//   {
// #if (PLATFORM_CHIP == INTEL)
//     CLEAR_MASK(BatteryAlarm,RSOCDISTURBO);
// #else
//     if( IS_MASK_SET(pProject0,bRSOCAPUTURBO) )
//     {
//       ECSMI_SCIEvent(APU_ENTURBO);
//       CLEAR_MASK(pProject0,bRSOCAPUTURBO);
//     }
// #endif
//   }
}

void Chk_AC_STPP( BYTE AcDcMode )
{
  BYTE BatteryAlarmFlag;
  BYTE pProject0Flag;
  //
  // AcDcMode -- 0: AC only,
  //             1: AC + DC(<=10%)
  //
  if (AcDcMode) {
    BatteryAlarmFlag = HybridSTPP;
    pProject0Flag = bHYBAPUTURBO;    
  } else {
    BatteryAlarmFlag = ACSTPP;
    pProject0Flag = bAPUTURBO;
  }
  
  if (Read_AC_IN())
  {
    CpuTurbo(MPC_ON);
    if (ADPI2Sec != 0 && !AcDcMode){
      ADPI2Sec--;
    }
    if (GPU2Sec != 0 && !AcDcMode){
      GPU2Sec--;
    }
    CLEAR_MASK(M_BATAlarm,ADPOUT);
    CLEAR_MASK(GPUAlarm,GPUAlarm_ADPOUT);

    if(AcDcMode){
      if (ADPI_AvgData >= WATT_TO_ADC_BASE(MpcCpuThrottlingPoint_L) )
      {
          SET_MASK(M_BATAlarm,BatteryAlarmFlag);
          UpBatteryPState();
        } 
      else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcCpuThrottlingRecoverPoint_L))
      {
          CLEAR_MASK(M_BATAlarm,BatteryAlarmFlag);
          DownBatteryPState();
        }

      if(GPU2Sec == 0){
    	  if (nSwitchableBTNGET) { // DIS
            if( ADPI_AvgData >= WATT_TO_ADC_BASE(MpcGpuThrottlingPoint_L)) {
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();
    //        GpuThrottling(MPC_ON);
            } else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcGpuThrottlingRecoverPoint_L) ){
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();
    //        GpuThrottling(MPC_OFF);
          }
        }
      }
    }else{
    
      if (ADPI_AvgData >= WATT_TO_ADC_BASE(MpcAcCpuThrottlingPoint) )
      {
          SET_MASK(BatteryAlarm,BatteryAlarmFlag);
          UpBatteryPState();			
        } 
      else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcAcCpuThrottlingRecoverPoint))
      {
          CLEAR_MASK(M_BATAlarm,BatteryAlarmFlag);
          DownBatteryPState();
        }

      if(GPU2Sec == 0){
    	  if (nSwitchableBTNGET) { // DIS
            if( ADPI_AvgData >= WATT_TO_ADC_BASE(MpcAcGpuThrottlingPoint)) {
              SET_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              UpGPUPState();
    //        GpuThrottling(MPC_ON);
            } else if (ADPI_AvgData < WATT_TO_ADC_BASE(MpcAcGpuThrottlingRecoverPoint) ){
              CLEAR_MASK(GPUAlarm,GPUAlarm_GPUThrRe);
              DownGPUPState();
    //        GpuThrottling(MPC_OFF);
          }
        }
      }
    }
  }
}

void SetPowerBatteryparameter (void)
{
  {
    #if !Support_SMTADP
	//-->Stard A485D00011: Setting the adaptor watt(45W), because can not communication with PD
//A285D00017: Fix wake on lan fail
    //AdapterID = AdapterID_45W;
    //u4ACAdaptorCap = 45;
    //AdpWatt = 45;
    //SecAdpWatt = 45;
//A285D00017: Fix wake on lan fail
	//-->End A485D00011
    #endif
  }
  if ((AdapterID == AdapterID_45W) ||
      (AdapterID==AdapterID_NON_SUPPORT) || (AdapterID==AdapterID_36W) || (AdapterID==AdapterID_Lower36W))
  {
//     TurboBoostCP = A45WCP;
//     TurboBoostEn = A45Wchargerturbo;
//     TurboBoostthrottlEn = A45Wthrottling;
//     TurboBoostthrottlDis =A45Wthrottlingrecove;
//     TurboBoostDis = A45Wturborecover;
    MpcCP                          = A45W_CP;
    MpcCpuTurboDisablePoint        = A45W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = A45W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = A45W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = A45W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = A45W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = A45W_GPU_THROTTLING_OFF_POINT;

    MpcCpuThrottlingPoint_L          = A45W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = A45W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = A45W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = A45W_GPU_THROTTLING_OFF_POINT_L;
	    
    MpcAcCpuTurboDisablePoint        = A45W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = A45W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = A45W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = A45W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = A45W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = A45W_AC_GPU_THROTTLING_OFF_POINT;
    
    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

    ACACOFFEn = A45W_ACOFF;
	ACCutoff_Watt = 45;
  }
  else if( (AdapterID>=AdapterID_65W) )
  {
//     TurboBoostCP = A65WCP;
//     TurboBoostEn = A65Wchargerturbo;
//     TurboBoostthrottlEn = A65Wthrottling;
//     TurboBoostthrottlDis =A65Wthrottlingrecove;
//     TurboBoostDis = A65Wturborecover;

    MpcCP                          = A65W_CP;
    MpcCpuTurboDisablePoint        = A65W_CPU_TURBO_BOOST_OFF_POINT;
    MpcCpuTurboRecoverPoint        = A65W_CPU_TURBO_BOOST_ON_POINT;
    MpcCpuThrottlingPoint          = A65W_CPU_THROTTLING_ON_POINT;
    MpcCpuThrottlingRecoverPoint   = A65W_CPU_THROTTLING_OFF_POINT;
    MpcGpuThrottlingPoint          = A65W_GPU_THROTTLING_ON_POINT;
    MpcGpuThrottlingRecoverPoint   = A65W_GPU_THROTTLING_OFF_POINT;
	
    MpcCpuThrottlingPoint_L          = A65W_CPU_THROTTLING_ON_POINT_L;
    MpcCpuThrottlingRecoverPoint_L   = A65W_CPU_THROTTLING_OFF_POINT_L;
    MpcGpuThrottlingPoint_L          = A65W_GPU_THROTTLING_ON_POINT_L;
    MpcGpuThrottlingRecoverPoint_L   = A65W_GPU_THROTTLING_OFF_POINT_L;

    MpcAcCpuTurboDisablePoint        = A65W_AC_CPU_TURBO_BOOST_OFF_POINT;
    MpcAcCpuTurboRecoverPoint        = A65W_AC_CPU_TURBO_BOOST_ON_POINT;
    MpcAcCpuThrottlingPoint          = A65W_AC_CPU_THROTTLING_ON_POINT;
    MpcAcCpuThrottlingRecoverPoint   = A65W_AC_CPU_THROTTLING_OFF_POINT;
    MpcAcGpuThrottlingPoint          = A65W_AC_GPU_THROTTLING_ON_POINT;
    MpcAcGpuThrottlingRecoverPoint   = A65W_AC_GPU_THROTTLING_OFF_POINT;

    Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
    Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

    ACACOFFEn = A65W_ACOFF;
	ACCutoff_Watt = 65;
  }
#if (PLATFORM_CHIP == INTEL)  
  else if( (AdapterID==AdapterID_90W) 
           || (AdapterID==AdapterID_135W) 
           || (AdapterID==AdapterID_170W) 
           || (AdapterID==AdapterID_230W) )
  {
      MpcCP                          = A90W_CP;
      MpcCpuTurboDisablePoint        = A90W_CPU_TURBO_BOOST_OFF_POINT;
      MpcCpuTurboRecoverPoint        = A90W_CPU_TURBO_BOOST_ON_POINT;
      MpcCpuThrottlingPoint          = A90W_CPU_THROTTLING_ON_POINT;
      MpcCpuThrottlingRecoverPoint   = A90W_CPU_THROTTLING_OFF_POINT;
      MpcGpuThrottlingPoint          = A90W_GPU_THROTTLING_ON_POINT;
      MpcGpuThrottlingRecoverPoint   = A90W_GPU_THROTTLING_OFF_POINT;
	  if(IS14Inch){
      MpcCpuThrottlingPoint_L          = A90W_CPU_THROTTLING_ON_POINT_14_L;
      MpcCpuThrottlingRecoverPoint_L   = A90W_CPU_THROTTLING_OFF_POINT_14_L;
      MpcGpuThrottlingPoint_L          = A90W_GPU_THROTTLING_ON_POINT_14_L;
      MpcGpuThrottlingRecoverPoint_L   = A90W_GPU_THROTTLING_OFF_POINT_14_L;
  	  }else{
      MpcCpuThrottlingPoint_L          = A90W_CPU_THROTTLING_ON_POINT_15_L;
      MpcCpuThrottlingRecoverPoint_L   = A90W_CPU_THROTTLING_OFF_POINT_15_L;
      MpcGpuThrottlingPoint_L          = A90W_GPU_THROTTLING_ON_POINT_15_L;
      MpcGpuThrottlingRecoverPoint_L   = A90W_GPU_THROTTLING_OFF_POINT_15_L;
	  }  

      MpcAcCpuTurboDisablePoint        = A90W_AC_CPU_TURBO_BOOST_OFF_POINT;
      MpcAcCpuTurboRecoverPoint        = A90W_AC_CPU_TURBO_BOOST_ON_POINT;
      MpcAcCpuThrottlingPoint          = A90W_AC_CPU_THROTTLING_ON_POINT;
      MpcAcCpuThrottlingRecoverPoint   = A90W_AC_CPU_THROTTLING_OFF_POINT;
      MpcAcGpuThrottlingPoint          = A90W_AC_GPU_THROTTLING_ON_POINT;
      MpcAcGpuThrottlingRecoverPoint   = A90W_AC_GPU_THROTTLING_OFF_POINT;

      Chk_Hybrid_STPP_min_BattGasgauge = BAT_HYBOOST_DIS;
      Chk_Hybrid_STPP_Batt_Output_Power_Limit = 204; // 30W

      ACACOFFEn = A90W_ACOFF;
	  ACCutoff_Watt = 90;

  }
#endif
}

void SetTurbo_V_Thro(void)
{
  if (AdapterID==AdapterID_95W || AdapterID==AdapterID_NON_SUPPORT)  //90W
      {
          CMP1THRDATM = (VC1TriggerValue_90W>>8);  // threshold value buffer
      CMP1THRDATL = (BYTE)VC1TriggerValue_90W;
      }
    else if (AdapterID==AdapterID_120W)      //120W
       {
            CMP1THRDATM = (VC1TriggerValue_120W>>8);    // threshold value buffer
        CMP1THRDATL = (BYTE)VC1TriggerValue_120W;
       }
       else if (AdapterID==AdapterID_170W)  //170W
          {
              CMP1THRDATM = (VC1TriggerValue_170W>>8);  // threshold value buffer
          CMP1THRDATL = (BYTE)VC1TriggerValue_170W;
          }
}

#if  Support_SMTADP
void CovertADPID(void)
{
#if Support_TYPE_C
  OEM_TYPE_C_ADPID_HOOK();
#endif //Support_TYPE_C

	if(Read_AC_IN())
	{
		if(AD_ID_Detect_Delay != 1)
		{
  		return;
		}
	}
 // if( !Read_AC_IN() || (!ADPID_ON_Detect()) )
  if( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) )//|| (!ADPID_ON_Detect()) )
  {
    return;
  }

#if Support_TYPE_C
	//if(!Read_DCIN_ATTACHED2())
	{
		cADPIDTemp &= 0x0F;
		cADPIDTemp |= (TypeCAdpID<<4);
		if(CurrentTypeCV!=0)
		{
			ADPI_MUL = CurrentTypeCV *5/100;	//Type-C voltage Unit:50mV
		}
		else
		{
			ADPI_MUL = ClassicAdaptor_ADPI_MUL;
		}
	}
	//else
#endif //Support_TYPE_C
    #if 0
	{
	  cADPIDTemp &= 0x0F;
	  if( (API_ID > ADP230W_Upper) || ( API_ID < ADP45W_Lower ) )
	  {
	    cADPIDTemp |= (AdapterID_NON_SUPPORT<<4);
	  }
	  else if( (API_ID >= ADP230W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_230W<<4);
	  }
	  else if( (API_ID >= ADP170W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_170W<<4);
	  }
	  else if( (API_ID >= ADP135W_Lower) )
	  {
	    cADPIDTemp |= (AdapterID_135W<<4);
	  }
	  else if( API_ID >= ADP90W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_90W<<4);
	  }
	  else if( API_ID >= ADP65W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_65W<<4);
	  }
	  else if( API_ID >= ADP45W_Lower )
	  {
	    cADPIDTemp |= (AdapterID_45W<<4);
	  }
    ADPI_MUL = ClassicAdaptor_ADPI_MUL;
	} 
    #endif

  ADP_ID_REC = API_ID; // Log the raw ADP_ID value for debug tracing.

  if( IS_MASK_CLEAR(pEdgeProjectFlag1,pEdgeADPIDReady) )
  {
    if( AdapterID == (cADPIDTemp>>4) )
    {
      cADPIDTemp++;
    }
    else
    {
      AdapterID = (cADPIDTemp>>4);
      cADPIDTemp &= 0xF0;
      CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
    }
    if( (cADPIDTemp&0x0F) <= 4 )
    {
      return;
    }
    else
    {
      switch(AdapterID)
      {
#if Support_TYPE_C
        case AdapterID_Lower36W:
          u4ACAdaptorCap = CurrentTypeCWatt;
          AdpWatt = CurrentTypeCWatt;
          break;
#endif  //Support_TYPE_C          
        case AdapterID_36W:
          u4ACAdaptorCap = 36;
          AdpWatt = 36;
          break;
        case AdapterID_45W:
          u4ACAdaptorCap = 45;
          AdpWatt = 45;          
          //AttachedPowerNOTsupportEn;
          break;        
		case AdapterID_60W:
          u4ACAdaptorCap = 60;
          AdpWatt = 60;
          break;
        case AdapterID_65W:
          u4ACAdaptorCap = 65;
          AdpWatt = 65;
          break;
        case AdapterID_90W:
          u4ACAdaptorCap = 90;
          AdpWatt = 90;
          break;
        case AdapterID_135W:
          u4ACAdaptorCap = 135;
          AdpWatt = 135;
          break;
        case AdapterID_170W:
          u4ACAdaptorCap = 170;
          AdpWatt = 170;
          break;
        case AdapterID_230W:
          u4ACAdaptorCap = 230;
          AdpWatt = 230;
          break;
        case AdapterID_NON_SUPPORT:
          AttachedPowerNOTsupportEn;
          u4ACAdaptorCap = 0;
          AdpWatt = 0;
          break;
      }
      SecAdpWatt = AdpWatt;
      SET_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
      Update_ChargeIC();        // [20140304 ITE CM.Wu]
      SetPowerBatteryparameter();
      SET_MASK(pEdgeProjectFlag1,pEdgeACINOUTTrigger);
//[-start-131213-T130124_9-add]//
      ADPID_ON_DIS();
//[-end-131213-T130124_9-add]//
    }
  }
}

void Edge_SMTADPProcess()
{
  #if !Support_MAXPWR
  return;
  #endif

  //
  // !! Attention !!
  // Code in here is for Edge Lonestar&Gordon specification only.
  // Required: 45W adapter is limited support in 65W shipped system(CPU 15W + dGPU 15~25W)
   // Limited Support: CPU Frequency is set to LFM under AC Adapter only(When battery capacity under 20%).
  //
  //Check System design watt and setting the limited support watt adaptor.
#if Support_TYPE_C
	//if(!Read_DCIN_ATTACHED2() && Read_AC_IN())
	if( Read_AC_IN())
	{
		if(OneOfTwoBattExistGet && (WorkingBattRSOC >= TypeCLimitedSupport_BatteryRSOC))
		{
		  if(nSwitchableBTNGET)
		  {	//DIS is 65/90 watt system
		  	  	LimitedSupportAdpWatt = A65W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = 0;	//No not-support adaptor		  	  	
				UnknowAdpWatt = A65W_System_Unknow_TypeC_Adapter;
		  }
		  else
		  {	//UMA is 45 watt system.
		  		LimitedSupportAdpWatt = A45W_System_Limited_support_TypeC_Adapter;
		  		NotSupportAdpWatt = 0;	//No not-support adaptor		  		
				UnknowAdpWatt = A45W_System_Unknow_TypeC_Adapter;

		  }
		}
		else
		{
		  if(nSwitchableBTNGET)
		  {
		  	  	LimitedSupportAdpWatt = A65W_System_Limited_support_TypeC_Adapter;
		  	  	NotSupportAdpWatt = A65W_System_Notsupport_TypeC_Adapter;
				UnknowAdpWatt = 0;
		  }
		  else
		  {	//UMA is 45 watt system.
		  		LimitedSupportAdpWatt = A45W_System_Limited_support_TypeC_Adapter;
		  		NotSupportAdpWatt = A45W_System_Notsupport_TypeC_Adapter;
				UnknowAdpWatt = 0;
		  }
		}
	}
	//else
#endif //Support_TYPE_C
  	//{
#if Support_TYPE_C
	//	LimitedSupportAdpWattUpper = 0;
#endif //Support_TYPE_C
	//  if(nSwitchableBTNGET)
	//  {	//DIS is 65 watt system
	//  	  LimitedSupportAdpWatt = A65W_System_Limited_support_Adapter;
	//  	  NotSupportAdpWatt = A65W_System_Not_support_Adapter;
	//  }
	//  else
	//  {	//UMA is 45 watt system.
	//  	LimitedSupportAdpWatt = A45W_System_Limited_support_Adapter;
	//  	NotSupportAdpWatt = A45W_System_Not_support_Adapter;
	//  }
	//}
 // if(((AdpWatt==LimitedSupportAdpWatt) && Read_DCIN_ATTACHED2())
//#if Support_TYPE_C
//  	|| (((AdpWatt < LimitedSupportAdpWatt) && (AdpWatt >= NotSupportAdpWatt)) && !Read_DCIN_ATTACHED2())
//#endif //Support_TYPE_C
//  	) 
  if((AdpWatt < LimitedSupportAdpWatt) && (AdpWatt >= NotSupportAdpWatt))
  { //dGPU is equipped which means it is 65W system. 
    if (OneOfTwoBattExistGet) {  //battery exist
      if (WorkingBattRSOC <= BAT_HYBOOST_DIS) {
        //
        // We use b2ProCH_EXTVGA to turn on prochot.
        //
//        SET_MASK(pEdgeProjectFlag1, pEdgeGPUOCTHROT); // dGPU throttling
        SET_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
        SET_MASK(Thro_Status, b2ProCH_EXTVGA);        // CPU throttling
	      SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
        cBATTThrottling = CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
      }  else if( WorkingBattRSOC > BAT_HYBOOST_EN ) {
//        CLEAR_MASK(pEdgeProjectFlag1, pEdgeGPUOCTHROT);
        if(IS_MASK_SET(Thro_Status,b2ProCH_EXTVGA)){
          cBATTThrottling = 0;
        }
        CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
        CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
				CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
      }
    } else { // battery detach, AC only mode.
//      SET_MASK(pEdgeProjectFlag1, pEdgeGPUOCTHROT);
      SET_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
      SET_MASK(Thro_Status, b2ProCH_EXTVGA);
	    SET_MASK(uProject0,NeedRecoveryLimitedSupportADP);
      SET_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
      cBATTThrottling = CPUMAXPSTATE;
      cGPUThrottling = GPUMAXPSTATE;
    }
  }
  else if(IS_MASK_SET(uProject0,NeedRecoveryLimitedSupportADP))
  {
    if(IS_MASK_SET(Thro_Status,b2ProCH_EXTVGA)){
      cBATTThrottling = 0;
    }
    CLEAR_MASK(GPU_THROTTLE_SRC, ADP_LIMIT_SUPPORT);
    CLEAR_MASK(Thro_Status, b2ProCH_EXTVGA);
		CLEAR_MASK(uProject0,NeedRecoveryLimitedSupportADP);
    CLEAR_MASK(GPUAlarm,GPUAlarm_ADPLimRe);
  }

//  return;

//  if(( IS_MASK_SET(pEdgeProjectFlag1,pEdgeADPIDReady) && Read_DCIN_ATTACHED2()) //classic
//#if Support_TYPE_C
//  	|| (((AdpWatt < UnknowAdpWatt)||(AdpWatt == 0)) && !Read_DCIN_ATTACHED2())
//#endif //Support_TYPE_C
//  	)
  if((AdpWatt < UnknowAdpWatt)||(AdpWatt == 0))
  {
//    if( nSwitchableBTNGET )
//    {
      if( (AdapterID==AdapterID_NON_SUPPORT) ||  (AdapterID == AdapterID_Lower36W) )
      {
        //AttachedPowerNOTsupportEn;
        if(( SystemIsS0 || (AdapterID==AdapterID_NON_SUPPORT) ||  (AdapterID==AdapterID_Lower36W)) && OneOfTwoBattExistGet)
        {
          SET_MASK(ACOFF_SOURCE, AIRADP);
          CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
          ACOFF_HI();
        }
        else
        {
          CLEAR_MASK(ACOFF_SOURCE, AIRADP);
          if( Read_AC_IN() )
          SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
          ACOFF_LOW();
        }
        SET_MASK(pEdgeProjectFlag1,pEdgeNonSUPPORTADP);
      }
      else
      {	//Normal support adaptor
        //AttachedPowerNOTsupportDis;
        CLEAR_MASK(pEdgeProjectFlag1,pEdgeNonSUPPORTADP);
      }
    }
	    if( (AdpWatt < NotSupportAdpWatt) && (AdpWatt > 0) )
	    {	// Not-support adaptor
	      AttachedPowerNOTsupportEn;
	      if( SystemIsS0 && OneOfTwoBattExistGet )
	      {	// "Battery exist + S0" set to DC only mode
	        SET_MASK(ACOFF_SOURCE, AIRADP);
	        CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
	        ACOFF_HI();
	      }
        else if(SystemIsS0 && NoBattExistGet )
        {
           SET_MASK(SysStatus,ERR_ShuntDownFlag);
           ProcessSID(AC_CUTOFF_ID);		
           ACOFF_HI();
	      }
	      else
	      { //S3/S4/S5 need charging
	        CLEAR_MASK(ACOFF_SOURCE, AIRADP);
	        if( Read_AC_IN() )
	        {
	        	SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
	        }
	        ACOFF_LOW();
	      }
	      SET_MASK(pEdgeProjectFlag1,pEdgeNonSUPPORTADP);
	      SET_MASK(uProject0,NeedRecoveryNotSupportADP);
	    }
	    else if((AdpWatt != 0) && IS_MASK_SET(uProject0,NeedRecoveryNotSupportADP))
	    {	//Normal support adaptor
	      AttachedPowerNOTsupportDis;
	      CLEAR_MASK(pEdgeProjectFlag1,pEdgeNonSUPPORTADP);
	        CLEAR_MASK(ACOFF_SOURCE, AIRADP);
	        if(!ACOFF_SOURCE)
	        {
		        if( Read_AC_IN() )
		        {
		        	SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
		        }
	        	ACOFF_LOW();
	        }
	      CLEAR_MASK(uProject0,NeedRecoveryNotSupportADP);
	    }
}

#endif

void  ScanADC_BoardID(void)
{
#if Support_ADC_BoardID

  if(BoardID_readyGET)
    return;
  
  //VCH3CTL |= 0x80;        // write clear data vaild flag
  //BoardADCID = ((((WORD)VCH3DATM << 8) + VCH3DATL)*(0.92));    
  if(BoardADCID <= BoardADC_LV0)
  {
    BoardID = SDV_board;//SDV
  }   
  else if((BoardADCID > BoardADC_LV0)&& (BoardADCID <= BoardADC_LV1))
  {
    BoardID = Board_ID1;//RESERVE1
  } 
  else if((BoardADCID > BoardADC_LV1)&& (BoardADCID <= BoardADC_LV2))
  {
    BoardID = FVT_board;//FVT
  }
  else if((BoardADCID > BoardADC_LV2)&& (BoardADCID <= BoardADC_LV3))
  {
    BoardID = SIT_board;//SIT
  }
  else if((BoardADCID > BoardADC_LV3)&& (BoardADCID <= BoardADC_LV4))
  {
    BoardID = Board_ID2;//RESERVE2
  }
  else if(BoardADCID > BoardADC_LV4)
  {
    BoardID = SVT_board;//SVT
  }
  BoardID_readyEN;  
#endif  
}


void CPUtuborONOFF(BYTE OnOff) 
{
  if (OnOff == Enable){
    cADPThrottling = 0x00;
    CLEAR_MASK(GPUAlarm,GPUAlarm_TubONOFF);
    DownGPUPState();			
  }else{             
    SET_MASK(GPUAlarm,GPUAlarm_TubONOFF);
#if (PLATFORM_CHIP == INTEL )
    if(pCPUBasePStage != 0){
      cADPThrottling = CPUturboDisPstage; 
    }
#else  //AMD
    cADPThrottling = CPUturboDisPstage_PWR; 
#endif
  }
}
//
//*****************************************************************************
 

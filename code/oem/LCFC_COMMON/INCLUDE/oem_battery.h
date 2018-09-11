//*****************************************************************************
//
//  oem_battery.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef BATTERY_H
#define BATTERY_H

/*-----------------------------------------------------------------------------
 * Data Parameter definition
 *---------------------------------------------------------------------------*/
#define SMbusTOC    200     // SMbus re-try counter


//-----------------------------------------------------------------
// Define smart battery command
//-------------------------------------------- --------------------
#define C_access             0x00     // manufacture access
#define C_RCalarm            0x01     // remaining capacity alarm
#define C_RTalarm            0x02     // remaining time alarm
#define C_mode               0x03     // battery mode
#define C_AtRate             0x04     // AtRate
#define C_AtRateFull         0x05     // AtRate time to full
#define C_AtRateEmpty        0x06     // AtRate time to empty
#define C_AtRateOK           0x07     // AtRate OK
#define C_temp               0x08     // temperature
#define C_volt               0x09     // voltage
#define C_current            0x0A     // current
#define C_AVcurrent          0x0B     // average current
#define C_MaxError           0x0C     // maximum error
#define C_RSOC               0x0D     // relative state of charge
#define C_ASOC               0x0E     // absolute state of charge
#define C_RMcap              0x0F     // remaining capacity
#define C_FCcap              0x10     // full charge capacity
#define C_RunTimeEmpty       0x11     // run time to empty
#define C_AVTimeEmpty        0x12     // average time to empty
#define C_AVTimeFull         0x13     // average time to full
#define C_ChargingI          0x14     // charging current
#define C_ChargingV          0x15     // charging voltage
#define C_BatStatus          0x16     // battery status
#define C_CycleCount         0x17     // cycle count
#define C_DCap               0x18     // design capacity
#define C_DVolt              0x19     // design voltage
#define C_Spec               0x1A     // spec information
#define C_Date               0x1B     // manufacture date
#define C_SerialNo           0x1C     // serial number
#define C_Mname              0x20     // manufacturer name
#define C_Dname              0x21     // device name
#define C_Dchem              0x22     // device chemistry
#define C_Mdata              0x23     // manufacture data
#define C_SmartCharge        0x26     // Smart Charge(Quick Charge)
#define C_OptMfgFunc5        0x2F     // OptionalMfgFunction5
#define C_BattUsageCond      0x30     // Battery Usage Condition
#define C_SnapShotData       0x31     // Snap shot data at permanent failure
#define C_BattLogData1       0x32     // Battery log data #1
#define C_BattLogData2       0x33     // Battery log data #2
#define C_D_FET              0x34     // Ship Mode and Additional Over Discharge Mode.
#define C_FW_Status          0x35     // FW Update Status.
#define C_FW_Data            0x36     // FW Update Data.
#define C_BAT_Info           0x37     // Get Battery Information.
#define C_WarrantyPeriod     0x38     // Warranty Period
#define C_BattMode2          0x39     // Battery Mode 2
//#define Reserved           0x3A     // Reserved
#define C_FETTemp            0x3B     // FET Temperature
#define FirstUsedDate        0x3F     // FirstUsedDate

#define C_Cell2              0x3E     // manufacture data  //OptionalMfgFunction2
#define C_LVMfgFun2          0x3E     // LV Smart Battery
#define C_Cell3              0x3D     // manufacture data
#if Lenovo_Authentication
#define AuthenticationCmd    0x3C
#define delayReadCnt    15
#define LeftRotateBYTE(x,bit) (((BYTE)(x) << (bit)) | ((BYTE)(x) >> (8-(bit))))
#else
#define C_Cell4              0x3C   // manufacture data
#endif


typedef struct RSmbusBStruct
{
    BYTE    cmd;
    BYTE   *smbdataA;
    BYTE   *smbdataB;
  BYTE    block_limit;
} sRSmbusBStruct;
//-----------------------------------------------------------------
// Charge and discharge charger rules
//-------------------------------------------- --------------------

#if (BatLowVot_Rule == LowVot_LGNotLG)
#define LG_3SLowVot      8250
#define Sanyo_3SLowVot    8500
#endif
#if (BatLowVot_Rule == LowVot_3S4S)
#define BAT3S4SVolt_MaxValue        (20000)
#define BAT3S4SVot_Divide   14000
#define BAT3SLowVot      8900
#define BAT4SLowVot      11900
#endif

extern void vPollingBatteryData(void);
extern void Charge_or_Discharge(void);
extern void  AC_DC_EVENT(void);
extern void  CheckAdapterInOut(void);
extern void ChkBattery_Percl();


//----------------------------------------------------------------------------
// adapter types
//----------------------------------------------------------
#define AC_TYPE_UNKNOWN         0
#define AC_TYPE_45W             45
#define AC_TYPE_50W             50
#define AC_TYPE_60W             60
#define AC_TYPE_65W             65
#define AC_TYPE_70W             70
#define AC_TYPE_90W             90
#define AC_TYPE_120W            120
#define AC_TYPE_130W            130
#define AC_TYPE_150W            150
#define AC_TYPE_230W            230
#define AC_TYPE_240W            240
#define BAT_HYBOOST_EN         10
#define BAT_HYBOOST_DIS        10
#define BAT_CPU_TURBO_EN       10
#define BAT_CPU_TURBO_DIS      10

#define BOTH_BAT_CPU_TURBO_DIS 5
#define BOTH_BAT_CPU_TURBO_EN  5

#define BATHYBITOTPTHOEN       60
#define  BATHYBITOTPTHODIS     55
#define BATHYBITOTPSHUT        70


#define BuildInBat		IS14Inch

#define BATTERY_HASH_VERIFY_DEBOUNCE_COUNT      (10)    //A285D00050: add battery verify debounce sequence

//-------------------------------------------------------------------------------
// battery function
//-------------------------------------------------------------------------------
extern void OEM_PollingBatData_TASK(int battery_id);
extern void FirstGetBatData(int battery_id);
extern void Battery1Sec(int battery_id);
extern void Chk_Shutdown_Volt(void);
extern void Chk_CHG_TimeOut(int battery_id);
extern void Chk_BatSMbusFailCount(int battery_id);
extern void Unlock_ShipMode(int battery_id);
extern void Unlock_ShipMode_ACIN (void);
extern void RST_ChgTimeOutCnt(int battery_id);
extern void Lock_ShipMode(void);
extern void Chk_Battery_Full(int battery_id);
extern void UpBatteryPState();
extern void DownBatteryPState();
extern void UpGPUPState();
extern void DownGPUPState();
extern void Battery100ms(int battery_id);
extern void ChkBattery_abnormal(int battery_id);
extern void LV_BAT_Authentication(void);
extern void OEMauthentication(void);
extern void RSMRST_shutdown(void);
extern void UpdateECNameSpace(int battery_id);
extern void SetBatteryStopChargeStateFlag (BYTE battery_id, BYTE event_id);


#if Lenovo_Authentication
extern void LAuthProcess(int battery_id);
#endif

extern void ChkFirstUsedDate(int battery_id);
extern void ACOffProcess(BYTE);
extern void BATLEARNProcess(BYTE);
extern void CriticalLowBATWakeS3(void);
extern void IBMbatteryStore(void);
extern void TWR_BATClear(int battery_id);
extern void TWR_Initial(void);
extern void EDGEPMBATTProcess(int battery_id);
extern void TPG_EXTBATCmd(void);
extern void Battery_Initial(void);
extern void CHK_Capacity_trip_point(void);
extern void Authentication(int battery_id);
extern void GetBatData(BYTE index, int battery_id);
extern void IBMChargeCondition(int battery_id);
extern void BatteryFirstUseDay(void);
extern void Switch2Trickle_Charge(BYTE battery_id);
extern void CHK_Trickle_Charge(BYTE event_id);
extern void Trickle2Fast_Charge(int battery_id, BOOL Chg_sts);
extern void Trickle_Charge_Mode(int battery_id);
extern void Select_Battery(void);
extern BOOL BAT_Trickle_Charge_flow(BYTE event_id);
extern void Battery_charge_Chk();
extern void Battery_discharge_Chk();
extern void Switch_Battery(void);
extern BOOL Chk_Stop_charge_flag();
extern void Chk_Trickle_Setting();
extern void SettingBattOCP(BYTE battery_id);

#if Support_TWO_BATT
extern void Switch_Battery_Process (void);
#define BAT_switch_delay_time  1// 20   //5ms
#define TWR_BAT(x,y)   (*(&x + (y << 8)))   
#define OEM_BAT(x,y)   (*(&x + (y << 7)))
#define OEM_BAT_1B(x,y)   (*(&x + y))
#define BAT_ECSMI_SCIEvent(x,y)  ECSMI_SCIEvent(x + (y<< 1))
#define BAT_SEL_M  1
#define BAT_SEL_S  5
#define BAT_SEL_INIT  3
#define BAT_state_target(x) if(x != Current_BAT_State){  \
                             BAT_switch_ready_CLR;BAT_Status &= ~BAT_switch_state_target;BAT_Status |= (x << 4);} 
#define Current_BAT_State  (BAT_Status & BAT_switch_state_now)
#define Target_BAT_state ((BAT_Status & BAT_switch_state_target) >> 4)
#define Current_Battery   ((BAT_Status & BIT2) >> 2)   //Current_BAT_id
#define AllBattExistGet	(BattExistGet && SecBattExistGet)
#define OneOfTwoBattExistGet		(BattExistGet || SecBattExistGet)
#define NoBattExistGet	(!BattExistGet && !SecBattExistGet)
#define TotalBattRSOC	(RelativeStateOfChgL + SecRelativeStateOfChgL)
#define WorkingBattRSOC	TWR_BAT(RelativeStateOfChgL,Current_Battery)
#define BAT_SEQ  ((u4Cfgspace4 & u4Cfgspace4BATseq) >> 5)

#else
#define TWR_BAT(x,y)   (x)   
#define OEM_BAT(x,y)   (x)
#define OEM_BAT_1B(x,y)   (x)
#define BAT_ECSMI_SCIEvent(x,y)  ECSMI_SCIEvent(x)
#define BAT_state_target(x) BAT_switch_ready_CLR;BAT_Status = BAT_SEL_INIT + (BAT_SEL_INIT << 4);
#define Current_Battery   ((BAT_Status & BIT2) >> 2)   //Current_BAT_id  //A285D00001+

#define AllBattExistGet	BattExistGet
#define OneOfTwoBattExistGet		BattExistGet
#define NoBattExistGet	!BattExistGet
#define TotalBattRSOC	RelativeStateOfChgL
#define WorkingBattRSOC	RelativeStateOfChgL
#endif 
#define MAX_TIME_TO_EMPTY           (48*60)

#endif


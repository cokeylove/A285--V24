//*****************************************************************************
//
//  oem_adc.h
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

#ifndef OEM_ADC_H
#define OEM_ADC_H  

//----------------------------------------------------------------------------
// The function of scaning ADC input
//----------------------------------------------------------------------------
#define ADCChannel_0     0x80
#define ADCChannel_1     0x81
#define ADCChannel_2     0x82
#define ADCChannel_3     0x83
#define ADCChannel_4     0x84
#define ADCChannel_5     0x85
#define ADCChannel_6     0x86
#define ADCChannel_7     0x87

#define ADCChannel_En     BIT7 + BIT4

#define IOUT          2.5
//#define  ADPI_MUL    2
#define  ClassicAdaptor_ADPI_MUL    20	//Classic adaptor ADPI_MUL setting to 20V

#define AdapterID_NON_SUPPORT   0x0F//0xFF
// +

#define  AdapterID_45W           0x01
#define  AdapterID_65W      0x02
#define  AdapterID_90W      0x03
#define  AdapterID_135W      0x04

#define AdapterID_95W           0x05
#define AdapterID_120W          0x06
#define AdapterID_170W          0x07
#define AdapterID_230W          0x08
#define AdapterID_36W          0x09
#define AdapterID_Lower36W      0x0A
#define AdapterID_60W           0x0B

#define ADP230W_Lower     905   //2.651v
#define ADP230W_Upper     1000  //2.929v
#define ADP170W_Lower     740   //2.149v
#define ADP170W_Upper     886   //2.59v
#define ADP135W_Lower     567   //1.663v
#define ADP135W_Upper     719   //2.109v
#define ADP90W_Lower      400   //1.172v
#define ADP90W_Upper      552   //1.168v
#define ADP65W_Lower      236   //0.693v
#define ADP65W_Upper      387   //1.134v
#define ADP45W_Lower      80    //0.234v
#define ADP45W_Upper      226   //0.663v
// -

// CMW 20121022 +
#define VC0TriggerValue      400  // 0.8V
#define VC0RestoreValue      573  // 1.8V

#define VC1TriggerValue      337
#define VC1TriggerValue_90W    346  // 0.99V
#define VC1TriggerValue_120W  403  // 1.3V
#define VC1TriggerValue_170W  573  // 1.85V
#define VC1RestoreValue      256  // 0.8V

#define VC2TriggerValue      0
#define VC2RestoreValue      0
// CMW 20121022 -
//----------------------------------------------------------------------------
// Define used ADC channel
//----------------------------------------------------------------------------
// #define ADC_Channel_SUPPORT (ADC_Channel_1 | ADC_Channel_2 | ADC_Channel_3 | ADC_Channel_4 | ADC_Channel_5 | ADC_Channel_6 | ADC_Channel_7 ) //A485D00014-
#define ADC_Channel_SUPPORT ( ADC_Channel_3 | ADC_Channel_4 | ADC_Channel_5 | ADC_Channel_6 | ADC_Channel_7)  //A485D00014 +
#define API_ID_Channel	0x07

#define ADC_Data0    
#define ADC_Data1    //M_BATTEMP ////A485D00014  remove both ADC scan
#define ADC_Data2    //S_BATTEMP  //A485D00014    remove both ADC scan
#define ADC_Data3    BoardADCID
#define ADC_Data4    FAN_ID
#define ADC_Data5    ADP_I
#define ADC_Data6    Device_detect
#define ADC_Data7    PSYS//API_ID //A485D00016 Modify API_ID-> PSYS


//----------------------------------------------------------------------------
// Define Board ADC ID 
//----------------------------------------------------------------------------
#define BoardADC_LV0      25
#define BoardADC_LV1      68
#define BoardADC_LV2      129
#define BoardADC_LV3      195
#define BoardADC_LV4      249

#define SDV_board        0x10         // 0 -- SDV phase
#define FVT_board        0x20         // 1 -- FVT phase
#define SIT_board        0x30         // 2 -- SIT phase
#define SVT_board        0x40         // 3 -- SVT phase
#ifndef Support_non_standard_boardID1 
#define Board_ID1        0x50         // reserve
#endif //Support_non_standard_boardID1 
#ifndef Support_non_standard_boardID2
#define Board_ID2        0x60         // reserve
#endif //Support_non_standard_boardID2

#define SDV_PHASE()             ( (BoardID & (~BoardID_ready)) == SDV_board)
#define FVT_PHASE()             ( (BoardID & (~BoardID_ready)) == FVT_board)
#define SIT_PHASE()             ( (BoardID & (~BoardID_ready)) == SIT_board)
#define SVT_PHASE()             ( (BoardID & (~BoardID_ready)) == SVT_board)

extern void ScanADCChannel(void);
extern void Init_VC(BYTE Group);
extern void Chk_Wrong_ADP(void);
extern void Chk_Hybrid_STPP(void);
extern void Chk_AC_STPP(BYTE);
extern void SetPowerBatteryparameter(void);
extern void SetTurbo_V_Thro(void);
extern void CovertADPID(void);
extern void Edge_SMTADPProcess(void);
extern void ScanADC_BoardID(void);
extern void CPUtuborONOFF(BYTE OnOff);
extern void Init_ADC(void);
// Max Power Control Finite state machine
#define SystemOverCP	0
#define SystemOverChargerTurboPoint_ChargerTurboOff	1
#define SystemOverChargerTurboPoint_ChargerTurboOn  2

//
// Ex1. 55W/20V-> 2.75A (Charger IC output 1A=0.2V)-> 0.55V ADP_I
// Ex2. 55W/12V-> 4.58333A (Charger IC output 1A=0.2V)-> 0.916666V ADP_I
// A*0.2*(1024/3)=Aadc ,W/V=A -> (W/V)*0.2*(1024/3)=Aadc -> (W/V)*68.26667=Aadc
// Watt to ADC: Aadc=(W*68.266670)/V
//
#define WATT_TO_ADC_BASE(x)   (((WORD)x * 68.26667) / ADPI_MUL)

//#define GpuThrottling(x)      ( x == 0 ? CLEAR_MASK(pEdgeProjectFlag1, pEdgeGPUOCTHROT) : SET_MASK(pEdgeProjectFlag1, pEdgeGPUOCTHROT) )
#define GpuThrottling(x)      ( x == 0 ? CLEAR_MASK(GPU_THROTTLE_SRC, MAX_POWER_CTRL) : SET_MASK(GPU_THROTTLE_SRC, MAX_POWER_CTRL) )
#define CpuThrottling(x)      if (x == 0) \
                              { \
                                CLEAR_MASK(M_BATAlarm, HybridSTPP); \
                                DownBatteryPState(); \
                              } else { \
                                SET_MASK(M_BATAlarm, HybridSTPP); \
                                UpBatteryPState(); \
                              }
                              
#define IsChargeOn()          IS_MASK_CLEAR(M_BATStopChgStatH, (HybridModeStopChg | ENBOOSTSTOPCHG))
#define IsCpuThrottlingOn()   IS_MASK_SET(M_BATAlarm, HybridSTPP)
//#define IsGpuThrottlingOn()   IS_MASK_SET(pEdgeProjectFlag2, pEdgeGPUOCTHROT)
#define IsGpuThrottlingOn()   IS_MASK_SET(GPU_THROTTLE_SRC, MAX_POWER_CTRL)
#define IsCpuTurboOn()        IS_MASK_CLEAR(pProject0, bHYBAPUTURBO)

#define MPC_ON                1
#define MPC_OFF               0


#define CPUturboDisPstage_PWR  1	//Max power control, same as "No turbo on/off", Step by step count up or count down even the CPU turbo off is not P1(P-satge 1).
#define CPUturboDisPstage  (pCPUBasePStage + 1)	//System can not change to Base P-stage, need change to base (P+1)-stage

 
#endif

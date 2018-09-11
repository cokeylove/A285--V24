/*-----------------------------------------------------------------------------
 * TITLE: EM_CHGIC_25700.H
 *
 * Author : Ivan
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/
 
#ifndef OEM_CHGIC_25700_H
#define OEM_CHGIC_25700_H

/*
*********************************************************
*     #EC ram define
*********************************************************
*/         
#define  CHGIC_ReadCmd0x31L       CHGIC_RAM0
  #define IFAULT_LOW             BIT4      
  #define IFAULT_HI              BIT5
  #define EN_IDCHG_REG           BIT6
  #define EN_EXTILIM             BIT7

//#define  CHGIC_ReadCmd0x31H       CHGIC_RAM1
//#define  CHGIC_ReadCmd0x3EL       CHGIC_RAM2
//#define  CHGIC_ReadCmd0x3EH       CHGIC_RAM3
#define  CHGIC_ReadCmd0x30L       CHGIC_RAM4
  #define AUTO_WAKEUP_EN        BIT0      //Auto wakeup enable
  #define FET_LATCHOFF          BIT3
#define  CHGIC_ReadCmd0x30H       CHGIC_RAM5

#define  CHGIC_ReadCmd0x33L       CHGIC_RAM6
  #define PROCHOT_HOST_EN   BIT5
  #define VSYS_T0           BIT6 
  #define VSYS_T1           BIT7
#define  CHGIC_ReadCmd0x33H       CHGIC_RAM7
  #define ICRIT_0        BIT3 
  #define ICRIT_1        BIT4 
  #define ICRIT_2        BIT5
  #define ICRIT_3        BIT6 
  #define ICRIT_4        BIT7 
#define  CHGIC_ReadCmd0x34L       CHGIC_RAM8
  #define BATPRES        BIT1 
  #define VSYS           BIT2
  #define PROFILE_5      BIT5
#define  CHGIC_ReadCmd0x34H       CHGIC_RAM9

#define  CHGIC_ReadCmd0x12L       CHGIC_RAMA                      
 #define ChargeInhibit    BIT0
 #define IDPM_EN          BIT1
// #define TurboBoost       BIT2      //
 #define EN_LDO           BIT2      //
 #define IADP_RATIO       BIT4      //
 #define BATLearn         BIT5      //
 #define IS_CHG_inhibit   IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit)
 #define CLR_IDPM_EN      CLEAR_MASK(CHGIC_ReadCmd0x12L, IDPM_EN)
 #define IS_BATLearn      IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn)

#define  CHGIC_ReadCmd0x12H       CHGIC_RAMB
 #define SwitchingFreq    BIT1      //0:800KHz,1:1200MHz
 #define AUDIO_Freq       BIT2
 #define OTG_ON_ChrgOK    BIT3
 #define IDPM_AUTO_DIS    BIT4
 #define LOW_POWER_MODE_EN	BIT7

#define  CHGIC_SMbusFailCnt       CHGIC_RAMC
#define  CHGIC_ptr                CHGIC_RAMD
#define  CHGIC_InputCurrentL      CHGIC_RAME
#define  CHGIC_InputCurrentH      CHGIC_RAMF
#define  CHGIC_ReadCmd0x14L       CHGIC_RAM10
#define  CHGIC_ReadCmd0x14H       CHGIC_RAM11
#define  CHGIC_ReadCmd0x15L       CHGIC_RAM12
#define  CHGIC_ReadCmd0x15H       CHGIC_RAM13
#define  CHGIC_ReadCmd0x3FL       CHGIC_RAM14
#define  CHGIC_ReadCmd0x3FH       CHGIC_RAM15
#define  CHGIC_ReadTempL          CHGIC_RAM16 
#define  CHGIC_ReadTempH          CHGIC_RAM17
#define  CHGIC_Status             CHGIC_RAM18
 #define  ChgIcInitial    BIT0
#define  ChargerIC_Init           CHGIC_RAM19
//A285D00047: fix charge ic volatage adapter
#define  ChargeIC_InitCnt       CHGIC_RAM1A

#define CHARGE_IC_VOLTAGE_SET_COUNT     (6)

//-----------------------------------------------------------------
// Define charger command
//-------------------------------------------- --------------------
#define C_ChargerMode      0x12    // Set charger mode M+
#define C_ChargeCurrent    0x14    // Set charge curretn
#define C_ChargeVoltage    0x15    // Set charge voltage
#define C_ChargeOption1    0x30
#define C_ChargeOption2    0x31
#define C_ChargeOption3    0x32
#define C_ProchotOption0   0x33
#define C_ProchotOption1   0x34
#define C_InputVoltage     0x3D
#define C_MinSystemVoltage 0x3E
#define C_InputCurrent     0x3F    // Set input current
#define C_MFID             0xFE  // Get Manufacturer ID
#define C_DeviceID         0xFF  // Get device ID
#define ChargeDeviceID     0x08
#define ChargeMFID         0x4D


//
// Input Current Register (0x3F)
//
// BIT[6:0] : Not used. Value Ignored.
// BIT7 : 0 = Add 0mA of input current, 1 = Add 128mA of input current.
// BIT8 : 0 = Add 0mA of input current, 1 = Add 256mA of input current.
// BIT9 : 0 = Add 0mA of input current, 1 = Add 512mA of input current.
// BIT10 : 0 = Add 0mA of input current, 1 = Add 1024mA of input current.
// BIT11 : 0 = Add 0mA of input current, 1 = Add 2048mA of input current.
// BIT12 : 0 = Add 0mA of input current, 1 = Add 4096mA of input current.
// BIT[15:13] : Not used. 1 = invalid write.
//

//
// Charge Current Limit is referenced 'Adapter define' of ThinkPad China Dev Lab power max power control design spec_V1.1_20140328.xslx
//
// Ex: Assumed adapter output is 20V and 45W adapter's charge current limit is 36W, then 36W / 20V = 1.8A
//     which means BIT8+BIT9+BIT10 = 256 + 512 + 1024 = 1792mA should be set.
//
// 45W adp: 41W charge current limit, input current should be 2.05A.
#define A45WINPUTCURRWATT    41
#define A45WINPUTCURRH    0x29 // 2050mA //AC only   //AC + DC S3 S4 S5 mode
#define A45WINPUTCURRL    0x00

// 60W adp: 55W charge current limit, input current should be 2.75A.
#define A60WINPUTCURRWATT    55
#define A60WINPUTCURRH    0x37 // 2750mA //AC only   //AC + DC S3 S4 S5 mode
#define A60WINPUTCURRL    0x00  

// 65W adp: 55W charge current limit, input current should be 2.75A.
#define A65WINPUTCURRWATT    60
#define A65WINPUTCURRH    0x3C // 3000mA //AC only   //AC + DC S3 S4 S5 mode
#define A65WINPUTCURRL    0x00  

// 90W adp: 80W charge current limit, input current should be 4A.
#define A90WINPUTCURRWATT    80
#define A90WINPUTCURRH_A    0x4B // 3750mA //AC only   //AC + DC S3 S4 S5 mode
#define A90WINPUTCURRL      0x00  
#define A90WINPUTCURRH_AD   0x55 // 4250mA //AC + DC S0 or DOS mode



#define CHARGE_CURRENT_1A_H  0x04 // BIT10 = 1024mA
#define CHARGE_CURRENT_1A_L  0x00

#define CHARGE_CURRENT_2_2A_H  0x08 // BIT11 = 2048mA
#define CHARGE_CURRENT_2_2A_L  0x80 // BIT7 = 128mA


extern void ChargerIC_Initial(void);
extern void ChargeIC_InitACDC(void);
extern void WriteSmartChgIC(void);
extern void ReadSmartChgIC(void);
extern void StopChgIC(void);
extern void StartChgIC(void);
extern void Trickle_ChgIC(BYTE OnOff);
extern void Update_ChargeIC(void);   
extern void ChargeIC_Polling_Task(void);
extern void Hook_CHGIC_AdapterIn(void);
extern void Hook_CHGIC_AdapterOut(void);
extern void Hook_CHGIC_BatIn(void);
extern void Hook_CHGIC_BatOut(void);
extern void ChargerTurbo (BYTE OnOff);


#define BAT_LEARN_ON()	{                    \
          SET_MASK(CHGIC_ReadCmd0x12L,BATLearn);    \
          CHGIC_ptr = W_InputCurrent;                \
          WriteSmartChgIC();            \
          }

#define BAT_LEARN_OFF()	{                    \
          CLEAR_MASK(CHGIC_ReadCmd0x12L,BATLearn);    \
          CHGIC_ptr = W_InputCurrent;                \
          WriteSmartChgIC();            \
          }

#define Read_BATLEARN()   IS_MASK_SET(CHGIC_ReadCmd0x12L,BATLearn)


#define   W_chargecurrent  1  
#define   W_chargevoltage  2     
#define   W_InputCurrent   0
#define   W_ChargerMode    3
#define   W_ChargeOption   4



#endif

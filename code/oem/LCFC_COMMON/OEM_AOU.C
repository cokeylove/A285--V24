/*-----------------------------------------------------------------------------
 * TITLE: OEM_AOU.C
 * TITLE: OEM_AOU.H
 *
 * Author : Ivan Chen
 *
 * spec : Always On USB Ver5 Specification V1.2
*---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************
#if Support_AOU5_V1_2
// Always On USB Ver5 Specification Rev1.2 (Draft 2013-08-28).pdf
// Table 6. Condition of default setting (enabled)
// ------------------------------------------------------------
//                        |   S0   |   S3   |   S4   |   S5   |
// -------------+---------+--------+--------+--------+--------|
// AOU port     | AC mode | PwrCDP | PwrDCP | PwrDCP | PwrDCP |
//              | DC mode | PwrCDP | PwrDCP | PwrOff | PwrOff |
// -------------+---------+--------+--------+--------+--------|
// Non-AOU port | AC mode | PwrSDP | PwrSDP | PwrOn  | PwrOn  |
//              | DC mode | PwrSDP | PwrSDP | PwrOff | PwrOff |
// -------------+---------+--------+--------+--------+---------

// Always On USB Ver5 Specification Rev1.2 (Draft 2013-08-28).pdf
// Table 8. Condition of disabled AOU
// ------------------------------------------------------------
//                        |   S0   |   S3   |   S4   |   S5   |
// -------------+---------+--------+--------+--------+--------|
// AOU port     | AC mode | PwrSDP | PwrSDP | PwrOff | PwrOff |
//              | DC mode | PwrSDP | PwrSDP | PwrOff | PwrOff |
// -------------+---------+--------+--------+--------+--------|
// Non-AOU port | AC mode | PwrSDP | PwrSDP | PwrOff | PwrOff |
//              | DC mode | PwrSDP | PwrSDP | PwrOff | PwrOff |
// -------------+---------+--------+--------+--------+---------
static AOU_USB_PORT_POWER_TABLE AouPortPowerTbl = {\
  {    // S0    // S3    // S4_5
    {{  PwrCDP,  PwrDCP,  PwrDCP }},  
    {{  PwrCDP,  PwrDCP,  PwrOff }}, // DcEnMode  (Table 6.)
    {{  PwrSDP,  PwrSDP,  PwrOff }}, // AcDisMode (Table 8.)
    {{  PwrSDP,  PwrSDP,  PwrOff }} // DcDisMode (Table 8.)
  }
};

static AOU_USB_PORT_POWER_TABLE NonAouPortPowerTbl = {
  {    // S0    // S3    // S4_5
    {{  PwrSDP,  PwrSDP,  PwrOn   }}, // AcEnMode  (Table 6.)
    {{  PwrSDP,  PwrSDP,  PwrOff  }}, // DcEnMode  (Table 6.)
    {{  PwrSDP,  PwrSDP,  PwrOff  }}, // AcDisMode (Table 8.)
    {{  PwrSDP,  PwrSDP,  PwrOff  }}, // DcDisMode (Table 8.)
  }
};


// Always On USB Ver5 Specification Rev1.2 (Draft 2013-08-28).pdf
// Table 7. AOU mode and the battery remaining for AOU disabling condition
// ------------------------------------------------------------------------
//                                 | 100%-16% | 15%-11% | 10%-6% | 5%-0%  |
// -------------+------------------+----------+---------+--------+--------|
// AOU port     | S0               | PwrCDP   | PwrCDP  | PwrCDP | PwrCDP |
//              | S3 w/ device     | PwrDCP   | PwrDCP  | PwrSDP | PwrOff |
//              | S3 w/o device    | PwrDCP   | PwrSDP  | PwrSDP | PwrOff |
//              | S4/S5 w/ device  | PwrDCP   | PwrDCP  | PwrOff | PwrOff |
//              | S4/S5 w/o device | PwrDCP   | PwrOff  | PwrOff | PwrOff |
// -------------+------------------+----------+---------+--------+--------|
// Non-AOU port | S0               | PwrSDP   | PwrSDP  | PwrSDP | PwrSDP |
//              | S3 w/ device     | PwrSDP   | PwrSDP  | PwrSDP | PwrOff |
//              | S3 w/o device    | PwrSDP   | PwrSDP  | PwrSDP | PwrOff |
//              | S4/S5 w/ device  | PwrOff   | PwrOff  | PwrOff | PwrOff |
//              | S4/S5 w/o device | PwrOff   | PwrOff  | PwrOff | PwrOff |
// -------------+------------------+----------+---------+--------+---------

static AOU_BATT_PERCENT_TABLE AouBattDcPercentTbl[5] = {
  // AOU port
     // 100%-16%   // 15%-11%    // 10%-6%     // 5%-0%
  {{  PwrCDP      , PwrCDP      , PwrCDP      , PwrCDP  }},  // S0
  {{  PwrDCP      , PwrDCP      , PwrSDP      , PwrOff  }},  // S3 w/ device
  {{  PwrDCP      , PwrSDP      , PwrSDP      , PwrOff  }},  // S3 w/o device
  {{  PwrDCP      , PwrDCP      , PwrOff      , PwrOff  }},  // S4/S5 w/ device
  {{  PwrDCP      , PwrOff      , PwrOff      , PwrOff  }}   // S4/S5 w/o device
};

static AOU_BATT_PERCENT_TABLE  NonAouDcBattPercentTbl[5] = {
  // Non-AOU port
     // 100%-16%   // 15%-11%    // 10%-6%     // 5%-0%
  {{  PwrSDP      , PwrSDP      , PwrSDP      , PwrSDP  }},  // S0
  {{  PwrSDP      , PwrSDP      , PwrSDP      , PwrOff  }},  // S3 w/ device
  {{  PwrSDP      , PwrSDP      , PwrSDP      , PwrOff  }},  // S3 w/o device
  {{  PwrOff      , PwrOff      , PwrOff      , PwrOff  }},  // S4/S5 w/ device
  {{  PwrOff      , PwrOff      , PwrOff      , PwrOff  }}   // S4/S5 w/o device
};

// Truth Table of AOU_CTL1 & AOU_CTL3 (from TPS2546 specification)
// Note that, we assume AOU_CTL2 and ILIM_SEL are dedicated be 1 in actual circuit.
//
// +----------+----------+------------------------------------------------------------------------------------------------+
// | AOU_CTL1 | AOU_CTL3 |  Comment                                                                                       |
// +----------+----------+------------------------------------------------------------------------------------------------+
// |    0     |    0     |  SDP1, keyboard/mouse wake-up (S3)                                                             |
// +----------+----------+------------------------------------------------------------------------------------------------+
// |    0     |    1     |  DCP_Auto, keyboard/mouse wake up, load detection with ILIM_LO + 60 mA thresholds (S3)         |
// +----------+----------+------------------------------------------------------------------------------------------------+
// |    1     |    0     |  SDP1 (S0)                                                                                     |
// +----------+----------+------------------------------------------------------------------------------------------------+
// |    1     |    1     |  CDP, load detection with ILIM_LO + 60mA thresholds or if a BC1.2 primary detection occurs (S0)|
// +----------+----------+------------------------------------------------------------------------------------------------+
//
void AouSdpModeS3 (
) {
  AOU_CTL1_LOW();
  AOU_CTL3_LOW();
}

void AouDcpMode(
) {
  AOU_CTL1_LOW();
  AOU_CTL3_HI();
}

void AouSdpModeS0(
) { 
  AOU_CTL1_HI();
  AOU_CTL3_LOW();
}

void AouCdpMode(
) {
  AOU_CTL1_HI();
  AOU_CTL3_HI();
}

void GetSystemSxType (
  AOU_SX_MODE     *AouSxMode
) {
  switch (SysPowState) {
    case SYSTEM_S0:
      *AouSxMode = S0PwrMode;
      break;

    case SYSTEM_S3:
    case SYSTEM_S0_S3:
 case SYSTEM_S3_S0:
      *AouSxMode = S3PwrMode;
      break;

    case SYSTEM_S4:
    case SYSTEM_S5:
    case SYSTEM_S0_S4:
    case SYSTEM_S0_S5:
 case SYSTEM_S5_S0:
 case SYSTEM_S4_S0:
      *AouSxMode = S4_5PwrMode;
      break;
  }
  
}

AOU_USB_POWER_MODE  DetectAouUsbPwrMode (
  AOU_USB_PORT_MODE UsbPortMode
) {
  AOU_USB_POWER_MODE  UsbPowerMode;
  AOU_STATE           AouState;
  AOU_SX_MODE         AouSxMode;
  GetSystemSxType (&AouSxMode);
  if (SYS_IS_AC_MODE) {
    // System is in AC mode
    if (USB_PWRGET) {
      // AOU AC enabled
      AouState = AcEnMode;
    } else {
      // AOU disabled
      AouState = AcDisMode;
    }
  } else {
    // System is in DC mode
    if ((!USB_PWRGET) || ((USBALWONGet) && (AouSxMode == S4_5PwrMode))) {
      // AOU disabled
      AouState = DcDisMode;      
    } else {
      // AOU AC/DC enabled
      AouState = DcEnMode;
    }
  }
  GetSystemSxType (&AouSxMode);
  if (UsbPortMode == AOU_PORT) {
    UsbPowerMode = AouPortPowerTbl.PwrMode[AouState].Sx[AouSxMode];
  } else {
    UsbPowerMode = NonAouPortPowerTbl.PwrMode[AouState].Sx[AouSxMode];
  }
  return UsbPowerMode;  
}

AOU_USB_POWER_MODE  DetectAouDcUsbPwrMode (
  AOU_USB_PORT_MODE UsbPortMode
) {
  AOU_USB_POWER_MODE  UsbPowerMode;
  BYTE                AouBattIndex;
  BYTE                BattLevel;
  AOU_SX_MODE         AouSxMode;
  AOU_DC_SX_MODE      AouDcSxIndex;

  //
  // A debug flag for engineer to assign battery level directly for testing.
  //
  if (IS_MASK_SET (EC_AOU_DBG_FLAG, AOU_DC_LEVEL_DBG)) {
    BattLevel = (EC_AOU_DBG_FLAG & (~AOU_DC_LEVEL_DBG));
    if (BattLevel > 100) {
      //
      //  Max battery level is up to 100%.
      //
      BattLevel = 100;
    }
  } else {
    //
    //  Real battery level.
    //    
#if Support_TWO_BATT
    if (SecRelativeStateOfChgL > RelativeStateOfChgL)
      {BattLevel = SecRelativeStateOfChgL;}
    else
      {BattLevel = RelativeStateOfChgL;} 
#else  // support one battery  
    BattLevel = RelativeStateOfChgL;
#endif 
  }

  AouBattIndex = 0; // Default 100%-16%

  if ((BattLevel <= 15) && (BattLevel >= 11)) {
    AouBattIndex = 1; // 15%-11%
  } 
  if ((BattLevel <= 10) && (BattLevel >= 5)) {
    AouBattIndex = 2; // 10%-5%
  } 
  if (BattLevel <= 5) {
    AouBattIndex = 3; // 5%-0%
  }

  GetSystemSxType (&AouSxMode);
  if (AouSxMode != S0PwrMode) {
    if (AouSxMode == S3PwrMode) {
      if (AOUPort_DevGET) {
        AouDcSxIndex = S3DcPwrMode;
      } else {
        AouDcSxIndex = S3DcPwrModeNoDev;
      }      
    } else if (AouSxMode == S4_5PwrMode) {
      if (AOUPort_DevGET) {
        AouDcSxIndex = S4_5DcPwrMode;
      } else {
        AouDcSxIndex = S4_5DcPwrModeNoDev;
      }
    }
  } else {
    AouDcSxIndex = S0DcPwrMode;
  };

  if (UsbPortMode == AOU_PORT) {
    UsbPowerMode = AouBattDcPercentTbl[AouDcSxIndex].AouBattLevel[AouBattIndex];
    if ((USBALWONGet) && (AouSxMode == S4_5PwrMode)) {    //disable charge when S4/S5 and DC mode
      UsbPowerMode = PwrOff;
    }
  } else {
    UsbPowerMode = NonAouDcBattPercentTbl[AouDcSxIndex].AouBattLevel[AouBattIndex];
  }

  return UsbPowerMode;  
}

void  SetAouUsbPortMode (
  AOU_USB_PORT_MODE   UsbPortMode,
  AOU_USB_POWER_MODE  UsbPwrMode
) {
//
// Description:
//      Handler for setting USB power mode.
//
// Arguments:
//      UsbPortMode  - AOU_PORT or NON_AOU_PORT.
//      UsbPwrMode   - There is PwrOff, PwrOn, PwrSDP, PwrCDP and PwrDCP for applying.
//
// Return:
//      None.
//
  if (UsbPortMode == AOU_PORT) {
    //
    // AOU port
    //
//    AOU_EN_OFF(); // switch off before changing mode.

    switch (UsbPwrMode) {
      case PwrOff:
        AOU_EN_OFF();
        break;

//      case PwrOn:
      case PwrSDP:
        AouSdpModeS0();
        AOU_EN_ON();
        break;

      case PwrCDP:
        AouCdpMode();
        AOU_EN_ON();
        break;

      case PwrDCP:
        AouDcpMode();
        AOU_EN_ON();
        break;     
    }
  } else {
    //
    // Non-AOU port
    //
    switch (UsbPwrMode) {
      case PwrOff:
        USB_ON_OFF();
        break;

//      case PwrOn:
      case PwrSDP:
      case PwrCDP: // should not exist
      case PwrDCP: // should not exist
        USB_ON_ON();
        break;     
    }
  }
}
#endif


//
// Description:
//      Main function of AOU processes.
//
// Arguments:
//      None.
//
// Return:
//      None.
//
void AOU_Main (
  )
{
#if Support_AOU5_V1_2

  static AOU_USB_POWER_MODE  AouUsbPowerMode, NonAouUsbPowerMode;
  AOU_USB_POWER_MODE  PastAouUsbPowerMode, PastNonAouUsbPowerMode;
  //
  // To detect if device is connected to AOU port.
  //
  if(Read_AOU_DET()) {
    AOUPort_DevCLR;
  	if(((SysPowState==SYSTEM_S0_S3)||(SysPowState==SYSTEM_S3))&&(AOU_IFG_Debounce_Cnt == 0))
  	{
    	AOU_IFG_Debounce_Cnt = 40;
    	AOUPort_DevSET;	//Work-around debounce 4 sec to avoid AOU charger IC need reset 3~4 sec and system ask device stop charging
    }
    else if(AOU_IFG_Debounce_Cnt>0)
    {
    	AOU_IFG_Debounce_Cnt--;
    	AOUPort_DevSET;	//Work-around debounce 4 sec to avoid AOU charger IC need reset 3~4 sec and system ask device stop charging
    }
  } else {
    AOUPort_DevSET;
  }
  //
  // Save past power mode.
  //
  PastAouUsbPowerMode = AouUsbPowerMode;
  PastNonAouUsbPowerMode = NonAouUsbPowerMode;

  //
  // Cover AOU v5 1.2 Table6. and Table8.
  //
  AouUsbPowerMode = DetectAouUsbPwrMode(AOU_PORT);
  EC_AOU_PWR_TYPE = ((BYTE)AouUsbPowerMode) << 4;
  NonAouUsbPowerMode = DetectAouUsbPwrMode(NON_AOU_PORT);
  EC_AOU_PWR_TYPE += (BYTE)NonAouUsbPowerMode;
  //
  // Process the conditions of enabled setting for DC mode in the each battery remaining conditions.
  //
  if ((!SYS_IS_AC_MODE) && ((M_BATdata_ready == 1) || (S_BATdata_ready == 1))) {
    //
    // DC mode && Battery is exist.
    //
    if (USB_PWRGET) {
      // AOU AC/DC enabled
      AouUsbPowerMode = DetectAouDcUsbPwrMode(AOU_PORT);
      EC_AOU_PWR_TYPE = ((BYTE)AouUsbPowerMode) << 4;
      NonAouUsbPowerMode = DetectAouDcUsbPwrMode(NON_AOU_PORT);
      EC_AOU_PWR_TYPE += (BYTE)NonAouUsbPowerMode;
    }
  }

  if (IS_MASK_SET (USB_Charger, b2USB_TmlDis)) {
    //
    // Unexpected power shutdown by thermal issue.
    // (The user needs to take off all power of the system for recovery from the thermal issue)
    //
    AouUsbPowerMode = NonAouUsbPowerMode = PwrOff;
  }
  //
  // Avoid power mode switched too frequently.
  //
  if (AouUsbPowerMode != PastAouUsbPowerMode) {
    SetAouUsbPortMode (AOU_PORT, AouUsbPowerMode);
  }
  if (NonAouUsbPowerMode != PastNonAouUsbPowerMode) {
    SetAouUsbPortMode (NON_AOU_PORT, NonAouUsbPowerMode);
  }

  
#else
  // AOU5 V1.0
    if( SystemIsS3 || SystemIsS5 )
    {
    //
    // System is S3 / S5.
    //
      if (
         (IS_MASK_SET(SYS_STATUS,AC_ADP)
         || ((RelativeStateOfChgL > NO_USBDevice_BAT_LOW) && (!AOUPort_DevGET))
         || ((RelativeStateOfChgL > USBDevice_BAT_LOW) && (AOUPort_DevGET)))
         && IS_MASK_CLEAR(USB_Charger, b2USB_TmlDis)
         )
      {
        if ( ((SystemIsS3) && (USB_PWRGET)) // USB charge for AC in S3 (should be DCP)
        || (USB_PWRGET&IS_MASK_SET(SYS_STATUS,AC_ADP)) // USB charge for AC
        || (USB_PWRGET&!USBALWONGet) // Enable charging when S4/S5
        )
        {
          // DCP mode, CTL1 low, CTL3 high.
          AOU_CTL1_LOW();  // Genesys Auto mode(DCP and Apple 2A).
          AOU_CTL3_HI();  // Clr SDP mode.
          AOU_EN_ON();    // Turn on USB charger port.
          return;
        }
        if ((SystemIsS3) && (!USB_PWRGET)) // AOU feature disabled (should be SDP)
        {
          if(!TurnOnUSBGET)
          {
            AOU_EN_OFF();    // Turn off USB charger port.
          }
          // SDP1(kb/mouse wake), CTL1 low, CTL3 low
          AOU_CTL1_LOW();  // Genesys CDP Mode
          AOU_CTL3_LOW(); // Set SDP mode.
          if(TurnOnUSBGET)
          {
            AOU_EN_ON();    // Turn on USB charger port.
          }
          return;
        }
      }
      AOU_EN_OFF();    // Turn off USB charger port.
      AOU_CTL1_HI();   // Genesys CDP Mode
      AOU_CTL3_LOW();  // Set SDP mode.
    }
    else
    {
    //
    // Not in S3 / S5.
    //
      if(SystemIsS0)
      {
        if(!USB_PWRGET) // AOU feature disabled
        {
          // SDP
          AOU_CTL1_HI();  // Genesys CDP Mode
          AOU_CTL3_LOW(); // Set SDP mode.
          AOU_EN_ON();    // Turn on USB charger port.
        }
        else // AOU feature enabled
        {
          if(Read_AOU_DET())
          {
            AOUPort_DevCLR; // AOU port without device.
          }
          else
          {
            AOUPort_DevSET; // AOU port with device.
          }
          // CDP mode
          AOU_CTL1_HI();  // Genesys CDP Mode
          AOU_CTL3_HI();  // Clr SDP mode.
          AOU_EN_ON();    // Turn on USB charger port.
        }
      }
    }
#endif
}
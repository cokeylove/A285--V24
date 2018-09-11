/*
--------------------------------------------------------------------
  Filename: OEM_AOU.H

  Purpose : Process AOU related for LCFC projects. (Always on USB)

  Author  : Dustin Lee 2014/03/18
--------------------------------------------------------------------
*/

#ifndef OEM_AOU_H
#define OEM_AOU_H
//------------------------------------------------------------------

extern void AOU_Main(void);
#if Support_AOU5_V1_2
typedef enum _AOU_USB_POWER_MODE {
  PwrOff = 0,
  PwrOn,       // PwrOn action in EC is as the same as PwrSDP for Non-AOU port.
  PwrSDP = 1,
  PwrCDP,
  PwrDCP
} AOU_USB_POWER_MODE;

typedef struct _AOU_SX_POWER_TABLE {
  AOU_USB_POWER_MODE  Sx[3];
} AOU_SX_POWER_TABLE;

typedef enum _AOU_SX_MODE {
  S0PwrMode = 0,
  S3PwrMode,
  S4_5PwrMode
} AOU_SX_MODE;

typedef enum _AOU_DC_SX_MODE {
  S0DcPwrMode = 0,
  S3DcPwrMode,
  S3DcPwrModeNoDev,
  S4_5DcPwrMode,
  S4_5DcPwrModeNoDev
} AOU_DC_SX_MODE;

typedef struct _AOU_USB_PORT_POWER_TABLE {
  AOU_SX_POWER_TABLE   PwrMode[4];
} AOU_USB_PORT_POWER_TABLE;

typedef enum _AOU_STATE {
  AcEnMode = 0,
  DcEnMode,
  AcDisMode,
  DcDisMode,
} AOU_STATE;

typedef enum _AOU_USB_PORT_MODE {
  AOU_PORT = 0,
  NON_AOU_PORT
} AOU_USB_PORT_MODE;

void AouSdpModeS3 ();
void AouDcpMode();
void AouSdpModeS0();
void AouCdpMode();

void GetSystemSxType (AOU_SX_MODE *AouSxMode);
AOU_USB_POWER_MODE  DetectAouUsbPwrMode (AOU_USB_PORT_MODE UsbPortMode);
AOU_USB_POWER_MODE  DetectAouDcUsbPwrMode (AOU_USB_PORT_MODE UsbPortMode);
void  SetAouUsbPortMode (AOU_USB_PORT_MODE UsbPortMode, AOU_USB_POWER_MODE UsbPwrMode);

#define SYS_IS_AC_MODE  (!(IS_MASK_CLEAR(SYS_STATUS,AC_ADP) || Read_ACOFF()))

// Default: // 1. AOU enabled in BIOS or AP in AC mode.
//          // 2. AOU DC enabled in BIOS in DC mode.
// 
// Disabled: // 1. AOU disabled in BIOS or AP.
//           // 2. System goes to S4/S5 under DC mode. (AOU DC disabled)
//           // 3. System automatically transition to S4 in 30 days standby mode.
//           // 4. Battery < 15% w/o device on AOU port in S3/S4/S5.
//           // 5. Battery < 10% w/ device on AOU port in S3/S4/S5.
//           // 6. Battery < 5%.
//           // 7. Thermal shutdown.

typedef unsigned char AOU_BATT_LEVEL;

typedef struct _AOU_BATT_PERCENT_TABLE {
  AOU_BATT_LEVEL  AouBattLevel[4];
} AOU_BATT_PERCENT_TABLE;

typedef struct _AOU_DC_USB_PORT_POWER_TABLE {
  AOU_BATT_PERCENT_TABLE *AouBattPercentTable;
  unsigned char          NumOfBattMode;
} AOU_DC_USB_PORT_POWER_TABLE;

typedef enum _AOU_SX_DEVICE_MODE {
  S0_PWR = 0,
  S3_WITH_DEVICE,
  S3_WO_DEVICE,
  S4_5_WITH_DEVICE,
  S4_5_WO_DEVICE
} AOU_SX_DEVICE_MODE;
#endif 

#define USBDevice_BAT_LOW    10  // The battery of the PC becomes very low (10% or under) with a device on AOU port
#define NO_USBDevice_BAT_LOW  15  // The battery of the PC becomes low (15% or under) with no device on AOU port

#endif

/*-----------------------------------------------------------------------------
 * TITLE: OEM_SHUTDOWN_ID.H
 *
 * Author : Ivan.Chen
 *
 * Note : Define SHUTDOWN ID.
 *---------------------------------------------------------------------------*/
 
#ifndef OEM_SHUTDOWN_ID_H
#define OEM_SHUTDOWN_ID_H

#define REV0_ID               0x00  // Reserved
#define S5_ID                 0x01  // Shutdown ID for SB SLP_S5 signal
//#define OVERTEMP_ID           0x02  // Shutdown ID for T-diode over temp
//#define BATTLEARN_ID          0x03  // Shutdown ID for battery learning
#define COMMAND_ID            0x04  // Shutdown ID for command C:0x59 D:0xA2
#define FLASH_ID              0x05  // Shutdown ID for flash
#define COLDBOOT_ID           0x06  // Shutdown ID for EC power reset
#define WDT_WARMBOOT_ID       0x07  // Shutdown ID for EC WDT reset
#define BOOTFAIL_ID           0x08  // Shutdown ID for boot fail restart by command C:0x40 D:0xXX
//#define FANFAIL_ID            0x09  // Shutdown ID for fan fail
#define BATTOVERTEMP_ID       0x0A  // Shutdown ID for battery over temp
#define THERMALCOMMFAIL_ID    0x0B  // Shutdown ID for thermal sensor communication fail
#define BATTEMPTY_ID          0x0C  // Shutdown ID for battery empty
//#define CPUHOT_ID             0x0D  // Shutdown ID for CPU hot
#define BATTCOMMFAIL_ID       0x0E  // Shutdown ID for battery communication fail
#define BATTABNORMAL_ID       0x0F  // Shutdown ID for battery abnormal device ID
//#define ESCD_ID               0x10  // Shutdown ID for ESCD--When receive command 0x59 0xF1 and power button is pressed
#define DTSOVERTEMP_ID        0x11  // Shutdown ID for DTS over temp
#define VGAOVERTEMP_ID        0x12  // Shutdown ID for VGA thermal sensor over temp
#define BATTLOWVOLT_ID        0x13  // Shutdown ID for battery low voltage
//#define UNKNOWN_WARMBOOT_ID   0x87  // Shutdown ID for EC code reset and it is not caused by WDT
#define EXTVGAOVERTEMP_ID     0x1C  // Shutdown ID for External VGA thermal sensor over temp
#define EXTCPUOVERTEMP_ID     0x1D  // Customer shutdown ID 1//  //A285D00008 Add fan table
//#define CUSTOMER2_ID          0x1E  // Customer shutdown ID 2
//#define S35_ID                0x1F  // Shutdown ID for SB SLP_3.5
#define VGACOMMFAIL_ID        0x20  // Shutdown ID VGA thermal sensor communication fail
#define EXTVGACOMMFAIL_ID     0x21  // Shutdown ID External VGA thermal sensor communication fail
#define CPUCOMMFAIL_ID        0x22  // Shutdown ID CPU thermal sensor communication fail
#define BATTOVERCURR_ID       0x23  // Shutdown ID for battery over current
//[CC20131118][Project][Start] Add shutdown function for hang on power sequence.
#define POWERSEQFAIL_ID       0x24  // Shutdown ID for power seqence fail
//[CC20131118][Project][End] Add shutdown function for hang on power sequence.
#define AC_CUTOFF_ID          0x25  // Shutdown ID for AC cut off
#define AC_WRONG_ADP_ID       0x26  // Shutdown ID for wrong adapter
#define PWR_OTP_ID            0x27  // Shutdown ID for power OTP(Over Temperature Protection)
#define BAT_ERROR_ID          0x28  // Shutdown ID for remain capacity report 0 to BIOS when battery error
#define BAT_SMBusFail_ID      0x29  // Shutdown ID for remain capacity report 0 to BIOS when battery smbus fail
#define BIOS_WDT_ID           0x2A  // Shutdown ID for BIOS WDT(For BIOS workaround)
#define BIOS_ID               0xB0  // Shutdown ID for BIOS shutdown
#endif
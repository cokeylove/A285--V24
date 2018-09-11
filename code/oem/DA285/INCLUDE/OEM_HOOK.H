/*-----------------------------------------------------------------------------
 * TITLE: OEM_HOOK.H
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

#ifndef  OEM_HOOK_H
#define OEM_HOOK_H

//=============================================================
//EC ram define for project
//0x6A0 ~ 0x6FF reserved for project usage
#define Batt_Eject_return_Cnt				SD_uchar_8(OEMRAM6+0xB0)
#define Attach2status				SD_uchar_8(OEMRAM6+0xB1)
  #define OcneSwitchPowerSource              BIT6
  #define StableLLevel              BIT7   // 1:high   0:low ,The Attach2 pin status when Attach2 stable.
#define Shutdown_check				SD_uchar_8(OEMRAM6+0xB2)
  #define PB4s_shutdown              BIT7
#define Wrong_ADP_check_Timeout_Clear				SD_uchar_8(OEMRAM6+0xB3)	//bit0~bit3:Timer counter; bit4~bit7:last Wrong adaptor counte


#if Support_KBD_BL_LED
#define KBL_Interval_Cnt				SD_uchar_8(OEMRAM6+0xB4)
#define KBL_Interval_Cnt_Temp				SD_uchar_8(OEMRAM6+0xB5)
#define KBL_Current_Level_Duty				SD_uchar_8(OEMRAM6+0xB6)
#define KBL_Target_Level_Duty				SD_uchar_8(OEMRAM6+0xB7)
#define KBL_Status						SD_uchar_8(OEMRAM6+0xB8)  //A285D00035 follow A485D00091
#define KBL_Blink_Level_Duty				SD_uchar_8(OEMRAM6+0xF4)
#define KBL_Blink_Level_State				SD_uchar_8(OEMRAM6+0xF5)
#define KBL_HotKey_Blink_Times			SD_uchar_8(OEMRAM6+0xF6)
#define KBL_Status_Time_Count				SD_uchar_8(OEMRAM6+0xF7)	//0-125ms/5=0---21
#define KBL_HotKey_Blink_Achive			SD_uchar_8(OEMRAM6+0xF8)
#define KBL_Hotkey_Blink_Time_Interval        SD_uint_16(OEMRAM6+0xF9) //A285D00043:fix backlight led blink breath as lenovo spec
#define KBL_Hotkey_Blink_Duty                       SD_uchar_8(OEMRAM6+0xFB)


typedef enum
{
	KBL_BLINK_STATUS_IDLE,

	KBL_BLINK_STATUS_UP,
	KBL_BLINK_STATUS_KEEP,
	KBL_BLINK_STATUS_DOWN,
	
	KBL_BLINK_STATUS_MAX
}eBlickStatus;


#define KBL_BLICK_TIMES_HOTKEY_COOL			(2)
#define KBL_BLICK_TIMES_HOTKEY_PERFORMANCE	(4)

 #define  TargetChange    						BIT0
 #define  Frone    						BIT1	//1:up,0:down
 #define  PowerOnCheck					BIT2	//1:Check mode,0:Normal mode
 #define  NeedUpdateAcpiKbdBlStatus					BIT3

 #define  HotKeyKbdBlBlick					BIT4
 #define  HotkeyLED_NONEED_update               BIT5
#endif //Support keyboard backlight

#define GSensorSelfTestState		 SD_uchar_8(OEMRAM6+0xF3)
  #define DataReady					     BIT6   // 1:Ready   0:done
  #define StartTest              BIT7   // 1:start   0:done
#define GSensorSelfTestCountX		        SD_uchar_8(OEMRAM6+0xBA)  // wait 50 ms for BMA255 count
#define GSensorTmpX_1		        SD_uchar_8(OEMRAM6+0xBB)
#define GSensorTmpX_2		        SD_uchar_8(OEMRAM6+0xBC)
#define GSensorTmpY_1		        SD_uchar_8(OEMRAM6+0xBD)
#define GSensorTmpY_2		        SD_uchar_8(OEMRAM6+0xBE)
#define GsensorStableDelay      SD_uchar_8(OEMRAM6+0xBF)
#define Test109				SD_uchar_8(OEMRAM6+0xF1)
#define SpecialCodeState				SD_uchar_8(OEMRAM6+0xF2)
  #define SpecialCodeActive BIT0  // 1:active 0:clear

#define UCSI_50CMD_WA				SD_uchar_8(OEMRAM6+0xFF)      //this is BIOS check flag for system hang logo when enable UCSI
//=============================================================

//@LCFC_CUSTOMIZE+
#define OEM_BIOS_LED_STATUS SD_uchar_8(OEMRAM6+0xB9)
 #define CapsLockOn BIT0  // 1:Capslock On, 0: Capslock off
//@LCFC_CUSTOMIZE-

#define OEM_DASH_DEBUG_B SD_uchar_8(OEMRAM6+0xC1)
#define OEM_DASH_DEBUG_C SD_uchar_8(OEMRAM6+0xC2)
#endif  //OEM_HOOK_H
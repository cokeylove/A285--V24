/*-----------------------------------------------------------------------------
 * TITLE: OEM_QEVENT.H
 *
 * Author : Ivan.Chen
 *
 * Note : Define Qevent number.
 *---------------------------------------------------------------------------*/
 
#ifndef OEM_QEVENT_H
#define OEM_QEVENT_H

//0x00 No Event Ignore this attention
//0x01-0x0F Reserved for ACPI
#define H1                    0x10  // Hotkey 1
#define H2_Screen_Lock        0x11  // Hotkey 2 Screen Lock
#define H3_Power_Manager      0x12  // Hotkey 3 Power Manager
#define H4_Suspend            0x13  // Hotkey 4 Suspend
#define LCD_BRIG_INC_EVENT    0x14  // Hotkey 5 Brightness up
#define LCD_BRIG_DEC_EVENT    0x15  // Hotkey 6 Brightness down
#define PRESENT_DIR_EVENT     0x16  // Hotkey 7 Presentation Display Mode
#define H8_Touch_Pad_EVENT    0x17  // Hotkey 8 ThinkPad Touch Pad
#define H9                    0x18  // Hotkey 9
#define H10_TVButton          0x19  // Hotkey 10 Think Vantage button
#define H11                   0x1A  // Hotkey 11
#define H12_Hibernation       0x1B  // Hotkey 12 Hibernation
#define H13                   0x1C  // Hotkey 13
#define H14                   0x1D  // Hotkey 14
#define H15                   0x1E  // Hotkey 15
#define KB_LIGHT_EVENT_1F     0x1F  // Hotkey 16 Keyboard Backlight/ Ho-taru

//0x20-0x21 Reserved
#define LLB_22                0x22  //Critical Low battery Trigger to suspend/hibernation
#define BAT_Empty             0x23  //Battery empty become all attached batteries empty, so the system is required to shutdown not to loose user data
#define BAT0_Cap_event        0x24  //Trip point capacity event for Battery 0
#define BAT1_Cap_event        0x25  //Trip point capacity event for Battery 1
#define ACPI_ACIN_SCI         0x26  // AC adapter attached
#define ACPI_ACOUT_SCI        0x27  // AC adapter detached

#define SDV_VPC_notify        0x28  //0x44  // Conflict, Defined as ODD eject button, Change to No_Use_event to avoid wrong behavior

#define PRW_OFF_SW_Pressed    0x28  //Power off switch pressed//Power off
#define PRW_OFF_SW_Released   0x29  //Power off switch released//Power off
#define EV_LID_Open           0x2A  //LID open
#define EV_LID_Close          0x2B  //LID close suspend trigger
#define Ultra_bay_Unlock      0x2C  //Ultra-bay Unlock suspend trigger
#define Ultra_bay_Lock        0x2D  //Ultra-bay Lock
//0x2E-0x2F Reserved

#define KP_Password           0x30  //Key pressed for password
#define Password_MH           0x31  //Password match
#define Password_MMH          0x32  //Password mismatch
#define KBD_AUX_ACT           0x33  //KBD/AUX activity
#define Timer_Event           0x34  //Event Timer
#define AM_SMB_EVENT          0x35  //SMBus Command completion, Error
#define PWD_EVENT             0x36  //PWD Command completion
#define DOCK_EVENT            0x37  //Dock status
#define FnKey_Pressed         0x39  //Fn Key pressed
//0x3A-0x3B Reserved
#define RING_EVENT            0x3C  //
#define FAN_BROKEN            0x3D  //
#define PME_3E                0x3E  // PME is active now
#define KB_NUMLOCK_3F         0x3F  // Numlock state changed; NumLock state of the embedded keyborad is changed.

#define Thermal_Event         0x40  // Temperature status (0x20)is changed or the temperatre is getting high or low for the value of the delta temperature in "Thermal control table".
#define Wireless_En_Switch    0x41  // Global wireless communication switch status changed
//0x42 Reserved
#define MUTE_EVENT_43         0x43  // Mute state change  // Mute(Fn+F1)
#define BAY_Media_Eject       0x44  // the eject button of a optical drive in the bay is pressed
#define Dock_attach_detach    0x45  // A docking station is attached or detached
#define BAT_FET_Hot           0x46  // FET control current from the battery become hot
#define BAT_FET_Normal        0x47  // FET control current from the battery become normal temperature
#define Assert_PowerSupply    0x48  // Power consumption is over the capability of the power supply
#define Deassert_PowerSupply  0x49  // FET control current from the battery become normal temperature
#define ACPI_BAT0IN_SCI       0x4A  //Battery 0 Attach/ Detach
#define ACPI_BAT0CHG_SCI      0x4B  //Battery 0 State Change
#define ACPI_BAT1IN_SCI       0x4C  //Battery 1 Attach/ Detach
#define ACPI_BAT1CHG_SCI      0x4D  //Battery 1 State Change
#define AM_UCSI_EVENT         0x4F  // UCSI event
//0x4E-0x4F Reserved

//0x50-0x52 System Unique Events
#define ODD_BTN_EVENT         0x53  // not using // For ODD eject Button event
//0x54-0x5F System Unique Events

#define H17                   0x60  //Hotkey 17
#define H18                   0x61  //Hotkey 18
#define H19                   0x62  //Hotkey 19
#define H20                   0x63  //Hotkey 20 Magnifier

#define WIRELESS_RADIO_EVENT  0x64  //Hotkey 21 Access connection// Wireless Radio(Fn+F8)
#define CAMERA_MUTE_EVENT     0x65  //Hotkey 22 // CameraMute(Fn+F5)
#define OSSETTING_EVENT       0x66  //Hotkey 23 Settings// OS Setting(Fn+F9)
#define OSSEARCH_EVENT        0x67  //Hotkey 24  Search// OS Search(Fn+F10)
#define OSVEAROPENAPP_EVENT   0x68  //Hotkey 25  View Open Apps// OS View All Open APP(Fn+F11)
#define OSVEARALLAPP_EVENT    0x69  //Hotkey 26  View all Apps// OS View All APP(Fn+F12)
#define MIC_MUTE_EVENT        0x6A  //Hotkey 27  Mic mute button// MicMute(Fn+F4)
//6Bh-6Fh Hotkey 28-33

#define FN_PRESS_EVENT_70     0x70  //Fn Key Sticked // Fn press
//0x71 Reserved
#define STICKY_ON_EVENT_72    0x72  //Fn Key Locked // Sticky Key on
#define STICKY_OFF_EVENT_73   0x73  //Fn Key Unlocked // Sticky Key off
#define FN4SEC_EVENT_74       0x74  //Fn+Esc lock state changed // Fn + sec press
//0x75 Reserved
#define OSD_Event             0x76  //OSD event //Issued by System BIOS
#define RTC_Alarm             0x77  //Real Time Clock Alarm //This will be issued only in S3.

#define GEN_CH_CV_EVENT_78    0x78  //0xFF  // Generate CH and CV
#define CPU_SLOW_AD           0x7B  //0x1D  // EC set throttle function and P-state
#define GSENSOR_PWRON_7D      0x7D  //0x7D  // Gravity Sensor Power On
#define GSENSOR_PWROFF_7E     0x7E  //0x7E  // Gravity Sensor Power Off

#define APU_ENTURBO		        0x84  //0x84 // APU Enable Turbo
#define APU_DISTURBO          0x85  //0x85 // APU Disable Turbo 

//A285D00052: add stp(skin temperature protection) function for thermal
#define Thermal_STP_Enable		0x86		// thermal temperature >= 52 qevent
#define Thermal_STP_Disable		0x87		//thermal temperature <= 48 qevent

#define GPU_pwrlmt_ON         0x88  //0x88 // GPU power limit ON

#define ATI_DGPU_SMBUS_90     0x90  //0x90  // ATI DGPU SMbus status by BIOS send status
#define CPUTemp_FromBIOS			 0x91  //0x91	// Work-around for Intel issue "Some time PECI will hangup".Update from BIOS.
//0x92-0xFF Reserved

//for CS16 keyboard 
#define HOTKEY_PrtSc         0x93
#define HOTKEY_calculator    0x94
#define CAPSLOCK_LED_ON 	   0x95	//Notify BIOS to turn on Capslock led
#define CAPSLOCK_LED_OFF  	 0x96	//Notify BIOS to turn off Caps led

#endif

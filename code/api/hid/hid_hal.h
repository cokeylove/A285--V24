#ifndef __HID_HAL_H__
#define __HID_HAL_H__
//********************************************************************************
//
//  Macro Define
//
//********************************************************************************
#define HID_INTERFACE_STEP_INIT         0x00
#define HID_INTERFACE_STEP_SERVICE      0x01

//********************************************************************************
//
//  External Function
//
//********************************************************************************
extern void hid_main(void);
extern void hid_isr(void);
extern void hid_gpio_cfg(void);
extern void hid_gpio_reset(void);
extern void hid_assert_interrupt(void);
extern void hid_deassert_interrupt(void);
#ifndef EC_FEATURE_ONLY 
extern void hid_time_check_report_interval(void);
#endif
extern int  hid_time_report_interval(void);
extern void hid_time_sensor_interval(void);
extern void hid_upgrade_flow(void);
extern void hid_extend_data_get(UINT8 pBuffer[13]);
extern void hid_extend_data_set(UINT8 pBuffer[13]);
extern void hid_init_os_hot_switch(void);
extern void hid_check_os_hot_switch(void);
//extern void pure_ec_isr_hid_service(void);

//********************************************************************************
//
//  Extern Value
//
//********************************************************************************
extern volatile unsigned short gusSensorOneShotCmd;
extern volatile UINT8 gucSensorStatusBusy;
extern const unsigned short aru16ALR_curve[];
extern const unsigned short CustomModel[];

//********************************************************************************
#endif // __HID_COMMON_H__
#ifndef _HID_CUSTOM_CMD_H_ //{
#define _HID_CUSTOM_CMD_H_

#define HID_CUSTOM_REPORT_ID    0x5A

extern int hcc_do_custom_cmd(UINT8 *pBuf);
extern int hcc_get_custom_data(UINT8 *pBuf);
extern void hcc_service(void);

#endif //}_HID_CUSTOM_CMD_H_


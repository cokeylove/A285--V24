#ifndef I2C_HID_H_  // { I2C_HID_H_
#define I2C_HID_H_  // I2C_HID_H_
/* ****************************************************************************
 * i2c.h
 *
 *  Created on: 2012/8/15
 *      Author: ite00595
 * ****************************************************************************/
#include "chip_type.h"
#include "hid_common.h"
#include "i2c.h"

//****************************************************************************
//  Macro define
//****************************************************************************
#define I2C_HID_REPORT_ID_NUM   TOTAL_HID_DEVICE

#define	_VendorID_LSB			((UINT8)(ITE_VENDOR_ID      )) //0x61
#define	_VendorID_MSB			((UINT8)(ITE_VENDOR_ID  >> 8)) //0x85
#define	_ProductID_LSB			((UINT8)(ITE_PRODUCT_ID     )) //0x32
#define	_ProductID_MSB			((UINT8)(ITE_PRODUCT_ID >> 8)) //0x00
#define	_VersionID_LSB			((UINT8)(ITE_FW_VERSION     )) //0x01
#define	_VersionID_MSB			((UINT8)(ITE_FW_VERSION >> 8)) //0x00


//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************
typedef enum i2c_reg_idx
{
	I2C_REG_IDX_RESERVED,
	I2C_REG_IDX_HID_DESC,
	I2C_REG_IDX_REPORT_DESC,
	I2C_REG_IDX_REPORT_INPUT,
	I2C_REG_IDX_REPORT_OUTPUT,
	I2C_REG_IDX_CMD,
	I2C_REG_IDX_DATA,
	I2C_REG_COUNT,
}I2C_REG_IDX;

typedef enum i2c_op_code
{
	I2C_OP_CODE_RESERVED,
	I2C_OP_CODE_RESET,
	I2C_OP_CODE_GET_REPORT,
	I2C_OP_CODE_SET_REPORT,
	I2C_OP_CODE_GET_IDLE,
	I2C_OP_CODE_SET_IDLE,
	I2C_OP_CODE_GET_PROTOCOL,
	I2C_OP_CODE_SET_PROTOCOL,
	I2C_OP_CODE_SET_POWER,
}I2C_OP_CODE;

typedef enum i2c_report_type
{
	I2C_REPORT_TYPE_NULL,
	I2C_REPORT_TYPE_INPUT,
	I2C_REPORT_TYPE_OUTPUT,
	I2C_REPORT_TYPE_FEATURE,
	I2C_REPORT_TYPE_COUNT,
}I2C_REPORT_TYPE;


//****************************************************************************
//  External Function
//****************************************************************************
extern void i2c_hid_isr(void);
extern void i2c_hid_init(void);
extern void i2c_hid_main(void);

extern void i2c_hid_sensor_init(void);
extern void i2c_hid_data_set(UINT8 *pBuf, void *pParam);
extern UINT8 i2c_check_interrupt(UINT8 ucCH);
extern void  i2c_hid_check_and_assert_interrupt(void);
extern void  i2c_hid_reset_hid_buffer(UINT8* pBuf);
#ifdef __ENABLE_I2C_PIO_MODE__ 
extern void i2c_hid_pio_slave(UINT8 ucMode);
extern void i2c_hid_pio_init(UINT8 ucSlave_Addr);
#else
extern void i2c_hid_parsing_cmd(UINT8 ucMode);
extern void i2c_set_hid_slave(UINT8 ucSlave_Addr);
#endif

extern void i2c_hid_power_on(void);
extern void i2c_hid_power_off(void);

#define HID_INT_EVENT_BUFFER_SIZE   14
extern void HID_INT_Event(BYTE device_number);
extern BYTE Get_HID_INT_Event(void);
extern void HID_INT_Event_Reset(void);

//****************************************************************************
#endif  // } I2C_HID_H_

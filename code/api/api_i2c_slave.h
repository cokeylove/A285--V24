/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_i2c_slave.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef _API_I2C_SLAVE_H_
#define _API_I2C_SLAVE_H_

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define _I2C_Slave_A                0
#define _I2C_Slave_B                1

#define _I2C_Match_Slave_Address1   0
#define _I2C_Match_Slave_Address2   1

#define _I2C_Slave_at_Channel_A     0x00
#define _I2C_Slave_at_Channel_B     0x01
#define _I2C_Slave_at_Channel_C     0x02
//#define _I2C_Slave_at_Channel_D   0x03
#define _I2C_Slave_at_Channel_NULL  0xFF

/* 
 * ****************************************************************************
 * global variables for I2C slave api
 * ****************************************************************************
 */
extern DWORD g_i2c_slave_a_byte_write_index;
extern DWORD g_i2c_slave_b_byte_write_index;
extern DWORD g_i2c_slave_a_byte_read_index;
extern DWORD g_i2c_slave_b_byte_read_index;

extern BYTE *g_i2c_slave_a_byte_write_buffer_pointer;
extern BYTE *g_i2c_slave_b_byte_write_buffer_pointer;

extern BYTE *g_i2c_slave_a_byte_read_buffer_pointer;
extern BYTE *g_i2c_slave_b_byte_read_buffer_pointer;

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void I2C_Slave_A_Variables_Reset(void);
extern void I2C_Slave_B_Variables_Reset(void);
extern void I2C_Slave_A_OEM_Init(
    BYTE *p_byte_write_buffer,
    BYTE *p_byte_read_buffer,
    FUNCT_PTR_V_V p_stop_condition_func,
    FUNCT_PTR_V_V p_stop_condition_func_addr2,
    FUNCT_PTR_V_V p_timeout_func);
extern void I2C_Slave_B_OEM_Init(BYTE *p_byte_write_buffer,
    BYTE *p_byte_read_buffer,
    FUNCT_PTR_V_V p_stop_condition_func,
    FUNCT_PTR_V_V p_stop_condition_func_addr2,
    FUNCT_PTR_V_V p_timeout_func);
extern void I2C_Slave_Initialization(
    BYTE p_slave_a_at_channel,
    BYTE p_slave_b_at_channel,
    BYTE p_slave_a_addr,
    BYTE p_slave_a_addr2,
    BYTE p_slave_b_addr,
    BYTE p_slave_b_addr2);
extern void I2C_Slave_Interface_Select(
    BYTE p_slave_module, BYTE p_local_channel);
extern void I2C_Slave_A_Enable(BYTE p_slave_address_1, BYTE p_slave_address_2);
extern void I2C_Slave_B_Enable(BYTE p_slave_address_1, BYTE p_slave_address_2);
extern void I2C_Slave_A_ISR(void);
extern void I2C_Slave_B_ISR(void);

/* 
 * ****************************************************************************
 * structure
 * ****************************************************************************
 */
typedef struct I2CSlaveConfig
{
	uchar_8	*SMBusPin0	;   /* clock pin */
	uchar_8	*SMBusPin1	;   /* data pin */
} sI2CSlaveConfig;

#endif


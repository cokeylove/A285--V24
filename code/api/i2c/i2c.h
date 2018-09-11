/*-----------------------------------------------------------------------------
* TITLE: i2c_pio.H
*
* Author : Clark
*
* History : 
* ------------------------------
* Version : 1.00
* ------------------------------
*
* Note : 
*
*---------------------------------------------------------------------------*/
 
#ifndef __I2C__H
#define __I2C__H


#include "i2c_module.h"


//*****************************************************************************
//							define
//*****************************************************************************
//Test
#define I2C_TEST									0
#define I2C_SWAP_TEST								0
#define  __ENABLE_HID_I2C_TIMEOUT_RESET__			0

//Set 
#define I2C_SET_WAIT_TIME(u8CH, u8Dev, wait_time)	do{ I2C_WCSR(u8CH, u8Dev) = wait_time; }while(0)
#define I2C_GET_WAIT_TIME(u8CH, u8Dev)				I2C_WCSR(u8CH, u8Dev)
#define I2C_SET_CLOCK_PERIOD(u8CH, u8ScaleTime)		do{ I2C_CSR(u8CH) = u8ScaleTime; }while(0)

/* get length */
#define I2C_GET_RX_LENGTH(ucCH)						(( (I2C_SLV_NUM_H(ucCH) << 8) + I2C_SLV_NUM_L(ucCH) + 1) & 0x7F)

//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************


//*****************************************************************************
//							Public variable
//*****************************************************************************
extern const UINT16 u8I2cBase[I2C_CH_CNT];


//*****************************************************************************
//							Public function
//*****************************************************************************
void i2c_init(void);

void i2c_ch_init_gpio(UINT8 u8CH, BOOL bIsSwap);
extern void i2c_ch_reset(UINT8 u8CH);
void i2c_slv_ch_reset(UINT8 u8CH, UINT8 u8SlaveId, UINT16 u16SramAddr);
void i2c_swap_channel(UINT8 ucChA, UINT8 ucChB);
extern BOOL i2c_trans(BYTE ucCH, IO_TRANS* p_stTrans,BOOL pecSupport,BYTE pecValue);
void i2c_stop_trans(BYTE ucCH, BYTE ucDev);
BOOL i2c_check_status(UINT8 ucCH, UINT8 ucDev, IO_TRANS* p_stTrans);
extern void i2c_set_trans_struc(IO_TRANS* p_stTrans, BYTE* p_WtBuf, DWORD dwWtCnt, 
						 BYTE* p_WtDataBuf, DWORD dwWtDataCnt, BYTE* p_RdBuf, DWORD dwRdCnt);

//enable
void i2c_enable_sleep_mode(BYTE ucCH, BOOL bIsEnable);

//for CMDQ
void i2c_fire(BYTE ucCH, UINT8 u8Dev);

//setting
BOOL i2c_set_cmd_q_buf(BYTE ucCH, BYTE* p_WtBuf, DWORD dwWtCnt, BYTE* p_RdBuf, DWORD dwRdCnt);
BOOL i2c_slave_set_buf(BYTE ucCH, DIRECTION enumDirection, BYTE* p_Buf);
/* interrupt */
void i2c_interrupt(void);
I2C_CNT i2c_get_cnt(void);

//wait time for CMD Queue. One wait time is equal one scale. One scale is equal how much clock.
void i2c_set_cmd_q_wait_time(BYTE ucCH, BYTE ucDev, BYTE ucWaitTime);
BYTE i2c_get_cmd_q_wait_time(BYTE ucCH, BYTE ucDev);
void i2c_set_cmd_q_scale_time(BYTE ucCH, BYTE ucScaleTime);
BYTE i2c_get_cmd_q_scale_time(BYTE ucCH);


#endif /* I2C_H_ */

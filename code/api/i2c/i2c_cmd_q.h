/*-----------------------------------------------------------------------------
* TITLE: i2c_cmdq.h
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
 
#ifndef __I2C_CMD_Q__H
#define __I2C_CMD_Q__H


#include "i2c_module.h"


//*****************************************************************************
//							define
//*****************************************************************************



//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************
//typedef struct i2c_cmd_q_header
//{
//	UINT32 u8Id:8;
//	UINT32 u8Cmd:5;
//	UINT32 u16DataLen:11;
//	UINT32 u8Reserved:8;
//}I2C_CMD_Q_HEADER;



//*****************************************************************************
//							Public variable
//*****************************************************************************


//*****************************************************************************
//							Public function
//*****************************************************************************
void i2c_cmd_q_init(void);
BOOL i2c_cmd_q_trans(UINT8 ucCH, IO_TRANS* p_stTrans);
void i2c_cmd_q_kick(UINT8 ucCH, UINT8 ucDev);
void i2c_cmd_q_stop_trans(UINT8 ucCH, UINT8 ucDev);
BOOL i2c_cmd_q_set_buf(UINT8 ucCH, UINT8* p_WtBuf, UINT32 dwWtCnt, UINT8* p_RdBuf, DWORD dwRdCnt);
BOOL i2c_cmd_q_set_reg(UINT8 ucCH, UINT8 ucDev, UINT32 dwWtAddr, UINT32 dwRdAddr, UINT32 dwRdBufSize, IO_TRANS_TYPE enumTransType);
#endif
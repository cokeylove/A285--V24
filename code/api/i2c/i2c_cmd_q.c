/*-----------------------------------------------------------------------------
* TITLE: si2c_cmd_q.c
*
* Author : Clark
*
* History : 
* ------------------------------
* Version : 1.10
* ------------------------------
*
* Note :
*
*---------------------------------------------------------------------------*/
//#include "chip_type.h"
//#include "rom.h"
//#include "i2c_cmd_q.h"
//#include "debug_print.h"
#include "..\include.h"


//*****************************************************************************
//							declaration of Define and Marco
//*****************************************************************************

/***********CMD¡@queue***************/
#define I2C_CMD_Q_IS_AUTO(ucCH)			(IS_MASK_SET((I2C_MODE_SEL(ucCH) >> (ucDev*2)), 0x3) == 0x3 ? TRUE:FALSE)
#define I2C_CMD_Q_IS_RUN(ucCH, ucDev)	(IS_MASK_SET(I2C_FST(ucCH), BIT(ucDev)) > 0 ? FALSE:TRUE)
#define I2C_CMD_Q_IS_ENABLE_INT(ucCH)	(IS_MASK_SET(I2C_CONTROL(ucCH), I2C_CRTL_INT_EN) > 0 ? TRUE:FALSE)
#define I2C_CMD_Q_IS_INT_PENDING(ucCH)	(IS_MASK_SET(I2C_STAT(ucCH), I2C_STAT_INT_PENDING) > 0 ? TRUE:FALSE)

#define I2C_CMD_Q_FIRE(ucCH, ucDev)		(I2C_CTRL2(ucCH) = BIT(ucDev))
#define I2C_CMD_Q_SET_OPTION(ucCH)		(I2C_CONTROL(ucCH) = I2C_CRTL_START | I2C_CRTL_MASTER_MODE | I2C_CRTL_ACK | I2C_CRTL_INT_EN)
//#define I2C_CMD_Q_SET_OPTION(ucCH)	(I2C_CONTROL(ucCH) = I2C_CRTL_START | I2C_CRTL_MASTER_MODE | I2C_CRTL_ACK )

//#define I2C_CMD_Q_CLEAR_CH(ucCH)		do{ ; }while(0)

#define I2C_CMD_Q_SET_MODE(type, ucCH, ucDev)	do{ (IO_TRANS_TYPE_CMD_Q_AUTO_MODE == type) ?	\
												(I2C_MODE_SEL(ucCH) |= (3 << (ucDev)*2)) : (I2C_MODE_SEL(ucCH) &= ~(3 << (ucDev)*2)); }while(0)


/***********CMD Header CMD***************/
#define I2C_CMD_Q_HDR_REPEAT_START	BIT(7)
#define I2C_CMD_Q_HDR_READ			BIT(6)
#define I2C_CMD_Q_HDR_STOP			BIT(5)
#define I2C_CMD_Q_HDR_END			BIT(4)
#define I2C_CMD_Q_HDR_LAST_ACK		BIT(3)

/***********other***************/
#define I2C_CMD_Q_PUSH_TO_MEM(dev, p_buf, cnt)	do{																 				\
												memcpy( m_stI2cCmdQ[dev].p_WtBuf + m_stI2cCmdQ[dev].dwWtIdx, p_buf, (cnt) );	\
												m_stI2cCmdQ[dev].dwWtIdx += cnt;												\
											}while(0)																			\

/***********debug***************/
#define I2C_CMD_Q_DBG_L(fmt, arg...)    //printf(fmt, ##arg)
#define I2C_CMD_Q_DBG_H(fmt, arg...)    printf(fmt, ##arg)

#define I2C_CMD_TEST					0

//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************
typedef struct i2c_cmd_q_buf
{
	UINT8*		p_RdBuf;
	UINT8*		p_WtBuf;
	UINT32		dwWtIdx;
	UINT32		dwRdIdx;
	UINT32		dwWtCnt;
	UINT32		dwRdCnt;
}I2C_CMD_Q_BUF;

//*****************************************************************************
//							declaration of Public variable
//*****************************************************************************
I2C_CMD_Q_BUF m_stI2cCmdQ[I2C_CH_CNT];


//*****************************************************************************
//							declaration of Private function
//*****************************************************************************
static BOOL i2c_cmd_q_wait_done(UINT8 ucCH, UINT8 ucDev, IO_TRANS* p_stTrans);

//*****************************************************************************
//							Public function
//*****************************************************************************
void i2c_cmd_q_init(void)
{
	memset( &m_stI2cCmdQ, 0, sizeof(m_stI2cCmdQ) );
}



//-----------------------------------------------------------------------------
// Description:
//		trigger CMDQ to transfer
//
//  Arguments:
//      device			: I2C_CH_0 || I2C_CH_1
//      enumTransType	: IO_TRANS_TYPE_CMD_Q_ONE_SHOT | IO_TRANS_CMD_Q_AUTO_MODE
//
//  Return Values:
//      BOOL
//-----------------------------------------------------------------------------
void i2c_cmd_q_kick(UINT8 ucCH, UINT8 ucDev)
{
	//clear nack status and error status
	I2C_ERR_ST(ucCH) = I2C_ERRST_CLEAR;
	I2C_NACK_ST(ucCH) |= I2C_NACK_ST_CLEAR;

	//Set One shot
	I2C_CMD_Q_FIRE(ucCH, ucDev);
}



//-----------------------------------------------------------------------------
// Description:
//		Before change the wt buffer, should check not in transfer status.
//		If we put new cmd to the wt buffer and auto mode access at the same time, maybe occur some trouble.
//
//  Arguments:
//      n/a
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_cmd_q_stop_trans(UINT8 ucCH, UINT8 ucDev)
{
	//UINT8 ucIntReg = 0;

	//ucIntReg = I2CCTRL1 & NTREN;
	//I2CCTRL1 &= ~NTREN;

	//change to one-shot mode. If doesn't this and clear end status, auto mdoe will run again
	I2C_CMD_Q_SET_MODE(IO_TRANS_TYPE_CMD_Q_ONE_SHOT, ucCH, ucDev);

	//wait for previous round end
	while( I2C_CMD_Q_IS_RUN(ucCH, ucDev) );

	//i2c_check_status(ucCH, ucDev);

	//clear end status
	//if( IS_MASK_SET(INTSTS, I2CCMDQEND) )
	//	INTSTS = (INTSTS & I2CCMDQENDMASK) | I2CCMDQEND;

	////clear device
	//if(ucCH == I2C_CH_0 || ucCH == I2C_CH_BOTH)		I2C_CMD_Q_CLEAR_CH(I2C_CH_0);
	//if(ucCH == I2C_CH_1 || ucCH == I2C_CH_BOTH)		I2C_CMD_Q_CLEAR_CH(I2C_CH_1);
	//
	//I2CCTRL1 |= ucIntReg;
}


//*****************************************************************************
// Description:
//		set cmd queue buf
//
//  Arguments:
//      device : I2C_CH_0 || I2C_CH_1
//      p_WtBuf: cmd queue write buf
//      ulWtCnt: cmd queue write count
//      p_RdBuf: cmd queue read buf
//      ulRdCnt: cmd queue read count
//
//  Return Values:
//     BOOL
//*****************************************************************************
BOOL i2c_cmd_q_set_buf(UINT8 ucCH, UINT8* p_WtBuf, UINT32 dwWtCnt, UINT8* p_RdBuf, DWORD dwRdCnt)
{
	//check channel
	if(ucCH >= I2C_CH_CNT)	
		return FALSE;

	//check point
	if(NULL == p_WtBuf || NULL == p_RdBuf)
		return FALSE;

	m_stI2cCmdQ[ucCH].p_WtBuf	= p_WtBuf;
	m_stI2cCmdQ[ucCH].dwWtCnt	= dwWtCnt;
	m_stI2cCmdQ[ucCH].p_RdBuf	= p_RdBuf;
	m_stI2cCmdQ[ucCH].dwRdCnt	= dwRdCnt;
	return TRUE;
}



//*****************************************************************************
//							private function
//*****************************************************************************

//-----------------------------------------------------------------------------
// Description:
//		set register for transfer
//
//  Arguments:
//      Device		: I2C_CH_0 || I2C_CH_1
//      dwWtAddr	: cmd queue write buf address
//      dwRdAddr	: cmd queue read buf address
//      dwRdBufSize	: cmd queue read count
//      bIsDma		: sue DMA or not
//
//  Return Values:
//      BOOL
//-----------------------------------------------------------------------------
BOOL i2c_cmd_q_set_reg(UINT8 ucCH, UINT8 ucDev, UINT32 dwWtAddr, UINT32 dwRdAddr, UINT32 dwRdBufSize, IO_TRANS_TYPE enumTransType)
{
	//check device id
	if(ucCH >= I2C_CH_CNT)
		return FALSE;

	if(ucDev >= I2C_DEV_CNT)
		return FALSE;

	if(0 == dwWtAddr)
		return FALSE;

	if( dwRdBufSize > 0 && dwRdAddr == 0)
		return FALSE;

	//convert to ram mapping address
	dwWtAddr = dm_get_dma_map(dwWtAddr);
	dwRdAddr = dm_get_dma_map(dwRdAddr);
   
	//cmd addr
	I2C_CMDHA(ucCH, ucDev) = (dwWtAddr >> 8) & 0xff;
	I2C_CMDLA(ucCH, ucDev) = dwWtAddr & 0xff;

	//memory addr
	I2C_RAMHA(ucCH, ucDev) = (dwRdAddr >> 8) & 0xff;
	I2C_RAMLA(ucCH, ucDev) = dwRdAddr & 0xff;

	//Ring buffer. Only use in Auto mode
	//auto mode , need set the DMA buffer lens setting for auto receive data to memory
	//set the receive data times in receive buffer   , for one shot mode used , set all zero
	if(enumTransType == IO_TRANS_TYPE_CMD_Q_AUTO_MODE){
		I2C_LNGRH(ucCH, ucDev)	= (dwRdBufSize >> 4) & 0xff;
		I2C_LNGRL(ucCH, ucDev)	= dwRdBufSize & 0xf;
	}
	else{
		I2C_LNGRH(ucCH, ucDev)	= 0;
		I2C_LNGRL(ucCH, ucDev)	= 0;
		I2C_LNGSTH(ucCH, ucDev)	= 0;
	}

	//Enable CMDQ
    #if __ENABLE_SLEEP_MODE__ // {
    //+SHAWN20151116
	I2C_CONTROL1(ucCH)	= (I2C_CONTROL1(ucCH) & I2C_CRTL1_SLEEP_ENABLE)
                            | (I2C_CRTL1_CMDQ_ENABLE
							| (I2C_DEV_CNT - 1) << 4
							| I2C_CRTL1_MDL_EN);
    //-SHAWN20151116
    #else
    I2C_CONTROL1(ucCH)	= (	I2C_CRTL1_CMDQ_ENABLE	| 
							(I2C_DEV_CNT - 1) << 4	| 
							I2C_CRTL1_MDL_EN			);
    #endif // } 1

	//Set mode
	I2C_CMD_Q_SET_MODE(enumTransType, ucCH, ucDev);

	//set option for data format
	I2C_CMD_Q_SET_OPTION(ucCH);

	return TRUE;
}


BOOL i2c_cmd_q_wait_done(UINT8 ucCH, UINT8 ucDev, IO_TRANS* p_stTrans)
{
	UINT32 u32TimeOut = 0;

	/* polling mode */
	if( I2C_CMD_Q_IS_ENABLE_INT(ucCH) )
	{	
		do{
			//check error status
			if( I2C_STAT(ucCH) & (I2C_STAT_TIMEOUT | I2C_STAT_ARB) )
				break;

			//check NACK
			if( I2C_ERR_ST(ucCH) & BIT(ucDev) )
				break;

			//check timeout
			if( ++u32TimeOut > 100000){
				stI2cCnt.dwTimeout++;
				goto check_status;
			}
		}while( !I2C_CMD_Q_IS_INT_PENDING(ucCH) );

		//clear interrupt
		I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;
	}
	//wait for end flag
	
	u32TimeOut = 0;
	do{	
		if( ++u32TimeOut > 100000){
			stI2cCnt.dwTimeout++;
			goto check_status;
		}

		//delay 15.26us
		WNCKR = 0; 
#if I2C_CMD_TEST
		printf(".");
#endif
	}while( I2C_IS_BUS_BUSY(ucCH) );  //try to check CMDQMODE, but fail. Change to check this bit


check_status:
	p_stTrans->ucErrStat = i2c_get_err_status(ucCH, ucDev);

	if( 0 != p_stTrans->ucErrStat ){
		stI2cCnt.dwCmdQFail++;
		//clear interrupt
		I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;
		return FALSE;
	}
	
	return  TRUE;
}

//-----------------------------------------------------------------------------
// Description:
//		Transfer I2C by CMDQ
//
//  Arguments:
//      device		: I2C_CH_0 || I2C_CH_1
//      p_stTrans	: transfer control sturcture
//
//  Return Values:
//      BOOL
//-----------------------------------------------------------------------------
BOOL i2c_cmd_q_trans(UINT8 ucCH, IO_TRANS* p_stTrans)
{
	UINT8 i = 0;
	UINT8 u8TxCmd[3];
	UINT8 u8RxCmd[3];

	memset( u8TxCmd, 0, sizeof(u8TxCmd) );
	memset( u8RxCmd, 0, sizeof(u8RxCmd) );

	if(NULL == p_stTrans)
		return FALSE;

	//make TX header
	if( p_stTrans->p_WtBuf && (p_stTrans->dwWtCnt > 0) ){
		u8TxCmd[0] = p_stTrans->u8SlaveId;
		u8TxCmd[1] = (p_stTrans->dwWtCnt + p_stTrans->dwWtDataCnt - 1) & 0x7;
		u8TxCmd[2] = (p_stTrans->dwWtCnt + p_stTrans->dwWtDataCnt - 1) >> 3;

		if( (NULL == p_stTrans->p_RdBuf) || (p_stTrans->dwRdCnt == 0) )		u8TxCmd[1] |= I2C_CMD_Q_HDR_STOP;
	}

	//make RX header
	if( p_stTrans->p_RdBuf && (p_stTrans->dwRdCnt > 0) ){
		u8RxCmd[0] = p_stTrans->u8SlaveId;
		u8RxCmd[1] = I2C_CMD_Q_HDR_READ | I2C_CMD_Q_HDR_STOP | ((p_stTrans->dwRdCnt - 1) & 0x7);	//read  mask
		u8RxCmd[2] = (p_stTrans->dwRdCnt - 1) >> 3; 	
		
		if( (NULL != p_stTrans->p_WtBuf) && (p_stTrans->dwWtCnt > 0) )		u8RxCmd[1] |= I2C_CMD_Q_HDR_REPEAT_START;
	}

	//end mask
	if(TRUE == p_stTrans->bIsEnd){
		if( NULL == p_stTrans->p_RdBuf )	u8TxCmd[1] |= I2C_CMD_Q_HDR_END;
		else								u8RxCmd[1] |= I2C_CMD_Q_HDR_END;
	}

	//scan all channel
	for(i = 0; i < I2C_CH_CNT; i++)
	{
		/* check device */
		if(ucCH != I2C_CH_BOTH){
			if(i != ucCH)
				continue;
		}

		//Check point
		if(NULL == m_stI2cCmdQ[i].p_WtBuf)
			return FALSE;

		//Before change the wt buffer, should check not in transfer status.
		//If we put new cmd to the wt buffer and auto mode access at the same time, maybe occur some troble.
		//i2c_cmd_q_stop_trans(i, p_stTrans->u8Dev);

		//push to TX memory
		if( p_stTrans->p_WtBuf && (p_stTrans->dwWtCnt > 0) ){			
			I2C_CMD_Q_PUSH_TO_MEM( i, u8TxCmd, sizeof(u8TxCmd) );
			I2C_CMD_Q_PUSH_TO_MEM( i, p_stTrans->p_WtBuf, p_stTrans->dwWtCnt );

			if( p_stTrans->p_WtDataBuf && (p_stTrans->dwWtDataCnt > 0) ){
				I2C_CMD_Q_PUSH_TO_MEM( i, p_stTrans->p_WtDataBuf, p_stTrans->dwWtDataCnt );
			}
		}

		//push to RX memory
		if( p_stTrans->p_RdBuf && (p_stTrans->dwRdCnt > 0) ){
			if( IS_MASK_SET(u8RxCmd[1], I2C_CMD_Q_HDR_REPEAT_START) )	I2C_CMD_Q_PUSH_TO_MEM( i, u8RxCmd + 1, sizeof(u8RxCmd) - 1 );
			else														I2C_CMD_Q_PUSH_TO_MEM( i, u8RxCmd, sizeof(u8RxCmd) );
		}

		//check end		/* note: need to think the buffer and count from where */
		if(TRUE == p_stTrans->bIsEnd)
		{
			//reset write buf index
			m_stI2cCmdQ[i].dwWtIdx = 0;

#if I2C_CMD_TEST
			WORD j = 0;
			printf("\r\n");
			for(j = 0; j <400; j++){
				if(j!=0 && (j % 16 == 0)){
					printf("\r\n");
				}
				printf("%02x  ",  m_stI2cCmdQ[i].p_WtBuf[j]);
			}
			printf("\r\n");
#endif

			//set register
			if( FALSE == i2c_cmd_q_set_reg(i, p_stTrans->u8Dev, (DWORD)m_stI2cCmdQ[i].p_WtBuf, (DWORD)p_stTrans->p_RdBuf, p_stTrans->dwRdCnt, p_stTrans->enumTransType) )
				return FALSE;	
			//if( FALSE == i2c_cmd_q_set_reg(i, ucDev, (DWORD)m_stI2cCmdQ[i].p_WtBuf, (DWORD)m_stI2cCmdQ[i].p_RdBuf, m_stI2cCmdQ[i].dwRdCnt, p_stTrans->enumTransType) )
			//	return FALSE;		

			//start to transfer
			i2c_cmd_q_kick(i, p_stTrans->u8Dev);

			//wait and check status
			if( FALSE == i2c_cmd_q_wait_done(i, p_stTrans->u8Dev, p_stTrans) )		return FALSE;			
		}
	}

	return TRUE;
}



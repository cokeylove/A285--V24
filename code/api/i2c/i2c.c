/*-----------------------------------------------------------------------------
* TITLE: i2c.c
*
* Author : Clark
*
* History : 
* ------------------------------
* Version : 1.10
* ------------------------------
*
* Note : To link [api_xxx.o] if related api function be used.
*
*---------------------------------------------------------------------------*/
 
//*****************************************************************************
//#include "chip_type.h"
//#include "rom.h"
//#include "core_gpio.h"
//#include "debug_print.h"
#include "..\include.h"


#include "i2c.h"
#include "i2c_pio.h"
#include "i2c_cmd_q.h"

//*****************************************************************************
//							declaration of Define and Marco
//*****************************************************************************
#define _nop_()		__nds32__nop()

//setting
//#define I2C_SET_MODE()			( I2CCTRL1 |= (I2C_MODE_0<<5) )

//debug
#define I2C_DBG_L(fmt, arg...)		//printf(fmt, ##arg)
#define I2C_DBG_H(fmt, arg...)		printf(fmt, ##arg)


#define EI2C_Freq_100Khz 			0x78 //0x28;
#define EI2C_Freq_400Khz 			//0x08;


const UINT8 I2C_PSR_DBG = EI2C_Freq_100Khz;
const UINT8 I2C_HSPR_DBG = EI2C_Freq_100Khz;


//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************


//*****************************************************************************
//							declaration of Public variable
//*****************************************************************************
const UINT16 u8I2cBase[I2C_CH_CNT] = {0x3680, 0x3500, 0x3580};


//*****************************************************************************
//							declaration of Private variable
//*****************************************************************************
I2C_CNT stI2cCnt;


//*****************************************************************************
//							declaration of Private function
//*****************************************************************************





//*****************************************************************************
//							Public function
//*****************************************************************************

//*****************************************************************************
// Description:
//		Init I2C
//
//  Arguments:
//      n/a
//
//  Return Values:
//      n/a
//*****************************************************************************
void i2c_init(void)
{
	memset( &stI2cCnt, 0, sizeof(I2C_CNT) );

	i2c_cmd_q_init();
	i2c_pio_init();
}



//*****************************************************************************
// Description:
//		Init I2C interface function
//
//  Arguments:
//      device			: I2C_CH_D || I2C_CH_D
//      bIs3wire		: TRUE=> 3 write. FALSE => 4 wire
//
//  Return Values:
//      n/a
//*****************************************************************************
void i2c_ch_init_gpio(BYTE ucCH, BOOL bIsSwap)
{  
	switch(ucCH)
	{
		case I2C_CH_D:
			GCR2   |= BIT5;      // Enable Ch3
			GPCRH1  = ALT;       // Ch3
			GPCRH2  = ALT;       // Ch3

			WUEMR6 |= BIT2;      // Falling triggle
			WUENR6 |= BIT2;      // Enable WUI
			break;

		case I2C_CH_E:
			PMER1  |= BIT0;     // Enable Ch4
			GPCRE0  = ALT;      // Ch4
			GPCRE7  = ALT;      // Ch4

			WUEMR4 |= BIT6;     // Falling triggle
			WUENR4 |= BIT6;     // Enable WUI
			break;

		case I2C_CH_F:
			PMER1  |= BIT1;     // Enable Ch5
			if(bIsSwap)
			{
				GPCRB3  = ALT;      // Ch0
				GPCRB4  = ALT;      // Ch0    

				WUEMR9 |= BIT6;     // Falling triggle
				WUENR9 |= BIT6;     // Enable WUI   
			}
			else
			{
				GPCRA4  = ALT;      // Ch5
				GPCRA5  = ALT;      // Ch5       

				WUEMR8 |= BIT2;     // Falling triggle
				WUENR8 |= BIT2;     // Enable WUI   
			}
			break;

		default:
			break;
	}	

	//swap channel
#if I2C_SWAP_TEST
	i2c_swap_channel(4,5);

	GPCRE0  = ALT;      // Ch4
	GPCRE7  = ALT;      // Ch4
	PMER1  |= BIT0;     // Enable Ch4

	GPCRA4  = ALT;      // Ch5
	GPCRA5  = ALT;      // Ch5       
	PMER1  |= BIT1;     // Enable Ch5
#endif
}

//****************************************************************************************
// Function name: i2c_swap_channel
// Description:
//   i2c swap channel
// Arguments:
//   UINT8 ucChA: Channel to swap.
//   UINT8 ucChB: Channel to swap
// Return Values:
//   void
// Note:
//   none
//****************************************************************************************
void i2c_swap_channel(UINT8 ucChA, UINT8 ucChB)
{
    //RTL_PRINTF("swap %X: %X\r\n",ucChA, ucChB);
    if(ucChA == ucChB)
    {
        return;    
    }
    if(ucChA == 0)
    {
        if(ucChB == 1)   // Ch0 and Ch1 swap
        {
            I2C_CH_SWAP10 = 0x01;                
        }    
        else if(ucChB == 2)   // Ch0 and Ch2 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0xF0) | 0x02;    
            I2C_CH_SWAP32 &= 0xF0;    
        }
        else if(ucChB == 3)   // Ch0 and Ch3 swap
        {
            
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0xF0) | 0x03;    
            I2C_CH_SWAP32 &= 0x0F;    
        }
        else if(ucChB == 4)   // Ch0 and Ch4 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0xF0) | 0x04;    
            I2C_CH_SWAP54 &= 0xF0;    
        }
        else if(ucChB == 5)   // Ch0 and Ch5 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0xF0) | 0x05;    
            I2C_CH_SWAP54 &= 0x0F;    
        }
    }
    else if(ucChA == 1)
    {
        if(ucChB == 2)   // Ch0 and Ch2 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0x0F) | 0x20;    
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0xF0) | 0x01;    
        }
        else if(ucChB == 3)   // Ch0 and Ch3 swap
        {    
		    I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0x0F) | 0x30;    
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0x0F) | 0x10;    
        }
        else if(ucChB == 4)   // Ch0 and Ch4 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0x0F) | 0x40;    
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0xF0) | 0x01;    
        }
        else if(ucChB == 5)   // Ch0 and Ch5 swap
        {
            I2C_CH_SWAP10 = (I2C_CH_SWAP10 & 0x0F) | 0x50;    
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0x0F) | 0x10;    
        }
    }
    else if(ucChA == 2)
    {
        if(ucChB == 3)   // Ch0 and Ch4 swap
        {
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0xF0) | 0x03;
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0x0F) | 0x20;    
        }
        else if(ucChB == 4)   // Ch0 and Ch4 swap
        {
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0xF0) | 0x04;
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0xF0) | 0x02;    
        }
        else if(ucChB == 5)   // Ch0 and Ch5 swap
        {
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0xF0) | 0x05;
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0x0F) | 0x20;    
        }        
    } 
    else if(ucChA == 3)
    {
        if(ucChB == 4)   // Ch0 and Ch4 swap
        {
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0x0F) | 0x40;            
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0xF0) | 0x03;    
        }
        else if(ucChB == 5)   // Ch0 and Ch5 swap
        {
            I2C_CH_SWAP32 = (I2C_CH_SWAP32 & 0x0F) | 0x50;            
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0x0F) | 0x30;    
        }        
    } 
    else if(ucChA == 4)
    {
        if(ucChB == 5)   // Ch0 and Ch5 swap
        {
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0xF0) | 0x05;    
            I2C_CH_SWAP54 = (I2C_CH_SWAP54 & 0x0F) | 0x40;    
        }        
    }     
}


void i2c_ch_reset(UINT8 u8CH)
{
	I2C_DHTR(u8CH)		|= I2C_DHTR_SOFT_RESET;
	I2C_DHTR(u8CH)		&= ~I2C_DHTR_SOFT_RESET;
	I2C_CONTROL( u8CH ) = ( I2C_CRTL_SOFT_RESET | I2C_CRTL_HALT );
	I2C_CONTROL( u8CH ) = 0;

	I2C_CONTROL( u8CH ) &= ( ~I2C_CRTL_START );
	I2C_CONTROL( u8CH ) |= ( I2C_CRTL_SOFT_RESET ); //state reset
	I2C_CONTROL( u8CH ) &= ( ~I2C_CRTL_SOFT_RESET );

	//set prescale and time out
	I2C_PRESCALE(u8CH)	= I2C_PSR_DBG;
	I2C_HSPR(u8CH)		= I2C_HSPR_DBG;
	I2C_TIME_OUT(u8CH)	= 0xFF;
	I2C_TIME_BUF(u8CH)	= 0x3F;

	//other		
	I2C_ID_ADD(u8CH)	= 0x11;
	I2C_CONTROL(u8CH)	= I2C_CRTL_MASTER_MODE | I2C_CRTL_INT_EN | I2C_CRTL_ACK;
    #if __ENABLE_SLEEP_MODE__ // {
    //+SHAWN20151116
	I2C_CONTROL1(u8CH)	= (	(I2C_DEV_CNT - 1) << 4	| 
							I2C_CRTL1_MDL_EN | 
							I2C_CRTL1_SLEEP_ENABLE);
    //-SHAWN20151116
    #else
	I2C_CONTROL1(u8CH)	= (	(I2C_DEV_CNT - 1) << 4	| 
							I2C_CRTL1_MDL_EN			);
    #endif // } 1
}


void i2c_slv_ch_reset(UINT8 u8CH, UINT8 u8SlaveId, UINT16 u16SramAddr)
{
	I2C_DHTR(u8CH)		|= I2C_DHTR_SOFT_RESET;
	I2C_DHTR(u8CH)		&= ~I2C_DHTR_SOFT_RESET;
	I2C_CONTROL( u8CH ) = ( I2C_CRTL_SOFT_RESET | I2C_CRTL_HALT );
	I2C_CONTROL( u8CH ) = 0;

	I2C_CONTROL( u8CH ) &= ( ~I2C_CRTL_START );
	I2C_CONTROL( u8CH ) |= ( I2C_CRTL_SOFT_RESET ); //state reset
	I2C_CONTROL( u8CH ) &= ( ~I2C_CRTL_SOFT_RESET );

	//enable rx count
	//I2C_BYTE_CNT_L(u8CH) = 0xF;
	//I2C_BYTE_CNT_H(u8CH) = 0xFF;

	I2C_ID_ADD(u8CH)	= u8SlaveId;

	//set prescale and time out
	I2C_PRESCALE(u8CH)	= I2C_PSR_DBG;
	I2C_HSPR(u8CH)		= I2C_HSPR_DBG;

    I2C_TIME_OUT(u8CH) 	= 0xFF;   	
	//I2C_TIME_BUF(u8CH)	= 0x3F;

	//Set read buffer. Device 1 always Read buffer
    I2C_RAMHA(u8CH, I2C_DEV_1)	= (UINT8)((u16SramAddr & 0xFF00) >> 8);  
    I2C_RAMLA(u8CH, I2C_DEV_1)	= (UINT8)(u16SramAddr & 0x00FF);

	//Set write buffer. Device 2 always write buffer
    I2C_RAMHA(u8CH, I2C_DEV_2)	= (UINT8)((u16SramAddr & 0xFF00) >> 8);  
    I2C_RAMLA(u8CH, I2C_DEV_2)	= (UINT8)(u16SramAddr & 0x00FF);

#if !(__ENABLE_HID_I2C_TIMEOUT_RESET__)
	//disable timeout
	I2C_TO_ARB_ST(u8CH) &= ~(I2C_TO_SCL_ENABLE | I2C_TO_SDA_ENABLE);
#endif

	I2C_CONTROL(u8CH)	= I2C_CRTL_INT_EN | I2C_CRTL_ACK | I2C_CRTL_SLAVE_MODE;
    I2C_CONTROL1(u8CH)	= (	I2C_CRTL1_CMDQ_ENABLE	|
							(I2C_DEV_CNT - 1) << 4	| 
							I2C_CRTL1_MDL_EN			);
}

//*****************************************************************************
// Description:
//		set cmd queue buf
//
//  Arguments:
//      device : I2C_CH_D || I2C_CH_D
//      p_WtBuf: cmd queue write buf
//      ulWtCnt: cmd queue write count
//      p_RdBuf: cmd queue read buf
//      ulRdCnt: cmd queue read count
//
//  Return Values:
//      BOOL
//*****************************************************************************
BOOL i2c_set_cmd_q_buf(BYTE ucCH, BYTE* p_WtBuf, DWORD dwWtCnt, BYTE* p_RdBuf, DWORD dwRdCnt)
{
	return i2c_cmd_q_set_buf(ucCH, p_WtBuf, dwWtCnt, p_RdBuf, dwRdCnt);
}


//*****************************************************************************
// Description:
//		set slave buf
//
//  Arguments:
//      device : I2C_CH_D || I2C_CH_D
//      p_WtBuf: cmd queue write buf
//      ulWtCnt: cmd queue write count
//      p_RdBuf: cmd queue read buf
//      ulRdCnt: cmd queue read count
//
//  Return Values:
//      BOOL
//*****************************************************************************
BOOL i2c_slave_set_buf(BYTE ucCH, DIRECTION enumDirection, BYTE* p_WtBuf)
{
	UINT16	u16Addr = 0;
	UINT8	u8Dev	= 0;

	u16Addr = dm_get_dma_map( (UINT32)p_WtBuf );
	if( 0 == u16Addr )	return FALSE;
	
	//Slave has only one device. 
	//Device 1 always put Read buffer. Device 2 always put Read buffer
	if(TX_DIRECT == enumDirection)		u8Dev = I2C_DEV_2;
	else								u8Dev = I2C_DEV_1;

    I2C_RAMHA(ucCH, u8Dev)	= (UINT8)((u16Addr & 0xFF00) >> 8);  
    I2C_RAMLA(ucCH, u8Dev)	= (UINT8)(u16Addr & 0x00FF);
	return TRUE;
}


//-----------------------------------------------------------------------------
// Put data to the structure
//
//  Arguments:
//		p_stTrans	: transfer management structure
//		p_WtBuf		: write command buffer
//		dwWtCnt		: write command count
//		p_WtDataBuf	: write data buffer
//		dwWtDataCnt	: write data count
//		p_RdBuf		: read data buffer
//		dwRdCnt		: read data count
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_set_trans_struc(IO_TRANS* p_stTrans, BYTE* p_WtBuf, DWORD dwWtCnt, BYTE* p_WtDataBuf, 
						 DWORD dwWtDataCnt, BYTE* p_RdBuf, DWORD  dwRdCnt)	
{
	p_stTrans->p_WtBuf		= p_WtBuf;
	p_stTrans->dwWtCnt		= dwWtCnt;
	p_stTrans->p_WtDataBuf	= p_WtDataBuf;
	p_stTrans->dwWtDataCnt	= dwWtDataCnt;
	p_stTrans->p_RdBuf		= p_RdBuf;
	p_stTrans->dwRdCnt		= dwRdCnt;
}



//-----------------------------------------------------------------------------
// Description:
//		Main Transfer API
//
//  Arguments:
//      device		: I2C_CH_D || I2C_CH_D
//      p_stTrans	: transfer control sturcture
//
//  Return Values:
//      TRUE or FALSE
//-----------------------------------------------------------------------------
BOOL i2c_trans(BYTE ucCH, IO_TRANS* p_stTrans,BOOL pecSupport,BYTE pecValue)
{
	BOOL bSucc = TRUE;

	//check point
	if(NULL == p_stTrans)
		return FALSE;

	switch(p_stTrans->enumTransType)
	{
		case IO_TRANS_TYPE_PIO:
			i2c_ch_reset(ucCH);
			if( FALSE == i2c_pio_trans(ucCH, p_stTrans,pecSupport,pecValue) )			bSucc = FALSE;	//If fail, should do finish
			if( FALSE == i2c_pio_trans_end(ucCH) )					bSucc = FALSE;

			p_stTrans->ucErrStat = i2c_get_err_status(ucCH, I2C_DEV_1);
			if( 0 != p_stTrans->ucErrStat )							bSucc = FALSE;

			return bSucc;
			break;

		case IO_TRANS_TYPE_CMD_Q_ONE_SHOT:
		case IO_TRANS_TYPE_CMD_Q_AUTO_MODE:
			i2c_ch_reset(ucCH);
			if( FALSE == i2c_cmd_q_trans(ucCH, p_stTrans) )
			{
				stI2cCnt.dwCmdQFail++;
				return FALSE;
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Description:
//		Stop CmdQ transfer
//
//  Arguments:
//      device		: I2C_CH_D || I2C_CH_D
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_stop_trans(BYTE ucCH, BYTE ucDev)
{
	i2c_cmd_q_stop_trans(ucCH, ucDev);
}


void i2c_fire(BYTE ucCH, UINT8 u8Dev)
{
	i2c_cmd_q_kick(ucCH, u8Dev);
}


UINT8 i2c_get_err_status(UINT8 ucCH, UINT8 ucDev)
{
	UINT8	u8ErrStat	= 0;
	UINT8	u8Stat		= I2C_STAT(ucCH);

	//Check status
	if(u8Stat & I2C_STAT_TIMEOUT){
		//check SCL timeout
		if(I2C_TO_ARB_ST(ucCH) & I2C_TO_SCL){
			I2C_TO_ARB_ST(ucCH) = (I2C_TO_ARB_ST(ucCH) & (I2C_TO_SCL_ENABLE | I2C_TO_SDA_ENABLE | I2C_TO_ARB_ENABLE)) | I2C_TO_SCL;				
			//do something
			u8ErrStat |= BIT(ucDev);
			stI2cCnt.dwSclTimeout++;
			I2C_DBG_L("I2C SCL Timeout\r\n");
		}
		
		//check SDA timeout
		if(I2C_TO_ARB_ST(ucCH) & I2C_TO_SDA){
			I2C_TO_ARB_ST(ucCH) = (I2C_TO_ARB_ST(ucCH) & (I2C_TO_SCL_ENABLE | I2C_TO_SDA_ENABLE | I2C_TO_ARB_ENABLE)) | I2C_TO_SDA;
			//do something
			u8ErrStat |= BIT(ucDev);
			stI2cCnt.dwSdaTimeout++;
			I2C_DBG_L("I2C SDA Timeout\r\n");
		}
	}

	//Check Arbiter
	if(u8Stat & I2C_STAT_ARB){
		if(I2C_TO_ARB_ST(ucCH) & I2C_TO_ARB_LOST){
			I2C_TO_ARB_ST(ucCH) = (I2C_TO_ARB_ST(ucCH) & (I2C_TO_SCL_ENABLE | I2C_TO_SDA_ENABLE | I2C_TO_ARB_ENABLE)) | I2C_TO_ARB_CLEAR;
			//do something
			u8ErrStat |= BIT(ucDev);
			stI2cCnt.dwArbiter++;
		}
		I2C_DBG_L("I2C Arbitration lost\r\n");
	}

	//Check Error
	if( I2C_ERR_ST(ucCH) & BIT(ucDev) ){
		//Get Error device and NACK reason
		u8ErrStat |= (I2C_ERR_ST(ucCH) & 0xf ) | (I2C_NACK_ST(ucCH) << 4);
		//clear error
		I2C_ERR_ST(ucCH) |= BIT(ucDev);
		//Get NACK reason and clear NACK status
		I2C_NACK_ST(ucCH) |= I2C_NACK_ST_CLEAR;			
		//do something	
		stI2cCnt.dwNack++;
		I2C_DBG_L("I2C Nack, reason: %x\r\n", u8ErrStat >> 4);
	}

	return u8ErrStat;
}


/************************************ enable function *****************************************/
//-----------------------------------------------------------------------------
// enable sleep mode for doze mode. Enable it before enter to doze mode. Disable it after leave doze mode
//
//  Arguments:
//		Device		: I2C_CH_D || I2C_CH_D
//		bIsEnable	: enable sleep mode or not
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_enable_sleep_mode(BYTE ucCH, BOOL bIsEnable)
{
	if(TRUE == bIsEnable)	I2C_CONTROL1(ucCH) |= I2C_CRTL1_SLEEP_ENABLE;
	else					I2C_CONTROL1(ucCH) &= ~I2C_CRTL1_SLEEP_ENABLE;
}



/************************************ Wait time *****************************************/
//-----------------------------------------------------------------------------
// set wait time to reg. One wait time is equal one scale
//
//  Arguments:
//		Device		: I2C_CH_D || I2C_CH_D
//		ucWaitTime	: wait time
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_set_cmd_q_wait_time(BYTE ucCH, BYTE ucDev, BYTE ucWaitTime)
{
	I2C_WCSR(ucCH, ucDev) = ucWaitTime;
}


//-----------------------------------------------------------------------------
// Get wait time to reg. One wait time is equal one scale
//
//  Arguments:
//		Device		: I2C_CH_D || I2C_CH_D
//
//  Return Values:
//     ucWaitTime	: wait time
//-----------------------------------------------------------------------------
BYTE i2c_get_cmd_q_wait_time(BYTE ucCH, BYTE ucDev)
{
	return I2C_WCSR(ucCH, ucDev);
}


//-----------------------------------------------------------------------------
// set scale time to reg. One scale is equal how much clock.
//
//  Arguments:
//		Device		: I2C_CH_D || I2C_CH_D
//		ucWaitTime	: wait time
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
void i2c_set_cmd_q_scale_time(BYTE u8CH, BYTE ucScaleTime)
{
	I2C_CSR(u8CH) = ucScaleTime;
}


//-----------------------------------------------------------------------------
// Get scale time to reg. One scale is equal how much clock.
//
//  Arguments:
//		n/a
//
//  Return Values:
//     ucWaitTime	: wait time
//-----------------------------------------------------------------------------
BYTE i2c_get_cmd_q_scale_time(BYTE ucCH)
{
	return I2C_CSR(ucCH);
}


/************************************ Interrupt *****************************************/
//-----------------------------------------------------------------------------
// handle interrupt 
//
//  Arguments:
//		n/a
//
//  Return Values:
//     n/a
//-----------------------------------------------------------------------------
void i2c_interrupt()
{
	//BOOL trigger = FALSE;
	//
	//stI2cCnt.dwIntTotal++;

	//I2C_DBG_L("I2CSTS reg: 0x%x\r\n",I2CSTS);
	//I2C_DBG_L("INTSTS reg: 0x%x\r\n",INTSTS);

	//if( IS_MASK_SET(INTSTS, I2CRING0FI) )			/* handle ring buffer 0 full */
	//{
	//	stI2cCnt.dwFull0++;
	//	I2C_DBG_L("handle ring buffer 0 ful\r\n");
	//	trigger = TRUE;
	//	//Clear status
	//	INTSTS = (INTSTS & I2CCMDQENDMASK) | I2CRING0FI;
	//}
	//
	//if( IS_MASK_SET(INTSTS, I2CRING1FI) )			/* handle ring buffer 1 full */
	//{
	//	stI2cCnt.dwFull1++;
	//	I2C_DBG_L("handle ring buffer 1 full\r\n");
	//	trigger = TRUE;
	//	//Clear status
	//	INTSTS = (INTSTS & I2CCMDQENDMASK) | I2CRING1FI;
	//}
	//
	//if( IS_MASK_SET(INTSTS, I2CCMDQEND) )			/* COMQ transfer completely */
	//{		
	//	if(IS_MASK_SET(INTSTS, CH0CMDQEND) )	stI2cCnt.dwCmdQEnd0++;
	//	if(IS_MASK_SET(INTSTS, CH1CMDQEND) )	stI2cCnt.dwCmdQEnd1++;

	//	I2C_DBG_L("COMQ transfer completely\r\n");
	//	trigger = TRUE;
	//	//Clear status
	//	INTSTS = (INTSTS & I2CCMDQENDMASK) | I2CCMDQEND;
	//}
	//
	//if( IS_MASK_SET(I2CSTS, TRANENDIF) )			/* PIO transfer end */
	//{				
	//	stI2cCnt.dwTransEnd++;
	//	I2C_DBG_L("PIO transfer end\r\n");
	//	trigger = TRUE;
	//	//Clear status
	//	I2CSTS |= TRANENDIF;
	//}

	//if(trigger == FALSE)						/* PIO transfer completely */
	//{
	//	stI2cCnt.dwOther++;
	//	I2C_DBG_L("PIO data transfer completely\r\n");
	//}
}


/************************************ other *****************************************/

//-----------------------------------------------------------------------------
// Get I2C count
//
//  Arguments:
//		n/a
//
//  Return Values:
//     I2C_CNT	: I2C count
//-----------------------------------------------------------------------------
I2C_CNT i2c_get_cnt()	
{
	return stI2cCnt;
}

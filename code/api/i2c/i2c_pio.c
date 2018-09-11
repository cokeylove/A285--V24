/*-----------------------------------------------------------------------------
* TITLE: i2c_cmd_q.c
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
//#include "chip_type.h"
//#include "rom.h"
//#include "debug_print.h"
#include "..\include.h"

#include "i2c_pio.h"
#include "i2c_module.h"



//*****************************************************************************
//							declaration of Define and Marco
//*****************************************************************************

/***********check the status***************/
//basic
#define I2C_IS_TRANS(channel)			(IS_MASK_CLEAR(I2C_STAT(channel), I2C_STAT_TRANS_IN_PROGRESS) > 0 ? TRUE:FALSE)


/***********debug***************/
#define I2C_PIO_DBG_L(fmt, arg...)    //printf(fmt, ##arg)
#define I2C_PIO_DBG_H(fmt, arg...)    printf(fmt, ##arg)

//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************


//*****************************************************************************
//							declaration of Public variable
//*****************************************************************************



//*****************************************************************************
//							declaration of Private function
//*****************************************************************************
BOOL i2c_pio_send_id(BYTE ucCH, UINT8 u8SlaveID, DIRECTION enumDirect);
BOOL i2c_pio_trans_data(BYTE ucCH, DIRECTION enumDirect, BYTE* p_buf, DWORD dwTransCnt, BOOL bIsStart,BOOL IsPEC,BYTE pecValue);


//*****************************************************************************
//							Public function
//*****************************************************************************
void i2c_pio_init(void)
{

}




//-----------------------------------------------------------------------------
// Description:
//		Transfer SPI by PIO
//
//  Arguments:
//      device		: SPI_CH_0 || SPI_CH_1
//      enumDirect	: TX or RX direction
//      p_buf		: TX or RX buffer
//      dwTransCnt	: transfer byte count
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
BOOL i2c_pio_trans(BYTE ucCH, IO_TRANS* p_stTrans,BOOL pecSupport,BYTE pecValue)
{
	//clear interrupt
    #if __ENABLE_SLEEP_MODE__ // {
    //+SHAWN20151116
	I2C_CONTROL1(ucCH)	= (I2C_CONTROL1(ucCH) & I2C_CRTL1_SLEEP_ENABLE)
	                      |I2C_CRTL1_MDL_EN;
    //-SHAWN20151116
    #else
    I2C_CONTROL1(ucCH)	= I2C_CRTL1_MDL_EN;
    #endif // } 1
	I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;


	if(p_stTrans->p_WtBuf && (p_stTrans->dwWtCnt > 0) ){
		//send slave ID
		if( FALSE == i2c_pio_send_id(ucCH, p_stTrans->u8SlaveId, TX_DIRECT) )		return FALSE;
		//Send command
		if( FALSE == i2c_pio_trans_data(ucCH, TX_DIRECT, p_stTrans->p_WtBuf, p_stTrans->dwWtCnt, FALSE,FALSE,pecValue) )		return FALSE;
		//Send data
		if (p_stTrans->p_WtDataBuf && (p_stTrans->dwWtDataCnt > 0)){
			if(pecSupport)
			{
				//p_stTrans->p_WtDataBuf[p_stTrans->dwWtDataCnt] = SMBCRC8_A;
				if( FALSE == i2c_pio_trans_data(ucCH, TX_DIRECT, p_stTrans->p_WtDataBuf, p_stTrans->dwWtDataCnt, FALSE,TRUE,pecValue) )	return FALSE;
			}
			else 
			{
				if( FALSE == i2c_pio_trans_data(ucCH, TX_DIRECT, p_stTrans->p_WtDataBuf, p_stTrans->dwWtDataCnt, FALSE,FALSE,pecValue) )	return FALSE;
			}
		}
	}

	//Read Data
	if(p_stTrans->p_RdBuf && (p_stTrans->dwRdCnt > 0) ){
		//send slave ID
		if( FALSE == i2c_pio_send_id(ucCH, p_stTrans->u8SlaveId, RX_DIRECT) )		return FALSE;
		//Read Data
		if( FALSE == i2c_pio_trans_data(ucCH, RX_DIRECT, p_stTrans->p_RdBuf, p_stTrans->dwRdCnt, FALSE,FALSE,pecValue) )		return FALSE;
	}

	return TRUE;
}

//*****************************************************************************
// Description:
//		To end the SPI transmission. It raise CS to finish the communication
//
//  Arguments:
//      n/a
//
//  Return Values:
//      n/a
//*****************************************************************************
BOOL i2c_pio_trans_end(BYTE ucCH)
{
	UINT32 i = 0;

	I2C_CONTROL(ucCH) &= ~(I2C_CRTL_START);
	//set  hat =1 , stop =1.
	I2C_CONTROL(ucCH) |= (I2C_CRTL_STOP | I2C_CRTL_HALT);

	//check bus busy
	for(i = 0; i < 0x10000; i++){
		if( !I2C_IS_BUS_BUSY(ucCH) ){		// 2017/5/17 CM modify
			return TRUE;
		}
	}    

	I2C_PIO_DBG_L("ERROR i2c_pio_trans_end check bus busy too long,STA:%x,CTL:%x\r\n", I2C_STAT(ucCH), I2C_CONTROL(ucCH) );
	return FALSE;
}


//*****************************************************************************
// Description:
//		To write byte of SPI interface
//
//  Arguments:
//      ucData	: write data
//      device	: SPI_CH_0 || SPI_CH_1
//
//  Return Values:
//      n/a
//*****************************************************************************
//void i2c_pio_write_byte(BYTE ucData, BYTE ucCH)
//{
//	spi_pio_trans(ucCH, TX_DIRECT, &ucData, 1);  
//	spi_pio_trans_end();
//}


//*****************************************************************************
// Description:
//		To write word of SPI interface
//
//  Arguments:
//      MSB		: msb of word.
//      LSB		: lsb of word.
//      device	: SPI_CH_0 || SPI_CH_1
//
//  Return Values:
//      n/a
//*****************************************************************************
//void i2c_pio_write_word(BYTE ucMsb, BYTE ucLsb, BYTE ucCH)
//{
//	spi_pio_write_byte(ucCH, ucMsb); 
//	spi_pio_write_byte(ucCH, ucLsb); 
//}


//*****************************************************************************
// Description:
//		To read byte of SPI interface
//
//  Arguments:
//      channel : SPI_CH_0 || SPI_CH_1
//
//  Return Values:
//      data : SPI data
//*****************************************************************************
//BYTE i2c_pio_read_byte(BYTE ucCH)
//{
//    BYTE ucData = 0;
//    
//	spi_pio_trans(ucCH, RX_DIRECT, &ucData, 1); 
//	spi_pio_trans_end();
//    
//    return ucData;
//}


//*****************************************************************************
// Description:
//		To read word of SPI interface
//
//  Arguments:
//      device : SPI_CH_0 || SPI_CH_1
//
//  Return Values:
//      data : word SPI data.
//*****************************************************************************
//WORD i2c_pio_read_word(BYTE ucCH)
//{
//    WORD usData = 0;
//    
//	usData = spi_pio_read_byte(ucCH);		//read MSB
//	usData = spi_pio_read_byte(ucCH) << 8;	//read LSB
//
//    return usData;
//}
//


//*****************************************************************************
//							private function
//*****************************************************************************
//-----------------------------------------------------------------------------
// Description:
//		Transfer SPI by PIO
//
//  Arguments:
//      device		: SPI_CH_0 || SPI_CH_1
//      enumDirect	: TX or RX direction
//      p_buf		: TX or RX buffer
//      dwTransCnt	: transfer byte count
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
BOOL i2c_pio_send_id(BYTE ucCH, UINT8 u8SlaveId, DIRECTION enumDirect)
{
	UINT8	u8Id		= 0;

	//send slave ID
	u8Id				= u8SlaveId | enumDirect;
	I2C_CONTROL(ucCH)	= (I2C_CRTL_INT_EN | I2C_CRTL_ACK | I2C_CRTL_MASTER_MODE);	
    #if __ENABLE_SLEEP_MODE__ // {
    //+SHAWN20151116
    I2C_CONTROL1(ucCH)	= (I2C_CONTROL1(ucCH) & I2C_CRTL1_SLEEP_ENABLE)
                           | ((I2C_DEV_CNT - 1) << 4	
                           | I2C_CRTL1_MDL_EN);
    //-SHAWN20151116
    #else
	I2C_CONTROL1(ucCH)	= (	(I2C_DEV_CNT - 1) << 4	| 
							I2C_CRTL1_MDL_EN			);
    #endif // } 1
	
	if( FALSE == i2c_pio_trans_data(ucCH, TX_DIRECT, &u8Id, 1, TRUE,FALSE,0xFF) )	return FALSE;

	//Clear start
	I2C_CONTROL(ucCH) &= ~(I2C_CRTL_START);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Description:
//		Transfer SPI by PIO
//
//  Arguments:
//      device		: SPI_CH_0 || SPI_CH_1
//      enumDirect	: TX or RX direction
//      p_buf		: TX or RX buffer
//      dwTransCnt	: transfer byte count
//
//  Return Values:
//      n/a
//-----------------------------------------------------------------------------
BOOL i2c_pio_trans_data(BYTE ucCH, DIRECTION enumDirect, BYTE* p_buf, DWORD dwTransCnt, BOOL bIsStart, BOOL bIsPEC,BYTE pecValue)
{
	UINT8	u8Status	= 0;
	UINT32	i			= 0;
	UINT32	u32TimeOut	= 0;

	if((TX_DIRECT == enumDirect) && (dwTransCnt > 2))
	{
		dwTransCnt++;
	}
	if(bIsPEC)
	{
		dwTransCnt++;
	}
        
	for(i = 0; i < dwTransCnt; i++){
		if(TX_DIRECT == enumDirect){
			//if(dwTransCnt > 2)
			if((dwTransCnt > 2)&&(!bIsPEC))
			{
				if( i == 0)
				I2C_DATA_TX(ucCH) = (dwTransCnt-1);
				else 
				I2C_DATA_TX(ucCH) = p_buf[i-1];
			}
			else
			{
				if((bIsPEC)&&(dwTransCnt>3))
				{
					if( i == 0)
						I2C_DATA_TX(ucCH) = (dwTransCnt-2);
					else 
					{
						if(i == (dwTransCnt-1))
						{
							I2C_DATA_TX(ucCH) = pecValue;//SMBCRC8_A;
						}
						else
							I2C_DATA_TX(ucCH) = p_buf[i-1];
					}
					//if( i == 0)
					//	I2C_DATA_TX(ucCH) = (dwTransCnt-2);
					//else 
					//	I2C_DATA_TX(ucCH) = p_buf[i-1];
				}
				else
				{
					if((bIsPEC)&&(i == (dwTransCnt-1)))
						I2C_DATA_TX(ucCH) = SMBCRC8_A;
					else
						I2C_DATA_TX(ucCH) = p_buf[i];
				}
					
			}
		}
		else{			
			//Final byte should be NACK 
			if(i + 1 == dwTransCnt){
				if(FALSE == bIsStart){				
					I2C_CONTROL(ucCH) &= ~I2C_CRTL_ACK;
				}
			}	
		}

		// Clear Interrupt
		if(TRUE == bIsStart)
			I2C_CONTROL(ucCH) |= (I2C_CRTL_START | I2C_CRTL_HALT);
		else	I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;
	
		//wait for transfer finish
		while( !I2C_IS_INT_PENDING(ucCH) ){
			if(++u32TimeOut > 100000){
				printf("I2C trans timeout\r\n");
				return FALSE;
			}
		}
		
		//check error status: arbitration, time out error
		u8Status = I2C_STAT(ucCH);
		if( u8Status & (I2C_STAT_ARB | I2C_STAT_TIMEOUT)  ){
			//do something
			printf("I2C Error: %x\r\n", u8Status);
			return FALSE;
		}
		

		if(RX_DIRECT == enumDirect){
			if(dwTransCnt == sizeSMBRBK)
			{
				if(i==0)
				{
					dwTransCnt = I2C_DATA_RX(ucCH);
					if(dwTransCnt > 0x20)
						dwTransCnt = 0x20;
					tmpACPISMBBCnt = dwTransCnt;
					I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;
					while( !I2C_IS_INT_PENDING(ucCH) ){
						if(++u32TimeOut > 100000){
							printf("I2C trans timeout\r\n");
							return FALSE;
						}
					}
				}
			}
			p_buf[i] = I2C_DATA_RX(ucCH);
		}
		else{
			//check error status: receive nack
			if( !(u8Status & I2C_STAT_ACK_RECV) ){
				//do something
				printf("I2C Error: Lost Ack\r\n");
				return FALSE;		
			}		
		}
	} 

	return TRUE;
}
#ifndef __I2C_MODULE__H
#define __I2C_MODULE__H

//#include "chip_chipregs.h"

#define EC_Register_Base_Address    0x00F00000


//*****************************************************************************
//							define
//*****************************************************************************
#define __ENABLE_SLEEP_MODE__   1

//TC1 version
#define I2C_CH_SWAP10			( ECReg(EC_Register_Base_Address + 0x1c20) )
#define I2C_CH_SWAP32			( ECReg(EC_Register_Base_Address + 0x1c21) )
#define I2C_CH_SWAP54			( ECReg(EC_Register_Base_Address + 0x1c11) )//the same CHSEF

#define	I2C_DATA_RX(x)			( ECReg(0x00 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_PRESCALE(x)			( ECReg(0x01 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_HSPR(x)				( ECReg(0x02 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_STAT_BASE(x)		( 0x03 + EC_Register_Base_Address + u8I2cBase[x] )
#define	I2C_STAT(x)				( ECReg(0x03 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_DHTR(x)				( ECReg(0x04 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_TIME_OUT(x)			( ECReg(0x05 + EC_Register_Base_Address + u8I2cBase[x]) )
#define I2C_ID_ADD(x)			( ECReg(0x06 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_DATA_TX(x)			( ECReg(0x08 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_CONTROL(x)			( ECReg(0x09 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_CONTROL1(x)			( ECReg(0x0A + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_BYTE_CNT_H(x)		( ECReg(0x0B + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_BYTE_CNT_L(x)		( ECReg(0x0C + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_IRQ_ST(x)			( ECReg(0x0D + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_SLV_NUM_H(x)		( ECReg(0x10 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_SLV_NUM_L(x)		( ECReg(0x11 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_NACK_ST(x)			( ECReg(0x13 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_TIME_BUF(x)			( ECReg(0x14 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_TO_ARB_ST(x)		( ECReg(0x18 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_ERR_ST(x)			( ECReg(0x19 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_FST_BASE(x)			( 0x1B + EC_Register_Base_Address + u8I2cBase[x] )
#define	I2C_FST(x)				( ECReg(0x1B + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_ERROR_MASK(x)		( ECReg(0x1D + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_MODE_SEL(x)			( ECReg(0x1D + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_CSR(x)				( ECReg(0x1F + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_CTRL2(x)			( ECReg(0x20 + EC_Register_Base_Address + u8I2cBase[x]) )
#define	I2C_WCSR(x, y)			( ECReg(0x22 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_RAMHA(x, y)			( ECReg(0x23 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_RAMLA(x, y)			( ECReg(0x24 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_CMDHA(x, y)			( ECReg(0x25 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_CMDLA(x, y)			( ECReg(0x26 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_LNGRH(x, y)			( ECReg(0x27 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_LNGRL(x, y)			( ECReg(0x28 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )
#define	I2C_LNGSTH(x, y)		( ECReg(0x29 + EC_Register_Base_Address + u8I2cBase[x] + 9*(y)) )

//Slave
#define I2C_ID_ADD2(x)			( ECReg(0x1f + EC_Register_Base_Address + u8I2cBase[x]) )
#define I2C_ID_ADD3(x)			( ECReg(0x22 + EC_Register_Base_Address + u8I2cBase[x]) )
#define I2C_ID_ADD4(x)			( ECReg(0x2b + EC_Register_Base_Address + u8I2cBase[x]) )


// Define Register I2C Transfer Status
#define I2C_STAT_TRANS_IN_PROGRESS      BIT7
#define I2C_STAT_ADDR_MATCH             BIT6
#define I2C_STAT_BUS_BUSY               BIT5
#define I2C_STAT_ARB                    BIT4
#define I2C_STAT_STOP_DETECT            BIT4
#define I2C_STAT_TIMEOUT                BIT3
#define I2C_STAT_DIR_M_READ             BIT2
#define I2C_STAT_INT_PENDING            BIT1
#define I2C_STAT_ACK_RECV               BIT0

// Define Register I2C Control
#define I2C_CRTL_RX_MODE                BIT7
#define I2C_CRTL_INT_EN                 BIT6
#define I2C_CRTL_MASTER_MODE            BIT5
#define I2C_CRTL_SLAVE_MODE				0
#define I2C_CRTL_SOFT_RESET             BIT4
#define I2C_CRTL_ACK                    BIT3
#define I2C_CRTL_START                  BIT2
#define I2C_CRTL_STOP                   BIT1
#define I2C_CRTL_HALT                   BIT0

// Define Register I2C Control1
#define I2C_CRTL1_CMDQ_ENABLE           BIT7
#define I2C_CRTL1_CMD_NUM2              BIT6
#define I2C_CRTL1_CMD_NUM1              BIT5
#define I2C_CRTL1_CMD_NUM0              BIT4
#define I2C_CRTL1_SLEEP_ENABLE          BIT2
#define I2C_CRTL1_MDL_EN                BIT1

// Define Register I2C Control2
#define I2C_CRTL2_M4_INT                BIT7
#define I2C_CRTL2_M3_INT                BIT6
#define I2C_CRTL2_M2_INT                BIT5
#define I2C_CRTL2_M1_INT                BIT4
#define I2C_CRTL2_I2C_SLAVE             BIT3
#define I2C_CRTL2_SMB_SLAVE             BIT2
#define I2C_CRTL2_RSVD                  BIT1
#define I2C_CRTL2_ID_SCL_HLD            BIT0

// Define Register I2C Control4
#define I2C_CRTL4_ONE_SHOT_M4           BIT7
#define I2C_CRTL4_ONE_SHOT_M3           BIT6
#define I2C_CRTL4_ONE_SHOT_M2           BIT5
#define I2C_CRTL4_ONE_SHOT_M1           BIT4
#define I2C_CRTL4_R_M4                  BIT3
#define I2C_CRTL4_R_M3                  BIT2
#define I2C_CRTL4_R_M2                  BIT1
#define I2C_CRTL4_R_M1                  BIT0

// Define Register I2C Data hold time
#define I2C_DHTR_SOFT_RESET             BIT7

//Define Register Arbiter status
#define I2C_TO_SCL_ENABLE			BIT7
#define I2C_TO_SDA_ENABLE			BIT6
#define I2C_TO_SCL					BIT5
#define I2C_TO_SDA					BIT4
#define I2C_TO_ARB_ENABLE			BIT3
#define I2C_TO_ARB_LOST				BIT2
#define I2C_TO_ARB_CLEAR			BIT0
#define I2C_TO_ARB_STATUS			(BIT0 | BIT1)

//error status register , 0x0e . which  master detect slave error , then see register 0x13 
#define I2C_ERRST_CLEAR         0x0F
#define I2C_ERRST_MASTER4       0x08
#define I2C_ERRST_MASTER3       0x04
#define I2C_ERRST_MASTER2       0x02
#define I2C_ERRST_MASTER1       0x01

//IRQ status
#define I2C_IRS_ST_BUF0_EMPTY			BIT7
#define I2C_IRS_ST_BUF1_EMPTY			BIT6
#define I2C_IRS_ST_MASTER_FINISH		BIT5
#define I2C_IRS_ST_HOLD_CNT				BIT4
#define I2C_IRS_ST_SLAVE_ID_TX			BIT3
#define I2C_IRS_ST_SLAVE_ID_RX			BIT2
#define I2C_IRS_ST_SLAVE_ID_RX_FLAG		BIT2
#define I2C_IRS_ST_SLAVE_RX_DATA		BIT1
#define I2C_IRS_ST_SLAVE_FINISH			BIT0

//Define Register Nack status
#define I2C_NACK_ST_CLEAR				BIT7

// MAS_CMD define , the first master cmd setting
#define MAS_CMD_MC          0x80  //set high speed i2c bus
#define MAS_CMD_READ        0x40  //set bit6 ,r = 1 
#define MAS_CMD_WRITE       0x00  //set bit6 ,w = 0
#define MAS_CMD_STOP        0x20  //set stop bit enable
#define MAS_CMD_END         0x10  //set all end commmand bit enable
#define MAS_CMD_LA          0x08  //set master read last data ack bit .usually set 0
#define MAS_CMD_NUM         0x07  //set cmd read write data number bit 2-0  
#define MAS_CMD_NUM_SF      0x03  //  


#define I2C_GET_STAT(ucCH)				I2C_STAT(ucCH)
#define I2C_GET_ERR_STAT(ucCH)			I2C_ERR_ST(ucCH)

/* Check status */
#define I2C_IS_BUS_BUSY(ucCH)			(IS_MASK_SET(I2C_STAT(ucCH), I2C_STAT_BUS_BUSY)	> 0 ? TRUE:FALSE)
#define I2C_IS_INT_PENDING(ucCH)		(IS_MASK_SET(I2C_STAT(ucCH), I2C_STAT_INT_PENDING) > 0 ? TRUE:FALSE)

//IRQ status
//#define I2C_IS_STOP(ucCH)				(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_FINISH)	> 0 ? TRUE:FALSE)
//#define I2C_IS_WRITE(ucCH)				(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_ID_TX) > 0 ? TRUE:FALSE)
//#define I2C_IS_READ(ucCH)				(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_ID_RX) > 0 ? TRUE:FALSE)
//#define I2C_IS_READ_DATA(ucCH)			(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_RX_DATA) > 0 ? TRUE:FALSE)
#define I2C_IS_STOP(ucCH)				(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_FINISH)	> 0 ? TRUE:FALSE)
#define I2C_IS_WRITE(ucCH)				(IS_MASK_SET(I2C_STAT(ucCH), I2C_STAT_DIR_M_READ) == 0 ? TRUE:FALSE)
#define I2C_IS_READ(ucCH)				(IS_MASK_SET(I2C_STAT(ucCH), I2C_STAT_DIR_M_READ) > 0 ? TRUE:FALSE)
#define I2C_IS_READ_DATA(ucCH)			(IS_MASK_SET(I2C_IRQ_ST(ucCH), I2C_IRS_ST_SLAVE_RX_DATA) > 0 ? TRUE:FALSE)

//clear IRQ interrupt
#define I2C_CLEAR_STOP(ucCH)			do{I2C_IRQ_ST(ucCH) = I2C_IRS_ST_SLAVE_FINISH;}while(0)
#define I2C_CLEAR_WRITE(ucCH)			do{I2C_IRQ_ST(ucCH) = I2C_IRS_ST_SLAVE_ID_TX;}while(0)
#define I2C_CLEAR_READ(ucCH)			do{I2C_IRQ_ST(ucCH) = I2C_IRS_ST_SLAVE_ID_RX;}while(0)
#define I2C_CLEAR_READ_DATA(ucCH)		do{I2C_IRQ_ST(ucCH) = I2C_IRS_ST_SLAVE_RX_DATA;}while(0)

/* clear interrupt */
#define I2C_CLEAR_INTERRUPT(ucCH)		do{I2C_CONTROL(ucCH) |= I2C_CRTL_HALT;}while(0)


//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************
typedef enum i2c_channel
{	
	I2C_CH_D,
	I2C_CH_E,
	I2C_CH_F,
	I2C_CH_BOTH,
	I2C_CH_CNT = I2C_CH_BOTH,
}I2C_CH;

typedef enum i2c_device
{	
	I2C_DEV_1,
	I2C_DEV_2,
	I2C_DEV_3,
	I2C_DEV_4,
	I2C_DEV_CNT,
}I2C_DEV;


typedef enum direction
{
	TX_DIRECT,
	RX_DIRECT,
}DIRECTION;


typedef enum  iO_trans_type
{
	IO_TRANS_TYPE_PIO,
	IO_TRANS_TYPE_CMD_Q_ONE_SHOT,
	IO_TRANS_TYPE_CMD_Q_AUTO_MODE,
	IO_TRANS_TYPE_CNT,
}IO_TRANS_TYPE;


typedef struct i2c_cnt
{
	DWORD dwIntTotal;
	DWORD dwFull0;
	DWORD dwFull1;
	DWORD dwCmdQEnd0;
	DWORD dwCmdQEnd1;
	DWORD dwTransEnd;
	DWORD dwSclTimeout;
	DWORD dwSdaTimeout;
	DWORD dwCmdQTimeout;
	DWORD dwArbiter;
	DWORD dwNack;
	DWORD dwCmdQFail;
	DWORD dwTimeout;
}I2C_CNT;

//transfer management structure
typedef struct iO_trans_mgr
{
	IO_TRANS_TYPE	enumTransType;	//SPI_TRANS_TYPE_PIO or SPI_TRANS_TYPE_CMD_Q
	BYTE			u8Dev;			//Device number(I2C)
	DWORD			u8SlaveId;		//Slave ID
	BYTE*			p_WtBuf;		//write command buffer
	DWORD			dwWtCnt;		//write command count
	BYTE*			p_WtDataBuf;	//write data buffer
	DWORD			dwWtDataCnt;	//write data count
	BYTE*			p_RdBuf;		//read data buffer
	DWORD			dwRdCnt;		//read data count
	BYTE			ucChkMAsk;		//Check status Mask
	BYTE			ucChkValue;		//Check status value
	BYTE			ucErrStat;		//Error status
	BOOL			bIsChkStat;		//Check status or not
	BOOL			bIsDma;			//DMA or not
	BOOL			bIsEnd;			//End or not. the finial command in CMDQ need to set TURE
}IO_TRANS;


//*****************************************************************************
//							Public variable
//*****************************************************************************
extern const UINT16 u8I2cBase[I2C_CH_CNT];
extern I2C_CNT stI2cCnt;


//*****************************************************************************
//							Public function
//*****************************************************************************
UINT8 i2c_get_err_status(UINT8 ucCH, UINT8 ucDev);

#endif
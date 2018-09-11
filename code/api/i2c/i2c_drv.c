//#include <stdlib.h>
#include "i2c_drv.h"
#include "chip_type.h"
#include "chip_chipregs.h"
#include "rom.h"

#include "i2c.h"
#include "i2c_cmd_q.h"
#include "i2c_hid.h"
#include "hid_hal.h"
#include "hal.h"
#include "api_gpio.h"
#include "debug_print.h"
#include "core_memory.h"
#include "direct_map.h"



//*****************************************************************************
//							declaration of Define and Marco
//*****************************************************************************
#define I2C_TEMP_TO_HIDE				1

// Define & Struct
#define I2C_DEBUG_PORT4(_X_)			(gpSHDInfo->pI2CDbgMsg[3] = (_X_))

#define I2C_MAX_FIFO_SIZE				64
#define I2C_CMDQ_QUICK_HEADER_SIZE		6

#define I2C_CMD_SRAM_BASE_ADDR			((UINT32)(gpSHDInfo->mCmdBuf))    //0x81200
#define I2C_CMD_SRAM_ADDR_TRANS(x)		 ((UINT32)(((UINT32)(x) & 0x00001FFF) + 0xD000))
#define I2C_CMD_SRAM_INIT()				 //memset((UINT8*)I2C_CMD_SRAM_BASE_ADDR, 0x00, 512)

//Check status
#define SET_DRV_READY(p)				SET_MASK((p)->bInit, BIT0)
#define SET_I2C_CMD_READY(p)			SET_MASK((p)->bInit, BIT1)

#define CLR_DRV_READY(p)				CLEAR_MASK((p)->bInit, BIT0)
#define CLR_AUTO_CMD_READY(p)			CLEAR_MASK((p)->bInit, BIT1)

#define IS_AUTO_CMD_READY(p)			IS_MASK_SET((p)->bInit, BIT1)

#define READ_SRAM_DATA(pCmd)                                  \
	do{ \
		/*printk("(%Xh, %Xh, %Xh)\r\n", (pCmd)->pBuf, (pCmd)->u32DataReg,(pCmd)->u8Count);*/ \
		memcpy((pCmd)->pBuf, (UINT8*)((pCmd)->u32DataReg), (pCmd)->u8Count); \
	}while(0)

//TODO
#define GET_I2C_CMD_SLAVE_ID(n, ucCH)  ECReg(I2C_SLAVE_ID##n(ucCH))		

//TODO
#define I2C_START_STOP_CMD(c, d, con, c_mask, d_mask)   \
	do{                                     \
		(c)  = OUTPUT;                      \
		(d)  = OUTPUT;                      \
		(con) |= ((c_mask)|(d_mask));       \
		(con) &= ~(d_mask); /* SDA low */   \
		(con) &= ~(c_mask); /* SCL low */   \
		(con) |= (c_mask);  /* SCL high */  \
		(con) |= (d_mask);  /* SDA high*/   \
		(c)  = ALT;                         \
		(d)  = ALT;                         \
	}while(0)


//For debug
#if 0
#define PRINT_DRV(pDrv)                                          \
	do{                                                          \
		printk("I2C_DRV:\r\n");                                  \
		printk(" u32StatReg   : %Xh\r\n", pDrv->u32StatReg);     \
		printk(" u32FinReg    : %Xh\r\n", pDrv->u32FinReg);      \
		printk(" bInit        : %d\r\n" , pDrv->bInit);          \
		printk(" u8CH         : %d\r\n" , pDrv->u8CH);           \
		printk(" u8AllDoneMask: %Xh\r\n", pDrv->u8AllDoneMask);  \
		printk(" bFIFOMode    : %d\r\n" , pDrv->bFIFOMode);      \
	}while(0);
#else
#define PRINT_DRV(pDrv)
#endif


//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************

typedef struct ResetI2Cs
{
    uchar_8   *I2CPin0      ;   // clock pin
    uchar_8   *I2CPin1      ;   // data pin
    uchar_8   *GPIODataReg  ;
    uchar_8   GPIOPinSetting;
    uchar_8   GPIOPinClock  ;
    uchar_8   GPIOPinData   ;
} sResetI2Cs;


//*****************************************************************************
//							declaration of Public variable
//*****************************************************************************
I2C_DATA volatile UINT8  gucSensorBusStatus = 0;         // Sensor I2C Bus busy status
I2C_DATA volatile UINT8  gucHIDBusStatus = 0;            // HID over I2C Bus busy status
I2C_BSS  volatile UINT8 gucI2CINTFlag[3];


//*****************************************************************************
//							declaration of Private variable
//*****************************************************************************
HID_DATA static T_I2C_DRV gtI2CDrv[I2C_DEV_CNT] = {{0}};

const sResetI2Cs asResetI2Cs[]=
{
    { &GPCRH1, &GPCRH2, &GPDRH, (BIT1+BIT2), BIT1, BIT2},
#if I2C_SWAP_TEST
    { &GPCRA4, &GPCRA5, &GPDRA, (BIT4+BIT5), BIT4, BIT5},	//TODO	handle swap
    { &GPCRE0, &GPCRE7, &GPDRE, (BIT0+BIT7), BIT0, BIT7},
#else
    { &GPCRE0, &GPCRE7, &GPDRE, (BIT0+BIT7), BIT0, BIT7},
    { &GPCRA4, &GPCRA5, &GPDRA, (BIT4+BIT5), BIT4, BIT5},
#endif
}; 


//*****************************************************************************
//							declaration of Private function
//*****************************************************************************
//static BOOL i2c_drv_wait_bus_idle(UINT8 u8Ch);

#ifdef EC_FEATURE_ONLY
void  i2c_interrupt_clear_isrx(UINT8 u8Ch);
#endif

//*****************************************************************************
//							Public function
//*****************************************************************************
void I2C_CODE_L i2c_drv_module_init()
{
	UINT8	i		= 0;
	UINT8*	p_buf	= NULL;

    i2c_init();

	for(i = 0; i < I2C_CH_CNT;i++){
		p_buf = (UINT8*)(I2C_CMDQ_TX_ADDR + i*I2C_CMDQ_TX_CNT);
		i2c_set_cmd_q_buf(i, p_buf, I2C_CMDQ_TX_CNT, p_buf, I2C_CMDQ_TX_CNT);
	}

	// Set SMB Clock = PLL / 6 = 8M
	//if( (PLLFREQR & 0xf) == PLL_48M )	SCDCR2 = ((SCDCR2 & 0xF0) | 0x05);	// PLL 48M, SMB 8M
	//else								SCDCR2 = ((SCDCR2 & 0xF0) | 0x0b);	// PLL 96M, SMB 8M

	// default SMB Clock = PLL 48 / 2 = 24M.
	// If enable USB, should set SMB Clock = PLL 96 / 4 = 24M
	if( (PLLFREQR & 0xf) != PLL_48M )	SCDCR2 = ((SCDCR2 & 0xF0) | 0x03);
    else                                SCDCR2 = ((SCDCR2 & 0xF0) | 0x01);
}



//****************************************************************************************
// Function name: i2c_drv_get_cmd_sram_addr()
// Description:
//   Get SRAM address by I2C cmd.
// Arguments:
//   UINT8 ucCH: channel 0:D, 1:E, 2:F.
//   UINT8 u8CmdIdx: i2c command index
// Return Values:
//   SRAM address
//
// Note:
//   none
//****************************************************************************************
UINT32 I2C_CODE i2c_drv_get_cmd_sram_addr(UINT8 u8CH, UINT8 u8CmdIdx)
{
	UINT32 u32BaseAddr;

	if(u8CH == I2C_F)
		u32BaseAddr = I2C_CMD_SRAM_BASE_ADDR + ((I2C_D_SENSOR_MAX_NUM + I2C_E_SENSOR_MAX_NUM) * I2C_CMD_MAX_LEN);

	else if(u8CH == I2C_E)
		u32BaseAddr = I2C_CMD_SRAM_BASE_ADDR + (I2C_D_SENSOR_MAX_NUM * I2C_CMD_MAX_LEN);

	else
		u32BaseAddr = I2C_CMD_SRAM_BASE_ADDR;

	return (u32BaseAddr + (u8CmdIdx * I2C_CMD_MAX_LEN));
}

//****************************************************************************************
// Function name: i2c_drv_ch_init()
// Description:
//   I2C drv initial
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1:E, 2:F)
// Return Values:
//   NULL: FALSE
//   Other: I2C Drv
// Note
//   none
//****************************************************************************************
HANDLE I2C_CODE_L i2c_drv_init(UINT8 u8CH)
{
	if(u8CH < I2C_DEV_CNT)
	{
		i2c_ch_init_gpio(u8CH, FALSE);        
		i2c_ch_reset(u8CH);
        
		gucI2CINTFlag[u8CH] = 1;
		gtI2CDrv[u8CH].u8CH = u8CH;

		SET_DRV_READY(gtI2CDrv + u8CH);
		return (gtI2CDrv + u8CH);
	}

	return NULL;
}


//****************************************************************************************
// Function name: i2c_interrupt_mask()
// Description:
//   開關i2c的interrupt mask
// Arguments:
//   UINT8 ucCH: channel 0:D, 1:E, 2:F.
//   UINT8 ucEnable: 1:enable, 0:disable
// Return Values:
//   void
// Note:
//   none
//****************************************************************************************
void I2C_CODE_L i2c_interrupt_mask(UINT8 ucCH, UINT8 ucEnable) 
{
    if(ucCH < 3)
    {        
        if(ucEnable)
        {
            i2c_drv_irq_init(ucCH);
        }
        else
        {
            i2c_drv_irq_disable(ucCH);
        }
    }
}

//****************************************************************************************
// Function name: i2c_drv_irq_init()
// Description:
//   I2C interrupt mask enable.
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1:E, 2:F)
// Return Values:
//   none
// Note
//   none
//****************************************************************************************
void I2C_CODE_L i2c_drv_irq_init(UINT8 u8CH)
{
#ifdef __ENABLE_I2C_INTERRUPT__
	switch(u8CH)
	{
		case I2C_D:
			SET_MASK_EXT(IER0, BIT4);
			break;

		case I2C_E:
			SET_MASK_EXT(IER19, BIT0);
			break;

		case I2C_F:
			SET_MASK_EXT(IER19, BIT1);
			break;
	}
#endif
}

//****************************************************************************************
// Function name: i2c_drv_irq_disable()
// Description:
//   I2C disable interrupt mask
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1:E, 2:F)
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE_L i2c_drv_irq_disable(UINT8 u8CH)
{
#ifdef __ENABLE_I2C_INTERRUPT__
	switch(u8CH)
	{
		case I2C_D:
			CLEAR_MASK_EXT(IER0, BIT4);
			break;

		case I2C_E:
			CLEAR_MASK_EXT(IER19, BIT0);
			break;

		case I2C_F:
			CLEAR_MASK_EXT(IER19, BIT1);
			break;
	} 
#endif
}

#if !I2C_TEMP_TO_HIDE
//****************************************************************************************
// Function name:
//   i2c_drv_reset_cmd()
// Description:
//   send "start" & "stop" i2c cmd
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1:E, 2:F)
// Return Values:
//   n/a
// Note:
//   n/a
//****************************************************************************************
void I2C_CODE_L i2c_drv_reset_cmd(UINT8 u8CH)
{
	if(u8CH < I2C_DEV_CNT)
	{
		switch(u8CH)
		{
			case I2C_D:
				I2C_START_STOP_CMD(GPCRH1, GPCRH2, GPDRH, 0x02, 0x04);
				GCR2   |= BIT5;     // Enable Ch3
				break;
                
			case I2C_E:
				I2C_START_STOP_CMD(GPCRE0, GPCRE7, GPDRE, 0x01, 0x80);
				PMER1  |= BIT0;     // Enable Ch4
				break;
                
			case I2C_F:
				if(HID_I2C_SWAP)
				{
					I2C_START_STOP_CMD(GPCRB3, GPCRB4, GPDRB, 0x08, 0x10);
				}
				else
				{
					I2C_START_STOP_CMD(GPCRA4, GPCRA5, GPDRA, 0x10, 0x20);
				}
				PMER1  |= BIT1;     // Enable Ch5
				break;
		}

		i2c_ch_reset(u8CH);
	}
}
#endif


//****************************************************************************************
// Function name: i2c_drv_trans()
// Description:
//   I2C one shot write
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1:E, 2:F)
//   UINT8 u8SlaveAddr: I2C Slave address
//   IO_TRANS_TYPE enumTransType: I2C transfer type, PIO or One-shot
//   UINT8 *pTxBuf:		I2C write cmd or addr
//   UINT8 u8TxCnt:		I2C write cmd or addr count
//   UINT8 *pTxDataBuf: I2C data buf
//   UINT8 u8TxDataCnt: I2C data count
//   UINT8 *pRxBuf:		I2C rx buf
//   UINT8 u8RxCnt:		I2C rx count
// Return Values:
//   0: TRUE
//   -1: FALSE
// Note
//   使用前Auto mode 需停止, 如要再使用auto mode, auto mode的內容需重新設定
//****************************************************************************************
INT8 I2C_CODE i2c_drv_trans(UINT8 u8CH, UINT8 u8SlaveAddr, IO_TRANS_TYPE enumTransType, UINT8 *pTxBuf, UINT8 u8TxCnt, 
							UINT8 *pTxDataBuf, UINT8 u8TxDataCnt, UINT8 *pRxBuf, UINT8 u8RxCnt)
{
	BOOL		bResult	= FALSE;
	IO_TRANS	stTrans;
	memset(&stTrans, 0, sizeof(IO_TRANS));

	if(FALSE == i2c_drv_wait_bus_idle(u8CH) ){
		//TODO
		gucI2CSensorErrStatus = i2c_get_err_status(u8CH, I2C_DEV_1);
		return -1;
	}

	stTrans.enumTransType	= enumTransType;
	stTrans.bIsEnd			= TRUE;
	stTrans.u8SlaveId		= u8SlaveAddr;
	stTrans.u8Dev			= I2C_DEV_1;
	i2c_set_trans_struc(&stTrans, pTxBuf, u8TxCnt, pTxDataBuf, u8TxDataCnt, pRxBuf, u8RxCnt);

#ifdef EC_FEATURE_ONLY
    SENSOR_ISR_SERVICE_FLAG_OS = 0;
    SENSOR_ISR_SERVICE_FLAG = 0;
#endif


#ifdef __ENABLE_I2C_INTERRUPT__
	gucSensorBusStatus = 1;
	i2c_interrupt_mask(u8CH,0);
#endif

	//transmit cmd
	bResult = i2c_trans(u8CH, &stTrans,FALSE,0xFF);

	if(bResult == FALSE)	I2C_DEBUG_PORT4(0x03);

#ifdef __ENABLE_I2C_INTERRUPT__
    #ifdef EC_FEATURE_ONLY
    i2c_interrupt_clear_isrx(u8CH);
    #endif

	gucSensorBusStatus = 0;
	i2c_interrupt_mask(u8CH,1);
#endif
    
	gucI2CSensorErrStatus = stTrans.ucErrStat;
	
	if( TRUE == bResult)	return 0;
	else					return -1;
}


//****************************************************************************************
// Function name: i2c_drv_cmd_read_start()
// Description:
//   Fill auto mode command content, and fire
// Arguments:
//   HANDLE hDrv: I2C drv information
//   P_I2C_CMD ppCmd[]: auto mode command
//   UINT8 u8CmdCnt: auto mode command count
//    BOOL bFIFOMode: 1: Use I2C FIFO mode, 0: use I2C SRAM mode
// Return Values:
//   0: TRUE
// Note:
//   Remember to call sensor_init() before calling this function!!!!!!!!
//****************************************************************************************
//todo
int I2C_CODE_L i2c_drv_cmd_read_start(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8CmdCnt, BOOL bFIFOMode)	
{	
	UINT32		i		= 0;
	P_I2C_DRV	p_Drv	= (P_I2C_DRV)hDrv;

    if(u8CmdCnt == 0)
        return -1;

	I2C_CMD_SRAM_INIT();
	if(IS_DRV_READY(p_Drv))
	{
		//i2c_reset(SENSOR_I2C_CH);
		// Set Private Member
		p_Drv->u32StatReg     = I2C_STAT_BASE(p_Drv->u8CH);
		p_Drv->u32FinReg      = I2C_FST_BASE(p_Drv->u8CH);

		// Init Interrupt Flag
        #ifdef __ENABLE_I2C_INTERRUPT__
		gucI2CINTFlag[p_Drv->u8CH] = 1;
        #endif

		i2c_ch_reset(p_Drv->u8CH);

		/* Initial Auto CMD Period(only works for 24MHz & 1~690ms) */
		I2C_SET_CLOCK_PERIOD(p_Drv->u8CH, 0x20);

		// Init Cmd
		for(i = 0; i < u8CmdCnt; i++){			
			i2c_drv_set_cmd(p_Drv, ppCmd, i);
		}

		SET_I2C_CMD_READY(p_Drv);
	}

	PRINT_DRV(p_Drv);

	return 0;
}

//****************************************************************************************
// Function name: i2c_drv_cmd_read_stop()
// Description:
//   Auto mode stop
// Arguments:
//   HANDLE hDrv: I2C drv information
// Return Values:
//   void
// Note:
//   none
//****************************************************************************************
void I2C_CODE_L i2c_drv_cmd_read_stop(HANDLE hDrv)
{
	P_I2C_DRV pDrv = (P_I2C_DRV)hDrv;

	if(IS_DRV_READY(pDrv))
	{
        #ifndef DEF_SENSOR_ONE_SHOT_MODE // {         
		i2c_stop_trans();
        #endif  // } DEF_SENSOR_ONE_SHOT_MODE
		CLR_AUTO_CMD_READY(pDrv);
	}
}


//****************************************************************************************
// Function name: i2c_drv_read_sram_data()
// Description:
//   I2C read data from SRAM when using I2C auto mode
// Arguments:
//   P_I2C_CMD pCmd: auto mode command content
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_read_sram_data(P_I2C_CMD pCmd)
{
	READ_SRAM_DATA(pCmd);
}

#ifdef DEF_SENSOR_ONE_SHOT_MODE // {
//****************************************************************************************
// Function name: i2c_drv_set_cmd()
// Description:
//  Set I2C auto mode content
// Arguments:
//   HANDLE hDrv: I2C drv.
//   P_I2C_CMD pCmd: auto mode command content.
//   UINT8 u8CmdIdx: Auto mode command index
// Return Values:
//   none
// Note
//   none
//****************************************************************************************
BOOL i2c_drv_set_cmd(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8Dev)
{
	UINT8		u8CH		= 0;
	UINT32		u32Addr		= 0;
	UINT8*		p_u8Header	= NULL;
	P_I2C_CMD	p_Cmd		= NULL;
	P_I2C_DRV	p_Drv		= (P_I2C_DRV)hDrv;

	if(NULL == hDrv || NULL == ppCmd)	return FALSE;

	u8CH		= p_Drv->u8CH;
	p_Cmd		= ppCmd[u8Dev];
	p_u8Header	= (UINT8*)(I2C_FAST_CMD_ADDR + u8Dev * I2C_CMDQ_QUICK_HEADER_SIZE);
	u32Addr		= i2c_drv_get_cmd_sram_addr(u8CH, u8Dev);

	p_u8Header[0] = p_Cmd->u8SlaveAddr;
	p_u8Header[1] = 0;
	p_u8Header[2] = 0;
	p_u8Header[3] = p_Cmd->u8Addr;
	p_u8Header[4] = ( (p_Cmd->u8Count - 1) & 0x7 ) | 0xf0;
	p_u8Header[5] = (p_Cmd->u8Count - 1) >> 3;

	if(p_Cmd->u16Period > 80)	I2C_SET_WAIT_TIME(u8CH, u8Dev, (p_Cmd->u16Period*24000)/((0xff+1)*256));
	else						I2C_SET_WAIT_TIME(u8CH, u8Dev, (p_Cmd->u16Period*24000)/((0x1f+1)*256));

#ifdef DEF_SENSOR_ONE_SHOT_MODE
	if( FALSE == i2c_cmd_q_set_reg(u8CH, u8Dev, (UINT32)p_u8Header, u32Addr, I2C_CMD_MAX_LEN, IO_TRANS_TYPE_CMD_Q_ONE_SHOT) )	
		return FALSE;	
#else
	if( FALSE == i2c_cmd_q_set_reg(u8CH, u8Dev, (UINT32)p_u8Header, u32Addr, I2C_CMD_MAX_LEN, IO_TRANS_TYPE_CMD_Q_AUTO_MODE) )	
		return FALSE;	
#endif
	
	p_Cmd->u32DataReg = u32Addr;
	return TRUE;
}

volatile unsigned char gucCurSensorFireIdx = 0;
//****************************************************************************************
// Function name: i2c_drv_fire_one_shot()
// Description:
//   I2C one shot mode fire
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1: E, 2:F)
//   UINT8 u8FireIdx:  the index of the command to fire
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_fire_one_shot(UINT8 u8CH, UINT8 u8Dev)
{
	UINT8 ucSlaveID  = 0;

    #ifndef EC_FEATURE_ONLY
	volatile UINT8 ucTimeout = 0;
    #endif

	ucSlaveID = *(UINT8*)(I2C_FAST_CMD_ADDR + u8Dev * I2C_CMDQ_QUICK_HEADER_SIZE);

    /* rtos */
    #ifndef EC_FEATURE_ONLY
	//Check already set device
	if(ucSlaveID != 0x00)
	{
		gucSensorBusStatus = 1;
		gucCurSensorFireIdx++;

		i2c_interrupt_mask(u8CH, 0);
		//start to get sensor data
		i2c_fire(u8CH, u8Dev);

		//wait for finish
		while( I2C_IS_INT_PENDING(u8CH) == FALSE )
		{
			hal_sleep(1);
			ucTimeout++;
			if(ucTimeout >= 50)
			{
                #ifndef DEF_SENSOR_ONE_SHOT_MODE
				sensor_get_data_stop();
                #endif // } DEF_SENSOR_ONE_SHOT_MODE
				gu8HIDStatus = HID_STATUS_SENSOR_TIMEOUT;
				break;
			}
		}
		i2c_interrupt_mask(u8CH, 1);
		//
		// RTOS only
		//
		gucCurSensorFireIdx = 0;
        #ifndef EC_FEATURE_ONLY
		gucSensorBusStatus = 0;
        #endif
	}
    #else
    if( ucSlaveID != 0x00)
    {
    	gucSensorBusStatus = 1;
    	gucCurSensorFireIdx++;    	
    	i2c_interrupt_mask(u8CH, 1);
		i2c_fire(u8CH, u8Dev);
        gucCurSensorFireIdx = 0;
    }
    #endif
}


#if !I2C_TEMP_TO_HIDE
//****************************************************************************************
// Function name: i2c_drv_get_bus_status()
// Description:
//   I2C one shot mode fire
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1: E, 2:F)
//
// Return Values:
//   0: Bus busy, 1: bus free
// Note
//   none
//****************************************************************************************
UINT8 I2C_CODE i2c_drv_get_bus_status(UINT8 u8CH)
{
	switch(u8CH)
	{
		case I2C_D:
			if((GPDRH & BIT1) == 0 || (GPDRH & BIT2) == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}

		case I2C_E:
			if((GPDRE & BIT0) == 0 || (GPDRE & BIT7) == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
            
		case I2C_F:
			if(HID_I2C_SWAP)
			{
				if((GPDRB & BIT3) == 0 || (GPDRB & BIT4) == 0)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if((GPDRA & BIT4) == 0 || (GPDRA & BIT5) == 0)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
	}
	return 0;
}
#endif

#endif // } DEF_SENSOR_ONE_SHOT_MODE

//****************************************************************************************
// Function name: i2c_drv_poweroff_handler()
// Description:
//  power off handler
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1: E, 2:F)
//
// Return Values:
//   0: Bus busy, 1: bus free
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_poweroff_handler(UINT8 u8CH)
{
	i2c_interrupt_mask(u8CH, 0);  // Mask interrupt

	if(u8CH == I2C_D)
	{
		GPCRH1 = INPUT;
		GPCRH2 = INPUT;
		GCR2  &= ~BIT5;
	}
	else if(u8CH == I2C_E)
	{
#if I2C_SWAP_TEST
		GPCRA4 = INPUT;
		GPCRA5 = INPUT;
		PMER1 &= ~BIT1;
#else
		GPCRE0 = INPUT;
		GPCRE7 = INPUT;
		PMER1 &= ~BIT0;
#endif
	}
	else if(u8CH == I2C_F)
	{
#if I2C_SWAP_TEST
		GPCRE0 = INPUT;
		GPCRE7 = INPUT;
		PMER1 &= ~BIT0;
#else
		if(HID_I2C_SWAP)
		{
			GPCRB3 = INPUT;
			GPCRB4 = INPUT;
		}
		else
		{
			GPCRA4 = INPUT;
			GPCRA5 = INPUT;
			PMER1 &= ~BIT1;
		}
#endif
	}
}

//****************************************************************************************
// Function name: i2c_drv_sensor_power_off_handler()
// Description:
// i2c_drv_sensor_power_off_handler
// Arguments:
//  void
//
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_sensor_power_off_handler(void)
{
	gu8HIDStatus = HID_STATUS_POWEROFF;
}

//****************************************************************************************
// Function name: i2c_drv_sensor_power_on_handler()
// Description:
// i2c_drv_sensor_power_on_handler
// Arguments:
//  void
//
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_sensor_power_on_handler(void)
{
	i2c_ch_reset(DEF_SENSOR_I2C_CH);
	i2c_drv_init(DEF_SENSOR_I2C_CH);
}


//****************************************************************************************
// Function name: i2c_drv_channel_initial_handler()
// Description:
// i2c_drv_sensor_power_on_handler
// Arguments:
//  void
//
// Return Values:
//   void
// Note
//   none
//****************************************************************************************
void I2C_CODE i2c_drv_channel_initial_handler(BYTE channel)
{
	i2c_drv_module_init();
		
	i2c_ch_reset(channel);
	i2c_drv_init(channel);
}

#if defined(EC_FEATURE_ONLY_SUPPORT_FUSION) && !defined(DEF_SENSOR_ONE_SHOT_MODE) // {
int I2C_CODE pure_ec_i2c_drv_cmd_read(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8CmdCnt)
{
	P_I2C_DRV pDrv = (P_I2C_DRV)hDrv;
	UINT8 l_data_ready;

	if(IS_AUTO_CMD_READY(pDrv))
	{
		l_data_ready = 1;
		gucI2CINTFlag[(pDrv->u8CH)] = 1;
		ECReg(pDrv->u32FinReg) |= 0xF0;


		if(l_data_ready)
		{
			// get cmd0 data
			READ_SRAM_DATA(ppCmd[0]);

			if(--u8CmdCnt)
			{
				// get cmd1 data
				READ_SRAM_DATA(ppCmd[1]);

				if(--u8CmdCnt)
				{
					// get cmd2 data
					READ_SRAM_DATA(ppCmd[2]);

					if(--u8CmdCnt)
					{
						// get cmd3 data
						READ_SRAM_DATA(ppCmd[3]);
					}
				}
			}

			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION


//****************************************************************************************
// Function name: i2c_drv_wait_bus_idle()
//
// Description:
//     Check bus is idle or not
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1: E, 2:F)
// Return Values:
//   TRUE : Bus is idle
//   FALSE: Bus is busy
// Note:
//   NULL.
//****************************************************************************************
BOOL I2C_CODE i2c_drv_wait_bus_idle(UINT8 u8CH)
{
	UINT16 u16BusBusy = 1500;

	while(u16BusBusy--){
		if( FALSE == I2C_IS_BUS_BUSY(u8CH) )	return TRUE;
		else									WNCKR = 0x00;	// Delay 15.26 us	
    }

	return FALSE;
}



//****************************************************************************************
// Function name: i2c_check_slave_exist()
//
// Description:
//     Check slave exist or not
// Arguments:
//   UINT8 u8CH: I2C channel(0:D, 1: E, 2:F)
//   UINT8 ucSlave: slave id
// Return Values:
//   TRUE : Exist
//   FALSE: Not Exist
// Note:
//   NULL.
//****************************************************************************************
BOOL I2C_CODE i2c_check_slave_exist(UINT8 ucCH, UINT8 ucSlave)
{
	UINT8 ucData = 0;

    if( -1 == i2c_drv_trans(ucCH, ucSlave, IO_TRANS_TYPE_CMD_Q_ONE_SHOT, NULL, 0, NULL, 0, &ucData, 1) )
    {
        //printk("%02X not exist.\r\n", ucSlave);
        return FALSE;    
    }
    else
    {
        //printk("%02X exist.\r\n", ucSlave);
        return TRUE;    
    }
}


void I2C_CODE_L i2c_channel_reset(UINT8 p_channel)
{
    ulong_32 l_delay;

    vPortEnterCritical();

    *asResetI2Cs[p_channel].I2CPin0 = 0x40;
    CLEAR_MASK(*asResetI2Cs[p_channel].GPIODataReg, asResetI2Cs[p_channel].GPIOPinClock);
    WNCKR = 0x00;
    WNCKR = 0x00;
    
    I2C_CONTROL(p_channel) = BIT4+BIT0;
    I2C_CONTROL(p_channel) = 0x00;

    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;

    //
    // output mode
    //
    *asResetI2Cs[p_channel].I2CPin0 = 0x40;
    *asResetI2Cs[p_channel].I2CPin1 = 0x40;

    CLEAR_MASK(*asResetI2Cs[p_channel].GPIODataReg, asResetI2Cs[p_channel].GPIOPinSetting);

    vPortExitCritical();

    //
    // To delay 25ms
    //
    for(l_delay=0x00; l_delay < 1667; l_delay++)
    {
        //
        // Delay 15.26 us
        //
        WNCKR = 0x00;
    }

    vPortEnterCritical();
    
    SET_MASK(*asResetI2Cs[p_channel].GPIODataReg, asResetI2Cs[p_channel].GPIOPinClock);
    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;
    SET_MASK(*asResetI2Cs[p_channel].GPIODataReg, asResetI2Cs[p_channel].GPIOPinData);
    
    *asResetI2Cs[p_channel].I2CPin0 = 0x00;
    *asResetI2Cs[p_channel].I2CPin1 = 0x00;

    vPortExitCritical();

    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;
    WNCKR = 0x00;
}


#ifdef EC_FEATURE_ONLY
//*****************************************************************************
//
// W/C isrx status of i2c channel D,E,and F
//
//  parameter :
//      u8Ch : 0 ~ 2
//
//  return :
//      none
//
//  note :
//
//*****************************************************************************
void I2C_CODE_L i2c_interrupt_clear_isrx(UINT8 u8Ch)
{
    switch(u8Ch)
    {
        case 0:
            ISR0 = BIT4;
            break;
	
        case 1:
            ISR19 = BIT0;
            break;
            
        case 2: 
            ISR19 = BIT1;
            break;
    }
}
#endif

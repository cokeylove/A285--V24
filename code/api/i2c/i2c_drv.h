#ifndef __I2C_DRV_H__
#define __I2C_DRV_H__

#include "../../chip/chip_type.h"
#include "../../oem/Project_H/INCLUDE/oem_project.h"
#include "i2c_module.h"



//*****************************************************************************
//							define
//*****************************************************************************
//interrupt
//#define __ENABLE_I2C_PIO_MODE__
#ifndef __ENABLE_I2C_PIO_MODE__
   #define __ENABLE_I2C_INTERRUPT__
#endif

#define I2C_CMD_SET(cmd, sa, addr, cnt, period, buf)   \
        do{                                            \
            (cmd).u8SlaveAddr = (sa);                  \
            (cmd).u8Addr      = (UINT8)(addr);         \
            (cmd).u8Count     = (cnt);                 \
            (cmd).u16Period   = (period);              \
            (cmd).pBuf        = (buf);                 \
        }while(0)

//I2C Transfer for API
#define I2C_TRANS(pDrv, SlaveId, TransType, pTxBuf, u8TxCnt, pTxDataBuf, u8TxDataCnt, pRxBuf, u8RxCnt)						\
	(																														\
		IS_DRV_READY(pDrv) ?																								\
		i2c_drv_trans( pDrv->u8CH, SlaveId, TransType, pTxBuf, u8TxCnt, pTxDataBuf, u8TxDataCnt, pRxBuf, u8RxCnt) :			\
		-1																													\
	)

//Check Device init or not
#define IS_DRV_READY(p)		IS_MASK_SET((p)->bInit, BIT0)

//API, In 8380, block_xx are PIO. In 8390 are one-shot mode. Keep old name for 8380.
#define i2c_drv_data_read(pHandle, SlaveId, u8Addr, pBuf, u8Cnt)															\
	I2C_TRANS( ((P_I2C_DRV)pHandle), SlaveId, IO_TRANS_TYPE_CMD_Q_ONE_SHOT, &u8Addr, sizeof(u8Addr), NULL, 0, pBuf, u8Cnt )
#define i2c_drv_data_write(pHandle, SlaveId, u8Addr, pBuf, u8Cnt)															\
	I2C_TRANS( ((P_I2C_DRV)pHandle), SlaveId, IO_TRANS_TYPE_CMD_Q_ONE_SHOT, &u8Addr, sizeof(u8Addr), pBuf, u8Cnt, NULL, 0 )
#define i2c_drv_data_block_read(pHandle, SlaveId, pTxBuf, u8TxCnt, pRxBuf, u8RxCnt)											\
	I2C_TRANS( ((P_I2C_DRV)pHandle), SlaveId, IO_TRANS_TYPE_CMD_Q_ONE_SHOT, pTxBuf, u8TxCnt, NULL, 0, pRxBuf, u8RxCnt  )
#define i2c_drv_data_block_write(pHandle, SlaveId, pTxBuf, u8TxCnt)															\
	I2C_TRANS( ((P_I2C_DRV)pHandle), SlaveId, IO_TRANS_TYPE_CMD_Q_ONE_SHOT, pTxBuf, u8TxCnt, NULL, 0, NULL, 0  )


//*****************************************************************************
//							 declaration of struct and enum
//*****************************************************************************
typedef struct _T_I2C_CMD_
{
   UINT32 u32DataReg;
   UINT8 *pBuf;
   UINT16 u16Period;
   UINT8  u8SlaveAddr;
   UINT8  u8Addr;
   UINT8  u8Count;
}T_I2C_CMD, *P_I2C_CMD;

enum _I2C_CHANNEL_
{
    I2C_D = 0,
    I2C_E,
    I2C_F
};


typedef struct I2CSlaveRegStruct
{
	unsigned char  ucCH;
	unsigned char  ucSlave;
	unsigned char  ucDataNum;
	unsigned char  ucI2CMode;
	unsigned char  ucDMAMode;
	unsigned char  ucSlaveAdd2;
	unsigned char  ucDMADataLen;
	unsigned short usSramAddDst;
	unsigned short usSramAddSrc;	
} i2cSlaveReg;


typedef struct _T_I2C_DRV_
{
	UINT32 u32StatReg;
	UINT32 u32FinReg;
	BOOL   bInit;    // bit0: driver, bit1: auto cmd
	UINT8  u8CH;
	UINT8  u8AllDoneMask;
	BOOL   bFIFOMode;
} T_I2C_DRV, *P_I2C_DRV;


//*****************************************************************************
//							Public variable
//*****************************************************************************
extern volatile UINT8  gucSensorBusStatus;         // Sensor I2C Bus busy status
extern volatile UINT8  gucHIDBusStatus;            // HID over I2C Bus busy status


//*****************************************************************************
//							Public function
//*****************************************************************************
void i2c_drv_module_init();
HANDLE i2c_drv_init(UINT8 u8CH);
void i2c_drv_irq_init(UINT8 u8CH);
void i2c_drv_irq_disable(UINT8 u8CH);
void i2c_drv_reset_cmd(UINT8 u8CH);
void i2c_channel_reset(UINT8 p_channel);
void i2c_interrupt_mask(UINT8 ucCH, UINT8 ucEnable);
int i2c_drv_cmd_read_start(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8CmdCnt, BOOL bFIFOMode);
void i2c_drv_cmd_read_stop(HANDLE hDrv);
BOOL i2c_drv_check_last(HANDLE hDrv);
void i2c_drv_read_sram_data(P_I2C_CMD pCmd);
UINT32 i2c_drv_get_cmd_sram_addr(UINT8 u8CH, UINT8 u8CmdIdx);
UINT8 i2c_drv_get_bus_status(UINT8 u8CH);
BOOL i2c_check_slave_exist(UINT8 ucCH, UINT8 ucSlave);
extern BOOL i2c_drv_wait_bus_idle(UINT8 u8Ch);

INT8  i2c_drv_trans(UINT8 u8CH, UINT8 u8SlaveAddr, IO_TRANS_TYPE enumTransType, 
					UINT8 *pTxBuf, UINT8 u8TxCnt, UINT8 *pTxDataBuf, UINT8 u8TxDataCnt, UINT8 *pRxBuf, UINT8 u8RxCnt);

void  i2c_drv_sensor_power_off_handler(void);
void  i2c_drv_sensor_power_on_handler(void);
extern void  i2c_drv_channel_initial_handler(BYTE channel);


void  i2c_drv_poweroff_handler(UINT8 u8CH);

#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION // {
int pure_ec_i2c_drv_cmd_read(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8CmdCnt);
#endif // } EC_FEATURE_ONLY_SUPPORT_FUSION

#ifdef DEF_SENSOR_ONE_SHOT_MODE  // {   
BOOL i2c_drv_set_cmd(HANDLE hDrv, P_I2C_CMD ppCmd[], UINT8 u8CmdIdx);
void i2c_drv_fire_one_shot(UINT8 ucCH, UINT8 ucFireIdx);

#endif // } DEF_SENSOR_ONE_SHOT_MODE

#endif //__I2C_DRV_H__

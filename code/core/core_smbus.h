/* 
 * ****************************************************************************
 *
 *  core_smbus.h
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/13
 *
 *      Author: ITE00580, Dino Li
 *
 * ****************************************************************************
 */

#ifndef CORE_SMBUS_H
#define CORE_SMBUS_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define SMbusCh1	    0x00    /* SMbus channel A index */
#define SMbusCh2	    0x01	/* SMbus channel B index */
#define SMbusCh3	    0x02	/* SMbus channel C index */
#define SMbusCh4	    0x03    /* SMbus channel D index */
#define SMbusCh5	    0x04	/* SMbus channel E index */
#define SMbusCh6	    0x05	/* SMbus channel F index */


#define	SMbusWQ		    0x80	/* Write Quick command */
#define	SMbusRQ		    0x00	/* Read Quick command */
#define	SMbusSBC	    0x84	/* Send Byte command */
#define	SMbusRBC	    0x04	/* Receive Byte command */
#define	SMbusWB		    0x88	/* Write byte command */
#define SMbusRB		    0x08	/* Read byte command */
#define	SMbusWW 	    0x8C	/* Write word command */
#define	SMbusRW		    0x0C	/* Read  word command */
#define	SMbusWBK	    0x94	/* Write block command */
#define	SMbusRBK	    0x14	/* Read block command */

#define SMBus_Busy      0x01    /* SMBus clock pin or data pin is low */
#define SMBus_Free      0x00    /* SMBus clock pin and data pin are both high */
#define SMBusPin_GPIO   0x00    /* SMBus interface is GPIO function */
#define SMBusPin_ALT    0x01    /* SMBus interface is ALT function */
#define SMBus_CanUse    0x01    /* SMBus interface can be used. */
#define SMBus_CanNotUse 0x00    /* SMBus interface can't be used. */
#define SMBus_AccessType_Fucn   0x01
#define SMBus_AccessType_Table  0x00
#define SMBus_NeedService       0x01
#define SMBus_ClearService      0x00
#define SMBus_NeedPEC           0x01
#define SMBus_NoPEC             0x00

extern volatile UINT8 gucI2CSensorErrStatus ;


/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void ResetSMBus(BYTE Channel);
extern BYTE bRWSMBus(
    BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,BYTE *Var,BYTE PECSupport);
extern BYTE bRSMBusBlock(
    BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,BYTE *Var ,BYTE i2cSize);
extern BYTE bWSMBusBlock(
    BYTE Channel,BYTE Protocol,BYTE Addr,BYTE Comd,BYTE
    *Var,BYTE ByteCont,BYTE PECsupport);
extern BYTE bSMBusSendByte(BYTE Channel,BYTE Addr,BYTE SData);
extern BYTE bSMBusReceiveByte(BYTE Channel,BYTE Addr,BYTE *Var);
extern void CalcCRC8(BYTE sourcebyte);
extern BYTE CheckSMBusFree(BYTE channel);
extern BYTE CheckSMBusInterfaceIsALT(BYTE channel);
extern BYTE CheckSMBusInterfaceCanbeUse(BYTE channel, BYTE accesstype);
extern void Core_Init_SMBus(void);
extern BYTE CheckSMCLK2PinSwitch(BYTE p_channel);

extern BYTE I2C_Read(BYTE p_channel, BYTE p_address, WORD p_length,
                        BYTE *p_data_buffer, BYTE P_first_word_is_read_length);
extern BYTE I2C_Write(BYTE p_channel, BYTE p_address, BYTE p_length,
                        BYTE *p_data_buffer);
extern BYTE I2C_Write_to_Read(BYTE p_channel, BYTE p_address,
                        BYTE p_wlength, BYTE *p_wdata_buffer,
                        WORD p_rlength, BYTE *p_rdata_buffer,
                        BYTE P_first_word_is_read_length);

extern BYTE SMBus_DEF_Transfer(BYTE channel, BYTE prtcl, BYTE smbus_address, BYTE smbus_cmd,BYTE *buf,BYTE count,BYTE pecSupport,BYTE pecValue);


/* 
 * ****************************************************************************
 * structure
 * ****************************************************************************
 */
typedef struct SMBusStruct
{
	uchar_8	*SMBusCTL	;	 
	uchar_8	*SMBusADR	;
	uchar_8	*SMBusCMD	;
	uchar_8	*SMBusSTA	;
	uchar_8	*SMBusData0	;
	uchar_8	*SMBusData1	;	
	uchar_8	*SMBusBData	;	
	uchar_8	*SMBusIER	;	
    uchar_8	*SMBusISR	;
	uchar_8	IERFlag		;	
    uchar_8	*SMBusPEC	;
} sSMBus;

typedef struct ResetSMBusS
{
	uchar_8	*SMBusSTA	;
	uchar_8	*SMBusCTL2	;
	uchar_8	*SMBusPinCTL;
	uchar_8	*SMBusPin0	;   /* clock pin */
	uchar_8	*SMBusPin1	;   /* data pin */
	uchar_8	*GPIOReg	;
	uchar_8	GPIOData	;
    uchar_8 *recoverCunt;
} sResetSMBusS;

typedef struct SMBChannel
{
  BYTE  SMBAddr    ;
  BYTE  SMBChannel ;
} sSMBChannel;


extern const sSMBus asSMBus[];
extern const sResetSMBusS asResetSMBusS[];

#define bRead_I2C_NULL                       0x00
#endif 


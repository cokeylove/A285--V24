/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_ow.h
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

#ifndef API_OW_H
#define API_OW_H

/* 
 * ****************************************************************************
 * Software OW F.W. OW pin definition [example]
 * ****************************************************************************
 */
#define OneWireInput 	        GPCRF2 = INPUT;
#define OneWireOutput 	        GPCRF2 = OUTPUT;
#define OneWireHigh		        SET_MASK(GPDRF, BIT2);
#define OneWireLow		        CLEAR_MASK(GPDRF, BIT2);
#define OneWireStatusLow        IS_MASK_CLEAR(GPDRF, BIT2)
#define OneWireStatusHigh       IS_MASK_SET(GPDRF, BIT2)

/* 
 * ****************************************************************************
 * H.W. OW (0x00F0_2Axxh) H.W. OW Registers
 * ****************************************************************************
 */
/* One Wire Data Register */
#define	OW_DATA		    ECReg(EC_Register_Base_Address+0x2A00)
/* One Wire Control Register */
#define	OW_CTRL		    ECReg(EC_Register_Base_Address+0x2A01)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define OW_RD           BIT(2)  /* One-Wire Read */
#define OW_WR           BIT(1)  /* One-Wire Write */
#define OW_RST          BIT(0)  /* One-Wire Reset */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* One Wire Mode Register */
#define	OW_MOD		    ECReg(EC_Register_Base_Address+0x2A02)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define OBMOD           BIT(1)  /* One Bit Mode */
#define OD              BIT(0)  /* OverDrive */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* One Wire Status Register */
#define	OW_STS		    ECReg(EC_Register_Base_Address+0x2A03)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define PDR             BIT(3)  /* Presence Detect Result */
#define OWHIGH_ERR      BIT(2)  /* One-Wire High Error */
#define OWLOW_ERR       BIT(1)  /* One-Wire Low Error */
#define OW_DONE         BIT(0)  /* One-Wire Done */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* One Wire Interrupt Enable Register */
#define	OW_INTEN		ECReg(EC_Register_Base_Address+0x2A04)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define OWHIGH_EN       BIT(2)  /* One-Wire High Error Interrupt Enable */
#define OWLOW_EN        BIT(1)  /* One-Wire Low Error Interrupt Enable */
#define OW_DONE_EN      BIT(0)  /* One-Wire Done Interrupt Enable */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* One Wire High bit of Reset Low and Reset High Register */
#define	OW_RSTL_RSTH_H  ECReg(EC_Register_Base_Address+0x2A05)

/* One Wire Reset Low Register */
#define	OW_RST_LOW	    ECReg(EC_Register_Base_Address+0x2A06)

/* One Wire Master Sample Present Register */
#define	OW_MSP		    ECReg(EC_Register_Base_Address+0x2A07)

/* One Wire Reset High Register */
#define	OW_RSTH		    ECReg(EC_Register_Base_Address+0x2A08)

/* One Wire Time Slot Register */
#define	OW_SLOT		    ECReg(EC_Register_Base_Address+0x2A09)

/* One Wire Write 1 Low Register */
#define	OW_W1L		    ECReg(EC_Register_Base_Address+0x2A0A)

/* One Wire Write 0 Low Register */
#define	OW_W0L		    ECReg(EC_Register_Base_Address+0x2A0B)

/* One Wire Master Sample Read Bit Register */
#define	OW_MSR		    ECReg(EC_Register_Base_Address+0x2A0C)

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern BYTE SW_OW_Device_Reset(void);
extern void SW_OW_Device_WriteByte(BYTE PsidByte);
extern BYTE SW_OW_Device_ReadByte(void);
extern BYTE HW_OW_Device_Reset(void);
extern BYTE HW_OW_Device_WriteByte(BYTE writedata);
extern BYTE HW_OW_Device_ReadByte(void);

#endif


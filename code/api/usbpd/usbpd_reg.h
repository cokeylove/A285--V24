
#ifndef __USBPD_REG__H
#define __USBPD_REG__H

#define	USBPD_BASE(Port)	(EC_Register_Base_Address + 0x3700 + (Port)*0x100)		// General Control Register


//USBPD addr
#define	USBPD_GCR(Port)		ECReg(USBPD_BASE(Port) + 0x0)	// General Control Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_SW_RESET_BIT				BIT7
#define USBPD_TYPE_C_DETECT_RESET		BIT6
#define USBPD_BMC_PHY					BIT4
#define USBPD_AUTO_SEND_SW_RESET		BIT3
#define USBPD_AUTO_SEND_HW_RESET		BIT2
#define USBPD_SNIFFER_MODE				BIT1
#define USBPD_GLOBAL_ENABLE				BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	PDMSR(Port)			ECReg(USBPD_BASE(Port) + 0x01)	// PD Mode Selection Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SOPPP_ENABLE		BIT7
#define SOPP_ENABLE			BIT6
#define SOP_ENABLE			BIT5
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	PDCSR(Port)			ECReg(USBPD_BASE(Port) + 0x02)	// PD Control Setting Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	VCONN_ENABLE		BIT2
#define	ACTIVE_MODE_ENABLE	BIT1
#define	VBUS_STAT_5V_ENABLE	BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	PEPDRSR(Port)		ECReg(USBPD_BASE(Port) + 0x03)	// PE Power/Data Role status Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_CC_BUSY				BIT4
#define USBPD_ROLE_DATA				BIT1
#define USBPD_ROLE_POWER			BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	CCGCR(Port)			ECReg(USBPD_BASE(Port) + 0x04)	// CC General Configuare Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_DISABLE_CC			BIT4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	CCCSR(Port)			ECReg(USBPD_BASE(Port) + 0x05)	// CC Channel Setting Register 
#define	CCPSR(Port)			ECReg(USBPD_BASE(Port) + 0x06)	// CC Pad Setting Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_CC2_DISCONNECT_REG			BIT6
#define USBPD_DISCONNECT_POWER_CC2			BIT5
#define USBPD_CC1_DISCONNECT_REG			BIT2
#define USBPD_DISCONNECT_POWER_CC1			BIT1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define	DFPVDR(Port)		ECReg(USBPD_BASE(Port) + 0x08)	// DFP Voltage Detection Result Register
#define	UFPVDR(Port)		ECReg(USBPD_BASE(Port) + 0x09)	// UFP Voltage Detection Result Register
#define	CCADCR(Port)		ECReg(USBPD_BASE(Port) + 0x0C)	// CC Auto Detect Configuration Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define AUTO_CC_VBUS_REMOVE_NOTIFY		BIT7
#define AUTO_CC_DISABLE_DURING_DETACH	BIT2
#define AUTO_CC_DETACH_DETECT_ENABLE	BIT1
#define AUTO_CC_ATTACH_DETECT_ENABLE	BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	CCADIR(Port)		ECReg(USBPD_BASE(Port) + 0x0D)	// CC Auto Detect Interrupt Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define AUTO_CC_DETECT_SOURCE_CUR_CHANGE_ISR	BIT2
#define AUTO_CC_DETECT_TYPE_C_DETACH_ISR		BIT1
#define AUTO_CC_DETECT_TYPE_C_ATTACH_ISR		BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	CCADRR(Port)		ECReg(USBPD_BASE(Port) + 0x0E)	// CC Auto Detect Result Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_VCONN_DETECT_NEED_ENABLE		BIT7
#define USBPD_AUTO_DETECT_CC				BIT6
#define USBPD_AUTO_DETECT_DEBUG_ATTACH		BIT3
#define USBPD_AUTO_DETECT_AUDIO_ATTACH		BIT2
#define USBPD_AUTO_DETECT_DFP_ATTACH		BIT1
#define USBPD_AUTO_DETECT_UFP_ATTACH		BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	CCADIMR(Port)		ECReg(USBPD_BASE(Port) + 0x0F)	// CC Auto Detect Interrupt Mask Register
#define	TSR0_OFFSET(Port)	(USBPD_BASE(Port) + 0x10)			// Timer Status Register 0
#define	TSR0(Port)			ECReg(USBPD_BASE(Port) + 0x10)	// Timer Status Register 0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_TIMEOUT_BIST_CON_MODE_BIT					BIT3
#define USBPD_TIMEOUT_SOURCE_CAP_BIT					BIT2
#define USBPD_TIMEOUT_SEND_RESP_BIT						BIT1
#define USBPD_TIMEOUT_CRC_RX_BIT						BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	TSR1(Port)			ECReg(USBPD_BASE(Port) + 0x11)	// Timer Status Register 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_TIMEOUT_VDM_WAIT_ENTRY_OR_EXIT_BIT		BIT7
#define USBPD_TIMEOUT_SOURCE_TRANS_PS_HARD_RESET_BIT	BIT6
#define USBPD_TIMEOUT_SINK_WAIT_CAP_BIT					BIT5
#define USBPD_TIMEOUT_VCON_ON_BIT						BIT4
#define USBPD_TIMEOUT_NO_RESP_BIT						BIT3
#define USBPD_TIMEOUT_PS_SOURCE_ON_BIT					BIT2
#define USBPD_TIMEOUT_PS_SOURCE_OFF_BIT					BIT1
#define USBPD_TIMEOUT_PS_TRANS_BIT						BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	TIMR0(Port)			ECReg(USBPD_BASE(Port) + 0x12)	// Timer Interrupt Mask Register 0
#define	TIMR1(Port)			ECReg(USBPD_BASE(Port) + 0x13)	// Timer Interrupt Mask Register 1
#define	USBPD_ISR(Port)		ECReg(USBPD_BASE(Port) + 0x14)	// Interrupt Status Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_TYPE_C_DETECT				BIT7
#define USBPD_CABLE_RESET_DETECT		BIT6
#define USBPD_HARD_RESET_DETECT			BIT5
#define USBPD_MSG_RX_DONE				BIT4
#define USBPD_AUTO_SOFT_RESET_TX_DONE	BIT3
#define USBPD_HARD_RESET_TX_DONE		BIT2
#define USBPD_MSG_TX_DONE				BIT1
#define USBPD_TIMER_TIMEOUT				BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	IMR(Port)			ECReg(USBPD_BASE(Port) + 0x15)	// Interrupt Mask Register
#define	MTCR(Port)			ECReg(USBPD_BASE(Port) + 0x18)	// Message Transmit Control Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_SW_RESET_TX_STAT			BIT3
#define USBPD_TX_BUSY_STAT				BIT2
#define USBPD_TX_ERR_STAT				BIT1
#define USBPD_TX_START					BIT0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	MTSR0(Port)			ECReg(USBPD_BASE(Port) + 0x19)	// Message Transmit Setting Register
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_CABLE_ENABLE				BIT7
#define USBPD_SEND_HW_RESET				BIT6
#define USBPD_SEND_BIST_MODE_2			BIT5
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	MTSR1(Port)			ECReg(USBPD_BASE(Port) + 0x1A)	// Message Transmit Setting Register
#define	VDMMCSR(Port)		ECReg(USBPD_BASE(Port) + 0x1B)	// VDM Message Command Setting Register
#define	MRSR(Port)			ECReg(USBPD_BASE(Port) + 0x1C)	// Message Receive Status Register 
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBPD_RX_MSG_VALID				BIT0
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	PEFSMR(Port)		ECReg(USBPD_BASE(Port) + 0x1D)	// Policy Engine FSM Register
#define	PES0R(Port)			ECReg(USBPD_BASE(Port) + 0x1E)	// Policy Engine Status 0 Register
#define	PES1R(Port)			ECReg(USBPD_BASE(Port) + 0x1F)	// Policy Engine Status 1 Register
#define	TDO_BASE(Port)		(USBPD_BASE(Port) + 0x20)			// Transmit Data Object Base
#define	TDO0R0(Port)		ECReg(USBPD_BASE(Port) + 0x20)	// Transmit Data Object 0 Register 0
#define	TDO0R1(Port)		ECReg(USBPD_BASE(Port) + 0x21)	// Transmit Data Object 0 Register 1
#define	TDO0R2(Port)		ECReg(USBPD_BASE(Port) + 0x22)	// Transmit Data Object 0 Register 2
#define	TDO0R3(Port)		ECReg(USBPD_BASE(Port) + 0x23)	// Transmit Data Object 0 Register 3
#define	TDO1R0(Port)		ECReg(USBPD_BASE(Port) + 0x24)	// Transmit Data Object 1 Register 0
#define	TDO1R1(Port)		ECReg(USBPD_BASE(Port) + 0x25)	// Transmit Data Object 1 Register 1
#define	TDO1R2(Port)		ECReg(USBPD_BASE(Port) + 0x26)	// Transmit Data Object 1 Register 2
#define	TDO1R3(Port)		ECReg(USBPD_BASE(Port) + 0x27)	// Transmit Data Object 1 Register 3
#define	TDO2R0(Port)		ECReg(USBPD_BASE(Port) + 0x28)	// Transmit Data Object 2 Register 0
#define	TDO2R1(Port)		ECReg(USBPD_BASE(Port) + 0x29)	// Transmit Data Object 2 Register 1
#define	TDO2R2(Port)		ECReg(USBPD_BASE(Port) + 0x2A)	// Transmit Data Object 2 Register 2
#define	TDO2R3(Port)		ECReg(USBPD_BASE(Port) + 0x2B)	// Transmit Data Object 2 Register 3
#define	TDO3R0(Port)		ECReg(USBPD_BASE(Port) + 0x2C)	// Transmit Data Object 3 Register 0
#define	TDO3R1(Port)		ECReg(USBPD_BASE(Port) + 0x2D)	// Transmit Data Object 3 Register 1
#define	TDO3R2(Port)		ECReg(USBPD_BASE(Port) + 0x2E)	// Transmit Data Object 3 Register 2
#define	TDO3R3(Port)		ECReg(USBPD_BASE(Port) + 0x2F)	// Transmit Data Object 3 Register 3
#define	TDO4R0(Port)		ECReg(USBPD_BASE(Port) + 0x30)	// Transmit Data Object 4 Register 0
#define	TDO4R1(Port)		ECReg(USBPD_BASE(Port) + 0x31)	// Transmit Data Object 4 Register 1
#define	TDO4R2(Port)		ECReg(USBPD_BASE(Port) + 0x32)	// Transmit Data Object 4 Register 2
#define	TDO4R3(Port)		ECReg(USBPD_BASE(Port) + 0x33)	// Transmit Data Object 4 Register 3
#define	TDO5R0(Port)		ECReg(USBPD_BASE(Port) + 0x34)	// Transmit Data Object 5 Register 0
#define	TDO5R1(Port)		ECReg(USBPD_BASE(Port) + 0x35)	// Transmit Data Object 5 Register 1
#define	TDO5R2(Port)		ECReg(USBPD_BASE(Port) + 0x36)	// Transmit Data Object 5 Register 2
#define	TDO5R3(Port)		ECReg(USBPD_BASE(Port) + 0x37)	// Transmit Data Object 5 Register 3
#define	TDO6R0(Port)		ECReg(USBPD_BASE(Port) + 0x38)	// Transmit Data Object 6 Register 0
#define	TDO6R1(Port)		ECReg(USBPD_BASE(Port) + 0x39)	// Transmit Data Object 6 Register 1
#define	TDO6R2(Port)		ECReg(USBPD_BASE(Port) + 0x3A)	// Transmit Data Object 6 Register 2
#define	TDO6R3(Port)		ECReg(USBPD_BASE(Port) + 0x3B)	// Transmit Data Object 6 Register 3
#define	AGTMH_BASE(Port)	(USBPD_BASE(Port) + 0x3C)	// Auto Genetate Transmit Message Header BASE
#define	AGTMHLR(Port)		ECReg(USBPD_BASE(Port) + 0x3C)	// Auto Genetate Transmit Message Header Low Register
#define	AGTMHHR(Port)		ECReg(USBPD_BASE(Port) + 0x3D)	// Auto Generate Transmit Message Header High Register
#define	TMHLR(Port)			ECReg(USBPD_BASE(Port) + 0x3E)	// Transmit Message Header Data Low Register
#define	TMHHR(Port)			ECReg(USBPD_BASE(Port) + 0x3F)	// Transmit Message Header Data High Register
#define	RDO_BASE(Port)		(USBPD_BASE(Port) + 0x40)			// Receive Data Object Base
#define	RDO0R0(Port)		ECReg(USBPD_BASE(Port) + 0x40)	// Receive Data Object 0 Register 0
#define	RDO0R1(Port)		ECReg(USBPD_BASE(Port) + 0x41)	// Receive Data Object 0 Register 1
#define	RDO0R2(Port)		ECReg(USBPD_BASE(Port) + 0x42)	// Receive Data Object 0 Register 2
#define	RDO0R3(Port)		ECReg(USBPD_BASE(Port) + 0x43)	// Receive Data Object 0 Register 3
#define	RDO1R0(Port)		ECReg(USBPD_BASE(Port) + 0x44)	// Receive Data Object 1 Register 0
#define	RDO1R1(Port)		ECReg(USBPD_BASE(Port) + 0x45)	// Receive Data Object 1 Register 1
#define	RDO1R2(Port)		ECReg(USBPD_BASE(Port) + 0x46)	// Receive Data Object 1 Register 2
#define	RDO1R3(Port)		ECReg(USBPD_BASE(Port) + 0x47)	// Receive Data Object 1 Register 3
#define	RDO2R0(Port)		ECReg(USBPD_BASE(Port) + 0x48)	// Receive Data Object 2 Register 0
#define	RDO2R1(Port)		ECReg(USBPD_BASE(Port) + 0x49)	// Receive Data Object 2 Register 1
#define	RDO2R2(Port)		ECReg(USBPD_BASE(Port) + 0x4A)	// Receive Data Object 2 Register 2
#define	RDO2R3(Port)		ECReg(USBPD_BASE(Port) + 0x4B)	// Receive Data Object 2 Register 3
#define	RDO3R0(Port)		ECReg(USBPD_BASE(Port) + 0x4C)	// Receive Data Object 3 Register 0
#define	RDO3R1(Port)		ECReg(USBPD_BASE(Port) + 0x4D)	// Receive Data Object 3 Register 1
#define	RDO3R2(Port)		ECReg(USBPD_BASE(Port) + 0x4E)	// Receive Data Object 3 Register 2
#define	RDO3R3(Port)		ECReg(USBPD_BASE(Port) + 0x4F)	// Receive Data Object 3 Register 3
#define	RDO4R0(Port)		ECReg(USBPD_BASE(Port) + 0x50)	// Receive Data Object 4 Register 0
#define	RDO4R1(Port)		ECReg(USBPD_BASE(Port) + 0x51)	// Receive Data Object 4 Register 1
#define	RDO4R2(Port)		ECReg(USBPD_BASE(Port) + 0x52)	// Receive Data Object 4 Register 2
#define	RDO4R3(Port)		ECReg(USBPD_BASE(Port) + 0x53)	// Receive Data Object 4 Register 3
#define	RDO5R0(Port)		ECReg(USBPD_BASE(Port) + 0x54)	// Receive Data Object 5 Register 0
#define	RDO5R1(Port)		ECReg(USBPD_BASE(Port) + 0x55)	// Receive Data Object 5 Register 1
#define	RDO5R2(Port)		ECReg(USBPD_BASE(Port) + 0x56)	// Receive Data Object 5 Register 2
#define	RDO5R3(Port)		ECReg(USBPD_BASE(Port) + 0x57)	// Receive Data Object 5 Register 3
#define	RDO6R0(Port)		ECReg(USBPD_BASE(Port) + 0x58)	// Receive Data Object 6 Register 0
#define	RDO6R1(Port)		ECReg(USBPD_BASE(Port) + 0x59)	// Receive Data Object 6 Register 1
#define	RDO6R2(Port)		ECReg(USBPD_BASE(Port) + 0x5A)	// Receive Data Object 6 Register 2
#define	RDO6R3(Port)		ECReg(USBPD_BASE(Port) + 0x5B)	// Receive Data Object 6 Register 3
#define	RMH_BASE(Port)		(USBPD_BASE(Port) + 0x5E)		// Receive Message Header Data BASE
#define	RMHLR(Port)			ECReg(USBPD_BASE(Port) + 0x5E)	// Receive Message Header Data Low Register
#define	RMHHR(Port)			ECReg(USBPD_BASE(Port) + 0x5F)	// Receive Message Header Data High Register
#define	CCPSR0(Port)		ECReg(USBPD_BASE(Port) + 0x60)	// CC Parameter Setting Register 0
#define	CCPSR1(Port)		ECReg(USBPD_BASE(Port) + 0x61)	// CC Parameter Setting Register 1
#define	CCPSR2(Port)		ECReg(USBPD_BASE(Port) + 0x62)	// CC Parameter Setting Register 2
#define	CCPSR3(Port)		ECReg(USBPD_BASE(Port) + 0x63)	// CC Parameter Setting Register 3
#define	BMCSR(Port)			ECReg(USBPD_BASE(Port) + 0x64)	// BMC Control Setting Register
#define	PDMHSR(Port)		ECReg(USBPD_BASE(Port) + 0x65)	// PD Message Header Setting Register

//tmp
#define	RSR				0x01
#define	CCCR0			0x06
#define	CCCR1			0x07
#define	CC1DRR			0x0B
#define	CC2DRR			0x0C
#define	VCNTDR0			0x0E
#define USBPD_ISR_TMP	0x28
#define	TCR0			0x30
#define TCR1			0x31
#define SOPTR			0x32
#define RMSR0			0x34
#define RHDLR			0x3A
#define RHDHR			0x3B
#define	RPDO0R0			0xA0
#define LCPDO0R0		0x80
#define USBPD_DCR1		0xD9


#endif
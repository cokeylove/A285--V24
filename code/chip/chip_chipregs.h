/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * chip_chipregs.h
 * Dino Li
 * ****************************************************************************
 */

#ifndef CHIP_CHIPREGS_H
#define CHIP_CHIPREGS_H

#define EC_Register_Base_Address    0x00F00000

#ifndef REG_BASE_ADDR
#define REG_BASE_ADDR       EC_Register_Base_Address
#endif

/* 
 * ****************************************************************************
 * (10XXh) Shared Memory Flash Interface Bridge (SMFI) 
 * ****************************************************************************
 */
/* FBIU Configuration */
#define FBCFG           ECReg(EC_Register_Base_Address+0x1000)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SSMC            BIT(7)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* FBIU Configuration */
#define FBCFG           ECReg(EC_Register_Base_Address+0x1000)

/* Flash Programming Configuration Register */
#define FPCFG			ECReg(EC_Register_Base_Address+0x1001)

/* Memory Zone Configuration */
#define MZCFG           ECReg(EC_Register_Base_Address+0x1002)

/* State Memory Zone Configuration */
#define SMZCFG          ECReg(EC_Register_Base_Address+0x1003)

/* Flash EC Code Banking Select Register */
#define FECBSR			ECReg(EC_Register_Base_Address+0x1005)

/* Flash Memory Size Select Registe */
#define FMSSR			ECReg(EC_Register_Base_Address+0x1007)

/* Flash Memory Pre-Scale */
#define FMPSR           ECReg(EC_Register_Base_Address+0x1010)

/* Shared Memory EC Control and Status */
#define SMECCS			ECReg(EC_Register_Base_Address+0x1020)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define HOSTWA          BIT(5)
#define LKPRR           BIT(2)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Shared Memory Host Semaphore */
#define SMHSR			ECReg(EC_Register_Base_Address+0x1022)

/* FWH Flash ID Register */
#define FWHFIDR         ECReg(EC_Register_Base_Address+0x1030)

/* Flash Control Register 1	*/
#define	FLHCTRL1R		ECReg(EC_Register_Base_Address+0x1031)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --		    BIT(7)  /* Reserved */
//#define --		    BIT(6)  /* Reserved */
/*
 * SPI Flash Read Mode
 * 11b: Uses ¡§Fast Read Dual Input/Output (DIOFR)¡¨ cycle (instruction = BBh)
 * 10b: Uses ¡§Fast Read Dual Output (DOFR)¡¨ cycle (instruction = 3Bh)
 * 01b: Uses ¡§Fast Read (FREAD)¡¨ cycle (instruction = 0Bh)
 * 00b: Uses ¡§Read¡¨ cycle (instruction = 03h)
 */
#define SPIFR1		    BIT(5)
#define SPIFR0		    BIT(4)
#define LFSW1T		    BIT(3)  /* Serial Wait 1T */
//#define --		    BIT(2)  /* Reserved */
//#define --		    BIT(1)  /* Reserved */
//#define --		    BIT(0)  /* Reserved */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Flash Control Register 2 */
#define	FLHCTRL2R		ECReg(EC_Register_Base_Address+0x1032)

/* 256 bytes cache */
#define	DCache			ECReg(EC_Register_Base_Address+0x1033)

/* uC Control Register */
#define	UCCTRLR			ECReg(EC_Register_Base_Address+0x1034)

/* Host Control 2 Register */
#define	HCTRL2R			ECReg(EC_Register_Base_Address+0x1036)

/* HSPI Control 2 Register */
#define HSPICTRL2R      ECReg(EC_Register_Base_Address+0x1039)

/* HSPI */
#define	HSPICTRL3R	    ECReg(EC_Register_Base_Address+0x103A)

/* EC-Indirect Memory Address Register 0 */
#define ECINDAR0		ECReg(EC_Register_Base_Address+0x103B)

/* EC-Indirect Memory Address Register 1 */
#define ECINDAR1		ECReg(EC_Register_Base_Address+0x103C)

/* EC-Indirect Memory Address Register 2 */
#define ECINDAR2		ECReg(EC_Register_Base_Address+0x103D)

/* EC-Indirect Memory Address Register 3 */
#define ECINDAR3		ECReg(EC_Register_Base_Address+0x103E)

/* EC-Indirect Memory Data Register */
#define ECINDDR			ECReg(EC_Register_Base_Address+0x103F)

/* Scratch SRAM 0 Address Low Byte Register */
#define	SCRA0L			ECReg(EC_Register_Base_Address+0x1040)

/* Scratch SRAM 0 Address Middle Byte Register */
#define	SCRA0M			ECReg(EC_Register_Base_Address+0x1041)

/* Scratch SRAM 0 Address High Byte Register */
#define	SCRA0H			ECReg(EC_Register_Base_Address+0x1042)

/* Scratch SRAM 1 Address Low Byte Register */
#define	SCRA1L			ECReg(EC_Register_Base_Address+0x1043)

/* Scratch SRAM 1 Address Middle Byte Register */
#define	SCRA1M			ECReg(EC_Register_Base_Address+0x1044)

/* Scratch SRAM 1 Address High Byte Register */
#define	SCRA1H			ECReg(EC_Register_Base_Address+0x1045)

/* Scratch SRAM 2 Address Low Byte Register */
#define	SCRA2L			ECReg(EC_Register_Base_Address+0x1046)

/* Scratch SRAM 2 Address Middle Byte Register */
#define	SCRA2M			ECReg(EC_Register_Base_Address+0x1047)

/* Scratch SRAM 2 Address High Byte Register */
#define	SCRA2H			ECReg(EC_Register_Base_Address+0x1048)

/* Scratch SRAM 3 Address Low Byte Register */
#define	SCRA3L			ECReg(EC_Register_Base_Address+0x1049)

/* Scratch SRAM 3 Address Middle Byte Register */
#define	SCRA3M			ECReg(EC_Register_Base_Address+0x104A)

/* Scratch SRAM 3 Address High Byte Register */
#define	SCRA3H			ECReg(EC_Register_Base_Address+0x104B)

/* Scratch SRAM 4 Address Low Byte Register */
#define	SCRA4L			ECReg(EC_Register_Base_Address+0x104C)

/* Scratch SRAM 4 Address Middle Byte Register */
#define	SCRA4M			ECReg(EC_Register_Base_Address+0x104D)

/* Scratch SRAM 4 Address High Byte Register */
#define	SCRA4H			ECReg(EC_Register_Base_Address+0x104E)

/* Protect 0 Base Addr Register 0 */
#define	P0BA0R			ECReg(EC_Register_Base_Address+0x104F)

/* Protect 0 Base Addr Register 1 */
#define	P0BA1R			ECReg(EC_Register_Base_Address+0x1050)

/* Protect 0 Size Register */
#define	P0ZR			ECReg(EC_Register_Base_Address+0x1051)

/* Protect 1 Base Addr Register 0 */
#define	P1BA0R			ECReg(EC_Register_Base_Address+0x1052)

/* Protect 1 Base Addr Register 1 */
#define	P1BA1R			ECReg(EC_Register_Base_Address+0x1053)

/* Protect 1 Size Register */
#define	P1ZR			ECReg(EC_Register_Base_Address+0x1054)

/* Deferred SPI Instruction */
#define	DSINST			ECReg(EC_Register_Base_Address+0x1055)

/* Deferred SPI Address */
#define	DSADR1			ECReg(EC_Register_Base_Address+0x1056)

/* Deferred SPI Address */
#define	DSADR2			ECReg(EC_Register_Base_Address+0x1057)

/* Host Instruction Control 1 */
#define	HINSTC1			ECReg(EC_Register_Base_Address+0x1058)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DISSV		    BIT(3)
#define DISS		    BIT(2)
#define ENDPI		    BIT(1)
#define ENDEI		    BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host Instruction Control 2 */
#define	HINSTC2			ECReg(EC_Register_Base_Address+0x1059)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DISEID8		    BIT(3)
#define DISEID7		    BIT(2)
#define DISEI52		    BIT(1)
#define DISEI20		    BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host RAM Window Control */
#define	HRAMWC			ECReg(EC_Register_Base_Address+0x105A)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define Window0En	    BIT(0)  /* Window 0 enabled */
#define Window1En	    BIT(1)  /* Window 1 enabled */
#define H2RAMS		    BIT(4)  /*
                                 * 0 : H2RAM-HLPC selected,
                                 * 1 : H2RAM-HSPI selected
                                 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host RAM Winodw 0 Base Address */
#define	HRAMW0BA		ECReg(EC_Register_Base_Address+0x105B)

/* Host RAM Window 1 Base Address */
#define	HRAMW1BA		ECReg(EC_Register_Base_Address+0x105C)

/* Host RAM Window 0 Access Allow Size */
#define	HRAMW0AAS		ECReg(EC_Register_Base_Address+0x105D)

/* Host RAM Window 1 Access Allow Size */
#define	HRAMW1AAS		ECReg(EC_Register_Base_Address+0x105E)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define HostRamSize16Byte 		0x00
#define HostRamSize32Byte 		0x01
#define HostRamSize64Byte 		0x02
#define HostRamSize128Byte 		0x03
#define HostRamSize256Byte 		0x04
#define HostRamSize512Byte 		0x05
#define HostRamSize1024Byte 	0x06
#define HostRamSize2048Byte 	0x07
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define CHECK64KSRAM    ECReg(EC_Register_Base_Address+0x1060)
#define CRC_HByte       ECReg(EC_Register_Base_Address+0x1061)
#define CRC_LByte       ECReg(EC_Register_Base_Address+0x1062)
#define	FLHCTRL3R		ECReg(EC_Register_Base_Address+0x1063)

/* Flash Control 4 Register */
#define FLHCTRL4R       ECReg(EC_Register_Base_Address+0x1064)

/* Protect 2 Base Addr Register 0 */
#define P2BA0R          ECReg(EC_Register_Base_Address+0x1070)

/* Protect 2 Base Addr Register 1 */
#define P2BA1R          ECReg(EC_Register_Base_Address+0x1071)

/* Protect 2 Size Register */
#define P2ZR            ECReg(EC_Register_Base_Address+0x1072)

/* Protect 3 Base Addr Register 0 */
#define P3BA0R          ECReg(EC_Register_Base_Address+0x1073)

/* Protect 3 Base Addr Register 1 */
#define P3BA1R          ECReg(EC_Register_Base_Address+0x1074)

/* Protect 3 Size Register */
#define P3ZR            ECReg(EC_Register_Base_Address+0x1075)

/* Host RAM Windows 2 Base Address */
#define HRAMW2BA        ECReg(EC_Register_Base_Address+0x1076)

/* Host RAM Windows 3 Base Address */
#define HRAMW3BA        ECReg(EC_Register_Base_Address+0x1077)

/* Host RAM Windows 2 Access Allow Size */
#define HRAMW2AAS       ECReg(EC_Register_Base_Address+0x1078)

/* Host RAM Windows 3 Access Allow Size */
#define HRAMW3AAS       ECReg(EC_Register_Base_Address+0x1079)

/* H2RAM EC Semaphore Interrupt Enable */
#define H2RAMECSIE      ECReg(EC_Register_Base_Address+0x107A)

/* H2RAM EC Semaphore Address */
#define H2RAMECSA       ECReg(EC_Register_Base_Address+0x107B)

/* H2RAM Host Semphore Status */
#define H2RAMHSS        ECReg(EC_Register_Base_Address+0x107C)

/* Host Protect Authentication Data Register */
#define HPADR           ECReg(EC_Register_Base_Address+0x107E)

/* Static DMA Control Register */
#define STCDMACR        ECReg(EC_Register_Base_Address+0x1080)

/* Scratch SRAM 5 Address Low Byte Register */
#define	SCRA5L			ECReg(EC_Register_Base_Address+0x1081)

/* Scratch SRAM 5 Address Middle Byte Register */
#define	SCRA5M			ECReg(EC_Register_Base_Address+0x1082)

/* Scratch SRAM 5 Address High Byte Register */
#define	SCRA5H			ECReg(EC_Register_Base_Address+0x1083)

/* Scratch SRAM 6 Address Low Byte Register */
#define	SCRA6L			ECReg(EC_Register_Base_Address+0x1084)

/* Scratch SRAM 6 Address Middle Byte Register */
#define	SCRA6M			ECReg(EC_Register_Base_Address+0x1085)

/* Scratch SRAM 6 Address High Byte Register */
#define	SCRA6H			ECReg(EC_Register_Base_Address+0x1086)

/* Scratch SRAM 7 Address Low Byte Register */
#define	SCRA7L			ECReg(EC_Register_Base_Address+0x1087)

/* Scratch SRAM 7 Address Middle Byte Register */
#define	SCRA7M			ECReg(EC_Register_Base_Address+0x1088)

/* Scratch SRAM 7 Address High Byte Register */
#define	SCRA7H			ECReg(EC_Register_Base_Address+0x1089)

/* Scratch SRAM 8 Address Low Byte Register */
#define	SCRA8L			ECReg(EC_Register_Base_Address+0x108A)

/* Scratch SRAM 8 Address Middle Byte Register */
#define	SCRA8M			ECReg(EC_Register_Base_Address+0x108B)

/* Scratch SRAM 8 Address High Byte Register */
#define	SCRA8H			ECReg(EC_Register_Base_Address+0x108C)

/* Scratch SRAM 9 Address Low Byte Register */
#define	SCRA9L			ECReg(EC_Register_Base_Address+0x108D)

/* Scratch SRAM 9 Address Middle Byte Register */
#define	SCRA9M			ECReg(EC_Register_Base_Address+0x108E)

/* Scratch SRAM 9 Address High Byte Register */
#define	SCRA9H			ECReg(EC_Register_Base_Address+0x108F)

/* Scratch SRAM 10 Address Low Byte Register */
#define	SCRA10L			ECReg(EC_Register_Base_Address+0x1090)

/* Scratch SRAM 10 Address Middle Byte Register */
#define	SCRA10M			ECReg(EC_Register_Base_Address+0x1091)

/* Scratch SRAM 10 Address High Byte Register */
#define	SCRA10H			ECReg(EC_Register_Base_Address+0x1092)

/* Scratch SRAM 11 Address Low Byte Register */
#define	SCRA11L			ECReg(EC_Register_Base_Address+0x1093)

/* Scratch SRAM 11 Address Middle Byte Register */
#define	SCRA11M			ECReg(EC_Register_Base_Address+0x1094)

/* Scratch SRAM 11 Address High Byte Register */
#define	SCRA11H			ECReg(EC_Register_Base_Address+0x1095)

/* Scratch SRAM 12 Address Low Byte Register */
#define	SCRA12L			ECReg(EC_Register_Base_Address+0x1096)

/* Scratch SRAM 12 Address Middle Byte Register */
#define	SCRA12M			ECReg(EC_Register_Base_Address+0x1097)

/* Scratch SRAM 12 Address High Byte Register */
#define	SCRA12H			ECReg(EC_Register_Base_Address+0x1098)

/* ROM Address Low Byte Register */
#define ROMARL          ECReg(EC_Register_Base_Address+0x1099)

/* ROM Address Middle Byte Register */
#define ROMARM          ECReg(EC_Register_Base_Address+0x109A)

/* ROM Address High Byte Register */
#define ROMARH          ECReg(EC_Register_Base_Address+0x109B)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SCRATH_SRAM     0x08
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (11xxh) Interrupt controller (INTC)
 * ****************************************************************************
 */
/* Interrupt Status Register 0 */
#define ISR0      		ECReg(EC_Register_Base_Address+0x1100)

/* Interrupt Status Register 1 */
#define ISR1       		ECReg(EC_Register_Base_Address+0x1101)

/* Interrupt Status Register 2 */
#define ISR2       		ECReg(EC_Register_Base_Address+0x1102)

/* Interrupt Status Register 3 */
#define ISR3       		ECReg(EC_Register_Base_Address+0x1103)

/* Interrupt Status Register 4 */
#define ISR4    		ECReg(EC_Register_Base_Address+0x1114)

/* Interrupt Status Register 5 */
#define ISR5    		ECReg(EC_Register_Base_Address+0x1118)

/* Interrupt Status Register 6 */
#define ISR6    		ECReg(EC_Register_Base_Address+0x111C)

/* Interrupt Status Register 7 */
#define ISR7    		ECReg(EC_Register_Base_Address+0x1120)

/* Interrupt Status Register 8 */
#define ISR8            ECReg(EC_Register_Base_Address+0x1124)

/* Interrupt Status Register 9 */
#define ISR9            ECReg(EC_Register_Base_Address+0x1128)

/* Interrupt Status Register 10 */
#define ISR10           ECReg(EC_Register_Base_Address+0x112C)

/* Interrupt Status Register 11 */
#define ISR11           ECReg(EC_Register_Base_Address+0x1130)

/* Interrupt Status Register 12 */
#define ISR12           ECReg(EC_Register_Base_Address+0x1134)

/* Interrupt Status Register 13 */
#define ISR13           ECReg(EC_Register_Base_Address+0x1138)

/* Interrupt Status Register 14 */
#define ISR14           ECReg(EC_Register_Base_Address+0x113C)

/* Interrupt Status Register 15 */
#define ISR15           ECReg(EC_Register_Base_Address+0x1140)

/* Interrupt Status Register 16 */
#define ISR16           ECReg(EC_Register_Base_Address+0x1144)

/* Interrupt Status Register 17 */
#define ISR17           ECReg(EC_Register_Base_Address+0x1148)

/* Interrupt Status Register 18 */
#define ISR18           ECReg(EC_Register_Base_Address+0x114C)

/* Interrupt Status Register 19 */
#define ISR19           ECReg(EC_Register_Base_Address+0x1150)

/* Interrupt Status Register 20 */
#define ISR20           ECReg(EC_Register_Base_Address+0x1154)

/* Interrupt Enable Register 0 */
#define IER0      		ECReg(EC_Register_Base_Address+0x1104)

/* Interrupt Enable Register 1 */
#define IER1       		ECReg(EC_Register_Base_Address+0x1105)

/* Interrupt Enable Register 2 */
#define IER2      		ECReg(EC_Register_Base_Address+0x1106)

/* Interrupt Enable Register 3 */
#define IER3       		ECReg(EC_Register_Base_Address+0x1107)

/* Interrupt Enable Register 4 */
#define IER4    		ECReg(EC_Register_Base_Address+0x1115)

/* Interrupt Enable Register 5 */
#define IER5    		ECReg(EC_Register_Base_Address+0x1119)

/* Interrupt Enable Register 6 */
#define IER6    		ECReg(EC_Register_Base_Address+0x111D)

/* Interrupt Enable Register 7 */
#define IER7    		ECReg(EC_Register_Base_Address+0x1121)

/* Interrupt Enable Register 8 */
#define IER8    		ECReg(EC_Register_Base_Address+0x1125)

/* Interrupt Enable Register 9 */
#define IER9    		ECReg(EC_Register_Base_Address+0x1129)

/* Interrupt Enable Register 10 */
#define IER10           ECReg(EC_Register_Base_Address+0x112D)

/* Interrupt Enable Register 11 */
#define IER11           ECReg(EC_Register_Base_Address+0x1131)

/* Interrupt Enable Register 12 */
#define IER12           ECReg(EC_Register_Base_Address+0x1135)

/* Interrupt Enable Register 13 */
#define IER13           ECReg(EC_Register_Base_Address+0x1139)

/* Interrupt Enable Register 14 */
#define IER14           ECReg(EC_Register_Base_Address+0x113D)

/* Interrupt Enable Register 15 */
#define IER15           ECReg(EC_Register_Base_Address+0x1141)

/* Interrupt Enable Register 16 */
#define IER16           ECReg(EC_Register_Base_Address+0x1145)

/* Interrupt Enable Register 17 */
#define IER17           ECReg(EC_Register_Base_Address+0x1149)

/* Interrupt Enable Register 18 */
#define IER18           ECReg(EC_Register_Base_Address+0x114D)

/* Interrupt Enable Register 19 */
#define IER19           ECReg(EC_Register_Base_Address+0x1151)

/* Interrupt Enable Register 20 */
#define IER20           ECReg(EC_Register_Base_Address+0x1155)

/* Interrupt Edge/Level-Triggered Mode Register 0 */
#define IELMR0    		ECReg(EC_Register_Base_Address+0x1108)

/* Interrupt Edge/Level-Triggered Mode Register 1 */
#define IELMR1     		ECReg(EC_Register_Base_Address+0x1109)

/* Interrupt Edge/Level-Triggered Mode Register 2 */
#define IELMR2   		ECReg(EC_Register_Base_Address+0x110A)

/* Interrupt Edge/Level-Triggered Mode Register 3 */
#define IELMR3     		ECReg(EC_Register_Base_Address+0x110B)

/* Interrupt Edge/Level-Triggered Mode Register 4 */
#define IELMR4    		ECReg(EC_Register_Base_Address+0x1116)

/* Interrupt Edge/Level-Triggered Mode Register 5 */
#define IELMR5    		ECReg(EC_Register_Base_Address+0x111A)

/* Interrupt Edge/Level-Triggered Mode Register 6 */
#define IELMR6    		ECReg(EC_Register_Base_Address+0x111E)

/* Interrupt Edge/Level-Triggered Mode Register 7 */
#define IELMR7    		ECReg(EC_Register_Base_Address+0x1122)

/* Interrupt Edge/Level-Triggered Mode Register 8 */
#define IELMR8    		ECReg(EC_Register_Base_Address+0x1126)

/* Interrupt Edge/Level-Triggered Mode Register 9 */
#define IELMR9    		ECReg(EC_Register_Base_Address+0x112A)

/* Interrupt Edge/Level-Triggered Mode Register 10 */
#define IELMR10         ECReg(EC_Register_Base_Address+0x112E)

/* Interrupt Edge/Level-Triggered Mode Register 11 */
#define IELMR11         ECReg(EC_Register_Base_Address+0x1132)

/* Interrupt Edge/Level-Triggered Mode Register 12 */
#define IELMR12         ECReg(EC_Register_Base_Address+0x1136)

/* Interrupt Edge/Level-Triggered Mode Register 13 */
#define IELMR13         ECReg(EC_Register_Base_Address+0x113A)

/* Interrupt Edge/Level-Triggered Mode Register 14 */
#define IELMR14         ECReg(EC_Register_Base_Address+0x113E)

/* Interrupt Edge/Level-Triggered Mode Register 15 */
#define IELMR15         ECReg(EC_Register_Base_Address+0x1142)

/* Interrupt Edge/Level-Triggered Mode Register 16 */
#define IELMR16         ECReg(EC_Register_Base_Address+0x1146)

/* Interrupt Edge/Level-Triggered Mode Register 17 */
#define IELMR17         ECReg(EC_Register_Base_Address+0x114A)

/* Interrupt Edge/Level-Triggered Mode Register 18 */
#define IELMR18         ECReg(EC_Register_Base_Address+0x114E)

/* Interrupt Edge/Level-Triggered Mode Register 19 */
#define IELMR19         ECReg(EC_Register_Base_Address+0x1152)

/* Interrupt Edge/Level-Triggered Mode Register 20 */
#define IELMR20         ECReg(EC_Register_Base_Address+0x1156)

/* Interrupt Polarity Register 0 */
#define IPOLR0     		ECReg(EC_Register_Base_Address+0x110C)

/* Interrupt Polarity Register 1 */
#define IPOLR1    		ECReg(EC_Register_Base_Address+0x110D)

/* Interrupt Polarity Register 2 */
#define IPOLR2    		ECReg(EC_Register_Base_Address+0x110E)

/* Interrupt Polarity Register 3 */
#define IPOLR3    		ECReg(EC_Register_Base_Address+0x110F)

/* Interrupt Polarity Register 4 */
#define IPOLR4    		ECReg(EC_Register_Base_Address+0x1117)

/* Interrupt Polarity Register 5 */
#define IPOLR5    		ECReg(EC_Register_Base_Address+0x111B)

/* Interrupt Polarity Register 6 */
#define IPOLR6    		ECReg(EC_Register_Base_Address+0x111F)

/* Interrupt Polarity Register 7 */
#define IPOLR7    		ECReg(EC_Register_Base_Address+0x1123)

/* Interrupt Polarity Register 8 */
#define IPOLR8    		ECReg(EC_Register_Base_Address+0x1127)

/* Interrupt Polarity Register 9 */
#define IPOLR9    		ECReg(EC_Register_Base_Address+0x112B)

/* Interrupt Polarity Register 10 */
#define IPOLR10         ECReg(EC_Register_Base_Address+0x112F)

/* Interrupt Polarity Register 11 */
#define IPOLR11         ECReg(EC_Register_Base_Address+0x1133)

/* Interrupt Polarity Register 12 */
#define IPOLR12         ECReg(EC_Register_Base_Address+0x1137)

/* Interrupt Polarity Register 13 */
#define IPOLR13         ECReg(EC_Register_Base_Address+0x113B)

/* Interrupt Polarity Register 14 */
#define IPOLR14         ECReg(EC_Register_Base_Address+0x113F)

/* Interrupt Polarity Register 15 */
#define IPOLR15         ECReg(EC_Register_Base_Address+0x1143)

/* Interrupt Polarity Register 16 */
#define IPOLR16         ECReg(EC_Register_Base_Address+0x1147)

/* Interrupt Polarity Register 17 */
#define IPOLR17         ECReg(EC_Register_Base_Address+0x114B)

/* Interrupt Polarity Register 18 */
#define IPOLR18         ECReg(EC_Register_Base_Address+0x114F)

/* Interrupt Polarity Register 19 */
#define IPOLR19         ECReg(EC_Register_Base_Address+0x1153)

/* Interrupt Polarity Register 20 */
#define IPOLR20         ECReg(EC_Register_Base_Address+0x1157)

/* Interrupt Vector Register 0 */
#define IVECT0          ECReg(EC_Register_Base_Address+0x1180)

/* Interrupt Vector Register 1 */
#define IVECT1          ECReg(EC_Register_Base_Address+0x1181)

/* Interrupt Vector Register 2 */
#define IVECT2          ECReg(EC_Register_Base_Address+0x1182)

/* Interrupt Vector Register 3 */
#define IVECT3          ECReg(EC_Register_Base_Address+0x1183)

/* Interrupt Vector Register 4 */
#define IVECT4          ECReg(EC_Register_Base_Address+0x1184)

/* Interrupt Vector Register 5 */
#define IVECT5          ECReg(EC_Register_Base_Address+0x1185)

/* Interrupt Vector Register 6 */
#define IVECT6          ECReg(EC_Register_Base_Address+0x1186)

/* Interrupt Vector Register 7 */
#define IVECT7          ECReg(EC_Register_Base_Address+0x1187)

/* Interrupt Vector Register 8 */
#define IVECT8          ECReg(EC_Register_Base_Address+0x1188)

/* Interrupt Vector Register 9 */
#define IVECT9          ECReg(EC_Register_Base_Address+0x1189)

/* Interrupt Vector Register 10 */
#define IVECT10         ECReg(EC_Register_Base_Address+0x118A)

/* Interrupt Vector Register 11 */
#define IVECT11         ECReg(EC_Register_Base_Address+0x118B)

/* Interrupt Vector Register 12 */
#define IVECT12         ECReg(EC_Register_Base_Address+0x118C)

/* Interrupt Vector Register 13 */
#define IVECT13         ECReg(EC_Register_Base_Address+0x118D)

/* Interrupt Vector Register 14 */
#define IVECT14         ECReg(EC_Register_Base_Address+0x118E)

/* Interrupt Vector Register 15 */
#define IVECT15         ECReg(EC_Register_Base_Address+0x118F)

/* Interrupt Vector Register */
#define IVECT    		ECReg(EC_Register_Base_Address+0x1110)

/* INT0# status */
#define INT0ST    		ECReg(EC_Register_Base_Address+0x1111)

/* Power Fail Register */
#define PFAILR    		ECReg(EC_Register_Base_Address+0x1112)

#define IGER0           ECReg(EC_Register_Base_Address+0x1160)
#define IGER1           ECReg(EC_Register_Base_Address+0x1161)
#define IGER2           ECReg(EC_Register_Base_Address+0x1162)
#define IGER3           ECReg(EC_Register_Base_Address+0x1163)
#define IGER4           ECReg(EC_Register_Base_Address+0x1164)
#define IGER5           ECReg(EC_Register_Base_Address+0x1165)
#define IGER6           ECReg(EC_Register_Base_Address+0x1166)
#define IGER7           ECReg(EC_Register_Base_Address+0x1167)
#define IGER8           ECReg(EC_Register_Base_Address+0x1168)
#define IGER9           ECReg(EC_Register_Base_Address+0x1169)
#define IGER10          ECReg(EC_Register_Base_Address+0x116A)
#define IGER11          ECReg(EC_Register_Base_Address+0x116B)
#define IGER12          ECReg(EC_Register_Base_Address+0x116C)
#define IGER13          ECReg(EC_Register_Base_Address+0x116D)
#define IGER14          ECReg(EC_Register_Base_Address+0x116E)
#define IGER15          ECReg(EC_Register_Base_Address+0x116F)
#define IGER16          ECReg(EC_Register_Base_Address+0x1170)
#define IGER17          ECReg(EC_Register_Base_Address+0x1171)
#define IGER18          ECReg(EC_Register_Base_Address+0x1172)
#define IGER19          ECReg(EC_Register_Base_Address+0x1173)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// [Group]                      [Interrupt] [ Description ]
//
// IER0
//		-			    BIT(0)	// INT0     // -
#define	Int_WKO20	    BIT(1)  // INT1     // WKO[20]
#define	Int_KBCOBFE	    BIT(2)	// INT2     // KBC Output Buffer Empty Interrupt
#define	Int_PMCOBFE	    BIT(3)  // INT3     // PMC Output Buffer Empty Interrupt
#define	Int_SMBUS3	    BIT(4)  // INT4     // SMBUS D Interrupt	
#define Int_WKINTAD	    BIT(5)  // INT5     // WKINTAD (WKINTA or WKINTD)
#define Int_WKO23	    BIT(6)  // INT6     // WKO[23]
#define Int_PWM		    BIT(7)  // INT7     // PWM Interrupt

// IER1
#define	Int_ADC		    BIT(0)	// INT8     // ADC Interrupt
#define	Int_SMBUS0	    BIT(1)  // INT9     // SMBUS A Interrupt
#define	Int_SMBUS1	    BIT(2)  // INT10    // SMBUS B Interrupt
#define	Int_KB		    BIT(3)  // INT11    // KB Matrix Scan Interrupt
#define	Int_WKO26	    BIT(4)  // INT12    // WKO[26]
#define Int_WKINTC	    BIT(5)  // INT13    // WKINTC
#define Int_WKO25	    BIT(6)  // INT14    // WKO[25]
#define Int_CIR		    BIT(7)  // INT15    // CIR Interrupt

// IER2
#define	Int_SMBUS2	    BIT(0)  // INT16    // SMBUS C Interrupt
#define Int_WKO24	    BIT(1)  // INT17    // WKO[24]
#define	Int_PS2_2       BIT(2)  // INT18    // PS/2 Interrupt 2
#define	Int_PS2_1	    BIT(3)  // INT19    // PS/2 Interrupt 1			// ONLY 2 PS2 CH ON 8186
#define	Int_PS2_0	    BIT(4)  // INT20    // PS/2 Interrupt 0
#define	Int_WKO22	    BIT(5)  // INT21    // WKO[22]
#define	SMFIS	        BIT(6)  // INT22    // SMFI Semaphore Interrupt
#define USBCIF			BIT(7)  // INT23    // USB Controller Interrupt

// IER3
#define	Int_KBCIBF	    BIT(0)  // INT24    // KBC Input Buffer Full Interrupt
#define Int_PMCIBF	    BIT(1)  // INT25    // PMC Input Buffer Full Interrupt
#define Int_PMC2OBE     BIT(2)  // INT26    // PMC2 Output Buffer Empty Intr.
#define Int_PMC2IBF	    BIT(3)  // INT27    // PMC2 Input Buffer Full Intr.
#define	Int_GINT	    BIT(4)  // INT28    // GINT from function 1 of GPD5
#define	Int_EGPC	    BIT(5)  // INT29    // EGPC Interrupt
#define	Int_EXTimer	    BIT(6)  // INT30    // External Timer 1 Interrupt
#define	Int_WKO21	    BIT(7)  // INT31    // WKO[21]

// IER4
#define	Int_GPINT0	    BIT(0)  // INT32    // GPINT0
#define	Int_GPINT1	    BIT(1)	// INT33    // GPINT1
#define	Int_GPINT2	    BIT(2)  // INT34    // GPINT2
#define	Int_GPINT3	    BIT(3)  // INT35    // GPINT3
#define	Int_CIRGPINT    BIT(4)  // INT36    // CIR GPINT
#define	Int_SSPI	    BIT(5)  // INT37    // SSPI Interrupt
#define	Int_UART1	    BIT(6)  // INT38    // UART1 Interrupt
#define Int_UART2       BIT(7)  // INT39    // UART2 Interrupt

// IER5
#define Int_WKO50	    BIT(0)  // INT40    // WKO[50]
#define	Int_WKO51	    BIT(1)  // INT41    // WKO[51]
#define	Int_WKO52	    BIT(2)  // INT42    // WKO[52]
#define	Int_WKO53       BIT(3)  // INT43    // WKO[53]
#define	Int_WKO54       BIT(4)  // INT44    // WKO[54]
#define	Int_WKO55	    BIT(5)  // INT45    // WKO[55]
#define	Int_WKO56	    BIT(6)  // INT46    // WKO[56]
#define	Int_WKO57	    BIT(7)  // INT47    // WKO[57]

// IER6
#define	Int_WKO60	    BIT(0)  // INT48    // WKO[60]
#define	Int_WKO61	    BIT(1)	// INT49    // WKO[61]
#define	Int_WKO62	    BIT(2)  // INT50    // WKO[62]
#define	Int_WKO63	    BIT(3)  // INT51    // WKO[63]
#define	Int_WKO64	    BIT(4)  // INT52    // WKO[64]
#define	Int_WKO65	    BIT(5)	// INT53    // WKO[65]
#define	Int_WKO66	    BIT(6)	// INT54    // WKO[66]
#define	Int_WKO67	    BIT(7)  // INT55    // WKO[67]

// IER7
#define Int_RTCTAlarm1  BIT(0)  // INT56    // RTCT Alarm 1
#define	Int_RTCTAlarm2  BIT(1)	// INT57    // RTCT Alarm 2
#define Int_ET2Intr	    BIT(2)  // INT58    // External Timer 2 Interrupt
//#define   Int_        BIT(3)  // INT59    // -
#define Int_TMRINTA0    BIT(4)  // INT60    // TMRINTA0
#define Int_TMRINTA1    BIT(5)  // INT61    // TMRINTA1
#define Int_TMRINTB0    BIT(6)  // INT62    // TMRINTB0
#define	Int_TMRINTB1    BIT(7)  // INT63    // TMRINTB1

// IER8
#define Int_PMC2EXOBE   BIT(0)  // INT64    // PMC2EX Output Buffer Empty Intr.
#define	Int_PMC2EXIBF   BIT(1)  // INT65    // PMC2EX Input Buffer Full Intr.
#define	Int_PMC3OBE	    BIT(2)  // INT66    // PMC3 Output Buffer Empty Intr.
#define Int_PMC3IBF	    BIT(3)  // INT67    // PMC3 Input Buffer Full Interrupt
#define Int_PMC4OBE	    BIT(4)  // INT68    // PMC4 Output Buffer Empty Intr.
#define	Int_PMC4IBF	    BIT(5)  // INT69    // PMC4 Input Buffer Full Intr.
//#define	Int_	    BIT(6)  // INT70    //
#define	Int_I2BRAM	    BIT(7)  // INT71    // I2BRAM Interrupt

// IER9
#define	Int_WKO70	    BIT(0)  // INT72    // WKO[70]
#define	Int_WKO71	    BIT(1)  // INT73    // WKO[71]
#define	Int_WKO72	    BIT(2)  // INT74    // WKO[72]
#define	Int_WKO73	    BIT(3)  // INT75    // WKO[73]
#define	Int_WKO74	    BIT(4)  // INT76    // WKO[74]
#define	Int_WKO75	    BIT(5)  // INT77    // WKO[75]
#define	Int_WKO76	    BIT(6)  // INT78    // WKO[76]
#define	Int_WKO77	    BIT(7)  // INT79    // WKO[77]

// IER10
#define Int_ET8Intr     BIT(0)  // INT80    // External Timer 8 Interrupt
#define Int_SMbusCHIner BIT(1)  // INT81    // SMbus Clock Held intr
#define Int_CEC         BIT(2)  // INT82    // CEC Interrupt
#define Int_H2RAMLPC    BIT(3)  // INT83    // H2RAM LPC Trigger
#define Int_KBSDVIntr   BIT(4)  // INT84    // KB Scan Data Valid Interrupt
#define Int_WKO88       BIT(5)  // INT85    // WKO[88]
#define Int_WKO89       BIT(6)  // INT86    // WKO[89]
#define Int_WKO90       BIT(7)  // INT87    // WKO[90]

// IER11
#define Int_WKO80       BIT(0)  // INT88    // WKO[80]
#define Int_WKO81       BIT(1)  // INT89    // WKO[81]
#define Int_WKO82       BIT(2)  // INT90    // WKO[82]
#define Int_WKO83       BIT(3)  // INT91    // WKO[83]
#define Int_WKO84       BIT(4)  // INT92    // WKO[84]
#define Int_WKO85       BIT(5)  // INT93    // WKO[85]
#define Int_WKO86       BIT(6)  // INT94    // WKO[86]
#define Int_WKO87       BIT(7)  // INT95    // WKO[87]

// IER12
#define Int_WKO91       BIT(0)  // INT96    // WKO[91]
#define Int_WKO92       BIT(1)  // INT97    // WKO[92]
#define Int_WKO93       BIT(2)  // INT98    // WKO[93]
#define Int_WKO94       BIT(3)  // INT99    // WKO[94]
#define Int_WKO95       BIT(4)  // INT100   // WKO[95]
#define Int_WKO96       BIT(5)  // INT101   // WKO[96]
#define Int_WKO97       BIT(6)  // INT102   // WKO[97]
#define Int_WKO98       BIT(7)  // INT103   // WKO[98]

// IER13
#define Int_WKO99       BIT(0)  // INT104   // WKO[99]
#define Int_WKO100      BIT(1)  // INT105   // WKO[100]
#define Int_WKO101      BIT(2)  // INT106   // WKO[101]
#define Int_WKO102      BIT(3)  // INT107   // WKO[102]
#define Int_WKO103      BIT(4)  // INT108   // WKO[103]
#define Int_WKO104      BIT(5)  // INT109   // WKO[104]
#define Int_WKO105      BIT(6)  // INT110   // WKO[105]
//#define Int_      BIT(7)  // Int   // 


// IER14
#define Int_WKO107      BIT(0)  // INT112   // WKO[107]
#define Int_WKO108      BIT(1)  // INT113   // WKO[108]
#define Int_WKO109      BIT(2)  // INT114   // WKO[109]
#define Int_WKO110      BIT(3)  // INT115   // WKO[110]
#define Int_WKO111      BIT(4)  // INT116   // WKO[111]
#define Int_WKO112      BIT(5)  // INT117   // WKO[112]
#define Int_WKO113      BIT(6)  // INT118   // WKO[113]
#define Int_WKO114      BIT(7)  // INT119   // WKO[114]

// IER15
#define Int_WKO115      BIT(0)  // INT120   // WKO[115]
#define Int_WKO116      BIT(1)  // INT121   // WKO[116]
#define Int_WKO117      BIT(2)  // INT122   // WKO[117]
#define Int_WKO118      BIT(3)  // INT123   // WKO[118]
#define Int_WKO119      BIT(4)  // INT124   // WKO[119]
#define Int_WKO120      BIT(5)  // INT125   // WKO[120]
#define Int_WKO121      BIT(6)  // INT126   // WKO[121]
#define Int_WKO122      BIT(7)  // INT127   // WKO[122]

// IER16
#define Int_WKO128      BIT(0)  // INT128   // WKO[128]
#define Int_WKO129      BIT(1)  // INT129   // WKO[129]
#define Int_WKO130      BIT(2)  // INT130   // WKO[130]
#define Int_WKO131      BIT(3)  // INT131   // WKO[131]
#define Int_WKO132      BIT(4)  // INT132   // WKO[132]
#define Int_WKO133      BIT(5)  // INT133   // WKO[133]
#define Int_WKO134      BIT(6)  // INT134   // WKO[134]
//#define Int_          BIT(7)  // INT135

// IER17
#define Int_WKO136	    BIT(0)  // INT136   // WKO[136]
#define	Int_WKO137	    BIT(1)  // INT137   // WKO[137]
#define	Int_WKO138	    BIT(2)  // INT138   // WKO[138]
#define	Int_WKO139      BIT(3)  // INT139   // WKO[139]
#define	Int_WKO140      BIT(4)  // INT140   // WKO[140]
#define	Int_WKO141	    BIT(5)  // INT141   // WKO[141]
#define	Int_WKO142	    BIT(6)  // INT142   // WKO[142]
#define	Int_WKO143	    BIT(7)  // INT143   // WKO[143]

// IER18
//#define Int_          BIT(0)  // INT144   // -
//#define Int_          BIT(1)  // INT145   // -
//#define Int_          BIT(2)  // INT146   // -
//#define Int_          BIT(3)  // INT147   // -
//#define Int_          BIT(4)  // INT148   // -
#define Int_PMC5OBE     BIT(5)  // INT149   // PMC5 Output Buffer Empty Intr.
#define Int_PMC5IBE     BIT(6)  // INT150   // PMC5 Input Buffer Full Intr.
#define Int_VCI         BIT(7)  // INT151   // Voltage Compare Interrupt

// IER19
#define Int_SMBUSE	    BIT(0)  // INT152   // SMBus E Interrupt
#define	Int_SMBUSF	    BIT(1)  // INT153   // SMBus F Interrupt
#define	Int_OSCDMAInter BIT(2)  // INT154   // OSC DMA Interrupt
#define	Int_ET3Intr     BIT(3)  // INT155   // External Timer 3 Interrupt
#define	Int_ET4Intr     BIT(4)  // INT156   // External Timer 4 Interrupt
#define	Int_ET5Intr	    BIT(5)  // INT157   // External Timer 5 Interrupt
#define	Int_ET6Intr	    BIT(6)  // INT158   // External Timer 6 Interrupt
#define	Int_ET7Intr	    BIT(7)  // INT159   // External Timer 7 Interrupt

//IER20
#define Int_PECI		BIT(0)  // INT160  //  PECI Interrupt
#define Int_SWInt		BIT(1)	// INT161	 //  SW Interrupt
#define Int_ESPIInt		BIT(2)	// INT162  //  ESPI Interrupt
#define Int_ESPIVWInt	BIT(3)	// INT163  //  ESPI VW Interrupt
#define Int_PORT80		BIT(4)  // INT164  //  PCH Port 80 Interrupt
#define Int_USBPD0      BIT(5)  // INT165	 // UABPD channel 0
#define Int_USBPD1      BIT(6)  // INT166	 // UABPD channel 1
#define Int_HOSTSPI		BIT(7)	// INT167  //  Host request SPI Slave Interrupt
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (12xxh) EC Access to ost Controlled Modules (EC2I Bridge)
 * ****************************************************************************
 */
/* Indirect Host I/O Address */
#define IHIOA     		ECReg(EC_Register_Base_Address+0x1200)

/* Indirect Host Data */
#define IHD      		ECReg(EC_Register_Base_Address+0x1201)

/* Lock Super I/O Host Access */
#define LSIOHA    		ECReg(EC_Register_Base_Address+0x1202)

/* Super I/O Access Lock Violation */
#define SIOLV     		ECReg(EC_Register_Base_Address+0x1203)

/* EC to I-Bus Modules Access Enable */
#define IBMAE			ECReg(EC_Register_Base_Address+0x1204)

/* I-Bus Control */
#define IBCTL   		ECReg(EC_Register_Base_Address+0x1205)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Lock Super I/O Host Access Register */
#define	LKCFG	    BIT(0)  /* Lock PNPCFG Registers Host Access */

/* Super I/O Access Lock Violation Register */
#define	CFGLV	    BIT(0)  /* PNPCFG Register Lock Violation */

/* EC to I-Bus Modules Access Enable Register */
#define	SWUCAE	    BIT(2)  /* Mobile System Wake-Up Control Access Enable */
#define	CFGAE	    BIT(0)  /* PNPCFG Register EC Access Enable */

/* I-Bus Control Register */
#define	CWIB	    BIT(2)  /* EC Write to I-Bus */
#define	CRIB	    BIT(1)  /* EC Read from I-Bus */
#define CSAE	    BIT(0)  /* EC to I-Bus Access Enabled */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (13xxh) Keyboard Controller (KBC)
 * ****************************************************************************
 */
/* KBC Host Interface Control Register */
#define KBHICR   		ECReg(EC_Register_Base_Address+0x1300)

/* KBC Interrupt Control Register */
#define KBIRQR     		ECReg(EC_Register_Base_Address+0x1302)

/* KBC Host Interface Keyboard/Mouse Status Register */
#define KBHISR   		ECReg(EC_Register_Base_Address+0x1304)

/* KBC Host Interface Keyboard Data Output Register */
#define KBHIKDOR 		ECReg(EC_Register_Base_Address+0x1306)

/* KBC Host Interface Mouse Data Output Register */
#define KBHIMDOR  		ECReg(EC_Register_Base_Address+0x1308)

/* KBC Host Interface Keyboard/Mouse Data Input Register */
#define KBHIDIR   		ECReg(EC_Register_Base_Address+0x130A)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* KBC Host Interface Control Register */
/* PM Channel 1 Input Buffer Full 8032 Interrupt Enable */
#define	PM1ICIE	    BIT(6)
/* PM Channel 1 Output Buffer Empty 8032 Interrupt Enable */
#define	PM1OCIE	    BIT(5)
/* PM Channel 1 Host Interface Interrupt Enable */
#define	PM1HIE	    BIT(4)
/* Input Buffer Full 8032 Interrupt Enable */
#define	IBFCIE	    BIT(3)
/* Output Buffer Empty 8032 Interrupt Enable */
#define OBECIE	    BIT(2)
/* Output Buffer Full Mouse Interrupt Enable */
#define	OBFMIE	    BIT(1)
/* Output Buffer Full Keyboard Interrupt Enable */
#define	OBFKIE 	    BIT(0)

/* KBC Interrupt Control Register */
#define	IRQNP	    BIT(6)  /* Interrupt Negative Polarity */
#define	IRQ11B	    BIT(2)  /* IRQ11 Control Bit */
#define	IRQ12B	    BIT(1)  /* IRQ12 Control Bit */
#define	IRQ1B	    BIT(0)  /* IRQ1 Control Bit */

/* KBC Host Interface Keyboard/Mouse Status Register */
#define PARE        BIT(7)  /* Programming Data */
#define GTIM        BIT(6)  /* Programming Data */
#define AOBF        BIT(5)  /* Programming Data */
#define KEYL        BIT(4)  /* Programming Data */
#define C_D         BIT(3)  /* Command or data port */
#define SYSF        BIT(2)  /* Programming Data (System flag) */
#define	IBF		    BIT(1)  /* Input Buffer Full */
#define	OBF		    BIT(0)  /* Output Buffer Full */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (14xxh) System Wake-Up Control (SWUC)
 * ****************************************************************************
 */
/* SWUC Control Status 1 Register */
#define SWCTL1			ECReg(EC_Register_Base_Address+0x1400)

/* SWUC Control Status 2 Register */
#define SWCTL2			ECReg(EC_Register_Base_Address+0x1402)

/* SWUC Control Status 3 Register */
#define SWCTL3			ECReg(EC_Register_Base_Address+0x1404)

/* SWUC Host Configuration Base Address Low Byte Register */
#define SWCBALR			ECReg(EC_Register_Base_Address+0x1408)

/* SWUC Host Configuration Base Address High Byte Register */
#define SWCBAHR			ECReg(EC_Register_Base_Address+0x140A)

/* SWUC Interrupt Enable Register */
#define SWCIER     		ECReg(EC_Register_Base_Address+0x140C)

/* SWUC Host Event Status Register */
#define SWCHSTR    		ECReg(EC_Register_Base_Address+0x140E)

/* SWUC Host Event Interrupt Enable Register */
#define SWCHIER  		ECReg(EC_Register_Base_Address+0x1410)

/* 
 * ****************************************************************************
 * (15xxh) Power Management Channel (PMC)
 * ****************************************************************************
 */
/* PM1 */
/* Host Interface PM Channel 1 Status */
#define PM1STS   		ECReg(EC_Register_Base_Address+0x1500)

/* Host Interface PM Channel 1 Data Out Port */
#define PM1DO      		ECReg(EC_Register_Base_Address+0x1501)

/* Host Interface PM Channel 1 Data Out Port with SCI */
#define PM1DOSCI  		ECReg(EC_Register_Base_Address+0x1502)

/* Host Interface PM Channel 1 Data Out Port with SMI */
#define PM1DOSMI  		ECReg(EC_Register_Base_Address+0x1503)

/* Host Interface PM Channel 1 Data In Port */
#define PM1DI			ECReg(EC_Register_Base_Address+0x1504)

/* Host Interface PM Channel 1 Data In Port with SCI */
#define PM1DISCI   		ECReg(EC_Register_Base_Address+0x1505)

/* Host Interface PM Channel 1 Control */
#define PM1CTL     		ECReg(EC_Register_Base_Address+0x1506)

/* Host Interface PM Channel 1 Interrupt Control */
#define PM1IC     		ECReg(EC_Register_Base_Address+0x1507)

/* Host Interface PM Channel 1 Interrupt Enable */
#define PM1IE    		ECReg(EC_Register_Base_Address+0x1508)

/* PM2 */
/* Host Interface PM Channel 2 Status */
#define PM2STS    		ECReg(EC_Register_Base_Address+0x1510)

/* Host Interface PM Channel 2 Data Out Port */
#define PM2DO     		ECReg(EC_Register_Base_Address+0x1511)

/* Host Interface PM Channel 2 Data Out Port with SCI */
#define PM2DOSCI   		ECReg(EC_Register_Base_Address+0x1512)

/* Host Interface PM Channel 2 Data Out Port with SMI */
#define PM2DOSMI  		ECReg(EC_Register_Base_Address+0x1513)

/* Host Interface PM Channel 2 Data In Port */
#define PM2DI     		ECReg(EC_Register_Base_Address+0x1514)

/* Host Interface PM Channel 2 Data In Port with SCI */
#define PM2DISCI   		ECReg(EC_Register_Base_Address+0x1515)

/* Host Interface PM Channel 2 Control */
#define PM2CTL     		ECReg(EC_Register_Base_Address+0x1516)

/* Host Interface PM Channel 2 Interrupt Control */
#define PM2IC     		ECReg(EC_Register_Base_Address+0x1517)

/* Host Interface PM Channel 2 Interrupt Enable */
#define PM2IE     		ECReg(EC_Register_Base_Address+0x1518)

/* Mailbox */
/* Mailbox Control */
#define	MBXCTRL			ECReg(EC_Register_Base_Address+0x1519)
/* 16-byte PMC2EX Mailbox 0 ~ Mailbox 15 */
/* 16-byte PMC2EX Mailbox 0 */
#define MBXEC_00        ECReg(EC_Register_Base_Address+0x15F0)

/* 16-byte PMC2EX Mailbox 1 */
#define MBXEC_01        ECReg(EC_Register_Base_Address+0x15F1)

/* 16-byte PMC2EX Mailbox 2 */
#define MBXEC_02        ECReg(EC_Register_Base_Address+0x15F2)

/* 16-byte PMC2EX Mailbox 3 */
#define MBXEC_03        ECReg(EC_Register_Base_Address+0x15F3)

/* 16-byte PMC2EX Mailbox 4 */
#define MBXEC_04        ECReg(EC_Register_Base_Address+0x15F4)

/* 16-byte PMC2EX Mailbox 5 */
#define MBXEC_05        ECReg(EC_Register_Base_Address+0x15F5)

/* 16-byte PMC2EX Mailbox 6 */
#define MBXEC_06        ECReg(EC_Register_Base_Address+0x15F6)

/* 16-byte PMC2EX Mailbox 7 */
#define MBXEC_07        ECReg(EC_Register_Base_Address+0x15F7)

/* 16-byte PMC2EX Mailbox 8 */
#define MBXEC_08        ECReg(EC_Register_Base_Address+0x15F8)

/* 16-byte PMC2EX Mailbox 9 */
#define MBXEC_09        ECReg(EC_Register_Base_Address+0x15F9)

/* 16-byte PMC2EX Mailbox 10 */
#define MBXEC_10        ECReg(EC_Register_Base_Address+0x15FA)

/* 16-byte PMC2EX Mailbox 11 */
#define MBXEC_11        ECReg(EC_Register_Base_Address+0x15FB)

/* 16-byte PMC2EX Mailbox 12 */
#define MBXEC_12        ECReg(EC_Register_Base_Address+0x15FC)

/* 16-byte PMC2EX Mailbox 13 */
#define MBXEC_13        ECReg(EC_Register_Base_Address+0x15FD)

/* 16-byte PMC2EX Mailbox 14 */
#define MBXEC_14        ECReg(EC_Register_Base_Address+0x15FE)

/* 16-byte PMC2EX Mailbox 15 */
#define MBXEC_15        ECReg(EC_Register_Base_Address+0x15FF)

/* Host Interface PM Channel 3 Status */
#define PM3STS    		ECReg(EC_Register_Base_Address+0x1520)

/* Host Interface PM Channel 3 Data Out Port */
#define PM3DO     		ECReg(EC_Register_Base_Address+0x1521)

/* Host Interface PM Channel 3 Data In Port */
#define PM3DI     		ECReg(EC_Register_Base_Address+0x1522)

/* Host Interface PM Channel 3 Control */
#define PM3CTL     		ECReg(EC_Register_Base_Address+0x1523)

/* Host Interface PM Channel 3 Interrupt Control */
#define PM3IC     		ECReg(EC_Register_Base_Address+0x1524)

/* Host Interface PM Channel 3 Interrupt Enable */
#define PM3IE     		ECReg(EC_Register_Base_Address+0x1525)

/* Host Interface PM Channel 4 Status */
#define PM4STS    		ECReg(EC_Register_Base_Address+0x1530)

/* Host Interface PM Channel 4 Data Out Port */
#define PM4DO     		ECReg(EC_Register_Base_Address+0x1531)

/* Host Interface PM Channel 4 Data In Port */
#define PM4DI     		ECReg(EC_Register_Base_Address+0x1532)

/* Host Interface PM Channel 4 Control */
#define PM4CTL     		ECReg(EC_Register_Base_Address+0x1533)

/* Host Interface PM Channel 4 Interrupt Control */
#define PM4IC     		ECReg(EC_Register_Base_Address+0x1534)

/* Host Interface PM Channel 4 Interrupt Enable */
#define PM4IE     		ECReg(EC_Register_Base_Address+0x1535)

/* Host Interface PM Channel 5 Status */
#define PM5STS          ECReg(EC_Register_Base_Address+0x1540)

/* Host Interface PM Channel 5 Data Out Port */
#define PM5DO           ECReg(EC_Register_Base_Address+0x1541)

/* Host Interface PM Channel 5 Data In Port */
#define PM5DI           ECReg(EC_Register_Base_Address+0x1542)

/* Host Interface PM Channel 5 Control */
#define PM5CTL          ECReg(EC_Register_Base_Address+0x1543)

/* Host Interface PM Channel 5 Interrupt Control */
#define PM5IC           ECReg(EC_Register_Base_Address+0x1544)

/* Host Interface PM Channel 5 Interrupt Enable */
#define PM5IE           ECReg(EC_Register_Base_Address+0x1545)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* PM Status Register */
#define SMIEVT          BIT(6)
#define SCIEVT   	    BIT(5)
#define BURST  		    BIT(4)
#define	P_C_D		    BIT(3)	    /* Command or data port */
#define	P_IBF		    BIT(1)	    /* Input Buffer Full */
#define	P_OBF	        BIT(0)	    /* Output Buffer Full */

/* PM Control */
#define	APM			    BIT(7)	    /* Enhance PM Mode */
#define SCINP		    BIT(6)	    /* SCI Negative Polarity */
#define	OBEIE		    BIT(1)	    /* Output Buffer Empty Interrupt Enable */
#define	IBFIE		    BIT(0)	    /* Input Buffer Full Interrupt Enable */

/* PM Interrupt Control */
#define	SMINP	        BIT(6)	    /* SMI Negative Polarity */
#define SCIB		    BIT(2)	    /* Host SCI Control Bit */
#define	SMIB	        BIT(1)	    /* Host SMI Control Bit */
#define	IRQB		    BIT(0)	    /* Host IRQ Control Bit */

/* PM Interrupt Enable */
#define HWSMIEN		    BIT(5)	    /* Hardware SMI Enable */
#define	HWSCIEN		    BIT(4)	    /* Hardware SCI Enable */
#define	HWIRQEN		    BIT(3)      /* Hardware IRQ Enable */
#define SMIEN		    BIT(2)	    /* SMI Enable */
#define SCIEN		    BIT(1)	    /* SCI Enable */
#define IRQEN		    BIT(0)	    /* IRQ Enable */

/* PM Interrupt Enable */
#define	MBXEN		    BIT(7)	    /* Mailbox Enable */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (16XXh) General Purpose I/O Control Register
 * ****************************************************************************
 */
/* General Control Register */
#define	GCR     		ECReg(EC_Register_Base_Address+0x1600)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	GFLE		    BIT(7)	    /* GPB5 Follow LPCRST# Enable */
#define	WUI7EN		    BIT(6)	    /* WUI7 Enabled */
#define	WUI6EN		    BIT(5)	    /* WUI6 Enabled */
//		-			    BIT(4)	    /* - */
//		-			    BIT(3)	    /* - */
#define	LPCRSTEN_GPB7   BIT(2)	    /* LPC Reset is enabled on GPB7 */
#define	LPCRSTEN_GPD2   BIT(1)	    /* LPC Reset is enabled on GPD2 */
//		-			    BIT(0)	    /* - */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 1 Register */
#define	GCR29     		ECReg(EC_Register_Base_Address+0x16EE)

#define	GCR1     		ECReg(EC_Register_Base_Address+0x16F0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SPICTRL_0       BIT(4)
#define SPICTRL_1       BIT(5)      /* SPI interface control */
#define SSSPIBP         BIT(6)      /* Support SSPI BUSY Pin */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 2 Register */
#define	GCR2     		ECReg(EC_Register_Base_Address+0x16F1)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define CK32OE	        BIT(6)	    /* CK32K Out Enable */
#define SMB4E	        BIT(5)      /* SMBus Channel 4 Enable */
#define PECIE	        BIT(4)	    /* PECI Enable */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 3 Register */
#define	GCR3     		ECReg(EC_Register_Base_Address+0x16F2)

/* General Control 4 Register */
#define	GCR4     		ECReg(EC_Register_Base_Address+0x16F3)

/* General Control 5 Register */
#define	GCR5     		ECReg(EC_Register_Base_Address+0x16F4)

/* General Control 6 Register */
#define	GCR6     		ECReg(EC_Register_Base_Address+0x16F5)

/* General Control 7 Register */
#define	GCR7     		ECReg(EC_Register_Base_Address+0x16F6)

/* General Control 8 Register */
#define GCR8            ECReg(EC_Register_Base_Address+0x16F7)
	#define  PWSW2EN    BIT(4)    // defautlt:(1):Dedicate PWRSW to timeout reset function.

/* General Control 9 Register */
#define GCR9            ECReg(EC_Register_Base_Address+0x16F8)
    #define  PWSWDT2GPEN BIT(3)
	#define  PWSW1EN2	BIT(2)
	#define  PWSW1EN1	BIT(1)

/* General Control 10 Register */
#define GCR10           ECReg(EC_Register_Base_Address+0x16F9)

/* General Control 11 Register */
#define GCR11           ECReg(EC_Register_Base_Address+0x16FA)
	#define  PWSW4S		0x00		// PWSW WDT 4s
	#define  PWSW5S		0x01		// PWSW WDT 5s
	#define  PWSW6s		0x02		// PWSW WDT 6s
	#define  PWSW7s     0x03		// PWSW WDT 7s
    #define  PWSW8s     0x04		// PWSW WDT 8s
/* General Control 12 Register */
#define GCR12           ECReg(EC_Register_Base_Address+0x16FB)

/* General Control 13 Register */
#define GCR13           ECReg(EC_Register_Base_Address+0x16FC)

/* General Control 14 Register */
#define GCR14           ECReg(EC_Register_Base_Address+0x16FD)

/* General Control 15 Register */
#define GCR15           ECReg(EC_Register_Base_Address+0x16FE)

/* General Control 16 Register */
#define GCR16           ECReg(EC_Register_Base_Address+0x16E0)

/* General Control 17 Register */
#define GCR17           ECReg(EC_Register_Base_Address+0x16E1)

/* General Control 18 Register */
#define GCR18           ECReg(EC_Register_Base_Address+0x16E2)

/* General Control 19 Register */
#define GCR19           ECReg(EC_Register_Base_Address+0x16E4)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define GPB5VS	        BIT(7)
#define GPB6VS	        BIT(6)
#define GPC1VS	        BIT(5)
#define GPC2VS	        BIT(4)
#define GPC7VS	        BIT(3)
#define GPD0VS	        BIT(2)
#define GPD1VS	        BIT(1)
#define GPD2VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 20 Register */
#define GCR20           ECReg(EC_Register_Base_Address+0x16E5)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define GPD3VS	        BIT(7)
#define GPD4VS	        BIT(6)
#define GPE0VS	        BIT(5)
#define GPE6VS	        BIT(4)
#define GPE7VS	        BIT(3)
#define GPF2VS	        BIT(2)
#define GPF3VS	        BIT(1)
#define GPF4VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 21 Register */
#define GCR21           ECReg(EC_Register_Base_Address+0x16E6)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define GPF5VS	        BIT(7)
#define GPF6VS	        BIT(6)
#define GPF7VS	        BIT(5)
//#define 	            BIT(4)
//#define   	        BIT(3)
#define GPH0VS	        BIT(2)
#define GPH1VS	        BIT(1)
#define GPH2VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 22 Register */
#define GCR22           ECReg(EC_Register_Base_Address+0x16E7)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define               BIT(7)
//#define               BIT(6)
//#define               BIT(5)
//#define 	            BIT(4)
//#define   	        BIT(3)
//#define               BIT(2)
#define GPB3VS	        BIT(1)
#define GPB4VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 23 Register */
#define GCR23           ECReg(EC_Register_Base_Address+0x16E8)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define GPI4VS	        BIT(7)
#define GPI3VS	        BIT(6)
#define GPI2VS	        BIT(5)
#define GPI1VS	        BIT(4)
#define GPJ3VS	        BIT(3)
#define GPJ2VS	        BIT(2)
#define GPJ1VS	        BIT(1)
#define GPJ0VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* General Control 24 Register */
#define GCR24           ECReg(EC_Register_Base_Address+0x16E9)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define               BIT(7)
//#define               BIT(6)
//#define               BIT(5)
//#define 	            BIT(4)
//#define   	        BIT(3)
//#define               BIT(2)
#define GPA5VS	        BIT(1)
#define GPA4VS	        BIT(0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define	GPDRA 			ECReg(EC_Register_Base_Address+0x1601)

/* Port B Data Register */
#define	GPDRB 			ECReg(EC_Register_Base_Address+0x1602)

/* Port C Data Register */
#define	GPDRC 			ECReg(EC_Register_Base_Address+0x1603)

/* Port D Data Register */
#define	GPDRD 			ECReg(EC_Register_Base_Address+0x1604)

/* Port E Data Register */
#define	GPDRE 			ECReg(EC_Register_Base_Address+0x1605)

/* Port F Data Register */
#define	GPDRF 			ECReg(EC_Register_Base_Address+0x1606)

/* Port G Data Register */
#define	GPDRG 			ECReg(EC_Register_Base_Address+0x1607)

/* Port H Data Register */
#define	GPDRH 			ECReg(EC_Register_Base_Address+0x1608)

/* Port I Data Register */
#define	GPDRI 			ECReg(EC_Register_Base_Address+0x1609)

/* Port J Data Register */
#define	GPDRJ 			ECReg(EC_Register_Base_Address+0x160A)

/* Port J Data Register */
#define	GPDRK 			ECReg(EC_Register_Base_Address+0x160B)	// Alex 

/* Port J Data Register */
#define	GPDRL 			ECReg(EC_Register_Base_Address+0x160C)	// Alex 

/* Port M Data Register */
#define	GPDRM 			ECReg(EC_Register_Base_Address+0x160D)

/* GPIO control register */
/* Port Control A0 Registers */
#define GPCRA0			ECReg(EC_Register_Base_Address+0x1610)

/* Port Control A1 Registers */
#define GPCRA1			ECReg(EC_Register_Base_Address+0x1611)

/* Port Control A3 Registers */
#define GPCRA2			ECReg(EC_Register_Base_Address+0x1612)

/* Port Control A3 Registers */
#define GPCRA3			ECReg(EC_Register_Base_Address+0x1613)

/* Port Control A4 Registers */
#define GPCRA4			ECReg(EC_Register_Base_Address+0x1614)

/* Port Control A5 Registers */
#define GPCRA5			ECReg(EC_Register_Base_Address+0x1615)

/* Port Control A6 Registers */
#define GPCRA6			ECReg(EC_Register_Base_Address+0x1616)

/* Port Control A7 Registers */
#define GPCRA7			ECReg(EC_Register_Base_Address+0x1617)

/* Port Control B0 Registers */
#define GPCRB0			ECReg(EC_Register_Base_Address+0x1618)

/* Port Control B1 Registers */
#define GPCRB1			ECReg(EC_Register_Base_Address+0x1619)

/* Port Control B2 Registers */
#define GPCRB2			ECReg(EC_Register_Base_Address+0x161A)

/* Port Control B3 Registers */
#define GPCRB3			ECReg(EC_Register_Base_Address+0x161B)

/* Port Control B4 Registers */
#define GPCRB4			ECReg(EC_Register_Base_Address+0x161C)

/* Port Control B5 Registers */
#define GPCRB5			ECReg(EC_Register_Base_Address+0x161D)

/* Port Control B6 Registers */
#define GPCRB6			ECReg(EC_Register_Base_Address+0x161E)

/* Port Control B7 Registers */
#define GPCRB7			ECReg(EC_Register_Base_Address+0x161F)

/* Port Control C0 Registers */
#define GPCRC0			ECReg(EC_Register_Base_Address+0x1620)

/* Port Control C1 Registers */
#define GPCRC1			ECReg(EC_Register_Base_Address+0x1621)

/* Port Control C2 Registers */
#define GPCRC2			ECReg(EC_Register_Base_Address+0x1622)

/* Port Control C3 Registers */
#define GPCRC3			ECReg(EC_Register_Base_Address+0x1623)

/* Port Control C4 Registers */
#define GPCRC4			ECReg(EC_Register_Base_Address+0x1624)

/* Port Control C5 Registers */
#define GPCRC5			ECReg(EC_Register_Base_Address+0x1625)

/* Port Control C6 Registers */
#define GPCRC6			ECReg(EC_Register_Base_Address+0x1626)

/* Port Control C7 Registers */
#define GPCRC7			ECReg(EC_Register_Base_Address+0x1627)

/* Port Control D0 Registers */
#define GPCRD0			ECReg(EC_Register_Base_Address+0x1628)

/* Port Control D1 Registers */
#define GPCRD1			ECReg(EC_Register_Base_Address+0x1629)

/* Port Control D2 Registers */
#define GPCRD2			ECReg(EC_Register_Base_Address+0x162A)

/* Port Control D3 Registers */
#define GPCRD3			ECReg(EC_Register_Base_Address+0x162B)

/* Port Control D4 Registers */
#define GPCRD4			ECReg(EC_Register_Base_Address+0x162C)

/* Port Control D5 Registers */
#define GPCRD5			ECReg(EC_Register_Base_Address+0x162D)

/* Port Control D6 Registers */
#define GPCRD6			ECReg(EC_Register_Base_Address+0x162E)

/* Port Control D7 Registers */
#define GPCRD7			ECReg(EC_Register_Base_Address+0x162F)

/* Port Control E0 Registers */
#define GPCRE0			ECReg(EC_Register_Base_Address+0x1630)

/* Port Control E1 Registers */
#define GPCRE1			ECReg(EC_Register_Base_Address+0x1631)

/* Port Control E2 Registers */
#define GPCRE2			ECReg(EC_Register_Base_Address+0x1632)

/* Port Control E3 Registers */
#define GPCRE3			ECReg(EC_Register_Base_Address+0x1633)

/* Port Control E4 Registers */
#define GPCRE4			ECReg(EC_Register_Base_Address+0x1634)

/* Port Control E5 Registers */
#define GPCRE5			ECReg(EC_Register_Base_Address+0x1635)

/* Port Control E6 Registers */
#define GPCRE6			ECReg(EC_Register_Base_Address+0x1636)

/* Port Control E7 Registers */
#define GPCRE7			ECReg(EC_Register_Base_Address+0x1637)

/* Port Control F0 Registers */
#define GPCRF0			ECReg(EC_Register_Base_Address+0x1638)

/* Port Control F1 Registers */
#define GPCRF1			ECReg(EC_Register_Base_Address+0x1639)

/* Port Control F2 Registers */
#define GPCRF2			ECReg(EC_Register_Base_Address+0x163A)

/* Port Control F3 Registers */
#define GPCRF3			ECReg(EC_Register_Base_Address+0x163B)

/* Port Control F4 Registers */
#define GPCRF4			ECReg(EC_Register_Base_Address+0x163C)

/* Port Control F5 Registers */
#define GPCRF5			ECReg(EC_Register_Base_Address+0x163D)

/* Port Control F6 Registers */
#define GPCRF6			ECReg(EC_Register_Base_Address+0x163E)

/* Port Control F7 Registers */
#define GPCRF7			ECReg(EC_Register_Base_Address+0x163F)

/* Port Control G0 Registers */
#define GPCRG0			ECReg(EC_Register_Base_Address+0x1640)

/* Port Control G1 Registers */
#define GPCRG1			ECReg(EC_Register_Base_Address+0x1641)

/* Port Control G2 Registers */
#define GPCRG2			ECReg(EC_Register_Base_Address+0x1642)

/* Port Control G3 Registers */
#define GPCRG3			ECReg(EC_Register_Base_Address+0x1643)

/* Port Control G4 Registers */
#define GPCRG4			ECReg(EC_Register_Base_Address+0x1644)

/* Port Control G5 Registers */
#define GPCRG5			ECReg(EC_Register_Base_Address+0x1645)

/* Port Control G6 Registers */
#define GPCRG6			ECReg(EC_Register_Base_Address+0x1646)

/* Port Control G7 Registers */
#define GPCRG7			ECReg(EC_Register_Base_Address+0x1647)

/* Port Control H0 Registers */
#define GPCRH0			ECReg(EC_Register_Base_Address+0x1648)

/* Port Control H1 Registers */
#define GPCRH1			ECReg(EC_Register_Base_Address+0x1649)

/* Port Control H2 Registers */
#define GPCRH2			ECReg(EC_Register_Base_Address+0x164A)

/* Port Control H3 Registers */
#define GPCRH3			ECReg(EC_Register_Base_Address+0x164B)

/* Port Control H4 Registers */
#define GPCRH4			ECReg(EC_Register_Base_Address+0x164C)

/* Port Control H5 Registers */
#define GPCRH5			ECReg(EC_Register_Base_Address+0x164D)

/* Port Control H6 Registers */
#define GPCRH6			ECReg(EC_Register_Base_Address+0x164E)

/* Port Control I0 Registers */
#define GPCRI0			ECReg(EC_Register_Base_Address+0x1650)

/* Port Control I1 Registers */
#define GPCRI1			ECReg(EC_Register_Base_Address+0x1651)

/* Port Control I2 Registers */
#define GPCRI2			ECReg(EC_Register_Base_Address+0x1652)

/* Port Control I3 Registers */
#define GPCRI3			ECReg(EC_Register_Base_Address+0x1653)

/* Port Control I4 Registers */
#define GPCRI4			ECReg(EC_Register_Base_Address+0x1654)

/* Port Control I5 Registers */
#define GPCRI5			ECReg(EC_Register_Base_Address+0x1655)

/* Port Control I6 Registers */
#define GPCRI6			ECReg(EC_Register_Base_Address+0x1656)

/* Port Control I7 Registers */
#define GPCRI7			ECReg(EC_Register_Base_Address+0x1657)

/* Port Control J0 Registers */
#define GPCRJ0			ECReg(EC_Register_Base_Address+0x1658)

/* Port Control J1 Registers */
#define GPCRJ1			ECReg(EC_Register_Base_Address+0x1659)

/* Port Control J2 Registers */
#define GPCRJ2			ECReg(EC_Register_Base_Address+0x165A)

/* Port Control J3 Registers */
#define GPCRJ3			ECReg(EC_Register_Base_Address+0x165B)

/* Port Control J4 Registers */
#define GPCRJ4			ECReg(EC_Register_Base_Address+0x165C)

/* Port Control J5 Registers */
#define GPCRJ5			ECReg(EC_Register_Base_Address+0x165D)

/* Port Control J6 Registers */
#define GPCRJ6			ECReg(EC_Register_Base_Address+0x165E)

/* Port Control J7 Registers */
#define GPCRJ7			ECReg(EC_Register_Base_Address+0x165F)

/* Port Control J0 Registers */
#define GPCRK0			ECReg(EC_Register_Base_Address+0x1690)

/* Port Control J1 Registers */
#define GPCRK1			ECReg(EC_Register_Base_Address+0x1691)

/* Port Control J2 Registers */
#define GPCRK2			ECReg(EC_Register_Base_Address+0x1692)

/* Port Control J3 Registers */
#define GPCRK3			ECReg(EC_Register_Base_Address+0x1693)

/* Port Control J4 Registers */
#define GPCRK4			ECReg(EC_Register_Base_Address+0x1694)

/* Port Control J5 Registers */
#define GPCRK5			ECReg(EC_Register_Base_Address+0x1695)

/* Port Control J6 Registers */
#define GPCRK6			ECReg(EC_Register_Base_Address+0x1696)

/* Port Control J7 Registers */
#define GPCRK7			ECReg(EC_Register_Base_Address+0x1697)


/* Port Control J0 Registers */
#define GPCRL0			ECReg(EC_Register_Base_Address+0x1698)

/* Port Control J1 Registers */
#define GPCRL1			ECReg(EC_Register_Base_Address+0x1699)

/* Port Control J2 Registers */
#define GPCRL2			ECReg(EC_Register_Base_Address+0x169A)

/* Port Control J3 Registers */
#define GPCRL3			ECReg(EC_Register_Base_Address+0x169B)

/* Port Control J4 Registers */
#define GPCRL4			ECReg(EC_Register_Base_Address+0x169C)

/* Port Control J5 Registers */
#define GPCRL5			ECReg(EC_Register_Base_Address+0x169D)

/* Port Control J6 Registers */
#define GPCRL6			ECReg(EC_Register_Base_Address+0x169E)

/* Port Control J7 Registers */
#define GPCRL7			ECReg(EC_Register_Base_Address+0x169F)

/* Port Control M0 Registers */
#define GPCRM0			ECReg(EC_Register_Base_Address+0x16A0)

/* Port Control M1 Registers */
#define GPCRM1			ECReg(EC_Register_Base_Address+0x16A1)

/* Port Control M2 Registers */
#define GPCRM2			ECReg(EC_Register_Base_Address+0x16A2)

/* Port Control M3 Registers */
#define GPCRM3			ECReg(EC_Register_Base_Address+0x16A3)

/* Port Control M4 Registers */
#define GPCRM4			ECReg(EC_Register_Base_Address+0x16A4)

/* Port Control M5 Registers */
#define GPCRM5			ECReg(EC_Register_Base_Address+0x16A5)

/* Port Control M6 Registers */
#define GPCRM6			ECReg(EC_Register_Base_Address+0x16A6)

/* Port Data Mirror Register */
/* Port A Data Mirror Register */
#define GPDMRA			ECReg(EC_Register_Base_Address+0x1661)

/* Port B Data Mirror Register */
#define GPDMRB			ECReg(EC_Register_Base_Address+0x1662)

/* Port C Data Mirror Register */
#define GPDMRC			ECReg(EC_Register_Base_Address+0x1663)

/* Port D Data Mirror Register */
#define GPDMRD			ECReg(EC_Register_Base_Address+0x1664)

/* Port E Data Mirror Register */
#define GPDMRE			ECReg(EC_Register_Base_Address+0x1665)

/* Port F Data Mirror Register */
#define GPDMRF			ECReg(EC_Register_Base_Address+0x1666)

/* Port G Data Mirror Register */
#define GPDMRG			ECReg(EC_Register_Base_Address+0x1667)

/* Port H Data Mirror Register */
#define GPDMRH			ECReg(EC_Register_Base_Address+0x1668)

/* Port I Data Mirror Register */
#define GPDMRI			ECReg(EC_Register_Base_Address+0x1669)

/* Port J Data Mirror Register */
#define GPDMRJ			ECReg(EC_Register_Base_Address+0x166A)

/* Port M Data Mirror Register */
#define GPDMRM			ECReg(EC_Register_Base_Address+0x166D)

/* Output Type A Register */
#define GPOTA			ECReg(EC_Register_Base_Address+0x1671)

/* Output Type B Register */
#define GPOTB			ECReg(EC_Register_Base_Address+0x1672)

/* Output Type D Register */
#define GPOTD			ECReg(EC_Register_Base_Address+0x1674)

/* Output Type E Register */
#define GPOTE			ECReg(EC_Register_Base_Address+0x1675)

/* Output Type F Register */
#define GPOTF           ECReg(EC_Register_Base_Address+0x1676)

/* Output Type H Register */
#define GPOTH           ECReg(EC_Register_Base_Address+0x1678)

/* Output Type J Register */
#define GPOTJ           ECReg(EC_Register_Base_Address+0x167A)

/* 
 * ****************************************************************************
 * (17XXh) PS/2 Interface Register
 * ****************************************************************************
 */
/* PS/2 Control Register 1 */
#define PSCTL1    		ECReg(EC_Register_Base_Address+0x1700)

/* PS/2 Control Register 2 */
#define PSCTL2			ECReg(EC_Register_Base_Address+0x1701)

/* PS/2 Control Register 3 */
#define PSCTL3     		ECReg(EC_Register_Base_Address+0x1702)

/* PS/2 Interrupt Control Register 1 */
#define PSINT1   		ECReg(EC_Register_Base_Address+0x1704)

/* PS/2 Interrupt Control Register 2 */
#define PSINT2    		ECReg(EC_Register_Base_Address+0x1705)

/* PS/2 Interrupt Control Register 3 */
#define PSINT3    		ECReg(EC_Register_Base_Address+0x1706)

/* PS/2 Status Register 1 */
#define PSSTS1    		ECReg(EC_Register_Base_Address+0x1708)

/* PS/2 Status Register 2 */
#define PSSTS2    		ECReg(EC_Register_Base_Address+0x1709)

/* PS/2 Status Register 3 */
#define PSSTS3    		ECReg(EC_Register_Base_Address+0x170A)

/* PS/2 Data Register 1 */
#define PSDAT1    		ECReg(EC_Register_Base_Address+0x170C)

/* PS/2 Data Register 2 */
#define PSDAT2     		ECReg(EC_Register_Base_Address+0x170D)

/* PS/2 Data Register 3 */
#define	PSDAT3 			ECReg(EC_Register_Base_Address+0x170E)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* PS/2 Control Register */
#define	DCEN	        BIT(4)	    /* Debounce Circuit Enable */
#define	TRMS		    BIT(3)	    /* Transmit,Receive Mode Selection */
#define	PSHE		    BIT(2)	    /* PS/2 Hardware Enable */
#define	CCLK	        BIT(1)	    /* Control CLK Line */
#define	CDAT	        BIT(0)	    /* Control DATA Line */

/* PS/2 Interrupt Control Register */
#define	TDIE		    BIT(2)	    /* Transaction Done Interrupt Enable */
#define	SIE			    BIT(1)	    /* Start Interrupt Enable */
#define	SMIE	        BIT(0)	    /* Software Mode Interrupt Enable */

/* PS/2 Status Register */
#define	FER			    BIT(5)	    /* Frame Error */
#define	PER			    BIT(4)	    /* Parity Error */
#define	TDS			    BIT(3)	    /* Transaction Done Status */
#define	SS			    BIT(2)	    /* Start Status */
#define	CLS			    BIT(1)	    /* CLK Line Status */
#define	DLS			    BIT(0)	    /* DATA Line Status */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (18xxh) PWM & SmartAuto Fan Control (PWM)
 * ****************************************************************************
 */
/* Channel 0 Clock Prescaler Register */
#define	C0CPRS  		ECReg(EC_Register_Base_Address+0x1800)

/* Cycle Time 0 */
#define	CTR       		ECReg(EC_Register_Base_Address+0x1801)

/* Channel 0 PWM Duty Cycle */
#define	DCR0     		ECReg(EC_Register_Base_Address+0x1802)

/* Channel 1 PWM Duty Cycle */
#define	DCR1      		ECReg(EC_Register_Base_Address+0x1803)

/* Channel 2 PWM Duty Cycle */
#define	DCR2     		ECReg(EC_Register_Base_Address+0x1804)

/* Channel 3 PWM Duty Cycle */
#define	DCR3       		ECReg(EC_Register_Base_Address+0x1805)

/* Channel 4 PWM Duty Cycle */
#define	DCR4      		ECReg(EC_Register_Base_Address+0x1806)

/* Channel 5 PWM Duty Cycle */
#define	DCR5      		ECReg(EC_Register_Base_Address+0x1807)

/* Channel 6 PWM Duty Cycle */
#define	DCR6       		ECReg(EC_Register_Base_Address+0x1808)

/* Channel 7 PWM Duty Cycle */
#define	DCR7     		ECReg(EC_Register_Base_Address+0x1809)

/* PWM Polarity */
#define	PWMPOL        	ECReg(EC_Register_Base_Address+0x180A)

/* Prescaler Clock Frequency Select Register */
#define	PCFSR      		ECReg(EC_Register_Base_Address+0x180B)

/* Prescaler Clock Source Select Group Low */
#define	PCSSGL   		ECReg(EC_Register_Base_Address+0x180C)

/* Prescaler Clock Source Select Group High */
#define	PCSSGH    		ECReg(EC_Register_Base_Address+0x180D)

/* CR256 Prescaler Clock Source Select Group */
#define	CR256PCSSG		ECReg(EC_Register_Base_Address+0x180E)

/* Prescaler Clock Source Gating Register */
#define	PCSGR			ECReg(EC_Register_Base_Address+0x180F)

/* Fan 1 Tachometer LSB Reading */
#define	F1TLRR     		ECReg(EC_Register_Base_Address+0x181E)

/* Fan 1 Tachometer MSB Reading */
#define	F1TMRR    		ECReg(EC_Register_Base_Address+0x181F)

/* Fan 2 Tachometer LSB Reading */
#define	F2TLRR    		ECReg(EC_Register_Base_Address+0x1820)

/* Fan 2 Tachometer MSB Reading */
#define	F2TMRR     		ECReg(EC_Register_Base_Address+0x1821)

/* Zone Interrupt Status Control */
#define	ZINTSCR   		ECReg(EC_Register_Base_Address+0x1822)

/* PWM Clock Control Register */
#define	ZTIER      		ECReg(EC_Register_Base_Address+0x1823)

/* Tachometer Switch Control Register */
#define	TSWCTLR    		ECReg(EC_Register_Base_Address+0x1824)

/* Channel 4 Clock Prescaler Register */
#define	C4CPRS    		ECReg(EC_Register_Base_Address+0x1827)

/* Channel 4 Clock Prescaler MSB Register */
#define	C4MCPRS    		ECReg(EC_Register_Base_Address+0x1828)

/* Channel 6 Clock Prescaler Register */
#define	C6CPRS    		ECReg(EC_Register_Base_Address+0x182B)

/* Channel 6 Clock Prescaler MSB Register */
#define	C6MCPRS    		ECReg(EC_Register_Base_Address+0x182C)

/* Channel 7 Clock Prescaler Register */
#define	C7CPRS     		ECReg(EC_Register_Base_Address+0x182D)

/* Channel 7 Clock Prescaler MSB Register */
#define	C7MCPRS    		ECReg(EC_Register_Base_Address+0x182E)

/* PWM Clock 6MHz Select Register */
#define	CLK6MSEL   		ECReg(EC_Register_Base_Address+0x1840)

/* Cycle Time 1 */
#define	CTR1    		ECReg(EC_Register_Base_Address+0x1841)

/* Cycle Time 2 */
#define	CTR2    		ECReg(EC_Register_Base_Address+0x1842)

/* Cycle Time 3 */
#define	CTR3    		ECReg(EC_Register_Base_Address+0x1843)

/* PWM5 Timeout Control Register */
#define	PWM5TOCTRL		ECReg(EC_Register_Base_Address+0x1844)

/* Color Frequency LSB Register */
#define	CFLRR    		ECReg(EC_Register_Base_Address+0x1845)

/* Color Frequency MSB Register */
#define	CFMRR    		ECReg(EC_Register_Base_Address+0x1846)

/* Color Frequency Interrupt Control Register */
#define	CFINTCTRL		ECReg(EC_Register_Base_Address+0x1847)

/* Tachometer Switch Control Register */
#define	TSWCTRL    		ECReg(EC_Register_Base_Address+0x1848)

/* PWM Output Open-Drain Enable Register */
#define	PWMODENR		ECReg(EC_Register_Base_Address+0x1849)

#define T0DIE           BIT(5) /* Enable Tach0A/Tach0B Interrupt */
#define T0DIC           BIT(4) /* Tach0A/Tach0B Interrupt Clear */
#define T0DIS           BIT(3) /* Tach0A/Tach0B Interrupt Status */
#define T1DIE           BIT(2) /* Enable Tach1A/Tach1B Interrupt */
#define T1DIC           BIT(1) /* Tach1A/Tach1B Interrupt Clear */
#define T1DIS           BIT(0) /* Tach1A/Tach1B Interrupt Status */

/* Backlight Duty Register for Color Sensor */
#define BDRCS 			ECReg(EC_Register_Base_Address+0x184C)

/* 
 * ****************************************************************************
 * (19xxh) Analog to Digital converter (ADC)
 * ****************************************************************************
 */
/* ADC Status */
#define	ADCSTS			ECReg(EC_Register_Base_Address+0x1900)

/* ADC Configuration */
#define	ADCCFG			ECReg(EC_Register_Base_Address+0x1901)

/* ADC Clock Control */
#define	ADCCTL			ECReg(EC_Register_Base_Address+0x1902)

/* ADC General Control Register */
#define ADCGCR          ECReg(EC_Register_Base_Address+0x1903)

/* Voltage Channel 0 Channel Control */
#define	VCH0CTL			ECReg(EC_Register_Base_Address+0x1904)

/* Calibration Data Control Register */
#define	KDCTL			ECReg(EC_Register_Base_Address+0x1905)

/* Voltage Channel 1 Control */
#define	VCH1CTL      	ECReg(EC_Register_Base_Address+0x1906)

/* Voltage Channel 1 Data Buffer LSB */
#define	VCH1DATL     	ECReg(EC_Register_Base_Address+0x1907)

/* Voltage Channel 1 Data Buffer MSB */
#define	VCH1DATM       	ECReg(EC_Register_Base_Address+0x1908)

/* Voltage Channel 2 Control */
#define	VCH2CTL        	ECReg(EC_Register_Base_Address+0x1909)

/* Voltage Channel 2 Data Buffer LSB */
#define	VCH2DATL      	ECReg(EC_Register_Base_Address+0x190A)

/* Voltage Channel 2 Data Buffer MSB */
#define	VCH2DATM     	ECReg(EC_Register_Base_Address+0x190B)

/* Voltage Channel 3 Control */
#define	VCH3CTL       	ECReg(EC_Register_Base_Address+0x190C)

/* Voltage Channel 3 Data Buffer LSB */
#define	VCH3DATL      	ECReg(EC_Register_Base_Address+0x190D)

/* Voltage Channel 3 Data Buffer MSB */
#define	VCH3DATM      	ECReg(EC_Register_Base_Address+0x190E)

/* Voltage High Scale Calibration Data Buffer LSB */
#define	VHSCDBL      	ECReg(EC_Register_Base_Address+0x1914)

/* Voltage High Scale Calibration Data Buffer MSB */
#define	VHSCDBM       	ECReg(EC_Register_Base_Address+0x1915)

/* Voltage Channel 0 Data Buffer LSB */
#define	VCH0DATL		ECReg(EC_Register_Base_Address+0x1918)

/* Voltage Channel 0 Data Buffer MSB */
#define	VCH0DATM		ECReg(EC_Register_Base_Address+0x1919)

/* Voltage High Scale Gain-error Calibration Data Buffer LSB */
#define	VHSGCDBL      	ECReg(EC_Register_Base_Address+0x191C)

/* Voltage High Scale Gain-error Calibration Data Buffer MSB */
#define VHSGCDBM        ECReg(EC_Register_Base_Address+0x191D)

/* Voltage Compare Scan Period */
#define VCMPSCP         ECReg(EC_Register_Base_Address+0x1937)

/* Voltage Channel 4 Control Register */
#define VCH4CTL         ECReg(EC_Register_Base_Address+0x1938)

/* Volt Channel 4 Data Buffer MSB */
#define VCH4DATM        ECReg(EC_Register_Base_Address+0x1939)

/* Volt Channel 4 Data Buffer LSB */
#define VCH4DATL        ECReg(EC_Register_Base_Address+0x193A)

/* Voltage Channel 5 Control Register */
#define VCH5CTL         ECReg(EC_Register_Base_Address+0x193B)

/* Volt Channel 5 Data Buffer MSB */
#define VCH5DATM        ECReg(EC_Register_Base_Address+0x193C)

/* Volt Channel 5 Data Buffer LSB */
#define VCH5DATL        ECReg(EC_Register_Base_Address+0x193D)

/* Voltage Channel 6 Control Register */
#define VCH6CTL         ECReg(EC_Register_Base_Address+0x193E)

/* Volt Channel 6 Data Buffer MSB */
#define VCH6DATM        ECReg(EC_Register_Base_Address+0x193F)

/* Volt Channel 6 Data Buffer LSB */
#define VCH6DATL        ECReg(EC_Register_Base_Address+0x1940)

/* Voltage Channel 7 Control Register */
#define VCH7CTL         ECReg(EC_Register_Base_Address+0x1941)

/* Volt Channel 7 Data Buffer MSB */
#define VCH7DATM        ECReg(EC_Register_Base_Address+0x1942)

/* Volt Channel 7 Data Buffer LSB */
#define VCH7DATL        ECReg(EC_Register_Base_Address+0x1943)

/* ADC Data Vaild Status */
#define ADCDVSTS        ECReg(EC_Register_Base_Address+0x1944)

/* Voltage Compare Status */
#define VCMPSTS         ECReg(EC_Register_Base_Address+0x1945)

/* Voltage Compare Channel 0 Control Register */
#define VCMP0CTL        ECReg(EC_Register_Base_Address+0x1946)

/* Voltage Compare 0 Threshold Data Buffer MSB */
#define CMP0THRDATM     ECReg(EC_Register_Base_Address+0x1947)

/* Voltage Compare 0 Threshold Data Buffer LSB */
#define CMP0THRDATL     ECReg(EC_Register_Base_Address+0x1948)

/* Voltage Compare Channel 1 Control Register */
#define VCMP1CTL        ECReg(EC_Register_Base_Address+0x1949)

/* Voltage Compare 1 Threshold Data Buffer MSB */
#define CMP1THRDATM     ECReg(EC_Register_Base_Address+0x194A)

/* Voltage Compare 1 Threshold Data Buffer LSB */
#define CMP1THRDATL     ECReg(EC_Register_Base_Address+0x194B)

/* Voltage Compare Channel 2 Control Register */
#define VCMP2CTL        ECReg(EC_Register_Base_Address+0x194C)

/* Voltage Compare 2 Threshold Data Buffer MSB */
#define CMP2THRDATM     ECReg(EC_Register_Base_Address+0x194D)

/* Voltage Compare 2 Threshold Data Buffer LSB */
#define CMP2THRDATL     ECReg(EC_Register_Base_Address+0x194E)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ADC Status Register */
#define	FIRHIACC	    BIT(7)  // Filter High Accuracy */
#define AINITB		    BIT(3)  // Analog Accuracy Initialization Bit */
#define	ADCPS		    BIT(2)  // ADC Power Statement */
#define	DOVE		    BIT(1)  // Data Overflow Event */
#define	EOCE	        BIT(0)  // End-of-Cycle Event */

/* ADC Configuration Register */
#define	DFILEN		    BIT(5)  // Digital Filter Enable */
#define	INTECEN		    BIT(2)  // Interrupt from End-of-Cycle Event Enable */
#define	ADCEN		    BIT(0)  // ADC Module Enable */

/* Voltage Channel Control Register */
#define	DATVAL	        BIT(7)  // Data Valid */
#define	INTDVEN		    BIT(5)  // Interrupt from Data Valid Enable */

/* Calibration Data Control Register */
#define	AHCE		    BIT(7)  // Automatic Hardware Calibration Enable */
#define	HCDATVAL	    BIT(5)  // High-Scaler Calibration Data Valid */
#define	GCDATVAL	    BIT(4)  // Gain_Error Calibration Data Valid */
#define	VHSCKE		    BIT(1)  // Volt High Scale Calibration Enable */
#define	GECKE	        BIT(0)  // Gain_Error Calibration Enable */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (1Axxh) Digital to Analog Converter (DAC)
 * ****************************************************************************
 */
/* DAC Power Down Register */
#define	DACPWRDN      	ECReg(EC_Register_Base_Address+0x1A01)

/* DAC Data Channel 0 */
#define	DACDAT0      	ECReg(EC_Register_Base_Address+0x1A02)

/* DAC Data Channel 1 */
#define	DACDAT1       	ECReg(EC_Register_Base_Address+0x1A03)

/* DAC Data Channel 2 */
#define	DACDAT2        	ECReg(EC_Register_Base_Address+0x1A04)

/* DAC Data Channel 3 */
#define	DACDAT3       	ECReg(EC_Register_Base_Address+0x1A05)

/* DAC Data Channel 4 */
#define	DACDAT4       	ECReg(EC_Register_Base_Address+0x1A06)

/* DAC Data Channel 5 */
#define	DACDAT5       	ECReg(EC_Register_Base_Address+0x1A07)

/* 
 * ****************************************************************************
 * (1Bxxh) Wack-Up control (WUC)
 * ****************************************************************************
 */
/* Wake-Up Edge Mode Register 1 */
#define	WUEMR1			ECReg(EC_Register_Base_Address+0x1B00)

/* Wake-Up Edge Mode Register 2 */
#define	WUEMR2      	ECReg(EC_Register_Base_Address+0x1B01)

/* Wake-Up Edge Mode Register 3 */
#define	WUEMR3       	ECReg(EC_Register_Base_Address+0x1B02)

/* Wake-Up Edge Mode Register 4 */
#define	WUEMR4       	ECReg(EC_Register_Base_Address+0x1B03)

/* Wake-Up Edge Mode Register 5 */
#define	WUEMR5       	ECReg(EC_Register_Base_Address+0x1B0C)

/* Wake-Up Edge Mode Register 6 */
#define	WUEMR6       	ECReg(EC_Register_Base_Address+0x1B10)

/* Wake-Up Edge Mode Register 7 */
#define	WUEMR7       	ECReg(EC_Register_Base_Address+0x1B14)

/* Wake-Up Edge Mode Register 8 */
#define WUEMR8          ECReg(EC_Register_Base_Address+0x1B18)

/* Wake-Up Edge Mode Register 9 */
#define WUEMR9          ECReg(EC_Register_Base_Address+0x1B1C)

/* Wake-Up Edge Mode Register 10 */
#define WUEMR10         ECReg(EC_Register_Base_Address+0x1B20)

/* Wake-Up Edge Mode Register 11 */
#define WUEMR11         ECReg(EC_Register_Base_Address+0x1B24)

/* Wake-Up Edge Mode Register 12 */
#define WUEMR12         ECReg(EC_Register_Base_Address+0x1B28)

/* Wake-Up Edge Mode Register 13 */
#define WUEMR13         ECReg(EC_Register_Base_Address+0x1B2C)

/* Wake-Up Edge Mode Register 14 */
#define WUEMR14         ECReg(EC_Register_Base_Address+0x1B30)

/* Wake-Up Edge Mode Register 15 */
#define	WUEMR15       	ECReg(EC_Register_Base_Address+0x1B34)

/* Wake-Up Edge Sense Register 1 */
#define	WUESR1       	ECReg(EC_Register_Base_Address+0x1B04)

/* Wake-Up Edge Sense Register 2 */
#define	WUESR2       	ECReg(EC_Register_Base_Address+0x1B05)

/* Wake-Up Edge Sense Register 3 */
#define	WUESR3       	ECReg(EC_Register_Base_Address+0x1B06)

/* Wake-Up Edge Sense Register 4 */
#define	WUESR4        	ECReg(EC_Register_Base_Address+0x1B07)

/* Wake-Up Edge Sense Register 5 */
#define	WUESR5        	ECReg(EC_Register_Base_Address+0x1B0D)

/* Wake-Up Edge Sense Register 6 */
#define	WUESR6        	ECReg(EC_Register_Base_Address+0x1B11)

/* Wake-Up Edge Sense Register 7 */
#define	WUESR7        	ECReg(EC_Register_Base_Address+0x1B15)

/* Wake-Up Edge Sense Register 8 */
#define WUESR8          ECReg(EC_Register_Base_Address+0x1B19)

/* Wake-Up Edge Sense Register 9 */
#define WUESR9          ECReg(EC_Register_Base_Address+0x1B1D)

/* Wake-Up Edge Sense Register 10 */
#define WUESR10         ECReg(EC_Register_Base_Address+0x1B21)

/* Wake-Up Edge Sense Register 11 */
#define WUESR11         ECReg(EC_Register_Base_Address+0x1B25)

/* Wake-Up Edge Sense Register 12 */
#define WUESR12         ECReg(EC_Register_Base_Address+0x1B29)

/* Wake-Up Edge Sense Register 13 */
#define WUESR13         ECReg(EC_Register_Base_Address+0x1B2D)

/* Wake-Up Edge Sense Register 14 */
#define WUESR14         ECReg(EC_Register_Base_Address+0x1B31)

/* Wake-Up Edge Sense Register 15 */
#define WUESR15         ECReg(EC_Register_Base_Address+0x1B35)

/* Wake-Up Enable Register 1 */
#define	WUENR1        	ECReg(EC_Register_Base_Address+0x1B08)

/* Wake-Up Enable Register 2 */
#define	WUENR2       	ECReg(EC_Register_Base_Address+0x1B09)

/* Wake-Up Enable Register 3 */
#define	WUENR3       	ECReg(EC_Register_Base_Address+0x1B0A)

/* Wake-Up Enable Register 4 */
#define	WUENR4			ECReg(EC_Register_Base_Address+0x1B0B)

/* Wake-Up Enable Register 6 */
#define	WUENR6			ECReg(EC_Register_Base_Address+0x1B12)

/* Wake-Up Enable Register 7 */
#define	WUENR7			ECReg(EC_Register_Base_Address+0x1B16)

/* Wake-Up Enable Register 8 */
#define WUENR8          ECReg(EC_Register_Base_Address+0x1B1A)

/* Wake-Up Enable Register 9 */
#define WUENR9          ECReg(EC_Register_Base_Address+0x1B1E)


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//                 [Group]               [Interrupt]        [ Description ]
//
//
// WUC Group 1
//
#define  WUC_PS2CLK0      	BIT(0)  // WKINTA to INT5
#define  WUC_PS2DAT0      	BIT(1)  // WKINTA to INT5
//#define  WUC_Reserved		BIT(2)     //
//#define  WUC_Reserved    	BIT(3)     //
#define  WUC_PS2CLK2      	BIT(4)  // WKINTA to INT5
#define  WUC_PS2DAT2      	BIT(5)  // WKINTA to INT5
//#define WUC_Reserved    		BIT(6)    //
//#define WUC_Reserved      	BIT(7)    //
//
// WUC Group 2
//
#define  WUC_WUI0      		BIT(0)  // GPD0
#define  WUC_WUI1      		BIT(1)  // GPD1
#define  WUC_WUI2        	BIT(2)  // GPC4
#define  WUC_WUI3           BIT(3)  // GPC6
#define  WUC_WUI4      		BIT(4)  // GPD2
#define  WUC_PWRSW      	BIT(5)  // GPB3
#define  WUC_SWUC_WakeUP    BIT(6)  //
//#define WUC_Reserved      	BIT(7)  	//
//
// WUC Group 3
//
#define  WUC_KSI0      		BIT(0)  //
#define  WUC_KSI1      		BIT(1)  //
#define  WUC_KSI2        	BIT(2)  //
#define  WUC_KSI3           BIT(3)  //
#define  WUC_KSI4      		BIT(4)  //
#define  WUC_KSI5      		BIT(5)  //
#define  WUC_KSI6           BIT(6)  //
#define  WUC_KSI7           BIT(7)  //
//
// WUC Group 4
//
#define  WUC_WUI5      		BIT(0)  // GPE5
#define  WUC_CRX0_CRX1    	BIT(1)  //
#define  WUC_LPC_Access     BIT(2)  //
#define  WUC_SMDAT0         BIT(3)  //
#define  WUC_SMDAT1      	BIT(4)  //
#define  WUC_WUI6      		BIT(5)  // GPE6
#define  WUC_WUI7           BIT(6)  // GPE7
#define  WUC_SMDAT2         BIT(7)  //
//
// WUC Group 5
//
#define WUC_WUI8			BIT(0)  // GPK0
#define WUC_WUI9			BIT(1)  // GPK1
#define WUC_WUI10			BIT(2)  // GPK2
#define WUC_WUI11			BIT(3)  // GPK3
#define WUC_WUI12			BIT(4)  // GPK4
#define WUC_WUI13			BIT(5)  // GPK5
#define WUC_WUI14			BIT(6)  // GPK6
#define WUC_WUI15			BIT(7)  // GPK7
//
// WUC Group 6
//
#define  WUC_WUI16      	BIT(0)  // GPH0
#define  WUC_WUI17          BIT(1)  // GPH1
#define  WUC_WUI18          BIT(2)  // GPH2
#define  WUC_WUI19          BIT(3)  // GPH3
#define  WUC_WUI20      	BIT(4)  // GPF4
#define  WUC_WUI21      	BIT(5)  // GPF5
#define  WUC_WUI22          BIT(6)  // GPF6
#define  WUC_WUI23          BIT(7)  // GPF7
//
// WUC Group 7
//
#define  WUC_WUI24      	BIT(0)  // GPE0
#define  WUC_WUI25          BIT(1)  // GPE1
#define  WUC_WUI26          BIT(2)  // GPE2
#define  WUC_WUI27          BIT(3)  // GPE3
#define  WUC_WUI28      	BIT(4)  // GPI4
#define  WUC_WUI29      	BIT(5)  // GPI5
#define  WUC_WUI30          BIT(6)  // GPI6
#define  WUC_WUI31          BIT(7)  // GPI7
//
// WUC Group 8
//
#define  WUC_WUI32      	BIT(0)  // GPA3
#define  WUC_WUI33          BIT(1)  // GPA4
#define  WUC_WUI34          BIT(2)  // GPA5
#define  WUC_WUI35          BIT(3)  // GPA6
#define  WUC_WUI36      	BIT(4)  // GPB2
#define  WUC_WUI37      	BIT(5)  // GPC0
#define  WUC_WUI38          BIT(6)  // GPC7
#define  WUC_WUI39          BIT(7)  // GPD7
//
// WUC Group 9
//
#define  WUC_WUI40      	BIT(0)  // GPH4
#define  WUC_WUI41          BIT(1)  // GPH5
#define  WUC_WUI42          BIT(2)  // GPH6
#define  WUC_WUI43          BIT(3)  // GPA0
#define  WUC_WUI44      	BIT(4)  // GPA1
#define  WUC_WUI45      	BIT(5)  // GPA2
//#define WUC_Reserved      BIT(6)  //
#define  WUC_WUI47          BIT(7)  // GPC2
//
// WUC Group 10
//
#define  WUC_WUI48      	BIT(0)  // GPF0
#define  WUC_WUI49          BIT(1)  // GPF1
#define  WUC_WUI50          BIT(2)  // GPF2
#define  WUC_WUI51          BIT(3)  // GPF3
#define  WUC_WUI52      	BIT(4)  // GPA7
#define  WUC_WUI53      	BIT(5)  // GPB0
#define  WUC_WUI54          BIT(6)  // GPB1
#define  WUC_WUI55          BIT(7)  // GPB3
//
// WUC Group 11
//
#define  WUC_WUI56      	BIT(0)  // GPB5
#define  WUC_WUI57          BIT(1)  // GPB6
//#define  WUC_Reserved        BIT(2)  //
#define  WUC_WUI59          BIT(3)  // GPC1
#define  WUC_WUI60      	BIT(4)  // GPC3
#define  WUC_WUI61      	BIT(5)  // GPC5
#define  WUC_WUI62          BIT(6)  // GPD3
#define  WUC_WUI63          BIT(7)  // GPD4
//
// WUC Group 12
//
#define  WUC_WUI64      	BIT(0)  // GPD5
#define  WUC_WUI65          BIT(1)  // GPD6
#define  WUC_WUI66          BIT(2)  // GPE4
#define  WUC_WUI67          BIT(3)  // GPG0
#define  WUC_WUI68      	BIT(4)  // GPG1
#define  WUC_WUI69      	BIT(5)  // GPG2
#define  WUC_WUI70          BIT(6)  // GPG6
#define  WUC_WUI71          BIT(7)  // GPI0
//
// WUC Group 13
//
#define  WUC_WUI72      	BIT(0)  // GPI1
#define  WUC_WUI73          BIT(1)  // GPI2
#define  WUC_WUI74          BIT(2)  // GPI3
//#define WUC_Reserved          	BIT(3)  //
//#define WUC_Reserved      	BIT(4)  //
//#define WUC_Reserved      	BIT(5)  //
//#define WUC_Reserved          	BIT(6)  //
//#define WUC_Reserved           BIT(7)  // GPH7
//
// WUC Group 14
//
#define  WUC_WUI80      	BIT(0)  // GPJ0
#define  WUC_WUI81          BIT(1)  // GPJ1
#define  WUC_WUI82          BIT(2)  // GPJ2
#define  WUC_WUI83          BIT(3)  // GPJ3
#define  WUC_WUI84      	BIT(4)  // GPJ4
#define  WUC_WUI85      	BIT(5)  // GPJ5
#define  WUC_WUI86          BIT(6)  // GPJ6
//#define WUC_Reserved           BIT(7)  //
//
// WUC Group 15
//
#define  WUC_WUI88      	BIT(0)  // GPL0
#define  WUC_WUI89          BIT(1)  // GPL1
#define  WUC_WUI90          BIT(2)  // GPL2
#define  WUC_WUI91          BIT(3)  // GPL3
#define  WUC_WUI92      	BIT(4)  // GPL4
#define  WUC_WUI93      	BIT(5)  // GPL5
#define  WUC_WUI94          BIT(6)  // GPL6
#define  WUC_WUI95          BIT(7)  // GPL7


/* 
 * ****************************************************************************
 * (1Cxxh) SMBus Interface (SMB)
 * ****************************************************************************
 */
/* Host Status A */
#define	HOSTA_A      	ECReg(EC_Register_Base_Address+0x1C40)

/* Host Status B */
#define	HOSTA_B      	ECReg(EC_Register_Base_Address+0x1C80)

/* Host Status C */
#define	HOSTA_C      	ECReg(EC_Register_Base_Address+0x1CC0)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define HOSTA_BDS       BIT(7)  /* Byte Done Status */
#define HOSTA_TMOE      BIT(6)  /* Time-out Error */
#define HOSTA_NACK      BIT(5)  /* Not Response ACK */
#define HOSTA_FAIL      BIT(4)  /* Fail */
#define HOSTA_BSER      BIT(3)  /* Bus Error */
#define HOSTA_DVER      BIT(2)  /* Device Error */
#define HOSTA_FINTR     BIT(1)  /* Finish Interrupt */
#define HOSTA_HOBY      BIT(0)  /* Host Busy */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host Control A */
#define	HOCTL_A       	ECReg(EC_Register_Base_Address+0x1C41)

/* Host Control B */
#define	HOCTL_B       	ECReg(EC_Register_Base_Address+0x1C81)

/* Host Control C */
#define	HOCTL_C       	ECReg(EC_Register_Base_Address+0x1CC1)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define HOCTL_PEC_EN    BIT(7)  /* PEC Enable */
#define HOCTL_SRT       BIT(6)  /* Start */
#define HOCTL_LABY      BIT(5)  /* Last Byte */
#define HOCTL_SMCD2     BIT(4)  /* SMBus Command */
#define HOCTL_SMCD1     BIT(3)  /* SMBus Command */
#define HOCTL_SMCD0     BIT(2)  /* SMBus Command */
#define HOCTL_KILL      BIT(1)  /* Kill */
#define HOCTL_INTREN    BIT(0)  /* Host Interrupt Enable */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host Command A */
#define	HOCMD_A       	ECReg(EC_Register_Base_Address+0x1C42)

/* Host Command B */
#define	HOCMD_B       	ECReg(EC_Register_Base_Address+0x1C82)

/* Host Command C */
#define	HOCMD_C       	ECReg(EC_Register_Base_Address+0x1CC2)

/* Transmit Slave Address A */
#define	TRASLA_A      	ECReg(EC_Register_Base_Address+0x1C43)

/* Transmit Slave Address B */
#define	TRASLA_B      	ECReg(EC_Register_Base_Address+0x1C83)

/* Transmit Slave Address C */
#define	TRASLA_C      	ECReg(EC_Register_Base_Address+0x1CC3)

/* Host Data 0 A */
#define	D0REG_A       	ECReg(EC_Register_Base_Address+0x1C44)

/* Host Data 0 B */
#define	D0REG_B       	ECReg(EC_Register_Base_Address+0x1C84)

/* Host Data 0 C */
#define	D0REG_C       	ECReg(EC_Register_Base_Address+0x1CC4)

/* Host Data 1 A */
#define	D1REG_A       	ECReg(EC_Register_Base_Address+0x1C45)

/* Host Data 1 B */
#define	D1REG_B      	ECReg(EC_Register_Base_Address+0x1C85)

/* Host Data 1 C */
#define	D1REG_C      	ECReg(EC_Register_Base_Address+0x1CC5)

/* Host Block Data Byte A */
#define	HOBDB_A       	ECReg(EC_Register_Base_Address+0x1C46)

/* Host Block Data Byte B */
#define	HOBDB_B        	ECReg(EC_Register_Base_Address+0x1C86)

/* Host Block Data Byte C */
#define	HOBDB_C        	ECReg(EC_Register_Base_Address+0x1CC6)

/* Packet Error Check A */
#define	PECERC_A      	ECReg(EC_Register_Base_Address+0x1C47)

/* Packet Error Check B */
#define	PECERC_B    	ECReg(EC_Register_Base_Address+0x1C87)

/* Packet Error Check C */
#define	PECERC_C    	ECReg(EC_Register_Base_Address+0x1CC7)

/* Receive Slave Adress A */
#define	RESLADR_A		ECReg(EC_Register_Base_Address+0x1C48)

/* Receive Slave Adress B */
#define	RESLADR_B		ECReg(EC_Register_Base_Address+0x1C88)

/* Receive Slave Adress_2 A */
#define	RESLADR_2_A		ECReg(EC_Register_Base_Address+0x1C51)

/* Receive Slave Adress_2 B */
#define	RESLADR_2_B		ECReg(EC_Register_Base_Address+0x1C91)

/* Slave Data A */
#define	SLDA_A			ECReg(EC_Register_Base_Address+0x1C49)

/* Slave Data B */
#define	SLDA_B			ECReg(EC_Register_Base_Address+0x1C89)

/* SMBUS Pin Control A */
#define	SMBPCTL_A      	ECReg(EC_Register_Base_Address+0x1C4A)
  #define SMBCCS             BIT0
  #define SMBDCS             BIT1

/* SMBUS Pin Control B */
#define	SMBPCTL_B      	ECReg(EC_Register_Base_Address+0x1C8A)

/* SMBUS Pin Control C */
#define	SMBPCTL_C      	ECReg(EC_Register_Base_Address+0x1CCA)

/* Slave Status A */
#define	SLSTA_A			ECReg(EC_Register_Base_Address+0x1C4B)

/* Slave Status B */
#define	SLSTA_B			ECReg(EC_Register_Base_Address+0x1C8B)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define INT81           BIT7    /* Status bit of INT81 */
#define BIS             BIT6    /* Bus idle status. 1 : bus free */
#define SPDS            BIT5
#define MSLA2           BIT4
#define RCS             BIT3
#define STS             BIT2
#define SDS             BIT1
#define HONOST          BIT0
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Slave Interrupt Control A */
#define	SICR_A			ECReg(EC_Register_Base_Address+0x1C4C)

/* Slave Interrupt Control B */
#define	SICR_B			ECReg(EC_Register_Base_Address+0x1C8C)

/* Notify Device Adress A */
#define	NDADR_A			ECReg(EC_Register_Base_Address+0x1C4D)

/* Notify Device Adress B */
#define	NDADR_B			ECReg(EC_Register_Base_Address+0x1C8D)

/* Notify Data Low Byte A */
#define	NDLB_A			ECReg(EC_Register_Base_Address+0x1C4E)

/* Notify Data Low Byte B */
#define	NDLB_B			ECReg(EC_Register_Base_Address+0x1C8E)

/* Notify Data High Byte A */
#define	NDHB_A			ECReg(EC_Register_Base_Address+0x1C4F)

/* Notify Data High Byte B */
#define	NDHB_B			ECReg(EC_Register_Base_Address+0x1C8F)

/* Host Control2 A */
#define	HOCTL2_A      	ECReg(EC_Register_Base_Address+0x1C50)

/* Host Control2 B */
#define	HOCTL2_B      	ECReg(EC_Register_Base_Address+0x1C90)

/* Host Control2 C */
#define	HOCTL2_C      	ECReg(EC_Register_Base_Address+0x1CD0)

/* 4.7 £gs Low Register */
#define	SMB4P7USL   	ECReg(EC_Register_Base_Address+0x1C00)

/* 4.7 £gs High Register */
#define	SMB4P0USH   	ECReg(EC_Register_Base_Address+0x1C01)

/* 300 ns Register */
#define	SMB300NS  		ECReg(EC_Register_Base_Address+0x1C02)

/* 250 ns Register */
#define	SMB250NS  		ECReg(EC_Register_Base_Address+0x1C03)

/* 25 ms Register */
#define	SMB25MS   		ECReg(EC_Register_Base_Address+0x1C04)

/* 45.3 £gs Low Register */
#define	SMB45P3USL 		ECReg(EC_Register_Base_Address+0x1C05)

/* 45.3 £gs High Register */
#define	SMB45P3USH 		ECReg(EC_Register_Base_Address+0x1C06)

/* 4.7 £gs and 4.0 £gs High Register */
#define	SMB4P7A4P0H		ECReg(EC_Register_Base_Address+0x1C07)

/* Slave Interface Select */
#define	SLVISEL			ECReg(EC_Register_Base_Address+0x1C08)

/* SMCLK Timing Setting Register A */
#define	SCLKTS_A		ECReg(EC_Register_Base_Address+0x1C09)

/* SMCLK Timing Setting Register B */
#define	SCLKTS_B		ECReg(EC_Register_Base_Address+0x1C0A)

/* SMCLK Timing Setting Register C */
#define	SCLKTS_C		ECReg(EC_Register_Base_Address+0x1C0B)

/* SMbus FIFO Control 1 Register */
#define SMBFFCTRL1      ECReg(EC_Register_Base_Address+0x1C0D)

/* SMBus FIFO Status 1 Register */
#define SMBFFSTS1       ECReg(EC_Register_Base_Address+0x1C0E)

/* SMBus FIFO Control 2 Register */
#define SMBFFCTRL2      ECReg(EC_Register_Base_Address+0x1C0F)

/* SMBus FIFO Status 2 Register */
#define SMBFFSTS2       ECReg(EC_Register_Base_Address+0x1C10)

/* Channel Select EF Register */
#define CHSEF           ECReg(EC_Register_Base_Address+0x1C11)

/* Host Control 3 A */
#define HOCTL3_A        ECReg(EC_Register_Base_Address+0x1C52)

/* Host Control 3 B */
#define HOCTL3_B        ECReg(EC_Register_Base_Address+0x1C92)

/* Host Control 3 C */
#define HOCTL3_C        ECReg(EC_Register_Base_Address+0x1CD2)

/* Master Code Register A */
#define MCODE_A         ECReg(EC_Register_Base_Address+0x1C53)

/* Master Code Register B */
#define MCODE_B         ECReg(EC_Register_Base_Address+0x1C93)

/* Master Code Register C */
#define MCODE_C         ECReg(EC_Register_Base_Address+0x1CD3)

/* 
 * ****************************************************************************
 * (1Dxxh) Keyboard Matrix Scan control (KBS)
 * ****************************************************************************
 */
/* Keyboard Scan Out */
#define	KSOL          	ECReg(EC_Register_Base_Address+0x1D00)

/* Keyboard Scan Out */
#define	KSOH1          	ECReg(EC_Register_Base_Address+0x1D01)

/* Keyboard Scan Out Control */
#define	KSOCTRL      	ECReg(EC_Register_Base_Address+0x1D02)

/* Keyboard Scan Out */
#define	KSOH2			ECReg(EC_Register_Base_Address+0x1D03)

/* Keyboard Scan In */
#define	KSI           	ECReg(EC_Register_Base_Address+0x1D04)

/* Keyboard Scan In Control */
#define	KSICTRL       	ECReg(EC_Register_Base_Address+0x1D05)

/* Keyboard Scan In [7:0] GPIO Control */
#define	KSIGCTRL       	ECReg(EC_Register_Base_Address+0x1D06)

/* Keyboard Scan In [7:0] GPIO Output Enable */
#define	KSIGOEN       	ECReg(EC_Register_Base_Address+0x1D07)

/* Keyboard Scan In [7:0] GPIO DATA */
#define	KSIGDAT       	ECReg(EC_Register_Base_Address+0x1D08)

/* Keyboard Scan In [7:0] GPIO DATA Mirror */
#define	KSIGDMRR       	ECReg(EC_Register_Base_Address+0x1D09)

/* Keyboard Scan Out [15:8] GPIO Control */
#define	KSOHGCTRL       ECReg(EC_Register_Base_Address+0x1D0A)

/* Keyboard Scan Out [15:8] GPIO Output Enable */
#define	KSOHGOEN       	ECReg(EC_Register_Base_Address+0x1D0B)

/* Keyboard Scan Out [15:8] GPIO DATA Mirror */
#define	KSOHGDMRR       ECReg(EC_Register_Base_Address+0x1D0C)

/* Keyboard Scan Out [7:0] GPIO Control */
#define	KSOLGCTRL       ECReg(EC_Register_Base_Address+0x1D0D)

/* Keyboard Scan Out [7:0] GPIO Output Enable */
#define	KSOLGOEN       	ECReg(EC_Register_Base_Address+0x1D0E)

/* Keyboard Scan Out [7:0] GPIO DATA Mirror */
#define	KSOLGDMRR       ECReg(EC_Register_Base_Address+0x1D0F)

/* KSO 0 Low Scan Data Register */
#define KSO0LSDR        ECReg(EC_Register_Base_Address+0x1D10)

/* KSO 1 Low Scan Data Register */
#define KSO1LSDR        ECReg(EC_Register_Base_Address+0x1D11)

/* KSO 2 Low Scan Data Register */
#define KSO2LSDR        ECReg(EC_Register_Base_Address+0x1D12)

/* KSO 3 Low Scan Data Register */
#define KSO3LSDR        ECReg(EC_Register_Base_Address+0x1D13)

/* KSO 4 Low Scan Data Register */
#define KSO4LSDR        ECReg(EC_Register_Base_Address+0x1D14)

/* KSO 5 Low Scan Data Register */
#define KSO5LSDR        ECReg(EC_Register_Base_Address+0x1D15)

/* KSO 6 Low Scan Data Register */
#define KSO6LSDR        ECReg(EC_Register_Base_Address+0x1D16)

/* KSO 7 Low Scan Data Register */
#define KSO7LSDR        ECReg(EC_Register_Base_Address+0x1D17)

/* KSO 8 Low Scan Data Register */
#define KSO8LSDR        ECReg(EC_Register_Base_Address+0x1D18)

/* KSO 9 Low Scan Data Register */
#define KSO9LSDR        ECReg(EC_Register_Base_Address+0x1D19)

/* KSO 10 Low Scan Data Register */
#define KSO10LSDR       ECReg(EC_Register_Base_Address+0x1D1A)

/* KSO 11 Low Scan Data Register */
#define KSO11LSDR       ECReg(EC_Register_Base_Address+0x1D1B)

/* KSO 12 Low Scan Data Register */
#define KSO12LSDR       ECReg(EC_Register_Base_Address+0x1D1C)

/* KSO 13 Low Scan Data Register */
#define KSO13LSDR       ECReg(EC_Register_Base_Address+0x1D1D)

/* KSO 14 Low Scan Data Register */
#define KSO14LSDR       ECReg(EC_Register_Base_Address+0x1D1E)

/* KSO 15 Low Scan Data Register */
#define KSO15LSDR       ECReg(EC_Register_Base_Address+0x1D1F)

/* KSO 16 Low Scan Data Register */
#define KSO16LSDR       ECReg(EC_Register_Base_Address+0x1D20)

/* KSO 17 Low Scan Data Register */
#define KSO17LSDR       ECReg(EC_Register_Base_Address+0x1D21)

/* Scan Data Control 1 Register */
#define SDC1R           ECReg(EC_Register_Base_Address+0x1D22)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SDEN            BIT7    /* Scan data enable */
//#define --            BIT6    /* Reserved */
#define INTSDVEN        BIT5    /* Interrupt from scan data valid enable */
//#define --            BIT4    /* Reserved */
//#define --            BIT3    /* Reserved */
/* --------------------------------- */
/* BIT2 ~ BIT0  Scan loop select */
#define SLS_00_Round    0x00
#define SLS_02_Round    0x01
#define SLS_03_Round    0x02
#define SLS_04_Round    0x03
#define SLS_05_Round    0x04
#define SLS_06_Round    0x05
#define SLS_07_Round    0x06
#define SLS_08_Round    0x07
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Scan Data Control 2 Register */
#define SDC2R           ECReg(EC_Register_Base_Address+0x1D23)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define KSOPCS1         BIT7    /* KSO pin count select */
#define KSOPCS0         BIT6    /* KSO pin count select */
//#define --            BIT5    /* Reserved */
//#define --            BIT4    /* Reserved */
/* --------------------------------- */
/* BIT3 ~ BIT0  Wait KSO high delay */
#define WKSOHDLY_23us   0x00
#define WKSOHDLY_31us   0x01
#define WKSOHDLY_39us   0x02
#define WKSOHDLY_47us   0x03
#define WKSOHDLY_55us   0x04
#define WKSOHDLY_63us   0x05
#define WKSOHDLY_71us   0x06
#define WKSOHDLY_79us   0x07
#define WKSOHDLY_87us   0x08
#define WKSOHDLY_95us   0x09
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Scan Data Control 3 Register */
#define SDC3R           ECReg(EC_Register_Base_Address+0x1D24)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* --------------------------------- */
/* BIT7 ~ BIT4  Wait KSO low delay */
#define WKSOLDLY_11us   (0x00 << 4)
#define WKSOLDLY_13us   (0x01 << 4)
#define WKSOLDLY_15us   (0x02 << 4)
#define WKSOLDLY_17us   (0x03 << 4)
#define WKSOLDLY_19us   (0x04 << 4)
#define WKSOLDLY_21us   (0x05 << 4)
#define WKSOLDLY_23us   (0x06 << 4)
#define WKSOLDLY_25us   (0x07 << 4)
#define WKSOLDLY_27us   (0x08 << 4)
#define WKSOLDLY_29us   (0x09 << 4)
/* --------------------------------- */
/* BIT3 ~ BIT0  Spacing delay between rounds */
#define SDLYBR_00ms     0x00
#define SDLYBR_01ms     0x01
#define SDLYBR_02ms     0x02
#define SDLYBR_03ms     0x03
#define SDLYBR_04ms     0x04
#define SDLYBR_05ms     0x05
#define SDLYBR_06ms     0x06
#define SDLYBR_07ms     0x07
#define SDLYBR_08ms     0x08
#define SDLYBR_09ms     0x09
#define SDLYBR_10ms     0x0A
#define SDLYBR_11ms     0x0B
#define SDLYBR_12ms     0x0C
#define SDLYBR_13ms     0x0D
#define SDLYBR_14ms     0x0E
#define SDLYBR_15ms     0x0F
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Scan Data Control 4 Register */
#define SDSR            ECReg(EC_Register_Base_Address+0x1D25)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT7    /* Reserved */
//#define --            BIT6    /* Reserved */
//#define --            BIT5    /* Reserved */
//#define --            BIT4    /* Reserved */
//#define --            BIT3    /* Reserved */
//#define --            BIT2    /* Reserved */
//#define --            BIT1    /* Reserved */
#define SDV             BIT0    /* Scan data valid */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Keyboard Scan Out Control Register */
#define	KSOPU		    BIT(2)  /* KSO Pull Up */
#define	KSOOD		    BIT(0)  /* KSO Open Drain */

/* Keyboard Scan In Control Register */
#define	KSIPU		    BIT(2)  /* KSI Pull Up */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (1Exxh) EC Clock and Power Management controller (ECPM)
 * ****************************************************************************
 */
/* Clock Gating Control 1 */
#define	CGCTRL1R    	ECReg(EC_Register_Base_Address+0x1E01)

/* Clock Gating Control 2 */
#define	CGCTRL2R      	ECReg(EC_Register_Base_Address+0x1E02)

/* Clock Gating Control 3 */
#define	CGCTRL3R		ECReg(EC_Register_Base_Address+0x1E05)

/* PLL Control */
#define	PLLCTRL        	ECReg(EC_Register_Base_Address+0x1E03)

/* Auto Clock Gating */
#define	AUTOCG			ECReg(EC_Register_Base_Address+0x1E04)

/* PLL Frequency */
#define	PLLFREQR		ECReg(EC_Register_Base_Address+0x1E06)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define PLL_8M			0
#define PLL_16M			1
#define PLL_24M			2
#define PLL_32M			3
#define PLL_48M			4
#define PLL_64M			5
#define PLL_72M			6
#define PLL_96M			7
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* PLL Frequency */
#define	PLLSSCR			ECReg(EC_Register_Base_Address+0x1E07)

/* PLL Clock Source Status */
#define PLLCSS          ECReg(EC_Register_Base_Address+0x1E08)

/* Clock Gating Control 4 Register */
#define CGCTRL4R        ECReg(EC_Register_Base_Address+0x1E09)

#define EC_1E00         ECReg(EC_Register_Base_Address+0x1E00)
#define ECPM_PDCTRL1R   ECReg(EC_Register_Base_Address+0x1E01)
#define EC_1E03         ECReg(EC_Register_Base_Address+0x1E03)
#define EC_1E06         ECReg(EC_Register_Base_Address+0x1E06)

/* LDO Control Register */
#define LDOCTR          ECReg(EC_Register_Base_Address+0x1E0A)

/* PLL Stable Time Control Register */
#define PLLSTCR         ECReg(EC_Register_Base_Address+0x1E0B)

/* System Clock Divide Control Register 0 */
#define SCDCR0          ECReg(EC_Register_Base_Address+0x1E0C)

/* System Clock Divide Control Register 1 */
#define SCDCR1          ECReg(EC_Register_Base_Address+0x1E0D)

/* System Clock Divide Control Register 2 */
#define SCDCR2          ECReg(EC_Register_Base_Address+0x1E0E)

/* System Clock Divide Control Register 3 */
#define SCDCR3          ECReg(EC_Register_Base_Address+0x1E0F)

/* System Clock Divide Control Register 4 */
#define SCDCR4          ECReg(EC_Register_Base_Address+0x1E10)

/* 
 * ****************************************************************************
 * (1Fxxh) External Timer & External Watchdog (ETWD)
 * ****************************************************************************
 */
/* External Timer/WDT Configuration Register */
#define	ETWCFG        	ECReg(EC_Register_Base_Address+0x1F01)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT7    /* Reserved */
//#define --            BIT6    /* Reserved */
#define EWDKEYEN        BIT5    /* External WDT Key Enabled */
#define EWDSRC          BIT4    /* External WDT Clock Source */
#define LEWDCNTL        BIT3    /* Lock EWDCNTLx Register */
#define LET1CNTL        BIT2    /* Lock ET1CNTLx Registers */
#define LET1PS          BIT1    /* Lock ET1PS Register */
#define LETWCFG         BIT0    /* Lock ETWCFG Register */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* External 1 Timer Prescaler Register */
#define	ET1PSR         	ECReg(EC_Register_Base_Address+0x1F02)

/* External 1 Timer Counter High Byte */
#define	ET1CNTLHR     	ECReg(EC_Register_Base_Address+0x1F03)

/* External 1 Timer Counter Low Byte */
#define	ET1CNTLLR      	ECReg(EC_Register_Base_Address+0x1F04)

/* External Timer/WDT Control Register */
#define	ETWCTRL      	ECReg(EC_Register_Base_Address+0x1F05)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT7    /* Reserved */
//#define --            BIT6    /* Reserved */
#define EWDSCEN         BIT5    /*
                                 * External WDT Stop Count Enable,
                                 * This bit cannot be set until EWDSCMS bit
                                 * is set to 1.
                                 */
#define EWDSCMS         BIT4    /* External WDT Stop Count Mode Select */
#define ET2TC           BIT3    /* External Timer 2 Terminal Count */
#define ET2RST          BIT2    /* External Timer 2 Reset */
#define ET1TC           BIT1    /* External Timer 1 Terminal Count */
#define ET1RST          BIT0    /* External Timer 1 Reset */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* External WDT Counter Low Byte */
#define	EWDCNTLR      	ECReg(EC_Register_Base_Address+0x1F06)

/* External WDT Key Register */
#define	EWDKEYR      	ECReg(EC_Register_Base_Address+0x1F07)

/* External WDT Counter High Byte */
#define	EWDCNTHR		ECReg(EC_Register_Base_Address+0x1F09)

/* External Timer 2 Prescaler Register */
#define	ET2PSR      	ECReg(EC_Register_Base_Address+0x1F0A)

/* External Timer 2 Counter High Byte */
#define	ET2CNTLHR      	ECReg(EC_Register_Base_Address+0x1F0B)

/* External Timer 2 Counter Low Byte */
#define	ET2CNTLLR      	ECReg(EC_Register_Base_Address+0x1F0C)

/* External Timer 2 Counter High Byte */
#define	ET2CNTLH2R		ECReg(EC_Register_Base_Address+0x1F0E)

/* External Timer 3 Control Register */
#define ET3CTRL         ECReg(EC_Register_Base_Address+0x1F10)

/* External Timer 3 Prescaler Register */
#define ET3PSR          ECReg(EC_Register_Base_Address+0x1F11)

/* External Timer 3 Counter Low Byte */
#define ET3CNTLLR       ECReg(EC_Register_Base_Address+0x1F14)

/* External Timer 3 Counter High Byte */
#define ET3CNTLHR       ECReg(EC_Register_Base_Address+0x1F15)

/* External Timer 3 Counter High Byte 2 */
#define ET3CNTLH2R      ECReg(EC_Register_Base_Address+0x1F16)

#define ET3CNTLH3R      ECReg(EC_Register_Base_Address+0x1F17)


/* External Timer 4 Control Register */
#define ET4CTRL         ECReg(EC_Register_Base_Address+0x1F18)

/* External Timer 4 Prescaler Register */
#define ET4PSR          ECReg(EC_Register_Base_Address+0x1F19)

/* External Timer 4 Counter Low Byte */
#define ET4CNTLLR       ECReg(EC_Register_Base_Address+0x1F1C)

/* External Timer 4 Counter High Byte */
#define ET4CNTLHR       ECReg(EC_Register_Base_Address+0x1F1D)

/* External Timer 4 Counter High Byte 2 */
#define ET4CNTLH2R      ECReg(EC_Register_Base_Address+0x1F1E)

#define ET4CNTLH3R      ECReg(EC_Register_Base_Address+0x1F1F)  // External Timer 4 Counter High Byte 3
#define ET6CNTLH3R      ECReg(EC_Register_Base_Address+0x1F2F)  // External Timer 6 Counter High Byte 3
#define ET8CNTLH3R      ECReg(EC_Register_Base_Address+0x1F3F)  // External Timer 8 Counter High Byte 3


/* External Timer 5 Control Register */
#define ET5CTRL         ECReg(EC_Register_Base_Address+0x1F20)

/* External Timer 5 Prescaler Register */
#define ET5PSR          ECReg(EC_Register_Base_Address+0x1F21)

/* External Timer 5 Counter Low Byte */
#define ET5CNTLLR       ECReg(EC_Register_Base_Address+0x1F24)

/* External Timer 5 Counter High Byte */
#define ET5CNTLHR       ECReg(EC_Register_Base_Address+0x1F25)

/* External Timer 5 Counter High Byte 2 */
#define ET5CNTLH2R      ECReg(EC_Register_Base_Address+0x1F26)

/* External Timer 6 Control Register */
#define ET6CTRL         ECReg(EC_Register_Base_Address+0x1F28)

/* External Timer 6 Prescaler Register */
#define ET6PSR          ECReg(EC_Register_Base_Address+0x1F29)

/* External Timer 6 Counter Low Byte */
#define ET6CNTLLR       ECReg(EC_Register_Base_Address+0x1F2C)

/* External Timer 6 Counter High Byte */
#define ET6CNTLHR       ECReg(EC_Register_Base_Address+0x1F2D)

/* External Timer 6 Counter High Byte 2 */
#define ET6CNTLH2R      ECReg(EC_Register_Base_Address+0x1F2E)

#define ET6CNTLH3R      ECReg(EC_Register_Base_Address+0x1F2F)  // External Timer 6 Counter High Byte 3


/* External Timer 7 Control Register */
#define ET7CTRL         ECReg(EC_Register_Base_Address+0x1F30)

/* External Timer 7 Prescaler Register */
#define ET7PSR          ECReg(EC_Register_Base_Address+0x1F31)

/* External Timer 7 Counter Low Byte */
#define ET7CNTLLR       ECReg(EC_Register_Base_Address+0x1F34)

/* External Timer 7 Counter High Byte */
#define ET7CNTLHR       ECReg(EC_Register_Base_Address+0x1F35)

/* External Timer 7 Counter High Byte 2 */
#define ET7CNTLH2R      ECReg(EC_Register_Base_Address+0x1F36)

/* External Timer 8 Control Register */
#define ET8CTRL         ECReg(EC_Register_Base_Address+0x1F38)

/* External Timer 8 Prescaler Register */
#define ET8PSR          ECReg(EC_Register_Base_Address+0x1F39)

/* External Timer 8 Counter Low Byte */
#define ET8CNTLLR       ECReg(EC_Register_Base_Address+0x1F3C)

/* External Timer 8 Counter High Byte */
#define ET8CNTLHR       ECReg(EC_Register_Base_Address+0x1F3D)

/* External Timer 8 Counter High Byte 2 */
#define ET8CNTLH2R      ECReg(EC_Register_Base_Address+0x1F3E)


#define ET8CNTLH3R      ECReg(EC_Register_Base_Address+0x1F3F)  // External Timer 8 Counter High Byte 3

/* External Timer 1 Counter Observation Low Byte */
#define ET3ET4COMB      BIT3                                    // External Timer 3 and Timer 4 Combine Mode
#define ET5ET6COMB      BIT3                                    // External Timer 5 and Timer 6 Combine Mode
#define ET7ET8COMB      BIT3                                    // External Timer 7 and Timer 8 Combine Mode
#define ET1CNTOLR       ECReg(EC_Register_Base_Address+0x1F40)

/* External Timer 1 Counter Observation High Byte */
#define ET1CNTOHR       ECReg(EC_Register_Base_Address+0x1F41)

#define ET2CNTOLR       ECReg(EC_Register_Base_Address+0x1F44)  // External Timer 2 Counter Observation Low Byte 
#define ET2CNTOHR       ECReg(EC_Register_Base_Address+0x1F45)  // External Timer 2 Counter Observation High Byte 
#define ET2CNTOH2R      ECReg(EC_Register_Base_Address+0x1F46)  // External Timer 2 Counter Observation High Byte 2

#define ET3CNTOLR       ECReg(EC_Register_Base_Address+0x1F48)  // External Timer 3 Counter Observation Low Byte 
#define ET3CNTOHR       ECReg(EC_Register_Base_Address+0x1F49)  // External Timer 3 Counter Observation High Byte 
#define ET3CNTOH2R      ECReg(EC_Register_Base_Address+0x1F4A)  // External Timer 3 Counter Observation High Byte 2
//#define ET3CNTOH3R      ECReg(EC_Register_Base_Address+0x1F4B)  // External Timer 3 Counter Observation High Byte 2

#define ET4CNTOLR       ECReg(EC_Register_Base_Address+0x1F4C)  // External Timer 4 Counter Observation Low Byte 
#define ET4CNTOHR       ECReg(EC_Register_Base_Address+0x1F4D)  // External Timer 4 Counter Observation High Byte 
#define ET4CNTOH2R      ECReg(EC_Register_Base_Address+0x1F4E)  // External Timer 4 Counter Observation High Byte 2
#define ET4CNTOH3R      ECReg(EC_Register_Base_Address+0x1F4F)  // External Timer 4 Counter Observation High Byte 3

#define ET5CNTOLR       ECReg(EC_Register_Base_Address+0x1F50)  // External Timer 5 Counter Observation Low Byte 
#define ET5CNTOHR       ECReg(EC_Register_Base_Address+0x1F51)  // External Timer 5 Counter Observation High Byte 
#define ET5CNTOH2R      ECReg(EC_Register_Base_Address+0x1F52)  // External Timer 5 Counter Observation High Byte 2

#define ET6CNTOLR       ECReg(EC_Register_Base_Address+0x1F54)  // External Timer 6 Counter Observation Low Byte 
#define ET6CNTOHR       ECReg(EC_Register_Base_Address+0x1F55)  // External Timer 6 Counter Observation High Byte 
#define ET6CNTOH2R      ECReg(EC_Register_Base_Address+0x1F56)  // External Timer 6 Counter Observation High Byte 2
#define ET6CNTOH3R      ECReg(EC_Register_Base_Address+0x1F57)  // External Timer 6 Counter Observation High Byte 3

#define ET7CNTOLR       ECReg(EC_Register_Base_Address+0x1F58)  // External Timer 7 Counter Observation Low Byte 
#define ET7CNTOHR       ECReg(EC_Register_Base_Address+0x1F59)  // External Timer 7 Counter Observation High Byte 
#define ET7CNTOH2R      ECReg(EC_Register_Base_Address+0x1F5A)  // External Timer 7 Counter Observation High Byte 2

#define ET8CNTOLR       ECReg(EC_Register_Base_Address+0x1F5C)  // External Timer 8 Counter Observation Low Byte 
#define ET8CNTOHR       ECReg(EC_Register_Base_Address+0x1F5D)  // External Timer 8 Counter Observation High Byte 
#define ET8CNTOH2R      ECReg(EC_Register_Base_Address+0x1F5E)  // External Timer 8 Counter Observation High Byte 2
#define ET8CNTOH3R      ECReg(EC_Register_Base_Address+0x1F5F)  // External Timer 8 Counter Observation High Byte 3

#define EWDCNTOLR       ECReg(EC_Register_Base_Address+0x1F60)  // External WDT Counter Observation Low Byte
#define EWDCNTOHR       ECReg(EC_Register_Base_Address+0x1F61)  // External WDT Counter Observation High Byte

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define ETPS_32_768_KHz 0x00
#define ETPS_1_024_KHz  0x01
#define ETPS_32_Hz      0x02
#define ETPS_8_MHz      0x03    /* only for external timer 2 ~ 8 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT7    /* Reserved */
//#define --            BIT6    /* Reserved */
//#define --            BIT5    /* Reserved */
//#define --            BIT4    /* Reserved */
//#define --            BIT3    /* Reserved */
#define ET_3_8_TC       BIT2    /* External Timer 2 ~ 8 Terminal Count */
#define ET_3_8_RST      BIT1    /* External Timer 2 ~ 8 Reset */
#define ET_3_8_EN       BIT0    /* External Timer 2 ~ 8 Enable Register */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (20xxh) General Control (GCTRL)
 * ****************************************************************************
 */
/* Chip ID Byte 1 */
#define	ECHIPID1		ECReg(EC_Register_Base_Address+0x2000)

/* Chip ID Byte 2 */
#define	ECHIPID2		ECReg(EC_Register_Base_Address+0x2001)

/* Chip Version */
#define	ECHIPVER		ECReg(EC_Register_Base_Address+0x2002)
#define	ECDEBUG			ECReg(EC_Register_Base_Address+0x2003)


/* Identify Input Register */
#define	IDR				ECReg(EC_Register_Base_Address+0x2004)

/* Reset Status */
#define	RSTS			ECReg(EC_Register_Base_Address+0x2006)

/* Reset Control 1 */
#define	RSTC1			ECReg(EC_Register_Base_Address+0x2007)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define RSMFI           BIT(7)  /* Reset SMFI */
#define RINTC           BIT(6)  /* Reset INTC */
#define REC2I           BIT(5)  /* Reset EC2I */
#define RKBC            BIT(4)  /* Reset KBC */
#define RSWUC           BIT(3)  /* Reset SWUC */
#define RPMC            BIT(2)  /* Reset PMC */
#define RGPIO           BIT(1)  /* Reset GPIO */
#define RPWM            BIT(0)  /* Reset PWM */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Reset Control 2 */
#define	RSTC2			ECReg(EC_Register_Base_Address+0x2008)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define RADC            BIT(7)  /* Reset ADC */
#define RDAC            BIT(6)  /* Reset DAC */
#define RWUC            BIT(5)  /* Reset WUC */
#define RKBS            BIT(4)  /* Reset KBS */
//#define --            BIT(3)  /* Reserved */
#define REGPC           BIT(2)  /* Reset EGPC */
#define RCIR            BIT(1)  /* Reset CIR */
//#define --            BIT(0)  /* Reserved */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Reset Control 3 */
#define	RSTC3			ECReg(EC_Register_Base_Address+0x2009)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT(7)  /* Reserved
#define RPS23           BIT(6)  /* Reset PS/2 Channel 3 */
#define RPS22           BIT(5)  /* Reset PS/2 Channel 2 */
#define RPS21           BIT(4)  /* Reset PS/2 Channel 1 */
                                /*
                                 * To reset the logic of PS/2 shared with
                                 * all channels, write 1111b to bit 7-4 at the
                                 * same time and writing 0111b is reserved.
                                 */

#define RSMBD           BIT(3)  /* Reset SMBus Channel D */
#define RSMBC           BIT(2)  /* Reset SMBus Channel C */
#define RSMBB           BIT(1)  /* Reset SMBus Channel B */
#define RSMBA           BIT(0)  /* Reset SMBus Channel A */
                                /*
                                 * To reset the logic of SMBus shared with
                                 * all channels, write 1111b to bit 3-0 at
                                 * the same time and writing 0111b is reserved.
                                 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Reset Control 4 */
#define	RSTC4			ECReg(EC_Register_Base_Address+0x2011)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//#define --            BIT(7)  /* Reserved*/
//#define --            BIT(6)  /* Reserved*/
//#define --            BIT(5)  /* Reserved*/
#define RPECI           BIT(4)  /* Reset PECI*/
#define RTMR            BIT(3)  /* Reset TMR*/
#define RUART2          BIT(2)  /* Reset UART2*/
#define RUART1          BIT(1)  /* Reset UART1*/
#define RSPI            BIT(0)  /* Reset SPI*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Base Address Select */
#define	BADRSEL			ECReg(EC_Register_Base_Address+0x200A)

/* Wait Next Clock Rising */
#define	WNCKR			ECReg(EC_Register_Base_Address+0x200B)

/* Oscillator Control Register */
#define	OSCTRL			ECReg(EC_Register_Base_Address+0x200C)

/* Special Control 1 */
#define	SPCTRL1			ECReg(EC_Register_Base_Address+0x200D)

/* Reset Control Host Side */
#define	RSTCH			ECReg(EC_Register_Base_Address+0x200E)

/* Generate IRQ */
#define	GENIRQ			ECReg(EC_Register_Base_Address+0x200F)

/* Reset Control DMMC */
#define	RSTDMMC			ECReg(EC_Register_Base_Address+0x2010)

/* Special Control 2 */
#define	SPECTRL2		ECReg(EC_Register_Base_Address+0x2012)

/* Special Control 3 */
#define	SPECTRL3		ECReg(EC_Register_Base_Address+0x2016)

/* Port I2EC High-Byte Register */
#define	PI2ECH			ECReg(EC_Register_Base_Address+0x2014)

/* Port I2EC Low-Byte Register */
#define	PI2ECL			ECReg(EC_Register_Base_Address+0x2015)

/* BRAM Interrupt Address 0 Register */
#define	BINTADDR0R		ECReg(EC_Register_Base_Address+0x2019)

/* BRAM Interrupt Address 1 Register */
#define	BINTADDR1R		ECReg(EC_Register_Base_Address+0x201A)

/* BRAM Interrupt Control Register */
#define	BINTCTRLR		ECReg(EC_Register_Base_Address+0x201B)

/* Special Control 4 */
#define SPCTRL4         ECReg(EC_Register_Base_Address+0x201C)

/* SHA1 Hash Control Register */
#define SHA1HASHCTRLR   ECReg(EC_Register_Base_Address+0x202D)

/* SHA1 Hash Base Address Register */
#define SHA1HBADDR      ECReg(EC_Register_Base_Address+0x202E)

/* Memory Controller Configuration Register */
#define MCCR            ECReg(EC_Register_Base_Address+0x2030)
#define EIDSR           ECReg(EC_Register_Base_Address+0x2031)
#define PMER1           ECReg(EC_Register_Base_Address+0x2032)
#define PMER2           ECReg(EC_Register_Base_Address+0x2033)
#define FRR0            ECReg(EC_Register_Base_Address+0x2034)
#define FRR1            ECReg(EC_Register_Base_Address+0x2035)
#define FRR2            ECReg(EC_Register_Base_Address+0x2036)
#define MCCR1           ECReg(EC_Register_Base_Address+0x203E)
#define IVTBAR          ECReg(EC_Register_Base_Address+0x2041)
#define MCCR2           ECReg(EC_Register_Base_Address+0x2044)


#define dummy_test      ECReg(EC_Register_Base_Address+0x2045)

#define SSCR			ECReg(EC_Register_Base_Address+0x204A)

/* 
 * ****************************************************************************
 * (21xxh) External GPIO Controller (EGPC) 
 * ****************************************************************************
 */
/* External GPIO Address Register */
#define	EADDR			ECReg(EC_Register_Base_Address+0x2100)

/* External GPIO Data Register */
#define	EDAT			ECReg(EC_Register_Base_Address+0x2101)

/* External GPIO Control Register */
#define	ECNT			ECReg(EC_Register_Base_Address+0x2102)

/* External GPIO Status Register */
#define	ESTS			ECReg(EC_Register_Base_Address+0x2103)

/* 
 * ****************************************************************************
 * (22xxh) Battery-Backed SRAM (BRAM) 
 * ****************************************************************************
 */
#define ECReg_Ptr(x) 	(((volatile unsigned char *)(x)))
#define BRAM  			ECReg_Ptr(EC_Register_Base_Address+0x2280) 	

#define BRAM00			  ECReg(EC_Register_Base_Address+0x2280+0x00)
#define BRAM01    		ECReg(EC_Register_Base_Address+0x2280+0x01)
#define BRAM02    		ECReg(EC_Register_Base_Address+0x2280+0x02)
#define BRAM03    		ECReg(EC_Register_Base_Address+0x2280+0x03)
#define BRAM04    		ECReg(EC_Register_Base_Address+0x2280+0x04)
#define BRAM05    		ECReg(EC_Register_Base_Address+0x2280+0x05)
#define BRAM06    		ECReg(EC_Register_Base_Address+0x2280+0x06)
#define BRAM07    		ECReg(EC_Register_Base_Address+0x2280+0x07)
#define BRAM08    		ECReg(EC_Register_Base_Address+0x2280+0x08)
#define BRAM09    		ECReg(EC_Register_Base_Address+0x2280+0x09)
#define BRAM0A    		ECReg(EC_Register_Base_Address+0x2280+0x0A)
#define BRAM0B    		ECReg(EC_Register_Base_Address+0x2280+0x0B)
#define BRAM0C    		ECReg(EC_Register_Base_Address+0x2280+0x0C)
#define BRAM0D    		ECReg(EC_Register_Base_Address+0x2280+0x0D)
#define BRAM0E    		ECReg(EC_Register_Base_Address+0x2280+0x0E)
#define BRAM0F    		ECReg(EC_Register_Base_Address+0x2280+0x0F)
	
#define BRAM10    		ECReg(EC_Register_Base_Address+0x2280+0x10)
#define BRAM11    		ECReg(EC_Register_Base_Address+0x2280+0x11)
#define BRAM12    		ECReg(EC_Register_Base_Address+0x2280+0x12)
#define BRAM13    		ECReg(EC_Register_Base_Address+0x2280+0x13)
#define BRAM14    		ECReg(EC_Register_Base_Address+0x2280+0x14)
#define BRAM15    		ECReg(EC_Register_Base_Address+0x2280+0x15)
#define BRAM16    		ECReg(EC_Register_Base_Address+0x2280+0x16)
#define BRAM17    		ECReg(EC_Register_Base_Address+0x2280+0x17)
#define BRAM18    		ECReg(EC_Register_Base_Address+0x2280+0x18)
#define BRAM19    		ECReg(EC_Register_Base_Address+0x2280+0x19)
#define BRAM1A    		ECReg(EC_Register_Base_Address+0x2280+0x1A)
#define BRAM1B    		ECReg(EC_Register_Base_Address+0x2280+0x1B)
#define BRAM1C    		ECReg(EC_Register_Base_Address+0x2280+0x1C)
#define BRAM1D    		ECReg(EC_Register_Base_Address+0x2280+0x1D)
#define BRAM1E    		ECReg(EC_Register_Base_Address+0x2280+0x1E)
#define BRAM1F    		ECReg(EC_Register_Base_Address+0x2280+0x1F)
	
#define BRAM20    		ECReg(EC_Register_Base_Address+0x2280+0x20)
#define BRAM21   		ECReg(EC_Register_Base_Address+0x2280+0x21)
#define BRAM22    		ECReg(EC_Register_Base_Address+0x2280+0x22)
#define BRAM23    		ECReg(EC_Register_Base_Address+0x2280+0x23)
#define BRAM24    		ECReg(EC_Register_Base_Address+0x2280+0x24)
#define BRAM25    		ECReg(EC_Register_Base_Address+0x2280+0x25)
#define BRAM26    		ECReg(EC_Register_Base_Address+0x2280+0x26)
#define BRAM27    		ECReg(EC_Register_Base_Address+0x2280+0x27)
#define BRAM28    		ECReg(EC_Register_Base_Address+0x2280+0x28)
#define BRAM29    		ECReg(EC_Register_Base_Address+0x2280+0x29)
#define BRAM2A    		ECReg(EC_Register_Base_Address+0x2280+0x2A)
#define BRAM2B    		ECReg(EC_Register_Base_Address+0x2280+0x2B)
#define BRAM2C    		ECReg(EC_Register_Base_Address+0x2280+0x2C)
#define BRAM2D    		ECReg(EC_Register_Base_Address+0x2280+0x2D)
#define BRAM2E    		ECReg(EC_Register_Base_Address+0x2280+0x2E)
#define BRAM2F    		ECReg(EC_Register_Base_Address+0x2280+0x2F)
	
#define BRAM30    		ECReg(EC_Register_Base_Address+0x2280+0x30)
#define BRAM31    		ECReg(EC_Register_Base_Address+0x2280+0x31)
#define BRAM32    		ECReg(EC_Register_Base_Address+0x2280+0x32)
#define BRAM33    		ECReg(EC_Register_Base_Address+0x2280+0x33)
#define BRAM34    		ECReg(EC_Register_Base_Address+0x2280+0x34)
#define BRAM35   		ECReg(EC_Register_Base_Address+0x2280+0x35)
#define BRAM36    		ECReg(EC_Register_Base_Address+0x2280+0x36)
#define BRAM37    		ECReg(EC_Register_Base_Address+0x2280+0x37)
#define BRAM38    		ECReg(EC_Register_Base_Address+0x2280+0x38)
#define BRAM39    		ECReg(EC_Register_Base_Address+0x2280+0x39)
#define BRAM3A    		ECReg(EC_Register_Base_Address+0x2280+0x3A)
#define BRAM3B    		ECReg(EC_Register_Base_Address+0x2280+0x3B)
#define BRAM3C    		ECReg(EC_Register_Base_Address+0x2280+0x3C)
#define BRAM3D    		ECReg(EC_Register_Base_Address+0x2280+0x3D)
#define BRAM3E    		ECReg(EC_Register_Base_Address+0x2280+0x3E)
#define BRAM3F    		ECReg(EC_Register_Base_Address+0x2280+0x3F)

/* 
 * ****************************************************************************
 * (23xxh) Consumer IR (CIR) 
 * ****************************************************************************
 */
/* CIR Data Register */
#define	C0DR			ECReg(EC_Register_Base_Address+0x2300)

/* CIR Master Control Register */
#define	C0MSTCR			ECReg(EC_Register_Base_Address+0x2301)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	CIR_CTXSEL	    BIT(7)  /* CTX Channel Select */
#define	CIR_CRXSEL      BIT(6)  /* CRX Channel Select */
#define	CIR_ILSEL	    BIT(5)  /* Internal Loopback Select */
#define	CIR_ILE		    BIT(4)  /* Internal Loopback Enable */
#define	CIR_FIFOTL1     BIT(3)  /* FIFO Threshold Level */
#define	CIR_FIFOTL0	    BIT(2)  /* FIFO Threshold Level */
#define	CIR_FIFOCLR	    BIT(1)  /* FIFO Clear */
#define	CIR_RESET	    BIT(0)  /* RESET */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* CIR Interrupt Enable Register */
#define	C0IER			ECReg(EC_Register_Base_Address+0x2302)

/* CIR Interrupt Identification Register */
#define	C0IIR			ECReg(EC_Register_Base_Address+0x2303)

/* CIR Carrier Frequency Register */
#define	C0CFR			ECReg(EC_Register_Base_Address+0x2304)

/* CIR Receive Control Register */
#define	C0RCR			ECReg(EC_Register_Base_Address+0x2305)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	CIR_RXEN	    BIT(7)  /* Receiver Enable */
//#define	CIR_	    BIT(6)  /* Reserved */
#define	CIR_RDWOS	    BIT(5)  /* Receiver Data Without Sync. */
#define	CIR_RXEND	    BIT(4)  /* Receiver Demodulation Enable */
#define	CIR_RXACT	    BIT(3)  /* Receiver Active */
#define	CIR_RXDCR2	    BIT(2)  /* Receiver Demodulation Carrier Range */
#define	CIR_RXDCR1	    BIT(1)  /* Receiver Demodulation Carrier Range */
#define	CIR_RXDCR0	    BIT(0)  /* Receiver Demodulation Carrier Range */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* CIR Transmitter Register */
#define	C0TCR			ECReg(EC_Register_Base_Address+0x2306)

/* CIR for sleep mode */
#define	C0SCK			ECReg(EC_Register_Base_Address+0x2307)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define	DLLOCK		    BIT(7)  /* DLL Lock */
#define	BRCM2		    BIT(6)  /* Baud Rate Count Mode */
#define	BRCM1		    BIT(5)  /* Baud Rate Count Mode */
#define	BRCM0		    BIT(4)  /* Baud Rate Count Mode */
#define	DLLTE		    BIT(3)  /* DLL Test Enable */
#define	DLL1P8E		    BIT(2)  /* DLL 1.8432M Enable */
#define	TXDCKG		    BIT(1)  /* TXD Clock Gating */
#define	SCKS	        BIT(0)  /* Slow Clock Select */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* CIR Baud Rate Divisor Low Byte Register */
#define	C0BDLR			ECReg(EC_Register_Base_Address+0x2308)

/* CIR Baud Rate Divisor High Byte Register */
#define	C0BDHR			ECReg(EC_Register_Base_Address+0x2309)

/* CIR Transmitter FIFO Status Register */
#define	C0TFSR			ECReg(EC_Register_Base_Address+0x230A)

/* CIR Receiver FIFO Status Register */
#define	C0RFSR			ECReg(EC_Register_Base_Address+0x230B)

/* CIR Wakeup Code Set Select Register */
#define	C0WCSSR			ECReg(EC_Register_Base_Address+0x230C)

/* CIR Wakeup Code Length Register */
#define	C0WCL			ECReg(EC_Register_Base_Address+0x230D)

/* CIR Wakeup Code Read/Write Register */
#define	C0WCR			ECReg(EC_Register_Base_Address+0x230E)

/* CIR Wakeup Code Power Control/Status Register */
#define	C0WPS			ECReg(EC_Register_Base_Address+0x230F)

/* CIR Scratch Register */
#define	CSCRR			ECReg(EC_Register_Base_Address+0x2310)

/* 
 * ****************************************************************************
 * (25xxh) Debugger (DBGR) 
 * ****************************************************************************
 */
/* Trigger 1 Address Low Byte Register */
#define BKA1L           ECReg(EC_Register_Base_Address+0x2510)

/* Trigger 1 Address Middle Byte Register */
#define BKA1M           ECReg(EC_Register_Base_Address+0x2511)

/* Trigger 1 Address High Byte Register */
#define BKA1H           ECReg(EC_Register_Base_Address+0x2512)

/* Trigger 2 Address Low Byte Register */
#define BKA2L           ECReg(EC_Register_Base_Address+0x2513)

/* Trigger 2 Address Middle Byte Register */
#define BKA2M           ECReg(EC_Register_Base_Address+0x2514)

/* Trigger 2 Address High Byte Register */
#define BKA2H           ECReg(EC_Register_Base_Address+0x2515)

/* Trigger 3 Address Low Byte Register */
#define BKA3L           ECReg(EC_Register_Base_Address+0x2516)

/* Trigger 3 Address Middle Byte Register */
#define BKA3M           ECReg(EC_Register_Base_Address+0x2517)

/* Trigger 3 Address High Byte Register */
#define BKA3H           ECReg(EC_Register_Base_Address+0x2518)

/* 
 * ****************************************************************************
 * (26xxh) Serial Peripheral Interface (SSPI) 
 * ****************************************************************************
 */
/* SPI Data Register */
#define SPIDATA			ECReg(EC_Register_Base_Address+0x2600)

/* SPI Control 1 Register */
#define SPICTRL1		ECReg(EC_Register_Base_Address+0x2601)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define CHPOL		    BIT(7)  /* Chip Select Polarity */
#define CLPOL           BIT(6)  /* Clock Polarity */
#define CLPHS           BIT(5)  /* Clock Phase */
#define SCKFREQ2        BIT(4)
#define SCKFREQ1        BIT(3)
#define SCKFREQ0        BIT(2)  /* SSCK Frequency */
#define NTREN           BIT(1)  /* Interrupt Enable */
#define WIRECH0         BIT(0)  /*
                                 * Device0 3-Wire Mode, 0: Disable (4-wire),
                                 * 1: Enable (3-wire)
                                 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI Control 2 Register */
#define SPICTRL2		ECReg(EC_Register_Base_Address+0x2602)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define HBANK           BIT(7)  /* Host Side Bank */
#define DEVBUSYPOL      BIT(6)  /* Device Busy Polarity */
#define BYTEWIDTH2      BIT(5)  /* Byte Width */
#define BYTEWIDTH1      BIT(4)  /* Byte Width */
#define BYTEWIDTH0      BIT(3)  /* Byte Width */
#define CHRW            BIT(2)  /* Channel Read/Write Cycle */
#define BLKSEL          BIT(1)  /* Block Select */
#define WIRECH1         BIT(0)  /* 
                                 * Device1 3-Wire Mode, 0: Disable (4-wire),
                                 * 1: Enable (3-wire)
                                 */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI Control 3 Register */
#define SPICTRL3		ECReg(EC_Register_Base_Address+0x2604)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DEVBUSYMODE     BIT(3)  /* Device Busy Mode */
#define CSPOLSEL        BIT(2)  /* Chip Select Polarity Select */
#define CHPOL1          BIT(1)  /* Chip Select Polarity 1 */
#define BUSYNOCLK       BIT(0)  /* No BUSY Clock */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* SPI Status Register */
#define SPISTS			ECReg(EC_Register_Base_Address+0x2603)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define WAITBUSYSTART   BIT(7)  /* Wait Busy Start Signal */
#define DEVBUSY         BIT(6)  /* Device Busy Signal */
#define TRANEND         BIT(5)  /* SPI Transmission End */
#define CH0START        BIT(4)  /* Channel 0 Start Signal */
#define CH1START        BIT(3)  /* Channel 1 Start Signal */
#define TRANIP          BIT(2)  /* Transfer In Progress */
#define TRANENDIF       BIT(1)  /* Transfer End Flag */
#define SPIBUSY         BIT(0)  /* SPI Busy */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* 
 * ****************************************************************************
 * (27xxh) Extern Serial Port (UART1)
 * ****************************************************************************
 */
#define REG_UART1_BASE  (EC_Register_Base_Address+0x2700)

/* Receiver Buffer Register */
#define UART1_RBR       ECReg(EC_Register_Base_Address+0x2700)

/* Interrupt Enable Register */
#define UART1_IER       ECReg(EC_Register_Base_Address+0x2701)

/* Interrupt Identification Register */
#define UART1_IIR       ECReg(EC_Register_Base_Address+0x2702)

/* Line Control Register */
#define UART1_LCR       ECReg(EC_Register_Base_Address+0x2703)

/* Modem Control Register */
#define UART1_MCR       ECReg(EC_Register_Base_Address+0x2704)

/* Line Status Register */
#define UART1_LSR       ECReg(EC_Register_Base_Address+0x2705)

/* Modem Status Register */
#define UART1_MSR       ECReg(EC_Register_Base_Address+0x2706)

/* Scratch Pad Register */
#define UART1_SCR       ECReg(EC_Register_Base_Address+0x2707)

/* EC Serial Port Mode Register */
#define UART1_ECSPMR    ECReg(EC_Register_Base_Address+0x2708)

/* EC Serial Port Prescaler Register */
#define UART1_SPPR      ECReg(EC_Register_Base_Address+0x2709)

/* 
 * ****************************************************************************
 * (28xxh) Extern Serial Port (UART2)
 * ****************************************************************************
 */
#define REG_UART2_BASE  (EC_Register_Base_Address+0x2800)

/* Receiver Buffer Register */
#define UART2_RBR		ECReg(EC_Register_Base_Address+0x2800)

/* Interrupt Enable Register */
#define UART2_IER		ECReg(EC_Register_Base_Address+0x2801)

/* Interrupt Identification Register */
#define UART2_IIR		ECReg(EC_Register_Base_Address+0x2802)

/* Line Control Register */
#define UART2_LCR		ECReg(EC_Register_Base_Address+0x2803)

/* Modem Control Register */
#define UART2_MCR		ECReg(EC_Register_Base_Address+0x2804)

/* Line Status Register */
#define UART2_LSR		ECReg(EC_Register_Base_Address+0x2805)

/* Modem Status Register */
#define UART2_MSR		ECReg(EC_Register_Base_Address+0x2806)

/* Scratch Pad Register */
#define UART2_SCR		ECReg(EC_Register_Base_Address+0x2807)

/* EC Serial Port Mode Register */
#define UART2_ECSPMR	ECReg(EC_Register_Base_Address+0x2808)

/* 
 * ****************************************************************************
 * (29xxh) 8 Bit Timer (TMR)
 * ****************************************************************************
 */
/* TMR Prescaler Register */
#define	PRSC			ECReg(EC_Register_Base_Address+0x2900)

/* Group Clock Source and Mode Select Register */
#define	GCSMS			ECReg(EC_Register_Base_Address+0x2901)

/* A0 Cycle Time Register */
#define	CTR_A0			ECReg(EC_Register_Base_Address+0x2902)

/* A1 Cycle Time Register */
#define	CTR_A1			ECReg(EC_Register_Base_Address+0x2903)

/* B0 Cycle Time Register */
#define	CTR_B0			ECReg(EC_Register_Base_Address+0x2904)

/* B1 Cycle Time Register */
#define	CTR_B1			ECReg(EC_Register_Base_Address+0x2905)

/* A0 Duty Time Register */
#define	DCR_A0			ECReg(EC_Register_Base_Address+0x2906)

/* A1 Duty Time Register */
#define	DCR_A1			ECReg(EC_Register_Base_Address+0x2907)

/* B0 Duty Time Register */
#define	DCR_B0			ECReg(EC_Register_Base_Address+0x2908)

/* B1 Duty Time Register */
#define	DCR_B1			ECReg(EC_Register_Base_Address+0x2909)

/* Channel Clock Group Select Register */
#define	CCGSR			ECReg(EC_Register_Base_Address+0x290A)

/* TMR Clock Enable Register */
#define	TMRCE			ECReg(EC_Register_Base_Address+0x290B)

/* TMR Interrupt Enable Register */
#define	TMEIE			ECReg(EC_Register_Base_Address+0x290C)

/* 
 * ****************************************************************************
 * (2Cxxh) Platform Environment Control Interface (PECI)
 * ****************************************************************************
 */
/* Host Status Register */
#define HOSTAR          ECReg(EC_Register_Base_Address+0x2C00)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define TEMPERR         BIT(7)  /* PECI Get Temperature Error */
#define BUSERR          BIT(6)  /* PECI Bus Abnormal/Contention Error */
#define EXTERR          BIT(5)  /*
                                 * PECI Slave Message Phase t-bit Extend
                                 * over Error
                                 */
#define WR_FCS_ERR      BIT(3)  /* Write_FCS Error */
#define RD_FCS_ERR      BIT(2)  /* Read_FCS Error */
#define FINISH          BIT(1)  /* Finish */
#define HOBY            BIT(0)  /* Host Busy */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host Control Register */
#define HOCTLR          ECReg(EC_Register_Base_Address+0x2C01)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define FIFOCLR         BIT(5)  /* Data FIFO Pointer Clear */
#define FCSERR_ABT      BIT(4)  /* PECI Host Auto-abort at FCS_Error */
#define PECIHEN         BIT(3)  /* PECI Host Enable */
#define CONCTRL         BIT(2)  /* PECI Contention Control */
#define AWFCS_EN        BIT(1)  /* Assured Write FCS Enable */
#define START           BIT(0)  /* Start */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* Host Command Register */
#define HOCMDR          ECReg(EC_Register_Base_Address+0x2C02)

/* Host Target Address Register */
#define HOTRADDR        ECReg(EC_Register_Base_Address+0x2C03)

/* Host Write Length Register */
#define HOWRLR          ECReg(EC_Register_Base_Address+0x2C04)

/* Host Read Length Register */
#define HORDLR          ECReg(EC_Register_Base_Address+0x2C05)

/* Host Write Data Register */
#define HOWRDR          ECReg(EC_Register_Base_Address+0x2C06)

/* Host Read Data Register */
#define HORDDR          ECReg(EC_Register_Base_Address+0x2C07)

/* Host Control 2 Register */
#define HOCTL2R         ECReg(EC_Register_Base_Address+0x2C08)

/* Received Write FCS Value */
#define RWFCSV          ECReg(EC_Register_Base_Address+0x2C09)

/* Received Read FCS Value */
#define RRFCSV          ECReg(EC_Register_Base_Address+0x2C0A)

/* Write FCS Value */
#define WFCSV           ECReg(EC_Register_Base_Address+0x2C0B)

/* Read FCS Falue */
#define RFCSV           ECReg(EC_Register_Base_Address+0x2C0C)

/* Assured Write FCS Value */
#define AWFCSV          ECReg(EC_Register_Base_Address+0x2C0D)

/* Pad Control Register */
#define PADCTLR         ECReg(EC_Register_Base_Address+0x2C0E)

#if 0
/* 
 * ****************************************************************************
 * (2Dxxh) I2C/JTAG
 * ****************************************************************************
 */
#define I2C_REG_BASE    (EC_Register_Base_Address + 0x2D00)

#define I2C_DRR_D	    ECReg(EC_Register_Base_Address + 0x2D00)
#define I2C_DRR_E		ECReg(EC_Register_Base_Address + 0x2D40)
#define I2C_DRR_F		ECReg(EC_Register_Base_Address + 0x2D80)
#define I2C_PSR_D		ECReg(EC_Register_Base_Address + 0x2D01)
#define I2C_PSR_E		ECReg(EC_Register_Base_Address + 0x2D41)
#define I2C_PSR_F		ECReg(EC_Register_Base_Address + 0x2D81)
#define I2C_STR_D		ECReg(EC_Register_Base_Address + 0x2D03)
#define I2C_STR_E		ECReg(EC_Register_Base_Address + 0x2D43)
#define I2C_STR_F		ECReg(EC_Register_Base_Address + 0x2D83)
#define I2C_RDST_D		ECReg(EC_Register_Base_Address + 0x2D04)
#define I2C_RDST_E		ECReg(EC_Register_Base_Address + 0x2D44)
#define I2C_RDST_F		ECReg(EC_Register_Base_Address + 0x2D84)
#define I2C_TOR_D		ECReg(EC_Register_Base_Address + 0x2D05)
#define I2C_TOR_E		ECReg(EC_Register_Base_Address + 0x2D45)
#define I2C_TOR_F		ECReg(EC_Register_Base_Address + 0x2D85)
#define I2C_IDR_D		ECReg(EC_Register_Base_Address + 0x2D06)
#define I2C_IDR_E		ECReg(EC_Register_Base_Address + 0x2D46)
#define I2C_IDR_F		ECReg(EC_Register_Base_Address + 0x2D86)
#define I2C_IDR2_D		ECReg(EC_Register_Base_Address + 0x2D07)
#define I2C_IDR2_E		ECReg(EC_Register_Base_Address + 0x2D47)
#define I2C_IDR2_F		ECReg(EC_Register_Base_Address + 0x2D87)
#define I2C_DTR_D		ECReg(EC_Register_Base_Address + 0x2D08)
#define I2C_DTR_E		ECReg(EC_Register_Base_Address + 0x2D48)
#define I2C_DTR_F		ECReg(EC_Register_Base_Address + 0x2D88)
#define I2C_CTR_D		ECReg(EC_Register_Base_Address + 0x2D09)
#define I2C_CTR_E		ECReg(EC_Register_Base_Address + 0x2D49)
#define I2C_CTR_F		ECReg(EC_Register_Base_Address + 0x2D89)
#define I2C_CTR1_D		ECReg(EC_Register_Base_Address + 0x2D0A)
#define I2C_CTR1_E		ECReg(EC_Register_Base_Address + 0x2D4A)
#define I2C_CTR1_F		ECReg(EC_Register_Base_Address + 0x2D8A)
#define I2C_CTR2_D		ECReg(EC_Register_Base_Address + 0x2D0B)
#define I2C_CTR2_E		ECReg(EC_Register_Base_Address + 0x2D4B)
#define I2C_CTR2_F		ECReg(EC_Register_Base_Address + 0x2D8B)
#define I2C_CTR3_D		ECReg(EC_Register_Base_Address + 0x2D0C)
#define I2C_CTR3_E		ECReg(EC_Register_Base_Address + 0x2D4C)
#define I2C_CTR3_F		ECReg(EC_Register_Base_Address + 0x2D8C)
#define I2C_CTR4_D		ECReg(EC_Register_Base_Address + 0x2D3A)
#define I2C_CTR4_E		ECReg(EC_Register_Base_Address + 0x2D7A)
#define I2C_CTR4_F		ECReg(EC_Register_Base_Address + 0x2DBA)
#define I2C_CTR5_D		ECReg(EC_Register_Base_Address + 0x2D38)
#define I2C_CTR5_E		ECReg(EC_Register_Base_Address + 0x2D78)
#define I2C_CTR5_F		ECReg(EC_Register_Base_Address + 0x2DB8)
#define I2C_NUM1_D		ECReg(EC_Register_Base_Address + 0x2D0D)
#define I2C_NUM1_E		ECReg(EC_Register_Base_Address + 0x2D4D)
#define I2C_NUM1_F		ECReg(EC_Register_Base_Address + 0x2D8D)
#define I2C_NUM2_D		ECReg(EC_Register_Base_Address + 0x2D0E)
#define I2C_NUM2_E		ECReg(EC_Register_Base_Address + 0x2D4E)
#define I2C_NUM2_F		ECReg(EC_Register_Base_Address + 0x2D8E)
#define I2C_NUM3_D		ECReg(EC_Register_Base_Address + 0x2D0F)
#define I2C_NUM3_E		ECReg(EC_Register_Base_Address + 0x2D4F)
#define I2C_NUM3_F		ECReg(EC_Register_Base_Address + 0x2D8F)
#define I2C_NUM4_D		ECReg(EC_Register_Base_Address + 0x2D26)
#define I2C_NUM4_E		ECReg(EC_Register_Base_Address + 0x2D66)
#define I2C_NUM4_F		ECReg(EC_Register_Base_Address + 0x2DA6)
#define I2C_ID1_D		ECReg(EC_Register_Base_Address + 0x2D10)
#define I2C_ID1_E		ECReg(EC_Register_Base_Address + 0x2D50)
#define I2C_ID1_F		ECReg(EC_Register_Base_Address + 0x2D90)
#define I2C_ID2_D		ECReg(EC_Register_Base_Address + 0x2D11)
#define I2C_ID2_E		ECReg(EC_Register_Base_Address + 0x2D51)
#define I2C_ID2_F		ECReg(EC_Register_Base_Address + 0x2D91)
#define I2C_ID3_D		ECReg(EC_Register_Base_Address + 0x2D12)
#define I2C_ID3_E		ECReg(EC_Register_Base_Address + 0x2D52)
#define I2C_ID3_F		ECReg(EC_Register_Base_Address + 0x2D92)
#define I2C_ID4_D		ECReg(EC_Register_Base_Address + 0x2D27)
#define I2C_ID4_E		ECReg(EC_Register_Base_Address + 0x2D67)
#define I2C_ID4_F		ECReg(EC_Register_Base_Address + 0x2DA7)
#define I2C_ADDR1_D		ECReg(EC_Register_Base_Address + 0x2D13)
#define I2C_ADDR1_E		ECReg(EC_Register_Base_Address + 0x2D53)
#define I2C_ADDR1_F		ECReg(EC_Register_Base_Address + 0x2D93)
#define I2C_ADDR2_D		ECReg(EC_Register_Base_Address + 0x2D14)
#define I2C_ADDR2_E		ECReg(EC_Register_Base_Address + 0x2D54)
#define I2C_ADDR2_F		ECReg(EC_Register_Base_Address + 0x2D94)
#define I2C_ADDR3_D		ECReg(EC_Register_Base_Address + 0x2D15)
#define I2C_ADDR3_E		ECReg(EC_Register_Base_Address + 0x2D55)
#define I2C_ADDR3_F		ECReg(EC_Register_Base_Address + 0x2D95)
#define I2C_ADDR4_D		ECReg(EC_Register_Base_Address + 0x2D28)
#define I2C_ADDR4_E		ECReg(EC_Register_Base_Address + 0x2D68)
#define I2C_ADDR4_F		ECReg(EC_Register_Base_Address + 0x2DA8)
#define I2C_RAMHA1_D	ECReg(EC_Register_Base_Address + 0x2D16)
#define I2C_RAMHA1_E	ECReg(EC_Register_Base_Address + 0x2D56)
#define I2C_RAMHA1_F	ECReg(EC_Register_Base_Address + 0x2D96)
#define I2C_RAMLA1_D	ECReg(EC_Register_Base_Address + 0x2D17)
#define I2C_RAMLA1_E	ECReg(EC_Register_Base_Address + 0x2D57)
#define I2C_RAMLA1_F	ECReg(EC_Register_Base_Address + 0x2D97)
#define I2C_RAMHA2_D	ECReg(EC_Register_Base_Address + 0x2D2D)
#define I2C_RAMHA2_E	ECReg(EC_Register_Base_Address + 0x2D6D)
#define I2C_RAMHA2_F	ECReg(EC_Register_Base_Address + 0x2DAD)
#define I2C_RAMLA2_D	ECReg(EC_Register_Base_Address + 0x2D2E)
#define I2C_RAMLA2_E	ECReg(EC_Register_Base_Address + 0x2D6E)
#define I2C_RAMLA2_F	ECReg(EC_Register_Base_Address + 0x2DAE)
#define I2C_RAMHA3_D	ECReg(EC_Register_Base_Address + 0x2D2F)
#define I2C_RAMHA3_E	ECReg(EC_Register_Base_Address + 0x2D6F)
#define I2C_RAMHA3_F	ECReg(EC_Register_Base_Address + 0x2DAF)
#define I2C_RAMLA3_D	ECReg(EC_Register_Base_Address + 0x2D30)
#define I2C_RAMLA3_E	ECReg(EC_Register_Base_Address + 0x2D70)
#define I2C_RAMLA3_F	ECReg(EC_Register_Base_Address + 0x2DB0)
#define I2C_RAMHA4_D	ECReg(EC_Register_Base_Address + 0x2D31)
#define I2C_RAMHA4_E	ECReg(EC_Register_Base_Address + 0x2D71)
#define I2C_RAMHA4_F	ECReg(EC_Register_Base_Address + 0x2DB1)
#define I2C_RAMLA4_D	ECReg(EC_Register_Base_Address + 0x2D32)
#define I2C_RAMLA4_E	ECReg(EC_Register_Base_Address + 0x2D72)
#define I2C_RAMLA4_F	ECReg(EC_Register_Base_Address + 0x2DB2)
#define I2C_CSR_D		ECReg(EC_Register_Base_Address + 0x2D18)
#define I2C_CSR_E		ECReg(EC_Register_Base_Address + 0x2D58)
#define I2C_CSR_F		ECReg(EC_Register_Base_Address + 0x2D98)
#define I2C_WCSR1_D		ECReg(EC_Register_Base_Address + 0x2D19)
#define I2C_WCSR1_E		ECReg(EC_Register_Base_Address + 0x2D59)
#define I2C_WCSR1_F		ECReg(EC_Register_Base_Address + 0x2D99)
#define I2C_WCSR2_D		ECReg(EC_Register_Base_Address + 0x2D1A)
#define I2C_WCSR2_E		ECReg(EC_Register_Base_Address + 0x2D5A)
#define I2C_WCSR2_F		ECReg(EC_Register_Base_Address + 0x2D9A)
#define I2C_WCSR3_D		ECReg(EC_Register_Base_Address + 0x2D1B)
#define I2C_WCSR3_E		ECReg(EC_Register_Base_Address + 0x2D5B)
#define I2C_WCSR3_F		ECReg(EC_Register_Base_Address + 0x2D9B)
#define I2C_WCSR4_D		ECReg(EC_Register_Base_Address + 0x2D29)
#define I2C_WCSR4_E		ECReg(EC_Register_Base_Address + 0x2D69)
#define I2C_WCSR4_F		ECReg(EC_Register_Base_Address + 0x2DA9)
#define I2C_DRR1_D		ECReg(EC_Register_Base_Address + 0x2D1C)
#define I2C_DRR1_E		ECReg(EC_Register_Base_Address + 0x2D5C)
#define I2C_DRR1_F		ECReg(EC_Register_Base_Address + 0x2D9C)
#define I2C_DRR2_D		ECReg(EC_Register_Base_Address + 0x2D1D)
#define I2C_DRR2_E		ECReg(EC_Register_Base_Address + 0x2D5D)
#define I2C_DRR2_F		ECReg(EC_Register_Base_Address + 0x2D9D)
#define I2C_DRR3_D		ECReg(EC_Register_Base_Address + 0x2D1E)
#define I2C_DRR3_E		ECReg(EC_Register_Base_Address + 0x2D5E)
#define I2C_DRR3_F		ECReg(EC_Register_Base_Address + 0x2D9E)
#define I2C_DRR4_D		ECReg(EC_Register_Base_Address + 0x2D2A)
#define I2C_DRR4_E		ECReg(EC_Register_Base_Address + 0x2D6A)
#define I2C_DRR4_F		ECReg(EC_Register_Base_Address + 0x2DAA)
#define I2C_ERRST_D		ECReg(EC_Register_Base_Address + 0x2D1F)
#define I2C_ERRST_E		ECReg(EC_Register_Base_Address + 0x2D5F)
#define I2C_ERRST_F		ECReg(EC_Register_Base_Address + 0x2D9F)
#define I2C_FST_EM_D	ECReg(EC_Register_Base_Address + 0x2D20)
#define I2C_FST_EM_E	ECReg(EC_Register_Base_Address + 0x2D60)
#define I2C_FST_EM_F	ECReg(EC_Register_Base_Address + 0x2DA0)
#define I2C_SAR2_D		ECReg(EC_Register_Base_Address + 0x2D21)
#define I2C_SAR2_E		ECReg(EC_Register_Base_Address + 0x2D61)
#define I2C_SAR2_F		ECReg(EC_Register_Base_Address + 0x2DA1)
#define I2C_SAR3_D		ECReg(EC_Register_Base_Address + 0x2D22)
#define I2C_SAR3_E		ECReg(EC_Register_Base_Address + 0x2D62)
#define I2C_SAR3_F		ECReg(EC_Register_Base_Address + 0x2DA2)
#define I2C_SAR4_D		ECReg(EC_Register_Base_Address + 0x2D2B)
#define I2C_SAR4_E		ECReg(EC_Register_Base_Address + 0x2D6B)
#define I2C_SAR4_F		ECReg(EC_Register_Base_Address + 0x2DAB)
#define I2C_LNGR1_D		ECReg(EC_Register_Base_Address + 0x2D33)
#define I2C_LNGR1_E		ECReg(EC_Register_Base_Address + 0x2D73)
#define I2C_LNGR1_F		ECReg(EC_Register_Base_Address + 0x2DB3)
#define I2C_LNGR2_D		ECReg(EC_Register_Base_Address + 0x2D34)
#define I2C_LNGR2_E		ECReg(EC_Register_Base_Address + 0x2D74)
#define I2C_LNGR2_F		ECReg(EC_Register_Base_Address + 0x2DB4)
#define I2C_LNGST1_D	ECReg(EC_Register_Base_Address + 0x2D35)
#define I2C_LNGST1_E	ECReg(EC_Register_Base_Address + 0x2D75)
#define I2C_LNGST1_F	ECReg(EC_Register_Base_Address + 0x2DB5)
#define I2C_LNGST2_D	ECReg(EC_Register_Base_Address + 0x2D36)
#define I2C_LNGST2_E	ECReg(EC_Register_Base_Address + 0x2D76)
#define I2C_LNGST2_F	ECReg(EC_Register_Base_Address + 0x2DB6)
#define I2C_NUMSRD_D	ECReg(EC_Register_Base_Address + 0x2D39)
#define I2C_NUMSRD_E	ECReg(EC_Register_Base_Address + 0x2D79)
#define I2C_NUMSRD_F	ECReg(EC_Register_Base_Address + 0x2DB9)

/* Define I2C D Register */
#define I2C_DATA_RX(x)      ( 0x00 + I2C_REG_BASE + (x  << 6))
#define I2C_PRESCALE(x)     ( 0x01 + I2C_REG_BASE + (x  << 6))
#define I2C_HSPR(x)         ( 0x02 + I2C_REG_BASE + (x  << 6))
#define I2C_STAT(x)         ( 0x03 + I2C_REG_BASE + (x  << 6))
#define I2C_RD_STAT(x)      ( 0x04 + I2C_REG_BASE + (x  << 6))
#define I2C_TIME_OUT(x)     ( 0x05 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD(x)          ( 0x06 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD2(x)         ( 0x07 + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_TX(x)      ( 0x08 + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL(x)      ( 0x09 + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL1(x)     ( 0x0A + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL2(x)     ( 0x0B + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL3(x)     ( 0x0C + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL4(x)     ( 0x3A + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL5(x)     ( 0x38 + I2C_REG_BASE + (x  << 6))
#define I2C_CONTROL6(x)     ( 0x39 + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM1(x)    ( 0x0D + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM2(x)    ( 0x0E + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM3(x)    ( 0x0F + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM4(x)    ( 0x26 + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM5(x)    ( 0x8F + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_NUM6(x)    ( 0xA6 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID1(x)    ( 0x10 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID2(x)    ( 0x11 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID3(x)    ( 0x12 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID4(x)    ( 0x27 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD1(x)   ( 0x13 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID5(x)    ( 0x92 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ID6(x)    ( 0xA7 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD2(x)   ( 0x14 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD3(x)   ( 0x15 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD4(x)   ( 0x28 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD5(x)   ( 0x95 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE_ADD6(x)   ( 0xA8 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H(x)        ( 0x16 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L(x)        ( 0x17 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H2(x)       ( 0x2D + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L2(x)       ( 0x2E + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H3(x)       ( 0x2F + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L3(x)       ( 0x30 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H4(x)       ( 0x31 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L4(x)       ( 0x32 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H5(x)       ( 0xAF + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L5(x)       ( 0xB0 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_H6(x)       ( 0xB1 + I2C_REG_BASE + (x  << 6))
#define I2C_ADD_L6(x)       ( 0xB2 + I2C_REG_BASE + (x  << 6))
#define I2C_CSR(x)          ( 0x18 + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR1(x)        ( 0x19 + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR2(x)        ( 0x1A + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR3(x)        ( 0x1B + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR4(x)        ( 0x29 + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR5(x)        ( 0x9B + I2C_REG_BASE + (x  << 6))
#define I2C_WTSR6(x)        ( 0xA9 + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_RX1(x)     ( 0x1C + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_RX2(x)     ( 0x1D + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_RX3(x)     ( 0x1E + I2C_REG_BASE + (x  << 6))
#define I2C_DATA_RX4(x)     ( 0x2A + I2C_REG_BASE + (x  << 6))
#define I2C_ERROR_STAT(x)   ( 0x1F + I2C_REG_BASE + (x  << 6))
#define I2C_ERROR_MASK(x)   ( 0x20 + I2C_REG_BASE + (x  << 6))
#define I2C_CTR1_D56(x)     ( 0x1E + I2C_REG_BASE + (x  << 6))
#define I2C_CTR2_D56(x)     ( 0x2A + I2C_REG_BASE + (x  << 6))
#define I2C_CTR3_D56(x)     ( 0x21 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE2_ADD(x)   ( 0x21 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE3_ADD(x)   ( 0x22 + I2C_REG_BASE + (x  << 6))
#define I2C_SLAVE4_ADD(x)   ( 0x2B + I2C_REG_BASE + (x  << 6))
#define I2C_M_CODE_S1(x)    ( 0x23 + I2C_REG_BASE + (x  << 6))
#define I2C_M_CODE_S2(x)    ( 0x24 + I2C_REG_BASE + (x  << 6))
#define I2C_M_CODE_S3(x)    ( 0x25 + I2C_REG_BASE + (x  << 6))
#define I2C_M_CODE_S4(x)    ( 0x2C + I2C_REG_BASE + (x  << 6))
#define I2C_DMA_DATA_LEN1(x)    ( 0x33 + I2C_REG_BASE + (x  << 6))
#define I2C_DMA_DATA_LEN2(x)    ( 0x34 + I2C_REG_BASE + (x  << 6))
#define I2C_LEN_ST1(x)      ( 0x35 + I2C_REG_BASE + (x  << 6))
#define I2C_LEN_ST2(x)      ( 0x36 + I2C_REG_BASE + (x  << 6))
#define I2C_LEN_ST3(x)      ( 0xB6 + I2C_REG_BASE + (x  << 6))
#define I2C_NSWDST(x)       ( 0x39 + I2C_REG_BASE + (x  << 6))
#define I2C_STR2(x)         ( 0x37 + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH1(x)        ( 0x3B + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH2(x)        ( 0x3C + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH3(x)        ( 0x3D + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH4(x)        ( 0x3E + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH5(x)        ( 0xBD + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMH6(x)        ( 0xBE + I2C_REG_BASE + (x  << 6))  
#define I2C_NUMSRHD(x)      ( 0x3F + I2C_REG_BASE + (x  << 6))  

#define I2C_DEV1_LEN(x)     ( EC_Register_Base_Address + 0x1C7F + (x  << 6))
#define I2C_DEV2_LEN(x)     ( EC_Register_Base_Address + 0x1C7E + (x  << 6))
#define I2C_DEV3_LEN(x)     ( EC_Register_Base_Address + 0x1C7D + (x  << 6))
#define I2C_DEV4_LEN(x)     ( EC_Register_Base_Address + 0x1C7C + (x  << 6))
#define I2C_DEV5_LEN(x)     ( EC_Register_Base_Address + 0x1C7B + (x  << 6))
#define I2C_DEV6_LEN(x)     ( EC_Register_Base_Address + 0x1C7A + (x  << 6))
#define I2C_LNGST_H1(x)     ( EC_Register_Base_Address + 0x1C79 + (x  << 6))
#define I2C_LNGST_H2(x)     ( EC_Register_Base_Address + 0x1C78 + (x  << 6))
#define I2C_LNGST_H3(x)     ( EC_Register_Base_Address + 0x1C77 + (x  << 6))
#define I2C_LNGST_H4(x)     ( EC_Register_Base_Address + 0x1C76 + (x  << 6))
#define I2C_LNGST_H5(x)     ( EC_Register_Base_Address + 0x1C75 + (x  << 6))
#define I2C_LNGST_H6(x)     ( EC_Register_Base_Address + 0x1C74 + (x  << 6))
#define I2C_SWAP12          ( EC_Register_Base_Address + 0x1C20)
#define I2C_SWAP34          ( EC_Register_Base_Address + 0x1C21)
#define I2C_SWAP56          ( EC_Register_Base_Address + 0x1C11)
#endif

/* 
 * ****************************************************************************
 * (2Exxh) Consumer Electronics Control (CEC)
 * ****************************************************************************
 */
/* CEC Data Register */
#define CECDR           ECReg(EC_Register_Base_Address+0x2E00)

/* CEC FIFO Status Register */
#define CECFSTS         ECReg(EC_Register_Base_Address+0x2E01)

/* CEC Device Logical Address Register */
#define CECDLA          ECReg(EC_Register_Base_Address+0x2E02)

/* CEC Control Register */
#define CECCTRL         ECReg(EC_Register_Base_Address+0x2E03)

/* CEC Status Register */
#define CECSTS          ECReg(EC_Register_Base_Address+0x2E04)

/* CEC Interrupt Enable Register */
#define CECIE           ECReg(EC_Register_Base_Address+0x2E05)

/* CEC Operation Status Register */
#define CECOPSTS        ECReg(EC_Register_Base_Address+0x2E06)

/* CEC Received Header Register */
#define CECCRH          ECReg(EC_Register_Base_Address+0x2E07)

/* 
 * ****************************************************************************
 * (2Fxxh) USB
 * ****************************************************************************
 */
#define Host_Int_Status_Register  		ECReg(EC_Register_Base_Address+0x2F08)
#define Host_RX_Connect_State  			ECReg(EC_Register_Base_Address+0x2F0E)
//0623
#define USB_SC_ADDRESS 				ECReg(EC_Register_Base_Address+0x2F54) 
#define Port0_MISC_Control          	ECReg(EC_Register_Base_Address+0x2FE4)
#define SWITCH_SEL         		BIT(6)  
#define IPAD_SWITCH      		BIT(5)  
#define PULL_DOWN_EN        	BIT(4)  
#define DCP_EN         			BIT(3)  
#define CHG_EN       	 		BIT(2)  
#define PWR_EN           		BIT(1)  
#define VSRC_EN          		BIT(0) 


#define  DCP_MODE		0x08
#define  CDP_MODE		0x11
#define  BY_PASS_MODE	0x40
#define  Port1_MISC_STATE_Control          	ECReg(EC_Register_Base_Address+0x2FE6) //         	ECReg(EC_Register_Base_Address+0x2FE6)

#endif /* CHIP_CHIPREGS_H */


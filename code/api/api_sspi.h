/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_sspi.h
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */
 
#ifndef API_SSPI_H
#define API_SSPI_H

/* 
 * ****************************************************************************
 * definition
 * ****************************************************************************
 */
#define SPI_Channel_0       0x00
#define SPI_Channel_1       0x01
#define SPI_Channel_0N1     0x02

#define SPI_SupportBusy     0x01
#define SPI_NoSupportBusy   0x00

#define SSCK                GPCRA6
#define SMOSI               GPCRC3
#define SMISO               GPCRC5
#define SSCE1               GPCRG0
#define SSCE0               GPCRG2

/* 
 * ****************************************************************************
 * function prototype
 * ****************************************************************************
 */
extern void Init_SPIInterface(BYTE channel, BYTE supportbusy);
extern void Disable_SPIInterface(BYTE channel);
extern void Enable_SPIInterface(void);
extern void SPITransmissionEnd(void);
extern void SPIWriteByte(BYTE spidata, BYTE channel);
extern void SPIWriteWord(BYTE MSB, BYTE LSB, BYTE channel);
extern BYTE SPIReadByte(BYTE channel);
extern WORD SPIReadWord(BYTE channel);

#endif

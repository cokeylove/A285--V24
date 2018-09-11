/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_sspi.c
 * Dino Li
 * Version, 1.10
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/**
 * ****************************************************************************
 * Init SPI interface function
 *
 * @return
 *
 * @parameter
 * channel, SPI_Channel_0   SPI channel 0 enable.
 *          SPI_Channel_1   SPI channel 1 enable.
 *          SPI_Channel_0N1 SPI channel 0 and 1 enable.
 * supportbusy
 *
 * ****************************************************************************
 */
void Init_SPIInterface(BYTE channel, BYTE supportbusy)
{
    BYTE settingOK;

    /* Pr-set OK */
    settingOK = 0x01;
    
    if(channel==SPI_Channel_0)
    {
        GCR1 |= SPICTRL_0;
        SSCE0 =ALT;
    }
    else if(channel==SPI_Channel_1)
    {
        GCR1 |= SPICTRL_1;
        SSCE1 =ALT;
    }
    else if(channel==SPI_Channel_0N1)
    {
        GCR1 |= (SPICTRL_0+SPICTRL_1);
        SSCE0 =ALT;
        SSCE1 =ALT;
    }
    else
    {
        GCR1 &= ~(SPICTRL_0+SPICTRL_1);
        settingOK = 0x00;
    }

    if(settingOK==0x01)
    {
        if(supportbusy == SPI_SupportBusy)
        {
            GCR1 |= SSSPIBP;
        }
        
        SSCK = ALT;
        SMOSI = ALT;
        SMISO = ALT;

        /* 100b: 1/10 FreqEC, Interrupt Enable, 4-wire */
        /* SPICTRL1 |= (SCKFREQ2+NTREN); */

        /* 100b: 1/10 FreqEC, Interrupt Disable, 4-wir, mode 3 */
        SPICTRL1 |= SCKFREQ2+CLPOL+CLPHS;
    }
}

/**
 * ****************************************************************************
 * Disable SPI interface function
 *
 * @return
 *
 * @parameter
 * channel, SPI_Channel_0   SPI channel 0 enable.
 *          SPI_Channel_1   SPI channel 1 enable.
 *          SPI_Channel_0N1 SPI channel 0 and 1 enable.
 *
 * ****************************************************************************
 */
void Disable_SPIInterface(BYTE channel)
{
    /* Disable SPI channel 0 */
    if(channel==SPI_Channel_0)
    {
        GCR1 &= ~(SPICTRL_0+SSSPIBP);
        SSCE0 =INPUT;

        /* If SPI cnannel 1 is disabled or no used. */
        if(IS_MASK_CLEAR(GCR1, SPICTRL_1))
        {
            SSCK = INPUT;
            SMOSI = INPUT;
            SMISO = INPUT;
        }
    }
    /* Disable SPI channel 1 */
    else if(channel==SPI_Channel_1)
    {
        GCR1 &= ~(SPICTRL_1+SSSPIBP);
        SSCE1 =INPUT;

        /* If SPI cnannel 0 is disabled or no used. */
        if(IS_MASK_CLEAR(GCR1, SPICTRL_0))
        {
            SSCK = INPUT;
            SMOSI = INPUT;
            SMISO = INPUT;
        }
    }
    /* Disable SPI channel 0 and 1. */
    else if(channel==SPI_Channel_0N1)
    {
        GCR1 &= ~(SPICTRL_0+SPICTRL_1+SSSPIBP);
        SSCE0 =INPUT;
        SSCE1 =INPUT;
        SSCK = INPUT;
        SMOSI = INPUT;
        SMISO = INPUT;
    }
}

/**
 * ****************************************************************************
 * Example for enabling SPI interface channel 0 function.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Enable_SPIInterface(void)
{
    Init_SPIInterface(SPI_Channel_0, SPI_NoSupportBusy);
}

/**
 * ****************************************************************************
 * To end the SPI transmission
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SPITransmissionEnd(void)
{
    BYTE l_time_out;
    
    /* To end SPI Transmission */
    SPISTS = TRANEND;

    /* To wait "Transfer End Flag" */
    for(l_time_out=0x00; l_time_out<100; l_time_out++)
    {
        if(IS_MASK_SET(SPISTS, TRANENDIF))
        {
            break;
        }
        WNCKR = 0x00; 
    }

    /* W/C Transfer End Flag */
    SPISTS = TRANENDIF;
}

/**
 * ****************************************************************************
 * To write byte of SPI interface. (Blocking mode)
 *
 * @return
 *
 * @parameter
 * spidata,
 * channel, SPI_Channel_0 || SPI_Channel_1.
 *
 * ****************************************************************************
 */
void SPIWriteByte(BYTE spidata, BYTE channel)
{
    SPIDATA = spidata;

    /* Blocking selection. */
    SET_MASK(SPICTRL2, BLKSEL);

    /* Write cycle. */
    CLEAR_MASK(SPICTRL2, CHRW);

    if(channel==SPI_Channel_0)
    {
        SET_MASK(SPISTS, CH0START);
    }
    else if(channel==SPI_Channel_1)
    {
        SET_MASK(SPISTS, CH1START);
    }
    
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    
    SPITransmissionEnd();
}

/**
 * ****************************************************************************
 * To write word of SPI interface. (Blocking mode)
 *
 * @return
 *
 * @parameter
 * MSB, msb of word.
 * LSB, lsb of word.
 * channel, SPI_Channel_0 || SPI_Channel_1.
 *
 * ****************************************************************************
 */
void SPIWriteWord(BYTE MSB, BYTE LSB, BYTE channel)
{
    SPIDATA = MSB;

    /* Blocking selection. */
    SET_MASK(SPICTRL2, BLKSEL);

    /* Write cycle. */
    CLEAR_MASK(SPICTRL2, CHRW);

    if(channel==SPI_Channel_0)
    {
        SET_MASK(SPISTS, CH0START);
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        
        SPIDATA = LSB;
        SET_MASK(SPISTS, CH0START);
    }
    else if(channel==SPI_Channel_1)
    {
        SET_MASK(SPISTS, CH1START);
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        
        SPIDATA = LSB;
        SET_MASK(SPISTS, CH1START);
    }
    
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    SPITransmissionEnd();
}

/**
 * ****************************************************************************
 * To read byte of SPI interface. (Blocking mode)
 *
 * @return
 * l_spi_byte, SPI data.
 *
 * @parameter
 * channel, SPI_Channel_0 || SPI_Channel_1.
 *
 * ****************************************************************************
 */
BYTE SPIReadByte(BYTE channel)
{
    BYTE l_spi_byte;
    
    l_spi_byte = 0x00;

    /* Blocking selection. */
    SET_MASK(SPICTRL2, BLKSEL);

    /* Read cycle. */
    SET_MASK(SPICTRL2, CHRW);

    if(channel==SPI_Channel_0)
    {
        SET_MASK(SPISTS, CH0START);
    }
    else if(channel==SPI_Channel_1)
    {
        SET_MASK(SPISTS, CH1START);
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();
    
    l_spi_byte = SPIDATA;

    SPITransmissionEnd();
    
    return(l_spi_byte);
}

/**
 * ****************************************************************************
 * To read word of SPI interface. (Blocking mode)
 *
 * @return
 * l_spi_word, word SPI data.
 *
 * @parameter
 * channel, SPI_Channel_0 || SPI_Channel_1.
 *
 * ****************************************************************************
 */
WORD SPIReadWord(BYTE channel)
{
    WORD l_spi_word;
    
    l_spi_word = 0x00;

    /* Blocking selection. */
    SET_MASK(SPICTRL2, BLKSEL);

    /* Read cycle. */
    SET_MASK(SPICTRL2, CHRW);

    if(channel==SPI_Channel_0)
    {
        SET_MASK(SPISTS, CH0START);
    }
    else if(channel==SPI_Channel_1)
    {
        SET_MASK(SPISTS, CH1START);
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();

    /* read LSB */
    l_spi_word += SPIDATA;

    if(channel==SPI_Channel_0)
    {
        SET_MASK(SPISTS, CH0START);
    }
    else if(channel==SPI_Channel_1)
    {
        SET_MASK(SPISTS, CH1START);
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();

    /* read MSB */
    l_spi_word += (SPIDATA*256);

    SPITransmissionEnd();

    return(l_spi_word);
}


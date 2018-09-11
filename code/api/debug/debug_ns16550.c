//***************************************************************************************
// debug_ns16550.c
// 
// Implementation Uart interrupt 
//
// Copyright (c) ITE, All Rights Reserved
//
// Created on: 2014/06/11
//***************************************************************************************
#include "debug_ns16550.h"
#include "chip_type.h"
#include "chip_chipregs.h"

#ifdef  __ENABLE_DBG_MSG__  // {
//***************************************************************************************
//
//  Structure define
//
//***************************************************************************************
struct NS16550 {
	unsigned char rbrthrdlb;	/* 0 */
	unsigned char ierdmb;		/* 1 */
	unsigned char iirfcrafr;	/* 2 */
	unsigned char lcr;		    /* 3 */
	unsigned char mcr;		    /* 4 */
	unsigned char lsr;		    /* 5 */
	unsigned char msr;		    /* 6 */
	unsigned char scr;		    /* 7 */
	unsigned char brsr;         /* 8 */
	unsigned char reserved[1];	/* 9 */
	unsigned char dsr;		    /* 10 */
	unsigned char dcr;		    /* 11 */
};
typedef volatile struct NS16550* NS16550_t;

typedef struct UartStruct
{
    NS16550_t mUartBase;
    uchar_8   *rx;
    uchar_8   *tx;
    uchar_8   *rts;
    uchar_8   *cts;
    uchar_8   u8GCR;
}sUart;

//***************************************************************************************
//
// Register define
//
//***************************************************************************************
// Baud rate defination,
#define BAUDRATE_4800           24  //4800
#define BAUDRATE_7200           16  //4800
#define BAUDRATE_9600           12  //9600
#define BAUDRATE_19200          6   //19200
#define BAUDRATE_38400          3   //38400
#define BAUDRATE_57600          2   //57600
#define BAUDRATE_115200         1   //115200

#define rbr rbrthrdlb
#define thr rbrthrdlb
#define dll rbrthrdlb
#define ier ierdmb
#define dlm ierdmb
#define iir iirfcrafr
#define fcr iirfcrafr
#define afr iirfcrafr

#define IER_EMSI        0x08	/* Enable Modem Status Interrupt */
#define IER_ERLSI       0x04	/* Enable Recv Line Status Interrupt */
#define IER_ETHREI      0x02	/* Enable Transmitter Holding Register Empty Interrupt */
#define IER_ERDVI       0x01    /* Enable Recv Data Available Interrupt */

#define FCR_FIFO_EN     0x01	/*fifo enable */
#define FCR_RXSR        0x02	/*reciever soft reset */
#define FCR_TXSR        0x04	/*transmitter soft reset */
#define FCR_DMS		    0x08	/* DMA Mode Select */
#define FCR_FIFO_14     0xC0    /* 14 Byte FIFO Trigger*/
#define FCR_FIFO_8      0x80    /* 8 Byte FIFO Trigger*/
#define FCR_FIFO_4      0x40    /* 4 Byte FIFO Trigger*/
#define FCR_FIFO_1      0x00    /* 1 Byte FIFO Trigger*/

#define LCR_WLS_MSK 0x03	/* character length slect mask */
#define LCR_WLS_5   0x00	/* 5 bit character length */
#define LCR_WLS_6   0x01	/* 6 bit character length */
#define LCR_WLS_7   0x02	/* 7 bit character length */
#define LCR_WLS_8   0x03	/* 8 bit character length */
#define LCR_STB     0x04	/* Number of stop Bits, off = 1, on = 1.5 or 2) */
#define LCR_PEN     0x08	/* Parity eneble */
#define LCR_EPS     0x10	/* Even Parity Select */
#define LCR_STKP    0x20	/* Stick Parity */
#define LCR_SBRK    0x40	/* Set Break */
#define LCR_BKSE    0x80	/* Bank select enable - aka DLAB on 8245 */

#define LSR_DR      0x01	/* Data ready */
#define LSR_OE      0x02	/* Overrun */
#define LSR_PE      0x04	/* Parity error */
#define LSR_FE      0x08	/* Framing error */
#define LSR_BI      0x10	/* Break */
#define LSR_THRE    0x20	/* Xmit holding register empty */
#define LSR_TEMT    0x40	/* Xmitter empty */
#define LSR_ERR     0x80	/* Error */

#define MCR_RTS     0x02    /* Readyu to Send */
#define MCR_LOOP	0x10    /* Local loopback mode enable */
//#define MCR_DTR     0x01    /* noton 8245 duart */
//#define MCR_DMA_EN  0x04    /* noton 8245 duart */
//#define MCR_TX_DFR  0x08    /* noton 8245 duart */
#define MCR_OUT2    0x08    /* enable interrupt */
#define MCR_OUT1    0x04
#define MCR_DTR     0x01
#define MSR_CTS     0x10

/* useful defaults for LCR*/
#define LCR_8N1     0x03
#define CTS_OK_VAL  0x01

//***************************************************************************************
//
//
//
//***************************************************************************************
enum _E_UART_FUN_NO_
{
    eUART_1,
    eUART_2,
    eUART_MAX,  /* leave as last */
};

//***************************************************************************************
//
//
//
//***************************************************************************************
const sUart COM_PORTS[eUART_MAX] =
{
    { ((NS16550_t)REG_UART1_BASE), &GPCRB0, &GPCRB1, &GPCRF3, &GPCRD5, 0x03 },  // UART1
    { ((NS16550_t)REG_UART2_BASE), &GPCRH1, &GPCRH2, &GPCRE5, &GPCRI7, 0x0C },  // UART2
};

/* **************************************************************************************
 * FUNCTION NAME: NS16550_init
 * DESCRIPTION:
 * 		ns16550 init
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 		Copyright(c) 2014 ITE Tech. Inc. All Rights Reserved.
 * 		Jun 11, 2014,  written.
 * *************************************************************************************/
static void NS16550_init (int chan, int baudrate, BOOL bINTMode)
{
	NS16550_t com_port;
    BYTE      param[2];

    // Init Multi-function Pin
    *COM_PORTS[chan].rx  = 0x00;
    *COM_PORTS[chan].tx  = 0x00;
    *COM_PORTS[chan].rts = 0x00;
    *COM_PORTS[chan].cts = 0x00;

    // Enable EC Side control
    #ifdef  __DISABLE_UART_AUTO_CLK_GATING__ // {
    AUTOCG  = 0x30;
    #endif  // } __DISABLE_UART_AUTO_CLK_GATING__
    //EC Side control
    RSTDMMC = 0x1F;
    RSTC4   = 0x02;
    GCR1 |= COM_PORTS[chan].u8GCR;
    GCR6 |= COM_PORTS[chan].u8GCR;
    GCR8 |= COM_PORTS[chan].u8GCR;

    //------------------------------------------------------------
    // Init UART Module
    //------------------------------------------------------------
	com_port = COM_PORTS[chan].mUartBase;
    
    if(baudrate <= 115200){
        param[0] = 115200/baudrate;
        param[1] = 0;
        com_port->brsr = 0x00;
    } else {
       	param[0] = 460800/baudrate;
    	param[1] = 0x80;
        com_port->brsr = 0x02; // EC high speed select
    }

	com_port->lcr = LCR_BKSE;	/* Access baud rate */
    com_port->dll = param[0];
	com_port->dlm = param[1];
	com_port->lcr = LCR_8N1;	/* 8 data, 1 stop, no parity */
	com_port->fcr = FCR_FIFO_EN | FCR_RXSR | FCR_TXSR | FCR_FIFO_8;	/* Clear & enable FIFOs */
	//com_port->brsr = 1; //by HW

    //------------------------------------------------------------
    // Enable Recv Data Available Interrupt
    // Enable Interrupt Controller and WC Status
    //------------------------------------------------------------
    if(TRUE == bINTMode) {
        com_port->ier = IER_ERDVI | IER_ERLSI;
        com_port->mcr = MCR_OUT2;   /* RTS/DTR */
        ISR4  = BIT(6+chan);
        IER4 |= BIT(6+chan);
    } else {
	com_port->ier =  0;
        com_port->mcr = 0;          /* RTS/DTR */
    }
    
}
    
/* **************************************************************************************
 * FUNCTION NAME: NS16550_putc
 * DESCRIPTION:
 * 		put char to Tx
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 		Copyright(c) 2014 ITE Tech. Inc. All Rights Reserved.
 * 		Jun 11, 2014,  written.
 * *************************************************************************************/
static void NS16550_putc(int chan, unsigned char c)
{
	while ((COM_PORTS[chan].mUartBase->lsr & LSR_THRE) == 0);
    
	COM_PORTS[chan].mUartBase->thr = c;
}

// ****************************************************************************************
// Function name: debug_putc( void)
// 
// Description: 
//   debug_putc Function. 
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note: 
//   
// ****************************************************************************************
void debug_putc(unsigned char c)
{
    NS16550_putc(eUART_1, c);
}

/* **************************************************************************************
 * FUNCTION NAME: Uart_Init
 * DESCRIPTION:
 * 		Initial Uart Module
 * 
 * ARGUMENTS: N/A
 * 
 * RETURNS: N/A
 * 
 * NOTE: 
 * 		Copyright(c) 2014 ITE Tech. Inc. All Rights Reserved.
 * 		Jun 11, 2014,  written.
 * *************************************************************************************/
void Uart_Init( void)
{
    NS16550_init(eUART_1, 57600, FALSE);
}
// ****************************************************************************************
#endif  // } __ENABLE_DBG_MSG__
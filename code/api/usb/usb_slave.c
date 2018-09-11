//******************************************************************************
//
//  usb_slave.c
//
//  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
//
//  Created on: 2012/11/13
//
//******************************************************************************
#include <stdlib.h>
#include <string.h>
#include "config.h"

#include "usb.h"
#include "usb_reg.h"
#include "debug_print.h"
#include "usb_slave.h"
#include "usb_slave_const.h"
#include "usb_slave_hid.h"
#include "chip_chipregs.h"
#include "Core_timer.h"
#include "hal.h"

// *****************************************************************************
//  Macro Definitions
// *****************************************************************************
#define GET_EP_CON_REG(Index) (ENDPOINT0_CONTROL_REG+0x04*Index)
//#define __CMD_DEBUG__
//#define __USB_SLAVE_DMA_MODE__
#define CHIPVER_CX 2
//#define USB_ON_FPGA
//#define __USB_SLAVE_SHUTDOWN_PAD__

#define USB_IS_AUTOREADY() 1

// *****************************************************************************
//
// *****************************************************************************
#define USB_DBG_MSG_L(fmt, arg...)              //rtl_printf(fmt, ##arg)
#define USB_DBG_MSG_M(fmt, arg...)              //rtl_printf(fmt, ##arg)
#define USB_DBG_MSG_H(fmt, arg...)              rtl_printf(fmt, ##arg)
#define USB_DBG_ERROR(fmt, arg...)              rtl_printf(fmt, ##arg)

// *****************************************************************************
//
// *****************************************************************************
//#define __CMD_ISR_DEBUG__

#ifdef __CMD_ISR_DEBUG__ // {
#define USB_DBG_MAIN_ENTER() GPDRA |= BIT0
#define USB_DBG_MAIN_LEAVE() GPDRA &= ~BIT0
#define USB_DBG_ISR_ENTER() GPDRA |= BIT1
#define USB_DBG_ISR_LEAVE() GPDRA &= ~BIT1
#define USB_DBG_SUPEND_ENTER() GPDRA |= BIT2
#define USB_DBG_SUPEND_LEAVE() GPDRA &= ~BIT2
#define USB_DBG_RESUME_ENTER() GPDRA |= BIT3
#define USB_DBG_RESUME_LEAVE() GPDRA &= ~BIT3
#define USB_DBG_RESET_ENTER() GPDRA |= BIT4
#define USB_DBG_RESET_LEAVE() GPDRA &= ~BIT4
#else
#define USB_DBG_MAIN_ENTER() 
#define USB_DBG_MAIN_LEAVE() 
#define USB_DBG_ISR_ENTER() 
#define USB_DBG_ISR_LEAVE() 
#define USB_DBG_SUPEND_ENTER()
#define USB_DBG_SUPEND_LEAVE()
#define USB_DBG_RESUME_ENTER()
#define USB_DBG_RESUME_LEAVE()
#define USB_DBG_RESET_ENTER()
#define USB_DBG_RESET_LEAVE()


#endif // } __CMD_ISR_DEBUG__

#define USB_DBG_LAST_CMD() printf(" %X %X %X %X\r\n",gpSHDInfo->pUSBDbgMsg[4],gpSHDInfo->pUSBDbgMsg[5],gpSHDInfo->pUSBDbgMsg[6],gpSHDInfo->pUSBDbgMsg[7])
//use BIT 0,1,4,5,6 for 8350 debug
#define USB_DBG_CODE_ENTER_MAIN   0x01
#define USB_DBG_CODE_LEAVE_MAIN   0x32
#define USB_DBG_CODE_LEAVE_MAIN_SUSPEND   0x33
#define USB_DBG_CODE_CHECK_RESET   0x10
#define USB_DBG_CODE_CHECK_RESET_OK   0x11

#define USB_DBG_CODE_DATA_STAGE_IN_START   0x20
#define USB_DBG_CODE_DATA_STAGE_IN_END     0x21
#define USB_DBG_CODE_DATA_STAGE_OUT_START   0x22
#define USB_DBG_CODE_DATA_STAGE_OUT_END     0x23

#define USB_DBG_CODE_STATUS_STAGE_IN_START   0x30
#define USB_DBG_CODE_STATUS_STAGE_IN_END   0x31
#define USB_DBG_CODE_STATUS_STAGE_OUT_START   0x12
#define USB_DBG_CODE_STATUS_STAGE_OUT_END   0x13

#define USB_DBG_CODE_ISR_RESET   0x40
#define USB_DBG_CODE_ISR_STATUS_OUT_RETURN   0x41
#define USB_DBG_CODE_ISR_STATUS_IN_RETURN   0x42
#define USB_DBG_CODE_ISR_NEXT_NOT_SETUP   0x50
#define USB_DBG_CODE_ISR_DONE   0x51


#define USB_DBG_ERROR_BUF_BIT   BIT0
#define USB_DBG_ERROR_STATUS_IN_BIT   BIT1
#define USB_DBG_ERROR_STATUS_OUT_BIT  BIT2
#define USB_DBG_ERROR_RX_BIT  BIT3 // this is normal when status stage is canceled
#define USB_DBG_ERROR_TX_BIT  BIT4  // this is normal when status stage is canceled
#define USB_DBG_ERROR_ISR_STATUS_IN_BIT_BIT  BIT5
#define USB_DBG_ERROR_ISR_STATUS_OUT_BIT_BIT  BIT6
#define USB_DBG_ERROR_SUSPEND_BIT  BIT7


#define USB_DBG_HELPER_REPLUG  0x64
#define USB_DBG_HELPER_RESETSENSOR  0x63

#define USB_DBG_ALL_ON
#ifdef __CMD_DEBUG__
#define USB_DBG_CMD(a) DEBUG_PORT(a)
#else
#define USB_DBG_CMD(a) (gpSHDInfo->pUSBDbgMsg[0] = (a))
#endif
#ifdef USB_DBG_ALL_ON
#define USB_DBG_LAST_TRAN(a)                     (gpSHDInfo->pUSBDbgMsg[1] = ((gpSHDInfo->pUSBDbgMsg[1]&0xFC)|(a))) // remain Bit7:2
#define USB_DBG_LAST_TRAN_ISR(a)                 (gpSHDInfo->pUSBDbgMsg[1] = ((gpSHDInfo->pUSBDbgMsg[1]&0xF3)|(a << 2))) // remain Bit7:4, BIT1:0

#define USB_DBG_GET_REPORT_SET()                 (gpSHDInfo->pUSBDbgMsg[1] |= BIT4) 
#define USB_DBG_GET_REPORT_CLEAR()               (gpSHDInfo->pUSBDbgMsg[1] &= ~BIT4) 
#define USB_DBG_SET_REPORT_SET()                 (gpSHDInfo->pUSBDbgMsg[1] |= BIT5) 
#define USB_DBG_SET_REPORT_CLEAR()               (gpSHDInfo->pUSBDbgMsg[1] &= ~BIT5) 


#define USB_DBG_ERROR_SET(a)              (gpSHDInfo->pUSBDbgMsg[2] |= (a))
#define USB_DBG_ERROR_CLEAR(a)              (gpSHDInfo->pUSBDbgMsg[2] &= ~(a))
#define USB_DBG_HELPER()                     (gpSHDInfo->pUSBDbgMsg[3])
#define USB_DBG_STAGE(a)                     (gpSHDInfo->pUSBDbgMsg[3] = ((gpSHDInfo->pUSBDbgMsg[3]&0xF8))|a)

#define USB_DBG_SET_LAST_CMD(pbuf)               (memcpy((void *)&(gpSHDInfo->pUSBDbgMsg[4]),(void *)pbuf,4))

#else
#define USB_DBG_LAST_TRAN(a)                     
#define USB_DBG_ERROR_SET(a)              //(gpSHDInfo->pUSBDbgMsg[2] |= (a))
#define USB_DBG_ERROR_CLEAR(a)              //(gpSHDInfo->pUSBDbgMsg[2] &= ~(a))
#define USB_DBG_HELPER()                     //(gpSHDInfo->pUSBDbgMsg[3])
#define USB_DBG_STAGE(a)                     
#endif

#define REPORT_IN  0
#define REPORT_OUT 0
#define FS_C1_HID_REPORT_DATA           1
#define FS_C1_HID_REPORT_DATA1          64

#ifdef __ENABLE_USB_INTERRUPT__
#define USB_SUSPEND_ALL(); //hal_suspend_all_thread();
#define USB_RESUME_ALL(); //hal_resume_all_thread();
#else
#define USB_SUSPEND_ALL(); hal_suspend_all_thread();
#define USB_RESUME_ALL(); hal_resume_all_thread();

#endif


#define USB_SUSPEND_COUNTER 4



// *****************************************************************************
// Enum Definitions
// *****************************************************************************
typedef enum 
{
	USB_TRANSACTION_SETUP = 0,
	USB_TRANSACTION_IN,
	USB_TRANSACTION_OUT,
} USB_TRANSACTION_TYPE;

typedef enum 
{
	USB_ACT_IDLE = 0,
	USB_ACT_DONE,
	USB_ACT_STALL
} USBDeviceAction;


typedef enum 
{
	USBEP_CON_REG = 0,
	USBEP_STATUS_REG,
	USBEP_TX_FIFO_CON_REG,
	USBEP_TX_FIFO_DATA_REG,
	USBEP_MAX_REG,
} USBEPReg;

typedef enum 
{
	USB_TRANSFER_STAGE_SETUP = 0,
	USB_TRANSFER_STAGE_DATA_IN,
    USB_TRANSFER_STAGE_DATA_OUT,
	USB_TRANSFER_STAGE_STATUS_OUT,
	USB_TRANSFER_STAGE_STATUS_IN,
} USB_TRANSFER_STAGE;


// *****************************************************************************
// Struct Definitions
// *****************************************************************************
typedef struct _Setup_Data_packet
{
	UINT8 bRequestType;
	UINT8 bRequest;
	UINT8 wValue[2];
	UINT8 wIndex[2];
	UINT8 wLength[2];
} SETUP_DATA_PACKET;


typedef struct _Usb_Control_Data
{
    UINT8 u8Addr;
    UINT8 u8TransferStage;    
    BOOL bEPHalt;
    BOOL bIsReset;
    UINT8 Slave_FinishAction;    
    UINT8 u8LastTransaction;  
    UINT8 *pu8DataBuffer; 
    UINT16 u16TotalDataNum; 
    UINT8 u8LastDataNum; 
    UINT8 u8EP0DataSeg; 
    UINT8 u8EP1DataSeg; 
    UINT8 u8Config;
    UINT8 u8SuspendCounter;
    UINT8 u8SuspendCountingFlag;

    UINT8 u8RemoteWakeup;

} USB_CONTROL_DATA;


// *****************************************************************************
// Global Variables
// *****************************************************************************

#ifdef __CMD_DEBUG__ // {
USB_DATA UINT8 gu8LastClass = 0;
USB_DATA UINT16 gu16WaitCount = 0;
#endif //} __CMD_DEBUG__

#if 0
USB_DATA UINT8 u8USBDataBuf[FS_C1_HID_REPORT_DATA][FS_C1_HID_REPORT_DATA1] =
{
    {0x00}
};
#else
typedef UINT8 (*USB_BUFFER)[FS_C1_HID_REPORT_DATA1];

#define u8USBDataBuf ((USB_BUFFER)gpHIDReportBuffer)
#endif

USB_DATA volatile BOOL gbUSBNoSuspend = FALSE;
USB_DATA static volatile UINT8 gu8USBResumeFlag = 0;
USB_DATA static volatile BOOL gbUSBResumeSet = FALSE;
#ifndef __USB_SLAVE_SHUTDOWN_PAD__
USB_DATA volatile BOOL gbUSBSuspended = FALSE;
#endif


USB_BSS USB_CONTROL_DATA gtUSBControl;
USB_BSS USB_CONTROL_DATA *gptUSBControl;
USB_BSS SETUP_DATA_PACKET *gptPacket;
USB_BSS SETUP_DATA_PACKET gtSetupDataPacket;

// *****************************************************************************
// USB Slave function delclare
// *****************************************************************************

BOOL usb_slave_send_data_for_interrupt(UINT8 *pu8Buffer, UINT16 u16Num, UINT8 u8DataSeq);
BOOL usb_slave_check_reset();
void usb_slave_check_in_loop();
void usb_slave_sw_init(BOOL bResume);

#ifdef __CMD_DEBUG__ // {
void USB_CODE_L usb_debug_printf()
{
    UINT8 uCount= 0;
    printf("ep %X\r\n",REG_READ_8BIT(ENDPOINT0_CONTROL_REG));
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG, 0);
    printf("T %X n T %X , st %X", REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_STATUS_REG));

    USB_DBG_LAST_CMD();
    printf("MSB %X LSB %X \r\n", REG_READ_8BIT(EP0_RX_FIFO_DATA_COUNT_MSB),REG_READ_8BIT(EP0_RX_FIFO_DATA_COUNT_LSB));

    for(uCount = 0; uCount < 64; uCount++)
    {
        printf("%X ",REG_READ_8BIT(EP0_RX_FIFO_DATA));
        if(uCount !=0 && uCount%8 ==0)
            printf("\r\n");
    }
}
#endif // } __CMD_DEBUG__

// *****************************************************************************
// USB Slave functions
// *****************************************************************************
#ifdef __USB_SLAVE_DMA_MODE__ // {
//****************************************************************************************
// Function name: usb_slave_dma_start()
//
// Description:
//   usb_slave_dma_start.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_dma_start(UINT8 endpoint, BOOL bIsRX, UINT32 ulAdd, UINT8 u8Count)
{
    UINT8 u8DMACtrl = (bIsRX)?USB_DMA_CTRL_DIRECTION:0;// DMA Write:  Data from USB RX FIFO to internal SRAM.
    
    if(ulAdd > GET_DATA_VMA_ADDR)
    {
    	ulAdd -= GET_DATA_VMA_ADDR;
    	ulAdd += 0xD000;
    }

        
    REG_WRITE_8BIT(USB_DMA_CTR_REG, u8DMACtrl);//Set DMA ep point and dir
    REG_WRITE_8BIT(USB_DMA_TX_COUNT_REG, u8Count);          //Set Count
    
    REG_WRITE_8BIT(USB_DMA_ADDRESS_7_0_REG, (UINT8)(ulAdd & 0x000000FF)); //Set Add 
    REG_WRITE_8BIT(USB_DMA_ADDRESS_15_8_REG, (UINT8)((ulAdd >> 8)& 0x000000FF)); 
    REG_WRITE_8BIT(USB_DMA_ADDRESS_23_16_REG, (UINT8)((ulAdd >> 16)& 0x000000FF)); 
    REG_WRITE_8BIT(USB_DMA_ADDRESS_31_24_REG, (UINT8)((ulAdd >> 24)& 0x000000FF));  
    u8DMACtrl |= ((endpoint<<USB_DMA_CTRL_EP_NUM_SHIFT)&USB_DMA_CTRL_EP_NUM_MASK);
    REG_WRITE_8BIT(USB_DMA_CTR_REG, u8DMACtrl|USB_DMA_CTRL_ENALBE); // DMA Set
    REG_WRITE_8BIT(USB_DMA_CTR_REG, u8DMACtrl|USB_DMA_CTRL_ENALBE|USB_DMA_CTRL_SET); // DMA Set


}
//****************************************************************************************
// Function name: usb_slave_dma_wait()
//
// Description:
//   usb_slave_dma_wait.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_dma_wait(UINT8 u8EndPointNo)
{
    UINT8 u8DMADoneBit;
    UINT8 u8DMAErrorBit;

    switch(u8EndPointNo)
    {
        case 1:
            u8DMADoneBit = USB_DMA_INT_EP1_DONE;
            u8DMAErrorBit= USB_DMA_INT_EP1_ERROR;
            break;
        case 0:
        default:
            u8DMADoneBit = USB_DMA_INT_EP0_HOST_DONE;
            u8DMAErrorBit= USB_DMA_INT_EP0_HOST_ERROR;
            break;
    }
    while(0 == (REG_READ_8BIT(USB_DMA_INTERRUPT_STAT_REG)&u8DMADoneBit ))//(0 !=(REG_READ_8BIT(USB_DMA_CTR_RB_REG)&(USB_DMA_CTRL_ENALBE)))
    {
        if(usb_slave_check_reset())
            return FALSE;
    }
    REG_WRITE_8BIT(USB_DMA_INTERRUPT_STAT_REG,u8DMADoneBit);    


   return TRUE;
}
#else
#define usb_slave_dma_wait(n)
#endif // } __USB_SLAVE_DMA_MODE__
//****************************************************************************************
// Function name: usb_slave_read_fifo()
//
// Description:
//   usb_slave_read_fifo.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT8 USB_CODE usb_slave_read_fifo(
    UINT8 *pDataPacket, UINT8 u8Size, UINT8 u8EndPointNo, BOOL u8DMA)
{
    UINT32 u32RXDataRegAddr, u32EMSBRegAddr, u32ELSBRegAddr, u32CtrlRegAddr;
    UINT8 u8Index = 0;
    
    switch(u8EndPointNo)
    {
        case 1:
            u32RXDataRegAddr = EP1_RX_FIFO_DATA;
            u32EMSBRegAddr = EP1_RX_FIFO_DATA_COUNT_MSB;
            u32ELSBRegAddr = EP1_RX_FIFO_DATA_COUNT_LSB;
            u32CtrlRegAddr = EP1_RX_FIFO_CONTROL_REG;
            break;
        case 0:
        default:
            u32RXDataRegAddr = EP0_RX_FIFO_DATA;
            u32EMSBRegAddr = EP0_RX_FIFO_DATA_COUNT_MSB;
            u32ELSBRegAddr = EP0_RX_FIFO_DATA_COUNT_LSB;
            u32CtrlRegAddr = EP0_RX_FIFO_CONTROL_REG;
            break;
    }    

    #ifdef __USB_SLAVE_DMA_MODE__ // {    
    if(u8DMA)
    {
        return TRUE;
    }
    #endif // }__USB_SLAVE_DMA_MODE__   
    
    while((u8Size--) >0)
    {
        if(!((REG_READ_8BIT(u32EMSBRegAddr)==0)&&(REG_READ_8BIT(u32ELSBRegAddr)==0)))
        {
            *(pDataPacket+u8Index)= REG_READ_8BIT(u32RXDataRegAddr);
            //SC_DBG("read fifo data =  %X \r\n", *(pDataPacket+u8Index));
            u8Index++;    
        }
        else
        {
            //SC_ERR_DBG("read error %X size %X,st %X las %X %X\r\n", u8Index,u8Size ,REG_READ_8BIT(ENDPOINT0_STATUS_REG),gptUSBControl->u8LastTransaction, gptUSBControl->u8TransferStage);

            //SC_ERR_DBG("epcon %X t %X %X\r\n", REG_READ_8BIT(ENDPOINT0_CONTROL_REG),REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));
            return u8Index;
        }
    }
    
    return u8Index;
}
//****************************************************************************************
// Function name: usb_slave_write_fifo()
//
// Description:
//   usb_slave_write_fifo.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_write_fifo(
    UINT8 *pu8Buffer, UINT8 u8Size, UINT8 u8EndPointNo, BOOL u8DMA)
{
    UINT32 u32FIFOCtlRegAddr,u32FIFODataRegAddr;
    UINT8 u8Index = 0;  


    #ifdef __USB_SLAVE_DMA_MODE__ // {    
    if(u8DMA)
    {
        usb_slave_dma_start(u8EndPointNo,FALSE, (UINT32)pu8Buffer, u8Size);  
        return;
    }
    #endif // }__USB_SLAVE_DMA_MODE__                



    switch(u8EndPointNo)
    {
        case 1:
            u32FIFOCtlRegAddr = EP1_TX_FIFO_CONTROL_REG;
            u32FIFODataRegAddr = EP1_TX_FIFO_DATA;
            break;
        case 0:
        default:
            u32FIFOCtlRegAddr = EP0_TX_FIFO_CONTROL_REG;
            u32FIFODataRegAddr = EP0_TX_FIFO_DATA;
            break;
    }
    REG_WRITE_8BIT(u32FIFOCtlRegAddr, BIT0);  //TX_FIFO
    
        
    for(u8Index = 0; u8Index < u8Size; u8Index++)
        REG_WRITE_8BIT(u32FIFODataRegAddr, pu8Buffer[u8Index]); //TX_FIFO    
}
//****************************************************************************************
// Function name: usb_slave_set_ep0_ready()
//
// Description:
//  usb_slave_set_ep0_ready.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_set_ep0_ready()
{   
    #ifdef __USB_SLAVE_DMA_MODE__ // {
    usb_slave_dma_start(0, TRUE, (UINT32)u8USBDataBuf[REPORT_IN], 0);//dma??
    #endif // }__USB_SLAVE_DMA_MODE__

    
    if(USB_IS_AUTOREADY())
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,ENDPOINT_ENABLE_BIT);
    else
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT);
}

//****************************************************************************************
// Function name: usb_slave_is_ep_stall()
//
// Description:
//   usb_slave_is_ep_stall.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_is_ep_stall(UINT8 endpoint, BOOL bDir)
{    
    return (0 != (REG_READ_8BIT(GET_EP_CON_REG(endpoint)) & ENDPOINT_SEND_STALL_BIT));
}
//****************************************************************************************
// Function name: usb_slave_set_ep_stall()
//
// Description:
//   usb_slave_set_ep_stall.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_set_ep_stall(UINT8 endpoint, BOOL bDir, BOOL bStall)
{
    UINT8 u8Con = REG_READ_8BIT(GET_EP_CON_REG(endpoint));
    if(bStall)
        REG_WRITE_8BIT(GET_EP_CON_REG(endpoint), u8Con|ENDPOINT_SEND_STALL_BIT);
    else        
        REG_WRITE_8BIT(GET_EP_CON_REG(endpoint), u8Con&(~ENDPOINT_SEND_STALL_BIT));
}
//****************************************************************************************
// Function name: usb_slave_reset()
//
// Description:
//   usb_slave_reset.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE_L usb_slave_reset()
{   
    if(!gptUSBControl->bIsReset)
    {
        USB_DBG_RESET_ENTER(); 
        usb_slave_sw_init(FALSE);
        SC_DBG("[Reset]\r\n");
        REG_WRITE_8BIT(EP0_RX_FIFO_CONTROL_REG, BIT0);
        REG_WRITE_8BIT(EP0_TX_FIFO_CONTROL_REG, BIT0);
        REG_WRITE_8BIT(EP1_RX_FIFO_CONTROL_REG, BIT0);
        REG_WRITE_8BIT(EP1_TX_FIFO_CONTROL_REG, BIT0);
        usb_slave_set_ep0_ready();
        REG_WRITE_8BIT(ENDPOINT1_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT|ENDPOINT_DIRECTION_BIT);
        REG_WRITE_8BIT(SC_ADDRESS,0x00);
        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG,SC_NAK_SENT_INT_BIT|SC_SOF_RECEIVED_BIT);
        #ifdef __CMD_DEBUG__
        printk("[Re]\r\n");
        #endif
        gptUSBControl->bIsReset = TRUE;
        USB_DBG_RESET_LEAVE();
        return TRUE;
    }
    return FALSE;
}

//****************************************************************************************
// Function name: usb_slave_check_interrupt_data()
//
// Description:
//   usb_slave_check_interrupt_data.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_check_interrupt_data()
{
    static UINT8 u8LastSeg = 1;
    UINT8 u8Len;
    if(0 == (REG_READ_8BIT(ENDPOINT1_CONTROL_REG) & ENDPOINT_READY_BIT)/*&& 0 != (REG_READ_8BIT(ENDPOINT1_STATUS_REG)&SC_NAK_SENT_BIT)*/)
    {
        u8Len = usb_slave_hid_cmd_dispatch(0, 0, u8USBDataBuf[REPORT_IN], NULL, 0);
        if(0 != u8Len)
        {
            usb_slave_send_data_for_interrupt(u8USBDataBuf[REPORT_IN], u8Len,u8LastSeg);
            u8LastSeg = (u8LastSeg == 1)? 0: 1;
        }
    }
}
//****************************************************************************************
// Function name: usb_slave_check_reset()
//
// Description:
//   usb_slave_check_reset.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE_L usb_slave_check_reset()
{
    BOOL bTmp = FALSE;
    if(0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_RESET_EVENT_BIT))
    {
        bTmp = usb_slave_reset();        
        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG,SC_RESET_EVENT_BIT);
    }
    return bTmp;
}
//****************************************************************************************
// Function name: usb_slave_check_in_loop()
//
// Description:
//   usb_slave_check_in_loop.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_check_in_loop()
{
    USB_DBG_CMD(USB_DBG_CODE_CHECK_RESET);      
    usb_slave_check_reset();    
    USB_DBG_CMD(USB_DBG_CODE_CHECK_RESET_OK);
}
//****************************************************************************************
// Function name: usb_slave_check_transaction_type()
//
// Description:
//   usb_slave_check_transaction_type.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_check_transaction_type(UINT8 u8TransType, BOOL *bIsSetup)
{        
    UINT8 u8Type = REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG);

    if(NULL != bIsSetup)
        *bIsSetup = FALSE;    
    if(u8TransType == u8Type)
        return TRUE; 

    if(USB_TRANSACTION_SETUP != u8Type)
    {
        #ifdef __CMD_DEBUG__ // {    
        printf("Wait T %X, lc %X, type %X nak type %X  st %X, GPA %X \r\n",u8TransType,gu8LastClass, u8Type,REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_STATUS_REG),GPDRA);            
        #endif //} __CMD_DEBUG__   
    }
    else
    {
        if(NULL != bIsSetup)
            *bIsSetup = TRUE;
    }

    return FALSE;
}
//****************************************************************************************
// Function name: usb_slave_polling_nak_transaction_type()
//
// Description:
//   usb_slave_polling_transaction_nak_type.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
#if 0
BOOL USB_CODE usb_slave_polling_nak_transaction_type(UINT8 u8TransType)
{        
    UINT8 u8Type;
    
    while(0 == (REG_READ_8BIT(ENDPOINT0_STATUS_REG)&SC_NAK_SENT_BIT))
    {
        if(usb_slave_check_reset())
            return FALSE;
    }
    u8Type = REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG);

    if(u8TransType == u8Type)
        return TRUE;
    //printf("Wait NakT %X, type %X nak type %X stage %X", u8TransType, REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),u8Type, gptUSBControl->u8TransferStage);    
    if(USB_TRANSACTION_SETUP == u8Type)
        {
        printf("ep 0 nak\r\n");
        usb_slave_set_ep0_ready();
        }

    #ifdef __CMD_DEBUG__ // {    
    if(USB_TRANSACTION_SETUP != u8Type)
        printf("Wait NakT %X, lc %X, type %X nak type %X  st %X, GPA %X \r\n",u8TransType,gu8LastClass, REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),u8Type,REG_READ_8BIT(ENDPOINT0_STATUS_REG),GPDRA);            
    #endif //} __CMD_DEBUG__   

    return FALSE;
}
#else
#define usb_slave_polling_nak_transaction_type(u8TransType) TRUE
#endif
//****************************************************************************************
// Function name: usb_slave_send_data_for_interrupt()
//
// Description:
//   usb_slave_send_data_for_interrupt.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_send_data_for_interrupt(UINT8 *pu8Buffer, UINT16 u16Num, UINT8 u8DataSeq)
{  
    UINT8 tmp;

    usb_slave_write_fifo(pu8Buffer, (UINT8)u16Num, 1,TRUE);                    

    //select data sequence and set ready
    tmp = REG_READ_8BIT(ENDPOINT1_CONTROL_REG);    

    if(u8DataSeq)
        REG_WRITE_8BIT(ENDPOINT1_CONTROL_REG,tmp|ENDPOINT_OUTDATA_SEQUENCE_BIT|ENDPOINT_READY_BIT|ENDPOINT_DIRECTION_BIT);
    else
        REG_WRITE_8BIT(ENDPOINT1_CONTROL_REG,(tmp&(~ENDPOINT_OUTDATA_SEQUENCE_BIT))|ENDPOINT_READY_BIT|ENDPOINT_DIRECTION_BIT);
            

    return TRUE;
}
//****************************************************************************************
// Function name: usb_slave_send_data()
//
// Description:
//   usb_slave_send_data.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_send_data(UINT8 *pu8Buffer, UINT8 u8Num, UINT8 u8DataSeq)
{ 
    UINT8 tmp;   
    
    usb_slave_write_fifo(pu8Buffer, u8Num, 0, (u8Num != 0));
    
    tmp = REG_READ_8BIT(ENDPOINT0_CONTROL_REG)|ENDPOINT_ENABLE_BIT;        

    //select data sequence and set ready
    if(u8DataSeq)
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,tmp|ENDPOINT_OUTDATA_SEQUENCE_BIT);
    else
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,(tmp&(~ENDPOINT_OUTDATA_SEQUENCE_BIT)));
    REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,(REG_READ_8BIT(ENDPOINT0_CONTROL_REG)|ENDPOINT_READY_BIT));

    //check Transaction Done bit

    return TRUE;
}
//****************************************************************************************
// Function name: usb_slave_status_stage_in()
//
// Description:
//   usb_slave_status_stage_in.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_status_stage_in()
{
    UINT8 aru8Empty[1] = {0};

    USB_DBG_CMD(USB_DBG_CODE_STATUS_STAGE_IN_START);       
    if(!(usb_slave_polling_nak_transaction_type(USB_TRANSACTION_IN)))
        return FALSE;

    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_STATUS_IN;
    USB_DBG_STAGE(gptUSBControl->u8TransferStage);    

    usb_slave_send_data(aru8Empty, 0, 1);
    
    USB_DBG_CMD(USB_DBG_CODE_STATUS_STAGE_IN_END);
    
    return TRUE;
}
//****************************************************************************************
// Function name: usb_slave_status_stage_out()
//
// Description:
//   usb_slave_status_stage_out.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_status_stage_out()
{    
    USB_DBG_CMD(USB_DBG_CODE_STATUS_STAGE_OUT_START);
    if(!(usb_slave_polling_nak_transaction_type(USB_TRANSACTION_OUT)))
        return FALSE;
    
    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_STATUS_OUT;
    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
    
    REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT);
    
    USB_DBG_CMD(USB_DBG_CODE_STATUS_STAGE_OUT_END);    
    
    return TRUE;
}

//****************************************************************************************
// Function name: usb_slave_data_stage_in()
//
// Description:
//   usb_slave_data_stage_in.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_data_stage_in(UINT8 *pu8Buffer, UINT16 u16Num)
{
    UINT8 u8temp = 0;
    
    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_DATA_IN;
    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
    if(u16Num != 0)
    {
        if(u16Num < EP0MAXPACKETSIZE)
            u8temp = (UINT8)u16Num;
        else
            u8temp = EP0MAXPACKETSIZE;        

        gptUSBControl->u8LastDataNum = u8temp;
        // Transmit u8Temp bytes data
        
        USB_DBG_CMD(USB_DBG_CODE_DATA_STAGE_IN_START);   
        
        if(!(usb_slave_polling_nak_transaction_type(USB_TRANSACTION_IN)))
            return FALSE;        
        
        memcpy(u8USBDataBuf[0], pu8Buffer, u8temp);
        
        if(!usb_slave_send_data(u8USBDataBuf[0], u8temp, gptUSBControl->u8EP0DataSeg))            
            return FALSE;
        
        
        USB_DBG_CMD(USB_DBG_CODE_DATA_STAGE_IN_END);
        
    }
    
    // end of the data stage
    return TRUE;
}
//****************************************************************************************
BOOL USB_CODE usb_slave_recieve_data(UINT8 *pu8Buffer, UINT8 u8Num)
{
    
    if(!(usb_slave_polling_nak_transaction_type(USB_TRANSACTION_OUT)))
        return FALSE;
    usb_slave_set_ep0_ready();

    if(USB_IS_AUTOREADY())
        REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT);

    return TRUE;
}

//****************************************************************************************
// Function name: usb_slave_data_stage_out()
//
// Description:
//   usb_slave_data_stage_out.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_data_stage_out(UINT8 *pu8Buffer, UINT16 u16Num)
{
    gptUSBControl->u8LastDataNum = 0;
    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_DATA_OUT;
    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
    if(u16Num != 0)
    {
        if(u16Num < EP0MAXPACKETSIZE)
            gptUSBControl->u8LastDataNum = (UINT8)u16Num;
        else
            gptUSBControl->u8LastDataNum = EP0MAXPACKETSIZE;        


        USB_DBG_CMD(USB_DBG_CODE_DATA_STAGE_OUT_START);
        if(!usb_slave_recieve_data(pu8Buffer, gptUSBControl->u8LastDataNum))
            return FALSE;

        USB_DBG_CMD(USB_DBG_CODE_DATA_STAGE_OUT_END);       
        
    }
    
    return TRUE;
}

//****************************************************************************************
// Function name: usb_slave_check_rx_status()
//
// Description:
//   usb_slave_check_rx_status.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_check_rx_status()
{
    UINT8 u8Stat;
    u8Stat = REG_READ_8BIT(ENDPOINT0_STATUS_REG);
    if((u8Stat&SC_CRC_ERROR_BIT)!=0)
    {                       
        REG_WRITE_8BIT(EP0_RX_FIFO_CONTROL_REG, BIT0);        
        SC_ERR_DBG("CRC Error %X last %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG), gptUSBControl->u8LastTransaction);
    }            
    else if((u8Stat&SC_BIT_STUFF_ERROR_BIT)!=0)
    {            
        REG_WRITE_8BIT(EP0_RX_FIFO_CONTROL_REG, BIT0);
        SC_ERR_DBG("Bit Stuff Error %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG));
    }     
    else if((u8Stat&SC_RX_OVERFLOW_BIT)!=0)
    { 
        REG_WRITE_8BIT(EP0_RX_FIFO_CONTROL_REG, BIT0);
        SC_ERR_DBG("RX Overflow Error %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG));
    }
    
    else if((u8Stat&SC_RX_TIME_OUT_BIT)!=0)
    { 
        REG_WRITE_8BIT(EP0_RX_FIFO_CONTROL_REG, BIT0);
        SC_ERR_DBG("RX Time out Error %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG));
    }
    else
    {
        return TRUE;
    }
    
    return FALSE;
}
//****************************************************************************************
// Function name: usb_slave_check_tx_status()
//
// Description:
//   usb_slave_check_tx_status.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_check_tx_status()
{
    UINT8 u8Stat;
    u8Stat = REG_READ_8BIT(ENDPOINT0_STATUS_REG);    
        
    if((u8Stat& SC_RX_TIME_OUT_BIT)!=0)
    {       
        SC_ERR_DBG("RX1 time out %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG));
    }
    else
    {
        return TRUE;
    }

    return FALSE;
}

//****************************************************************************************
// Function name: usb_slave_txdata()
//
// Description:
//   usb_slave_txdata.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_txdata(UINT8 *pu8Buffer, UINT16 u16Num, BOOL bBegin)
{
    if(bBegin)
    {
        gptUSBControl->pu8DataBuffer = pu8Buffer;   
        gptUSBControl->u16TotalDataNum = u16Num;    
        gptUSBControl->u8EP0DataSeg = 1;
    }
    else
    {    
        gptUSBControl->u8EP0DataSeg = (gptUSBControl->u8EP0DataSeg == 1) ? 0 : 1;
        if(0 != gptUSBControl->u16TotalDataNum && usb_slave_check_tx_status())
        {
            gptUSBControl->u16TotalDataNum -= (UINT16)(gptUSBControl->u8LastDataNum);
            gptUSBControl->pu8DataBuffer += gptUSBControl->u8LastDataNum;
        }
    }
    
    if(0 != gptUSBControl->u16TotalDataNum)
    {
        if(!(usb_slave_data_stage_in(gptUSBControl->pu8DataBuffer, gptUSBControl->u16TotalDataNum)))
            return ERROR_USB_TX_SEND_FAIL;
    }
    else
    {
        // IN-Transation data size = 0
        if((0 == gptUSBControl->u16TotalDataNum))
        {
            if(gptUSBControl->u8EP0DataSeg)
            {
                REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG, REG_READ_8BIT(ENDPOINT0_CONTROL_REG)|ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT|ENDPOINT_OUTDATA_SEQUENCE_BIT);
            }
            else
            {
                REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,((REG_READ_8BIT(ENDPOINT0_CONTROL_REG)|ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT)&(~ENDPOINT_OUTDATA_SEQUENCE_BIT)));
            }
            return USB_ERROR_SUCCESS;
        }
        
        //command abort
        if(!usb_slave_check_transaction_type(USB_TRANSACTION_IN,NULL))
        {
            gptUSBControl->u8TransferStage = USB_TRANSACTION_SETUP;
            //printf("data in error\r\n");
            return USB_ERROR_SUCCESS;
        }
        if(!(usb_slave_status_stage_out()))
            return ERROR_USB_TX_STATUS_FAIL;
    }
    
    return USB_ERROR_SUCCESS;
}
//****************************************************************************************
// Function name: usb_slave_rxdata()
//
// Description:
//   usb_slave_rxdata.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_rxdata(UINT8 *pu8Buffer, UINT16 u16Num, BOOL bBegin)
{
    if(bBegin)
    {
        gptUSBControl->pu8DataBuffer = pu8Buffer;   
        gptUSBControl->u16TotalDataNum = u16Num;  
    }
    else
    {    
        if(0 != gptUSBControl->u16TotalDataNum && usb_slave_check_rx_status())
        {
            if(pu8Buffer != u8USBDataBuf[0])
            {                
                USB_DBG_ERROR_SET(USB_DBG_ERROR_BUF_BIT);
                return ERROR_USB_RX_READFIFO_FAIL;
            }
                
            if(gptUSBControl->u8LastDataNum != usb_slave_read_fifo(pu8Buffer, gptUSBControl->u8LastDataNum, 0, TRUE))
                return ERROR_USB_RX_READFIFO_FAIL;
            
            gptUSBControl->u16TotalDataNum -= (UINT16)gptUSBControl->u8LastDataNum;
            
        }
    }
    
    if(0 != gptUSBControl->u16TotalDataNum)
    {
        if(!(usb_slave_data_stage_out(pu8Buffer, u16Num)))
            return ERROR_USB_RX_RECEIVE_FAIL;
    }
    else
    {
        USB_DBG_SET_REPORT_SET();
        usb_slave_hid_cmd_dispatch(gptPacket->bRequest, *((UINT16 *)(gptPacket->wValue)), NULL, u8USBDataBuf[REPORT_OUT], u16Num);        

        USB_DBG_SET_REPORT_CLEAR();

        
        //command abort . 
        if(!usb_slave_check_transaction_type(USB_TRANSACTION_OUT,NULL))
        {
            gptUSBControl->u8TransferStage = USB_TRANSACTION_SETUP;
            //printf("data in error\r\n");
            return USB_ERROR_SUCCESS;
        }

        if(!(usb_slave_status_stage_in()))
            return ERROR_USB_RX_STATUS_FAIL;
    }
    
    
    return USB_ERROR_SUCCESS;
}


//****************************************************************************************
// Function name: usb_slave_cmd_set_address()
//
// Description:
//   usb_slave_cmd_set_address.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE_L usb_slave_cmd_set_address(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT16 wValue = *((UINT16 *)(ptSetupDataPacket->wValue));
    gptUSBControl->u8Addr = wValue;
    usb_slave_status_stage_in();
    
    return USB_ERROR_SUCCESS;
}
//****************************************************************************************
// Function name: usb_slave_cmd_get_descriptor()
//
// Description:
//   usb_slave_cmd_get_descriptor.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_get_descriptor(SETUP_DATA_PACKET *ptSetupDataPacket)
{   
    UINT32 result = 0;
    UINT16 wValue = *((UINT16 *)(ptSetupDataPacket->wValue));    
    const UINT8 *pu8DescriptorEX = NULL;
    UINT16 u16TxRxCounter = 0;


    switch((UINT8)(wValue >> 8))
    {
        case DT_DEVICE:
            pu8DescriptorEX = &u8FSDeviceDescriptor[0];
            u16TxRxCounter = u8FSDeviceDescriptor[0];
            break;

        case DT_CONFIGURATION:
            // It includes Configuration, Interface and Endpoint Table
            switch((UINT8)wValue)
            {
            case 0x00:      // configuration no: 0
                pu8DescriptorEX = &u8FSConfigDescriptor01[0];
                u16TxRxCounter = u8FSConfigDescriptor01[2] + (u8FSConfigDescriptor01[3] << 8);
                break;
            default:
                result = ERROR_USB_DEVICE_INVALID_CONFIGURATION_NUM;
                goto end;
            }
            break;  
                    
        case DT_STRING:
            // DescriptorIndex = low_byte of wValue
            switch((UINT8)wValue)
            {
            case 0x00://Language ID
                pu8DescriptorEX = &u8String00Descriptor[0];
                u16TxRxCounter = u8String00Descriptor[0];
                break;
            
            case 0x01:// iManufacturer
                pu8DescriptorEX = &u8String10Descriptor[0];
                u16TxRxCounter = u8String10Descriptor[0];
                break;
            
            case 0x02:
                pu8DescriptorEX = &u8String20Descriptor[0];
                u16TxRxCounter = u8String20Descriptor[0];
                break;
            
               /* 
            case 0x30:
                pu8DescriptorEX = &u8String30Descriptor[0];
                u16TxRxCounter = u8String30Descriptor[0];
                break;
            case 0x40:
                pu8DescriptorEX = &u8String40Descriptor[0];
                u16TxRxCounter = u8String40Descriptor[0];
                break;
                
            case 0x50:
                pu8DescriptorEX = &u8String50Descriptor[0];
                u16TxRxCounter = u8String50Descriptor[0];
                break;
                
            case 0x03://iSerialNumber
                pu8DescriptorEX = &u8StringSerialNum[0];
                u16TxRxCounter = u8StringSerialNum[0];
                break;
                */
            case 0xEE:
                pu8DescriptorEX = &u8StringOSDescriptor[0];
                u16TxRxCounter = u8StringOSDescriptor[0];
                break;
            default:
                result = ERROR_USB_DEVICE_INVALID_STRING_NUM;
                goto end;
            }
            break;  
                    
        case DT_INTERFACE:
            // It cannot be accessed individually, it must follow "Configuraton"
            break;
            
        case DT_ENDPOINT:
            // It cannot be accessed individually, it must follow "Configuraton"
            break;

        case DT_HID:            
            pu8DescriptorEX = &u8FSConfigDescriptor01[CONFIG_LENGTH+INTERFACE_LENGTH];
            u16TxRxCounter = HID_LENGTH;
            break;

        case DT_DEVICE_QUALIFIER:
            result = ERROR_USB_DEVICE_INVALID_STRING_NUM;
            goto end;
        
        case DT_OTHER_SPEED_CONFIGURATION:  
            result = ERROR_USB_DEVICE_INVALID_STRING_NUM;
            goto end;

        #if (OTG_AP_Satus == Interrupt_AP)
        case DT_REPORT_DESCRIPTOR:
            pu8DescriptorEX = &hid_report_descriptor[0];
            u16TxRxCounter = HID_REPORT_DES_TOTAL_LENGTH;
            break;
        #endif
        default:
            result = ERROR_USB_DEVICE_INVALID_DESCRIPTOR;
            goto end;
    }

    if(u16TxRxCounter > *((WORD *)(ptSetupDataPacket->wLength)))
        u16TxRxCounter = *((WORD *)(ptSetupDataPacket->wLength));
    

    usb_slave_txdata((UINT8 *)pu8DescriptorEX, u16TxRxCounter, TRUE);
    
    if(DT_DEVICE == (UINT8)(wValue >> 8))
    {
        if(HID_STATUS_RESET== gu8HIDStatus)
        {        
            sensor_get_data_stop();
            SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;
            gu8HIDStatus = HID_STATUS_INIT;
            #ifdef __HID_FUSION_FUN__ // {
            hal_resume_task(tthi_FUSION);
            #endif // } __HID_FUSION_FUN__
        }
    }
end:
    if(result)
        SC_DBG("USB_DEVICE_CxGetDescriptor() return error code 0x%08X \r\n", (UINT)result);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_get_config()
//
// Description:
//   usb_slave_cmd_get_config.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE_L usb_slave_cmd_get_config()
{
    u8USBDataBuf[REPORT_IN][0]= gptUSBControl->u8Config;
    usb_slave_txdata(u8USBDataBuf[REPORT_IN], 1, TRUE);
    //Not implemented
    return USB_ERROR_SUCCESS;
}
//****************************************************************************************
// Function name: usb_slave_cmd_set_config()
//
// Description:
//   usb_slave_cmd_set_config.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE_L usb_slave_cmd_set_config(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    gptUSBControl->u8Config = *((UINT16 *)(ptSetupDataPacket->wValue));
    usb_slave_status_stage_in();
    return USB_ERROR_SUCCESS;
}
//****************************************************************************************
// Function name: usb_slave_cmd_get_status()
//
// Description:
//   usb_slave_cmd_get_status.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_get_status(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT32   result = 0;
    UINT8 endpointIndex = 0;
    UINT8 fifoIndex = 0;
    UINT8  direction = 0;
    UINT8 RecipientStatusLow  = 0;
    UINT8 RecipientStatusHigh = 0;
    UINT8 u8Tmp[2] = {0};
    UINT8 u8Object = (UINT8)(ptSetupDataPacket->bRequestType & RECIPIENT_TYPE_MASK);
    UINT16 u16Index = *((UINT16 *)(ptSetupDataPacket->wIndex));
    
    switch(u8Object) // Judge which recipient type is at first
    {
        case RECIPIENT_TYPE_DEVICE:
        /**
             * Return 2-byte's Device status (Bit1:Remote_Wakeup, Bit0:Self_Powered) to Host
             * Notice that the programe sequence of RecipientStatus
             */
            RecipientStatusLow = (gptUSBControl->u8RemoteWakeup)? BIT1 : 0;
            // Bit0: Self_Powered--> DescriptorTable[0x23], D6(Bit 6)
            RecipientStatusLow |= ((u8FSConfigDescriptor01[0x07] >> 6) & 0x01);
            break;
            
        case RECIPIENT_TYPE_INTERFACE:
            // Return 2-byte ZEROs Interface status to Host
            break;

        case RECIPIENT_TYPE_ENDPOINT:
            if(u16Index == 0x00)
            {
                RecipientStatusLow = (UINT8)gptUSBControl->bEPHalt;
            }
            else
            {
                endpointIndex = u16Index & 0x7F;      // which ep will be clear
                direction = u16Index >> 7;            // the direction of this ep
                if(endpointIndex > DEVICE_MAX_ENDPOINT_NUM)      // over the Max. ep count ?
                {
                    result = ERROR_USB_DEVICE_INVALID_ENDPOINT_NUM;
                    goto end;
                }

                fifoIndex = endpointIndex;//USB_DEVICE_GetEndpointMapReg(endpointIndex, direction);      // get the relatived FIFO number
                if(fifoIndex >= DEVICE_MAX_FIFO_NUM) // over the Max. fifo count ?
                {
                    result = ERROR_USB_DEVICE_INVALID_FIFO_NUM;
                    goto end;
                }

                // Check the FIFO had been enable ?
                if(REG_READ_8BIT(GET_EP_CON_REG(endpointIndex)) == 0)
                {
                    result = ERROR_USB_DEVICE_FIFO_NOT_ENABLED;
                    goto end;
                }
                
                RecipientStatusLow = usb_slave_is_ep_stall(endpointIndex, direction);
            }
            break;
            
        default :
            result = ERROR_USB_DEVICE_INVALID_RECIPIENT;
            goto end;
    }

    // return RecipientStatus;
    u8Tmp[0] = RecipientStatusLow;
    u8Tmp[1] = RecipientStatusHigh;
    
    usb_slave_txdata(u8Tmp, 2, TRUE);    

end:
    if(result)
        SC_DBG("USB_DEVICE_CxGetStatus() return error code 0x%08X \r\n", (UINT)result);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_set_feature()
//
// Description:
//   usb_slave_cmd_set_feature.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_set_feature(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT32   result = 0;
    UINT8 endpointIndex = 0;
    UINT8 fifoIndex = 0;
    /*UINT8 i = 0;
    UINT8 u8Tmp[52] = {0};
    UINT8 *pp = NULL;*/
    BOOL direction = 0;
    UINT16 u16Value = *((UINT16 *)(ptSetupDataPacket->wValue));
    UINT16 u16Index = *((UINT16 *)(ptSetupDataPacket->wIndex));

    
    // FeatureSelector
    switch(u16Value)        
    {
        case FEATURE_SEL_ENDPOINT_HALT:
            // Set "Endpoint_Halt", Turn on the "STALL" bit in Endpoint Control Function Register
            if(u16Index == 0x00)
                gptUSBControl->bEPHalt = TRUE;
            else
            {
                endpointIndex = u16Index & 0x7F;        // which ep will be clear
                direction = u16Index >> 7;            // the direction of this ep
                if(endpointIndex > DEVICE_MAX_ENDPOINT_NUM)           // over the Max. ep count ?
                {
                    result = ERROR_USB_DEVICE_INVALID_ENDPOINT_NUM;
                    goto end;
                }

                fifoIndex = endpointIndex;//USB_DEVICE_GetEndpointMapReg(endpointIndex, direction);      // get the relatived FIFO number
                if(fifoIndex >= DEVICE_MAX_FIFO_NUM) // over the Max. fifo count ?
                {
                    result = ERROR_USB_DEVICE_INVALID_FIFO_NUM;
                    goto end;
                }

                // Check the FIFO had been enable ?
                if((REG_READ_8BIT(GET_EP_CON_REG(endpointIndex))& BIT0) == 0)
                {
                    result = ERROR_USB_DEVICE_FIFO_NOT_ENABLED;
                    goto end;
                }
                
                usb_slave_set_ep_stall(endpointIndex, direction, TRUE);// Set Stall Bit

            }
            break;
                
        case FEATURE_SEL_DEVICE_REMOTE_WAKEUP:
            // Set "Device_Remote_Wakeup", Turn on the"RMWKUP" bit in Mode Register
            //USB_DEVICE_EnableRemoteWakeupReg(MMP_TRUE);
            gptUSBControl->u8RemoteWakeup = TRUE;
            break;
 
        default :
            result = ERROR_USB_DEVICE_INVALID_FEATURE_SEL;
            goto end;
    }

end:
    if(result)
        SC_DBG("USB_DEVICE_CxSetFeature() return error code 0x%08X \r\n", (UINT)result);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_clear_feature()
//
// Description:
//   usb_slave_cmd_set_feature.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************

INT32 USB_CODE usb_slave_cmd_clear_feature(SETUP_DATA_PACKET *ptSetupDataPacket)

{
    UINT32   result = 0;
    UINT8 endpointIndex = 0;
    UINT8 fifoIndex = 0;
    /*UINT8 i = 0;
    UINT8 u8Tmp[52] = {0};
    UINT8 *pp = NULL;*/
    BOOL direction = 0;
    UINT16 u16Value = *((UINT16 *)(ptSetupDataPacket->wValue));
    UINT16 u16Index = *((UINT16 *)(ptSetupDataPacket->wIndex));


    // FeatureSelector
    switch(u16Value)        
    {
    case FEATURE_SEL_ENDPOINT_HALT:
        // Clear "Endpoint_Halt", Turn off the "STALL" bit in Endpoint Control Function Register
        if(u16Index == 0x00)
                gptUSBControl->bEPHalt = FALSE;
        else
        {
            endpointIndex = u16Index & 0x7F;        // which ep will be clear
            direction = u16Index >> 7;            // the direction of this ep
            if(endpointIndex > DEVICE_MAX_ENDPOINT_NUM)           // over the Max. ep count ?
            {
                result = ERROR_USB_DEVICE_INVALID_ENDPOINT_NUM;
                goto end;
            }
            
            fifoIndex = endpointIndex;//USB_DEVICE_GetEndpointMapReg(endpointIndex, direction);      // get the relatived FIFO number
            if(fifoIndex >= DEVICE_MAX_FIFO_NUM) // over the Max. fifo count ?
            {
                result = ERROR_USB_DEVICE_INVALID_FIFO_NUM;
                goto end;
            }
            
            // Check the FIFO had been enable ?
            if((REG_READ_8BIT(GET_EP_CON_REG(endpointIndex))& BIT0) == 0)
            {
                result = ERROR_USB_DEVICE_FIFO_NOT_ENABLED;
                goto end;
            }
            
            usb_slave_set_ep_stall(endpointIndex, direction, FALSE);// Set Stall Bit

        }
        break;
                
    case FEATURE_SEL_DEVICE_REMOTE_WAKEUP:
        /**
         * Clear "Device_Remote_Wakeup", Turn off the"RMWKUP" bit in Main Control Register
         */
        gptUSBControl->u8RemoteWakeup = FALSE;
        break;

    case FEATURE_SEL_DEVICE_TEST_MODE:
        result = ERROR_USB_DEVICE_UNSUPPORT_FEATURE_SEL;
        goto end;
            
    default :
        result = ERROR_USB_DEVICE_INVALID_FEATURE_SEL;
        goto end;
    }
    
    
end:
    if(result)
        SC_DBG("USB_DEVICE_CxClearFeature() return error code 0x%08X \r\n", (UINT)result);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_std()
//
// Description:
//   usb_slave_cmd_std.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_std(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT32 result = 0;



    // by Standard Request codes
    switch(ptSetupDataPacket->bRequest) 
    {

        case SET_ADDRESS:
            if(!gptUSBControl->bEPHalt)
                result = usb_slave_cmd_set_address(ptSetupDataPacket);
            break;

        case GET_DESCRIPTOR:
            if(!gptUSBControl->bEPHalt)
                result = usb_slave_cmd_get_descriptor(ptSetupDataPacket);
            break;

        case SET_FEATURE:
            usb_slave_cmd_set_feature(ptSetupDataPacket);
            usb_slave_status_stage_in();
            break;

        case GET_STATUS:  
            usb_slave_cmd_get_status(ptSetupDataPacket);
            break;      
        case CLEAR_FEATURE:
            usb_slave_cmd_clear_feature(ptSetupDataPacket);
            usb_slave_status_stage_in();
            break;
        case GET_CONFIGURATION:
            if(!gptUSBControl->bEPHalt)
                result = usb_slave_cmd_get_config();
            break;

        case SET_CONFIGURATION:
            if(!(gptUSBControl->bEPHalt))
                result = usb_slave_cmd_set_config(ptSetupDataPacket);
                break;
            
        default:
            result = ERROR_USB_DEVICE_INVALID_REQUEST_CODE; 
            break;
    }
    
    if(result)
        SC_DBG("USB_DEVICE_StandardCommand() return error code 0x%08X, request code = %d \r\n", (UINT)result, ptSetupDataPacket->bRequest);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_class()
//
// Description:
//   usb_slave_setup.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_class(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT32 result = 0;
    UINT16 u16Length;
    static BYTE u8IdleRate = 0;
    static BYTE u8Protocol = REPORT_PROTOCOL;
    //UINT8 u8Tmp[2] = {0};



    // by Standard Request codes
    switch(ptSetupDataPacket->bRequest) 
    {
        //for warning temporarily (when HID is not enabled)
        case 0:
            result = ERROR_USB_DEVICE_INVALID_REQUEST_CODE; 
            goto end;

        //for keyboard
        case GET_IDLE://Get IDLE
            u16Length = 1; 
            u8USBDataBuf[REPORT_IN][0]= u8IdleRate;
            usb_slave_txdata(u8USBDataBuf[REPORT_IN], u16Length, TRUE);
            break;
         
        case GET_PROTOCOL://Get protocol
            u16Length = 1; 
            u8USBDataBuf[REPORT_IN][0]= u8Protocol;
            usb_slave_txdata(u8USBDataBuf[REPORT_IN], u16Length, TRUE);
            break;            
         
        case SET_PROTOCOL:
            u8Protocol = *((UINT16 *)(ptSetupDataPacket->wValue));
            usb_slave_status_stage_in();  
            break;

        #if(OTG_AP_Satus == Interrupt_AP) //Shawn: for HID device
        case GET_REPORT:  // GET_REPORT 
            //printk("ge %X %X\r\n", ptSetupDataPacket->bRequest,  *((UINT16 *)(ptSetupDataPacket->wValue)));
            #ifdef __CMD_DEBUG__ // {            
            gu8LastClass = 1;
            #endif //} __CMD_DEBUG__
            USB_DBG_GET_REPORT_SET();
            u16Length = (UINT16)usb_slave_hid_cmd_dispatch(ptSetupDataPacket->bRequest, *((UINT16 *)(ptSetupDataPacket->wValue)), u8USBDataBuf[REPORT_IN], NULL, 0);
            if(u16Length == 0)
            {
                result = ERROR_USB_DEVICE_INVALID_VALUE;
                goto end;
            }            
            USB_DBG_GET_REPORT_CLEAR();
            usb_slave_txdata(u8USBDataBuf[REPORT_IN], u16Length, TRUE);
                        
            break;
        
        case SET_REPORT:  // SET_REPORT    
            //printk("se %X %X\r\n", ptSetupDataPacket->bRequest,  *((UINT16 *)(ptSetupDataPacket->wValue)));                      
            #ifdef __CMD_DEBUG__ // {            
            gu8LastClass = 2;
            #endif //} __CMD_DEBUG__
            if(*((UINT16 *)(ptSetupDataPacket->wLength)) > 64)
            {                
                result = ERROR_USB_DEVICE_INVALID_REQUEST_CODE;
                goto end;
            }
            if(USB_ERROR_SUCCESS != (result = usb_slave_rxdata(u8USBDataBuf[REPORT_OUT],*((UINT16 *)(ptSetupDataPacket->wLength)), TRUE)))
            {
                goto end;
            }     
                            
            break;
            #endif

        case SET_IDLE:              
            u8IdleRate = ptSetupDataPacket->wValue[1];  
            usb_slave_status_stage_in();  
            break;
        default:
            result = ERROR_USB_DEVICE_INVALID_REQUEST_CODE; 
            break;
    }
    
end:
    if(result)
        SC_DBG("usb_slave_cmd_class() return error code 0x%08X, request code = %d \r\n", (UINT)result, ptSetupDataPacket->bRequest);
    return result;
}
//****************************************************************************************
// Function name: usb_slave_cmd_vendor()
//
// Description:
//   usb_slave_cmd_vendor.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
UINT32 USB_CODE usb_slave_cmd_vendor(SETUP_DATA_PACKET *ptSetupDataPacket)
{
    UINT32 result = 0;
    //UINT8 u8Tmp[2] = {0};     
    const UINT8 *pu8DescriptorEX = NULL;
    UINT16 u16TxRxCounter = 0;

    // by Standard Request codes
    switch(ptSetupDataPacket->bRequestType)
    {
    case 0xC1:  // Get Vender .  
            pu8DescriptorEX = &u8StringExtendPropertyDescriptor[0];
            u16TxRxCounter = u8StringExtendPropertyDescriptor[0];
            break;

    default:
        result = ERROR_USB_DEVICE_INVALID_REQUEST_CODE; 
        goto end;
    } 

    if(u16TxRxCounter > *((WORD *)(ptSetupDataPacket->wLength)))
        u16TxRxCounter = *((WORD *)(ptSetupDataPacket->wLength));
    

    usb_slave_txdata((UINT8 *)pu8DescriptorEX, u16TxRxCounter, TRUE);

end:
    if(result)
        SC_DBG("usb_slave_cmd_vendor() return error code 0x%08X, request code = %d \r\n", (UINT)result, ptSetupDataPacket->bRequest);
    return result;    
}

//****************************************************************************************
// Function name: usb_slave_setup()
//
// Description:
//   usb_slave_setup.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_setup(UINT8 u8EndPointNo)
{

    UINT32     result = 0;
    UINT8 *pu8PacketBuf = NULL;
    UINT8 u8DataSize = 0;

    //====================================================
    /**
     * First we must check if this is the first Cx 8 byte command after USB reset.
     * If this is the first Cx 8 byte command, we can check USB High/Full speed right now.
     */
    //====================================================
    // first ep0 command after usb reset, means we can check usb speed right now.

    //==========================================
    // Read 8-byte setup packet from FIFO
    //==========================================    

    pu8PacketBuf = (UINT8 *)u8USBDataBuf[REPORT_IN];

    if(USB_IS_AUTOREADY())
    {
        u8DataSize = usb_slave_read_fifo(pu8PacketBuf, sizeof(u8USBDataBuf[REPORT_IN]), u8EndPointNo,TRUE);

        if(u8DataSize >= sizeof(gtSetupDataPacket))
            memcpy(&gtSetupDataPacket,pu8PacketBuf+u8DataSize-sizeof(gtSetupDataPacket),sizeof(gtSetupDataPacket));
        else
            return;
    }
    else
    {
        u8DataSize = sizeof(gtSetupDataPacket);
        if(u8DataSize != usb_slave_read_fifo(pu8PacketBuf, u8DataSize, u8EndPointNo,TRUE))
        {
            usb_slave_set_ep0_ready();
            return;
        }
        memcpy(&gtSetupDataPacket,pu8PacketBuf,sizeof(gtSetupDataPacket));
    }
    //Setup data packet and out data packet share the same buffer
    gptPacket = &gtSetupDataPacket;
    USB_DBG_SET_LAST_CMD(pu8PacketBuf);
    //SC_DBG("[s0]t=%X, nak t=%X\r\n",REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));   
    
    //==========================================
    // Save to setup command data structure.
    //==========================================
    if(!(((pu8PacketBuf[0] == 0x40)&&(pu8PacketBuf[1] == 0x00)&&(pu8PacketBuf[2] == 0x00)&&(pu8PacketBuf[3] == 0x00)&&(pu8PacketBuf[4] == 0x00)&&(pu8PacketBuf[5] == 0x00))||
        ((pu8PacketBuf[0] == 0xC0)&&(pu8PacketBuf[1] == 0x00)&&(pu8PacketBuf[2] == 0x00)&&(pu8PacketBuf[3] == 0x00)&&(pu8PacketBuf[4] == 0x00)&&(pu8PacketBuf[5] == 0x00))))
    {       
        // do not print test vendor command
        SC_DBG("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X \r\n",
                  pu8PacketBuf[0], pu8PacketBuf[1], pu8PacketBuf[2], pu8PacketBuf[3], 
                  pu8PacketBuf[4], pu8PacketBuf[5], pu8PacketBuf[6], pu8PacketBuf[7]);
    }

    //==========================================
    // Command Decode
    //==========================================
    if((gptPacket->bRequestType& REQUEST_TYPE_MASK) == REQUEST_TYPE_STANDARD)     // standard command
    {
        result = usb_slave_cmd_std(gptPacket);
        if(result)
            goto end;
    }    
    else if((gptPacket->bRequestType& REQUEST_TYPE_MASK) == REQUEST_TYPE_CLASS)     // standard command
    {
        result = usb_slave_cmd_class(gptPacket);
        if(result)
            goto end;
    }
    else if((gptPacket->bRequestType& REQUEST_TYPE_MASK) == REQUEST_TYPE_VENDOR)     // standard command
    {
        result = usb_slave_cmd_vendor(gptPacket);
        if(result)
            goto end;
    }
    else
    {
        result = ERROR_USB_DEVICE_INVALID_REQUEST_TYPE;
        goto end;
    }
    
end:

    if(result)
    {
        gptUSBControl->Slave_FinishAction = USB_ACT_STALL;        
    }
    else
    {
        gptUSBControl->Slave_FinishAction = USB_ACT_DONE;
    }
    return;
}
//****************************************************************************************
// Function name: usb_slave_sw_init()
//
// Description:
//   usb_slave_sw_init.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_sw_init(BOOL bResume)
{
    gptUSBControl->bIsReset = FALSE;
    gptUSBControl->bEPHalt = FALSE;
    gptUSBControl->Slave_FinishAction = USB_ACT_IDLE;
    gptUSBControl->bEPHalt = FALSE;
    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;
    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
    gptUSBControl->u8LastTransaction = USB_TRANSACTION_SETUP;
    gptUSBControl->u8SuspendCounter = 0;
    gptUSBControl->u8SuspendCountingFlag = FALSE;
    if(!bResume)
        gptUSBControl->u8RemoteWakeup = FALSE;
}
//****************************************************************************************
// Function name: usb_slave_ip_init()
//
// Description:
//   usb_slave_ip_init.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_ip_init(UINT8 p_action)
{    
    UINT8 l_usb_speed_detect;

    if(p_action==0)
    {
        //init start    

        SC_DBG("Slave init cREG=0x%X, \r\n",REG_READ_8BIT(HOST_SLAVE_CONTROL_REG));
        REG_WRITE_8BIT(HOST_SLAVE_CONTROL_REG, BIT1);   // Reset slave    
        hal_sleep(1);   
        REG_WRITE_8BIT(PORT0_MISC_CONTROL_REGISTER, REG_READ_8BIT(PORT0_MISC_CONTROL_REGISTER)&(~BIT4));
    	REG_WRITE_8BIT(PORT1_MISC_CONTROL_REGISTER, REG_READ_8BIT(PORT1_MISC_CONTROL_REGISTER)&(~BIT4));
        
        REG_WRITE_8BIT(HOST_SLAVE_CONTROL_REG, 0x00);   // Reset slave
        
        //REG_WRITE_8BIT(SC_CONTROL_REG, 0x08);    
        //REG_WRITE_8BIT(SC_CONTROL_REG, 0x00);
    }
    
    //Detect Speed
	#if 0
	{
		SC_DBG("Slave low speed connection\r\n");
        REG_WRITE_8BIT(SC_CONTROL_REG, 0x01);
        l_usb_speed_detect = 1;
	}
	#else
	{
		SC_DBG("Slave full speed connection\r\n");        
    	REG_WRITE_8BIT(SC_CONTROL_REG, SC_GLOBAL_ENABLE_BIT|
                                       SC_FULL_SPEED_LINE_POLARITY_BIT|
                                       SC_FULL_SPEED_LINE_RATE_BIT|
                                       SC_CONNECT_TO_HOST_BIT);
        l_usb_speed_detect = 1;
	}
    #endif
        
        //REG_WRITE_8BIT(SC_CONTROL_REG, 0x31);

    if(l_usb_speed_detect)
    {
        SC_DBG("SC_CONTROL_REG=0x%X\r\n",REG_READ_8BIT(SC_CONTROL_REG));
        SC_DBG("HOST_SLAVE_CONTROL_REG=0x%X\r\n",REG_READ_8BIT(HOST_SLAVE_CONTROL_REG));

        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG,SC_TRANS_DONE_BIT|SC_RESET_EVENT_BIT|SC_SOF_RECEIVED_BIT); 
        REG_WRITE_8BIT(SC_INTERRUPT_MASK_REG, SC_TRANS_DONE_BIT|SC_RESET_EVENT_BIT);
        IER2 |= BIT7; 
        REG_WRITE_8BIT(SC_ADDRESS,0x00);  
        usb_slave_set_ep0_ready();
        REG_WRITE_8BIT(ENDPOINT1_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT|ENDPOINT_DIRECTION_BIT);
        SC_DBG("SC LINE REG=0x%X, INTERRUPT = 0x%X\r\n",REG_READ_8BIT(SC_LINE_STATUS_REG),REG_READ_8BIT(SC_INTERRUPT_STATUS_REG));
        SC_DBG("EP c=%X,st=%X,t=%X, nak t=%X\r\n", REG_READ_8BIT(ENDPOINT0_CONTROL_REG),REG_READ_8BIT(ENDPOINT0_STATUS_REG),REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));
        if(USB_IS_AUTOREADY())
            gpSHDInfo->pUSBDbgMsg[1] = 0x80;
        else
            gpSHDInfo->pUSBDbgMsg[1] = 0;
        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_READY;
    }
    else
    {
        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_DETECT_SPEED;
    }
}
//****************************************************************************************
// Function name: usb_slave_is_shutdown()
//
// Description:
//   usb_slave_is_shutdown.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
inline BOOL USB_CODE_L usb_slave_is_shutdown(void)
{    
#ifdef __USB_SLAVE_SHUTDOWN_PAD__ // {
    return ((USB_SLAVE_INIT_STEP_IDLE == USB_SLAVE_INIT_STEP)&&(0 == (PMER2&BIT7)));
#else
    return ((USB_SLAVE_INIT_STEP_IDLE == USB_SLAVE_INIT_STEP)&&(gbUSBSuspended));
#endif // } __USB_SLAVE_SHUTDOWN_PAD__
}
//****************************************************************************************
// Function name: usb_slave_is_require_resume()
//
// Description:
//  usb_slave_is_require_resume.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
inline BOOL USB_CODE_L usb_slave_is_require_resume(void)
{
    return gbUSBNoSuspend|gbUSBResumeSet;
}
//****************************************************************************************
// Function name: usb_slave_check_supend()
//
// Description:
//   usb_slave_check_supend.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_check_suspend()
{
    BOOL bSuspend = FALSE;
    if(USB_SLAVE_INIT_STEP_IDLE == USB_SLAVE_INIT_STEP)
    {
    
        #ifdef __USB_SLAVE_SHUTDOWN_PAD__ // {
        if(0 == (PMER2&BIT7))// suspended
        #else
        if(gbUSBSuspended)
        #endif // } __USB_SLAVE_SHUTDOWN_PAD__
        {
            return;
        }
        if(0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_SOF_RECEIVED_BIT))//suspend /reset no first
        {
            REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, SC_SOF_RECEIVED_BIT|SC_RESUME_INT_BIT);  
            gptUSBControl->u8SuspendCounter = 0;
            gptUSBControl->u8SuspendCountingFlag = TRUE;
        }
        else
        {
            if(gptUSBControl->u8SuspendCountingFlag)
            {
                if(gptUSBControl->u8SuspendCounter < USB_SUSPEND_COUNTER)
                {
                    gptUSBControl->u8SuspendCounter++;
                }
                else 
                {
                    bSuspend = TRUE;
                }
            }
            else
            {
                if(gptUSBControl->u8SuspendCounter < 0xFF)
                    gptUSBControl->u8SuspendCounter++;
                else        
                    bSuspend = TRUE;
            }
        }
        if(bSuspend)
        {
            if(((GPDRH & BIT5) == 0) && ((GPDRH & BIT6) == 0))
                return;
            USB_DBG_SUPEND_ENTER();
            USB_DBG_ERROR_SET(USB_DBG_ERROR_SUSPEND_BIT);      
            #ifdef __USB_SLAVE_SHUTDOWN_PAD__ // {
            PMER2  &= ~BIT7; // disable USB Pad     
            #else
            gbUSBSuspended = TRUE;
            #endif // } __USB_SLAVE_SHUTDOWN_PAD__                     
            REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, REG_READ_8BIT(SC_INTERRUPT_STATUS_REG));                                   
            
            WUESR9 = BIT1;      // clear interrupt
            WUESR9 = BIT2;      // clear interrupt 
            IER10  |= BIT6;      // INT86 GPDRH5 WUI interrupt mask enable
            IER10  |= BIT7;      // INT87 GPDRH6 WUI interrupt mask enable

            SET_MASK(CGCTRL3R, BIT5); // Gating USB CLK
                        
            USB_DBG_SUPEND_LEAVE();
//	            usb_slave_hid_cmd_dispatch(0xFF, 0,u8USBDataBuf[REPORT_IN] , NULL);
            usb_slave_hid_update_last_data(); //Shawn: 20131030
            SC_DBG("sus\r\n");   
        }
    }
}

//****************************************************************************************
// Function name: usb_slave_check_resume()
//
// Description:
//   usb_slave_check_resume.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_check_resume()
{
    #ifdef __USB_SLAVE_SHUTDOWN_PAD__ // {
    UINT8 u8TmpAddr = 0;
    #endif // } __USB_SLAVE_SHUTDOWN_PAD__     
    if(usb_slave_is_shutdown()) // suspended
    {
        if(USB_RESUME_REMOTEWAKEUP == gu8USBResumeFlag)
        {
            if(!gptUSBControl->u8RemoteWakeup)
            {
                gbUSBResumeSet = FALSE;
                return;
            }
            //if(0 == usb_slave_hid_cmd_dispatch(0xFF, 0,u8USBDataBuf[REPORT_IN] , NULL))
            if(FALSE == usb_slave_hid_check_data_ready()) //Shawn
            {
                gbUSBResumeSet = FALSE;
                return;                  
            }
            else if(IS_MASK_SET(CGCTRL3R, BIT5))
            {
                CLEAR_MASK(CGCTRL3R, BIT5);     // Enable USB clock
            }
        }
        
        USB_DBG_RESUME_ENTER();

        #ifdef __USB_SLAVE_SHUTDOWN_PAD__ // {
                
        if(USB_RESUME_RESET != gu8USBResumeFlag)
            u8TmpAddr = REG_READ_8BIT(SC_ADDRESS);
        PMER2  |= BIT7; // enable USB Pad      
        usb_slave_sw_init((USB_RESUME_RESET != gu8USBResumeFlag));
        usb_slave_ip_init(0);
        usb_slave_ip_init(1);  
        if(USB_RESUME_RESET != gu8USBResumeFlag)
            REG_WRITE_8BIT(SC_ADDRESS, u8TmpAddr);

        #else        
        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, REG_READ_8BIT(SC_INTERRUPT_STATUS_REG));        
        usb_slave_sw_init((USB_RESUME_RESET != gu8USBResumeFlag));
        gbUSBSuspended = FALSE;
            
        #endif // } __USB_SLAVE_SHUTDOWN_PAD__     

        
        USB_DBG_ERROR_CLEAR(USB_DBG_ERROR_SUSPEND_BIT);     
        USB_DBG_RESUME_LEAVE();
        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_IDLE;
        SC_DBG("res %X\r\n", gu8USBResumeFlag);  
        
        if(USB_RESUME_REMOTEWAKEUP == gu8USBResumeFlag)
        {   
            REG_WRITE_8BIT(SC_CONTROL_REG, SC_GLOBAL_ENABLE_BIT|
                                           SC_FULL_SPEED_LINE_POLARITY_BIT|
                                           SC_FULL_SPEED_LINE_RATE_BIT|
                                           SC_DIRECT_CONTROL_BIT|
                                           SC_TX_LINE_STATE_DM|
                                           SC_CONNECT_TO_HOST_BIT);    
            // The remote wakeup device must hold the resume signaling for at least 1 ms but for no more than 15 ms
            hal_sleep(1);    	
            REG_WRITE_8BIT(SC_CONTROL_REG, SC_GLOBAL_ENABLE_BIT|
                                           SC_FULL_SPEED_LINE_POLARITY_BIT|
                                           SC_FULL_SPEED_LINE_RATE_BIT|
                                           SC_CONNECT_TO_HOST_BIT);    
        }
        gbUSBResumeSet = FALSE;
        
    }
}

//****************************************************************************************
// Function name: usb_slave_handler()
//
// Description:
//   usb_slave_handler.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_handler()
{
    UINT32 u32Error;
    USB_DBG_MSG_L("(%d)waiting fifo data... \r\n", __LINE__);

    if(usb_slave_is_shutdown())
    {
        if(gbUSBResumeSet)    
            usb_slave_check_resume();
        return;
    }
    #ifdef USB_DBG_ALL_ON // {
    if(USB_DBG_HELPER_REPLUG == USB_DBG_HELPER())
    {        
        IER2 &= ~BIT7; 
        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, SC_TRANS_DONE_BIT|SC_RESET_EVENT_BIT);        
        usb_slave_sw_init(FALSE);          
        REG_WRITE_8BIT(SC_CONTROL_REG, 0x08);    
        DelayXms(1);        
        REG_WRITE_8BIT(SC_CONTROL_REG, 0x71);   
        IER2 |= BIT7; 
        USB_DBG_HELPER() = 0;
        return;
    }
    else if(USB_DBG_HELPER_RESETSENSOR == USB_DBG_HELPER())
    {
        sensor_get_data_stop();  
        SENSOR_INIT_STEP = SENSOR_INIT_STEP_RESET_PARAMETER;    
        gu8HIDStatus = HID_STATUS_INIT;  
        USB_DBG_HELPER() = 0;
        #ifdef __HID_FUSION_FUN__ // {
        hal_resume_task_in_isr(tthi_FUSION);
        #endif // } __HID_FUSION_FUN__
    }
    #endif // USB_DBG_ALL_ON

    if(0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_RESET_EVENT_BIT))
    {
        SC_DBG("Handle reset  INT %X \r\n", REG_READ_8BIT(SC_INTERRUPT_STATUS_REG)); 
        usb_slave_check_reset();
    }
    else
    {     
        gptUSBControl->bIsReset = FALSE;  
        REG_WRITE_8BIT(SC_INTERRUPT_MASK_REG, SC_TRANS_DONE_BIT|SC_RESET_EVENT_BIT);
        if(USB_TRANSFER_STAGE_SETUP == gptUSBControl->u8TransferStage)
            usb_slave_check_interrupt_data();
        if(0 == (REG_READ_8BIT(ENDPOINT0_CONTROL_REG)& ENDPOINT_READY_BIT)) //EP0 Done
        {    
            gptUSBControl->u8LastTransaction = REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG);            
            USB_DBG_LAST_TRAN(gptUSBControl->u8LastTransaction);

            SC_DBG("Handle EP0 con %X %X, INT %X sta %X nak %X\r\n", REG_READ_8BIT(ENDPOINT0_CONTROL_REG),gptUSBControl->u8LastTransaction, REG_READ_8BIT(SC_INTERRUPT_STATUS_REG),gptUSBControl->u8TransferStage,REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG)); 

            if(0 != (REG_READ_8BIT(ENDPOINT0_STATUS_REG)&SC_STALL_SENT_BIT))
            {
                SC_ERR_DBG("st");
                gptUSBControl->Slave_FinishAction = USB_ACT_IDLE;
                gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;
                USB_DBG_STAGE(gptUSBControl->u8TransferStage);
                usb_slave_set_ep0_ready();  
                return;
            }
                
            switch(gptUSBControl->u8LastTransaction)
            {
                case USB_TRANSACTION_SETUP:                    
                    if(USB_IS_AUTOREADY())
                    {
                        if(((REG_READ_8BIT(EP0_RX_FIFO_DATA_COUNT_MSB)==0)&&(REG_READ_8BIT(EP0_RX_FIFO_DATA_COUNT_LSB)==0)))
                            break;
                    }
                    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;
                    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
                    if(usb_slave_check_rx_status())
                    {
                        usb_slave_setup(0);   
                    }
                    else
                    {                 
                        USB_DBG_ERROR("Setup Err %X nT %X\r\n", REG_READ_8BIT(ENDPOINT0_STATUS_REG), REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));
                        usb_slave_set_ep0_ready();
                    }
                    break;
                case USB_TRANSACTION_OUT:   
                    
                    if(USB_TRANSFER_STAGE_STATUS_OUT == gptUSBControl->u8TransferStage)
                    { 
                        //handled in ISR               
                        USB_DBG_ERROR_SET(USB_DBG_ERROR_STATUS_OUT_BIT);
                    }
                    else if (USB_TRANSFER_STAGE_SETUP== gptUSBControl->u8TransferStage)
                    {
                        //if status ISR is delayed, EP control is 1 at first, but it may be trigger after EP done check is passed
                        //, this condition will occured , but at this time EP should be modified to 3 after ISR is done. so do nothing here
                        USB_DBG_MSG_L("(%05d)out set nak T %X\r\n", __LINE__, REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));
                        return;
                    }
                    else
                    {
                        if(USB_TRANSFER_STAGE_DATA_IN == gptUSBControl->u8TransferStage)
                        {
                            // wrong TX retry may cause next command is recieved
                            gptUSBControl->u8TransferStage = USB_TRANSACTION_SETUP;
                            USB_DBG_ERROR_SET(USB_DBG_ERROR_RX_BIT);
                            return;
                        }
                        if(USB_ERROR_SUCCESS != (u32Error = usb_slave_rxdata(gptUSBControl->pu8DataBuffer, gptUSBControl->u16TotalDataNum, FALSE)))
                        {                                
                            USB_DBG_ERROR_SET(USB_DBG_ERROR_RX_BIT); 
                            USB_DBG_ERROR("(%05d)Error OUT %X.\r\n", __LINE__, u32Error);
                        }
        
                    }                     
                    break;
                case USB_TRANSACTION_IN:
                    if(USB_TRANSFER_STAGE_STATUS_IN == gptUSBControl->u8TransferStage)
                    {            
                        if(gptUSBControl->u8Addr != 0)
                        {
                            REG_WRITE_8BIT(SC_ADDRESS,gptUSBControl->u8Addr);        
                            gptUSBControl->u8Addr = 0;
                        }
                        //handled in ISR
                        USB_DBG_ERROR_SET(USB_DBG_ERROR_STATUS_IN_BIT);
                        return;
                    }
                    else if (USB_TRANSFER_STAGE_SETUP== gptUSBControl->u8TransferStage)
                    {
                        //if status ISR is delayed, EP control is 1 at first, but it may be trigger after EP done check is passed
                        //, this condition will occured , but at this time EP should be modified to 3 after ISR is done. so do nothing here
                        USB_DBG_MSG_L("(%05d)in set nak T %X.\r\n", __LINE__, REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));
                        return;
                    }
                    else
                    {
                        if(USB_TRANSFER_STAGE_DATA_OUT == gptUSBControl->u8TransferStage)
                        {
                            USB_DBG_ERROR_SET(USB_DBG_ERROR_TX_BIT);
                            return;
                        }
                        if(USB_ERROR_SUCCESS != (u32Error = usb_slave_txdata(gptUSBControl->pu8DataBuffer, gptUSBControl->u16TotalDataNum, FALSE)))
                        {
                            USB_DBG_ERROR_SET(USB_DBG_ERROR_TX_BIT);
                            USB_DBG_ERROR("(%05d)Error IN %X st %X \r\n", __LINE__, (UINT)u32Error, REG_READ_8BIT(ENDPOINT0_STATUS_REG));
                        }
                    }
                    break;
            }
            if(USB_ACT_STALL == gptUSBControl->Slave_FinishAction)
            {
                SC_DBG("STALL\r\n");
                REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG,ENDPOINT_ENABLE_BIT|ENDPOINT_READY_BIT|ENDPOINT_SEND_STALL_BIT);
             
            }
        }   
    }
}
//****************************************************************************************
// Function name: usb_slave_main()
//
// Description:
//   usb_slave_main.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_slave_main()
{
    USB_DBG_MAIN_ENTER();

    USB_DBG_CMD(USB_DBG_CODE_ENTER_MAIN);

    usb_slave_handler();        
     
    USB_DBG_CMD(USB_DBG_CODE_LEAVE_MAIN);
          
    if(!gbUSBNoSuspend)
    {
        USB_DBG_MAIN_LEAVE();        
        USB_DBG_CMD(USB_DBG_CODE_LEAVE_MAIN_SUSPEND);
        #ifdef __HID_FUSION_FUN__ // {
        hal_suspend_task(tthi_HID);   
        #endif
    }
    else
    {        
        gbUSBNoSuspend = FALSE;
    }
    //SC_DBG("[m1]ep c=%X,st=%X,t=%X, nak t=%X\r\n", REG_READ_8BIT(ENDPOINT0_CONTROL_REG),REG_READ_8BIT(ENDPOINT0_STATUS_REG),REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG));

}

//****************************************************************************************
// Function name: usb_hid_isr()
//
// Description:
//   usb_hid_isr.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE usb_hid_isr(void)
{ 
    BOOL bIsSetup = FALSE;

    ISR2 = BIT7;

    //
    // Do ISR
    //
    if(usb_slave_is_shutdown())
    {
        gbUSBSuspended = FALSE;
        USB_DBG_ERROR_CLEAR(USB_DBG_ERROR_SUSPEND_BIT);  
    }
    if(0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_RESET_EVENT_BIT))
    {        
        REG_WRITE_8BIT(SC_INTERRUPT_MASK_REG, SC_TRANS_DONE_BIT);  

        USB_DBG_CMD(USB_DBG_CODE_ISR_RESET);         
        usb_slave_check_reset();
        #ifdef __HID_FUSION_FUN__ // {
        hal_resume_task_in_isr(tthi_HID);
        #endif

        //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        usb_isr_service_flag();
        //#endif
    }
    else if(0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_TRANS_DONE_BIT))
    {
        gptUSBControl->u8SuspendCounter = 0;
        USB_DBG_LAST_TRAN_ISR(REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG));
        #ifdef __HID_FUSION_FUN__ // {
        if(!hal_thread_task_Is_suspended(tthi_HID))
            gbUSBNoSuspend = TRUE;
        #endif  // } __HID_FUSION_FUN__
        
        if(USB_IS_AUTOREADY())
        {
            // prevent stall is sent for wrong transaction
            if(0 != (REG_READ_8BIT(ENDPOINT0_CONTROL_REG)&ENDPOINT_SEND_STALL_BIT))
                REG_WRITE_8BIT(ENDPOINT0_CONTROL_REG, REG_READ_8BIT(ENDPOINT0_CONTROL_REG)&~ENDPOINT_SEND_STALL_BIT); 
        }
        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, SC_TRANS_DONE_BIT);  

        if(0 != (REG_READ_8BIT(ENDPOINT0_CONTROL_REG)& ENDPOINT_READY_BIT)) //only for EP0 Done
            return;

        if(USB_TRANSFER_STAGE_STATUS_OUT == gptUSBControl->u8TransferStage)
        {
            USB_DBG_CMD(USB_DBG_CODE_ISR_STATUS_OUT_RETURN); 
            USB_DBG_ISR_ENTER();
            if(!(usb_slave_check_transaction_type(USB_TRANSACTION_OUT, &bIsSetup)))
            {              
                if(bIsSetup)
                {
                    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;
                    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
                }
                
                #ifdef __HID_FUSION_FUN__ // {
                if(USB_IS_AUTOREADY())
                    hal_resume_task_in_isr(tthi_HID);
                #endif  // } __HID_FUSION_FUN__
                //printf("ISR Error OUT %X %X\r\n",REG_READ_8BIT(ENDPOINT0_CONTROL_REG),REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG));

                //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
                usb_isr_service_flag();
                //#endif
                
                return;
            }            
            USB_DBG_ISR_LEAVE();
        }
        else if(USB_TRANSFER_STAGE_STATUS_IN == gptUSBControl->u8TransferStage)
        {
            USB_DBG_CMD(USB_DBG_CODE_ISR_STATUS_IN_RETURN);  
            USB_DBG_ISR_ENTER();
            
            if(!usb_slave_check_transaction_type(USB_TRANSACTION_IN, &bIsSetup))
            {         
                if(bIsSetup)
                {
                    gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;   
                    USB_DBG_STAGE(gptUSBControl->u8TransferStage);
                }
                //printf("ISR Error IN %X T %X %X  st %X\r\n", REG_READ_8BIT(ENDPOINT0_CONTROL_REG),REG_READ_8BIT(ENDPOINT0_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_NAK_TRANSTYPE_STATUS_REG),REG_READ_8BIT(ENDPOINT0_STATUS_REG));

                #ifdef __HID_FUSION_FUN__ // {
                if(USB_IS_AUTOREADY())
                hal_resume_task_in_isr(tthi_HID);
                #endif  // } __HID_FUSION_FUN__

                //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
                usb_isr_service_flag();
                //#endif
                
                return;
            }
            USB_DBG_ISR_LEAVE();
            if(gptUSBControl->u8Addr != 0)
            {
                REG_WRITE_8BIT(SC_ADDRESS,gptUSBControl->u8Addr);        
                gptUSBControl->u8Addr = 0;
            }
        }
        else
        {
            USB_DBG_CMD(USB_DBG_CODE_ISR_NEXT_NOT_SETUP);  
            if((USB_TRANSFER_STAGE_SETUP == gptUSBControl->u8TransferStage && usb_slave_check_rx_status())
                || USB_TRANSFER_STAGE_SETUP != gptUSBControl->u8TransferStage)
            {
                USB_DBG_ISR_ENTER();
                if(USB_IS_AUTOREADY())
                {
                    //if(USB_TRANSFER_STAGE_SETUP != gptUSBControl->u8TransferStage)
                    //    hal_suspend_task(tthi_HID);
                }
                #ifdef __HID_FUSION_FUN__ // {
                hal_resume_task_in_isr(tthi_HID);
                #endif
                
                //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
                usb_isr_service_flag();
                //#endif
                
                USB_DBG_ISR_LEAVE();
                
                return;
            }
        }
        usb_slave_set_ep0_ready();
        gptUSBControl->u8TransferStage = USB_TRANSFER_STAGE_SETUP;          
        USB_DBG_STAGE(gptUSBControl->u8TransferStage);
        USB_DBG_CMD(USB_DBG_CODE_ISR_DONE);   
    }  

}

//****************************************************************************************
// Function name: do_usb_slave_hid_init()
//
// Description:
//   do_usb_slave_hid_init.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L do_usb_slave_hid_init(void)
{
    if(USB_SLAVE_INIT_STEP == USB_SLAVE_INIT_STEP_READY)
    {
        usb_slave_hid_init();
        //usb_slave_hid_data_set(u8USBDataBuf[REPORT_IN], 0);
    }
}

//****************************************************************************************
// Function name: usb_slave_init()
//
// Description:
//   usb_slave_init.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
void USB_CODE_L usb_slave_init(void)
{
    // For it8390 series
    GPCRH5 = 0x86;
    GPCRH6 = 0x86;

    //Disable USB debug path, prevent CPU enter JTAG mode and then reset by USB command
    MCCR &= ~BIT7; 

    // Set PLL 96MHz. Make sure timer ISR was enabled before doing this.
    cpu_clock_select_ext(CPU_CLOCK_96MHz);
    SCDCR0 = 0x51; // CPU: 48MHz
    
    //TODO: rename it in chip_chipregs.h
    ECReg(EC_Register_Base_Address+0x1e21) = 0x01;     // cali time sel
//	    ECReg(EC_Register_Base_Address+0x1e25) = 0x69;     // ref count15-8
//	    ECReg(EC_Register_Base_Address+0x1e26) = 0x78;     // ref count7-0
    ECReg(EC_Register_Base_Address+0x1e20) = 0x9A;     // unlock & sw control
    ECReg(EC_Register_Base_Address+0x1e55) = 0x03;     // LCVCO PD disable
    ECReg(EC_Register_Base_Address+0x1e57) = 0x02;     // turn-on lcvco ldo
//	    ECReg(EC_Register_Base_Address+0x1e50) = 0x2E;     // PTI = 'h2E
//	    ECReg(EC_Register_Base_Address+0x1e51) = 0x27;     // CTI = 'h27
//	    ECReg(EC_Register_Base_Address+0x1e54) = 0x2B;     // SC = 'h2, LCPDV = 'hB
    ECReg(EC_Register_Base_Address+0x1e54) |= 0x20;     // SC = 'h2, LCPDV = 'hB
    ECReg(EC_Register_Base_Address+0x1e40) = 0x03;     // auto cali enable
	
    // Enable USB pad power-on
    PMER2  |= BIT7;    
    
    WUEMR9 &= ~BIT1;      // Rising triggle
    WUESR9 = BIT1;      // clear interrupt      
    WUENR9 |= BIT1;      // Enable WUI         

    WUEMR9 |= BIT2;      // Falling triggle  , for reset wake up
    WUESR9 = BIT2;      // clear interrupt      
    WUENR9 |= BIT2;      // Enable WUI       
    
    gptUSBControl = &gtUSBControl;
    usb_slave_sw_init(FALSE);
    usb_slave_ip_init(0); 
    do_usb_slave_hid_init();
}

//****************************************************************************************
// Function name: usb_slave_is_busy()
//
// Description:
//   usb_slave_is_busy.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_is_busy(void)
{
    if((0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_TRANS_DONE_BIT))
        || (0 != (REG_READ_8BIT(SC_INTERRUPT_STATUS_REG) & SC_RESET_EVENT_BIT)))
        return TRUE;
    
    #ifdef __HID_FUSION_FUN__ // {
    if(!hal_thread_task_Is_suspended(tthi_HID))
        return TRUE;
    #endif
    
    //if(USB_TRANSFER_STAGE_SETUP != gptUSBControl->u8TransferStage)
    //    return TRUE;
    
    return FALSE;
}

//****************************************************************************************
// Function name: usb_slave_resume()
//
// Description:
//   usb_slave_resume.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
//****************************************************************************************
BOOL USB_CODE usb_slave_resume(UINT8 u8Flag)
{
    /*if(USB_RESUME_RESET == u8Flag)
    {
        if((GPDRH & BIT5) != 0 || (GPDRH & BIT6) != 0) // reset
            return FALSE;   
    }
    else */if(USB_RESUME_RESUME == u8Flag)
    {
        if((GPDRH & BIT6) != 0) // evb floating
            return FALSE; 
    }
    else if(USB_RESUME_REMOTEWAKEUP == u8Flag)
    {
        if(gbUSBResumeSet)
            return FALSE;
    }
    
    gu8USBResumeFlag = u8Flag;
    gbUSBResumeSet = TRUE;
    gbUSBNoSuspend = TRUE;
    
    
    #ifdef __HID_FUSION_FUN__ // {
    hal_resume_task_in_isr(tthi_HID); 
    #endif

    //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    usb_isr_service_flag();
    
    /*if(USB_RESUME_RESET == gu8USBResumeFlag)
    {
        usb_slave_sw_init(FALSE);
        REG_WRITE_8BIT(SC_ADDRESS, 0);     

        REG_WRITE_8BIT(SC_INTERRUPT_STATUS_REG, REG_READ_8BIT(SC_INTERRUPT_STATUS_REG));        

        gbUSBSuspended = FALSE;
        IER2 |= BIT7;

        USB_DBG_ERROR_CLEAR(USB_DBG_ERROR_SUSPEND_BIT);     
        USB_DBG_RESUME_LEAVE();
        USB_SLAVE_INIT_STEP = USB_SLAVE_INIT_STEP_IDLE;           
        gbUSBResumeSet = FALSE;

    }*/
    
    return TRUE;
    //#endif
}


#ifndef __USB_REG_H__
#define __USB_REG_H__


/******************************************************************************
Define
Description: Define USB Host Slave Common Registers
Footnote:
******************************************************************************/
//#define __CX_DMA__


#define	    USB_BASE_ADDR		        (REG_BASE_ADDR + 0x2F00) // For  IC
//#define	USB_BASE_ADDR		0xF400		// For FPGA

#define		HOST_SLAVE_CONTROL_REG		(USB_BASE_ADDR + 0xE0)
#define		HOST_SLAVE_VERSION_REG      (USB_BASE_ADDR + 0xE1)
#define		PME_WAKE_UP_REG             (USB_BASE_ADDR + 0xE2)
#define		RX_RESUME_DETECTION_TIMER   (USB_BASE_ADDR + 0xE3)

#define		PORT0_MISC_CONTROL_REGISTER (USB_BASE_ADDR + 0xE4)
#define		PORT0_MISC_STATUS_REGISTER  (USB_BASE_ADDR + 0xE6)
#define		PORT1_MISC_CONTROL_REGISTER (USB_BASE_ADDR + 0xE8)
#define		PORT1_MISC_STATUS_REGISTER  (USB_BASE_ADDR + 0xEA)
#define		SUSB_CONTROL_REGISTER  	    (USB_BASE_ADDR + 0xEC)
#define		SUSB_STATUS_REGISTER  	    (USB_BASE_ADDR + 0xED)

#define		HOST_TX_CONTROL_REG         (USB_BASE_ADDR + 0x00)
#define		HOST_TX_TRANS_TYPE_REG      (USB_BASE_ADDR + 0x01)
#define		HOST_TX_LINE_CONTROL_REG    (USB_BASE_ADDR + 0x02)
#define		HOST_TX_SOF_ENABLE_REG      (USB_BASE_ADDR + 0x03)
#define		HOST_TX_ADDR_REG            (USB_BASE_ADDR + 0x04)
#define		HOST_TX_ENDP_REG            (USB_BASE_ADDR + 0x05)
#define		HOST_FRAME_NUM_MSP_REG      (USB_BASE_ADDR + 0x06)
#define		HOST_FRAME_NUM_LSP_REG      (USB_BASE_ADDR + 0x07)
#define		HOST_INTERRUPT_STATUS_REG   (USB_BASE_ADDR + 0x08)
#define		HOST_INTERRUPT_MASK_REG     (USB_BASE_ADDR + 0x09)
#define		HOST_RX_STATUS_REG          (USB_BASE_ADDR + 0x0A)
#define		HOST_RX_PID_REG             (USB_BASE_ADDR + 0x0B)
#define		MISC_CONTROL_REG            (USB_BASE_ADDR + 0x0C)
#define		MISC_STATUS_REG             (USB_BASE_ADDR + 0x0D)
#define		HOST_RX_CONNECT_STATE_REG   (USB_BASE_ADDR + 0x0E)
#define		HOST_SOF_TIMER_MSB_REG      (USB_BASE_ADDR + 0x0F)

#define		HOST_RX_FIFO_DATA           (USB_BASE_ADDR + 0x20)
#define		HOST_RX_FIFO_DATA_COUNT     (USB_BASE_ADDR + 0x21)
#define		HOST_RX_FIFO_DATA_COUNT_MSB (USB_BASE_ADDR + 0x22)
#define		HOST_RX_FIFO_DATA_COUNT_LSB (USB_BASE_ADDR + 0x23)
#define		HOST_RX_FIFO_CONTROL_REG    (USB_BASE_ADDR + 0x24)

#define		USB_DMA_CTR_REG             (USB_BASE_ADDR + 0xF0)

#ifdef __CX_DMA__

#define		USB_DMA_TX_COUNT_REG        (USB_BASE_ADDR + 0xF1)
#define		USB_DMA_INTERRUPT_MASK_REG  (USB_BASE_ADDR + 0xF2)
#define		USB_DMA_INTERRUPT_STAT_REG  (USB_BASE_ADDR + 0xF3)
#define		USB_DMA_ADDRESS_7_0_REG     (USB_BASE_ADDR + 0xF4)
#define		USB_DMA_ADDRESS_15_8_REG    (USB_BASE_ADDR + 0xF5)
#define		USB_DMA_ADDRESS_23_16_REG   (USB_BASE_ADDR + 0xF6)
#define		USB_DMA_ADDRESS_31_24_REG   (USB_BASE_ADDR + 0xF7)
#define		USB_DMA_STATUS_REG          (USB_BASE_ADDR + 0xF8)

#define		USB_DMA_CTR_RB_REG              (USB_BASE_ADDR + 0xF9)
#define		USB_DMA_TX_COUNT_RB_REG         (USB_BASE_ADDR + 0xFA)
#define		USB_DMA_ADDRESS_7_0_RB_REG      (USB_BASE_ADDR + 0xFC)
#define		USB_DMA_ADDRESS_15_8_RB_REG     (USB_BASE_ADDR + 0xFD)
#define		USB_DMA_ADDRESS_23_16_RB_REG    (USB_BASE_ADDR + 0xFE)
#define		USB_DMA_ADDRESS_31_24_RB_REG    (USB_BASE_ADDR + 0xFF)

//#define		USB_DMA_ADD_L_REG           (USB_BASE_ADDR + 0xF4)
//#define		USB_DMA_ADD_H_REG           (USB_BASE_ADDR + 0xF5)

#else

#define		USB_DMA_ADD_L_REG           (USB_BASE_ADDR + 0xF1)
#define		USB_DMA_ADD_H_REG           (USB_BASE_ADDR + 0xF2)
#define		USB_DMA_TX_COUNT_REG        (USB_BASE_ADDR + 0xF3)

#endif

#define		HOST_TX_FIFO_DATA           (USB_BASE_ADDR + 0x30)

/******************************************************************************
Define
Description: Define USB Slave Common Registers
Footnote:
******************************************************************************/
#define	    USBS_BASE_ADDR		                    (REG_BASE_ADDR + 0x2F00)//(REG_BASE_ADDR + 0xF500)

#define		ENDPOINT0_CONTROL_REG					(USBS_BASE_ADDR + 0x40)
#define		ENDPOINT0_STATUS_REG					(USBS_BASE_ADDR + 0x41)
#define		ENDPOINT0_TRANSTYPE_STATUS_REG			(USBS_BASE_ADDR + 0x42)
#define		ENDPOINT0_NAK_TRANSTYPE_STATUS_REG		(USBS_BASE_ADDR + 0x43)

#define		ENDPOINT1_CONTROL_REG					(USBS_BASE_ADDR + 0x44)
#define		ENDPOINT1_STATUS_REG					(USBS_BASE_ADDR + 0x45)
#define		ENDPOINT1_TRANSTYPE_STATUS_REG			(USBS_BASE_ADDR + 0x46)
#define		ENDPOINT1_NAK_TRANSTYPE_STATUS_REG		(USBS_BASE_ADDR + 0x47)

#define		ENDPOINT2_CONTROL_REG					(USBS_BASE_ADDR + 0x48)
#define		ENDPOINT2_STATUS_REG					(USBS_BASE_ADDR + 0x49)
#define		ENDPOINT2_TRANSTYPE_STATUS_REG			(USBS_BASE_ADDR + 0x4A)
#define		ENDPOINT2_NAK_TRANSTYPE_STATUS_REG		(USBS_BASE_ADDR + 0x4B)

#define		ENDPOINT3_CONTROL_REG					(USBS_BASE_ADDR + 0x4C)
#define		ENDPOINT3_STATUS_REG					(USBS_BASE_ADDR + 0x4D)
#define		ENDPOINT3_TRANSTYPE_STATUS_REG			(USBS_BASE_ADDR + 0x4E)
#define		ENDPOINT3_NAK_TRANSTYPE_STATUS_REG		(USBS_BASE_ADDR + 0x4F)

#define		SC_CONTROL_REG							(USBS_BASE_ADDR + 0x50)
#define		SC_LINE_STATUS_REG						(USBS_BASE_ADDR + 0x51)
#define		SC_INTERRUPT_STATUS_REG					(USBS_BASE_ADDR + 0x52)
#define		SC_INTERRUPT_MASK_REG					(USBS_BASE_ADDR + 0x53)
#define		SC_ADDRESS								(USBS_BASE_ADDR + 0x54)
#define		SC_FRAME_NUM_MSP						(USBS_BASE_ADDR + 0x55)
#define		SC_FRAME_NUM_LSP						(USBS_BASE_ADDR + 0x56)

#define		EP0_RX_FIFO_DATA						(USBS_BASE_ADDR + 0x60)
#define		EP0_RX_FIFO_DMA_DATA_COUNT			    (USBS_BASE_ADDR + 0x61)
#define		EP0_RX_FIFO_DATA_COUNT_MSB				(USBS_BASE_ADDR + 0x62)
#define		EP0_RX_FIFO_DATA_COUNT_LSB				(USBS_BASE_ADDR + 0x63)
#define		EP0_RX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0x64)
#define		EP0_TX_FIFO_DATA						(USBS_BASE_ADDR + 0x70)
#define		EP0_TX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0x74)

#define		EP1_RX_FIFO_DATA						(USBS_BASE_ADDR + 0x80)
#define		EP1_RX_FIFO_DATA_COUNT_MSB				(USBS_BASE_ADDR + 0x82)
#define		EP1_RX_FIFO_DATA_COUNT_LSB				(USBS_BASE_ADDR + 0x83)
#define		EP1_RX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0x84)
#define		EP1_TX_FIFO_DATA						(USBS_BASE_ADDR + 0x90)
#define		EP1_TX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0x94)


#define		EP2_RX_FIFO_DATA						(USBS_BASE_ADDR + 0xA0)
#define		EP2_RX_FIFO_DATA_COUNT_MSB				(USBS_BASE_ADDR + 0xA2)
#define		EP2_RX_FIFO_DATA_COUNT_LSB				(USBS_BASE_ADDR + 0xA3)
#define		EP2_RX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0xA4)
#define		EP2_TX_FIFO_DATA						(USBS_BASE_ADDR + 0xB0)
#define		EP2_TX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0xB4)

#define		EP3_RX_FIFO_DATA						(USBS_BASE_ADDR + 0xC0)
#define		EP3_RX_FIFO_DATA_COUNT_MSB				(USBS_BASE_ADDR + 0xC2)
#define		EP3_RX_FIFO_DATA_COUNT_LSB				(USBS_BASE_ADDR + 0xC3)
#define		EP3_RX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0xC4)
#define		EP3_TX_FIFO_DATA						(USBS_BASE_ADDR + 0xD0)
#define		EP3_TX_FIFO_CONTROL_REG					(USBS_BASE_ADDR + 0xD4)


/******************************************************************************
Define
Description: Define USB Slave Bit
Footnote:
******************************************************************************/    

//ENDPOINT[3..0]_STATUS_REG
#define		SC_CRC_ERROR_BIT					BIT0
#define		SC_BIT_STUFF_ERROR_BIT				BIT1
#define		SC_RX_OVERFLOW_BIT					BIT2
#define		SC_RX_TIME_OUT_BIT					BIT3
#define		SC_NAK_SENT_BIT					    BIT4
#define		SC_STALL_SENT_BIT					BIT5
#define		SC_ACK_RXED_BIT 					BIT6
#define		SC_DATA_SEQUENCE_BIT				BIT7

//SC_INTERRUPT_STATUS_REG
#define		SC_TRANS_DONE_BIT					BIT0
#define		SC_RESUME_INT_BIT				    BIT1
#define		SC_RESET_EVENT_BIT					BIT2
#define		SC_SOF_RECEIVED_BIT					BIT3
#define		SC_NAK_SENT_INT_BIT				    BIT4
#define		VBUS_DET_INT_BIT					BIT5

//SC_CONTROL_REG
#define		SC_GLOBAL_ENABLE_BIT				BIT0
#define		SC_TX_LINE_STATE_DM				    BIT1
#define		SC_TX_LINE_STATE_DP					BIT2
#define		SC_DIRECT_CONTROL_BIT				BIT3
#define		SC_FULL_SPEED_LINE_POLARITY_BIT		BIT4
#define		SC_FULL_SPEED_LINE_RATE_BIT			BIT5
#define		SC_CONNECT_TO_HOST_BIT  			BIT6 // internal pull-up

//ENDPOINT[3..0]_CONTROL_REG
#define		ENDPOINT_ENABLE_BIT					BIT0
#define		ENDPOINT_READY_BIT				    BIT1
#define		ENDPOINT_OUTDATA_SEQUENCE_BIT		BIT2
#define		ENDPOINT_SEND_STALL_BIT			    BIT3
#define		ENDPOINT_ISO_ENABLE_BIT				BIT4
#define		ENDPOINT_DIRECTION_BIT				BIT5



//USB DMA 
#ifdef __CX_DMA__ // { __CX_DMA__

#define		USB_DMA_CTRL_ENALBE 				BIT0
#define		USB_DMA_CTRL_DIRECTION			    BIT1 
//1: DMA Write. Data from USB RX FIFO to internal SRAM. 
//0: DMA Read. Data from internal SRAM to USB TX FIFO.
#define		USB_DMA_CTRL_SET			        BIT4 

#define		USB_DMA_CTRL_EP_NUM_SHIFT		    2
#define		USB_DMA_CTRL_EP_NUM_MASK		    0x0C

#define		USB_DMA_INT_EP0_HOST_DONE			BIT0
#define		USB_DMA_INT_EP1_DONE 				BIT1
#define		USB_DMA_INT_EP2_DONE 				BIT2
#define		USB_DMA_INT_EP3_DONE 				BIT3
#define		USB_DMA_INT_EP0_HOST_ERROR			BIT4
#define		USB_DMA_INT_EP1_ERROR 				BIT5
#define		USB_DMA_INT_EP2_ERROR 				BIT6
#define		USB_DMA_INT_EP3_ERROR 				BIT7

#define		USB_DMA_STATUS_EP0_DIR_ERROR		BIT0
#define		USB_DMA_STATUS_EP1_DIR_ERROR 		BIT1
#define		USB_DMA_STATUS_EP2_DIR_ERROR 		BIT2
#define		USB_DMA_STATUS_EP3_DIR_ERROR 		BIT3
#endif // } __CX_DMA__

/******************************************************************************
Define
Description: Define USB GPIO output registers
Footnote:
******************************************************************************/
#ifdef IC_VER
#define		UC_CCONFIG_BASE_ADDR    (REG_BASE_ADDR + 0x2900)

#define		LOGICAL_DEV_NUM			(UC_CCONFIG_BASE_ADDR + 0x07)    //Logical Device Number (LDN)	
#define		GPIO_6x_OUTPUT_EN		(UC_CCONFIG_BASE_ADDR + 0xCD)    
#define		GPIO_BASE_ADDR_MSB		(UC_CCONFIG_BASE_ADDR + 0x62)    
#define		GPIO_BASE_ADDR_LSB		(UC_CCONFIG_BASE_ADDR + 0x63)    
#define 	GPIO_6x_FUN_SELECT		(UC_CCONFIG_BASE_ADDR + 0x29)    
#define		GPIO_DATA_BASE				0x2C00							// simple I/O can be accessed by uc, and base address is 2C00h
#define		GPIO_6x_DATA_BASE			0x2C05
#else
#define    GPIO_BASE_ADDR   (REG_BASE_ADDR + 0xF000)

#define    USB_0x		    (GPIO_BASE_ADDR + 0x00)    //
#define    USB_1x           (GPIO_BASE_ADDR + 0x01)    //
#define    USB_2x           (GPIO_BASE_ADDR + 0x02)    //
#define    USB_9x           (GPIO_BASE_ADDR + 0x09)
#endif

#define	   EC_2907     (REG_BASE_ADDR + 0x2907)
#define	   EC_292A     (REG_BASE_ADDR + 0x292A)
#define	   EC_292D     (REG_BASE_ADDR + 0x292D)
#define	   EC_2930     (REG_BASE_ADDR + 0x2930)
#define	   EC_29F0     (REG_BASE_ADDR + 0x29F0)
#define	   EC_2A5A     (REG_BASE_ADDR + 0x2A5A)
#define	   EC_2A5C     (REG_BASE_ADDR + 0x2A5C)


#define REG_WRITE_8BIT(Addr, Data)      *(volatile unsigned char*)(Addr) = (Data)

#define REG_READ_8BIT(Addr)             (*(volatile unsigned char*)(Addr))



#endif //__USB_REG_H__


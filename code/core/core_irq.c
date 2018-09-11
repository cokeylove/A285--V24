/* 
 * ****************************************************************************
 * core_irq.c
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
 * Input full irq lock scan
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IrqLock_scan(void)
{
	Int_Var.Scan_Lock = 1;
}

/**
 * ****************************************************************************
 * Null function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT0_Null(void)
{

}

/**
 * ****************************************************************************
 * WKO[20]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT1_WKO20(void)
{
    Hook_IRQ_INT1_WKO20();
}

/**
 * ****************************************************************************
 * KBC Output Buffer Empty Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT2_KBCOBE(void)
{
    Hook_IRQ_INT2_KBCOBE();
}

/**
 * ****************************************************************************
 * PMC Output Buffer Empty Intr. PMC1 Output Buffer Empty Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT3_PMCPMC1OBE(void)
{
    Hook_IRQ_INT3_PMCPMC1OBE();
}

/**
 * ****************************************************************************
 * SMBus D Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT4_SMBusD(void)
{
    /* rtos only */
    /* Disable ISR */
    #ifndef EC_FEATURE_ONLY
        #ifdef  __HID_FUSION_FUN__
        hal_SMbusD_irq();
        #endif /* __HID_FUSION_FUN__ */
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_SMbusD_irq();
        #endif
    #endif /* EC_FEATURE_ONLY */

    Hook_IRQ_INT4_SMBusD();
}

/**
 * ****************************************************************************
 * WKINTAD (WKINTA or WKINTD)
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT5_WKINTAD(void)
{
    #ifndef EC_FEATURE_ONLY
    hal_WUI_SmbE_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_SmbE_irq();
        #endif
    #endif
    
    Hook_IRQ_INT5_WKINTAD();
}

/**
 * ****************************************************************************
 * WKO[23]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT6_WKO23(void)
{
    Hook_IRQ_INT6_WKO23();
}

/**
 * ****************************************************************************
 * PWM Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT7_PWM(void)
{
    Hook_IRQ_INT7_PWM();
}

/**
 * ****************************************************************************
 * ADC Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT8_ADC(void)
{
    Hook_IRQ_INT8_ADC();
}

/**
 * ****************************************************************************
 * SMBus A Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT9_SMBusA(void)
{
    Hook_IRQ_INT9_SMBusA();
}

/**
 * ****************************************************************************
 * SMBus B Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT10_SMBusB(void)
{
    Hook_IRQ_INT10_SMBusB();
}

/**
 * ****************************************************************************
 * KB Matrix Scan Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT11_KBMatrixScan(void)
{
    /* Firmware scan keyboard */
    #ifndef KB_HW_KeyScan
	CLEAR_MASK(IER1,Int_KB);
    ISR1 = Int_KB;

	/* Post service request to scan internal keyboard. */
	F_Service_KEY = 1;

    Hook_IRQ_INT11_KBMatrixScan();
    #endif /* #ifndef KB_HW_KeyScan #endif*/
}

/**
 * ****************************************************************************
 * WKO[26]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT12_WKO26(void)
{
    Hook_IRQ_INT12_WKO26();
}

/**
 * ****************************************************************************
 * WKINTC
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT13_WKINTC(void)
{
    Hook_IRQ_INT13_WKINTC();
}

/**
 * ****************************************************************************
 * WKO[25]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT14_WKO25(void)
{
    Hook_IRQ_INT14_WKO25();
}

/**
 * ****************************************************************************
 * CIR Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT15_CIR(void)
{
    Hook_IRQ_INT15_CIR();
}

/**
 * ****************************************************************************
 * SMBus C Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT16_SMBusC(void)
{
    Hook_IRQ_INT16_SMBusC();
}

/**
 * ****************************************************************************
 * WKO[24]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT17_WKO24(void)
{
    Hook_IRQ_INT17_WKO24();
}

/**
 * ****************************************************************************
 * PS/2 Interrupt port 2
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT18_PS2Interrupt2(void)
{
    #if TouchPad_only
    /* Disable PS2 interrupt 2 */ 
	CLEAR_MASK(IER2,Int_PS2_2);
    /* Write to clear pending interrupt */
    ISR2 = Int_PS2_2;
    
    if(SendtoAUXFlag)
    {
        for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
            PS2DataPinStatusCounter++)
        {
            /* DATA Line Status is high */
            if(IS_MASK_SET(GPDRF, BIT5))
            {
                break;
            }
            else
            {
                /* Delay 15.26 us */
                WNCKR = 0x00;
            }
        }
    }

    /* Inhibit clock pin1 */
	PSCTL1 = PS2_InhibitMode;
    /* Inhibit clock pin2 */
	PSCTL2 = PS2_InhibitMode;
    /* Inhibit clock pin3 */
	PSCTL3 = PS2_InhibitMode;

    /* Set PS2 2 service flag */
	PS2_IRQ_Channel = 2;
    /* Set main service flag */
	F_Service_PS2 = 1;
    /* clear start bit flag */
	PS2StartBit=0;
    PS2_SSIRQ_Channel = 0xFF;
    PS2PortxData[2]=PSDAT3;
    #else
    /* Transaction done interrupt */
	if(IS_MASK_SET(PSSTS3, TDS))
	{
        /* Disable PS2 interrupt 2 */
		CLEAR_MASK(IER2,Int_PS2_2);
        /* Write to clear pending interrupt */
	    ISR2 = Int_PS2_2;
	    
	    if(SendtoAUXFlag)
	    {
            for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
                PS2DataPinStatusCounter++)
            {
                /* DATA Line Status is high */
                if(IS_MASK_SET(PSSTS3, DLS))
                {
                    break;
                }
                else
                {
                    /* Delay 15.26 us */
                    WNCKR = 0x00;
                }
            }
        }
        else
        {

        }

        /* Inhibit clock pin1 */
		PSCTL1 = PS2_InhibitMode;
        /* Inhibit clock pin2 */
		PSCTL2 = PS2_InhibitMode;
        /* Inhibit clock pin3 */
		PSCTL3 = PS2_InhibitMode;

        /* Set PS2 2 service flag */
		PS2_IRQ_Channel = 2;
        /* Set main service flag */
		F_Service_PS2 = 1;
        /* clear start bit flag */
		PS2StartBit=0;
	    PS2_SSIRQ_Channel = 0xFF;
	    PS2PortxData[2]=PSDAT3;
	}
    else    /* Start bit interrupt */
    {
        /* Inhibit clock pin1 */
        PSCTL1 = PS2_InhibitMode;
        /* Inhibit clock pin2 */
        PSCTL2 = PS2_InhibitMode;
        /* Disable all PS2 interrupt */
        IER2&= ~(Int_PS2_0+Int_PS2_1+Int_PS2_2);
        /* Write to clear all PS2 pending interrupt */
        ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
	    /* Set start bit flag */
        PS2StartBit = 1;
        PS2_SSIRQ_Channel = 2;
        /* Enable PS2 interrupt 2 */
        SET_MASK(IER2,Int_PS2_2);
    }	
    #endif
    Hook_IRQ_INT18_PS2Interrupt2();
}

/**
 * ****************************************************************************
 * PS/2 Interrupt port 1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT19_PS2Interrupt1(void)
{
    #if TouchPad_only
    /* Disable PS2 interrupt 1 */
    CLEAR_MASK(IER2,Int_PS2_1);
    /* Write to clear pending interrupt */
    ISR2 = Int_PS2_1;
    
    if(SendtoAUXFlag)
    {
        for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
            PS2DataPinStatusCounter++)
        {
            /* DATA Line Status is high */
            if(IS_MASK_SET(GPDRF, BIT3))
            {
                break;
            }
            else
            {
                /* Delay 15.26 us */
                WNCKR = 0x00;
            }
        }
    }

    /* Inhibit clock pin1 */
	PSCTL1 = PS2_InhibitMode;
    /* Inhibit clock pin2 */
	PSCTL2 = PS2_InhibitMode;
    /* Inhibit clock pin3 */
	PSCTL3 = PS2_InhibitMode;

    /* Set PS2 1 service flag */
    PS2_IRQ_Channel = 1;
    /* Set main service flag */
	F_Service_PS2 = 1;
    /* clear start bit flag */
    PS2StartBit=0;
    PS2_SSIRQ_Channel = 0xFF;
    PS2PortxData[1]=PSDAT2;
    #else
    /* Transaction done interrupt */
	if(IS_MASK_SET(PSSTS2, TDS))
	{
        /* Disable PS2 interrupt 1 */
		CLEAR_MASK(IER2,Int_PS2_1);
        /* Write to clear pending interrupt */
	    ISR2 = Int_PS2_1;
	    
	    if(SendtoAUXFlag)
	    {
            for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
                PS2DataPinStatusCounter++)
            {
                /* DATA Line Status is high */
                if(IS_MASK_SET(PSSTS2, DLS))
                {
                    break;
                }
                else
                {
                    /* Delay 15.26 us */
                    WNCKR = 0x00;
                }
            }
        }
        else
        {

        }

        /* Inhibit clock pin1 */
		PSCTL1 = PS2_InhibitMode;
        /* Inhibit clock pin2 */
		PSCTL2 = PS2_InhibitMode;
        /* Inhibit clock pin3 */
		PSCTL3 = PS2_InhibitMode;

        /* Set PS2 1 service flag */
	    PS2_IRQ_Channel = 1;
        /* Set main service flag */
		F_Service_PS2 = 1;
	    /* clear start bit flag */
        PS2StartBit=0;
        PS2_SSIRQ_Channel = 0xFF;
	    PS2PortxData[1]=PSDAT2;
	}
    else    /* Start bit interrupt */
    {
        /* Inhibit clock pin1 */
        PSCTL1 = PS2_InhibitMode;
        /* Inhibit clock pin3 */
        PSCTL3 = PS2_InhibitMode;
        /* Disable all PS2 interrupt */
        IER2&= ~(Int_PS2_0+Int_PS2_1+Int_PS2_2);
        /* Write to clear all PS2 pending interrupt */
        ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
	    /* Set start bit flag */
        PS2StartBit = 1;
        PS2_SSIRQ_Channel = 1;
        /* Enable PS2 interrupt 1 */
        SET_MASK(IER2,Int_PS2_1);
    }	
    #endif
    Hook_IRQ_INT19_PS2Interrupt1();
}

/**
 * ****************************************************************************
 * PS/2 Interrupt port 0
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT20_PS2Interrupt0(void)
{
    #if TouchPad_only
    /* Disable PS2 interrupt 0 */
    CLEAR_MASK(IER2,Int_PS2_0);
    /* Write to clear pending interrupt */
    ISR2 = Int_PS2_0;

    if(SendtoAUXFlag)
    {
        for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
            PS2DataPinStatusCounter++)
        {
            /* DATA Line Status is high */
            if(IS_MASK_SET(GPDRF, BIT1))
            {
                break;
            }
            else
            {
                /* Delay 15.26 us */
                WNCKR = 0x00;
            }
        }
    }

    /* Inhibit clock pin1 */
	PSCTL1 = PS2_InhibitMode;
    /* Inhibit clock pin2 */
	PSCTL2 = PS2_InhibitMode;
    /* Inhibit clock pin3 */
	PSCTL3 = PS2_InhibitMode;

    /* Set PS2 0 service flag */
    PS2_IRQ_Channel = 0;
    /* Set main service flag */
	F_Service_PS2 = 1;
    /* clear start bit flag */
    PS2StartBit=0;
    PS2_SSIRQ_Channel = 0xFF;
	PS2PortxData[0]=PSDAT1;
    #else
    /* Transaction done interrupt */
	if(IS_MASK_SET(PSSTS1, TDS))
	{
        /* Disable PS2 interrupt 0 */
        CLEAR_MASK(IER2,Int_PS2_0);
        /* Write to clear pending interrupt */
	    ISR2 = Int_PS2_0;

        if(SendtoAUXFlag)
	    {
            for(PS2DataPinStatusCounter=0x00;PS2DataPinStatusCounter<5;
                PS2DataPinStatusCounter++)
            {
                /* DATA Line Status is high */
                if(IS_MASK_SET(PSSTS1, DLS))
                {
                    break;
                }
                else
                {
                    /* Delay 15.26 us */
                    WNCKR = 0x00;
                }
            }
        }
        else
        {

        }

        /* Inhibit clock pin1 */
		PSCTL1 = PS2_InhibitMode;
        /* Inhibit clock pin2 */
		PSCTL2 = PS2_InhibitMode;
        /* Inhibit clock pin3 */
		PSCTL3 = PS2_InhibitMode;

        /* Set PS2 0 service flag */
	    PS2_IRQ_Channel = 0;
        /* Set main service flag */
		F_Service_PS2 = 1;
	    /* clear start bit flag */
	    PS2StartBit=0;
	    PS2_SSIRQ_Channel = 0xFF;
		PS2PortxData[0]=PSDAT1;
	}
    else    /* Start bit interrupt */
    {
        /* Inhibit clock pin2 */
        PSCTL2 = PS2_InhibitMode;
        /* Inhibit clock pin3 */
        PSCTL3 = PS2_InhibitMode;
        /* Disable all PS2 interrupt */
        IER2&= ~(Int_PS2_0+Int_PS2_1+Int_PS2_2);
        /* Write to clear all PS2 pending interrupt */
        ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
	    /* Set start bit flag */
        PS2StartBit = 1;
        PS2_SSIRQ_Channel = 0;
        /* Enable PS2 interrupt 0 */
        SET_MASK(IER2,Int_PS2_0);
    }	
    #endif
    Hook_IRQ_INT20_PS2Interrupt0();
}

/**
 * ****************************************************************************
 * WKO[22]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT21_WKO22(void)
{
    Hook_IRQ_INT21_WKO22();
}

/**
 * ****************************************************************************
 * SMFI Semaphore Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT22_SMFISemaphore(void)
{
    Hook_IRQ_INT22_SMFISemaphore();
}

/**
 * ****************************************************************************
 * USB Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT23_Null(void)
{
    /* rtos only */
    #ifndef EC_FEATURE_ONLY
        #ifdef  __HID_FUSION_FUN__
        hal_usb_irq();
        #endif /* __HID_FUSION_FUN__ */
    #else
        /* pure ec + sensor fusion feature */
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_usb_irq();
        #endif
    #endif /* EC_FEATURE_ONLY */

    Hook_IRQ_INT23_Null();
}

/**
 * ****************************************************************************
 * KBC Input Buffer Full Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT24_KBCIBF(void)
{
	IrqLock_scan();
	CLEAR_MASK(IER3,Int_KBCIBF);
    ISR3 = Int_KBCIBF;
    F_Service_PCI = 1;

    Hook_IRQ_INT24_KBCIBF();
}

/**
 * ****************************************************************************
 * PMC Input Buffer Full Interrupt PMC1 Input Buffer Full Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_CODE IRQ_INT25_PMCPMC1IBF(void)
{
	if(IS_MASK_SET(PM2STS, P_IBF))	/* 68/6C input buffer full */
	{
		F_Service_PCI3 = 1;
	}
	else						    /* 62/66 input buffer full */
	{
		F_Service_PCI2 = 1;
	}
	CLEAR_MASK(IER3,Int_PMCIBF);
    ISR3 = Int_PMCIBF;

    Hook_IRQ_INT25_PMCPMC1IBF();
}

/**
 * ****************************************************************************
 * PMC2 Output Buffer Empty Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT26_PMC2OBE(void)
{
    Hook_IRQ_INT26_PMC2OBE();
}

/**
 * ****************************************************************************
 * PMC2 Input Buffer Full Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT27_PMC2IBF(void)
{
    Hook_IRQ_INT27_PMC2IBF();
}

/**
 * ****************************************************************************
 * GINT from function 1 of GPD5
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT28_GINTofGPD5(void)
{
    Hook_IRQ_INT28_GINTofGPD5();
}

/**
 * ****************************************************************************
 * EGPC Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT29_EGPC(void)
{
    Hook_IRQ_INT29_EGPC();
}

/**
 * ****************************************************************************
 * External Timer 1 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT30_ET1(void)
{
    Hook_IRQ_INT30_ET1();
}

/**
 * ****************************************************************************
 * WKO[21]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT31_WKO21(void)
{
    Hook_IRQ_INT31_WKO21();
}

/**
 * ****************************************************************************
 * GPINT0
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT32_GPINT0(void)
{
    Hook_IRQ_INT32_GPINT0();
}

/**
 * ****************************************************************************
 * GPINT1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT33_GPINT1(void)
{
    Hook_IRQ_INT33_GPINT1();
}

/**
 * ****************************************************************************
 * GPINT2
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT34_GPINT2(void)
{
    Hook_IRQ_INT34_GPINT2();
}

/**
 * ****************************************************************************
 * GPINT3
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT35_GPINT3(void)
{
    Hook_IRQ_INT35_GPINT3();
}

/**
 * ****************************************************************************
 * CIR GPINT
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT36_CIRGPINT(void)
{
    Hook_IRQ_INT36_CIRGPINT();
}

/**
 * ****************************************************************************
 * SSPI Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT37_SSPI(void)
{
    Hook_IRQ_INT37_SSPI();
}

/**
 * ****************************************************************************
 * UART1 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT38_UART1(void)
{
    Hook_IRQ_INT38_UART1();
}

/**
 * ****************************************************************************
 * UART2 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT39_UART2(void)
{
    Hook_IRQ_INT39_UART2();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT40_WKO50(void)
{
    Hook_IRQ_INT40_WKO50();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT41_WKO51(void)
{
    Hook_IRQ_INT41_WKO51();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT42_WKO52(void)
{
    Hook_IRQ_INT42_WKO52();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT43_WKO53(void)
{
    Hook_IRQ_INT43_WKO53();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT44_WKO54(void)
{
    Hook_IRQ_INT44_WKO54();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT45_WKO55(void)
{
    Hook_IRQ_INT45_WKO55();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT46_WKO56(void)
{
    Hook_IRQ_INT46_WKO56();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT47_WKO57(void)
{
    Hook_IRQ_INT47_WKO57();
}

/**
 * ****************************************************************************
 * WKO[60]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT48_WKO60(void)
{
    Hook_IRQ_INT48_WKO60();
}

/**
 * ****************************************************************************
 * WKO[61]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT49_WKO61(void)
{
    Hook_IRQ_INT49_WKO61();
}

/**
 * ****************************************************************************
 * WKO[62]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT50_WKO62(void)
{
    #ifndef EC_FEATURE_ONLY
    hal_WUI_SmbD_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_SmbD_irq();
        #endif
    #endif
    
    Hook_IRQ_INT50_WKO62();
}

/**
 * ****************************************************************************
 * WKO[63]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT51_WKO63(void)
{
    Hook_IRQ_INT51_WKO63();
}

/**
 * ****************************************************************************
 * WKO[64]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT52_WKO64(void)
{
    Hook_IRQ_INT52_WKO64();
}

/**
 * ****************************************************************************
 * WKO[65]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT53_WKO65(void)
{
    Hook_IRQ_INT53_WKO65();
}

/**
 * ****************************************************************************
 * WKO[66]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT54_WKO66(void)
{
    Hook_IRQ_INT54_WKO66();
}

/**
 * ****************************************************************************
 * WKO[67]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT55_WKO67(void)
{
    Hook_IRQ_INT55_WKO67();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT56_Null(void)
{
    Hook_IRQ_INT56_Null();
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT57_Null(void)
{
    Hook_IRQ_INT57_Null();
}

/**
 * ****************************************************************************
 * External Timer 2 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT58_ET2(void)
{
    Hook_IRQ_INT58_ET2();
}

/**
 * ****************************************************************************
 * Deferred SPI Instruction Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT59_DeferredSPIInstruction(void)
{
    Hook_IRQ_INT59_DeferredSPIInstruction();
}

/**
 * ****************************************************************************
 * TMRINTA0
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT60_TMRINTA0(void)
{
    Hook_IRQ_INT60_TMRINTA0();
}

/**
 * ****************************************************************************
 * TMRINTA1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT61_TMRINTA1(void)
{
    Hook_IRQ_INT61_TMRINTA1();
}

/**
 * ****************************************************************************
 * TMRINTB0
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT62_TMRINTB0(void)
{
    Hook_IRQ_INT62_TMRINTB0();
}

/**
 * ****************************************************************************
 * TMRINTB1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT63_TMRINTB1(void)
{
    Hook_IRQ_INT63_TMRINTB1();
}

/**
 * ****************************************************************************
 * PMC2EX Output Buffer Empty Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT64_PMC2EXOBE(void)
{
    Hook_IRQ_INT64_PMC2EXOBE();
}

/**
 * ****************************************************************************
 * PMC2EX Input Buffer Full Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT65_PMC2EXIBF(void)
{
    Hook_IRQ_INT65_PMC2EXIBF();
}

/**
 * ****************************************************************************
 * PMC3 Output Buffer Empty Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT66_PMC3OBE(void)
{
    Hook_IRQ_INT66_PMC3OBE();
}

/**
 * ****************************************************************************
 * PMC3 Input Buffer Full Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT67_PMC3IBF(void)
{
    // 20160913 Build Port 86 +
	F_Service_PCI4 = 1;
	
	CLEAR_MASK(IER8,Int_PMC3IBF);
	ISR8 = Int_PMC3IBF;
	// 20160913 Build Port 86 -
    Hook_IRQ_INT67_PMC3IBF();
}

/**
 * ****************************************************************************
 * PMC4 Output Buffer Empty Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT68_PMC4OBE(void)
{
    Hook_IRQ_INT68_PMC4OBE();
}

/**
 * ****************************************************************************
 * PMC4 Input Buffer Full Intr.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT69_PMC4IBF(void)
{
    Hook_IRQ_INT69_PMC4IBF();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT70_Null(void)
{
    Hook_IRQ_INT70_Null();
}

/**
 * ****************************************************************************
 * I2BRAM Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT71_I2BRAM(void)
{
    Hook_IRQ_INT71_I2BRAM();
}

/**
 * ****************************************************************************
 * WKO[70]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT72_WKO70(void)
{
    Hook_IRQ_INT72_WKO70();
}

/**
 * ****************************************************************************
 * WKO[71]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT73_WKO71(void)
{
    Hook_IRQ_INT73_WKO71();
}

/**
 * ****************************************************************************
 * WKO[72]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT74_WKO72(void)
{
    Hook_IRQ_INT74_WKO72();
}

/**
 * ****************************************************************************
 * WKO[73]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT75_WKO73(void)
{
    Hook_IRQ_INT75_WKO73();
}

/**
 * ****************************************************************************
 * WKO[74]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT76_WKO74(void)
{
    Hook_IRQ_INT76_WKO74();
}

/**
 * ****************************************************************************
 * WKO[75]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT77_WKO75(void)
{
    Hook_IRQ_INT77_WKO75();
}

/**
 * ****************************************************************************
 * WKO[76]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT78_WKO76(void)
{
    Hook_IRQ_INT78_WKO76();
}

/**
 * ****************************************************************************
 * WKO[77]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT79_WKO77(void)
{
    Hook_IRQ_INT79_WKO77();
}

/**
 * ****************************************************************************
 * External timer 8 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT80_ET8(void)
{
    Hook_IRQ_INT80_ET8();
}

/**
 * ****************************************************************************
 * SMBus clock held interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT81_SMBusClockHeld(void)
{
    Hook_IRQ_INT81_SMBusClockHeld();
}

/**
 * ****************************************************************************
 * CEC interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT82_CEC(void)
{
    Hook_IRQ_INT82_CEC();
}

/**
 * ****************************************************************************
 * H2RAM LPC trigger
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT83_H2RAMLPC(void)
{
    Hook_IRQ_INT83_H2RAMLPC();
}

/**
 * ****************************************************************************
 * ISR of hardware key scan
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT84_Null(void)
{
    /* Hardware scan keyboard */
    #ifdef KB_HW_KeyScan
    CLEAR_MASK(IER10, Int_KBSDVIntr);
    ISR10 = Int_KBSDVIntr;

    /* Post service request to scan internal keyboard. */
    F_Service_KEY = 1;
    
    Hook_IRQ_INT84_Null();
    #endif /* #ifdef KB_HW_KeyScan #endif */
}

/**
 * ****************************************************************************
 * WKO[88]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT85_WKO88(void)
{
    Hook_IRQ_INT85_WKO88();
}

/**
 * ****************************************************************************
 * WKO[89]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT86_WKO89(void)
{
    #ifndef EC_FEATURE_ONLY
    hal_WUI_USBDminus_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_USBDminus_irq();
        #endif
    #endif
    
    Hook_IRQ_INT86_WKO89();
}

/**
 * ****************************************************************************
 * WKO[90]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT87_WKO90(void)
{
    #ifndef EC_FEATURE_ONLY
    hal_WUI_USBDplus_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_USBDplus_irq();
        #endif
    #endif
    
    Hook_IRQ_INT87_WKO90();
}

/**
 * ****************************************************************************
 * WKO[80]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT88_WKO80(void)
{
    Hook_IRQ_INT88_WKO80();
}

/**
 * ****************************************************************************
 * WKO[81]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT89_WKO81(void)
{
    Hook_IRQ_INT89_WKO81();
}

/**
 * ****************************************************************************
 * WKO[82]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT90_WKO82(void)
{
    #ifndef EC_FEATURE_ONLY
    hal_WUI_SmbF_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_SmbF_irq();
        #endif
    #endif
    
    Hook_IRQ_INT90_WKO82();
}

/**
 * ****************************************************************************
 * WKO[83]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT91_WKO83(void)
{
    Hook_IRQ_INT91_WKO83();
}

/**
 * ****************************************************************************
 * WKO[84]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT92_WKO84(void)
{
    Hook_IRQ_INT92_WKO84();
}

/**
 * ****************************************************************************
 * WKO[85]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT93_WKO85(void)
{
    Hook_IRQ_INT93_WKO85();
}

/**
 * ****************************************************************************
 * WKO[86]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT94_WKO86(void)
{
    Hook_IRQ_INT94_WKO86();
}

/**
 * ****************************************************************************
 * WKO[87]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT95_WKO87(void)
{
    Hook_IRQ_INT95_WKO87();
}

/**
 * ****************************************************************************
 * WKO[91]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT96_WKO91(void)
{
    Hook_IRQ_INT96_WKO91();
}

/**
 * ****************************************************************************
 * WKO[92]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT97_WKO92(void)
{
    Hook_IRQ_INT97_WKO92();
}

/**
 * ****************************************************************************
 * WKO[93]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT98_WKO93(void)
{
    Hook_IRQ_INT98_WKO93();
}

/**
 * ****************************************************************************
 * WKO[94]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT99_WKO94(void)
{   
    #ifndef EC_FEATURE_ONLY
    hal_WUI_SmbF_swap2ch0_irq();
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_WUI_SmbF_swap2ch0_irq();
        #endif
    #endif
    
    Hook_IRQ_INT99_WKO94();
}

/**
 * ****************************************************************************
 * WKO[95]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT100_WKO95(void)
{
    Hook_IRQ_INT100_WKO95();
}

/**
 * ****************************************************************************
 * WKO[96]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT101_WKO96(void)
{
    Hook_IRQ_INT101_WKO96();
}

/**
 * ****************************************************************************
 * WKO[97]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT102_WKO97(void)
{
    Hook_IRQ_INT102_WKO97();
}

/**
 * ****************************************************************************
 * WKO[98]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT103_WKO98(void)
{
    Hook_IRQ_INT103_WKO98();
}

/**
 * ****************************************************************************
 * WKO[99]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT104_WKO99(void)
{
    Hook_IRQ_INT104_WKO99();
}

/**
 * ****************************************************************************
 * WKO[100]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT105_WKO100(void)
{
    Hook_IRQ_INT105_WKO100();
}

/**
 * ****************************************************************************
 * WKO[101]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT106_WKO101(void)
{
    Hook_IRQ_INT106_WKO101();
}

/**
 * ****************************************************************************
 * WKO[102]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT107_WKO102(void)
{
    Hook_IRQ_INT107_WKO102();
}

/**
 * ****************************************************************************
 * WKO[103]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT108_WKO103(void)
{
    Hook_IRQ_INT108_WKO103();
}

/**
 * ****************************************************************************
 * WKO[104]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT109_WKO104(void)
{
    Hook_IRQ_INT109_WKO104();
}

/**
 * ****************************************************************************
 * WKO[105]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT110_WKO105(void)
{
    Hook_IRQ_INT110_WKO105();
}

/**
 * ****************************************************************************
 * WKO[106]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT111_WKO106(void)
{
    Hook_IRQ_INT111_WKO106();
}

/**
 * ****************************************************************************
 * WKO[107]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT112_WKO107(void)
{
    Hook_IRQ_INT112_WKO107();
}

/**
 * ****************************************************************************
 * WKO[108]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT113_WKO108(void)
{
    Hook_IRQ_INT113_WKO108();
}

/**
 * ****************************************************************************
 * WKO[109]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT114_WKO109(void)
{
    Hook_IRQ_INT114_WKO109();
}

/**
 * ****************************************************************************
 * WKO[110]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT115_WKO110(void)
{
    Hook_IRQ_INT115_WKO110();
}

/**
 * ****************************************************************************
 * WKO[111]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT116_WKO111(void)
{
    Hook_IRQ_INT116_WKO111();
}

/**
 * ****************************************************************************
 * WKO[112]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT117_WKO112(void)
{
    Hook_IRQ_INT117_WKO112();
}

/**
 * ****************************************************************************
 * WKO[113]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT118_WKO113(void)
{
    Hook_IRQ_INT118_WKO113();
}

/**
 * ****************************************************************************
 * WKO[114]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT119_WKO114(void)
{
    Hook_IRQ_INT119_WKO114();
}

/**
 * ****************************************************************************
 * WKO[115]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT120_WKO115(void)
{
    Hook_IRQ_INT120_WKO115();
}

/**
 * ****************************************************************************
 * WKO[116]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT121_WKO116(void)
{
    Hook_IRQ_INT121_WKO116();
}

/**
 * ****************************************************************************
 * WKO[117]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT122_WKO117(void)
{
    Hook_IRQ_INT122_WKO117();
}

/**
 * ****************************************************************************
 * WKO[118]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT123_WKO118(void)
{
    Hook_IRQ_INT123_WKO118();
}

/**
 * ****************************************************************************
 * WKO[119]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT124_WKO119(void)
{
    Hook_IRQ_INT124_WKO119();
}

/**
 * ****************************************************************************
 * WKO[120]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT125_WKO120(void)
{
    Hook_IRQ_INT125_WKO120();
}

/**
 * ****************************************************************************
 * WKO[121]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT126_WKO121(void)
{
    Hook_IRQ_INT126_WKO121();
}

/**
 * ****************************************************************************
 * WKO[122]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT127_WKO122(void)
{
    Hook_IRQ_INT127_WKO122();
}

/**
 * ****************************************************************************
 * WKO[128]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT128_WKO128(void)
{
    Hook_IRQ_INT128_WKO128();
}

/**
 * ****************************************************************************
 * WKO[129]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT129_WKO129(void)
{
    Hook_IRQ_INT129_WKO129();
}

/**
 * ****************************************************************************
 * WKO[130]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT130_WKO130(void)
{
    Hook_IRQ_INT130_WKO130();
}

/**
 * ****************************************************************************
 * WKO[131]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT131_WKO131(void)
{
    Hook_IRQ_INT131_WKO131();
}

/**
 * ****************************************************************************
 * WKO[132]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT132_WKO132(void)
{
    Hook_IRQ_INT132_WKO132();
}

/**
 * ****************************************************************************
 * WKO[133]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT133_WKO133(void)
{
    Hook_IRQ_INT133_WKO133();
}

/**
 * ****************************************************************************
 * WKO[134]
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT134_WKO134(void)
{
    Hook_IRQ_INT134_WKO134();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT135_Null(void)
{
    Hook_IRQ_INT135_Null();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT136_WKO136(void)
{
    Hook_IRQ_INT136_WKO136();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT137_WKO137(void)
{
    Hook_IRQ_INT137_WKO137();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT138_WKO138(void)
{
    Hook_IRQ_INT138_WKO138();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT139_WKO139(void)
{
    Hook_IRQ_INT139_WKO139();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT140_WKO140(void)
{
    Hook_IRQ_INT140_WKO140();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT141_WKO141(void)
{
    Hook_IRQ_INT141_WKO141();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT142_WKO142(void)
{
    Hook_IRQ_INT142_WKO142();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT143_WKO143(void)
{
    Hook_IRQ_INT143_WKO143();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT144_Null(void)
{
    #ifndef EC_FEATURE_ONLY
    if(2 == (ECHIPVER&0xF))
    {
        hal_power_save_irq();
    }
    #endif /* EC_FEATURE_ONLY */
    Hook_IRQ_INT144_Null();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT145_Null(void)
{
    Hook_IRQ_INT145_Null();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT146_Null(void)
{
    Hook_IRQ_INT146_Null();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT147_Null(void)
{
    Hook_IRQ_INT147_Null();
}

/**
 * ****************************************************************************
 * Null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT148_Null(void)
{
    Hook_IRQ_INT148_Null();
}

/**
 * ****************************************************************************
 * PMC5 output buffer empty interrupt.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT149_PMC5OBE(void)
{
    Hook_IRQ_INT149_PMC5OBE();
}

/**
 * ****************************************************************************
 * PMC5 input buffer full interrupt.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT150_PMC5IBF(void)
{
    Hook_IRQ_INT150_PMC5IBF();
}

/**
 * ****************************************************************************
 * Voltage comparator interrupt.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT151_VCI(void)
{
    Hook_IRQ_INT151_VCI();
}

/**
 * ****************************************************************************
 * SMBus E Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT152_SMBusE(void)
{
    /* rtos only */
    /* Disable ISR */
    #ifndef EC_FEATURE_ONLY
        #ifdef  __HID_FUSION_FUN__
        hal_SMbusE_irq();
        #endif /* __HID_FUSION_FUN__ */
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_SMbusE_irq();
        #endif 
    #endif /* EC_FEATURE_ONLY */

    Hook_IRQ_INT152_SMBusE();
}

/**
 * ****************************************************************************
 * SMBus F Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT153_SMBusF(void)
{
    /* rtos only */
    /* Disable ISR */
    #ifndef EC_FEATURE_ONLY
        #ifdef  __HID_FUSION_FUN__
        hal_SMbusF_irq();
        #endif /* __HID_FUSION_FUN__ */
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        hal_SMbusF_irq();
        #endif      
    #endif /* EC_FEATURE_ONLY */

    Hook_IRQ_INT153_SMBusF();
}

/**
 * ****************************************************************************
 * OSC DMA Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT154_OSCDMA(void)
{
    Hook_IRQ_INT154_OSCDMA();
}

/**
 * ****************************************************************************
 * External Timer 3 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void RTOS_CODE_H IRQ_INT155_ET3(void)
{
    /* Write to clear external timer 3 interrupt */
    ISR19 = Int_ET3Intr;

    /* Request 1 mS timer service. */
    F_Service_MS_1 = 1;

    TIMER_1MS_TICKS++;

    #ifndef EC_FEATURE_ONLY
    ISR_Timer1msEvent();

	Timer1msCnt++;
    if(Timer1msCnt>=10)
    {
        Timer1msCnt = 0x00;
    }
    #endif

    if(Hook_Only_Timer1msEvent()==Only_Timer1msEvent)
    {   

    }
    else
    {
        /* to increase 1ms counter */
        Timer1msCounter++;

        if(Timer1msCounter%5 == 0)
        {
            /*
             * set 5ms service event
             * freertos only
             */
            #ifndef EC_FEATURE_ONLY
            ISR_Timer5msEvent();
            #else
            Service_Timer5msEvent++;
            #endif
        }

        if(Timer1msCounter%10 == 0)
        {
            /* set 10ms service event A */
            Service_Timer10msEventA++;
        }

        if(Timer1msCounter%10 == 5)
        {
            ISR_Timer10msEvent();
            /* set 10ms service event B */
            Service_Timer10msEventB++;
        }

        if(Timer1msCounter%50 == 0)
        {
            /* set 50ms service event A */
            Service_Timer50msEventA++;
        }

        if(Timer1msCounter%50 == 16)
        {
            /* set 50ms service event B */
            Service_Timer50msEventB++;
        }

        if(Timer1msCounter%50 == 32)
        {
            /* set 50ms service event C */
            Service_Timer50msEventC++;
        }

        if(Timer1msCounter%100 == 0)
        {
            /* set 100ms service event A */
            Service_Timer100msEventA++;
        }

        if(Timer1msCounter%100 == 33)
        {
            /* set 100ms service event B */
            Service_Timer100msEventB++;
        }

        if(Timer1msCounter%100 == 66)
        {
            /* set 100ms service event C */
            Service_Timer100msEventC++;
        }

        if(Timer1msCounter%500 == 0)
        {
            /* set 500ms service event A */
            Service_Timer500msEventA++;
        }

        if(Timer1msCounter%500 == 160)
        {
            /* set 500ms service event B */
            Service_Timer500msEventB++;
        }

        if(Timer1msCounter%500 == 320)
        {
            /* set 500ms service event C */
            Service_Timer500msEventC++;
        }     

        if(Timer1msCounter%1000 == 0)
        {
            /* set 1sec service event A */
            Service_Timer1SecEventA++;
        }

        if(Timer1msCounter%1000 == 330)
        {
            /* set 1sec service event B */
            Service_Timer1SecEventB++;
        }

        if(Timer1msCounter%1000 == 660)
        {
            /* set 1sec service event C */
            Service_Timer1SecEventC++;
        }

        if(Timer1msCounter%60000 == 0)
        {
            /* set 1min service event */
            Service_Timer1MinEvent++;

            /* reset 1ms counter */
            Timer1msCounter=0x00;
        }
    }
    
    Hook_IRQ_INT155_ET3();
}

/**
 * ****************************************************************************
 * External Timer 4 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT156_ET4(void)
{
    Hook_IRQ_INT156_ET4();
}

/**
 * ****************************************************************************
 * External Timer 5 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT157_ET5(void)
{
    Hook_IRQ_INT157_ET5();
}

/**
 * ****************************************************************************
 * External Timer 6 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT158_ET6(void)
{
    #ifndef EC_FEATURE_ONLY 
    hid_time_check_report_interval();
	#endif
    ISR19 = Int_ET6Intr;
    
    Hook_IRQ_INT158_ET6();
}

/**
 * ****************************************************************************
 * External Timer 7 Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void RTOS_CODE_H IRQ_INT159_ET7(void)
{
    /* rtos only */
    #ifndef EC_FEATURE_ONLY
    ISR19 = Int_ET7Intr;
    hal_yield_thread_in_isr();
    /* EC only */
    #else
        #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
        ISR19 = Int_ET7Intr;

        /* To set extend event service flag */
        EXTEND_EVENT_ISR_SERVICE_FLAG = 1;

        /* To set extend task service flag */
        F_Service_Extend_Task = 1;
        #endif
    #endif /* EC_FEATURE_ONLY */
    Hook_IRQ_INT159_ET7();
}

/**
 * ****************************************************************************
 * INT 160 null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT160_NULL(void)
{
    Hook_IRQ_INT160_Null();
}

/**
 * ****************************************************************************
 * INT 161 null
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT161_NULL(void)
{
    #ifndef EC_FEATURE_ONLY
    if(3 == (ECHIPVER&0xF))
    {
        hal_power_save_irq();
    }
    #endif /* EC_FEATURE_ONLY */
    Hook_IRQ_INT161_Null();
}

/**
 * ****************************************************************************
 * INT 165 USB PD 0
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT165_USBPD0(void)
{
#if _ITE_USB_PD_ENABLE_
    Hook_IRQ_INT165_USBPD0();
#endif
}

/**
 * ****************************************************************************
 * INT 166 USB PD 1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INT166_USBPD1(void)
{
#if _ITE_USB_PD_ENABLE_
    Hook_IRQ_INT166_USBPD1();
#endif
}

/**
 * ****************************************************************************
 * ISR reserved
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void IRQ_INTxxx_Reserved(void)
{

}

/*
 * ****************************************************************************
 * IVT for Isr_Int1 function
 * ****************************************************************************
 */
const ISR_RODATA FUNCT_PTR_V_V IRQ_Service[] = 
{
    IRQ_INT0_Null,              //   INT0	Reserved
    IRQ_INT1_WKO20,             //   INT1	WUC interrupt WUI0	
    IRQ_INT2_KBCOBE,            //   INT2	KBC output buffer empty interrupt 
    IRQ_INT3_PMCPMC1OBE,        //   INT3	PMC output buffer empty interrupt 
    IRQ_INT4_SMBusD,            //   INT4	SMBus D Interrupt
    IRQ_INT5_WKINTAD,           //   INT5	WUC interrupt (WU10 ~ WU15)(WU40~47)	
    IRQ_INT6_WKO23,             //   INT6	WUC interrupt WUI3
    IRQ_INT7_PWM,               //   INT7	PWM interrupt 
    
    IRQ_INT8_ADC,    		    //   INT8  	ADC Interrupt 	
    IRQ_INT9_SMBusA,      	    //   INT9  	SMB0 Interrupt 
    IRQ_INT10_SMBusB,           //   INT10 	SMB1 Interrupt
    IRQ_INT11_KBMatrixScan,     //   INT11 	Key matrix scan Int
    IRQ_INT12_WKO26,            //   INT12	WUC interrupt SWUC wake up
    IRQ_INT13_WKINTC,           //   INT13	WUC interrupt KSI wake-up (WU30~37)
    IRQ_INT14_WKO25,            //   INT14	WUC interrupt Power switch
    IRQ_INT15_CIR,              //   INT15 	CIR interrupt 
    
    IRQ_INT16_SMBusC,           //   INT16	SMB2 Interrupt
    IRQ_INT17_WKO24,            //   INT17	WUC Interrupt WUI4
    IRQ_INT18_PS2Interrupt2,    //   INT18 	PS2 P2 Interrupt
    IRQ_INT19_PS2Interrupt1,    //   INT19 	PS2 P1 Interrupt
    IRQ_INT20_PS2Interrupt0,    //   INT20 	PS2 P0 Interrupt
    IRQ_INT21_WKO22,            //   INT21	WUC Interrupt WUI2
    IRQ_INT22_SMFISemaphore,    //   INT22  SMFI Semaphore Interrupt
    IRQ_INT23_Null,             //   INT23  --
    
    IRQ_INT24_KBCIBF,           //   INT24 	KBC input buffer empty interrupt 
    IRQ_INT25_PMCPMC1IBF,       //   INT25 	PMC input buffer empty interrupt 
    IRQ_INT26_PMC2OBE,          //   INT26  PMC2 Output Buffer Empty Intr.
    IRQ_INT27_PMC2IBF,          //   INT27  PMC2 Input Buffer Full Intr.
    IRQ_INT28_GINTofGPD5,       //   INT28  GINT from function 1 of GPD5
    IRQ_INT29_EGPC,             //   INT29  EGPC Interrupt
    IRQ_INT30_ET1,              //   INT30	External timer interrupt 
    IRQ_INT31_WKO21,            //   INT31	WUC	interrupt WUI1
    
    IRQ_INT32_GPINT0,           //   INT32  GPINT0	
    IRQ_INT33_GPINT1,           //   INT33  GPINT1	    
    IRQ_INT34_GPINT2,           //   INT34  GPINT2
    IRQ_INT35_GPINT3,           //   INT35  GPINT3
    IRQ_INT36_CIRGPINT,         //   INT36  CIR GPINT
    IRQ_INT37_SSPI,             //   INT37  SSPI Interrupt
    IRQ_INT38_UART1,            //   INT38  UART1 Interrupt
    IRQ_INT39_UART2,            //   INT39  UART2 Interrupt
    
    IRQ_INT40_WKO50,       		//   INT40 	--
    IRQ_INT41_WKO51,       		//   INT41 	--
    IRQ_INT42_WKO52,             //   INT42  --
    IRQ_INT43_WKO53,             //   INT43  --
    IRQ_INT44_WKO54,             //   INT44  --
    IRQ_INT45_WKO55,             //   INT45  --
    IRQ_INT46_WKO56, 			//   INT46	--
    IRQ_INT47_WKO57,             //   INT47	--
    
    IRQ_INT48_WKO60,       	    //   INT48  WKO[60]	
    IRQ_INT49_WKO61,       	    //   INT49  WKO[61] 	
    IRQ_INT50_WKO62,            //   INT50  WKO[62]
    IRQ_INT51_WKO63,            //   INT51  WKO[63]
    IRQ_INT52_WKO64,            //   INT52  WKO[64]
    IRQ_INT53_WKO65,            //   INT53  WKO[65]
    IRQ_INT54_WKO66,            //   INT54  WKO[66]	
    IRQ_INT55_WKO67,            //   INT55  WKO[67]    

    IRQ_INT56_Null,             //   INT56  -- 	
    IRQ_INT57_Null,             //   INT57  --
    IRQ_INT58_ET2,              //   INT58  External Timer 2 Interrupt
    IRQ_INT59_DeferredSPIInstruction,   //   INT59  Deferred SPI Instruction     
    IRQ_INT60_TMRINTA0,         //   INT60  TMRINTA0
    IRQ_INT61_TMRINTA1,         //   INT61  TMRINTA1
    IRQ_INT62_TMRINTB0,         //   INT62  TMRINTB0  
	IRQ_INT63_TMRINTB1,         //   INT63  TMRINTB1

    IRQ_INT64_PMC2EXOBE,        //   INT64  PMC2EX Output Buffer Empty Intr.
    IRQ_INT65_PMC2EXIBF,        //   INT65  PMC2EX Input Buffer Full Intr.
    IRQ_INT66_PMC3OBE,          //   INT66  PMC3 Output Buffer Empty Intr.
    IRQ_INT67_PMC3IBF,          //   INT67  PMC3 Input Buffer Full Intr.
    IRQ_INT68_PMC4OBE,          //   INT68  PMC4 Output Buffer Empty Intr.
    IRQ_INT69_PMC4IBF,          //   INT69  PMC4 Input Buffer Full Intr.
    IRQ_INT70_Null,             //   INT70  --
    IRQ_INT71_I2BRAM,           //   INT71  I2BRAM Interrupt

    IRQ_INT72_WKO70,            //   INT72  WKO[70]	
    IRQ_INT73_WKO71,            //   INT73  WKO[71] 	
    IRQ_INT74_WKO72,            //   INT74  WKO[72]
    IRQ_INT75_WKO73,            //   INT75  WKO[73]
    IRQ_INT76_WKO74,            //   INT76  WKO[74]
    IRQ_INT77_WKO75,            //   INT77  WKO[75]
    IRQ_INT78_WKO76,            //   INT78  WKO[76]	
    IRQ_INT79_WKO77,            //   INT79  WKO[77]   

    IRQ_INT80_ET8,              //   INT80  External timer 8 interrupt 
    IRQ_INT81_SMBusClockHeld,   //   INT81  SMBus clock held interrupt.
    IRQ_INT82_CEC,              //   INT82  CEC interrupt.
    IRQ_INT83_H2RAMLPC,         //   INT83  H2RAM LPC Trigger.
    IRQ_INT84_Null,             //   INT84  --
    IRQ_INT85_WKO88,            //   INT85  WKO[88] 
    IRQ_INT86_WKO89,            //   INT86  WKO[89] 
    IRQ_INT87_WKO90,            //   INT87  WKO[90]

    IRQ_INT88_WKO80,            //   INT88  WKO[80]
    IRQ_INT89_WKO81,            //   INT89  WKO[81]
    IRQ_INT90_WKO82,            //   INT90  WKO[82]
    IRQ_INT91_WKO83,            //   INT91  WKO[83]
    IRQ_INT92_WKO84,            //   INT92  WKO[84]
    IRQ_INT93_WKO85,            //   INT93  WKO[85]
    IRQ_INT94_WKO86,            //   INT94  WKO[86]
    IRQ_INT95_WKO87,            //   INT95  WKO[87]

    IRQ_INT96_WKO91,            //   INT96  WKO[91]
    IRQ_INT97_WKO92,            //   INT97  WKO[92]
    IRQ_INT98_WKO93,            //   INT98  WKO[93]
    IRQ_INT99_WKO94,            //   INT99  WKO[94]
    IRQ_INT100_WKO95,           //   INT100 WKO[95]
    IRQ_INT101_WKO96,           //   INT101 WKO[96]
    IRQ_INT102_WKO97,           //   INT102 WKO[97]
    IRQ_INT103_WKO98,           //   INT103 WKO[98]

    IRQ_INT104_WKO99,           //   INT104 WKO[99]
    IRQ_INT105_WKO100,          //   INT105 WKO[100]
    IRQ_INT106_WKO101,          //   INT106 WKO[101]
    IRQ_INT107_WKO102,          //   INT107 WKO[102]
    IRQ_INT108_WKO103,          //   INT108 WKO[103]
    IRQ_INT109_WKO104,          //   INT109 WKO[104]
    IRQ_INT110_WKO105,          //   INT110 WKO[105]
    IRQ_INT111_WKO106,          //   INT111 WKO[106]

    IRQ_INT112_WKO107,          //   INT112 WKO[107]
    IRQ_INT113_WKO108,          //   INT113 WKO[108]
    IRQ_INT114_WKO109,          //   INT114 WKO[109]
    IRQ_INT115_WKO110,          //   INT115 WKO[110]
    IRQ_INT116_WKO111,          //   INT116 WKO[111]
    IRQ_INT117_WKO112,          //   INT117 WKO[112]
    IRQ_INT118_WKO113,          //   INT118 WKO[113]
    IRQ_INT119_WKO114,          //   INT119 WKO[114]

    IRQ_INT120_WKO115,          //   INT120 WKO[115]
    IRQ_INT121_WKO116,          //   INT121 WKO[116]
    IRQ_INT122_WKO117,          //   INT122 WKO[117]
    IRQ_INT123_WKO118,          //   INT123 WKO[118]
    IRQ_INT124_WKO119,          //   INT124 WKO[119]
    IRQ_INT125_WKO120,          //   INT125 WKO[120]
    IRQ_INT126_WKO121,          //   INT126 WKO[121]
    IRQ_INT127_WKO122,          //   INT127 WKO[122]

    IRQ_INT128_WKO128,          //   INT128 WKO[128]
    IRQ_INT129_WKO129,          //   INT129 WKO[129]
    IRQ_INT130_WKO130,          //   INT130 WKO[130]
    IRQ_INT131_WKO131,          //   INT131 WKO[131]
    IRQ_INT132_WKO132,          //   INT132 WKO[132]
    IRQ_INT133_WKO133,          //   INT133 WKO[133]
    IRQ_INT134_WKO134,          //   INT134 WKO[134]
    IRQ_INT135_Null,            //   INT135 --

    IRQ_INT136_WKO136,            //   INT136 --
    IRQ_INT137_WKO137,            //   INT137 --
    IRQ_INT138_WKO138,            //   INT138 --
    IRQ_INT139_WKO139,            //   INT139 --
    IRQ_INT140_WKO140,            //   INT140 --
    IRQ_INT141_WKO141,            //   INT141 --
    IRQ_INT142_WKO142,            //   INT142 --
    IRQ_INT143_WKO143,            //   INT143 --

    IRQ_INT144_Null,            //   INT144 --
    IRQ_INT145_Null,            //   INT145 --
    IRQ_INT146_Null,            //   INT146 --
    IRQ_INT147_Null,            //   INT147 --
    IRQ_INT148_Null,            //   INT148 --
    IRQ_INT149_PMC5OBE,         //   INT149 PMC5 output buffer empty interrupt.
    IRQ_INT150_PMC5IBF,         //   INT150 PMC5 input buffer full interrupt.
    IRQ_INT151_VCI,             //   INT151 Voltage comparator interrupt.

    IRQ_INT152_SMBusE,          //   INT152 SMBus E Interrupt.
    IRQ_INT153_SMBusF,          //   INT153 SMBus F Interrupt.
    IRQ_INT154_OSCDMA,          //   INT154 OSC DMA Interrupt.
    IRQ_INT155_ET3,             //   INT155 External Timer 3 Interrupt.
    IRQ_INT156_ET4,             //   INT156 External Timer 4 Interrupt.
    IRQ_INT157_ET5,             //   INT157 External Timer 5 Interrupt.
    IRQ_INT158_ET6,             //   INT158 External Timer 6 Interrupt.
    IRQ_INT159_ET7,             //   INT159 External Timer 7 Interrupt.

    IRQ_INT160_NULL,            //   INT160 --
    IRQ_INT161_NULL,            //   INT161 --
    IRQ_INTxxx_Reserved,        //   INT162 Reserved
    IRQ_INTxxx_Reserved,        //   INT163 Reserved
    IRQ_INTxxx_Reserved,        //   INT164 Reserved
    IRQ_INT165_USBPD0,          //   INT165 USB PD 0
    IRQ_INT166_USBPD1,          //   INT166 USB PD 1
    IRQ_INTxxx_Reserved,        //   INT167 Reserved

    IRQ_INTxxx_Reserved,        //   INT168 Reserved
    IRQ_INTxxx_Reserved,        //   INT169 Reserved
    IRQ_INTxxx_Reserved,        //   INT170 Reserved
    IRQ_INTxxx_Reserved,        //   INT171 Reserved
    IRQ_INTxxx_Reserved,        //   INT172 Reserved
    IRQ_INTxxx_Reserved,        //   INT173 Reserved
    IRQ_INTxxx_Reserved,        //   INT174 Reserved
    IRQ_INTxxx_Reserved,        //   INT175 Reserved

    IRQ_INTxxx_Reserved,        //   INT176 Reserved
    IRQ_INTxxx_Reserved,        //   INT177 Reserved
    IRQ_INTxxx_Reserved,        //   INT178 Reserved
    IRQ_INTxxx_Reserved,        //   INT179 Reserved
    IRQ_INTxxx_Reserved,        //   INT180 Reserved
    IRQ_INTxxx_Reserved,        //   INT181 Reserved
    IRQ_INTxxx_Reserved,        //   INT182 Reserved
    IRQ_INTxxx_Reserved,        //   INT183 Reserved

    IRQ_INTxxx_Reserved,        //   INT184 Reserved
    IRQ_INTxxx_Reserved,        //   INT185 Reserved
    IRQ_INTxxx_Reserved,        //   INT186 Reserved
    IRQ_INTxxx_Reserved,        //   INT187 Reserved
    IRQ_INTxxx_Reserved,        //   INT188 Reserved
    IRQ_INTxxx_Reserved,        //   INT189 Reserved
    IRQ_INTxxx_Reserved,        //   INT190 Reserved
    IRQ_INTxxx_Reserved,        //   INT191 Reserved

    IRQ_INTxxx_Reserved,        //   INT192 Reserved
    IRQ_INTxxx_Reserved,        //   INT193 Reserved
    IRQ_INTxxx_Reserved,        //   INT194 Reserved
    IRQ_INTxxx_Reserved,        //   INT195 Reserved
    IRQ_INTxxx_Reserved,        //   INT196 Reserved
    IRQ_INTxxx_Reserved,        //   INT197 Reserved
    IRQ_INTxxx_Reserved,        //   INT198 Reserved
    IRQ_INTxxx_Reserved,        //   INT199 Reserved

    IRQ_INTxxx_Reserved,        //   INT200 Reserved
    IRQ_INTxxx_Reserved,        //   INT201 Reserved
    IRQ_INTxxx_Reserved,        //   INT202 Reserved
    IRQ_INTxxx_Reserved,        //   INT203 Reserved
    IRQ_INTxxx_Reserved,        //   INT204 Reserved
    IRQ_INTxxx_Reserved,        //   INT205 Reserved
    IRQ_INTxxx_Reserved,        //   INT206 Reserved
    IRQ_INTxxx_Reserved,        //   INT207 Reserved

    IRQ_INTxxx_Reserved,        //   INT208 Reserved
    IRQ_INTxxx_Reserved,        //   INT209 Reserved
    IRQ_INTxxx_Reserved,        //   INT210 Reserved
    IRQ_INTxxx_Reserved,        //   INT211 Reserved
    IRQ_INTxxx_Reserved,        //   INT212 Reserved
    IRQ_INTxxx_Reserved,        //   INT213 Reserved
    IRQ_INTxxx_Reserved,        //   INT214 Reserved
    IRQ_INTxxx_Reserved,        //   INT215 Reserved

    IRQ_INTxxx_Reserved,        //   INT216 Reserved
    IRQ_INTxxx_Reserved,        //   INT217 Reserved
    IRQ_INTxxx_Reserved,        //   INT218 Reserved
    IRQ_INTxxx_Reserved,        //   INT219 Reserved
    IRQ_INTxxx_Reserved,        //   INT220 Reserved
    IRQ_INTxxx_Reserved,        //   INT221 Reserved
    IRQ_INTxxx_Reserved,        //   INT222 Reserved
    IRQ_INTxxx_Reserved,        //   INT223 Reserved

    IRQ_INTxxx_Reserved,        //   INT224 Reserved
    IRQ_INTxxx_Reserved,        //   INT225 Reserved
    IRQ_INTxxx_Reserved,        //   INT226 Reserved
    IRQ_INTxxx_Reserved,        //   INT227 Reserved
    IRQ_INTxxx_Reserved,        //   INT228 Reserved
    IRQ_INTxxx_Reserved,        //   INT229 Reserved
    IRQ_INTxxx_Reserved,        //   INT230 Reserved
    IRQ_INTxxx_Reserved,        //   INT231 Reserved

    IRQ_INTxxx_Reserved,        //   INT232 Reserved
    IRQ_INTxxx_Reserved,        //   INT233 Reserved
    IRQ_INTxxx_Reserved,        //   INT234 Reserved
    IRQ_INTxxx_Reserved,        //   INT235 Reserved
    IRQ_INTxxx_Reserved,        //   INT236 Reserved
    IRQ_INTxxx_Reserved,        //   INT237 Reserved
    IRQ_INTxxx_Reserved,        //   INT238 Reserved
    IRQ_INTxxx_Reserved,        //   INT239 Reserved
};

/**
 * ****************************************************************************
 * ISR for interrupt HW1
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */			
void Section_Isr_Int1 Isr_Int1 (void)
{
    UINT32 GetIVECT(void);
    
    ulong_32 l_temp_evect;

    /* rtos only */
    #ifndef  EC_FEATURE_ONLY
    ulong_32 l_ivect_index;
    #endif

    _nop_();
    _nop_();
    _nop_();
    _nop_();

    l_temp_evect = GetIVECT();

    (IRQ_Service[(l_temp_evect-0x10)])();  /* Dispatch to service handler. */

    /* rtos only */
    #ifndef  EC_FEATURE_ONLY
    l_ivect_index = (l_temp_evect-0x10);

    if(l_ivect_index == IVECT_INT4)         /* SMBUS D Interrupt */
    {
    }
    else if(l_ivect_index == IVECT_INT153)  /* SMBus F Interrupt */
    {      
    }
    else /* other */
    {
        #ifdef  __ENABLE_EC_MAIN__
        hal_resume_task_in_isr(tthi_EC);
        #else
        _nop_();
        #endif
    }
    
    #endif /* EC_FEATURE_ONLY */
    
    Hook_Isr_Int1();
} 

/**
 * ****************************************************************************
 * The function of disable all interrupts
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void DisableAllInterrupt(void)
{
    DisableGlobalInt();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

/**
 * ****************************************************************************
 * The function of enable all interrupts
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void EnableAllInterrupt(void)
{
    EnableGlobalInt();
}

/**
 * ****************************************************************************
 * The function of enabling KBC Input Buffer Full (60 64 port)Interrupt 
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void EnableKBCIBFInt(void)
{
    SET_MASK(IER3,Int_KBCIBF);
}

/**
 * ****************************************************************************
 * The function of enabling PMC Input Buffer Full (62 66 68 6C port)Interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void EnablePMCIBFInt(void)
{
	SET_MASK(IER3,Int_PMCIBF);	
}

//------------------------------------------------------------------------------
// The function of enabling PM3 Input Buffer Full (86 port)Interrupt
//------------------------------------------------------------------------------
void EnablePM3IBFInt(void)
{
  SET_MASK(IER8,Int_PMC3IBF);
}

/**
 * ****************************************************************************
 * The function of enabling interrupts
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void InitEnableInterrupt(void)
{
	ISR0 = 0xFF;
	ISR1 = 0xFF;
	ISR2 = 0xFF;
	ISR3 = 0xFF;
	ISR4 = 0xFF;
    ISR5 = 0xFF;
	ISR6 = 0xFF;
    ISR7 = 0xFF;
    ISR8 = 0xFF;
    ISR9 = 0xFF;
    ISR10 = 0xFF;
    ISR11 = 0xFF;
    ISR12 = 0xFF;
    ISR13 = 0xFF;
    ISR14 = 0xFF;
    ISR15 = 0xFF;
    ISR16 = 0xFF;
    ISR17 = 0xFF;
    ISR18 = 0xFF;
    ISR19 = 0xFF;

    EnableExternalInt();
	EnableAllInterrupt();

    /* enable global interrupt */
    GIE_ENABLE();
}

/**
 * ****************************************************************************
 * The function of enabling module interrupt
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */	
void EnableModuleInterrupt(void)
{
    /* Enable 60/64 port input buffer full interrup */
    EnableKBCIBFInt();
    /* Enable PMC Input Buffer Full (62 66 68 6C port)Interrupt */
  	EnablePMCIBFInt();
    EnablePMC3IBFInt();		// 20160913 Build Port 86
  	Hook_EnableInterrupt();

    /* rtos only */
    #ifndef EC_FEATURE_ONLY
    hal_hw_module_int();
    #endif  /* #ifndef EC_FEATURE_ONLY */
    /* enable all interrupts */
    EnableAllInterrupt();
}

/**
 * ****************************************************************************
 * Exception debug message dump function.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_General_Exception General_Exception(void)
{
    DBG_IPC   = GET_IPC();
    DBG_IPSW  = GET_IPSW();
    DBG_SP    = __nds32__get_current_sp();
    DBG_ITYPE = GET_ITYPE();

    if(*((BYTE*)DBG_IPC) & 0x80)
    {
        SET_IPC(DBG_IPC + 2);
    }
    else
    {
        SET_IPC(DBG_IPC + 4);
    }
    
    while(1);
}

/**
 * ****************************************************************************
 * HW0 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW0 Interrupt_HW0(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW1 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW1 Interrupt_HW1(void)
{
    Isr_Int1();
}

/**
 * ****************************************************************************
 * HW2 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW2 Interrupt_HW2(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW3 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW3 Interrupt_HW3(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW4 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW4 Interrupt_HW4(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW5 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW5 Interrupt_HW5(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW6 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW6 Interrupt_HW6(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW7 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW7 Interrupt_HW7(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW8 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW8 Interrupt_HW8(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW9 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW9 Interrupt_HW9(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW10 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW10 Interrupt_HW10(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW11 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW11 Interrupt_HW11(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW12 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW12 Interrupt_HW12(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW13 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW13 Interrupt_HW13(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW14 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW14 Interrupt_HW14(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * HW15 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_HW15 Interrupt_HW15(void)
{
    while(1);
}

/**
 * ****************************************************************************
 * SW0 ISR
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Section_Interrupt_SW0 Interrupt_SW0(void)
{
    while(1);
}

/*
 * ****************************************************************************
 *
 *            Copyright Andes Technology Corporation 2009-2010
 *                         All Rights Reserved.
 *
 *  Revision History:
 *
 *    Dec.28.2009     Created.
 *    Sep.05.2012     Modify for IT8380, by Jyunlin [ite01242]
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 * EC firmware only
 * ****************************************************************************
 */
#ifdef EC_FEATURE_ONLY
#define HAL_DISABLE_INTERRUPTS		        (0)
#define HAL_ENABLE_INTERRUPTS		        (1)

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 * int_op
 *
 * ****************************************************************************
 */
inline unsigned int hal_global_int_ctl(int int_op)
{
	int ret = GET_PSW() & PSW_mskGIE;

	if (int_op == HAL_DISABLE_INTERRUPTS) 
    {
        GIE_DISABLE();
        SET_INT_MASK(0x00000000);   /*  CPU int mask */
	}
	else
    {
        SET_INT_MASK(0x40010002);   /*  CPU int mask */
		GIE_ENABLE();
	}

	return ret ? !HAL_DISABLE_INTERRUPTS : HAL_DISABLE_INTERRUPTS;
}
#endif

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void EnableGlobalInt(void)
{
    SET_INT_MASK(0x40010002);     /*  CPU int mask */
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void DisableGlobalInt(void)
{
    SET_INT_MASK(0x00000000);     /*  CPU int mask */
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void EnableExternalInt(void)
{
    /* EC firmware only */
    #ifdef EC_FEATURE_ONLY
    /* hal_global_int_ctl(HAL_ENABLE_INTERRUPTS); */
    SET_INT_MASK(0x40010002);     /* CPU int mask */

    /* rtos */
    #else
    SET_INT_MASK(0x40010002);     /* CPU int mask */
    #endif
}

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
UINT32 GetIVECT(void)
{
    ulong_32 l_temp_evect;
    uchar_8  ucCnt = 0;
    
    l_temp_evect = IVECT;
    
    // double check IVECT value
    do
    {
        if(l_temp_evect != IVECT)
        {
            HW1_ISR_DBG_MSG = (UINT8)l_temp_evect; // debug message
            l_temp_evect = IVECT;
            ucCnt = 0;
        }
        else
        {
            ucCnt++;
        }                        
    }while(ucCnt < 2);
    
    return l_temp_evect;
}


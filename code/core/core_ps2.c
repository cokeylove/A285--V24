/* 
 * ****************************************************************************
 * Filename: CORE_PS2.C - PS/2 devices handler
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE Tech. Inc.
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/* 
 * ****************************************************************************
 * PS2 interface registers table
 * ****************************************************************************
 */
const sPS2Struct asPS2Struct[] = 
{
 	{ &PSCTL1, &PSINT1, &PSSTS1,	&PSDAT1,	&IER2,	&ISR2,	Int_PS2_0	},
	{ &PSCTL2, &PSINT2, &PSSTS2,	&PSDAT2,	&IER2,	&ISR2,	Int_PS2_1	},
	{ &PSCTL3, &PSINT3, &PSSTS3,	&PSDAT3,	&IER2,	&ISR2,	Int_PS2_2	}
};

/**
 * ****************************************************************************
 * FUNCTION: Service_Send_PS2
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_PS2_data(void)
{
    if(IS_MASK_SET(KBHISR,OBF)||IS_MASK_SET(KBHISR,IBF))
    /* if(IS_MASK_SET(KBHISR,OBF)) */
	{
        SetPS2InhibitTime(InactiveTime);
		return;
	}

	if(TPACKCounter!=0x00)
	{
		TPACKCounter--;
	}
	else
	{
        /* For internal TB disable */
        #ifdef Hook_Func_DisableTP
        if(Ccb42_DISAB_AUX || (Hook_DisableInternalTB()==InternalTB_Disable))
        #else
		if(Ccb42_DISAB_AUX)		
        #endif
		{
            SetPS2InhibitTime(InactiveTime);
			return;
		}
	}

	SendFromAux(MousePendingBuf);

    if(MULPX_Multiplex)
    {
        /* Enable all ps2 interface */
        PS2Deviceactive();
        SetPS2SkipScanTime(MSSkipScanAUXTime);
    }
    else
    {
        /* Enable all ps2 interface */
        PS2Deviceactive();
        SetPS2InhibitTime(InactiveTime);
    }
    ResetMSPendingData();
}

/**
 * ****************************************************************************
 * Send data from aux mouse 
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SendFromAux(BYTE auxdata)
{
    KBHISR &= 0x0F;

	if(MULPX_Multiplex)
	{
		if(AUXPortNum==0x00)
		{
            /* source bits is 01 */
			KBHISR = 0x40;
		}
		else if(AUXPortNum==0x01)
		{
            /* source bits is 10 */
			KBHISR = 0x80;
		}
        else if(AUXPortNum==0x02)
        {
            /* source bits is 11 */
			KBHISR = 0xC0;
		}
        /* clear error bit */
		CLEAR_MASK(KBHISR,BIT2);
	}

    /* set AUX bit */
	SET_MASK(KBHISR,AOBF);
	KBHICR &= 0xFC;
    if ( Ccb42_INTR_AUX )
    {
       	SET_MASK(KBHICR,OBFMIE);
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();

	KBHIMDOR = auxdata;
    ShortDelayAfterWriteDOR();

	if(PS2_MSCMD)
	{   
        #ifdef __UART_DEBUG__
            printf("MS Port_%X ACK : 0x%X\n",AUXPortNum, auxdata);
        #endif
	}

#if PS2_Pack_Debug    
    if(PS_Debug_ECToHost==0xFF)
    {
       PS_Debug_ECToHost = 0x03;
    }
    else
    {
        PS_Debug_ECToHost++;
        if(PS_Debug_ECToHost>3)
        {
            PS_Debug_ECToHost = 0x01;
        }
    }
#endif
}

/**
 * ****************************************************************************
 * FUNCTION: SetPS2InhibitTime
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SetPS2InhibitTime(BYTE time)
{
    if(AUXInactiveCounter<time)
    {
        AUXInactiveCounter = time;
    }
}

/**
 * ****************************************************************************
 * FUNCTION: SetPS2SkipScanTime
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SetPS2SkipScanTime(BYTE time)
{
    if(SKIP_AUX_SCAN_CUNT<time)
    {
        SKIP_AUX_SCAN_CUNT = time;
    }
}

/**
 * ****************************************************************************
 * Process mouse data from ps2 interface
 *
 * @return
 *
 * @parameter, 0 ~ 2
 *
 * ****************************************************************************
 */
void ProcessMouseData(BYTE channel)
{
    /* Debug only */
#if PS2_Pack_Debug
    if(PS_Debug_ECToHost==0xFF)
    {
       PS_Debug_DeviceToEC = 0x03;
    }
    else
    {
        PS_Debug_DeviceToEC++;
        if(PS_Debug_DeviceToEC>3)
        {
            PS_Debug_DeviceToEC = 0x01;
        }
    }
#endif

    /* Save mouse driver type(3bytes, 4bytes, or 5bytes) */
    if(MS_ID_CUNT!=0x00)
    {
        if(--MS_ID_CUNT==0x00)
        {
            if(PS2PortxData[channel]==0x00||
                PS2PortxData[channel]==0x03||
                 PS2PortxData[channel]==0x04)
            {
                MS_DRIVER_TYPE = PS2PortxData[channel];
            }
        }
    }

    /* For internal TB disable */
    #ifdef Hook_Func_DisableTP
    if(IS_MASK_SET(KBHISR,OBF)||IS_MASK_SET(KBHISR,IBF)||
        (Ccb42_DISAB_AUX)||(Hook_DisableInternalTB()==InternalTB_Disable))
    #else
    if(IS_MASK_SET(KBHISR,OBF)||IS_MASK_SET(KBHISR,IBF)||(Ccb42_DISAB_AUX))
    #endif
    {	
        AUXPortNum = channel;
	    MousePendingBuf=PS2PortxData[channel];
	    MSPending = 0x33;
    }
    else
    {
        AUXPortNum = channel;
	    SendFromAux(PS2PortxData[channel]);
	    if(TPACKCounter!=0x00)
	    {
		    TPACKCounter--;
	    }

        if(MULPX_Multiplex)
	    {
            if(!AUXInterfaceBusy)
            {
                /* Enable all ps2 interface */
                PS2Deviceactive();
                SetPS2SkipScanTime(MSSkipScanAUXTime);
            }
        }
        else
	    {
            if(!AUXInterfaceBusy)
            {
                /* Enable all ps2 interface */
                PS2Deviceactive();
                SetPS2InhibitTime(InactiveTime);
            }
	    }
    }
} 

/**
 * ****************************************************************************
 * Process keybaord data from ps2 interface
 *
 * @return
 *
 * @parameter, 0 ~ 2
 *
 * ****************************************************************************
 */
void ProcessKeyboardData(BYTE channel)
{
    if(Main_KB_ACK_CUNT!=0x00)
    {
        Main_KB_ACK_CUNT--;

        KBC_DataPending(PS2PortxData[channel]);
        SetServiceSendFlag();
        #if PS2_KBCMD
            #ifdef __UART_DEBUG__
            printf("External KB ACK : 0x%X\n",PS2PortxData[channel]);
            #endif
        #endif
    }
    else
    {
        Buffer_Key(PS2PortxData[channel]);
        SetServiceSendFlag();
    }

    if(Main_KB_ACK_CUNT==0x00)
    {
        /* PS2 data is pending */
        if(MSPending==0x33)
        {
            /* PS2 interface isn't busy */
            if(!AUXInterfaceBusy)
            {
                PS2ReceiveMode_OneChannel(channel, 0x01);
            }
        }
        else
        {
            /* PS2 interface isn't busy */
            if(!AUXInterfaceBusy)
            {
                PS2Deviceactive();
                SetPS2SkipScanTime(KBSkipScanAUXTime);
            }
        }
    }
    else
    {
        /* PS2 interface isn't busy */
        if(!AUXInterfaceBusy)
        {
            PS2ReceiveMode_OneChannel(channel, 0x01);
        }
    }
}

/**
 * ****************************************************************************
 * ProcessECTestAUXACK
 *
 * @return
 *
 * @parameter
 * PortNum, 0 ~ 2
 * enablenow
 *
 * ****************************************************************************
 */
void ProcessECTestAUXACK(BYTE PortNum, BYTE enablenow)
{
    ECSendtoAUXFlag = 0;
    ClearAUXScanTimeOutCunt();
    SetAUXScanIndexHS(AUXScanIndexFree);
    ScanAUXStepIndex = ScanAUXStep_CheckOK + PortNum;

    /* command 0xF5 ACK */
    if(IS_MASK_SET(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED))
    {
        if(PS2PortxData[PortNum]==0xFA)
        {
            AuxFlags2[AUX_SCAN_INDEX] = 0x00;
            SET_MASK(AuxFlags2[AUX_SCAN_INDEX],DEVICE_IS_DISABLED);
        }
        else
        {
            PS2_ReTryCunt++;
            if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
            {
                PS2_ReTryCunt = 0x00;
                AuxFlags2[AUX_SCAN_INDEX] = 0x00;
                SET_MASK(AuxFlags2[AUX_SCAN_INDEX],DEVICE_DISABLE_TIME_OUT);
            }
        }
    }
    /* command 0xF4 ACK */
    else if(IS_MASK_SET(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED))
    {
        if(PS2PortxData[PortNum]==0xFA)
        {
            AuxFlags2[AUX_SCAN_INDEX] = 0x00;
            SET_MASK(AuxFlags2[AUX_SCAN_INDEX],DEVICE_IS_ENABLED);
        }
        else
        {
            PS2_ReTryCunt++;
            if(PS2_ReTryCunt>PS2_OnOffTimeOutCUNT)
            {
                PS2_ReTryCunt = 0x00;
                AuxFlags2[AUX_SCAN_INDEX] = 0x00;
                SET_MASK(AuxFlags2[AUX_SCAN_INDEX],DEVICE_ENABLED_TIME_OUT);
            }
        }
    }
    else    /* Test Command 0xF1 ACK */
    {
        /* Device first time plug-in */
        if(AuxFlags[PortNum]==0x00)
        {
            AuxFlags[PortNum] = DEVICE_NEED_INIT;
            SetAUXScanIndexHS(AUXScanIndexInUse);
            /* goto check device is keyboard or mouse */
            ScanAUXStepIndex = ScanAUXStep_CheckType + PortNum;
        }
        else
        {
            if(IS_MASK_SET(AuxFlags[PortNum], DEVICE_NEED_CONFIG))
            {
                SetAUXScanIndexHS(AUXScanIndexInUse);
                /* goto config. device */
                ScanAUXStepIndex = ScanAUXStep_Init + PortNum;
            }
            else if(AuxFlags[PortNum] == DEVICE_NEED_INIT)
            {
                SetAUXScanIndexHS(AUXScanIndexInUse);
                /* goto check device is keyboard or mouse */
                ScanAUXStepIndex = ScanAUXStep_CheckType + PortNum;
            }
            else
            {
                if(AuxFlags_KB_CMD[PortNum]!=0x00)
                {
                    SetAUXScanIndexHS(AUXScanIndexInUse);
                    ScanAUXStepIndex = ScanAUXStep_UpdateKB + PortNum;
                }
            }
        }
    }

    if(enablenow==0x01)
    {
        PS2Deviceactive();	
    }
}

/**
 * ****************************************************************************
 * Saving PS2 mouse device data to buffer.
 *
 * @return
 *
 * @parameter
 * PortNum, 0 ~ 2
 *
 * ****************************************************************************
 */
void SendAUXData(BYTE PortNum)          
{
    /* Send command to device and transmit done. */
    if(SendtoAUXFlag)
    {
        SendtoAUXFlag = 0;		
        PS2ReceiveMode_OneChannel(PortNum, 0x00);
        ClearAUXScanTimeOutCunt();
    }
    else    /* Receive done and get data */
    {
        /* If thsi byte is EC scan channel ACK, no send to host */
        if(ECSendtoAUXFlag)
        {
            ProcessECTestAUXACK(PortNum, 1);
        }
        /* receive mode transmit done. */
        else  
        {
            SetAUXScanIndexHS(AUXScanIndexFree);
            ClearAUXScanTimeOutCunt();
            /* If data from mosue */
            if(IS_MASK_SET(AuxFlags[PortNum], DEVICE_IS_MOUSE))
	        {
                /* Process mosue data */
                ProcessMouseData(PortNum);
	        }
            /* If data from keyboard */
            else if(IS_MASK_SET(AuxFlags[PortNum], DEVICE_IS_KEYBOARD))
	        {
                /* Process keyboard data */
	            ProcessKeyboardData(PortNum);
	        }
            else
            {
                /* Enable all ps2 interface */
                PS2Deviceactive();
            }
        }
    }
}

/**
 * ****************************************************************************
 * Send command to PS2 device
 *
 * @return
 *
 * @parameter
 * p_PortNum, 0 ~ 2
 * p_cmd
 *
 * ****************************************************************************
 */
__attribute__((optimize("O0")))
void CORE_FLASH_CODE RAM_Send2Port(BYTE p_PortNum, BYTE p_cmd)
{
    if(p_PortNum==0x00)
    {
        PSCTL1 = 0x1D;
        PSDAT1 = p_cmd;
        PSCTL1 = 0x1C;
        PSCTL1 = 0x1E;
    }
    else if(p_PortNum==0x01)
    {
        PSCTL2 = 0x1D;
        PSDAT2 = p_cmd;
        PSCTL2 = 0x1C;
        PSCTL2 = 0x1E;
    }
    else if(p_PortNum==0x02)
    {
        PSCTL3 = 0x1D;
        PSDAT3 = p_cmd;
        PSCTL3 = 0x1C;
        PSCTL3 = 0x1E;
    }
}

/**
 * ****************************************************************************
 * Let PS/2 Data pin keep low
 * Disable All PS/2 + Clear pending flags
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ClaerPS2IFPendingEvent(void)
{
    InhibitAUXDevice();

    /* Disable all PS2 interrupt */
    IER2&= ~(Int_PS2_0+Int_PS2_1+Int_PS2_2);

    /* Write to clear all PS2 pending interrupt */
    ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);

    /* Clear ps2 interrupt start bit flag */
    PS2StartBit = 0;

	/* Clear PS2 interrupt service flag */
    F_Service_PS2 = 0;
}

/**
 * ****************************************************************************
 * EC Send command to ps2 interface no ack to host
 *
 * @return
 *
 * @parameter
 * PortNum, 0 ~ 2
 * PortData
 *
 * ****************************************************************************
 */
void ECSend2Port( BYTE PortNum, BYTE PortData)
{
    DisableAllInterrupt();
    ClaerPS2IFPendingEvent();
	ECSendtoAUXFlag = 1;
    SendtoAUXFlag = 1;
	RAM_Send2Port(PortNum, PortData);
    EnableAllInterrupt();
    /* Enable interrupt */
    SET_MASK(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit); 
}

/**
 * ****************************************************************************
 * Before sending command to PS2 device to make sure no any data is done
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckNWaitReceiveDone(void)
{
    BYTE busy;
    busy=0x00;
        
    if(IS_MASK_SET(PSSTS1, SS)||IS_MASK_SET(PSSTS2, SS)||IS_MASK_SET(PSSTS3, SS)
    ||F_Service_PS2 == 1||PS2StartBit == 1)
    {
        /* To enable 30ms time-out timer */
        Enable_ETimer_T(30);

        while(1)
        {
            /* no time-out */
            if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
            {
                if(F_Service_PS2)
                {
                    busy=0x01;
                    break;
                }
            }
            else    /* time-out */
            {
                if(PS2CheckPendingISR()==0x01)
                {
                    busy=0x01;
                }
                break;
            }
        }

        if(busy==0x01)
        {
            F_Service_PS2=0;
            AUXInterfaceBusy=1;
            service_ps2();
            AUXInterfaceBusy=0;
        }

        /* disable timer1 */
        Stop_ETimer_T();
    }
    else
    {
        PS2DeviceInactive();
    }
}

/**
 * ****************************************************************************
 * Send data to ps2 interface
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Send2Port( BYTE PortNum, BYTE PortData, BYTE action)
{
    BYTE tempinactvietime;

    #if !TouchPad_only
        if(ECSendtoAUXFlag)
        {
            if(WaitECSend2PortACK(AUX_SCAN_INDEX)==0x01)
            {
                AUXDevicePlugOutClearVariables();
            }
        }
        else
        {
            if(action == SendCmdToKB)
            {
                CheckNWaitReceiveDone();
            }
        }
    #endif

    DisableAllInterrupt();
    ClaerPS2IFPendingEvent();

    /* reset command */
    if(PortData==0xFF)
    {
        /* 1.2sec */
        SetPS2InhibitTime(DeviceResetCmdITTime);
    }
    else
    {
        if(TPACKCounter!=0x00)
        {
            tempinactvietime = InactiveTime*TPACKCounter;
        }
        else if(Main_KB_ACK_CUNT!=0x00)
        {
            tempinactvietime = InactiveTime*Main_KB_ACK_CUNT;
        }
        else
        {
            tempinactvietime = InactiveTime;
        }
        SetPS2InhibitTime(tempinactvietime);
    }
    
    SetAUXScanIndexHS(AUXScanIndexFree);
    ECSendtoAUXFlag = 0;
    SendtoAUXFlag = 1;
	RAM_Send2Port(PortNum, PortData);
    EnableAllInterrupt();
    SET_MASK(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit);
    WaitPS2DeviceACK(PortNum);
}

/**
 * ****************************************************************************
 * Send command to ps2 interface and check ack or not
 *
 * @return
 * 0, Transaction done
 * 1, Transaction fail
 *
 * @parameter
 * PortNum
 * cmd
 * bytecunt
 *
 * @note
 * After this function, all ps2 channel is inhibit and channel interrupt
 * is disabled
 *
 * ****************************************************************************
 */
BYTE Send2PortNWait( BYTE PortNum, BYTE cmd, BYTE bytecunt)
{
    BYTE result;
    BYTE index;

    PS2IFAck = 0x00;

    DisableAllInterrupt();
    ClaerPS2IFPendingEvent();
    ECSendtoAUXFlag = 0;
    SendtoAUXFlag = 0;

	/*
	 * Set tramsmit mode & HW mode and send Cmd to PS/2 device
	 * Please refer to PS/2 interface -> HW Transmit Mode
	 */
	RAM_Send2Port(PortNum, cmd);
    EnableAllInterrupt();

    result = bExtAUXTimeOutCheck(PortNum, PS2_Transmission_Mode);

    /* Transaction done */
    if(result==0x00)
    {
        for(index=0x00;index<bytecunt;index++)
        {
            /* Get Last ACK by bytecunt */
            *asPS2Struct[PortNum].ps2ctrl=PS2_ReceiveMode;

            result = bExtAUXTimeOutCheck(PortNum, PS2_Receive_Mode);

            if(result!=0x00)
            {
                /* Transaction fail */
                break;
            }
        }
    }

    *asPS2Struct[PortNum].ps2ctrl=PS2_InhibitMode;

    /* Write to clear all PS2 pending interrupt */
    ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
  
    return(result);		                        
}

/**
 * ****************************************************************************
 * Process AUX command
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SendCmdtoMouse(BYTE PortNum)
{
    BYTE index;

    Cmd_A8();
    ResetMSPendingData();
    SetPS2CmdACKCounter(0);
    MS_ID_CUNT = 0x00;

	if(AUX2ByteCommand)
	{	
        SetPS2CmdACKCounter(1);
		if(TPResolution == 0xFF)
		{
			TPResolution = KBHIData;
		}
		else if(TPSamplingRate == 0xFF)
		{
			TPSamplingRate = KBHIData;		
		}

		Send2Port(PortNum, KBHIData, SendCmdToMS);	
		AUX2ByteCommand = 0;
		return;
	}

	switch(KBHIData)
	{
		case 0xE8:  /* Set Resolution (2 byte) */
			TPResolution = 0xFF;
			AUX2ByteCommand=1;
			break;
		
		case 0xF3:  /* Set Sampling Rate (2 byte) */
			TPSamplingRate = 0xFF;	
			AUX2ByteCommand=1;
			break;	
			
		case 0xF4:	
            #if PS2_Pack_Debug
            PS_Debug_ECToHost = 0xFF;
            #endif
			MouseDriverIn = 1;
			break;
			
		case 0xF5:
			MouseDriverIn = 0;
			break;	
			
		case 0xE6:
			break;
			
		case 0xE7:
			break;
			
		case 0xEA:
			break;
			
		case 0xEC:
			break;
			
		case 0xEE:
			break;
			
		case 0xF0:
			break;
			
		case 0xF6:		
			break;	
			
		case 0xE9:	
			break;
			
		case 0xF2:	
            MS_ID_CUNT = 2;
			break;
			
		case 0xEB:
		case 0xFE:	
			break;
			
		case 0xFF:
			MouseDriverIn = 0;
            /* Disable Activate Multiplex mode */
            MULPX_Multiplex = 0;

            for(index=0x00; index<3; index++)
            {
                if((Main_MOUSE_CHN-1)!=index)
                {
                    if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_MOUSE))
                    {
                        AuxFlags[index] = 0x00;
                    }
                }
            }
			break;

		default:    /* Invalid command */
			break;
	}

    if(KBHIData==0xFF)      /* Reset command */
    {
        SetPS2CmdACKCounter(3);
    }
    else if(KBHIData==0xF2) /* Read ID command */
    {
        SetPS2CmdACKCounter(2);
    }
    else if(KBHIData==0xE9) /* Read status cmmand */
    {
        SetPS2CmdACKCounter(4);
    }
    else                    /* Other commands */
    {
        SetPS2CmdACKCounter(1);
    }

	Send2Port(PortNum, KBHIData, SendCmdToMS);
}

/**
 * ****************************************************************************
 * Process AUX MUX command
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SendCmdtoMUX(BYTE port)
{
    SetPS2CmdACKCounter(0);
    ResetMSPendingData();
	if(AUX2ByteCommand)
	{	
        SetPS2CmdACKCounter(1);
		Send2Port(port, KBHIData, SendCmdToMS);	
		AUX2ByteCommand = 0;
		return;
	}

	switch(KBHIData)
	{
		case 0xE8:     		/* Set Resolution (2 byte) */
		case 0xF3:     		/* Set Sampling Rate (2 byte) */
			AUX2ByteCommand=1;
			break;	

		case 0xF4:	
		case 0xF5:	
		case 0xE6:
		case 0xE7:
		case 0xEA:
		case 0xEC:
		case 0xEE:
		case 0xF0:	
		case 0xF6:	
		case 0xE9:
		case 0xEB:
		case 0xF2:				
		case 0xFE:	
		case 0xE1:		    /* For older Synaptics TouchPads */		
		case 0xFF:
			break;

		default:		    /* Invalid command */
			break;
	}

    if(KBHIData==0xFF)      /* Reset command */
    {
        SetPS2CmdACKCounter(3);
    }
    else if(KBHIData==0xF2) /* Read ID command */
    {
        SetPS2CmdACKCounter(2);
    }
    else if(KBHIData==0xE9) /* Read status cmmand */
    {
        SetPS2CmdACKCounter(4);
    }
    else                    /* Other commands */
    {
        SetPS2CmdACKCounter(1);
    }

    Send2Port(port, KBHIData, SendCmdToMS); 
}

/**
 * ****************************************************************************
 * Init PS2 registers and variables.  
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Ps2_Init(void)
{

}

/**
 * ****************************************************************************
 * The function of PS2 interface interrupt service routine 
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_ps2(void)
{	
    BYTE channel;

    channel = PS2_IRQ_Channel;
    if(channel!=0xFF)
    {
        Hook_service_ps2(channel, PS2PortxData[channel]);
        PS2_IRQ_Channel = 0xFF;
        SendAUXData(channel);
    }
}

/**
 * ****************************************************************************
 * The function of stopping ps2 devices send data to host
 * Let Data Pin Keep Low
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PS2DeviceInactive(void)
{
	PSCTL1 = PS2_InhibitMode;
	PSCTL2 = PS2_InhibitMode;
	PSCTL3 = PS2_InhibitMode;
}

/**
 * ****************************************************************************
 * The function of starting ps2 devices send data to host
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PS2Deviceactive(void)
{
    BYTE index;
    
    for(index=0x00;index<3;index++)
    {
        /* keyboard or mouse */
        if(IS_MASK_SET(AuxFlags[index],(DEVICE_IS_KEYBOARD+DEVICE_IS_MOUSE)))
        {
            PS2ReceiveMode_OneChannel(index, 0x00);

            /* Delay 15.26 us */
            WNCKR = 0x00;
            /* Delay 15.26 us */
            WNCKR = 0x00;

            if(PS2StartBit || F_Service_PS2)
            {
                SetPS2InhibitTime(InactiveTime);
                break;
            }
        }
    }  
}

/**
 * ****************************************************************************
 * The function of starting one ps2 device send data to KBC
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PS2ReceiveMode_OneChannel(BYTE PortNum, BYTE SetInhibitTime)
{
    *asPS2Struct[PortNum].ps2ctrl=PS2_ReceiveMode;
    SET_MASK(*asPS2Struct[PortNum].ps2ier, asPS2Struct[PortNum].ctrlbit);

    if(SetInhibitTime==0x01)
    {
        SetPS2InhibitTime(InactiveTime);
    }
}

/**
 * ****************************************************************************
 * The function of inhibit PS2 device
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void InhibitAUXDevice(void)
{ 
	PS2DeviceInactive();	/* PS/2 Data = Low */

    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */

    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */

    WNCKR = 0x00;           /* Delay 15.26 us */
    WNCKR = 0x00;           /* Delay 15.26 us */
}

/**
 * ****************************************************************************
 * The function of scaning AUX device
 *
 * @return
 *
 * @parameter
 * 0 : scan all channel
 * 1 : scan mouse channel
 * 2 : scan keyboard channel
 *
 * ****************************************************************************
 */
void ScanAUXDevice(BYTE scan_selection)
{
	BYTE index;
    BYTE timeout;

    /* Scan mouse channel */
    if(scan_selection==ScanMouseChannel)
    {
        /* Main mouse is presetn */
        if(Main_MOUSE_CHN!=0x00)
        {
            return;
        }
    }
    /* Scan keyboard channel */
    else if(scan_selection==ScanKeyboardChannel)
    {
        /* Main keyboard is present */
        if(Main_KB_CHN!=0x00)
        {
            return;
        }
    }
   
	for(index=0x00;index<3;index++)
	{
        if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_MOUSE)||
            IS_MASK_SET(AuxFlags[index], DEVICE_IS_KEYBOARD))
        {
            continue;
        }
		AuxFlags[index] = 0x00;                 

		if(index==0x00)
		{
            /* If this channel isn't PS2 ALT function, scan next. */
			if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00||
                (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
				continue;        
		}
		else if(index==0x01)
		{
            /* If this channel isn't PS2 ALT function, scan next. */
			if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00||
                (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
				continue;   
		}
		else if(index==0x02)
		{
            /* If this channel isn't PS2 ALT function, scan next. */
			if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00||
                (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
				continue;   
		}

        if(Send2PortNWait(index,0xFF,1)==0x00)
        {
            if(PS2IFAck==0xFA)
            {
                SET_MASK(AuxFlags[index], DEVICE_IS_ATTACHED);

                *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
                /* 510 ms time-out */
                for(timeout=0x00;timeout<=ResetCmdTOTime;timeout++)
                {
                    if(!bExtAUXTimeOutCheck(index, PS2_Receive_Mode))
                    {
                        break;
                    }
                }

                /* wait aa time-out */
                if(timeout>ResetCmdTOTime)
                {
                    CLEAR_MASK(AuxFlags[index], DEVICE_IS_ATTACHED);
                }
                else
                {
                    *asPS2Struct[index].ps2ctrl=PS2_ReceiveMode;
                    /* 510 ms time-out */
                    for(timeout=0x00;timeout<=ResetCmdTOTime;timeout++)
                    {
                        if(!bExtAUXTimeOutCheck(index, PS2_Receive_Mode))
                        {
                            break;
                        }
                    }

                    /* wait 00 time-out */
                    if(timeout>ResetCmdTOTime)
                    {
                        SET_MASK(AuxFlags[index], DEVICE_IS_KEYBOARD);
                        CheckNSetMainKBCHN(index);
                        /* Scan keyboard channel */
                        if(scan_selection==ScanKeyboardChannel)
                        {
                            break;
                        }
                    }
                    else
                    {
                        /* Mouse */
                        if(PS2IFAck==0x00)
                        {
                            SET_MASK(AuxFlags[index], DEVICE_IS_MOUSE);
                            CheckNSetMainMSCHN(index);
                            /* Scan mouse channel */
                            if(scan_selection==ScanMouseChannel)
                            {
                                break;
                            }
                        }
                        else    /* Keyboard */
                        {
                            SET_MASK(AuxFlags[index], DEVICE_IS_KEYBOARD);
                            CheckNSetMainKBCHN(index);
                            /* Scan keyboard channel */
                            if(scan_selection==ScanKeyboardChannel)
                            {
                                break;
                            }
                        }
                    }
                }

                *asPS2Struct[index].ps2ctrl=PS2_InhibitMode;
                /* Write to clear all PS2 pending interrupt */
                ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
            }
            else
            {
                *asPS2Struct[index].ps2ctrl=PS2_InhibitMode;
                /* Write to clear all PS2 pending interrupt */
                ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
    			continue;   /* Device non-ack */
            }
	    }
        else
        {
            *asPS2Struct[index].ps2ctrl=PS2_InhibitMode;
            /* Write to clear all PS2 pending interrupt */
            ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
			continue;   /* Device non-ack */
        }       
	}

    /* No any device */
    if((Main_MOUSE_CHN==0x00)&&(Main_KB_CHN==0x00))
    {
        ClearAUXScanTimeOutCunt();
        SetAUXScanIndexHS(AUXScanIndexFree);
    }
}

/**
 * ****************************************************************************
 * The function of checking AUX device is keyborad or mouse
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckAUXDeviceType(void)
{
    BYTE timeout;

    if(AuxFlags[AUX_SCAN_INDEX] == DEVICE_NEED_INIT)
    {
        AuxFlags[AUX_SCAN_INDEX]=0x00;
        PS2IFAck = 0x00;

        if(Send2PortNWait(AUX_SCAN_INDEX,0xFF,1)==0x00)
        {
            if(PS2IFAck==0xFA)
            {
                SET_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_ATTACHED);

                *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_ReceiveMode;
                /* 510 ms time-out */
                for(timeout=0x00;timeout<=ResetCmdTOTime;timeout++)
                {
                    if(!bExtAUXTimeOutCheck(AUX_SCAN_INDEX, PS2_Receive_Mode))
                    {
                        break;
                    }
                }
                /* wait aa time-out */
                if(timeout>ResetCmdTOTime)
                {
                    CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_ATTACHED);
                }
                else
                {
                    *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_ReceiveMode;
                    /* 510 ms time-out */
                    for(timeout=0x00;timeout<=ResetCmdTOTime;timeout++)
                    {
                        if(!bExtAUXTimeOutCheck(AUX_SCAN_INDEX,
                            PS2_Receive_Mode))
                        {
                             break;
                        }
                    }
                    /* wait 00 time-out */
                    if(timeout>ResetCmdTOTime)
                    {
                        SET_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD);
                        CheckNSetMainKBCHN(AUX_SCAN_INDEX);      
                    }
                    else
                    {
                        /* Mouse */
                        if(PS2IFAck==0x00)
                        {
                            /* Device is mouse */
                            SET_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_MOUSE);
                            CheckNSetMainMSCHN(AUX_SCAN_INDEX);
                        }
                        /* Keyboard */
                        else
                        {
                            SET_MASK(AuxFlags[AUX_SCAN_INDEX],
                                DEVICE_IS_KEYBOARD);
                            CheckNSetMainKBCHN(AUX_SCAN_INDEX);
                        }
                    }
                }

                *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_InhibitMode;
                /* Write to clear all PS2 pending interrupt */
                ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);

                if(IS_MASK_SET(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_ATTACHED))
                {
                    SET_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
                    ScanAUXStepIndex = ScanAUXStep_Init + AUX_SCAN_INDEX;
                }
                else
                {
                    SetAUXScanIndexHS(AUXScanIndexFree);
                    ScanAUXStepIndex =
                        ScanAUXStep_CheckTypeFail + AUX_SCAN_INDEX;
                }
            }
            else
            {
                *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_InhibitMode;
                /* Write to clear all PS2 pending interrupt */
                ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
                SetAUXScanIndexHS(AUXScanIndexFree);
                ScanAUXStepIndex = ScanAUXStep_InitFail + AUX_SCAN_INDEX;
            }
        }
        else
        {
            *asPS2Struct[AUX_SCAN_INDEX].ps2ctrl=PS2_InhibitMode;
            /* Write to clear all PS2 pending interrupt */
            ISR2=(Int_PS2_0+Int_PS2_1+Int_PS2_2);
            SetAUXScanIndexHS(AUXScanIndexFree);
            ScanAUXStepIndex = ScanAUXStep_InitFail + AUX_SCAN_INDEX;
        }
        PS2Deviceactive();
    } 
}

/**
 * ****************************************************************************
 * Clear variables for device plug-out
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void AUXDevicePlugOutClearVariables(void)
{
    BYTE index;

    ClearAUXScanTimeOutCunt();
    SetAUXScanIndexHS(AUXScanIndexFree);
    AuxFlags[AUX_SCAN_INDEX] = 0x00;
    AuxFlags2[AUX_SCAN_INDEX] = 0x00;
    PS2_ReTryCunt = 0x00;
    SendtoAUXFlag = 0;	
    ECSendtoAUXFlag = 0;

    if(Main_MOUSE_CHN == (AUX_SCAN_INDEX+1))
    {
        Main_MOUSE_CHN = 0x00;
        for(index=0x00;index<3;index++)
        {
            if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_MOUSE))
            {
                SET_MASK(AuxFlags[index], DEVICE_IS_MAIN_MOUSE);
                Main_MOUSE_CHN = index + 1;
                break;
            }
        }   
    }

    if(Main_KB_CHN == (AUX_SCAN_INDEX+1))
    {
        Main_KB_CHN = 0x00;
        for(index=0x00;index<3;index++)
        {
            if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_KEYBOARD))
            {
                SET_MASK(AuxFlags[index], DEVICE_IS_MAIN_KB);
                Main_KB_CHN = index + 1;
                break;
            }
        }
    }
}

/**
 * ****************************************************************************
 * The polling aux device center
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void PollingAUXDeviceCenter(void)
{
    if(AUX_SCAN_INDEX_HS!=AUXScanIndexInUse)
    {
        SetAUXScanIndexHS(AUXScanIndexInUse);
        AUX_SCAN_INDEX++;
        if(AUX_SCAN_INDEX>2)
        {
            AUX_SCAN_INDEX=0x00;
        }
        ScanAUXStepIndex = ScanAUXStep_ScanPort+AUX_SCAN_INDEX;
    }
    else
    {
        if(ECSendtoAUXFlag)
        {
            AUX_SCAN_TIMEOUT_CUNT++;
            if(AUX_SCAN_TIMEOUT_CUNT>=CheckDeviceTimeOut)
            {
                AUXDevicePlugOutClearVariables();
                PS2Deviceactive();	
            }
        }
    }
}

/**
 * ****************************************************************************
 * The function of update external keyboard device
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
/* Command table */
const CBYTE UpdateConfigAUXKB[] = {0xF5, 0xF4, 0xF3, 0xED};

void UpdateExtKeyboard(BYTE portnum)
{
	BYTE index;

    for(index=0x00; index<sizeof(UpdateConfigAUXKB); index++)	
    {
        if(IS_MASK_SET(AuxFlags_KB_CMD[AUX_SCAN_INDEX], BIT(index)))
        {
            if(!Send2PortNWait(portnum,UpdateConfigAUXKB[index],1))
            {
                if(UpdateConfigAUXKB[index]==0xF3)
                {
                    Send2PortNWait(portnum,Save_Typematic,1);
                }
                else if(UpdateConfigAUXKB[index]==0xED)
                {
                    Send2PortNWait(portnum,(Led_Data&0x07),1);
                }
                CLEAR_MASK(AuxFlags_KB_CMD[AUX_SCAN_INDEX], BIT(index));
            }
        }
    }
}

/**
 * ****************************************************************************
 * The function of update aux keyboard device
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void UpdateAUXKBDevice(void)
{
    UpdateExtKeyboard(AUX_SCAN_INDEX);
    SetAUXScanIndexHS(AUXScanIndexFree);
}

/**
 * ****************************************************************************
 * The function of init, AUX device
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void InitAUXDevice(void)
{
    /* device is keyboard */
    if(IS_MASK_SET(AuxFlags[AUX_SCAN_INDEX], DEVICE_IS_KEYBOARD))
    {
        if(KeyboardDriverIn)
        {
            if(AuxFlags_KB_CMD[AUX_SCAN_INDEX]==0x00)
            {
                ConfigExtKeyboard(AUX_SCAN_INDEX);
            }
            else
            {
                UpdateExtKeyboard(AUX_SCAN_INDEX);
            }
            CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
        }
        else
        {
            
        }
    }
    else    /* device is mouse */
    {
        if(MouseDriverIn)
        {
	        if(MULPX_Multiplex)
	        {
                AUXData_To_Host_nWait(0xAA,AUX_SCAN_INDEX);
                AUXData_To_Host_nWait(0x00,AUX_SCAN_INDEX);
                CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
                SetPS2SkipScanTime(10);
	        }
            else
            {
                if(SetIntellimouse(MS_DRIVER_TYPE,AUX_SCAN_INDEX))
                {
                    if(ReadMouseID(AUX_SCAN_INDEX)==MS_DRIVER_TYPE)
                    {
                        if(ConfigExtMouse(AUX_SCAN_INDEX))
                        {
                            CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX],
                                DEVICE_NEED_CONFIG);
                        }
                        else
                        {
                            /* device error */
                            AuxFlags[AUX_SCAN_INDEX] = 0x00;
                        }
                    }
                    else
                    {
                        SET_MASK(AuxFlags[AUX_SCAN_INDEX],
                            DEVICE_MS_DRIVER_NO_MATCH);
                        CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX],
                            DEVICE_NEED_CONFIG);
                    }
                }
                else
                {
                    /* device error */
                    AuxFlags[AUX_SCAN_INDEX] = 0x00;
                }
            }
        }
        else
        {
	        if(MULPX_Multiplex)
	        {
                AUXData_To_Host_nWait(0xAA,AUX_SCAN_INDEX);
                AUXData_To_Host_nWait(0x00,AUX_SCAN_INDEX);
                CLEAR_MASK(AuxFlags[AUX_SCAN_INDEX], DEVICE_NEED_CONFIG);
	        }
        }
    }
    PS2Deviceactive();
    SetAUXScanIndexHS(AUXScanIndexFree);
}

/**
 * ****************************************************************************
 * The function of enabling/disabling TP by EC self.
 *
 * @return
 *
 * @parameter
 *
 * @note, If want to enable mouse device. Please ensrue "MouseDriverIn" is set.
 *
 * ****************************************************************************
 */
void TPOnlyLowLevelFunc(void)
{
    BYTE index;

    /* 10ms time base */
    if(StartENAUXDevice)
    {
        StartENAUXDevice = 0;

        /* PS2 data is pending */
        if(MSPending==0x33)
        {
            /* Exit */
            return;
        }

        if(TPACKCounter!=0x00)
        {
            return;
        }

        Hook_TPOnlyLowLevelFunc();

        if(AUXInactiveCounter!=0x00)
	    {
		    AUXInactiveCounter--;
            return;
	    }
        else
        {
            if(IS_MASK_SET(PSSTS1, SS)||IS_MASK_SET(PSSTS2, SS)||
                IS_MASK_SET(PSSTS3, SS)||F_Service_PS2)
            {
                return;
            }
        }

        if(AUXInactiveCounter==0x00)
        {
	        for(index=0x00;index<3;index++)
	        {
		        if(IS_MASK_SET(AuxFlags2[index], DEVICE_NEED_DISABLED))
		        {
    		        if(!Send2PortNWait(index,0xF5,1))
    		        {
        		        CLEAR_MASK(AuxFlags2[index], DEVICE_NEED_DISABLED);
            	        SET_MASK(AuxFlags2[index],DEVICE_IS_DISABLED);
    		        }
		        }
		        else if(IS_MASK_SET(AuxFlags2[index], DEVICE_NEED_ENABLED))
		        {
    		        if(!Send2PortNWait(index,0xF4,1))
    		        {
        		        CLEAR_MASK(AuxFlags2[index], DEVICE_NEED_ENABLED);
            	        SET_MASK(AuxFlags2[index],DEVICE_IS_ENABLED);
                        PS2ReceiveMode_OneChannel(index, 0x00);
    		        }
		        }
	        }
        }
    }
}

/**
 * ****************************************************************************
 * The function of scaning external AUX device
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ExternalAUXLowLevelFunc(void)
{
    BYTE ISR2Temp;

    /* 10ms time base */
    if(StartENAUXDevice)
    {        
        StartENAUXDevice = 0;

        /* PS2 data is pending */
        if(MSPending==0x33)
        {
            /* Exit */
            return;
        }
        
        EnableAUXDevice();

        /* PS2 channel in inhibit. */
        if(AUXInactiveCounter!=0x00)
        {
            /* Exit */
            return;
        }

        /* EC skip to scan PS2 interface */
        if(SKIP_AUX_SCAN_CUNT!=0x00)
        {
            return; 
        }   

        #if !PS2IFHotPlugHotSwap
            return;
        #endif

        if(IS_MASK_SET(PSSTS1, SS)||IS_MASK_SET(PSSTS2, SS)||
            IS_MASK_SET(PSSTS3, SS)||PS2StartBit==1)
        {
            AuxScanWDT++;
            /* Interface watch dog for hot-plug  (1sec). */
            if(AuxScanWDT>100)
            {
                AuxScanWDT = 0x00;
                PS2StartBit = 0;

                AUXDevicePlugOutClearVariables();
                PS2Deviceactive();

                if(IS_MASK_SET(PSSTS1, SS)||PS2_SSIRQ_Channel == 0)
                {
	                PSCTL1 = PS2_InhibitMode;
                }
                else if(IS_MASK_SET(PSSTS2, SS)||PS2_SSIRQ_Channel == 1)
                {
                    PSCTL2 = PS2_InhibitMode;
                }
                else if(IS_MASK_SET(PSSTS3, SS)||PS2_SSIRQ_Channel == 2)
                {
                    PSCTL3 = PS2_InhibitMode;
                }
            }
            return;
        }
        else
        {
            AuxScanWDT = 0x00;

            ISR2Temp = ISR2;
            ISR2Temp &= (Int_PS2_0+Int_PS2_1+Int_PS2_2);
            if(ISR2Temp !=0x00)
            {
                return;
            }
            else
            {
                /* Any ps2 channel has getten data. */
                if(F_Service_PS2)
                {
                    return;
                }
                else
                {
                    PS2DeviceInactive();
                }
            }
        }

        PollingAUXDeviceCenter();

        switch(ScanAUXStepIndex)
        {
            case ScanAUXStep_ScanPort0:   
            case ScanAUXStep_ScanPort1:
            case ScanAUXStep_ScanPort2:
                ScanAUXPortX();
                break;      

            case ScanAUXStep_CheckType0:
            case ScanAUXStep_CheckType1:
            case ScanAUXStep_CheckType2:
                CheckAUXDeviceType();
                break;

            case ScanAUXStep_Init0:
            case ScanAUXStep_Init1:
            case ScanAUXStep_Init2:
                InitAUXDevice();
                break;

            case ScanAUXStep_UpdateKB0:
            case ScanAUXStep_UpdateKB1:
            case ScanAUXStep_UpdateKB2:
                UpdateAUXKBDevice();
                break;

            case ScanAUXStep_CheckOK1:
            case ScanAUXStep_CheckOK2:
            case ScanAUXStep_CheckOK3:

            case ScanAUXStep_InitFail0:
            case ScanAUXStep_InitFail1:
            case ScanAUXStep_InitFail2:    

            case ScanAUXStep_CheckTypeFail0:
            case ScanAUXStep_CheckTypeFail1:
            case ScanAUXStep_CheckTypeFail2:

            case ScanAUXStep_Skip0:
            case ScanAUXStep_Skip1:
            case ScanAUXStep_Skip2:

            case ScanAUXStep_WaitPort0:
            case ScanAUXStep_WaitPort1:
            case ScanAUXStep_WaitPort2:

            default:
                break;
        } 
    }
}

/**
 * ****************************************************************************
 * The function of scaning AUX device step
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ScanAUXDeviceStep(void)
{
    /* 10ms time base */
    if(StartENAUXDevice)
    {
        /* rtos only */
        #ifndef EC_FEATURE_ONLY
        hal_suspend_all_thread();
        #endif
        
        #if TouchPad_only
        TPOnlyLowLevelFunc();
        #else
        ExternalAUXLowLevelFunc();
        #endif

        /* rtos only */
        #ifndef EC_FEATURE_ONLY
        hal_resume_all_thread();
        #endif
    }
}

/**
 * ****************************************************************************
 * The function of checking AUX device
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ScanAUXPortX(void)
{
    if(AUX_SCAN_INDEX_HS==AUXScanIndexInUse)
    {
        if(AUX_SCAN_INDEX==0x00)
        {
            if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00 ||
                (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
            {
                 SetAUXScanIndexHS(AUXScanIndexFree);
            }
        }
        else if(AUX_SCAN_INDEX==0x01)
        {
            if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00 ||
                (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
            {
                SetAUXScanIndexHS(AUXScanIndexFree);
            }
        }
        else if(AUX_SCAN_INDEX==0x02)
        {
            if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00 ||
                (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
            {
                 SetAUXScanIndexHS(AUXScanIndexFree);
            }
        }

        if(AUX_SCAN_INDEX_HS == AUXScanIndexFree)
        {
            ScanAUXStepIndex = ScanAUXStep_Skip + AUX_SCAN_INDEX;
        }
        else
        {
            ClearAUXScanTimeOutCunt();
            if(IS_MASK_SET(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_DISABLED))
            {
                ECSend2Port(AUX_SCAN_INDEX, 0xF5);
            }
            else if(IS_MASK_SET(AuxFlags2[AUX_SCAN_INDEX],DEVICE_NEED_ENABLED))
            {
                ECSend2Port(AUX_SCAN_INDEX, 0xF4);
            }
            else
            {
                ECSend2Port(AUX_SCAN_INDEX, 0xF1);
            }
            ScanAUXStepIndex = ScanAUXStep_WaitPort + AUX_SCAN_INDEX;
        }
    }
}

/**
 * ****************************************************************************
 * The function of enabling all PS2 interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void EnableAUXDevice(void)
{
	if(AUXInactiveCounter!=0x00)
	{
		if(--AUXInactiveCounter==0x00)
		{
			PS2Deviceactive();
		}
	}

    if(SKIP_AUX_SCAN_CUNT!=0x00)
    {
        SKIP_AUX_SCAN_CUNT--;
    }
}

/**
 * ****************************************************************************
 * The function of starting to check AUX device
 *
 * @return
 *
 * @parameter
 * channel, 0 ~ 2
 * p_mode, PS2_Transmission_Mode or PS2_Receive_Mode
 *
 * ****************************************************************************
 */
BYTE bExtAUXTimeOutCheck(BYTE channel, BYTE p_mode)
{
    BYTE result;

    /* pre-set resutl is fail */
    result = 0x01;

    /* To enable 30ms time-out timer */
    Enable_ETimer_T(30);

    /* Wait PS2 transaction Done Status */
	do
	{
        /* Wait PS2 transaction Done */
        if(IS_MASK_SET(*asPS2Struct[channel].ps2isr,
            asPS2Struct[channel].ctrlbit))
        {
            if(p_mode == PS2_Transmission_Mode)
            {
                for(PS2DataPinStatus=0x00;PS2DataPinStatus<5;PS2DataPinStatus++)
                {
                    if(channel==2)
                    {
                        /* DATA Line Status is high of ps2 port 2 */
                        if(IS_MASK_SET(GPDRF, BIT5))
                        {
                            break;
                        }
                    }
                    else if(channel==1)
                    {
                        /* DATA Line Status is high of ps2 port 1 */
                        if(IS_MASK_SET(GPDRF, BIT3))
                        {
                            break;
                        }
                    }
                    else if(channel==0)
                    {
                        /* DATA Line Status is high of ps2 port 0 */
                        if(IS_MASK_SET(GPDRF, BIT1))
                        {
                            break;
                        }
                    }
                    else
                    {

                    }

                    /* Delay 15.26 us */
                    WNCKR = 0x00;
                }
            }

            *asPS2Struct[channel].ps2ctrl=PS2_InhibitMode;
            PS2IFAck=*asPS2Struct[channel].ps2data;
            result = 0x00;

			/* Clear ISR */
            *asPS2Struct[channel].ps2isr = asPS2Struct[channel].ctrlbit;
            break;
        }
    /* waitting for overflow flag */
	}while(Check_ETimer_T_Overflow()==ExTimerNoOverflow);

    Stop_ETimer_T();
    
	return(result);
}

/**
 * ****************************************************************************
 * The function of setting external keyboard device
 *
 * @return
 *
 * @parameter
 * portnum, 0 ~ 2
 *
 * ****************************************************************************
 */
/* Command table */
const CBYTE ConfigAUXKB[] = {0xF3, 0xED, 0xF4 };

void ConfigExtKeyboard(BYTE portnum)
{   
	BYTE index;

    for(index=0x00; index<sizeof(ConfigAUXKB); index++)	
    {
        if(!Send2PortNWait(portnum,ConfigAUXKB[index],1))
        {
            if(ConfigAUXKB[index]==0xF3)
            {
                Send2PortNWait(portnum,Save_Typematic,1);
            }
            else if(ConfigAUXKB[index]==0xED)
            {
                Send2PortNWait(portnum,(Led_Data&0x07),1);
            }
        }
    }
}

/**
 * ****************************************************************************
 * The function of sending mouse enable command
 *
 * @return
 * 1 OK, 0 fail
 *
 * @parameter
 * port, 0 ~ 2
 *
 * ****************************************************************************
 */
BYTE EnableMouse(BYTE port)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xF4,1))
    {
        resault = 1;
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

/**
 * ****************************************************************************
 * The function of setting Resolution
 *
 * @return
 * 1 OK, 0 fail
 *
 * @parameter
 * port, 0 ~ 2
 * resolution
 *
 * ****************************************************************************
 */
BYTE SetResolution(BYTE port, BYTE resolution)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xE8,1))
    {
        if(!Send2PortNWait(port,resolution,1))
        {
            resault = 1;
        }
        else
        {
            resault = 0;
        }
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

/**
 * ****************************************************************************
 * The function of setting samplerate
 *
 * @return
 * 1 OK, 0 fail
 *
 * @parameter
 * port, 0 ~ 2
 * samplerate
 *
 * ****************************************************************************
 */
BYTE Setsamplerate(BYTE port, BYTE samplerate)
{
    BYTE resault;
    if(!Send2PortNWait(port,0xF3,1))
    {
        if(!Send2PortNWait(port,samplerate,1))
        {
            resault = 1;
        }
        else
        {
            resault = 0;
        }
    }
    else
    {
        resault = 0;
    }
    return(resault);
}

/**
 * ****************************************************************************
 * The Microsoft Intellimouse
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
const CBYTE Intellimouse_5Byte[3] = 
{	
    200,    200,    80
};	

const CBYTE Intellimouse_4Byte[3] = 
{	
    200,    100,    80
};

BYTE SetIntellimouse(BYTE DriverType, BYTE port)
{
    BYTE index;
    BYTE result;

    /* pr-set OK */
    result = 0x01;
    
    if(DriverType==3)
    {
        for(index=0x00;index<sizeof(Intellimouse_4Byte);index++)
        {
            if(Setsamplerate(port, Intellimouse_4Byte[index])==0x00)
            {
                result = 0x00;
                break;
            }
        }
    }
    else if(DriverType==4)
    {
        for(index=0x00;index<sizeof(Intellimouse_5Byte);index++)
        {
            if(Setsamplerate(port, Intellimouse_5Byte[index])==0x00)
            {
                result = 0x00;
                break;
            }
        }
    }
    return(result);
}

/**
 * ****************************************************************************
 * Read PS/2 device ID
 *
 * @return
 * ID of device
 *
 * @parameter
 * port, 0 ~ 2
 *
 * ****************************************************************************
 */
BYTE ReadMouseID(BYTE port)
{
    if(Send2PortNWait(port,0xF2,2)==0x00)
    {
        return(PS2IFAck);
    }

    return(0xFF);
}

/**
 * ****************************************************************************
 * The function of setting external mouse device
 *
 * @return
 *
 * @parameter
 * portnum, 0 ~ 2
 *
 * ****************************************************************************
 */
BYTE ConfigExtMouse(BYTE portnum)
{  
    BYTE result;
    result = 0x00;
    
    if(SetResolution(portnum,TPResolution))
    {
        if(EnableMouse(portnum))
        {
            result = 0x01;
        }
    }
    return(result);
}

/**
 * ****************************************************************************
 * The function of checking AUX Multiplexed modes
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE CheckAuxMux(void)
{
	BYTE current,data_byte;
 	current = 1;
	if (KBHIData == 0xF0)		 	/* Data from Host is F0h */
 	{  
       	MULPX_Mx_state2 = 1;
      	MULPX_Mx_state3 = 0;
      	current = 0;    			/* Do NOT clear State2 and State3 */
      	data_byte = 0xF0;
	}
  	else if (KBHIData == 0x56)		/* Data from Host is 56h */
	{ 
     	if (MULPX_Mx_state2) 		/* Current is State2 */
		{ 
         	MULPX_Mx_state2 = 0;
         	MULPX_Mx_state3 = 1;
        	current = 0;
			data_byte = 0x56;
     	}
	}
	else if (KBHIData == 0xA4)		/* Data from Host is A4h */
  	{  
   		if (MULPX_Mx_state3) 		/* Current is State3 */
		{  
       		MULPX_Multiplex = 1;	/* Enable Activate Multiplex mode */
          	data_byte = 0x11; 		/* Return Version number */
     	}
	}
 	else if (KBHIData == 0xA5)		/* Data from Host is A5h. */
	{   
   		if (MULPX_Mx_state3) 		/* Current is State3 */
		{  
          	MULPX_Multiplex = 0;	/* Reveret to Legacy Mode */
           	data_byte = 0x11;       /* Return version number */
      	}
  	} 
    else
    {
        data_byte = KBHIData;
    }

 	if (current) 
	{
      	MULPX_Mx_state2 = 0;		/* Clear State2 and State3 */
		MULPX_Mx_state3 = 0;
   	}
	return(data_byte);
}

/**
 * ****************************************************************************
 * The function of disable ps2 channel x
 *
 * @return
 *
 * @parameter
 * portnum --> ps2 channel
 * active  --> 1 : On, 0 : Off
 *
 * @note, If want to enable mouse device. Please ensrue "MouseDriverIn" is set.
 *
 * ****************************************************************************
 */
void OnOffPS2Channel_X(BYTE portnum, BYTE active)
{
    if(portnum==0x00)
    {
        if((PS2_Port0_CLK_CtrlReg&0xFB)!=0x00 ||
            (PS2_Port0_Data_CtrlReg&0xFB)!=0x00)
        {
            /* If this channel isn't PS2 ALT function. return; */
            return; 
        }                  
    }
    else if(portnum==0x01)
    {
	    if((PS2_Port1_CLK_CtrlReg&0xFB)!=0x00 ||
            (PS2_Port1_Data_CtrlReg&0xFB)!=0x00)
        {
            /* If this channel isn't PS2 ALT function. return; */
            return; 
        }    
    }
    else if(portnum==0x02)
    {
	    if((PS2_Port2_CLK_CtrlReg&0xFB)!=0x00 ||
            (PS2_Port2_Data_CtrlReg&0xFB)!=0x00)
        {
            /* If this channel isn't PS2 ALT function. return; */
            return;
        }     
    }

    if(IS_MASK_SET(AuxFlags[portnum], DEVICE_IS_MOUSE))
    {
        if(MouseDriverIn)
        {
            if(active == 0x00)
            {
                SET_MASK(AuxFlags2[portnum], DEVICE_NEED_DISABLED);
            }
            else if(active == 0x01)
            {
                SET_MASK(AuxFlags2[portnum], DEVICE_NEED_ENABLED);
            }
            PS2_ReTryCunt = 0x00;
        }
    }
    else
    {
        if(KeyboardDriverIn)
        {
            if(active == 0x00)
            {
                SET_MASK(AuxFlags2[portnum], DEVICE_NEED_DISABLED);
            }
            else if(active == 0x01)
            {
                SET_MASK(AuxFlags2[portnum], DEVICE_NEED_ENABLED);
            }
            PS2_ReTryCunt = 0x00;
        }
    }
}

/**
 * ****************************************************************************
 * KBCmdAckByteCunt
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void KBCmdAckByteCunt(BYTE ackcunt)
{
    Main_KB_ACK_CUNT = ackcunt;
}

/**
 * ****************************************************************************
 * SetOtherKBNeedUpdataFlag
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetOtherKBNeedUpdataFlag(BYTE KBCmd)
{
    BYTE index;

    for(index=0x00;index<3;index++)
    {
        if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_KEYBOARD))
        {
            if(Main_KB_CHN != (index+1))
            {
                if(IS_MASK_CLEAR(AuxFlags[index], DEVICE_NEED_CONFIG))
                {
                    if(KBCmd==0xED)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xED;
                    }
                    else if(KBCmd==0xF3)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF3;
                    }
                    else if(KBCmd==0xF4)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF4;
                    }
                    else if(KBCmd==0xF5)
                    {
                        AuxFlags_KB_CMD[index] |= KB_CMD_0xF5;
                    }      
                }
            }
        }
    } 
}

/**
 * ****************************************************************************
 * SetStartScanAUXFlag
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetStartScanAUXFlag(void)
{
    StartENAUXDevice = 1;
}

/**
 * ****************************************************************************
 * AUXData_To_Host_nWait
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void AUXData_To_Host_nWait(BYTE data_byte, BYTE portnum)
{
    if(portnum!=0xFF)
    {
        AUXPortNum = portnum;
    }
    SendFromAux(data_byte);

    /* To enable 26ms time-out timer */
    Enable_ETimer_T(26);

    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
	{
	    if(IS_MASK_CLEAR(KBHISR, OBF))
        {
            break;
        }   
        if(IS_MASK_SET(KBHISR, IBF))
        {
            break;
        }  
	}

    Stop_ETimer_T();
}

/**
 * ****************************************************************************
 * ReSendPS2PendingData
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ReSendPS2PendingData(void)
{
    if(MSPending==0x33)
    {
        F_Service_Send_PS2 = 1;
    }
}

/**
 * ****************************************************************************
 * CheckNSetMainKBCHN
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void CheckNSetMainKBCHN(BYTE PortNum)
{
    if(Main_KB_CHN == 0x00)
    {
        SET_MASK(AuxFlags[PortNum], DEVICE_IS_MAIN_KB);
        Main_KB_CHN = PortNum+1;
    }
}

/**
 * ****************************************************************************
 * CheckNSetMainMSCHN
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void CheckNSetMainMSCHN(BYTE PortNum)
{
    if(Main_MOUSE_CHN == 0x00)
    {
        SET_MASK(AuxFlags[PortNum], DEVICE_IS_MAIN_MOUSE);
        Main_MOUSE_CHN = PortNum+1;
    }
}

/**
 * ****************************************************************************
 * ResetMSPendingData
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ResetMSPendingData(void)
{
    MSPending = 0x88;
    F_Service_Send_PS2 = 0;
}

/**
 * ****************************************************************************
 * Disable port 0 ( send 0xF5 command to device)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void DisablePS2Port_0(void)
{
    OnOffPS2Channel_X(0,0);
}

/**
 * ****************************************************************************
 * Disable port 1 ( send 0xF5 command to device)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void DisablePS2Port_1(void)
{
    OnOffPS2Channel_X(1,0);
}

/**
 * ****************************************************************************
 * Disable port 2 ( send 0xF5 command to device)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void DisablePS2Port_2(void)
{
    OnOffPS2Channel_X(2,0);
}

/**
 * ****************************************************************************
 * Enable port 0 ( send 0xF4 command to device) 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void EnablePS2Port_0(void)
{
    OnOffPS2Channel_X(0,1);
}

/**
 * ****************************************************************************
 * Enable port 1 ( send 0xF4 command to device) 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void EnablePS2Port_1(void)
{
    OnOffPS2Channel_X(1,1);
}

/**
 * ****************************************************************************
 * Enable port 2 ( send 0xF4 command to device) 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void EnablePS2Port_2(void)
{
    OnOffPS2Channel_X(2,1);
}

/**
 * ****************************************************************************
 * Power off clock and data pins of all supported channel
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void PS2_PowerOff(void)
{
    BYTE index;
    ResetMSPendingData();
    ClaerPS2IFPendingEvent();

    ClearAUXScanTimeOutCunt();
    SetAUXScanIndexHS(AUXScanIndexFree);
    PS2_ReTryCunt = 0x00;
    SendtoAUXFlag = 0;	
    ECSendtoAUXFlag = 0;
    AUXInactiveCounter = 0;
    StartENAUXDevice = 0;

    for(index=0x00;index<3;index++)
    {
        AuxFlags[index] = 0x00;
        AuxFlags2[index] = 0x00;
    }
    
    if((PS2_Port0_CLK_CtrlReg&0xFB)==0x00 &&
        (PS2_Port0_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port0_CLK_CtrlReg = OUTPUT+PULL_DW;
        PS2_Port0_Data_CtrlReg = OUTPUT+PULL_DW;
        SET_MASK(AuxFlags2[0], DEVICE_POWER_DOWN);
    }
           
    if((PS2_Port1_CLK_CtrlReg&0xFB)==0x00 &&
        (PS2_Port1_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port1_CLK_CtrlReg = OUTPUT+PULL_DW;
        PS2_Port1_Data_CtrlReg = OUTPUT+PULL_DW;
        SET_MASK(AuxFlags2[1], DEVICE_POWER_DOWN);
    }
    
    if((PS2_Port2_CLK_CtrlReg&0xFB)==0x00 &&
        (PS2_Port2_Data_CtrlReg&0xFB)==0x00)
    {
        PS2_Port2_CLK_CtrlReg = OUTPUT+PULL_DW;
        PS2_Port2_Data_CtrlReg = OUTPUT+PULL_DW;
        SET_MASK(AuxFlags2[2], DEVICE_POWER_DOWN);
    }
}

/**
 * ****************************************************************************
 * Power on clock and data pins of all supported channel
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void PS2_PowerOn(void)
{
    PS2DeviceInactive();
    if(IS_MASK_SET(AuxFlags2[0], DEVICE_POWER_DOWN))
    {
        CLEAR_MASK(AuxFlags2[0], DEVICE_POWER_DOWN);
        PS2_Port0_CLK_CtrlReg = ALT;
        PS2_Port0_Data_CtrlReg = ALT;
    }

    if(IS_MASK_SET(AuxFlags2[1], DEVICE_POWER_DOWN))
    {
        CLEAR_MASK(AuxFlags2[1], DEVICE_POWER_DOWN);
        PS2_Port1_CLK_CtrlReg = ALT;
        PS2_Port1_Data_CtrlReg = ALT;
    }

    if(IS_MASK_SET(AuxFlags2[2], DEVICE_POWER_DOWN))
    {
        CLEAR_MASK(AuxFlags2[2], DEVICE_POWER_DOWN);
        PS2_Port2_CLK_CtrlReg = ALT;
        PS2_Port2_Data_CtrlReg = ALT;
    }    
}

/**
 * ****************************************************************************
 * Power on clock and data pins of all supported channel
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void PowerOffClearTPPendingData(void)
{
    ResetMSPendingData();
    ClaerPS2IFPendingEvent();
}

/**
 * ****************************************************************************
 * SetPS2CmdACKCounter
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetPS2CmdACKCounter(BYTE ackcunt)
{
    TPACKCounter = ackcunt;
}

/**
 * ****************************************************************************
 * Wait ECSend2Port transaction done  
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE WaitECSend2PortACK(BYTE PortNum)
{
    BYTE timeout;
    BYTE channel;

    timeout = 0x00;
    if(AuxFlags[PortNum] == 0x00)
    {
        return(timeout);
    }

    /* To enable 85ms time-out timer */
    Enable_ETimer_T(85);

    /* waitting for overflow flag */
    while(1)
    {
        /* Wait PS2 transaction Done Status */
        if(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
        {
            if(F_Service_PS2)
    	    {
                /* Send command to device and transmit done. */
    		    if(SendtoAUXFlag)
    		    {
                    /* Clear main service flag */
    			    F_Service_PS2 = 0;
    			    SendtoAUXFlag = 0;
                    /* Clear PS2 2 service flag */
                    PS2_IRQ_Channel = 0xFF;
    			    PS2ReceiveMode_OneChannel(PortNum, 0x00);
    			    ClearAUXScanTimeOutCunt();
    		    }
    		    else
    		    {
                    channel = PS2_IRQ_Channel;
                    /* Clear main service flag */
                    F_Service_PS2 = 0;
                    ECSendtoAUXFlag = 0;
                    /* Clear PS2 2 service flag */
                    PS2_IRQ_Channel = 0xFF;
                    ProcessECTestAUXACK(channel, 0);
                    break;
    		    }
    	    }
        }
        else
        {
            /* overflow */
            timeout = 0x01;
            break;
        }
    }   

    Stop_ETimer_T();

    return(timeout);
}

/**
 * ****************************************************************************
 * Wait Send2Port transaction done  
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void WaitPS2DeviceACK(BYTE PortNum)
{
    /* To enable 40ms time-out timer */
    Enable_ETimer_T(40);

    /* waitting for overflow flag */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
    {
        /* Wait PS2 transaction Done Status */
	    if(F_Service_PS2)
	    {
            /* Send command to device and transmit done. */
		    if(SendtoAUXFlag)
		    {
                /* Clear main service flag */
			    F_Service_PS2 = 0;
			    SendtoAUXFlag = 0;
                /* Clear PS2 2 service flag */
                PS2_IRQ_Channel = 0xFF;
			    PS2ReceiveMode_OneChannel(PortNum, 0x00);
			    ClearAUXScanTimeOutCunt();
		    }
		    else
		    {
                /* Receive transmit done. */
                break;
		    }
	    }
    }

    Stop_ETimer_T();                             
}

/**
 * ****************************************************************************
 * PS2CheckPendingISR 
 *
 * @return
 * 1 : PS2 TDS pending
 * 0 : No PS2 TDS pending
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE PS2CheckPendingISR(void)
{
    BYTE pending;

    /* Pre-clear pending flag. */
    pending = 0x00;

    if(IS_MASK_SET(PSSTS3, TDS))
    {
        if(IS_MASK_SET(IER2,Int_PS2_2)&&IS_MASK_CLEAR(ISR2,Int_PS2_2)&&
            (PSCTL3==PS2_ReceiveMode))
        {
            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_suspend_all_thread();
            #endif
            
            IRQ_INT18_PS2Interrupt2();
            pending=0x01;

            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_resume_all_thread();
            #endif
        }
    }
    else if(IS_MASK_SET(PSSTS2, TDS))
    {
        if(IS_MASK_SET(IER2,Int_PS2_1)&&IS_MASK_CLEAR(ISR2,Int_PS2_1)&&
            (PSCTL2==PS2_ReceiveMode))
        {
            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_suspend_all_thread();
            #endif
            
            IRQ_INT19_PS2Interrupt1();
            pending=0x01;

            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_resume_all_thread();
            #endif 
        }
    }
    else if(IS_MASK_SET(PSSTS1, TDS))
    {
        if(IS_MASK_SET(IER2,Int_PS2_0)&&IS_MASK_CLEAR(ISR2,Int_PS2_0)&&
            (PSCTL1==PS2_ReceiveMode))
        {
            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_suspend_all_thread();
            #endif
            
            IRQ_INT20_PS2Interrupt0();
            pending=0x01;

            /* rtos only */
            #ifndef EC_FEATURE_ONLY
            hal_resume_all_thread();
            #endif
        }
    }

    return(pending);
}

/**
 * ****************************************************************************
 * SetAUXScanIndexHS 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetAUXScanIndexHS(BYTE status)
{
    AUX_SCAN_INDEX_HS = status;
}

/**
 * ****************************************************************************
 * ClearAUXScanTimeOutCunt 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ClearAUXScanTimeOutCunt(void)
{
    AUX_SCAN_TIMEOUT_CUNT = 0x00;
}


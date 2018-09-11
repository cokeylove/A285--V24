/*
 * ****************************************************************************
 * TITLE: CORE_PORT6064.C - Standard 8042 Keyboard Controller Commands
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
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
 * Process Command/Data received from System via the KBC interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void service_pci1(void)
{
    if(IS_MASK_CLEAR(KBHISR,IBF))  
	{
		return;
    }

    /* command */
    if(KBHISR & C_D)
	{
        /* command start */
        KBHIStep = 0;
       	KbdNeedResponseFlag = 0;
       	#ifdef DEF_PCUSTOM_SERVICE_6064_CMD
		pcustom_service_6064_cmd();
       	#else
 		KBHICmd  = KBHIDIR;
 		#endif
        
        Core_64Port(KBHICmd);
        Hook_64Port(KBHICmd);
 	    if(KbdNeedResponseFlag) 
	    {
		    KbdNeedResponseFlag = 0;
		    KBC_DataToHost(KBHIReponse);
        }
	}
    else    /* data */
    {
        KBHIData2 = KBHIData1;    
        KBHIData1 = KBHIData;     
        KBHIData  = KBHIDIR;
	    
	    if(FastA20)
	    {
		    if((KBHIData&0x02)!= 0x00)
		    {
  			    Hook_A20ON();
		    }
		    else
		    {
    		    Hook_A20OFF();
		    }
		    FastA20=0;
		    KBHIStep = 0;
	    }
        else
        {
         	/* if need data */
       	    if(KBHIStep!=0x00)
    	    {
        	    Core_60Port(KBHICmd);
                Hook_60Port(KBHICmd);
			    KBHIStep --;
        	    if (KbdNeedResponseFlag) 
			    {
				    KbdNeedResponseFlag = 0;
				    KBC_DataToHost(KBHIReponse);
        	    }
     	    }
  		    else
    	    {
   				/* Enable auxiliary keyboard. */
    		    Cmd_AE();
    		
    	        #if PS2_KBCMD
                    #ifdef __UART_DEBUG__
                        printf("KB CMD : 0x%X\n",KBHIData);
                    #endif    
                #endif
        		/* Keyboard command */
     		    KeyboardCmd(KBHIData);
     	    }
	    }
    }

    if(IS_MASK_SET(KBHISR,P_IBF))
    {
  	    F_Service_PCI = 1;
    }
    else
    {
        /* enable interrupt again */
        EnableKBCIBFInt();
    }
}

/**
 * ****************************************************************************
 * Handle command 20h
 * Read command byte
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_20(void)	
{
	ResponseKBData(Ccb42);
}

/**
 * ****************************************************************************
 * Handle command 33h
 * Send Security Code On byte to host
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_33(void)	
{
	ResponseKBData(Pass_On);
}

/**
 * ****************************************************************************
 * Handle command 34h
 * Send Security Code Off byte to host
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_34(void)	
{
	ResponseKBData(Pass_Off);
}

/**
 * ****************************************************************************
 * Handle command 36h
 * Send Reject make code 1 to host
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_36(void)
{
	ResponseKBData(Pass_Make1);
}

/**
 * ****************************************************************************
 * Handle command 37h
 * Send Reject make code 2 to host
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_37(void)
{
	ResponseKBData(Pass_Make2);
}

/**
 * ****************************************************************************
 * Handle command 60h
 * Write command byte to KBC
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_60(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 73h
 * Write security Code On byte. 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_73(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 74h
 * Write security Code Off byte.  
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_74(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 76h
 * Write Password Code Discard 1  
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_76(void)
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 77h
 * Write Password Code Discard 2  
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_77(void)
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 90h
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_90(void)	
{
	SetHandleCmdData(1);
}	

/**
 * ****************************************************************************
 * Handle command 91h
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_91(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 92h
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_92(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command 93h
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_93(void)	
{
	SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command A4h
 * Test Password Installed
 *
 * @return, data to send to HOST (FA or F1)
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A4(void)	
{
    if(Flag.PASS_READY) 
	{
    	/* Return FA if password is loaded */
       	ResponseKBData(0xFA);
    }
    else 
	{
    	/* Return F1 if password not loaded */
       	ResponseKBData(0xF1);
    }
}

/**
 * ****************************************************************************
 * Handle command A5h
 * Load Password
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A5(void)
{
	Tmp_Load = 0x00;
	SetHandleCmdData(8);
}

/**
 * ****************************************************************************
 * Handle command A6h
 * Enable Password
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A6(void)
{
    if(Flag.PASS_READY)
    {  
        Gen_Info_PASS_ENABLE = 1;
        Pass_Buff_Idx = 0;
        Flag.SCAN_INH = 0;
		ResponseKBData(Pass_On);
    }
    else
    {
    	ResponseKBData(0x00);
    }
}

/**
 * ****************************************************************************
 * Handle command A7h
 * Disable Aux Device Interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A7(void)
{
    /* Disable auxiliary device (mouse)	*/
	Ccb42_DISAB_AUX = 1;
}

/**
 * ****************************************************************************
 * Handle command A8h
 * Enable Auxiliary Device Interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A8(void)
{
 	/* Enable aux device (mouse) */
	Ccb42_DISAB_AUX = 0;
}

/**
 * ****************************************************************************
 * Handle command A9h
 * Enable Test Aux Device Interface
 *
 * @return
 * 0 = no error
 * 1 = Clock line stuck low
 * 2 = Clock line stuck high
 * 3 = Data line stuck low
 * 4 = Data line stuck high
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A9(void)
{
    MULPX_Multiplex = 0;
	ResponseKBData(0x00);
}

/**
 * ****************************************************************************
 * Handle command AAh
 * Self Test
 *
 * @return, 0x55 to signify that the test passed
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AA(void)
{
    #if TouchPad_only
    /* Scan Mouse channel */
    ScanAUXDevice(ScanMouseChannel);
    #else
    /* Scan all PS2 channels */
    ScanAUXDevice(ScanAllPS2Channel);
    #endif

    /* MUX ps2 */
    MULPX_Multiplex = 0;

    Data_To_Host(0x55 );
    /* KBC_DataToHost(0x55); */
}

/**
 * ****************************************************************************
 * Handle command ABh
 * Test Keyboard Interface
 *
 * @return, test error code
 * 0 = no error
 * 1 = Clock line stuck low
 * 2 = Clock line stuck high
 * 3 = Data line stuck low
 * 4 = Data line stuck high
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AB(void)
{
	ResponseKBData(0x00);
}

/**
 * ****************************************************************************
 * Handle command ACh
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AC(void)
{

}

/**
 * ****************************************************************************
 * Handle command ADh
 * Disable Keyboard Interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AD(void)
{
    /* Disable auxiliary keyboard. */
    Ccb42_DISAB_KEY = 1;
    /* Inhibit scanner (internal keyboard). */
    Flag.SCAN_INH = 1;
    /* Lock scanner. */
    Lock_Scan();
    /* Start inhibit delay timer. */
    /* Load_Timer_B(); */
}

/**
 * ****************************************************************************
 * Handle command AEh
 * Enable Keyboard Interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AE(void)
{
    /* Enable auxiliary keyboard. */
   	Ccb42_DISAB_KEY = 0;
	/* Enable scanner (internal keyboard). */
   	Flag.SCAN_INH = 0;
}

/**
 * ****************************************************************************
 * Handle command AFh
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_AF(void)
{

}

/**
 * ****************************************************************************
 * Handle command C0h
 * Emulate reading the 8042 Input port and send data to the system
 *
 * @return
 *
 * @parameter
 *
 * @note
 * Byte from input port.
 * Bit 7 = Keyboard not inhibited via switch.
 * Bit 6 = Monochrome adapter (0 = Color/Graphics adapter).
 * Bit 5 = Manufacturing jumper not installed.
 * Bit 4 = Enable 2nd 256K of system board RAM.
 * Bit 3 =
 * Bit 2 =
 * Bit 1 = Auxiliary data in line (PS/2 only).
 * Bit 0 = Keyboard data in line (PS/2 only).
 *
 * ****************************************************************************
 */
void Cmd_C0(void)
{
	/* Just return the compatibility value for now. */
	ResponseKBData(0xBF);
}

/**
 * ****************************************************************************
 * Handle command D0h
 * Send 8042 Output port value to the system 
 * (emulates data since there¡¦s no real Output port)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D0(void)
{
	ResponseKBData(Read_Output_Port_2());
}

/**
 * ****************************************************************************
 * Handle command D1h
 * Only set/reset GateA20 line based on the System Data bit1.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D1(void)	
{	
	FastA20=1;
	/* SetHandleCmdData(1); */
}

/**
 * ****************************************************************************
 * Handle command D2h
 * Send data to the system as if it came from the keyboard.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D2(void)
{
    /* To enable 20ms time-out timer */
    Enable_ETimer_T(20);

    /* No overflow */
	while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
	{
        if(IS_MASK_SET(KBHISR,OBF))
        {
            continue;
        }

  	    if(IS_MASK_SET(KBHISR,IBF))
  		{
            /* Data Port */
            if(IS_MASK_CLEAR(KBHISR,C_D))
            {
                KBHIData = KBHIDIR;
                Data_To_Host(KBHIData);
            }
            /* To stop timer */
            Stop_ETimer_T();
            return;
  		}
    }

    /* To stop timer */
    Stop_ETimer_T();

    SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command D3h
 * Send data to the system as if it came from the auxiliary device.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D3(void)
{
    /* To enable 20ms time-out timer */
    Enable_ETimer_T(20);

    /* no overflow */
	while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
	{
        if(IS_MASK_SET(KBHISR,OBF))
        {
            continue;
        }
        
  	    if(IS_MASK_SET(KBHISR,IBF))
  		{
            /* data port */
            if(IS_MASK_CLEAR(KBHISR,C_D))
            {
                KBHIData = KBHIDIR;
                #if MultiplexedModes
	                Aux_Data_To_Host(CheckAuxMux());
                #else
                    Aux_Data_To_Host(KBHIData);
                #endif
            }
            /* To stop timer */
            Stop_ETimer_T();
            return;
  		}
    }
    /* To stop timer */
    Stop_ETimer_T();

    SetHandleCmdData(1);
}

/**
 * ****************************************************************************
 * Handle command D4h
 * Output next received byte of data from system to auxiliary device.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D4(void)
{
	SetHandleCmdData(1);
    /* MUX ps2 */
	MULPX_Multiplex = 0;
}

/**
 * ****************************************************************************
 * Handle command E0h
 * Reports the state of the test inputs
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_E0(void)			
{
	ResponseKBData(0x00);
}

/**
 * ****************************************************************************
 * Handle command Fxh
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_FX(void)
{
    /* Even command */
	if((KBHICmd % 2)==0x00)
    {
        /* CLEAR_MASK(KBHISR,SYSF);	*/
		Hook_A20ON();
		Hook_KBRSTON();
        /* Delay. */
		/* Microsecond_Delay(64); */
	    WNCKR = 0x00;
        WNCKR = 0x00;
        WNCKR = 0x00;
        WNCKR = 0x00;
        WNCKR = 0x00;
		Hook_KBRSTOFF();
    }
    /* Odd command do no thing */
}

/**
 * ****************************************************************************
 * Process command data 60h
 * Write command byte
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_60Data(void)
{
    /* Write the data. */
	Ccb42 = KBHIData;

	if(Ccb42_DISAB_KEY) 
	{
        /* Disable keyboard interface. */
    	Cmd_AD();
	}
	else 
	{
 		/* Enable keyboard interface. */
     	Cmd_AE();
 	}

	/* Put system flag bit in Status Reg. */
	if (Ccb42_SYS_FLAG)
  		SET_MASK(KBHISR,SYSF);
  	else
		CLEAR_MASK(KBHISR,SYSF);
}

/**
 * ****************************************************************************
 * Process command data 73h
 * Write Security Code On byte
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_73Data(void)
{
   	/* Write the data. */
 	Pass_On = KBHIData;
}

/**
 * ****************************************************************************
 * Process command data 74h
 * Write Security Code Off byte
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_74Data(void)
{
   	/* Write the data. */
 	Pass_Off = KBHIData;
}

/**
 * ****************************************************************************
 * Process command data 76h
 * Reject make code 1
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_76Data(void)
{
	/* Write the data. */
 	Pass_Make1 = KBHIData;
}

/**
 * ****************************************************************************
 * Process command data 77h
 * Reject make code 2
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_77Data(void)
{
	/* Write the data. */
 	Pass_Make2 = KBHIData;
}

/**
 * ****************************************************************************
 * Process command data 90h
 * MUX AUX port #0
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_90Data(void)
{
    if(PS2_MSCMD)
    {
        #ifdef __UART_DEBUG__
            printf("MS 90Port CMD : 0x%X\n",KBHIData);
        #endif
    }

    #if !TouchPad_only
        if(ECSendtoAUXFlag)
        {
            if(WaitECSend2PortACK(AUX_SCAN_INDEX)==0x01)
            {
                AUXDevicePlugOutClearVariables();
            }
        }

        SetPS2SkipScanTime(100);
    #endif

    if(KBHIData==0xFF)
    {
        ScanAUXDevice(ScanAllPS2Channel);
    }

    /* set error bit and AUX bit, source bits is 00 */
    KBHISR = 0x24;
    KBHICR &= 0xFC;
    /* If AUX IRQ bit of command is present	*/
    if(Ccb42_INTR_AUX)
    {
        /* Enable IRQ */
        SET_MASK(KBHICR,OBFMIE);
    }

    /* Emulate transmission delay times */
    DelayXms(5);
    /* timeout error */
    KBHIMDOR = 0xFC;
    ShortDelayAfterWriteDOR();
    Hook_Mouse_90Cmd(KBHIData);
}

/**
 * ****************************************************************************
 * Process command data 91h
 * MUX AUX port #1
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_91Data(void)
{
    if(PS2_MSCMD)
    {
        #ifdef __UART_DEBUG__
            printf("MS 91Port CMD : 0x%X\n",KBHIData);
        #endif
    }

    if(IS_MASK_SET(AuxFlags[0], DEVICE_IS_MOUSE))
    {
        /* Send command to channel 0 mouse */
        SendCmdtoMUX(0);
    }
    else
    {
	    KBHISR = 0x40;
        /* Emulate transmission delay times */
	    DelayXms(5);
        /* timeout error */
	    KBHIMDOR = 0xFC;
	    ShortDelayAfterWriteDOR();
    }

    Hook_Mouse_91Cmd(KBHIData);
}

/**
 * ****************************************************************************
 * Process command data 92h
 * MUX AUX port #2
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_92Data(void)
{
    if(PS2_MSCMD)
    {
        #ifdef __UART_DEBUG__
            printf("MS 92Port CMD : 0x%X\n",KBHIData);
        #endif
    }

    if(IS_MASK_SET(AuxFlags[1], DEVICE_IS_MOUSE))
    {
        /* Send command to channel 0 mouse */
	    SendCmdtoMUX(1);
    }
    else
    {
        KBHISR = 0x80;
        /* Emulate transmission delay times */
	    DelayXms(5);
        /* timeout error */
        KBHIMDOR = 0xFC;
        ShortDelayAfterWriteDOR();
    }

    Hook_Mouse_92Cmd(KBHIData);
}

/**
 * ****************************************************************************
 * Process command data 93h
 * MUX AUX port #3
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_93Data(void)
{
    if(PS2_MSCMD)
    {
        #ifdef __UART_DEBUG__
            printf("MS 93Port CMD : 0x%X\n",KBHIData);
        #endif
    }

    if(IS_MASK_SET(AuxFlags[2], DEVICE_IS_MOUSE))
    {
        /* Send command to channel 2 mouse */
	    SendCmdtoMUX(2);
    }
    else
    {
        /* source bits is 11 */
	    KBHISR = 0xC0;
        /* Emulate transmission delay times */
	    DelayXms(5);
	    KBHIMDOR = 0xFC;
        ShortDelayAfterWriteDOR();
    }

    Hook_Mouse_93Cmd(KBHIData);
}

/**
 * ****************************************************************************
 * Process command data A5h
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_A5Data(void)
{
	if((Tmp_Load == 0) && (KBHIData == 0))
	{
 		Flag.PASS_READY = 0;
	}
	else
	{
     	if(Tmp_Load < 8)
     	{
      		Pass_Buff[Tmp_Load] = KBHIData;
          	Tmp_Load++;
      	}

   		if(KBHIData == 0)
      	{   
        	/* Cmd_Byte = 0; */
         	Flag.PASS_READY = 1;
    	}
	}
}

/**
 * ****************************************************************************
 * Process command data D1h
 * set/reset GateA20 line based on the System Data bit1.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D1Data(void)
{
	if((KBHIData & 0x02) != 0x00)
	{
    	Hook_A20ON();
	}
	else
	{
     	Hook_A20OFF();
	}
}

/**
 * ****************************************************************************
 * Process command data D2h
 * Send data back to the system as if it came from the keyboard.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D2Data(void)
{
    Data_To_Host(KBHIData);
}

/**
 * ****************************************************************************
 * Process command data D3h
 * Send data back to the system as if it came from the auxiliary device.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D3Data(void)
{
    #if MultiplexedModes
	    Aux_Data_To_Host(CheckAuxMux());
    #else
        Aux_Data_To_Host(KBHIData);
    #endif
}

/**
 * ****************************************************************************
 * Process command data D4h
 * Output next received byte of data from system to auxiliary device.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Cmd_D4Data(void)
{
    if(PS2_MSCMD)
    {
        #ifdef __UART_DEBUG__
            printf("MS CMD : 0x%X\n",KBHIData);
        #endif
    }
    /* if is reset command */
    if(KBHIData==0xFF)
    {
        /* Scan mouse channel */
        ScanAUXDevice(ScanMouseChannel);
    }
    /* mouse device is attached */
    if(Main_MOUSE_CHN!=0x00)
    {
        CLEAR_MASK(AuxFlags[(Main_MOUSE_CHN-1)], DEVICE_NEED_INIT);
        CLEAR_MASK(AuxFlags[(Main_MOUSE_CHN-1)], DEVICE_NEED_CONFIG);
	    SendCmdtoMouse((Main_MOUSE_CHN-1));
    }
    else
    {
        /* Emulate transmission delay times */
	    DelayXms(20);
	    #ifdef Hook_ECACKMouseDriver
        Hook_ECRespondtoMouseDriver(KBHIData);
        #else
	    SendFromAux(0xFC);
        #endif
    }

    Hook_Mouse_D4Cmd(KBHIData);
}

/**
 * ****************************************************************************
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ResponseKBData(BYTE RData)
{
	KBHIReponse = RData;
	KbdNeedResponseFlag = 1;
}

/**
 * ****************************************************************************
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetHandleCmdData(BYTE step)
{
    KBHIStep = step;
}

/**
 * ****************************************************************************
 * Passes data sent from the Host (port 60 write) to the keyboard.
 *
 * @return
 *
 * @parameter, nKB60DAT = data byte to send
 *
 * @note
 *
 * ****************************************************************************
 */
void KeyboardCmd(BYTE nKB60DAT)
{
    BYTE ack,ack1,ack2,index,cmdbk;

    /* New keyboard command, clear ack counter */
    KBCmdAckByteCunt(0x00);
    ack = 0x00;
    ack1 = 0x00;
    ack2 = 0x00;

    /* Keyboard Command Received Data Process */
    if(KB_Command)
    {
        cmdbk = KB_Command;

        switch(KB_Command)
        {
            case 0xED:  /* Update LEDs command. */
                        Led_Data = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        /* Hook Oem On-board LED control */
                        OEM_Write_Leds(Led_Data);

                        /* Update scanner numlock state. */
                        Scanner_State_NUM_LOCK = Led_Data_NUM;
                        break;
            case 0xF0:  /* Set alternate scan codes. */
                        KB_CodeSet = nKB60DAT;
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            case 0xF3:  /* Set typematic rate/delay. */
                        PS2KB_Typematic = nKB60DAT;
                        Set_Typematic(PS2KB_Typematic);
                        ack = 0xFA;
                        KB_Command = 0x00;
                        break;
            default:    /* Unknown command request system resend */
                        ack = 0xFE; /* Resend */
                        /* KBCUnProcessCnt++; */
                        break;
        }

        if((cmdbk==0xF3)||(cmdbk==0xED))
        {
            KeyboardDriverIn = 1;
        }

        /* no any external keyboard */
        if(Main_KB_CHN ==0x00)
        {
            /* if need send ack to host */
            if(ack != 0x00)
            {
                /* send to host */
                KBC_DataToHost(ack);
                #if PS2_KBCMD
                    #ifdef __UART_DEBUG__
                    printf("EC KB ACK : 0x%X\n",ack); 
                    #endif
                #endif
            }
        }
        else    /* Send command to external keyboard */
        {

            CLEAR_MASK(AuxFlags[(Main_KB_CHN-1)], DEVICE_NEED_INIT);
            CLEAR_MASK(AuxFlags[(Main_KB_CHN-1)], DEVICE_NEED_CONFIG);

            /* set ec need send one byte to host */
            KBCmdAckByteCunt(0x01);

            if(cmdbk==0xED)
            {
	            Send2Port((Main_KB_CHN-1), (nKB60DAT&0x07), SendCmdToKB);
            }
            else
            {
	            Send2Port((Main_KB_CHN-1), nKB60DAT, SendCmdToKB);
            }

            if((cmdbk==0xF3)||(cmdbk==0xED))
            {
                SetOtherKBNeedUpdataFlag(cmdbk);   
            } 
        }

        Hook_Keyboard_Cmd_Parameter(nKB60DAT);
        return;
    }
    
    /* Keyboard Command Process */
    switch(nKB60DAT)
    {
        KB_Command = 0x00;
        case 0xED:  /* Update LEDs command. */
                    ack = 0xFA;
                    KB_Command = 0xED;
                    break;
        case 0xEC:
                    ack = 0xFA;
                    break;

        case 0xEE:  /* ECHO command. */
                    ack = 0xEE;
                    break;

        case 0xF0:  /* Set alternate scan codes. */
                    ack = 0xFA;
                    KB_Command = 0xF0;
                    break;

        case 0xF2:  /* Read manufacturers ID */
                    ack = 0xFA;
                    ack1 = 0xAB;
                    if( Ccb42_XLATE_PC )
                    {
                        ack2 = 0x83;
                    }
                    else
                    {
                        ack2 = 0x41;
                    }
                    break;

        case 0xF3:  /* Set typematic rate/delay. */
                    ack = 0xFA;
                    KB_Command = 0xF3;
                    break;

        case 0xF4:  /* Enable scanning. */
                    KeyboardDriverIn = 1;
                    Clear_Key();
                    ack = 0xFA;
                    break;

        case 0xF5:  /* Default disable. */
                    KeyboardDriverIn = 0;
                    Clear_Key();
                    Clear_Typematic();
                    ack = 0xFA;
                    break;

        case 0xF6:  /* Set defaults. */
                    Clear_Key();
                    Clear_Typematic();
                    ack = 0xFA;
                    break;
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
                    ack = 0xFA;
                    break;

        case 0xFF:  /* Reset keyboard. */
                    KeyboardDriverIn = 0;
                    /* Scan keyboard channel */
                    ScanAUXDevice(ScanKeyboardChannel);
                    Clear_Key();
                    Clear_Typematic();
#if KB_FnSticky_Key
//if S3 resume and in sticky mode/sticky lock  don't clear fn status
                   	if ((IS_MASK_SET(CURRENT_STATUS, RESUME_S3)) && (STICKY_MODE_GET || STICKY_LOCK_GET))
	                  {  
		                  Scanner_State &= 0x40; //Scanner_State_FN        u_Scanner_State.fbit.bit6
	                  }else{
                      Scanner_State = 0;    
	                  }
#else
                    Scanner_State = 0;               
#endif //KB_FnSticky_Key
                    Clear_Fn_Keys();
                    ack = 0xFA;
                    ack1 = 0xAA;

                    for(index=0x00; index<3; index++)
                    {
                        if((Main_KB_CHN-1)!=index)
                        {
                            if(IS_MASK_SET(AuxFlags[index], DEVICE_IS_KEYBOARD))
                            {
                                AuxFlags[index] = 0x00;
                            }
                        }
                    }

                    #if KB_FnStickKey
                    FnStickKey = 0;
                    #endif

                    break;

        default:    /* Unknown command request system resend */
                    ack = 0xFE;
                    /* KBCUnProcessCnt++; */
                    /* KBCUnProcessRec = nKB60DAT; */
                    break;

    }

    /* no any external keyboard */
    if(Main_KB_CHN ==0x00)
    {
        KBCmdAckByteCunt(0x00);

        if(ack != 0x00)
        {
            KBC_DataToHost(ack);

            #if PS2_KBCMD
                #ifdef __UART_DEBUG__
                printf("EC KB ACK : 0x%X\n",ack);  
                #endif
            #endif

            if(ack1 != 0x00)
            {
                KBC_DataPending(ack1);

                #if PS2_KBCMD
                    #ifdef __UART_DEBUG__
                    printf("EC KB ACK : 0x%X\n",ack1);
                    #endif
                #endif

                if(ack2 != 0x00)
                {
                    KBC_DataPending(ack2);

                    #if PS2_KBCMD
                        #ifdef __UART_DEBUG__
                        printf("EC KB ACK : 0x%X\n",ack2);   
                        #endif
                    #endif 
                }
            }
        }
    }
    else    /* Send command to external keyboard */
    {
        CLEAR_MASK(AuxFlags[(Main_KB_CHN-1)], DEVICE_NEED_INIT);
        CLEAR_MASK(AuxFlags[(Main_KB_CHN-1)], DEVICE_NEED_CONFIG);

        /* reset command */
        if(nKB60DAT==0xFF)
        {
            KBCmdAckByteCunt(0x02);
        }
        else
        {
            /* read keyboard ID command */
            if(nKB60DAT==0xF2)
            {
                KBCmdAckByteCunt(0x03);
            }
            else if((nKB60DAT==0xF3)||(nKB60DAT==0xED))
            {
                KBCmdAckByteCunt(0x02);
            }
            else    /* other */
            {
                KBCmdAckByteCunt(0x01);
            }
        }

	    Send2Port((Main_KB_CHN-1), nKB60DAT, SendCmdToKB);

        if((nKB60DAT==0xF4)||(nKB60DAT==0xF5))
        {
            SetOtherKBNeedUpdataFlag(nKB60DAT);   
        }  
    }

    Hook_Keyboard_Cmd(nKB60DAT);
}

/**
 * ****************************************************************************
 * Set_Overlay_Mask
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE Set_Overlay_Mask(BYTE data_byte, FLAG bit_state)
{
    BITS_8 temp;
    temp.byte = data_byte;

    if(Led_Ctrl_NUM)
    {
        temp.fbit.NUM = bit_state;
    }

    if(Led_Ctrl_OVL)
    {
        temp.fbit.OVL = bit_state;
    }

    data_byte = temp.byte;
    Scanner_State_NUM_LOCK = bit_state;
    /* Ext_Cb3_OVL_STATE = bit_state; */

    return(data_byte);
}

/**
 * ****************************************************************************
 * Read_Output_Port_2
 * Read 8042 output port 2.
 *
 * @return
 * Byte from output port.
 *     Bit 7 = Keyboard data output line (inverted on PS/2).
 *     Bit 6 = Keyboard clock output line.
 *     Bit 5 = Input buffer empty (auxiliary interrupt IRQ12 on PS/2).
 *     Bit 4 = Output buffer full (Generates IRQ1).
 *     Bit 3 = Reserved (inverted auxiliary clock output line on PS/2).
 *     Bit 2 = Reserved (inverted auxiliary data output line on PS/2).
 *     Bit 1 = Gate A20.
 *     Bit 0 = System reset.
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE Read_Output_Port_2(void)
{
    BYTE p2byte;

    /*
     * "data" will hold bits read from different inputs.
     * Start with bit 0 set and set bits 2 and 3 for AT mode.
     * If PS2 mode is being used, bits 2 and 3 will be changed.
     */
    p2byte = (1 << 3) | (1 << 2) | (1 << 0);

    return (p2byte);
}

/**
 * ****************************************************************************
 * The function of processing 60 port
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Core_60Port(BYTE p_KBHIData)
{
    switch(p_KBHIData)
    {
        case 0x60:
        Cmd_60Data();
        break;

        case 0x73:
        Cmd_73Data();
        break;

        case 0x74:
        Cmd_74Data();
        break;

        case 0x76:
        Cmd_76Data();
        break;

        case 0x77:
        Cmd_77Data();
        break;

        case 0x90:
        Cmd_90Data();
        break;

        case 0x91:
        Cmd_91Data();
        break;

        case 0x92:
        Cmd_92Data();
        break;

        case 0x93:
        Cmd_93Data();
        break;

        case 0xA5:
        Cmd_A5Data();
        break;

        case 0xD1:
        Cmd_D1Data();
        break;

        case 0xD2:
        Cmd_D2Data();
        break;

        case 0xD3:
        Cmd_D3Data();
        break;

        case 0xD4:
        Cmd_D4Data();
        break;

        default:
        break;
    }
}

/**
 * ****************************************************************************
 * The function of processing 64 port
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Core_64Port(BYTE p_KBHICmd)
{
    switch(p_KBHICmd)
    {
        case 0x20:
        Cmd_20();
        break;

        case 0x33:
        Cmd_33();
        break;

        case 0x34:
        Cmd_34();
        break;

        case 0x36:
        Cmd_36();
        break;

        case 0x37:
        Cmd_37();
        break;

        case 0x60:
        Cmd_60();
        break;

        case 0x73:
        Cmd_73();
        break;

        case 0x74:
        Cmd_74();
        break;

        case 0x76:
        Cmd_76();
        break;

        case 0x77:
        Cmd_77();
        break;

        case 0x90:
        Cmd_90();
        break;

        case 0x91:
        Cmd_91();
        break;

        case 0x92:
        Cmd_92();
        break;

        case 0x93:
        Cmd_93();
        break;

        case 0xA4:
        Cmd_A4();
        break;

        case 0xA5:
        Cmd_A5();
        break;

        case 0xA6:
        Cmd_A6();
        break;

        case 0xA7:
        Cmd_A7();
        break;

        case 0xA8:
        Cmd_A8();
        break;

        case 0xA9:
        Cmd_A9();
        break;

        case 0xAA:
        Cmd_AA();
        break;

        case 0xAB:
        Cmd_AB();
        break;

        case 0xAC:
        Cmd_AC();
        break;

        case 0xAD:
        Cmd_AD();
        break;

        case 0xAE:
        Cmd_AE();
        break;

        case 0xAF:
        Cmd_AF();
        break;

        case 0xC0:
        Cmd_C0();
        break;

        case 0xD0:
        Cmd_D0();
        break;

        case 0xD1:
        Cmd_D1();
        break;

        case 0xD2:
        Cmd_D2();
        break;

        case 0xD3:
        Cmd_D3();
        break;

        case 0xD4:
        Cmd_D4();
        break;

        case 0xE0:
        Cmd_E0();
        break;

        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF:
        Cmd_FX();
        break;
        
        default:
        break;
    }
}

/**
 * ****************************************************************************
 * TShort delay after write data to output register of 64h port.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ShortDelayAfterWriteDOR(void)
{
    /* Delay 15.26 us */
    WNCKR = 0x00;
    /* Delay 15.26 us */
    WNCKR = 0x00;
}


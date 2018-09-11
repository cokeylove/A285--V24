/*
 * ****************************************************************************
 * TITLE: CORE_ACPI.C - ACPI Handler
 *
 * SCI event generation.
 * ACPI SMBus transfer initiated by ACPI command.
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
 * Process Command/Data received from System via the 2nd Host interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void service_pci2(void)
{
    if ( IS_MASK_CLEAR(PM1STS,P_IBF) )  
    {
		return; 
    }

	/* Set Burst mode total time (2ms) */
	SetTotalBurstTime();
    /* Process Command/Data */
 	service_pci2f();	
}

/**
 * ****************************************************************************
 * Process Command/Data received from System via the 2nd Host interface
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void service_pci2f(void)
{
	while(1)
	{
        if(IS_MASK_SET(PM1STS,P_IBF) ) 
        {
			/* Command port */
    		if ( PM1STS & P_C_D )
     		{
     			#ifdef DEF_PCUSTOM_SERVICE_ACPI_CMD
     			pcustom_service_acpi_cmd();
     			#else
                /* Load command */
       			PM1Cmd  = PM1DI;
       			#endif
            	PM1Step =0x00;				
    			Core_66Port(PM1Cmd);
                Hook_66Port(PM1Cmd);
      		}
      		else    /* Data port */
     		{
                /* Load data */
                PM1Data2 = PM1Data1;
                PM1Data1 = PM1Data;
       			PM1Data = PM1DI;
    			if (PM1Step!=0x00)
    			{
                    #ifdef DEF_CUSTOM_SERVICE_ACPI_DATA
                    /* Handle command data */
    				Core_62Port(PM1Cmd);
    				Hook_62Port(PM1Data);
                    #else
                    /* Handle command data */
    				Core_62Port(PM1Cmd);
    				Hook_62Port(PM1Cmd);
                    #endif
    				PM1Step--;
    			}
      		}

            if(IS_MASK_SET(PM1STS,BURST))
            {
                /* time-out */
                if(CheckBurstMode()==0x00)
                {
                    break;
                }
                else
                {

                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
	}
    
	EnableAllInterrupt();

    if(IS_MASK_SET(PM1STS,P_IBF))
    {
  	    F_Service_PCI2 = 1;
    } 
    else
    {
	    EnablePMCIBFInt();
    }
	//Hook_ACPICommand();	// // A485D00023:Modify Hook_ACPICommand() hook point
}

/*
 * ****************************************************************************
 * Handle ACPI commands.
 * ****************************************************************************
 */
/**
 * ****************************************************************************
 * Handle ACPI command 80h.
 * Read Embedded Controller.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_80(void)
{
	ACPI_Gen_Int(ACPICmdSCI);
	PM1Step =0x01;
}

/**
 * ****************************************************************************
 * Handle ACPI command 81h.
 * Write Embedded Controller.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_81(void)
{
 	ACPI_Gen_Int(ACPICmdSCI);
  	PM1Step =0x02;
}

/**
 * ****************************************************************************
 * Handle ACPI command 82h.
 * Burst Enable Embedded Controller.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_82(void)
{
   SET_MASK(SYS_MISC1,ACPI_OS);
    /* PM1STS.4 Set Burst mode flag */
	SET_MASK(PM1STS,BURST);
    /* Get Burst Acknowledge byte to send to Host. */
 	SendFromAcpi(0x90);
}

/**
 * ****************************************************************************
 * Handle ACPI command 83h.
 * Burst Disable Embedded Controller.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_83(void)
{
 	/* PM1STS.4 Set Burst mode flag */
	CLEAR_MASK(PM1STS,BURST);
    /* Clear Burst bit in secondary Host interface status register. */
 	ACPI_Gen_Int(ACPICmdSCI);
}

/**
 * ****************************************************************************
 * Handle ACPI command 84h.
 * Query Embedded Controller.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_84(void)
{
	#ifdef DEF_PCUSTOM_SERVICE_ACPI_CMD
	Oem_ACPI_Cmd_84();
	#endif
  ITempB05 = 0x00;
  if (SCI_Event_Out_Index != SCI_Event_In_Index)
  {
    ITempB05 = SCI_Event_Buffer[SCI_Event_Out_Index];
      SCI_Event_Out_Index++;
      if (SCI_Event_Out_Index >= EVENT_BUFFER_SIZE)
    {
             SCI_Event_Out_Index = 0;
       }
       if (SCI_Event_Out_Index == SCI_Event_In_Index)
           CLEAR_MASK(PM1STS,SCIEVT);

    SendFromAcpi(ITempB05);
  }
  else
  {
    CLEAR_MASK(PM1STS,SCIEVT);
  }
}

/**
 * ****************************************************************************
 * Handle ACPI command 92h.
 * Oem function of reading EC external ram
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_92(void)
{
	PM1Step = 0x02;
}

/**
 * ****************************************************************************
 * Handle ACPI command 93h.
 * Oem function of writing EC external ram
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_93(void)
{
	PM1Step =0x03;
}

/**
 * ****************************************************************************
 * Handle ACPI command DCh.
 * EC flash utility
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_DC(void)
{
	ITE_Flash_Utility();
}

/**
 * ****************************************************************************
 * Handle ACPI command F0h.
 * Keyboard matrix utility
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_F0(void)
{
	KeyScanACK = KU_ACK_Start;
}

/**
 * ****************************************************************************
 * Handle ACPI command data.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SendFromAcpi(BYTE ecdata)
{
	PM1DO = ecdata;
 	ACPI_Gen_Int(ACPICmdSCI);
}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 80
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_80Data(void)
{
	if (PM1Step == 1)
	{
		SendFromAcpi(Hook_ReadMapECSpace(PM1Data));
        ECCheckBurstMode = 0;
	}
}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 81
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_81Data(void)
{
 	ACPI_Gen_Int(ACPICmdSCI);
  	if (PM1Step == 2)
	{
     	PM1Data1=PM1Data;  		
	}
	else if(PM1Step == 1)
	{
		Hook_WriteMapECSpace(PM1Data1,PM1Data);
        ECCheckBurstMode = 0;
 	}
}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 82
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_82Data(void)
{

}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 83
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_83Data(void)
{

}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 84
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_84Data(void)
{

}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 92
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_92Data(void)
{
	switch(PM1Step)
	{
		case 2:
            /* address high */
			PM1Data1=PM1Data;
			break;

		case 1:
			Read_External_Ram();
			break;

		default:
			break;
	}
}

/**
 * ****************************************************************************
 * Handle the data of ACPI command 93
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Cmd_93Data(void)
{
	if (PM1Step==0x03)
	{
        /* address high */
   		PM1Data2=PM1Data;
	}
	else if (PM1Step==0x02)
	{
        /* address low */
      	PM1Data1=PM1Data;
	}
	else if (PM1Step==0x01)
	{
    	Write_External_Ram();
	}
}

/**
 * ****************************************************************************
 * Read external ram Space 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Read_External_Ram()
{
    BYTE *Tmp_XPntr;
	BYTE tmp;

	tmp = PM1Data1&0xF0;

    if(((PM1Data1<<8)+PM1Data) < 0x1000)
    {
        Tmp_XPntr=(BYTE *)((PM1Data1<<8)+PM1Data+EC_RAMBase);
    }
    else
    {
    	if((tmp == 0xD0) || (tmp == 0xE0))
		{
			Tmp_XPntr=(BYTE *)(((PM1Data1-0xD0)<<8)+PM1Data+EC_RAMBase);
		}
		else
		{
			Tmp_XPntr=(BYTE *)((PM1Data1<<8)+PM1Data+EC_Register_Base_Address);
		}
    }

    PM1DO = *Tmp_XPntr;
}

/**
 * ****************************************************************************
 * Write external ram Space 
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Write_External_Ram()
{
    BYTE *Tmp_XPntr;
	BYTE tmp;
	
	tmp = PM1Data2&0xF0;
	
    if(((PM1Data2<<8)+PM1Data1) < 0x1000)
    {
        Tmp_XPntr=(BYTE *)((PM1Data2<<8)+PM1Data1+EC_RAMBase);
    }
    else
    {
    	if((tmp == 0xD0) || (tmp == 0xE0))
		{
			Tmp_XPntr=(BYTE *)(((PM1Data2-0xD0)<<8)+PM1Data1+EC_RAMBase);
		}
		else
		{
			Tmp_XPntr=(BYTE *)((PM1Data2<<8)+PM1Data1+EC_Register_Base_Address);
		}
    }

    *Tmp_XPntr=PM1Data;
}

/**
 * ****************************************************************************
 * Burst mode or normal mode
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE CheckBurstMode(void)
{
    /* wait input buffer full */
	while(IS_MASK_CLEAR(PM1STS,P_IBF))
	{
        /* time out */
        if(BurstModeInstructionDelay==0)
        {
		    CLEAR_MASK(PM1STS,BURST);
	        ACPI_Gen_Int(ACPICmdSCI);
            ECCheckBurstMode = 0;
			return(0);
        }
        else
        {
            BurstModeInstructionDelay--;

            /* Delay 15.26 us */
            WNCKR = 0x00;
            WNCKR = 0x00;
        }
	}
	return(1);
}

/**
 * ****************************************************************************
 * Set Burst mode total time (2ms)
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetTotalBurstTime(void)
{
    DisableAllInterrupt();
    BurstModeInstructionDelay = 140;
}

/**
 * ****************************************************************************
 * Generate an SCI interrupt.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void ACPI_Gen_Int(BYTE Qevent)
{
  if( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
  {
    if(Qevent==QeventSCI)
    {
        DisableAllInterrupt();
    }
	Hook_SCION();
    /* Delay 15.26 us */
    WNCKR = 0x00;
	Hook_SCIOFF();
    if(Qevent==QeventSCI)
    {
        EnableAllInterrupt();
    }
  }
}

/**
 * ****************************************************************************
 * The function of processing 62 port data
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Core_62Port(BYTE p_PM1Data)
{
    switch(p_PM1Data)
    {
        case 0x80:
        ACPI_Cmd_80Data();
	    break;

		case 0x81:
        ACPI_Cmd_81Data();
	    break;

        case 0x82:
        ACPI_Cmd_82Data();
	    break;

		case 0x83:
        ACPI_Cmd_83Data();
	    break;    

        case 0x84:
        ACPI_Cmd_84Data();
	    break;  

        case 0x92:
        ACPI_Cmd_92Data();
        break;

        case 0x93:
        ACPI_Cmd_93Data();
        break;

		default:
	    break;
    }
}

/**
 * ****************************************************************************
 * The function of processing 66 port command.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void Core_66Port(BYTE p_PM1Cmd)
{
    switch(p_PM1Cmd)
    {
        case 0x80:
        ACPI_Cmd_80();
	    break;

		case 0x81:
        ACPI_Cmd_81();
	    break;

        case 0x82:
        ACPI_Cmd_82();
	    break;

		case 0x83:
        ACPI_Cmd_83();
	    break;    

        case 0x84:
        ACPI_Cmd_84();
	    break;  

        case 0x92:
        ACPI_Cmd_92();
        break;

        case 0x93:
        ACPI_Cmd_93();
        break;

        case 0xDC:
        ACPI_Cmd_DC();
        break;   

        case 0xF0:
        ACPI_Cmd_F0();
        break;  

		default:
	    break;
    }
}


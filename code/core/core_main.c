/*
 * ****************************************************************************
 * TITLE: CORE_MAIN.C - Main Program for KBC firmware.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
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
 * update
 *
 *  core_main.c
 *
 *  Copyright (c) 2012- ITE TECH. INC.  All rights reserved.
 *
 *  Created on: 2012/11/13
 *
 *      Author: ITE00580, Dino Li
 *
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
 * main function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
int __attribute__ ((section (".func__main"))) main(void);
int main(void)
{
	DisableAllInterrupt();
    #ifdef EC_FEATURE_ONLY
    _init_sp_();
    #endif /* EC_FEATURE_ONLY */
    
    /* Exit from follow mode or EC scatch ROM */
	if(Hook_ECRetunrMainFuncKeepCondition()==0x33)
	{
        Direct_Map_Core_Flash();
        WinFlashMark = exitFollowMode;
    	WinFlashMark2 = 0x00;
        Hook_ECExitFollowMode();
        Init_Kbd();
        Init_Timers();
		EnableModuleInterrupt(); 
		#if chPWSW1WDT
  		OEM_PWRSWWDTEn();		
		#endif  //chPWSW1WDT	
	}
	else
	{
		ECSleepCount = 3;
		if(RSTStatus & 0x02)
		{
			pLastSID  = WDT_WARMBOOT_ID;
		}
		else
		{
    		pLastSID  = COLDBOOT_ID;
		}
		if( CheckCrisisMode() )
    	{ SET_MASK(SYS_MISC1,Crisis_On); }
    	else
    	{ CLEAR_MASK(SYS_MISC1,Crisis_On); }
		Core_Initialization();
		Oem_Initialization();
        InitEnableInterrupt();
	}
	#ifdef UART_Debug
    Init_UART();
    printf("************************************\r\n");
    printf("EC Init OK !!!\r\n");
    printf("************************************\r\n");
	#endif
	#if WDT_Support
  	EnableInternalWDT();
	#endif
	while(1)
   	{
   		#if chPWSW1WDT
		if (IS_MASK_SET(pProject4,pPWSWdisable)) 
	    {
			OEM_RESETPWSWTIMER1();
		}
		#endif  //chPWSW1WDT	
        if(OEM_SkipMainServiceFunc()==Normal_MainService)
        {
        	#ifdef DEF_PCUSTOM_MAIN_SERVICE
        	oem_main_service();
        	#else /* #ifdef DEF_PCUSTOM_MAIN_SERVICE #else */
    		main_service();
    		EnableModuleInterrupt();
    		_nop_();
    		_nop_();
    		_nop_();
    		_nop_();

            #if TouchPad_only

            #else
            if(PS2CheckPendingISR()==0x00 && (PS2StartBit==0) &&
                (F_Service_PS2==0))
            {
                ScanAUXDeviceStep();
            }
            #endif

            /* No pending service event. */
            if(CheckEventPending()==0x00)
    		{
    			
                #ifdef EC_FEATURE_ONLY
                    #if DEF_EC_FIRMWARE_DOZE_SUPPORT
					if(IS_MASK_CLEAR(dummy_test, BIT7))
					{
                    	PowerM_Doze();
					}
                    #endif /* EC_FEATURE_ONLY */
                #else
                    #if DEF_EC_FIRMWARE_DOZE_SUPPORT
                        #ifdef __ENABLE_EC_MAIN__
                    hal_suspend_task(tthi_EC);
                        #endif /* __ENABLE_EC_MAIN__ */
                    #endif
                #endif /* DEF_EC_FIRMWARE_DOZE_SUPPORT */
				
                
                _nop_();
    		    _nop_();
    		    _nop_();
    		    _nop_();
            }
            #endif /* #ifdef DEF_PCUSTOM_MAIN_SERVICE #endif */
        }
  	} 

    return(0x0001);
}


/*
 * ****************************************************************************
 * table of main_service function
 * ****************************************************************************
 */
const FUNCT_PTR_V_V service_table[32] =
{
	service_pci1,    	    /* Host command/data service */
	service_extend_task,    /* service hid and fusion event */
	service_send,           /* Send byte from KBC */
	service_PS2_data, 	    /* Send PS2 interface dat */
	service_ps2, 		    /* process PS2 interface data */
	service_pci2,		    /* Secondary Host command/data service */
    service_1mS,            /* 1 millisecond elapsed */
 	service_scan,           /* Keyboard scanner service */

    service_Low_LVEvent,    /* Low level event */
    service_pci3,   	    /* Third Host command/data service */
    service_cir,     	    /* CIR IRQ. */
    service_pci4,           /* fourth command/data service */
	service_OEM_1,
	service_OEM_2,
	service_OEM_3,
	service_OEM_4,

    service_reserved_16,
    service_reserved_17,
    service_reserved_18,
    service_reserved_19,
    service_reserved_20,
    service_reserved_21,
    service_reserved_22,
    service_reserved_23,

    service_reserved_24,
    service_reserved_25,
    service_reserved_26,
    service_reserved_27,
    service_reserved_28,
    service_reserved_29,
    service_reserved_30,
    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    service_usb_isr,
    #else
    service_reserved_31,
    #endif
}; 

/**
 * ****************************************************************************
 * main service function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void main_service(void) 
{
    ulong_32 service_index;

    /* rtos only */
    #ifndef EC_FEATURE_ONLY
    ulong_32 l_suspend_all_thread;
    #endif
    
    /* rtos only */
    #ifndef EC_FEATURE_ONLY
    l_suspend_all_thread = 0x00;
    if(CheckEventPending()==0x01)
    {
        hal_suspend_all_thread();
        l_suspend_all_thread = 0x01;
    }
    #endif

    /* service event not free. */
    while(CheckEventPending()==0x01)
    {
        for(service_index = 0; service_index<32; service_index++)
        {
            #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
            service_usb_isr();
            #endif

            /* new event need service */
            if(F_Service_All[service_index]!=0x00)
            {
                F_Service_All[service_index]=0x00;

                /* Dispatch to service handler */
                (service_table[service_index])();

                #ifdef DEF_SERVICE_ROUTINE_AFTER_ANY_EVENT
                service_routine_after_any_event();
                #endif
                
                #if DEF_MAIN_SERVICE_WITH_PRIORITY
                break;
                #endif
            }
        }
    }

    /* rtos only */
    #ifndef EC_FEATURE_ONLY
    if(l_suspend_all_thread)
    {
        hal_resume_all_thread();
    }
    #endif    
}

/**
 * ****************************************************************************
 * service_Low_LVEvent
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_Low_LVEvent(void)
{    
    if(IS_MASK_SET(KBHISR,P_IBF))
    {
  	    F_Service_PCI = 1;
    }
    
    if(IS_MASK_SET(PM1STS,P_IBF))
    {
  	    F_Service_PCI2 = 1;
    }

    if(IS_MASK_SET(PM2STS,P_IBF))
    {
  	    F_Service_PCI3= 1;
    }

    if(IS_MASK_SET(PM3STS,P_IBF))
    {
  	    F_Service_PCI4 = 1;
    }
    
}

/**
 * ****************************************************************************
 * service_unlock
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_unlock(void)
{
	Unlock_Scan();
}

/**
 * ****************************************************************************
 * core 1ms service function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_1mS(void)
{
    #if DEF_HOOK_FUNCTION_WITH_SERVICE_1MS
    Hook_Service_1ms();
    #endif

    #ifdef EC_FEATURE_ONLY
    Timer1msEvent();

	Timer1msCnt++;
    if(Timer1msCnt>=10)
    {
        Timer1msCnt = 0x00;
    }
    #endif

    if(Hook_Only_Timer1msEvent()==Only_Timer1msEvent)
    {   
		#ifdef DEF_PCUSTOM_TIMER
		oem_timer_hook();
		#endif
    }
    else
    {
        #ifdef EC_FEATURE_ONLY
        if(Service_Timer5msEvent)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer5msEvent = 0;
            #else
            Service_Timer5msEvent--;
            #endif
            
            Timer5msEvent();
        }
        #endif
        
        if(Service_Timer10msEventA)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer10msEventA = 0;
            #else
            Service_Timer10msEventA--;
            #endif
            
            Timer10msEventA();
        }

        if(Service_Timer10msEventB)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer10msEventB = 0;
            #else
            Service_Timer10msEventB--;
            #endif
            
            Timer10msEventB();
        }

        if(Service_Timer50msEventA)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer50msEventA = 0;
            #else
            Service_Timer50msEventA--;
            #endif
            
            Timer50msEventA();
        }

        if(Service_Timer50msEventB)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer50msEventB = 0;
            #else
            Service_Timer50msEventB--;
            #endif
            
            Timer50msEventB();
        }

        if(Service_Timer50msEventC)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer50msEventC = 0;
            #else
            Service_Timer50msEventC--;
            #endif
            
            Timer50msEventC();
        }

        if(Service_Timer100msEventA)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer100msEventA = 0;
            #else
            Service_Timer100msEventA--;
            #endif
            
            Timer100msEventA();
        }

        if(Service_Timer100msEventB)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer100msEventB = 0;
            #else
            Service_Timer100msEventB--;
            #endif
            
            Timer100msEventB();
        }

        if(Service_Timer100msEventC)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer100msEventC = 0;
            #else
            Service_Timer100msEventC--;
            #endif
            
            Timer100msEventC();
        }

        if(Service_Timer500msEventA)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer500msEventA = 0;
            #else
            Service_Timer500msEventA--;
            #endif
            
            Timer500msEventA();
        }

        if(Service_Timer500msEventB)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer500msEventB = 0;
            #else
            Service_Timer500msEventB--;
            #endif
            
            Timer500msEventB();
        }

        if(Service_Timer500msEventC)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer500msEventC = 0;
            #else
            Service_Timer500msEventC--;
            #endif
            
            Timer500msEventC();
        }

        if(Service_Timer1SecEventA)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer1SecEventA = 0;
            #else
            Service_Timer1SecEventA--;
            #endif
            
            Timer1SecEventA();
        }

        if(Service_Timer1SecEventB)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer1SecEventB = 0;
            #else
            Service_Timer1SecEventB--;
            #endif
            
    	    Timer1SecEventB();
        }

        if(Service_Timer1SecEventC)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer1SecEventC = 0;
            #else
            Service_Timer1SecEventC--;
            #endif
            
            Timer1SecEventC();
        }

        if(Service_Timer1MinEvent)
        {
            #ifdef EC_FEATURE_ONLY
            Service_Timer1MinEvent = 0;
            #else
            Service_Timer1MinEvent--;
            #endif
            
            Timer1MinEvent();
        }
    }
}

/**
 * ****************************************************************************
 * ISR 1ms events
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_Timer1msEvent(void)
{
	Timer1msEvent();
}

/**
 * ****************************************************************************
 * ISR 5ms events
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_Timer5msEvent(void)
{
    Timer5msEvent();
}

/**
 * ****************************************************************************
 * ISR 10ms events
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ISR_Timer10msEvent(void)
{
    SetStartScanAUXFlag();
}

/**
 * ****************************************************************************
 * 1ms service function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer1msEvent(void)
{
    ReSendPS2PendingData();
    Hook_Timer1msEvent(Timer1msCnt);

    if((KBPendingTXCount != KBPendingRXCount )||
        (scan.kbf_head != scan.kbf_tail))
	{
        /* 2ms time based */
        if((Timer1msCnt&2)==0)
        {
            /* Post service request. */
            F_Service_SEND = 1;
        }
	}
}

/**
 * ****************************************************************************
 * 5ms service function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer5msEvent(void)
{
    F_Service_Low_LV = 1;
	if (Timer_A.fbit.TMR_SCAN) 
	{
        #ifdef KB_HW_KeyScan
        HW_KB_Check_TM();
        #else
		/* Request scanner service. */
  		F_Service_KEY = 1;
  		#endif  /* #ifdef KB_HW_KeyScan #endif */
  	}
    Hook_Timer5msEvent();
}

/**
 * ****************************************************************************
 * 10ms service function A
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer10msEventA(void)
{
	Hook_Timer10msEventA();	
}

/**
 * ****************************************************************************
 * 10ms service function B
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer10msEventB(void)
{
	Hook_Timer10msEventB();	 
}

/**
 * ****************************************************************************
 * 50ms service function A
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer50msEventA(void)
{
	Hook_Timer50msEventA();
}

/**
 * ****************************************************************************
 * 50ms service function B
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer50msEventB(void)
{
	Hook_Timer50msEventB();
}

/**
 * ****************************************************************************
 * 50ms service function C
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer50msEventC(void)
{
	Hook_Timer50msEventC();
}

/**
 * ****************************************************************************
 * 100ms service function A
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer100msEventA(void)
{
	Hook_Timer100msEventA();
}

/**
 * ****************************************************************************
 * 100ms service function B
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer100msEventB(void)
{
	Hook_Timer100msEventB();
}

/**
 * ****************************************************************************
 * 100ms service function C
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer100msEventC(void)
{
	Hook_Timer100msEventC();
}

/**
 * ****************************************************************************
 * 500ms service function A
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer500msEventA(void)
{
	Hook_Timer500msEventA();
}

/**
 * ****************************************************************************
 * 500ms service function B
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer500msEventB(void)
{
	Hook_Timer500msEventB();
}

/**
 * ****************************************************************************
 * 500ms service function C
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer500msEventC(void)
{
	Hook_Timer500msEventC();
}

/**
 * ****************************************************************************
 * 1sec service function A
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer1SecEventA(void)
{	
	Hook_Timer1SecEventA();
}

/**
 * ****************************************************************************
 * 1sec service function B
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer1SecEventB(void)
{
	Hook_Timer1SecEventB();
}

/**
 * ****************************************************************************
 * 1sec service function C
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer1SecEventC(void)
{
	Hook_Timer1SecEventC();
}

/**
 * ****************************************************************************
 * 1min service function
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Timer1MinEvent(void)
{
    Hook_Timer1MinEvent();
}

/**
 * ****************************************************************************
 * To check service event free or not.
 *
 * @return
 * 0x01, service event not free.
 * 0x00, No pending service event.
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE CheckEventPending(void)
{
    if((F_Service_0 != 0x00)||(F_Service_1 != 0x00)||
        (F_Service_2 != 0x00)||(F_Service_3 != 0x00)||
        (F_Service_4 != 0x00)||(F_Service_5 != 0x00)||
        (F_Service_6 != 0x00)||(F_Service_7 != 0x00))
    {
        return(0x01);
    }
    else
    {
        return(0x00);
    }
}

/**
 * ****************************************************************************
 * handle hid_main()
 *
 * @return
 *
 * @parameter
 *
 * note, 1 ms time based
 *
 * ****************************************************************************
 */
void service_extend_task(void)
{
    //#ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    //if(Hook_Start_EC_Handle_Task()==0xFF)
    //{
    //    EC_Handle_Task();
    //}
    //#endif
}

void Sleep_Now(void)
{
	PowerM_Sleep();
	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

BYTE CheckCanEnterDeepSleep()
{
	//RamDebug(0xDC);
#if Support_TWO_BATT
	if(IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
	{
  		if (Current_BAT_State != BAT_SEL_INIT){
    		return FALSE;
  		}
	}
#endif //Support_TWO_BATT
	if( Read_EC_ON() && Read_TYPEC_INT() )
	{
		return FALSE;
	}

  //if(Read_AC_IN()) return FALSE;
  	if ( IS_MASK_SET(SYS_STATUS,AC_ADP) )  
  	{ 
		if(BattExistGet&&ChargingGet)
			return FALSE;
	//	if(SecBattExistGet&&SecChargingGet)
	//		return FALSE;
	}

  //if( (IS_MASK_SET(cCmd, bPCHPWR_Keep)) || (IS_MASK_SET(cCmd, b1Chg_VGAMode)) )
  //{ return FALSE; }
  //RamDebug(0x01);

    if ( AutoTimer != 0
    ||( pWorkAround > 0)
    )
  { return FALSE; }

	//RamDebug(0x02);

  if(SystemIsS5)
  {
    if ( (ECSleepCount != 0)||(LOWBATT_3TIMES != 0) ) 
    {
    	Check_EC_ON();
		//RamDebug(0x03);
    	return FALSE; 
    }
    if(IS_MASK_SET(LENOVOPMFW_Temp,b3EC_ONStatus))
    {
    	//RamDebug(0x04);
      CLEAR_MASK(LENOVOPMFW_Temp,b3EC_ONStatus);
      return FALSE;
    }
	//RamDebug(0x05);
    return TRUE;
  }
  //RamDebug(0x06);

  return FALSE;
}

void InitEnterDeepSleepForAC(void)
{
	ADCCFG &= 0xFE;
	AUTOCG = 0x00;
	
	IER0=0;
	IER1=0;
	IER2=0;
	IER3=0;
	IER4=0;
  	IER5=0;
	IER6=0;
	IER7=0;
	IER8=0;
	IER9=0;
	IER10=0; 
	IER11=0; 
	IER12=0; 
	IER13=0; 
	IER14=0; 
	IER15=0; 
	IER16=0; 
	IER17=0; 	
	IER18=0; 
	IER19=0;

	DisableGlobalInt();	

     //GCR2 &=0xDF;
	// SMBus3_CLK_OUTDW;
	 // SMBus3_DAT_OUTDW;
	 //GPDRH &= 0xFD;
	 PMER1 &= 0xFC;  //A285D00033:
		GPCRE7 = OUTPUT+PULL_UP;
		GPDRE &= 0x7F;
    // CGCTRL2R |= 0x70;
	// CGCTRL3R |= 0xEF;
	// CGCTRL4R |= 0xFF;
	 

  	SetACIN_Int();  // IER13 AC in bit2
  	SetPWRSW_Int();  // IER1 power switch bit6
  	SetBAT1INOUT_Int();
	//SetBAT2INOUT_Int();
	SetLANWake_Int();
	SetPDC_Int();
	SetSLPS3_Int();
#if Support_BOTTOM_TAMPER_FUNC	
    SetBottomTamper_Wake();
#endif //Support_BOTTOM_TAMPER_FUNC

  	if( Read_AOU_EN() )
  	{
    	SetUSBExTimerAC();
  	}
	
    ADCSTS  = 0x00;
  	ADCCFG  = 0x20;
	DACPWRDN = 0xFF;
  

  	WUESR1 = 0xFF;			
	WUESR2 = 0xFF;
	WUESR3 = 0xFF;
	WUESR4 = 0xFF;
	WUESR6 = 0xFF;
	WUESR7 = 0xFF;
	WUESR8 = 0xFF;
	WUESR9 = 0xFF;
	WUESR10 = 0xFF;
	WUESR11 = 0xFF;
	WUESR12 = 0xFF;
	WUESR13 = 0xFF;
	WUESR14 = 0xFF;

 // WUESR1  = 0xFF;  // power switch bit6
 // WUESR2  = 0xFF;
 // WUESR3  = 0xFF;  // ExTimer1 Bit6
 // WUESR4  = 0xFF;
 // WUESR7  = 0xFF;
 // WUESR10  = 0xFF;  // AC in bit2
 // WUESR14  = 0xFF;  // Novo bit0

 // ISR1 = 0xFF;  // power switch bit6
 // ISR2 = 0xFF;
 // ISR3 = 0xFF;  // ExTimer1 Bit6
 // ISR4 = 0xFF;
 // ISR13= 0xFF;   // AC in bit2
 // ISR16= 0xFF;   // Novo bit0
}

//-----------------------------------------------------------------------------
void InitEnterDeepSleep(void)
{
  	ADCCFG &= 0xFE;
	AUTOCG = 0x00;
	
	IER0=0;
	IER1=0;
	IER2=0;
	IER3=0;
	IER4=0;
  IER5=0;
	IER6=0;
	IER7=0;
	IER8=0;
	IER9=0;
	IER10=0; 
	IER11=0; 
	IER12=0; 
	IER13=0; 
	IER14=0; 
	IER15=0; 
	IER16=0; 
	IER17=0; 	
	IER18=0; 
	IER19=0;

	DisableGlobalInt();	
	
  	KSOL  = 0x00;
  	KSOH1  = 0x00;
  	KSOH2  = 0x00;
  	KSICTRL = 0x00;

  	SetACIN_Int();  // IER13 AC in bit2
  	SetPWRSW_Int();  // IER1 power switch bit6
#if Support_BOTTOM_TAMPER_FUNC	
    SetBottomTamper_Wake();
#endif //Support_BOTTOM_TAMPER_FUNC

  	if( Read_AOU_EN() )
  	{
    	SetUSBExTimer();
  	}

  	ADCSTS  = 0x00;
  	ADCCFG  = 0x20;
	DACPWRDN = 0xFF;

	
	
		
		GPCRF2 = INPUT;
		GPCRF3 = INPUT;
		GPCRA4 = INPUT+PULL_UP;
		GPCRA5 = INPUT+PULL_UP;
		GPCRC1 = INPUT+PULL_UP;
		GPCRC2 = INPUT+PULL_UP;
		GPCRE0 = INPUT;
		GPCRE7 = INPUT;
		SMBus3_CLK_INDW;  // SMBus3 CLK to Input+pull_DW pin
  		SMBus3_DAT_INDW;  // SMBus3 DATA to Input+pull_DW pin
		CGCTRL2R |= 0x70;
		CGCTRL3R |= 0xEF;
		CGCTRL4R |= 0xFF;
		PMER1 &= 0xFC;  //A285D00033:
		GPCRE7 = OUTPUT+PULL_UP;
		GPDRE &= 0x7F;
			
	
		I2C_CONTROL(0) |= 0x11;
		I2C_CONTROL(1) |= 0x11;
		I2C_CONTROL(2) |= 0x11;
	
		SPCTRL1 = 0x03;

	GPCRD3 = INPUT;
  
  	PM_PWRBTN_LOW();
  	GPOTF = 0x00;
  	LOGO_LED_PWM_REG = INPUT; //Prevent PWM low pulse for issue "LOGO LED will flash in deepsleep mode".
	#if Support_PWRBTN_LED      
  	PWRBTN_LED_PWM_REG = INPUT; //Prevent PWM low pulse for issue "PWR LED will flash in deepsleep mode".
	#endif //Support_PWRBTN_LED  
#if Support_KBD_BL_LED
#if Support_KBD_BL_DETECT
	if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
	{	
  	KBD_BL_LED_PWM_REG = INPUT; //Prevent PWM low pulse for issue "LED will flash in deepsleep mode".
  }
#endif //Support keyboard backlight

  	WUESR1 = 0xFF;			
	WUESR2 = 0xFF;
	WUESR3 = 0xFF;
	WUESR4 = 0xFF;
	WUESR6 = 0xFF;
	WUESR7 = 0xFF;
	WUESR8 = 0xFF;
	WUESR9 = 0xFF;
	WUESR10 = 0xFF;
	WUESR11 = 0xFF;
	WUESR12 = 0xFF;
	WUESR13 = 0xFF;
	WUESR14 = 0xFF;

 // WUESR1  = 0xFF;  // power switch bit6
 // WUESR2  = 0xFF;
 // WUESR3  = 0xFF;  // ExTimer1 Bit6
 // WUESR4  = 0xFF;
 // WUESR7  = 0xFF;
 // WUESR10  = 0xFF;  // AC in bit2
 // WUESR14  = 0xFF;  // Novo bit0

 // ISR1 = 0xFF;  // power switch bit6
 // ISR2 = 0xFF;
 // ISR3 = 0xFF;  // ExTimer1 Bit6
 // ISR4 = 0xFF;
 // ISR13= 0xFF;   // AC in bit2
 // ISR16= 0xFF;   // Novo bit0
}

void InitWakeFromDeepSleepForAC(void)
{
	
	  CGCTRL2R &= 0x8F;
      CGCTRL3R &= 0x10;
      CGCTRL4R &= 0x00;
	  AUTOCG = 0x70;
      DACPWRDN = 0x00;
	 
	  ISR0	  = 0xFF;
	  ISR1	  = 0xFF;
	  ISR2	 = 0xFF;
	  ISR3	  = 0xFF;
	  ISR4	= 0xFF;
	  ISR5	= 0xFF;
	  ISR9	  = 0xFF;
	  ISR13   = 0xFF; // AC in
	  ISR16   = 0xFF; // Novo
	
	  WUESR1  = 0xFF;
	  WUESR2  = 0xFF;
	  WUESR3  = 0xFF;
	  WUESR4  = 0xFF;
	  WUESR7  = 0xFF;
	  WUESR10  = 0xFF; // AC in
	  WUESR14  = 0xFF; // Novo
	
	 
	 
		
	  Init_Kbd();
	  Core_Init_Regs();
	  Init_Regs();
	  Enable_Any_Key_Irq();  //msmart
	
	  Init_ADC();
	 
	  Init_Timers();
	  EnableModuleInterrupt();
	
	  Enable_Any_Key_Irq(); // Enable Keyboard

        Init_Dash();    //A285D00079+

	
		GPCRF2 = ALT;
		GPCRF3 = ALT;
		GPCRA4 = ALT;
		GPCRA5 = ALT;
		GPCRC1 = ALT;
		GPCRC2 = ALT;	
		//GPCRE0 = ALT;
		//GPCRE7 = ALT;
		//GCR2 |= 0x20;
		PMER1 |= 0x03;  //A285D00033:
	    //SMBus3_CLK_ALT;
		//SMBus3_DAT_ALT;
		LOGO_LED_PWM_REG = ALT; //Set to PWM again
#if Support_PWRBTN_LED       
	  PWRBTN_LED_PWM_REG = ALT; //Set to PWM again	
#endif //Support_PWRBTN_LED       
#if Support_KBD_BL_LED
#if Support_KBD_BL_DETECT
		if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
		{
		KBD_BL_LED_PWM_REG = ALT; //Set to PWM again  
	  }
#endif //Support keyboard backlight

}

//-----------------------------------------------------------------------------
void InitWakeFromDeepSleep(void)
{
   
  //IER0   = 0x00;  // 7 ~ 0
  //IER1   = 0x00;  // 15 ~ 8
  //IER2   = 0x00;  // 23 ~ 16
  //IER3   = 0x00;
  //IER4    = 0x00;
  //IER9    = 0x00;
  //IER13  = 0x00; // AC in
  //IER16   = 0x00; // Novo
  
  CGCTRL2R &= 0x8F;
  CGCTRL3R &= 0x10;
  CGCTRL4R &= 0x00;
  AUTOCG = 0x70;
  DACPWRDN = 0x00;

  ISR0    = 0xFF;
  ISR1    = 0xFF;
  ISR2   = 0xFF;
  ISR3    = 0xFF;
  ISR4  = 0xFF;
  ISR5  = 0xFF;
  ISR9    = 0xFF;
  ISR13   = 0xFF; // AC in
  ISR16   = 0xFF; // Novo

  WUESR1  = 0xFF;
  WUESR2  = 0xFF;
  WUESR3  = 0xFF;
  WUESR4  = 0xFF;
  WUESR7  = 0xFF;
  WUESR10  = 0xFF; // AC in
  WUESR14  = 0xFF; // Novo

  GPCRD3 = OUTPUT;
 
    
  Init_Kbd();
  Core_Init_Regs();
  Init_Regs();
  Enable_Any_Key_Irq();  //msmart

  Init_ADC();
 
  Init_Timers();
  EnableModuleInterrupt();

  Enable_Any_Key_Irq();	// Enable Keyboard

  Init_Dash();  //A285D00079+

	GPCRF2 = ALT;
	GPCRF3 = ALT;
	GPCRA4 = ALT;
	GPCRA5 = ALT;
	GPCRC1 = ALT;
	GPCRC2 = ALT;	
	PMER1 |= 0x03;  //A285D00033:
	GPCRE0 = ALT;
	GPCRE7 = ALT;

	LOGO_LED_PWM_REG = ALT; //Set to PWM again
#if Support_PWRBTN_LED       
  PWRBTN_LED_PWM_REG = ALT; //Set to PWM again  
#endif //Support_PWRBTN_LED       
#if Support_KBD_BL_LED
#if Support_KBD_BL_DETECT
	if(Read_KBD_BL_DTCT())
#endif //Support keyboard backlight detect
	{
  	KBD_BL_LED_PWM_REG = ALT; //Set to PWM again  
  }
#endif //Support keyboard backlight
}

//----------------------------------------------------------------------------
// Output : 1 Crisis mode
//          0 Normal mode
//----------------------------------------------------------------------------
BYTE CheckCrisisMode(void)
{  // To add condition here
  BYTE BKSOL,BKSOH1,BKSOH2;
    BKSOL = KSOL;
    BKSOH1 = KSOH1;
    BKSOH2 = KSOH2;
 

  KSOL  = 0xEF;
  KSOH1 = 0xFF;
  KSOH2 = 0xFF;      // "R" key (scan line b4)
  WNCKR = 0x00;           // Delay 15.26 us
  WNCKR = 0x00;           // Delay 15.26 us
  if ((KSI&0x04) != 0)  // "R" key (data line b2)
  {
    KSOL  = 0xFF;

        KSOL  = BKSOL;
        KSOH1 = BKSOH1;
        KSOH2 = BKSOH2;
      return FALSE;
  }

  KSOL  = 0xFF;

    KSOL  = BKSOL;
    KSOH1 = BKSOH1;
    KSOH2 = BKSOH2;

  return TRUE;
  
}


const sDMAScratchSRAM asDMAScratchSRAM[]=
{
    {	&SCRA0H,	&SCRA0M,	&SCRA0L	    },
	{	&SCRA1H,	&SCRA1M,	&SCRA1L	    },
	{	&SCRA2H,	&SCRA2M,	&SCRA2L	    },
	{	&SCRA3H,	&SCRA3M,	&SCRA3L	    },
	{	&SCRA4H,	&SCRA4M,	&SCRA4L	    },
    {	&SCRA5H,	&SCRA5M,	&SCRA5L	    },
	{	&SCRA6H,	&SCRA6M,	&SCRA6L	    },
	{	&SCRA7H,	&SCRA7M,	&SCRA7L	    },
	{	&SCRA8H,	&SCRA8M,	&SCRA8L	    },
	{	&SCRA9H,	&SCRA9M,	&SCRA9L	    },
    {	&SCRA10H,	&SCRA10M,	&SCRA10L	},
	{	&SCRA11H,	&SCRA11M,	&SCRA11L	},
	{	&SCRA12H,	&SCRA12M,	&SCRA12L	},
};
void CacheDma(BYTE sramnum,DWORD addr, BYTE option)
{
    if(option==0x01)
    {
        eflash_to_direct_map(addr, sramnum);
        dm_set_segment_valid(sramnum);
    }
    else
    {
        dm_set_segment_invalid(sramnum);
    }	
}
extern char __ramcode_begin;
#define GET_RAMCODE_START_ADDRESS     ((unsigned long)&__ramcode_begin)
void Direct_Map_Core_Flash(void)
{
    #if DEF_DIRECT_MAPPING_CORE_FLASH_SECTION
    CacheDma(0, GET_RAMCODE_START_ADDRESS, 1);
    #endif
    #ifdef EC_FEATURE_ONLY_SUPPORT_FUSION
    hal_init_os();
    #endif
}

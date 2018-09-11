/* 
 * ****************************************************************************
 * TITLE: CORE_SCN.C - code to handle local keyboard scanning.
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

/* Firmware scan keyboard */
#ifndef KB_HW_KeyScan

/**
 * ****************************************************************************
 * short delay for reading ksi
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CapDelay(void)
{
    /* Delay 15.26 us */
    WNCKR = 0x00;

    /* Delay 15.26 us */
    WNCKR = 0x00;
}

/**
 * ****************************************************************************
 * FUNCTION:   scan_keys
 *
 * Scan key matrix once.
 *
 * @return
 * value != 0, scan activity detected, scan again later.
 * value  = 0, no scan activity detected.
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE scan_keys(void)
{
	BYTE result;
	result = FALSE;

    //open-A285D00062: fix keyboard noise when crazy press.
    /* key buffer half-full or not */
    if(IS_MASK_SET(u4Reserve0A, Chk_Buf_Half_Full_En))  //A285D00067+
    {
    if(check_keyscan_buffer_half_full()!=0x00)
    {
         return(0x01);
    }
    }
    
    scan.saf_make = 0;
    scan.saf_break = 0;
    scan.saf_keys = 0;

 	if (typematic.byte) 
	{
     	check_tm(typematic);  	// Check typematic. 
 	}
    /* Exist Debounce key? */
	if(new_keyh.byte)
 	{
        /* Yes, bounding. */
   	    /* Set debounce make bit. */
     	scan.saf_make = 1;
      	debounce_key();
      	result = TRUE;
 	}
    
    /* Otherwise, scan all. */
    if(!result)
    {  
        for(ITempB03=0;ITempB03<16;ITempB03++)
        {
        	Write_Strobe(ITempB03);
			CapDelay();

            /* Read in KI 0 - 7 sense line data. */
         	ITempB02 = KSI;
            ITempB02 = (~ITempB02) ^ bscan_matrix[ITempB03];
            if(ITempB02 != 0) 
           	{
            	check_scan(ITempB02, ITempB03); 
          	}

            if(bscan_matrix[ITempB03]) 
          	{
                /* Here, if current still active. */

                /* Set keys active bits. Check all key release. */
                scan.saf_keys = 1; 
            }   
			KSOL=0xFF;	
			KSOH1=0xFF;
        }
        								
        /* If ghost exist, make key clear. */
        if(new_keyl.field.ghost == 1)
        {
            /* Ghost flag is set. Clear it. */
            new_keyl.field.ghost = 0;
            ITempB01 = new_keyl.byte & 7;
            if(ITempB01 == 0)
            {  
                ITempB01 = Byte_Mask((BYTE) (new_keyh.field.input));
                ITempB01 = ITempB01 & diode_key; 
            }

            if(ITempB01 == 0)
            {
                /* Clear new key. */
                new_keyh.byte = 0;
                new_keyl.byte = 0;
            }
            IP_GHOSTBEEP=TRUE;		
        } 
    }

	return((BYTE) (scan.saf_make + scan.saf_break + scan.saf_keys));
}

/**
 * ****************************************************************************
 * FUNCTION: Write_Strobe
 *
 * Writes the scan matrix KSO[15:0] lines based on Scan Line bit number.
 *
 * KSO[15:0] will be all floats or open except for the bit corresponding to
 * the Scan Line number.  This bit will be low to be detected by the sense
 * line input port later.
 *
 * Config.Msk_Strobe_H and Config.Msk_Strobe_L specify which of the 16 bits on
 * the port are used for the scanner.  (1 = Scanner, 0 = Other OEM function).
 * This allows port pins which are not used for the scanner output lines to be
 * used for something else.
 *
 * @return
 *
 * @parameter, Scan line number.
 *
 * ****************************************************************************
 */
void Write_Strobe(BYTE scan_line_num)
{
	if (scan_line_num<8)
   	{
     	KSOL=~(0x01<<scan_line_num);
      	KSOH1=0xFF;
  	}
 	else
  	{
     	KSOL=0xFF;
   		KSOH1=~(0x01<<(scan_line_num-0x08));
   	} 

	if(ExtendMatrix)
	{
		KSOH2 = 0xFF;
		Hook_SetGPIOScanPinH();
	}
}

/**
 * ****************************************************************************
 * FUNCTION: check_scan
 *
 * @return
 *
 * @parameter,
 *     changes = Any bit set indicates there is a difference in state from
 *         last KSI to this KSI.
 *     scan_address = Address of strobe line that was low when changes was
 *         computed.
 *
 * NOTE:  For the shift and bit test of "changes" to work, "changes" MUST be
 * unsigned.  (Right shifting an unsigned quantity fills vacated bits with
 * zeros.  Right shifting a signed quantity will fill vacated bits with sign
 * bits on some machines and zero bits on others.)
 *
 * ****************************************************************************
 */
void check_scan(BYTE changes, BYTE scan_address)
{
    BYTE flag;
    BYTE change_make_key;
    BYTE bit_num; 

    /* Simple Matrix. */
	flag = FALSE;
	if(new_keyl.field.ghost == 1)
 	{
    	flag = TRUE;
	}
	else
	{
        /* Find 0 -> 1 's. */
		change_make_key = changes & ~bscan_matrix[scan_address];
     	if(change_make_key == 0) 
    {
             flag = FALSE;
      }
       else if (find_paths(change_make_key, scan_address))
    {
        if(((pProject4>>5) != (BIT0+BIT1))&&((pProject4>>5) != (BIT1+BIT2))&&((pProject4>>5) != (BIT0+BIT2)))	//Work-around for combo "key Win + Left-Shift + Right-Arrow"(Ghost key).
        {
           flag = TRUE;    /* TRUE if ghost keys. */
        }
         }
  }

  if (flag)
   {   /* Ghost exists. */
        /* Ghost exists. */

        /* Set ghost flag. */
  		new_keyl.field.ghost = 1;

    	/* Only look at 1 -> 0 transition & diode key always no ghost. */
     	changes &= bscan_matrix[scan_address] | diode_key;
	}

    bit_num = 0;
    while(changes != 0)
    {
 	    /* Look at changes 1 bit at a time. */
        if(changes & 0x01)
		{
            cscfnd(bit_num, scan_address);
        }
        bit_num++;

        /* Shift bit out (and a zero bit in) to check next bit. */
        changes = changes >> 1;
    }
}

/**
 * ****************************************************************************
 * FUNCTION:   cscfnd
 *
 * Find changed bit.  This subroutine is called for each bit in this KSI that
 * is different from last KSI.
 *
 * ****************************************************************************
 */
void cscfnd(BYTE bit_num, BYTE scan_address)
{
    if(bscan_matrix[scan_address] & Byte_Mask(bit_num))
    {  
        if(scan.saf_break == 0) 
		{
            scan.saf_break = 1;
            setup_debounce(bit_num, scan_address, BREAK_EVENT);
        }
    }
    else
    {   
        if(scan.saf_make == 0) 
		{
            scan.saf_make = 1;
            setup_debounce(bit_num, scan_address, MAKE_EVENT);
        }
    } 
}

/**
 * ****************************************************************************
 * FUNCTION:   setup_debounce
 *
 * @return
 *
 * @parameter,
 *     scan_address = number of bit of KO.
 *     bit_num = number of bit that changed from last KI to this KI for KO.
 *     event = contact event (MAKE_EVENT or BREAK_EVENT).
 *
 * ****************************************************************************
 */
void setup_debounce(BYTE bit_num, BYTE scan_address, BYTE event)
{
    new_keyh.field.output = scan_address;
    new_keyh.field.input = bit_num;
    new_keyh.field.debounce_status = 1;

    /* Debounce has not counted out. */
    new_keyl.field.state = 0;

    /* Key activity indication. */
    new_keyl.field.same = 0;

    if (event == MAKE_EVENT)
    {
        /* For MAKE key (key pressed). */
        new_keyl.field.trans = 0;

        /* Shouldn't this be Make_Count? */
        new_keyl.field.count = Ext_Cb2.field.Break_Count;
    }
    else
    {
        /* For BREAK key (key released). */
        new_keyl.field.trans = 1;

        /* Shouldn't this be Break_Count? */
        new_keyl.field.count = Ext_Cb2.field.Make_Count;
    }
}

/**
 * ****************************************************************************
 * FUNCTION: find_paths
 *
 * @return, FALSE if no paths found, Otherwise returns TRUE.
 *
 * @return, DOWN_STATE  scan_address
 *          change_make_key
 *
 * ****************************************************************************
 */
FLAG find_paths(BYTE change_make_key, BYTE scan_address)
{
    FLAG paths_found, done;
    BYTE paths, temp, bits;
    BYTE first_address;

    /* 
     * For the shift and bit test to work, "bits" MUST be unsigned.  (Right
     * shifting an unsigned quantity fills vacated bits with zeros.  Right
     * shifting a signed quantity will fill vacated bits with sign bits on some
     * machines and zero bits on others.)
     */

    done = FALSE;
    paths_found = FALSE;
    first_address = scan_address;

    /* Ignore diode key. */
    change_make_key &= ~(diode_key);
    /* change_make_key = bKO_BITS. */

    if(change_make_key == 0) 
	{
        /* paths_found == FALSE */
        done = TRUE;
    }

    if(!done)
    {
        paths = bscan_matrix[scan_address] | change_make_key;

        /* Ignore diode key. */
        paths &= ~(diode_key);
        /* paths = bKO_PATHS. */
        if(paths == 0) 
		{
            /* paths_found == FALSE */
            done = TRUE;
        }
    }

    while(!done)
    {
        scan_address++;

        if(scan_address >= MAX_SCAN_LINES) 	
        /* if(scan_address >= STANDARD_SCAN_LINES) */
		{
            /* Wrap around */
            scan_address = 0;
        }

        if(scan_address == first_address) 
		{
            /* No scan lines left.  paths_found == FALSE */
            done = TRUE;
        }

        if(!done)
        {   
            /* Check Path */

            /* Any paths? */
            temp  = bscan_matrix[scan_address];

            /* Ignore diode key */
            temp &= ~(diode_key);

            if(temp != 0) 
			{
                /* Paths found */

                /* Do paths line up? */
                temp &= paths;
            }

            if(temp != 0)
            {
                /* Paths line up */

                /* Only 1 bit set? */
                if (change_make_key != temp)
                {
                    /* No, ghost exists. */
                    paths_found = TRUE;
                    done = TRUE;
                }

                if (!done)
                {   
                    /* 
                     * The following checks if more than one bit is set to 1.
                     * Because "bits" is unsigned, each shift moves the bit
                     * under test out and a 0 bit in.  When the first 1 bit is
                     * found, if "bits" == 0, this indicates that there is only
                     * one bit set. 
                     */

                    /* Only 1 bit set? */
                    bits = paths;
                    temp = FALSE;

                    do
                    {
                        if (bits & 0x01) 
						{
                            temp = TRUE;
                        }

                        /*
                         * Shift bit out (and a zero bit in)
                         * to check next bit.
                         */
                        bits = bits >> 1;
                    } while (temp == FALSE);

                    if (bits != 0)
                    {   
                        /* No, more than 1 bit set. */
                        paths_found = TRUE;
                        done = TRUE;
                    }
                }
            }
        }
    }

   return (paths_found);
}

/**
 * ****************************************************************************
 * FUNCTION: debounce_key
 *
 * @return
 *
 * @return
 *
 * ****************************************************************************
 */
void debounce_key(void)
{							
    scan.scan_line = new_keyh.field.output;	
    Write_Strobe(scan.scan_line); 
	CapDelay();	
    ITempB01 = Read_Scan_Lines(); 
    ITempB01 = ~ITempB01;            

    ITempB02 = Byte_Mask((BYTE) (new_keyh.field.input));	
    ITempB02 = ITempB02 & ITempB01;                       			

    KSOL=0xFF;	
    KSOH1=0xFF;

    if(ITempB02 != 0)
    {  
        if(new_keyl.field.trans == 0) 
		{
            /* last key detected as a MAKE,  same = 1. */
            new_keyl.field.same = 1;
        }
        else 
		{
            /* last key detected as a BREAK, same = 0. */
            new_keyl.field.same = 0;
        }
    }
    else
    { 
        if(new_keyl.field.trans == 0) 
		{
            /* last key detected as a MAKE,  same = 0. */
            new_keyl.field.same = 0;
        }
        else 
		{
            /* last key detected as a BREAK, same = 1. */
            new_keyl.field.same = 1;
        }
    }

    /* change keyboard debounce time to 5ms */

    /*
     * if (new_keyl.field.state == 0)
     * {  
     *      if (new_keyl.field.count != 0)
     *      {   
     *          new_keyl.field.count--;
     *      }
     *      else
     *      {
     *          if (new_keyl.field.same == 0)
     *          {   
     *              new_keyh.byte = 0;	// Debounce failed, so claer all data
     *              new_keyl.byte = 0;
     *          }
     *          else
     *          {   
     *              new_keyl.field.state = 1;
     *          } 
     *      }
     * }
     * else
     */	
    { 
        if(new_keyl.field.same == 0)
        {
            /* Debounce failed. */
            new_keyh.byte = 0;
            new_keyl.byte = 0;
        }
        else
        {
            /* Debounce OK */
      		change_valid();
        } 
    }
}

/**
 * ****************************************************************************
 * FUNCTION: Read_Scan_Lines - Read KSI[7:0] of the scan matrix
 *
 * @return: Data from sense lines that are enabled.
 *         The lines that are disabled return 1s.
 *
 * @parameter
 *
 * C prototype: BYTE Read_Scan_Lines();
 *
 * ****************************************************************************
 */
BYTE Read_Scan_Lines(void)
{
  	return (KSI);                  
}

/**
 * ****************************************************************************
 * FUNCTION: change_valid
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void change_valid(void)
{
    ITempB04 = Byte_Mask((BYTE) (new_keyh.field.input)); 
    ITempB03 = bscan_matrix[new_keyh.field.output]; 
    ITempB03 = ITempB03 ^ ITempB04;                         
    bscan_matrix[new_keyh.field.output] = ITempB03; 

    /* Ready for BREAK indication. */
	ITempB05 = BREAK_EVENT;
    if ((ITempB04 & ITempB03) != 0)
    {   						
        ITempB05 = MAKE_EVENT;

        /* Set New Typematic Key. */
        typematic.byte = new_keyh.byte;
        scan.TMscale = TM_SCALE;
        bTMcount = bTMdelay;

		etkeytypematic.byte = 0x00;
    }
	
    Send_Scan2(new_keyh, ITempB05);
    new_keyh.byte = 0;
    new_keyl.byte = 0; 
}

/**
 * ****************************************************************************
 * FUNCTION: check_tm - Handle typematic function.
 *
 * @return
 *
 * @parameter, Typematic key
 *
 * ****************************************************************************
 */
void check_tm(uKEY key)
{
 	ITempB02 = FALSE;										
    ITempB01 = Byte_Mask((BYTE) (key.field.input)); 
    ITempB01 = ITempB01 & bscan_matrix[key.field.output];

 	/* Release Typematic key? */
    if(ITempB01 == 0)
    {
        /* Clear Typematic. */
        typematic.byte = 0;
        ITempB02 = TRUE;
    }

    if(!ITempB02)
    {
        /* Count down Prescale. */
        scan.TMscale--;
        if (scan.TMscale != 0) 
		{
            ITempB02 = TRUE;
        }
    }

    if(!ITempB02)
    {
        /* Reload prescale counter. */
        scan.TMscale = TM_SCALE;
        /* Count down TMcount. */
        bTMcount--;
        if (bTMcount != 0) 
		{
            ITempB02 = TRUE;
        }
    }

    if(!ITempB02)
    {
        /* Reload TMcount. */
        bTMcount = bTMrepeat;
        Send_Scan2(key, REPEAT_EVENT);   
    }
}

/**
 * ****************************************************************************
 * FUNCTION: Scan_Init - Initialize internal keyboard (scanner)
 *
 * Lower all KSO lines for scan matrix
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Scan_Init(void)
{
 	KSOL=0x00;  
  	KSOH1=0x00;
	
    /* The function of extend keys */
	if(ExtendMatrix)
	{
		KSOH2=0x00;
		
		Hook_SetGPIOScanPinCtrl();
		Hook_SetGPIOScanPinL();
	}
}

/**
 * ****************************************************************************
 * FUNCTION: Enable_Any_Key_Irq
 *
 * Setup for any key from scanner to generate an interrupt.
 * Lower all KSO lines for scan matrix
 *
 * Lower the "strobe" lines so that when any key is pressed at least one input
 * line will go low.  Any bits (out of the possible 16) that are not used for
 * the internal keyboard (scanner) are left alone.  This is done using
 * Config.Msk_Strobe_H and Config.Msk_Strobe_L.
 *
 * Then, if the internal keyboard (scanner) is enabled, allow the interrupt to
 * occur when a key is pressed on the scanner.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Enable_Any_Key_Irq(void)	
{
    #ifdef DEF_NO_INTERNAL_KEY_SCAN_FUNC
    return;
    #endif

	KSOL=0x00;             
  	KSOH1=0x00;
	
	if(ExtendMatrix)
	{
		KSOH2=0x00;
		Hook_SetGPIOScanPinL();
	}

    /* enable KSI interrupt	*/
	SET_MASK(IER1,Int_KB);				
}

/**
 * ****************************************************************************
 * FUNCTION: Start_Scan_Transmission
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Start_Scan_Transmission(void)
{
    SetServiceSendFlag();
}

/**
 * ****************************************************************************
 * FUNCTION: Check_Scan_Transmission
 *
 * @return, Transmission status indication.  TRUE if ok to transmit.
 *
 * @parameter
 *
 * ****************************************************************************
 */
FLAG Check_Scan_Transmission(void)
{
    FLAG allow_transmission = FALSE;

    if ((Int_Var.Scan_Lock == 0) && (Flag.SCAN_INH == 0)) 
    {
        /* Scanner transmission is locked and inhibited. */ 
        if (scan.kbf_head != scan.kbf_tail) 
		{
            allow_transmission = TRUE;  /* Allow transmission */
        }
    }    

    return (allow_transmission);
}

/**
 * ****************************************************************************
 * FUNCTION: Get_Kbd_Type
 *
 * @return, the type of the internal keyboard.
 * 0 = U.S. keyboard
 * 1 = Japanese keyboard.
 *
 * @parameter
 *
 * ****************************************************************************
 */
FLAG Get_Kbd_Type(void)
{
	if(1)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

/**
 * ****************************************************************************
 * FUNCTION: Lock_Scan
 * Lock out the scanner via "Scan_Lock" and clear pending scanner "SEND"
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Lock_Scan(void)
{
    Int_Var.Scan_Lock = 1;
    /* Timer_B.fbit.SEND_ENABLE = 0; */
}

/**
 * ****************************************************************************
 * FUNCTION: Init_Kbd - Scanner Keyboard Initialization.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Unlock_Scan(void) 
{
	Int_Var.Scan_Lock = 0;
}

/**
 * ****************************************************************************
 * check kso16 or kso17 support or not
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckKSO1617Support(void)
{
	ExtendScanPin = 0x00;

	if((KSO16CtrlReg&0xFB)==0x00)
	{
		ExtendScanPin++;
	}

	if((KSO17CtrlReg&0xFB)==0x00)
	{
		ExtendScanPin++;
	}
	
	if(ExtendScanPin!=0x00)
	{
		ExtendMatrix = 1; 
	} 
	else
	{
		ExtendMatrix = 0;
	}
}

/**
 * ****************************************************************************
 * FUNCTION: Init_Kbd - Scanner Keyboard Initialization.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Init_Kbd(void)
{
	CheckKSO1617Support();

	Scan_Init();  

	Hook_Setup_Scanner_Pntr();
    Setup_Diode_Key();

    new_keyh.byte = 0;
    new_keyl.byte = 0;

	/* for extend keys */
 	ClearExtendKeys();

    Clear_Fn_Keys();

    /* Clear key matrix/buffer */
    Clear_Key();
    Clear_Typematic();

	Enable_Any_Key_Irq();

    /* Clear FN_Key_Break */
    FN_Key_Break = 0x00;
    FN_Key_Break_HS = 0x00;

    ClearFnKeyBundledFalg();
    
    #if KB_FnStickKey
    FnStickKey = 0;
    #endif
}

/**
 * ****************************************************************************
 * FUNCTION:   Setup_Diode_Key
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Setup_Diode_Key(void)
{
 	if(1)
	{   
        /* Simple matrix (NO diodes) */
        diode_key = 0x00;
    }
    else 
	{
        /* N-key (diodes in keyboard) */
        diode_key = 0xFF;
    }
}

/**
 * ****************************************************************************
 * FUNCTION: Clear_Key - Clear local keyboard buffer and related variables.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_Key(void)
{
    BYTE i;

	/* Clear scan matrix. */
    for(i = 0; i < MAX_SCAN_LINES; i++)
	{
        bscan_matrix[i] = 0;
    }

    /* Clear key buffer. */
    for(i = 0; i < KBF_SIZE; i++)
	{
        bKEY_BUFF[i] = 0;
    }

    scan.kbf_head = 0;
    scan.kbf_tail = 0;
    typematic.byte = 0;
    scan.saf_make = 0;
    scan.saf_break = 0;
    scan.saf_keys = 0;
	scan.scan_line = 0;
}

/**
 * ****************************************************************************
 * FUNCTION: Clear_Typematic
 *
 * Set default typematic delay and clear type-matic action.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_Typematic(void)
{
    typematic.byte = 0;
    bTMdelay = TM_DELAY;
    bTMrepeat = TM_REPEAT;
}

/**
 * ****************************************************************************
 * FUNCTION: Set_Typematic - Set the typematic rate.
 *
 * @return
 *
 * @parameter, typematic rate
 * Bit[4:0] Typematic repeat interval
 * Bit[6:5] Delay time
 *
 * ****************************************************************************
 */
const CBYTE repeat_tbl[] =
{
     3,  4,  4,  5,  5,  5,  6,  6,  7,  7,  8, 9, 10, 11, 11, 12,
    13, 15, 16, 18, 20, 21, 23, 25, 27, 30, 33, 37, 40, 43, 47, 50
};
const CBYTE delay_tbl[] = {25, 50, 75, 100};

void Set_Typematic(WORD type_rate)
{
    Save_Typematic = type_rate;   /* Save for suspend/resume. */

    /* Bit 4 - 0 typematic repeat interval index. */
    bTMrepeat = repeat_tbl[type_rate & 0x1F];

    /* Bit 6 - 5 delay time index. */
    bTMdelay = delay_tbl[(type_rate >> 5) & 0x03];
}


/**
 * ****************************************************************************
 * FUNCTION: Handle_Scan
 *
 * Service the local keyboard in response to a key press.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_scan(void)
{
 	BYTE scan_activity;
  // CM +
  if( IS_MASK_SET(SysStatus2,LidCloseKBMSDis) )
      return;
  // CM -
	
	
    if(Timer_A.fbit.TMR_SCAN == 0)
    {
        Timer_A.fbit.TMR_SCAN = 1;	// Activate scan timer.
    }
	
    /* extend keys */
	if(ExtendMatrix)
	{
		scan_activity = scan_keys()|bScanExtendKeys();	 
	}
	else 
	{
        /* Scan keyboard matrix one time. */
		scan_activity = scan_keys();
	}

    /* There is scan activity. */
  	if (scan_activity)
    {   

	}
    /* No scan activity */
    else
    {
        /* Disable the scan timer. */
        Timer_A.fbit.TMR_SCAN = 0;

        /* Clear any pending service request. */
        F_Service_KEY = 0;

        /* Enable Any Key interrupt. */
        Enable_Any_Key_Irq();

        /* Clear FN_Key_Break */
        FN_Key_Break = 0x00;
        FN_Key_Break_HS = 0x00;

        ClearFnKeyBundledFalg();
  	}
}

/* SCANNER KEY BUFFER ROUTINES */
/**
 * ****************************************************************************
 * FUNCTION:   Get_Buffer
 *
 * Get data byte from keyboard buffer (if not empty) and update "scan.kbf_head"
 * with new index into keyboard buffer.
 *
 * @return, Data from buffer (WORD of all 1's if buffer is empty).
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE Get_Buffer(void)
{
    BYTE buffer_data;

    if(scan.kbf_head != scan.kbf_tail)
    {
		buffer_data = bKEY_BUFF[scan.kbf_head++];
        if(scan.kbf_head >= KBF_SIZE) 
		{
            scan.kbf_head = 0;
        }
    }
    else
    {
        /* Buffer is empty. */
        buffer_data = 0xFF;
    }
    return (buffer_data);
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_Mark - Mark local keyboard buffer tail.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Buffer_Mark(void)
{
    /* Copy scan.kbf_tail to scan.kbf_mark. */
    scan.kbf_mark = scan.kbf_tail;
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_Key
 *
 * @return, TRUE operation successful, FALSE unsuccessful.
 *
 * @parameter, Row/Column (0iii,oooo) to put in buffer.
 *
 * ****************************************************************************
 */
FLAG Buffer_Key(BYTE row_column)
{
    /* Ready for successful operation */
    FLAG ready = TRUE;

    /* Store Data to Buffer Tail */
    bKEY_BUFF[scan.kbf_tail] = row_column;

    /* Increment Buffer Tail (pointer) */
    scan.kbf_tail++;

    if(scan.kbf_tail >= KBF_SIZE) 
	{
        /* Wrap pointer if too large. */
        scan.kbf_tail = 0;
    }

    /* Check Overflow */
    if(scan.kbf_tail == scan.kbf_head)
    {
        /* Overflow */

        /* Restore buffer marker. */
        scan.kbf_tail = scan.kbf_mark;

        /* Set OverFlow Mark. */
        bKEY_BUFF[scan.kbf_tail] = 0;

        /* Overflow Indication !!! */
        ready = FALSE;
    }

    return (ready);
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_String
 *
 * Places a string in the local keyboard buffer (without a terminating NULL).
 * Call Buffer_Key to place data in key buffer.
 *
 * @return, 0x80 if buffer overflow error, else 0.
 *
 * @parameter, Pointer to null terminated string.
 *
 * ****************************************************************************
 */
BYTE Buffer_String(const CBYTE *pntr)
{
    register BYTE error = 0;

    while((*pntr != 0) && (error == 0))
    {
        if(Buffer_Key(*pntr++) == 0)
        {
            /* Indicate Overflow */
            error = 0x80;
        }
    }

    return (error);
}

/* The functions of extend keys */

/**
 * ****************************************************************************
 * The function of Scaning extend keys
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE bScanExtendKeys(void)
{
	ITempB06 = FALSE;

    /* buffer half-full */
    if(check_keyscan_buffer_half_full()!=0x00)
    {
        return(0x01);
    }
    
	extendscan.saf_break = 0;
	extendscan.saf_make= 0;
	extendscan.saf_keys= 0;

 	if (etkeytypematic.byte) 
	{
     	CheckEtKeystm(etkeytypematic);	// Check typematic. 
 	}

	if(new_extendkey.byte)   		
 	{
        /* Yes, bounding. */
     	extendscan.saf_make = 1;  
      	DebounceExtendkey();
      	ITempB06 = TRUE;
 	}

	if(ITempB06 == FALSE)
	{
        for(ITempB01=0;ITempB01<ExtendScanPin;ITempB01++)
        {
			SetExtendScanLines(ITempB01);
      CapDelay();          //msmart
      CapDelay();          //msmart
      CapDelay();          //msmart
			ITempB02 = KSI;
			ITempB02 = (~ITempB02)^bscan_matrix[ITempB01+STANDARD_SCAN_LINES];
            if(ITempB02 != 0) 
           	{
				CheckExtendKeys(ITempB02,(ITempB01+STANDARD_SCAN_LINES));
          	}

            if(bscan_matrix[ITempB01+STANDARD_SCAN_LINES]) 
          	{
                /* Here, if current still active. */

                /* Set keys active bits. Check all key release. */
                extendscan.saf_keys = 1;
            }   
            
            KSOH2 = 0xFF;
		    Hook_SetGPIOScanPinH();
        }
	}

	if(extendstatus.field.ghost == 1)
 	{
	    /* Ghost flag is set. */
     	extendstatus.field.ghost = 0;
     	ITempB01 = extendstatus.byte & 7;
   		if(ITempB01 == 0)
     	{  
        	ITempB01 = Byte_Mask((BYTE)(new_extendkey.field.input));
        	ITempB01 = ITempB01 & diode_key; 
  		}
    	if(ITempB01 == 0)
      	{
            /* Clear new key. */
        	new_extendkey.byte = 0;
        	extendstatus.byte = 0;
        }
      IP_GHOSTBEEP=TRUE;
	} 
	
	return((BYTE) (extendscan.saf_make + extendscan.saf_break +
            extendscan.saf_keys));
}

/**
 * ****************************************************************************
 * The function of Setting extend scan lines
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void SetExtendScanLines(BYTE scan_line)
{
	if(scan_line<0x02)
	{
		KSOL = 0xFF;
		KSOH1 = 0xFF;
		KSOH2 = ~(0x01<<scan_line);
		Hook_SetGPIOScanPinH();
	}
	else 
	{
		KSOL = 0xFF;
		KSOH1 = 0xFF;
		KSOH2 = 0xFF;
		Hook_SetGPIOScanPinL();
	}
    CapDelay();
}

/**
 * ****************************************************************************
 * The function of Setting extend scan lines
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckExtendKeys(BYTE KSI_bit_num, BYTE scan_address)
{
    BYTE flag;
    BYTE change_make_key;
       
	flag = FALSE;				
	if(extendstatus.field.ghost == 1)
 	{
    	flag = TRUE;
   	}
	else
  	{
        /* Find 0 -> 1 's. */
 		change_make_key = KSI_bit_num & ~bscan_matrix[scan_address];
     	if(change_make_key == 0) 
		{
        	flag = FALSE;
     	}
     	else if(find_paths(change_make_key, scan_address)) 
		{
    	    /* TRUE if ghost keys. */
          	flag = TRUE;
      	}
	}

	if(flag)
 	{
        /* Ghost exists. */

        /* Set ghost flag. */
     	extendstatus.field.ghost = 1;
      	KSI_bit_num &= bscan_matrix[scan_address] | diode_key;
	}

	ITempB03 = 0x00;
    while (KSI_bit_num != 0)
    {   
        if (KSI_bit_num & 0x01) 	
		{
    		if (bscan_matrix[scan_address] & Byte_Mask(ITempB03))
   		 	{  
        		if (extendscan.saf_break == 0) 
				{
            		extendscan.saf_break = 1;
					ExtendKeyDebounce(ITempB03, scan_address, BREAK_EVENT);
        		}
    		}
    		else
    		{
        		if (extendscan.saf_make == 0) 
				{
            		extendscan.saf_make = 1;
					ExtendKeyDebounce(ITempB03, scan_address, MAKE_EVENT);
        		}
    		} 
        }
        ITempB03++;

        /* Shift bit out (and a zero bit in) to check next bit. */
        KSI_bit_num = KSI_bit_num >> 1;
    }
}

/**
 * ****************************************************************************
 * The function of extend keys debounce
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ExtendKeyDebounce(BYTE KSI_bit_num, BYTE scan_address, BYTE event)
{
    new_extendkey.field.output = scan_address-STANDARD_SCAN_LINES;
    new_extendkey.field.input = KSI_bit_num;
    new_extendkey.field.debounce_status = 1;

    /* Debounce has not counted out. */
    extendstatus.field.state = 0;

    /* Key activity indication. */
    extendstatus.field.same = 0;

    if (event == MAKE_EVENT)
    { 
        extendstatus.field.trans = 0;

        /* Ext_Cb2.field.Break_Count; */
        extendstatus.field.count = 1;
    }
    else
    {  
        extendstatus.field.trans = 1;

        /* Ext_Cb2.field.Make_Count; */
        extendstatus.field.count = 1;
    }
}

/**
 * ****************************************************************************
 * The function of extend keys debounce
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void DebounceExtendkey(void)
{
	SetExtendScanLines(new_extendkey.field.output);
	ITempB04 = Read_Scan_Lines();
	ITempB04 = ~ITempB04;
 	ITempB05 = Byte_Mask((BYTE)(new_extendkey.field.input)); 
	ITempB05 = ITempB05 & ITempB04;

    KSOH2 = 0xFF;

	if(ITempB05 != 0)
    {
 	    /* key make */
        if(extendstatus.field.trans == 0)
		{
            /* last key detected as a MAKE, same = 1. */
            extendstatus.field.same = 1;
        }
        else 
		{
            /* last key detected as a BREAK, same = 0. */
            extendstatus.field.same = 0;
        }
    }
    else
    { 
        if(extendstatus.field.trans == 0) 
		{
            /* last key detected as a MAKE, same = 0. */
            extendstatus.field.same = 0;
        }
        else 
		{
            /* last key detected as a BREAK, same = 1. */
            extendstatus.field.same = 1;
        }
    } 

	/* 1 means debounce counted out. */

    /*
    if(extendstatus.field.state == 0)
    {  
        if (extendstatus.field.count != 0)
        {  
            extendstatus.field.count--;
        }
        else
        {
            if(extendstatus.field.same == 0)
            {
                new_extendkey.byte = 0;
                extendstatus.byte = 0;
            }
            else
            {  
                extendstatus.field.state = 1;
            } 
        } 
    }
    else
    */
    { 
        if(extendstatus.field.same == 0)
        {   
            new_extendkey.byte = 0;	/* Debounce failed. */
            extendstatus.byte = 0;
        }
        else
        {   
          	CheckExtendKeyValid();
        } 
    }
}

/**
 * ****************************************************************************
 * The function of checking extend key valid
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckExtendKeyValid(void)
{
    ITempB02 = Byte_Mask((BYTE) (new_extendkey.field.input)); 
    ITempB01 = bscan_matrix[(new_extendkey.field.output + STANDARD_SCAN_LINES)];
    ITempB01 = ITempB01 ^ ITempB02;                        
    bscan_matrix[(new_extendkey.field.output + STANDARD_SCAN_LINES)] = ITempB01; 

	ITempB03 = BREAK_EVENT;                       
    if ((ITempB02 & ITempB01) != 0)
    {   						
        ITempB03 = MAKE_EVENT;  							
        etkeytypematic.byte = new_extendkey.byte;
        extendscan.TMscale = TM_SCALE;
        bTMcount = bTMdelay;
		
		typematic.byte = 0x00;
    }
	
    Send_EtScan2(new_extendkey, ITempB03);
    new_extendkey.byte = 0;
    extendstatus.byte = 0;
}

/**
 * ****************************************************************************
 * Send_EtScan2
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Send_EtScan2(uKEY key, BYTE event)
{	
	if(IS_MASK_SET(UtilityFlag,KBDebug))
	{
		if(KeyScanACK==KU_ACK_Start)
		{
			if(event==MAKE_EVENT)
			{
				KeyScanKSI = key.field.input;
				KeyScanKSO = key.field.output;
				KeyScanACK = KU_ACK_FullKeyboard;
			}
		}
	}
	else
	{
    	ITempB02 = (key.field.input * 3) + key.field.output; 
		ITempB01 = *(Extendkey_Table_Pntr + ITempB02);

		Et_Hook_keyboard(key.field.input, key.field.output);
    	Send_Key(ITempB01, event);
	}
}

/**
 * ****************************************************************************
 * The function of clearing extend keys
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ClearExtendKeys(void)
{
	new_extendkey.byte = 0;		
	extendstatus.byte = 0;
	etkeytypematic.byte = 0;
}

/**
 * ****************************************************************************
 * The function of checking extend keys Typematic
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckEtKeystm(uKEY key)
{
	ITempB02 = FALSE;
    ITempB01 = Byte_Mask((BYTE) (key.field.input)); 
    ITempB01 = ITempB01 & bscan_matrix[key.field.output+STANDARD_SCAN_LINES];

    if(ITempB01 == 0) 
    {
        etkeytypematic.byte = 0;
        ITempB02 = TRUE;
    }

    if(!ITempB02)
    {
        extendscan.TMscale--;
        if(extendscan.TMscale != 0) 
		{
            ITempB02 = TRUE;
        }
    }

    if(!ITempB02)
    {
        extendscan.TMscale = TM_SCALE;
        bTMcount--;            
        if(bTMcount != 0) 
		{
            ITempB02 = TRUE;
        }
    }

    if(!ITempB02)
    {
        bTMcount = bTMrepeat;   
        Send_EtScan2(key,REPEAT_EVENT);  
    }
}

/* Hardware scan keyboard */
#else

/**
 * ****************************************************************************
 * FUNCTION:   Get_Buffer
 *
 * Get data byte from keyboard buffer (if not empty) and update "scan.kbf_head"
 * with new index into keyboard buffer.
 *
 * @return, Data from buffer (WORD of all 1's if buffer is empty).
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE Get_Buffer(void)
{
    BYTE buffer_data;

    if (scan.kbf_head != scan.kbf_tail)
    {
		buffer_data = bKEY_BUFF[scan.kbf_head++];
        if (scan.kbf_head >= KBF_SIZE) 
		{
            scan.kbf_head = 0;
        }
    }
    else
    {
        /* Buffer is empty. */
        buffer_data = 0xFF;
    }
    return (buffer_data);
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_Mark - Mark local keyboard buffer tail.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Buffer_Mark(void)
{
    /* Copy scan.kbf_tail to scan.kbf_mark. */
    scan.kbf_mark = scan.kbf_tail;
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_Key
 *
 * @return, TRUE operation successful, FALSE unsuccessful.
 *
 * @parameter, Row/Column (0iii,oooo) to put in buffer.
 *
 * ****************************************************************************
 */
FLAG Buffer_Key(BYTE row_column)
{
    /* Ready for successful operation */
    FLAG ready = TRUE;

    /* Store Data to Buffer Tail */
    bKEY_BUFF[scan.kbf_tail] = row_column;

    /* Increment Buffer Tail (pointer) */
    scan.kbf_tail++;

    if(scan.kbf_tail >= KBF_SIZE) 
	{
        /* Wrap pointer if too large. */
        scan.kbf_tail = 0;
    }

    /* Check Overflow */
    if(scan.kbf_tail == scan.kbf_head)
    {   
        /* Overflow */

        /* Restore buffer marker. */
        scan.kbf_tail = scan.kbf_mark;

        /* Set OverFlow Mark. */
        bKEY_BUFF[scan.kbf_tail] = 0;

        /* Overflow Indication !!! */
        ready = FALSE;
    }

    return (ready);
}

/**
 * ****************************************************************************
 * FUNCTION:   Buffer_String
 *
 * Places a string in the local keyboard buffer (without a terminating NULL).
 * Call Buffer_Key to place data in key buffer.
 *
 * @return, 0x80 if buffer overflow error, else 0.
 *
 * @parameter, Pointer to null terminated string.
 *
 * ****************************************************************************
 */
BYTE Buffer_String(const CBYTE *pntr)
{
    register BYTE error = 0;

    while((*pntr != 0) && (error == 0))
    {
        if(Buffer_Key(*pntr++) == 0)
        {
            /* Indicate Overflow */
            error = 0x80;
        }
    }

    return (error);
}

/**
 * ****************************************************************************
 * FUNCTION: Get_Kbd_Type
 *
 * @return, the type of the internal keyboard.
 * 0 = U.S. keyboard
 * 1 = Japanese keyboard
 *
 * @parameter
 *
 * ****************************************************************************
 */
FLAG Get_Kbd_Type(void)
{
	if(1)
	{
		return(0);
	}
	else
	{
		return(1);
	}
}

/**
 * ****************************************************************************
 * FUNCTION: find_paths
 *
 * @return, FALSE if no paths found, Otherwise returns TRUE.
 *
 * @parameter,
 * DOWN_STATE  scan_address
 * change_make_key
 *
 * ****************************************************************************
 */
FLAG find_paths(BYTE change_make_key, BYTE scan_address)
{
    FLAG paths_found, done;
    BYTE paths, temp, bits;
    BYTE first_address;

    /*
     * For the shift and bit test to work, "bits" MUST be unsigned.  (Right
     * shifting an unsigned quantity fills vacated bits with zeros.  Right
     * shifting a signed quantity will fill vacated bits with sign bits on some
     * machines and zero bits on others.)
     */

    done = FALSE;
    paths_found = FALSE;
    first_address = scan_address;

    if(change_make_key == 0) 
	{
        /* paths_found == FALSE */
        done = TRUE;
    }

    if(!done)
    {
        paths = bscan_matrix[scan_address] | change_make_key;

        if(paths == 0) 
		{
            /* paths_found == FALSE */
            done = TRUE;
        }
    }

    while(!done)
    {
        scan_address++;

        if(scan_address >= 18) 	
		{
            /* Wrap around */
            scan_address = 0;
        }

        if(scan_address == first_address) 
		{
            /* No scan lines left.  paths_found == FALSE */
            done = TRUE;
        }

        if(!done)
        {
            /* Check Path */

            /* Any paths? */
            temp  = bscan_matrix[scan_address];

            /* Ignore diode key */
            /* temp &= ~(diode_key); */

            if(temp != 0) 
			{
                /* Paths found */

                /* Do paths line up? */
                temp &= paths;
            }

            if(temp != 0)
            {   
                /* Paths line up */

                /* Only 1 bit set? */
                if(change_make_key != temp)
                {
                    /* No, ghost exists. */
                    paths_found = TRUE;
                    done = TRUE;
                }

                if(!done)
                {
                    /* The following checks if more than one bit is set to 1.
                     * Because "bits" is unsigned, each shift moves the bit
                     * under test out and a 0 bit in.  When the first 1 bit is
                     * found, if "bits" == 0, this indicates that there is only
                     * one bit set.
                     */

                    /* Only 1 bit set? */
                    bits = paths;
                    temp = FALSE;

                    do
                    {
                        if(bits & 0x01) 
						{
                            temp = TRUE;
                        }

                        /*
                         * Shift bit out (and a zero bit in)
                         * to check next bit.
                         */
                        bits = bits >> 1;
                    } while (temp == FALSE);

                    if(bits != 0)
                    {
                        /* No, more than 1 bit set. */
                        paths_found = TRUE;
                        done = TRUE;
                    }
                }
            }
        }
    }

   return (paths_found);
}

/**
 * ****************************************************************************
 * Service the local keyboard in response to a key press. (hardware scan)
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void service_scan(void)
{
    BYTE kso_index, ksi_index;
    BYTE ksitemp;
    //BYTE extend_key;
    //ECReg *SDRPointer;
    BYTE *SDRPointer;

    /* All key is pressed */
    if(Timer_A.fbit.TMR_SCAN == 0)
    {
	    /* Activate scan timer. */
        Timer_A.fbit.TMR_SCAN = 1;
    }

    /* buffer half-full */
    if(check_keyscan_buffer_half_full()!=0x00)
    {

    }
    else
    {
        scan.saf_keys = 0;
        ksitemp = 0x00;
        SDRPointer = (BYTE *)&KSO0LSDR;

    /* Scan KSO0 ~ 15, 16, or 17 */
    for(kso_index=0x00; kso_index<(16+ExtendScanPin); kso_index++)
    {
        /* Read KSI and inverse */
        ksitemp = ~(SDRPointer[kso_index]);
        ksitemp = ksitemp ^ bscan_matrix[kso_index];
        
        /* Status change (make-->brak, or break-->make) */
        if(ksitemp!=0x00)
        {
            /* KSO16, KSO17 */
            if(kso_index>15)
            {
                KB_HW_ExtendKeyIndex=0x01;
            }
            /* KSO0 ~ KSO15 */
            else
            {
                KB_HW_ExtendKeyIndex=0x00;
            }

            /* Check KSI0 ~ 7 */
            for(ksi_index=0x00; ksi_index<8; ksi_index++)
            {
                if(IS_MASK_SET(ksitemp, BIT(ksi_index)))
                {
                    /* KSO16, KSO17 */
                    if(KB_HW_ExtendKeyIndex==0x01)
                    {
                        /* Save KSO for type union KEY */
                        new_keyh.field.output = (kso_index-16);
                    }
                    /* KSO0 ~ KSO15 */
                    else
                    {
                        /* Save KSO for type union KEY */
                        new_keyh.field.output = kso_index;
                    }

                    /* Save KSI for type union KEY */
                    new_keyh.field.input = ksi_index;
 
                    /* Make */
                    if(IS_MASK_CLEAR(bscan_matrix[kso_index], BIT(ksi_index)))
                    {
                        /* No ghost key */
                        if(find_paths(ksitemp, kso_index)==FALSE)
                        {
                            /* Key make */
                            Send_Scan2(new_keyh, MAKE_EVENT);

                            /* Set typematic key */
                            typematic.byte = new_keyh.byte;

                            /* Repeat time */
                            scan.TMscale = TM_SCALE;

                            /* Delay time */
                            bTMcount = bTMdelay;

                            /* KSO16, KSO17 */
                            if(KB_HW_ExtendKeyIndex==0x01)
                            {
                                /* Set key typematic of KSO16, KSO17 */
                                KB_HW_ExtendKeyTypematic=0x01;
                            }
                            else
                            {
                                /* Set key typematic of KSO0 ~ KSO15 */
                                KB_HW_ExtendKeyTypematic=0x00;
                            }
                            SET_MASK(bscan_matrix[kso_index], BIT(ksi_index));
                        }
                        /* ghost key found */
                        else
                        {
                            /* Clear Typematic. */
                            typematic.byte = 0;
                        }
                    }
                    /* Break */
                    else
                    {
                        /* Key break; */
                        Send_Scan2(new_keyh, BREAK_EVENT);
                        CLEAR_MASK(bscan_matrix[kso_index], BIT(ksi_index));
                    }
                }
            }
        }
        
        /* Any key still active. */
        if(bscan_matrix[kso_index]) 
        {  					    
            /* Here, if current still active. */

            /* Set keys active bits. Check all key release. */
            scan.saf_keys = 1; 
        }  
    }

    /* All key break */
    if((scan.saf_keys)==0x00)
    {
        Clear_AllKeysBreak();
    }
    }

    /* Write to clear data valid bit and enabling interrupt */
    /* Scan data valid W/C */
    SET_MASK(SDSR, SDV);

    /* Enable interrupt */
    SET_MASK(IER10, Int_KBSDVIntr); // Enable interrupt 
}

/**
 * ****************************************************************************
 * FUNCTION: Init_Kbd - Scanner Keyboard Initialization.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Unlock_Scan(void) 
{
	Int_Var.Scan_Lock = 0;
}

/**
 * ****************************************************************************
 * FUNCTION: Lock_Scan
 * Lock out the scanner via "Scan_Lock" and clear pending scanner "SEND"
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Lock_Scan(void)
{
    Int_Var.Scan_Lock = 1;
    /* Timer_B.fbit.SEND_ENABLE = 0; */
}

/**
 * ****************************************************************************
 * FUNCTION: Set_Typematic - Set the typematic rate.
 *
 * @return
 *
 * @parameter, typematic rate
 * Bit[4:0] Typematic repeat interval
 * Bit[6:5] Delay time
 *
 * ****************************************************************************
 */
const CBYTE repeat_tbl[] =
{
     3,  4,  4,  5,  5,  5,  6,  6,  7,  7,  8, 9, 10, 11, 11, 12,
    13, 15, 16, 18, 20, 21, 23, 25, 27, 30, 33, 37, 40, 43, 47, 50
};
const CBYTE delay_tbl[] = {25, 50, 75, 100};

void Set_Typematic(WORD type_rate)
{
    Save_Typematic = type_rate;   /* Save for suspend/resume. */

    /* Bit 4 - 0 typematic repeat interval index. */
    bTMrepeat = repeat_tbl[type_rate & 0x1F];

    /* Bit 6 - 5 delay time index. */
    bTMdelay = delay_tbl[(type_rate >> 5) & 0x03];
}

/**
 * ****************************************************************************
 * Clear local keyboard buffer and related variables.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_Key(void)
{
    BYTE i;

	/* Clear scan matrix. */
    for(i = 0; i < MAX_SCAN_LINES; i++)
	{
        bscan_matrix[i] = 0;
    }

    /* Clear key buffer. */
    for(i = 0; i < KBF_SIZE; i++)
	{
        bKEY_BUFF[i] = 0;
    }

    /* Clear keyboard buffer head */
    scan.kbf_head = 0;

    /* Clear keyboard buffer tail */
    scan.kbf_tail = 0;

    /* Clear Scan activity flag - keys active. */
    scan.saf_keys = 0;

    /* Clear KSO KSI */
    new_keyh.byte = 0;

    Clear_AllKeysBreak();
    
    #if KB_FnStickKey
    FnStickKey = 0;
    #endif
}

/**
 * ****************************************************************************
 * Set default typematic delay and clear type-matic action.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_Typematic(void)
{
    typematic.byte = 0;
    bTMdelay = TM_DELAY;
    bTMrepeat = TM_REPEAT;
}

/**
 * ****************************************************************************
 * Scanner Keyboard Initialization.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Init_Kbd(void)
{
    
    /* Check KSO16 and KSO17 */
	CheckKSO1617Support();

    /* to define the possible scanner tables */
	Hook_Setup_Scanner_Pntr();

    /* Clear key matrix/buffer */
    Clear_Key();

    /* Clear Typematic */ 
    Clear_Typematic();

    /* Enable any key interrupt (HW mode) */
	Enable_Any_Key_Irq();
}

/**
 * ****************************************************************************
 * check kso 16 and kso 17 support or not.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void CheckKSO1617Support(void)
{
	ExtendScanPin = 0x00;

	if((KSO16CtrlReg&0xFB)==0x00)
	{
		ExtendScanPin++;
	}

	if((KSO17CtrlReg&0xFB)==0x00)
	{
		ExtendScanPin++;
	}

    if(ExtendScanPin>2)
    {
        ExtendScanPin=2;
    }

    /* Initialize internal keyboard (HW scanner) */
    Scan_Init();
}

/**
 * ****************************************************************************
 * Initialize internal keyboard (scanner)
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Scan_Init(void)
{
    /* Scan data enable, interrupt from scan data valid enable, 3 round */
    SDC1R = SDEN+INTSDVEN+SLS_03_Round;

    /* 18 x 8 */
    if(ExtendScanPin==0x02)
    {
        /* 18 x 8, Wait KSO high delay 55us */
        SDC2R = KSOPCS1+WKSOHDLY_55us;
    }
    /* 17 x 8 */
    else if(ExtendScanPin==0x01)
    {
        /* 17 x 8, Wait KSO high delay 55us */
        SDC2R = KSOPCS0+WKSOHDLY_55us;
    }
    else
    {
        /* 16 x 8 */

        /* 16 x 8, Wait KSO high delay 55us */
        SDC2R = WKSOHDLY_55us;
    }
    
    /* Wait KSO low delay 29us, spacing delay between rounds 2ms. */
    SDC3R = WKSOLDLY_29us+SDLYBR_02ms;
}

/**
 * ****************************************************************************
 * Setup for any key from scanner to generate an interrupt.
 *
 * Lower the "strobe" lines so that when any key is pressed at least one input
 * line will go low.  Any bits (out of the possible 16) that are not used for
 * the internal keyboard (scanner) are left alone.  This is done using
 * Config.Msk_Strobe_H and Config.Msk_Strobe_L.
 *
 * Then, if the internal keyboard (scanner) is enabled, allow the interrupt to
 * occur when a key is pressed on the scanner.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Enable_Any_Key_Irq(void)
{
	ISR10 = Int_KBSDVIntr;
    SET_MASK(IER10, Int_KBSDVIntr);  
}

/**
 * ****************************************************************************
 * FUNCTION: check_tm - Handle typematic function.
 *
 * @return
 *
 * @parameter, Typematic key
 *
 * ****************************************************************************
 */
void check_tm(uKEY key)
{
    BYTE sendit;
    BYTE check;

 	sendit = FALSE;										
    check = Byte_Mask((BYTE) (key.field.input)); 
    if(KB_HW_ExtendKeyTypematic==0x00)
    {
        check = check & bscan_matrix[key.field.output];
    }
    else
    {
        check = check & bscan_matrix[(key.field.output + 16)];
    }

    /* Release Typematic key? */
    if(check == 0)
    {
	    /* Clear Typematic. */
        typematic.byte = 0;
        sendit = TRUE;
    }

    if(!sendit)
    {
        /* Count down Prescale. */
        scan.TMscale--;
        if (scan.TMscale != 0) 
		{
            sendit = TRUE;
        }
    }

    if(!sendit)
    {
        /* Reload prescale counter. */
        scan.TMscale = TM_SCALE;
        /* Count down TMcount. */
        bTMcount--;
        if (bTMcount != 0) 
		{
            sendit = TRUE;
        }
    }

    if(!sendit)
    {
        if(KB_HW_ExtendKeyTypematic==0x00)
        {
            KB_HW_ExtendKeyIndex=0x00;
        }
        else
        {
            KB_HW_ExtendKeyIndex=0x01;
        }
        /* Reload TMcount. */
        bTMcount = bTMrepeat;
        Send_Scan2(key, REPEAT_EVENT);   
    }
}

/**
 * ****************************************************************************
 *
 * make sure all key have break code.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void HW_KB_Check_TM(void)
{
    BYTE kso_index;
 	if(typematic.byte) 
	{
        /* buffer half-full */
        if(check_keyscan_buffer_half_full()!=0x00)
        {

        }
        else
        {
            /* Check typematic. */
     	    check_tm(typematic);
        }
 	}

    /* All FFh */
    if(IS_MASK_SET(SDSR, BIT1))
    {
        for(kso_index=0x00; kso_index<(16+ExtendScanPin); kso_index++)
        {
            if(bscan_matrix[kso_index]!=0x00)
            {
                /* Request scanner service. */
                F_Service_KEY = 1;
                break;
            }
        }
    }
}

/**
 * ****************************************************************************
 *
 * all key have break
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_AllKeysBreak(void)
{
    /* Disable the scan timer. */
    Timer_A.fbit.TMR_SCAN = 0;

    /* Clear typematic */
    typematic.byte = 0;

    /* Clear FN_Key_Break */
    FN_Key_Break = 0x00;
    FN_Key_Break_HS = 0x00;

    ClearFnKeyBundledFalg();
    KB_HW_ExtendKeyIndex = 0x00;
    KB_HW_ExtendKeyTypematic = 0x00;
}

/* #ifndef KB_HW_KeyScan */
#endif

/**
 * ****************************************************************************
 *
 * Support PS2 scan code to usb hid or not.
 *
 * @return
 * 0, PS2 scan code mode
 * other, PS2 scan code to usb hid
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE PS2_Scan_Code_To_USB_HID(void)
{
    return(0x00);
}

/**
 * ****************************************************************************
 *
 * key buffer half full or not.
 *
 * @return
 * 0, buffer no half-full
 * 1, buffer half-full
 *
 * @parameter
 *
 * ****************************************************************************
 */
BYTE check_keyscan_buffer_half_full(void)
{
    BYTE lb_pending_size;
    BYTE lb_result;

    /* pre-set buffer no half-full */
    lb_result = 0x00;
        
    if(scan.kbf_head > scan.kbf_tail)
    {
        lb_pending_size = (scan.kbf_tail + KBF_SIZE) - scan.kbf_head;
    }
    else
    {
        lb_pending_size = scan.kbf_tail - scan.kbf_head;
    }

    if(lb_pending_size >= (KBF_SIZE/2))
    {
        /* buffer half-full */
        lb_result = 0x01;
    }

    return(lb_result);
}


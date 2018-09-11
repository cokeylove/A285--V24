/* 
 * ****************************************************************************
 * TITLE: CORE_XLT.C - Scan Matrix handler at CORE level
 *
 * The procedure Send_Scan2 of this module is called to translate keyboard
 * scanner contact events into set 2 scan codes.
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

/* 
 * ****************************************************************************
 * FUNCTION: Send_Scan2
 *
 * Input: key   = Row/Column number
 *                key.field.output = 0 - 15
 *                key.field.input  = 0 - 7
 *
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 *        Scanner_State byte
 *
 *        Ext_Cb3_OVL_STATE
 *
 * On return, Scanner_State byte and Ext_Cb3_OVL_STATE are updated.
 *
 * Combination flag for each row/column
 *     0 = Software key number 2 in Row/Column Table
 *     1 = index for control_ptr_table in Row/Column Table
 *
 * Software Key Number 2
 *     01-7F = Same as Scan Code, Set 2
 *     80-FF = E0-prefix, cursor, printscreen, pause/break, special key
 * ****************************************************************************
 */
void Send_Scan2(uKEY key, BYTE event)
{
  Hook_keyboard_Oem (&key, event);
	if(IS_MASK_SET(UtilityFlag,KBDebug))
	{
		if(KeyScanACK==KU_ACK_Start)
		{
			if(event==MAKE_EVENT)
			{
				KeyScanKSI = key.field.input;
				KeyScanKSO = key.field.output;

                /* Firmware scan keyboard */
                #ifndef KB_HW_KeyScan
				KeyScanACK = KU_ACK_Keyboard;
                /* Hardware scan keyboard */
                #else                   
                if(KB_HW_ExtendKeyIndex==0x00)
                {
                    /* KSO0 ~ KSO15 */
				    KeyScanACK = KU_ACK_Keyboard;
                }
                else
                {
                    /* KSO16, KSO17 */
                    KeyScanACK = KU_ACK_FullKeyboard;
                }
                /* KB_HW_KeyScan */
                #endif
			}
		}
	}
	else
	{
        /* Firmware scan keyboard */
        #ifndef KB_HW_KeyScan
        ITempB04 = (key.field.input << 4) | key.field.output;
		ITempB03 = *(Scanner_Table_Pntr + ITempB04);
        /* Hardware scan keyboard */
        #else
        /* KSO0 ~ KSO15 */
        if(KB_HW_ExtendKeyIndex==0x00)
        {
            ITempB04 = (key.field.input << 4) | key.field.output;
		    ITempB03 = *(Scanner_Table_Pntr + ITempB04);
        }
        /* KSO16, KSO17 */
        else
        {
            ITempB04 = (key.field.input *3) + key.field.output;
            ITempB03 = *(Extendkey_Table_Pntr + ITempB04);
        }
        /* KB_HW_KeyScan */
        #endif

		if( ITempB03 < 0x80 ){
	  		nSSKeyTbl = *(Scanner_LOC_Pntr + ITempB03);
		}

		Hook_keyboard(key.field.input, key.field.output);
    	Send_Key(ITempB03, event);
	}
}

/**
 * ****************************************************************************
 * The function for clearing Fn key bundled flag.
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void ClearFnKeyBundledFalg(void)
{
    BYTE index;

    for(index=0x00; index<16; index++)
    {
        FnKeyBundledFalg[index]=0x00;
    }
}

/**
 * ****************************************************************************
 * The function for checking Fn key bundled flag.
 *
 * @return
 * 0, no fn key status
 * 1, with fn key status
 *
 * @parameter
 * p_table_entry, the value of Rc_ROM_Tables.
 * p_event, MAKE_EVENT or BREAK_EVENT or REPEAT_EVENT
 *
 * ****************************************************************************
 */
BYTE CheckFnKeyBundledFalg(BYTE p_table_entry, BYTE p_event)
{
    BYTE l_Fn_bundled;
    BYTE l_buffer_index;
    BYTE l_buffer_mask;

    /* pre-set no bundled fn key */
    l_Fn_bundled = 0x00;
    
    if(p_table_entry >= SSKEY2_SPE_CODE)
    {
        /* index of buffer "FnKeyBundledFalg" */
        l_buffer_index = ((p_table_entry-0x90)/8);

        /* mask of buffer "FnKeyBundledFalg" */
        l_buffer_mask = ((p_table_entry-0x90)%8);

        /* Key break */
        if(p_event==BREAK_EVENT)
        {
            /* with Fn key */
            if(temp_scanner_state.field.tFN)
            {
                /* with fn key but no bundled fn key flag. */
                if(IS_MASK_CLEAR(FnKeyBundledFalg[l_buffer_index],
                    BIT(l_buffer_mask)))
                {
                    /* with fn key but on bundled fn key flag. */
                    l_Fn_bundled = 0x00;
                }
                else
                {
                    /* with fn key and bundled fn key flag. */
                    l_Fn_bundled = 0x01;
                }
            }
            /* without Fn key */
            else
            {
                if(IS_MASK_SET(FnKeyBundledFalg[l_buffer_index],
                    BIT(l_buffer_mask)))
                {
                    /* no fn key but with bundled fn key flag. */
                    l_Fn_bundled = 0x01;
                }
                else
                {
                    /* no fn key and no bundled fn key flag. */
                    l_Fn_bundled = 0x00;
                }
            }
            CLEAR_MASK(FnKeyBundledFalg[l_buffer_index], BIT(l_buffer_mask));
        }
        /* Key make or repeat */
        else
        {
            #if KB_FnStickKey
            /* Fn key or Stick key flag */
            if(temp_scanner_state.field.tFN || FnStickKey)
            #else
            /* Fn key */
            if(temp_scanner_state.field.tFN)
            #endif
            {
                /* Key make */
                if(p_event==MAKE_EVENT)
                {
                    SET_MASK(FnKeyBundledFalg[l_buffer_index],
                        BIT(l_buffer_mask));
                }
                /* Key repeat */
                else
                {

                }
            }
            /* No Fn key */
            else
            {

            }

            if(temp_scanner_state.field.tFN ||
            IS_MASK_SET(FnKeyBundledFalg[l_buffer_index], BIT(l_buffer_mask)))
            {
                /* with fn key or bundled fn key flag. */
                l_Fn_bundled = 0x01;
            }
        }
    }

    return(l_Fn_bundled);
}

/**
 * ****************************************************************************
 * Send_Key
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Send_Key(BYTE table_entry, BYTE event)
{
    BYTE temp;
    const CBYTE *pntr;
    BYTE temp_table_entry;

    OEM_Hook_Send_Key(&table_entry,event);
//    if(OEM_Hook_Skip_Send_Key(&table_entry,event)==0xFF)
//    {
//        return;
//    }
	
	temp = OEM_Hook_Skip_Send_Key(&table_entry,event);
    if(IS_MASK_SET(SYS_MISC1, ACPI_OS)  && event==BREAK_EVENT){
      if(STICKY_MODE_GET){
        if( FnStickKey == 1 ) {
          FnStickKey = 0;
          STICKY_MODE_CLR;    
          Scanner_State_FN = 0;      
          temp_scanner_state.field.tFN = 0;           
          ECSMI_SCIEvent(STICKY_OFF_EVENT_73);
        } else{
        FnStickKey = 1;
        } 
      }
    }
    if(temp == 0xFF)
    {
        return;
    } 
    temp_table_entry = table_entry;

    /* Mark Buffer in case of overrun. */
    Buffer_Mark();
    
    temp_scanner_state.byte = Scanner_State;

    /* Fn key + any key. */
    if(table_entry >= SSKEY2_OVL_CODE)
    {
        temp = (table_entry - SSKEY2_OVL_CODE);

        /* Multiply 2 because each entry takes 2 bytes */
        temp = temp << 1;
        if(CheckFnKeyBundledFalg(temp_table_entry, event)==0x01)
        {
            /* Increment index to get the odd byte of table entry */
            temp++;
        }

        /* Get a sskey2 value. */
        table_entry = sskey2_overlay_table[temp];
    }
    /* Combination Key */
    else if(table_entry >= SSKEY2_SPE_CODE)
    {
 	    /* Get index into table. */
        temp = (table_entry - SSKEY2_SPE_CODE);

        /* table_entry contains the combination table entry. */
        table_entry = sskey2_A2_table[temp].comb;

        /* pntr points to the first key byte. */
        pntr = sskey2_A2_table[temp].pntr;

		/* Check combination.  Point to the correct byte. */
        pntr += calc_index_comb(table_entry,temp_table_entry,event);

        /* Get a sskey2. */
        table_entry = *pntr;
    }

    /* Generate scan code, set 2. */
    sskey3_proc(table_entry, event);

	/* Update scanner state. */
	Scanner_State = temp_scanner_state.byte;

    #if KB_FnStickKey 
    if(event==BREAK_EVENT)
    {
        /* Fn key */
        if(temp_table_entry!=0x8E)
        {
            FnStickKey=0;
        }
    }
    #endif
}

/**
 * ****************************************************************************
 * Clear_Fn_Keys
 *
 * @return
 *
 * @parameter
 *
 * ****************************************************************************
 */
void Clear_Fn_Keys(void)
{
    //Fn_Make_Key1 = 0;
}

/**
 * ****************************************************************************
 * FUNCTION: sskey3_proc
 *
 * @return
 *
 * @parameter
 * sskey2, Software Key 2
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
/* Key Code Process Table. */
const FUNCT_PTR_V_BS kcp_vector_table[] =
{
    /* index 00h DO_SIMPLE_CODE IBM key number 83,84 */
    simple_code,

    /* index 01h DO_E0_PREFIX_CODE */
    e0_prefix_code,

    /* index 02h DO_CURSOR_SHIFT */
    cursor_shift,

    /* index 03h DO_CURSOR_NUMLOCK */
    cursor_numlock,

    /* index 04h DO_SPECIAL_CODE IBM key number 126 */
    special_code,

    /* index 05h DO_CONTROL_EFFECT Shift, Ctrl, Alt, Numlock */
    control_effect,

	/* index 06h DO_CONTROL_EFFECT_E0 Ctrl, Alt */
    control_effect_e0,

	/* index 07h DO_COSTOMER_FUNCTION costomer function key */
    costomer_function
};

void sskey3_proc(BYTE sskey2, BYTE event)
{
    BYTE code_byte;
    BYTE index;

	
    /* Null code */
    if(sskey2 == 0)
    {   			
        ;					 
    }
    else if((sskey2 & 0x80) == 0)
    {
	    /* 01h through 7Fh = scan code. */
     	//simple_code(sskey2, event);
     	//ivan	
       //simple_code(sskey2, event);  // 01h through 7Fh = scan code.
       
       if( tmpSSKeyCnt != 0)
       {
        
       	//if(tmpSSKey2 < sskey2)
       	if(ntmpKeyTbl < nSSKeyTbl)
       	{
		       if(IS_MASK_SET(CombineKeyStatus, b6Fn4Key4) && (0x25 == sskey2) && (REPEAT_EVENT == event))
		       {
			       ;
		       }
		       else if(IS_MASK_SET(CombineKeyStatus, b6Fn4Key4) && (0x25 == sskey2) && (BREAK_EVENT == event))
		       {
			       CLEAR_MASK(CombineKeyStatus, b6Fn4Key4);
		       }
		       else
		       {
       		  simple_code(sskey2, event);  // 01h through 7Fh = scan code.
		       }       		
//			simple_code(sskey2, event); 
			return;
       	}
		else
		{
			simple_code(tmpSSKey2, tmpEvent); 
		}
       	
       }
       tmpSSKey2 = sskey2;
	   tmpEvent = event;
	   ntmpKeyTbl = nSSKeyTbl;
	   tmpSSKeyCnt = 5;

    }
    /* 80h through FFh. */
    else
    {   
        sskey2 &= 0x7F;
        sskey2 = sskey2 << 1;
        code_byte = sskey3_80_table[sskey2+0];
        index = sskey3_80_table[sskey2+1];

        /* Do procedure */
        (kcp_vector_table[index])(code_byte, event);
    } 
}

/**
 * ****************************************************************************
 * FUNCTION: calc_index_comb
 *
 * @return, offset.
 *
 * @parameter, combination setting
 *             Bit   Meaning
 *             ---   --------
 *             7     reserved
 *             6     reserved
 *             5     Fn
 *             4     Num Lock
 *             3     Ctrl
 *             2     Alt
 *             1     Shift
 *             0     reserved
 *       temp_scanner_state
 *
 * ****************************************************************************
 */
BYTE calc_index_comb(BYTE comb, BYTE table_entry, BYTE event)
{
    BYTE offset;
    BYTE bit_num;

    offset  = 0;
    bit_num = 0;

    if (comb & BIT(1))
    {
        /* Combination has Shift. */
    	if (temp_scanner_state.field.tLEFT | temp_scanner_state.field.tRIGHT)
		{
            /* Either left or right shift is pressed. */
            offset |= Byte_Mask(bit_num);
        }
        /* Increment bit position. */
        bit_num++;
    }

    if (comb & BIT(2))
    {
        /* Combination has Alt. */
		if (temp_scanner_state.field.tALT)
        {
            /* Alt is pressed. */
            offset |= Byte_Mask(bit_num);
        }

  	    /* Increment bit position. */
        bit_num++;
    }

   	if (comb & BIT(3))
    {
        /* Combination has Ctrl. */
		if (temp_scanner_state.field.tCONTROL)
        {
            /* Ctrl is pressed. */
            offset |= Byte_Mask(bit_num);
        }

        /* Increment bit position. */
        bit_num++;
    }

    if (comb & BIT(4))
    {
        /* Combination has Num Lock. */
        if (temp_scanner_state.field.tNUM_LOCK)
        {
            /* NumLock has been pressed. */
            offset |= Byte_Mask(bit_num);
        }

        /* Increment bit position. */
        bit_num++;
    }

    if (comb & BIT(5))
    {
        /* Combination has Fn. */
        if(CheckFnKeyBundledFalg(table_entry, event)==0x01)
        {
            offset |= Byte_Mask(bit_num);
        }

        /* Increment bit position. */
        bit_num++;
    }

    if (comb & BIT(6))
    {
        /* Combination has Hook_calc_index_comb_BIT6 */
        if(Hook_calc_index_comb_BIT6()==0xFF)
        {
            offset |= Byte_Mask(bit_num);
        }

  	    /* Increment bit position. */
        bit_num++;
    }

    if (comb & BIT(7))
    {
        /* Combination has Hook_calc_index_comb_BIT7 */
        if(Hook_calc_index_comb_BIT7()==0xFF)
        {
            offset |= Byte_Mask(bit_num);
        }

        /* Increment bit position. */
        bit_num++;
    }
    
    return(offset);
}

/**
 * ****************************************************************************
 * FUNCTION: simple_code
 *
 * @return
 *
 * @parameter
 * scan_code, Set 2
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
void simple_code(BYTE scan_code, BYTE event)
{
    /* ps2 scan code to usb hid no repeat code */
    if((PS2_Scan_Code_To_USB_HID()!=0x00)&&(event == REPEAT_EVENT))
    {

    }
    else
    {
        if (event == BREAK_EVENT)
        {
            /* Buffer break prefix for break contact. F0h is break prefix. */
            Buffer_Key(0xF0);
        }

        /* Buffer base code. */
        Buffer_Key(scan_code); 
    }
}

/**
 * ****************************************************************************
 * FUNCTION: simple_code
 *
 * @return
 * 0x00, not additional key
 * 0x01, additional key [group]
 * 0x02, additional key [Numeric / on US keyboards]
 *
 * @parameter
 * scan_code_a, scan_code, Set 2
 *
 * ****************************************************************************
 */
BYTE CheckAdditionalConditionKey(BYTE scan_code_a)
{
    if(PS2_Scan_Code_To_USB_HID()==0x00)
    {
        /* PS2 scan code mode */

        /* Insert */
        if(scan_code_a==0x70
            /* Delete */
            ||scan_code_a==0x71
            /* Left Arrow */
            ||scan_code_a==0x6B
            /* Home */
            ||scan_code_a==0x6C
            /* End */
            ||scan_code_a==0x69
            /* Up Arrow */
            ||scan_code_a==0x75
            /* Dn Arrow */
            ||scan_code_a==0x72
            /* Page Up */
            ||scan_code_a==0x7D
            /* Page Down */
            ||scan_code_a==0x7A
            /* Right Arrow */
            ||scan_code_a==0x74)
        {
            return(0x01);
        }
        /* Numeric / on US keyboards */
        else if(scan_code_a==0x4A)
        {
            return(0x02); 
        }
        else
        {
            return(0x00);
        }
    }
    else
    {
        /* PS2 scan code to usb hid */
        return(0x00);
    }
}

/**
 * ****************************************************************************
 * FUNCTION: e0_prefix_code
 *
 * @return
 * 0x00, not additional key
 * 0x01, additional key [group]
 * 0x02, additional key [Numeric / on US keyboards]
 *
 * @parameter
 * scan_code, Set 2
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
void e0_prefix_code(BYTE scan_code, BYTE event)
{
    BYTE additional_key;

    additional_key = CheckAdditionalConditionKey(scan_code);

    /* To check make and repeat */
    if(event != BREAK_EVENT)
    {
        /* ps2 scan code to usb hid no repeat code */
        if((PS2_Scan_Code_To_USB_HID()!=0x00)&&(event == REPEAT_EVENT))
        {

        }
        else
        {
            if(additional_key==0x01)
            {
                /* Num Lock ON */
                if(Scanner_State_NUM_LOCK)
                {
                    /* LShift || Rshift */
                    if(Scanner_State_LEFT || Scanner_State_RIGHT)
                    {
                        /* NULL */
                    }
                    else
                    {
                        /* Precede Base Make code with (E0 2A) */
                        Buffer_Key(0xE0);
                        Buffer_Key(0x12);
                    }
                }
                /* Num Lock OFF */
                else
                {
                    if(Scanner_State_LEFT && Scanner_State_RIGHT)
                    {
                        /* Precede Base Make code with (E0 AA) */
                        Buffer_Key(0xE0);
                        Buffer_Key(0xF0);
                        Buffer_Key(0x12);

                        /* Precede Base Make code with (E0 B6) */
                        Buffer_Key(0xE0);
                        Buffer_Key(0xF0);
                        Buffer_Key(0x59);
                    }
                    else if(Scanner_State_LEFT)
                    {
                        /* Precede Base Make code with (E0 AA) */
                        Buffer_Key(0xE0);
                        Buffer_Key(0xF0);
                        Buffer_Key(0x12);
                    }
                    else if(Scanner_State_RIGHT)
                    {
                        /* Precede Base Make code with (E0 B6) */
                        Buffer_Key(0xE0);
                        Buffer_Key(0xF0);
                        Buffer_Key(0x59);
                    }
                    else
                    {
                        /* NULL */
                    }
                }
            }
            else if(additional_key==0x02)
            {
                if(Scanner_State_LEFT && Scanner_State_RIGHT)
                {
                    /* Precede Base Make code with (E0 AA) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0xF0);
                    Buffer_Key(0x12);

                    /* Precede Base Make code with (E0 B6) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0xF0);
                    Buffer_Key(0x59);
                }
                else if(Scanner_State_LEFT)
                {
                    /* Precede Base Make code with (E0 AA) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0xF0);
                    Buffer_Key(0x12);
                }
                else if(Scanner_State_RIGHT)
                {
                    /* Precede Base Make code with (E0 B6) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0xF0);
                    Buffer_Key(0x59);
                }
            }
        }
    }

    /* ps2 scan code to usb hid no repeat code */
    if((PS2_Scan_Code_To_USB_HID()!=0x00)&&(event == REPEAT_EVENT))
    {

    }
    else
    {
        /* Buffer E0h prefix. */
        Buffer_Key(0xE0);
        if (event == BREAK_EVENT)
        {
            /* Buffer break prefix for break contact. F0h is break prefix. */
            Buffer_Key(0xF0);
        }

        /* Buffer base code. */
        Buffer_Key(scan_code);
    }

    /* To check break */
    if(event == BREAK_EVENT)
    {
        if(additional_key==0x01)
        {
            /* Num Lock ON */
            if(Scanner_State_NUM_LOCK)
            {
                /* LShift || Rshift */
                if(Scanner_State_LEFT || Scanner_State_RIGHT)
                {
                    /* NULL */
                }
                else                    
                {
                    /* follow Base Break code with (E0 AA) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0xF0);
                    Buffer_Key(0x12);
                }
            }
            /* Num Lock OFF */
            else
            {
                if(Scanner_State_LEFT && Scanner_State_RIGHT)
                {
                    /* follow Base Break code with (E0 36) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0x59);
                    
                    /* follow Base Break code with (E0 2A) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0x12);
                }
                else if(Scanner_State_LEFT)
                {
                    /* follow Base Break code with (E0 2A) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0x12);
                }
                else if(Scanner_State_RIGHT)
                {
                    /* follow Base Break code with (E0 36) */
                    Buffer_Key(0xE0);
                    Buffer_Key(0x59);
                }
                else
                {
                    /* NULL */
                }
            }
        }
        else if(additional_key==0x02)
        {
            if(Scanner_State_LEFT && Scanner_State_RIGHT)
            {
                /* follow Base Break code with (E0 36) */
                Buffer_Key(0xE0);
                Buffer_Key(0x59);
                
                /* follow Base Break code with (E0 2A) */
                Buffer_Key(0xE0);
                Buffer_Key(0x12);
            }
            else if(Scanner_State_LEFT)
            {
                /* follow Base Break code with (E0 2A) */
                Buffer_Key(0xE0);
                Buffer_Key(0x12);
            }
            else if(Scanner_State_RIGHT)
            {
                /* follow Base Break code with (E0 36) */
                Buffer_Key(0xE0);
                Buffer_Key(0x59);
            }
        }
    }
}

/**
 * ****************************************************************************
 * FUNCTION: cursor_shift
 *
 * @return
 *
 * @parameter
 * scan_code, Set 2
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 * temp_scanner_state
 *
 * ****************************************************************************
 */
/* BREAK_EVENT key string for cursor_shift and cursor_numlock. */
const CBYTE csr_brk1[] = {0xE0,0xF0,0x00};

void cursor_shift(BYTE scan_code, BYTE event)
{
    const CBYTE csr_sftl_mk[]   = {0xE0,0xF0,0x12,0x00};
    const CBYTE csr_sftr_mk[]   = {0xE0,0xF0,0x59,0x00};
    const CBYTE csr_sftl_brk2[] = {0xE0,0x12,0x00};
    const CBYTE csr_sftr_brk2[] = {0xE0,0x59,0x00};

    if(event == BREAK_EVENT)
    {
        /* Key has just been released. This is a "break event". */

        /* Buffer pre-string. */
        Buffer_String(csr_brk1);

        /* Buffer base code. */
        Buffer_Key(scan_code);

        if (temp_scanner_state.field.tLEFT)
        {
            /* Left shift has been pressed. */

            /* Buffer tail-string. */
            Buffer_String(csr_sftl_brk2);
        }

        if (temp_scanner_state.field.tRIGHT)
        {
            /* Right shift has been pressed. */

            /* Buffer tail-string. */
            Buffer_String(csr_sftr_brk2);
        }
    }
    else if(event == MAKE_EVENT)
    {
        /* Key is pressed for the first time, a "make event". */

        /* Left and/or Right SHIFT is pressed. */
        if (temp_scanner_state.field.tLEFT)
        {
            /* Left SHIFT is pressed. */

            /* Buffer pre-string. */
            Buffer_String(csr_sftl_mk);
        }

        if (temp_scanner_state.field.tRIGHT)
        {
            /* Right SHIFT is pressed. */

            /* Buffer pre-string. */
            Buffer_String(csr_sftr_mk);
        }

        /* Buffer end of pre-string. */
        Buffer_Key(0xE0);

        /* Buffer base code. */
        Buffer_Key(scan_code);
    }
    else
    {
        /* Key is being held pressed for a length of time, a "repeat event". */
        
        /* Buffer E0h prefix */
        Buffer_Key(0xE0);

  	    /* Buffer base code */
        Buffer_Key(scan_code);
    } 
}

/**
 * ****************************************************************************
 * FUNCTION: cursor_numlock
 *
 * @return
 *
 * @parameter
 * scan_code, Set 2
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
void cursor_numlock(BYTE scan_code, BYTE event)
{
    const CBYTE csr_numlock_mk[]   = {0xE0,0x12,0xE0,0x00};
    const CBYTE csr_numlock_brk2[] = {0xE0,0xF0,0x12,0x00};

    if(event == BREAK_EVENT)
    {
        /* Buffer pre-string. */
        Buffer_String(csr_brk1);

        /* Buffer base code. */
        Buffer_Key(scan_code);

        /* Buffer tail-string. */
        Buffer_String(csr_numlock_brk2);
    }
    else if(event == MAKE_EVENT)
    {  
        /* The Num Lock key is pressed. */

        /* Buffer pre-string. */
        Buffer_String(csr_numlock_mk);

        /* Buffer base code. */
        Buffer_Key(scan_code);
    }
    else
    {
        /* Buffer E0h prefix. */
        Buffer_Key(0xE0);

  	    /* Buffer base code. */
        Buffer_Key(scan_code);
    } 
}

/**
 * ****************************************************************************
 * FUNCTION: special_code
 *
 * @return
 *
 * @parameter
 * code_byte
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
void special_code(BYTE code_byte, BYTE event)
{
    const CBYTE key126_normal_mk[] =
        {0xE1,0x14,0x77,0xE1,0xF0,0x14,0xF0,0x77,0x00};
    const CBYTE key126_ctrl_mk[]   = {0xE0,0x7E,0xE0,0xF0,0x7E,0x00};

    /* Print Scr */
    if(code_byte == 2)
    {
        /* PS2 scan code mode */
        if(PS2_Scan_Code_To_USB_HID()==0x00)
        {
            /* + ALT */
            if(Scanner_State_ALT)
            {
                simple_code(0x84, event);
            }
            /* + Ctrl || LShift || Rshift */
            else if(Scanner_State_CONTROL || Scanner_State_LEFT ||
                Scanner_State_RIGHT)
            {
                e0_prefix_code(0x7C, event);
            }
            else
            {
                /* Break */
                if(event == BREAK_EVENT)
                {
                    e0_prefix_code(0x7C, event);
                    e0_prefix_code(0x12, event);
                }
                /* Make or repeat */
                else
                {
                    e0_prefix_code(0x12, event);
                    e0_prefix_code(0x7C, event);
                }
            }
        }
        else
        {
            /* PS2 scan code to usb hid */
            e0_prefix_code(0x7C, event);
        }
    }else if(code_byte == 0x00){    // 0x00,   DO_SPECIAL_CODE,        // [91h] Pause (126)
        if(event == MAKE_EVENT){
	        /* Buffer Ctrl case string. */
            if(Scanner_State_CONTROL){
                Buffer_String(key126_ctrl_mk);
            }else{
                /* Buffer normal code string. */
                Buffer_String(key126_normal_mk);
            }
            SET_MASK(SpecialCodeState,SpecialCodeActive);  

        }else{
            if(Scanner_State_CONTROL){
              simple_code(0x14, BREAK_EVENT);
            }

        }

    }else if(code_byte == 0x01){  // 0x01,   DO_SPECIAL_CODE,        // [92h] Break
        if(event == MAKE_EVENT){
          simple_code(0x14, MAKE_EVENT); // LCtrl MAKE
          Buffer_String(key126_ctrl_mk);
          SET_MASK(SpecialCodeState,SpecialCodeActive); 

        }else{
          simple_code(0x14, BREAK_EVENT); // LCtrl BREAK
        }

    }
}

/**
 * ****************************************************************************
 * FUNCTION: costomer_function
 * The function of hotkey Fn + Fx
 *
 * @return
 *
 * @parameter
 * code_byte
 * event, Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *
 * ****************************************************************************
 */
void costomer_function(BYTE code_byte, BYTE event)
{
	(HotKey_Fn_Fx[code_byte])(event);
	if (scan.kbf_head != scan.kbf_tail)
	{
        SetServiceSendFlag();
	}
}

/**
 * ****************************************************************************
 * FUNCTION: control_effect
 *
 * Generate scan code set 2 and update scanner status.
 *
 * @return, Updated temp_scanner_state
 *
 * @parameter, state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 *  The following bits are defined for state flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 * ****************************************************************************
 */
void control_effect(BYTE state, BYTE event)
{
    if((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
        /* First generate scan code set 2. */
    	if (state & LShift)
		{
        	simple_code(0x12, event);
    	}
 		else if (state & LAlt)
		{
        	simple_code(0x11, event);
    	}
		else if (state & LCtrl) 
		{
        	simple_code(0x14, event);
    	}
    	else if (state & RShift) 
		{
        	simple_code(0x59, event);
    	}
		//else if (state & FN) 
		//{
        //	e0_prefix_code(0x63, event);
    	//}
    }

    /* Then update scanner state. */
    if(event == MAKE_EVENT)
    {
        /* Key is pressed for the first time. This is a "make event". */

		/* Set scanner state for SHIFT, Alt, Ctrl, or Fn. */
    	temp_scanner_state.byte |= state;
        if(state & FN) 
        {
            Hook_Fn_Key_Make();
            #if KB_FnStickKey
            FnStickKey = 1;
            #endif
        }
    }
    else if(event == BREAK_EVENT)
    {   
        if(state & FN) 
        {
            Hook_Fn_Key_Break();
        }
     	temp_scanner_state.byte &= ~state;
    }
    else
    {
        /* Do nothing for repeat event. */
        ;
    }
}

/**
 * ****************************************************************************
 * FUNCTION: control_effect_e0
 *
 * Generate scan code set 2 and update scanner status.
 * Only for Alt-Right and Ctrl-Right.
 *
 * @return, Updated temp_scanner_state
 *
 * @parameter, state flags
 *        event = Contact event (MAKE_EVENT, BREAK_EVENT, or REPEAT_EVENT)
 *        temp_scanner_state
 *
 * The following bits are defined for state_flags and temp_scanner_state:
 *       NUM_LOCK
 *       LEFT
 *       RIGHT
 *       ALT
 *       CONTROL
 *       OVERLAY
 *       FN
 *
 * ****************************************************************************
 */
void control_effect_e0(BYTE state, BYTE event)
{
    /* This is only called for Alt-Right and Ctrl-Right. */
    if((event == MAKE_EVENT)||(event == BREAK_EVENT))
    {
    	/* First setup to generate scan code set 2. */
    	if(state & RAlt) 
		{
			e0_prefix_code(0x11, event);
    	}
		if(state & RCtrl) 
		{
			e0_prefix_code(0x14, event);
    	}
	}

	if(event == MAKE_EVENT)
 	{
        /* Key is pressed for the first time. This is a "make event". */

        /* Set one flag in scanner state flags. */
     	temp_scanner_state.byte |= state;
  	}
	else if(event == BREAK_EVENT)
	{
        /* Key has just been released. This is a "break event". */

        /* Clear one flag in scanner state flags. */
      	temp_scanner_state.byte &= ~state;
  	}
 	else
  	{
        /* Do nothing for repeat event. */
       	;
 	} 		
}


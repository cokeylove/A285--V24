/* 
 * ****************************************************************************
 * TITLE: CORE_HOSTIF.C - Host Interface Handler
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
 * Data_To_Host
 *
 * @return
 *
 * @parameter, data to send to Host.
 *
 * @note
 *
 * ****************************************************************************
 */
void Data_To_Host(BYTE data_byte)
{
    KBHISR &= 0x0F;
	SET_MASK(KBHISR,KEYL);
    
    KBHICR &= 0xFC;
    if ( Ccb42_INTR_KEY )
    {
		SET_MASK(KBHICR,OBFKIE);
    }
    
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    KBHIKDOR = data_byte;
    ShortDelayAfterWriteDOR();
}

/**
 * ****************************************************************************
 * Data_To_Host_nWait
 *
 * @return
 *
 * @parameter, data to send to Host.
 *
 * @note
 *
 * ****************************************************************************
 */
void Data_To_Host_nWait(BYTE data_byte)
{
    Data_To_Host(data_byte);

    /* To enable 26ms time-out timer */
    Enable_ETimer_T(26);

    /* No overflow */
    while(Check_ETimer_T_Overflow()==ExTimerNoOverflow)
	{
        /* 64h port OBE */
	    if(IS_MASK_CLEAR(KBHISR, OBF))
        {
            break;
        }

        /* 64h port IBF */
        if(IS_MASK_SET(KBHISR, IBF))
        {
            break;
        }  
	}

    /* To stop timer */
    Stop_ETimer_T();
}

/**
 * ****************************************************************************
 * KBC_DataPending
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void KBC_DataPending(BYTE nPending)
{
    if( KBPendingRXCount > 3 )  return;

    KBDataPending[(KBPendingRXCount&0x03)] = nPending;
    KBPendingRXCount++;
    SetServiceSendFlag();
}

/**
 * ****************************************************************************
 * GetKB_PendingData
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE GetKB_PendingData(void)
{
    BYTE buffer_data;
    buffer_data=KBDataPending[(KBPendingTXCount&0x03)];
    KBPendingTXCount++;
    if( KBPendingTXCount >= KBPendingRXCount )
    {
        KBPendingTXCount = 0;
        KBPendingRXCount = 0;
    }
    return buffer_data;
}

/**
 * ****************************************************************************
 * KBC_DataToHost
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void KBC_DataToHost(BYTE nKBData)
{
    if(IS_MASK_SET(KBHISR,OBF)||IS_MASK_SET(KBHISR,IBF))
	/* if(IS_MASK_SET(KBHISR,OBF)) */
	{
		KBC_DataPending(nKBData);
	}
	else
	{
    	/* Send data to host. */
		Data_To_Host(nKBData );
	}
}

/**
 * ****************************************************************************
 * Aux_Data_To_Host - Send auxiliary device (mouse) data to the Host.
 *
 * @return
 *
 * @parameter, data to send to Host.
 *
 * @note
 *
 * ****************************************************************************
 */
void Aux_Data_To_Host(BYTE data_byte)
{
    KBHISR &= 0x0F;
	SET_MASK(KBHISR,AOBF);

    /* Put system flag bit in Status Reg. */
    /*
	 * if (Ccb42_SYS_FLAG)
  	 *	SET_MASK(KBHISR,SYSF);
  	 * else
	 *	CLEAR_MASK(KBHISR,SYSF);
     */

	KBHICR &= 0xFC;
    if ( Ccb42_INTR_AUX )
    {
       	SET_MASK(KBHICR,OBFMIE);
    }

    _nop_();
    _nop_();
    _nop_();
    _nop_();

 	KBHIMDOR = data_byte;
    ShortDelayAfterWriteDOR();
}

/**
 * ****************************************************************************
 * SetServiceSendFlag
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void SetServiceSendFlag(void)
{

}

/**
 * ****************************************************************************
 * service_send
 * Send data from the scanner keyboard or from multibyte command responses to
 * the Host.
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
void service_send(void)
{
    BYTE temp_flag, send;
    BYTE data_word;
    send = FALSE;

 	SetServiceSendFlag();
	
    if( IS_MASK_SET(KBHISR,OBF) || IS_MASK_SET(KBHISR,IBF) )
    /* if(IS_MASK_SET(KBHISR,OBF)) */
    {
        return;
    }

    if( KBPendingRXCount > 0 )
    {
        Data_To_Host(GetKB_PendingData());
        return;
    }

	if((Ccb42_DISAB_KEY == 1)|| IS_MASK_SET(KBHISR,IBF))
	{
		return;
	}
  
    data_word = Get_Buffer();
    if(data_word == 0xFF) 
    {   
	    /* Timer_B.fbit.SEND_ENABLE = 0; */
    }
    else
    {   
        send = TRUE;
    }

    /* Send it */
	if(send)
    {
   		temp_flag = Gen_Info_BREAK_SCAN;
        Gen_Info_BREAK_SCAN = 0;
        
        if (send_to_pc(data_word, temp_flag)) 
		{
            /* Break prefix code. */
            Gen_Info_BREAK_SCAN = 1;
        }
    } 
}

/**
 * ****************************************************************************
 * get_response
 * Gets data required for scanner keyboard responses to keyboard commands sent
 * from Host.
 *
 * @return, data to send.
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
/* 
 * This table has commands that the keyboard may send to the Host in response
 * to transmissions, etc.
 */
const CBYTE response_table[] =
{
    0x00,   /* 0 Undefined. */
    0xFA,   /* 1 Manufacurer ID. */
    0xAB,   /* 2 Byte 1 of keyboard ID. */
    0xAA,   /* 3 BAT completion. */
    0xFC,   /* 4 BAT failure. */
    0xEE,   /* 5 Echo. */
    0xFA,   /* 6 Acknowledge. */
    0xFE,   /* 7 Resend. */
    0xFA,   /* 8 Return scan code 2. */
    0x83,   /* 9 Byte 2 of U.S. keyboard ID. */
    0xFA,   /* A reset ack. */
    0xEE,   /* B Echo command. */
    0xAA,   /* C First response. */
    0xFA,   /* D Return scan code 2. */
    0x02,   /* E. */
    0x84    /* F Byte 2 of Japanese keyboard ID. */
};

extern BYTE get_response(void)
{
    BYTE result, code_word;
    code_word = 0;

    switch (Kbd_Response & maskKBD_RESPONSE_CODE)
    {
        case 2:
            if (Get_Kbd_Type() == 0) 
			{
                code_word = 9;   /* U.S. keyboard. */
            }
            else 
			{
                code_word = 0xF; /* Japanese keyboard. */
            }
            break;
    }

    result = response_table[Kbd_Response & maskKBD_RESPONSE_CODE];
    Kbd_Response = (Kbd_Response & ~maskKBD_RESPONSE_CODE) | code_word;

    return(result);
}

/**
 * ****************************************************************************
 * send_to_pc
 *
 * @return,
 * TRUE if translation mode is enabled and scan code was a break prefix.
 *
 * @parameter,
 * data, Data to send to PC.
 * break_prefix_flag, TRUE if last scan code was a break prefix.
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE send_to_pc(BYTE data_word, BYTE break_prefix_flag)
{
    return(common_send_to_pc(data_word, break_prefix_flag));
}

BYTE common_send_to_pc(BYTE data_word, BYTE break_prefix_flag)
{
   	BYTE	send_it = FALSE;

	if (Ccb42_XLATE_PC == 0)			 /* Send data as is. */
   	{
      	send_it = TRUE;
      	break_prefix_flag = FALSE;
   	}
   	else								/* Translation mode is enabled. */
   	{
    	data_word = translate_to_pc(data_word, break_prefix_flag);
      	if (data_word == 0xFF)
      	{
         	break_prefix_flag = TRUE;	/* Don't send break code prefix. */
      	}
	  	else if(data_word == 0x00)
		{
         	break_prefix_flag = TRUE;;	/* Don't send break code prefix. */
      	}
      	else
      	{
         	break_prefix_flag = FALSE;
		 	send_it = TRUE;
      	}
   	}

	if (send_it) {Data_To_Host(data_word);}

   	return(break_prefix_flag);
}

/**
 * ****************************************************************************
 * translate_to_pc
 * Translate scan code from "set 2 scan code" to "set 1 scan code".
 *
 * @return,
 * a value with all bits set if data is break prefix (0xF0);
 * otherwise, returns translated key.
 *
 * @parameter,
 * data, scan code received from aux keyboard or local keyboard.
 * break_prefix_flag, TRUE if last scan code was a break prefix.
 *
 * @note
 *
 * ****************************************************************************
 */
BYTE translate_to_pc(BYTE data_word, BYTE break_prefix_flag)
{
    /*
     * Scan code set 2 to scan code set 1 translation table. First byte is a
     * dummy entry because scan code "0" is not translated.
     */
    const CBYTE scan2_table[] =
    {
        0x00, 0x43, 0x41, 0x3F, 0x3D, 0x3B, 0x3C, 0x58,
        0x64, 0x44, 0x42, 0x40, 0x3E, 0x0F, 0x29, 0x59,
        0x65, 0x38, 0x2A, 0x70, 0x1D, 0x10, 0x02, 0x5A,
        0x66, 0x71, 0x2C, 0x1F, 0x1E, 0x11, 0x03, 0x5B,
        0x67, 0x2E, 0x2D, 0x20, 0x12, 0x05, 0x04, 0x5C,
        0x68, 0x39, 0x2F, 0x21, 0x14, 0x13, 0x06, 0x5D,
        0x69, 0x31, 0x30, 0x23, 0x22, 0x15, 0x07, 0x5E,
        0x6A, 0x72, 0x32, 0x24, 0x16, 0x08, 0x09, 0x5F,
        0x6B, 0x33, 0x25, 0x17, 0x18, 0x0B, 0x0A, 0x60,
        0x6C, 0x34, 0x35, 0x26, 0x27, 0x19, 0x0C, 0x61,
        0x6D, 0x73, 0x28, 0x74, 0x1A, 0x0D, 0x62, 0x6E,
        0x3A, 0x36, 0x1C, 0x1B, 0x75, 0x2B, 0x63, 0x76,
        0x55, 0x56, 0x77, 0x78, 0x79, 0x7A, 0x0E, 0x7B,
        0x7C, 0x4F, 0x7D, 0x4B, 0x47, 0x7E, 0x7F, 0x6F,
        0x52, 0x53, 0x50, 0x4C, 0x4D, 0x48, 0x01, 0x45,
        0x57, 0x4E, 0x51, 0x4A, 0x37, 0x49, 0x46, 0x54
    };

    BYTE check_break_bit = FALSE;

    if (data_word == 0xF0)
    {   /* Signify that break code prefix was encountered. */
		data_word = 0xFF;
    }
    else if (data_word == 0x00)
    {
        data_word = 0x00;      		/* Key detection error/overrun. */
    }
    else if ((data_word & 0x80) == 0)
    {   /* Translate codes 01 thru 7F. */
        /*
         * The variable "data" has scan code (set 2) to translate.
         * Set "data" to the translated (to set 1) scan code.
         */
        data_word = scan2_table[data_word];
        check_break_bit = TRUE;
    }
    else if (data_word == 0x83)  	/* ID code for 101/102 keys. */
    {
        data_word = 0x41;        	/* Translate ID code. */
        check_break_bit = TRUE;
    }
    else if (data_word == 0x84)     /* ID code for 84 keys. */
    {
        data_word = 0x54;         	/* Translate ID code. */
        check_break_bit = TRUE;
    }

    if (check_break_bit && break_prefix_flag)
    {   /*
         * Last code received by this routine was the break prefix.
         * This must be a break code.
         * Set high bit to indicate that this is a break code.
         */
        data_word |= 0x80;
    }

    return(data_word);
}

/**
 * ****************************************************************************
 * Kernel Init super IO function
 *
 * @return
 *
 * @parameter
 *
 * @note
 *
 * ****************************************************************************
 */
const CBYTE Core_initsio_table[]=
{
				/* Configure and Enable Logical Device 06h(KBD) */
	0x07 ,0x06,	/* Select Logical Device 06h(KBD) */
  	0x70 ,0x01,	/* Set IRQ=01h for Logical Device 06h(KBD) */
  	0x30 ,0x01,	/* Enable Logical Device 06h(Mouse) */
				/* Configure and Enable Logical Device 05h(Mouse) */
 	0x07 ,0x05,	/* Select Logical Device 05h(Mouse) */
  	0x70 ,0x0C,	/* Set IRQ=0Ch for Logical Device 05h(Mouse) */
	0x30 ,0x01,	/* Enable Logical Device 05h(Mouse) */
				/* Configure and Enable Logical Device 11h(PM1) */
   	0x07 ,0x11,	/* Select Logical Device 11h(PM1) */
  	0x70 ,0x00,	/* Clear IRQ=0 for  Logical Device 11h(PM1) */
 	0x30 ,0x01,	/* Enable Logical Device 11h(PM1) */
				/* Configure and Enable Logical Device 0Fh(Shared Memory) */
 	0x07 ,0x0F,	/* Logical Device 0Fh(Shared Memory) */
	0x30 ,0x01,	/* Enable Logical Device 0Fh(Shared Memory) */
};

void Core_InitSio(void)
{
    const CBYTE * data_pntr;
    BYTE cnt;
    BYTE setting;

  	SET_MASK(LSIOHA,LKCFG);
  	SET_MASK(IBMAE,CFGAE);
  	SET_MASK(IBCTL,CSAE);

    cnt=0;
    data_pntr=Core_initsio_table;
    while(cnt < (sizeof(Core_initsio_table)/2) )
    {
        IHIOA=0;              /* Set indirect Host I/O Address */
        setting=*data_pntr;
        IHD=setting;
        /* IHD=*data_pntr; */
        while( IS_MASK_SET(IBCTL,CWIB));
        data_pntr ++;

        IHIOA=1;              /* Set indirect Host I/O Address */
        setting=*data_pntr;
        IHD=setting;
        /* IHD=*data_pntr; */
        while( IS_MASK_SET(IBCTL,CWIB));
        data_pntr ++;
        cnt ++;
    }

 	CLEAR_MASK(LSIOHA,LKCFG);
  	CLEAR_MASK(IBMAE,CFGAE);
  	CLEAR_MASK(IBCTL,CSAE);
}


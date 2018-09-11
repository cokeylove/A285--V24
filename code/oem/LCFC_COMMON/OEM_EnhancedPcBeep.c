/*
********************************************************************************
* LCFC Confidential                                                          
*                                                                                             
* Microcode Source Materials                                                         
*  
* ThinkPad Embedded Controller Microcode
* 
* COPYRIGHT LCFC 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_CCG3.C
* 
* Description:
*    Enhanced PC Beep.
* 
* History:
*   08/23/2016    Isaac Lin
*     created as new
********************************************************************************
*/

//#include <CORE_INCLUDE.H>
//#include <OEM_INCLUDE.H>
#include "..\..\include.h"


/*
*******************************************************************************
*       Local Macros
*******************************************************************************
*/


/*
*******************************************************************************
*       Local Data Type
*******************************************************************************
*/

enum __INFO_ID {
    BEEP_ERROR_CODE = 1,
    BEEP_SYSTEM_UNIT_SERIAL_NUMBER,
};
enum __CHECK_ERROR_PROCESS {
    SET_TIMER_3S = 0,
    WAIT_3S_TIME_OUT,
    CHECK_PLT_RST,
    SET_TIMER_3MIN,
    WAIT_MILEPOST,
    CHECK_MILEPOST,
    CONFIRM_ERR_CODE,
    SET_TIMER_1S,
    WAIT_1S_TIME_OUT,
    PLAY_ERR_CODE_BEEP,
    WAIT_BEEP_COMPLETE,
    WAIT_FN_ACTION,
    SET_FN_DELAY_2S,
    WAIT_FN_DELAY_2S_TIME_OUT,
    REPLAY_ERR_CODE_BEEP,
    CHECK_SERIAL_INFO,
    GET_SERIAL_NUMBER,
    PLAY_SERIAL_NUMBER_BEEP,
    STATE_IDLE,
};
enum __ERROR_CODE_PROCESS {
    ERROR_CODE_BLANK = 0,
    ERROR_INFO_ID_H,
    ERROR_INFO_ID_L,
    ERROR_CODE_HH,
    ERROR_CODE_HL,
    ERROR_CODE_LH,
    ERROR_CODE_LL,
    ERROR_CODE_CHECKSUM,
    ERROR_CODE_END,
};
enum __SN_CODE_PROCESS {
    PLAY_BLANK = 0,
    PLAY_INFO_ID,
    PLAY_SN,
    PLAY_CHECKSUM,
    PLAY_END,
};

/*
*******************************************************************************
*       Local Variable
*******************************************************************************
*/

/*
*******************************************************************************
*       Externals
*******************************************************************************
*/

/*
*******************************************************************************
*       Local Function Prototypes
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: MpHousekeeper
*
* Descriptoin: This is used to judge error by according to milepost and play beep.
*
*
* Arguments:
*       int this task's task ID
* Return Values:
*       BOOL TRUE if allow to low power mode.
*******************************************************************************
*/

BOOL MpHousekeeper(BYTE task_id) {
    BOOL rc;
    rc = FALSE;                     // set default as not allow to enter standby
    switch (Mp_Fsm) {
            /*
             ***********************************
             *     Set 3s Timer
             ***********************************
             */
        case SET_TIMER_3S:
            if (IS_MASK_SET(EnhancedPcBeep_Status,MilePostInterrupt)){
                /*
                 * milepost arrived, go to check immediately
                 */
                Mp_Fsm = CHECK_MILEPOST;
            }else{
                EnhancedPcBeep_DelayCnt = 30;
                Mp_Fsm++;
            }
            break;

            /*
             ***********************************
             *     Wait Time out
             ***********************************
             */
        case WAIT_3S_TIME_OUT:
            if (IS_MASK_SET(EnhancedPcBeep_Status,MilePostInterrupt)){
                /*
                 * milepost arrived, go to check immediately
                 */
                Mp_Fsm = CHECK_MILEPOST;
                EnhancedPcBeep_DelayCnt = 0;    // timer expired immediately
            }else if (0 == EnhancedPcBeep_DelayCnt) {
                Mp_Fsm++;
            } else {
                EnhancedPcBeep_DelayCnt--;
            }
            break;

            /*
             ***********************************
             *  Check if PLTRST# is de-asserted
             ***********************************
             */
        case CHECK_PLT_RST:
            if (Read_LPC_RST()){
                Mp_Fsm++;
            } else {
//                CmnDisableInt();       // disable INT
                /*
                 * platform Reset is not asserted,
                 * judge error code is 0001h
                 */
                EnhancedPcBeep_DATA[0] = ERR_CODE_0001>>8;
                EnhancedPcBeep_DATA[1] = ERR_CODE_0001 & 0x00FF;
  							EnhancedPcBeep_ID = 0x01;
                EnhancedPcBeep_repeat_time = 1;
                SET_MASK(EnhancedPcBeep_Status,BeepActive);	//Mp_Control.err_confirm = TRUE;
//                CmnEnableInt();        // Enable INT
                Mp_Fsm = CONFIRM_ERR_CODE;
            		break;
            }

            /*
             ***********************************
             * Set 3min Timer to wait milepost
             ***********************************
             */
        case SET_TIMER_3MIN:
            if (IS_MASK_SET(EnhancedPcBeep_Status,MilePostInterrupt)) {
                /*
                 * milepost arrived, go to check immediately
                 */
                Mp_Fsm = CHECK_MILEPOST;
            } else {
                /*
                 * Set 3min Timer
                 */
                EnhancedPcBeep_DelayCnt = 767;  //2485D00020: Workaround for Beep function test fail
                Mp_Fsm++;
            }
            break;

            /*
             ***********************************
             * waiting milepost
             ***********************************
             */
        case WAIT_MILEPOST:
            if (IS_MASK_SET(EnhancedPcBeep_Status,MilePostInterrupt)){
                Mp_Fsm ++;
                EnhancedPcBeep_DelayCnt = 0;
            } else if (0 == EnhancedPcBeep_DelayCnt) {
                /*
                 * none milepost arrive in 3min.
                 */

//                HwBdpOff(1);    //disable Bdp1
//                CmnDisableInt();    // disable INT
	                SET_MASK(EnhancedPcBeep_Status,boot_done);	//Mp_Control.boot_done = TRUE;
//                CmnEnableInt();     // enable INT
                EnhancedPcBeep_DelayCnt = 0;
                Mp_Fsm = STATE_IDLE;
            } else {
            		EnhancedPcBeep_DelayCnt--;
//                TmrSetTimer(task_id, 10000);  // set 10 seconds timer to wake up
//                TskWaitTask(task_id);
                rc = TRUE;
            }
            break;

            /*
             ***********************************
             * check milepost
             ***********************************
             */
        case CHECK_MILEPOST:
            if (TRUE == Mp_Check_Milepost(EnhancedPcBeep_DATA[1], &EnhancedPcBeep_DATA[0])){
                /*
                 * confirm error code or boot complete
                 */
                Mp_Fsm ++;
            }else{
                Mp_Fsm = SET_TIMER_3MIN;
            }
            break;

            /*
             ***********************************
             *     Confirm error code
             ***********************************
             */
        case CONFIRM_ERR_CODE:
            if (IS_MASK_SET(EnhancedPcBeep_Status,BeepActive)) {
                /*
                 * error is confirmed, go to play beep
                 */
//                HwBdpOff(1);    //disable Bdp1
                Mp_Fsm ++;
            } else if (IS_MASK_SET(EnhancedPcBeep_Status,boot_done)) {
                /*
                 * boot complete, close milepost host interface
                 */
//                HwBdpOff(1);    //disable Bdp1
                Mp_Fsm = STATE_IDLE;
            }
            break;

            /*
             ***********************************
             *     Set 1s Timer
             ***********************************
             */
        case SET_TIMER_1S:
        		EnhancedPcBeep_DelayCnt = 10;
            //TmrSetTimer(task_id, 1000);
            Mp_Fsm++;
            break;

            /*
             ***********************************
             *     Wait Time out
             ***********************************
             */
        case WAIT_1S_TIME_OUT:
            if (0 == EnhancedPcBeep_DelayCnt) {
                Mp_Fsm++;
            } else {
            		EnhancedPcBeep_DelayCnt--;
                //TskWaitTask(task_id);
            }
            break;

            /*
             ***********************************
             *     Play error code beep
             ***********************************
             */
        case PLAY_ERR_CODE_BEEP:
            if (IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepBusy)){
                if(IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepActive)){
                    Mp_Fsm = WAIT_FN_ACTION;
                }else{
                    Mp_Fsm ++;
                }
            }else{
                Mp_Fsm = WAIT_FN_ACTION;
            }
            break;

            /*
             ***********************************
             *     Wait beep complete
             ***********************************
             */
        case WAIT_BEEP_COMPLETE:
            if (IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepBusy) && (EnhancedPcBeep_repeat_time == 0)){
                Mp_Fsm ++;
            }else{
                //TskWaitTask(task_id);
                rc = TRUE;
            }
            break;

            /*
             ***********************************
             *     Wait hotkey action
             ***********************************
             */
        case WAIT_FN_ACTION:
            //TskWaitTask(task_id);
            rc = TRUE;
            break;

            /*
             ***********************************
             *     Set Timer to wait
             *     hotkey be pressed for 2S
             ***********************************
             */
        case SET_FN_DELAY_2S:
        		EnhancedPcBeep_DelayCnt = 20;
            //TmrSetTimer(task_id, 2000);
            Mp_Fsm++;
            break;

            /*
             ***********************************
             *     Wait hotkey delay 2S Time out
             ***********************************
             */
        case WAIT_FN_DELAY_2S_TIME_OUT:
            if (0 == EnhancedPcBeep_DelayCnt) {
                if (1 == Read_FnKey_Status) {
                    Mp_Fsm = CHECK_SERIAL_INFO;
                }
            } else {
            		EnhancedPcBeep_DelayCnt--;
                //TskWaitTask(task_id);
            }
            break;

            /*
             ***********************************
             *     Replay error code beep
             ***********************************
             */
        case REPLAY_ERR_CODE_BEEP:
            if (IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepBusy)) {
                if(IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepActive)){
                    Mp_Fsm = WAIT_FN_ACTION;
                }else{
										EnhancedPcBeep_repeat_time = 3;
                    Mp_Fsm = WAIT_BEEP_COMPLETE;
                }
            } else {
                Mp_Fsm = WAIT_FN_ACTION;
            }
            break;

            /*
             ***********************************
             *     Check MTM/SN
             ***********************************
             */
        case CHECK_SERIAL_INFO:
            if (IS_MASK_CLEAR(EnhancedPcBeep_Status,BeepBusy)) {
                if (FALSE == Mp_Check_SerialInfo()) {
                    /*
                     * fail, go to play dummy Serial#
                     */
                    Mp_Set_Sn_Dummy(&EnhancedPcBeep_DATA[0]);
	    							EnhancedPcBeep_Status &= 0xE0;	//Clear for new process flow step restart.
                    Mp_Fsm = PLAY_SERIAL_NUMBER_BEEP;
                } else {
                    Mp_Fsm++;
                }
            } else {
                Mp_Fsm = WAIT_FN_ACTION;
            }
            break;

            /*
             ***********************************
             *     Get Serial Number
             ***********************************
             */
        case GET_SERIAL_NUMBER:
            if (FALSE == Mp_Get_SerialNumber(&EnhancedPcBeep_DATA[0])) {
                /*
                 * fail, go to play dummy Serial#
                 */
                Mp_Set_Sn_Dummy(&EnhancedPcBeep_DATA[0]);
	    					EnhancedPcBeep_Status &= 0xE0;	//Clear for new process flow step restart.
                Mp_Fsm = PLAY_SERIAL_NUMBER_BEEP;
            } else {
	    					EnhancedPcBeep_Status &= 0xE0;	//Clear for new process flow step restart.
                Mp_Fsm++;
            }
            break;

            /*
             ***********************************
             *     Play Serial# Beep
             ***********************************
             */
        case PLAY_SERIAL_NUMBER_BEEP:
            if(IS_MASK_SET(EnhancedPcBeep_Status,BeepBusy)){
            //if (FALSE == Mp_Play_Sn_Beep()){
                //TskWaitTask(task_id);
                rc = TRUE;
            }else{
				  			EnhancedPcBeep_ID = 0x02;
				        EnhancedPcBeep_repeat_time = 1;
                Mp_Fsm = WAIT_BEEP_COMPLETE;
            }
            break;
            /*
             ***********************************
             *     State idle
             ***********************************
             */
        case STATE_IDLE:
            //TskWaitTask(task_id);
            rc = TRUE;
            break;
    }

    return rc;
}

/*
*******************************************************************************
* Function name: Mp_Set_Sn_Dummy
*
* Descriptoin: This function is used to set Serial# to dummy
*              (all characters as "X")
*
* Arguments:
*       None
* Return Values:
*       None
*******************************************************************************
*/

void Mp_Set_Sn_Dummy(BYTE *sn_char) {
    BYTE i;

    for (i = 0; i < SN_DATA_LENGTH; i++, sn_char++) {
        *sn_char = SN_CHAR_DUMMY;
    }
}

/*
*******************************************************************************
* Function name: Mp_Get_SerialNumber
*
* Descriptoin: This function is used to get Serial# from NVS.
*
* Arguments:
*       UCHAR: *sn_char
* Return Values:
*       BOOL :true if success to get Serial#
*******************************************************************************
*/

BOOL Mp_Get_SerialNumber(BYTE *sn_char) {
    BOOL rc;
    BYTE offset;                     // offset of NVS
    BYTE block_size;                 // block size
    BYTE char_temp;                // character temporary
    BYTE i;
    
    rc = TRUE;

    /*
     ***********************************
     * Find MTM/SN
     ***********************************
     */
    offset = NVS_HEADER_SIZE;
    block_size = readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,SI_BlOCk_COUNT);

    do {
#if OPTION_TP1_NEC  // NEC system case (Device type C2h)

        if (DEVICE_TYPE_NEC == readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
#else               // LENOVO system case (Device type C0h)

        if (DEVICE_TYPE_LENOVO == readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
#endif
            /*
             * Device type ID found
             */
            break;
        }
        offset += block_size;

    } while (offset < NVS_BLOCK_SIZE);

    if (NVS_BLOCK_SIZE <= offset) {
        return FALSE;
    }

#if OPTION_TP1_NEC  // NEC system case (Device type C2h)
    /*
     * Currently, we will not check NEC MTM/SN and Product Name format.
     */
    return FALSE;

#else               // LENOVO system case (Device type C0h)

    /*
     ***********************************
     * Check MTMSN prefix. Starting with '1S'?
     ***********************************
     */
    offset++;
    if (MTM_SN_PREFIX1 != readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
        return FALSE;
    }

    offset++;
    if (MTM_SN_PREFIX2 != readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
        return FALSE;
    }

    /*
     ***********************************
     * Check MTM is AlphaNumeric?
     ***********************************
     */
    offset++;
    for (i = 0; i < MTM_DATA_LENGTH; i++, offset++) {
        char_temp = readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset);
        if (!((char_temp >= 0x30 && char_temp <= 0x39) ||
              (char_temp >= 0x41 && char_temp <= 0x5A))) {
            // Not AlphaNumeric.
            return FALSE;
        }
    }

    /*
     ***********************************
     * Get SN and check it is AlphaNumeric?
     ***********************************
     */
    for (i = 0; i < SN_DATA_LENGTH; i++, offset++, sn_char++) {
        char_temp = readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset);
        if (!((char_temp >= 0x30 && char_temp <= 0x39) ||
              (char_temp >= 0x41 && char_temp <= 0x5A))) {
            // Not AlphaNumeric.
            return FALSE;
        } else {
            // append serial# character
            *sn_char = char_temp;
        }
    }

    // MTMSN is valid.
    return TRUE;

#endif
}

/*
*******************************************************************************
* Function name: Mp_Check_SerialInfo
*
* Descriptoin: This function is used to check Serial info is valid.
*
* Arguments:
*       None
* Return Values:
*       BOOL :true if Serial info is valid
*******************************************************************************
*/

BOOL Mp_Check_SerialInfo(void) {
    BOOL rc;
    WORD offset;                     // offset of NVS
    BYTE checksum;                   // checksum
    BYTE block_size;                 // block size
    BYTE nvs_ver_id;               // nvs version id
    CBYTE nvs_header_id[]=NVS_HEADER_ID;//*nvs_header_id;            // nvs header id
    
    rc = TRUE;

    /*
     ***********************************
     * Check ID in header.
     ***********************************
     */
//    nvs_header_id = NVS_HEADER_ID;
//    for (offset = 0x0; offset < sizeof(nvs_header_id); offset++, nvs_header_id++) {
//        if (*nvs_header_id != readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
//            return FALSE;
//        }
//    }
    for (offset = 0x0; offset < sizeof(nvs_header_id); offset++) {
        if (nvs_header_id[offset] != readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset)) {
            return FALSE;
        }
    }

    /*
     ***********************************
     * check NVS VersionID.
     ***********************************
     */
    nvs_ver_id = readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,SI_VERSION_ID);
    if (NVS_VER_ID != nvs_ver_id) {
        return FALSE;
    }

    /*
     ***********************************
     * Check header checksum.
     ***********************************
     */
    for (checksum = 0, offset = PNP_IDENTIFIER; offset < SI_AREA_CHECKSUM; offset++) {
        checksum += readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset);
    }
    if (0 != checksum) {
        return FALSE;
    }

    /*
     ***********************************
     * Calculate and check the contents length.
     ***********************************
     */
    block_size = readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,SI_BlOCk_COUNT) * readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,SI_MAX_ENTRIES);
    if (block_size + NVS_HEADER_SIZE > NVS_BLOCK_SIZE) {
        return FALSE;
    }

    /*
     ***********************************
     * Check area checksum.
     ***********************************
     */
    for (checksum = 0, offset = 0; offset < NVS_BLOCK_SIZE; offset++) {
        checksum += readRFIDEEprom(EEPROM_SERIAL_INFO_AREA,offset);
    }
    if (0 != checksum) {
        return FALSE;
    }

    // Serial info is valid.
    return TRUE;
}

/*
*******************************************************************************
* Function name: Mp_Check_Milepost
*
* Descriptoin: This is used to check milepost and confirm error code.
*
* Arguments:
*       UCHAR milepost
*       UCHAR *err_code
* Return Values:
*       BOOL :true if error code is confirmed or confirmed boot done
*******************************************************************************
*/
BOOL Mp_Check_Milepost(BYTE milepost, BYTE *err_code) {
    BOOL rc;
    rc = TRUE;

		CLEAR_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
    if (0 != (MSB_MASK & milepost)) {
        /*
         * if MSB is 1, confirm error code
         */
        *err_code = ERR_CODE_PREFIX_02>>8;
        *(err_code+1) = milepost;
  			EnhancedPcBeep_ID = 0x01;
        EnhancedPcBeep_repeat_time = 1;
        //CmnDisableInt();    // disable INT
        SET_MASK(EnhancedPcBeep_Status,BeepActive);	//Mp_Control.err_confirm = TRUE;
        //CmnEnableInt();     // enable INT
        rc = TRUE;
    } else if ((MP_START_OF_NONE_ERR <= milepost) && (MP_END_OF_NONE_ERR >= milepost)) {
        /*
         * after 0x6A arrived, confirm Boot complete
         */
        *err_code = ERR_CODE_RESERVED>>8;
        *(err_code+1) = ERR_CODE_RESERVED&&0x00FF;
        //CmnDisableInt();    // disable INT
        SET_MASK(EnhancedPcBeep_Status,boot_done);	//Mp_Control.boot_done = TRUE;
        CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
        //CmnEnableInt();     // enable INT
        rc = TRUE;
    } else {
        *err_code = ERR_CODE_RESERVED>>8;
        *(err_code+1) = ERR_CODE_RESERVED&&0x00FF;
        CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
        rc = FALSE;
    }

    return rc;
}

/*
********************************************************************************
* Function name: MpFnStateChanged
*
* Description:
*
*          This is invoked when the Fn key is pressed or released
*
* Arguments:
*    int state of the Fn key
* Return Values:
*    none
********************************************************************************
*/

void MpFnStateChanged(BYTE state) {
    if (IS_MASK_SET(EnhancedPcBeep_Status,BeepActive) && IS_MASK_CLEAR(EnhancedPcBeep_Status,boot_done)) {
        /*
         * error code has been confirmed and boot has not completed yet.
         */
        if (1 == state) {
            /*
             *    Fn key is pressed
             */
            if (WAIT_FN_ACTION == Mp_Fsm) {
                Mp_Fsm = SET_FN_DELAY_2S;
                //TskActivateTask(TASK_MILE_POST);
            }
        } else {
            /*
             *    Fn key is released
             */
            if ((SET_FN_DELAY_2S <= Mp_Fsm) && (WAIT_FN_DELAY_2S_TIME_OUT >= Mp_Fsm)) {
                Mp_Fsm = REPLAY_ERR_CODE_BEEP;
                //TskActivateTask(TASK_MILE_POST);
            }
        }
    }
}

/*
*******************************************************************************
* Function name: PlayErrorCodeBeep           
*
* Descriptoin: This function is used to request playing Err code Beep.
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void PlayErrorCodeBeep(void)
{
	BYTE CodeTemp;

  audBEEPedgeon;
	EnhancedPcBeep_TimerCnt = 1;
	switch(EnhancedPcBeep_Status & 0x0F)
	{
    case ERROR_CODE_BLANK:
	  	//Beep for error code BLANK
			SET_MASK(EnhancedPcBeep_Status,BeepBusy);
	  	DataToFrequence(FREQ_BLANK);
	  	EnhancedPcBeep_CheckSum = 0;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_INFO_ID_H:
	  	//Beep for error code INFO ID high '0'
	  	DataToFrequence(INFO_ID_01 >> 4);
	  	EnhancedPcBeep_CheckSum += 0;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_INFO_ID_L:
	  	//Beep for error code INFO ID low '1'
	  	DataToFrequence(INFO_ID_01 & 0x0F);
	  	EnhancedPcBeep_CheckSum += 1;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_HH:
	  	//Beep for error code data high byte high
	  	CodeTemp = EnhancedPcBeep_DATA[0] >> 4;
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_HL:
	  	//Beep for error code data high byte low
	  	CodeTemp = EnhancedPcBeep_DATA[0] & 0x0F;
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_LH:
	  	//Beep for error code data low byte high
	  	CodeTemp = EnhancedPcBeep_DATA[1] >> 4;
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_LL:
	  	//Beep for error code data low byte low
	  	CodeTemp = EnhancedPcBeep_DATA[1] & 0x0F;
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_CHECKSUM:
	  	//Beep for error code check sum
	  	DataToFrequence(EnhancedPcBeep_CheckSum&0x0F);
	  	EnhancedPcBeep_Status++;
      break;
    case ERROR_CODE_END:
	  	//Beep for error code end
    	audBEEPoff;
	  	EnhancedPcBeep_TimerCnt = 0;
//	  	CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
//	  	CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
	    EnhancedPcBeep_Status &= 0xF0;	//Clear for new process flow step restart.
	    if(EnhancedPcBeep_repeat_time>0)
	    {
	    	EnhancedPcBeep_repeat_time--;
	    	if(EnhancedPcBeep_repeat_time == 0)
	    	{
					CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
				}
	    }
	    else// if(EnhancedPcBeep_repeat_time == 0)
	    {
				CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
	    }
      break;
    default:
    	audBEEPoff;
	  	EnhancedPcBeep_TimerCnt = 0;
	    EnhancedPcBeep_Status &= 0xE0;	//Clear for new process flow step restart.
      break;
  }

}

/*
*******************************************************************************
* Function name: PlaySnBeep           
*
* Descriptoin: This function is used to request playing Sn Beep.
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void PlaySnBeep(void)
{
	BYTE CodeTemp;
  //BYTE cnt;

  audBEEPedgeon;
	EnhancedPcBeep_TimerCnt = 1;
	switch(EnhancedPcBeep_Status & 0x0F)
	{
    case PLAY_BLANK:
	  	//Beep for SN code BLANK
			SET_MASK(EnhancedPcBeep_Status,BeepBusy);
	  	DataToFrequence(FREQ_BLANK);
	  	EnhancedPcBeep_CheckSum = 0;
      EnhancedPcBeep_Data_Cnt = NUM_OF_INFO_ID_DIGIT;
	  	EnhancedPcBeep_Status++;
      break;
    case PLAY_INFO_ID:
	  	//Beep for SN code INFO ID
	  	CodeTemp = (INFO_ID_02 >> ((EnhancedPcBeep_Data_Cnt - 1) * 4)) & 0x0F;
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_Data_Cnt--;
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	if(EnhancedPcBeep_Data_Cnt == 0)
	  	{
        EnhancedPcBeep_Data_Cnt = NUM_OF_SN_CODE_DIGIT * 2;
	  		EnhancedPcBeep_Status++;
	  	}
      break;
    case PLAY_SN:
	  	//Beep for SN code
	  	if((EnhancedPcBeep_Data_Cnt & 0x01) == 0)
	  	{
	  		CodeTemp = (EnhancedPcBeep_DATA[NUM_OF_SN_CODE_DIGIT - (EnhancedPcBeep_Data_Cnt>>1)] >> 4) & 0x0F;
	  	}
	  	else
	  	{
	  		CodeTemp = EnhancedPcBeep_DATA[NUM_OF_SN_CODE_DIGIT - ((EnhancedPcBeep_Data_Cnt+1)>>1)] & 0x0F;
	  	}
	  	DataToFrequence(CodeTemp);
	  	EnhancedPcBeep_CheckSum += CodeTemp;
	  	EnhancedPcBeep_Data_Cnt--;
	  	if(EnhancedPcBeep_Data_Cnt == 0)
	  	{
	  		EnhancedPcBeep_Status++;
	  	}
      break;
    case PLAY_CHECKSUM:
	  	//Beep for check sum
	  	DataToFrequence(EnhancedPcBeep_CheckSum&0x0F);
	  	EnhancedPcBeep_Status++;
      break;
    case PLAY_END:
	  	//Beep for SN code end
    	audBEEPoff;
	  	EnhancedPcBeep_TimerCnt = 0;
//	  	CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
//	  	CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
	    EnhancedPcBeep_Status &= 0xF0;	//Clear for new process flow step restart.
	    if(EnhancedPcBeep_repeat_time>0)
	    {
	    	EnhancedPcBeep_repeat_time--;
	    	if(EnhancedPcBeep_repeat_time == 0)
	    	{
					CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
				}
	    }
	    else// if(EnhancedPcBeep_repeat_time == 0)
	    {
				CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
	    }
      break;
    default:
    	audBEEPoff;
	  	EnhancedPcBeep_TimerCnt = 0;
	    EnhancedPcBeep_Status &= 0xE0;	//Clear for new process flow step restart.
      break;
  }

}

/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: DataToFrequence
*
* Descriptoin: This is to transfer data to beep frequence.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void DataToFrequence(BYTE BeepNumber)
{
	switch(BeepNumber)
	{
    case 0x00:
	  	//Beep for error code 0
			BEEP_800HZ;
      break;
    case 0x01:
	  	//Beep for error code 1
			BEEP_850HZ;
      break;
    case 0x02:
	  	//Beep for error code 2
			BEEP_900HZ;
      break;
    case 0x03:
	  	//Beep for error code 3
			BEEP_960HZ;
      break;
    case 0x04:
	  	//Beep for error code 4
			BEEP_1010HZ;
      break;
    case 0x05:
	  	//Beep for error code 5
			BEEP_1070HZ;
      break;
    case 0x06:
	  	//Beep for error code 6
			BEEP_1140HZ;
      break;
    case 0x07:
	  	//Beep for error code 7
			BEEP_1200HZ;
      break;
    case 0x08:
	  	//Beep for error code 8
			BEEP_1270HZ;
      break;
    case 0x09:
	  	//Beep for error code 9
			BEEP_1350HZ;
      break;
    case 0x0A:
	  	//Beep for error code A
			BEEP_1430HZ;
      break;
    case 0x0B:
	  	//Beep for error code B
			BEEP_1520HZ;
      break;
    case 0x0C:
	  	//Beep for error code C
			BEEP_1600HZ;
      break;
    case 0x0D:
	  	//Beep for error code D
			BEEP_1700HZ;
      break;
    case 0x0E:
	  	//Beep for error code E
			BEEP_1800HZ;
      break;
    case 0x0F:
	  	//Beep for error code F
			BEEP_1910HZ;
      break;
    case 0x10:
	  	//Beep for error code BLANK
			BEEP_2020HZ;
      break;
    default:
      break;
  }
	
}
/*
*******************************************************************************
* Function name: InitialCCG2
*
* Descriptoin: This is to detect CCG2 in ready and initial CCG2 to auto mode.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

//void InitialEnhancedPcBeep(void)
//{
//
//}

/*
*******************************************************************************
* Function name: EnhancedPcBeepProcess           
*
* Descriptoin: This is invoked when beep request
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void EnhancedPcBeepProcess(void) {

	if(IS_MASK_SET(EnhancedPcBeep_Status,BeepActive) && (EnhancedPcBeep_TimerCnt == 0))
	{
		switch(EnhancedPcBeep_ID)
		{
	    case BEEP_ERROR_CODE:
		  	//Beep for error code
		  	if(EnhancedPcBeep_repeat_time > 0)
		  	{
		  		PlayErrorCodeBeep();
		  	}
	      break;
	    case BEEP_SYSTEM_UNIT_SERIAL_NUMBER:
		  	//Beep for system unit serial number
		  	if(EnhancedPcBeep_repeat_time > 0)
		  	{
		  		PlaySnBeep();
		  	}		  	
	      break;
	    default:
		    EnhancedPcBeep_ID = 0;	//Clear for new process flow step restart.
	      break;
	  }
  }
  else if(EnhancedPcBeep_TimerCnt > 0)
  {
  	EnhancedPcBeep_TimerCnt--;
  }
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_init_hook           
*
* Descriptoin: Oem_Initialization for TYPE-C                 
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
//void OEM_TYPE_C_init_hook(void)
//{
//	TypeCIniDelay = TypeC_Ini_Delay_time;	//Delay 80ms for type C stable
//  TypeCStatus = 0;
//  TypeCCommFailCnt = 0;
//  TypeCAdpID = 0;
//  CurrentTypeCV = 0;
//  CurrentTypeCI = 0;
//  CurrentTypeCWatt = 0;
//}

/*
*******************************************************************************
* Function name: OEM_ENHANCED_PC_BEEP_HOOK           
*
* Descriptoin: Enhanced Pc Beep hook for Hook_Timer100msEventC
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_ENHANCED_PC_BEEP_HOOK(void)
{
	if(SystemIsS0 || (SysPowState==SYSTEM_S5_S0))
	{
		MpHousekeeper(0x01);
		EnhancedPcBeepProcess();
		MpFnStateChanged(Read_FnKey_Status);
	}
}

/*
*******************************************************************************
* Function name: OEM_ENHANCED_PC_BEEP_Init_HOOK           
*
* Descriptoin: Enhanced Pc Beep hook for initial            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_ENHANCED_PC_BEEP_Init_HOOK(void)
{
		CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
		CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
		CLEAR_MASK(EnhancedPcBeep_Status,boot_done);
		CLEAR_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
}

/*
*******************************************************************************
* Function name: OEM_ENHANCED_PC_BEEP_S3_HOOK           
*
* Descriptoin: Enhanced Pc Beep hook for S3            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_ENHANCED_PC_BEEP_S3_HOOK(void)
{
   //CLEAR_MASK(PDStatus,TypeCFwUpdating);
}


/*
*******************************************************************************
* Function name: OEM_ENHANCED_PC_BEEP_S5_HOOK           
*
* Descriptoin: Enhanced Pc Beep hook for S5            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_ENHANCED_PC_BEEP_S5_HOOK(void)
{
	//Clear all status for nect power on checking
	if((EnhancedPcBeep_Status & 0xF0) || (Mp_Fsm != 0))
	{
		CLEAR_MASK(EnhancedPcBeep_Status,BeepActive);
		CLEAR_MASK(EnhancedPcBeep_Status,BeepBusy);
		CLEAR_MASK(EnhancedPcBeep_Status,boot_done);
		CLEAR_MASK(EnhancedPcBeep_Status,MilePostInterrupt);
		EnhancedPcBeep_Status &= 0xF0;
		EnhancedPcBeep_CheckSum = 0;
		EnhancedPcBeep_TimerCnt = 0;
		Mp_Fsm = 0;
		EnhancedPcBeep_repeat_time = 0;
	}
}


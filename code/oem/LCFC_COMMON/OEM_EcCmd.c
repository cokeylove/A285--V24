/*
********************************************************************************
* Lenovo Confidential                                                          
*
* Microcode Source Materials
*  
* ThinkPad Embedded Controller Microcode
* 
* (C) COPYRIGHT LENOVO 2007, 2016 ALL RIGHTS RESERVED
*
* Filename: OEM_eccmd.c   
* 
* Description:
*
*      This is used to handle commands through SMbus interface in the interace
*    space to EC.                                         
* 
* History:
*   10/27/2016    Isaac Lin
*     created as new
********************************************************************************
*/

//*****************************************************************************
// Include all header file
//*****************************************************************************
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

enum _EC_CMD {
    EC_CMD_RESERVED0,  
    EC_CMD_UCSI_READ_VERSION,         // access UCSI data structure (VERSION)
    EC_CMD_UCSI_READ_RESERVED,        // access UCSI data structure (RESERVED)
    EC_CMD_UCSI_READ_CCI,             // access UCSI data structure (CCI)
    EC_CMD_UCSI_WRITE_CONTROL,        // access UCSI data structure (CONTROL)
    EC_CMD_UCSI_READ_MESSAGE_IN,      // access UCSI data structure (MESSAGE IN)
    EC_CMD_UCSI_WRITE_MESSAGE_OUT,    // access UCSI data structure (MESSAGE OUT)
    EC_CMD_RESERVED7,
    EC_CMD_RESERVED8,
    EC_CMD_RESERVED9,
    EC_CMD_RESERVED10,
    EC_CMD_SET_LID_OPEN_CLOSE_COUNT,
    EC_CMD_QUERY_FAN_TABLE,
    EC_CMD_RESERVED13,
    EC_CMD_LOAD_WAKE_EVENT, 
    EC_CMD_RESERVED15,
    EC_CMD_RESERVED16,
    EC_CMD_RESERVED17,
    EC_CMD_RESERVED18,
    EC_CMD_RESERVED19,
    EC_CMD_FUPDATE_CONTROL,
    EC_CMD_RESERVED21,
    EC_CMD_RESERVED22,
    EC_CMD_RESERVED23,
    EC_CMD_RESERVED24,
    EC_CMD_SET_DATE_TIME,
    EC_CMD_SET_RESUME_TIMER,
    EC_CMD_RESERVED27,
    EC_CMD_SET_CHALLENGE_DATA,
    EC_CMD_SET_CHECK_DATA,
    EC_CMD_RESERVED30,
    EC_CMD_CLEAR_BAT_DATE,
    EC_CMD_RESERVED32,
    EC_CMD_END                
};

enum _EC_CMD_MEM {
    NOP,                         // 0x80
    MEM_READ_BYTE,               // 0x81
    MEM_READ_WORD,               // 0x82 
    MEM_WRITE_BYTE,              // 0x83 
    MEM_WRITE_WORD,              // 0x84
    MEM_READ_8,                  // 0x85
    MEM_BSET,                    // 0x86
    MEM_BCLR,                    // 0x87
    IO_READ,                     // 0x88
    IO_WRITE,                    // 0x89
    ROM_READ,                    // 0x8a (absolute use READ_ROM instead)
    GET_IIC_LOG,                 // 0x8b
    DISABLE_LOG,                 // 0x8c
    ENABLE_LOG,                  // 0x8d
    GET_LOG,                     // 0x8e
    GET_LOG_UNIT_TIME,           // 0x8f
    QUERY_LOG,                   // 0x90
    RESET_LOG,                   // 0x91
    WRITE_CP,                    // 0x92
    CP_CONTROL,                  // 0x93
    WRITE_EC_CP,                 // 0x94
    MEM_READ_DOUBLE_WORD,        // 0x95
    MEM_WRITE_DOUBLE_WORD,       // 0x96
    SET_RELATIVE_STATE_OF_CHARGE,// 0x97
    READ_INTERFACE_SPACE,        // 0x98
    WRITE_INTERFACE_SPACE,       // 0x99
    READ_INTERFACE_SPACE_8,      // 0x9A
    DISABLE_SMSBUS_SLAVE,        // 0x9B 
    ENABLE_SMSBUS_SLAVE,         // 0x9C 
    READ_FROM_PMH,               // 0x9D 
    WRITE_TO_PMH,                // 0x9E 
    READ_IO_CONTROL,             // 0x9F 
    GS_CONTROL,                  // 0xA0
    GS_GET_DATA,                 // 0xA1
    GET_LOG_AT_POSITION,         // 0xA2
    ENABLE_REMOTE_LOG,           // 0xA3
    DISABLE_REMOTE_LOG,          // 0xA4
    READ_ROM,                    // 0xA5
    GS_GET_VERSION,              // 0xA6
    GS_GET_DATA_3_AXES,          // 0xA7
    GS_GET_SENSOR_TYPE,          // 0xA8
    SET_REMAINING_CAPACITY,      // 0xA9
    SET_BATTERY_CURRENT,         // 0xAA
    READ_TEMP,                   // 0xAB
    READ_USB_PD,                 // 0xAC
    GET_STATUS_USB_PD,           // 0xAD
    GET_DATA_USB_PD,             // 0xAE
    SET_USB_PD_DATA,             // 0xAF
    WRITE_USB_PD,                // 0xB0
};

enum _EC_CMD_CP_CONTROL {
    DISABLE,          
    ENABLE,           
    SECONDARY_IF,        
    MAIN_IF,          
    AUTO_SELECT,      
    REAL_TIME,        
    SEND_LAST_PERIODIC,
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
*       Global Function Declarations
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: EcCmd        
*
* Descriptoin: This is used to handle commands through SMBus interface in the                
*              interface space.
*
* Arguments:
*       UCHAR protocol: protocol of SMBus
*       UCHAR command                     
*       UCHAR length: length of data      
*       UCHAR *data                       
*       int   *return_length              
* Return Values:
*       UCHAR result: a status value of SMBus
*******************************************************************************
*/

BYTE EcCmd(BYTE protocol, BYTE command, BYTE length, BYTE *Tempdata, BYTE *return_length) {
    BYTE       rc;
    BYTE         i;
//    BYTE       *p0;
//    BYTE *p1;
    WORD ucsi_version;
    static const BYTE clear_fud_key[] = {
        0x54, 0x36, 0x38, 0x2d,
        0x73, 0xFC, 0x02, 0xD8  
    };

    *return_length = 0;
    switch (command) {
        case EC_CMD_LOAD_WAKE_EVENT:
             if (SMB_PROTO_WRITE_BLOCK == protocol) {
//                 AcpiLoadWakeEventTable(data, length); // NULL function
                 rc = SMB_STS_STATUS_OK;
             } else {
                 rc = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_FUPDATE_CONTROL:
             rc = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_SET_RESUME_TIMER:
             rc = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_QUERY_FAN_TABLE:
             if (SMB_PROTO_READ_BLOCK == protocol) {
//                 for (i = 0, p0 = data, p1 = FanSpeedLevelMapTable.level_map_table; 
//                      FanSpeedLevelMapTable.number_of_level > i; i++, p0++, p1++) {
//                     *p0 = *p1;
//                 }
//                 *return_length = FanSpeedLevelMapTable.number_of_level;
                 rc = SMB_STS_STATUS_OK;
             } else {
                 rc = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_SET_CHALLENGE_DATA:
        case EC_CMD_SET_CHECK_DATA:
                 rc = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_SET_LID_OPEN_CLOSE_COUNT:
             if ((SMB_PROTO_WRITE_BLOCK == protocol) && (8 == length)) {
                 /*
                 *   Protocol is Write Block
                 *   Data length is 8
                 */
                 if (('i' == Tempdata[0]) && ('d' == Tempdata[1]) && ('i' == Tempdata[2]) && ('o' == Tempdata[3])){
                 }
             }
             rc = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_UCSI_READ_VERSION:
             /*
             * access UCSI data structure (VERSION)
             */
             if (SMB_PROTO_READ_BLOCK == protocol) {
                 ucsi_version  = UcsiGetVersion();
                 Tempdata[0]       = ucsi_version & 0xff;
                 Tempdata[1]       = (ucsi_version >> 8) & 0xff;
                 *return_length = 2;
                 rc            = SMB_STS_STATUS_OK;
             } else {
                 rc = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_UCSI_READ_RESERVED:
             /*
             *    access UCSI data structure (RESERVED)
             */
             Tempdata[0]       = 0;
             Tempdata[1]       = 0;
             *return_length = 2;
             rc            = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_UCSI_READ_CCI:
             /*
             *   access UCSI data structure (CCI)
             */
             if (SMB_PROTO_READ_BLOCK == protocol) {
                 UcsiGetCci(Tempdata, UCSI_CCI_DATA_LENGTH);
                 *return_length = 4;
                 rc            = SMB_STS_STATUS_OK;
             } else {
                 rc = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_UCSI_WRITE_CONTROL:
             /*
             *   access UCSI data structure (CONTROL)
             */
             if (SMB_PROTO_WRITE_BLOCK == protocol) {

                 UcsiSetControl(Tempdata, length);
                 *return_length = length;
                 rc             = SMB_STS_STATUS_OK;
             } else if (SMB_PROTO_READ_BLOCK == protocol) {
                 *return_length = UcsiGetControl(Tempdata, length);
                 rc             = SMB_STS_STATUS_OK;
             } else {
                 *return_length = length;
                 rc             = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_UCSI_READ_MESSAGE_IN:
             /*
             *   access UCSI data structure (MESSAGE IN)
             */
             if (SMB_PROTO_READ_BLOCK == protocol) {
                 *return_length = UcsiGetMessageIn(Tempdata, length);
                 rc             = SMB_STS_STATUS_OK;
             } else {
                 rc             = SMB_STS_STATUS_DATA_ACC_DENIED;
             }
             break;
        case EC_CMD_UCSI_WRITE_MESSAGE_OUT:
             /*
             *    access UCSI data structure (MESSAGE OUT)
             */
             if (SMB_PROTO_WRITE_BLOCK == protocol) {
                 UcsiSetMessageOut(Tempdata, length);
                 *return_length = length;
                 rc             = SMB_STS_STATUS_OK;
             } else if (SMB_PROTO_READ_BLOCK == protocol) {
                 *return_length = UcsiGetMessageOut(Tempdata, length);
                 rc             = SMB_STS_STATUS_OK;
             } else {
                 *return_length = length;
                 rc             = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        case EC_CMD_SET_DATE_TIME:
             if ((SMB_PROTO_WRITE_BLOCK == protocol) &&
                 (7 == length)) {
//                 StmrSetDateTime(*Tempdata + ((UINT)*(Tempdata+1) << 8), *(Tempdata+2), *(Tempdata+3),
//                                 *(Tempdata+4), *(Tempdata+5), *(Tempdata+6));
				          p2FUDayTemp = 0;
				          p2FUDay = 0;
									p2FUD_Year = ((WORD)*(Tempdata+1) << 8) + *Tempdata;
									p2FUD_Month = *(Tempdata+2);
									p2FUD_Day = *(Tempdata+3);
							    SET_MASK(uProject0,uSysTimeOK); //Caculate Current date flag
                  SET_MASK(uProject0, uFUDUpdateTimes); // Clear Flag of Bat0Plug
                  SET_MASK(uBattFUDStatus, uFUDUpdateTimes); // Clear Flag of Bat1Plug
      						CLEAR_MASK(uProject0,uFUdayFail);
             }
             rc = SMB_STS_STATUS_OK;
             break;
        case EC_CMD_CLEAR_BAT_DATE:
             if ((SMB_PROTO_WRITE_BLOCK == protocol) &&
                 (8 == length)) {
                 for (i=0; 8 > i; i++) {
                     if (*(Tempdata+i) != clear_fud_key[i]) {
                         break;
                     }
                 }
                 if (8 <= i) {
//                     SbcClearFirstUsedDate(0);
//                     SbcClearFirstUsedDate(1);
              				SET_MASK(uProject0,uFUdayClr);
              				SET_MASK(uBattFUDStatus,uFUdayClr2);
			SET_MASK(uProject0, uFUDUpdateTimes); // Clear Flag of Bat0Plug
			SET_MASK(uBattFUDStatus, uFUDUpdateTimes); // Clear Flag of Bat1Plug
			CLEAR_MASK(uProject0,uFUdayFail);
                     rc = SMB_STS_STATUS_OK;
                 } else {
                     rc = SMB_STS_STATUS_CMD_ACC_DENIED;
                 }
             } else {
                 rc = SMB_STS_STATUS_CMD_ACC_DENIED;
             }
             break;
        default:
            rc = SMB_STS_STATUS_OK;
    }
    return rc;
}

/*
*******************************************************************************
* Function name: EcCmdMemoryManipulate
*
* Descriptoin: This is used to debug, analyze problems and get status                      
*              by reading memory or I/O port.
*
* Arguments:
*       EC_MEM_MANIPULATE_DATA *ec_mem: address of memory manipulatyion parameter
* Return Values:
*       none
*******************************************************************************
*/

//void EcCmdMemoryManipulate(EC_MEM_MANIPULATE_DATA *ec_mem) {
//    UCHAR                        *addr, *p, *p0;
//    ULONG                        word_data;
//    int                          i, channel, offset;
//    UCHAR                        log_device; 
//    UCHAR                        log_type; 
//    USHORT                       log_data; 
//    ULONG                        log_time;
//    ULONG                        total_log_count;
//    int                          log_size;
//    long                         position; 
//    GSNS_MS_STANDARD_ORIENTATION x, y, z;
//    static IIC_BUF_HDR           iic_hdr;
//    static UCHAR                 iic_buffer[64];
//    static short                 iic_buffer_pointer;
//    GSNS_ORIENTATION             *gs_orientation;
//
//    addr = (UCHAR *)(((ULONG)ec_mem->addr_highest << 16) + ((ULONG)ec_mem->addr_high << 8) + 
//                          ec_mem->addr_low);
//    switch (ec_mem->cmd & 0x7f) {
//        case NOP:
//        case MEM_READ_BYTE:
//        case MEM_READ_WORD:
//        case MEM_WRITE_BYTE:
//        case MEM_WRITE_WORD:
//        case MEM_READ_8:
//        case MEM_BSET:
//        case MEM_BCLR:
//        case MEM_READ_DOUBLE_WORD:
//        case MEM_WRITE_DOUBLE_WORD:
//        case SET_RELATIVE_STATE_OF_CHARGE:
//        case WRITE_TO_PMH:
//        case ENABLE_REMOTE_LOG:
//        case DISABLE_REMOTE_LOG:
//        case SET_REMAINING_CAPACITY:
//        case SET_BATTERY_CURRENT:
//        case SET_USB_PD_DATA:
//        case WRITE_USB_PD:
//             if (FALSE == PwdIsEcLocked()) {
//                 /*
//                 *   restricted commands
//                 */
//                 switch (ec_mem->cmd & 0x7f) {
//                     case MEM_READ_BYTE:
//                          ec_mem->data[0] = HwMemReadByte(addr);
//                          break;
//                     case MEM_READ_WORD:
//                          word_data       = HwMemReadWord(addr);
//                          ec_mem->data[0] = word_data >> 8;
//                          ec_mem->data[1] = word_data & 0xff;
//                          break;
//                     case MEM_WRITE_BYTE:
//                          HwMemWriteByte(addr, ec_mem->data[0]);
//                          break;
//                     case MEM_WRITE_WORD:
//                          HwMemWriteWord(addr, (ec_mem->data[0] << 8) + ec_mem->data[1]);
//                          break;
//                     case MEM_READ_8:
//                          /*
//                          *    read 8 bytes
//                          */
//                          for (i = 0, p = ec_mem->data ; 8 > i; i++, addr++, p++) {
//                              *p = HwMemReadByte(addr);
//                          }
//                          break;
//                     case MEM_BSET:
//                          HwMemSet(addr, ec_mem->data[0]);
//                          break;
//                     case MEM_BCLR:
//                          HwMemClr(addr, ec_mem->data[0]);
//                          break;
//                     case MEM_READ_DOUBLE_WORD:
//                          word_data       = HwMemReadDoubleWord(addr);
//                          ec_mem->data[0] = (word_data >> 24) & 0xff;
//                          ec_mem->data[1] = (word_data >> 16) & 0xff;
//                          ec_mem->data[2] = (word_data >> 8)  & 0xff;
//                          ec_mem->data[3] = word_data & 0xff;
//                          break;
//                     case MEM_WRITE_DOUBLE_WORD:
//                          HwMemWriteDoubleWord(addr, (ec_mem->data[0] << 24) + (ec_mem->data[1] << 16) + 
//                                                     (ec_mem->data[2] << 8) + ec_mem->data[3]);
//                          break;
//                     case SET_RELATIVE_STATE_OF_CHARGE:
//                          SbcSetRelativeStateOfChargeForTest(ec_mem->data[0], (char)ec_mem->data[1]);
//                          break;
//                     case WRITE_TO_PMH:
//                          SpiWrite((ec_mem->addr_high << 8) | ec_mem->addr_low, ec_mem->data[0]);
//                          break;
//                     case ENABLE_REMOTE_LOG:
//                          SysLogEnableRemoteLogging(TRUE);
//                          break;
//                     case DISABLE_REMOTE_LOG:
//                          SysLogEnableRemoteLogging(FALSE);
//                          break;
//                     case SET_REMAINING_CAPACITY:
//                          SbcSetRemainingCapacityForTest(ec_mem->data[0], (short)((ec_mem->data[1] << 8) |
//                                                                                   ec_mem->data[2]));
//                          break;
//                     case SET_BATTERY_CURRENT:
//                          SbcSetCurrentForTest(ec_mem->data[0], (short)((ec_mem->data[1] << 8) |
//                                                                         ec_mem->data[2]));
//                          break;
//                     case SET_USB_PD_DATA:
//                          offset = ec_mem->addr_low;
//                          for (i = 0; (sizeof(ec_mem->data) > i) && (sizeof(iic_buffer) > offset); i++, offset++) {
//                              iic_buffer[offset] = ec_mem->data[i];
//                          }
//                          break;
//                     case WRITE_USB_PD:
//                          /*
//                          *    addr_low : USB ID
//                          *    data[0]  : command
//                          *    data[1]  : length
//                          */
//                          i = SMBusRequest(
//                              SMB_PROTO_WRITE_BLOCK,                               // protocol
//                              (0 == ec_mem->addr_low) ? (0x38 << 1) :              // address
//                              (1 == ec_mem->addr_low) ? (0x3f << 1) :
//                              (2 == ec_mem->addr_low) ? (0x20 << 1) :
//                              (3 == ec_mem->addr_low) ? (0x27 << 1) : (0x38 << 1),
//                              ec_mem->data[0],                                     // command
//                              &iic_hdr,                                            // address of IIC_BUF_HDR
//                              iic_buffer,                                          // address of buffer
//                              (sizeof(iic_buffer) >= ec_mem->data[1]) ?            // length
//                                  ec_mem->data[1] : sizeof(iic_buffer), 
//                              NULL,                                                // attention handler
//                              IoExtractAlternateFunction1(POutI2CDataTbt),         // selector
//                              FALSE,                                               // CRC/ no CRC
//                              IIC_DEFAULT_TIME_OUT);                               // time out value
//                          ec_mem->data[0]    = (i & 0xff); 
//                          iic_buffer_pointer = 0;
//                          break;
//                 }
//             } else {
//                 /*
//                 *    clear data
//                 */
//                 for (i = 0; (sizeof(ec_mem->data)/ sizeof(ec_mem->data[0])) > i; i++) {
//                     ec_mem->data[i] = 0;
//                 }
//                 ec_mem->addr_low     = 0;
//                 ec_mem->addr_high    = 0;
//                 ec_mem->addr_highest = 0;
//             }
//             break;
//        case IO_READ:
//        case IO_WRITE:
//             ec_mem->data[0] = HwMemReadIo(ec_mem->addr_high, &ec_mem->data[1]);
//             break;
//        case GET_IIC_LOG:
//             for (i = 0, p = ec_mem->data; 12 > i; i++, p++) {
//                 word_data = IicGetCommErrorLog(i);
//                 if (0xff < word_data) {
//                     word_data = 0xff;
//                 }
//                 *p = word_data & 0xff;
//             }
//             break;
//        case DISABLE_LOG:
//             SysDisableLog();
//             break;
//        case ENABLE_LOG:
//             SysEnableLog();
//             break;
//        case GET_LOG:
//             if (TRUE == SysGetLog((ec_mem->addr_high << 8) + ec_mem->addr_low, 
//                        &log_device, 
//                        &log_type, 
//                        &log_data, 
//                        &log_time)){
//                 ec_mem->data[1] = log_device;
//                 ec_mem->data[2] = log_type;
//                 ec_mem->data[3] = log_data >> 8;
//                 ec_mem->data[4] = log_data & 0xff;
//                 ec_mem->data[5] = (log_time >> 24) & 0xFF;
//                 ec_mem->data[6] = (log_time >> 16) & 0xFF;
//                 ec_mem->data[7] = (log_time >> 8) & 0xFF;
//                 ec_mem->data[8] = log_time  & 0xFF;
//                 ec_mem->data[0] = 0;
//             } else {
//                 ec_mem->data[0] = 1;
//             }
//             break;
//        case GET_LOG_UNIT_TIME:
//             ec_mem->data[0] = (SysLogGetUnitTime() >> 8) & 0xFF;
//             ec_mem->data[1] = SysLogGetUnitTime() & 0xFF;
//             break;
//        case QUERY_LOG:
//             ec_mem->data[0] = SysQueryLog(&i, &total_log_count, &log_size);
//             ec_mem->data[1] = ((unsigned int)i) >> 8;
//             ec_mem->data[2] = i & 0xff;
//             ec_mem->data[3] = (total_log_count >> 24) & 0xff;
//             ec_mem->data[4] = (total_log_count >> 16) & 0xff;
//             ec_mem->data[5] = (total_log_count >> 8) & 0xff;
//             ec_mem->data[6] = total_log_count & 0xff;
//             ec_mem->data[7] = (log_size >> 8) & 0xff;
//             ec_mem->data[8] = log_size & 0xff;
//             break;
//        case RESET_LOG:
//             SysResetLog();
//             break;
//        case GET_LOG_AT_POSITION:
//             if (TRUE == SysGetLogAtPosition(
//                        ((ULONG)ec_mem->data[0]      << 24) + 
//                        ((ULONG)ec_mem->addr_highest << 16) +
//                        (ec_mem->addr_high           << 8)  + 
//                        ec_mem->addr_low, 
//                        &log_device, 
//                        &log_type, 
//                        &log_data, 
//                        &log_time)){
//                 ec_mem->data[1] = log_device;
//                 ec_mem->data[2] = log_type;
//                 ec_mem->data[3] = log_data >> 8;
//                 ec_mem->data[4] = log_data & 0xff;
//                 ec_mem->data[5] = (log_time >> 24) & 0xFF;
//                 ec_mem->data[6] = (log_time >> 16) & 0xFF;
//                 ec_mem->data[7] = (log_time >> 8) & 0xFF;
//                 ec_mem->data[8] = log_time  & 0xFF;
//                 ec_mem->data[0] = 0;
//             } else {
//                 ec_mem->data[0] = 1;
//             }
//             break;
//        case WRITE_CP:
//             CpPutCheckPointCode(ec_mem->data[0]);
//             break;
//        case CP_CONTROL:
//             /*
//             *   Change Check Point properties
//             */
//             switch (ec_mem->data[0]) {
//                 case DISABLE:
//                      CpDisable();               
//                      break;
//                 case ENABLE:
//                      CpEnable();                
//                      break;
//                 case SECONDARY_IF:
//                      CpUse2ndBatIF();           
//                      break;
//                 case MAIN_IF:
//                      CpUseMainBatIF();          
//                      break;
//                 case AUTO_SELECT:
//                      CpUseBatIFAuto();          
//                      break;
//                 case REAL_TIME:
//                      CpSendCpRealTime();        
//                      break;
//                 case SEND_LAST_PERIODIC:
//                      CpSendLastPeriodically();
//                      break;
//             }
//             break;
//        case WRITE_EC_CP:
//             CpPutEcCheckPointCode(ec_mem->data[0]);
//             break;
//        case READ_INTERFACE_SPACE:
//        case WRITE_INTERFACE_SPACE:
//        case READ_INTERFACE_SPACE_8:
//             /*
//             *   access in the interface space
//             */
//             word_data = ((ULONG)ec_mem->addr_highest << 16) + ((ULONG)ec_mem->addr_high << 8) + 
//                         ec_mem->addr_low;
//             switch (ec_mem->cmd & 0x7f) {
//                 case READ_INTERFACE_SPACE:
//                      ec_mem->data[0] = AcpiRead(word_data);
//                      break;
//                 case WRITE_INTERFACE_SPACE:
//                      AcpiWrite(word_data, ec_mem->data[0]);
//                      break;
//                 case READ_INTERFACE_SPACE_8:
//                      for (i = 0; sizeof(ec_mem->data) > i; i++) {
//                          ec_mem->data[i] = AcpiRead(word_data + i);
//                      }
//                      break;
//             }
//             break;
//        case DISABLE_SMSBUS_SLAVE:
//             /*
//             *   disable slave address for remote debugging
//             *   this is needed to become the remote debugging machine
//             */
//             EccDisable();
//             break;
//        case ENABLE_SMSBUS_SLAVE:
//             /*
//             *   ebable slave address for remote debugging
//             */
//             EccEnable();
//             break;
//        case READ_FROM_PMH:
//             ec_mem->data[0] = SpiRead((ec_mem->addr_high << 8) | ec_mem->addr_low);
//             break;
//        case READ_IO_CONTROL:
//             /*
//             *    Get I/O control register 
//             */
//             word_data       = HwMemReadIoPortSetting(ec_mem->addr_high, ec_mem->addr_low);
//             ec_mem->data[0] = (word_data >> 24) & 0xff;
//             ec_mem->data[1] = (word_data >> 16) & 0xff;
//             ec_mem->data[2] = (word_data >> 8)  & 0xff;
//             ec_mem->data[3] = word_data & 0xff;
//             break;
//        case READ_ROM:
//             addr = (UCHAR *)(((ULONG)ec_mem->addr_highest << 16) + 
//                                  ((ULONG)ec_mem->addr_high << 8) + 
//                                  ec_mem->addr_low);
//             for (i = 0, p = ec_mem->data ; 8 > i; i++, addr++) {
//                 ec_mem->data[i] = HwMemReadRom(addr);
//             }
//             break;
//        case GS_CONTROL:
//             switch (ec_mem->data[0]) {
//                case 0:
//                     /*
//                     *   stop monitor and turn off
//                     */
//                     GsnsSetSTPin(0);
//                     GsnsStartGravitySense(FALSE, 0, 0, 0);
//                     GsnsPowerOnSensor(FALSE);
//                     break;
//                case 1:
//                     /*
//                     *   turn on and start monitor
//                     */
//                     GsnsSetSTPin(0);
//                     GsnsPowerOnSensor(TRUE);
//                     GsnsStartGravitySense(TRUE, 50, 2, 1);
//                     break;
//                case 2:
//                     /*
//                     *   enable diago mode and
//                     *   turn on and start monitor
//                     */
//                     GsnsSetSTPin(1);
//                     GsnsPowerOnSensor(TRUE);
//                     GsnsStartGravitySense(TRUE, 200, 2, 1);
//                     break;
//                case 3:
//                     /*
//                     *   get status
//                     */
//                     GsnsGetMonitorStatus(&ec_mem->data[1],                    // status
//                                          &ec_mem->data[2], &ec_mem->data[3],  // frequency
//                                          &ec_mem->data[4]);                   // average factor
//                     ec_mem->data[0] = 0;
//                     break;
//                case 4:
//                     /*
//                     *   turn on and start monitor with frequency and average factor
//                     */
//                     GsnsSetSTPin(0);
//                     GsnsPowerOnSensor(TRUE);
//                     GsnsStartGravitySense(TRUE,
//                                           ec_mem->data[1] | (ec_mem->data[2] << 8),  // frequency
//                                           ec_mem->data[3], 10);                      // average factor
//                     break;
//                default:
//                     for (i = 0; (sizeof(ec_mem->data) / sizeof(ec_mem->data[0])) > i; i++) {
//                         ec_mem->data[i] = 0;
//                     }
//             }
//             break;
//        case GS_GET_DATA:
//             i                    = sizeof(ec_mem->data) / 5;
//             if (ec_mem->addr_low < i) {
//                 i = ec_mem->addr_low;
//             }
//             ec_mem->addr_low     = GsnsGetData(i, ec_mem->data);
//             ec_mem->addr_high    = 0;
//             ec_mem->addr_highest = 0;
//             break;
//        case GS_GET_VERSION:   
//             GsnsGetModuleVersion(&ec_mem->data[0], &ec_mem->data[1], &ec_mem->data[2]);
//             break;
//        case GS_GET_DATA_3_AXES:
//             i                    = sizeof(ec_mem->data) / 7;
//             if (ec_mem->addr_low < i) {
//                 i = ec_mem->addr_low;
//             }
//             ec_mem->addr_low     = GsnsGetDataEx(i, ec_mem->data);
//             ec_mem->addr_high    = 0;
//             ec_mem->addr_highest = 0;
//             break;
//        case GS_GET_SENSOR_TYPE:
//             gs_orientation = GsnsGetOrientation();
//             ec_mem->addr_low     = GsnsGetSensorType(&ec_mem->data[0], &ec_mem->data[1],
//                                                      &ec_mem->data[2], &ec_mem->data[3]);
//             ec_mem->data[4]      = gs_orientation->x;
//             ec_mem->data[5]      = gs_orientation->y;
//             ec_mem->data[6]      = gs_orientation->z;
//             ec_mem->addr_high    = 0;
//             ec_mem->addr_highest = 0;
//             break;
//        case READ_TEMP:  
//             channel = ec_mem->data[0];
//             for (i = 0; (sizeof(ec_mem->data)/ sizeof(ec_mem->data[0])) > i; i++) {
//                 ec_mem->data[i] = 0x80;    // fill with value as unknwown
//             }
//             for (i = 0; ((sizeof(ec_mem->data) / sizeof(ec_mem->data[0]) > i) && (16 > channel)); i++, channel++) {
//                 ec_mem->data[i] = ThermGetTemperature(channel);
//             }
//             break;
//        case READ_USB_PD:
//             /*
//             *    addr_low : USB ID
//             *    data[0]  : command
//             */
//             i = SMBusRequest(
//                 SMB_PROTO_READ_BLOCK,                                // protocol
//                 (0 == ec_mem->addr_low) ? (0x38 << 1) :              // address
//                 (1 == ec_mem->addr_low) ? (0x3f << 1) :
//                 (2 == ec_mem->addr_low) ? (0x20 << 1) :
//                 (3 == ec_mem->addr_low) ? (0x27 << 1) : (0x38 << 1),
//                 ec_mem->data[0],                                     // command
//                 &iic_hdr,                                            // address of IIC_BUF_HDR
//                 iic_buffer,                                          // address of buffer
//                 sizeof(iic_buffer),                                  // length
//                 NULL,                                                // attention handler
//                 IoExtractAlternateFunction1(POutI2CDataTbt),         // selector
//                 FALSE,                                               // CRC/ no CRC
//                 IIC_DEFAULT_TIME_OUT);                               // time out value
//             ec_mem->data[0]    = (i & 0xff); 
//             iic_buffer_pointer = 0;
//             break;
//        case GET_STATUS_USB_PD:
//             ec_mem->data[0] = iic_hdr.flag.done;
//             ec_mem->data[1] = iic_hdr.status;
//             ec_mem->data[2] = iic_hdr.length;
//             break;
//        case GET_DATA_USB_PD:
//             ec_mem->data[0] = iic_hdr.length & 0xff;
//             ec_mem->data[1] = iic_buffer_pointer & 0xff;
//             for (i = 3; (sizeof(iic_buffer) > iic_buffer_pointer) &&
//                         (iic_hdr.length > iic_buffer_pointer) && 
//                         (sizeof(ec_mem->data) > i); i++, iic_buffer_pointer++) {
//                 ec_mem->data[i] = iic_buffer[iic_buffer_pointer];
//             }
//             ec_mem->data[2] = (i - 3) & 0xff;
//             break;
//        default:
//             /*
//             *    clear data
//             */
//             for (i = 0; (sizeof(ec_mem->data)/ sizeof(ec_mem->data[0])) > i; i++) {
//                 ec_mem->data[i] = 0;
//             }
//    }
//    /*
//    *   clear the busy bit
//    */
//    ec_mem->cmd &= 0x7f;
//}

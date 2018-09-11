/*
********************************************************************************
* LCFC Confidential
*
* Microcode Source Materials
*
* ThinkPad Embedded Controller Microcode
*
* COPYRIGHT LCFC 2017 ALL RIGHTS RESERVED
*
* Filename: OEM_TPS65988.C
*
* Description:
*    TPS65988 TI Type-C PD controller process function.
*
* History:
*   01/04/2017    Isaac Lin
*     created as new
********************************************************************************
*/

//*****************************************************************************
// Include all header file
//*****************************************************************************
#include "..\..\include.h"


#if (Support_TYPE_C == TPS65988)

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

enum __TYPEC_DETECTION
{
    TPS65988_READ_MODE = 0,
    TPS65988_READ_BOOT_FLAGS_AND_OTP_CONFIG,
    TPS65988_SET_TIMER_FOR_PORT_B_READY,
    TPS65988_INIT_EVENT_MASK_READ_TO_GET_SIZE,
    TPS65988_INIT_SEND_EVENT_MASK,
    TPS65988_INIT_SEND_EVENT_CLEAR,
    CHECK_POWER_STATUS,
    CHECK_AND_CLEAR_DEAD_BATTERY_MODE,
    POLLING_IRQ,
    INITIAL_FINISH,
    TPS65988_ERROR_STATE,
};
enum __TPS65988_4CC_CMD_LIST
{
    TPS65988_4CC_CMD_CLEAR_DEAD_BATTERY_FLAG = 0,
    TPS65988_4CC_CMD_TURN_ON_INPUT_PATH,
    TPS65988_4CC_CMD_TURN_OFF_INPUT_PATH,
    TPS65988_4CC_CMD_COLD_BOOT,
    TPS65988_4CC_CMD_SOFT_RESET,
    TPS65988_4CC_CMD_SWAP_TO_SINK,
    TPS65988_4CC_CMD_SWAP_TO_SOURCE,
    TPS65988_4CC_CMD_SWAP_TO_DFP,
    TPS65988_4CC_CMD_SWAP_TO_UFP,
    TPS65988_4CC_CMD_SEND_VDM,
    TPS65988_4CC_CMD_AUTO_NEGOTIATE_SINK,
    TPS65988_4CC_CMD_HARDWARE_RESET,
    TPS65988_4CC_CMD_GSRC,
    TPS65988_4CC_CMD_RETRY_I2C, ////A285D00065: Add PD control for i2c retry--ti spec
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
* Function name: OEM_TYPE_C_ADPID_HOOK
*
* Descriptoin:
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_ADPID_HOOK(void)
{
#if 0
    if(Read_DCIN_ATTACHED2() != IS_MASK_SET(TypeCStatus,OldDcinAttached2))
    {
//      if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_SYSTEM_CONFIG, SMB3_DATA))
//      {
//          TypeCCommFailCnt++;
//      }
//      else
//      {
//        TypeCCommFailCnt = 0;
//      }
        PowerSourceSwitchDelay = 20;
        //PowerSourceSwitch();
        if(Read_DCIN_ATTACHED2())
        {
            SET_MASK(TypeCStatus,OldDcinAttached2);
            PD_VBUS_CTRL1_DIS_SDV();
        }
        else
        {
            CLEAR_MASK(TypeCStatus,OldDcinAttached2);
            PD_VBUS_CTRL1_EN_SDV();
        }
        if(Read_AC_IN()
           && (Read_DCIN_ATTACHED2() || (!Read_DCIN_ATTACHED2() && IS_MASK_SET(TypeCStatus,TypeCAdpReady)))
          )
        {
            //If DCIN_ATTACHED2 status change,that means adaptor ID need re-setting.
            ADPID_ON_EN();
            CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
        }
    }
#endif

}


/*
*******************************************************************************
* Function name: DetectAdaptorWatt
*
* Descriptoin: Get the currend PDO and calculate current adaptor output watt
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void DetectAdaptorWatt()
{
    BYTE i;
    WORD VoltageTemp;
//A485D00080---S
    if(TypeCAdpDetectDelay)
    {
        TypeCAdpDetectDelay--;
        return;
    }//A485D00080---E
    if(TypeCAdpPdStableCheck != 0)
    {

        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_SOURCE_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
            TypeCAdpPdStableCheck--;
        }
        else
        {
            TypeCCommFailCnt = 0;
            ObjectPosition20V = SMB3_DATA[0];   //PDOs number
            CurrentRDO = ObjectPosition20V;
            for(i=0; i<SMB3_DATA[0]; i++)
            {
                VoltageTemp = ((SMB3_DATA[(i*4)+3]&0x0F)<<6)+(SMB3_DATA[(i*4)+2]>>2);   //Unit:50mV
                if(VoltageTemp == 180)  //180*50mV=9000mV=9V
                {
                    ObjectPosition09V = i+1;
                    PDO09V = VoltageTemp;
                    PDO09I = ((SMB3_DATA[(i*4)+2]&0x03)<<8)+SMB3_DATA[(i*4)+1]; //Unit:10mA
                }
                if(VoltageTemp == 400)  //400*50mV=20000mV=20V
                {
                    ObjectPosition20V = i+1;
                    PDO20V = VoltageTemp;
                    PDO20I = ((SMB3_DATA[(i*4)+2]&0x03)<<8)+SMB3_DATA[(i*4)+1]; //Unit:10mA
                }
            }

        }
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_ACTIVE_CONTRACT_PDO, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
            TypeCAdpPdStableCheck--;
        }
        else
        {
            TypeCCommFailCnt = 0;

            Port1CurrentTypeCV = ((SMB3_DATA[2]&0x0F)<<6)+(SMB3_DATA[1]>>2);    //Unit:50mV
            Port1CurrentTypeCI = ((SMB3_DATA[1]&0x03)<<8)+SMB3_DATA[0];         //Unit:10mA
            Port1CurrentTypeCWatt = (BYTE)((DWORD)(Port1CurrentTypeCV)*Port1CurrentTypeCI/2000);    //Watt=(V*50mV)*(I*10mA)=(V*50/1000)*(I*10/1000)=V*I/2000
            if(Port1CurrentTypeCWatt == 0)
            {
                //A485D00097: Add HW reset when can not read adaptor watt--->Start
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_MODE, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCCommFailCnt++;
                    TypeCAdpPdStableCheck--;
                }
                else
                {
                    TypeCCommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if((SMB3_DATA[0] == 'A') && (SMB3_DATA[1] == 'P') && (SMB3_DATA[2] == 'P'))
                    {
                        //  mode is APP
                        TypeCAdpPdStableCheck--;
                    }
                    else
                    {
                        RamDebug(0xDD);
                        RamDebug(0x11);
                        RamDebug(0xDD);
                        CLEAR_MASK(TypeCStatus,TypeCIniOK);
                        Send4ccCmd(TPS65988_4CC_CMD_HARDWARE_RESET, TYPECPort1);
                    }
                }
            }  //A485D00097: Add HW reset when can not read adaptor watt--->End
            else
            {
                TypeCAdpPdStableCheck = 0;
            }
            //A485D00080---S
            switch(Port1CurrentTypeCV)
            {
                case 100:
                    // 100*50mV=5000mV=5V
                    //CurrentRDO = ObjectPosition05V;
                    break;
                case 180:
                    // 180*50mV=9000mV=9V
                    CurrentRDO = ObjectPosition09V;
                    break;
                case 240:
                    // 240*50mV=12000mV=12V
                    //CurrentRDO = ObjectPosition12V;
                    break;
                case 300:
                    // 300*50mV=15000mV=15V
                    //CurrentRDO = ObjectPosition15V;
                    break;
                case 400:
                    // 400*50mV=20000mV=20V
                    CurrentRDO = ObjectPosition20V;
                    break;
            }
            //A485D00080---E
            if(Port1CurrentTypeCWatt >= 230)
            {
                TypeCPort1AdpID = AdapterID_230W;
            }
            else if(Port1CurrentTypeCWatt >= 170)
            {
                TypeCPort1AdpID = AdapterID_170W;
            }
            else if(Port1CurrentTypeCWatt >= 135)
            {
                TypeCPort1AdpID = AdapterID_135W;
            }
            else if(Port1CurrentTypeCWatt >= 90)
            {
                TypeCPort1AdpID = AdapterID_90W;
            }
            else if(Port1CurrentTypeCWatt >= 65)
            {
                TypeCPort1AdpID = AdapterID_65W;
            }
            else if(Port1CurrentTypeCWatt >= 60)
            {
                TypeCPort1AdpID = AdapterID_60W;
            }
            else if(Port1CurrentTypeCWatt >= 45)
            {
                TypeCPort1AdpID = AdapterID_45W;
            }
            else if(Port1CurrentTypeCWatt >= 36)
            {
                TypeCPort1AdpID = AdapterID_36W;
            }
            else if(Port1CurrentTypeCWatt > 15) //A285D00069+ 0->15
            {
                TypeCPort1AdpID = AdapterID_Lower36W;
            }
            else
            {
                TypeCPort1AdpID = AdapterID_NON_SUPPORT;
                //A485D00080---S
                if(UnknowAdpRetry <= 5) //A285D00069+ 3-5times
                {
                    UnknowAdpRetry++;
                    TypeCAdpDetectDelay = 10;
                    TypeCAdpPdStableCheck = 6;
                    return;
                }
                else
                {
                    //A285D00069+ start
                    UnknowAdpRetry = 0;
                    TypeCAdpDetectDelay = 0;
                    TypeCAdpPdStableCheck = 0;
                    //A285D00069+ end
                    //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                    SinkPowerPathControl(0, TYPECPort1);
                    //set unsupport adapater flag
                    SET_MASK(u4SenseSTS3, UnSupportAdapterPort1);
                }
                //A485D00080---E
            }

            if(TypeCPort1AdpID != AdapterID_NON_SUPPORT)    //A285D00069+
            {
                UnknowAdpRetry = 0;
                //A285D00069+ normal watt clear flag
                CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort1);
            }

            //Re-setting adaptor parameter
            if(0 == Port_Select)
            {
                Port_Select         = TYPECPort1;
                TypeCAdpID      = TypeCPort1AdpID;
                CurrentTypeCWatt    = Port1CurrentTypeCWatt;
                CurrentTypeCV   = Port1CurrentTypeCV;
                CurrentTypeCI       = Port1CurrentTypeCI;
            }

            //init port1 pd switch -20180413
            if(isBatteryExist())
            {
                SetMultiPortSNK(TYPECPort1);
            }

            //ADPID_ON_EN();
            //CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
            UsbPdcGetPowerStatus(0);
            SET_MASK(TypeCStatus,TypeCAdpReady);
        }
    }
}

/*
*******************************************************************************
* Function name: DetectPort2AdaptorWatt
*
* Descriptoin: Get the currend PDO and calculate current adaptor output watt
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void DetectPort2AdaptorWatt()
{
    BYTE i;
    WORD VoltageTemp;
//A485D00080---S
    if(TypeCPort2AdpDetectDelay)
    {
        TypeCPort2AdpDetectDelay--;
        return;
    }//A485D00080---E
    if(TypeCPort2AdpPdStableCheck != 0)
    {
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_RX_SOURCE_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCPort2CommFailCnt++;
            TypeCPort2AdpPdStableCheck--;
        }
        else
        {
            TypeCPort2CommFailCnt = 0;
            Port2ObjectPosition20V = SMB3_DATA[0];  //PDOs number
            Port2CurrentRDO = Port2ObjectPosition20V;
            for(i=0; i<SMB3_DATA[0]; i++)
            {
                VoltageTemp = ((SMB3_DATA[(i*4)+3]&0x0F)<<6)+(SMB3_DATA[(i*4)+2]>>2);   //Unit:50mV
                if(VoltageTemp == 180)  //180*50mV=9000mV=9V
                {
                    Port2ObjectPosition09V = i+1;
                    Port2PDO09V = VoltageTemp;
                    Port2PDO09I = ((SMB3_DATA[(i*4)+2]&0x03)<<8)+SMB3_DATA[(i*4)+1];    //Unit:10mA
                }
                if(VoltageTemp == 400)  //400*50mV=20000mV=20V
                {
                    Port2ObjectPosition20V = i+1;
                    Port2PDO20V = VoltageTemp;
                    Port2PDO20I = ((SMB3_DATA[(i*4)+2]&0x03)<<8)+SMB3_DATA[(i*4)+1];    //Unit:10mA
                }
            }

        }
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_ACTIVE_CONTRACT_PDO, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCPort2CommFailCnt++;
            TypeCPort2AdpPdStableCheck--;
        }
        else
        {
            TypeCPort2CommFailCnt = 0;

            Port2CurrentTypeCV = ((SMB3_DATA[2]&0x0F)<<6)+(SMB3_DATA[1]>>2);    //Unit:50mV
            Port2CurrentTypeCI = ((SMB3_DATA[1]&0x03)<<8)+SMB3_DATA[0];         //Unit:10mA
            Port2CurrentTypeCWatt = (BYTE)((DWORD)(Port2CurrentTypeCV)*Port2CurrentTypeCI/2000);    //Watt=(V*50mV)*(I*10mA)=(V*50/1000)*(I*10/1000)=V*I/2000
            if(Port2CurrentTypeCWatt == 0)
            {
                ////A485D00097: Add HW reset when can not read adaptor watt--->Start
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_MODE, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCPort2CommFailCnt++;
                    TypeCPort2AdpPdStableCheck--;
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if((SMB3_DATA[0] == 'A') && (SMB3_DATA[1] == 'P') && (SMB3_DATA[2] == 'P'))
                    {
                        //  mode is APP
                        //TypeCPort2ProcessStep++;
                        TypeCPort2AdpPdStableCheck--;
                    }
                    else
                    {
                        RamDebug(0xDD);
                        RamDebug(0x22);
                        RamDebug(0xDD);
                        CLEAR_MASK(TypeCPort2Status,TypeCIniOK);
                        Send4ccCmd(TPS65988_4CC_CMD_HARDWARE_RESET, TYPECPort2);
                    }

                }


                //A485D00097: Add HW reset when can not read adaptor watt--->End
            }
            else
            {
                TypeCPort2AdpPdStableCheck = 0;
            }
            //A485D00080---S
            switch(Port2CurrentTypeCV)
            {
                case 100:
                    // 100*50mV=5000mV=5V
                    //  Port2CurrentRDO = ObjectPosition05V;
                    break;
                case 180:
                    // 180*50mV=9000mV=9V
                    Port2CurrentRDO = Port2ObjectPosition09V;
                    break;
                case 240:
                    // 240*50mV=12000mV=12V
                    //  Port2CurrentRDO = ObjectPosition12V;
                    break;
                case 300:
                    // 300*50mV=15000mV=15V
                    //  Port2CurrentRDO = ObjectPosition15V;
                    break;
                case 400:
                    // 400*50mV=20000mV=20V
                    Port2CurrentRDO = Port2ObjectPosition20V;
                    break;
            }
            //A485D00080---E

            if(Port2CurrentTypeCWatt >= 230)
            {
                TypeCPort2AdpID = AdapterID_230W;
            }
            else if(Port2CurrentTypeCWatt >= 170)
            {
                TypeCPort2AdpID = AdapterID_170W;
            }
            else if(Port2CurrentTypeCWatt >= 135)
            {
                TypeCPort2AdpID = AdapterID_135W;
            }
            else if(Port2CurrentTypeCWatt >= 90)
            {
                TypeCPort2AdpID = AdapterID_90W;
            }
            else if(Port2CurrentTypeCWatt >= 65)
            {
                TypeCPort2AdpID = AdapterID_65W;
            }
            else if(Port2CurrentTypeCWatt >= 60)
            {
                TypeCPort2AdpID = AdapterID_60W;
            }
            else if(Port2CurrentTypeCWatt >= 45)
            {
                TypeCPort2AdpID = AdapterID_45W;
            }
            else if(Port2CurrentTypeCWatt >= 36)
            {
                TypeCPort2AdpID = AdapterID_36W;
            }
            else if(Port2CurrentTypeCWatt > 15) //A285D00069+ 0->15
            {
                TypeCPort2AdpID = AdapterID_Lower36W;
            }
            else
            {
                TypeCPort2AdpID = AdapterID_NON_SUPPORT;
                //A485D00080---S

                if(Port2UnknowAdpRetry <= 5)    //A285D00069+ 3->5times
                {
                    Port2UnknowAdpRetry++;
                    TypeCPort2AdpDetectDelay = 10;
                    TypeCPort2AdpPdStableCheck = 6;
                    return;
                }//A485D00080---E
                else
                {
                    //A285D00069 start+
                    Port2UnknowAdpRetry = 0;
                    TypeCPort2AdpPdStableCheck = 0; 
                    TypeCPort2AdpDetectDelay = 0;
                    //A285D00069 end+
                    //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                    SinkPowerPathControl(0, TYPECPort2);
                    //set unsupport adapater flag
                    SET_MASK(u4SenseSTS3, UnSupportAdapterPort2);
                }
            }

            if(TypeCPort2AdpID != AdapterID_NON_SUPPORT)    //A285D00069+
            {
                Port2UnknowAdpRetry=0; ////A485D00080

                CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort2); //A285D00069+ normal watt clear flag
            }
            // use same setting here that need to confirm spec define.
            if(0 == Port_Select)
            {
                Port_Select         = TYPECPort2;
                TypeCAdpID = TypeCPort2AdpID;
                CurrentTypeCWatt = Port2CurrentTypeCWatt;
                CurrentTypeCV = Port2CurrentTypeCV;
                CurrentTypeCI = Port2CurrentTypeCI;
            }
            //init port2 pd switch -20180413
            if(isBatteryExist())
            {
                SetMultiPortSNK(TYPECPort2); //A285D00056: fix cs18 dock led blink on s5 mode plug-in
            }
            //Re-setting adaptor parameter
            //ADPID_ON_EN();
            //CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
            UsbPdcGetPowerStatus(1);
            SET_MASK(TypeCPort2Status,TypeCAdpReady);
        }
    }
}


/*
*******************************************************************************
*       Global Function Declarations
*******************************************************************************
*/

/*
*******************************************************************************
* Function name: ECResetTypecPD
*
* Descriptoin: This is to reset TypeC PD by EC command.
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void ECResetTypecPD(void)
{

//  rI2C_DATA = TYPEC_RESET;
//  rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
//  rI2C_DATA1[1] = 0x52;
//  rI2C_DATA1[2] = 0x01;
//  if(I2C_Write(chSMB_TYPEC, TypeC_01_Addr, 0x04,&I2C_DATA)==FALSE)
//  {
//    CHGIC_SMbusFailCnt++;
//  }
//  else
//  {
//    CHGIC_SMbusFailCnt = 0;
//  }
}
/*
*******************************************************************************
* Function name: InitialTypecPD
*
* Descriptoin: This is to detect TypeC PD in ready and initial TypeC PD.
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void InitialTypecPD(void)
{
    BYTE TempRomData, Temp8;

    if(TypeCIniDelay == 0)
    {
        switch(TypeCProcessStep)
        {
            case TPS65988_READ_MODE:
                //Get PD mode
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_MODE, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCCommFailCnt++;
                    TypeCIniDelay = 100;
                    if(TypeCCommFailCnt == 15)
                    {
                        TypeCIniDelay = 200;
                        //    TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCCommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if((SMB3_DATA[0] == 'A') && (SMB3_DATA[1] == 'P') && (SMB3_DATA[2] == 'P'))
                    {
                        //  mode is APP
                        TypeCProcessStep++;
                    }
                }
                break;
            case TPS65988_READ_BOOT_FLAGS_AND_OTP_CONFIG:
                //Read Boot Flags and OTP configuration register first
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_BOOT_FLAG_OTP_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
                    {
                        TypeCIniDelay = 200;
                        //    TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCCommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if(TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEAD_BATTERY(SMB3_DATA[0]))
                    {
                        SET_MASK(TypeCStatus2,TypeC_Dead_Battery_Mode);
                        TypeCProcessStep = TPS65988_INIT_EVENT_MASK_READ_TO_GET_SIZE;
                    }
                    else
                    {
                        /*
                        *    Power supplied from USB-C PORT B
                        *    then wait for 500ms
                        */
                        CLEAR_MASK(TypeCStatus2,TypeC_Dead_Battery_Mode);
                        TypeCProcessStep++;
                    }
                }
                break;
            case TPS65988_SET_TIMER_FOR_PORT_B_READY:
                /*
                ********************************************************
                *     Wait for 500ms if power is supplied from USB PDC B port
                *   because it takes more than 400ms after APP read
                *   to be able to read correct status, power status and PD status
                ********************************************************
                */
                TypeCProcessStep++;
                TypeCIniDelay = TypeC_IntSetting_Delay_time;
                break;
            case TPS65988_INIT_EVENT_MASK_READ_TO_GET_SIZE:
                /*
                **************************************************
                *     Initialize INT_EVENT
                **************************************************
                */

                //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_EVENT2, tmpPDPort1INT,bRead_I2C_NULL);

                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_MASK2, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
                    {
                        TypeCIniDelay = 200;
                        // TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCCommFailCnt = 0;
                    TypeCProcessStep++;
                }
                break;
            case TPS65988_INIT_SEND_EVENT_MASK:
                /*
                *    Initialize event interrupt masks
                */
                SMB3_DATA[0] = 0x38;//BIT5+BIT4+BIT3;
                SMB3_DATA[1] = 0x78;//BIT5+BIT4+BIT3;
                SMB3_DATA[2] = 0x02;//BIT1
                SMB3_DATA[3] = 0x4D;//BIT6+BIT3+BIT2+BIT0;
                SMB3_DATA[4] = 0x00;
                SMB3_DATA[5] = 0x00;
                SMB3_DATA[6] = 0x00;
                SMB3_DATA[7] = 0x0F;//BIT3+BIT2+BIT1+BIT0;
                SMB3_DATA[8] = 0x00;
                SMB3_DATA[9] = 0x00;
                SMB3_DATA[10] = 0x00;

                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_MASK2, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                {
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
                    {
                        TypeCIniDelay = 200;
                        // TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }
                else
                {
                    //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_EVENT2, tmpPDPort1INT,bRead_I2C_NULL);
#if 0
                    // WA for PD rework,WTF +
                    SMB3_DATA[0] = 0x00;//BIT3;
                    SMB3_DATA[1] = 0x00;//BIT5+BIT4+BIT3;
                    SMB3_DATA[2] = 0x00;
                    SMB3_DATA[3] = 0x00;//BIT7+BIT3+BIT2+BIT0;
                    SMB3_DATA[4] = 0x00;
                    SMB3_DATA[5] = 0x00;
                    SMB3_DATA[6] = 0x00;
                    SMB3_DATA[7] = 0x00;//BIT3+BIT2+BIT1+BIT0;
                    SMB3_DATA[8] = 0x00;
                    SMB3_DATA[9] = 0x00;
                    SMB3_DATA[10] = 0x00;

                    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_MASK1, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                    {
                        TypeCCommFailCnt++;
                        if(TypeCCommFailCnt == 3)
                        {
                            TypeCIniDelay = 200;
                            TypeCProcessStep = TPS65988_ERROR_STATE;
                            TypeCCommFailCnt = 0;
                            break;
                        }
                    }
                    // WA for PD rework,WTF -
#endif
                    TypeCCommFailCnt = 0;
                    TypeCProcessStep++;
                }
                break;
            case TPS65988_INIT_SEND_EVENT_CLEAR:
                /*
                *    Initialize event interrupt masks
                */
#if 0
                // WA for PD rework,WTF +
                SMB3_DATA[0] = 0xFF;
                SMB3_DATA[1] = 0xFF;
                SMB3_DATA[2] = 0xFF;
                SMB3_DATA[3] = 0xFF;
                SMB3_DATA[4] = 0xFF;
                SMB3_DATA[5] = 0xFF;
                SMB3_DATA[6] = 0xFF;
                SMB3_DATA[7] = 0xFF;
                SMB3_DATA[8] = 0xFF;
                SMB3_DATA[9] = 0xFF;
                SMB3_DATA[10] = 0xFF;

                //if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_CLEAR1, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                {
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
                    {
                        TypeCIniDelay = 200;
                        TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }

                //else if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_CLEAR2, &SMB3_DATA[0], 11, SMBus_NoPEC))
                //{
                //  TypeCCommFailCnt++;
                //      if(TypeCCommFailCnt == 3)
                //      {
                //      TypeCIniDelay = 200;
                //      TypeCProcessStep = TPS65988_ERROR_STATE;
                //      TypeCCommFailCnt = 0;
                //      }
                //}
                else
#endif
                    // WA for PD rework,WTF -
                {
                    TypeCCommFailCnt = 0;
                    TypeCProcessStep++;
                }
                break;
            case CHECK_POWER_STATUS:
                //Check power status for dead battery
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_POWER_STATUS, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCCommFailCnt++;
                    if(TypeCCommFailCnt == 3)
                    {
                        TypeCIniDelay = 200;
                        // TypeCProcessStep = TPS65988_ERROR_STATE;
                        TypeCCommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCCommFailCnt = 0;
                    if(TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA[0]) == TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT)
                    {
                        /*
                        *   Connection is present
                        */
                        if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA[0]))
                        {
                            /*
                            *    USB PD act as Sink
                            */
                            SET_MASK(TypeCStatus,TypeCAdpExist);
                            if(TypeCAdpPdStableCheck == 0)
                            {
                                TypeCAdpPdStableCheck = 3;
                            }
                            DetectAdaptorWatt();
                            if(TypeCAdpPdStableCheck == 0)
                            {
                                TypeCProcessStep++;
                            }
                        }
                        else
                        {
                            TypeCProcessStep++;
                        }
                    }
                    else
                    {
                        TypeCProcessStep++;
                    }
                }
                break;

            case CHECK_AND_CLEAR_DEAD_BATTERY_MODE:
//          if(OneOfTwoBattExistGet || (Read_AC_IN()&&IS_MASK_SET(Attach2status,StableLLevel)))
//          {
                Send4ccCmd(TPS65988_4CC_CMD_CLEAR_DEAD_BATTERY_FLAG,TYPECPort1);
                CLEAR_MASK(TypeCStatus2,TypeC_Dead_Battery_Mode);
//          }
                TypeCProcessStep++;
                break;

            case POLLING_IRQ:

                if(TypecPDProcess(TRUE))
                {
                    TypeCProcessStep++;
                }
                break;

            case INITIAL_FINISH:
                //Initial finish
                SET_MASK(TypeCStatus,TypeCIniOK);
                //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_EVENT2, tmpPDPort1INT,bRead_I2C_NULL);
                TempRomData = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_TYPEC_STATUS);
                if((TempRomData & 0x01) == 0x01)
                {
                    Temp8 = TempRomData & 0xFE;   //Clear bit0:Type-C chip exist
                    WRITE_DATA2EEPROM(&Temp8, 1, EEPROM_Bank7, EEPROM_TYPEC_STATUS);
                }
                TypeCProcessStep = 0;   //Clear for new process flow step restart.
                break;
            case TPS65988_ERROR_STATE:
                //Type-C communiction error
                break;
            default:
                TypeCProcessStep = 0;   //Clear for new process flow step restart.
                break;
        }
    }
    else
    {
        TypeCIniDelay--;
    }
}

/*
*******************************************************************************
* Function name: InitialTypecPDPort2
*
* Descriptoin: This is to detect TypeC PD in ready and initial TypeC PD port2.
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

void InitialTypecPDPort2(void)
{
    BYTE TempRomData, Temp8;

    if(TypeCPort2IniDelay == 0)
    {
        switch(TypeCPort2ProcessStep)
        {
            case TPS65988_READ_MODE:
                //Get PD mode
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_MODE, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCPort2CommFailCnt++;
                    TypeCPort2IniDelay = 100;
                    if(TypeCPort2CommFailCnt == 15)
                    {
                        TypeCPort2IniDelay = 200;
                        //     TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if((SMB3_DATA[0] == 'A') && (SMB3_DATA[1] == 'P') && (SMB3_DATA[2] == 'P'))
                    {
                        //  mode is APP
                        TypeCPort2ProcessStep++;
                    }
                }
                break;
            case TPS65988_READ_BOOT_FLAGS_AND_OTP_CONFIG:
                //Read Boot Flags and OTP configuration register first
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_BOOT_FLAG_OTP_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCPort2CommFailCnt++;
                    if(TypeCPort2CommFailCnt == 3)
                    {
                        TypeCPort2IniDelay = 200;
                        //     TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                    /*
                    *    confirm TPS65988 in Application mode
                    */
                    if(TPS65988_BOOT_FLAG_OTP_CONFIG_BOOT_DEAD_BATTERY(SMB3_DATA[0]))
                    {
                        SET_MASK(TypeCPort2Status2,TypeC_Dead_Battery_Mode);
                        TypeCPort2ProcessStep = TPS65988_INIT_EVENT_MASK_READ_TO_GET_SIZE;
                    }
                    else
                    {
                        /*
                        *    Power supplied from USB-C PORT B
                        *    then wait for 500ms
                        */
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Dead_Battery_Mode);
                        TypeCPort2ProcessStep++;
                    }
                }
                break;
            case TPS65988_SET_TIMER_FOR_PORT_B_READY:
                /*
                ********************************************************
                *     Wait for 500ms if power is supplied from USB PDC B port
                *   because it takes more than 400ms after APP read
                *   to be able to read correct status, power status and PD status
                ********************************************************
                */
                TypeCPort2ProcessStep++;
                TypeCPort2IniDelay = TypeC_IntSetting_Delay_time;
                break;
            case TPS65988_INIT_EVENT_MASK_READ_TO_GET_SIZE:
                /*
                **************************************************
                *     Initialize INT_EVENT
                **************************************************
                */



                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_INT_MASK2, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCPort2CommFailCnt++;
                    if(TypeCPort2CommFailCnt == 3)
                    {
                        TypeCPort2IniDelay = 200;
                        //      TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                    TypeCPort2ProcessStep++;
                }
                break;
            case TPS65988_INIT_SEND_EVENT_MASK:
                /*
                *    Initialize event interrupt masks
                */
                SMB3_DATA[0] = 0x38;//BIT5+BIT4+BIT3;
                SMB3_DATA[1] = 0x78;//BIT5+BIT4+BIT3;
                SMB3_DATA[2] = 0x02;//BIT1
                SMB3_DATA[3] = 0x8D;//BIT7+BIT3+BIT2+BIT0;
                SMB3_DATA[4] = 0x00;
                SMB3_DATA[5] = 0x00;
                SMB3_DATA[6] = 0x00;
                SMB3_DATA[7] = 0x0F;//BIT3+BIT2+BIT1+BIT0;
                SMB3_DATA[8] = 0x00;
                SMB3_DATA[9] = 0x00;
                SMB3_DATA[10] = 0x00;
                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_MASK2, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                {
                    TypeCPort2CommFailCnt++;
                    if(TypeCPort2CommFailCnt == 3)
                    {
                        TypeCPort2IniDelay = 200;
                        //      TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }
                else
                {

#if 0
                    // WA for PD rework,WTF +
                    SMB3_DATA[0] = 0x00;//BIT3;
                    SMB3_DATA[1] = 0x00;//BIT5+BIT4+BIT3;
                    SMB3_DATA[2] = 0x00;
                    SMB3_DATA[3] = 0x00;//BIT7+BIT3+BIT2+BIT0;
                    SMB3_DATA[4] = 0x00;
                    SMB3_DATA[5] = 0x00;
                    SMB3_DATA[6] = 0x00;
                    SMB3_DATA[7] = 0x00;//BIT3+BIT2+BIT1+BIT0;
                    SMB3_DATA[8] = 0x00;
                    SMB3_DATA[9] = 0x00;
                    SMB3_DATA[10] = 0x00;

                    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_MASK1, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                    {
                        TypeCPort2CommFailCnt++;
                        if(TypeCPort2CommFailCnt == 3)
                        {
                            TypeCPort2IniDelay = 200;
                            TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                            TypeCPort2CommFailCnt = 0;
                            break;
                        }
                    }
                    // WA for PD rework,WTF -
#endif
                    TypeCPort2CommFailCnt = 0;
                    TypeCPort2ProcessStep++;
                }
                break;
            case TPS65988_INIT_SEND_EVENT_CLEAR:
                /*
                *    Initialize event interrupt masks
                */
#if 0
                // WA for PD rework,WTF +
                SMB3_DATA[0] = 0xFF;
                SMB3_DATA[1] = 0xFF;
                SMB3_DATA[2] = 0xFF;
                SMB3_DATA[3] = 0xFF;
                SMB3_DATA[4] = 0xFF;
                SMB3_DATA[5] = 0xFF;
                SMB3_DATA[6] = 0xFF;
                SMB3_DATA[7] = 0xFF;
                SMB3_DATA[8] = 0xFF;
                SMB3_DATA[9] = 0xFF;
                SMB3_DATA[10] = 0xFF;

                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_CLEAR1, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
                {
                    TypeCPort2CommFailCnt++;
                    if(TypeCPort2CommFailCnt == 3)
                    {
                        TypeCPort2IniDelay = 200;
                        TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }

                //else  if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_CLEAR2, &SMB3_DATA[0], 8, SMBus_NoPEC))
                //{
                //  TypeCPort2CommFailCnt++;
                //      if(TypeCPort2CommFailCnt == 3)
                //      {
                //      TypeCPort2IniDelay = 200;
                //      TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                //      TypeCPort2CommFailCnt = 0;
                //      }
                //}
                else
#endif
                    // WA for PD rework,WTF -
                {
                    TypeCPort2CommFailCnt = 0;
                    TypeCPort2ProcessStep++;
                }
                break;
            case CHECK_POWER_STATUS:
                //Check power status for dead battery
                //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_POWER_STATUS, tmpPDPort1INT,bRead_I2C_NULL);
                if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_POWER_STATUS, SMB3_DATA,bRead_I2C_NULL))
                {
                    TypeCPort2CommFailCnt++;
                    if(TypeCPort2CommFailCnt == 3)
                    {
                        TypeCPort2IniDelay = 200;
                        //    TypeCPort2ProcessStep = TPS65988_ERROR_STATE;
                        TypeCPort2CommFailCnt = 0;
                    }
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                    if(TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA[0]) == TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT)
                    {
                        /*
                            *   Connection is present
                            */
                        if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA[0]))
                        {
                            /*
                                *    USB PD act as Sink
                                */
                            SET_MASK(TypeCPort2Status,TypeCAdpExist);
                            if(TypeCPort2AdpPdStableCheck == 0)
                            {
                                TypeCPort2AdpPdStableCheck = 3;
                            }
                            DetectPort2AdaptorWatt();
                            if(TypeCPort2AdpPdStableCheck == 0)
                            {
                                TypeCPort2ProcessStep++;
                            }
                        }
                        else
                        {
                            TypeCPort2ProcessStep++;
                        }
                    }
                    else
                    {
                        TypeCPort2ProcessStep++;
                    }
                }
                break;

            case CHECK_AND_CLEAR_DEAD_BATTERY_MODE:
//          if(OneOfTwoBattExistGet || (Read_AC_IN()&&IS_MASK_SET(Attach2status,StableLLevel)))
//          {
                Send4ccCmd(TPS65988_4CC_CMD_CLEAR_DEAD_BATTERY_FLAG,TYPECPort2);
                CLEAR_MASK(TypeCPort2Status2,TypeC_Dead_Battery_Mode);
//          }
                TypeCPort2ProcessStep++;
                break;

            case POLLING_IRQ:
                if(TypecPDPort2Process(TRUE))
                {
                    TypeCPort2ProcessStep++;
                }
                break;
            case INITIAL_FINISH:
                //Initial finish
                SET_MASK(TypeCPort2Status,TypeCIniOK);
                TempRomData = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_TYPEC_STATUS);
                if((TempRomData & 0x01) == 0x01)
                {
                    Temp8 = TempRomData & 0xFE; //Clear bit0:Type-C chip exist
                    WRITE_DATA2EEPROM(&Temp8, 1, EEPROM_Bank7, EEPROM_TYPEC_STATUS);
                }
                TypeCPort2ProcessStep = 0;  //Clear for new process flow step restart.
                break;
            case TPS65988_ERROR_STATE:
                //Type-C communiction error
                break;
            default:
                TypeCPort2ProcessStep = 0;  //Clear for new process flow step restart.
                break;
        }
    }
    else
    {
        TypeCPort2IniDelay--;
    }
}

/*
*******************************************************************************
* Function name: TypecPDProcess
*
* Descriptoin: This is invoked when the system is powered on
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

BOOL TypecPDProcess(BOOL verify)
{
    BYTE i,j,k;
    if(Read_TYPEC_INT() || verify)
    {
        //Get Event number
        //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_MASK2, tmpPDPort1INT,bRead_I2C_NULL);
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_INT_EVENT2, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
            if(TypeCCommFailCnt == 3)
            {
                TypeCIniDelay = 200;
                TypeCCommFailCnt = 0;
            }
        }
        else
        {
            TypeCCommFailCnt = 0;
            TypeCEventNum = TPS65988_INT_NO_EVENT_Num;
//          if(TPS65988_INT_VDM_RECEIVED(SMB3_DATA[0]))
//          {
//              SMB3_DATA[0] = 0x00;
//              SMB3_DATA[1] = 0x08;    //Clear event bit
//              SMB3_DATA[2] = 0x00;
//              SMB3_DATA[3] = 0x00;
//              SMB3_DATA[4] = 0x00;
//              SMB3_DATA[5] = 0x00;
//              SMB3_DATA[6] = 0x00;
//              SMB3_DATA[7] = 0x00;
//              TypeCEventNum        = 11;                  //Event number for "INT_VDM_RECEIVED" is 11.
//          }
//          else
            {
                for(i = 0; TPS65988_INT_SIZE > i; i++)
                {
                    if (0 != SMB3_DATA[i])
                    {
                        for(j = 0; 8 > j; j++)
                        {
                            if (0 != (SMB3_DATA[i] & (1 << j)))
                            {
                                /*
                                *    find bit 1 in the event
                                *    then clear it and convert to bit number
                                */
                                SMB3_DATA[0] = 0x00;
                                SMB3_DATA[1] = 0x00;
                                SMB3_DATA[2] = 0x00;
                                SMB3_DATA[3] = 0x00;
                                SMB3_DATA[4] = 0x00;
                                SMB3_DATA[5] = 0x00;
                                SMB3_DATA[6] = 0x00;
                                SMB3_DATA[7] = 0x00;
                                SMB3_DATA[8] = 0x00;
                                SMB3_DATA[9] = 0x00;
                                SMB3_DATA[10] = 0x00;


                                SMB3_DATA[i] = 0x00;
                                SMB3_DATA[i] |= 0x01 << j;
                                TypeCEventNum        = i * 8 + j;
                                break;
                            }
                        }
                    }
                }
            }
            //Clear events
            if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_CLEAR2, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
            {
                TypeCCommFailCnt++;
            }
            else
            {
                TypeCCommFailCnt = 0;
            }
            //Process Event
            switch(TypeCEventNum)
            {
                case TPS65988_INT_PLUG_INSERT_OR_REMOVAL_Num:
                    //A285D00046--->Start
                    //RamDebug(0xC5);
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_CONTROL_CONFIG, SMB3_DATA_TEMP1,bRead_I2C_NULL))
                    {
                        //RamDebug(SMB3_DATA_TEMP1[1]);
                        SMB3_DATA_TEMP1[1] = (SMB3_DATA_TEMP1[1]&0x7F);
                        bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_CONTROL_CONFIG, &SMB3_DATA_TEMP1[0], 2, SMBus_NoPEC);
                    }
                    ////A285D00046--->End
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_PR_SWAP_Num:
                    //RamDebug(0xC0);
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]))
                        {
                            UCSI_POWER_SOURCE = 0;
                            Usb_Pdc_Power_Status.power_source = 0;
                        }
                        else
                        {
                            UCSI_POWER_SOURCE = 1;
                            Usb_Pdc_Power_Status.power_source = 1;
                        }
                    }

                    UCSI_POWER_ROLE_SWAP_COMPLETE = TRUE;
                    UCSI_POWER_DIRECTION_CHANGE = 1; //printf ("("@B1\n");
                    UCSI_CONNECTOR_NUMBER_CHANGED[0] = 0;
                    UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_SWAP_REQUESTED_Num:
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_DR_SWAP_Num:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        //RamDebug(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]));
                        if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                        {
                            UCSI_DATA_ROLE = 1;
                        }
                        else
                        {
                            UCSI_DATA_ROLE = 0;
                        }
                    }
                    UCSI_DATA_ROLE_SWAP_COMPLETE = TRUE;
                    UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    UCSI_CONNECTOR_NUMBER_CHANGED[0] = 0;
                    UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_VDM_RECEIVED_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_NEW_CONTRACT_AS_CONS_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_ACTIVE_CONTRACT_RDO, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        UCSI_RDO1 = SMB3_DATA_TEMP[0];
                        UCSI_RDO2 = SMB3_DATA_TEMP[1];
                        UCSI_RDO3 = SMB3_DATA_TEMP[2];
                        UCSI_RDO4 = SMB3_DATA_TEMP[3];
                        RamDebug(0xE6);
                        //RamDebug(UCSI_RDO1);
                        //RamDebug(UCSI_RDO2);
                        //RamDebug(UCSI_RDO3);
                        //RamDebug(UCSI_RDO4);
                        UCSI_NEG_POWER_LEVEL_CHANGE = 1;
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_NEW_CONTRACT_AS_PROV_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_ACTIVE_CONTRACT_RDO, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        UCSI_RDO1 = SMB3_DATA_TEMP[0];
                        UCSI_RDO2 = SMB3_DATA_TEMP[1];
                        UCSI_RDO3 = SMB3_DATA_TEMP[2];
                        UCSI_RDO4 = SMB3_DATA_TEMP[3];
                        RamDebug(0xE7);
                        //RamDebug(UCSI_RDO1);
                        //RamDebug(UCSI_RDO2);
                        //RamDebug(UCSI_RDO3);
                        //RamDebug(UCSI_RDO4);
                        UCSI_NEG_POWER_LEVEL_CHANGE = 1;
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_SOURCE_CAP_MSG_READY_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_SOURCE_CAPABILITIES, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        for (k =0; k < 29 ; k++)
                        {
                            UCSI_PDOs_Buffer[k] = SMB3_DATA_TEMP[k];
                        }
                        RamDebug(0xE0);
                        UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE = 1;
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_POWER_STATUS_UPDATE_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        PowerSourceSwitchDelay = 20;
                        //if(Read_DCIN_ATTACHED2())
                        //{
                        //  PD_VBUS_CTRL1_DIS_SDV();
                        //}
                        //else
                        //{
                        //  PD_VBUS_CTRL1_EN_SDV();
                        //}
                        TypeCCommFailCnt = 0;
                        if(TPS65988_POWER_STATUS_POWER_CONNECTION(SMB3_DATA_TEMP[0]) == TPS65988_POWER_STATUS_TYPE_C_CONNECTION_CONNECTION)
                        {
                            //connection
                            if(0 != (TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA_TEMP[0]) & TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT))
                            {
                                /*
                                *   Connection is present
                                */
                                if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]))
                                {
                                    /*
                                    *    USB PD act as Sink
                                    */
                                    SET_MASK(TypeCStatus,TypeCAdpExist);
                                    if(UCSI_POWER_SOURCE)
                                    {
                                        //RamDebug(0x8D);
                                        UCSI_POWER_SOURCE = 0;
                                        Usb_Pdc_Power_Status.power_source = 0;
                                        UCSI_POWER_DIRECTION_CHANGE = 1;
                                        UCSI_CONNECTOR_NUMBER_CHANGED[0] = 0;
                                        UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    }
                                    TypeCAdpPdStableCheck = 30;
                                }
                                else
                                {

                                    if(!UCSI_POWER_SOURCE)
                                    {
                                        //RamDebug(0x8E);
                                        UCSI_POWER_SOURCE = 1;
                                        Usb_Pdc_Power_Status.power_source = 1;
                                        UCSI_POWER_DIRECTION_CHANGE = 1;
                                        UCSI_CONNECTOR_NUMBER_CHANGED[0] = 0;
                                        UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    }
                                }
                            }
                            else
                            {
                                //add retry mode
                                TypeCAdpPdStableCheck = 10;//A285D00069+:
                                /*
                                //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                                SinkPowerPathControl(0, TYPECPort1);
                                //set unsupport adapater flag
                                SET_MASK(u4SenseSTS3, UnSupportAdapterPort1);*/
                            }
                        }
                        else
                        {
                            //Dis-connection
                            CLEAR_MASK(TypeCStatus,TypeCAdpExist);
                            CLEAR_MASK(TypeCStatus,TypeCAdpReady);
                            CLEAR_MASK(TypeCStatus2,TypeC_Docking_Attached);
                            //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                            CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort1);
                            UsbPdcGetPowerStatus(0);
                            TypeCPort1AdpID = 0;
                            Port1CurrentTypeCWatt = 0;
                            TypeCAdpPdStableCheck = 0;
                            Port1CurrentTypeCV = 0;
                            Port1CurrentTypeCI = 0;
                            UnknowAdpRetry = 0; //A285D00069+ plug out remove retry count
                            SendPdoSet(7,TYPECPort1);   // Switch PDO to max
                        }
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_DATA_STATUS_UPDATE_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_DATA_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        if(TPS65988_DATA_USB2_CONNECT(SMB3_DATA_TEMP[0]) || TPS65988_DATA_USB3_CONNECT(SMB3_DATA_TEMP[0]))
                        {
                            Usb_Pdc_Status.connect_status = USB_CONNECT_CONNECTED;
                            RamDebug(0xC5);
                        }
                        if(TPS65988_DATA_DP_CONNECTION(SMB3_DATA_TEMP[1]))
                        {
                            Usb_Pdc_Status.connect_status = USB_CONNECT_DP;
                            UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            RamDebug(0xC0);
                        }
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_STATUS_UPDATE_Num:
                    //RamDebug(0xC2);
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP[0]) == 1)
                        {
                            Usb_Pdc_Status.device_connected = TRUE;
                            if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                            {
                                UCSI_DATA_ROLE = 1;
                            }
                            else
                            {
                                UCSI_DATA_ROLE = 0;
                            }
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_DATA_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                            {
                                if(TPS65988_DATA_USB2_CONNECT(SMB3_DATA_TEMP[0]) || TPS65988_DATA_USB3_CONNECT(SMB3_DATA_TEMP[0]))
                                {
                                    Usb_Pdc_Status.connect_status = USB_CONNECT_CONNECTED;
                                    RamDebug(0xC6);
                                }
                                if(TPS65988_DATA_DP_CONNECTION(SMB3_DATA_TEMP[1]))
                                {
                                    Usb_Pdc_Status.connect_status = USB_CONNECT_DP;
                                    UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    RamDebug(0xC1);
                                }
                            }
                        }
                        else
                        {

                            //A285D00068 start+
                            CLEAR_MASK(TypeCStatus,TypeCAdpExist);
                            CLEAR_MASK(TypeCStatus,TypeCAdpReady);
                            CLEAR_MASK(TypeCStatus2,TypeC_Docking_Attached);
                            CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort1);
                            //A285D00068 end+
                            UnknowAdpRetry = 0; //A285D00069 clear retry count
                            Usb_Pdc_Status.device_connected = FALSE;
                            Usb_Pdc_Status.connect_status = USB_CONNECT_NONE;
                            UCSI_RDO1 = 0;
                            UCSI_RDO2 = 0;
                            UCSI_RDO3 = 0;
                            UCSI_RDO4 = 0;
                            for (k =0; k < 32 ; k++ )
                            {
                                UCSI_PDOs_Buffer[k] = 0;
                            }
                            RamDebug(0xE1);
                        }
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_PD_STATUS_UPDATE_Num:
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_CMD1_COMPLETE_Num:
                    //RamDebug(0xC3);
                    UCSI_4CC_CMD_COMPLETE = TRUE;
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_CMD2_COMPLETE_Num:
                    UCSI_4CC_CMD_COMPLETE = TRUE;
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_ENTERED_Num:
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_EXITED_Num:
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_ATTN_VDM_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_USER_VID_ATTENTION_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        //Device's VDM infromation beging from Byte1,Byte0 is PDO length
                        if((SMB3_DATA[4] == 0x17) && (SMB3_DATA[3] == 0xEF))    //Lenovo's USB-IF ID is "17EFh"
                        {
                            OldDockingACKStatus = SMB3_DATA[5];
                            OldDockingEvent = SMB3_DATA[8];
                            OldVDO2[0] = SMB3_DATA[9];
                            OldVDO2[1] = SMB3_DATA[10];
                            OldVDO2[2] = SMB3_DATA[11];
                            OldVDO2[3] = SMB3_DATA[12];
                            if((SMB3_DATA[1]& 0x1F) == 0x10)
                            {
                                if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT)
                                {
                                    // Is WOL/Button action?
                                    if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
                                    {
                                        // Is WOL action?
                                        if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
                                        {
                                            SET_MASK(TypeCStatus2,DockingEvent);
                                            LanWakeLow();
                                        }
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE))
                                    {
                                        // Is Button Make action?
                                        PSWPressed();
                                        // Add for DOCK PWRBTN event// A485D00083
                                        if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                        {
                                            // Is Button Break at the same time?
                                            DockingPSW_ActiveCnt = 11;
                                        }
                                        // Add for DOCK PWRBTN event //A485D00083-
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                    {
                                        // Is Button Break action?
                                        PSWReleased();
                                    }
                                }
                            }

                            if(((SMB3_DATA[1] == 0x44)&&(SMB3_DATA[2] == 0x81)) || (SMB3_DATA[1] == 0x06))  //0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
                            {
                                //Send "Get Lenovo status" package
                                SET_MASK(TypeCStatus2,TypeC_Docking_Attached);
                                SendPowerModeChangeToPdc();
                            }
                        }
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_OTHER_VDM_Num:
                    //  bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_USER_VID_OTHER_VDM, &BiosReserved3E2,bRead_I2C_NULL);//A485D00075-
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_RX_USER_VID_OTHER_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        TypeCCommFailCnt = 0;
                        //Device's VDM infromation beging from Byte1,Byte0 is PDO length
                        if((SMB3_DATA[4] == 0x17) && (SMB3_DATA[3] == 0xEF))    //Lenovo's USB-IF ID is "17EFh"
                        {
                            OldDockingACKStatus = SMB3_DATA[5];
                            OldDockingEvent = SMB3_DATA[8];
                            OldVDO2[0] = SMB3_DATA[9];
                            OldVDO2[1] = SMB3_DATA[10];
                            OldVDO2[2] = SMB3_DATA[11];
                            OldVDO2[3] = SMB3_DATA[12];
                            if((SMB3_DATA[1]& 0x1F) == 0x10)
                            {
                                if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT)
                                {
                                    // Is WOL/Button action?
                                    if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
                                    {
                                        // Is WOL action?
                                        if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
                                        {
                                            SET_MASK(TypeCStatus2,DockingEvent);
                                            LanWakeLow();
                                        }
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE))
                                    {
                                        // Is Button Make action?
                                        PSWPressed();
                                        // CMW20180323 + //A485D00083
                                        if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                        {
                                            // Is Button Break at the same time?
                                            DockingPSW_ActiveCnt = 11;
                                        }
                                        // CMW20180323 -//A485D00083
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                    {
                                        // Is Button Break action?
                                        PSWReleased();
                                    }
                                    SMB3_DATA[7] = SMB3_DATA[8];
                                    SMB3_DATA[8] = 0x00;
                                    SendPowerModeChangeToPdc();
                                }
                            }

                            if(((SMB3_DATA[1] == 0x44)&&(SMB3_DATA[2] == 0x81)) || (SMB3_DATA[1] == 0x06))  //0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
                            {
                                //Send "Get Lenovo status" package
                                SET_MASK(TypeCStatus2,TypeC_Docking_Attached);
                                SendPowerModeChangeToPdc();
                            }
                        }
                    }
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                    break;
                case TPS65988_INT_NO_EVENT_Num:
                    TypeCCommFailCnt = 0;
                    return TRUE;
                default:
                {
                    /*
                    *    Clear other events
                    */
                    //SMB3_DATA[0] = 0xF7;//~0x08;//BIT3;
                    //SMB3_DATA[1] = 0xC7;//~0x38;//BIT5+BIT4+BIT3;
                    //SMB3_DATA[2] = 0xFF;//~0x00;
                    //SMB3_DATA[3] = 0x72;//~0x8D;//BIT7+BIT3+BIT2+BIT0;
                    //SMB3_DATA[4] = 0xFF;//~0x00;
                    //SMB3_DATA[5] = 0xFF;//~0x00;
                    //SMB3_DATA[6] = 0xFF;//~0x00;
                    //SMB3_DATA[7] = 0xF0;//~0x0F;//BIT3+BIT2+BIT1+BIT0;
                    SET_MASK(TypeCStatus,TypeCProcessOK);
                }
                break;
            }
            if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
            {
                //Clear events
//              if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_INT_CLEAR1, &SMB3_DATA[0], 8, SMBus_NoPEC))
//              {
//                  TypeCCommFailCnt++;
//              }
//              else
                {
                    TypeCCommFailCnt = 0;
                    //Process finish
                    CLEAR_MASK(TypeCStatus,TypeCProcessOK);
                }
            }
        }
    }
    return FALSE;
}

/*
*******************************************************************************
* Function name: TypecPDPort2Process
*
* Descriptoin: This is invoked when the system is powered on
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/

BOOL TypecPDPort2Process(BOOL verify)
{
    BYTE i,j,k;
    if(Read_TYPEC_INT() || verify)
    {
        //Get Event number
        //bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_INT_MASK2, tmpPDPort2INT,bRead_I2C_NULL);
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_INT_EVENT2, SMB3_DATA,bRead_I2C_NULL))
        {
            TypeCPort2CommFailCnt++;
            if(TypeCPort2CommFailCnt == 3)
            {
                TypeCPort2IniDelay = 200;
                TypeCPort2CommFailCnt = 0;
            }
        }
        else
        {
            TypeCPort2CommFailCnt = 0;
            TypeCPort2EventNum = TPS65988_INT_NO_EVENT_Num;
//          if(TPS65988_INT_VDM_RECEIVED(SMB3_DATA[0]))
//          {
//              SMB3_DATA[0] = 0x00;
//              SMB3_DATA[1] = 0x08;    //Clear event bit
//              SMB3_DATA[2] = 0x00;
//              SMB3_DATA[3] = 0x00;
//              SMB3_DATA[4] = 0x00;
//              SMB3_DATA[5] = 0x00;
//              SMB3_DATA[6] = 0x00;
//              SMB3_DATA[7] = 0x00;
//              TypeCEventNum        = 11;                  //Event number for "INT_VDM_RECEIVED" is 11.
//          }
//          else
            {
                for(i = 0; TPS65988_INT_SIZE > i; i++)
                {
                    if (0 != SMB3_DATA[i])
                    {
                        for(j = 0; 8 > j; j++)
                        {
                            if (0 != (SMB3_DATA[i] & (1 << j)))
                            {
                                /*
                                *    find bit 1 in the event
                                *    then clear it and convert to bit number
                                */
                                SMB3_DATA[0] = 0x00;
                                SMB3_DATA[1] = 0x00;
                                SMB3_DATA[2] = 0x00;
                                SMB3_DATA[3] = 0x00;
                                SMB3_DATA[4] = 0x00;
                                SMB3_DATA[5] = 0x00;
                                SMB3_DATA[6] = 0x00;
                                SMB3_DATA[7] = 0x00;
                                SMB3_DATA[8] = 0x00;
                                SMB3_DATA[9] = 0x00;
                                SMB3_DATA[10] = 0x00;

                                SMB3_DATA[i] = 0x00;
                                SMB3_DATA[i] |= 0x01 << j;
                                TypeCPort2EventNum        = i * 8 + j;
                                break;
                            }
                        }
                    }
                }
            }
            //Clear events
            if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_CLEAR2, &SMB3_DATA[0], TPS65988_INT_SIZE, SMBus_NoPEC))
            {
                TypeCPort2CommFailCnt++;
            }
            else
            {
                TypeCPort2CommFailCnt = 0;
            }
            //Process Event
            //RamDebug(0x0F);
            //RamDebug(TypeCPort2EventNum);
            switch(TypeCPort2EventNum)
            {
                case TPS65988_INT_PLUG_INSERT_OR_REMOVAL_Num:
                    //A285D00046--->Start
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_CONTROL_CONFIG, SMB3_DATA_TEMP1,bRead_I2C_NULL))
                    {
                        //RamDebug(SMB3_DATA_TEMP1[1]);
                        SMB3_DATA_TEMP1[1] = (SMB3_DATA_TEMP1[1]&0x7F);
                        bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_CONTROL_CONFIG, &SMB3_DATA_TEMP1[0], 2, SMBus_NoPEC);
                    }
                    //A285D00046--->End
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_PR_SWAP_Num:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]))
                        {
                            UCSI_POWER_SOURCE2 = 0;
                        }
                        else
                        {
                            UCSI_POWER_SOURCE2 = 1;
                        }
                    }

                    UCSI_POWER_ROLE_SWAP_COMPLETE = TRUE;
                    UCSI_POWER_DIRECTION_CHANGE = 1; //printf ("("@B1\n");
                    UCSI_CONNECTOR_NUMBER_CHANGED[1] = 0;
                    UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_DR_SWAP_Num:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        //RamDebug(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]));
                        if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                        {
                            UCSI_DATA_ROLE2 = 1;
                        }
                        else
                        {
                            UCSI_DATA_ROLE2 = 0;
                        }
                    }
                    UCSI_DATA_ROLE_SWAP_COMPLETE = TRUE;
                    UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    UCSI_CONNECTOR_NUMBER_CHANGED[1] = 0;
                    UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_CONNECTOR_PARTNER | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_VDM_RECEIVED_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_RX_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_NEW_CONTRACT_AS_CONS_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_ACTIVE_CONTRACT_RDO, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        UCSI_RDO1_CONNECTOR2 = SMB3_DATA_TEMP[0];
                        UCSI_RDO2_CONNECTOR2 = SMB3_DATA_TEMP[1];
                        UCSI_RDO3_CONNECTOR2 = SMB3_DATA_TEMP[2];
                        UCSI_RDO4_CONNECTOR2 = SMB3_DATA_TEMP[3];
                        //RamDebug(0xE4);
                        //RamDebug(UCSI_RDO1_CONNECTOR2);
                        //RamDebug(UCSI_RDO2_CONNECTOR2);
                        //RamDebug(UCSI_RDO3_CONNECTOR2);
                        //RamDebug(UCSI_RDO4_CONNECTOR2);
                        UCSI_NEG_POWER_LEVEL_CHANGE = 1;
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_NEW_CONTRACT_AS_PROV_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_ACTIVE_CONTRACT_RDO, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        UCSI_RDO1_CONNECTOR2 = SMB3_DATA_TEMP[0];
                        UCSI_RDO2_CONNECTOR2 = SMB3_DATA_TEMP[1];
                        UCSI_RDO3_CONNECTOR2 = SMB3_DATA_TEMP[2];
                        UCSI_RDO4_CONNECTOR2 = SMB3_DATA_TEMP[3];
                        //RamDebug(0xE5);
                        //RamDebug(UCSI_RDO1_CONNECTOR2);
                        //RamDebug(UCSI_RDO2_CONNECTOR2);
                        //RamDebug(UCSI_RDO3_CONNECTOR2);
                        //RamDebug(UCSI_RDO4_CONNECTOR2);
                        UCSI_NEG_POWER_LEVEL_CHANGE = 1;
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_SOURCE_CAP_MSG_READY_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_RX_SOURCE_CAPABILITIES, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        for (k =0; k < 29 ; k++)
                        {
                            UCSI_PDOs_Buffer_CONNECTOR2[k] = SMB3_DATA_TEMP[k];
                        }
                        //RamDebug(0xE2);
                        UCSI_SUPPORTED_PROVIDER_CAPABILITIES_CHANGE = 1;
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_POWER_STATUS_UPDATE_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        PowerSourceSwitchDelay = 20;
                        //if(Read_DCIN_ATTACHED2())
                        //{
                        //  PD_VBUS_CTRL1_DIS_SDV();
                        //}
                        //else
                        //{
                        //  PD_VBUS_CTRL1_EN_SDV();
                        //}
                        TypeCPort2CommFailCnt = 0;
                        if(TPS65988_POWER_STATUS_POWER_CONNECTION(SMB3_DATA_TEMP[0]) == TPS65988_POWER_STATUS_TYPE_C_CONNECTION_CONNECTION)
                        {
                            //connection
                            if(0 != (TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA_TEMP[0]) & TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT))
                            {
                                /*
                                    *   Connection is present
                                    */
                                if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]))
                                {
                                    /*
                                        *    USB PD act as Sink
                                        */
                                    SET_MASK(TypeCPort2Status,TypeCAdpExist);
                                    if(UCSI_POWER_SOURCE2)
                                    {
                                        UCSI_POWER_SOURCE2 = 0;
                                        Usb_Pdc_Power_Status2.power_source = 0;
                                        UCSI_POWER_DIRECTION_CHANGE = 1;
                                        UCSI_CONNECTOR_NUMBER_CHANGED[1] = 0;
                                        UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    }
                                    TypeCPort2AdpPdStableCheck = 30;
                                }
                                else
                                {

                                    if(!UCSI_POWER_SOURCE2)
                                    {
                                        UCSI_POWER_SOURCE2 = 1;
                                        Usb_Pdc_Power_Status2.power_source = 1;
                                        UCSI_POWER_DIRECTION_CHANGE = 1;
                                        UCSI_CONNECTOR_NUMBER_CHANGED[1] = 0;
                                        UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    }
                                }
                            }
                            else
                            {
                                //A285D00069+ port2 retry 5times
                                TypeCPort2AdpPdStableCheck = 10;
                                //mark for retry
                                /*
                                //RamDebug(0x83);
                                //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                                SinkPowerPathControl(0, TYPECPort2);
                                //set unsupport adapater flag
                                SET_MASK(u4SenseSTS3, UnSupportAdapterPort2);
                                */
                            }
                        }
                        else
                        {
                            //RamDebug(0x84);
                            //Dis-connection

                            CLEAR_MASK(TypeCPort2Status,TypeCAdpExist);
                            CLEAR_MASK(TypeCPort2Status,TypeCAdpReady);
                            CLEAR_MASK(TypeCPort2Status2,TypeC_Docking_Attached);
                            //A285D00048: Add unsupport adapter process,set flag for bios,close ac in
                            CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort2);
                            UsbPdcGetPowerStatus(1);
                            TypeCPort2AdpID = 0;
                            Port2CurrentTypeCWatt = 0;
                            TypeCPort2AdpPdStableCheck = 0;
                            Port2CurrentTypeCV = 0;
                            Port2CurrentTypeCI = 0;
                            Port2UnknowAdpRetry = 0;    //A285D00069+

                            SendPdoSet(7,TYPECPort2);   // Switch PDO to max
                        }
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_DATA_STATUS_UPDATE_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_DATA_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        if(TPS65988_DATA_USB2_CONNECT(SMB3_DATA_TEMP[0]) || TPS65988_DATA_USB3_CONNECT(SMB3_DATA_TEMP[0]))
                        {
                            Usb_Pdc_Status2.connect_status = USB_CONNECT_CONNECTED;
                            RamDebug(0xC7);
                        }
                        if(TPS65988_DATA_DP_CONNECTION(SMB3_DATA_TEMP[1]))
                        {
                            Usb_Pdc_Status2.connect_status = USB_CONNECT_DP;
                            UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            RamDebug(0xC2);
                        }
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_STATUS_UPDATE_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP[0]) == 1)
                        {
                            Usb_Pdc_Status2.device_connected = TRUE;
                            if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                            {
                                UCSI_DATA_ROLE2 = 1;
                            }
                            else
                            {
                                UCSI_DATA_ROLE2 = 0;
                            }
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_DATA_STATUS, SMB3_DATA_TEMP,bRead_I2C_NULL))
                            {
                                if(TPS65988_DATA_USB2_CONNECT(SMB3_DATA_TEMP[0]) || TPS65988_DATA_USB3_CONNECT(SMB3_DATA_TEMP[0]))
                                {
                                    Usb_Pdc_Status2.connect_status = USB_CONNECT_CONNECTED;
                                    RamDebug(0xC8);
                                }
                                if(TPS65988_DATA_DP_CONNECTION(SMB3_DATA_TEMP[1]))
                                {
                                    Usb_Pdc_Status2.connect_status = USB_CONNECT_DP;
                                    UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_SUPPORT_CAM | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                    RamDebug(0xC3);
                                }
                            }
                        }
                        else
                        {
                            //A285D00068 start+
                            CLEAR_MASK(TypeCPort2Status,TypeCAdpExist);
                            CLEAR_MASK(TypeCPort2Status,TypeCAdpReady);
                            CLEAR_MASK(TypeCPort2Status2,TypeC_Docking_Attached);
                            //A485D000116: Add unsupport adapter process,set flag for bios,close ac in
                            CLEAR_MASK(u4SenseSTS3, UnSupportAdapterPort2);
                            //A285D00068 end+
                            Port2UnknowAdpRetry = 0;    //A285D00069+ 
                            Usb_Pdc_Status2.device_connected = FALSE;
                            Usb_Pdc_Status2.connect_status = USB_CONNECT_NONE;
                            UCSI_RDO1_CONNECTOR2 = 0;
                            UCSI_RDO2_CONNECTOR2 = 0;
                            UCSI_RDO3_CONNECTOR2 = 0;
                            UCSI_RDO4_CONNECTOR2 = 0;
                            for (k =0; k < 32 ; k++ )
                            {
                                UCSI_PDOs_Buffer_CONNECTOR2[k] = 0;
                            }
                            //RamDebug(0xE3);
                        }
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_PD_STATUS_UPDATE_Num:
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_CMD2_COMPLETE_Num:
                    UCSI_4CC_CMD_COMPLETE = TRUE;
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_ENTERED_Num:
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_EXITED_Num:
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_ATTN_VDM_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_RX_USER_VID_ATTENTION_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        //Device's VDM infromation beging from Byte1,Byte0 is PDO length
                        if((SMB3_DATA[4] == 0x17) && (SMB3_DATA[3] == 0xEF))    //Lenovo's USB-IF ID is "17EFh"
                        {
                            OldDockingACKStatus = SMB3_DATA[5];
                            OldDockingEvent = SMB3_DATA[8];
                            OldVDO2[0] = SMB3_DATA[9];
                            OldVDO2[1] = SMB3_DATA[10];
                            OldVDO2[2] = SMB3_DATA[11];
                            OldVDO2[3] = SMB3_DATA[12];
                            if((SMB3_DATA[1]& 0x1F) == 0x10)
                            {
                                if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT)
                                {
                                    // Is WOL/Button action?
                                    if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
                                    {
                                        // Is WOL action?
                                        if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
                                        {
                                            SET_MASK(TypeCPort2Status2,DockingEvent);
                                            LanWakeLow();
                                        }
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE))
                                    {
                                        // Is Button Make action?
                                        PSWPressed();
                                        // CMW20180323 + //A485D00083
                                        if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                        {
                                            // Is Button Break at the same time?
                                            DockingPSW_ActiveCnt = 11;
                                        }
                                        // CMW20180323 - //A485D00083
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                    {
                                        // Is Button Break action?
                                        PSWReleased();
                                    }
                                }
                            }

                            if(((SMB3_DATA[1] == 0x44)&&(SMB3_DATA[2] == 0x81)) || (SMB3_DATA[1] == 0x06))  //0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
                            {
                                //Send "Get Lenovo status" package
                                RamDebug(0xA2);
                                SET_MASK(TypeCPort2Status2,TypeC_Docking_Attached);
                                SendPowerModeChangeToPdc();
                            }
                        }
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_USER_VID_ALT_MODE_OTHER_VDM_Num:
                    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_02_Addr, TPS65988_RX_USER_VID_OTHER_VDM, SMB3_DATA,bRead_I2C_NULL))
                    {
                        TypeCPort2CommFailCnt++;
                    }
                    else
                    {
                        TypeCPort2CommFailCnt = 0;
                        //Device's VDM infromation beging from Byte1,Byte0 is PDO length
                        if((SMB3_DATA[4] == 0x17) && (SMB3_DATA[3] == 0xEF))    //Lenovo's USB-IF ID is "17EFh"
                        {
                            OldDockingACKStatus = SMB3_DATA[5];
                            OldDockingEvent = SMB3_DATA[8];
                            OldVDO2[0] = SMB3_DATA[9];
                            OldVDO2[1] = SMB3_DATA[10];
                            OldVDO2[2] = SMB3_DATA[11];
                            OldVDO2[3] = SMB3_DATA[12];
                            if((SMB3_DATA[1]& 0x1F) == 0x10)
                            {
                                if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT)
                                {
                                    // Is WOL/Button action?
                                    if(SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_WOL)
                                    {
                                        // Is WOL action?
                                        if(IS_MASK_SET(u4Cfgspace2,DockingWOLEn))
                                        {
                                            SET_MASK(TypeCPort2Status2,DockingEvent);
                                            LanWakeLow();
                                        }
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_MAKE))
                                    {
                                        // Is Button Make action?
                                        PSWPressed();
                                        // CMW20180323 + //A485D00083
                                        if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                        {
                                            // Is Button Break at the same time?
                                            DockingPSW_ActiveCnt = 11;
                                        }
                                        // CMW20180323 - //A485D00082
                                    }
                                    else if((SMB3_DATA[8] & TYPE_C_DOCKING_EVENT_BUTTON_BREAK))
                                    {
                                        // Is Button Break action?
                                        PSWReleased();
                                    }
                                }
                            }

                            if(((SMB3_DATA[1] == 0x44)&&(SMB3_DATA[2] == 0x81)) || (SMB3_DATA[1] == 0x06))  //0x44 for User VID Alt mode enter;0x06 for Lenovo Attention VDM
                            {
                                //Send "Get Lenovo status" package
                                SET_MASK(TypeCPort2Status2,TypeC_Docking_Attached);
                                SendPowerModeChangeToPdc();
                            }
                        }
                    }
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                    break;
                case TPS65988_INT_NO_EVENT_Num:
                    TypeCPort2CommFailCnt = 0;
                    return TRUE;
                default:
                {
                    /*
                    *    Clear other events
                    */
                    //SMB3_DATA[0] = 0xF7;//~0x08;//BIT3;
                    //SMB3_DATA[1] = 0xC7;//~0x38;//BIT5+BIT4+BIT3;
                    //SMB3_DATA[2] = 0xFF;//~0x00;
                    //SMB3_DATA[3] = 0x72;//~0x8D;//BIT7+BIT3+BIT2+BIT0;
                    //SMB3_DATA[4] = 0xFF;//~0x00;
                    //SMB3_DATA[5] = 0xFF;//~0x00;
                    //SMB3_DATA[6] = 0xFF;//~0x00;
                    //SMB3_DATA[7] = 0xF0;//~0x0F;//BIT3+BIT2+BIT1+BIT0;
                    SET_MASK(TypeCPort2Status,TypeCProcessOK);
                }
                break;
            }
            if(IS_MASK_SET(TypeCPort2Status,TypeCProcessOK))
            {
                //Clear events
                //              if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_INT_CLEAR1, &SMB3_DATA[0], 8, SMBus_NoPEC))
                //              {
                //                  TypeCCommFailCnt++;
                //              }
                //              else
                {
                    TypeCPort2CommFailCnt = 0;
                    //Process finish
                    CLEAR_MASK(TypeCPort2Status,TypeCProcessOK);
                }
            }
        }
    }
    return FALSE;
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
void OEM_TYPE_C_init_hook(void)
{
    if(Read_EC_ON())
    {
//  if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TypeC_01_Addr, TPS65988_SYSTEM_CONFIG, SMB3_DATA))
//  {
//      TypeCCommFailCnt++;
//  }
//  else
//  {
//    TypeCCommFailCnt = 0;
//  }
        PowerSourceSwitchDelay = 20;
        //if(Read_DCIN_ATTACHED2())
        //{
        //  PD_VBUS_CTRL1_DIS_SDV();
        //}
        //else
        //{
        //  PD_VBUS_CTRL1_EN_SDV();
        //}
        //if(Read_DCIN_ATTACHED2())
        //{
        //    SET_MASK(TypeCStatus,OldDcinAttached2);
        //}
        //else
        //{
        //    CLEAR_MASK(TypeCStatus,OldDcinAttached2);
        //}
        CLEAR_MASK(TypeCStatus2,TypeC_ChipOff);
        CLEAR_MASK(TypeCPort2Status2,TypeC_ChipOff);
    }
    else
    {
        TypeCIniDelay = TypeC_Ini_Delay_time;//TypeC_Ini_Delay_time;    //Delay for type C stable
        TypeCStatus = 0;
        TypeCCommFailCnt = 0;
        TypeCPort1AdpID = 0;
        Port1CurrentTypeCV = 0;
        Port1CurrentTypeCI = 0;
        Port1CurrentTypeCWatt = 0;
        UnknowAdpRetry = 0; //A285D00069+
        SET_MASK(TypeCStatus2,TypeC_ChipOff);
        PowerSourceSwitchDelay = 20;//11;

        TypeCPort2IniDelay = TypeC_Ini_Delay_time;//TypeC_Ini_Delay_time;   //Delay for type C stable
        TypeCPort2Status = 0;
        TypeCPort2CommFailCnt = 0;
        TypeCPort2AdpID = 0;
        Port2CurrentTypeCV = 0;
        Port2CurrentTypeCI = 0;
        Port2CurrentTypeCWatt = 0;
        Port2UnknowAdpRetry = 0;    //A285D00069+
        SET_MASK(TypeCPort2Status2,TypeC_ChipOff);

        TypeCAdpID = 0;
        CurrentTypeCWatt = 0;
        CurrentTypeCI = 0;
        CurrentTypeCV = 0;
        //PowerSourceSwitchDelay = 20;//11;

        //init battery capacitive read-out for adapter check-20180413
        TPS65988_UcsiRelativeInit();
    }
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_HOOK
*
* Descriptoin: TYPE-C hook for Hook_Timer10msEventA
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_HOOK(void)
{
    //if( bITETestVar5 == 0x00)  //David- add for code debug
    // CMW20180323 +  //A485D00083
    if(!Read_PM_PWRBTN())
    {
        //Work-around for quick press and release docking power button
        if(DockingPSW_ActiveCnt)
        {
            if(DockingPSW_ActiveCnt == 1)
            {
                PSWReleased();
            }
            DockingPSW_ActiveCnt--;
        }
    }
    else
    {
        DockingPSW_ActiveCnt = 0;
    }
    // CMW20180323 - //A485D00083

    if(IS_MASK_CLEAR(PDStatus,TypeCFwUpdating))
    {
        if(IS_MASK_SET(TypeCStatus2,TypeC_ChipOff))
        {
            OEM_TYPE_C_init_hook();
        }
        if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
        {
            TypecPDProcess(FALSE);
            DetectAdaptorWatt();
            //P2P_Process();
        }
        else
        {
            InitialTypecPD();
        }
        if(IS_MASK_SET(TypeCPort2Status,TypeCIniOK))
        {
            TypecPDPort2Process(FALSE);
            DetectPort2AdaptorWatt();
            // P2P_Process_Port2();
        }
        else
        {
            InitialTypecPDPort2();
        }
        if(PowerSourceSwitchDelay)
        {
            if(PowerSourceSwitchDelay==1)
            {
                //  PowerSourceSwitch(); // swith port 1 or port 2
            }
            PowerSourceSwitchDelay--;
        }
    }

}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_S0_HOOK
*
* Descriptoin: TYPE-C hook for S0
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S0_HOOK(void)
{
    //add port switch to 20v for cs18 dock cannot support 9v-20180418
    PDOPortSwitchToNormal();
}


/*
*******************************************************************************
* Function name: OEM_TYPE_C_S3_HOOK
*
* Descriptoin: TYPE-C hook for S3
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S3_HOOK(void)
{
    CLEAR_MASK(PDStatus,TypeCFwUpdating);
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_S3_S0_HOOK
*
* Descriptoin: TYPE-C hook for S3 to S0
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S3_S0_HOOK(void)
{
    SendPowerModeChangeToPdc();
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_S0_S3_HOOK
*
* Descriptoin: TYPE-C hook for S0 to S3
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S0_S3_HOOK(void)
{
    SendPowerModeChangeToPdc();
}
/*
*******************************************************************************
* Function name: OEM_TYPE_C_S5_HOOK
*
* Descriptoin: TYPE-C hook for S5
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S5_HOOK(void)
{
    CLEAR_MASK(PDStatus,TypeCFwUpdating);
    if(IS_MASK_SET(TypeCStatus,TypeCAdpExist)&&IS_MASK_SET(TypeCStatus,TypeCAdpReady))
    {
        do
        {
            if(!IS_MASK_SET(SYS_STATUS,AC_ADP))
            {
                break;
            }

            if(!IS_MASK_SET(TypeCStatus,TypeCIniOK))
            {
                break;
            }

            if((CurrentRDO != ObjectPosition09V))
            {

                if(IS_MASK_SET(PriBattSTS,BattExist)&&IS_MASK_CLEAR(BattStatusL,FullyChg))
                {
                    break;
                }

                if(IS_MASK_SET(SecBattSTS,BattExist)&&IS_MASK_CLEAR(SecBattStatusL,FullyChg))
                {
                    break;
                }

                if((IS_MASK_SET(PriBattSTS,BattExist))||(IS_MASK_SET(SecBattSTS,BattExist))) //A485D000102+  //Fix AC only cannot boot up
                {
                    //20V->9V
                    RamDebug(0x99);
                    SendPDO20VTo9V(TYPECPort1);
                    break;
                }
            }

            if(CurrentRDO != ObjectPosition20V)
            {
                if(IS_MASK_SET(PriBattSTS,BattExist)&&IS_MASK_CLEAR(BattStatusL,FullyChg))
                {
                    //9v->20V
                    RamDebug(0xAB);
                    SendPDO9VTo20V(TYPECPort1);
                    //reset battery
                    TypeCAdpPdStableCheck = 5;
                    TypeCAdpDetectDelay = 5;
                    break;
                }

                if(IS_MASK_SET(SecBattSTS,BattExist)&&IS_MASK_CLEAR(SecBattStatusL,FullyChg))
                {
                    //9v->20V
                    RamDebug(0xBA);
                    SendPDO9VTo20V(TYPECPort1);
                    //reset battery
                    TypeCAdpPdStableCheck = 5;
                    TypeCAdpDetectDelay = 5;
                    break;
                }
            }
        }
        while(0);
#if 0
        //if((CurrentRDO != ObjectPosition09V) && IS_MASK_SET(BattStatusL,FullyChg))
        if((CurrentRDO != ObjectPosition09V)&&(IS_MASK_SET(SYS_STATUS,AC_ADP))
          )
        {
            //SendPdoSet(ObjectPosition5V,TYPECPort1);  // Switch PDO to 5V
            if((IS_MASK_SET(PriBattSTS,BattExist)&&IS_MASK_CLEAR(BattStatusL,FullyChg)) ||
               (IS_MASK_SET(SecBattSTS,BattExist)&&IS_MASK_CLEAR(SecBattStatusL,FullyChg))
              )
            {
                return;
            }
            else
            {
                RamDebug(0x99);
                SendPDO20VTo9V(TYPECPort1);
            }
        }
        //else ((CurrentRDO!= ObjectPosition20V) &&(IS_MASK_CLEAR(SecBattStatusL ,FullyChg) || IS_MASK_CLEAR(BattStatusL,FullyChg)))
        //{
        //  //SendPdoSet(ObjectPosition20V,TYPECPort1); // Switch PDO to 20V
        //  SendPDO9VTo20V();
        //}
#endif
    }
    if(IS_MASK_SET(TypeCPort2Status,TypeCAdpExist)&&IS_MASK_SET(TypeCPort2Status,TypeCAdpReady))
    {
        do
        {
            if(!IS_MASK_SET(SYS_STATUS,AC_ADP))
            {
                break;
            }

            if(!IS_MASK_SET(TypeCStatus,TypeCIniOK))
            {
                break;
            }

            if((Port2CurrentRDO != Port2ObjectPosition09V))
            {

                if(IS_MASK_SET(PriBattSTS,BattExist)&&IS_MASK_CLEAR(BattStatusL,FullyChg))
                {
                    break;
                }

                if(IS_MASK_SET(SecBattSTS,BattExist)&&IS_MASK_CLEAR(SecBattStatusL,FullyChg))
                {
                    break;
                }

                if((IS_MASK_SET(PriBattSTS,BattExist))||(IS_MASK_SET(SecBattSTS,BattExist))) //A485D000102+  //Fix AC only cannot boot up
                {
                    //20V->9V
                    RamDebug(0x99);
                    SendPDO20VTo9V(TYPECPort2);
                    break;
                }
            }

            if(Port2CurrentRDO != Port2ObjectPosition20V)
            {
                if(IS_MASK_SET(PriBattSTS,BattExist)&&IS_MASK_CLEAR(BattStatusL,FullyChg))
                {
                    //9V->20V
                    RamDebug(0xAA);
                    SendPDO9VTo20V(TYPECPort2);
                    //reRead battery watt
                    TypeCPort2AdpPdStableCheck = 5;
                    TypeCPort2AdpDetectDelay = 5;
                    break;
                }

                if(IS_MASK_SET(SecBattSTS,BattExist)&&IS_MASK_CLEAR(SecBattStatusL,FullyChg))
                {
                    //9V->20V
                    RamDebug(0xBB);
                    SendPDO9VTo20V(TYPECPort2);
                    //reRead battery watt
                    TypeCPort2AdpPdStableCheck = 5;
                    TypeCPort2AdpDetectDelay = 5;
                    break;
                }
            }
        }
        while(0);

    }
#if 0
    if(IS_MASK_SET(TypeCPort2Status,TypeCAdpExist)&&IS_MASK_SET(TypeCPort2Status,TypeCAdpReady))
    {
        if((Port2CurrentRDO != ObjectPosition5V) && IS_MASK_SET(BattStatusL,FullyChg))
        {
            SendPDO20VTo9V();
            //SendPdoSet(ObjectPosition5V,TYPECPort2);  // Switch PDO to 5V
        }
        else if((Port2CurrentRDO != Port2ObjectPosition20V) && IS_MASK_CLEAR(BattStatusL,FullyChg))
        {
            SendPDO9VTo20V();
            //SendPdoSet(Port2ObjectPosition20V,TYPECPort2);    // Switch PDO to 20V
        }
    }
#endif
}

/*
*******************************************************************************
* Function name: OEM_TYPE_C_S5_S0_HOOK
*
* Descriptoin: TYPE-C hook for S5 to S0
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S5_S0_HOOK(void)
{
    //A285D00065: Add PD control for i2c retry--ti spec start
    Send4ccCmd(TPS65988_4CC_CMD_RETRY_I2C, TYPECPort1);
    //Send4ccCmd(TPS65988_4CC_CMD_RETRY_I2C, TYPECPort2);   //A285D00071-
    //A285D00065: Add PD control for i2c retry--ti spec end
    SendPowerModeChangeToPdc();
    PDOPortSwitchToNormal();
}
/*
*******************************************************************************
* Function name: OEM_TYPE_C_S0_S5_HOOK
*
* Descriptoin: TYPE-C hook for S0 to S5
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void OEM_TYPE_C_S0_S5_HOOK(void)
{
    SendPowerModeChangeToPdc();
}
/*
*******************************************************************************
* Function name: PowerSourceSwitch
*
* Descriptoin: Switch power source from Classic adaptor or Type-C adaptor
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void PowerSourceSwitch(void)
{
    return;
    //if(Read_DCIN_ATTACHED2())
    //{ //Turn off Type-C VBus power
    //SET_MASK(TypeCStatus,OldDcinAttached2);
    //PD_VBUS_CTRL1_DIS_SDV();
//      SMB3_DATA[3] &= 0x8F;   //Clear bit28~30, PP_EXT is disabled
    //      SMB3_DATA[0] = 'S';
    //      SMB3_DATA[1] = 'R';
    //      SMB3_DATA[2] = 'Y';
    //      SMB3_DATA[3] = 'R';
    //}
    //else
    {
        //Turn on Type-C VBus power
        //CLEAR_MASK(TypeCStatus,OldDcinAttached2);
        //PD_VBUS_CTRL1_EN_SDV();
        SMB3_DATA[0] = 0x02;
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_DATA1, &SMB3_DATA[0], 01, SMBus_NoPEC))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;
        }
//      SMB3_DATA[3] &= 0x8F;   //Clear bit28~30
//      SMB3_DATA[3] |= 0x30;   //Set bit28~30 to 011b, PP_EXT as input but wait for SYS_RDY
        SMB3_DATA[0] = 'S';
        SMB3_DATA[1] = 'R';
        SMB3_DATA[2] = 'D';
        SMB3_DATA[3] = 'Y';
    }
    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_CMD1, &SMB3_DATA[0], 04, SMBus_NoPEC))
    {
        TypeCCommFailCnt++;
    }
    else
    {
        TypeCCommFailCnt = 0;
    }
}

/*
*******************************************************************************
* Function name: Oem_Hook_Type_C_BeforeSleep
*
* Descriptoin:
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void Oem_Hook_Type_C_BeforeSleep(void)
{

    //SET_MASK(TypeCStatus2,TypeC_ChipOff);

    OEM_TYPE_C_init_hook();   //A485D00051
}

/*
*******************************************************************************
* Function name: Send4ccCmd
*
* Descriptoin: Send 4cc command to PDC
*
* Arguments:
*       CmdNum:4cc command number
* Return Values:
*       none
*******************************************************************************
*/
void Send4ccCmd(BYTE CmdNum,BYTE portID)
{
    BYTE tmpTYPECAddr,tmpTYPCCmd;
    unsigned char * tmpCommFailCnt;
    BYTE retryTimes = 0;    //A285D00071+

    if(IS_MASK_SET(PDStatus,TypeCFwUpdating))
    {
        return;
    }

    if(portID == TYPECPort1) // for typec port 1
    {
        tmpTYPECAddr = TypeC_01_Addr;
        tmpCommFailCnt = &TypeCCommFailCnt;
        tmpTYPCCmd = TPS65988_CMD1; //A485D00051
        //RamDebug(0x06);
        //RamDebug(CmdNum);
    }
    else if(portID == TYPECPort2) // for typec port 2
    {
        tmpTYPECAddr = TypeC_02_Addr;
        tmpCommFailCnt = &TypeCPort2CommFailCnt;
        tmpTYPCCmd = TPS65988_CMD2;  //A485D00051
        //RamDebug(0x07);
        //RamDebug(CmdNum);
    }

    //A285D00075 start+
    if(IS_MASK_SET(PDStatus, TypeC4CCCmd))
    {
        Delay1MS(5 * PD4ccCMD_WaitCnt);
    }
    //A285D00075 end+

    switch(CmdNum)
    {
        case TPS65988_4CC_CMD_CLEAR_DEAD_BATTERY_FLAG :
            SMB3_DATA[0] = 'D';
            SMB3_DATA[1] = 'B';
            SMB3_DATA[2] = 'f';
            SMB3_DATA[3] = 'g';
            break;
        case TPS65988_4CC_CMD_TURN_ON_INPUT_PATH :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'R';
            SMB3_DATA[2] = 'D';
            SMB3_DATA[3] = 'Y';
            break;
        case TPS65988_4CC_CMD_TURN_OFF_INPUT_PATH :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'R';
            SMB3_DATA[2] = 'Y';
            SMB3_DATA[3] = 'R';
            break;
        case TPS65988_4CC_CMD_COLD_BOOT :
            SMB3_DATA[0] = 'G';
            SMB3_DATA[1] = 'A';
            SMB3_DATA[2] = 'I';
            SMB3_DATA[3] = 'D';
            break;
        case TPS65988_4CC_CMD_SOFT_RESET :
            SMB3_DATA[0] = 'G';
            SMB3_DATA[1] = 'a';
            SMB3_DATA[2] = 'i';
            SMB3_DATA[3] = 'd';
            break;
        case TPS65988_4CC_CMD_SWAP_TO_SINK :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'W';
            SMB3_DATA[2] = 'S';
            SMB3_DATA[3] = 'k';
            break;
        case TPS65988_4CC_CMD_SWAP_TO_SOURCE :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'W';
            SMB3_DATA[2] = 'S';
            SMB3_DATA[3] = 'r';
            break;
        case TPS65988_4CC_CMD_SWAP_TO_DFP :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'W';
            SMB3_DATA[2] = 'D';
            SMB3_DATA[3] = 'F';
            break;
        case TPS65988_4CC_CMD_SWAP_TO_UFP :
            SMB3_DATA[0] = 'S';
            SMB3_DATA[1] = 'W';
            SMB3_DATA[2] = 'U';
            SMB3_DATA[3] = 'F';
            break;
        case TPS65988_4CC_CMD_SEND_VDM :
            SMB3_DATA[0] = 'V';
            SMB3_DATA[1] = 'D';
            SMB3_DATA[2] = 'M';
            SMB3_DATA[3] = 's';
            break;
        case TPS65988_4CC_CMD_AUTO_NEGOTIATE_SINK :
            SMB3_DATA[0] = 'A';
            SMB3_DATA[1] = 'N';
            SMB3_DATA[2] = 'e';
            SMB3_DATA[3] = 'g';
            break;
        case TPS65988_4CC_CMD_HARDWARE_RESET :   //A485D00080+
            SMB3_DATA[0] = 'H';
            SMB3_DATA[1] = 'R';
            SMB3_DATA[2] = 'S';
            SMB3_DATA[3] = 'T';
            break;
        case TPS65988_4CC_CMD_GSRC :
            SMB3_DATA[0] = 'G';
            SMB3_DATA[1] = 'S';
            SMB3_DATA[2] = 'r';
            SMB3_DATA[3] = 'C';
            break;
        //A285D00065: Add PD control for i2c retry--ti spec start
        case TPS65988_4CC_CMD_RETRY_I2C:
            SMB3_DATA[0] = 'M';
            SMB3_DATA[1] = 'u';
            SMB3_DATA[2] = 'x';
            SMB3_DATA[3] = 'R';
            break;
        //A285D00065: Add PD control for i2c retry--ti spec end
        default :
            break;
    }
    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, tmpTYPECAddr, tmpTYPCCmd, &SMB3_DATA[0], 04, SMBus_NoPEC))
    {
        tmpCommFailCnt++;
    }
    else
    {
        tmpCommFailCnt = 0;
    }

    //A285D00071 start+ --A285D00075 move
    do
    {
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpTYPECAddr, tmpTYPCCmd, &SMB3_DATA[0], bRead_I2C_NULL))
        {
            Delay1MS(5);
            RamDebug(0xE5);
        }
        else if((0x00 == SMB3_DATA[0]) && (0x00 == SMB3_DATA[1]) && (0x00 == SMB3_DATA[2]) && (0x00 == SMB3_DATA[3]))
        {
            RamDebug(0xDA);
            RamDebug(0xAD);
            break;
        }
    }while(++retryTimes < 20);

    if(retryTimes >= 20)
    {
        RamDebug(0xED);
        RamDebug(SMB3_DATA[0]);
        RamDebug(SMB3_DATA[1]);
        RamDebug(SMB3_DATA[2]);
        RamDebug(SMB3_DATA[3]);
        RamDebug(0xED);
    }
    else
    {
        //A285D00075 start+
        SET_MASK(PDStatus, TypeC4CCCmd);
        PD4ccCMD_WaitCnt = 4;
        //A285D00075 end+
    }
    //A285D00071 end+

}



/*
*******************************************************************************
* Function name: SendPowerModeChangeToPdc
*
* Descriptoin: Send power mode change to PDC
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void SendPowerModeChangeToPdc(void)
{
    BYTE i;
    if((IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)&&IS_MASK_CLEAR(TypeCPort2Status,TypeCIniOK))
       || (IS_MASK_CLEAR(TypeCStatus2,TypeC_Docking_Attached)&&IS_MASK_CLEAR(TypeCPort2Status2,TypeC_Docking_Attached))
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
      )
    {
        return;
    }
    for(i=0; i<16; i++)
    {
        SMB3_DATA[i+1] = 0;
    }
    SMB3_DATA[5] = OldDockingACKStatus;
    SMB3_DATA[5] &= 0xFE;   //Clear Docking ACK flag
    SMB3_DATA[8] = OldDockingEvent;
    SMB3_DATA[7] = OldDockingEvent;
    SMB3_DATA[7] |= 0x01;   //Set System ACK flag
    SMB3_DATA[9] = OldVDO2[0];
    SMB3_DATA[10] = OldVDO2[1];
    SMB3_DATA[11] = OldVDO2[2];
    SMB3_DATA[12] = OldVDO2[3];

    //Header
    SMB3_DATA[1] = 0x10;
    SMB3_DATA[2] = 0xA0;
    SMB3_DATA[3] = 0xEF;
    SMB3_DATA[4] = 0x17;

    switch (SysPowState)
    {
        //Set power status
        case SYSTEM_S4 :
        case SYSTEM_S5 :
            SMB3_DATA[6] |= (0x05 << 1);
            break;
        case SYSTEM_S3 :
            SMB3_DATA[6] |= (0x03 << 1);
            break;
        case SYSTEM_S0 :
            SMB3_DATA[6] |= (0x01 << 1);
            break;
        default :
            break;
    }
    if(ACPOWER_ON)
    {
        //Set power mode
        SMB3_DATA[6] |= (0x01 << 4);
    }
    SMB3_DATA[6] |= 0x01;   //Set power status change
    SMB3_DATA[0] = 3;   //PDO number is 3
    //A485D00051: Enable Docking wake on LAN---Start
    if( IS_MASK_SET(TypeCStatus2,TypeC_Docking_Attached) && IS_MASK_SET(TypeCStatus,TypeCIniOK) )
    {
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_DATA1, &SMB3_DATA[0], 13, SMBus_NoPEC))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;
        }
        Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort1);
    }
    if( IS_MASK_SET(TypeCPort2Status2,TypeC_Docking_Attached) && IS_MASK_SET(TypeCPort2Status,TypeCIniOK) )
    {
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_DATA2, &SMB3_DATA[0], 13, SMBus_NoPEC))
        {
            TypeCPort2CommFailCnt++;
        }
        else
        {
            TypeCPort2CommFailCnt = 0;
        }
        Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort2);
    }
}
//A485D00051: Enable Docking wake on LAN--->End
/*
*******************************************************************************
* Function name: SendPdoSet
*
* Descriptoin: Send PDO select to PDC
*
* Arguments:
*       PDOSetNum: 01:5V,02:9V,03:15V,04:20V
* Return Values:
*       none
*******************************************************************************
*/
//A485D00051: Enable Docking wake on LAN---Start
void SendPdoSet(BYTE PDOSetNum,BYTE PortNum)
{
    if(IS_MASK_SET(PDStatus,TypeCFwUpdating))
    {
        return;
    }
    SMB3_DATA[0] = PDOSetNum;
    if( PortNum == TYPECPort1 )
    {
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], 1, SMBus_NoPEC))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;
            CurrentRDO = PDOSetNum;
        }
        Send4ccCmd(TPS65988_4CC_CMD_AUTO_NEGOTIATE_SINK,TYPECPort1);
    }
    else
    {
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], 1, SMBus_NoPEC))
        {
            TypeCPort2CommFailCnt++;
        }
        else
        {
            TypeCPort2CommFailCnt = 0;
            Port2CurrentRDO = PDOSetNum;
        }
        Send4ccCmd(TPS65988_4CC_CMD_AUTO_NEGOTIATE_SINK,TYPECPort2);
    }
////A485D00051: Enable Docking wake on LAN--->End
}
/*
*******************************************************************************
* Function name: P2P_Process
*
* Descriptoin: P to P function process
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
#if 0
void P2P_Process(void)
{
    BYTE i,UnitARSOCTemp,UnitASxTemp;
    WORD RemainCap1,RemainCap2;
    WORD FullChgCap1,FullChgCap2;
    DWORD RelativeStateOfChg;
    if(IS_MASK_SET(TypeCStatus3,NeedUpdateToPartner)&&(TypeCProcessStep==0))
    {
        SendUnitInformationToPdc();
        return;
    }
    if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
       || IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Attached)
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
       || IS_MASK_SET(TypeCStatus3,AllEventsNotClear)
      )
    {
        return;
    }
    if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    RemainCap1 = ((RemainCapH << 8) + RemainCapL);
    RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
    FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
    FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
    RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
    UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;

    if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Type))
    {
        //This unit is Sink side
        UCSI_P2P_Charge_Disable = 0;
        if(IS_MASK_SET(TypeCStatus3,GotPartnerInformation))
        {
            UnitARSOCTemp = AnoutherUnitInformation[0] & 0x7F;
            UnitASxTemp = ((AnoutherUnitInformation[1]&0x03) << 1) + (AnoutherUnitInformation[0] >> 7);
            if(((i == 1) && (UnitASxTemp == 0))
               || ((i != 1) && (UnitASxTemp != 0))
              )
            {
                //Two units both are in Non-S0 or S0.
                if(UnitARSOCTemp >= UCSI_RelativeStateOfChg)
                {
                    if(UnitARSOCTemp > 30)
                    {
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        if((UnitARSOCTemp - UCSI_RelativeStateOfChg) < 20)
                        {
                            CLEAR_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        }
                        else
                        {
                            SET_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        }
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }

                }
                else
                {
                    if(UCSI_RelativeStateOfChg > 30)
                    {
                        if((UCSI_RelativeStateOfChg - UnitARSOCTemp) < 20)
                        {
                            CLEAR_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                            CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        }
                        else
                        {
                            SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        }
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }
                }
            }
            else
            {
                //Two units' system status is different

                if(UnitASxTemp != 0)
                {
                    //Partner unit in non-S0
                    if((UCSI_RelativeStateOfChg > 30) && (UnitARSOCTemp < 10))
                    {
                        SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        SET_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        if(UCSI_P2P_Charge_Disable_SINK == 1)
                            CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
                    }
                }
                else
                {
                    //This unit in non-S0

                    if(UCSI_RelativeStateOfChg > 30)
                    {
                        SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
                        SET_MASK(TypeCStatus3,TypeC_Laptop_Power_Charge_Target);
                        if(UCSI_P2P_Charge_Disable_SINK == 1)
                            CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
                    }
                }
            }

            if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type_Target) != IS_MASK_SET(TypeCStatus3,TypeC_Laptop_Power_Type))
            {
                //Send PD command to swap the power type.
                if(IS_MASK_SET(TypeCStatus2,TypeC_Laptop_Power_Type_Target))
                {
                    if(IS_MASK_CLEAR(TypeCStatus3,P2PPowerSwapWait))
                    {
                        RamDebug(0xB0);
                        if(IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Power_Charge_Status))
                        {
                            RamDebug(0xB3);
                            SinkPowerPathControl(1,TYPECPort1);
                        }
                        Send4ccCmd(TPS65988_4CC_CMD_SWAP_TO_SOURCE,TYPECPort1);
                        SET_MASK(TypeCStatus3,P2PPowerSwapWait);

//                          //SendUnitInformationToPdc();
                    }
                }
            }
            else
            {
                //RTS5455WaitPingDelay=20;
                CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
                if(IS_MASK_CLEAR(TypeCStatus2,TypeC_Laptop_Power_Charge_Status) != IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
                {
                    if(IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Power_Charge_Target))
                    {
                        //Disable changing
                        RamDebug(0xB1);
                        SinkPowerPathControl(0,TYPECPort1);
                        UCSI_P2P_Charge_Disable_SINK = 1;

                    }
                    else
                    {
                        //Enable changing
                        RamDebug(0xB2);
                        SinkPowerPathControl(1,TYPECPort1);
                        UCSI_P2P_Charge_Disable_SINK = 0;
                    }
                }
            }
        }
    }
    else
    {
        //This unit is Source side
        //RTS5455WaitPingDelay=20;
        CLEAR_MASK(TypeCStatus3,P2PPowerSwapWait);
        SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Type_Target);
        CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
        if((UCSI_RelativeStateOfChg != (OldInformation&0x7F))||((OldInformation>>7) != i))
        {
            SendUnitInformationToPdc();
        }
        if(UCSI_RelativeStateOfChg < 30)
        {
            if(UCSI_P2P_Charge_Disable == 0)
            {
                RamDebug(0xBF);
                SinkPowerPathControl(0,TYPECPort1);
                UCSI_P2P_Charge_Disable = 1;
            }
        }
        else
        {
            if(UCSI_P2P_Charge_Disable == 1)
            {
                RamDebug(0xFB);
                SinkPowerPathControl(1,TYPECPort1);
                UCSI_P2P_Charge_Disable = 0;
            }
        }
    }
}

void P2P_Process_Port2(void)
{
    BYTE i,UnitARSOCTemp,UnitASxTemp;
    WORD RemainCap1,RemainCap2;
    WORD FullChgCap1,FullChgCap2;
    DWORD RelativeStateOfChg;
    if(IS_MASK_SET(TypeCPort2Status3,NeedUpdateToPartner)&&(TypeCPort2ProcessStep==0))
    {
        SendUnitInformationToPdc_Port2();
        return;
    }
    if(IS_MASK_CLEAR(TypeCPort2Status,TypeCIniOK)
       || IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Attached)
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
       || IS_MASK_SET(TypeCPort2Status3,AllEventsNotClear)
      )
    {
        return;
    }
    if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    RemainCap1 = ((RemainCapH << 8) + RemainCapL);
    RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
    FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
    FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
    RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
    UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;

    if(IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Power_Type))
    {
        //This unit is Sink side
        UCSI_P2P_Charge_Disable = 0;
        if(IS_MASK_SET(TypeCPort2Status3,GotPartnerInformation))
        {
            UnitARSOCTemp = AnoutherUnitInformation2[0] & 0x7F;
            UnitASxTemp = ((AnoutherUnitInformation2[1]&0x03) << 1) + (AnoutherUnitInformation2[0] >> 7);
            if(((i == 1) && (UnitASxTemp == 0))
               || ((i != 1) && (UnitASxTemp != 0))
              )
            {
                //Two units both are in Non-S0 or S0.
                if(UnitARSOCTemp >= UCSI_RelativeStateOfChg)
                {
                    if(UnitARSOCTemp > 30)
                    {
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                        if((UnitARSOCTemp - UCSI_RelativeStateOfChg) < 20)
                        {
                            CLEAR_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        }
                        else
                        {
                            SET_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        }
                    }
                    else
                    {
                        CLEAR_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                    }

                }
                else
                {
                    if(UCSI_RelativeStateOfChg > 30)
                    {
                        if((UCSI_RelativeStateOfChg - UnitARSOCTemp) < 20)
                        {
                            CLEAR_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                            SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
                        }
                        else
                        {
                            SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                        }
                    }
                    else
                    {
                        CLEAR_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                    }

                }
            }
            else
            {
                //Two units' system status is different
                if(UnitASxTemp != 0)
                {
                    //Partner unit in non-S0
                    if((UCSI_RelativeStateOfChg > 30) && (UnitARSOCTemp < 10))
                    {
                        SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                        SET_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        if(UCSI_P2P_Charge_Disable_SINK == 1)
                            CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
                    }
                }
                else
                {
                    //This unit in non-S0

                    if(UCSI_RelativeStateOfChg > 30)
                    {
                        SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                    }
                    else
                    {
                        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
                        SET_MASK(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target);
                        if(UCSI_P2P_Charge_Disable_SINK == 1)
                            CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
                    }
                }
            }

            if(IS_MASK_SET(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target) != IS_MASK_SET(TypeCPort2Status3,TypeC_Laptop_Power_Type))
            {
                //Send PD command to swap the power type.
                if(IS_MASK_SET(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target))
                {
                    if(IS_MASK_CLEAR(TypeCPort2Status3,P2PPowerSwapWait))
                    {
                        if(IS_MASK_CLEAR(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status))
                        {
                            SinkPowerPathControl(1,TYPECPort2);
                        }
                        Send4ccCmd(TPS65988_4CC_CMD_SWAP_TO_SOURCE,TYPECPort2);
                        SET_MASK(TypeCPort2Status3,P2PPowerSwapWait);

//                          //SendUnitInformationToPdc();
                    }
                }
            }
            else
            {
                //RTS5455WaitPingDelay=20;
                CLEAR_MASK(TypeCPort2Status3,P2PPowerSwapWait);
                if(IS_MASK_CLEAR(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status) != IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target))
                {
                    if(IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Power_Charge_Target))
                    {
                        //Disable changing
                        SinkPowerPathControl(0,TYPECPort2);
                        UCSI_P2P_Charge_Disable_SINK = 1;
                    }
                    else
                    {
                        //Enable changing
                        SinkPowerPathControl(1,TYPECPort2);
                        UCSI_P2P_Charge_Disable_SINK = 0;
                    }
                }
            }
        }
    }
    else
    {
        //This unit is Source side
        //RTS5455WaitPingDelay=20;
        CLEAR_MASK(TypeCPort2Status3,P2PPowerSwapWait);
        SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Type_Target);
        CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
        if((UCSI_RelativeStateOfChg != (OldInformation2&0x7F))||((OldInformation2>>7) != i))
        {
            SendUnitInformationToPdc_Port2();
        }
        if(UCSI_RelativeStateOfChg < 30)
        {
            if(UCSI_P2P_Charge_Disable == 0)
            {
                RamDebug(0xAF);
                SinkPowerPathControl(0,TYPECPort2);
                UCSI_P2P_Charge_Disable = 1;
            }
        }
        else
        {
            if(UCSI_P2P_Charge_Disable == 1)
            {
                RamDebug(0xFA);
                SinkPowerPathControl(1,TYPECPort2);
                UCSI_P2P_Charge_Disable = 0;
            }
        }
    }
}


/*
*******************************************************************************
* Function name: SendUnitInformationToPdc
*
* Descriptoin: Send unit information to another unit for P2P function
*
* Arguments:
*       none
* Return Values:
*       none
*******************************************************************************
*/
void SendUnitInformationToPdc(void)
{
    BYTE i;
    WORD RemainCap1,RemainCap2;
    WORD FullChgCap1,FullChgCap2;
    DWORD RelativeStateOfChg;
    if(IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)
       || IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Attached)
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
      )
    {
        return;
    }
    for(i=0; i<9; i++)
    {
        SMB3_DATA[i+1] = 0;
    }

    //Header
    SMB3_DATA[1] = 0x12;
    SMB3_DATA[2] = 0xA0;
    SMB3_DATA[3] = 0xEF;
    SMB3_DATA[4] = 0x17;

    //Set RSOC
    RemainCap1 = ((RemainCapH << 8) + RemainCapL);
    RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
    FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
    FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
    RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
    UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;
    //RamDebug(0xBB);
    //RamDebug(UCSI_RelativeStateOfChg);
    OldInformation = UCSI_RelativeStateOfChg;
    if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
    {
        OldInformation |= 0x80;
    }
    SMB3_DATA[5] = UCSI_RelativeStateOfChg;

    switch (SysPowState)
    {
        //Set power status
        case SYSTEM_S4 :
        case SYSTEM_S0_S4 :
        case SYSTEM_S5 :
        case SYSTEM_S0_S5 :
            SMB3_DATA[5] |= (0x05 << 7);
            SMB3_DATA[6] |= (0x05 >> 1);
            break;
        case SYSTEM_S3 :
        case SYSTEM_S0_S3 :
            SMB3_DATA[5] |= (0x03 << 7);
            SMB3_DATA[6] |= (0x03 >> 1);
            break;
        case SYSTEM_S0 :
        case SYSTEM_S3_S0 :
        case SYSTEM_S4_S0 :
        case SYSTEM_S5_S0 :
            //SMB3_DATA[5] |= (0x00 << 7);
            //SMB3_DATA[6] |= (0x00 >> 1);
            break;
        default :
            break;
    }
    if(ACPOWER_ON)
    {
        //Set power mode
        SMB3_DATA[6] |= (0x01 << 2);
    }

    SMB3_DATA[0] = 2;   //PDO number is 2
    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_DATA1, &SMB3_DATA[0], 9, SMBus_NoPEC))
    {
        TypeCCommFailCnt++;
    }
    else
    {
        TypeCCommFailCnt = 0;
    }
    Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort1);
    CLEAR_MASK(TypeCStatus3,NeedUpdateToPartner);
}
void SendUnitInformationToPdc_Port2(void)
{
    BYTE i;
    WORD RemainCap1,RemainCap2;
    WORD FullChgCap1,FullChgCap2;
    DWORD RelativeStateOfChg;
    if(IS_MASK_CLEAR(TypeCPort2Status,TypeCIniOK)
       || IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Attached)
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
      )
    {
        return;
    }
    for(i=0; i<9; i++)
    {
        SMB3_DATA[i+1] = 0;
    }

    //Header
    SMB3_DATA[1] = 0x12;
    SMB3_DATA[2] = 0xA0;
    SMB3_DATA[3] = 0xEF;
    SMB3_DATA[4] = 0x17;

    //Set RSOC
    RemainCap1 = ((RemainCapH << 8) + RemainCapL);
    RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
    FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
    FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
    RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
    UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;
    //RamDebug(0xBC);
    //RamDebug(UCSI_RelativeStateOfChg);
    OldInformation2 = UCSI_RelativeStateOfChg;
    if((SysPowState == SYSTEM_S0)||(SysPowState == SYSTEM_S5_S0))
    {
        OldInformation2 |= 0x80;
    }
    SMB3_DATA[5] = UCSI_RelativeStateOfChg;

    switch (SysPowState)
    {
        //Set power status
        case SYSTEM_S4 :
        case SYSTEM_S0_S4 :
        case SYSTEM_S5 :
        case SYSTEM_S0_S5 :
            SMB3_DATA[5] |= (0x05 << 7);
            SMB3_DATA[6] |= (0x05 >> 1);
            break;
        case SYSTEM_S3 :
        case SYSTEM_S0_S3 :
            SMB3_DATA[5] |= (0x03 << 7);
            SMB3_DATA[6] |= (0x03 >> 1);
            break;
        case SYSTEM_S0 :
        case SYSTEM_S3_S0 :
        case SYSTEM_S4_S0 :
        case SYSTEM_S5_S0 :
            //SMB3_DATA[5] |= (0x00 << 7);
            //SMB3_DATA[6] |= (0x00 >> 1);
            break;
        default :
            break;
    }
    if(ACPOWER_ON)
    {
        //Set power mode
        SMB3_DATA[6] |= (0x01 << 2);
    }

    SMB3_DATA[0] = 2;   //PDO number is 2
    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_DATA2, &SMB3_DATA[0], 9, SMBus_NoPEC))
    {
        TypeCPort2CommFailCnt++;
    }
    else
    {
        TypeCPort2CommFailCnt = 0;
    }
    Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort2);
    CLEAR_MASK(TypeCPort2Status3,NeedUpdateToPartner);
}
#endif

/*
*******************************************************************************
* Function name: SinkPowerPathControl
*
* Descriptoin: Enable/Disable sink side power path from source side or sink side
*
* Arguments:
*       OnOff:1:Enable,0:Disable
* Return Values:
*       none
*******************************************************************************
*/

//A285D00048: Add unsupport adapter process,set flag for bios,close ac in
void SinkPowerPathControl(BYTE OnOff,BYTE portID)
{
    BYTE i;
    if((IS_MASK_CLEAR(TypeCStatus,TypeCIniOK)&&IS_MASK_CLEAR(TypeCPort2Status,TypeCIniOK))
       //|| (IS_MASK_CLEAR(TypeCStatus3,TypeC_Laptop_Attached)&&IS_MASK_CLEAR(TypeCPort2Status3,TypeC_Laptop_Attached))
       || IS_MASK_SET(PDStatus,TypeCFwUpdating)
      )
    {
        return;
    }
    if(portID == TYPECPort1)
    {
        if(IS_MASK_SET(TypeCStatus3,TypeC_Laptop_Power_Type))
        {
            //Send VDM command to enable/disable from source side to sink
            for(i=0; i<9; i++)
            {
                SMB3_DATA[i+1] = 0;
            }

            //Header
            SMB3_DATA[1] = 0x13;
            SMB3_DATA[2] = 0xA0;
            SMB3_DATA[3] = 0xEF;
            SMB3_DATA[4] = 0x17;

            if(OnOff)
            {
                SMB3_DATA[5] |= 0x01;
            }

            SMB3_DATA[0] = 2;   //PDO number is 2
            if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_DATA1, &SMB3_DATA[0], 9, SMBus_NoPEC))
            {
                TypeCCommFailCnt++;
            }
            else
            {
                TypeCCommFailCnt = 0;
            }
            Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort1);
            //      SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
            //      SMB3_DATA[1] = RTS5455_PORT_NUM;
            //      if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 10, SMBus_NoPEC))
            //      {
            //          TypeCCommFailCnt++;
            //      }
            //      else
            //      {
            //          TypeCCommFailCnt = 0;
            //        if(OnOff)
            //        {
            //              SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            //        }
            //        else
            //        {
            //              CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            //        }
            //      }
        }
        else
        {
            //Send PD command to enable/disable from sink side
            if(OnOff)
            {
                //Turn on Type-C VBus power
                SMB3_DATA[0] = 0x02;
                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_01_Addr, TPS65988_DATA1, &SMB3_DATA[0], 01, SMBus_NoPEC))
                {
                    TypeCCommFailCnt++;
                }
                else
                {
                    TypeCCommFailCnt = 0;
                }
                Send4ccCmd(TPS65988_4CC_CMD_TURN_ON_INPUT_PATH,TYPECPort1);
                SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            }
            else
            {
                //Turn off Type-C VBus power
                Send4ccCmd(TPS65988_4CC_CMD_TURN_OFF_INPUT_PATH,TYPECPort1);
                CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            }
            //      SMB3_DATA[0] = RTS5455_ENABLE_DISABLE_SINK & 0x00FF;
            //      SMB3_DATA[1] = RTS5455_PORT_NUM;
            //          SMB3_DATA[2] = 0;
            //    if(OnOff)
            //    {
            //          SMB3_DATA[2] |= 0x01;
            //    }
            //      if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ENABLE_DISABLE_SINK>>8), &SMB3_DATA[0], RTS5455_ENABLE_DISABLE_SINK_LEN, SMBus_NoPEC))
            //      {
            //          TypeCCommFailCnt++;
            //      }
            //      else
            //      {
            //          TypeCCommFailCnt = 0;
            //      }
        }
    }
    else if(portID == TYPECPort2)
    {
        //RamDebug(0xD2);
        //RamDebug(OnOff);
        //RamDebug(0xD2);
        if(IS_MASK_SET(TypeCPort2Status3,TypeC_Laptop_Power_Type))
        {
            //Send VDM command to enable/disable from source side to sink
            for(i=0; i<9; i++)
            {
                SMB3_DATA[i+1] = 0;
            }

            //Header
            SMB3_DATA[1] = 0x13;
            SMB3_DATA[2] = 0xA0;
            SMB3_DATA[3] = 0xEF;
            SMB3_DATA[4] = 0x17;

            if(OnOff)
            {
                SMB3_DATA[5] |= 0x01;
            }

            SMB3_DATA[0] = 2;   //PDO number is 2
            if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_DATA2, &SMB3_DATA[0], 9, SMBus_NoPEC))
            {
                TypeCPort2CommFailCnt++;
            }
            else
            {
                TypeCPort2CommFailCnt = 0;
            }
            Send4ccCmd(TPS65988_4CC_CMD_SEND_VDM,TYPECPort2);
            //      SMB3_DATA[0] = RTS5455_SEND_VDM & 0x00FF;
            //      SMB3_DATA[1] = RTS5455_PORT_NUM;
            //      if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_SEND_VDM>>8), &SMB3_DATA[0], 10, SMBus_NoPEC))
            //      {
            //          TypeCCommFailCnt++;
            //      }
            //      else
            //      {
            //          TypeCCommFailCnt = 0;
            //        if(OnOff)
            //        {
            //              SET_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            //        }
            //        else
            //        {
            //              CLEAR_MASK(TypeCStatus2,TypeC_Laptop_Power_Charge_Status);
            //        }
            //      }
        }
        else
        {
            //RamDebug(0x66);
            //RamDebug(OnOff);
            //RamDebug(0x66);
            //Send PD command to enable/disable from sink side
            if(OnOff)
            {
                //Turn on Type-C VBus power
                SMB3_DATA[0] = 0x02;
                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_02_Addr, TPS65988_DATA2, &SMB3_DATA[0], 01, SMBus_NoPEC))
                {
                    TypeCPort2CommFailCnt++;
                }
                else
                {
                    TypeCPort2CommFailCnt = 0;
                }
                Send4ccCmd(TPS65988_4CC_CMD_TURN_ON_INPUT_PATH,TYPECPort2);
                SET_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
            }
            else
            {
                //Turn off Type-C VBus power
                Send4ccCmd(TPS65988_4CC_CMD_TURN_OFF_INPUT_PATH,TYPECPort2);
                CLEAR_MASK(TypeCPort2Status2,TypeC_Laptop_Power_Charge_Status);
            }
            //      SMB3_DATA[0] = RTS5455_ENABLE_DISABLE_SINK & 0x00FF;
            //      SMB3_DATA[1] = RTS5455_PORT_NUM;
            //          SMB3_DATA[2] = 0;
            //    if(OnOff)
            //    {
            //          SMB3_DATA[2] |= 0x01;
            //    }
            //      if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TypeC_Addr, (RTS5455_ENABLE_DISABLE_SINK>>8), &SMB3_DATA[0], RTS5455_ENABLE_DISABLE_SINK_LEN, SMBus_NoPEC))
            //      {
            //          TypeCCommFailCnt++;
            //      }
            //      else
            //      {
            //          TypeCCommFailCnt = 0;
            //      }
        }
    }
}

/*
*******************************************************************************
* Function name: UsbPqcRequest
*
* Descriptoin:
*          This is used to enqueue the request block into USB PD queue
*
* Arguments:
*       int         usb_pdc_id
*       USB_PDC_REQ *request_packet
*       UCHAR       command
*       USHORT      parm1
*       USHORT      parm2
*       void        *address
*       USB_PD_CALL_BACK *call_back
* Return Values:
*       UCHAR : return code of USB PD
*******************************************************************************
*/
BYTE UsbPdcRequest(BYTE usb_pdc_id, USB_PDC_REQ*request_packet, BYTE cmd, WORD parm1, WORD parm2, void *address)
{
//    INT_MASK_REG int_mask;
//    USB_PDC_REQ  *p;
    BYTE        rc = USB_PDC_RC_OK;
    BYTE        i;//,j;
    BYTE TempAddr,tmpTYPCCmd,TempTypeData,TempValue;
    if(usb_pdc_id > 0)
    {
        TempAddr = TypeC_02_Addr;
        tmpTYPCCmd = TPS65988_CMD2;
    }
    else
    {
        TempAddr = TypeC_01_Addr;
        tmpTYPCCmd = TPS65988_CMD1;
    }
#if DEBUG_UCSI
    RamDebug(0xA1);
    RamDebug(usb_pdc_id);
#endif

    if ((0 <= usb_pdc_id) && (NUMBER_OF_USB_PDC > usb_pdc_id))
    {
        /*
        *     valid USB PD ID
        */
//    int_mask = CmnGetIntMask();
//    CmnDisableInt();
#if DEBUG_UCSI
        RamDebug(0xA2);
#endif
//        if (NULL == Usb_Cb[usb_pdc_id].p_request) {
//            /*
//            *   There is no request
//            */
//            Usb_Cb[usb_pdc_id].p_request = request_packet;
//        } else {
//            for (p = Usb_Cb[usb_pdc_id].p_request;; p = p->next) {
//                if (NULL == p->next) {
//                    /*
//                    *    reach last request block then add the request
//                    */
//                    p->next = request_packet;
//                    break;
//                }  else if (request_packet == p) {
//                    /*
//                    *   the request block is already in the queue then reqject
//                    */
//                    rc = USB_PDC_RC_BUSY;
//                    break;
//                }
//            }
//        }
        if ((TRUE == request_packet->status.busy)
            && (request_packet->status.done == FALSE))
        {
            /*
            *   PDC is busy
            */
            rc = USB_PDC_RC_BUSY;
#if DEBUG_UCSI
            RamDebug(0xA3);
#endif
        }
        if (USB_PDC_RC_OK == rc)
        {
#if DEBUG_UCSI
            RamDebug(0xA4);
            RamDebug(cmd);
#endif
            /*
            *   request packet is enqueued then set up the packet
            */
            //request_packet->status.done  = FALSE;
            CLEAR_MASK(TypeCStatus2,PdcRequestDone);
            request_packet->status.busy  = TRUE;
            //RamDebug(0x0A);
            //RamDebug(cmd);
            switch (cmd)
            {
                case USB_PDC_REQ_HARD_RESET:
                    SMB3_DATA[0] = 'G';
                    SMB3_DATA[1] = 'A';
                    SMB3_DATA[2] = 'I';
                    SMB3_DATA[3] = 'D';
                    if(UCSI_CONNECTOR_RESET_WAIT == 1)
                        request_packet->status.done  = TRUE;
                    else
                    {
                        UCSI_CONNECTOR_RESET_WAIT = 1;
                        UCSI_CONNECTOR_RESET_WAIT_CNT = 500;    //10ms X 500 = 5s
                    }
                    break;
                case USB_PDC_REQ_SOFT_RESET:
                    SMB3_DATA[0] = 'G';
                    SMB3_DATA[1] = 'a';
                    SMB3_DATA[2] = 'i';
                    SMB3_DATA[3] = 'd';
                    if(UCSI_CONNECTOR_RESET_WAIT == 1)
                        request_packet->status.done  = TRUE;
                    else
                    {
                        UCSI_CONNECTOR_RESET_WAIT = 1;
                        UCSI_CONNECTOR_RESET_WAIT_CNT = 200;    //10ms X 200 =2s
                    }
                    break;
                case USB_PDC_REQ_SWAP_TO_SINK:
                    if(usb_pdc_id == 0)
                    {
                        if(Usb_Pdc_Power_Status.power_source)
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'S';
                            SMB3_DATA[3] = 'k';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                        else
                        {

                            UCSI_POWER_SOURCE = FALSE;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                    }
                    else
                    {
                        if(Usb_Pdc_Power_Status2.power_source)
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'S';
                            SMB3_DATA[3] = 'k';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                        else
                        {

                            UCSI_POWER_SOURCE2 = FALSE;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                    }
                    break;
                case USB_PDC_REQ_SWAP_TO_SOURCE:
                    if(usb_pdc_id == 0)
                    {
                        if(Usb_Pdc_Power_Status.power_source)
                        {
                            UCSI_POWER_SOURCE = TRUE;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                        else
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'S';
                            SMB3_DATA[3] = 'r';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                    }
                    else
                    {
                        if(Usb_Pdc_Power_Status2.power_source)
                        {
                            UCSI_POWER_SOURCE2 = TRUE;
                            //UCSI_POWER_DIRECTION_CHANGE = 1;
                            UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                        else
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'S';
                            SMB3_DATA[3] = 'r';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                    }
                    break;
                case USB_PDC_REQ_SWAP_TO_UFP:
                    if(usb_pdc_id == 0)
                    {
                        if(Usb_Pdc_Status.dfp)
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'U';
                            SMB3_DATA[3] = 'F';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                        else
                        {
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            UCSI_DATA_ROLE = 0;
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                    }
                    else
                    {
                        if(Usb_Pdc_Status2.dfp)
                        {
                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'U';
                            SMB3_DATA[3] = 'F';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                        else
                        {
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            UCSI_DATA_ROLE2 = 0;
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                    }
                    break;
                case USB_PDC_REQ_SWAP_TO_DFP:
                    if(usb_pdc_id == 0)
                    {
                        if(Usb_Pdc_Status.dfp)
                        {
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            UCSI_DATA_ROLE = 1;
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                        else
                        {

                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'D';
                            SMB3_DATA[3] = 'F';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                    }
                    else
                    {
                        if(Usb_Pdc_Status2.dfp)
                        {
                            //UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                            UCSI_DATA_ROLE2 = 1;
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                        }
                        else
                        {

                            SMB3_DATA[0] = 'S';
                            SMB3_DATA[1] = 'W';
                            SMB3_DATA[2] = 'D';
                            SMB3_DATA[3] = 'F';
                            if(UCSI_4CC_CMD_RETRY == TRUE)
                            {
                                request_packet->status.done = FALSE;
                            }
                        }
                    }
                    break;
                case USB_PDC_REQ_GET_RX_SOURCE_PDO:
                    SMB3_DATA[0] = 'G';
                    SMB3_DATA[1] = 'S';
                    SMB3_DATA[2] = 'r';
                    SMB3_DATA[3] = 'C';
                    // Isaac Pdc_Data = RETRIEVE_SOURCE_CAPABILITIES;
                    break;
                case USB_PDC_REQ_GET_RX_SINK_PDO:
                    SMB3_DATA[0] = 'G';
                    SMB3_DATA[1] = 'S';
                    SMB3_DATA[2] = 'k';
                    SMB3_DATA[3] = 'C';
                    // Isaac Pdc_Data = RETRIEVE_SINK_CAPABILITIES;
                    break;
                case USB_PDC_REQ_GET_TX_SOURCE_PDO:
                    // Isaac Pdc_Data = READ_SRC_PDO;
#if DEBUG_UCSI
                    RamDebug(0xFE);
#endif
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TX_SOURCE_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                    {
#if DEBUG_UCSI
                        RamDebug(SMB3_DATA[0]);
                        RamDebug(0xEF);
#endif
                        /*request_packet->pdos_valid      = SMB3_DATA[0];
                        for(i = 0;(i < request_packet->pdos_valid)&&(i < 4); i++)
                        {
                            Ucsi_message_in[0 + 4*i] = SMB3_DATA[3 + 4*i];
                            Ucsi_message_in[1 + 4*i] = SMB3_DATA[4 + 4*i];
                            Ucsi_message_in[2 + 4*i] = SMB3_DATA[5 + 4*i];
                            Ucsi_message_in[3 + 4*i] = SMB3_DATA[6 + 4*i];
                            RamDebug(Ucsi_message_in[0 + 4*i]);
                            RamDebug(Ucsi_message_in[1 + 4*i]);
                            RamDebug(Ucsi_message_in[2 + 4*i]);
                            RamDebug(Ucsi_message_in[3 + 4*i]);
                         }*/
                        if((SMB3_DATA[3] == 0) && (SMB3_DATA[4] == 0)
                           && (SMB3_DATA[5] == 0) && (SMB3_DATA[6] == 0))
                        {
                            Ucsi_message_in[0] = 0;
                            Ucsi_message_in[1] = 0;
                            Ucsi_message_in[2] = 0;
                            Ucsi_message_in[3] = 0;
                            Ucsi_cci[1] = 0x00;
                        }
                        else
                        {
                            if(UCSI_GET_PDOS_COMMAND_PDO_OFFSET < SMB3_DATA[0])
                            {
                                Ucsi_message_in[0] = SMB3_DATA[3 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[1] = SMB3_DATA[4 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[2] = SMB3_DATA[5 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[3] = SMB3_DATA[6 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_cci[1] = 0x04;
                            }
                            else
                            {
                                Ucsi_message_in[0] = 0;
                                Ucsi_message_in[1] = 0;
                                Ucsi_message_in[2] = 0;
                                Ucsi_message_in[3] = 0;
                                Ucsi_cci[1] = 0x00;
                            }
                        }
                    }
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_GET_TX_SINK_PDO:
                    // Isaac Pdc_Data = READ_SINK_PDO;
#if DEBUG_UCSI
                    RamDebug(0xFD);
#endif
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TX_SINK_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                    {
#if DEBUG_UCSI
                        RamDebug(SMB3_DATA[0]);
                        RamDebug(0xDF);
#endif
                        /*  request_packet->pdos_valid      = SMB3_DATA[0];
                          for(i = 0;(i < request_packet->pdos_valid)&&(i < 4); i++)
                          {
                              Ucsi_message_in[0 + 4*i] = SMB3_DATA[1 + 4*i];
                              Ucsi_message_in[1 + 4*i] = SMB3_DATA[2 + 4*i];
                              Ucsi_message_in[2 + 4*i] = SMB3_DATA[3 + 4*i];
                              Ucsi_message_in[3 + 4*i] = SMB3_DATA[4 + 4*i];
                              RamDebug(Ucsi_message_in[0 + 4*i]);
                              RamDebug(Ucsi_message_in[1 + 4*i]);
                              RamDebug(Ucsi_message_in[2 + 4*i]);
                              RamDebug(Ucsi_message_in[3 + 4*i]);
                           }*/
                        if((SMB3_DATA[1] == 0) && (SMB3_DATA[2] == 0)
                           && (SMB3_DATA[3] == 0) && (SMB3_DATA[4] == 0))
                        {
                            Ucsi_message_in[0] = 0;
                            Ucsi_message_in[1] = 0;
                            Ucsi_message_in[2] = 0;
                            Ucsi_message_in[3] = 0;
                            Ucsi_cci[1] = 0x00;
                        }
                        else
                        {
                            if(UCSI_GET_PDOS_COMMAND_PDO_OFFSET < SMB3_DATA[0])
                            {
                                Ucsi_message_in[0] = SMB3_DATA[1 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[1] = SMB3_DATA[2 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[2] = SMB3_DATA[3 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_message_in[3] = SMB3_DATA[4 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                Ucsi_cci[1] = 0x04;
                            }
                            else
                            {
                                Ucsi_message_in[0] = 0;
                                Ucsi_message_in[1] = 0;
                                Ucsi_message_in[2] = 0;
                                Ucsi_message_in[3] = 0;
                                Ucsi_cci[1] = 0x00;
                            }
                        }
                    }
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_OPERATE_AS_DFP:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_CONTROL_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                    {
                        SMB3_DATA[1] |= TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_DFP;
                    }
                    //if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TPS65988_CONTROL_CONFIG, &SMB3_DATA[0], 2, SMBus_NoPEC))
                    //{
                    //    TypeCCommFailCnt++;
                    //}
                    //else
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_OPERATE_AS_UFP:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_CONTROL_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                    {
                        SMB3_DATA[1] |= TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_DFP;
                    }
                    //if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TPS65988_CONTROL_CONFIG, &SMB3_DATA[0], 2, SMBus_NoPEC))
                    //{
                    //    TypeCCommFailCnt++;
                    //}
                    //else
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_OPERATE_AS_SOURCE:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_CONTROL_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                    {
                        SMB3_DATA[0] |= TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SOURCE;
                    }
                    //if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TPS65988_CONTROL_CONFIG, &SMB3_DATA[0], 1, SMBus_NoPEC))
                    //{
                    //    TypeCCommFailCnt++;
                    //}
                    //else
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_OPERATE_AS_SINK:
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_CONTROL_CONFIG, SMB3_DATA,bRead_I2C_NULL))
                    {
                        SMB3_DATA[0] |= TPS65988_CONTROL_CONFIG_INITIATE_SWAP_TO_SINK;
                    }
                    //if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TPS65988_CONTROL_CONFIG, &SMB3_DATA[0], 1, SMBus_NoPEC))
                    //{
                    //    TypeCCommFailCnt++;
                    //}
                    //else
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_GET_ALTERNATE_MODES:
                    //RamDebug(0xF1);

                    if(TempAddr == TypeC_01_Addr)
                    {
                        TempTypeData = TPS65988_DATA1;
                    }
                    else
                    {
                        TempTypeData = TPS65988_DATA2;
                    }
                    TempValue = UCSI_GET_ALTERNATE_MODES_COMMAND_RECIPIENT;
                    //RamDebug(TempValue);
                    if(TempValue == 0)
                    {
						i = 0;
						if(UCSI_GET_ALTERNATE_MODES_COMMAND_ALTERNATE_MODE_OFFSET == 0)
						{
		            		Ucsi_message_in[0] = 0xEF;
							Ucsi_message_in[1] = 0x17;
							Ucsi_message_in[2] = 0x01;
							Ucsi_message_in[3] = 0x00;
							Ucsi_message_in[4] = 0x00;
							Ucsi_message_in[5] = 0x00;
							Ucsi_cci[1] = 0x06;
						}
						else if(UCSI_GET_ALTERNATE_MODES_COMMAND_ALTERNATE_MODE_OFFSET == 1)
						{
					    	Ucsi_message_in[0] = 0xFF;
							Ucsi_message_in[1] = 0x01;
							Ucsi_message_in[2] = 0x01;
							Ucsi_message_in[3] = 0x00;
							Ucsi_message_in[4] = 0x00;
							Ucsi_message_in[5] = 0x00;
							Ucsi_cci[1] = 0x06;
						}
						else
						{
							Ucsi_cci[1] = 0;
						}
					}

                    else
                    {
                        SMB3_DATA[0] = 0x01 + (((TempValue &0x03) - 1) << 4);
                        SMB3_DATA[1] = 0x02;
                        SMB3_DATA[2] = 0xA0;
                        SMB3_DATA[3] = 0x00;
                        SMB3_DATA[4] = 0xFF;

                        if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, TempTypeData, &SMB3_DATA[0], 5, SMBus_NoPEC))
                        {
                            SMB3_DATA_TEMP[0] = 'V';
                            SMB3_DATA_TEMP[1] = 'D';
                            SMB3_DATA_TEMP[2] = 'M';
                            SMB3_DATA_TEMP[3] = 's';
                            if(bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, tmpTYPCCmd, &SMB3_DATA_TEMP[0], 4, SMBus_NoPEC))
                            {
                                if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_RX_VDM,SMB3_DATA,bRead_I2C_NULL))
                                {
                                    /*i = SMB3_DATA[0]&0x07;
                                    Ucsi_message_in[0] = 0x00;
                                    Ucsi_message_in[1] = 0xFF;
                                    for(j = 0;j < i;j++)
                                    {
                                        Ucsi_message_in[(j*4)+2] = SMB3_DATA[(j*4)+1];
                                        Ucsi_message_in[(j*4)+3] = SMB3_DATA[(j*4)+2];
                                        Ucsi_message_in[(j*4)+4] = SMB3_DATA[(j*4)+3];
                                        Ucsi_message_in[(j*4)+5] = SMB3_DATA[(j*4)+4];
                                    }*/
                                    if((SMB3_DATA[1] == 0)&&(SMB3_DATA[2] == 0)&&(SMB3_DATA[3] == 0)&&(SMB3_DATA[4] == 0))
                                    {
                                        Ucsi_cci[1] = 0;
                                    }
                                    else
                                    {
                                        Ucsi_message_in[0] = 0x00;
                                        Ucsi_message_in[1] = 0xFF;
                                        Ucsi_message_in[2] = SMB3_DATA[1];
                                        Ucsi_message_in[3] = SMB3_DATA[2];
                                        Ucsi_message_in[4] = SMB3_DATA[3];
                                        Ucsi_message_in[5] = SMB3_DATA[4];
                                    }
                                }
                            }
                        }
                    }
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
                case USB_PDC_REQ_GET_CURRENT_CAM:

                    Ucsi_message_in[0] = 0x00;
                    Ucsi_cci[1] = 0x01;
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_DP_SID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                    {
                        if(SMB3_DATA[0] != 0x00)
                        {
                            Ucsi_message_in[0]++;
                        }
                    }
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_INTEL_VID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                    {
                        if(SMB3_DATA[0] != 0x00)
                        {
                            Ucsi_message_in[0]++;
                        }
                    }
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TI_VID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                    {
                        if(SMB3_DATA[0] != 0x00)
                        {
                            Ucsi_message_in[0]++;
                        }
                    }
                    if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_USER_SID_STATUS,SMB3_DATA,bRead_I2C_NULL))
                    {
                        if(SMB3_DATA[0] != 0x00)
                        {
                            Ucsi_message_in[0]++;
                        }
                    }
                    /*           if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_DP_SID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                               {
                                   if(SMB3_DATA[0] == 0x00)
                                   {
                                       if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_INTEL_VID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                                       {
                                           if(SMB3_DATA[0] == 0x00)
                                           {
                                               if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TI_VID_STATUS, SMB3_DATA,bRead_I2C_NULL))
                                               {
                                                   if(SMB3_DATA[0] == 0x00)
                                                   {
                                                       if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_USER_SID_STATUS,

                    SMB3_DATA,bRead_I2C_NULL))
                                                       {
                                                           if(SMB3_DATA[0] == 0x00)
                                                           {
                                                               Ucsi_message_in[0] = 0x00;
                                                               Ucsi_cci[1] = 0x00;
                                                           }
                                                           else
                                                           {
                                                               Ucsi_message_in[0] = SMB3_DATA[0];
                                                               Ucsi_cci[1] = 0x01;
                                                           }
                                                       }
                                                   }
                                                   else
                                                   {
                                                       Ucsi_message_in[0] = SMB3_DATA[0];
                                                       Ucsi_cci[1] = 0x01;
                                                   }
                                               }
                                           }
                                           else
                                           {
                                               Ucsi_message_in[0] = SMB3_DATA[0];
                                               Ucsi_cci[1] = 0x01;
                                           }
                                       }
                                   }
                                   else
                                   {
                                       Ucsi_message_in[0] = SMB3_DATA[0];
                                       Ucsi_cci[1] = 0x01;
                                   }
                               }*/
                    SET_MASK(TypeCStatus2,PdcRequestDone);
                    break;
            }
            //if(request_packet->status.done == FALSE)
            if(IS_MASK_CLEAR(TypeCStatus2,PdcRequestDone))
            {
#if DEBUG_UCSI
                RamDebug(0xA5);
                RamDebug(request_packet->status.done);
                RamDebug(request_packet->request);
#endif
                if(request_packet->status.done == FALSE)
                {
#if DEBUG_UCSI
                    RamDebug(0xA6);
#endif
                    if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, TempAddr, tmpTYPCCmd, &SMB3_DATA[0], 4, SMBus_NoPEC))
                    {
                        TypeCCommFailCnt++;
                    }
                    else
                    {
                        if(UCSI_4CC_CMD_RETRY == FALSE)
                            UCSI_PDC_REQ_RETRY = 0;
                        else
                            UCSI_4CC_CMD_RETRY = FALSE;
                        request_packet->status.done  = TRUE;
                    }

                }
                else
                {
#if DEBUG_UCSI
                    RamDebug(0xA7);
                    RamDebug(request_packet->status.done);
                    RamDebug(request_packet->request);
#endif
                    TypeCCommFailCnt = 0;
                    switch (request_packet->request)
                    {
                        case USB_PDC_REQ_HARD_RESET:
                            if(UCSI_CONNECTOR_RESET_WAIT_CNT == 100)
                            {
                                if(usb_pdc_id > 0)
                                {
                                    CLEAR_MASK(TypeCPort2Status,TypeCIniOK);
                                }
                                else
                                {
                                    CLEAR_MASK(TypeCStatus,TypeCIniOK);
                                }
                            }
                            UCSI_CONNECTOR_RESET_WAIT_CNT--;
                            if(UCSI_CONNECTOR_RESET_WAIT_CNT == 0)    //10ms X 500 = 5s
                            {
                                if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_STATUS, SMB3_DATA_TEMP1, bRead_I2C_NULL))
                                {
                                    if(usb_pdc_id == 0)
                                    {
                                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP1[0]) == 1)
                                            Usb_Pdc_Status.device_connected = TRUE;
                                        else
                                            Usb_Pdc_Status.device_connected = FALSE;
                                    }
                                    else
                                    {
                                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP1[0]) == 1)
                                            Usb_Pdc_Status2.device_connected = TRUE;
                                        else
                                            Usb_Pdc_Status2.device_connected = FALSE;
                                    }
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                                UCSI_CONNECTOR_RESET_WAIT = 0;
                            }
                            break;
                        case USB_PDC_REQ_SOFT_RESET:
                            UCSI_CONNECTOR_RESET_WAIT_CNT--;
                            if(UCSI_CONNECTOR_RESET_WAIT_CNT == 0)    //10ms X 500 = 5s
                            {
                                if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_STATUS, SMB3_DATA_TEMP1, bRead_I2C_NULL))
                                {
                                    if(usb_pdc_id == 0)
                                    {
                                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP1[0]) == 1)
                                            Usb_Pdc_Status.device_connected = TRUE;
                                        else
                                            Usb_Pdc_Status.device_connected = FALSE;
                                    }
                                    else
                                    {
                                        if(TPS65988_STATUS_PLUG_PRESENT(SMB3_DATA_TEMP1[0]) == 1)
                                            Usb_Pdc_Status2.device_connected = TRUE;
                                        else
                                            Usb_Pdc_Status2.device_connected = FALSE;
                                    }
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                                UCSI_CONNECTOR_RESET_WAIT = 0;
                            }
                            break;
                        case USB_PDC_REQ_SWAP_TO_SINK:
                            if((UCSI_4CC_CMD_COMPLETE == TRUE) && (UCSI_POWER_ROLE_SWAP_COMPLETE == TRUE))
                            {
                                UCSI_PDC_REQ_RETRY = 0;
                                UCSI_4CC_CMD_COMPLETE = FALSE;
                                UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                                UCSI_POWER_DIRECTION_CHANGE = 1;
                                if(usb_pdc_id == 0)
                                {
                                    UCSI_POWER_SOURCE = FALSE;
                                    Usb_Pdc_Power_Status.power_source = FALSE;
                                    UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                }
                                else
                                {
                                    UCSI_POWER_SOURCE2 = FALSE;
                                    Usb_Pdc_Power_Status2.power_source = FALSE;
                                    UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                            }
                            else
                            {
                                if(UCSI_4CC_CMD_COMPLETE == TRUE)
                                    UCSI_4CC_CMD_RETRY = TRUE;

                                UCSI_PDC_REQ_RETRY++;

                                if(UCSI_PDC_REQ_RETRY == 10)
                                {
                                    UCSI_POWER_DATA_SOURCE_REJECT = TRUE;
                                    UCSI_4CC_CMD_COMPLETE = FALSE;
                                    UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                                    if(usb_pdc_id == 0)
                                    {
                                        UCSI_POWER_SOURCE = TRUE;
                                    }
                                    else
                                    {
                                        UCSI_POWER_SOURCE2 = TRUE;
                                    }
                                    UCSI_PDC_REQ_RETRY = 0;
                                    SET_MASK(TypeCStatus2,PdcRequestDone);
                                }
                            }
                            break;
                        case USB_PDC_REQ_SWAP_TO_SOURCE:
                            if((UCSI_4CC_CMD_COMPLETE == TRUE) && (UCSI_POWER_ROLE_SWAP_COMPLETE == TRUE))
                            {
                                UCSI_PDC_REQ_RETRY = 0;
                                UCSI_4CC_CMD_COMPLETE = FALSE;
                                UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                                UCSI_POWER_DIRECTION_CHANGE = 1;
                                if(usb_pdc_id == 0)
                                {
                                    UCSI_POWER_SOURCE = TRUE;
                                    Usb_Pdc_Power_Status.power_source = TRUE;
                                    UCSI_CONNECTOR_CHANGE_STATUS[0] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                }
                                else
                                {
                                    UCSI_POWER_SOURCE2 = TRUE;
                                    Usb_Pdc_Power_Status2.power_source = TRUE;
                                    UCSI_CONNECTOR_CHANGE_STATUS[1] |= (UCSI_CONNECTOR_STATUS_CHANGE_POWER_DIRECTION | UCSI_CONNECTOR_STATUS_CHANGE_CONNECT);
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                            }
                            else
                            {
                                if(UCSI_4CC_CMD_COMPLETE == TRUE)
                                    UCSI_4CC_CMD_RETRY = TRUE;

                                UCSI_PDC_REQ_RETRY++;

                                if(UCSI_PDC_REQ_RETRY == 6)
                                {
                                    UCSI_POWER_DATA_SOURCE_REJECT = TRUE;
                                    UCSI_4CC_CMD_COMPLETE = FALSE;
                                    UCSI_POWER_ROLE_SWAP_COMPLETE = FALSE;
                                    if(usb_pdc_id == 0)
                                    {
                                        UCSI_POWER_SOURCE = FALSE;
                                    }
                                    else
                                    {
                                        UCSI_POWER_SOURCE2 = FALSE;
                                    }
                                    UCSI_PDC_REQ_RETRY = 0;
                                    SET_MASK(TypeCStatus2,PdcRequestDone);
                                }
                            }
                            break;
                        case USB_PDC_REQ_SWAP_TO_UFP:
                            if((UCSI_4CC_CMD_COMPLETE == TRUE) && (UCSI_DATA_ROLE_SWAP_COMPLETE == TRUE))
                            {
                                UCSI_PDC_REQ_RETRY = 0;
                                UCSI_4CC_CMD_COMPLETE = FALSE;
                                UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                                UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                                if(usb_pdc_id == 0)
                                {
                                    Usb_Pdc_Status.dfp = FALSE;
                                    UCSI_DATA_ROLE = 0;
                                }
                                else
                                {
                                    Usb_Pdc_Status2.dfp = FALSE;
                                    UCSI_DATA_ROLE2 = 0;
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                            }
                            else
                            {
                                if(UCSI_4CC_CMD_COMPLETE == TRUE)
                                    UCSI_4CC_CMD_RETRY = TRUE;
                                UCSI_PDC_REQ_RETRY++;
                                if(UCSI_PDC_REQ_RETRY == 6)
                                {
                                    UCSI_POWER_DATA_SOURCE_REJECT = TRUE;
                                    UCSI_4CC_CMD_COMPLETE = FALSE;
                                    UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                                    UCSI_PDC_REQ_RETRY = 0;
                                    SET_MASK(TypeCStatus2,PdcRequestDone);
                                }
                            }
                            break;
                        case USB_PDC_REQ_SWAP_TO_DFP:
                            if((UCSI_4CC_CMD_COMPLETE == TRUE) && (UCSI_DATA_ROLE_SWAP_COMPLETE == TRUE))
                            {
                                UCSI_PDC_REQ_RETRY = 0;
                                UCSI_4CC_CMD_COMPLETE = FALSE;
                                UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                                UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                                if(usb_pdc_id == 0)
                                {
                                    Usb_Pdc_Status.dfp = TRUE;
                                    UCSI_DATA_ROLE = 0;
                                }
                                else
                                {
                                    Usb_Pdc_Status2.dfp = TRUE;
                                    UCSI_DATA_ROLE2 = 0;
                                }
                                SET_MASK(TypeCStatus2,PdcRequestDone);
                            }
                            else
                            {
                                if(UCSI_4CC_CMD_COMPLETE == TRUE)
                                    UCSI_4CC_CMD_RETRY = TRUE;
                                UCSI_PDC_REQ_RETRY++;
                                if(UCSI_PDC_REQ_RETRY == 10)
                                {
                                    UCSI_POWER_DATA_SOURCE_REJECT = TRUE;
                                    UCSI_4CC_CMD_COMPLETE = FALSE;
                                    UCSI_DATA_ROLE_SWAP_COMPLETE = FALSE;
                                    UCSI_PDC_REQ_RETRY = 0;
                                    SET_MASK(TypeCStatus2,PdcRequestDone);
                                }
                            }
                            break;
                        case USB_PDC_REQ_GET_RX_SOURCE_PDO:   //RX:Device;TX:PDC
                            //if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_RX_SOURCE_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                            //{
                            /*request_packet->pdos_valid      = SMB3_DATA[0];
                            for(i = 0;(i < request_packet->pdos_valid)&&(i < 4); i++)
                            {
                               Ucsi_message_in[0 + 4*i] = SMB3_DATA[1 + 4*i];
                               Ucsi_message_in[1 + 4*i] = SMB3_DATA[2 + 4*i];
                               Ucsi_message_in[2 + 4*i] = SMB3_DATA[3 + 4*i];
                               Ucsi_message_in[3 + 4*i] = SMB3_DATA[4 + 4*i];
                               RamDebug(Ucsi_message_in[0 + 4*i]);
                               RamDebug(Ucsi_message_in[1 + 4*i]);
                               RamDebug(Ucsi_message_in[2 + 4*i]);
                               RamDebug(Ucsi_message_in[3 + 4*i]);
                            }*/
                            //RamDebug(0xFF);
                            //RamDebug(usb_pdc_id);
                            // RamDebug(Ucsi_cci[1]);

                            if(usb_pdc_id == 0)
                            {
                                if((UCSI_PDOs_Buffer[1] == 0) && (UCSI_PDOs_Buffer[2] == 0)
                                   && (UCSI_PDOs_Buffer[3] == 0) && (UCSI_PDOs_Buffer[4] == 0))
                                {
                                    Ucsi_message_in[0] = 0;
                                    Ucsi_message_in[1] = 0;
                                    Ucsi_message_in[2] = 0;
                                    Ucsi_message_in[3] = 0;
                                    Ucsi_cci[1] = 0x00;
                                    // RamDebug(0xAD);
                                }
                                else
                                {
                                    if(UCSI_GET_PDOS_COMMAND_PDO_OFFSET < UCSI_PDOs_Buffer[0])
                                    {
                                        Ucsi_message_in[0] = UCSI_PDOs_Buffer[1 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[1] = UCSI_PDOs_Buffer[2 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[2] = UCSI_PDOs_Buffer[3 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[3] = UCSI_PDOs_Buffer[4 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_cci[1] = 0x04;
                                        // RamDebug(0xAE);
                                    }
                                    else
                                    {
                                        Ucsi_message_in[0] = 0;
                                        Ucsi_message_in[1] = 0;
                                        Ucsi_message_in[2] = 0;
                                        Ucsi_message_in[3] = 0;
                                        Ucsi_cci[1] = 0x00;
                                        //RamDebug(0xAF);
                                    }
                                }
                            }
                            else
                            {
                                if((UCSI_PDOs_Buffer_CONNECTOR2[1] == 0) && (UCSI_PDOs_Buffer_CONNECTOR2[2] == 0)
                                   && (UCSI_PDOs_Buffer_CONNECTOR2[3] == 0) && (UCSI_PDOs_Buffer_CONNECTOR2[4] == 0))
                                {
                                    Ucsi_message_in[0] = 0;
                                    Ucsi_message_in[1] = 0;
                                    Ucsi_message_in[2] = 0;
                                    Ucsi_message_in[3] = 0;
                                    Ucsi_cci[1] = 0x00;
                                    //RamDebug(0xAA);
                                }
                                else
                                {
                                    if(UCSI_GET_PDOS_COMMAND_PDO_OFFSET < UCSI_PDOs_Buffer_CONNECTOR2[0])
                                    {
                                        Ucsi_message_in[0] = UCSI_PDOs_Buffer_CONNECTOR2[1 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[1] = UCSI_PDOs_Buffer_CONNECTOR2[2 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[2] = UCSI_PDOs_Buffer_CONNECTOR2[3 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[3] = UCSI_PDOs_Buffer_CONNECTOR2[4 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_cci[1] = 0x04;
                                        // RamDebug(0xAB);
                                    }
                                    else
                                    {
                                        Ucsi_message_in[0] = 0;
                                        Ucsi_message_in[1] = 0;
                                        Ucsi_message_in[2] = 0;
                                        Ucsi_message_in[3] = 0;
                                        Ucsi_cci[1] = 0x00;
                                        //RamDebug(0xAC);
                                    }
                                }
                            }
                            //RamDebug(Ucsi_cci[1]);
                            //}
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                            break;
                        case USB_PDC_REQ_GET_RX_SINK_PDO:
                            if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_RX_SINK_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                            {
                                /*request_packet->pdos_valid      = SMB3_DATA[0];
                                for(i = 0;(i < request_packet->pdos_valid)&&(i < 4); i++)
                                {
                                   Ucsi_message_in[0 + 4*i] = SMB3_DATA[1 + 4*i];
                                   Ucsi_message_in[1 + 4*i] = SMB3_DATA[2 + 4*i];
                                   Ucsi_message_in[2 + 4*i] = SMB3_DATA[3 + 4*i];
                                   Ucsi_message_in[3 + 4*i] = SMB3_DATA[4 + 4*i];
                                   RamDebug(Ucsi_message_in[0 + 4*i]);
                                   RamDebug(Ucsi_message_in[1 + 4*i]);
                                   RamDebug(Ucsi_message_in[2 + 4*i]);
                                   RamDebug(Ucsi_message_in[3 + 4*i]);
                                }*/
                                if((SMB3_DATA[1] == 0) && (SMB3_DATA[2] == 0)
                                   && (SMB3_DATA[3] == 0) && (SMB3_DATA[4] == 0))
                                {
                                    Ucsi_message_in[0] = 0;
                                    Ucsi_message_in[1] = 0;
                                    Ucsi_message_in[2] = 0;
                                    Ucsi_message_in[3] = 0;
                                    Ucsi_cci[1] = 0x00;
                                }
                                else
                                {
                                    if(UCSI_GET_PDOS_COMMAND_PDO_OFFSET < SMB3_DATA[0])
                                    {
                                        Ucsi_message_in[0] = SMB3_DATA[1 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[1] = SMB3_DATA[2 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[2] = SMB3_DATA[3 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_message_in[3] = SMB3_DATA[4 + 4*UCSI_GET_PDOS_COMMAND_PDO_OFFSET];
                                        Ucsi_cci[1] = 0x04;
                                    }
                                    else
                                    {
                                        Ucsi_message_in[0] = 0;
                                        Ucsi_message_in[1] = 0;
                                        Ucsi_message_in[2] = 0;
                                        Ucsi_message_in[3] = 0;
                                        Ucsi_cci[1] = 0x00;
                                    }
                                }
                            }
                            SET_MASK(TypeCStatus2,PdcRequestDone);
                            break;
                        case USB_PDC_REQ_GET_TX_SOURCE_PDO:
                        /*if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TX_SOURCE_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                        {
                            Ucsi_message_in[0] = SMB3_DATA[0];
                            Ucsi_message_in[1] = SMB3_DATA[1];
                            Ucsi_message_in[2] = SMB3_DATA[2];
                            Ucsi_message_in[3] = SMB3_DATA[3];
                        }
                        SET_MASK(TypeCStatus2,PdcRequestDone);
                        break;*/
                        case USB_PDC_REQ_GET_TX_SINK_PDO:
                            /*if(bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_TX_SINK_CAPABILITIES, SMB3_DATA,bRead_I2C_NULL))
                            {
                                Ucsi_message_in[0] = SMB3_DATA[0];
                                Ucsi_message_in[1] = SMB3_DATA[1];
                                Ucsi_message_in[2] = SMB3_DATA[2];
                                Ucsi_message_in[3] = SMB3_DATA[3];
                            }*/
                            //request_packet->status.done  = TRUE;
                            //SET_MASK(TypeCStatus2,PdcRequestDone);
                            break;
                    }
                    if(UCSI_PDC_REQ_RETRY == 0)
                    {
                        request_packet->status.done  = FALSE;
                        request_packet->status.busy  = FALSE;
                    }
                }
            }
            else
            {
                request_packet->status.busy  = FALSE;
            }
//      request_packet->next         = NULL;
            request_packet->request      = cmd;
            request_packet->parameter[0] = parm1;
            request_packet->parameter[1] = parm2;
            request_packet->address      = address;
//      request_packet->call_back    = call_back;
//      request_packet->status.done  = FALSE;
//      request_packet->status.busy  = FALSE;
        }
//    CmnSetIntMask(int_mask);
    }
    else
    {
        /*
        *    invalid USB PD ID
        */
#if DEBUG_UCSI
        RamDebug(0xAF);
#endif
        request_packet->status.done = TRUE;
        request_packet->status.busy = FALSE;
        rc                          = USB_PDC_RC_NOT_SUPPORTED;
    }
    if (USB_PDC_RC_OK == rc)
    {
//    TskActivateTask(TASK_USB_PDC_0 + usb_pdc_id);
    }
    return rc;
}

void UsbPdcGetPowerRole(BYTE usb_pdc_id)
{
    BYTE TempAddr;
    if(usb_pdc_id > 0)
    {
        TempAddr = TypeC_02_Addr;
    }
    else
    {
        TempAddr = TypeC_01_Addr;
    }
    if (NUMBER_OF_USB_PDC > usb_pdc_id)
    {
        //  if (IS_MASK_SET(TypeCStatus,TypeCIniOK)) {
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP, bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;
#if DEBUG_UCSI
            RamDebug(0xB3);
            RamDebug(Usb_Pdc_Power_Status.power_source);
            RamDebug(TPS65988_STATUS_PORT_ROLE(SMB3_DATA_TEMP[0]));
#endif
            if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]) == 1)
            {
                //RamDebug(0xB3);

                if(usb_pdc_id == 0)
                {
                    UCSI_POWER_SOURCE = FALSE;
                    Usb_Pdc_Power_Status.power_source = FALSE;
                }
                else
                {
                    UCSI_POWER_SOURCE2 = FALSE;
                    Usb_Pdc_Power_Status2.power_source = FALSE;
                }
                //printf ("("@E5-1\n");
            }
            else
            {
                //RamDebug(0xB4);
                if(usb_pdc_id == 0)
                {
                    UCSI_POWER_SOURCE = TRUE;
                    Usb_Pdc_Power_Status.power_source = TRUE;
                }
                else
                {
                    UCSI_POWER_SOURCE2 = TRUE;
                    Usb_Pdc_Power_Status2.power_source = TRUE;
                }
                //printf ("("@E5-2\n");
            }

        }
        // }
    }
}

void UsbPdcGetDataRole(BYTE usb_pdc_id)
{
    BYTE TempAddr;
    if(usb_pdc_id > 0)
    {
        TempAddr = TypeC_02_Addr;
    }
    else
    {
        TempAddr = TypeC_01_Addr;
    }
    if (NUMBER_OF_USB_PDC > usb_pdc_id)
    {
        //if (IS_MASK_SET(TypeCStatus,TypeCIniOK)) {
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_STATUS, SMB3_DATA_TEMP, bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;

            if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
            {
                if(usb_pdc_id == 0)
                {
                    Usb_Pdc_Status.dfp = TRUE;
                }
                else
                {
                    Usb_Pdc_Status2.dfp = TRUE;
                }
            }
            else
            {
                if(usb_pdc_id == 0)
                {
                    Usb_Pdc_Status.dfp = FALSE;
                }
                else
                {
                    Usb_Pdc_Status2.dfp = FALSE;
                }
            }

        }
        // }
    }
}

/*
*******************************************************************************
* Function name: UsbPdcGetPowerStatus
*
* Descriptoin:
*          This is used to get power status.
*
* Arguments:
*       int usb_pdc_id
* Return Values:
*       USB_POWER_STATUS
*******************************************************************************
*/

void UsbPdcGetPowerStatus(BYTE usb_pdc_id)
{
//      USB_POWER_STATUS Temp2;
//      USB_STATUS  Temp3;
//      Temp2 = Usb_Pdc_Power_Status;
//      Temp3 = Usb_Pdc_Status;
    BYTE TempAddr;
    if(usb_pdc_id > 0)
    {
        //RamDebug(0xB0);
        TempAddr = TypeC_02_Addr;
        Usb_Pdc_Power_Status2.power_connection = FALSE;
    }
    else
    {
        //RamDebug(0xB1);
        TempAddr = TypeC_01_Addr;
        Usb_Pdc_Power_Status.power_connection = FALSE;
    }

    if (NUMBER_OF_USB_PDC > usb_pdc_id)
    {
        // if (IS_MASK_SET(TypeCStatus,TypeCIniOK)) {
#if DEBUG_UCSI
        RamDebug(0xB0);
#endif
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_POWER_STATUS, SMB3_DATA_TEMP, bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;
#if DEBUG_UCSI
            RamDebug(0xB1);
            RamDebug(SMB3_DATA_TEMP[0]);
#endif
            if (TPS65988_POWER_STATUS_POWER_CONNECTION(SMB3_DATA_TEMP[0]) == 1)
            {
#if DEBUG_UCSI
                RamDebug(0xB2);
#endif
                /*
                *    there is power connection
                */
                if(usb_pdc_id == 0)
                {
                    Usb_Pdc_Power_Status.power_connection = TRUE;

                    if (TPS65988_POWER_STATUS_BC12_DETECTION(SMB3_DATA_TEMP[0]) == 1)
                    {
                        /*
                        *    BC 1.2 connection
                        */
                        switch (TPS65988_POWER_STATUS_BC12_STATUS_MASK(SMB3_DATA_TEMP[0]))
                        {
                            case TPS65988_POWER_STATUS_BC12_STATUS_SDP:
                                Usb_Pdc_Power_Status.bc12_status = USB_BC12_SDP;
                                break;
                            case TPS65988_POWER_STATUS_BC12_STATUS_CDP:
                                Usb_Pdc_Power_Status.bc12_status = USB_BC12_CDP;
                                break;
                            case TPS65988_POWER_STATUS_BC12_STATUS_DCP:
                                Usb_Pdc_Power_Status.bc12_status = USB_BC12_DCP;
                                break;
                            default:
                                Usb_Pdc_Power_Status.bc12_status = USB_BC12_NONE;
                                break;
                        }
                        Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_NONE;
                    }
                    else
                    {
                        /*
                        *    Get Type-C current
                        */
                        Usb_Pdc_Power_Status.bc12_status = USB_BC12_NONE;
#if DEBUG_UCSI
                        RamDebug(Usb_Pdc_Power_Status.power_connection);
                        RamDebug(Usb_Pdc_Power_Status.bc12_status);
#endif
                        switch (TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA_TEMP[0]))
                        {
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_DEFAULT:
                                Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_1_5_A:
                                Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_1_5_A;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_3_0_A:
                                Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_3_0_A;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT:
                                Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_PD_CONTRACT;
                                break;
                            default:
                                Usb_Pdc_Power_Status.type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
                        }
#if 1
                        RamDebug(Usb_Pdc_Power_Status.type_c_current);
#endif
                    }
                }
                else
                {
                    Usb_Pdc_Power_Status2.power_connection = TRUE;

                    if (TPS65988_POWER_STATUS_BC12_DETECTION(SMB3_DATA_TEMP[0]) == 1)
                    {
                        /*
                        *    BC 1.2 connection
                        */
                        switch (TPS65988_POWER_STATUS_BC12_STATUS_MASK(SMB3_DATA_TEMP[0]))
                        {
                            case TPS65988_POWER_STATUS_BC12_STATUS_SDP:
                                Usb_Pdc_Power_Status2.bc12_status = USB_BC12_SDP;
                                break;
                            case TPS65988_POWER_STATUS_BC12_STATUS_CDP:
                                Usb_Pdc_Power_Status2.bc12_status = USB_BC12_CDP;
                                break;
                            case TPS65988_POWER_STATUS_BC12_STATUS_DCP:
                                Usb_Pdc_Power_Status2.bc12_status = USB_BC12_DCP;
                                break;
                            default:
                                Usb_Pdc_Power_Status2.bc12_status = USB_BC12_NONE;
                                break;
                        }
                        Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_NONE;
                    }
                    else
                    {
                        /*
                        *    Get Type-C current
                        */
                        Usb_Pdc_Power_Status2.bc12_status = USB_BC12_NONE;
#if DEBUG_UCSI
                        RamDebug(Usb_Pdc_Power_Status2.power_connection);
                        RamDebug(Usb_Pdc_Power_Status2.bc12_status);
#endif
                        switch (TPS65988_POWER_STATUS_TYPE_C_CURRENT_MASK(SMB3_DATA_TEMP[0]))
                        {
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_DEFAULT:
                                Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_1_5_A:
                                Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_1_5_A;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_3_0_A:
                                Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_3_0_A;
                                break;
                            case TPS65988_POWER_STATUS_TYPE_C_CURRENT_PD_CONTRACT:
                                Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_PD_CONTRACT;
                                break;
                            default:
                                Usb_Pdc_Power_Status2.type_c_current = USB_TYPE_C_CURRENT_DEFAULT;
                        }
#if DEBUG_UCSI
                        RamDebug(Usb_Pdc_Power_Status2.type_c_current);
#endif
                    }
                }

                /*
                *    Get Source/Sink
                */
                /*
                #if DEBUG_UCSI_CMD
                RamDebug(0xB2);
                RamDebug(UCSI_POWER_SOURCE);
                RamDebug(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]));
                #endif
                if(TPS65988_POWER_STATUS_SOURCE_SINK(SMB3_DATA_TEMP[0]) == 1)
                {
                    if(UCSI_POWER_SOURCE == TRUE)
                    {
                        UCSI_POWER_SOURCE = FALSE;
                        UCSI_POWER_DIRECTION_CHANGE = 1;
                    }
                }
                else
                {
                     if(UCSI_POWER_SOURCE == FALSE)
                     {
                        UCSI_POWER_SOURCE = TRUE;
                    UCSI_POWER_DIRECTION_CHANGE = 1;
                     }
                }*/

//            UcsiUsbPdcStatusChanged(Temp2, Temp3);// mingjer dbg
            }

        }
        if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, TempAddr, TPS65988_STATUS, SMB3_DATA_TEMP, bRead_I2C_NULL))
        {
            TypeCCommFailCnt++;
        }
        else
        {
            TypeCCommFailCnt = 0;

            if(usb_pdc_id == 0)
            {
                if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                {

                    if(Usb_Pdc_Status.dfp == FALSE)
                    {
#if DEBUG_UCSI_CMD
                        RamDebug(0xB6);
#endif
                        Usb_Pdc_Status.dfp = TRUE;
                        UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    }
                }
                else
                {

                    if(Usb_Pdc_Status.dfp == TRUE)
                    {
#if DEBUG_UCSI_CMD
                        RamDebug(0xB5);
#endif
                        Usb_Pdc_Status.dfp = FALSE;
                        UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    }
                }
            }
            else
            {
                if(TPS65988_STATUS_DATA_ROLE(SMB3_DATA_TEMP[0]) == 1)
                {

                    if(Usb_Pdc_Status2.dfp == FALSE)
                    {
#if DEBUG_UCSI_CMD
                        RamDebug(0xB6);
#endif
                        Usb_Pdc_Status2.dfp = TRUE;
                        UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    }
                }
                else
                {

                    if(Usb_Pdc_Status2.dfp == TRUE)
                    {
#if DEBUG_UCSI_CMD
                        RamDebug(0xB5);
#endif
                        Usb_Pdc_Status2.dfp = FALSE;
                        UCSI_CONNECTOR_PARTNER_CHANGE = 1;
                    }
                }
            }


            UCSI_CONNECTOR_STATE = ((SMB3_DATA_TEMP[0]&0x0E) >> 1);
#if DEBUG_UCSI_CMD
            RamDebug(Usb_Pdc_Status.dfp);
            RamDebug(UCSI_CONNECTOR_STATE);
#endif
        }


        //}
    }

//    return Usb_Pdc_Power_Status;
}

void SendPDO20VTo9V(BYTE portNum)
{
    BYTE tmpAddr,tmpCOMFailFlag;

    if(portNum == TYPECPort1)
    {
        tmpAddr = TypeC_01_Addr;
    }
    else if(portNum == TYPECPort2)
    {
        tmpAddr = TypeC_02_Addr;
    }
    else
        return;
    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], SMBus_NoPEC))
    {
        //TypeCCommFailCnt++;
        tmpCOMFailFlag = 1;
    }
    else
    {
        // TypeCCommFailCnt = 0;
        tmpCOMFailFlag = 0;

        RamDebug(0xD9);
        RamDebug(SMB3_DATA[7]);
        RamDebug(SMB3_DATA[8]);
        RamDebug(SMB3_DATA[9]);
        RamDebug(0xD9);
        if((SMB3_DATA[7]==0x01)&&(SMB3_DATA[8] == 0x99 ))
        {
            SMB3_DATA[7] = 0x41;
            SMB3_DATA[8] = 0x8B;
        }

        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], 10, SMBus_NoPEC))
        {
            //TypeCCommFailCnt++;
            tmpCOMFailFlag = 1;
        }
        else
        {
            TypeCCommFailCnt = 0;
            if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], SMBus_NoPEC))
            {
                //TypeCCommFailCnt++;
                tmpCOMFailFlag = 1;
            }
            else
            {
                //TypeCCommFailCnt = 0;
                tmpCOMFailFlag = 0;

                RamDebug(0xD8);
                RamDebug(SMB3_DATA[7]);
                RamDebug(SMB3_DATA[8]);
                RamDebug(SMB3_DATA[9]);
                RamDebug(0xD8);
                //TypeCCommFailCnt = 0;
            }
        }
        //}
        Send4ccCmd(TPS65988_4CC_CMD_GSRC,portNum);
        if(portNum == TYPECPort1)
        {
            SET_MASK(PDOVoltageFlag, Port1PDOVoltage9V);
            CurrentRDO = ObjectPosition09V;
        }
        else if(portNum == TYPECPort2)
        {
            SET_MASK(PDOVoltageFlag, Port2PDOVoltage9V);
            Port2CurrentRDO = Port2ObjectPosition09V;
        }
    }

    if(portNum == TYPECPort1)
    {
        if(tmpCOMFailFlag == 1)
            TypeCCommFailCnt++;
        else
            TypeCCommFailCnt = 0;
    }
    else if(portNum == TYPECPort2)
    {
        if(tmpCOMFailFlag == 1)
            TypeCPort2CommFailCnt++;
        else
            TypeCPort2CommFailCnt = 0;
    }

    //A285D00048 reapdater voltage
    ChargeIC_InitCnt = CHARGE_IC_VOLTAGE_SET_COUNT;

}

void SendPDO9VTo20V(BYTE portNum)
{
    BYTE tmpAddr,tmpCOMFailFlag;

    if(portNum == TYPECPort1)
    {
        tmpAddr = TypeC_01_Addr;
    }
    else if(portNum == TYPECPort2)
    {
        tmpAddr = TypeC_02_Addr;
    }
    else
        return;
    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], SMBus_NoPEC))
    {
        tmpCOMFailFlag = 1;
    }
    else
    {
        tmpCOMFailFlag = 0;

        RamDebug(0xDD);
        RamDebug(SMB3_DATA[7]);
        RamDebug(SMB3_DATA[8]);
        RamDebug(SMB3_DATA[9]);
        RamDebug(0xDD);
        if((SMB3_DATA[7]==0x41)&&(SMB3_DATA[8] == 0x8B ))
        {
            SMB3_DATA[7] = 0x01;
            SMB3_DATA[8] = 0x99;
        }
        if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], 10, SMBus_NoPEC))
        {
            tmpCOMFailFlag =1;
        }
        else
        {
            tmpCOMFailFlag = 0;
            if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpAddr, TPS65988_TX_SINK_CAPABILITIES, &SMB3_DATA[0], SMBus_NoPEC))
            {
                tmpCOMFailFlag = 1;
            }
            else
            {
                tmpCOMFailFlag = 0;

                RamDebug(0xDC);
                RamDebug(SMB3_DATA[7]);
                RamDebug(SMB3_DATA[8]);
                RamDebug(SMB3_DATA[9]);
                RamDebug(0xDC);
                //TypeCCommFailCnt = 0;
            }
        }
        //}
        Send4ccCmd(TPS65988_4CC_CMD_GSRC,portNum);
        if(0 == tmpCOMFailFlag)
        {
            if(portNum == TYPECPort1)
            {
                CurrentRDO = ObjectPosition20V;
                CLEAR_MASK(PDOVoltageFlag, Port1PDOVoltage9V);
            }
            else if(portNum == TYPECPort2)
            {
                Port2CurrentRDO = Port2ObjectPosition20V;
                CLEAR_MASK(PDOVoltageFlag, Port2PDOVoltage9V);
            }
        }
    }

    if(portNum == TYPECPort1)
    {
        if(tmpCOMFailFlag == 1)
            TypeCCommFailCnt++;
        else
            TypeCCommFailCnt = 0;
    }
    else if(portNum == TYPECPort2)
    {
        if(tmpCOMFailFlag == 1)
            TypeCPort2CommFailCnt++;
        else
            TypeCPort2CommFailCnt = 0;
    }

    //A285D00048 reapdater voltage
    ChargeIC_InitCnt = CHARGE_IC_VOLTAGE_SET_COUNT;
}

void PDOPortSwitchToNormal(void)
{

    //if(IS_MASK_SET(TypeCStatus,TypeCAdpExist)&&IS_MASK_SET(TypeCStatus,TypeCAdpReady)) --mark 20180418 for cs18 dock maybe canot identify adapter
    {
        if((CurrentRDO!= ObjectPosition20V) || IS_MASK_SET(PDOVoltageFlag, Port1PDOVoltage9V))
        {
            //SendPdoSet(ObjectPosition20V,TYPECPort1); // Switch PDO to 20V
            SendPDO9VTo20V(TYPECPort1);
            //adapter port exist reRead watt.
            if(IS_MASK_SET(TypeCStatus,TypeCAdpExist)&&IS_MASK_SET(TypeCStatus,TypeCAdpReady))
            {
                //reRead battery watt
                TypeCAdpPdStableCheck = 5;
                TypeCAdpDetectDelay = 5;
            }
        }
    }

    //if(IS_MASK_SET(TypeCPort2Status,TypeCAdpExist)&&IS_MASK_SET(TypeCPort2Status,TypeCAdpReady))--mark 20180418 for cs18 dock maybe canot identify adapter
    {
        if((Port2CurrentRDO != Port2ObjectPosition20V) || IS_MASK_SET(PDOVoltageFlag, Port2PDOVoltage9V))
        {
            //SendPdoSet(Port2ObjectPosition20V,TYPECPort2);    // Switch PDO to 20V
            SendPDO9VTo20V(TYPECPort2);
            //adapter port exist reRead watt.
            if(IS_MASK_SET(TypeCPort2Status,TypeCAdpExist)&&IS_MASK_SET(TypeCPort2Status,TypeCAdpReady))
            {
                //reRead battery watt
                TypeCPort2AdpPdStableCheck = 5;
                TypeCPort2AdpDetectDelay = 5;
            }
        }
    }
}

BOOL isBatteryExist(void)
{
    return (((TRUE == BattExistGet) && (1 == M_BATdata_ready))  \
            ||  ((TRUE == SecBattExistGet) && (1 == S_BATdata_ready))); //A285D00044:fix dual-battery shipmode ec hangup with black screen
}

void TPS65988_UcsiRelativeInit(void)
{
    if(isBatteryExist())
    {
        WORD RemainCap1,RemainCap2;
        WORD FullChgCap1,FullChgCap2;
        DWORD RelativeStateOfChg;

        RemainCap1 = ((RemainCapH << 8) + RemainCapL);
        RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
        FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
        FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
        if(0 != (FullChgCap1 + FullChgCap2))
        {
            RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
            UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;

            UCSI_LastRelativeStateOfChg = UCSI_RelativeStateOfChg;
        }
        //RamDebug(0xBB);
        //RamDebug(UCSI_RelativeStateOfChg);
    }
}

void TPS65988_SelectAdapterPort(void)
{
    BOOL isNeedSwitch = FALSE;
    BOOL isNeedSNK = FALSE;

    do
    {
        if(isBatteryExist())
        {
            WORD RemainCap1,RemainCap2;
            WORD FullChgCap1,FullChgCap2;
            DWORD RelativeStateOfChg;

            RemainCap1 = ((RemainCapH << 8) + RemainCapL);
            RemainCap2 = ((SecRemainCapH << 8) + SecRemainCapL);
            FullChgCap1 = ((FullChgCapH << 8) + FullChgCapL);
            FullChgCap2 = ((SecFullChgCapH << 8) + SecFullChgCapL);
            if(0 == (FullChgCap1 + FullChgCap2)) //A285D00044:fix dual-battery shipmode ec hangup with black screen
            {
                break;
            }
            RelativeStateOfChg = ((RemainCap1 + RemainCap2)*100)/(FullChgCap1 + FullChgCap2);
            UCSI_RelativeStateOfChg = (BYTE)RelativeStateOfChg;

            isNeedSwitch = (UCSI_RelativeStateOfChg > 7);
            //RamDebug(0xBB);
            //RamDebug(UCSI_RelativeStateOfChg);
            if((UCSI_LastRelativeStateOfChg <= PD_SWITCH_BATTERY_CAPA_THERSHOLD)
               && (UCSI_RelativeStateOfChg > PD_SWITCH_BATTERY_CAPA_THERSHOLD))
            {
                //battery capa from low to high need open switch port
                isNeedSNK = TRUE;
            }
            else if((UCSI_LastRelativeStateOfChg >= PD_SWITCH_BATTERY_CAPA_THERSHOLD)
                    && (UCSI_RelativeStateOfChg < PD_SWITCH_BATTERY_CAPA_THERSHOLD))
            {
                //battery capa from high to low need close switch port
                isNeedSNK = TRUE;
            }

            UCSI_LastRelativeStateOfChg = UCSI_RelativeStateOfChg;
        }


        if(IS_MASK_SET(TypeCStatus, TypeCAdpReady) && IS_MASK_SET(TypeCPort2Status, TypeCAdpReady))
        {

            if(isNeedSwitch)
            {
                if((Port2CurrentTypeCWatt > Port1CurrentTypeCWatt)
                    && (TYPECPort2 != Port_Select)) //A285D00077
                {
                    Port_Select         = TYPECPort2;
                    TypeCAdpID      = TypeCPort2AdpID;
                    CurrentTypeCWatt    = Port2CurrentTypeCWatt;
                    CurrentTypeCV   = Port2CurrentTypeCV;
                    CurrentTypeCI       = Port2CurrentTypeCI;
                    
                    //A285D00077 start+
                    if((Port2CurrentTypeCWatt > 27) && (ACIN_BLINK3 == 0))
                    {
                        if (SystemNotS5)
                        {
                            ACIN_BLINK3 = 35;        // LED blink 3times in 1.5sec
                        }
                        else
                        {
                            ACIN_BLINK3 = 30;        // LED blink 3times in 1sec
                        }
                    }                    
                    //A285D00077 end+
                }
                else if((Port2CurrentTypeCWatt < Port1CurrentTypeCWatt)
                    && (TYPECPort1 != Port_Select)) //A285D00077+
                {
                    Port_Select         = TYPECPort1;
                    TypeCAdpID      = TypeCPort1AdpID;
                    CurrentTypeCWatt    = Port1CurrentTypeCWatt;
                    CurrentTypeCV   = Port1CurrentTypeCV;
                    CurrentTypeCI       = Port1CurrentTypeCI;
                    //A285D00077 start+

                    if((Port1CurrentTypeCWatt > 27) && (ACIN_BLINK3 == 0))
                    {
                        if (SystemNotS5)
                        {
                            ACIN_BLINK3 = 35;        // LED blink 3times in 1.5sec
                        }
                        else
                        {
                            ACIN_BLINK3 = 30;        // LED blink 3times in 1sec
                        }
                    }
                    //A285D00077 end+
                }
            }

            if(isNeedSNK)
            {
                SetMultiPortSNK(TYPECPort2);
                SetMultiPortSNK(TYPECPort1);
            }
            break;
        }

        if(IS_MASK_SET(TypeCStatus, TypeCAdpReady))
        {
            Port_Select         = TYPECPort1;
            TypeCAdpID      = TypeCPort1AdpID;
            CurrentTypeCWatt    = Port1CurrentTypeCWatt;
            CurrentTypeCV   = Port1CurrentTypeCV;
            CurrentTypeCI       = Port1CurrentTypeCI;
            if(isNeedSNK)
            {
                SetMultiPortSNK(TYPECPort1);
            }
            break;
        }

        if(IS_MASK_SET(TypeCPort2Status, TypeCAdpReady))
        {
            Port_Select         = TYPECPort2;
            TypeCAdpID      = TypeCPort2AdpID;
            CurrentTypeCWatt    = Port2CurrentTypeCWatt;
            CurrentTypeCV   = Port2CurrentTypeCV;
            CurrentTypeCI       = Port2CurrentTypeCI;

            if(isNeedSNK)
            {
                SetMultiPortSNK(TYPECPort2);
            }
            break;
        }

        Port_Select         = 0;
        TypeCAdpID      = 0;
        CurrentTypeCWatt    = 0;
        CurrentTypeCV   = 0;
        CurrentTypeCI       = 0;

    }
    while(0);
}

void SetMultiPortSNK(BYTE portNum)
{
    BYTE tmpAddr,tmpCOMFailFlag;

    if(portNum == TYPECPort1)
    {
        tmpAddr = TypeC_01_Addr;
    }
    else if(portNum == TYPECPort2)
    {
        tmpAddr = TypeC_02_Addr;
    }

    if(!bRSMBusBlock(chSMB_TYPEC, SMbusRBK, tmpAddr, TPS65988_GLOBAL_SYSTEM_CONFIG, &SMB3_DATA[0], SMBus_NoPEC))
    {
        tmpCOMFailFlag = 1;
    }
    else
    {
        tmpCOMFailFlag = 0;

        //RamDebug(0xCC);
        //RamDebug(SMB3_DATA[12]);
        //RamDebug(0xCC);
        if(UCSI_RelativeStateOfChg > PD_SWITCH_BATTERY_CAPA_THERSHOLD)
        {
            if((SMB3_DATA[12]&0x03) == 0x02)
            {
                RamDebug(0xDD);
                SMB3_DATA[12] &= 0xFC;
                SMB3_DATA[12] |= 0x01;
                RamDebug(SMB3_DATA[12]);
                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, tmpAddr, TPS65988_GLOBAL_SYSTEM_CONFIG, &SMB3_DATA[0], 14, SMBus_NoPEC))
                {
                    tmpCOMFailFlag =1;
                }
                else
                {
                    tmpCOMFailFlag = 0;
                }
            }
        }
        else
        {
            if((SMB3_DATA[12]&0x03) == 0x01)
            {
                RamDebug(0xEE);
                SMB3_DATA[12] &= 0xFC;
                SMB3_DATA[12] |= 0x02;
                RamDebug(SMB3_DATA[12]);
                if(!bWSMBusBlock(chSMB_TYPEC, SMbusWBK, tmpAddr, TPS65988_GLOBAL_SYSTEM_CONFIG, &SMB3_DATA[0], 14, SMBus_NoPEC))
                {
                    tmpCOMFailFlag =1;
                }
                else
                {
                    tmpCOMFailFlag = 0;
                }
            }
        }
    }

    if(portNum == TYPECPort1)
    {
        if(tmpCOMFailFlag == 1)
            TypeCCommFailCnt++;
        else
            TypeCCommFailCnt = 0;
    }
    else if(portNum == TYPECPort2)
    {
        if(tmpCOMFailFlag == 1)
            TypeCPort2CommFailCnt++;
        else
            TypeCPort2CommFailCnt = 0;
    }
}

#endif //Support_TYPE_C

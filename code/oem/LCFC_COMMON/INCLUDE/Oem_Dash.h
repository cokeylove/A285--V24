
#ifndef	OEM_DASH_H
#define	OEM_DASH_H

//----------------------------------------------------------------------------
//Configuration for DASH
// This is nececessary for first use DASH module.
//----------------------------------------------------------------------------	

/*
*********************************************************
*     #EC ram define
*********************************************************
*/
//#define  DASH_RPM1             Dash_RAM0_16          
//#define  TMPIN1                Dash_RAM0_8          
//#define  TMPIN2                Dash_RAM1_8          
//#define  ADC0_test             Dash_RAM2_8
//#define  ADC1_test             Dash_RAM3_8
#define  recvCnt               Dash_RAM0_8
//#define  sla_addr              Dash_RAM5_8
#define  SmbReadDataBuf        Dash_RAM0_48
#define  SmbWriteDataBuf       Dash_RAM1_48



//Define smbus control registers for DASH
#define DASH_SLDA		    SLDA_A			    //0x1C09
#define DASH_SLSTA		  SLSTA_A			  //0x1C0B
#define DASH_SICR		    SICR_A			    //0x1C0C
#define DASH_RESLADR 	  RESLADR_A		    //0x1C08
#define DASH_RESLADR2 	  RESLADR_2_A		  //0x1C3F
#define DASH_HOCTL		  HOCTL_A         //0x1C01
#define DASH_HOCTL2		  HOCTL2_A       	//0x1C10 
#define DASH_HOSTA		  HOSTA_A         //0x1C00
#define DASH_TRASLA		  TRASLA_A        //0x1C03
#define DASH_HOCMD		  HOCMD_A       	//0x1C02
#define DASH_D0REG		  D0REG_A         //0x1C04
#define DASH_HOBDB		  HOBDB_A       	//0x1C06  
#define DASH_PECERC		  PECERC_A       	//0x1C07 

#define SMBUS_BUFFER_SIZE	48	//recv/write buffer size
#define SMBUS_RECV_BUFFER	(DashRxPkt *)SmbReadDataBuf   //(_DashRxPkt *)SmbReadDataBuf		//the size of buffer must >= 48bytes
#define SMBUS_WRITE_BUFFER	(DashTxPkt *)SmbWriteDataBuf  //(_DashRxPkt *)SmbWriteDataBuf	  //the size of buffer must >= 48bytes
//----------------------------------------------------------------------------
//End Configuration for DASH
//----------------------------------------------------------------------------	

/********************
 *   Slave Status
********************/
#define I2C_SLV_STATE_INTRS		0xF7
#define I2C_SLV_STATE_TIMEOUT	0x04
#define I2C_SLV_STATE_STOP		0x20
#define I2C_SLV_STATE_READ		0x02

/********************
 *   Address
********************/
//ivan #define DASH_SLV_ADDR	 0x1C	    //EC SMBus Slave Address
#define DASH_XMIT_ADDR	 0xC8    //DASH SMBus Transmitted Target Address
#define DASH_EC_ADDR	 0x1C    // 0xDB   //EC's Source Address, only used for response DASH's request 

/***********************
*    Sensor ID
***********************/
#define DASH_SNR_ID_APU_T	0x0001	//APU_Temperature
#define DASH_SNR_ID_DDR_T	0x0002	//DDR_Temperature
#define DASH_SNR_ID_APU_V	0x0003	//APU_Voltage
#define DASH_SNR_ID_DDR_V 	0x0004 //DDR_Voltage
#define DASH_SNR_ID_FAN		0x0005	//FAN_SPEED
//#define DASH_SNR_ID_APU_T	0x0100	//APU_Temperature
//#define DASH_SNR_ID_DDR_T	0x0200	//DDR_Temperature
//#define DASH_SNR_ID_APU_V	0x0300	//APU_Voltage
//#define DASH_SNR_ID_DDR_V 	0x0400  //DDR_Voltage
//#define DASH_SNR_ID_FAN		0x0500	//FAN_SPEED

/***********************
*    Sensor Registers
***********************/
//#define DashFan1	RPM1
//#define DashFan2	RPM2
//#define DashFan3	RPM3
//#define DashTmp1	TMPIN1
//#define DashTmp2	TMPIN2
//#define DashTmp3	TMPIN3
//#define DashVIN0	ADC0_test
//#define DashVIN1	ADC1_test
//#define DashVIN2	ADC2_test
//#define DashVIN3	ADC3_test
//#define DashVIN4	ADC4_test
//#define DashVIN5	ADC5_test

/***********************
*    PLDM Commands Definition
***********************/
//Terminus Commands
#define DASH_PCMD_SET_TID					  0x01	//SetTID
#define DASH_PCMD_GET_TID					  0x02	//GetTID
#define DASH_PCMD_GET_PLDM_VER				0x03	//GetPLDMVersion
#define DASH_PCMD_GET_PLDM_TYPES			0x04	//GetPLDMTypes
#define DASH_PCMD_GET_PLDM_CMD				0x05	//GetPLDMCommands
#define DASH_PCMD_GET_TMNS_UID				0xF3	//GetTerminusUID
#define DASH_PCMD_SET_EVNT_RECVR			0xF4	//SetEventReceiver
#define DASH_PCMD_GET_EVNT_RECVR			0xF5	//GetEventReceiver
#define DASH_PCMD_PLFM_EVNT_MSG				0x0A	//PlatformEventMessage
//Numeric Sensor Commands
#define DASH_PCMD_SET_NUM_SNR_EN			0x10	//SetNumericSensorEnable
#define DASH_PCMD_GET_SNR_READING			0x11	//GetSensorReading
#define DASH_PCMD_GET_SNR_THR				0x12	//GetSensorThresholds
#define DASH_PCMD_SET_SNR_THR				0x13	//SetSensorThresholds
#define DASH_PCMD_RST_SNR_HYS				0x14	//RestoreSensorThresholds
#define DASH_PCMD_GET_SNR_HYS				0x15	//GetSensorHysteresis
#define DASH_PCMD_SET_SNR_HYS				0x16	//SetSensorHysteresis
#define DASH_PCMD_INIT_NUM_SNR				0x17	//InitNumericSensor
//State Sensor Commands
#define DASH_PCMD_SET_STS_SNR_EN			0x20	//SetStateSensorEnables
#define DASH_PCMD_GET_STS_SNR_READING		0x21	//GetStateSensorReadings
#define DASH_PCMD_INIT_STS_SNR				0x22	//InitStateSensor
//PLDM Effeter Commands
#define DASH_PCMD_SET_NUM_EFF_EN			0x30	//SetNumbericEffetEnable
#define DASH_PCMD_SET_NUM_EFF_VAL			0x31	//SetNumericEffectValue
#define DASH_PCMD_GET_NUM_EFF_VAL			0x32	//GetNumericEffecterValue
#define DASH_PCMD_SET_STS_EFF_EN			0x38	//SetStateEffecterEnables
#define DASH_PCMD_SET_STS_EFF_STS			0x39	//SetStateEffecterStates
#define DASH_PCMD_GET_STS_EFF_STS			0x3A//GetStateEffecterStates
//PLDM Event Log Commands
#define DASH_PCMD_GET_PLDM_EVNT_LOG_INFO	0x40	//GetPLDMEventLogInfo
#define DASH_PCMD_EN_PLDM_EVNT_LOG			0x41	//EnablePLDMEventLogging
#define DASH_PCMD_CLR_PLDM_EVNT_LOG			0x42	//ClearPLDMEventLog
#define DASH_PCMD_GET_PLDM_EVNT_LOG_TSTMP	0x43	//GetPLDMEventLogTimestamp
#define DASH_PCMD_SET_PLDM_EVNT_LOG_TSTMP	0x44	//SetPLDMEventLogTimestamp
#define DASH_PCMD_READ_PLDM_EVNT_LOG		0x45	//ReadPLDMEventLog
#define DASH_PCMD_GET_PLDM_EVNT_LOG_POL_INFO 0x46	//GetPLDMEventLogPolicyInfo
#define DASH_PCMD_SET_PLDM_EVNT_LOG_POL		0x47	//SetPLDMEventLogPolicy
#define DASH_PCMD_FIND_PLDM_EVNT_LOG_ENTY	0x48	//FindPLDMEventLogEntry
//PDR Repository Commands
#define DASH_PCMD_GET_PDR_REP_INFO			0x50	//GetPDRRepositoryInfo
#define DASH_PCMD_GET_PDR					0x51	//GetPDR
#define DASH_PCMD_FIND_PDR					0x52	//FindPDR
#define DASH_PCMD_RUN_INIT_AGNT				0x58	//RunInitAgent


/********************
 *    Completion Codes
********************/
//#define DASH_COMP_CODE_PLDM_BASE_CODES			0x00
#define DASH_COMP_CODE_SUCCESS                        0x00
#define DASH_COMP_CODE_ERROR                          0x01
#define DASH_COMP_CODE_ERROR_INVALID_DATA             0x02
#define DASH_COMP_CODE_ERROR_INVALID_LENGTH           0x03
#define DASH_COMP_CODE_ERROR_NOT_READY                0x04
#define DASH_COMP_CODE_ERROR_UNSUPPORTED_PLDM_CMD     0x05  
#define DASH_COMP_CODE_ERROR_INVALID_PLDM_TYPE        0x20  
//GetSensorReading Command
#define DASH_COMP_CODE_INVALID_SENSOR_ID		      0x80
#define DASH_COMP_CODE_REARM_PRESENT_STATE		      0x81 	//REARM_UNAVAILABLE_IN_PRESENT_STATE



extern void Init_Smbus4Dash(void);
extern void Init_Dash(void);
extern void SmbusSlvMode(void);
extern void SmbusHstMode(void);
extern BYTE DashCrc8(BYTE crc, BYTE dat);
extern BYTE DashGetPEC(void);
extern BYTE ReadSensorData(WORD SnrID, DWORD *value);
//ivan extern BYTE SensorDataSize(void *vaddr, DWORD val, BYTE size);
//ivan dash extern BYTE SmbSlvRead(BYTE *recvbuff);
extern void SmbSlvRead(void);
//ivan dash extern void RecvDashPacket(void);
extern void SendDashPacket(void);
extern BYTE ProcessDASHCommand(void);
extern void DashMain(void);

//----------------------------------------------------------------------------
//PLDM Data Structure
//----------------------------------------------------------------------------

typedef struct _DashRxPkt
{
	//smbus format
	struct {
		BYTE cmd;									                  //offset 0
		BYTE bcnt;									                  //offset 1
		BYTE saddr;									                  //offset 2, source address
	}smbus;

	//mctp format
	struct {
		BYTE HdrVer:4, rsvd:4;				                  //offset 3
		BYTE deid;									                  //offset 4
		BYTE seid;									                  //offset 5
		BYTE MsgTag:3, to:1, PktSeg:2, eom:1, som:1;	  //offset 6,	Msg Tag
	}mctp;

	//pldm format
	struct {
		BYTE MsgType:7, ic:1;							           //offset 7,	Message Type, IC=0: No MCTP message integrity check, otherwise check it
		BYTE iid:5, reserved:1, d:1, rq:1;				    //offset 8,	Instance ID, Reserved, (Rq, D)=00b: response; =01b: request; =10b: reserved; =11b: unacknoledged
		BYTE PldmType:6, HdrVer:2;					           //offset 9,	PLDM Type, Header Version
		BYTE pcmd;									                  //offset 10,	PLDM Command Code
	}pldm;

	struct {
		BYTE SnrID;										                //offset 0~1,	Sensor ID
		BYTE SnrID1;	
		BYTE RearmEvntSts;								            //offset 2,	Rearm Event State
	}NumSensorRep;
	
}DashRxPkt;


typedef struct _DashTxPkt
{
	//smbus format
	struct {
		BYTE cmd;									//offset 0
		BYTE bcnt;									//offset 1
		BYTE saddr;									//offset 2, source address
	}smbus;

	//mctp format
	struct {
		BYTE HdrVer:4, rsvd:4;						//offset 3
		BYTE deid;									//offset 4
		BYTE seid;									//offset 5
		BYTE MsgTag:3, to:1, PktSeg:2, eom:1, som:1;	//offset 6,	Msg Tag
	}mctp;

	//pldm format
	struct {
		BYTE MsgType:7, ic:1;							//offset 7,	Message Type, IC=0: No MCTP message integrity check, otherwise check it
		BYTE iid:5, reserved:1, d:1, rq:1;				//offset 8,	Instance ID, Reserved, (Rq, D)=00b: response; =01b: request; =10b: reserved; =11b: unacknoledged
		BYTE PldmType:6, HdrVer:2;					//offset 9,	PLDM Type, Header Version
		BYTE pcmd;									//offset 10,	PLDM Command Code
		BYTE pcmp;									//offset 11,	PLDM Completion Code
	}pldm;
	
	struct {  
	  BYTE DataSize;									//offset 0
	  BYTE OptionalState;								//offset 1
	  BYTE EvntMsgEn;									//offset 2
	  BYTE PresentState;								//offset 3
	  BYTE PrevState;									//offset 4
	  BYTE EvntState;									//offset 5
	  WORD PresentReading;							//offset 6~9
  }NumSensorRsp;
}DashTxPkt;

//----------------------------------------------------------------------------
//Get Sensor Reading Data Structure
//----------------------------------------------------------------------------
/*
struct _NumSensorReq
{
	WORD SnrID;										//offset 0~1,	Sensor ID
	BYTE RearmEvntSts;								//offset 2,	Rearm Event State
};

struct _NumSensorRsp
{
	BYTE DataSize;									//offset 0
	BYTE OptionalState;								//offset 1
	BYTE EvntMsgEn;									//offset 2
	BYTE PresentState;								//offset 3
	BYTE PrevState;									//offset 4
	BYTE EvntState;									//offset 5
	DWORD PresentReading;							//offset 6~9
};
*/
#endif	


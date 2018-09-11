#include "sensor_fusion.h"

#define DI5112_ACC_FQ_REG       0x08
#define DI5112_ACC_FQ_REG_MASK  0x0F       

#define DATA_UNIT_G_ACC_X(n)    ((n)>>11)
#define DATA_UNIT_G_ACC_Y(n)    ((n)>>11)
#define DATA_UNIT_G_ACC_Z(n)    ((n)>>11)

const static UINT16 di5112_acc_freq[]             = {  32,   16,   8,     4,    2,   64,  128,  256, 0}; // Hz
const static UINT8 di5112_acc_freq_reg_value[]    = {0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x09, 0x0A};

static UINT8 gu8FregIdxAcc = 5;
static UINT8 gu8CurModeAcc;

static UINT8 di5112_acc_init(void)
{
	UINT8 u8data = 0;
    UINT8 l_init_status;

    //
    // pre-set init fail
    //
    l_init_status = 0x00;
    // Power on
    u8data = 0x00; // only written in standby mode
    sensor_write(DI5112_ACC_ID, 0x07, &u8data, 1);
    sensor_sleep(3);
	if(SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, 0x20, u8data, 0x15))
	{
		if(SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, DI5112_ACC_FQ_REG, u8data, di5112_acc_freq_reg_value[gu8FregIdxAcc]))
	    {
	    	if(SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, 0x07, u8data, 0x01))
	    	{
	    		l_init_status = 0x01;
	    	}
	    }
	}

    if(l_init_status)
    {
	    gu8CurModeAcc = 1;
    }

    return(l_init_status);
}

static inline void di5112_acc_shutdown(void)
{
	UINT8 u8data = 0x00;

	sensor_write(DI5112_ACC_ID, 0x07, &u8data, 1);
//		_delay(0x20);

	gu8CurModeAcc = 0;
	sensor_sleep(3);
}

//****************************************************************************************
// extern parameter & function for general use
//****************************************************************************************

// Initial Function
UINT8 do_initial_acc(UINT16 u16Freq)
{
	gu8FregIdxAcc = sensor_get_freq_idx(di5112_acc_freq, u16Freq);

	return(di5112_acc_init());
}

// Initial Calibrate Function
UINT8 do_init_calibrate_acc(UINT8 *pParam)
{ 	
    sensor_update_acc_bias(pParam[1]);

    return 0;
}

// Self-Test Function
UINT32 do_self_test_acc(void)
{
	UINT8 i;
	UINT8 u8data;
	UINT8 u8dataBuf[6];
	SINT32 data_tmp;
	SINT32 st_data[3] = {0};
	UINT8 u8Result;
	
	// Standby Mode
	u8data = 0x00;
	sensor_write(DI5112_ACC_ID, 0x07, &u8data, 1);
	
	// Set Range
	u8data = 0x15;
	sensor_write(DI5112_ACC_ID, 0x20, &u8data, 1);
	
	// Set ODR=128Hz 
	u8data = 0x09;
	sensor_write(DI5112_ACC_ID, 0x08, &u8data, 1);
	
	// Active Mode
	u8data = 0x01;
	sensor_write(DI5112_ACC_ID, 0x07, &u8data, 1);
	sensor_sleep(15);
	
	data_tmp = 0;
	for(i=0 ; i<256 ; i++)	// Dyna people suggest, just more than 100 and threshold less +/-0.1g
	{
		// Read Acc X/Y/Z
		sensor_read(DI5112_ACC_ID, 0x0D, &u8dataBuf[0], 6);
		
		data_tmp = ((SINT32)(*((SINT16*)&u8dataBuf[0])));
		st_data[0] = st_data[0] + data_tmp;
		data_tmp = ((SINT32)(*((SINT16*)&u8dataBuf[2])));
		st_data[1] = st_data[1] + data_tmp;
		data_tmp = ((SINT32)(*((SINT16*)&u8dataBuf[4])));
		st_data[2] = st_data[2] + data_tmp;
	}
	st_data[0] = ((st_data[0] >> 8) << 8);
	st_data[1] = ((st_data[1] >> 8) << 8);
	st_data[2] = ((st_data[2] >> 8) << 8);
	do_unit_correct_acc(st_data, st_data);
	
	if((abs(st_data[0]) <= 25) && (abs(st_data[1]) <= 25) && (abs(st_data[2]-256) <= 25)) 
	{
		u8Result = 0;
		rtl_printf("SUCCESS\r\n");
	}
	else
	{
		u8Result = 1;
		rtl_printf("FAILURE\r\n");
	}
	
	// Standby Mode
	u8data = 0x00;
	sensor_write(DI5112_ACC_ID, 0x07, &u8data, 1);
	
	return u8Result;
}

// Unit: G
void do_unit_correct_acc(SINT32 pBufIn[3], SINT32 pBufOut[3])
{ 
	pBufOut[0] =  DATA_UNIT_G_ACC_X(pBufIn[0]);
	pBufOut[1] =  DATA_UNIT_G_ACC_Y(pBufIn[1]);
	pBufOut[2] =  DATA_UNIT_G_ACC_Z(pBufIn[2]);
}

// Data Correction Function
void do_data_correct_acc(UINT8 *pBuf)
{

}

// Power Mode Control Function
void do_power_mode_switch_acc(UINT8 u8Mode)
{
	if(gu8CurModeAcc != u8Mode)
	{
		if(u8Mode == 0)
			di5112_acc_shutdown();
		else
			di5112_acc_init();
	}
}

// Data Rate Switch Function
BOOL do_data_rate_check_acc(UINT16 u16Freq, UINT8 *pu8FreqIdx)
{
	*pu8FreqIdx = sensor_get_freq_idx(di5112_acc_freq, u16Freq);

	return (gu8FregIdxAcc == *pu8FreqIdx);
}

UINT16 do_data_rate_switch_acc(UINT8 u8FreqIdx)
{
	UINT8 u8Data, u8Val;

	gu8FregIdxAcc = u8FreqIdx;
    
    if(gu8CurModeAcc == 1)
    {
    	sensor_read(DI5112_ACC_ID, DI5112_ACC_FQ_REG, &u8Val, 1);
    
    	CLEAR_MASK(u8Val, DI5112_ACC_FQ_REG_MASK);
    	SET_MASK(u8Val, di5112_acc_freq_reg_value[gu8FregIdxAcc]);
    	
    	SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, 0x07, u8Data, 0x00);
    	
    	SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, DI5112_ACC_FQ_REG, u8Data, u8Val);
    	
    	SENSOR_WRITE_WITH_CHECK(DI5112_ACC_ID, 0x07, u8Data, 0x01);
    }
	
	return di5112_acc_freq[gu8FregIdxAcc];
}

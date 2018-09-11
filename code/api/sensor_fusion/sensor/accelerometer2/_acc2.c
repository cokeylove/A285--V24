#include "sensor_fusion.h"

//	#define __ENABLE_BOSCH_INIT_CALIBRATION__

#define ACC2_FQ_REG           0x10
#define ACC2_FQ_REG_MASK      0x0F

#define DATA_UNIT_G_ACC2_X(n)    ((n)>>14)
#define DATA_UNIT_G_ACC2_Y(n)    ((n)>>14)
#define DATA_UNIT_G_ACC2_Z(n)    ((n)>>14)

#define BMA255_ACC2_RETRY_LOOP		200

const static UINT16 acc2_freq[]             = {  16,   32,   63,  125,  250,  500, 1000, 2000, 0}; // Hz
const static UINT8 acc2_freq_reg_value[]    = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

static UINT8 gu8FregIdxAcc2 = 3;
static UINT8 gu8CurModeAcc2;

static UINT8 bma255_acc2_init(void)
{
	UINT8 u8data = 0;
    UINT8 l_init_status;

    //
    // pre-set init fail
    //
    l_init_status = 0x00;
    //  Power on
    sensor_write(SLAVE_ADDRESS_ACC2, 0x11, &u8data, 1);
    sensor_sleep(5);
    //  power on done
	//if(SENSOR_WRITE_WITH_CHECK(SLAVE_ADDRESS_ACC2, 0x11, u8data, 0x00))
	{
	    if(SENSOR_WRITE_WITH_CHECK(SLAVE_ADDRESS_ACC2, 0x0F, u8data, 0x03))
	    {
	        if(SENSOR_WRITE_WITH_CHECK(SLAVE_ADDRESS_ACC2, ACC2_FQ_REG, u8data, acc2_freq_reg_value[gu8FregIdxAcc2]))
	        {
                l_init_status = 0x01;
            }
	    }
	}

    if(l_init_status)
    {
	    gu8CurModeAcc2 = 1;
    }

    return(l_init_status);
}

static inline void bma255_acc2_shutdown(void)
{
	UINT8 u8data = 0x20;

	sensor_write(SLAVE_ADDRESS_ACC2, 0x11, &u8data, 1);
//		_delay(0x20);

	gu8CurModeAcc2 = 0;
	sensor_sleep(3);
}

// ****************************************************************************************
// Function name: bma255_acc2_initial_calibration()
//
// Description:
//   Accelerometer Initial Calibration X/Y/Z.
// Arguments:
//   NULL.
// Return Values:
//   NULL.
// Note:
//   NULL.
// ****************************************************************************************
#ifdef __ENABLE_BOSCH_INIT_CALIBRATION__ // {
static inline UINT8 bma255_acc2_initial_calibration(UINT8 write_eeprom_flag, UINT8 acc_calibration_phase_mode)
{
	UINT8 u8data = 0;
	UINT8 u8data_acc_x_cal_mode = 0;
	UINT8 u8data_acc_y_cal_mode = 0;
	UINT8 u8data_acc_z_cal_mode = 0;
	UINT8 check_val = 0, u8Retry;

	switch(acc_calibration_phase_mode)
	{
			// {x,y,z} = {0,0,g}
		case 0 :
			u8data_acc_x_cal_mode = 0x00;
			u8data_acc_y_cal_mode = 0x00;
			u8data_acc_z_cal_mode = 0x20;
			break;

			// {x,y,z} = {0,0,-g}
		case 1 :
			u8data_acc_x_cal_mode = 0x00;
			u8data_acc_y_cal_mode = 0x00;
			u8data_acc_z_cal_mode = 0x40;
			break;

			// {x,y,z} = {g,0,0}
		case 2 :
			u8data_acc_x_cal_mode = 0x02;
			u8data_acc_y_cal_mode = 0x00;
			u8data_acc_z_cal_mode = 0x00;
			break;

			// {x,y,z} = {-g,0,0}
		case 3 :
			u8data_acc_x_cal_mode = 0x04;
			u8data_acc_y_cal_mode = 0x00;
			u8data_acc_z_cal_mode = 0x00;
			break;

			// {x,y,z} = {0,g,0}
		case 4 :
			u8data_acc_x_cal_mode = 0x00;
			u8data_acc_y_cal_mode = 0x08;
			u8data_acc_z_cal_mode = 0x00;
			break;

			// {x,y,z} = {0,-g,0}
		case 5 :
			u8data_acc_x_cal_mode = 0x00;
			u8data_acc_y_cal_mode = 0x10;
			u8data_acc_z_cal_mode = 0x00;
			break;

			// {x,y,z} = {0,0,g}
		default :
			u8data_acc_x_cal_mode = 0x00;
			u8data_acc_y_cal_mode = 0x00;
			u8data_acc_z_cal_mode = 0x20;
			break;
	}

	// Set 2g
	u8data = 0x03;
	sensor_write(BMA255_ACC2_ID, 0x0F, &u8data, 1);
	_delay(0x10);

	// Set 31Hz
	u8data = 0x0A;
	sensor_write(BMA255_ACC2_ID, 0x10, &u8data, 1);
	_delay(0x10);

	// ----- Reset Old Calibration Information -----
	u8data = 0x80;
	sensor_write(BMA255_ACC2_ID, 0x36, &u8data, 1);
	_delay(0x10);

	// ----- Calibration On X-Axis -----
	u8data = u8data_acc_x_cal_mode;
	sensor_write(BMA255_ACC2_ID, 0x37, &u8data, 1);
	_delay(0x10);
	u8data = 0x20;
	sensor_write(BMA255_ACC2_ID, 0x36, &u8data, 1);
	_delay(0x10);

	// Check Calibration On X-Axis Done
	SENSOR_READ_WITH_CHECK(BMA255_ACC2_ID, 0x36, u8data, 0x10);


	// ----- Calibration On Y-Axis -----
	u8data = u8data_acc_y_cal_mode;
	sensor_write(BMA255_ACC2_ID, 0x37, &u8data, 1);
	_delay(0x10);
	u8data = 0x40;
	sensor_write(BMA255_ACC2_ID, 0x36, &u8data, 1);
	_delay(0x10);

	// Check Calibration On Y-Axis Done
	SENSOR_READ_WITH_CHECK(BMA255_ACC2_ID, 0x36, u8data, 0x10);


	// ----- Calibration On Z-Axis -----
	u8data = u8data_acc_z_cal_mode;
	sensor_write(BMA255_ACC2_ID, 0x37, &u8data, 1);
	_delay(0x10);
	u8data = 0x60;
	sensor_write(BMA255_ACC2_ID, 0x36, &u8data, 1);
	_delay(0x10);

	// Check Calibration On Z-Axis Done
	SENSOR_READ_WITH_CHECK(BMA255_ACC2_ID, 0x36, u8data, 0x10);

	// ----- Store Calibration Data Into EEPROM -----
	u8Retry = 0;
	if(write_eeprom_flag)
	{
		u8data = 0x00;
		sensor_read(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);
		u8data = u8data | (0x01);
		sensor_write(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);

		u8data = 0x00;
		sensor_read(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);
		u8data = u8data | (0x02);
		sensor_write(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);

		// Check Stored Done
		check_val = 0x04;
		do
		{
			u8data = 0x00;
			sensor_read(BMA255_ACC2_ID, 0x33, &u8data, 1);
			_delay(0x10);
			u8data = (u8data & 0x04);
			rtl_printf("%02Xh -> %02Xh\r\n", u8data, check_val);
			
			if(u8Retry++ > BMA255_ACC2_RETRY_LOOP)
            	return 2;
            	
		}
		while(u8data != check_val);

		u8data = 0x00;
		sensor_read(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);
		u8data = u8data & (0xFE);
		sensor_write(BMA255_ACC2_ID, 0x33, &u8data, 1);
		_delay(0x10);
	}

	// Get EEPROM Count
	sensor_read(BMA255_ACC2_ID, 0x33, &u8data, 1);
	u8data >>= 4;

	rtl_printf("bma255_acc: calibration(%d, %d) -> %d.\r\n", write_eeprom_flag, acc_calibration_phase_mode, u8data);

	return u8data;
}
#endif // } __ENABLE_BOSCH_INIT_CALIBRATION__

// ****************************************************************************************
// Function name: bma255_acc2_self_test(()
//
// Description:
//   Accelerometer Self-Test.
// Arguments:
//   NULL.
// Return Values:
//   Test Result.
// Note:
//   NULL.
// ****************************************************************************************
static inline UINT8 bma255_acc2_self_test(void)
{
	UINT8 u8DataBuf[2];
	UINT8 u8data = 0;
	SINT8 acc_l = 0;
	SINT8 acc_h = 0;
	SINT32 acc_data_p = 0;
	SINT32 acc_data_n = 0;
	SINT32 acc_xdata_diff = 0;
	SINT32 acc_ydata_diff = 0;
	SINT32 acc_zdata_diff = 0;
	UINT8 u8Result;

	// Soft Reset
	u8data = 0xB6;
	sensor_write(BMA255_ACC2_ID, 0x14, &u8data, 1);
	sensor_delay_ms(50);

	// ----- Reset And Initial -----
	u8data = 0x00;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);

	// Set 8g;
	u8data = 0x08;
	sensor_write(BMA255_ACC2_ID, 0x0F, &u8data, 1);
	sensor_delay_ms(50);

	// ----- Self-Test On X-Axis (g,0,0)-----
	// Self-Test Excitation As '1' -> Positive
	u8data = 0x15;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read X-Axis Data For Positive
	sensor_read(BMA255_ACC2_ID, (0x02 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_p = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_p = (acc_data_p << 8) >> 12;

	// Self-Test Excitation As '0' -> Nagative
	u8data = 0x11;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read X-Axis Data For Nagative
	sensor_read(BMA255_ACC2_ID, (0x02 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_n = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_n = (acc_data_n << 8) >> 12;

	// Positive - Nagative
	acc_xdata_diff = acc_data_p - acc_data_n;
	rtl_printf("X(%d:%d:%d)\r\n", acc_data_p, acc_data_n, acc_xdata_diff);

	// Reset And Initial
	u8data = 0x00;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// ----- Self-Test On Y-Axis (0,g,0)-----
	// Self-Test Excitation As '1' -> Positive
	u8data = 0x16;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read Y-Axis Data For Positive
	sensor_read(BMA255_ACC2_ID, (0x04 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_p = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_p = (acc_data_p << 8) >> 12;

	// Self-Test Excitation As '0' -> Nagative
	u8data = 0x12;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read Y-Axis Data For Nagative
	sensor_read(BMA255_ACC2_ID, (0x04 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_n = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_n = (acc_data_n << 8) >> 12;

	// Positive - Nagative
	acc_ydata_diff = acc_data_p - acc_data_n;
	rtl_printf("Y(%d:%d:%d)\r\n", acc_data_p, acc_data_n, acc_ydata_diff);

	// Reset And Initial
	u8data = 0x00;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// ----- Self-Test On Z-Axis (0,0,g)-----
	// Self-Test Excitation As '1' -> Positive
	u8data = 0x17;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read Z-Axis Data For Positive
	sensor_read(BMA255_ACC2_ID, (0x06 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_p = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_p = (acc_data_p << 8) >> 12;

	// Self-Test Excitation As '0' -> Nagative
	u8data = 0x13;
	sensor_write(BMA255_ACC2_ID, 0x32, &u8data, 1);
	sensor_delay_ms(50);

	// Read Z-Axis Data For Nagative
	sensor_read(BMA255_ACC2_ID, (0x06 + 0), &u8DataBuf[0], 2);
	acc_l = u8DataBuf[0];
	acc_h = u8DataBuf[1];
	acc_data_n = (((SINT32)acc_h) << 8) | (acc_l & 0x000000FF);
	acc_data_n = (acc_data_n << 8) >> 12;

	// Positive - Nagative
	acc_zdata_diff = acc_data_p - acc_data_n;
	rtl_printf("Z(%d:%d:%d)\r\n", acc_data_p, acc_data_n, acc_zdata_diff);

	if((acc_xdata_diff > 206) && (acc_ydata_diff > 206) && (acc_zdata_diff > 103))
	{
		u8Result = 0;
		rtl_printf("SUCCESS\r\n");
	}
	else
	{
		u8Result = 1;
		rtl_printf("FAILURE\r\n");
	}

	// Soft Reset
	u8data = 0xB6;
	sensor_write(BMA255_ACC2_ID, 0x14, &u8data, 1);
	sensor_delay_ms(50);

	return u8Result;
}

//****************************************************************************************
// extern parameter & function for general use
//****************************************************************************************

// Initial Function
UINT8 do_initial_acc2(UINT16 u16Freq)
{
	gu8FregIdxAcc2 = sensor_get_freq_idx(acc2_freq, u16Freq);

	return(bma255_acc2_init());
}

// Initial Calibrate Function
UINT8 do_init_calibrate_acc2(UINT8 *pParam)
{
    #ifdef __ENABLE_BOSCH_INIT_CALIBRATION__ // {
	UINT8 u8Rtn;

	u8Rtn = bma255_acc2_initial_calibration(pParam[0], pParam[1]);

	sensor_update_acc2_bias(6); // reset bias buffer

	return u8Rtn;
    #else
	sensor_update_acc2_bias(pParam[1]);
    
    return 0;
    #endif // } __ENABLE_BOSCH_INIT_CALIBRATION__    
}

// Self-Test Function
UINT32 do_self_test_acc2(void)
{
	return bma255_acc2_self_test();
}

// Unit: G
void do_unit_correct_acc2(SINT32 pBufIn[3], SINT32 pBufOut[3])
{
	pBufOut[0] =  DATA_UNIT_G_ACC2_X(pBufIn[0]);
	pBufOut[1] =  DATA_UNIT_G_ACC2_Y(pBufIn[1]);
	pBufOut[2] =  DATA_UNIT_G_ACC2_Z(pBufIn[2]);
}

// Data Correction Function
void do_data_correct_acc2(UINT8 *pBuf)
{

}

// Power Mode Control Function
void do_power_mode_switch_acc2(UINT8 u8Mode)
{
	if(gu8CurModeAcc2 != u8Mode)
	{
		if(u8Mode == 0)
			bma255_acc2_shutdown();
		else
			bma255_acc2_init();
	}
}

// Data Rate Switch Function
BOOL do_data_rate_check_acc2(UINT16 u16Freq, UINT8 *pu8FreqIdx)
{
	*pu8FreqIdx = sensor_get_freq_idx(acc2_freq, u16Freq);

	return (gu8FregIdxAcc2 == *pu8FreqIdx);
}

UINT16 do_data_rate_switch_acc2(UINT8 u8FreqIdx)
{
	UINT8 u8Data, u8Val;

	gu8FregIdxAcc2 = u8FreqIdx;

    if(gu8CurModeAcc2 == 1)
    {
    	sensor_read(SLAVE_ADDRESS_ACC2, ACC2_FQ_REG, &u8Val, 1);

    	CLEAR_MASK(u8Val, ACC2_FQ_REG_MASK);
    	SET_MASK(u8Val, acc2_freq_reg_value[gu8FregIdxAcc2]);

    	SENSOR_WRITE_WITH_CHECK(SLAVE_ADDRESS_ACC2, ACC2_FQ_REG, u8Data, u8Val);
    }

	return acc2_freq[gu8FregIdxAcc2];
}

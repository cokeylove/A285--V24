#include "sensor_fusion.h"

#define DATA_UNIT_LUX_ALS(n)    ((n)*513/1000) //0.513

static UINT8 gu8CurModeAls;

static UINT8 di5112_als_init(void)
{
	UINT8 u8data;
    UINT8 l_init_status;

    //
    // pre-set init fail
    //
    l_init_status = 0x00;

	u8data = 0x04; // Soft-Reset
	if(sensor_write(DI5112_ALS_ID, 0x00, &u8data, 1)==0)
	{
	    sensor_sleep(3);
    	u8data = 0x00;
    	if(sensor_write(DI5112_ALS_ID, 0x06, &u8data, 1)==0)
    	{
          	u8data = 0x01;
        	if(sensor_write(DI5112_ALS_ID, 0x07, &u8data, 1)==0)
        	{
            	u8data = 0x0F;
            	if(sensor_write(DI5112_ALS_ID, 0x09, &u8data, 1)==0)
            	{
                	u8data = 0x00;
                	if(sensor_write(DI5112_ALS_ID, 0x02, &u8data, 1)==0)
                	{
                	    u8data = 0x01;
                	    if(sensor_write(DI5112_ALS_ID, 0x00, &u8data, 1)==0)
                	    {
                	        sensor_sleep(3);
                            l_init_status = 0x01;
                        }
                	}
            	}
        	}
    	}
	}

    if(l_init_status)
    {
	    gu8CurModeAls = 1;
    }

    return(l_init_status);
}

static inline void di5112_als_shutdown(void)
{
	UINT8 u8data;

	// ALS
	u8data = 0x00;

	// Format        ID    Address  data
	sensor_write(DI5112_ALS_ID, 0x00, &u8data, 1);

	gu8CurModeAls = 0;
	sensor_sleep(3);
}

//****************************************************************************************
// extern parameter & function for general use
//****************************************************************************************

// Initial Function
UINT8 do_initial_als(UINT16 u16Freq)
{
	return(di5112_als_init());
}

// Data Correction Function
void do_data_correct_als(UINT8 *pBuf)
{

}

// Unit: LUX
UINT32 do_unit_correct_als(UINT32 u32Data)
{
    return DATA_UNIT_LUX_ALS(u32Data);
}

// Power Mode Control Function
void do_power_mode_switch_als(UINT8 u8Mode)
{
	if(gu8CurModeAls != u8Mode)
	{
		if(u8Mode == 0)
			di5112_als_shutdown();
		else
			di5112_als_init();
	}
}

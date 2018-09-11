/*-----------------------------------------------------------------------------
 * TITLE: OEM_GSNS.C
 * TITLE: OEM_GSNS.H
 *
 * Author : Ivan Chen
 *
 * spec : support chip
           1.LIS3DH
           2.LIS3DSH
           3.KX023
            
*---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************



#if Support_LenovoAPS && Support_GSNS_I2C
BYTE GSENSOR_ADDRESS, GSENSOR_OUT_X_L, GSENSOR_OUT_X_H, GSENSOR_OUT_Y_L, GSENSOR_OUT_Y_H;
void GSensorDeviceID(void) {

  if ( IS_MASK_CLEAR(GS.Status,GS_Enable) ) { // Scan G-sensor raw data if is enabled.  
    return;
  }

	if(GSSmbusFailCnt==0)
	{
		GSSmbusFailCnt++;
	}
	if(GSSmbusFailCnt)
	{
		if (!bRWSMBus(chSMB_GS, SMbusRB, LIS3DH_ADDRESS, Who_AM_I, &GSDeviceID, 0)) {
	    GSSmbusFailCnt++;
		}
		else
		{
	  	GSSmbusFailCnt=0;
		}
	}
	if(GSSmbusFailCnt)
	{
		if (!bRWSMBus(chSMB_GS, SMbusRB, LIS3DSH_ADDRESS, Who_AM_I, &GSDeviceID, 0)){
      GSSmbusFailCnt++;
    }
		else
		{
    	GSSmbusFailCnt=0;
		}
	}
	if(GSSmbusFailCnt)
	{
	  if (!bRWSMBus(chSMB_GS, SMbusRB, KX022_ADDRESS, Who_AM_I, &GSDeviceID, 0)){
      GSSmbusFailCnt++;
    }
		else
		{
    	GSSmbusFailCnt=0;
		}
	}
	if(GSSmbusFailCnt)
	{
	  if (!bRWSMBus(chSMB_GS, SMbusRB, KX023_ADDRESS, Who_AM_I, &GSDeviceID, 0)){
      GSSmbusFailCnt++;
    }
		else
		{
    	GSSmbusFailCnt=0;
		}
	}
#if Support_BMA255
	if(GSSmbusFailCnt)
	{
	  if (!bRWSMBus(chSMB_GS, SMbusRB, BMA255_ADDRESS, BMA255_BGW_CHIPID, &GSDeviceID, 0)){
      GSSmbusFailCnt++;
    }
		else
		{
    	GSSmbusFailCnt=0;
		}
	}
#endif

}

void InitGSensor (
  void
)
{
  BYTE  SmbBuff, SmbBuff2;
  BYTE GSENSOR_CTRL_REGa, GSENSOR_CTRL_REGb;

  SmbBuff = 0;
  SmbBuff2 = 0;
  
	switch (GSDeviceID)
	{
	case LIS3DH_WHO_AM_I:
    SmbBuff = LIS3DH_CTRL_REG1_ODR_400HZ  |
              LIS3DH_CTRL_REG1_Z_EN       |
              LIS3DH_CTRL_REG1_Y_EN       |
              LIS3DH_CTRL_REG1_X_EN;
    SmbBuff2 = LIS3DH_CTRL_REG4_FS_2G   |
               LIS3DH_CTRL_REG4_BDU     |
               LIS3DH_CTRL_REG4_HR;
    
    GSENSOR_ADDRESS = LIS3DH_ADDRESS;
		GSENSOR_CTRL_REGa = LIS3DH_CTRL_REG1;
		GSENSOR_CTRL_REGb = LIS3DH_CTRL_REG4;
		GSENSOR_OUT_X_L = LIS3DH_OUT_X_L | 0x80;	//Set bit7 to 1 for enable read block mode.
//		GSENSOR_OUT_X_H = LIS3DH_OUT_X_H;
//		GSENSOR_OUT_Y_L = LIS3DH_OUT_Y_L;
//		GSENSOR_OUT_Y_H = LIS3DH_OUT_Y_H;
		break;

	case LIS3DSH_WHO_AM_I:
    SmbBuff = LIS3DSH_CTRL_REG4_ODR_400HZ  |
              LIS3DSH_CTRL_REG4_BDU        |
              LIS3DSH_CTRL_REG4_Z_EN       |
              LIS3DSH_CTRL_REG4_Y_EN       |
              LIS3DSH_CTRL_REG4_X_EN;
    SmbBuff2 = LIS3DSH_CTRL_REG5_FS_2G   |
               LIS3DSH_CTRL_REG5_HR;
    
		GSENSOR_ADDRESS = LIS3DSH_ADDRESS;
		GSENSOR_CTRL_REGa = LIS3DSH_CTRL_REG4;
		GSENSOR_CTRL_REGb = LIS3DSH_CTRL_REG5;
		GSENSOR_OUT_X_L = LIS3DSH_OUT_X_L;
//		GSENSOR_OUT_X_H = LIS3DSH_OUT_X_H;
//		GSENSOR_OUT_Y_L = LIS3DSH_OUT_Y_L;
//		GSENSOR_OUT_Y_H = LIS3DSH_OUT_Y_H;
		break;
	case KX022_WHO_AM_I:
    SmbBuff = KX023_ODCNTL_OSA_400;
    SmbBuff2 = KX023_CNTL1_GSEL_2G  |
               KX023_CNTL1_RES      |
               KX023_CNTL1_PC1;
    
		GSENSOR_ADDRESS = KX022_ADDRESS;
		GSENSOR_CTRL_REGa = KX023_ODCNTL;
		GSENSOR_CTRL_REGb = KX023_CNTL1;
		GSENSOR_OUT_X_L = KX023_XOUT_L;
//		GSENSOR_OUT_X_H = KX023_XOUT_H;
//		GSENSOR_OUT_Y_L = KX023_YOUT_L;
//		GSENSOR_OUT_Y_H = KX023_YOUT_H;
		break;
	case KX023_WHO_AM_I:
    SmbBuff = KX023_ODCNTL_OSA_400;
    SmbBuff2 = KX023_CNTL1_GSEL_2G  |
               KX023_CNTL1_RES      |
               KX023_CNTL1_PC1;
    
		GSENSOR_ADDRESS = KX023_ADDRESS;
		GSENSOR_CTRL_REGa = KX023_ODCNTL;
		GSENSOR_CTRL_REGb = KX023_CNTL1;
		GSENSOR_OUT_X_L = KX023_XOUT_L;
//		GSENSOR_OUT_X_H = KX023_XOUT_H;
//		GSENSOR_OUT_Y_L = KX023_YOUT_L;
//		GSENSOR_OUT_Y_H = KX023_YOUT_H;
		break;
#if Support_BMA255
	case BMA255_BGW_CHIPID_VALUE:
    SmbBuff = BMA255_PMU_BW_250HZ;
    SmbBuff2 = BMA255_BGW_SOFTRESET_VALUE;
    
		GSENSOR_ADDRESS = BMA255_ADDRESS;
		GSENSOR_CTRL_REGa = BMA255_PMU_BW;
		GSENSOR_CTRL_REGb = BMA255_BGW_SOFTRESET;
		GSENSOR_OUT_X_L = BMA255_ACCD_X_LSB;
    GSENSOR_OUT_Y_L = BMA255_ACCD_Y_LSB;
		break;
#endif
	}


  if (!bRWSMBus(chSMB_GS, SMbusWB, GSENSOR_ADDRESS, GSENSOR_CTRL_REGa, &SmbBuff, 0)) {
    GSSmbusFailCnt++;
    return;
  }
	else
	{
  	GSSmbusFailCnt=0;
	}
	
	if (!bRWSMBus(chSMB_GS, SMbusWB, GSENSOR_ADDRESS, GSENSOR_CTRL_REGb, &SmbBuff2, 0)) {
		GSSmbusFailCnt++;
		return;
	}
	else
	{
  	GSSmbusFailCnt=0;
	}

  SET_MASK(GS.Status, GS_Init);
}


void ScanGsensorI2C (
  void
)
{
  WORD  TmpVal;
  BYTE  TempVal[4];
//  BYTE  *TmpValL = ((BYTE *)(&TmpVal)+1);
//  BYTE  *TmpValH = (BYTE *)(&TmpVal);
  if ( IS_MASK_CLEAR(GS.Status,GS_Enable) || !Read_SUSP() || (GsensorStableDelay > 0)) {
    return;
  }
	if (GSDeviceID == 0x00){
		GSensorDeviceID();
	}
	
  if (IS_MASK_CLEAR(GS.Status, GS_Init) && SystemIsS0){
    InitGSensor();
    return;
  }


  if(IS_MASK_SET(GSensorSelfTestState,StartTest)){
#if Support_BMA255
    BMA255_SelfTest();
    TempVal[0] = GSensorTmpX_1;
    TempVal[1] = GSensorTmpX_2;
    TempVal[2] = GSensorTmpY_1;
    TempVal[3] = GSensorTmpY_2;
#endif
  }
  else
  {
  //
  // Read X and Y axis value
  //
		rI2C_DATA = GSENSOR_OUT_X_L;
     if (!bRSMBusBlock(chSMB_GS, SMbusRBK, GSENSOR_ADDRESS, rI2C_DATA, &TempVal[0], 4))
		{
    		GSSmbusFailCnt++;
		}
		else
		{
    		GSSmbusFailCnt=0;
		}
  }
		
  
  TmpVal = TempVal[1]*256+TempVal[0];
	if(TmpVal&0x8000)
	{
		TmpVal = (WORD)(0xFFFF - TmpVal + 1);
		TmpVal = (WORD)(0xFFFF- ((TmpVal>>4)/5) + 1);
		GS_VOUTX = TmpVal + 512;
	}
	else
	{
		GS_VOUTX = ((TmpVal>>4)/5) + 512;
	}
  GS.Buf1X[GS.Buf1X_E] = GS_VOUTX;

  if ( ++GS.Buf1X_E == BUF1_SIZE )
  {
    GS.Buf1X_E = 0;
  }

  if ( GS.Buf1X_N < BUF1_SIZE )
  {
    GS.Buf1X_N ++;
  }



  TmpVal = TempVal[3]*256+TempVal[2];
	if(TmpVal&0x8000)
	{
		TmpVal = (WORD)(0xFFFF - TmpVal + 1);
		TmpVal = (WORD)(0xFFFF- ((TmpVal>>4)/5) + 1);
		GS_VOUTY = TmpVal + 512;
	}
	else
	{
		GS_VOUTY = ((TmpVal>>4)/5) + 512;
	}
  GS.Buf1Y[GS.Buf1Y_E] = GS_VOUTY;

  if ( ++GS.Buf1Y_E == BUF1_SIZE )
  {
    GS.Buf1Y_E = 0;
  }

  if ( GS.Buf1Y_N < BUF1_SIZE )
  {
    GS.Buf1Y_N ++;
  }
}
#else

void ScanGsensorADC(void)
{
#if Support_LenovoAPS
  BYTE i;
  unsigned long int TmpVal = 0; // For value calculating > XWORD

  if ( IS_MASK_CLEAR(GS.Status,GS_Enable) ) { // Scan G-sensor raw data if is enabled.  
    return;
  }

  if (VCH6CTL & BIT7)
  {
    VCH6CTL |= BIT7;        // write clear data vaild flag
    TmpVal = VCH6DATM;  // MSB
    TmpVal = ((TmpVal << 8) + VCH6DATL); // (MSB << 8) + LSB
    TmpVal = TmpVal * 3 / 3.3; // Re-calculate input range from 3.3v to 3v.
    GS_VOUTX = TmpVal;
    GS.Buf1X[GS.Buf1X_E] = GS_VOUTX;

    if ( ++GS.Buf1X_E == BUF1_SIZE )
    {
      GS.Buf1X_E = 0;
    }

    if ( GS.Buf1X_N < BUF1_SIZE )
    {
      GS.Buf1X_N ++;
    }
  }



  if( VCH1CTL & BIT7)
  {
    VCH1CTL |= BIT7;        // write clear data vaild flag
    TmpVal = VCH1DATM;  // MSB
    TmpVal = ((TmpVal << 8) + VCH1DATL);  // (MSB << 8) + LSB
    TmpVal = TmpVal * 3 / 3.3;  // Re-calculate input range from 3.3v to 3v.
    GS_VOUTY = TmpVal;
    GS.Buf1Y[GS.Buf1Y_E] = GS_VOUTY;

    if ( ++GS.Buf1Y_E == BUF1_SIZE )
    {
      GS.Buf1Y_E = 0;
    }

    if ( GS.Buf1Y_N < BUF1_SIZE )
    {
      GS.Buf1Y_N ++;
    }
  }
#endif  
}
#endif


void Gsensor_Initial(void)
{
  #if Support_LenovoAPS
  BYTE i;
  for( i=0; i < 17; i++ )
  {
    BRAM[0x20+i] = 0x00;
  }
  #endif  // Support_LenovoAPS
}  

#if Support_LenovoAPS
// For Lenovo G-sensor  =====================================================
/*============================================
     Command 10h-1Fh, For Lenovo G-sensor
==============================================*/
void APS_Cmd_10(BYTE nPort)  // start/stop status report
{
  GS_D0 = 0x10;
  GS.SampleRate = (GS_D2<<8) | GS_D1;
  GS.Factor = GS_D3;

  if( GS.SampleRate )
  {
    SET_MASK(GS.Status,GS_Enable);
    CLEAR_MASK(GS.Status,GS_Request);

    // Initial after get enable request
    GS.Buf1X_S = 0;
    GS.Buf1X_E = 0;
    GS.Buf1X_N = 0;
    GS.Buf1Y_S = 0;
    GS.Buf1Y_E = 0;
    GS.Buf1Y_N = 0;



    GS.Buf2X_S = 0;
    GS.Buf2X_E = 0;
    GS.Buf2X_N = 0;
    GS.Buf2Y_S = 0;
    GS.Buf2Y_E = 0;
    GS.Buf2Y_N = 0;

    uAPSLastX=0;
    uAPSLastY=0;

    uCountX=0;
    uTotalX=0;
    uCountY=0;
    uTotalY=0;

    uAPSThresholdX=0x04;
    uAPSThresholdY=0x04;
    uAPSTimeOut=20; // 5ms unit.

  }
  else
  {
    CLEAR_MASK(GS.Status,GS_Enable);
    CLEAR_MASK(GS.Status,GS_Request);
  }
  GS_DF = 0;

  Data2Port(nPort,0x00);
}

void APS_Cmd_11(BYTE nPort)  // Get Data
{
  BYTE store_n;
  GS_D0 = 0x11;


  GS_D1 = 0;
  GS_D2 = 0;
  GS_D3 = 0;
  GS_D4 = 0;
  GS_D5 = 0;
  GS_D6 = u4CpuThermal;
  GS_D7 = 0;
  GS_D8 = 0;
  GS_D9 = 0;
  GS_DA = 0;
  GS_DB = u4CpuThermal;

  if( GS.Buf2X_N < GS.Buf2Y_N )
    store_n = GS.Buf2X_N;
  else
    store_n = GS.Buf2Y_N;
  if( store_n )
  {
    GS_D1++;  //GS.DIO[1]++;
    // bute 1
    GS_D2 = GS.Buf2X[GS.Buf2X_S] & 0xFF;
    GS_D3 = GS.Buf2X[GS.Buf2X_S] >> 8;
    if( ++GS.Buf2X_S == BUF2_SIZE )
      GS.Buf2X_S = 0;
    GS.Buf2X_N --;
    GS_D4 = GS.Buf2Y[GS.Buf2Y_S] & 0xFF;
    GS_D5 = GS.Buf2Y[GS.Buf2Y_S] >> 8;
    if( ++GS.Buf2Y_S == BUF2_SIZE )
      GS.Buf2Y_S = 0;
    GS.Buf2Y_N --;

    if( store_n > 1 && GS.Buf2X[GS.Buf2X_S] > 0x00)
    {
      GS_D1++;
      // bute 2
      GS_D7 = GS.Buf2X[GS.Buf2X_S] & 0xFF;
      GS_D8 = GS.Buf2X[GS.Buf2X_S] >> 8;
      if( ++GS.Buf2X_S == BUF2_SIZE )
        GS.Buf2X_S = 0;
      GS.Buf2X_N --;
      GS_D9 = GS.Buf2Y[GS.Buf2Y_S] & 0xFF;
      GS_DA = GS.Buf2Y[GS.Buf2Y_S] >> 8;
      if( ++GS.Buf2Y_S == BUF2_SIZE )
        GS.Buf2Y_S = 0;
      GS.Buf2Y_N --;
    }
  }
  GS_DC = store_n - GS_D1;  //GS.DIO[0xC] = store_n - GS.DIO[1];

  if( IS_MASK_SET(SysStatus,CloseLid) ) //if( lidSWpres )    // Bit7: Lid status(0:Close, 1:Open)
    GS_DD &=~(BIT7);
  else
    GS_DD |= BIT7;

  if ( PSSTS3&0x0C )  //Bit6: Mouse Data bit.PS2 Port3
    GS_DD |= BIT6;
  else
    GS_DD &= ~(BIT6);

  if( typematic.byte != 0x00 ) //Bit5: Keyboard Data bit.
    GS_DD |= BIT5;
  else
    GS_DD &= ~(BIT5);

  GS_DF = 0;

//  GS_D10 &= 0xDF;

  Data2Port(nPort,0x00);
}

void APS_Cmd_12(BYTE nPort)  // Diag, self-test
{

  GS_D0 = 0x12;
  //jim, not implement yet
  GS_DF = 0;


  Data2Port(nPort,0x00);
}

void APS_Cmd_13(BYTE nPort)  // Get sensor type
{
  GS_D0 = 0x13;
  GS_D1 = 1;  // Maker ID, ADI (ANA_DEV)
  GS_D2 = 2+1;// Feature: (MASK_PWR_CTRL_SUPPORT+MASK_DIAG_SUPPORT)
  GS_D3 = 0;  // LPF implementation (Low)
  GS_D4 = 0;  // LPF implementation (High)
  GS_DF = 0;

  Data2Port(nPort,0x00);
}

void APS_Cmd_14(BYTE nPort)  // G-sensor power control
{
  GS_D0 = 0x14;
  if( GS_D1 ) {
    ECQEvent(GSENSOR_PWRON_7D);
#if Support_EC_GS_ON    
    EC_GS_ON();
#endif //Support_EC_GS_ON
  } else {
    ECQEvent(GSENSOR_PWROFF_7E);
//#if Support_EC_GS_ON        
    EC_GS_OFF();
//#endif //Support_EC_GS_ON    
  }
  GS_DF = 0;

  Data2Port(nPort,0x00);
}

void APS_Cmd_15(BYTE nPort)  // Get calibration data -- None-use yet
{
  GS_D0 = 0x15;

  GS_DF = 0;

  Data2Port(nPort,0x00);
}

void APS_Cmd_16(BYTE nPort)  // Load calibration data to ITE -- None-use yet
{
  GS_D0 = 0x16;

  GS_DF = 0;

  Data2Port(nPort,0x00);
}

void APS_Cmd_17(BYTE nPort)  // Extended command
{
  GS_D0 = 0x17;
  switch( GS_D1 )
  {
    case 0x80: // Hibernation entry timing notice -- None-use yet
      break;
    case 0x81: // Get module version number
      GS_D1 = 0x01;  // MAJOR_VERSION    //0Dh
      GS_D2 = 0x00;  // MINOR_VERSION
      GS_D3 = 0x00;  // SUFIX
      break;
    case 0x82: // Get G-sensor monitor status
      GS_D1 = GS.Status;
      GS_D2 = GS.SampleRate & 0xFF;
      GS_D3 = GS.SampleRate >> 8;
      GS_D4 = GS.Factor;
      break;
  }
  GS_DF = 0;
  SET_MASK(PM2STS,GTIM);
  Data2Port(nPort,0x00);
}

void APS_Cmd_18(BYTE nPort)  // ST pin control command
{
#if Support_GSNS_I2C
  BYTE SmbBuff;

  GS_D0 = 0x18;

	switch (GSDeviceID)
	{
	case LIS3DH_WHO_AM_I:
		SmbBuff = 0;
		SmbBuff = LIS3DH_CTRL_REG4_FS_2G |    // +-2G
            LIS3DH_CTRL_REG4_BDU   |
            LIS3DH_CTRL_REG4_HR;
		SmbBuff &= ~LIS3DH_CTRL_REG4_ST_MASK;
		if(GS_D1)
			SmbBuff |= LIS3DH_CTRL_REG4_ST_SELF_TEST0;

		if(!bRWSMBus(chSMB_GS, SMbusWB, LIS3DH_ADDRESS, LIS3DH_CTRL_REG4, &SmbBuff, 0)){
			GSSmbusFailCnt++;
		}
		else
		{
    	GSSmbusFailCnt=0;
		}
		break;

	case LIS3DSH_WHO_AM_I:
		SmbBuff = 0;
		SmbBuff = LIS3DSH_CTRL_REG5_FS_2G	|
						 LIS3DSH_CTRL_REG5_HR;
		SmbBuff &= LIS3DSH_CTRL_REG5_ST_MASK;
		if(GS_D1)
			SmbBuff |= LIS3DSH_CTRL_REG5_ST_SELF_TEST0;

		if(!bRWSMBus(chSMB_GS, SMbusWB, LIS3DSH_ADDRESS, LIS3DSH_CTRL_REG5, &SmbBuff, 0)){
			GSSmbusFailCnt++;
		}
		else
		{
    	GSSmbusFailCnt=0;
		}
		break;
	case KX022_WHO_AM_I:
		SmbBuff = 0;
		if(GS_D1)
			SmbBuff = 0xCA;

		if(!bRWSMBus(chSMB_GS, SMbusWB, KX022_ADDRESS, KX023_SELF_TEST, &SmbBuff, 0)){
			GSSmbusFailCnt++;
		}
		else
		{
    	GSSmbusFailCnt=0;
		}
		break;
	case KX023_WHO_AM_I:
		SmbBuff = 0;
		if(GS_D1)
			SmbBuff = 0xCA;

		if(!bRWSMBus(chSMB_GS, SMbusWB, KX023_ADDRESS, KX023_SELF_TEST, &SmbBuff, 0)){
			GSSmbusFailCnt++;
		}
		else
		{
    	GSSmbusFailCnt=0;
		}
		break;

#if Support_BMA255
  case BMA255_BGW_CHIPID_VALUE:
		if(GS_D1){
			SET_MASK(GSensorSelfTestState,StartTest);
			SET_MASK(GSensorSelfTestState,DataReady);
			GSensorSelfTestState &= 0xC0;
			GSensorSelfTestCountX=0;
    }
    else
    {
			CLEAR_MASK(GSensorSelfTestState,StartTest);
			CLEAR_MASK(GSensorSelfTestState,DataReady);
	    SmbBuff = BMA255_PMU_SELF_TEST_AXIS_DISABLE;
			if(!bRWSMBus(chSMB_GS, SMbusWB, BMA255_ADDRESS, BMA255_PMU_SELF_TEST, &SmbBuff, 0)){
				GSSmbusFailCnt++;
			}else{
	    	GSSmbusFailCnt=0;
			}
	
	    SmbBuff = BMA255_PMU_RANGE_2G;  
	    if(!bRWSMBus(chSMB_GS, SMbusWB, BMA255_ADDRESS, BMA255_PMU_RANGE, &SmbBuff, 0)){
				GSSmbusFailCnt++;
			}else{
	    	GSSmbusFailCnt=0;
			}
    }
    break;
#endif
	}

  GS_DF = 0;

  Data2Port(nPort,0x00);
#else
  GS_D0 = 0x18;
  if( GS_D1 )
    GS_ST_HI();
  else
    GS_ST_LOW();

  GS_DF = 0;

  Data2Port(nPort,0x00);
#endif 
}

#endif  // Support_LenovoAPS

void LnvApsProtect(void)
{
#if Support_LenovoAPS
  //#if SW_ISCT
  WORD WXY_Mult;
  if (IS_MASK_SET(GS.Status, GS_Request))
  {
    SET_MASK (GS.Status, GS_Enable);
    CLEAR_MASK (GS.Status, GS_Request);

    // Initial after get enable request
    GS.Buf1X_S = 0;
    GS.Buf1X_E = 0;
    GS.Buf1X_N = 0;
    GS.Buf1Y_S = 0;
    GS.Buf1Y_E = 0;
    GS.Buf1Y_N = 0;
    
    GS.Buf2X_S = 0;
    GS.Buf2X_E = 0;
    GS.Buf2X_N = 0;
    GS.Buf2Y_S = 0;
    GS.Buf2Y_E = 0;
    GS.Buf2Y_N = 0;

    uAPSLastX=0;
    uAPSLastY=0;

    uCountX=0;
    uTotalX=0;
    uCountY=0;
    uTotalY=0;

    uAPSThresholdX=0x04;
    uAPSThresholdY=0x04;
    uAPSTimeOut=20;
  }
  // Calulation G-X/Y value then store to buffer 2
  else if (  IS_MASK_SET(GS.Status,GS_Enable) )  // Run G-sensor function
  {
    if ( GS.Buf1X_N > 0 )  //  for X-chanel    // buffer 1 store number > 0
    {
//       if (  GS.Buf2X_N < BUF2_SIZE )//8  // buffer 2 store number < size
//       {
        // Read buffer 1
        for (uCountX = 0, uTotalX = 0; uCountX < GS.Buf1X_N; uCountX++) {
          uTotalX += GS.Buf1X[GS.Buf1X_S];   // Add X for buffer1
          if (  ++GS.Buf1X_S == BUF1_SIZE )
          { GS.Buf1X_S = 0; }
        }

        // Average be writed to buffer 2
        if ( uCountX )
        {
          GS.Buf2X[GS.Buf2X_E] = (uTotalX/uCountX); //(X1+X2+...+X8)/8

          if ( GS.Buf2X_N < BUF2_SIZE )
          {
            GS.Buf2X_N ++;
          }

          if ( GS.Buf2X[GS.Buf2X_E]>uAPSLastX)
          { AD_Xdiff=GS.Buf2X[GS.Buf2X_E]-uAPSLastX; }
          else
          { AD_Xdiff=uAPSLastX-GS.Buf2X[GS.Buf2X_E]; }

          if ( AD_Xdiff>uAPSThresholdX)
          {
            uAPSLastX=GS.Buf2X[GS.Buf2X_E];
            uAPStickCount=0;
            if(IS_MASK_CLEAR(GSensorSelfTestState,StartTest)||IS_MASK_CLEAR(GSensorSelfTestState,DataReady))
            {
            GS_D10 |= BIT6;  //Gravity sensor data is changed
            }
          }
          if (  ++GS.Buf2X_E == BUF2_SIZE )
          { GS.Buf2X_E = 0; }

        }

        if( (PSSTS3&0x0C) | (typematic.byte!=0x00) )
        { GS_D10 |= BIT4; }
        else
        { GS_D10 &= ~(BIT4); }
    }
    //Notice the while loop
    if ( GS.Buf1Y_N > 0 )  // for Y-chanel
    {
        // Read buffer 1
        for (uCountY = 0, uTotalY = 0; uCountY < GS.Buf1Y_N; uCountY++) {
          uTotalY += GS.Buf1Y[GS.Buf1Y_S];   // Add Y
          if (  ++GS.Buf1Y_S == BUF1_SIZE )
          { GS.Buf1Y_S = 0; }
        }
        // Write to buffer 2
        if ( uCountY )
        {
          GS.Buf2Y[GS.Buf2Y_E] = (uTotalY/uCountY); //(Y1+Y2+...+Y8)/8

          if (GS.Buf2Y_N < BUF2_SIZE) {
            GS.Buf2Y_N ++;
          }

          if ( GS.Buf2Y[GS.Buf2Y_E]>uAPSLastY)
          { AD_Ydiff=GS.Buf2Y[GS.Buf2Y_E]-uAPSLastY; }
          else
          { AD_Ydiff=uAPSLastY-GS.Buf2Y[GS.Buf2Y_E]; }

          if ( AD_Ydiff>uAPSThresholdY)
          {
            uAPSLastY=GS.Buf2Y[GS.Buf2Y_E];
            uAPStickCount=0;
            if(IS_MASK_CLEAR(GSensorSelfTestState,StartTest)||IS_MASK_CLEAR(GSensorSelfTestState,DataReady))
            {
            GS_D10 |= BIT6;  //Gravity sensor data is changed
          	}
          }
          if (  ++GS.Buf2Y_E == BUF2_SIZE )
          { GS.Buf2Y_E = 0; }

        }

    }

    if ((AD_Xdiff<=uAPSThresholdX)&&(AD_Ydiff<=uAPSThresholdY))
    {
      if ((uAPStickCount > uAPSTimeOut) && (GS_D10 & BIT6))
      {
        GS_D10 &= ~(BIT6 | BIT5 | BIT4);   // set offset 20h bit6 =0
      }
      else
        uAPStickCount++; // Polling rate: 5ms
    }
  }

  if(IS_MASK_SET( APSHDDFLAG , APSHDDPRT_CHK) )
  {
    if (APSHDDCount == 181)
    {
      SET_MASK( APSHDDFLAG , APSHDDPRT_STA);
      CLEAR_MASK( APSHDDFLAG , APSHDDPRT_CHK);
      APSHDDCount = 0;
      X_Avg = X_Max180 - X_Min180;
      Y_Avg = Y_Max180 - Y_Min180;
      X_Max180 = X_Min180 = 0;
      Y_Max180 = Y_Min180 = 0;

      if ( (X_Avg >= 11) || (Y_Avg >= 11) || ((X_Avg >= 4)&&(Y_Avg >= 4)) )
      {
        SET_MASK(APSHDDFLAG,b5SlowPortect);
        CLEAR_MASK(APSHDDFLAG,b4QuickPortect);
#if SW_ISCT
        ISCT_Timer = 1;    // delay to 1 min.
        ISCT_Again = 40;  // Last 40 Sec count.
#endif // SW_ISCT
      }
      else
      {
        X_Avg *= X_Avg;  // Square X.
        Y_Avg *= Y_Avg;  // Square Y.
        WXY_Mult = (X_Avg * Y_Avg);
        if ( (WXY_Mult > 36) && (WXY_Mult < 100) )
        {
          CLEAR_MASK(APSHDDFLAG,b5SlowPortect);
          SET_MASK(APSHDDFLAG,b4QuickPortect);
#if SW_ISCT
          ISCT_Timer = 1;    // delay to 1 min.
          ISCT_Again = 40;  // Last 40 Sec count.
#endif // SW_ISCT
        }
        else
        {
          if ( WXY_Mult > 100 )
          {
            SET_MASK(APSHDDFLAG,b5SlowPortect);
            CLEAR_MASK(APSHDDFLAG,b4QuickPortect);
#if SW_ISCT
            ISCT_Timer = 1;    // delay to 1 min.
            ISCT_Again = 40;  // Last 40 Sec count.
#endif // SW_ISCT
          }
          else
          {
            CLEAR_MASK(APSHDDFLAG,b5SlowPortect);
            CLEAR_MASK(APSHDDFLAG,b4QuickPortect);
            CLEAR_MASK(APSHDDFLAG,b6PMEWAKE);
            CLEAR_MASK(APSHDDFLAG,b7RTCWAKE);
#if SW_ISCT
            if (IS_MASK_SET( APSHDDFLAG , b6PMEWAKE)){
              AOAC_STATUS |= 0x80;  // Set PME wake function.
            }
            else{
              AOAC_STATUS |= 0x40;  // Set EC RTC wake function.
            }
#endif // SW_ISCT
          }
        }
      }
      ADTOTALHDDCount = 0;
      APSHDDFLAG &= 0x30;
    }
    else
    {
      if ( uTotalX > X_Max180 )
      {
        X_Max180 = uTotalX;
      }
      else if ( (uTotalX < X_Min180) || (X_Min180 == 0) )
      {
        X_Min180 = uTotalX;
      }

      if ( uTotalY > Y_Max180 )
      {
        Y_Max180 = uTotalY;
      }
      else if ( (uTotalY < Y_Min180) || (Y_Min180 == 0) )
      {
        Y_Min180 = uTotalY;
      }
      APSHDDCount++;
    }
  }
#endif    // Support_LenovoAPS
}


void GSensor_DIAG(void)
{
  SET_MASK(GS.Status,GS_DiagRun);
  	GSENSOR_DIAG_DONE_SET;
  	SET_MASK(GS.Status,GS_Enable);
  	ScanGsensorI2C();
  CLEAR_MASK(GS.Status,GS_Enable);
  	if(GSSmbusFailCnt != 0)
  	{
  		GSENSOR_ERROR_SET;
  	}
  	else
  	{
  		GSENSOR_ERROR_CLR;
  	}
	CLEAR_MASK(GS.Status,GS_DiagRun);
  	GSENSOR_DIAG_DONE_CLR;
}

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
* Filename: OEM_BMA255.C
* 
* Description:
*    BMA255 BOSCH G-sensor controller process function.
* 
* History:
*   02/20/2017    Isaac Lin
*     created as new
********************************************************************************
*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//*****************************************************************************

#if (G_SENSOR == BMA255)

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

enum __BMA255_SELF_TEST {
		BMA255_SELF_TEST_SEND_X_AXIS = 0,
		BMA255_SELF_TEST_WAIT_X_50MS,
		BMA255_SELF_TEST_GET_X_AXIS,
		BMA255_SELF_TEST_SEND_Y_AXIS,
		BMA255_SELF_TEST_WAIT_Y_50MS,
		BMA255_SELF_TEST_GET_Y_AXIS,
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
* Function name: ECResetBMA255
*
* Descriptoin: This is to reset BMA255 by EC command.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void ECResetBMA255(void) {
	
}
/*
*******************************************************************************
* Function name: InitialBMA255
*
* Descriptoin: This is to detect G-sensor BMA255 in ready and initial.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void InitialBMA255(void) {
}

/*
*******************************************************************************
* Function name: BMA255Process           
*
* Descriptoin: This is invoked when the system is powered on                  
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void BMA255Process(void) {
}

/*
*******************************************************************************
* Function name: OEM_BMA255_init_hook           
*
* Descriptoin: Oem_Initialization for G-sensor BMA255
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_BMA255_init_hook(void)
{
	
}

/*
*******************************************************************************
* Function name: OEM_BMA255_HOOK           
*
* Descriptoin: G-Sensor BMA255 hook for Hook_Timer10msEventA               
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_BMA255_HOOK(void)
{
}


/*
*******************************************************************************
* Function name: OEM_BMA255_S3_HOOK           
*
* Descriptoin: G-sensor BMA255 hook for S3            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_BMA255_S3_HOOK(void)
{
}


/*
*******************************************************************************
* Function name: OEM_BMA255_S5_HOOK           
*
* Descriptoin: G-sensor BMA255 hook for S5            
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void OEM_BMA255_S5_HOOK(void)
{
}

/*
*******************************************************************************
* Function name: sendBMA255_SelfTestCommand           
*
* Descriptoin: G-sensor BMA255 hook for Sending selfTest enable command
*
* Arguments:
*       axis             
* Return Values:
*       none
*******************************************************************************
*/
void sendBMA255_SelfTestCommand(BYTE axis)
{
  BYTE SmbBuff;
  SmbBuff = BMA255_PMU_RANGE_4G;  
  if(!bRWSMBus(chSMB_GS, SMbusWB, BMA255_ADDRESS, BMA255_PMU_RANGE, &SmbBuff, 0)){
  	GSSmbusFailCnt++;
  }else{
  	GSSmbusFailCnt=0;
  }
  
  SmbBuff = axis;
  if(!bRWSMBus(chSMB_GS, SMbusWB, BMA255_ADDRESS, BMA255_PMU_SELF_TEST, &SmbBuff, 0)){
  	GSSmbusFailCnt++;
  }else{
  	GSSmbusFailCnt=0;
  }
}

/*
*******************************************************************************
* Function name: BMA255_SelfTest           
*
* Descriptoin: G-sensor BMA255 hook for selftest mode process
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/
void BMA255_SelfTest(void)
{
	switch(GSensorSelfTestState & 0x37)
	{
	  case BMA255_SELF_TEST_SEND_X_AXIS:
      sendBMA255_SelfTestCommand(BMA255_PMU_SELF_TEST_AXIS_X);
    	SET_MASK(GSensorSelfTestState,DataReady);
      GSensorSelfTestCountX = BMA255_WaitCommandMS;
      GSensorSelfTestState++;
	  	break;
	  case BMA255_SELF_TEST_WAIT_X_50MS:
	  	if(GSensorSelfTestCountX == 0)
	  	{
      	GSensorSelfTestState++;
	  	}
	  	break;
	  case BMA255_SELF_TEST_GET_X_AXIS:
	  	if(!bRWSMBus(chSMB_GS, SMbusRW, BMA255_ADDRESS,BMA255_ACCD_X_LSB,&GSensorTmpX_1 ,SMBus_NoPEC))
	  	{
		  		GSSmbusFailCnt++;
		  }
		  else
		  {
		  		GSSmbusFailCnt=0; 
		  }
      GSensorSelfTestState++;
	  	break;
	  case BMA255_SELF_TEST_SEND_Y_AXIS:
      sendBMA255_SelfTestCommand(BMA255_PMU_SELF_TEST_AXIS_Y);
      GSensorSelfTestCountX = BMA255_WaitCommandMS;
      GSensorSelfTestState++;
	  	break;
	  case BMA255_SELF_TEST_WAIT_Y_50MS:
	  	if(GSensorSelfTestCountX == 0)
	  	{
      	GSensorSelfTestState++;
	  	}
	  	break;
	  case BMA255_SELF_TEST_GET_Y_AXIS:
	  	if(!bRWSMBus(chSMB_GS, SMbusRW, BMA255_ADDRESS,BMA255_ACCD_Y_LSB,&GSensorTmpY_1 ,SMBus_NoPEC))
	  	{
		  		GSSmbusFailCnt++;
		  }
		  else
		  {
		  		GSSmbusFailCnt=0; 
		  }
    	CLEAR_MASK(GSensorSelfTestState,DataReady);
      GSensorSelfTestState &= 0xC0;
	  	break;
	}
}

#endif //Support_BMA255
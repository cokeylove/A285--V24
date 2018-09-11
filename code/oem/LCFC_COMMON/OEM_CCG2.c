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
* Filename: OEM_CCG2.C
* 
* Description:
*    CYPD2122(CCG2) Cypress Type-C controller process function.
* 
* History:
*   01/15/2016    Isaac Lin
*     created as new
********************************************************************************
*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//*****************************************************************************


#if (Support_TYPE_C == CCG2)

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


//enum __AC_DETECTION {
//    INIT,
//    CHECK_AC_ATTACHED,
//    GET_EXT_POWER_STATE,
//    SET_TIMER_10MS,
//    WAIT_TIME_OUT,        
//    CHECK_AC_STATE_AGAIN,
//    LET_START_AC_IDENTIFY,
//    SET_TIMER_1SEC,
//    WAIT_TIMER_1SEC,
//    START_AD,
//    WAIT_AD_DONE,
//    IDENTIFY_AC_CAPABILITY,
//    VALIDATE_AC_CAPABILITY,
//    WAIT_AC_OUT_INT,
//    CHECK_AC_OUT,
//    GET_EXT_POWER_STATE_FOR_AC_OUT,
//};


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

void InitialCCG2(void) {

	if(Read_TYPEC_INT())
	{
  	if(TypeCIniDelay == 0)
  	{
    	//Get Event number
			if(!bRWSMBus(chSMB_TYPEC, SMbusRW, TypeC_Addr, RESPONSE_REGISTER, &WSMbusTemp01, SMBus_NoPEC))
			{
				TypeCCommFailCnt++;
			}
			else
			{
				TypeCCommFailCnt = 0;
				TypeCEventNum = WSMbusTemp01;
	      if(TypeCEventNum == 0x80)
	      {
	      	//To de-assert INTR pin to high.
					WSMbusTemp01 = 0x01;
	  			if(!bRWSMBus(chSMB_TYPEC, SMbusWB, TypeC_Addr, INTR_REG, &WSMbusTemp01, SMBus_NoPEC))
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  				//Initial finish      	
			      SET_MASK(TypeCStatus,TypeCIniOK);
		      	//TypeCStatus = TypeCStatus & 0x80;	//Clear for new process flow step restart.
			    }
	      }
	    }
    }
    else
    {
    	TypeCIniDelay--;
    }
  }
}

/*
*******************************************************************************
* Function name: CCG2Process           
*
* Descriptoin: This is invoked when the system is powered on                  
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void CCG2Process(void) {

	if(Read_TYPEC_INT())
	{
  	//Get Event number
		if(!bRWSMBus(chSMB_TYPEC, SMbusRW, TypeC_Addr, RESPONSE_REGISTER, &WSMbusTemp01, SMBus_NoPEC))
		{
			TypeCCommFailCnt++;
		}
		else
		{
			TypeCCommFailCnt = 0;
			TypeCEventNum = WSMbusTemp01;
    	//Process Event
			switch(TypeCEventNum)
			{
	      case CMD_SUCCESS:
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case RESET_COMPLETE:
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      default:
	        break;
	    }
      if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
      {
      	//To de-assert INTR pin to high.
				WSMbusTemp01 = 0x01;
  			if(!bRWSMBus(chSMB_TYPEC, SMbusWB, TypeC_Addr, INTR_REG, &WSMbusTemp01, SMBus_NoPEC))
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
  				TypeCCommFailCnt = 0;
  				//Process finish      	
		      CLEAR_MASK(TypeCStatus,TypeCProcessOK);
		    }
      }
    }
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
void OEM_TYPE_C_init_hook(void)
{
	TypeCIniDelay = TypeC_Ini_Delay_time;	//Delay 300ms for type C stable
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
	if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
	{
		CCG2Process();
	}
	else
	{
		InitialCCG2();
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
void Oem_Hook_Type_C_BeforeSleep(void){
  
}

#endif //Support_TYPE_C


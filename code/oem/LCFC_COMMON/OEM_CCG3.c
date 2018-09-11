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

#if (Support_TYPE_C == CCG3)

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

enum __TYPEC_DETECTION {
    GET_EVENT_NUM = 0,
    DEASSERT_INTR_PIN,
    CHECK_DEVICE_MODE,
    SETTING_EVENT_MASK,
    CHECK_ADAPTOR_EXIST_DELAY,
    CHECK_ADAPTOR_EXIST,
    DETECT_CURRENT_PDO,
    ADAPTOR_DETECT_FINISH,
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
	if(Read_AC_IN())
	{
		if((Read_DCIN_ATTACHED2() != IS_MASK_SET(TypeCStatus,OldDcinAttached2))
			&& (Read_DCIN_ATTACHED2() || (!Read_DCIN_ATTACHED2() && IS_MASK_SET(TypeCStatus,TypeCAdpReady)))
			)
		{	//If DCIN_ATTACHED2 status change,that means adaptor ID need re-setting.
			if(Read_DCIN_ATTACHED2())
			{
				SET_MASK(TypeCStatus,OldDcinAttached2);
			}
			else
			{
				CLEAR_MASK(TypeCStatus,OldDcinAttached2);
			}
			ADPID_ON_EN();
			CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
		}
	}

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
void DetectAdaptorWatt(void)
{
	if(TypeCAdpPdStableCheck != 0)
	{
		rI2C_DATA = PD_STATUS;
		rI2C_DATA1[0] = PORT0_ADDRESS_HIGH;
		if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)	
		{
			TypeCCommFailCnt++;
			TypeCAdpPdStableCheck--;
		}
		else
		{
			TypeCCommFailCnt = 0;
			TypeCAdpPdStableCheck = 0;
			rI2C_DATA = CURRENT_PDO;
			rI2C_DATA1 = PORT0_ADDRESS_HIGH;
			if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x04,&SMB3_DATA, 0) == FALSE)	
			{
				TypeCCommFailCnt++;
			}
			else
			{
				TypeCCommFailCnt = 0;
				CurrentTypeCV = ((SMB3_DATA[2]&0x0F)<<6)+(SMB3_DATA[1]>>2);	//Unit:50mV
				CurrentTypeCI = ((SMB3_DATA[1]&0x03)<<8)+SMB3_DATA[0];			//Unit:10mA
				CurrentTypeCWatt = (BYTE)((DWORD)(CurrentTypeCV)*CurrentTypeCI/2000);	//Watt=(V*50mV)*(I*10mA)=(V*50/1000)*(I*10/1000)=V*I/2000
				if(CurrentTypeCWatt >= 230)
				{
					TypeCAdpID = AdapterID_230W;
				}
				else if(CurrentTypeCWatt >= 170)
				{
					TypeCAdpID = AdapterID_170W;
				}
				else if(CurrentTypeCWatt >= 135)
				{
					TypeCAdpID = AdapterID_135W;
				}
				else if(CurrentTypeCWatt >= 90)
				{
					TypeCAdpID = AdapterID_90W;
				}
				else if(CurrentTypeCWatt >= 65)
				{
					TypeCAdpID = AdapterID_65W;
				}
				else if(CurrentTypeCWatt >= 45)
				{
					TypeCAdpID = AdapterID_45W;
				}
				else if(CurrentTypeCWatt >= 36)
				{
					TypeCAdpID = AdapterID_36W;
				}
				else if(CurrentTypeCWatt > 0)
				{
					TypeCAdpID = AdapterID_Lower36W;
				}
				else
				{
					TypeCAdpID = AdapterID_NON_SUPPORT;
				}
				//Re-setting adaptor parameter
				ADPID_ON_EN();
				CLEAR_MASK(pEdgeProjectFlag1,pEdgeADPIDReady);
				SET_MASK(TypeCStatus,TypeCAdpReady);
			}
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
* Function name: ECResetCCG2
*
* Descriptoin: This is to reset CCG2 by EC command.                   
*
* Arguments:
*       none             
* Return Values:
*       none
*******************************************************************************
*/

void ECResetCCG3(void) {
	
	rI2C_DATA = TYPEC_RESET;
	rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
	rI2C_DATA1[1] = 0x52;
	rI2C_DATA1[2] = 0x01;
	if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x04,&I2C_DATA)==FALSE)	
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
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

void InitialCCG3(void) {
	BYTE TempRomData, Temp8;

	if(TypeCIniDelay == 0)
	{
		switch(TypeCProcessStep)
		{
	    case GET_EVENT_NUM:
		  	//Get Event number
				if(Read_TYPEC_INT())
				{
//					I2C_DATA = PORT0_RESPONSE_REGISTER & 0x00FF;
//					I2C_DATA1[0] = PORT0_RESPONSE_REGISTER >> 8;
//					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&rI2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
					rI2C_DATA = RESPONSE_REGISTER;
					rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
					if (TRUE)//(I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&rI2C_DATA, 0x02,&WSMbusTemp01, 0) == FALSE)	// WAITOTADD
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
//						TypeCEventNum = SMB3_DATA[0];
//						I2CRBDataCount = (SMB3_DATA[03]<<8)+SMB3_DATA[2];
						TypeCEventNum = WSMbusTemp01;
						I2CRBDataCount = WSMbusTemp02;
			      if(TypeCEventNum == 0x80)
			      {
			      	TypeCProcessStep++;
			      }
			    }
			  }
	      break;
	    case DEASSERT_INTR_PIN:
      	//To de-assert INTR pin to high.
				rI2C_DATA = INTR_REG;
				rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				rI2C_DATA1[1] = 0x01;
				if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)	
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
					TypeCCommFailCnt = 0;
		      TypeCProcessStep++;
		    }
	      break;
	    case CHECK_DEVICE_MODE:
	    	//Check Device mode is normal mode
				rI2C_DATA = DEVICE_MODE;
				rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
				  TypeCCommFailCnt = 0;
  				if((WSMbusTemp01& 0x03) > 0)
  				{
      			TypeCProcessStep++;
				  }
				}
	      break;
	    case SETTING_EVENT_MASK:
	    	//Setting EVENT mask
//				SMB3_DATA[0] = 0xFF;//0x98;
//				SMB3_DATA[1] = 0xF9;//0x01;
//				SMB3_DATA[2] = 0xFE;//0x00;
//				SMB3_DATA[3] = 0xFF;//0x00;
				rI2C_DATA = EVENT_MASK;
				rI2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				rI2C_DATA1[1] = 0xFF;//0x98;
				rI2C_DATA1[2] = 0xF9;//0x01;
				rI2C_DATA1[3] = 0xFE;//0x00;
				rI2C_DATA1[4] = 0xFF;//0x00;
				if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x06,&I2C_DATA)==FALSE)
			  {
  				TypeCCommFailCnt++;
  			}
  			else
  			{
					TypeCCommFailCnt = 0;
					rI2C_DATA = PD_CONTROL;
					rI2C_DATA1[0] = PORT0_ADDRESS_HIGH;
					rI2C_DATA1[1] = 0x10;
					if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)	
				  {
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
  					TypeCCommFailCnt = 0;
						TypeCProcessStep++;
		      }
				}
	      break;
	    case CHECK_ADAPTOR_EXIST_DELAY:
	    	TypeCIniDelay = 2;
	    	TypeCProcessStep++;
	      break;
	    case CHECK_ADAPTOR_EXIST:
				rI2C_DATA = TYPE_C_STATUS;
				rI2C_DATA1[0] = PORT0_ADDRESS_HIGH;
				if (I2C_Write_to_Read(chSMB_TYPEC,TypeC_Addr, 0x02,&rI2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
  			{
  				TypeCCommFailCnt++;
  			}
  			else
  			{
  				TypeCCommFailCnt = 0;
  				if(((WSMbusTemp01&0x1C) >> 2) == 0x02)
  				{
	      		SET_MASK(TypeCStatus,TypeCAdpExist);
  				}
					TypeCProcessStep++;
		    }
	      break;
	    case DETECT_CURRENT_PDO:
	    	//Detect current PDO(Current adaptor output watt)
    		if(IS_MASK_SET(TypeCStatus,TypeCAdpExist))
    		{
	  			TypeCAdpPdStableCheck = 3;
					DetectAdaptorWatt();
//					if(TypeCCommFailCnt == 0)
//					{
						TypeCProcessStep++;
//					}
    		}
    		else
    		{
					TypeCProcessStep++;
    		}
	      break;
	    case ADAPTOR_DETECT_FINISH:
				//Initial finish
	      SET_MASK(TypeCStatus,TypeCIniOK);
        TempRomData = Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_TYPEC_STATUS);
        if((TempRomData & 0x01) == 0x01)
        {
          Temp8 = TempRomData & 0xFE;	//Clear bit0:Type-C chip exist
          WRITE_DATA2EEPROM(&Temp8 , 1 , EEPROM_Bank7, EEPROM_TYPEC_STATUS);           
        }
		    TypeCProcessStep = 0;	//Clear for new process flow step restart.
	      break;
	    default:
		    TypeCProcessStep = 0;	//Clear for new process flow step restart.
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

void CCG3Process(void) {

	if(Read_TYPEC_INT())
	{
		if((PDStatus & 0x03) == 0)
		{
			//Detect INTR type
			rI2C_DATA = INTR_REG;
			rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
			if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
			{
				TypeCCommFailCnt++;
			}
			else
			{
				PDStatus = PDStatus | (WSMbusTemp01 & 0x07);	//Store current DEV_INTR and PORT0_INTR and PORT1_INTR status.
				TypeCCommFailCnt = 0;
			}
	  }
	  else
	  {
	  	//Get Event number
	  	if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
	  	{
				rI2C_DATA = RESPONSE_REGISTER;
				rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x02,&WSMbusTemp01, 0) == FALSE)
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
					TypeCEventNum = WSMbusTemp01;
					I2CRBDataCount = WSMbusTemp02;
		    }
	  	}
	  	else
	  	{
				rI2C_DATA = PORT0_RESPONSE_REGISTER & 0x00FF;
				rI2C_DATA1[0] = PORT0_RESPONSE_REGISTER >> 8;
				if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x04,&SMB3_DATA[0], 0) == FALSE)
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
					TypeCEventNum = SMB3_DATA[0];
					I2CRBDataCount = (SMB3_DATA[03]<<8)+SMB3_DATA[2];
				}
			}
    	//Process Event
			switch(TypeCEventNum)
			{
	      case CMD_SUCCESS:
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case RESET_COMPLETE:
	      	// Type-C need re-initial after Type-C IC power(Valw) once off.
	  			OEM_TYPE_C_init_hook();
		      //SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case PORT_CONNECT_DETECTED:
					rI2C_DATA = TYPE_C_STATUS;
					rI2C_DATA1[0] = PORT0_ADDRESS_HIGH;
					if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, 0x01,&WSMbusTemp01, 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  				if(((WSMbusTemp01&0x1C) >> 2) == 0x02)
	  				{
		      		SET_MASK(TypeCStatus,TypeCAdpExist);
	  				}
			    }
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case PORT_DISCONNECT_DETECTED:
	      	if(IS_MASK_SET(TypeCStatus,TypeCAdpExist))
	      	{
			      CLEAR_MASK(TypeCStatus,TypeCAdpExist);
			      CLEAR_MASK(TypeCStatus,TypeCAdpReady);
			      TypeCAdpID = 0;
			      CurrentTypeCV = 0;
			      CurrentTypeCI = 0;
			      CurrentTypeCWatt = 0;
    				ADPI_MUL = ClassicAdaptor_ADPI_MUL;	//
			    }
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case CONTRACT_INFO:
					rI2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					rI2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(chSMB_CHG, TypeC_Addr, 0x02,&rI2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  			}
	  			TypeCAdpPdStableCheck = 3;
//					I2C_DATA = PD_STATUS;
//					I2C_DATA1[0] = PORT0_ADDRESS_HIGH;
//					if (I2C_Write_to_Read(SMbusChA, TypeC_Addr, 0x02,&rI2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE)
//	  			{
//	  				TypeCCommFailCnt++;
//	  				TypeCAdpPdStableCheck = 3;
//	  			}
//	  			else
//	  			{
//	  				TypeCCommFailCnt = 0;
//	  				TypeCAdpPdStableCheck = 1;
//	  			}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case VDM_RECEIVED:
					rI2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					rI2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE) 
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  			}
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      case SOURCE_CAPABILITIES_MESSAGE_RECEIVED:
					rI2C_DATA = PORT0_READ_DATA_AREA & 0x00FF;
					rI2C_DATA1[0] = PORT0_READ_DATA_AREA >> 8;
					if (I2C_Write_to_Read(chSMB_TYPEC, TypeC_Addr, 0x02,&rI2C_DATA, I2CRBDataCount,&SMB3_DATA[0], 0) == FALSE) 
	  			{
	  				TypeCCommFailCnt++;
	  			}
	  			else
	  			{
	  				TypeCCommFailCnt = 0;
	  			}
					//HOCTL2_A &= 0xFD;
		      SET_MASK(TypeCStatus,TypeCProcessOK);
	        break;
	      default:
  				{
		      SET_MASK(TypeCStatus,TypeCProcessOK);
		    	}
	        break;
	    }
      if(IS_MASK_SET(TypeCStatus,TypeCProcessOK))
      {
      	//To de-assert INTR pin to high.
				WSMbusTemp01 = 0x01;
				rI2C_DATA = INTR_REG;
				rI2C_DATA1[0] = PORT0_PORT1_COM_ADDRESS_HIGH;
				rI2C_DATA1[1] = 0x00;
				if(IS_MASK_SET(PDStatus,CurrentDEV_INTR))
				{
					//I2C_DATA1[1] = I2C_DATA1[1] | CurrentDEV_INTR;
					rI2C_DATA1[1] = 0x01;
					CLEAR_MASK(PDStatus,CurrentDEV_INTR);
				}
				else if(IS_MASK_SET(PDStatus,CurrentPORT0_INTR))
				{
					//I2C_DATA1[1] = I2C_DATA1[1] | CurrentPORT0_INTR;
					rI2C_DATA1[1] = 0x02;
					CLEAR_MASK(PDStatus,CurrentPORT0_INTR);
				}
				if(I2C_Write(chSMB_TYPEC, TypeC_Addr, 0x03,&I2C_DATA)==FALSE)
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
	if(Read_DCIN_ATTACHED2())
	{
		SET_MASK(TypeCStatus,OldDcinAttached2);
	}
	else
	{
		CLEAR_MASK(TypeCStatus,OldDcinAttached2);
	}
	TypeCIniDelay = TypeC_Ini_Delay_time;	//Delay 80ms for type C stable
  TypeCStatus = 0;
  TypeCCommFailCnt = 0;
  TypeCAdpID = 0;
  CurrentTypeCV = 0;
  CurrentTypeCI = 0;
  CurrentTypeCWatt = 0;
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
	if(IS_MASK_CLEAR(PDStatus,TypeCFwUpdating))
	{
		if(IS_MASK_SET(TypeCStatus,TypeCIniOK))
		{
			CCG3Process();
			DetectAdaptorWatt();
		}
		else
		{
			InitialCCG3();
		}
	}
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


//*****************************************************************************
//
//  oem_battery.c
//
//  Copyright (c) 2012-
//
//  Created on: 2012/11/21
//
//      Author: 
//
//*****************************************************************************

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

//****************************************************************************
// The function of smart battery
//****************************************************************************

void UpdateBatteryLevel (int battery_id)
{
//
// Update battery level
//
  BYTE BatteryLevel;

  BatteryLevel = 0;

  if( TWR_BAT(RelativeStateOfChgL,battery_id) >= 80 )
  {
    BatteryLevel = SBC_BATTERY_LEVEL_HIGH;
    CLEAR_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
  }
  else if( (TWR_BAT(RelativeStateOfChgL,battery_id) >= 20) && (TWR_BAT(RelativeStateOfChgL,battery_id) < 80) )
  {
    BatteryLevel = SBC_BATTERY_LEVEL_MIDDLE_HIGH;
    CLEAR_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
    if( IS_MASK_CLEAR(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTOver20) )
    {
      SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTOver20);
      CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTUnder20);
      ECSMI_SCIEvent(LLB_22);
    }
  }
  else if((TWR_BAT(RelativeStateOfChgL,battery_id) >= 5) && (TWR_BAT(RelativeStateOfChgL,battery_id) < 20) )
  {
    BatteryLevel = SBC_BATTERY_LEVEL_MIDDLE;
    CLEAR_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
    if( IS_MASK_CLEAR(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTUnder20) )
    {
      SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTUnder20);
      CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeBATTOver20);
      ECSMI_SCIEvent(LLB_22);
    }

  }
  else if( (TWR_BAT(RelativeStateOfChgL,battery_id) < 5) && (TWR_BAT(RemainCapL,battery_id)  >= 30) )
  {
    BatteryLevel = SBC_BATTERY_LEVEL_LOW;
    CLEAR_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
  }
  else if( (TWR_BAT(RemainCapL,battery_id)  < 30) && (TWR_BAT(RemainCapL,battery_id)  >= 20) )
  {
    // remaining capacity
    if( IS_MASK_CLEAR(TWR_BAT(TWR01,battery_id),BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
    BatteryLevel = SBC_BATTERY_LEVEL_WARNING;
    SET_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
  }
  else if( (TWR_BAT(RemainCapL,battery_id)  < 20) && (TWR_BAT(RemainCapL,battery_id)  > 0) )
  {
    if( IS_MASK_CLEAR(TWR_BAT(TWR01,battery_id),BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
    BatteryLevel = SBC_BATTERY_LEVEL_CRITICAL_LOW;
    SET_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
  }
  else
  {
    if( IS_MASK_CLEAR(TWR_BAT(TWR01,battery_id),BatteryCritical) )
    {
      ECQEvent(LLB_22);
    }
    BatteryLevel = SBC_BATTERY_LEVEL_EMPTY;
    SET_MASK(TWR_BAT(TWR01,battery_id),BatteryCritical);
  }

  /*  (From TPG sbcomm.c Update_Battery_Status_In_Interface_Space() )
  *   leave level as 0 if battery level is less than low
  *   and adjust it start from 1 for low level.,
  */
  OEM_BAT_1B(PriBattSTS, battery_id) &= ~(BIT2 | BIT1 | BIT0);
  if (SBC_BATTERY_LEVEL_LOW <= BatteryLevel) {
    if (BM_MAX_LEVEL < (BatteryLevel - SBC_BATTERY_LEVEL_LOW + 1)) {
      OEM_BAT_1B(PriBattSTS, battery_id) |= BM_MAX_LEVEL;
    } else {
      OEM_BAT_1B(PriBattSTS, battery_id) |= (BatteryLevel - SBC_BATTERY_LEVEL_LOW + 1);
    }
  }
//trickle charge setting
  if(IS_MASK_SET(OEM_BAT(M_BATFlag1, battery_id),pEdgeProjectBATWake))
  {
    OEM_BAT_1B(PriBattSTS, battery_id) |= (BIT2 | BIT1 | BIT0);
  }
}



void SetBatteryStopChargeStateFlag (BYTE battery_id, BYTE event_id)
{

    if(event_id != 0) {  	   //Special Event
	   switch(event_id) {
          case AC_IN:   
            //   M_BATStopChgStatL 
            //   ENB0B3STOP      BIT1
            //   Battery name space B0/B3 start stop charge enable      
			if ((OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) != 0) && (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) != 0)){
				 if ((OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) <= TWR_BAT(RelativeStateOfChgL,battery_id))){ 
				   SET_MASK(OEM_BAT(M_BATStopChgStatL, battery_id), ENB0B3STOP);  
				   pFakeCHGCnt = 0;
				 } else {
				   CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id), ENB0B3STOP);
				 }
		    }			
            //   M_BATStopChgStatH 
            //   PMTHRESHOLD     BIT2
            //   Check and Set Stop Charge Threshold of PM AP 
			if (TWR_BAT(RelativeStateOfChgL,battery_id) >= 96){
				return;
			}
			if((OEM_BAT(M_BATStartCharge,battery_id) < TWR_BAT(RelativeStateOfChgL,battery_id)) &&
			   (TWR_BAT(RelativeStateOfChgL,battery_id) < OEM_BAT(M_BATStopCharge,battery_id))){
				 if (pInhitTimer != 0x00) {
					SET_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
				 }				
				 SET_MASK(OEM_BAT(M_BATStopChgStatH, battery_id), PMTHRESHOLD);
			}
		    break;
		  default:
		  break;	
	   }
    }	
    else {                  // Normal Polling
	   // M_BATStopChgStatL 
	   // ENB0B3STOP		BIT1
	   // Battery name space B0/B3 start stop charge enable  

	   //   Check start & stop for polling
	   if ((OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) != 0) && (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) != 0)){
		  if (OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) <= TWR_BAT(RelativeStateOfChgL, battery_id)){ 
			 SET_MASK(OEM_BAT(M_BATStopChgStatL, battery_id), ENB0B3STOP); 
		  }
		  if (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) > TWR_BAT(RelativeStateOfChgL, battery_id)){ 
			 CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id), ENB0B3STOP); 
		  }			 
	   }

	   //   Only set stop 
	   if ((OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) != 0) && (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) == 0)){
		  if (OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) <= TWR_BAT(RelativeStateOfChgL, battery_id)){ 
			 SET_MASK(OEM_BAT(M_BATStopChgStatL, battery_id), ENB0B3STOP);	
		   } else {
			 CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id), ENB0B3STOP);
		   }
	   }
	   //  Only set start    
	   else if ((OEM_BAT_1B(u4MainBattChgOffCap, (battery_id << 1)) == 0) && (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) != 0)){
		  if (OEM_BAT_1B(u4MainBattChgOnCap, (battery_id << 1)) <= TWR_BAT(RelativeStateOfChgL, battery_id)){ 
			 SET_MASK(OEM_BAT(M_BATStopChgStatL, battery_id), ENB0B3STOP);	
		  } else {
			 CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id), ENB0B3STOP);
		  }
	   }
	 
       //  M_BATStopChgStatH 
       //  PMTHRESHOLD     BIT2
       //  Check and Set Stop Charge Threshold of PM AP  	
       
	  // if( TWR_BAT(RelativeStateOfChgL,battery_id) >= OEM_BAT(M_BATStopCharge,battery_id) )
	   if( TWR_BAT(RelativeStateOfChgL,battery_id) > OEM_BAT(M_BATStopCharge,battery_id) )  //A285D00037 modify for LG Bat RSOC=64 but full charge bit not set
	   {
	       if (pInhitTimer != 0x00) {
		      SET_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
	       }
	       SET_MASK(OEM_BAT(M_BATStopChgStatH, battery_id), PMTHRESHOLD);
	   }
	   else if( TWR_BAT(RelativeStateOfChgL,battery_id) <= OEM_BAT(M_BATStartCharge,battery_id) )
	   {
	       if (pInhitTimer == 0x00) {
		     CLEAR_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
	       }
	       CLEAR_MASK(OEM_BAT(M_BATStopChgStatH,battery_id), PMTHRESHOLD);
	   }
    }		        	
}


void UpdateECNameSpace(int battery_id)
{
  UINT8 Temp8;  
  if(IS_MASK_SET(OEM_BAT_1B(PriBattSTS, battery_id),BattExist) || (IS_MASK_SET(EVT_STATUS1,(M_bat_in << battery_id))&&IS_MASK_CLEAR(checkCardStatus,checkCardExist)))  // Battery1 exists // debug card
  {
    //(TBD) determine whether battery2 exists too

    if( IS_MASK_SET(u4PeriCtrl1,(Edge_PriBattInhib << battery_id)) )
    {
      CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),BattExist);
      CLEAR_MASK(TWR00, (MainBattAttached >> battery_id));
      if( IS_MASK_CLEAR(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATFWUP) )
      {
        TWR_BAT(CurrentL,battery_id) = 0;
        TWR_BAT(CurrentH,battery_id) = 0;
        Bat0x0BFakeCnt = 0;
        TWR_BAT(M_BATErrorCnt,battery_id) = 0;
        SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATFWUP);
        SET_MASK(BATUPDATEFW,(PriBattInhib << battery_id));
      }
    }
    else
    {
      SET_MASK(TWR00, (MainBattAttached >> battery_id) );
      SET_MASK(OEM_BAT_1B(PriBattSTS, battery_id),BattExist);
      if( IS_MASK_SET(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATFWUP) )
      {
        CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATFWUP);
        FirstGetBatData(battery_id);
        Service_Auth_Step = 1;        // start AUTH
#if Thinkpad_Battery_HW_Auth
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
#endif
//A485D00029:Modify Read BATMakerID&FWUpdate flow.--->Start
		if( battery_id == M_BAT )
		{
			bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr,C_FW_Status,&FirmwareUpdateStatusL,SMBus_NoPEC);
    		bRSMBusBlock(chSMB_BAT,SMbusRBK,SmBat_Addr,C_BAT_Info,&BatterymakerIDL, bRead_I2C_NULL);
		}
		else
		{
			bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr,C_FW_Status,&SecFirmwareUpdateStatusL,SMBus_NoPEC);
    		bRSMBusBlock(chSMB_2ndBAT,SMbusRBK,SecSmBat_Addr,C_BAT_Info,&SecBatterymakerIDL, bRead_I2C_NULL);
		}//A485D00029:Modify Read BATMakerID&FWUpdate flow.--->end
        CLEAR_MASK(BATUPDATEFW,(PriBattInhib << battery_id));
      }
    }


    if( IS_MASK_SET(BATUPDATEFW,(PriBattExistTemp << battery_id)) != IS_MASK_SET(u4PeriCtrl1,(Edge_PriBattInhib << battery_id)) )
    {
      if(IS_MASK_SET(u4PeriCtrl1,(Edge_PriBattInhib << battery_id)))
      {
      	 OEM_BAT(M_BAT_pFKTemp, battery_id) = 0;
      	 OEM_BAT(M_BAT_Batt0x3CFailRetry5sTimer, battery_id) = 0;
        SET_MASK(BATUPDATEFW,(PriBattExistTemp << battery_id));
      }
      else
      {
        CLEAR_MASK(BATUPDATEFW,(PriBattExistTemp << battery_id));
      }

      SET_MASK(OEM_BAT(M_BATFlag1, battery_id),pEdgeProjectBATIn);
    }

    UpdateBatteryLevel(battery_id);


//
// Update battery charge/discharge status
//
    Temp8 = nBatteryStatH;
    if (battery_id == Current_Battery){
      if( IS_MASK_CLEAR(SYS_STATUS,AC_ADP) || Read_ACOFF())
      {
        nBatteryStatH &= ~(PreBat_DISCHARGE + PreBat_CHARGE + SecBat_DISCHARGE + SecBat_CHARGE);
        nBatteryStatH |= (PreBat_DISCHARGE << battery_id);
      }

      else if ((IS_MASK_CLEAR(TWR_BAT(CurrentH,battery_id), BIT7) && ((TWR_BAT(CurrentH,battery_id) > 0) || (TWR_BAT(CurrentL,battery_id) > 0))) // In charging and current > 0
          || (IS_MASK_CLEAR(TWR_BAT(BattStatusL,battery_id),FullyChg) && (pFakeCHGCnt != 0) && IS_MASK_SET(SYS_STATUS,AC_ADP))) //)
      {
        nBatteryStatH &= ~(PreBat_DISCHARGE + PreBat_CHARGE + SecBat_DISCHARGE + SecBat_CHARGE);    
        nBatteryStatH |= (PreBat_CHARGE<< battery_id);
      }
      else
      {
        nBatteryStatH &= ~(PreBat_DISCHARGE + PreBat_CHARGE + SecBat_DISCHARGE + SecBat_CHARGE);   
//trickle charge setting
        if (M_CTRL_GET())   //M trickle
        {     
          nBatteryStatH |= PreBat_CHARGE;
        }
        if (S_CTRL_GET())   //S trickle
        {
          nBatteryStatH |= SecBat_CHARGE;
        }        
      }
      if ((Temp8 & ~(PreBat_DISCHARGE + PreBat_CHARGE)) != (nBatteryStatH & ~(PreBat_DISCHARGE + PreBat_CHARGE)))
      {
        ECSMI_SCIEvent(ACPI_BAT1CHG_SCI);
      }

      if ((Temp8 & ~(SecBat_DISCHARGE + SecBat_CHARGE)) != (nBatteryStatH & ~(SecBat_DISCHARGE + SecBat_CHARGE)))
      {
        ECSMI_SCIEvent(ACPI_BAT0CHG_SCI);
      }      
    }  //if (BAT_Active_Num == battery_id)
//
// Charge threshold
//

//
// If pInhitTimer != 0, battery can be inhibit charged.
// If pInhitTimer == 0, battery should be charged immediately.
//

/*
    if( TWR_BAT(RelativeStateOfChgL,battery_id) <= OEM_BAT(M_BATStartCharge,battery_id) )
    {
      if (pInhitTimer == 0x00) {
        CLEAR_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
      }
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatH,battery_id), PMTHRESHOLD);
    }
    else if( TWR_BAT(RelativeStateOfChgL,battery_id) >= OEM_BAT(M_BATStopCharge,battery_id) )
    {
      if (pInhitTimer != 0x00) {
        SET_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
      }
      SET_MASK(OEM_BAT(M_BATStopChgStatH, battery_id), PMTHRESHOLD);
    }
*/
    
	SetBatteryStopChargeStateFlag(battery_id,0);

//
// Set Charge/Discharge status into TWR register.
// If Charge and Dischage both set '0', it means "No Activity".
//
    if (battery_id == Current_Battery){
      if(nBatteryStatH & (PreBat_CHARGE<< battery_id) )  // Battery Charging
      {      
        SET_MASK(OEM_BAT_1B(PriBattSTS, battery_id),Charging);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),Charging);      
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),DisChg);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),DisChg);      
        //BattErrorDis;
        SET_MASK(TWR_BAT(TWR01,battery_id),Charge);
        CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Charge);      
        CLEAR_MASK(TWR_BAT(TWR01,battery_id),Discharge);//CLEAR_MASK(TWR01,Discharge);
        CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Discharge);//CLEAR_MASK(TWR01,Discharge);      
        if(IS_MASK_SET(pProject3,uReSendBat1ChgQevent) && (inhibit2sec == 0)){
          CLEAR_MASK(pProject3,uReSendBat1ChgQevent);
          SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATChange);
        }
      }
      else
      {
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),Charging);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),Charging);      
        //BattErrorDis;
        CLEAR_MASK(TWR_BAT(TWR01,battery_id),Charge);
        CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Charge);      
      }
      if( nBatteryStatH & (PreBat_DISCHARGE << battery_id) )  // Battery Discharge
      {
        SET_MASK(OEM_BAT_1B(PriBattSTS, battery_id),DisChg);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),DisChg);      
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),Charging);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),Charging);      
        CLEAR_MASK(TWR_BAT(TWR01,battery_id),Charge);
        CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Charge);      
        if ((IS_MASK_SET(TWR_BAT(BattStatusL,battery_id),FullyChg) || ( !Read_ACOFF() && (OEM_BAT(M_BATStopChgStatL,battery_id) | OEM_BAT(M_BATStopChgStatH,battery_id)) ))
        &&(IS_MASK_SET(SYS_STATUS,AC_ADP)))
        {
          CLEAR_MASK(TWR_BAT(TWR01,battery_id),Discharge);
          CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Discharge);
        }
        else
        {
          SET_MASK(TWR_BAT(TWR01,battery_id),Discharge);//SET_MASK(TWR01,Discharge);
          CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Discharge);//SET_MASK(TWR01,Discharge);          
        }
        if(( TWR_BAT(RelativeStateOfChgL,battery_id) > OEM_BAT(M_BATStartCharge,battery_id) ) 
          && ( TWR_BAT(RelativeStateOfChgL,battery_id) <= OEM_BAT(M_BATStopCharge,battery_id) )
          && (IS_MASK_SET(SYS_STATUS,AC_ADP)))
        {
          CLEAR_MASK(TWR_BAT(TWR01,battery_id),Discharge);
          CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Discharge);        
        }
      }
      else
      {
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),DisChg);
        CLEAR_MASK(OEM_BAT_1B(PriBattSTS, (1^battery_id)),DisChg);      
        CLEAR_MASK(TWR_BAT(TWR01,battery_id),Discharge);
        CLEAR_MASK(TWR_BAT(TWR01,(1^battery_id)),Discharge);      
      }
//trickle charge setting
      if( nBatteryStatH & (PreBat_CHARGE << (1^battery_id)))  // Battery charge
      {
        if (M_CTRL_GET()){
          SET_MASK(PriBattSTS, Charging);
          SET_MASK(TWR01,Charge);
        }
        if (S_CTRL_GET()){
          SET_MASK(SecBattSTS, Charging);
          SET_MASK(SecTWR01,Charge);
        }
      }
    } //   if (BAT_Active_Num == battery_id){
  }
  else    // Battery not exist
  {
    OEM_BAT_1B(PriBattSTS, battery_id) = 0x00;
    CLEAR_MASK(TWR00, (MainBattAttached >> battery_id));    //TWR00.bit.MainBattAttached = 0
    CLEAR_MASK(OEM_BAT_1B(PriBattSTS, battery_id),BattExist);
  }

  if (IS_MASK_SET(OEM_BAT(M_BATStopChgStatL,battery_id),ENCOMMFAIL))
  {
    if(IS_MASK_CLEAR(OEM_BAT_1B(PriBattSTS, battery_id),BattError))
    {
      CLEAR_MASK(TWR_BAT(TWR01,battery_id), Commestablished);
    }
  }
  else
    {SET_MASK(TWR_BAT(TWR01,battery_id), Commestablished);}

  if(IS_MASK_SET(OEM_BAT_1B(PriBattSTS,battery_id),BattExist))  // Battery1 exists
  {
    TWR_BAT(Major,battery_id) = TPGPM_Major;
    TWR_BAT(Minor,battery_id) = TPGPM_Minor;
    if((TWR_BAT(ManufactureAccessL,battery_id) & 0x03)!= 0x00)
    {
      SET_MASK(OEM_BAT_1B(PriBattSTS,battery_id),BattError);
    }
    else
    {
      CLEAR_MASK(OEM_BAT_1B(PriBattSTS,battery_id),BattError);
    }
  }

  if (OEM_BAT(M_BATin_delay,battery_id) !=0 )
  {
    OEM_BAT(M_BATin_delay,battery_id) -- ;
    if (IS_MASK_SET(OEM_BAT_1B(PriBattSTS,battery_id),BattExist))
    return;
  }

  if( IS_MASK_SET(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATIn) )
  {
    IBMbatteryStore();  // update info
    CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATIn);
    BAT_ECSMI_SCIEvent(ACPI_BAT0IN_SCI, battery_id);
  }
  else if( IS_MASK_SET(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATChange) )
  {
    IBMbatteryStore();  // update info
    CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATChange);
    BAT_ECSMI_SCIEvent(ACPI_BAT0CHG_SCI, battery_id);
  }

  if( IS_MASK_SET(u4PeriSTS5,(PriBattDisChg << battery_id)))
  {
    if( ( TWR_BAT(RelativeStateOfChgL,battery_id) <= 1 ) && IS_MASK_SET(pBattLearn,uRefreshmode) )
    {
      CLEAR_MASK(pBattLearn,uACoffstchg);
      SET_MASK(pBattLearn,uACoffdschg);
		  if( Read_BATLEARN() )
		  {
        	BAT_LEARN_OFF();
		  }
		  if( nAcOffCmdGET )
		  {
		   	BATLEARNProcess(nAcOffCmdDIS);
		  }
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENCUSTOMER);
    }
    else if( IS_MASK_SET(TWR_BAT(BattStatusL,battery_id),FullyChg) && IS_MASK_SET(pBattLearn,uACoffdschg) )
    {
      if( Read_BATLEARN() )
		  {
       	BAT_LEARN_OFF();
		  }
		  if( nAcOffCmdGET )
		  {
		   	BATLEARNProcess(nAcOffCmdDIS);
		  }
      CLEAR_MASK(pBattLearn,uACoffstchg);
      CLEAR_MASK(pBattLearn,uACoffdschg);
      CLEAR_MASK(u4PeriSTS5,(PriBattDisChg << battery_id));
      CLEAR_MASK(pBattLearn,uRefreshmode);
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENCUSTOMER);
    }
  }
}

void UpdateNameSpace(int battery_id)
{  
  if (IS_MASK_CLEAR(TWR_BAT(BattStatusL,battery_id),FullyChg))
  {
    if( (pFakeCHGCnt != 0) && IS_MASK_SET(SYS_STATUS,AC_ADP) )
    {
      CLEAR_MASK(TWR_BAT(BattStatusL,battery_id),Dsg);
    }
  }
  if (IS_MASK_CLEAR(TWR_BAT(BattStatusL,battery_id),FullyChg)||(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))   
  {
    if (OEM_BAT(M_BATChkBattery_abnormal_status,battery_id) == ChkBattery_abnormal_status_error)
    {
      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      TWR_BAT(RemainCapL,battery_id) = 0;
      TWR_BAT(RemainCapH,battery_id) = 0;
    }
  }
}


void ChkS3ResumeRSOC(void)
{
  WORD BAT0_RC, BAT1_RC;
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
  {
    if ( SystemIsS3 )
    {
#if BATS3WAKEcondition
      BAT0_RC = (RemainCapH <<8) + RemainCapL; 
      BAT1_RC = (SecRemainCapH <<8) + SecRemainCapL;
      //check  battery under 300mWh.
      if ((BAT0_RC <= BATS3WAKEcondition) && (BAT1_RC <= BATS3WAKEcondition))
//      if( (TWR_BAT(RemainCapL,Current_Battery)<= (BATS3WAKEcondition & 0xFF)) && (TWR_BAT(RemainCapH,Current_Battery) == (BATS3WAKEcondition >> 8)))          //1st battery 
#else
      if ((RelativeStateOfChgL <= S3RSOCPercentage) && (SecRelativeStateOfChgL <= S3RSOCPercentage))  // check battery under 5%.    
#endif      
      {
        CriticalLowBATWakeS3();
      }
    }
  }
}

void RSMRST_shutdown(void)          //shutdown (RSMRST HI-->low-->HI)

{
#if !Support_STDNProct
  return;
#endif
  RSMRST_LOW();              //shutdown (remark first)
  Delay1MS(1);
  RSMRST_HI();
  RSMshutdownCnt++;
}

void DownGPUPState()
{
  if ((cGPUThrottling != 0)&&(GPUAlarm == 0)){
    cGPUThrottling = 0;
    GPU2Sec = 0;
  }
}

void UpGPUPState()
{
  if (cGPUThrottling != GPUMAXPSTATE) {
    if(GPU2Sec == 0){
      GPU2Sec = 200;
      cGPUThrottling++;
    }
  }
}  

void DownBatteryPState()
{
  if ((cBATTThrottling !=0)&&(M_BATAlarm == 0)){
    if(ADPI2Sec == 0){
      ADPI2Sec = 200;
      cBATTThrottling--;
      SET_MASK(Recovery2Sec,RecoveryCPU);
    }
  }
}

void UpBatteryPState()
{
  if (IS_MASK_SET(Recovery2Sec,RecoveryCPU)){
    ADPI2Sec = 0;
  }
  if (cBATTThrottling != CPUMAXPSTATE) {
    if(ADPI2Sec == 0){
      ADPI2Sec = 200;
      cBATTThrottling++;
      CLEAR_MASK(Recovery2Sec,RecoveryCPU);
    }
  }
}  

void ChkBattery_OTP(int battery_id)
{
  if(battery_id != Current_Battery){
    return;
  }
  ITempW01 = (WORD)(( TWR_BAT(TemperatureH,battery_id)<<8)+ TWR_BAT(TemperatureL,battery_id));    // get temperature
  if (ITempW01 > 2730)
    {
    nBattAverTemp = (ITempW01 - 2730) / 10;
    }
  else
    return;

  if ((SystemIsS0)&&(IS_MASK_CLEAR(SYS_STATUS,AC_ADP)))
    {
    if (nBattAverTemp >= BatteryOTPHi)        // 60
      {
        SET_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENOVERTEMP);
        SET_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
        cBATTThrottling = CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
        SET_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOTP);

        if (nBattAverTemp >= BatteryOTPSD)       //70    // 2 min
        {
#if !EN_PwrSeqTest
          SET_MASK(SysStatus,ERR_ShuntDownFlag);
          ProcessSID(BATTOVERTEMP_ID);
          RSMRST_shutdown();
          CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
          CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
#endif
        }
        else
          OEM_BAT(M_BATOvrTempCnt,battery_id) ++;
      }

    if (nBattAverTemp <= BatteryOTPLow)      //55
    {
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENOVERTEMP);
      OEM_BAT(M_BATOvrTempCnt,battery_id) =0;       
      if(GPU2Sec == 0){
        if((cGPUThrottling != 0) && ((GPUAlarm & (~GPUAlarm_BATOTP)) == 0)){
          GPU2Sec = 100;
          cGPUThrottling = 0;
        }else{
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
        }
      }
      if(ADPI2Sec == 0){
        if((cBATTThrottling != 0) && ((OEM_BAT(M_BATAlarm,battery_id) & (~BATOTP)) == 0)){
          ADPI2Sec = 100;
          cBATTThrottling--;
        }else{
          CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
        }
      }
    }
  }
  else
  {
    if( (SystemIsS0)
      && IS_MASK_SET(SYS_STATUS,AC_ADP)
      && IS_MASK_SET(TWR_BAT(BattStatusL,battery_id),Dsg)
      && (TWR_BAT(CurrentH,battery_id) & BIT7)
      )
    {
      if( nBattAverTemp >= BATHYBITOTPSHUT )
      {
#if !EN_PwrSeqTest
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTOVERTEMP_ID);
        RSMRST_shutdown();
        CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
#endif
      }
      else if(nBattAverTemp >= BATHYBITOTPTHOEN)
      {
        SET_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
        cBATTThrottling = CPUMAXPSTATE;
        cGPUThrottling = GPUMAXPSTATE;
        SET_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOTP);
      }
      else if ( nBattAverTemp <= BATHYBITOTPTHODIS )
      {
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
        if(GPU2Sec == 0){
          if((cGPUThrottling != 0) && ((GPUAlarm & (~GPUAlarm_BATOTP)) == 0)){
            GPU2Sec = 100;
            cGPUThrottling = 0;
          }else{
            CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
          }
        }
        if(ADPI2Sec == 0){
          if((cBATTThrottling != 0) && ((OEM_BAT(M_BATAlarm,battery_id) & (~BATOTP)) == 0)){
            ADPI2Sec = 100;
            cBATTThrottling--;
          }else{
            CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
          }
        }
      }
    }
    else
    {
      CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATOTP);
      CLEAR_MASK(GPU_THROTTLE_SRC, BAT_HYBRID_OTP);
      CLEAR_MASK(GPUAlarm,GPUAlarm_BATOTP);
    }
    CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENOVERTEMP);
    OEM_BAT(M_BATOvrTempCnt,battery_id) =0;
  }
}

void ChkBattery_OCP(int battery_id)
{
  DWORD BattPresentVolt,BattNowCurrent;
  BYTE Temp1;
  WORD Temp2;
  if(battery_id != Current_Battery){
    return;
  }
  if (!Read_AC_IN() || Read_ACOFF())
  {
    // Battery only mode
    if (((BAT_Status & BIT2) >> 2) != battery_id)
    {
    	return;
    }
		if(battery_id==M_BAT)
		{
			Temp1 = M_BAT_Max_Discharge_Watt;
			Temp2 = M_BAT_OCP_Current;
		}
		else if(battery_id==S_BAT)
		{
			Temp1 = S_BAT_Max_Discharge_Watt;
			Temp2 = S_BAT_OCP_Current;
		}
    if ((TWR_BAT(CurrentH,battery_id) & BIT7)) // battery is discharging
    {
      BattNowCurrent = 0xFFFF - (WORD)((TWR_BAT(CurrentH, battery_id)<<8)+TWR_BAT(CurrentL, battery_id)); // mA
      BattPresentVolt = (WORD)((TWR_BAT(VoltageH,battery_id) << 8) + TWR_BAT(VoltageL, battery_id)); // mV
      XWTemp1 = (BattPresentVolt * BattNowCurrent)/1000000; // Watt

      if (XWTemp1 >= Temp1)
      {
        SET_MASK(OEM_BAT(M_BATAlarm, battery_id),BATOCP);    
        UpBatteryPState();
        SET_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        SET_MASK(GPUAlarm,GPUAlarm_BATOCP);
        UpGPUPState();
      }

      if (XWTemp1 <= Temp1 * 0.9)
      {
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        if(GPU2Sec == 0){
          if((cGPUThrottling != 0) && ((GPUAlarm & (~GPUAlarm_BATOCP)) == 0)){
            GPU2Sec = 100;
            cGPUThrottling = 0;
          }else{
          CLEAR_MASK(GPUAlarm,GPUAlarm_BATOCP);
          }
        }
        if(ADPI2Sec == 0){
          if((cBATTThrottling != 0) && ((OEM_BAT(M_BATAlarm, battery_id) & (~BATOCP)) == 0)){
            ADPI2Sec = 100;
            cBATTThrottling--;
          }else{
            CLEAR_MASK(OEM_BAT(M_BATAlarm, battery_id),BATOCP);
          }
        }
      }
      if(BattNowCurrent >= (Temp2 * 0.95))
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTOVERCURR_ID);
        RSMRST_shutdown();
        CLEAR_MASK(OEM_BAT(M_BATAlarm, battery_id),BATOCP);
        CLEAR_MASK(GPU_THROTTLE_SRC, BAT_OCP);
        CLEAR_MASK(GPUAlarm,GPUAlarm_BATOCP);
      }
    }
  }
 
}

/*
*******************************************************************************
* Function name: SettingBattOCP
*
* Descriptoin: 
*          Check battery's "Device Name" and determine battery OCP protect point for "MAX discharge".
* 
* Arguments:
*       int usb_pdc_id
* Return Values:
*       USB_POWER_STATUS 
*******************************************************************************
*/

void SettingBattOCP(BYTE battery_id)
{
	WORD Temp1,Temp3;
	BYTE Temp2;
	
	Temp1 = ((TWR_BAT(DeviceName[4],battery_id)&0x0F)*100)+((TWR_BAT(DeviceName[5],battery_id)&0x0F)*10)+(TWR_BAT(DeviceName[6],battery_id)&0x0F);
	if((TWR_BAT(DeviceName[0],battery_id)=='4')&&(TWR_BAT(DeviceName[1],battery_id)=='5')&&(TWR_BAT(DeviceName[2],battery_id)=='N')&&(TWR_BAT(DeviceName[3],battery_id)=='1'))
	{
		switch(Temp1)
		{	//Setting Max discharge watt
      case 773:
      case 111:
      case 113:
      case 775:
      case 127:
      	Temp2 = 50;
        break;
      case 767:
      case 735:
      case 736:
      case 777:
      case 738:
      	Temp2 = 60;
        break;
	  	default:
      	Temp2 = 50;
        break;
    }
		switch(Temp1)
		{	//Setting OCP current
      case 113:
      case 127:
      case 735:
      case 738:
//      	Temp3 = 10000;
//        break;
      case 773:
      case 111:
      case 775:
      case 767:
      case 777:
//      	Temp3 = 11000;
//        break;
      case 736:
//      	Temp3 = 12000;
//        break;
	  	default:
      	Temp3 = 9474;
        break;
    }
	}
	if((TWR_BAT(DeviceName[0],battery_id)=='0')&&(TWR_BAT(DeviceName[1],battery_id)=='1')&&(TWR_BAT(DeviceName[2],battery_id)=='A')&&(TWR_BAT(DeviceName[3],battery_id)=='V'))
	{
		switch(Temp1)
		{	//Setting Max discharge watt
      case 421:
      case 452:
      	Temp2 = 45;
        break;
      case 419:
      case 420:
      case 422:
      case 423:
      case 424:
      case 425:
      case 426:
      case 427:
      case 428:
      	Temp2 = 50;
        break;
	  	default:
      	Temp2 = 45;
        break;
    }
		switch(Temp1)
		{	//Setting OCP current
      case 424:
      	Temp3 = 6000;
        break;
      case 419:
      case 422:
      case 425:
//      	Temp3 = 9000;
//        break;
      case 427:
//      	Temp3 = 9600;
//        break;
      case 421:
      case 452:
//      	Temp3 = 10500;
//        break;
      case 420:
      case 423:
      case 426:
      case 428:
      	Temp3 = 8948;
        break;
	  	default:
      	Temp3 = 6000;
        break;
    }
	}
	if((battery_id==M_BAT)&&BattExistGet)
	{
		M_BAT_Max_Discharge_Watt = Temp2;
		M_BAT_OCP_Current = Temp3;
	}
	else if((battery_id==S_BAT)&&SecBattExistGet)
	{
		S_BAT_Max_Discharge_Watt = Temp2;
		S_BAT_OCP_Current = Temp3;
	}
}

void ChkBattery_FCCchg(int battery_id)
{
  OEM_BAT(M_BATChk_FCCchg_count,battery_id)++;
  if (OEM_BAT(M_BATChk_FCCchg_count,battery_id) >= 100) // 10 Sec(100ms 100times)
  {
    OEM_BAT(M_BATChk_FCCchg_count,battery_id) = 0;

    // Load default
    if ((OEM_BAT(M_BATChk_FCCchg_lastFCCL, battery_id) == 0) && (OEM_BAT(M_BATChk_FCCchg_lastFCCH, battery_id) == 0))
    {
      OEM_BAT(M_BATChk_FCCchg_lastFCCL, battery_id) = TWR_BAT(FullChgCapL,battery_id);
      OEM_BAT(M_BATChk_FCCchg_lastFCCH, battery_id) = TWR_BAT(FullChgCapH,battery_id);
    }

    // Check FCC and Notify OS if FCC change for each 10 sec
    if ((OEM_BAT(M_BATChk_FCCchg_lastFCCL, battery_id) != TWR_BAT(FullChgCapL,battery_id))
      || (OEM_BAT(M_BATChk_FCCchg_lastFCCH, battery_id) != TWR_BAT(FullChgCapH,battery_id)))
    {
      //ECSMI_SCIEvent(ACPI_BAT0IN_SCI);
      SET_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATIn);
      OEM_BAT(M_BATChk_FCCchg_lastFCCL, battery_id) = FullChgCapL;
      OEM_BAT(M_BATChk_FCCchg_lastFCCH, battery_id) = FullChgCapH;
    }
  }
}

void ChkAvgCurrent(int battery_id)
{
  // Modify for meet specification
  //Average current report to OS
  //Timer<=60 seconds(The timer starts counting when AC adapter plug out.)
  //Report "0x00" to EC name space 0xd2, 0xd3 by one time, and then
  //Report battery Current(0x0a) to EC name space 0xd2, 0xd3
  //Reset condition:When Timer>60 seconds,Report battery AverageCurrent(0x0b) to EC name space 0xd2, 0xd3
  if ( Bat0x0BFakeCnt == 0)
  {
    
  }
  else if(Bat0x0BFakeCnt < 60)
  {
    TWR_BAT(AvgCurrentL, battery_id) = TWR_BAT(CurrentL, battery_id);
    TWR_BAT(AvgCurrentH, battery_id) = TWR_BAT(CurrentH, battery_id);
  }
  else if(Bat0x0BFakeCnt == 60)
  {
    TWR_BAT(AvgCurrentL, battery_id) = 0;
    TWR_BAT(AvgCurrentH, battery_id) = 0;
  }
}

void RST_ChgTimeOutCnt(int battery_id)
{
  if (battery_id == M_BAT){
    M_BATTrickleChgTimeOutCnt = 0;
    M_BATFastChgTimeOutCnt = 0;  
  }else{
    S_BATTrickleChgTimeOutCnt = 0;
    S_BATFastChgTimeOutCnt = 0;  
  }
  CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENSTOPCHG);
  CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENTRITIMEOUT);
}

void Battery100ms(int battery_id)
{

  if (inhibit2sec!=0)
  {
    inhibit2sec--;
    if (inhibit2sec==0)
    {
      CHGIC_ptr = W_InputCurrent;
    }
  }
  else
    {CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENCHARGESUSP);}    // use for detect battery charging suspend

  if( pFakeCHGCnt != 0 )
    {pFakeCHGCnt--;}

}


void Battery1Sec(int battery_id)
{
  if (Bat0x0BFakeCnt != 0)
    Bat0x0BFakeCnt--;

  if (OEM_BAT(M_BATSMbusFailCount,battery_id) != 0)
  {
      OEM_BAT(M_BATErrorCnt,battery_id)++;
      if (OEM_BAT(M_BATErrorCnt,battery_id) > 250)
        OEM_BAT(M_BATErrorCnt,battery_id) = 251;
      if (OEM_BAT(M_BATSMbusFailCount,battery_id) > 220)
        OEM_BAT(M_BATSMbusFailCount,battery_id) = 221;
  }
  else
  {
    OEM_BAT(M_BATErrorCnt,battery_id) = 0;
    if (CurrentL|CurrentH)      // if  CurrentL != 0
    {
      if ((CurrentH&0x80) == 0)    // nNowCurrent > 0, charging
      {                //current(0x0A)>0mA&polling time10cycles
        nBattTempCnt ++;
        if (nBattTempCnt >= 10)
          nBattTempCnt = 11;      // nBattTempCnt > 10
      }
      else                // discharging
      {
        nBattTempCnt = 0;
        RST_ChgTimeOutCnt(battery_id);
      }
    }
    else
    {
      nBattTempCnt = 0;
    }
  }
}

void SbcSetLongLifeMode (int battery_id, 
  BYTE long_life_span_mode
) 
{
  BYTE tempaddr, tempch;
  if (battery_id == M_BAT){             //Main battery
    tempaddr = SmBat_Addr;
    tempch = chSMB_BAT; 
  }else if (battery_id == S_BAT) {      //Second battery
    tempaddr = SecSmBat_Addr;
    tempch = chSMB_2ndBAT; 
  }
//
//  long_life_span_mode -- 0: Long run time mode, 1:Long life span mode
//

  if (!(TWR_BAT(OptMfgFunction2H, battery_id) & COMMON_DUAL_MODE)) {
    // The battery is no dual mode capability, just return.
    return;
  }
//
// Bit 6: Dual Mode Select(0: Long runtime mode(Default), 1:Long Lifespan mode, 
// This bit must be kept until next write operation and must not be reset at OFF 
// State
//

  if (long_life_span_mode) {
    TWR_BAT(OptMfgFunction2L, battery_id)|= BIT6;    // Long Lifespan mode
  } else {
    TWR_BAT(OptMfgFunction2L, battery_id)  &= (~BIT6); // Long runtime mode
  }

  if(!bRWSMBus(tempch, SMbusWW, tempaddr, C_LVMfgFun2, &OptMfgFunction2L , SMBus_NeedPEC))
  {
    OEM_BAT(M_BATSMbusFailCount, battery_id)++;
  }
  //
  // TODO: It needs to add error condition for handling SMBus fail.
  //
}

void IBMChargeCondition(int battery_id)
{
  switch(OEM_BAT_1B(u4MainBattCtrl, battery_id))
  {
    case 0x00:
      
      break;
    case 0x01:  // Reset Battery Charge Inhibit
      Inhibit_chargingCLR;
      CLEAR_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatH,battery_id),ENBOOSTSTOPCHG);
      break;
    case 0x02:  // Peakshift Inhibit Charge
      SET_MASK(u4PeriSTS5,(PriBattinhibit << battery_id));
      Inhibit_chargingSET;
      SET_MASK(M_BATStopChgStatH,ENBOOSTSTOPCHG);
      break;
    case 0x03: // Stop peakshift discharge and Refresh
      if( Read_BATLEARN() )
      {
        BAT_LEARN_OFF();
      }
      if( nAcOffCmdGET )
      {
        BATLEARNProcess(nAcOffCmdDIS);
      }
      CLEAR_MASK(u4PeriSTS5,(PriBattDisChg << battery_id));
      CLEAR_MASK(pBattLearn,uACoffstchg);
      CLEAR_MASK(pBattLearn,uRefreshmode);
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatL, battery_id),ENCUSTOMER);
      break;
    case 0x04: // PeakShiftDischarge
      if( !Read_AC_IN() || (SysPowState == SYSTEM_S3) || NoBattExistGet)
      {
        if( Read_BATLEARN() )
        {
          BAT_LEARN_OFF();
        }
        if( nAcOffCmdGET )
        {
          BATLEARNProcess(nAcOffCmdDIS);
        }
        PeakShiftDischargSTSCLR;
      }
      else
      {
        if( (!Read_BATLEARN()) )
        {
          BAT_LEARN_ON();
        }
        if( (!nAcOffCmdGET) )
        {
          BATLEARNProcess(nAcOffCmdEN);
        }
        PeakShiftDischargSTSSET;
      }
      break;
      case 0x05: // Inhibit to charge battery
      if( ((TWR_BAT(ChargeCurrL,battery_id) + TWR_BAT(ChargeCurrH,battery_id) * 256) != 0) && IS_MASK_SET(OEM_BAT_1B(PriBattSTS, battery_id) ,BattExist))
      {
        if( TWR_BAT(RelativeStateOfChgL, battery_id) <= OEM_BAT_1B(u4MainBattChgOnCap,(battery_id << 1)))  // Start Charge.
        {
          CLEAR_MASK(OEM_BAT(M_BATStopChgStatH, battery_id),ENBOOSTSTOPCHG);
        }
        else if( TWR_BAT(RelativeStateOfChgL, battery_id) >= OEM_BAT_1B(u4MainBattChgOffCap,(battery_id << 1)))  // Stop Charge.
        {
          SET_MASK(OEM_BAT(M_BATStopChgStatH, battery_id),ENBOOSTSTOPCHG);
        }
      }
      break;
    case 0x06: // Refresh battery
      SET_MASK(u4PeriSTS5, (PriBattDisChg << battery_id));
      if( !Read_AC_IN() || (SysPowState == SYSTEM_S3) ||  NoBattExistGet)
      {
        if( Read_BATLEARN() )
        {
          BAT_LEARN_OFF();
        }
        if( nAcOffCmdGET )
        {
          BATLEARNProcess(nAcOffCmdDIS);
        }
//        PeakShiftDischargSTSCLR;
      }
      else
      {
        if( (!Read_BATLEARN()) )
        {
          BAT_LEARN_ON();
        }
        if( (!nAcOffCmdGET) )
        {
          BATLEARNProcess(nAcOffCmdEN);
        }
//        PeakShiftDischargSTSSET;
      }
      
      break;
    case 0x07: // Set long life span mode
      SbcSetLongLifeMode(battery_id, TRUE);
      break;

    case 0x08: // Set long run mode
      SbcSetLongLifeMode(battery_id, FALSE);
      break;

    case 0x09: // Turn off the battery
      Lock_ShipMode();
      break;

    default:
      CLEAR_MASK(OEM_BAT(M_BATStopChgStatH, battery_id),ENBOOSTSTOPCHG);
      break;
  }
  OEM_BAT_1B(u4MainBattCtrl, battery_id) = 0; // clear
}

void Calculate_AVTimeEmpty(int battery_id, BYTE *Data){
  DWORD Temp32;// AVGcurrent, AVGvolatage;//A285D00027-
  Temp32 = (TWR_BAT(RemainCapH, battery_id) << 8) + TWR_BAT(RemainCapL, battery_id);
  Temp32 &= 0x0000FFFF; //A285D00027+
  AVGcurrent = 0xFFFF - ((TWR_BAT(AvgCurrentH, (1^battery_id)) << 8) + TWR_BAT(AvgCurrentL, (1^battery_id)));  
  AVGvolatage = (TWR_BAT(VoltageH, battery_id) << 8) + TWR_BAT(VoltageL, battery_id); //A285D00027+
  /*//A285D00027-: Fix battery low EC exception-->S.
  if((AVGcurrent == 0) || (volatage == 0)) 
  {
  	Temp32 = 0;
  }
  else
  {
  	Temp32 = (Temp32 * 60l) / (AVGcurrent * volatage / (1000 * 10));
	}
 *///A285D00027-: Fix battery low EC exception-->E
   //A285D00027+: Fix battery low EC exception-->S
 if((AVGcurrent != 0) && (AVGvolatage != 0)) 
  {
   //Temp32 = (Temp32 * 60l) / (AVGcurrent * AVGvolatage / (1000 * 10)); //A485D00074-
    Temp32 = ((Temp32 * 60l * 100) / (AVGcurrent * AVGvolatage))*100;//A485D00074+
  }
 else
  {
   Temp32 = 0;
  }
 //A285D00027+: Fix battery low EC exception-->E
  if (MAX_TIME_TO_EMPTY < Temp32) {
    Temp32 = MAX_TIME_TO_EMPTY;
  }  
  *Data = (BYTE)Temp32; 
  *(Data+1) = (BYTE)(Temp32 >> 8);  
}  



const sRSmbusBStruct ABatCommandTable [] =
{//command          1st BAT REG            2nd BAT REG             no.
{ C_mode,           &BattModeL          ,  &SecBattModeL          ,0x00},   //BATpollstep = 1    WORD   
{ C_ChargingV,      &ChargeVoltL        ,  &SecChargeVoltL        ,0x00},   //Batpollstep = 2    WORD
{ C_DCap,           &DesignCapL         ,  &SecDesignCapL         ,0x00},   //Batpollstep = 3    WORD    0~8 initial only
{ C_DVolt,          &DesignVoltL        ,  &SecDesignVoltL        ,0x00},   //Batpollstep = 4    WORD
{ C_Spec,           &SpecInfoL          ,  &SecSpecInfoL          ,0x00},   //Batpollstep = 5    WORD
{ C_Date,           &ManufactureDateL   ,  &SecManufactureDateL   ,0x00},   //Batpollstep = 6    WORD
{ C_SerialNo,       &SerialNumberL      ,  &SecSerialNumberL      ,0x00},   //Batpollstep = 7    WORD
{ C_Mname,          ManufacturerName    ,  SecManufacturerName    ,0x0A},   //Batpollstep = 8    BLOCK
{ C_Dname,          DeviceName          ,  SecDeviceName          ,0x0A},   //Batpollstep = 9    BLOCK
{ C_Dchem,          DeviceChemistry     ,  SecDeviceChemistry     ,0x04},   //Batpollstep = 10   BLOCK
{ C_Mdata,          ManufactureData     ,  SecManufactureData     ,0x0C},   //Batpollstep = 11   BLOCK
{ C_OptMfgFunc5,    OptMfgFunction5     ,  SecOptMfgFunction5     ,0x0E},   //Batpollstep = 12   BLOCK
{ C_BattUsageCond,  BattUsageCondition  ,  SecBattUsageCondition  ,0x0A},   //Batpollstep = 13   BLOCK
{ C_WarrantyPeriod, &WarrantyPeriodL    ,  &SecWarrantyPeriodL    ,0x00},   //Batpollstep = 14   WORD
{ FirstUsedDate,    &OptMfgFunction1L   ,  &SecOptMfgFunction1L   ,0x00},   //Batpollstep = 15   WORD     //First Used Date
{ C_access,         &ManufactureAccessL ,  &SecManufactureAccessL ,0x00},   //Batpollsetp = 16   WORD
{ C_temp,           &TemperatureL       ,  &SecTemperatureL       ,0x00},   //Batpollstep = 17   WORD
{ C_volt,           &VoltageL           ,  &SecVoltageL           ,0x00},   //Batpollstep = 18   WORD
{ C_current,        &CurrentL           ,  &SecCurrentL           ,0x00},   //Batpollstep = 19   WORD
{ C_AVcurrent,      &AvgCurrentL        ,  &SecAvgCurrentL        ,0x00},   //Batpollstep = 20   WORD
{ C_RSOC,           &RelativeStateOfChgL,  &SecRelativeStateOfChgL,0x00},   //Batpollstep = 21   WORD
{ C_RMcap,          &RemainCapL         ,  &SecRemainCapL         ,0x00},   //Batpollstep = 22   WORD  9~19 normal polling
{ C_FCcap,          &FullChgCapL        ,  &SecFullChgCapL        ,0x00},   //Batpollstep = 23   WORD
{ C_RunTimeEmpty,   &RunTimeToEmptyL    ,  &SecRunTimeToEmptyL    ,0x00},   //Batpollstep = 24   WORD
{ C_AVTimeEmpty,    &AvgTimeToEmptyL    ,  &SecAvgTimeToEmptyL    ,0x00},   //Batpollstep = 25   WORD
{ C_AVTimeFull,     &AvgTimeToFullL     ,  &SecAvgTimeToFullL     ,0x00},   //Batpollstep = 26   WORD
{ C_ChargingI,      &ChargeCurrL        ,  &SecChargeCurrL        ,0x00},   //Batpollstep = 27   WORD
{ C_BatStatus,      &BattStatusL        ,  &SecBattStatusL        ,0x00},   //Batpollstep = 28   WORD
{ C_CycleCount,     &CycleCountL        ,  &SecCycleCountL        ,0x00},   //Batpollstep = 29   WORD
{ C_LVMfgFun2,      &OptMfgFunction2L   ,  &SecOptMfgFunction2L   ,0x00},   //Batpollsetp = 30   WORD
{ C_FETTemp,        &FETTemperatureL    ,  &SecFETTemperatureL    ,0x00},   //Batpollstep = 31   WORD
{ C_ChargingV,      &ChargeVoltL        ,  &SecChargeVoltL        ,0x00},   //Batpollstep = 32   WORD
};
//-----------------------------------------------------------------------------
// get 1 battery data
//-----------------------------------------------------------------------------
void GetBatData(BYTE index, int battery_id)
{
  BYTE i;
  BYTE *Tempdata, Bat_ADD,Bat_ch;
  WORD Temp16, Temp_Avg_curr, Temp_curr;
  //check data address and battery smbus address
  if (battery_id == M_BAT){   //1st battery 
    Tempdata = ABatCommandTable[index].smbdataA;
    Bat_ADD = SmBat_Addr;
    Bat_ch = chSMB_BAT;
  }else{                 //2nd battery
    Tempdata = ABatCommandTable[index].smbdataB;
    Bat_ADD = SecSmBat_Addr;
    Bat_ch = chSMB_2ndBAT;
  }
    
  if(ABatCommandTable[index].block_limit != 0)
    {
    if(  !bRSMBusBlock(Bat_ch, SMbusRBK, Bat_ADD,
      ABatCommandTable[index].cmd,Tempdata,bRead_I2C_NULL))
    {  // SMBUS FAIL
      OEM_BAT(M_BATSMbusFailCount, battery_id)++;
	    ResetSMBus(Bat_ch);
    }
    else
    {  // SMBUS OK
      OEM_BAT(M_BATSMbusFailCount, battery_id)=0;
      OEM_BAT(M_BATErrorCnt,battery_id) = 0; 
    }
  }
  else
  {
//Calculate_Average_Time_To_Empty_Using_Another    
    if((ABatCommandTable[index].cmd == C_AVTimeEmpty) && AllBattExistGet && IS_MASK_CLEAR(SYS_STATUS,AC_ADP)){
      if(!bRWSMBus(Bat_ch, SMbusRW, Bat_ADD,
         ABatCommandTable[index].cmd, &WSMbusTemp01, SMBus_NoPEC))
      {  // SMBUS FAIL
        OEM_BAT(M_BATSMbusFailCount, battery_id)++;
	      ResetSMBus(Bat_ch);
      }
      else
      {  // SMBUS OK
        if ((WSMbusTemp01 == 0xFF) && (WSMbusTemp02 == 0xFF)){
          Calculate_AVTimeEmpty(battery_id, &WSMbusTemp01);
        }
        TWR_BAT(AvgTimeToEmptyL,battery_id) = WSMbusTemp01;
        TWR_BAT(AvgTimeToEmptyH,battery_id) = WSMbusTemp02;
        
        OEM_BAT(M_BATSMbusFailCount, battery_id) = 0;
        OEM_BAT(M_BATErrorCnt,battery_id) = 0; 
      }  
    }else{
//get battery data    
      if(!bRWSMBus(Bat_ch, SMbusRW, Bat_ADD,
         ABatCommandTable[index].cmd, Tempdata, SMBus_NoPEC))
      {  // SMBUS FAIL
        OEM_BAT(M_BATSMbusFailCount, battery_id)++;
	      ResetSMBus(Bat_ch);
      }
      else
      {  // SMBUS OK
        OEM_BAT(M_BATSMbusFailCount, battery_id)=0;
        OEM_BAT(M_BATErrorCnt,battery_id) = 0; 
      }
    }
  }

//remove "LNV" prefix"
 if (ABatCommandTable[index].cmd == C_Dname)
  {
    for( i=0; i < 14; i++ )
    {
      if (i < 8){
        if (battery_id == M_BAT)   //1st battery 
          {DeviceName[i]  = DeviceName[i+4];}
        else                    //2nd battery 
          {SecDeviceName[i]  = SecDeviceName[i+4];}
      } else {
        if (battery_id == M_BAT)   //1st battery
          {DeviceName[i] = 0;}
        else                    //2nd battery
          {SecDeviceName[i] = 0;}
      }
    }
  }
//here is for RSOC debug 
 if (ABatCommandTable[index].cmd == C_RSOC)
  {
    if (RSOC1_DEBUG_GET && (battery_id == M_BAT))
      { RelativeStateOfChgL= RSOC1_DEBUG & ~RSOC_DEBUG_EN;}
    if (RSOC2_DEBUG_GET && (battery_id == S_BAT))
      { SecRelativeStateOfChgL= RSOC2_DEBUG & ~RSOC_DEBUG_EN;}
  }

//calculate average current 
  if (ABatCommandTable[index].cmd == C_AVcurrent){
    if (Current_Battery == battery_id){
      if (battery_id == M_BAT){
        Temp_Avg_curr = (AvgCurrentH << 8) + AvgCurrentL;
        Temp_curr = (CurrentH << 8) + CurrentL;
      } else {    //S_BAT
        Temp_Avg_curr = (SecAvgCurrentH << 8) + SecAvgCurrentL;
        Temp_curr = (SecCurrentH << 8) + SecCurrentL;
      }
      if ((Temp_curr == 0) || ((Temp_curr >> 15) != (Temp_Avg_curr >> 15))){
        Temp16 = Temp_curr;
        BAT_Average_curr_sum =0;
      }else if (BAT_Average_curr_CNT != 0){
        BAT_Average_curr_CNT--;
        BAT_Average_curr_sum += Temp_curr;
        Temp16 = BAT_Average_curr_sum / (30 - BAT_Average_curr_CNT);     
      }else{
        BAT_Average_curr_sum =0;
        BAT_Average_curr_CNT =0;
        Temp16 = Temp_Avg_curr;
      }

      if (battery_id == M_BAT){
        AvgCurrentL = Temp16 & 0xFF;
        AvgCurrentH = Temp16 >> 8;
      }else{
        SecAvgCurrentL = Temp16 & 0xFF;
        SecAvgCurrentH = Temp16 >> 8;
      }    
    } else {
      if (battery_id == M_BAT){
        AvgCurrentL = 0;
        AvgCurrentH = 0;
      }else{
        SecAvgCurrentL = 0;
        SecAvgCurrentH = 0;
      }
    }
  }
 
//Battery cut off voltage setting
 if (ABatCommandTable[index].cmd == C_DVolt)
  {
#if (BatLowVot_Rule == LowVot_LGNotLG)
	  if( ((ManufacturerName[0]+(battery_id<<8)) == 'L' ) && ( (ManufacturerName[1]+(battery_id<<8)) == 'G' ) )
	  {	// Shutdown Voltage 8.25V  for LG 3S battery
	  	//OEM_BAT(M_BatLowVoltage,battery_id) = LG_3SLowVot ;
	  	if(battery_id == M_BAT)
	  	{
	  		M_BatLowVoltage = LG_3SLowVot;
	  	}
	  	else
	  	{
	  		S_BatLowVoltage = LG_3SLowVot;
	  	}
	  }
	  else
	  {	// Shutdown Voltage 8.5V for Sanyo battery
	  	//OEM_BAT(M_BatLowVoltage,battery_id) = Sanyo_3SLowVot;
	  	if(battery_id == M_BAT)
	  	{
	  		M_BatLowVoltage = Sanyo_3SLowVot;
	  	}
	  	else
	  	{
	  		S_BatLowVoltage = Sanyo_3SLowVot;
	  	}
	  }
#endif

#if (BatLowVot_Rule == LowVot_3S4S)
	  ITempW01 = (WORD)((TWR_BAT(DesignVoltH,battery_id)<<8)+TWR_BAT(DesignVoltL,battery_id));    //get design voltage
	
        //over 20.0v maybe issue occured--A285D00050: add battery verify debounce sequence
        if (ITempW01 >= BAT3S4SVolt_MaxValue)
        {
            Get_Batt_debounce_count=0;
        }
        
	  if ((ITempW01 >= BAT3S4SVot_Divide) && (ITempW01 < BAT3S4SVolt_MaxValue))
	  {	// Shutdown Voltage 11.9V  for 4S battery
	  	//OEM_BAT(M_BatLowVoltage,battery_id) = BAT4SLowVot;
	  	if(battery_id == M_BAT)
	  	{
	  		M_BatLowVoltage = BAT4SLowVot;
	  	}
	  	else
	  	{
	  		S_BatLowVoltage = BAT4SLowVot;
	  	}
	  }
	  else
	  {	// Shutdown Voltage 8.9V  for 3S battery
	  	//OEM_BAT(M_BatLowVoltage,battery_id) = BAT3SLowVot;
	  	if(battery_id == M_BAT)
	  	{
	  		M_BatLowVoltage = BAT3SLowVot;
	  	}
	  	else
	  	{
	  		S_BatLowVoltage = BAT3SLowVot;
	  	}
	  }
#endif
	}

}


void ChkBattery_Percl()
{
    bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr,C_RSOC,&RelativeStateOfChgL ,SMBus_NoPEC);
#if Support_TWO_BATT
    bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr,C_RSOC,&SecRelativeStateOfChgL ,SMBus_NoPEC);
#endif 
}

//-----------------------------------------------------------------------------
// battery plug in first fast update all information
//-----------------------------------------------------------------------------
void FirstGetBatData(int battery_id)
{
  	BYTE i;
	BYTE Bat_addr, Bat_CH;
	BYTE *tmp, *BatmodeH, *BatmodeL;


  if( battery_id == M_BAT )     //main battery 
	{
	  tmp = &M_BATpollstep;
    Bat_addr = SmBat_Addr;
    BatmodeH = &BattModeH;
    BatmodeL = &BattModeL;
    Bat_CH = chSMB_BAT;
	}
	else if( battery_id == S_BAT )  //2nd battery
	{
	  tmp = &S_BATpollstep;
    Bat_addr = SecSmBat_Addr;
    BatmodeH = &SecBattModeH;
    BatmodeL = &SecBattModeL;    
    Bat_CH = chSMB_2ndBAT;
	}

	*tmp = 0;
    for (i=0;i<(sizeof(ABatCommandTable)/sizeof(sRSmbusBStruct));i++)
    {
    GetBatData(*tmp, battery_id);
    (*tmp)++;
    }
   *tmp = 15;

  UpdateNameSpace(battery_id);
  Chk_Battery_Full(battery_id);
  OEM_BAT(M_BATErrorCnt,battery_id) = 0;
  


  if (OEM_BAT(M_BATSMbusFailCount,battery_id) ==0)
  {
    OEM_BAT(M_BATdata_ready,battery_id) = 1;
    WSMbusTemp01 = *BatmodeH;
    if ((WSMbusTemp01^0xE0)&0xE0)        // check bit15,bit14,bit13=1  ??
    {
      WSMbusTemp01= *BatmodeL;
      WSMbusTemp02= *BatmodeH;
      WSMbusTemp02|=0xE0;
      if(!bRWSMBus(Bat_CH, SMbusWW, Bat_addr, C_mode, &WSMbusTemp01,SMBus_NoPEC))
      {
        OEM_BAT(M_BATSMbusFailCount ,battery_id)++;
      }
    }
  }
}

void Chk_Battery_Full(int battery_id)
{
  if (battery_id != Current_Battery){
    return;
  }

  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
  {
      nBatteryStatH &= ~(PreBat_CHARGE + SecBat_CHARGE);
      SET_MASK(nBatteryStatH,(PreBat_DISCHARGE << battery_id));  //set battery discharging flag
      CLEAR_MASK(nBatteryStatH,(PreBat_DISCHARGE << (1^battery_id)));  //set battery discharging flag
      return;
  }

  if (IS_MASK_SET(cBattFlag0,cCmdAcOff)||Read_ACOFF()||IS_MASK_SET(TWR_BAT(BattStatusL,battery_id),FullyChg)
      ||((OEM_BAT(M_BATStopChgStatL,battery_id) | OEM_BAT(M_BATStopChgStatH,battery_id))!=0))
  {
    nBatteryStatH &= ~(PreBat_CHARGE + PreBat_DISCHARGE + SecBat_CHARGE + SecBat_DISCHARGE);
    return;
  }
  

  if (IS_MASK_SET(TWR_BAT(BattStatusL,battery_id),FullyChg))  // || IS_MASK_SET(CHGIC_ReadCmd0x12L,ChargeInhibit))
  {
     nBatteryStatH &= ~(PreBat_CHARGE + SecBat_CHARGE);
  }

}


void Unlock_ShipMode_ACIN (void)
{
#if Support_TWO_BATT
	//if(SecBattExistGet)
	{
	  WSMbusTemp01=0x00;
	  WSMbusTemp02=0x20;
	  if(!bRWSMBus(chSMB_2ndBAT, SMbusWW, SecSmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	  {
	    S_BATSMbusFailCount++;
	  }
	  else{
	    WSMbusTemp01=0x00;
	    WSMbusTemp02=0x40;
	    if(!bRWSMBus(chSMB_2ndBAT, SMbusWW, SecSmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	    {
	      S_BATSMbusFailCount++;
	    }
	  }
	}
#endif

	//if(BattExistGet)
	{	//Can not detect battery exist status when all battery no discharge,this status will be "0" when ac attaching.
	  WSMbusTemp01=0x00;
	  WSMbusTemp02=0x20;
	  if(!bRWSMBus(chSMB_BAT, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	  {
	    M_BATSMbusFailCount++;
	  }
	  else{
	    WSMbusTemp01=0x00;
	    WSMbusTemp02=0x40;
	    if(!bRWSMBus(chSMB_BAT, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
	    {
	      M_BATSMbusFailCount++;
	    }
	  }
	}
}



void Unlock_ShipMode(int battery_id)
{
  BYTE TempchSMB, TempAddr;

  if (battery_id == M_BAT){
    TempchSMB = chSMB_BAT;
    TempAddr = SmBat_Addr; 
  } else {                     //2bat
    TempchSMB = chSMB_2ndBAT;
    TempAddr = SecSmBat_Addr;
  }
  WSMbusTemp01=0x00;
  WSMbusTemp02=0x00;
  if(bRWSMBus(TempchSMB, SMbusRW, TempAddr, C_access, &WSMbusTemp01,SMBus_NeedPEC))
  {
    if (((WSMbusTemp01 & 0x10) == 0x10) && (WSMbusTemp02 == 0)){ 
      CLEAR_MASK(BAT_Unship_mode, (M_unship_mode << battery_id));
      if (battery_id == S_BAT){
        BAT_Unship_CNT = 30;
      }
      return;
    }
  }
  else
  {
    OEM_BAT(M_BATSMbusFailCount,battery_id)++;
  }

  BAT_unlock_ship_mode_CNT++;
  WSMbusTemp01=0x00;
  WSMbusTemp02=0x20;
  if(!bRWSMBus(TempchSMB, SMbusWW, TempAddr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
  {
    OEM_BAT(M_BATSMbusFailCount,battery_id)++;
  }
  else{
    WSMbusTemp01=0x00;
    WSMbusTemp02=0x40;
    if(!bRWSMBus(TempchSMB, SMbusWW, TempAddr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
    {
      OEM_BAT(M_BATSMbusFailCount,battery_id)++;
    }
  }
}

void Lock_ShipMode(void)
{
  BAT_Unship_mode &= 0xFC;
  BAT_ship_mode_CNT++;
//main battery
  if(BattExistGet
    || (IS_MASK_SET(EVT_STATUS1, M_bat_in) && (IS_MASK_SET(u4PeriCtrl1,Edge_PriBattInhib) )))//A285D00076+
 {
//    if(IS_MASK_SET(OptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
    {
       SET_MASK(uShipModeWorkRun, BAT1_SHIPMODE_ENABLE); //A285D00076+
       CLEAR_MASK(uShipModeWorkRun, BAT1_SHIPMODE_SUCCESS);//A285D00076+
      if(bRWSMBus(chSMB_BAT, SMbusRW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
      {
        if((WSMbusTemp01 == 0x00) && (WSMbusTemp02 == 0x00)){
          WSMbusTemp01=0x00;
          WSMbusTemp02=0x00;
          if(!bRWSMBus(chSMB_BAT, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
          {
            SMbusFailCnt3++;
          }
          else{
            WSMbusTemp01=0x00;
            WSMbusTemp02=0x10;

            if(!bRWSMBus(chSMB_BAT, SMbusWW, SmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
            {
              SMbusFailCnt2++;
            }
            else
            {//A285D00076+
                CLEAR_MASK(uShipModeWorkRun, BAT1_SHIPMODE_ENABLE);
                SET_MASK(uShipModeWorkRun, BAT1_SHIPMODE_SUCCESS);
            }
          }
        }
      }
    }  
  }
  else
  {//A285D00076+
        CLEAR_MASK(uShipModeWorkRun, BAT1_SHIPMODE_ENABLE);
        SET_MASK(uShipModeWorkRun, BAT1_SHIPMODE_SUCCESS);
  }
//Second battery
  if(SecBattExistGet
    || (IS_MASK_SET(EVT_STATUS1, S_bat_in) && (IS_MASK_SET(u4PeriCtrl1, Edge_SecBattInhib) )))
  {
//    if(IS_MASK_SET(SecOptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
    {
       SET_MASK(uShipModeWorkRun, BAT2_SHIPMODE_ENABLE);//A285D00076+
       CLEAR_MASK(uShipModeWorkRun, BAT2_SHIPMODE_SUCCESS);//A285D00076+
      if(bRWSMBus(chSMB_2ndBAT, SMbusRW, SecSmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
      {
        if((WSMbusTemp01 == 0x00) && (WSMbusTemp02 == 0x00)){
          WSMbusTemp01=0x00;
          WSMbusTemp02=0x00;

          if(!bRWSMBus(chSMB_2ndBAT, SMbusWW, SecSmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
          {
            SMbusFailCnt3++;
          }
          else{
            WSMbusTemp01=0x00;
            WSMbusTemp02=0x10;

            if(!bRWSMBus(chSMB_2ndBAT, SMbusWW, SecSmBat_Addr, C_D_FET, &WSMbusTemp01,SMBus_NeedPEC))
            {
              SMbusFailCnt2++;
            }
            else
            {//A285D00076+
                CLEAR_MASK(uShipModeWorkRun, BAT2_SHIPMODE_ENABLE);
                SET_MASK(uShipModeWorkRun, BAT2_SHIPMODE_SUCCESS);
            }
          }
        }
      }
    }  
  }  
  else
  {//A285D00076
        CLEAR_MASK(uShipModeWorkRun, BAT2_SHIPMODE_ENABLE);
        SET_MASK(uShipModeWorkRun, BAT2_SHIPMODE_SUCCESS);
  }
}



//-----------------------------------------------------------------------------
// read all battery about information
// polling time : 1sec
//-----------------------------------------------------------------------------
void OEM_PollingBatData_TASK(int battery_id)
{
  BYTE i,j;
  BYTE *ptr, *tempptr, *SMBdataptr;

  if (IS_MASK_SET(BATUPDATEFW,(PriBattInhib << battery_id)))
  {
    return;
  }
    
  if (battery_id == M_BAT){
     tempptr = &M_BATpollstep;
     SMBdataptr = ABatCommandTable[i].smbdataA;
  }else {
     tempptr = &S_BATpollstep;
     SMBdataptr = ABatCommandTable[i].smbdataB;
  }

  GetBatData(*tempptr, battery_id);

  if (inhibit2sec > 0){
    TWR_BAT(CurrentL,battery_id) = 0;
    TWR_BAT(CurrentH,battery_id) = 0;
  }
  
  (*tempptr)++;

    if(*tempptr >= (sizeof(ABatCommandTable)/sizeof(sRSmbusBStruct)))
    {
    ////////////////////////
    // Battery Debounce Block
    //Get 9 entry from battery table, loop and XOR each byte, calculate a hash byte
    //If hash result same as previous, then counter +1
    //If counter > 3 times, mean battery data stable, keep Batpollstep1 = 9 to skip first 9 entry in battery table
    //otherwise, clean counter and loop the table again.
    if (Get_Batt_debounce_count < BATTERY_HASH_VERIFY_DEBOUNCE_COUNT)
    {
      Get_Batt_debounce_hash2 = Get_Batt_debounce_hash1;
      Get_Batt_debounce_hash1 = 0x00;
      for (i=0; i<9; i++)
      {
        if(ABatCommandTable[i].block_limit == 0x00)
          Get_Batt_debounce_hash1 ^= *SMBdataptr;
        else
        {
          ptr = SMBdataptr;
          for(j=0; j<ABatCommandTable[i].block_limit; j++)
          {
            Get_Batt_debounce_hash1 ^= *ptr;
            ptr++;
          }
        }
      }

      if (Get_Batt_debounce_hash2 == Get_Batt_debounce_hash1)
        Get_Batt_debounce_count++;
      else
        Get_Batt_debounce_count = 0;

      *tempptr = 0; //revert Batpollstep for start over
      // Fill nBatteryStatL after Battery Data stable.     
    }
    else
    {
      *tempptr = 15;
    }
    ////////////////////////////
    UpdateNameSpace(battery_id);
    Chk_Battery_Full(battery_id);
    ChkBattery_OTP(battery_id);
    ChkBattery_OCP(battery_id);
    ChkBattery_FCCchg(battery_id);
    ChkAvgCurrent(battery_id);
//    ChkS3ResumeRSOC();
    ChkFirstUsedDate(battery_id);
    }
}



void Chk_Shutdown_Volt(void)
{
  WORD cutoff_volt,shutdnFlag;		// A285D00026+

  shutdnFlag = FALSE;				// A285D00026+

//#if (BatLowVot_Rule == LowVot_LGNotLG)
//  if (Current_BAT_State == BAT_SEL_M)
//  {
//	  if( (ManufacturerName[0] == 'L' ) && ( ManufacturerName[1] == 'G' ) )
//	  { cutoff_volt = LG_3SLowVot ; }    // Shutdown Voltage 8.25V  for LG 3S battery
//	  else
//	  { cutoff_volt = Sanyo_3SLowVot; }  // Shutdown Voltage 8.5V for Sanyo battery
//	}
//  else if(Current_BAT_State == BAT_SEL_S)
//  {
//	  if( (SecManufacturerName[0] == 'L' ) && ( SecManufacturerName[1] == 'G' ) )
//	  { cutoff_volt = LG_3SLowVot ; }    // Shutdown Voltage 8.25V  for LG 3S battery
//	  else
//	  { cutoff_volt = Sanyo_3SLowVot; }  // Shutdown Voltage 8.5V for Sanyo battery
//	}
//#endif
//
//#if (BatLowVot_Rule == LowVot_3S4S)
//  if (Current_BAT_State == BAT_SEL_M)
//  {
//  	ITempW01 = (WORD)((DesignVoltH<<8)+DesignVoltL);    //get design voltage
//  }
//  else if(Current_BAT_State == BAT_SEL_S)
//  {
//  	ITempW01 = (WORD)((SecDesignVoltH<<8)+SecDesignVoltL);    //get design voltage
//  }
//
//  if (ITempW01 >= BAT3S4SVot_Divide)
//  { cutoff_volt = BAT4SLowVot; }   // Shutdown Voltage 11.9V  for 4S battery
//  else
//  { cutoff_volt = BAT3SLowVot; }   // Shutdown Voltage 8.9V  for 3S battery
//#endif
	// A285D00026+ --->S
	if(AllBattExistGet)
	{
		if((RelativeStateOfChgL==0)&&(SecRelativeStateOfChgL==0))
			shutdnFlag = TRUE;			
	}
	else if(BattExistGet)
	{
		if(RelativeStateOfChgL==0)
			shutdnFlag = TRUE;
	}
	else if(SecBattExistGet)
	{
		if(SecRelativeStateOfChgL==0)
			shutdnFlag = TRUE;
	}
	// A285D00026+ ---->E
	
  if (Current_BAT_State == BAT_SEL_M)
  {
  	ITempW01 = (WORD)((VoltageH<<8)+VoltageL);  // get now voltage
		cutoff_volt = M_BatLowVoltage;
  }
  else if(Current_BAT_State == BAT_SEL_S)
  {
  	ITempW01 = (WORD)((SecVoltageH<<8)+SecVoltageL);  // get now voltage
		cutoff_volt = S_BatLowVoltage;
  }
  else
  {
        return;//A285D00059
  }

  if ((ITempW01 <= cutoff_volt) || (shutdnFlag == TRUE))  //  shutdown Voltage //  A285D00026+
  {
    BatLowCnt++;
    if ( (BatLowCnt>=6)&&((SystemIsS0)||(SystemIsS3)) )
    {
    	BATTBATLOWSHDProt = TRUE;						//  A285D00026+
      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      ProcessSID(BATTLOWVOLT_ID);
      RSMRST_shutdown();  //shutdown (RSMRST HI-->low-->HI)
    }
    else if ( (BatLowCnt>=6)&&(SystemIsS5)&&(Read_EC_ON()) )
       {
        SET_MASK(Thro_Status,b5BAT_TooLow);
        Check_EC_ON();
       }
  }
  else
  { BatLowCnt =0; }
}


void Chk_BAT1PERCL_5(int battery_id)
{
  if ((TWR_BAT(RelativeStateOfChgL, battery_id) <= 5)&&(IS_MASK_SET(TWR_BAT(BattStatusL, battery_id),Dsg)))  // BAT1PERCL <= 5%
  {   
    SET_MASK(OEM_BAT(M_BATAlarm,battery_id),BATPercl_5);
    if (IS_MASK_SET(M_BATAlarm, BATPercl_5) 
#if Support_TWO_BATT
     && IS_MASK_SET(S_BATAlarm, BATPercl_5)
#endif     
      )
      {cBATTThrottling = CPUMAXPSTATE;}
  }
  else
  {
    if (IS_MASK_SET(OEM_BAT(M_BATAlarm,battery_id),BATPercl_5))
    {
      CLEAR_MASK(OEM_BAT(M_BATAlarm,battery_id),BATPercl_5);
      if ((M_BATAlarm == 0)
#if Support_TWO_BATT        
        && (S_BATAlarm == 0)
#endif        
        )
      {
        cBATTThrottling = 0;
      }
    }
  }
}


void Chk_BatSMbusFailCount(int battery_id)
{
//   BYTE sCmd, *sData;
  if (OEM_BAT(M_BATErrorCnt,battery_id)==0)
    {
    CLEAR_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENCOMMFAIL);      // clear bat communication fail and clear STOP charge
    Chk_BAT1PERCL_5(battery_id);
    if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
    {
      Chk_Shutdown_Volt();
    }
//    if( IS_MASK_SET(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake) )
//    {
//      if( pWakeCnt  <= 4 )
//      {
//        pWakeCnt++;
//      }
//      else
//      {
//        if (((TWR_BAT(ChargeCurrH,battery_id) << 8) + TWR_BAT(ChargeCurrL,battery_id)) >= 300 ){     //charge current > 300mA disable trickle charge
//          CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake);
//        }
//        pWakeCnt=0;
//      }
//    }

    return;
    }

  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))    // discharge
  {
    RST_ChgTimeOutCnt(battery_id);
//      if (OEM_BAT(M_BATErrorCnt,battery_id)==30)
    if (((OEM_BAT(M_BATErrorCnt,battery_id)==30) && (IS_MASK_CLEAR(OEM_BAT_1B(PriBattSTS,(1^battery_id)),BattExist))) ||
        ((OEM_BAT(M_BATErrorCnt,battery_id)==30) &&(OEM_BAT(M_BATErrorCnt,(1^battery_id))==30) && (IS_MASK_SET(OEM_BAT_1B(PriBattSTS,(1^battery_id)),BattExist))))
    {
      cBATTThrottling = CPUMAXPSTATE;

      SET_MASK(SysStatus,ERR_ShuntDownFlag);
      TWR_BAT(RemainCapL,battery_id) = 0;
      TWR_BAT(RemainCapH,battery_id) = 0;
      SMbusFailCnt2++;
    }
//      if (OEM_BAT(M_BATErrorCnt,battery_id)>=150)
    if (((OEM_BAT(M_BATErrorCnt,battery_id)>=150) && (IS_MASK_CLEAR(OEM_BAT_1B(PriBattSTS,(1^battery_id)),BattExist))) ||
        ((OEM_BAT(M_BATErrorCnt,battery_id)>=150) &&(OEM_BAT(M_BATErrorCnt,(1^battery_id))>=150) && (IS_MASK_SET(OEM_BAT_1B(PriBattSTS,(1^battery_id)),BattExist))))
    {
#if !EN_PwrSeqTest
      cBATTThrottling = CPUMAXPSTATE;
      SMbusFailCnt3++;
      OEM_BAT(M_BATErrorCnt, battery_id) = 151;
      if ((SysPowState==SYSTEM_S0)||(SysPowState==SYSTEM_S3))
      {
        SET_MASK(SysStatus,ERR_ShuntDownFlag);
        ProcessSID(BATTCOMMFAIL_ID);
        RSMRST_shutdown();   
      }
#endif
    }
  }
  else
  {
    if (IS_MASK_SET(SYS_STATUS,AC_ADP)&&(!Read_ACOFF()))          // battery charge
    {
      if( pWakeCnt != 0)
      {
        pWakeCnt = 0;
      }
      if ((OEM_BAT(M_BATErrorCnt,battery_id) != 0)&&(OEM_BAT(M_BATErrorCnt,battery_id) < 30))
      {
        SMbusFailCnt3++;
      }
    }
  }
}


void Chk_CHG_TimeOut(int battery_id)      // every 1 min
{ 
  WORD *Prttrickle, *Prtfast;
  if (battery_id == M_BAT){
    Prttrickle = &M_BATTrickleChgTimeOutCnt;
    Prtfast =  &M_BATFastChgTimeOutCnt; 
  }else{
    Prttrickle = &S_BATTrickleChgTimeOutCnt;
    Prtfast =  &S_BATFastChgTimeOutCnt; 
  }

  
  if( (nBattTempCnt < 10)&&(IS_MASK_CLEAR(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake)) )
    return;
  if((TWR_BAT(ChargeCurrH,battery_id) > 1) || ((TWR_BAT(ChargeCurrH,battery_id) == 1) && (TWR_BAT(ChargeCurrL,battery_id) >= 0xF4)))
  {
 
    *Prttrickle = 0;
    (*Prtfast)++;
    if (*Prtfast >= 720)    // 12hour
    {
      *Prtfast = 721;
      SET_MASK(OEM_BAT(M_BATStopChgStatL,battery_id),ENSTOPCHG);
    }
  }
  else                  // nBattCharCurrent < 0x01F4  (500mA)
  {
    if (battery_id == M_BAT){ 
      if (!M_CTRL_GET()){  
        return;
      }
    }
    
    if (battery_id == S_BAT){
      if (!S_CTRL_GET()){  
      return;
      }
    }
    
    *Prtfast = 0;
    (*Prttrickle)++;
    if (*Prttrickle >= 480)    // 8hour
    {
      *Prttrickle = 481;
      SET_MASK(OEM_BAT(M_BATStopChgStatH,battery_id),ENTRITIMEOUT);
    }
  }       
}

void ChkBattery_abnormal(int battery_id)
{
  if(IS_MASK_SET(BATUPDATEFW,(PriBattInhib << battery_id )))
    return;

  switch(OEM_BAT(M_BATChkBattery_abnormal_status,battery_id))
  {
    case ChkBattery_abnormal_status_normal:
      if(IS_CHG_inhibit
        && IS_MASK_CLEAR(TWR_BAT(CurrentH, battery_id), BIT7) && (TWR_BAT(CurrentL,battery_id) > 100) )
      {
        OEM_BAT(M_BATChkBattery_abnormal_count,battery_id) ++;
        if (OEM_BAT(M_BATChkBattery_abnormal_count,battery_id)>5)
        {
          OEM_BAT(M_BATChkBattery_abnormal_status,battery_id) = ChkBattery_abnormal_status_error;
        }
      }
      else
      {
        OEM_BAT(M_BATChkBattery_abnormal_status,battery_id) = ChkBattery_abnormal_status_normal;
        OEM_BAT(M_BATChkBattery_abnormal_count,battery_id) = 0;
      }
      break;
    case ChkBattery_abnormal_status_error:
      // Reset until EC power down
      SET_MASK(ACOFF_SOURCE, CHARGECURRENT);
      ACOFF_HI();
      break;
    default:
      OEM_BAT(M_BATChkBattery_abnormal_status,battery_id) = ChkBattery_abnormal_status_normal;
      OEM_BAT(M_BATChkBattery_abnormal_count,battery_id) = 0;
      break;
  }
}



BYTE SendChallengeToBat(void)
{
  BYTE SMBus_work;
#if Thinkpad_Battery_HW_Auth
  SMBus_work = bWSMBusBlock(chSMB_2ndBAT, SMbusWBK, SecSmBat_Addr, 0x27, LV_HW_BatteryChallengeKey, 17, TRUE);
#endif
  if( SMBus_work ) return TRUE;
  return FALSE;
}

BYTE GetChallengeFromBat(void)
{
#if Thinkpad_Battery_HW_Auth
  if( bRSMBusBlock(chSMB_2ndBAT, SMbusRBK, SecSmBat_Addr, 0x28, (unsigned char *)LV_HW_BatteryResponseX, bRead_I2C_NULL) ) return TRUE;
#endif
  return FALSE;
}

void LV_BAT_Authentication(void)
{
#if Thinkpad_Battery_HW_Auth
  int j;
#endif

 // if(((WORD)((TWR_BAT(VoltageH,S_BAT)<<8)+TWR_BAT(VoltageL,S_BAT)))< OEM_BAT(M_BatLowVoltage,S_BAT))  //A485D00017-//A485D00079+
  //A485D00017: Enable Battery SW authentication.
  if(((WORD)((TWR_BAT(VoltageH,S_BAT)<<8)+TWR_BAT(VoltageL,S_BAT)))< //A485D00079-
 	((WORD)((OEM_BAT(M_BatLowVoltage_HB,S_BAT)<<8)+OEM_BAT(M_BatLowVoltage_LB,S_BAT)))) //A285D00039A485D00095 A485D00017+//A485D00079-
  { return; }
  //===================================================================
  // Service SHA1 algorithm
  //===================================================================
  switch(Service_Auth_Step)
  {
    case 1:
//      InitializeFix();    // initial H and W(0~4/9~15)
//      SetRandomKey(1);    // initial W(4~8)
#if Thinkpad_Battery_HW_Auth
      for (j=0;j<4;j++){
        LV_HW_BatteryResponseX[j] = 0x0000;
      }
      for (j=0;j<9;j++){
        LV_HW_BatteryResponseZ[j] = 0x0000;
      }
      CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
      CLEAR_MASK(u4SecBattType,u4PriBattTypeHWauthenticated);
      CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
     // AuthenticationFailedDis;
      CLEAR_MASK(SecTWR00,AuthenticationFailed);
      //Service_Auth_Step=2;
      SET_MASK(SecTWR06,HWauthSupport);
#endif
#if Lenovo_Authentication
      SET_MASK(S_BAT_uAuthenFlag,uDF_SWstart);
#endif
      Service_Auth_Step=21;
      break;
    case 21:          // SHA1 start send data delay 10 sec
      if(IS_MASK_SET(SecOptMfgFunction2L, BIT7)) // Replaceable(0)/Non-replaceable(1)
      {
#if Lenovo_Authentication
        //
        // Embedded built-in battery (Non-replaceable)
        // Only support SW authentication.
        //
        if ( IS_MASK_SET(S_BAT_edgeProject4,uECAuthTruePass) && IS_MASK_SET(S_BAT_edgeProject4,uAuthComplete))
        {
          CLEAR_MASK(SecTWR00,AuthenticationFailed);
          CLEAR_MASK(S_BATStopChgStatH,NotLenovoBattery);
          CLEAR_MASK(SecBattSTS,NonGenBatt);// NonGenuineBattDis;
          SET_MASK(SecBattAuthStat,OEMauthPass);
          SET_MASK(SecBattAuthStat,SWauthPass);
        }
        else
        {
          if ( (S_BAT_pFKTemp >= 30) 
            && (S_BAT_Batt0x3CFailRetry5sTimer != 0xFF) 
            && (S_BAT_Batt0x3CFailRetry5sTimer > 0) ) 
          {
            //
            // A workaround for battery w/ special case. Ex. Panasonic has TDA protection, the BMU would not connectable while low voltage.
            // Set to auth pass while BIOS POST and re-auth again once after 5s.
            //
           CLEAR_MASK(S_BATStopChgStatH,NotLenovoBattery); // Keep charging to power up BMU
            //NonGenuineBattDis;  // BIOS check this flag for SW auth result.
          } else {
            SET_MASK(SecTWR00,AuthenticationFailed);
            SET_MASK(S_BATStopChgStatH,NotLenovoBattery); 
            SET_MASK(SecBattSTS,NonGenBatt);//NonGenuineBattEn;  // BIOS check this flag for SW auth result. 
            CLEAR_MASK(SecBattAuthStat,OEMauthPass);
            CLEAR_MASK(SecBattAuthStat,SWauthPass);
          }
        }
#endif
        Service_Auth_Step=0;
      }
      else
      {
#if Thinkpad_Battery_HW_Auth
        // Processed Send challenge key only if We get CH and CV from BIOS.
        if (IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryDataReady))
        {
          SendChallengeToBat();  // send to battery
          if(SHA1failCnt == 0) {
            Service_Auth_Step=22;
          } else {
            Service_Auth_Step=23;
          }
        }
#else
            Service_Auth_Step=25;
#endif
      }
      break;
    case 22:
    case 23:
    case 24:
      Service_Auth_Step++;
      break;
    case 25:
#if Thinkpad_Battery_HW_Auth
      //GetChallengeFromBat(); //A285D00076-
	  //A285D00025+  --->S
      if(!GetChallengeFromBat())
      {
      	HWAUthSMBusFailCnt++;
		return;
      }
	  //A285D00025+  --->E
#endif
      Service_Auth_Step=0;
     
     
#if Thinkpad_Battery_HW_Auth
      for (j=0;j<9;j++) {
        LV_HW_BatteryCheckValue[j] = (((LV_HW_BatteryCheckValue[j] & 0xFF00) >> 8)
                                   | ((LV_HW_BatteryCheckValue[j] & 0x00FF) << 8));
      }
      //Compare_Auth_Result();
      if (Ecc_VerifyResponse(LV_HW_BatteryResponseX,LV_HW_BatteryResponseZ,LV_HW_BatteryCheckValue))	
      {
        SET_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        CLEAR_MASK(SecBattAuthStat,HWauthFail);
        CLEAR_MASK(u4SecBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
      }
      else
      {
        SHA1failCnt ++;
        if (SHA1failCnt < 3)
        {
          for (j=0;j<4;j++){
            LV_HW_BatteryResponseX[j] = 0x0000;
          }
          for (j=0;j<9;j++){
            LV_HW_BatteryResponseZ[j] = 0x0000;
          }
          Service_Auth_Step = 21;           //if  SHA1failCnt <3, retry
        }
        else
        {	// Set fail if retry 3
	        SET_MASK(SecTWR00, AuthenticationFailed);
	        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
	        SET_MASK(SecBattAuthStat,HWauthFail);
	        SET_MASK(u4SecBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
        }
      }
      
      SET_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthFinished);
      if( IS_MASK_SET(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed)) {
#endif
#if Lenovo_Authentication
        if ( IS_MASK_SET(S_BAT_edgeProject4,uECAuthTruePass) && IS_MASK_SET(S_BAT_edgeProject4,uAuthComplete) )
#endif
        {
          CLEAR_MASK(S_BATStopChgStatH,NotLenovoBattery); // Keep charging to power up BMU
          CLEAR_MASK(SecTWR00,AuthenticationFailed);
          SET_MASK(SecBattAuthStat,OEMauthPass);
          SET_MASK(SecBattAuthStat,SWauthPass);
          CLEAR_MASK(SecBattSTS,NonGenBatt);//NonGenuineBattDis;
        }
#if Thinkpad_Battery_HW_Auth
      }
      else
      {
        if ( (S_BAT_pFKTemp >= 30) 
          && (S_BAT_Batt0x3CFailRetry5sTimer != 0xFF) 
          && (S_BAT_Batt0x3CFailRetry5sTimer > 0) ) 
        {
          //
          // A workaround for battery w/ special case. Ex. Panasonic has TDA protection, the BMU would not connectable while low voltage.
          // Set to auth pass while BIOS POST and re-auth again once after 5s.
          //

          CLEAR_MASK(S_BATStopChgStatH,NotLenovoBattery); // Keep charging to power up BMU
          CLEAR_MASK(SecBattSTS,NonGenBatt);//NonGenuineBattDis;  // BIOS check this flag for SW auth result.
          CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
        } else {
        	if (SHA1failCnt >= 3)
        	{
	          SET_MASK(SecTWR00,AuthenticationFailed);
	          SET_MASK(S_BATStopChgStatH,NotLenovoBattery); 
	          CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryAuthPassed);
	          SET_MASK(SecBattAuthStat,HWauthFail);
	          SET_MASK(u4SecBattType,u4PriBattTypeHWauthenticated); // BIOS check this flag for HW auth result.
	        }
        }
      }
#endif
      //If We're in ACPI_OS mode, Send Q78 to notify and get new Data
      //Otherwise, We need clear LV_HW_BatteryDataReady flag before POST,
      // And get new Data while POST procedure.
      if ( IS_MASK_SET(SYS_MISC1, ACPI_OS) )
      {
#if Thinkpad_Battery_HW_Auth
        CLEAR_MASK(LV_HW_BatteryControl,LV_HW_BatteryDataReady);
#endif
        ECSMI_SCIEvent(GEN_CH_CV_EVENT_78);
      }
      break;
    case 0:
      Service_Auth_Step=0;
      break;
    default:
      Service_Auth_Step ++;
      break;
  }
//#endif // Thinkpad_Battery_HW_Auth

}


void ACOffProcess(BYTE ACOffState)
{
  if (ACOffState)
  {
    SET_MASK(cBattFlag0,cCmdAcOff);
    CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
    ACOFF_HI();
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);
  }
  else if (!ACOffState)
  {
    CLEAR_MASK(cBattFlag0,cCmdAcOff);
    SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
    ACOFF_LOW();
    ECSMI_SCIEvent(ACPI_ACIN_SCI);
  }
}

void BATLEARNProcess(BYTE ACOffState)
{
  if (ACOffState)
  {
    SET_MASK(cBattFlag0,cCmdAcOff);
    CLEAR_MASK(SYS_STATUS,AC_ADP);  //Clear AC in  flag
    ECSMI_SCIEvent(ACPI_ACOUT_SCI);
  }
  else if (!ACOffState)
  {
    CLEAR_MASK(cBattFlag0,cCmdAcOff);
    SET_MASK(SYS_STATUS,AC_ADP);  //Set AC in  flag
    ECSMI_SCIEvent(ACPI_ACIN_SCI);
  }
}


void ChkFirstUsedDate(int battery_id)
{
  BYTE Temp_Bat_Addr;
  BYTE Temp_Bat_SMBusChl;
  if (battery_id == M_BAT)
  {
  	Temp_Bat_Addr = SmBat_Addr;
  	Temp_Bat_SMBusChl = chSMB_BAT;
  }
  else
  {
  	Temp_Bat_Addr = SecSmBat_Addr;
  	Temp_Bat_SMBusChl = chSMB_2ndBAT;
  }
  
  if( (IS_MASK_SET(uProject0,uFUDWriteEN)&&(battery_id == M_BAT)) || (IS_MASK_SET(uBattFUDStatus,uFUDWriteEN2)&&(battery_id == S_BAT)) )
  {
    if(bRWSMBus(Temp_Bat_SMBusChl, SMbusRW, Temp_Bat_Addr, FirstUsedDate,(unsigned char *) &p2FUDayTemp,SMBus_NeedPEC))
    {
      TWR_BAT(OptMfgFunction1H, battery_id) = (p2FUDayTemp >> 8) & 0xFF;
      TWR_BAT(OptMfgFunction1L, battery_id) = p2FUDayTemp & 0xFF;
    }
    if( (TWR_BAT(OptMfgFunction1L, battery_id) == 0x00) && (TWR_BAT(OptMfgFunction1H, battery_id) == 0x00) && (p2FUDay != 0) )
    {
//      p2FUDayTemp = (p2FUDay & 0xff) << 8 ;
//      p2FUDayTemp = p2FUDayTemp +((p2FUDay & 0xff00) >> 8) ;
      p2FUDayTemp = p2FUDay;
      if(bRWSMBus(Temp_Bat_SMBusChl, SMbusWW, Temp_Bat_Addr, FirstUsedDate,(unsigned char *) &p2FUDayTemp,SMBus_NeedPEC))
      {
        bRWSMBus(Temp_Bat_SMBusChl, SMbusRW, Temp_Bat_Addr, FirstUsedDate,(unsigned char *) &p2FUDayTemp,SMBus_NeedPEC);
	      TWR_BAT(OptMfgFunction1H, battery_id) = (p2FUDayTemp >> 8) & 0xFF;
	      TWR_BAT(OptMfgFunction1L, battery_id) = p2FUDayTemp & 0xFF;
        if( (TWR_BAT(OptMfgFunction1L, battery_id) != 0x00) && (TWR_BAT(OptMfgFunction1H, battery_id) != 0x00) )
        {
  				if (battery_id == M_BAT)
  				{
          	CLEAR_MASK(uProject0,uFUDWriteEN);
            CLEAR_MASK(uProject0, uFUDUpdateTimes); // Clear Flag of Bat0Plug
          }
          else
          {
          	CLEAR_MASK(uBattFUDStatus,uFUDWriteEN2);
            CLEAR_MASK(uBattFUDStatus, uFUDUpdateTimes); // Clear Flag of Bat1Plug
          }
//          p2FUD_Year = 0;
//          p2FUD_Month = 0;
//          p2FUD_Day = 0;
//          p2FUDayTemp = 0;
//          p2FUDay = 0;
        }
      }
    }
    else if( (TWR_BAT(OptMfgFunction1L, battery_id) != 0x00) && (TWR_BAT(OptMfgFunction1H, battery_id) != 0x00) )
    {
			if (battery_id == M_BAT)
			{
      	CLEAR_MASK(uProject0,uFUDWriteEN);
      }
      else
      {
      	CLEAR_MASK(uBattFUDStatus,uFUDWriteEN2);
      }
//      p2FUD_Year = 0;
//      p2FUD_Month = 0;
//      p2FUD_Day = 0;
//      p2FUDayTemp = 0;
//      p2FUDay = 0;
    }
  }
}

void BatteryFirstUseDay(void)
{
  if( IS_MASK_SET(uProject0,uSysTimeOK) )
  {
    p2FUDayTemp = p2FUD_Year - 0x7BC;  //Year -1980
    if( p2FUDayTemp <= 127 )
    {
      p2FUDay = p2FUDayTemp << 9 ;
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }
    p2FUDayTemp = p2FUD_Month;  //Month
    if( p2FUDayTemp <= 12 )
    {
      p2FUDay = p2FUDay + (p2FUDayTemp << 5);
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }
    p2FUDayTemp = p2FUD_Day;  //Day
    if( p2FUDayTemp <= 31 )
    {
      p2FUDay = p2FUDay + p2FUDayTemp;
    }
    else
    {
      SET_MASK(uProject0,uFUdayFail);
    }


  }
  if( IS_MASK_SET(uProject0,uSysTimeOK) && IS_MASK_CLEAR(uProject0,uFUdayFail) )
  {
    SET_MASK(uProject0,uFUDWriteEN);
    SET_MASK(uBattFUDStatus,uFUDWriteEN2);
    CLEAR_MASK(uProject0,uSysTimeOK);
  }
}

#if Lenovo_Authentication
/*===========================================================================
 * Name : Authentication
 *
 * Authentication of Lenovo battery
 *
 * Input  :     NONE
 * Output :     NONE
 *===========================================================================*/

void Authentication(int battery_id)
{
  WORD TEPMRpc, TEMPRpack, TEMPNpack, TEMPNpc;
  if (battery_id == M_BAT){
    TEPMRpc = (M_BAT_uAuthenRand1 << 8) + M_BAT_uAuthenRand2;
    TEMPRpack = (M_BAT_uAuthenRPackH << 8) + M_BAT_uAuthenRPackL;
    TEMPNpack = (M_BAT_uAuthenNPackH << 8) + M_BAT_uAuthenNPackL;
  } else if (battery_id == S_BAT){
    TEPMRpc = (S_BAT_uAuthenRand1 << 8) + S_BAT_uAuthenRand2;
    TEMPRpack = (S_BAT_uAuthenRPackH << 8) + S_BAT_uAuthenRPackL;
    TEMPNpack = (S_BAT_uAuthenNPackH << 8) + S_BAT_uAuthenNPackL;
  }
  if( IS_MASK_SET(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return) )
  {
//(Rpc_L+Rpack_L*16)*(Rpc_H+Rpack_H*16)+0x339E)
    TEMPNpc = ((TEPMRpc & 0xff) + (TEMPRpack & 0xff) * 16) * ((TEPMRpc >> 8) + (TEMPRpack >> 8) * 16) + 0x339e;
    if (battery_id == M_BAT){
      M_BAT_uAuthenNPC = TEMPNpc;
    } else if (battery_id == S_BAT){
      S_BAT_uAuthenNPC = TEMPNpc;
    }    
  }
  // Verify
  if( ((IS_MASK_SET(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return) && (TEMPNpack != TEMPNpc))|| IS_MASK_SET(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_ReadFail)
    || IS_MASK_SET(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_ErrorReturn)) && IS_MASK_CLEAR(OEM_BAT(M_BATStopChgStatH,battery_id),NotLenovoBattery) )
  {
    OEM_BAT(M_BAT_uAuthenRand1,battery_id) = LeftRotateBYTE(OEM_BAT(M_BAT_uAuthenRand1,battery_id),1);
    OEM_BAT(M_BAT_uAuthenRand2,battery_id) = LeftRotateBYTE(OEM_BAT(M_BAT_uAuthenRand2,battery_id),2);
    OEM_BAT(M_BAT_uAuthenRand3,battery_id) = LeftRotateBYTE(OEM_BAT(M_BAT_uAuthenRand3,battery_id),3);
    OEM_BAT(M_BAT_uAuthenRand4,battery_id) = LeftRotateBYTE(OEM_BAT(M_BAT_uAuthenRand4,battery_id),4);
    OEM_BAT(M_BAT_uAuthenFailCnt,battery_id)++;
    OEM_BAT(M_BAT_uAuthenFlag,battery_id) = 0;
    CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uLENOVO_Error);
    CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uLenovo_JapanError);
    CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),usLenovo_Error);
    //p2OrigaFlag.bit.p2AuthFinish = 0;
    CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uSWDone);  // Retry when fail.
    SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_HWstart);
    SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_SWstart);  // SW authentication
  }
  else if( IS_MASK_SET(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return) && (TEMPNpack == TEMPNpc) )
  {
    OEM_BAT(M_BAT_uAuthenFailCnt,battery_id) = 0;
    if(IS_MASK_SET(OEM_BAT_1B(PriBattSTS,battery_id),BattExist))
    {
      CLEAR_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_HWstart);
      //SET_MASK(BattAuthStat,SWauthPass);
      //CLEAR_MASK(TWR00,AuthenticationFailed);
      CLEAR_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_SWstart);  // SW authentication done.
      CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uAuthSWstatus);  // SW isn't fail, then HW fail won't be set.
      SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uSWDone);
      SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uECAuthTruePass);
      SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uAuthComplete);
      //A285D00055: fix battery auth success bios show error for status unsync
      CLEAR_MASK(OEM_BAT_1B(PriBattSTS,battery_id),NonGenBatt);
    }
  }

  if( OEM_BAT(M_BAT_uAuthenFailCnt,battery_id) >= 2 )
  {
    //uAuthenFailCnt = 0;
    CLEAR_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_HWstart);
    CLEAR_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_SWstart);
    //BattAuthStat.bit.SWauthPass = 0;    // PM Bit7
    SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uAuthSWstatus);    // SW is fail, then HW fail will set.
    SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uSWDone);
    CLEAR_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uECAuthTruePass);
    //SET_MASK(TWR00,AuthenticationFailed); 
    
    SET_MASK(TWR_BAT(TWR00,battery_id),AuthenticationFailed);
    SET_MASK(OEM_BAT(M_BATStopChgStatH,battery_id),NotLenovoBattery); 
    //NonGenuineBattEn;  // BIOS check this flag for SW auth result. 
    SET_MASK(OEM_BAT_1B(PriBattSTS,battery_id),NonGenBatt);
    CLEAR_MASK(TWR_BAT(BattAuthStat,battery_id),OEMauthPass);
    CLEAR_MASK(TWR_BAT(BattAuthStat,battery_id),SWauthPass);
  }
}

void LAuthProcess(int battery_id)
{
  BYTE result;
  //if(((WORD)((TWR_BAT(VoltageH,battery_id)<<8)+TWR_BAT(VoltageL,battery_id)))< OEM_BAT(M_BatLowVoltage,battery_id))  //A485D00017-//A485D00079+
 //A485D00017: Enable Battery SW authentication.
 if(((WORD)((TWR_BAT(VoltageH,battery_id)<<8)+TWR_BAT(VoltageL,battery_id)))< 
 	((WORD)((OEM_BAT(M_BatLowVoltage_HB,battery_id)<<8)+OEM_BAT(M_BatLowVoltage_LB,battery_id))))//A285D00039 A485D00095//A485D00017+ //A485D00079-
  { return; }
 
  if(IS_MASK_SET(OEM_BAT_1B(PriBattSTS,battery_id),BattExist)&& IS_MASK_CLEAR(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Done))
  {
    if( IS_MASK_CLEAR(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Write))
    {
      if(battery_id == M_BAT)
	  	    {result = bWSMBusBlock(chSMB_BAT, SMbusWBK, SmBat_Addr, AuthenticationCmd, &M_BAT_uAuthenRand1, 4, TRUE);}
	    else if(battery_id == S_BAT)
	  	    {result = bWSMBusBlock(chSMB_2ndBAT, SMbusWBK, SecSmBat_Addr, AuthenticationCmd, &S_BAT_uAuthenRand1, 4, TRUE);}
      if(result)
      {
        SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Write);
        OEM_BAT(M_BAT_uAuthenReadAfter1Sec,battery_id) = 0;
        OEM_BAT(M_BAT_pFKTemp,battery_id) = 0;
      }
      else
      {
        OEM_BAT(M_BAT_pFKTemp,battery_id)++;
        if (OEM_BAT(M_BAT_pFKTemp,battery_id) >= 30) {
          if (OEM_BAT(M_BAT_Batt0x3CFailRetry5sTimer,battery_id) == 0) 
            {OEM_BAT(M_BAT_Batt0x3CFailRetry5sTimer,battery_id) = 5;}
          else if (OEM_BAT(M_BAT_Batt0x3CFailRetry5sTimer,battery_id) == 0xFF) 
            {
            // After a 5s retry, 0x3C cmd is still failed.
            // Set battery is illegal.
           
            	SET_MASK(TWR_BAT(TWR00,battery_id),AuthenticationFailed);
            	SET_MASK(OEM_BAT(M_BATStopChgStatH,battery_id),NotLenovoBattery);
            	//NonGenuineBattEn;  // BIOS check this flag for SW auth result.	
            	SET_MASK(OEM_BAT_1B(PriBattSTS,battery_id),NonGenBatt);
            	CLEAR_MASK(TWR_BAT(BattAuthStat,battery_id),OEMauthPass);
            	CLEAR_MASK(TWR_BAT(BattAuthStat,battery_id),SWauthPass);
            
          }
        }
      }
    }
    else if( IS_MASK_CLEAR(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Read))
    {
      if( OEM_BAT(M_BAT_uAuthenReadAfter1Sec,battery_id) >= delayReadCnt )
      {
        if(battery_id == M_BAT)
	  		    {result = bRSMBusBlock(chSMB_BAT, SMbusRBK, SmBat_Addr, AuthenticationCmd, M_BAT_LV_SW_SIGNERTURE, bRead_I2C_NULL);}
	  	    else if(battery_id == S_BAT)
	  		    {result = bRSMBusBlock(chSMB_2ndBAT, SMbusRBK, SecSmBat_Addr, AuthenticationCmd, S_BAT_LV_SW_SIGNERTURE, bRead_I2C_NULL);}
      	  if(result)
         {
           SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Read);
           SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Done);

           if((OEM_BAT(M_BAT_LV_SW_SIGNERTURE[0],battery_id) != 0x4C) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[1],battery_id) != 0x45) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[2],battery_id) != 0x4E) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[3],battery_id) != 0x4F) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[4],battery_id) != 0x56) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[5],battery_id) != 0x4F))
            {
              SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uLENOVO_Error);
            }

            if((OEM_BAT(M_BAT_LV_SW_SIGNERTURE[0],battery_id) != 0x4C) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[1],battery_id) != 0x65) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[2],battery_id) != 0x6E) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[3],battery_id) != 0x6F) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[4],battery_id) != 0x76) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[5],battery_id) != 0x6F) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[6],battery_id) != 0x20) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[7],battery_id) != 0x4A) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[8],battery_id) != 0x61) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[9],battery_id) != 0x70) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[10],battery_id) != 0x61) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[11],battery_id) != 0x6E))
            {
              SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),uLenovo_JapanError);
            }
          
            if((OEM_BAT(M_BAT_LV_SW_SIGNERTURE[0],battery_id) != 0x4C) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[1],battery_id) != 0x65) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[2],battery_id) != 0x6E) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[3],battery_id) != 0x6F) ||
            (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[4],battery_id) != 0x76) || (OEM_BAT(M_BAT_LV_SW_SIGNERTURE[5],battery_id) != 0x6F))
            {
              SET_MASK(OEM_BAT(M_BAT_edgeProject4,battery_id),usLenovo_Error);
            }

            if( IS_MASK_CLEAR(OEM_BAT(M_BAT_edgeProject4,battery_id),uLENOVO_Error) )
            {
              OEM_BAT(M_BAT_uAuthenRPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[6],battery_id);
              OEM_BAT(M_BAT_uAuthenRPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[8],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[7],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[9],battery_id);

              SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return);
            }
            else if( IS_MASK_CLEAR(OEM_BAT(M_BAT_edgeProject4,battery_id),uLenovo_JapanError) )
            {
              OEM_BAT(M_BAT_uAuthenRPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[12],battery_id);
              OEM_BAT(M_BAT_uAuthenRPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[14],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[13],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[15],battery_id);
              SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return);
            }
            else if( IS_MASK_CLEAR(OEM_BAT(M_BAT_edgeProject4,battery_id),usLenovo_Error) )
            {
              OEM_BAT(M_BAT_uAuthenRPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[6],battery_id);
              OEM_BAT(M_BAT_uAuthenRPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[8],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackL,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[7],battery_id);
              OEM_BAT(M_BAT_uAuthenNPackH,battery_id) = OEM_BAT(M_BAT_LV_SW_SIGNERTURE[9],battery_id);
              SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_Return);
            }

            if( IS_MASK_SET(OEM_BAT(M_BAT_edgeProject4,battery_id),uLENOVO_Error) && IS_MASK_SET(OEM_BAT(M_BAT_edgeProject4,battery_id),uLenovo_JapanError) && IS_MASK_SET(OEM_BAT(M_BAT_edgeProject4,battery_id), usLenovo_Error))
            {
              SET_MASK(OEM_BAT(M_BAT_uAuthenFlag,battery_id),uDF_ErrorReturn);
            }
              Authentication(battery_id);
          }
        }
        else
        {
          OEM_BAT(M_BAT_uAuthenReadAfter1Sec,battery_id)++;
        }
      }
   }
}
#endif

void CriticalLowBATWakeS3(void)
{
  LLBWakeEn;
  EC_WAKE_OUT_LOW();
  Delay1MS(1);
  EC_WAKE_OUT_HI();
}

void IBMbatteryStore(void)
{
    switch(u4BattInfoID) 
    {
      case 0x00:
        u4BattInfo1 = RemainCapL;
        u4BattInfo2 = RemainCapH;
        u4BattInfo3 = FullChgCapL;
        u4BattInfo4 = FullChgCapH;
        //We do not fetch this data in previous project
        //Battery CMD 0x12
        u4BattInfo5 = AvgTimeToEmptyL;
        u4BattInfo6 = AvgTimeToEmptyH;
        //In K7DE017A project,
        //They're using battery cmd 0x0E to get ABSOLUTE STATE OF CHARGE
        //u4BattInfo7 = RelativeStateOfChgL;
        //u4BattInfo8 = RelativeStateOfChgH;
        //We're using cmd 0x0D to get RELATIVE STATE OF CHARGE
        u4BattInfo7 = RelativeStateOfChgL;
        u4BattInfo8 = RelativeStateOfChgH;
        u4BattInfo9 = AvgCurrentL;
        u4BattInfo10 = AvgCurrentH;
        u4BattInfo11 = VoltageL;
        u4BattInfo12 = VoltageH;
        //Battery CMD 0x13
        u4BattInfo13 = AvgTimeToFullL;
        u4BattInfo14 = AvgTimeToFullH;
        u4BattInfo15 = BattStatusL;//BattStatusL;
        u4BattInfo16 = BattStatusH;//BattStatusH;
        break;
      case 0x01:
        u4BattInfo1 = BattModeL;//BattModeL;
        u4BattInfo2 = BattModeH;//BattModeH;
        u4BattInfo3 = ManufactureAccessL ;//ManufactureAccessL;
        u4BattInfo4 = ManufactureAccessH;//ManufactureAccessH;
        u4BattInfo5 = CycleCountL;//CycleCountL;
        u4BattInfo6 = CycleCountH;//CycleCountH;
        u4BattInfo7 = CurrentL;//CurrentL;
        u4BattInfo8 = CurrentH;//CurrentH;
        u4BattInfo9 = TemperatureL ;//TemperatureL;
        u4BattInfo10 = TemperatureH;//TemperatureH;
        //Battery CMD 0x11
        u4BattInfo11 = RunTimeToEmptyL;
        u4BattInfo12 = RunTimeToEmptyH;
        //Battery CMD 0x3B, Not define in Previous project
        u4BattInfo13 = FETTemperatureL;
        u4BattInfo14 = FETTemperatureH;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x02:
        u4BattInfo1 = DesignCapL;//DesignCapL;
        u4BattInfo2 = DesignCapH;//DesignCapH;
        u4BattInfo3 = DesignVoltL;//DesignVoltL;
        u4BattInfo4 = DesignVoltH;//DesignVoltH;
        //Battery CMD 0x3F
        u4BattInfo5 = OptMfgFunction1L; //First Used Date;
        u4BattInfo6 = OptMfgFunction1H; //First Used Date;
        //Battery CMD 0x1A
        u4BattInfo7 = 0;//SpecInfoL;
        u4BattInfo8 = 0;//SpecInfoH;
        u4BattInfo9 = ManufactureDateL;//ManufactureDateL;
        u4BattInfo10 = ManufactureDateH;//ManufactureDateH;
        u4BattInfo11 = SerialNumberL;//SerialNumberL;
        u4BattInfo12 = SerialNumberH;//SerialNumberH;
        //Battery CMD 0x0C
        u4BattInfo13 = 0;//MaxErroL;
        u4BattInfo14 = 0;//MaxErroH;
        u4BattInfo15 = OptMfgFunction2L ;//OptMfgFunction2L;
        u4BattInfo16 = OptMfgFunction2H ;//OptMfgFunction2H;
        break;
      case 0x04:
        //We do not keep DeviceChemistry at Yx00 project
        u4BattInfo1 = DeviceChemistry[0];//DeviceChemistry1;
        u4BattInfo2 = DeviceChemistry[1];//DeviceChemistry2;
        u4BattInfo3 = DeviceChemistry[2];//DeviceChemistry3;
        u4BattInfo4 = DeviceChemistry[3];//DeviceChemistry4;
        u4BattInfo5 = DeviceChemistry[4];//DeviceChemistry5;
        u4BattInfo6 = DeviceChemistry[5];//DeviceChemistry6;
        u4BattInfo7 = DeviceChemistry[6];//DeviceChemistry7;
        u4BattInfo8 = DeviceChemistry[7];//DeviceChemistry8;
        u4BattInfo9 = DeviceChemistry[8];//DeviceChemistry9;
        u4BattInfo10 = DeviceChemistry[9];//DeviceChemistry10;
        u4BattInfo11 = DeviceChemistry[10];//DeviceChemistry11;
        u4BattInfo12 = DeviceChemistry[11];//DeviceChemistry12;
        u4BattInfo13 = DeviceChemistry[12];//DeviceChemistry13;
        u4BattInfo14 = DeviceChemistry[13];//DeviceChemistry14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x05:
        u4BattInfo1 = ManufacturerName[0];//ManufacturerName1;
        u4BattInfo2 = ManufacturerName[1];//ManufacturerName2;
        u4BattInfo3 = ManufacturerName[2];//ManufacturerName3;
        u4BattInfo4 = ManufacturerName[3];//ManufacturerName4;
        u4BattInfo5 = ManufacturerName[4];//ManufacturerName5;
        u4BattInfo6 = ManufacturerName[5];//ManufacturerName6;
        u4BattInfo7 = ManufacturerName[6];//ManufacturerName7;
        u4BattInfo8 = ManufacturerName[7];//ManufacturerName8;
        u4BattInfo9 = ManufacturerName[8];//ManufacturerName9;
        u4BattInfo10 = 0;//ManufacturerName10;
        u4BattInfo11 = 0;//ManufacturerName11;
        u4BattInfo12 = 0;//ManufacturerName12;
        u4BattInfo13 = 0;//ManufacturerName13;
        u4BattInfo14 = 0;//ManufacturerName14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x06:
        u4BattInfo1 = DeviceName[0];//DeviceName0;
        u4BattInfo2 = DeviceName[1];//DeviceName1;
        u4BattInfo3 = DeviceName[2];//DeviceName2;
        u4BattInfo4 = DeviceName[3];//DeviceName3;
        u4BattInfo5 = DeviceName[4];//DeviceName4;
        u4BattInfo6 = DeviceName[5];//DeviceName5;
        u4BattInfo7 = DeviceName[6];//DeviceName6;
        u4BattInfo8 = DeviceName[7];//DeviceName7;
        u4BattInfo9 = 0;
        u4BattInfo10 = 0;
        u4BattInfo11 = 0;
        u4BattInfo12 = 0;
        u4BattInfo13 = 0;
        u4BattInfo14 = 0;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x07:
        //Battery CMD 0x2F, Not Define in  previous project
        u4BattInfo1 = OptMfgFunction5[0];//OptMfgFunction5_1;
        u4BattInfo2 = OptMfgFunction5[1];//OptMfgFunction5_2;
        u4BattInfo3 = OptMfgFunction5[2];//OptMfgFunction5_3;
        u4BattInfo4 = OptMfgFunction5[3];//OptMfgFunction5_4;
        u4BattInfo5 = OptMfgFunction5[4];//OptMfgFunction5_5;
        u4BattInfo6 = OptMfgFunction5[5];//OptMfgFunction5_6;
        u4BattInfo7 = OptMfgFunction5[6];//OptMfgFunction5_7;
        u4BattInfo8 = OptMfgFunction5[7];//OptMfgFunction5_8;
        u4BattInfo9 = OptMfgFunction5[8];//OptMfgFunction5_9;
        u4BattInfo10 = OptMfgFunction5[9];//OptMfgFunction5_10;
        u4BattInfo11 = OptMfgFunction5[10];//OptMfgFunction5_11;
        u4BattInfo12 = OptMfgFunction5[11];//OptMfgFunction5_12;
        u4BattInfo13 = OptMfgFunction5[12];//OptMfgFunction5_13;
        u4BattInfo14 = OptMfgFunction5[13];//OptMfgFunction5_14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x08:
        //Battery CMD 0x35
        u4BattInfo1 = FirmwareUpdateStatusL;
        u4BattInfo2 = FirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = BatterymakerIDL;
        u4BattInfo4 = BatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = HardwareIDL;
        u4BattInfo6 = HardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = FirmwareversionL;
        u4BattInfo8 = FirmwareversionH;
        //argument of Battery CMD 0x37
        u4BattInfo9 = DataVersionL;
        u4BattInfo10 = DataVersionH;
        //Not define in previous project
        u4BattInfo11 = WarrantyPeriodL;
        u4BattInfo12 = WarrantyPeriodH;
        u4BattInfo13 = ChargeCurrL;
        u4BattInfo14 = ChargeCurrH;
        u4BattInfo15 = ChargeVoltL;
        u4BattInfo16 = ChargeVoltH;
        break;
      case 0x0C:
        //Battery CMD 0x35
        u4BattInfo1 = FirmwareUpdateStatusL;
        u4BattInfo2 = FirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = BatterymakerIDL;
        u4BattInfo4 = BatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = HardwareIDL;
        u4BattInfo6 = HardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = FirmwareversionL;
        u4BattInfo8 = FirmwareversionH;
        // argument of Battery CMD 0x37
        u4BattInfo9 = DataVersionL;
        u4BattInfo10 = DataVersionH;
        // Not define in previous project
        u4BattInfo11 = WarrantyPeriodL;
        u4BattInfo12 = WarrantyPeriodH;
        u4BattInfo13 = ChargeCurrL;
        u4BattInfo14 = ChargeCurrH;
        u4BattInfo15 = ChargeVoltL;
        u4BattInfo16 = ChargeVoltH;
        break;
      case 0x10:
        u4BattInfo1 = SecRemainCapL;
        u4BattInfo2 = SecRemainCapH;
        u4BattInfo3 = SecFullChgCapL;
        u4BattInfo4 = SecFullChgCapH;
        //We do not fetch this data in previous project
        //Battery CMD 0x12
        u4BattInfo5 = SecAvgTimeToEmptyL;
        u4BattInfo6 = SecAvgTimeToEmptyH;
        //In K7DE017A project,
        //They're using battery cmd 0x0E to get ABSOLUTE STATE OF CHARGE
        //u4BattInfo7 = RelativeStateOfChgL;
        //u4BattInfo8 = RelativeStateOfChgH;
        //We're using cmd 0x0D to get RELATIVE STATE OF CHARGE
        u4BattInfo7 = SecRelativeStateOfChgL;
        u4BattInfo8 = SecRelativeStateOfChgH;
        u4BattInfo9 = SecAvgCurrentL;
        u4BattInfo10 = SecAvgCurrentH;
        u4BattInfo11 = SecVoltageL;
        u4BattInfo12 = SecVoltageH;
        //Battery CMD 0x13
        u4BattInfo13 = SecAvgTimeToFullL;
        u4BattInfo14 = SecAvgTimeToFullH;
        u4BattInfo15 = SecBattStatusL;//BattStatusL;
        u4BattInfo16 = SecBattStatusH;//BattStatusH;
        break;
      case 0x11:
        u4BattInfo1 = SecBattModeL;//BattModeL;
        u4BattInfo2 = SecBattModeH;//BattModeH;
        u4BattInfo3 = SecManufactureAccessL ;//ManufactureAccessL;
        u4BattInfo4 = SecManufactureAccessH;//ManufactureAccessH;
        u4BattInfo5 = SecCycleCountL;//CycleCountL;
        u4BattInfo6 = SecCycleCountH;//CycleCountH;
        u4BattInfo7 = SecCurrentL;//CurrentL;
        u4BattInfo8 = SecCurrentH;//CurrentH;
        u4BattInfo9 = SecTemperatureL ;//TemperatureL;
        u4BattInfo10 = SecTemperatureH;//TemperatureH;
        //Battery CMD 0x11
        u4BattInfo11 = SecRunTimeToEmptyL;
        u4BattInfo12 = SecRunTimeToEmptyH;
        //Battery CMD 0x3B, Not define in Previous project
        u4BattInfo13 = SecFETTemperatureL;
        u4BattInfo14 = SecFETTemperatureH;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x12:
        u4BattInfo1 = SecDesignCapL;//DesignCapL;
        u4BattInfo2 = SecDesignCapH;//DesignCapH;
        u4BattInfo3 = SecDesignVoltL;//DesignVoltL;
        u4BattInfo4 = SecDesignVoltH;//DesignVoltH;
        //Battery CMD 0x3F
        u4BattInfo5 = SecOptMfgFunction1L; //First Used Date;
        u4BattInfo6 = SecOptMfgFunction1H; //First Used Date;
        //Battery CMD 0x1A
        u4BattInfo7 = 0;//SpecInfoL;
        u4BattInfo8 = 0;//SpecInfoH;
        u4BattInfo9 = SecManufactureDateL;//ManufactureDateL;
        u4BattInfo10 = SecManufactureDateH;//ManufactureDateH;
        u4BattInfo11 = SecSerialNumberL;//SerialNumberL;
        u4BattInfo12 = SecSerialNumberH;//SerialNumberH;
        //Battery CMD 0x0C
        u4BattInfo13 = 0;//MaxErroL;
        u4BattInfo14 = 0;//MaxErroH;
        u4BattInfo15 = SecOptMfgFunction2L ;//OptMfgFunction2L;
        u4BattInfo16 = SecOptMfgFunction2H ;//OptMfgFunction2H;
        break;
      case 0x14:
        //We do not keep DeviceChemistry at Yx00 project
        u4BattInfo1 = SecDeviceChemistry[0];//DeviceChemistry1;
        u4BattInfo2 = SecDeviceChemistry[1];//DeviceChemistry2;
        u4BattInfo3 = SecDeviceChemistry[2];//DeviceChemistry3;
        u4BattInfo4 = SecDeviceChemistry[3];//DeviceChemistry4;
        u4BattInfo5 = SecDeviceChemistry[4];//DeviceChemistry5;
        u4BattInfo6 = SecDeviceChemistry[5];//DeviceChemistry6;
        u4BattInfo7 = SecDeviceChemistry[6];//DeviceChemistry7;
        u4BattInfo8 = SecDeviceChemistry[7];//DeviceChemistry8;
        u4BattInfo9 = SecDeviceChemistry[8];//DeviceChemistry9;
        u4BattInfo10 = SecDeviceChemistry[9];//DeviceChemistry10;
        u4BattInfo11 = SecDeviceChemistry[10];//DeviceChemistry11;
        u4BattInfo12 = SecDeviceChemistry[11];//DeviceChemistry12;
        u4BattInfo13 = SecDeviceChemistry[12];//DeviceChemistry13;
        u4BattInfo14 = SecDeviceChemistry[13];//DeviceChemistry14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x15:
        u4BattInfo1 = SecManufacturerName[0];//ManufacturerName1;
        u4BattInfo2 = SecManufacturerName[1];//ManufacturerName2;
        u4BattInfo3 = SecManufacturerName[2];//ManufacturerName3;
        u4BattInfo4 = SecManufacturerName[3];//ManufacturerName4;
        u4BattInfo5 = SecManufacturerName[4];//ManufacturerName5;
        u4BattInfo6 = SecManufacturerName[5];//ManufacturerName6;
        u4BattInfo7 = SecManufacturerName[6];//ManufacturerName7;
        u4BattInfo8 = SecManufacturerName[7];//ManufacturerName8;
        u4BattInfo9 = SecManufacturerName[8];//ManufacturerName9;
        u4BattInfo10 = 0;//ManufacturerName10;
        u4BattInfo11 = 0;//ManufacturerName11;
        u4BattInfo12 = 0;//ManufacturerName12;
        u4BattInfo13 = 0;//ManufacturerName13;
        u4BattInfo14 = 0;//ManufacturerName14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x16:
        u4BattInfo1 = SecDeviceName[0];//DeviceName0;
        u4BattInfo2 = SecDeviceName[1];//DeviceName1;
        u4BattInfo3 = SecDeviceName[2];//DeviceName2;
        u4BattInfo4 = SecDeviceName[3];//DeviceName3;
        u4BattInfo5 = SecDeviceName[4];//DeviceName4;
        u4BattInfo6 = SecDeviceName[5];//DeviceName5;
        u4BattInfo7 = SecDeviceName[6];//DeviceName6;
        u4BattInfo8 = SecDeviceName[7];//DeviceName7;
        u4BattInfo9 = 0;
        u4BattInfo10 = 0;
        u4BattInfo11 = 0;
        u4BattInfo12 = 0;
        u4BattInfo13 = 0;
        u4BattInfo14 = 0;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x17:
        //Battery CMD 0x2F, Not Define in  previous project
        u4BattInfo1 = SecOptMfgFunction5[0];//OptMfgFunction5_1;
        u4BattInfo2 = SecOptMfgFunction5[1];//OptMfgFunction5_2;
        u4BattInfo3 = SecOptMfgFunction5[2];//OptMfgFunction5_3;
        u4BattInfo4 = SecOptMfgFunction5[3];//OptMfgFunction5_4;
        u4BattInfo5 = SecOptMfgFunction5[4];//OptMfgFunction5_5;
        u4BattInfo6 = SecOptMfgFunction5[5];//OptMfgFunction5_6;
        u4BattInfo7 = SecOptMfgFunction5[6];//OptMfgFunction5_7;
        u4BattInfo8 = SecOptMfgFunction5[7];//OptMfgFunction5_8;
        u4BattInfo9 = SecOptMfgFunction5[8];//OptMfgFunction5_9;
        u4BattInfo10 = SecOptMfgFunction5[9];//OptMfgFunction5_10;
        u4BattInfo11 = SecOptMfgFunction5[10];//OptMfgFunction5_11;
        u4BattInfo12 = SecOptMfgFunction5[11];//OptMfgFunction5_12;
        u4BattInfo13 = SecOptMfgFunction5[12];//OptMfgFunction5_13;
        u4BattInfo14 = SecOptMfgFunction5[13];//OptMfgFunction5_14;
        u4BattInfo15 = 0;
        u4BattInfo16 = 0;
        break;
      case 0x18:
        //Battery CMD 0x35
        u4BattInfo1 = SecFirmwareUpdateStatusL;
        u4BattInfo2 = SecFirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = SecBatterymakerIDL;
        u4BattInfo4 = SecBatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = SecHardwareIDL;
        u4BattInfo6 = SecHardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = SecFirmwareversionL;
        u4BattInfo8 = SecFirmwareversionH;
        //argument of Battery CMD 0x37
        u4BattInfo9 = SecDataVersionL;
        u4BattInfo10 = SecDataVersionH;
        //Not define in previous project
        u4BattInfo11 = SecWarrantyPeriodL;
        u4BattInfo12 = SecWarrantyPeriodH;
        u4BattInfo13 = SecChargeCurrL;
        u4BattInfo14 = SecChargeCurrH;
        u4BattInfo15 = SecChargeVoltL;
        u4BattInfo16 = SecChargeVoltH;
        break;
      case 0x1C:
        //Battery CMD 0x35
        u4BattInfo1 = SecFirmwareUpdateStatusL;
        u4BattInfo2 = SecFirmwareUpdateStatusH;
        //Battery CMD 0x37, But we did not fetch data by this command
        u4BattInfo3 = SecBatterymakerIDL;
        u4BattInfo4 = SecBatterymakerIDH;
        //argument of Battery CMD 0x37
        u4BattInfo5 = SecHardwareIDL;
        u4BattInfo6 = SecHardwareIDH;
        //argument of Battery CMD 0x37
        u4BattInfo7 = SecFirmwareversionL;
        u4BattInfo8 = SecFirmwareversionH;
        // argument of Battery CMD 0x37
        u4BattInfo9 = SecDataVersionL;
        u4BattInfo10 = SecDataVersionH;
        // Not define in previous project
        u4BattInfo11 = SecWarrantyPeriodL;
        u4BattInfo12 = SecWarrantyPeriodH;
        u4BattInfo13 = SecChargeCurrL;
        u4BattInfo14 = SecChargeCurrH;
        u4BattInfo15 = SecChargeVoltL;
        u4BattInfo16 = SecChargeVoltH;
        break;
       
      case 0x60:  
        //Temperature (channel 0 to 15, A0 has channel 0 temperature and so on)
        u4BattInfo1 = u4CpuThermal;     //CPU
        u4BattInfo2 = 0x80  ;     //Local
        u4BattInfo3 = nCpuTempLocal;     //Fan
#if PLATFORM_DGPU
        u4BattInfo4 = VGA_TEMP;     //DGPU
#else
        u4BattInfo4 = 0x80;         //DGPU
#endif
      if(!BattExistGet || ((FETTemperatureL == 0) && (FETTemperatureH ==0))) 
        u4BattInfo5 = 0x80;     //Battery FET #0
      else
        u4BattInfo5 = (((FETTemperatureH << 8)+ FETTemperatureL) - 2730)/10;     //Battery FET #0
        
      if(!SecBattExistGet || ((SecFETTemperatureL == 0) && (SecFETTemperatureH ==0)))     
        u4BattInfo6 = 0x80;                   //Battery FET #1
      else  
        u4BattInfo6 = (((SecFETTemperatureH << 8)+ SecFETTemperatureL) - 2730)/10;     //Battery FET #1

      if(!BattExistGet || ((TemperatureL == 0) && (TemperatureH ==0))) 
        u4BattInfo7 = 0x80;             //Battery Cell #0
      else       
        u4BattInfo7 = (((TemperatureH << 8)+ TemperatureL) - 2730)/10;     //Battery Cell #0

      if(!SecBattExistGet || ((SecTemperatureL == 0) && (SecTemperatureH ==0)))     
        u4BattInfo8 = 0x80;                   //Battery FET #1
      else          
        u4BattInfo8 = (((SecTemperatureH << 8)+ SecTemperatureL) - 2730)/10;     //Battery Cell #1
        
        u4BattInfo9 = 0x80;           //CPU DC/DC
        u4BattInfo10 = nVramTemp;     //nVramTempnVramTemp
        u4BattInfo11 = nRamTemp;      //DIMM Bottom
#if (PLATFORM_CHIP == INTEL)    
        u4BattInfo12 = PCH_Temp3;     //PCH
#else
        u4BattInfo12 = 0x80;     //PCH
#endif
        u4BattInfo13 = 0x80;     //
        u4BattInfo14 = 0x80;     //
        u4BattInfo15 = 0x80;     //-
        u4BattInfo16 = 0x80;     //-      
        break;
    
    }
}

void TWR_BATClear(int battery_id)
{
  unsigned char* bat_pntr;
  BYTE i,j;

  for( i=0; i < 0x0C; i++)
  {
    bat_pntr = ((&TWR_BAT(TWR10,battery_id)+2)+i*16); // Base on address 0x0AA0.
    for( j=2; j<=16; j++ ) // Clear 0AA0F~0AAF.
    {
      *bat_pntr=0;
      bat_pntr++;
    }
  }
}

void TPG_EXTBATCmd(void)
{
   switch(u4ExtCommand)
  {
    case 0x01:
      // Reset battery charge inhibit
        CLEAR_MASK(M_BATStopChgStatL,ENCMDSTOPCHG);
        Inhibit_chargingCLR;
        PriBattinhibitDis;//PriBattinhibitEn;
        SecBattinhibitDis;//PriBattinhibitEn;        
        pInhitTimer = 0; // clear Inhibit timer.
        u4InhChgTimerL = 0;
        u4InhChgTimerH = 0;
        break;
    case 0x02:
      // Peakshift inhibit charge.
        if (pInhitTimer != 0x0000) {
          SET_MASK(M_BATStopChgStatL,ENCMDSTOPCHG);
          Inhibit_chargingSET;  
          PriBattinhibitEn;
          SecBattinhibitEn;
        }
		u4Cfgspace4 &= ~u4Cfgspace4BATseq;//A485D00047: Fix Charge test fail
        break;
    case 0x03:
        if( Read_BATLEARN() )
        {
          BAT_LEARN_OFF();
        }
        if( nAcOffCmdGET )
        {
          BATLEARNProcess(nAcOffCmdDIS);
        }
        PriBattDisChgDis;
        SecBattDisChgDis;        
        CLEAR_MASK(pBattLearn,uACoffstchg);
        CLEAR_MASK(pBattLearn,uRefreshmode);
        CLEAR_MASK(M_BATStopChgStatL,ENCUSTOMER);		
		u4Cfgspace4 &= ~u4Cfgspace4BATseq; //A485D00047: Fix Charge test fail
        break;
    case 0x04:
        if( !Read_AC_IN() || (SysPowState == SYSTEM_S3) ||  NoBattExistGet)
        {
          if( Read_BATLEARN() )
          {
            BAT_LEARN_OFF();
          }
          if( nAcOffCmdGET )
          {
            BATLEARNProcess(nAcOffCmdDIS);
          }
          PeakShiftDischargSTSCLR;
        }
        else
        {
          if( (!Read_BATLEARN()) )
          {
            BAT_LEARN_ON();
          }
          if( (!nAcOffCmdGET) )
          {
            BATLEARNProcess(nAcOffCmdEN);
          }
          PeakShiftDischargSTSSET;
        }//A485D00047: Fix Charge test fail--->Start
		if (SecBattExistGet&&!PriBattDisChgGet)		// if no assign which battery discharge then force 2nd
		{
			SecBattDisChgEn;
		}//A485D00047: Fix Charge test fail--->End
        break;
    case 0x05:
      // Inhibit to charge a primary battery
        Inhibit_chargingSET;
        PriBattinhibitEn;
        SecBattinhibitEn;        
        break;
    case 0x07:
        PriBattDisChgEn;
        CLEAR_MASK(pBattLearn,us4ResumeACOFF);
        SET_MASK(pBattLearn,uACoffstchg);
        SET_MASK(pBattLearn,uRefreshmode);
        if( !Read_BATLEARN() )
        {
          BAT_LEARN_ON();
        }
        if( (!nAcOffCmdGET) )
        {
          BATLEARNProcess(nAcOffCmdEN);
        }
        SET_MASK(M_BATStopChgStatL,ENCUSTOMER);
        break;
    case 0x08:
        SecBattDisChgEn;        
        CLEAR_MASK(pBattLearn,us4ResumeACOFF);
        SET_MASK(pBattLearn,uACoffstchg);
        SET_MASK(pBattLearn,uRefreshmode);
        if( !Read_BATLEARN() )
        {
          BAT_LEARN_ON();
        }
        if( (!nAcOffCmdGET) )
        {
          BATLEARNProcess(nAcOffCmdEN);
        }
        SET_MASK(M_BATStopChgStatL,ENCUSTOMER);
        break;
        
    case 0x10:    // Start gravity sensor(G-sensor) diagnotic program
       GSensor_DIAG();
        break;
  }
  if( u4ExtCommand )
  {
    u4ExtCommand = 0;
  }
}


void EDGEPMBATTProcess(int battery_id)
{
  if( ( u4Cfgspace3 & BIT2 ) == BIT2 )  // 0x03 bit2=1,Charge stop capacity
  {
    if( OEM_BAT_1B(uThresholdSetting, battery_id) == 0x80 )
    {
      OEM_BAT(M_BATStopCharge, battery_id)  = 100;
    }
    else if (OEM_BAT_1B(uThresholdSetting, battery_id) & SET_CHARGE_CAPACITY_FLAG)
    {
      OEM_BAT(M_BATStopCharge, battery_id) = ( OEM_BAT_1B(uThresholdSetting, battery_id) & 0x7F );
    }
    if( OEM_BAT(M_BATStopCharge, battery_id) != (Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStopCharge + (battery_id << 1)))){
      WRITE_DATA2EEPROM((&OEM_BAT(M_BATStopCharge, battery_id)), 1 , EEPROM_Bank7, EEPROM_MBATStopCharge+ (battery_id << 1));
    }
  }
  else  // 0x03 bit2=0,Charge start capacity
  {
    if( OEM_BAT_1B(uThresholdSetting, battery_id) == 0x80 )
    {
      OEM_BAT(M_BATStartCharge, battery_id)  = 95;
    }
    else if (OEM_BAT_1B(uThresholdSetting, battery_id) & SET_CHARGE_CAPACITY_FLAG)
    {
      OEM_BAT(M_BATStartCharge, battery_id) = ( OEM_BAT_1B(uThresholdSetting, battery_id) & 0x7F );
    }
    if( OEM_BAT(M_BATStartCharge, battery_id) != (Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_MBATStartCharge + (battery_id << 1)))){
      WRITE_DATA2EEPROM(&OEM_BAT(M_BATStartCharge, battery_id) , 1 , EEPROM_Bank7, EEPROM_MBATStartCharge + (battery_id << 1));      
    }
  }

  OEM_BAT_1B(uThresholdSetting, battery_id) &= (~SET_CHARGE_CAPACITY_FLAG); // clear BIT7
// 0xA24 and 0xA25
// Battery Charge start/stop threshold setting of the primary(0x24)/seconday(0x25) battery.
// When bit 2 of configuration space 3 is 0, bit 0 - 6 represent the capacity to start to charge.
// Otherwise bit 0-6 represent the capacity to stop charging. 
// Bit 7 is needed to set to 1 when a value is written to this byte. 
// When the command is done, bit 7 is cleared.

  //if( RelativeStateOfChgL <= pEdgeBATStartCharge )
  //{
  //  PriBattinhibitDis;//PriBattinhibitEn;
  //  CLEAR_MASK(M_BATStopChgStatH, PMTHRESHOLD);
  //}
  //else if( RelativeStateOfChgL >= pEdgeBATStopCharge )
  //{
  //  PriBattinhibitEn;//PriBattinhibitDis;
  //  SET_MASK(M_BATStopChgStatH, PMTHRESHOLD);
  //}
}

void TWR_Initial(void)
{
  BYTE i;
  BYTE * ptr;
// 1st battery
  if(BattExistGet){
    if( BatteryCriticalGET ){
      CriticalLowEn;
    }else{
      CriticalLowDis;
    }
    ErrorDis;   //TWR01.bit.Error = 0;
    TWRvalidEn; //TWR01.bit.TWRvalid = 1;
  }else{
//    TWR00 &= 0x80;
    TWR01 = 0x00;
  }

// 2nd battery 
#if Support_TWO_BATT
  if(SecBattExistGet){
    if( SecBatteryCriticalGET ){
      SecCriticalLowEn;
    }else{
      SecCriticalLowDis;
    }
    SecErrorDis;   //TWR01.bit.Error = 0;
    SecTWRvalidEn; //TWR01.bit.TWRvalid = 1;
  }else{
 //   SecTWR00 &= 0x80;
    SecTWR01 = 0x00;
  }

  SecTWR00 =  ((TWR00 & ~AuthenticationFailed ) | (SecTWR00 & AuthenticationFailed));
#endif //Support_TWO_BATT


//   SecTWR00 = (TWR00 | (SecTWR00 & AuthenticationFailed));
  for (i=1;i<0x0D;i++)
  {
    ptr = &TWR00;
    *(ptr + (i*0x10)) = TWR00;
    *(ptr + (i*0x10) + 1) = TWR01;
#if Support_TWO_BATT
    ptr = &SecTWR00;
    *(ptr + (i*0x10)) = SecTWR00;
    *(ptr + (i*0x10) + 1) = SecTWR01;
#endif //support_two_BATT
  }

}

void Battery_Initial(void)
{    
  u4HighBattLV = BATMAXLVL;  // update battery hightest level
  capSmartBattEn;

  MajorVer = 0x01;
  MinorVer = 0x03;
  TWRInfo = 0xE0;
#if Support_TWO_BATT
  SecMajorVer = 0x01;
  SecMinorVer = 0x03;
  SecTWRInfo = 0xE0;
  #if Only_One_BATT
  BAT_Status = BAT_SEL_M + (BAT_SEL_M << 4);//A285D00009: Change to support single battery
  #else
  BAT_Status = BAT_SEL_INIT + (BAT_SEL_INIT << 4);
  #endif // End of Only_One_BATT
  BAT_switch_ready_SET;
#endif //Support_TWO_BATT  
}

void CHK_Capacity_trip_point (void)
{
#if SupportCHKtrippoint
////A485D00027-Start
   //if (BattExistGet && (u4TripPointCapacity0L == RemainCapL) && (u4TripPointCapacity0H == RemainCapH))
   // { ECSMI_SCIEvent(BAT0_Cap_event);}
   if( IS_MASK_SET(SYS_MISC1,ACPI_OS) && BattExistGet )//&& IS_MASK_CLEAR(BAT_CAPTripFlag,MBAT_triggered) )
   {
   		if((CurrentH&0x80)&&(((WORD)(u4TripPointCapacity0H<<4)+u4TripPointCapacity0L) >= 
   						((WORD)(RemainCapH<<4)+RemainCapL)))
   		{
   			ECSMI_SCIEvent(BAT0_Cap_event);
			//SET_MASK(BAT_CAPTripFlag,MBAT_triggered);
   		}
   }
//A485D00027->end
#if Support_TWO_BATT
//A485D00027-> start
   //if (SecBattExistGet && (u4TripPointCapacity1L == SecRemainCapL) && (u4TripPointCapacity1H == SecRemainCapH))
   // { ECSMI_SCIEvent(BAT1_Cap_event);}

	if( IS_MASK_SET(SYS_MISC1,ACPI_OS) && SecBattExistGet )//&& IS_MASK_CLEAR(BAT_CAPTripFlag,SBAT_triggered) )
   {
   		if((SecCurrentH&0x80)&&(((WORD)(u4TripPointCapacity1H<<4)+u4TripPointCapacity1L) >= 
   						((WORD)(SecRemainCapH<<4)+SecRemainCapL)))
   		{
			ECSMI_SCIEvent(BAT1_Cap_event);
			//SET_MASK(BAT_CAPTripFlag,SBAT_triggered);
   		}
   }
   ////A485D00027->end
#endif

#endif //SupportCHKtrippoint
}

void Chk_Trickle_Setting (void)
{
  if (!(IS_M_Trickle || IS_S_Trickle) || BAT_switch_not_ready || !(M_BATWake || S_BATWake)){
   return;
 }
 if (!SecBattExistGet && BattExistGet)   //main battery only
 {
   S_CTRL_LOW();
   if (M_BATWake & !M_CTRL_GET()){
     BAT_Trickle_Charge_step = (M_BAT << 4) | 1;    
     Switch2Trickle_Charge(M_BAT);
   }
   if (Current_BAT_State != BAT_SEL_M){
     BAT_state_target(BAT_SEL_M);
   }
 }
 if (!BattExistGet && SecBattExistGet)    // select second battery
 {
   M_CTRL_LOW();
   if (S_BATWake & !S_CTRL_GET()){
     BAT_Trickle_Charge_step = (S_BAT << 4) | 1;      
     Switch2Trickle_Charge(S_BAT);
   }   
   if (Current_BAT_State != BAT_SEL_S){
     BAT_state_target(BAT_SEL_S);
   }	 	     
 }
 if (AllBattExistGet)    //two BAT
 {
   if ((Current_BAT_State == BAT_SEL_INIT) && IS_MASK_SET(SYS_STATUS,AC_ADP)){
     if (M_BATWake){
       BAT_Trickle_Charge_step = (M_BAT << 4) | 1;    
       Switch2Trickle_Charge(M_BAT);
       return;
     }	 	     
     if (S_BATWake){
       BAT_Trickle_Charge_step = (S_BAT << 4) | 1;    
       Switch2Trickle_Charge(S_BAT);
       return;
     }	 
   }

   if (M_CTRL_GET()){
     BAT_state_target(BAT_SEL_S);
   }

   if (S_CTRL_GET()){
     BAT_state_target(BAT_SEL_M);
   } 
 }
}

/********************************************************************************
* Function name: Switch2Trickle_Charge
*
* Descriptoin: 
*
*   ----------------------------------------------------------------------------------------------------------------
*   |                       | M1_DRV | #M2_DRV | S1_DRV | #S2_DRV | M_Ctrl | S_Ctrl |            note              |
*   |---------------------------------------------------------------------------------------------------------------
*   |       | M_BAT only    |  High  |   Low   |  Low   |  High   |  High  |  Low   | state1 & M_ctrl on           |
*   | 1 bat |-------------------------------------------------------------------------------------------------------
*   |       | S_BAT only    |  Low   |   High  |  High  |  Low    |  Low   |  High  | state5 & S_ctrl on           |
*   |-------|-------------------------------------------------------------------------------------------------------
*   |       | M_BAT trickle |  Low   |   High  |  High  |  Low    |  High  |  Low   | state5 & M_ctrl on           |
*   | 2 bat |-------------------------------------------------------------------------------------------------------
*   |       | S_BAT trickle |  High  |   Low   |  Low   |  High   |  Low   |  High  | state1 & S_ctrl on           |
*   ----------------------------------------------------------------------------------------------------------------
*   
* 
* Arguments: 
*           1.INHIBIT 1
*           2.setting dual mos path
*           3.system min voltage 12.544v
*           4.M_CTRL/S_CTRL
*
* Return Values: None
*
*       
*******************************************************************************/
void Switch2Trickle_Charge (BYTE battery_id)
{
#if !Support_TWO_BATT   
  return;
#endif //Support_TWO_BATT
  if (BAT_Trickle_Charge_step == 0 || BAT_switch_not_ready)
    {return;} 
  switch (BAT_Trickle_Charge_step & 0xF){
//step1 inhibit 1
    case 1: 
      StopChgIC();
      BAT_Trickle_Charge_step &= 0xF0;
      BAT_Trickle_Charge_step |= 0x2;
//step2 setting dual mos path
    case 2: 
	    if (!SecBattExistGet && BattExistGet)   //main battery only
	    {
		    BAT_state_target(BAT_SEL_M);
	    }
	    if (!BattExistGet && SecBattExistGet)
	    {
		    BAT_state_target(BAT_SEL_S);	 	      // select second battery
	    }
	    if (AllBattExistGet)
	    {
//        if (IS_MASK_SET(M_BATFlag1,pEdgeProjectBATWake) && IS_MASK_CLEAR(S_BATFlag1,pEdgeProjectBATWake)){
        if ((BAT_Trickle_Charge_step >> 4) == M_BAT){
          BAT_state_target(BAT_SEL_S);
        }else{ 
          BAT_state_target(BAT_SEL_M);
        }    	             
      }
      BAT_Trickle_Charge_step &= 0xF0;
      BAT_Trickle_Charge_step |= 0x3;
      break;
//step3  3.system min voltage 12.544v (0x3100)
    case 3:
      Trickle_ChgIC(Enable);
      BAT_Trickle_Charge_step &= 0xF0;
      BAT_Trickle_Charge_step |= 0x4;
//step4  4.M_CTRL/S_CTRL
	  case 4:
//      if (IS_MASK_SET(M_BATFlag1,pEdgeProjectBATWake)){
      if ((BAT_Trickle_Charge_step >> 4) == M_BAT){
        S_CTRL_LOW();
        M_CTRL_HI();
      }
//      if (IS_MASK_SET(S_BATFlag1,pEdgeProject2ndBATWake)){
      if ((BAT_Trickle_Charge_step >> 4) == S_BAT){
        M_CTRL_LOW();        
        S_CTRL_HI();
      }        
      BAT_Trickle_Charge_step = 0;
      break;
   }
}


void CHK_Trickle_Charge(BYTE event_id)     //event_id, 0:M_BAT 1:S_BAT 0xAC:AC_IN  0xc91:Polling 
{
  BYTE i;
  WORD Temp16;
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP))
    return;

//DC_IN
  if ((event_id == M_BAT) || (event_id == S_BAT)){
    Temp16 = (TWR_BAT(ChargeCurrH,event_id) << 8) + (TWR_BAT(ChargeCurrL,event_id));
// smbus fail || chargecurr < 300
    if((OEM_BAT(M_BATSMbusFailCount,event_id) != 0) || ((Temp16 != 0) && (Temp16 < Tricklechargecurrent) && (IS_MASK_CLEAR(TWR_BAT(BattStatusL,event_id),FullyChg))))
    {
#if Trickle_Control_By_CHGIC      //setting charge IC
      TWR_BAT(ChargeCurrL,event_id) = Trickle_ChargeCurr  & 0xFF;         // 256mA
      TWR_BAT(ChargeCurrH,event_id) = Trickle_ChargeCurr >> 8;
      TWR_BAT(ChargeVoltL,event_id) = Trickle_ChargeVolt   & 0xFF;        // 12V (wake up voltage)  2012.04.13 update by AC.
      TWR_BAT(ChargeVoltH,event_id) = Trickle_ChargeVolt >> 8;
#endif   //HW control
      OEM_BAT(M_BAT_Trickle_switch_time,(1^event_id)) = 30;
      OEM_BAT(M_BAT_Trickle_switch_Voltage,(1^event_id)) = 80;
      SET_MASK(OEM_BAT(M_BATFlag1,event_id),pEdgeProjectBATWake);  
      SET_MASK(OEM_BAT(M_BAT_Status1,event_id),M_Trickle_T);
      CLEAR_MASK(OEM_BAT(M_BAT_Status1,event_id), M_Fast_T);
//check trickle yes or no  
      if (BAT_Trickle_Charge_flow(event_id)){
        BAT_Trickle_Charge_step = (event_id << 4) | 1;
        Switch2Trickle_Charge(event_id);
      }  
    } else {
      CLEAR_MASK(OEM_BAT(M_BAT_Status1,event_id),M_Trickle_T );
      SET_MASK(OEM_BAT(M_BAT_Status1,event_id),M_Fast_T ); 
    }
  }

  if (IS_M_Trickle || IS_S_Trickle)
    return;
  
//AC_IN  
  if ((event_id == Polling) && (BAT_Trickle_ACIN_timer < 2000)){   //(event_id == AC_IN) || (
    BAT_Trickle_ACIN_timer++;  // 5m X 2000 => 10S
    for (i = 0; i <=1; i++){
      if ((IS_MASK_SET(OEM_BAT_1B(PriBattSTS,i),BattExist)) && (IS_MASK_CLEAR(OEM_BAT(M_BATStopChgStatH,i),ENTRITIMEOUT))){
        Temp16 = (TWR_BAT(ChargeCurrH,i) << 8) + TWR_BAT(ChargeCurrL,i);
        if((OEM_BAT(M_BATSMbusFailCount,i) >= 8) || ((Temp16 != 0) && (Temp16 < Tricklechargecurrent) && (TWR_BAT(RelativeStateOfChgL,i) < 5))){
          SET_MASK(OEM_BAT(M_BATFlag1,i),pEdgeProjectBATWake);
          OEM_BAT(M_BAT_Trickle_switch_time,(1^i)) = 30;
          OEM_BAT(M_BAT_Trickle_switch_Voltage,(1^i)) = 80;
          SET_MASK(OEM_BAT(M_BAT_Status1,i),M_Trickle_T);
          CLEAR_MASK(OEM_BAT(M_BAT_Status1,i), M_Fast_T);          
//check trickle yes or no  
          if (BAT_Trickle_Charge_flow(event_id)){
            BAT_Trickle_Charge_step =  (i << 4) | 1;
            Switch2Trickle_Charge(i);
          }            
        } 
        else {
          CLEAR_MASK(OEM_BAT(M_BAT_Status1,i),M_Trickle_T);
          SET_MASK(OEM_BAT(M_BAT_Status1,i),M_Fast_T );          
        }              
      }        
    }  
  }
}


void Trickle2Fast_Charge(int battery_id, BOOL Chg_sts)
{
  if (battery_id == M_BAT){
    M_CTRL_LOW();
  }
  if (battery_id == S_BAT){
    S_CTRL_LOW();
  }
  Trickle_ChgIC(Disable);
  if (Chg_sts){
    StartChgIC();
  }
}


void Trickle_Charge_Mode(int battery_id)
{
  WORD Temp16;
//  if (OEM_BAT(M_BATSMbusFailCount,battery_id) = 0){
//    return;
//  }
//trickle charge
  if (IS_MASK_SET(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake)){
    Temp16 = (TWR_BAT(ChargeCurrH,battery_id) << 8) + TWR_BAT(ChargeCurrL,battery_id);
    if (Temp16 >= Tricklechargecurrent){ // || (IS_MASK_SET(OEM_BAT(M_BATStopChgStatH,battery_id), ENTRITIMEOUT))) { 
     CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake); 
     if (AllBattExistGet){  //dual battery
        if (battery_id == M_BAT){
          BAT_state_target(BAT_SEL_M);		// select main battery
        }
        if (battery_id == S_BAT){
          BAT_state_target(BAT_SEL_S);		// select Second battery
        }
        SET_MASK(OEM_BAT(M_BAT_Status1,battery_id), M_Trickle2Fast);
      } else {                               //singel battery 
        Trickle2Fast_Charge(battery_id, FALSE);
      }
    }
  }

  if (IS_MASK_CLEAR(OEM_BAT(M_BAT_Status1,battery_id), M_Trickle_T)){
    return;
  }

//trickle mode  
  if (TWR_BAT(RelativeStateOfChgL,battery_id) >= 5){
    CLEAR_MASK(OEM_BAT(M_BAT_Status1,battery_id),M_Trickle_T); 
    SET_MASK(OEM_BAT(M_BAT_Status1,battery_id),M_Fast_T);
    CLEAR_MASK(OEM_BAT(M_BATFlag1,battery_id),pEdgeProjectBATWake); 
    if (AllBattExistGet){  //dual battery
      if (battery_id == M_BAT){
        BAT_state_target(BAT_SEL_M);		// select main battery
      }
      if (battery_id == S_BAT){
        BAT_state_target(BAT_SEL_S);		// select Second battery
      }
      SET_MASK(OEM_BAT(M_BAT_Status1,battery_id), M_Trickle2Fast);
 //     SET_MASK(OEM_BAT(M_BAT_Status1,battery_id), M_Fast_T);
    } else {                               //singel battery 
      Trickle2Fast_Charge(battery_id, FALSE);
    }   
  }
}

#if Support_TWO_BATT
void Switch_Battery_Process (void)
{
  if (Current_BAT_State == Target_BAT_state){
    BAT_switch_ready_SET; 
    return;
  }
  BAT_switch_delay_timer++;
  if (BAT_switch_delay_timer >= BAT_switch_delay_time){ ////A285D00059
    Switch_Battery();
    BAT_switch_delay_timer = 0;
  }
  
}

/********************************************************************************
* Function name: Switch_Battery
*
* Descriptoin: 
*
*     -------------------------------------------------------------------------------------------------------------------
*     | State  | M1_DRV | #M2_DRV | S1_DRV | #S2_DRV | M1 FET | M2 FET | S1 FET | S2 FET |            note              |
*     -------------------------------------------------------------------------------------------------------------------
*     | State1 |  High  |   Low   |  Low   |  High   |  ON    |   ON   |   OFF  |   OFF  | Main battery supplies power  |
*     -------------------------------------------------------------------------------------------------------------------
*     | State2 |  Low   |   Low   |  Low   |  High   |  OFF   |   ON   |   OFF  |   OFF  |                              |
*     -------------------------------------------------------------------------------------------------------------------
*     | State3 |  Low   |   Low   |  Low   |  Low    |  OFF   |   ON   |   OFF  |   ON   | Initial state                |
*     -------------------------------------------------------------------------------------------------------------------
*     | State4 |  Low   |   High  |  Low   |  Low    |  OFF   |   OFF  |   OFF  |   ON   |                              |
*     -------------------------------------------------------------------------------------------------------------------
*     | State5 |  Low   |   High  |  High  |  Low    |  OFF   |   OFF  |   OFF  |   ON   | Second battery supplies power| 
*     -------------------------------------------------------------------------------------------------------------------
*
*
*
* Arguments: 
*           Change active battery from Second to Main
*	          State5 -----> State4 -----> State3 -----> State2 -----> State1 (main battery supplies power)
*                         5msec        5msec        5msec      
*
*           Change active battery from Main to Second
*	          State1 -----> State2 -----> State3 -----> State4 -----> State5 (Second battery supplies power)
*                         5msec        5msec        5msec      
*
* Return Values: None
*
*       
*******************************************************************************/


void Switch_Battery (void)
{
  if (Current_BAT_State == Target_BAT_state)
  {
    return;
  }
  
  if (Current_BAT_State < Target_BAT_state)
    { BAT_Status++; }
  else
    { BAT_Status--; } 

  switch(Current_BAT_State)
  {
    case 1:  //state 1
      M1_DRV_HI();
      M2_DRV_LOW();
      S1_DRV_LOW();
      S2_DRV_HI();
      BAT_Average_curr_CNT = 30;
      BAT_Average_curr_sum = 0;
      break;
    case 2:  //state 2
      M1_DRV_LOW();
      M2_DRV_LOW();
      S1_DRV_LOW();
      S2_DRV_HI();   
      BAT_Average_curr_CNT = 0;
      BAT_Average_curr_sum = 0;            
      break;
    case 3:  //state 3 
      M1_DRV_LOW();
      M2_DRV_LOW();
      S1_DRV_LOW();
      S2_DRV_LOW();    
      BAT_Average_curr_CNT = 0;
      BAT_Average_curr_sum = 0;      
      break;
    case 4:  //state 4 
      M1_DRV_LOW();
      M2_DRV_HI();
      S1_DRV_LOW();
      S2_DRV_LOW();    
      BAT_Average_curr_CNT = 0;
      BAT_Average_curr_sum = 0;      
      break;
    case 5:  //state 5 
      M1_DRV_LOW();
      M2_DRV_HI();
      S1_DRV_HI();
      S2_DRV_LOW();  
      BAT_Average_curr_CNT = 30;
      BAT_Average_curr_sum = 0;    
      break;
    default : 
      BAT_switch_ready_SET;
      break; 
  }
  if (Current_BAT_State == Target_BAT_state)
  {
  	BAT_switch_ready_SET;
    if( Read_AC_IN() && !ACOFF_SOURCE)// &&(!PriBattInhibGET ))
	{
      	WriteSmartChgIC(); 
	}
     
  }
}

BOOL Chk_Stop_charge_flag()
{

	if ((S_BATStopChgStatL& ~ENCHARGESUSP)|S_BATStopChgStatH)	// ignore "ENCHARGESUSP" flag
	{														// BAT2 stop charge
		BAT_state_target(BAT_SEL_M);									// select main battery
     CLEAR_MASK(nBatteryStatH, SecBat_CHARGE );
     return TRUE;
	}
	if ((M_BATStopChgStatL& ~ENCHARGESUSP)|M_BATStopChgStatH)			// ignore "ENCHARGESUSP" flag
	{													// BAT1 stop charge
	  BAT_state_target(BAT_SEL_S);								// select 2nd battery

     CLEAR_MASK(nBatteryStatH, PreBat_CHARGE );
    return TRUE;
	}
	return FALSE;
}


/********************************************************************************
* Function name: BAT_Trickle_Charge_flow
*
* Descriptoin: 
*
*    (Pre charge battery)         (Quick charge battery)
*              |
*              V
*      pre cahrge 30 mins   ------> quick battery to 80%
*                                           |
*                                           V
*      pre cahrge 30 mins   <---------------- 
*              |
*              V
*              -------------------- >quick battery to 100%
*                                           |
*                                           V
*        pre cahrge 8Hr     <----------------       
*
*                                            
* Arguments: 
*
* Return Values: 
*   Ture : trickle charge
*   FALSE : fast charge
*       
*******************************************************************************/
BOOL BAT_Trickle_Charge_flow(BYTE event_id)
{
  BYTE i;
  WORD *Prttimeout;

//not trickle mode
  if ((BattExistGet == (M_BAT_Status1 == M_Fast_T)) && ( SecBattExistGet == (S_BAT_Status1 == M_Fast_T))){ 
//  if (!(IS_M_Trickle || IS_S_Trickle)){   //M_Trickle2Fast
    return FALSE;}

  Chk_Trickle_Setting();
//DC_IN
  if (IS_MASK_SET(OEM_BAT_1B(PriBattSTS,(1^event_id)),Charging) && ((event_id == M_BAT) || (event_id == S_BAT))){
    return FALSE;
  }
  
  if ((event_id == M_BAT) && (S_CTRL_GET())){
    BAT_state_target(BAT_SEL_M);
    return FALSE;
  } 

  if ((event_id == S_BAT) && (M_CTRL_GET())){
    BAT_state_target(BAT_SEL_S);
    return FALSE;
  }
    
  
  for (i = 0; i <=1; i++){
    if (i==0){
      Prttimeout = &M_BATTrickleChgTimeOutCnt;
    }else{
      Prttimeout = &S_BATTrickleChgTimeOutCnt;
    }
   
// 8Hr timeout
//two battery time out
      if ((IS_MASK_SET(OEM_BAT(M_BATStopChgStatH,i), ENTRITIMEOUT)) && (IS_MASK_SET(OEM_BAT(M_BATStopChgStatH,(1^i)), ENTRITIMEOUT))){
        BAT_state_target(BAT_SEL_INIT);        
        M_CTRL_LOW();
        S_CTRL_LOW();
        return TRUE;
      }
//dual battery and one battery time out      
      if (IS_MASK_SET(OEM_BAT(M_BATStopChgStatH,i), ENTRITIMEOUT)){
         CLEAR_MASK(OEM_BAT(M_BATFlag1,i),pEdgeProjectBATWake); 
         CLEAR_MASK(OEM_BAT(M_BAT_Status1,i),M_Trickle_T); 
         SET_MASK(OEM_BAT(M_BAT_Status1,i),M_Fast_T); 
        if (i == M_BAT){
//          if (BattExistGet){
          if (IS_MASK_SET(S_BATFlag1,pEdgeProjectBATWake) && IS_MASK_CLEAR(S_BATStopChgStatH, ENTRITIMEOUT)){
            BAT_Trickle_Charge_step =  (S_BAT << 4) |  1;
            Switch2Trickle_Charge(S_BAT);
  //          }
          }else{
//              BAT_state_target(BAT_SEL_S);           // select Second battery
            Trickle2Fast_Charge(M_BAT, TRUE); 
          }
        }
        if (i == S_BAT){
          if (IS_MASK_SET(M_BATFlag1,pEdgeProjectBATWake) && IS_MASK_CLEAR(M_BATStopChgStatH, ENTRITIMEOUT)){
            BAT_Trickle_Charge_step =  (M_BAT << 4) |  1;
            Switch2Trickle_Charge(M_BAT);
          }else{
 //           BAT_state_target(BAT_SEL_M);
            Trickle2Fast_Charge(S_BAT, TRUE); 
          }		      // select main battery
        }  
        return TRUE;
      }    

//check trickle flow    
    if (IS_MASK_SET(OEM_BAT(M_BATFlag1,i),pEdgeProjectBATWake)){
//dual battery    
      if (*Prttimeout == OEM_BAT(M_BAT_Trickle_switch_time,(1^i))){
        OEM_BAT(M_BAT_Trickle_switch_time,(1^i)) += 30;    
//dual battery need trickle
        if (IS_MASK_SET(OEM_BAT(M_BATFlag1,(1^i)),pEdgeProjectBATWake)){
          BAT_Trickle_Charge_step =  ((1^i) << 4) |  1;
          Switch2Trickle_Charge(1^i);         
          return TRUE;
        }

        if ((IS_MASK_SET(OEM_BAT_1B(PriBattSTS,(1^i)),BattExist)) && (IS_MASK_CLEAR(TWR_BAT(BattStatusL,(1^i)),FullyChg))){
          Trickle2Fast_Charge(i, TRUE);          
          return TRUE;
        }
      }
    

      if ((TWR_BAT(RelativeStateOfChgL, (1^i))) == (OEM_BAT(M_BAT_Trickle_switch_Voltage,(1^i))) || \
         (IS_MASK_SET(TWR_BAT(BattStatusL,(1^i)),FullyChg))){
          BAT_Trickle_Charge_step =  (i << 4) | 1;
          Switch2Trickle_Charge(i);
          OEM_BAT(M_BAT_Trickle_switch_Voltage,(1^i)) = 101;          
          return TRUE;
        }    
      } else {
        if (IS_MASK_SET(OEM_BAT(M_BAT_Status1,i),M_Trickle2Fast)){
//          if (IS_MASK_SET(OEM_BAT(M_BAT_Status1, i), M_Trickle2Fast) && IS_BAT_switch_ready){                     
          if (IS_BAT_switch_ready){                     
            Trickle2Fast_Charge(i, TRUE);  
            CLEAR_MASK(OEM_BAT(M_BAT_Status1, i), M_Trickle2Fast);
          }
          return TRUE;
        }
     }
      
  }   //for loop
         
  return TRUE;
}

		
void Battery_charge_Chk(void)
{ 
//for Eject pin
  if ((BAT_Eject_Cnt >= 400) && AllBattExistGet){    // > 2S (5mS X 400)      M/S battery exist switch to BAT_M 
		BAT_state_target(BAT_SEL_M);		// select main battery
		return;
  }


//for Trickle charge
  if (BAT_Trickle_Charge_flow(Polling))
    {return;} 

	if (!SecBattExistGet && BattExistGet)
	{
		BAT_state_target(BAT_SEL_M);		// select main battery
		return;
	}
	if (!BattExistGet && SecBattExistGet)
	{
		BAT_state_target(BAT_SEL_S);		// select second battery
		return;
	}
	if (AllBattExistGet)
	{	
    if ((BAT_SEQ == 1) && (IS_MASK_CLEAR(BattStatusL,FullyChg))){ //force 1st battery charge
       BAT_state_target(BAT_SEL_M);							// select second battery
       return;
    }
  
    if ((BAT_SEQ == 2) && (IS_MASK_CLEAR(SecBattStatusL,FullyChg))){ //force 2nd battery charge
       BAT_state_target(BAT_SEL_S);							// select second battery
       return;
    }
  
		if (Chk_Stop_charge_flag())
		{
			return;												// check stop charge flag
		}
		if (Current_BAT_State == BAT_SEL_M)
		{														// now is charging Battery1
			if (RelativeStateOfChgL < 80)
			{
				return;
			}
			else
			{
				if (IS_MASK_CLEAR(SecBattStatusL,FullyChg)) 	// if Battery2 != 100%
				{
					BAT_state_target(BAT_SEL_S);							// select second battery
				}
				else
				{												// Battery2 = 100%
					BAT_state_target(BAT_SEL_M);							// select main battery
				}
			}
		}
		else if(Current_BAT_State == BAT_SEL_S)
		{
			if (RelativeStateOfChgL < 80)
			{
				BAT_state_target(BAT_SEL_M);							// select main battery
				return;
       }
			if (IS_MASK_SET(SecBattStatusL,FullyChg)) 		// if Battery != 100%
			{
				BAT_state_target(BAT_SEL_M);								// select Main battery
			}
		}
		else
		{		
			if (RelativeStateOfChgL < 80)
			{
				BAT_state_target(BAT_SEL_M);	
			}
			else
			{
				BAT_state_target(BAT_SEL_S);								// select Second battery
			}
		}
	}
}	

void Battery_discharge_Chk(void)
{
//for Eject pin	
  if (BAT_Eject_Cnt && AllBattExistGet){     //M & S battery exist 
		BAT_state_target(BAT_SEL_M);		// select main battery
		return;
  }

//PriBattDisChgGet
  if (PriBattDisChgGet){
		BAT_state_target(BAT_SEL_M);		// select main battery
		return;
  }

//SecBattDisChgGet
  if (SecBattDisChgGet){
		BAT_state_target(BAT_SEL_S);		// select second battery
		return;
  }
  
	if (!SecBattExistGet && BattExistGet)
	{
		BAT_state_target(BAT_SEL_M);		// select main battery
		return;
	}
	if (!BattExistGet && SecBattExistGet)
	{
		BAT_state_target(BAT_SEL_S);		// select second battery
		return;
	}
	// SecTo5%->MainTo5%->SecTo...
	if (AllBattExistGet)
	{
//for ship mode check
    if ((IS_MASK_SET(BAT_Unship_mode, S_unship_mode)) || (BAT_Unship_CNT != 0)){     //S battery is ship mode  
		  BAT_state_target(BAT_SEL_M);		// select main battery
		  return;
    }
    if (IS_MASK_SET(BAT_Unship_mode, M_unship_mode)){     //M battery is ship mode
		  BAT_state_target(BAT_SEL_S);		// select main battery
		  return;
    }

  
    // if Sec Battery > 5%
		if(SecRelativeStateOfChgL > 5)
		{
			BAT_state_target(BAT_SEL_S);									// select Sec battery
		  return;
		}
		else
		{														
			if (((WORD)((RemainCapH << 8)+RemainCapL )) > 45) 								// if Main Battery remain capacity <= 500mWh
			{
				BAT_state_target(BAT_SEL_M);								// select second battery
			}
			else												
			{														//  Sec Battery > 1%												
				BAT_state_target(BAT_SEL_S);							// select Sec battery
			}
		}
	}
}


void Select_Battery(void)
{
	BYTE BATT_SEL_BAK;

//Don't switch BAT under S5 and DC mode , fix on status 3
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP) && SystemIsS5)  
    {return;}

	BATT_SEL_BAK = Current_BAT_State;
  if (IS_MASK_CLEAR(SYS_STATUS,AC_ADP)||(IS_MASK_SET(SYS_STATUS,AC_ADP)&&Read_ACOFF()))   // discharge
  {					// discharge
    CLEAR_MASK(BAT_Status, BAT_Charger_sts);
    Battery_discharge_Chk();
	  if (Target_BAT_state == BATT_SEL_BAK )
			{BAT_switch_ready_SET;}
  }
	else
	{					// charge
    SET_MASK(BAT_Status, BAT_Charger_sts);
		Battery_charge_Chk();
		if (Target_BAT_state != BATT_SEL_BAK )
		{						                  // Battery select is changed.
      	nBattTempCnt = 0;
//			M_BATTrickleChgTimeOutCnt = 0;
			M_BATFastChgTimeOutCnt = 0;
//			S_BATTrickleChgTimeOutCnt = 0;
			S_BATFastChgTimeOutCnt = 0;      
		}
		else
			BAT_switch_ready_SET;
	}

  if (Target_BAT_state != BATT_SEL_BAK )
  {
	  Switch_Battery();
  }
}
#endif //if support_two_BATT

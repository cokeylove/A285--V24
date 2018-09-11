/*-----------------------------------------------------------------------------
 * TITLE: OEM_CHGIC_25700.C
 *
 * Author : Ivan
 *
 * Note : These functions are reference only.
 *---------------------------------------------------------------------------*/
 
// Include all header file
#include "..\..\include.h"


void ChargerIC_Initial(void)
{  
  for( CHGIC_ptr = 3; CHGIC_ptr < 0x05; CHGIC_ptr++ )
  {
		ReadSmartChgIC();
	}
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x12H,SwitchingFreq);   //Set frequence to 800KHz
//  CLEAR_MASK(CHGIC_ReadCmd0x12L,EN_LDO);          //Cathy for power request.
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L, SMBus_NoPEC);
//  CLEAR_MASK(CHGIC_ReadCmd0x31L,IFAULT_HI);
//  SET_MASK(CHGIC_ReadCmd0x31L,IFAULT_LOW);
  CLEAR_MASK(CHGIC_ReadCmd0x31L,EN_EXTILIM);
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
  CLEAR_MASK(CHGIC_ReadCmd0x30L,AUTO_WAKEUP_EN);  
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x33L,VSYS_T0); 
  SET_MASK(CHGIC_ReadCmd0x33L,VSYS_T1);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_0);  
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_1);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_2);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_3);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_4);
  	CLEAR_MASK(CHGIC_ReadCmd0x33L,PROCHOT_HOST_EN);  
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }

  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  CLEAR_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
  CLEAR_MASK(CHGIC_ReadCmd0x34L,VSYS);
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
  CHGIC_ReadTempH = 0x00;//A285D00047 //0x02; //A485D000103
  CHGIC_ReadTempL = 0x00;//A285D00047//0xC0;  //A485D000103
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  SET_MASK(CHGIC_Status,ChgIcInitial);

  ChargeIC_InitCnt = CHARGE_IC_VOLTAGE_SET_COUNT; //A285D00047 set time out 3s
}

void ChargeIC_InitACDC(void){

  if(ChargerIC_Init > 4){
      return;
  }

  
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  
  if(OneOfTwoBattExistGet){
    SET_MASK(CHGIC_ReadCmd0x12L,IDPM_EN);
  }
  else
  	CLEAR_MASK(CHGIC_ReadCmd0x12L,IDPM_EN);
   SET_MASK(CHGIC_ReadCmd0x12H,IDPM_AUTO_DIS);
  //if((IS_MASK_SET(SYS_STATUS, AC_ADP) || (ADPID_ON_Detect())) && OneOfTwoBattExistGet){
  if(IS_MASK_SET(SYS_STATUS, AC_ADP) && OneOfTwoBattExistGet){
  	SET_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
    SET_MASK(CHGIC_ReadCmd0x34L,VSYS);
  	}else{
  	CLEAR_MASK(CHGIC_ReadCmd0x34L,BATPRES); 
    CLEAR_MASK(CHGIC_ReadCmd0x34L,VSYS);
  }
	
  
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }  

  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  ChargerIC_Init++;
  
}



//-----------------------------------------------------------------
// Service Charger SMBus
//-----------------------------------------------------------------

//  CHGIC_ptr
//  00 : W_chargecurrent
//  01 : W_chargevoltage
//  02 : W_InputCurrent
//  03 : W_ChargerMode
//  04 : W_ChargeOption

void WriteSmartChgIC(void)
{
  BYTE sCmd, *sData;
  WORD AirplaneModeChargeCurrentLimit;
#if Support_TYPE_C
  WORD AdpWattTemp;
#endif

#if Support_TWO_BATT
  if ((BAT_Eject_Cnt != 0) && (BAT_Eject_Cnt < 400))
    {return;}

  if (BAT_switch_not_ready)
    {return;}
  Select_Battery();
#endif

	if(Read_AC_IN())
	{
		if(AD_ID_Detect_Delay != 1)
		{
  		return;
		}
	}
  if (!Read_AC_IN())
    return;

  if( CHGIC_ptr>=W_ChargerMode ) 
  {
  	CHGIC_ptr = W_InputCurrent;//W_chargecurrent;
  }
  else
  {
  	CHGIC_ptr++;
  }

  switch( CHGIC_ptr )
    {
      case W_chargecurrent:
	  	sCmd = C_ChargeCurrent;
	  	if((BAT_Status&BAT_switch_state_now) == BAT_SEL_M )
	  	{
        	sData = &ChargeCurrL;
	  	}
		else if((BAT_Status&BAT_switch_state_now) == BAT_SEL_S )
		{
			sData = &SecChargeCurrL;
		}

#if (PLATFORM_CHIP == AMD)
		if(CurrentTypeCWatt <= 45)//A285D00036
		{
          if(((WORD)((ChargeCurrH<< 8) + ChargeCurrL)) >= 0x0BB8){
		  	if((BAT_Status&BAT_switch_state_now) == BAT_SEL_M )
		  	{
            	ChargeCurrL = 0xB8;
            	ChargeCurrH = 0x0B;
		  	}
			else if((BAT_Status&BAT_switch_state_now) == BAT_SEL_S )
			{
				SecChargeCurrL = 0xB8;
            	SecChargeCurrH = 0x0B;
			}
            sData = &ChargeCurrL;
          }
		  }
#endif		
        if (IS_MASK_SET(u4Cfgspace3, AirplaneMode)) {
          if (SystemIsS0) {
            AirplaneModeChargeCurrentLimit = (CHARGE_CURRENT_1A_H << 8) | CHARGE_CURRENT_1A_L;
          } else if (SystemIsS3 || SystemIsS5) {
            AirplaneModeChargeCurrentLimit = (CHARGE_CURRENT_2_2A_H << 8) | CHARGE_CURRENT_2_2A_L;
          }
          sData = (BYTE*)&AirplaneModeChargeCurrentLimit;
        } else {
          //
          // Not in AirPlane mode.
          //
        }
        break;

      case W_chargevoltage:
	  	sCmd = C_ChargeVoltage;
	  	if((BAT_Status&BAT_switch_state_now) == BAT_SEL_M )
       		sData = &ChargeVoltL;
		else if((BAT_Status&BAT_switch_state_now) == BAT_SEL_S )
			sData = &SecChargeVoltL;
        
        break;
      case W_InputCurrent:
  
#if Support_TYPE_C
				//if(!Read_DCIN_ATTACHED2())
				{	//Write the Type C adaptor output current value to Charger IC "InputCurrent" register.
					if(CurrentTypeCI != 0)
					{
						if(CurrentTypeCWatt >= 65)
						{
              				CHGIC_InputCurrentH = ((A65WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;	//(60/V)*1000/50 ;"(CurrentTypeCV-1)" just for carry(take ceiling).
              				CHGIC_InputCurrentL = A65WINPUTCURRL;
              				InputCurrent_Watt =65;
						}
						else if(CurrentTypeCWatt >= 60)
						{
              				CHGIC_InputCurrentH = ((A60WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;
              				CHGIC_InputCurrentL = A60WINPUTCURRL;
              				InputCurrent_Watt =60;
						}
						else if(CurrentTypeCWatt >= 45)
						{
							CHGIC_InputCurrentH = ((A45WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;
							CHGIC_InputCurrentL = A45WINPUTCURRL;
	            InputCurrent_Watt =45;
						}
						else
						{
							AdpWattTemp = CurrentTypeCI / 5;
					    CHGIC_InputCurrentL = AdpWattTemp >> 8;
					    CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
			       	InputCurrent_Watt =CurrentTypeCWatt;
			      }
				  }
				  else
				  {
            		CHGIC_InputCurrentH = A45WINPUTCURRH;
            		CHGIC_InputCurrentL = A45WINPUTCURRL;
            		InputCurrent_Watt =45;

				  }
				}
				//else
#endif //Support_TYPE_C
                #if 0
				{
#if (PLATFORM_CHIP == INTEL)
        if (!(nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) )&& IS15Inch ) {  //DIS 
          if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap >= 45)){
            CHGIC_InputCurrentH = A45WINPUTCURRH;
            CHGIC_InputCurrentL = A45WINPUTCURRL;
            InputCurrent_Watt =45;
          }
				}else{				
          if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
              (OneOfTwoBattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
            if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
              CHGIC_InputCurrentH = A45WINPUTCURRH;
              CHGIC_InputCurrentL = A45WINPUTCURRL;
              InputCurrent_Watt =45;
            }else if (u4ACAdaptorCap == 65){
              CHGIC_InputCurrentH = A65WINPUTCURRH;
              CHGIC_InputCurrentL = A65WINPUTCURRL;
              InputCurrent_Watt =65;
          }else{                                                         
            if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
              CHGIC_InputCurrentH = A45WINPUTCURRH;
              CHGIC_InputCurrentL = A45WINPUTCURRL;
              InputCurrent_Watt =45;
            }else if (u4ACAdaptorCap == 65){
              CHGIC_InputCurrentH = A65WINPUTCURRH;
              CHGIC_InputCurrentL = A65WINPUTCURRL;
              InputCurrent_Watt =65;
          }
				}
#endif
#if (PLATFORM_CHIP == AMD)
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = A45WINPUTCURRL;
          InputCurrent_Watt = 45;
        }else if (u4ACAdaptorCap >= 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt = 65;
        }  
#endif

			}
		#endif
        sCmd = C_InputCurrent;
        sData = &CHGIC_InputCurrentL;
        break;
      case W_ChargerMode:

#if !Trickle_Control_By_CHGIC
         if (M_BATWake || S_BATWake)
           {return;}
#endif 

        if ((CHGIC_ReadCmd0x12L!=0) || (CHGIC_ReadCmd0x12H!=0))
        {
          if (IS_MASK_SET(cBattFlag0,cCmdAcOff)||Read_ACOFF()||
		    	    (((BAT_Status&BAT_switch_state_now) == BAT_SEL_M)&&IS_MASK_SET(BattStatusL,FullyChg))||
		  	      (((BAT_Status&BAT_switch_state_now) == BAT_SEL_S)&&IS_MASK_SET(SecBattStatusL,FullyChg))		)  //  || IS_MASK_CLEAR(nBatteryStatH,CMBS_CHARGE))
          {
            SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
          }
          else
          {        
            {
              CLEAR_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Enable Charge
            }
          }
		     if(((Current_BAT_State == BAT_SEL_M)&&(M_BATStopChgStatL|M_BATStopChgStatH))||
		  		  ((Current_BAT_State == BAT_SEL_S)&&(S_BATStopChgStatL|S_BATStopChgStatH))||
		  		  (Current_BAT_State == BAT_SEL_INIT) || inhibit2sec ||
		  		  ACOFF_SOURCE)
	  	     {
            SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);   //Inhibit Charge
          }
            sCmd = C_ChargerMode;
            sData = &CHGIC_ReadCmd0x12L;
        }
        else
        {
          return;
        }
        break;

      case W_ChargeOption:
        CLEAR_MASK(CHGIC_ReadCmd0x31L,IFAULT_HI);
        SET_MASK(CHGIC_ReadCmd0x31L,IFAULT_LOW);
        sCmd = C_ChargeOption2;
        sData = &CHGIC_ReadCmd0x31L;
        break;

      default:
        return;
        break;
    }
    if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, sCmd, sData,SMBus_NoPEC))
    {
      CHGIC_SMbusFailCnt++;
    }
	//A485D000103+ for charger test
    if(ChargeIC_InitCnt > 0)
    {
	 bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
        CHGIC_ReadTempH = 0x00; //A285D00047 //0x02;
        CHGIC_ReadTempL = 0x00; //A285D00047//0xC0;
    if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
    {
      CHGIC_SMbusFailCnt++;
     }
        ChargeIC_InitCnt--;
    }
	//A485D000103+ for charger test
}

void StopChgIC(void)
{
  SET_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}

void StartChgIC(void)
{
  BYTE battery_id;
  battery_id =  (Current_BAT_State == BAT_SEL_M) ? M_BAT : S_BAT; 

//update Charge Current
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeCurrent, &TWR_BAT(ChargeCurrL,battery_id),SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }

//update Charge Voltage
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeVoltage, &TWR_BAT(ChargeVoltL,battery_id),SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }

  CLEAR_MASK(CHGIC_ReadCmd0x12L,ChargeInhibit);  //Inhibit Charge
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}

void Trickle_ChgIC(BYTE OnOff)
{
  if (OnOff == Enable){     //13.000v
    WSMbusTemp02=0x32;    //A285D00045
    WSMbusTemp01=0xC8;    //A285D00045
  }else{                    //9.2V
    WSMbusTemp02=0x24;
    WSMbusTemp01=0x00;         
  }
 
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_MinSystemVoltage, &WSMbusTemp01,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}
      


const sRSmbusBStruct ReadChgIcCmdTable [] =
{   //     command                   REG         no.
  { C_ChargeCurrent,    &CHGIC_ReadCmd0x14L      ,0x00},
  { C_ChargeVoltage,    &CHGIC_ReadCmd0x15L      ,0x00},
  { C_InputCurrent,     &CHGIC_ReadCmd0x3FL      ,0x00},
  { C_ChargerMode,      &CHGIC_ReadCmd0x12L      ,0x00},
  { C_ChargeOption2,    &CHGIC_ReadCmd0x31L      ,0x00},      
};


void ReadSmartChgIC(void)
{
  {
    if(!bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr,
           ReadChgIcCmdTable[CHGIC_ptr].cmd,
           ReadChgIcCmdTable[CHGIC_ptr].smbdataA,
             SMBus_NoPEC))
    {  // SMBUS FAIL
      CHGIC_SMbusFailCnt++;
    }
  }
}


void Update_ChargeIC(void)
{
  BYTE BakCHGIC_InputCurrentL, BakCHGIC_InputCurrentH;
#if Support_TYPE_C  
  WORD AdpWattTemp;
#endif

	if(Read_AC_IN())
	{
		if(AD_ID_Detect_Delay != 1)
		{
  		return;
		}
	}
  // Save origin values.
  BakCHGIC_InputCurrentL = CHGIC_InputCurrentL;
  BakCHGIC_InputCurrentH = CHGIC_InputCurrentH;
#if Support_TYPE_C
	//if(!Read_DCIN_ATTACHED2())
	{	//Write the Type C adaptor output current value to Charger IC "InputCurrent" register.
		if(CurrentTypeCI != 0)
		{
			if(CurrentTypeCWatt >= 65)
			{
        			CHGIC_InputCurrentH = ((A65WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;
        			CHGIC_InputCurrentL = A65WINPUTCURRL;
        			InputCurrent_Watt =65;
			}
			else if(CurrentTypeCWatt >= 60)
			{
        			CHGIC_InputCurrentH = ((A60WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;
        			CHGIC_InputCurrentL = A60WINPUTCURRL;
        			InputCurrent_Watt =60;
			}
			else if(CurrentTypeCWatt >= 45)
			{
				CHGIC_InputCurrentH = ((A45WINPUTCURRWATT*400)+(CurrentTypeCV-1))/CurrentTypeCV;
				CHGIC_InputCurrentL = A45WINPUTCURRL;
        			InputCurrent_Watt =45;
			}
			else
			{
				AdpWattTemp = CurrentTypeCI / 5;
		    	CHGIC_InputCurrentL = AdpWattTemp >> 8;
		    	CHGIC_InputCurrentH = AdpWattTemp & 0x00FF;
        		InputCurrent_Watt =CurrentTypeCWatt;
      }
	  }
	  else
	  {
		CHGIC_InputCurrentH = A45WINPUTCURRH;
		CHGIC_InputCurrentL = A45WINPUTCURRL;
    	InputCurrent_Watt =45;


	  }
	}
	//else
#endif //Support_TYPE_C
    #if 0
	{
#if (PLATFORM_CHIP == INTEL)
#ifdef OEM_Hook_Power_90WSystem_support
    OEM_Hook_Power_90WSystem();
#endif
    if (!(nSwitchableBTNGET || IS_MASK_SET(pProject0,b7SWITCHBAK) )&& IS15Inch ) {  //DIS 
      if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap >= 45)){
        CHGIC_InputCurrentH = A45WINPUTCURRH;
        CHGIC_InputCurrentL = A45WINPUTCURRL;
        InputCurrent_Watt =45;
      }
	  
    }else{				
      if ((SystemIsS3 || SystemIsS4 || SystemIsS5) && 
          (OneOfTwoBattExistGet && (Read_AC_IN() || !Read_ACOFF()))){          //AC + DC _ S3 4 5
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = A45WINPUTCURRL;
          InputCurrent_Watt =45;
        }else if (u4ACAdaptorCap == 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt =65;
        }else if (u4ACAdaptorCap>=90){
          CHGIC_InputCurrentH = A90WINPUTCURRH_AD - Intel15InputCurrentAD;
          CHGIC_InputCurrentL = A90WINPUTCURRL;
          InputCurrent_Watt =90;
        }
      }else{                                                         
        if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
          CHGIC_InputCurrentH = A45WINPUTCURRH;
          CHGIC_InputCurrentL = 0x00;
          InputCurrent_Watt =45;
        }else if (u4ACAdaptorCap == 65){
          CHGIC_InputCurrentH = A65WINPUTCURRH;
          CHGIC_InputCurrentL = A65WINPUTCURRL;
          InputCurrent_Watt =65;
        }else if (u4ACAdaptorCap>=90){
          CHGIC_InputCurrentH = A90WINPUTCURRH_A - Intel15InputCurrentA;
          CHGIC_InputCurrentL = A90WINPUTCURRL;
          InputCurrent_Watt =90;
        }
      }
    }
#endif
#if (PLATFORM_CHIP == AMD)
    if ((u4ACAdaptorCap == 0) || (u4ACAdaptorCap == 45)){
      CHGIC_InputCurrentH = A45WINPUTCURRH;
      CHGIC_InputCurrentL = A45WINPUTCURRL;
      InputCurrent_Watt = 45;
    }else if (u4ACAdaptorCap >= 65){
      CHGIC_InputCurrentH = A65WINPUTCURRH;
      CHGIC_InputCurrentL = A65WINPUTCURRL;
      InputCurrent_Watt = 65;
    }  
#endif

  } 
  #endif
  if ((CHGIC_InputCurrentH == BakCHGIC_InputCurrentH) && (CHGIC_InputCurrentL == BakCHGIC_InputCurrentL)) {
    // Values are not changed, no need to do SMBUS write.
    return;
  }

  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_InputCurrent, &CHGIC_InputCurrentL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  else
  {
    CHGIC_SMbusFailCnt = 0;
  }
}

void ChargeIC_Polling_Task(void)
{
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
  CHGIC_ReadTempH = 0x00;//0x02;
  CHGIC_ReadTempL = 0x00;//0xC0;
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  
  if(IS_MASK_CLEAR(BATUPDATEFW,PriBattInhib))
  {
  	bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L, SMBus_NoPEC);
	CLEAR_MASK(CHGIC_ReadCmd0x12L,IADP_RATIO);
  	SET_MASK(CHGIC_ReadCmd0x12H,SwitchingFreq);   //Set frequence to 800KHz
  	CLEAR_MASK(CHGIC_ReadCmd0x12H,AUDIO_Freq);
  	CLEAR_MASK(CHGIC_ReadCmd0x12H,OTG_ON_ChrgOK);
  	if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  	{
    	CHGIC_SMbusFailCnt++;
  	}

	bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L, SMBus_NoPEC);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_0);  
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_1);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_2);
  	CLEAR_MASK(CHGIC_ReadCmd0x33H,ICRIT_3);
  	SET_MASK(CHGIC_ReadCmd0x33H,ICRIT_4);
  	CLEAR_MASK(CHGIC_ReadCmd0x33L,PROCHOT_HOST_EN);
  	if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ProchotOption0, &CHGIC_ReadCmd0x33L,SMBus_NoPEC))
  	{
   		CHGIC_SMbusFailCnt++;
  	}
	
//  	bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
//  	CLEAR_MASK(CHGIC_ReadCmd0x30L,FET_LATCHOFF);
//	CHGIC_ReadCmd0x30H &= 0x03;      //bit 10~15 : 0
//  	if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
//  	{
//    	CHGIC_SMbusFailCnt++;
//  	}
  }
}


void Hook_CHGIC_AdapterIn(void)
{
  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL, SMBus_NoPEC);
  CHGIC_ReadTempH = 0x00; //A285D00047 //0x02;  //A485D000103
  CHGIC_ReadTempL = 0x00; //A285D00047  //0xC0;  //A485D000103
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_InputVoltage, &CHGIC_ReadTempL,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }

  bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L, SMBus_NoPEC);
  SET_MASK(CHGIC_ReadCmd0x34L,PROFILE_5);  
  if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ProchotOption1, &CHGIC_ReadCmd0x34L,SMBus_NoPEC))
  {
    CHGIC_SMbusFailCnt++;
  }
  	CLEAR_MASK(CHGIC_ReadCmd0x12H,LOW_POWER_MODE_EN);
  	SET_MASK(CHGIC_ReadCmd0x12H,SwitchingFreq);   //Set frequence to 800KHz    
  	if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
      CHGIC_SMbusFailCnt++;
  }
  	else
  	{
      CHGIC_SMbusFailCnt = 0;
  	}
  
  ChargerIC_Init = 0;
}


void Hook_CHGIC_AdapterOut(void)
{
  	CLEAR_MASK(CHGIC_ReadCmd0x12L,BATLearn);
  	SET_MASK(CHGIC_ReadCmd0x12H,LOW_POWER_MODE_EN);
  	if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargerMode, &CHGIC_ReadCmd0x12L,SMBus_NoPEC))
  {
      CHGIC_SMbusFailCnt++;
  }
  	else
  	{
      CHGIC_SMbusFailCnt = 0;
  	}

  ChargerIC_Init = 0;  
}

void Hook_CHGIC_BatIn(void)
{ 
  ChargerIC_Init = 0;
}


void Hook_CHGIC_BatOut(void)
{
  ChargerIC_Init = 0;  
}



void ChargerTurbo (
  BYTE OnOff
)
{
  //
  // 0: Stop charger turbo, 1: Start charger turbo
  //
 if (OnOff){  //start charger turbo
 

 } else{      //stop charge turbo
  if( IS_MASK_SET(CHGIC_ReadCmd0x31L,EN_IDCHG_REG) || IS_MASK_SET(CHGIC_ReadCmd0x30H,BIT7) )  
  {
     bRWSMBus(chSMB_CHG,SMbusRW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L, SMBus_NoPEC);
     CLEAR_MASK(CHGIC_ReadCmd0x31L,EN_IDCHG_REG);      //CLEAR_MASK(CHGIC_ReadCmd0x3BL,BIT2);  // BIT2  IDCHG_REG_EN
//     bRWSMBus(chSMB_CHG, SMbusRW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L, SMBus_NoPEC);
//     CLEAR_MASK(CHGIC_ReadCmd0x30H,BIT7);  // BIT15 IDCHG_EN
//     if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeOption1, &CHGIC_ReadCmd0x30L,SMBus_NoPEC))
//     {
//       CHGIC_SMbusFailCnt++;
//     }
     if (!bRWSMBus(chSMB_CHG, SMbusWW, Charger_Addr, C_ChargeOption2, &CHGIC_ReadCmd0x31L,SMBus_NoPEC))
     {
       CHGIC_SMbusFailCnt++;
     }
   }
 }
}



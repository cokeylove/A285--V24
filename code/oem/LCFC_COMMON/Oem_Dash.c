/*-----------------------------------------------------------------------------
 * Filename: OEM_EC6266.C
 * Function: OEM EC CMD HANDLE
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *---------------------------------------------------------------------------*/
//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

#if Support_DASH
DashRxPkt * sDashRxPkt;
DashTxPkt * sDashTxPkt;

//----------------------------------------------------------------------------
//Config SMBus registers for DASH
//----------------------------------------------------------------------------	
void Init_Smbus4Dash(void)
{
    DASH_HOCTL2 = BIT5;	//enable slave mode
    DASH_SICR = 0x0A;		//enable slave interrupts
    DASH_RESLADR = DASH_EC_ADDR;	//set slave address, default address is 0x00
//	  sla_addr = DASH_EC_ADDR;
	
    DASH_HOCTL = 0x02;		//kill current session
    DASH_HOCTL = 0x00;		//kill current session
    DASH_HOSTA = 0xFF;		//clear host's state
    DASH_SLSTA = 0xFF;		//clear slave's state
}

//----------------------------------------------------------------------------
//Initialization
//----------------------------------------------------------------------------	
void Init_Dash(void)
{    
//    SLVISEL |= 0x03;				// Slave A interface is located at SMCLK3/SMDAT3
    Init_Smbus4Dash();
    //Assign entity SRAM space
    sDashRxPkt = SMBUS_RECV_BUFFER; 
    sDashTxPkt = SMBUS_WRITE_BUFFER;
    DASH_HOCTL |= BIT0;   //host interrupt enable

	  SET_MASK(IER1,Int_SMBUS0);	// SET SMBUS A Interrupt
}


//----------------------------------------------------------------------------
//Change smbus mode as slave
//----------------------------------------------------------------------------	
void SmbusSlvMode(void)
{
    DASH_HOCTL2 = BIT5;
}

//----------------------------------------------------------------------------
//Change smbus mode as host
//----------------------------------------------------------------------------	
void SmbusHstMode(void)
{
    DASH_HOCTL2 = BIT0;
}


//----------------------------------------------------------------------------
//Check sensor ID and report reading data
//  Return PLDM completion codes
//----------------------------------------------------------------------------	
BYTE ReadSensorData(WORD SnrID, DWORD *value)
{
    BYTE pcmp;
#if (Support_DASH_APU_Voltage || Support_DASH_DDR_Voltage)    
    DWORD VinTmp1;
#endif
    RamDebug(0xD7); 
    RamDebugWord(SnrID); 
    pcmp = DASH_COMP_CODE_SUCCESS;
    switch(SnrID) 
    {         
    	case DASH_SNR_ID_APU_T: //APU Temperature
            *value = (WORD)TEMP_Buff_3;               
            break;

    	case DASH_SNR_ID_DDR_T:  //DDR Temperature
            *value = (WORD)nRamTemp;            
            break;              
#if Support_DASH_APU_Voltage            
		case DASH_SNR_ID_APU_V: //APU Voltage
			VinTmp1 = (WORD)VDD_CORE * 100;						
			VinTmp1 *= 29;
			VinTmp1 /= 1024;
			*value = (WORD)VinTmp1;
			break;
#endif //Support_DASH_APU_Voltage

#if Support_DASH_DDR_Voltage   
		case DASH_SNR_ID_DDR_V: //DDR Voltage
			VinTmp1 = (WORD)DDR_V_135*100;
			VinTmp1 *= 29;	  
			VinTmp1 /= 1024;
			*value = (WORD)VinTmp1;
			break;
#endif //Support_DASH_DDR_Voltage   

    	case DASH_SNR_ID_FAN: //FAN SPEED    	        
            *value = (nAtmFanSpeed > 0)?(WORD)nAtmFanSpeed*100:0;
            break;

    	default:
            pcmp = DASH_COMP_CODE_INVALID_SENSOR_ID;
            *value = 0;
            break;
    }//end switch

    return pcmp;	
}

//----------------------------------------------------------------------------
//SMBus slave receive data
//  Return length of receving data
//----------------------------------------------------------------------------	
//ivan dash BYTE SmbSlvRead(BYTE *recvbuff)
void SmbSlvRead(void)
{
    BYTE state;

    while(1)
    {
    	state = DASH_SLSTA;
    	if(!(state & I2C_SLV_STATE_INTRS))		//if no interrupts, continue.
    { 
       RamDebug(0xD2);     
    		continue;
    	} 
    	DASH_SLSTA = state;
    	
    	if(state & I2C_SLV_STATE_TIMEOUT) 
     {   //Timeout
       RamDebug(0xD3);
       recvCnt = 0;
       break;
     }//end if

    	if(state & I2C_SLV_STATE_STOP)
    	{
       RamDebug(0xD4);
       recvCnt |= 0x80;		// set stop receive flag
    		break;
    	}
     RamDebug(0xD5);   
     SmbReadDataBuf[recvCnt] = DASH_SLDA;
     SmbReadDataBuf[recvCnt] = DASH_SLDA;
     SmbReadDataBuf[recvCnt] = DASH_SLDA;   //A285D00074 re-read data find once always fail. 
     recvCnt ++;
   	 break;
  }//end while

    return; 
}


//----------------------------------------------------------------------------
//Receve packet from DASH
//----------------------------------------------------------------------------	
//void RecvDashPacket(void)
//{
//  BYTE *buff = sDashRxPkt;

//  recvCnt = SmbSlvRead(buff);
//}
//----------------------------------------------------------------------------
//Caculate CRC
//----------------------------------------------------------------------------	
BYTE DashCrc8(BYTE crc, BYTE dat)
{
    BYTE temp;
    crc ^=dat;
    temp = crc;

    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        crc ^= temp;
    }
    else
    {
        temp = temp<<1;
        crc ^= 0x09;
        crc ^= temp;
    }
    
    if(IS_MASK_CLEAR(temp, BIT7))
    {
        temp = temp<<1;
        crc ^= temp;
    }
    else
    {
        temp = temp<<1;
        crc ^= 0x07;
        crc ^= temp;
    }

    return crc;
}

//----------------------------------------------------------------------------
//Return packet error check code
//----------------------------------------------------------------------------	
BYTE DashGetPEC(void)
{
    BYTE *dat;
    BYTE i, bcnt, crc;
    dat = (BYTE*)&sDashTxPkt;

    crc = 0x00;
    crc = DashCrc8(crc, DASH_XMIT_ADDR);
    crc = DashCrc8(crc, sDashTxPkt->smbus.cmd);
    crc = DashCrc8(crc, bcnt = sDashTxPkt->smbus.bcnt);
    for(i=0; i<bcnt ; i++)
      {crc = DashCrc8(crc, *(dat+i));}

    return crc;
}

//----------------------------------------------------------------------------
//For response DASH's request
//----------------------------------------------------------------------------	
void SendDashPacket(void)
{
    BYTE state;
    BYTE *dat;

    dat = (BYTE*)&sDashTxPkt->smbus.saddr;

    DASH_HOSTA = 0xFF;
    DASH_TRASLA = DASH_XMIT_ADDR;
    DASH_HOCMD = sDashTxPkt->smbus.cmd;
    DASH_D0REG = sDashTxPkt->smbus.bcnt;
    DASH_HOBDB =  *(dat++);	//first byte data
//    DASH_PECERC = DashGetPEC();
//    DASH_HOCTL = 0xD5;		// with PEC
    DASH_HOCTL = 0x55;		// non PEC

    while(1)
    {
        //RamDebug(0xD1);
        //RamDebug(DASH_HOSTA); //A285D00074- remove log for easy debug
        state = DASH_HOSTA;
        if(!(state & 0xFE))
            continue;

        //error interrupts
        if(state & 0x7C)
            break;

        //transmitting is done
        if(state & 0x02)
            break;

        if(state & 0x80)
            DASH_HOBDB = *(dat++);

        DASH_HOSTA = state;
    }//end while

    DASH_HOSTA = state;

}

//----------------------------------------------------------------------------
//Define/Caculate the type/size of sensor reading data for report to DASH.
//Return the size of sensor reading data
//----------------------------------------------------------------------------

#if 0
BYTE SensorDataSize(void *vaddr, DWORD val, BYTE size)
{
switch(size) 
{
    case 0x00:
    case 0x01:
    	*(BYTE*)vaddr = val & 0xFF;
    	return 1;
    case 0x02:
    case 0x03:
    	//*(WORD*)vaddr = val & 0xFFFF;
    	//convert data arrangement from MSB first to LSB first
    	*(WORD*)vaddr = ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
    	return 2;
    case 0x04:
    case 0x05:
    	//*(DWORD*)vaddr = val & 0xFFFFFFFF;
    	//convert data arrangement from MSB first to LSB first
    	*(DWORD*)vaddr = ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8)
    		| ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8);
    	return 4;
}//end switch


return 0;
}
#endif 

//----------------------------------------------------------------------------
//return: Return 0, EC igonore current request of DASH.
//----------------------------------------------------------------------------	
BYTE ProcessDASHCommand(void)
{
    DWORD PresentReading;
    BYTE i, bCnt;
    struct _DashTxPkt *txpkt;
    struct _DashRxPkt *rxpkt;
    txpkt = sDashTxPkt;
    rxpkt = sDashRxPkt;
    //get common fields from RxPkt
    for(i=0;i <SMBUS_BUFFER_SIZE; i++)
    {   
        *((BYTE*)txpkt+i) = *((BYTE*)rxpkt+i);
    }
    txpkt->smbus.saddr = DASH_EC_ADDR;
    txpkt->mctp.deid = rxpkt->mctp.seid;
    txpkt->mctp.seid = rxpkt->mctp.deid;
    //process PLDM command
    RamDebug(0xD6); 
    RamDebug(rxpkt->pldm.pcmd); 
    switch(rxpkt->pldm.pcmd) {
    	//Numeric Sensor Commands
    	case DASH_PCMD_GET_SNR_READING: 
   		txpkt->pldm.pcmp = ReadSensorData(rxpkt->NumSensorRep.SnrID, &PresentReading);
    		if(txpkt->pldm.pcmp == DASH_COMP_CODE_SUCCESS)
       {
    			txpkt->NumSensorRsp.DataSize = 0x02;		//UINT32
    			txpkt->NumSensorRsp.OptionalState = 0x00;	//Enable to report valid present state
    			txpkt->NumSensorRsp.EvntMsgEn = 0x02;	//Events enable
    			txpkt->NumSensorRsp.PresentState = 0x01;	//Normal
    			txpkt->NumSensorRsp.PrevState = 0x01;	//Normal
    			txpkt->NumSensorRsp.EvntState = 0x01;	//Normal
    			txpkt->NumSensorRsp.PresentReading = PresentReading; //
    		}
       else
    		{
    			txpkt->NumSensorRsp.DataSize = 0x00;		//UINT8
    			txpkt->NumSensorRsp.OptionalState = 0x00;	//Enable to report valid present state
    			txpkt->NumSensorRsp.EvntMsgEn = 0x02;	//Events enable
    			txpkt->NumSensorRsp.PresentState = 0x00;	//UNKNOWN
    			txpkt->NumSensorRsp.PrevState = 0x00;	//UNKNOWN
    			txpkt->NumSensorRsp.EvntState = 0x00;	//UNKNOWN
    		}//end if
    		//byte count caculation
    		bCnt = sizeof(struct _DashTxPkt ); //ivan + sizeof(struct _NumSensorRsp);
//    		bCnt -= 2;	//skip command and byteCnt fields
//   		bCnt = bCnt - 4 + SensorDataSize((void *)(&txpkt->NumSensorRsp.PresentReading ), PresentReading, txpkt->NumSensorRsp.DataSize );
    		txpkt->smbus.bcnt = bCnt;
    		break;
    	default:
    		bCnt = 0;
    		break;
    }//end switch

    return bCnt;
}

//----------------------------------------------------------------------------
//DASH Main Function
//  This function will be called when SMBus interrupt is asserted.
//----------------------------------------------------------------------------	
void DashMain(void)
{
  BYTE i;
//RecvDashPacket();
  SmbSlvRead();  
	if (recvCnt & BIT7)
	{
	  recvCnt = 0;
	  if(ProcessDASHCommand() > 0 )
	  {
	 	  SmbusHstMode();
	    	SendDashPacket();
	    	SmbusSlvMode();

	    	//clear read/write buffer
	    	for(i=0; i<SMBUS_BUFFER_SIZE; i++) 
	     {
	    	  SmbReadDataBuf[i] = 0;
      	  SmbWriteDataBuf[i] = 0;
	    	 }//end for
	  }//end if
	}
}

#endif

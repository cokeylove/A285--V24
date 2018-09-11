/*-----------------------------------------------------------------------------
 * TITLE: OEM_RFID.C
 * TITLE: OEM_RFID.H
 *
 * Author : AC & CM
 *
 * Copyright (c) ITE INC. All Rights Reserved.
*---------------------------------------------------------------------------*/

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\..\include.h"


//----------------------------------------------------------------------------
// For RFID
//----------------------------------------------------------------------------

BYTE readRFIDEEprom(BYTE BANKx,BYTE addr)
{
	ECINDAR3 = 0x40; 
	ECINDAR2 = RFID_EEPROMA2;									
	if( BANKx <= 6 )
	{
		ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_index);	
	}
	else if( BANKx == 7 )
	{
		ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_index2);	
	}
	
	if ((BANKx&0x01)==0)
	{
		ECINDAR0 = addr;							
	}
	else
	{
		ECINDAR0 = (addr|0x80);						
	}
  
	if( (BANKx <= 5) )
	{
   		if( ECINDDR == 0x07 ) {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N2); 	
	
    	} else if( ECINDDR == 0x0F ) {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N1); 	
	
    	}  else {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_RFID1); 
		}
	}
	else if( BANKx == 6 )
	{
		if (ECINDDR == 0x07)
		{
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N5); 	
		
    	} else if( ECINDDR == 0x0F ) {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N4); 	
	
    	} else  {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_RFID2); 	
	
    	}
	}
	else if( BANKx == 7 )
	{
		if (ECINDDR == 0x07)
		{
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N7); 	
		
    	} else if( ECINDDR == 0x0F ) {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_N6); 	
	
    	} else  {
		
			ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_RFID3); 	
	
    	}
	}
	
	return ECINDDR;
}

BYTE readRFIDindex(BYTE BANKx,BYTE addr)
{
	ECINDAR3 = 0x40; 
	ECINDAR2 = RFID_EEPROMA2;									
	if( BANKx <= 6 )
	{
		ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_index);	
	}
	else if( BANKx == 7 )
	{
		ECINDAR1 = ((BANKx >> 1)|RFID_EEPROMA1_index2);
	}
	if ((BANKx&0x01)==0)
	{
		ECINDAR0 = addr;							
	}
	else
	{
		ECINDAR0 = (addr|0x80);						
	}
	return ECINDDR;
}

void writeRFIDEEprom(BYTE BANKx,BYTE addr,BYTE nData)
{

    BYTE nTemp;

	
	DisableAllInterrupt();

	

	if (readRFIDindex(BANKx,addr) == 0x07)
	{		
		if( BANKx <= 6 )
		{							
			RFID_Update_EEPROMB07(1);
		}
		else if( BANKx == 7 )
		{
			RFID_Update_EEPROMB07(2);
		}
	}
	

	if ((BANKx&0x01)==0)
	{
		RFID_eFlashA0 = (addr&0x7F);					
	}
	else
	{
		RFID_eFlashA0 = (addr|0x80); 					
	}
	nTemp = readRFIDindex(BANKx,addr);

	if( (BANKx <= 5) )
	{
     	if( nTemp == 0x0F ) {
						
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N2),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index),RFID_eFlashA0,0x07);
    	} else {
    				
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N1),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index),RFID_eFlashA0,0x0F);
    	}
	}
	else if ( BANKx == 6 )
	{
		if(nTemp == 0x0F)
		{										
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N5),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index),RFID_eFlashA0,0x07);
    	} else {
    										
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N4),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index),RFID_eFlashA0,0x0F);
    	} 
	}
	else if ( BANKx == 7 )
	{
		if(nTemp == 0x0F)
		{										
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N7),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index2),RFID_eFlashA0,0x07);
    	} else {
    										
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_N6),RFID_eFlashA0,nData);
			Write_Eflash_1Byte(RFID_EEPROMA2,((BANKx >> 1)|RFID_EEPROMA1_index2),RFID_eFlashA0,0x0F);
    	} 
	}
	

	EnableAllInterrupt();
}

void RFID_Update_EEPROMB07(BYTE region)
{

	BYTE tIndex, t1EflashA1, t2EflashA1, t3EflashA1;
#if WDT_Support
	ResetInternalWDT();
#endif

	if( region == 1 )
	{
		tIndex = RFID_EEPROMA1_index;
		t1EflashA1 = RFID_EEPROMA1_N1;
		t2EflashA1 = RFID_EEPROMA1_RFID1;
		t3EflashA1 = RFID_EEPROMA1_N2;
	}
	else if (region == 2)
	{
		tIndex = RFID_EEPROMA1_index2;
		t1EflashA1 = RFID_EEPROMA1_N6;
		t2EflashA1 = RFID_EEPROMA1_RFID3;
		t3EflashA1 = RFID_EEPROMA1_N7;
	}


	for(RFID_eFlashA1=0x00;RFID_eFlashA1<4;RFID_eFlashA1++)	
	{
		e256ByteCnt=0;
		do
		{
			RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|tIndex),e256ByteCnt);
			if (RFID_eEEPROMData != 0x07)
			{
				if( (RFID_eFlashA1 == 0x03) && ( e256ByteCnt <= 0x7F) && (region == 1) )
				{
					if (RFID_eEEPROMData == 0x0F)
					{
						RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_N4),e256ByteCnt);
					}
					else 
					{
						RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_RFID2),e256ByteCnt);
					}
					Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_N5),e256ByteCnt,RFID_eEEPROMData);
					Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_index),e256ByteCnt,0x07);
				}
				else
				{
					if (RFID_eEEPROMData == 0x0F)
					{
						RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|t1EflashA1),e256ByteCnt);
					}
					else
					{
						RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|t2EflashA1),e256ByteCnt);
					}
					Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|t3EflashA1),e256ByteCnt,RFID_eEEPROMData);
					Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|tIndex),e256ByteCnt,0x07);
				}
			}
			e256ByteCnt++;
		}while(e256ByteCnt!=0x00);								
	}


	Erase_Eflash_1K(RFID_EEPROMA2,t2EflashA1,0x00);
	
	if( region == 1 )
	{
		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_RFID2,0x00);
	}
		
	for(RFID_eFlashA1=0x00;RFID_eFlashA1<4;RFID_eFlashA1++)	
	{
		e256ByteCnt=0;
		do
		{
			if( (RFID_eFlashA1 == 0x03) && ( e256ByteCnt <= 0x7F) && (region == 1) )
			{
				RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_N5),e256ByteCnt);
				Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|RFID_EEPROMA1_RFID2),e256ByteCnt,RFID_eEEPROMData);
			}
			else
			{
				RFID_eEEPROMData = Read_Eflash_Byte(RFID_EEPROMA2,(RFID_eFlashA1|t3EflashA1),e256ByteCnt);
				Write_Eflash_1Byte(RFID_EEPROMA2,(RFID_eFlashA1|t2EflashA1),e256ByteCnt,RFID_eEEPROMData);
			}
			e256ByteCnt++;

		}while(e256ByteCnt!=0x00);						
	}

	if( region == 1)
	{
		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N4,0x00);
		Write_Eflash_1Byte(RFID_EEPROMA2,(0x03|tIndex),0xFF,0x03);
		Erase_Eflash_1K(RFID_EEPROMA2,RFID_EEPROMA1_N5,0x00);
	}
	Erase_Eflash_1K(RFID_EEPROMA2,t1EflashA1,0x00);
	Write_Eflash_1Byte(RFID_EEPROMA2,(0x03|tIndex),0xFF,0x01);
  Erase_Eflash_1K(RFID_EEPROMA2,t3EflashA1,0x00);

	Erase_Eflash_1K(RFID_EEPROMA2,tIndex,0x00);
}

void writeRFIDProtect(BYTE BANKx,BYTE acc)
{
    BYTE nRTempL,nRTempH,nWTempL,nWTempH,nCRTempL,nCRTempH,nCWTempL,nCWTempH,
		nTRTempL,nTRTempH,nTWTempL,nTWTempH;

	BYTE dRTempH,dWTempH,dCRTempH,dCWTempH,dTRTempH,dTWTempH;
	
	BYTE rByte=0xFF,rByte1=0xFF,rByte2=0xFF,tCtrl=0,tCtrl1=0;
	BYTE wByte=0xFF,wByte1=0xFF,wByte2=0xFF;
  	BYTE rMark;


	DisableAllInterrupt();


	if( BANKx == 0x06 )
	{
		nRTempL = (BYTE)(RFID_EEPROM_RN4);
		nWTempL = (BYTE)(RFID_EEPROM_WN4);
		nRTempH = (BYTE)(RFID_EEPROM_RN4>>8);
		nWTempH = (BYTE)(RFID_EEPROM_WN4>>8);		
		nCRTempL = (BYTE)(RFID_EEPROM_RN0);
		nCWTempL = (BYTE)(RFID_EEPROM_WN0);
		nCRTempH = (BYTE)(RFID_EEPROM_RN0>>8);
		nCWTempH = (BYTE)(RFID_EEPROM_WN0>>8);		
		nTRTempL = (BYTE)(RFID_EEPROM_RN6);
		nTWTempL = (BYTE)(RFID_EEPROM_WN6);
		nTRTempH = (BYTE)(RFID_EEPROM_RN6>>8);
		nTWTempH = (BYTE)(RFID_EEPROM_WN6>>8);

		dRTempH = (BYTE)(dRFID_EEPROM_RN4>>8);
		dWTempH = (BYTE)(dRFID_EEPROM_WN4>>8);
		dCRTempH = (BYTE)(dRFID_EEPROM_RN0>>8);
		dCWTempH = (BYTE)(dRFID_EEPROM_WN0>>8);
		dTRTempH = (BYTE)(dRFID_EEPROM_RN6>>8);
		dTWTempH = (BYTE)(dRFID_EEPROM_WN6>>8);
	}
	else if( BANKx == 0x07 )
	{
		nRTempL = (BYTE)(RFID_EEPROM_RN6);
		nWTempL = (BYTE)(RFID_EEPROM_WN6);
		nRTempH = (BYTE)(RFID_EEPROM_RN6>>8);
		nWTempH = (BYTE)(RFID_EEPROM_WN6>>8);
		nCRTempL = (BYTE)(RFID_EEPROM_RN0);
		nCWTempL = (BYTE)(RFID_EEPROM_WN0);
		nCRTempH = (BYTE)(RFID_EEPROM_RN0>>8);
		nCWTempH = (BYTE)(RFID_EEPROM_WN0>>8);
		nTRTempL = (BYTE)(RFID_EEPROM_RN4);
		nTWTempL = (BYTE)(RFID_EEPROM_WN4);
		nTRTempH = (BYTE)(RFID_EEPROM_RN4>>8);
		nTWTempH = (BYTE)(RFID_EEPROM_WN4>>8);

		dRTempH = (BYTE)(dRFID_EEPROM_RN6>>8);
		dWTempH = (BYTE)(dRFID_EEPROM_WN6>>8);
		dCRTempH = (BYTE)(dRFID_EEPROM_RN0>>8);
		dCWTempH = (BYTE)(dRFID_EEPROM_WN0>>8);
		dTRTempH = (BYTE)(dRFID_EEPROM_RN4>>8);
		dTWTempH = (BYTE)(dRFID_EEPROM_WN4>>8);
	}
	else
	{
		nRTempL = (BYTE)(RFID_EEPROM_RN0);
		nWTempL = (BYTE)(RFID_EEPROM_WN0);
		nRTempH = (BYTE)(RFID_EEPROM_RN0>>8);
		nWTempH = (BYTE)(RFID_EEPROM_WN0>>8);
		nCRTempL = (BYTE)(RFID_EEPROM_RN4);
		nCWTempL = (BYTE)(RFID_EEPROM_WN4);
		nCRTempH = (BYTE)(RFID_EEPROM_RN4>>8);
		nCWTempH = (BYTE)(RFID_EEPROM_WN4>>8);
		nTRTempL = (BYTE)(RFID_EEPROM_RN6);
		nTWTempL = (BYTE)(RFID_EEPROM_WN6);
		nTRTempH = (BYTE)(RFID_EEPROM_RN6>>8);
		nTWTempH = (BYTE)(RFID_EEPROM_WN6>>8);

		dRTempH = (BYTE)(dRFID_EEPROM_RN0>>8);
		dWTempH = (BYTE)(dRFID_EEPROM_WN0>>8);
		dCRTempH = (BYTE)(dRFID_EEPROM_RN4>>8);
		dCWTempH = (BYTE)(dRFID_EEPROM_WN4>>8);
		dTRTempH = (BYTE)(dRFID_EEPROM_RN6>>8);
		dTWTempH = (BYTE)(dRFID_EEPROM_WN6>>8);
	}

	rByte = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nRTempH));

	wByte = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nWTempH));
	

	if( acc&0x01 )
	{
		if( wByte&nWTempL )
		{
			tCtrl|=0x01;
		}
	}
	else
	{
		if( !(wByte&nWTempL) )
		{
			tCtrl|=0x10;
		}
	}

	if( acc&0x02 )
	{
		if( rByte&nRTempL )
		{
			tCtrl|=0x02;
		}
	}
	else
	{
		if( !(rByte&nRTempL) )
		{
			tCtrl|=0x20;
		}
	}

	rByte1 = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nCRTempH));

	wByte1 = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nCWTempH));

	rByte2 = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nTRTempH));
	wByte2 = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,(nTWTempH));

	if( tCtrl&0x30 )
	{	
		if( !(wByte1&nCWTempL) )
		{
			tCtrl |= 0x04;
		}

		if( !(rByte1&nCRTempL) )
		{
			tCtrl |= 0x08;
		}

		if( !(wByte2&nTWTempL) )
		{
			tCtrl1 |= 0x01;
		}

		if( !(rByte2&nTRTempL) )
		{
			tCtrl1 |= 0x02;
		}

    rMark = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK);

		Erase_Eflash_1K(RFID_PROCTA2,RFID_PROCTA1,0x00);
		
    Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK,rMark);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFC,0xA5);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFD,0xA5);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFE,0x5A);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFF,0x5A);

		tCtrl |= 0x80;
				
	}

	if( BANKx == 0x06 )
	{
		
		if( tCtrl )
		{
			wByte |= (BYTE)(RFID_EEPROM_RN4 | RFID_EEPROM_RN5);
			wByte1 |= (BYTE)(RFID_EEPROM_RN3);

			rByte |= (BYTE)(RFID_EEPROM_WN4 | RFID_EEPROM_WN5);
			rByte1 |= (BYTE)(RFID_EEPROM_WN3);
		}
		
		if( tCtrl&0x01 )	
		{
			wByte &= ((BYTE)(RFID_EEPROM_WN4 | RFID_EEPROM_WN5)^0xFF);
			wByte1 &= (BYTE)(RFID_EEPROM_WN3 ^ 0xFF);
		}
		else if( tCtrl&0x10 ) 
		{
			wByte |= (BYTE)(RFID_EEPROM_WN4 | RFID_EEPROM_WN5);
			wByte1 |= (BYTE)(RFID_EEPROM_WN3);
		}
		
		if( tCtrl&0x02 )
		{
			rByte &= ((BYTE)(RFID_EEPROM_RN4 | RFID_EEPROM_RN5)^0xFF);
			rByte1 &= (BYTE)(RFID_EEPROM_RN3 ^ 0xFF);
		}
		else if( tCtrl&0x20 )
		{
			rByte |= (BYTE)(RFID_EEPROM_RN4 | RFID_EEPROM_RN5);
			rByte1 |= (BYTE)(RFID_EEPROM_RN3);
		}
	
		
		
	}
	else if( BANKx == 0x07 )
	{
		if( tCtrl )
		{
			rByte |= (BYTE)(RFID_EEPROM_WN6);

			wByte |= (BYTE)(RFID_EEPROM_RN6);
		}
		
		if( tCtrl&0x01 )	
		{
			wByte &= (BYTE)(RFID_EEPROM_WN6^0xFF);
		}
		else if( tCtrl&0x10 ) 
		{
			wByte |= (BYTE)(RFID_EEPROM_WN6);
		}
		
		if( tCtrl&0x02 )
		{
			rByte &= (BYTE)(RFID_EEPROM_RN6^0xFF);
		}
		else if( tCtrl&0x20 )
		{
			rByte |= (BYTE)(RFID_EEPROM_RN6);
		}
	}
	else if( (BANKx >= 0x00) && (BANKx <= 0x05) )
	{

		if( tCtrl )
		{
			wByte |= (BYTE)(RFID_EEPROM_RN0 | RFID_EEPROM_RN1 | RFID_EEPROM_RN2);

			rByte |= (BYTE)(RFID_EEPROM_WN0 | RFID_EEPROM_WN1 | RFID_EEPROM_WN2);
		}
		
		if( tCtrl&0x01 )	
		{
			wByte &= ((BYTE)(RFID_EEPROM_WN0 | RFID_EEPROM_WN1 | RFID_EEPROM_WN2)^(0xFF));
		}
		else if( tCtrl&0x10 ) 
		{
			wByte |= (BYTE)(RFID_EEPROM_WN0 | RFID_EEPROM_WN1 | RFID_EEPROM_WN2);
		}
		
		if( tCtrl&0x02 )
		{
			rByte &= ((BYTE)(RFID_EEPROM_RN0 | RFID_EEPROM_RN1 | RFID_EEPROM_RN2)^(0xFF));
		}
		else if( tCtrl&0x20 )
		{
			rByte |= (BYTE)(RFID_EEPROM_RN0 | RFID_EEPROM_RN1 | RFID_EEPROM_RN2);
		}
		
		
	}

	
	if( nCRTempH == nCWTempH )
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nCRTempH,rByte1&wByte1);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dCRTempH,rByte1&wByte1);
	}
	else
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nCRTempH,rByte1);	
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nCWTempH,wByte1);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dCRTempH,rByte1);	
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dCWTempH,wByte1);
		
	}

	if( nTRTempH == nTWTempH )
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nTRTempH,rByte2&wByte2);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dTRTempH,rByte2&wByte2);
	}
	else
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nTRTempH,rByte2);	
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nTWTempH,wByte2);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dTRTempH,rByte2);	
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dTWTempH,wByte2);

	}

	if( nRTempH == nWTempH )
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nRTempH,rByte&wByte);

		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dRTempH,rByte&wByte);
	}
	else
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nRTempH,rByte);	
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,nWTempH,wByte);

		
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dRTempH,rByte);			
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,dWTempH,wByte);

	}

	
	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xF1,0x00);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFA,0xFE);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xF8,0xFE);


	EnableAllInterrupt();
}

void unlockRFIDProtect(void)
{
	BYTE i,rMark;
	
	
	DisableAllInterrupt();

	
	
	for( i=0; i < 8;i++)
	{
		rACCPAGE[i] = 0x00;
	}

  rMark = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK);

	Erase_Eflash_1K(RFID_PROCTA2,RFID_PROCTA1,0x00);
	
  Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK,rMark);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xF1,0x00);
	
	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFC,0xA5);
	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFD,0xA5);
	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFE,0x5A);
	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFF,0x5A);


	EnableAllInterrupt();
}

void initRFIDProtect(void)
{
	BYTE rMark,i;

	DisableAllInterrupt();

	rMark = Read_Eflash_Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK);
	
	if( rMark != 0x78 )
	{
		Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,INITMARK,0x78);
		for( i = 0;i < 128;i++)
		{
			writeRFIDEEprom(0x06,i,0x00);     // bank6 init to be 0x00
		}
		
		for( i = 0;i < 128;i++)
		{
			writeRFIDEEprom(0x07,i,0x00);     // bank7 init to be 0x00
		}
		DisableAllInterrupt();
	}

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xF1,0x00);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFC,0xA5);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFD,0xA5);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFE,0x5A);

	Write_Eflash_1Byte(RFID_PROCTA2,RFID_PROCTA1,0xFF,0x5A);

	
	EnableAllInterrupt();
}






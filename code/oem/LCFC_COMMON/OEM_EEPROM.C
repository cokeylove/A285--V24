/*-----------------------------------------------------------------------------
 * TITLE: OEM_EEPROM.C
 * TITLE: OEM_EEPROM.H
 *
 * Author : Ivan Chen
 *
 * spec : Read/Write EC EEPROM
*---------------------------------------------------------------------------*/
// Include all header file
#include "..\..\include.h"



void Chk_pDevStus(void)
{
    BYTE Temp8;
    if(IS_MASK_CLEAR(cSPIEEPROM,cSPI_ROMCopyStart))
    {
        eEEPROMData=Read_Eflash_Byte(EEPROMA2,EEPROM_Bank7,EEPROM_DevStus);
        if (((eEEPROMData & EEPROM_DevStus_USB) !=  (u4PeriCtrl3 & USB_PWR))
        	||(IS_MASK_SET(eEEPROMData,EEPROM_DevStus_AOU_DC_Dis) !=  USBALWONGet))
        {
          eUpdatepDevStusCnt++;
          Temp8 = u4PeriCtrl3 & USB_PWR;
          Temp8 |= USBALWONGet << 1;
          WRITE_DATA2EEPROM(&Temp8, 1, EEPROM_Bank7, EEPROM_DevStus); 
        }
   }
}

#if SYSTEM_ENABLE_G3_WAKEUP_LAN
//A285D00041:Fix sysem cannot be WoLAN from S5 (G3 then plugin AC) with magic packet
void Chk_WOLStatus(void)
{
    BYTE Temp8 = 0x00;
    if(IS_MASK_CLEAR(cSPIEEPROM,cSPI_ROMCopyStart))
    {
        eEEPROMMarkData = Read_Eflash_Byte(EEPROMA2, EEPROM_Bank7, EEPROM_WOLStatus);
        if(WOLACWakeEnGet)
        {
            Temp8 |= EEPROM_WOLACEn;
        }

        if(WOLDCWakeEnGet)
        {
            Temp8 |= EEPROM_WOLDCEn;
        }

        if((0xFF == eEEPROMMarkData) || ((eEEPROMMarkData & (BIT0 | BIT1)) != Temp8))
        {
            eUpdatepDevStusCnt++;

            eEEPROMMarkData = Temp8;
            
            WRITE_DATA2EEPROM(&Temp8, 1, EEPROM_Bank7, EEPROM_WOLStatus); 
        }
   }
}
#endif

// for command use
void Update_EEPROMMark(void)
{
  WRITE_DATA2EEPROM(&eEEPROMData, 1, eEEPROMBank, eEEPROMAddrsss);
}

void WRITE_DATA2EEPROM(UINT8* ram_pntr, WORD size, BYTE ROMAddress, WORD offset)
{
  BYTE Temp8;
  UINT8 *Temp_ram_pntr = ram_pntr;
  WORD Temp_size = size;
  WORD Temp_offest = offset;
//  CacheDma(1,FSCEHighLevel);
  DisableAllInterrupt();
  SET_MASK(cSPIEEPROM,cSPI_ROMCopyStart);

//write data to EEPROM
  do
  {
//write EEPROM 
    Temp8 = *Temp_ram_pntr;
    Write_Eflash_1Byte(EEPROMA2,(ROMAddress + (Temp_offest >> 8)),Temp_offest,Temp8);
    if ( Temp8 != (Read_Eflash_Byte(EEPROMA2,(ROMAddress + (Temp_offest >> 8)),Temp_offest))){
      //*************************************           
      // Write Fail !!! need update EEPROM
      //*************************************
       UPDATA_EEPROM_DATA(ram_pntr, size, ROMAddress, offset);    
       break;
     } 
     Temp_offest++;
     Temp_ram_pntr++;
     Temp_size--;
  }while(Temp_size!=0x00);

       
//  Update_EEPROMB07(); 
#if(uMBID_LoadRom)
  Update_ECEEPROM();
#else
  uMBID = Current_MBID;
#endif
  CLEAR_MASK(cSPIEEPROM,cSPI_ROMCopyStart);
  SCRA2H = 3; //disable SCRA2H
  EnableAllInterrupt();
}

void UPDATA_EEPROM_DATA(UINT8* ram_pntr, WORD size, BYTE ROMAddress, WORD offset)
{
   BYTE i, B256cnt, Temp8;
   WORD TempADD, Updateadd;
   Updateadd = ((ROMAddress & 0x3) << 8) + offset;
 //  TempADD = ROMAddress & 0xFC; 
   Erase_Eflash_1K(EEPROMA2,EEPROM_Temp_Swith,0x00);   

   for(i=0x00;i<4;i++)  // 256 bytes *4   //here has issue  256 * 4 need update oence eeprom
   {
     B256cnt = 0;
     do
     {
       TempADD = i;
       TempADD = TempADD << 8;
       TempADD += B256cnt;
       if ((TempADD < Updateadd) || (TempADD > (Updateadd + size -1))){      //Don't update  
         Temp8 = Read_Eflash_Byte(EEPROMA2,((ROMAddress & 0xFC) + i),B256cnt);
       } else {                         //need update data address
         Temp8 = *ram_pntr;
         ram_pntr ++;
       }     
       Write_Eflash_1Byte(EEPROMA2,(EEPROM_Temp_Swith+i),B256cnt,Temp8);          
       B256cnt++;
       offset ++;
     }while(B256cnt!=0x00);                // To check 256 bytes
   }

//update temp data to eeprom   
  TempADD = ROMAddress & 0xFC; 
  Erase_Eflash_1K(EEPROMA2,TempADD,0x00);
  for(i=0x00;i<4;i++)          // 256 bytes *4
  {
    B256cnt=0;
    do
    {
      Temp8 = Read_Eflash_Byte(EEPROMA2,(EEPROM_Temp_Swith+i),B256cnt);
      Write_Eflash_1Byte(EEPROMA2,(TempADD+i),B256cnt,Temp8);
      B256cnt++;
    }while(B256cnt!=0x00);                // To check 256 bytes
  }   
}

void Update_16ByteEEPROM(void)
{
  BYTE i,j, Temp8;

//  CacheDma(1,FSCEHighLevel);
  DisableAllInterrupt();

  if (Read_Eflash_Byte(EEPROMA2,EEPROMM1_16Byte_end ,0xF0) != 0xFF)
  {
  
    Erase_Eflash_1K(EEPROMA2,EEPROMA1_16Byte,0x00);
  }

  for (i=0; i<4 ;i++) // 01C800 ~ 01CC00
  {
    for (j=0; j< 0x10 ;j++)
    {
      Temp8 = Read_Eflash_Byte(EEPROMA2,(EEPROMA1_16Byte + i),(j*16));
      if(Temp8 == 0xFF)
        break;
    }
    if (Temp8 == 0xFF)
      break;
  }

  Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16),j+80);
  Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+4,pLastSID);

  if ((pLastSID&0x7F) == THERMALCOMMFAIL_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+6,TMErrCnt);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+7,TmlICStep);
  }
  
  if (pLastSID == DTSOVERTEMP_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+6,TEMP_Buff_1);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+7,TEMP_Buff_2);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,TEMP_Buff_3);
  }

  if (pLastSID == VGAOVERTEMP_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,VGA_TBuff1);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+9,VGA_TBuff2);  
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0A,VGA_TBuff3);

    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0C,SMB3_DATA[0]);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0D,SMB3_DATA[1]);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0E,SMB3_DATA[2]);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0F,SMB3_DATA[3]);
  }

  if (pLastSID == AC_CUTOFF_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,(ADPI_AvgData>>8)); //MSB
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+9,(ADPI_AvgData&0xFF));  //LSB
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0A,ACACOFFEn);  
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0B,AdpCUTCnt);    

    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0C,(ADPI_Data[0]>>8)); //MSB
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0D,(ADPI_Data[0]&0xFF)); //LSB
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0E,(ADPI_Data[1]>>8)); //MSB
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0F,(ADPI_Data[1]&0xFF)); //LSB
  }

  if ((pLastSID&0x7F) == AC_WRONG_ADP_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,Chk_Wrong_ADP_Status);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+9,Chk_Wrong_10ms_Count);  
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0A,ACIN_FallINT_Count);    
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0B,AC_DROP_COUNT);
  }

  if (pLastSID == BATTOVERTEMP_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,SYS_STATUS);
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+9,nBattAverTemp);  
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+0x0A,M_BATAlarm);        
  }

  if (pLastSID == BIOS_ID) {
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+8,(BIOS_SID_REC >> 8 ));   //MSB 
    Write_Eflash_1Byte(EEPROMA2,(EEPROMA1_16Byte+i),(j*16)+9,(BIOS_SID_REC & 0xFF )); //LSB 
  }  
//  Write_Eflash_1Byte(EEPROMA2,EEPROMM1_16Byte_end,0xF0,0xAA);
    SCRA2H = 3; //disable SCRA2H
    EnableAllInterrupt();
}

void Erase_EEPROMAll(void)
{
//  CacheDma(1,FSCEHighLevel);
  DisableAllInterrupt();
//  CacheDma(1,FSCEHighLevel);
  Erase_Eflash_1K(EEPROMA2,EEPROM_Bank0,0x00);
  Erase_Eflash_1K(EEPROMA2,EEPROM_Bank4,0x00);
  SCRA2H = 3; //disable SCRA2H
  EnableAllInterrupt();
}

#if(uMBID_LoadRom)

void Update_ECEEPROM(void)
{
  uMBID = Read_Eflash_Byte(EEPROMA2,(EEPROM_Bank7),EEPROM_MBID);
}

#endif
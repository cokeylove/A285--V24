/*-----------------------------------------------------------------------------
 * TITLE: OEM_Edge.C
 *
 * Author : ITE
 *
 * Note : These functions are reference only.
 *        Please follow your project software specification to do some modification.
 *---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************




void AttentionPlease(BYTE AttentionCode)
{
  WORD temp32;
  // Attention code can be assigned from 1 to 126, 0 and 127 are reserved for future.
  if (AttentionCode < 1 || AttentionCode > 126)
    return;

//  SET_MASK((AttentionOffset + (AttentionCode/8)),BIT(AttentionCode&0x07));
  temp32 = AttentionOffset + (AttentionCode/8); 
  SET_MASK(temp32,BIT(AttentionCode&0x07));

  return;
}



void Edge_Initial(void)
{
     WORD time;
     u4BuildDate2 = BUILD_DATE;
     u4BuildDate2 = u4BuildDate2 | (BUILD_MONTH << 5);
     u4BuildDate1 = BUILD_MONTH >> 3;
     u4BuildDate1 = u4BuildDate1 | (BUILD_YEAR << 1);
     time = (BUILD_HOURS * 3600) + (BUILD_MINUTES * 60) + BUILD_SECONDS;
     time = time >> 1; //Time = Time/2
     u4BuildTime2 = time & 0x00FF;
     u4BuildTime1 = (time & 0xFF00) >> 8;

    // CMW 20121031 for interface space +
    u4VersionH = REV0_BYTE0<<4;
    u4VersionH |= (REV0_BYTE1&0xF0)>>4;
    u4VersionL = (REV0_BYTE1&0x0F)<<4;
    u4VersionT = REV0_BYTE3;
    if( REV0_BYTE2 == 0x00 )
    {
      u4VersionL &= 0xF0;
    }
    else
    {
      u4VersionL |= ((REV0_BYTE2 - 'A') + 1);
    }
    u4BuildID1 = OEM_BuildID1;
    u4BuildID2 = OEM_BuildID2;
    u4BuildID3 = OEM_BuildID3;
    u4BuildID4 = OEM_BuildID4;
    u4BuildID5 = OEM_BuildID5;
    u4BuildID6 = OEM_BuildID6;
    u4BuildID7 = OEM_BuildID7;
    u4BuildID8 = OEM_BuildID8;

//Specification version
    u4SpecversionL =  (CBYTE)OEM_SPECVERSION;
    u4SpecversionH =  OEM_SPECVERSION >> 8;


}

void EdgePasswordProcess(void)
{
  if( PassCtrlBusyGET )
  {
    edgePasswordID = u4PassCtrl&0x0F;
    if( edgePasswordID >= PWDNUM)
    {
      u4PassData1 = 0;
      u4PassData2 = 0;
      u4PassData3 = 0;
      u4PassData4 = 0;
      u4PassData5 = 0;
      u4PassData6 = 0;
      u4PassData7 = 0;
      u4PassData8 = 0;
      PassCtrlBusyDIS;
      return;
    }
    if( (u4PassCtrl & 0xF0) == 0x80)  // Read
    {
      //edgePasswordID = u4PassCtrl&0x0F;
      u4PassData1 = edgePassword[0+(edgePasswordID*8)];
      u4PassData2 = edgePassword[1+(edgePasswordID*8)];
      u4PassData3 = edgePassword[2+(edgePasswordID*8)];
      u4PassData4 = edgePassword[3+(edgePasswordID*8)];
      u4PassData5 = edgePassword[4+(edgePasswordID*8)];
      u4PassData6 = edgePassword[5+(edgePasswordID*8)];
      u4PassData7 = edgePassword[6+(edgePasswordID*8)];
      u4PassData8 = edgePassword[7+(edgePasswordID*8)];
    }
    else if( (u4PassCtrl & 0xF0) == 0xC0)  // Write
    {
      edgePassword[0+(edgePasswordID*8)] = u4PassData1;
      edgePassword[1+(edgePasswordID*8)] = u4PassData2;
      edgePassword[2+(edgePasswordID*8)] = u4PassData3;
      edgePassword[3+(edgePasswordID*8)] = u4PassData4;
      edgePassword[4+(edgePasswordID*8)] = u4PassData5;
      edgePassword[5+(edgePasswordID*8)] = u4PassData6;
      edgePassword[6+(edgePasswordID*8)] = u4PassData7;
      edgePassword[7+(edgePasswordID*8)] = u4PassData8;
      SET_MASK(edgePasswordID,ePWDIDSet);
      switch(u4PassCtrl & 0x0F)
      {
        case 0x02:
          u4PasswordSts1 |= 0x04;
          break;
        case 0x03:
          u4PasswordSts1 |= 0x08;
          break;
        case 0x04:
          u4PasswordSts1 |= 0x10;
          break;
        case 0x05:
          u4PasswordSts1 |= 0x20;
          break;
        case 0x06:
          u4PasswordSts1 |= 0x40;
          break;
        case 0x07:
          u4PasswordSts1 |= 0x80;
          break;
        case 0x08:
          u4PasswordSts2 |= 0x01;
          break;
        case 0x09:
          u4PasswordSts2 |= 0x02;
          break;
        case 0x0A:
          u4PasswordSts2 |= 0x04;
          break;
        case 0x0B:
          u4PasswordSts2 |= 0x05;
          break;
        case 0x0C:
          u4PasswordSts2 |= 0x10;
          break;
        case 0x0D:
          u4PasswordSts2 |= 0x20;
          break;
        case 0x0E:
          u4PasswordSts2 |= 0x40;
          break;
        default:
          break;
      }
      u4PassData1 = 0;
      u4PassData2 = 0;
      u4PassData3 = 0;
      u4PassData4 = 0;
      u4PassData5 = 0;
      u4PassData6 = 0;
      u4PassData7 = 0;
      u4PassData8 = 0;
    }

    PassCtrlBusyDIS;
  }
}

void PowerMonitor(void)
{
  if( ACPOWER_ON && SystemIsS0)
  {
    if( uAdpWattCnt < 10 )  // 0.1 sec average
    {
      uAdpWattCnt++;
      //uAdpWattTemp = ADPI_AvgData;   //uAdpWattTemp += cADC_TableH[CHANNEL2];
    }
    else
    {   //(Reference)#define A90WCP    256 //(((75/20)/IOUT)/(3/1024))    //75W -> 3.75A
        //ADPI_AvgData * (3 / 1024) * 100 * 10  //((75/20)/IOUT)-> 0.75V    //100: 0.75V stands for 75W //10: unit = 100mW = 0.1W
      //uAdpWattTemp = ( uAdpWattTemp * 3 * 100 * 10 / 1024 ) / 100;
            //uAdpWattTemp = ( uAdpWattTemp * 3 * 100 * 10 / 1024 ) /10;
      uAdpWattTemp = (WORD)((DWORD)ADPI_AvgData * 3000 >> 10);
            //uAdpWattTemp = uAdpWattTemp / 200 * 10;  // Due to Power Manager unit is 100mW. 1W=10mW.
      //uAdpWattTemp = ( (DWORD)uAdpWattTemp * ADPPowerKP * 33 / 2550 );
      u4AC_PwrL = (uAdpWattTemp & 0x00ff);
      u4AC_PwrH = uAdpWattTemp >> 8;
      uAdpWattCnt = 0;
      uAdpWattTemp = 0;
    }
  }
  else
  {
    u4AC_PwrL = 0;
    u4AC_PwrH = 0;
    uAdpWattCnt = 0;
    uAdpWattTemp = 0;
  }
  // A.2.21 Power Montior End
}

//(JSN)-

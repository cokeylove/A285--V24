/*-----------------------------------------------------------------------------
 * TITLE: OEM_BottomTamper.C
 *
 * Author : Ivan Chen
 *
 * Note : This is Dcover open check function. 
 *
 *---------------------------------------------------------------------------*/

//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//
//*****************************************************************************

#if Support_BOTTOM_TAMPER_FUNC

void DCoverOpened (void)
{
  SET_MASK(TamperSTS, BottomTamper);
}


void ClearBottomTamper(BYTE data)
{
   if ((data & BottomTamper) && !Read_D_Cover_SW()){
     CLEAR_MASK(TamperSTS, BottomTamper);
   }    
}

void SetBottomTamper_Wake(void)
{
  SET_MASK(IELMR5,Int_WKO53);
  SET_MASK(WUEMR5,WUC_WUI11);
  CLEAR_MASK(IPOLR5,Int_WKO53);        // falling
  SET_MASK(WUESR5, WUC_WUI11);
  SET_MASK(ISR5, Int_WKO53); 
  SET_MASK(IER5, Int_WKO53);       
}


void BottomTamper_Wake(void)
{
   SET_MASK(WUEMR5,WUC_WUI11);
   SET_MASK(WUESR5, WUC_WUI11);
   SET_MASK(ISR5, Int_WKO53); 
   CLEAR_MASK(IER5, Int_WKO53);      
   DCoverOpened();
}

void CheckBottomTamper (void)
{
  if (BottomTamper_ram0 != BottomTamper_signature0 ||
      BottomTamper_ram1 != BottomTamper_signature1 ||
      BottomTamper_ram2 != BottomTamper_signature2 ||
      BottomTamper_ram3 != BottomTamper_signature3) 
  {
    SET_MASK(TamperSTS, BottomTamper);
    BottomTamper_ram0 = BottomTamper_signature0;
    BottomTamper_ram1 = BottomTamper_signature1;
    BottomTamper_ram2 = BottomTamper_signature2;
    BottomTamper_ram3 = BottomTamper_signature3;
  }
}
#endif //Support_BottomTamper


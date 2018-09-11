/*-----------------------------------------------------------------------------
 * TITLE: OEM_BottomTamper.H
 *
 * Author : Ivan Chen
 *
 * Note : 
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_BottomTamper_H
#define OEM_BottomTamper_H

/**********************************************************
*     #EC ram define
**********************************************************/
#define BottomTamper_ram0         BRAM10
#define BottomTamper_ram1         BRAM11
#define BottomTamper_ram2         BRAM12
#define BottomTamper_ram3         BRAM13


#define BottomTamper_signature0   0x5A
#define BottomTamper_signature1   0x5A
#define BottomTamper_signature2   0xA5
#define BottomTamper_signature3   0xA5


extern void DCoverOpened(void);
extern void CheckBottomTamper (void);
extern void ClearBottomTamper(BYTE data);
extern void SetBottomTamper_Wake(void);
extern void BottomTamper_Wake(void);

#endif   //OEM_BottomTamper_H


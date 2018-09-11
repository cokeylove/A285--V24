/*-----------------------------------------------------------------------------
 * TITLE: OEM_Tamper.H
 *
 * Author : Ivan Chen
 *
 * Note : 
 *
 *---------------------------------------------------------------------------*/

#ifndef OEM_Tamper_H
#define OEM_Tamper_H



/**********************************************************
*     #EC ram define
**********************************************************/
#define  Tamper_status      Tamper_ram0           
#define  Tamper_Dev_Ste     Tamper_ram1
// BIT7~4 last status
// BIT0~3 current status
 #define  Tamper_BayHDD    BIT0
 #define  Tamper_WWAN      BIT0 
 #define  Tamper_SSD       BIT1
 #define  Tamper_HDD       BIT2
 #define  Tamper_DEBOUNCE  BIT3
#define  Tamper_Dev_temp     Tamper_ram2
#define  Device_detect       Tamper_ram3

#define  Device_detect_support    BIT6 + BIT5 + BIT4 + BIT2 + BIT1 + BIT0

#define Tamper_ADC    1   //detect by ADC
#define Tamper_GPIO   2   //detect by GPIO

extern void Tamper_DET(void);
                                 //DAx75  //CEx7x
#define TamperADC_LV0      983   //983    //996
#define TamperADC_LV1      904   //904    //923
#define TamperADC_LV2      818   //818    //831
#define TamperADC_LV3      723   //723    //744
#define TamperADC_LV4      641   //641    //671
#define TamperADC_LV5      589   //589    //612
#define TamperADC_LV6      542   //542    //572
#define Tamper_detect_tolerance 8

#endif

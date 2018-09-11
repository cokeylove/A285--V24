
//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//*****************************************************************************


#define ORIGA_ECCSTATIC_TEST
//#pragma ot(9,speed)
//void display_char(unsigned char i);
#define display_char(x)
#if Thinkpad_Battery_HW_Auth
/** square root of the parameter b of the
  * elliptic curve E(a,b) defined by the equation
  * y^2 + xy = x^3 + ax^2 + b,  a = 0
  */
//MP Key
gf2n_t g_gf2n_sqrt_coeff_b = { 0x055eu, 0x1b80u, 0x5a9bu, 0xe2edu, 0xf787u, 0x1dcfu, 0x8a70u, 0x3d07u, 0x0000u }; // para1

/* affine x-coordinate of the base point P = (xP, yP) */
gf2n_t g_gf2n_xP = { 0x601fu, 0xd285u, 0xb6f5u, 0x4ec4u, 0x2fd3u, 0x5025u, 0xa61bu, 0x0720u, 0x0006u }; // para1

/* public key */
gf2n_t g_gf2nPublicKey = { 0x3B81u, 0x45A4u, 0xC5CBu, 0xA73Eu, 0xE233u, 0xF065u, 0x0D86u, 0xA9FAu, 0x0u };

//Dummy Key
//gf2n_t g_gf2n_sqrt_coeff_b = {0x33D9u,0x2C36u, 0xA5F8u,0xE070u, 0x3C69u,0x49D3u, 0x8F89u,0xE480u, 0x0u};
//
///* affine x-coordinate of the base point P = (xP, yP) */
//gf2n_t g_gf2n_xP = {0xC1F5u,0x281Eu, 0xC9B7u,0x6C20u, 0xC4FCu,0xAE22u, 0x4282u,0x651Au, 0x0003u};
//
///* public key */
//gf2n_t g_gf2nPublicKey ={0xA613u,0x5DACu,0x99B3u,0x2E27u,0x2688u,0xE7D5u,0x779Au,0x1D20u,0x0000u};



/** !!!!!!!!!! W A R N I N G !!!!!!!!!!
  *
  * this implementation calls the system function rand() for random number generation.
  * rand() is not a cryptographically strong random number generator.
  *
  * !!!!! this implementation must be replaced by a programm using an unpredictable
  * true or pseudo random number generator with good statistics !!!!!
  */

/** generates a 128 bit random number
  * \param[out] erg generated random number
  */
void Ecc_Rng128( gf2n_t  gf2n_RandomValue )
{

  UBYTE ubCount;
#ifndef ORIGA_ECCSTATIC_TEST
  UWORD uwRand;
#endif

#ifndef ORIGA_ECCSTATIC_TEST
  #error "WARNING: Randomize rand() during each Host startup (e.g. using srand()) !!!"
#endif

  for( ubCount = 0u; ubCount < 8u; ubCount++ )
  {

    #ifndef ORIGA_ECCSTATIC_TEST

    /* NOTE: split for debug purpose (gf2n_RandomValue[ubCount] = rand();) */
    uwRand = rand();
    gf2n_RandomValue[ubCount] = uwRand;

    #endif
  }
  gf2n_RandomValue[8] = 0u;

}



/* ****************************************************************************
   name:      Ecc_DoAuthentication()

   function:  execute a complete authentication sequence.

   input:     OUT: bp_AuthenticationResult
                pointer to bool variable to store authentication result into.
   output:    bool

   return:    true, if all was ok.
              false, if errors detected.

   date:      .
   ************************************************************************* */

//BOOL Ecc_DoAuthentication( BOOL * bp_AuthenticationResult )
//{
//
//  gf2n_t gf2nRandomValue;
//  gf2n_t gf2nChallenge;
//  gf2n_t gf2nReturnX;
//  gf2n_t gf2nReturnZ;
//  gf2n_t gf2nCheck;
//
//
//  /* Generate Challenge using the ECC routine */
//  *bp_AuthenticationResult = FALSE;
//
//  if( Ecc_GenerateChallenge( gf2nChallenge, gf2nRandomValue ) == TRUE)
//  {
//   if( Ecc_GenerateCheckValue( gf2nCheck, gf2nRandomValue, g_gf2nPublicKey )== TRUE)
//   {
//    //if( Ecc_SendChallengeAndGetResponse( gf2nChallenge, gf2nReturnX, gf2nReturnZ, FALSE ) == TRUE )
//     //Use polling for Fixed time wait
//     if( Ecc_SendChallengeAndGetResponse( gf2nChallenge, gf2nReturnX, gf2nReturnZ, TRUE ) == TRUE )
//     {
//     if( Ecc_VerifyResponse( gf2nReturnX, gf2nReturnZ, gf2nCheck ) == TRUE )
//     {
//       *bp_AuthenticationResult = TRUE;
//     }
//    }
//   }
//  }
//
//  return *bp_AuthenticationResult;
//
//}



/* ****************************************************************************
   name:      Ecc_GenerateChallenge()

   function:  create a new challenge to be sent to ORIGA.

   input:     OUT: gf2n_Challenge
                gf2n_t array for challenge to be stored into.
              OUT: gf2n_RandomValue
                gf2n_t array for random values to be stored into.
   output:    bool

   return:    true, if all was ok.
              false, if errors detected.

   date:      .
   ************************************************************************* */
#if 0
BOOL Ecc_GenerateChallenge( gf2n_t gf2n_Challenge, gf2n_t gf2n_RandomValue )
{

  enum E_GF2N_RETURNVALUE eResult;


  /* get new random value and calculate challenge */
  Ecc_Rng128( gf2n_RandomValue );

//  display_char(gf2n_RandomValue[0]);
//  display_char(gf2n_RandomValue[0]>>8);
//  display_char(gf2n_RandomValue[1]);
//  display_char(gf2n_RandomValue[1]>>8);
//  display_char(gf2n_RandomValue[2]);
//  display_char(gf2n_RandomValue[2]>>8);
//  display_char(gf2n_RandomValue[3]);
//  display_char(gf2n_RandomValue[3]>>8);
//  display_char(gf2n_RandomValue[4]);
//  display_char(gf2n_RandomValue[4]>>8);
//  display_char(gf2n_RandomValue[5]);
//  display_char(gf2n_RandomValue[5]>>8);
//  display_char(gf2n_RandomValue[6]);
//  display_char(gf2n_RandomValue[6]>>8);
//  display_char(gf2n_RandomValue[7]);
//  display_char(gf2n_RandomValue[7]>>8);
//  display_char(gf2n_RandomValue[8]);
//  display_char(gf2n_RandomValue[8]>>8);

  putchar('\n');
  putchar('X');
  display_char(g_gf2n_xP[0]);
  display_char(g_gf2n_xP[0]>>8);
  display_char(g_gf2n_xP[1]);
  display_char(g_gf2n_xP[1]>>8);
  display_char(g_gf2n_xP[2]);
  display_char(g_gf2n_xP[2]>>8);
  display_char(g_gf2n_xP[3]);
  display_char(g_gf2n_xP[3]>>8);
  display_char(g_gf2n_xP[4]);
  display_char(g_gf2n_xP[4]>>8);
  display_char(g_gf2n_xP[5]);
  display_char(g_gf2n_xP[5]>>8);
  display_char(g_gf2n_xP[6]);
  display_char(g_gf2n_xP[6]>>8);
  display_char(g_gf2n_xP[7]);
  display_char(g_gf2n_xP[7]>>8);
  display_char(g_gf2n_xP[8]);
  display_char(g_gf2n_xP[8]>>8);

  putchar('\n');

  //eResult = scalar_mul( gf2n_Challenge, g_gf2n_xP, gf2n_RandomValue );

  /* do active check that creation passed */
  if( eResult == GF2N_RETURN_OK )
  {
    return TRUE;
  }

  /* error case */
  return FALSE;

}
#endif


/* ****************************************************************************
   name:      Ecc_GenerateCheckValue()

   function:  get check value that is needed to verify the ORIGA response.
              the checkvalue is linked to the random value and pubkey!

   input:     OUT: gf2n_CheckValue
                gf2n_t array for checkvalue to be stored into.
              IN: gf2n_RandomValue
                random values that are used for the current challenge.
              IN: gf2n_PublicKey
                public key that is used for the current challenge.

   output:    bool

   return:    true, if all was ok.
              false, if errors detected.

   date:      .
   ************************************************************************* */
#if 0
BOOL Ecc_GenerateCheckValue( gf2n_t gf2n_CheckValue, gf2n_t gf2n_RandomValue, gf2n_t gf2n_PublicKey )
{

  enum E_GF2N_RETURNVALUE eResult;


  /* calculate checkvalze */
  //eResult = scalar_mul( gf2n_CheckValue, gf2n_PublicKey, gf2n_RandomValue );

  /* do active check that creation passed */
  if(eResult == GF2N_RETURN_OK)
  {
    return TRUE;
  }

  /* error case */
  return FALSE;

}

#endif

/* ****************************************************************************
   name:      Ecc_SendChallengeAnGetResponse()

   function:  send a calculated challenge to ORIGA and start ECC engine of
              the ORIGA IC.
              if ORIGA indicates calculation finished, the results are read
              from ORIGA memory space into the arrays gf2n_XResponse and
              gf2n_ZResponse.

   input:     IN: gf2n_Challenge
                gf2n_t array holding the challenge to be issued.
              OUT: gf2n_XResponse
                gf2n_t array holding the x part of the ORIGA response.
              OUT: gf2n_ZResponse
                gf2n_t array holding the z part of the ORIGA response.
              IN: bPolling
                polling mode to check ORIGA engine for calculation finished
                state.
                if 'true', then a wait of 200ms is done before the data is
                read back, else the host waits for SWI IRQ signal.
                NOTE: please use FALSE, as that is the most efficient way
                      to handle the ECC engine.
   output:    bool

   return:    true, if all was ok.
              false, if errors detected.

   date:      v0.93; 2009-05-25: Swi_SendRawWord return treated as void.
              v1.03; 2011-07-22: Restrict reading of XResponse to 8 bytes.
   ************************************************************************* */
//bool Ecc_SendChallengeAndGetResponse( gf2n_t gf2n_Challenge, gf2n_t gf2n_XResponse, gf2n_t gf2n_ZResponse, BOOL bPolling )
//{
//
//  BOOL bEccIrq;
//  UBYTE ubDelayCount;
//  UBYTE ubCap7Value;
//  UBYTE ubInt0Value;
//  UBYTE ubIndex;
//  UBYTE ubWordIndex;
//  UBYTE ubData;
//
//
//  /* en/disable capabilities according polling selection */
//  ubCap7Value = 0x80u;
//  ubInt0Value = 0x01u;
//  if( bPolling == TRUE )
//  {
//    ubCap7Value = 0x00u;
//    ubInt0Value = 0x00u;
//  }
//  if( Swi_WriteConfigSpace( SWI_CAP7, ubCap7Value, 0x80u ) == FALSE )
//  {
//    return FALSE;
//  }
//  if( Swi_WriteConfigSpace( SWI_INT0, ubInt0Value, 0x01u ) == FALSE )
//  {
//    return FALSE;
//  }
//
//
//  /*select device register set */
//  Swi_SendRawWordNoIrq(SWI_BC, SWI_EREG );
//
//
//  /* write 16 bytes of challenge */
//  for( ubIndex = 0u; ubIndex < 16u; ubIndex++ )
//  {
//    /* set start each aligned 8 byte addresses */
//    if( (ubIndex & 0x07u) == 0u )
//    {
//      Swi_SendRawWordNoIrq(SWI_ERA, (((0x0040u + ubIndex) >> 8u) & 0xFFu) );
//      Swi_SendRawWordNoIrq(SWI_WRA, ( (0x0040u + ubIndex)        & 0xFFu) );
//    }
//
//    ubWordIndex = (ubIndex >> 1u);
//    if( (ubIndex & 1u) == 1u )
//    {
//      ubData = (UBYTE)((gf2n_Challenge[ubWordIndex] >> 8u) & 0x00FFu);
//    }
//    else
//    {
//      ubData = (UBYTE)(gf2n_Challenge[ubWordIndex] & 0xFFu);
//    }
//
//    /* write data w/o any interruption */
//   // Swi_SendRawWord( SWI_WD, ubData, FALSE, FALSE, &bEccIrq );
//    Swi_SendRawWordNoIrq( SWI_WD, ubData);
//  }
//
//  /* write remaining last bytes */
//  Swi_SendRawWordNoIrq(SWI_ERA, ((0x0340u >> 8u) & 0xFFu) );
//  Swi_SendRawWordNoIrq(SWI_WRA, ( 0x0340u        & 0xFFu) );
//
//  ubData = (UBYTE)(gf2n_Challenge[8] & 0xFFu);
//  //Swi_SendRawWord( SWI_WD, ubData, FALSE, FALSE, &bEccIrq );
//    Swi_SendRawWordNoIrq( SWI_WD, ubData);
//
//  /* start ECC calculation */
//  Swi_SendRawWordNoIrq(SWI_BC,SWI_ORIGA_ECCSTART);
//
//  if( bPolling == TRUE )
//  {
//#ifdef COUNTER16BIT
//  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UWORD_MAX )
//  #error "ERROR: 1ms counter loop setup does not fit into 16bit counter!"
//  #endif
//#elif defined COUNTER8BIT
//  #if( (System_CyclesToLoops( System_MicrosToCycles(1000u))) > UBYTE_MAX )
//  #error "ERROR: 1ms counter loop setup does not fit into 8bit counter!"
//  #endif
//#endif
//    for( ubDelayCount = 0u; ubDelayCount < 80u; ubDelayCount++ )
//    {
//      //System_IdleDelay( (COUNTER)(System_CyclesToLoops(System_MicrosToCycles(1000u))) ); /* wait for 80ms */
//      System_IdleDelay( (COUNTER)(System_CyclesToLoops(System_MicrosToCycles(200000u))) ); /* wait for 200ms */
//    }
//  }
//  else
//  {
//    Swi_SendRawWord( SWI_BC, SWI_EINT, TRUE, FALSE, &bEccIrq);
//    if( bEccIrq == FALSE )
//    {
//      return FALSE;
//    }
//  }
//
//  /* extract responses */
//  /* set burst length to 8 bytes in a row */
//  Swi_SendRawWordNoIrq(SWI_BC, SWI_RBL3 );
//  /* select register set */
//  Swi_SendRawWordNoIrq(SWI_BC, SWI_EREG );
//
//
//  /* extract ZResponse */
//  for( ubIndex = 0u; ubIndex < 16u; ubIndex++ )
//  {
//    /* set start each aligned 8 byte addresses */
//    if( (ubIndex & 0x07u) == 0u )
//    {
//      Swi_SendRawWordNoIrq(SWI_ERA, (((0x0030u + ubIndex) >> 8u) & 0xFFu) );
//      Swi_SendRawWordNoIrq(SWI_RRA, ( (0x0030u + ubIndex)        & 0xFFu) );
//    }
//
//    /* read out data */
//    if( Swi_ReadActualSpace( &ubData ) == FALSE )
//    {
//      return FALSE;
//    }
//
//    /* strore data properly */
//    ubWordIndex = (ubIndex >> 1u);
//    if( (ubIndex & 1u) == 1u )
//    {
//      gf2n_ZResponse[ubWordIndex] |= ((UWORD)ubData << 8u);
//    }
//    else
//    {
//      gf2n_ZResponse[ubWordIndex] = ((UWORD)ubData & 0xFFu);
//    }
//  }
//
//  /* extract XResponse */
//  /* set start each aligned 8 byte addresses */
//  Swi_SendRawWordNoIrq(SWI_ERA, ((0x0010u >> 8u)& 0xFFu) );
//  Swi_SendRawWordNoIrq(SWI_RRA, ( 0x0010u       & 0xFFu) );
//
//  for( ubIndex = 0u; ubIndex < 8u; ubIndex++ )
//  {
//    /* read out data */
//    if( Swi_ReadActualSpace( &ubData ) == FALSE )
//    {
//      return FALSE;
//    }
//
//    /* strore data properly */
//    ubWordIndex = (ubIndex >> 1u);
//    if( (ubIndex & 1u) == 1u )
//    {
//      gf2n_XResponse[ubWordIndex] |= ((UWORD)ubData << 8u);
//    }
//    else
//    {
//      gf2n_XResponse[ubWordIndex] = ((UWORD)ubData & 0xFFu);
//    }
//  }
//  /* only lower 8 bytes of XResponse needed */
//  gf2n_XResponse[4] = 0u;
//  gf2n_XResponse[5] = 0u;
//  gf2n_XResponse[6] = 0u;
//  gf2n_XResponse[7] = 0u;
//  gf2n_XResponse[8] = 0u;
//
//
//  /* read remaining ZResponse bits */
//  /* use High level function to switch back to RBL0 and set new address */
//  if( Swi_ReadRegisterSpace( 0x0330u, &ubData ) == FALSE )
//  {
//    return FALSE;
//  }
//  gf2n_ZResponse[8] = ((UWORD)ubData & 0xFFu );
//
//
//  /* all done correct. */
//  return TRUE;
//
//}



/* ****************************************************************************
   name:      Ecc_VerifyResponse()

   function:  check, if response to sent challenge was valid.

   input:     IN: gf2n_XResponse
                gf2n_t array holding the x part of the ORIGA response.
              IN: gf2n_ZResponse
                gf2n_t array holding the z part of the ORIGA response.
              IN: gf2n_CheckValue
                gf2n_t array holding checkvalue.
   output:    bool

   return:    true, if authentication is valid.
              false, if authentication failed.

   date:      .
   ************************************************************************* */
BOOL Ecc_VerifyResponse( gf2n_t  gf2n_XResponse, gf2n_t  gf2n_ZResponse, gf2n_t gf2n_CheckValue )
{

  UWORD uwResult;
  //gf2n_t gf2n_AV;

 
  /* z must not be zero */
  if( gf2n_iszero(gf2n_ZResponse) == GF2N_RETURN_TRUE )
  {
    return FALSE;
  }

  /* do last calculation step and check values */
  gf2n_mul( gf2n_t_a, gf2n_CheckValue, gf2n_ZResponse);
  uwResult  = (gf2n_XResponse[0] ^ gf2n_t_a[0]);
  uwResult |= (gf2n_XResponse[1] ^ gf2n_t_a[1]);
  uwResult |= (gf2n_XResponse[2] ^ gf2n_t_a[2]);
  uwResult |= (gf2n_XResponse[3] ^ gf2n_t_a[3]);

  /* check state of authentication */
  if( uwResult == 0 )
  {
    return TRUE;
  }
  /* authentication failed */
  return FALSE;

}


#endif


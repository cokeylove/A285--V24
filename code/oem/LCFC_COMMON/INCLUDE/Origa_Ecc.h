#ifndef ORIGA_ECC_H
#define ORIGA_ECC_H

#include <stdlib.h>
//#include "Origa_Gf2nMath.h"
//#include "Origa_Swi.h"


/* ****************************************************************************
   *** v0.90; 2008-11-10: 1st version 
   ************************************************************************* */

extern gf2n_t g_gf2nPublicKey;
extern gf2n_t g_gf2n_sqrt_coeff_b;
extern gf2n_t g_gf2n_xP;



void Ecc_Rng128( gf2n_t  gf2n_RandomValue );

//BOOL Ecc_DoAuthentication(BOOL * bp_AuthenticationResult );                

BOOL Ecc_GenerateChallenge( gf2n_t  gf2n_Challenge, gf2n_t gf2n_RandomValue );
BOOL Ecc_GenerateCheckValue( gf2n_t gf2n_CheckValue, gf2n_t gf2n_RandomValue, gf2n_t gf2n_PublicKey );
//BOOL Ecc_SendChallengeAndGetResponse( gf2n_t gf2n_Challenge, gf2n_t gf2n_XResponse, gf2n_t gf2n_ZResponse, BOOL bPolling) ;
BOOL Ecc_VerifyResponse( gf2n_t  gf2n_XResponse, gf2n_t  gf2n_ZResponse, gf2n_t  gf2n_CheckValue );



#endif


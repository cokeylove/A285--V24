/* 
 * ****************************************************************************
 * Copyright (c) ITE INC. All rights reserved.
 * api_sha1.c
 * Dino Li
 * Version, 1.00
 * Note, To link [api_xxx.o] if related api function be used.
 * ****************************************************************************
 */

/* 
 * ****************************************************************************
 * Include all header file
 * ****************************************************************************
 */
#include "..\include.h"

/* 
 * ****************************************************************************
 *
 * ****************************************************************************
 */
/* 128 bytes */
const CDWORD SHA1_Table[32]=
{
    0x61626380,         /* Array[0] W[0] */
	0x00000000,	        /* Array[1] W[1] */
	0x00000000,	        /* Array[2] W[2] */
	0x00000000,	        /* Array[3] W[3] */
	0x00000000,	        /* Array[4] W[4] */
	0x00000000,	        /* Array[5] W[5] */
	0x00000000,	        /* Array[6] W[6] */
	0x00000000,	        /* Array[7] W[7] */
	0x00000000,	        /* Array[8] W[8] */
	0x00000000,	        /* Array[9] W[9] */
    0x00000000,         /* Array[10] W[10] */
    0x00000000,         /* Array[11] W[11] */
    0x00000000,         /* Array[12] W[12] */
    0x00000000,         /* Array[13] W[13] */
    0x00000000,         /* Array[14] W[14] */
    0x00000018,         /* Array[15] W[15] */

    0x00000000,         /* Array[16] HW mode, reserved 16 */
    0x00000000,         /* Array[17] HW mode, reserved 17 */
    0x00000000,         /* Array[18] HW mode, reserved 18 */
    0x00000000,         /* Array[19] HW mode, reserved 19 */
    0x00000000,         /* Array[20] HW mode, reserved 20 */

    0x67452301,         /* Array[21] H0 */
    0xEFCDAB89,         /* Array[22] H1 */
    0x98BADCFE,         /* Array[23] H2 */
    0x10325476,         /* Array[24] H3 */
    0xC3D2E1F0,         /* Array[25] H4 */

    0x00000000,         /* Array[26] HW mode, reserved 26 */
    0x00000000,         /* Array[27] HW mode, reserved 27 */

    0x5a827999,         /* Array[28] K0 */
    0x6ed9eba1,         /* Array[29] K1 */
    0x8f1bbcdc,         /* Array[30] K2 */
    0xca62c1d6          /* Array[31] K3 */
};

/**
 * ****************************************************************************
 *
 *
 * @return
 *
 * @parameter
 * sha1_buffer_addr, SHA1 variables buffer start address,
 *                   128-byte boundary is necessary.
 *
 * @note, sha1_buffer_addr ~ sha1_buffer_addr+128 will be overwrite.
 *
 * ****************************************************************************
 */
void SHA1_Set_W_H_K(BYTE *sha1_buffer_addr)
{
    CDWORD *sha1_buffer_pointer;
    BYTE index;

    sha1_buffer_pointer = (CDWORD *)sha1_buffer_addr;

    for(index=0x00; index<32; index++)
    {
        *sha1_buffer_pointer = SHA1_Table[index];
        sha1_buffer_pointer++;
    }
}

/**
 * ****************************************************************************
 * Firmware SHA-1 hash calculation.
 *
 * @return
 * Address of H0 variable.
 *
 * @parameter
 * sha1_buffer_addr, SHA1 variables buffer start address,
 * 128-byte boundary is necessary.
 *
 * @note
 * 1. sha1_buffer_addr ~ sha1_buffer_addr+128 will be overwrite.
 * 2. H0, H1, H2, H3, and H4 are little-endian or big-endian by toolchain.
 *
 * ****************************************************************************
 */
BYTE * FW_SHA1(BYTE *sha1_buffer_addr)
{
    BYTE index;
    
    DWORD *sha1_buffer_pointer;
    DWORD   sha_a;
    DWORD   sha_b;
    DWORD   sha_c;
    DWORD   sha_d;
    DWORD   sha_e;
    DWORD   sha_f;	
    DWORD   sha_k;	
    DWORD   sha_temp;

    sha1_buffer_pointer = ( DWORD *)sha1_buffer_addr;

    sha_a= sha1_buffer_pointer[21];     /* H0 */
  	sha_b= sha1_buffer_pointer[22];     /* H1 */
	sha_c= sha1_buffer_pointer[23];     /* H2 */
  	sha_d= sha1_buffer_pointer[24];     /* H3 */
  	sha_e= sha1_buffer_pointer[25];     /* H4 */

	for(index=0; index<=79; index++)
    {
		if(index>=16 && index<=79)
        {
		    sha1_buffer_pointer[(index)%16]=
                sha1_buffer_pointer[(index-3)%16]^
                sha1_buffer_pointer[(index-8)%16]^
                sha1_buffer_pointer[(index-14)%16]^
                sha1_buffer_pointer[(index-16)%16];
			sha1_buffer_pointer[(index)%16]=
                (sha1_buffer_pointer[(index)%16]>>31)|
                (sha1_buffer_pointer[(index)%16]<<1);
		}
	 
		if(index>=0 && index<=19)
        {
			sha_f=(sha_b&sha_c)|((~sha_b)&sha_d);
            sha_k=sha1_buffer_pointer[28];  /* K0 */
		}
        else if(index>=20 && index<=39)
        {
        	sha_f=sha_b^sha_c^sha_d;
        	sha_k=sha1_buffer_pointer[29];  /* K1 */
		}
        else if(index>=40 && index<=59)
		{
        	sha_f=(sha_b&sha_c)|(sha_b&sha_d)|(sha_c&sha_d);
        	sha_k=sha1_buffer_pointer[30];  /* K2 */
		}
        else if(index>=60 && index<=79)
        {
        	sha_f=sha_b^sha_c^sha_d;
        	sha_k=sha1_buffer_pointer[31];  /* K3 */
     	}
        else
	 	{	
            ;   
        }
	 
		sha_temp=(sha_a>>27)|(sha_a<<5);

		sha_temp=sha_temp+sha_f+sha_e+sha_k+sha1_buffer_pointer[(index)%16];

		sha_e=sha_d;
		sha_d=sha_c;
		sha_c=(sha_b>>2)+(sha_b<<30);
		sha_b=sha_a;
		sha_a=sha_temp;
	}/* for(i=0; i<=79; i++) */

    sha1_buffer_pointer[21] +=sha_a;;   /* H0 */
    sha1_buffer_pointer[22] +=sha_b;;   /* H1 */
    sha1_buffer_pointer[23] +=sha_c;;   /* H2 */
    sha1_buffer_pointer[24] +=sha_d;;   /* H3 */
    sha1_buffer_pointer[25] +=sha_e;;   /* H4 */
    
    return((BYTE *) &sha1_buffer_pointer[21]);   /* H0 address */
}


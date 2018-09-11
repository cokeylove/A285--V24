
//*****************************************************************************
// Include all header file
#include "..\..\include.h"
//*****************************************************************************

#define ECC_PRECOMLESS
//#include <opgpio.h>
//#include <ofreg.h>
//#include <ocglobal.h>
//#include <oftypes.h>
//#include <CORE_INCLUDE.H>
//#include <OEM_INCLUDE.H>

#if Thinkpad_Battery_HW_Auth

//#pragma ot(9,speed)
//void display_char(unsigned char i);
#define display_char(x)

/** irreducible polynomial */
gf2n_t irred_polynomial = {0x10du, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x8u};
/** constant polynomial with value 0*/
gf2n_t zero_polynomial = {0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};
/** constant polynomial with value 1*/
gf2n_t one_polynomial = {0x1u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u};



/** elliptic curve point addition */
/*
void point_add(gf2n_t gf2n_Epa, gf2n_t gf2n_Fpa, gf2n_t gf2n_Apa, gf2n_t gf2n_Bpa, gf2n_t gf2n_Cpa, gf2n_t gf2n_Dpa, gf2n_t x_point)
{

  gf2n_t gf2nG;

  gf2n_mul(gf2nG, gf2n_Apa, gf2n_Dpa);
  gf2n_mul(gf2n_Fpa, gf2n_Bpa, gf2n_Cpa);
  gf2n_mul(gf2n_Epa, gf2nG, gf2n_Fpa);
  gf2n_sum(gf2n_Fpa, gf2nG);
  gf2n_square(gf2n_Fpa, gf2n_Fpa);
  gf2n_mul(gf2nG, x_point, gf2n_Fpa);
  gf2n_sum(gf2n_Epa, gf2nG);
}*/

/** elliptic curve point doubling */
/*
void point_double(gf2n_t gf2n_Epd, gf2n_t gf2n_Fpd, gf2n_t gf2n_Apd, gf2n_t gf2n_Bpd)
{

  gf2n_t gf2nCpd;

  gf2n_square(gf2n_Epd, gf2n_Apd);
  gf2n_square(gf2n_Fpd, gf2n_Bpd);
  gf2n_mul(gf2nCpd, g_gf2n_sqrt_coeff_b, gf2n_Fpd);
  gf2n_mul(gf2n_Fpd, gf2n_Epd, gf2n_Fpd);
  gf2n_sum(gf2n_Epd, gf2nCpd);
  gf2n_square(gf2n_Epd, gf2n_Epd);

}*/

/** scalar elliptic curve muliplication
  * \param[out] output affine x-coordinate of scalar*G
  * \param[in] x_point affine x-coordinate of the base point G
  * \param[in] scalar scalar
  */
/* internal Functions */


#if 0
enum E_GF2N_RETURNVALUE scalar_mul(gf2n_t output, gf2n_t x_point, gf2n_t scalar)
{

  SWORD i, ret_val;
  gf2n_t gf2n_A;
  gf2n_t gf2n_B;
  gf2n_t gf2n_C;
  gf2n_t gf2n_D;
  gf2n_t gf2n_E;
  gf2n_t gf2n_F;

//     gpio[GPIOOE18] |= 0x02;
//      gpio[GPIOD18] |= 0x02;
  /* initialization of coordinates */
  gf2n_copy(gf2n_A, one_polynomial);
//  gpio[GPIOD18] &= 0xFD;
  gf2n_copy(gf2n_B, zero_polynomial);
  gf2n_copy(gf2n_C, x_point);
  gf2n_copy(gf2n_D, one_polynomial);

  /* main loop */
  for(i = 127; i >= 0; i--)
    {
      //ORIGAcpl;

      if ( ((scalar[i >> 4] >> (i & 15)) & 1) == 1 )
        {
          if ( (i & 1) == 1 )
            {
              point_add(gf2n_E, gf2n_F, gf2n_A, gf2n_B, gf2n_C, gf2n_D, x_point);
              point_double(gf2n_C, gf2n_D, gf2n_C, gf2n_D);
              gf2n_copy(gf2n_A, gf2n_E);
              gf2n_copy(gf2n_B, gf2n_F);
            }
          else
            {
              point_double(gf2n_E, gf2n_F, gf2n_C, gf2n_D);
              point_add(gf2n_A, gf2n_B, gf2n_A, gf2n_B, gf2n_C, gf2n_D, x_point);
              gf2n_copy(gf2n_C, gf2n_E);
              gf2n_copy(gf2n_D, gf2n_F);
            }
        }
      else
        {
          if ( (i & 1) == 1 )
            {
              point_add(gf2n_E, gf2n_F, gf2n_A, gf2n_B, gf2n_C, gf2n_D, x_point);
              point_double(gf2n_A, gf2n_B, gf2n_A, gf2n_B);
              gf2n_copy(gf2n_C, gf2n_E);
              gf2n_copy(gf2n_D, gf2n_F);
            }
          else
            {
              point_double(gf2n_E, gf2n_F, gf2n_A, gf2n_B);
              point_add(gf2n_C, gf2n_D, gf2n_A, gf2n_B, gf2n_C, gf2n_D, x_point);
              gf2n_copy(gf2n_A, gf2n_E);
              gf2n_copy(gf2n_B, gf2n_F);
            }
        }
//        ORIGAcpl;

    }
  /* compute affine result */
  if( gf2n_iszero(gf2n_B) == GF2N_RETURN_TRUE )
  {
   return GF2N_ERROR_POINT_O;
  }

  ret_val = gf2n_inverse(gf2n_B, gf2n_B);
  if( ret_val != GF2N_RETURN_OK )
  {
   return ret_val;
  }

  gf2n_mul(output, gf2n_A, gf2n_B);

  return GF2N_RETURN_OK;

}
#endif


/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Addition in the finite field GF(2^131)
  *  \param[out] out output of addition
  *  \param[in] op1 first operand
  *  \param[in] op2 second operand
  */
void gf2n_add(gf2n_t out, gf2n_t op1, gf2n_t op2)
{

  out[0] = (op1[0] ^ op2[0]);
  out[1] = (op1[1] ^ op2[1]);
  out[2] = (op1[2] ^ op2[2]);
  out[3] = (op1[3] ^ op2[3]);
  out[4] = (op1[4] ^ op2[4]);
  out[5] = (op1[5] ^ op2[5]);
  out[6] = (op1[6] ^ op2[6]);
  out[7] = (op1[7] ^ op2[7]);
  out[8] = (op1[8] ^ op2[8]);

}



/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Addition in the finite field GF(2^131)
  * \param[in,out] io output of addition and first operand
  * \param[in] op second operand
  */
void gf2n_sum(gf2n_t io, gf2n_t op)
{

  io[0] ^= op[0];
  io[1] ^= op[1];
  io[2] ^= op[2];
  io[3] ^= op[3];
  io[4] ^= op[4];
  io[5] ^= op[5];
  io[6] ^= op[6];
  io[7] ^= op[7];
  io[8] ^= op[8];

}



/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Reduction in the finite field GF(2^131) modulo X^131+X^8+X^3+X^2+1
  * \param[in,out] temp vector of WORD
  */
void gf2n_reduction(UWORD *temp)
{

  BYTE i;
  SWORD t, t1, t2, t3;
  /* reduction modulo X^131 + X^8 + X^3 + X^2 + 1 */
  /* X^144 = X^21 + X^16 + X^15 + X^13 */

  for (i = 16; i > 8; i--)
    {
      t = temp[i];

      /*X^16 */
      t2 = t;

      /* X^15 */
      t1 = ((t << 15)&0x8000u);
      t2 ^= ((t >> 1)&0x7FFFu);

      /* X^13 */
      t1 ^= ((t << 13)&0xE000u);
      t2 ^= ((t >> 3)&0x1FFFu);

      /* X^21 */
      t2 ^= ((t << 5) &0xFFE0u);
      t3 = ((t >> 11)&0x001Fu);

      temp[i - 9] ^= t1;
      temp[i - 8] ^= t2;
      temp[i - 7] ^= t3;
    }

  t = (temp[8] & 0xFFF8u);
  temp[8] &= 0x7;

  /*X^16 */
  t2 = t;

  /* X^15 */
  t2 ^= ((t >> 1)&0x7fff);

  /* X^13 */
  t2 ^= ((t >> 3)&0x1fff);

  /* X^21 */
  t2 ^= ((t << 5)&0xFFE0u);
  t3 = ((t >> 11)&0x001F);

  temp[0] ^= t2;
  temp[1] ^= t3;

}



/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Multiplication in the finite field GF(2^131) modulo X^131+X^8+X^3+X^2+1
  * \param[out] out result of multiplication
  * \param[in] op1 first operand
  * \param[in] op2 second operand
  */
void gf2n_mul(gf2n_t out, gf2n_t op1, gf2n_t op2)
{

#ifdef ECC_PRECOMLESS

    int i, j;
    //UWORD temp[17];
    UWORD t;

    for (i = 0; i < 17; i++) gf2n_temp[i] = 0;

    /* first bits > X^4 in the registers */
    t = 0x8000;
    for (j = 15; j > 3; j--)
      {
        for (i = 7; i > -1; i--)
          if (op1[i] & t) gf2n_sum(gf2n_temp + i, op2);

        /* leftshift */
        for (i = 16; i > 0; i--)
          gf2n_temp[i] = ((gf2n_temp[i] << 1) | (gf2n_temp[i - 1] >> 15));
        gf2n_temp[0] <<= 1;

        t >>= 1;
      }

    for (j = 3; j > 0; j--)
      {
        for (i = 8; i > -1; i--)
          if (op1[i] & t) gf2n_sum(gf2n_temp + i, op2);

        /* leftshift */
        for (i = 16; i > 0; i--)
          gf2n_temp[i] = ((gf2n_temp[i] << 1) | (gf2n_temp[i-1] >> 15));
        gf2n_temp[0] <<= 1;

        t >>= 1;
      }

    for (i = 8; i > -1; i--)
      if (op1[i] & 1) gf2n_sum(gf2n_temp + i, op2);

    /* reduction */
    gf2n_reduction(gf2n_temp);
    gf2n_copy(out, gf2n_temp);

#else

  BYTE i;
  BYTE j;
  SWORD t;
  UWORD * pointer;
  UWORD * pointer2;
  UWORD temp[17];
  gf2n_t precom[16];


  /* precomputation */
  gf2n_copy(precom[0], zero_polynomial);
  gf2n_copy(precom[1], op2);

  pointer = precom[1];
  pointer2 = precom[2];
  for (i = 8; i >= 0; i--)
  {
    pointer2[i] = (((pointer[i] << 1)&0xFFFEu) | ((pointer[i-1] >> 15)&1));
  }
  pointer2[0] = (pointer[0] << 1);
  pointer = precom[4];
  for (i = 8; i >= 0; i--)
  {
    pointer[i] = (((pointer2[i] << 1)&0xFFFEu) | ((pointer2[i-1] >> 15)&1));
  }
  pointer[0] = (pointer2[0] << 1);
  pointer2 = precom[8];
  for (i = 8; i >= 0; i--)
  {
    pointer2[i] = (((pointer[i] << 1)&0xFFFEu) | ((pointer[i-1] >> 15)&1));
  }
  pointer2[0] = (pointer[0] << 1);
  TEST3on;
//  gf2n_add_asm(precom[3], precom[2], precom[1]);
//  gf2n_add_asm(precom[5], precom[4], precom[1]);
//  gf2n_add_asm(precom[6], precom[4], precom[2]);
//  gf2n_add_asm(precom[7], precom[4], precom[3]);
//  gf2n_add_asm(precom[9], precom[8], precom[1]);
//  gf2n_add_asm(precom[10], precom[8], precom[2]);
//  gf2n_add_asm(precom[11], precom[8], precom[3]);
//  gf2n_add_asm(precom[12], precom[8], precom[4]);
//  gf2n_add_asm(precom[13], precom[8], precom[5]);
//  gf2n_add_asm(precom[14], precom[8], precom[6]);
//  gf2n_add_asm(precom[15], precom[8], precom[7]);

  gf2n_add(precom[3], precom[2], precom[1]);
  gf2n_add(precom[5], precom[4], precom[1]);
  gf2n_add(precom[6], precom[4], precom[2]);
  gf2n_add(precom[7], precom[4], precom[3]);
  gf2n_add(precom[9], precom[8], precom[1]);
  gf2n_add(precom[10], precom[8], precom[2]);
  gf2n_add(precom[11], precom[8], precom[3]);
  gf2n_add(precom[12], precom[8], precom[4]);
  gf2n_add(precom[13], precom[8], precom[5]);
  gf2n_add(precom[14], precom[8], precom[6]);
  gf2n_add(precom[15], precom[8], precom[7]);
  TEST3off;
  /* precomputation finished */

  for (i = 0; i < 17; i++)
  {
    temp[i] = 0;
  }

  /* multiplication */
  for (j = 12; j > 0; j -= 4)
    {
      for (i = 7; i >= 0; i--)
        {
          t = ((op1[i] >> (SWORD) j) & 0xF);
          gf2n_sum(temp + i, precom[t]);
        }
      for (i = 16; i > 0; i--)
      {
        temp[i] = (((temp[i] << 4)&0xFFF0u) | ((temp[i - 1] >> 12)&0xf));
      }
      temp[0] <<= 4;
    }

  for (i = 8; i >= 0; i--)
    {
      t = (op1[i] & 0xF);
      gf2n_sum(temp + i, precom[t]);
    }
  /* multiplication finished */

  /* reduction */
  gf2n_reduction(temp);
  gf2n_copy(out, temp);

#endif

}



/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Squaring in the finite field GF(2^131) modulo X^131+X^8+X^3+X^2+1
  * \param[out] out result of squaring
  * \param[in] op operand
  */
void gf2n_square(gf2n_t out, gf2n_t op)
{

  /** Lookup-table for faster squaring */
  static const UWORD  square_tab[256] = {   // add code directive to push this part to code rom.
    0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
    0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
    0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
    0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
    0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
    0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
    0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
    0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
    0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
    0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
    0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
    0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
    0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
    0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
    0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
    0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
    0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
    0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
    0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
    0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
    0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
    0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
    0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
    0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
    0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
    0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
    0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
    0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
    0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
    0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
    0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
    0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555};

  SWORD t;
  //UWORD temp[17];

  /* spreading via table lookup*/
  t = op[0];
  gf2n_temp[0] = square_tab[t & 0xFF];
  gf2n_temp[1] = square_tab[((t >> 8)&0xff)];
  t = op[1];
  gf2n_temp[2] = square_tab[t & 0xFF];
  gf2n_temp[3] = square_tab[((t >> 8)&0xff)];
  t = op[2];
  gf2n_temp[4] = square_tab[t & 0xFF];
  gf2n_temp[5] = square_tab[((t >> 8)&0xff)];
  t = op[3];
  gf2n_temp[6] = square_tab[t & 0xFF];
  gf2n_temp[7] = square_tab[((t >> 8)&0xff)];
  t = op[4];
  gf2n_temp[8] = square_tab[t & 0xFF];
  gf2n_temp[9] = square_tab[((t >> 8)&0xff)];
  t = op[5];
  gf2n_temp[10] = square_tab[t & 0xFF];
  gf2n_temp[11] = square_tab[((t >> 8)&0xff)];
  t = op[6];
  gf2n_temp[12] = square_tab[t & 0xFF];
  gf2n_temp[13] = square_tab[((t >> 8)&0xff)];
  t = op[7];
  gf2n_temp[14] = square_tab[t & 0xFF];
  gf2n_temp[15] = square_tab[((t >> 8)&0xff)];
  t = op[8];
  gf2n_temp[16] = square_tab[t & 0xFF];

  /* reduction */
  gf2n_reduction(gf2n_temp);
  gf2n_copy(out, gf2n_temp);

}



#if 0

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Multiplication by X of a GF(2^131)-element without reduction
  * \param[in,out] el GF(2^131) element
  */
static void l_shift(gf2n_t el)
{

  el[8] = ((el[8] << 1) | ((el[7] >> 15)&1));
  el[7] = ((el[7] << 1) | ((el[6] >> 15)&1));
  el[6] = ((el[6] << 1) | ((el[5] >> 15)&1));
  el[5] = ((el[5] << 1) | ((el[4] >> 15)&1));
  el[4] = ((el[4] << 1) | ((el[3] >> 15)&1));
  el[3] = ((el[3] << 1) | ((el[2] >> 15)&1));
  el[2] = ((el[2] << 1) | ((el[1] >> 15)&1));
  el[1] = ((el[1] << 1) | ((el[0] >> 15)&1));
  el[0] = (el[0] << 1);

}


/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** right shift of a GF(2^131)-element
  * \param[in,out] el GF(2^131) element
  */
static void r_shift(gf2n_t el)
{

  el[0] = ((el[0] >> 1) | (el[1] << 15));
  el[1] = ((el[1] >> 1) | (el[2] << 15));
  el[2] = ((el[2] >> 1) | (el[3] << 15));
  el[3] = ((el[3] >> 1) | (el[4] << 15));
  el[4] = ((el[4] >> 1) | (el[5] << 15));
  el[5] = ((el[5] >> 1) | (el[6] << 15));
  el[6] = ((el[6] >> 1) | (el[7] << 15));
  el[7] = ((el[7] >> 1) | (el[8] << 15));
  el[8] = (el[8] >> 1);

}



/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Swapping of two GF(2^131)-elements
  * \param[in,out] el1 GF(2^131) element
  * \param[in,out] el2 GF(2^131) element
  */
static void swap(gf2n_t el1, gf2n_t el2)
{

  SWORD t;

  t = el1[0];
  el1[0] = el2[0];
  el2[0] = t;
  t = el1[1];
  el1[1] = el2[1];
  el2[1] = t;
  t = el1[2];
  el1[2] = el2[2];
  el2[2] = t;
  t = el1[3];
  el1[3] = el2[3];
  el2[3] = t;
  t = el1[4];
  el1[4] = el2[4];
  el2[4] = t;
  t = el1[5];
  el1[5] = el2[5];
  el2[5] = t;
  t = el1[6];
  el1[6] = el2[6];
  el2[6] = t;
  t = el1[7];
  el1[7] = el2[7];
  el2[7] = t;
  t = el1[8];
  el1[8] = el2[8];
  el2[8] = t;

}


/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Inversion of a GF(2^131) element modulo X^131+X^8+X^3+X^2+1
  * \param[out] out result of inversion
  * \param[in] op GF(2^131) field element
  * \return GF2N_ERROR_DIVISION_BY_ZERO if \a op is 0, GF2N_RETURN_OK otherwise
  */

enum E_GF2N_RETURNVALUE gf2n_inverse(gf2n_t out, gf2n_t op)
{

  SWORD count1;
  SWORD count2;

  gf2n_t r0, r1, t0, t1;

  if( gf2n_iszero(op) == GF2N_RETURN_TRUE )
  {
    return GF2N_ERROR_DIVISION_BY_ZERO;
  }

  count1 = DEGREE_N;
  count2 = 0;

  gf2n_copy(t0, zero_polynomial);
  gf2n_copy(t1, one_polynomial);

  gf2n_copy(r0, irred_polynomial);
  gf2n_copy(r1, op);

  while (count1 > 0)
    {
      while ( (r1[8] & 0x8) == 0 )
        {
          l_shift(r1);
          l_shift(t1);
          count1--;
          count2++;
        }

      while ( 1 == 1 )
        {
          if ( (r0[8] & 0x8) != 0 )
            {
              gf2n_sum(r0, r1);
              gf2n_sum(t0, t1);
            }
          if( count2 == 0 )
          {
            break;
          }
          l_shift(r0);
          r_shift(t1);
          count2--;
        }
      swap(t0, t1);
      swap(r0, r1);
    }

  gf2n_copy(out, t0);
  return GF2N_RETURN_OK;

}
#endif
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/** Comparison of a GF(2^131)-variables
  * \param[in] el1 GF(2^131)-element
  * \param[in] el2 GF(2^131)-element
  * \return GF2N_RETURN_TRUE if \a el1 = \a el2, GF2N_RETURN_FALSE otherwise
  */
enum E_GF2N_RETURNVALUE gf2n_isequal(gf2n_t el1, gf2n_t el2)
{

  SWORD t;

  t = (el1[0] ^ el2[0]);
  t |= (el1[1] ^ el2[1]);
  t |= (el1[2] ^ el2[2]);
  t |= (el1[3] ^ el2[3]);
  t |= (el1[4] ^ el2[4]);
  t |= (el1[5] ^ el2[5]);
  t |= (el1[6] ^ el2[6]);
  t |= (el1[7] ^ el2[7]);
  t |= (el1[8] ^ el2[8]);

  if( t == 0 )
  {
    return GF2N_RETURN_TRUE;
  }

  return GF2N_RETURN_FALSE;

}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Check if GF(2^131)-variable is zero
  * \param[in] el GF(2^131)-element
  * \return GF2N_RETURN_TRUE if \a el0 = 0, GF2N_RETURN_FALSE otherwise
  */
enum E_GF2N_RETURNVALUE gf2n_iszero(gf2n_t el)
{

  SWORD t;

  t = (el[0] | el[1]);
  t |= el[2];
  t |= el[3];
  t |= el[4];
  t |= el[5];
  t |= el[6];
  t |= el[7];
  t |= el[8];

  if( t == 0 )
  {
    return GF2N_RETURN_TRUE;
  }

  return GF2N_RETURN_FALSE;

}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/** Copy of a GF(2^131)-element
  * \param[out] copy GF(2^131) element
  * \param[in] in GF(2^131) element
  */
void gf2n_copy(gf2n_t copy, gf2n_t in)
{

  copy[0] = in[0];
  copy[1] = in[1];
  copy[2] = in[2];
  copy[3] = in[3];
  copy[4] = in[4];
  copy[5] = in[5];
  copy[6] = in[6];
  copy[7] = in[7];
  copy[8] = in[8];

}

#endif

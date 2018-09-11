#ifndef ORIGA_GF2N_MATH_H
#define ORIGA_GF2N_MATH_H

//#include "Origa_DataTypes.h"
//#include "Origa_Ecc.h"



/* ****************************************************************************
   *** v0.90; 2008-11-10: 1st version 
   ************************************************************************* */

#define DEGREE_N (131u)

/** Return Values: 
    - positive return code: GF2N_RETURN_*
    - error return code:    GF2N_ERROR_* with GF2N_ERROR_* < \a GF2N_RETURN_OK 
*/
enum E_GF2N_RETURNVALUE {
  /* GF2N_RETURN_* <-- the next return code must be included here with incremented value*/
  /** function return value "FALSE" */
  GF2N_RETURN_FALSE =                      2, 
  /** function return value "TRUE" */
  GF2N_RETURN_TRUE =                       1, 
  /** function finished successfully */
  GF2N_RETURN_OK =                         0, 
  
  /* Error codes have to be placed under GF2N_RETURN_OK */
  /** division by zero */
  GF2N_ERROR_DIVISION_BY_ZERO =            -1, 
  /** unequal parameters in response verification */
  GF2N_ERROR_EQUALITY_TEST =               -2,
  /** point at infinity */
  GF2N_ERROR_POINT_O =                     -3
};

/** irreducible polynomial */
extern gf2n_t irred_polynomial;

/** constants zero and one */
extern gf2n_t zero_polynomial;
extern gf2n_t one_polynomial;


void point_add(gf2n_t gf2n_Epa, gf2n_t gf2n_Fpa, gf2n_t gf2n_Apa, gf2n_t gf2n_Bpa, gf2n_t gf2n_Cpa, gf2n_t gf2n_Dpa, gf2n_t x_point);
void point_double(gf2n_t gf2n_Epd, gf2n_t gf2n_Fpd, gf2n_t gf2n_Apd, gf2n_t gf2n_Bpd);
enum E_GF2N_RETURNVALUE scalar_mul(gf2n_t output, gf2n_t x_point, gf2n_t scalar);


/*---------------------------------------------------------------------------*/
/* arithmetic in the finite field */

/* addition: out = op1 + op2 */
void gf2n_add(gf2n_t out, gf2n_t op1, gf2n_t op2);
/* addition: io += op */
void gf2n_sum(gf2n_t io, gf2n_t op);

/* multiplication: out = op1 * op2 */
void gf2n_mul(gf2n_t out, gf2n_t op1, gf2n_t op2);

/* squaring: out = op^2 */
void gf2n_square(gf2n_t out, gf2n_t op);

/* inversion: out = op^-1 */
enum E_GF2N_RETURNVALUE gf2n_inverse(gf2n_t out, gf2n_t op);

/*---------------------------------------------------------------------------*/
/* Other useful functions */

/* initialization */
void gf2n_setzero(gf2n_t el);
void gf2n_setone(gf2n_t el);

/* check if two elements are equal */
enum E_GF2N_RETURNVALUE gf2n_isequal(gf2n_t el1, gf2n_t el2);

/* check if element is zero */
enum E_GF2N_RETURNVALUE gf2n_iszero(gf2n_t el);

/** Copy of a GF(2^131)-element */
void gf2n_copy(gf2n_t copy, gf2n_t in);

#endif


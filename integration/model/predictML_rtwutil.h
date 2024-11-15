/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML_rtwutil.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef PREDICTML_RTWUTIL_H
#define PREDICTML_RTWUTIL_H

/* Include Files */
#include "predictML_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emlrtFreeThreadStackData(void);

extern predictMLTLS *emlrtGetThreadStackData(void);

extern void emlrtInitThreadStackData(void);

extern double rt_powd_snf(double u0, double u1);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for predictML_rtwutil.h
 *
 * [EOF]
 */

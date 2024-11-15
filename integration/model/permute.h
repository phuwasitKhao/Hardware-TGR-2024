/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: permute.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef PERMUTE_H
#define PERMUTE_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_permute(const double a_data[], const int a_size[3], double b_data[],
               int b_size[2]);

void permute(const double a[624000], double b[624000]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for permute.h
 *
 * [EOF]
 */

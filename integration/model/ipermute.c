/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: ipermute.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "ipermute.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double b[624000]
 *                double a[624000]
 * Return Type  : void
 */
void ipermute(const double b[624000], double a[624000])
{
  int b_k;
  int k;
  for (k = 0; k < 13; k++) {
    for (b_k = 0; b_k < 48000; b_k++) {
      a[k + 13 * b_k] = b[b_k + 48000 * k];
    }
  }
}

/*
 * File trailer for ipermute.c
 *
 * [EOF]
 */

/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: permute.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "permute.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double a_data[]
 *                const int a_size[3]
 *                double b_data[]
 *                int b_size[2]
 * Return Type  : void
 */
void b_permute(const double a_data[], const int a_size[3], double b_data[],
               int b_size[2])
{
  static const signed char iv[3] = {1, 3, 2};
  int b_k;
  int k;
  int plast;
  bool b;
  b = true;
  if (a_size[2] != 0) {
    bool exitg1;
    plast = 0;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < 3)) {
      if (a_size[iv[k] - 1] != 1) {
        if (plast > iv[k]) {
          b = false;
          exitg1 = true;
        } else {
          plast = iv[k];
          k++;
        }
      } else {
        k++;
      }
    }
  }
  if (b) {
    b_size[0] = a_size[0];
    b_size[1] = a_size[2];
    plast = a_size[0] * a_size[2];
    if (plast - 1 >= 0) {
      memcpy(&b_data[0], &a_data[0], (unsigned int)plast * sizeof(double));
    }
  } else {
    int i;
    plast = a_size[0];
    b_size[0] = a_size[0];
    i = a_size[2];
    b_size[1] = a_size[2];
    for (k = 0; k < i; k++) {
      for (b_k = 0; b_k < plast; b_k++) {
        b_data[b_k + b_size[0] * k] = a_data[b_k + a_size[0] * k];
      }
    }
  }
}

/*
 * Arguments    : const double a[624000]
 *                double b[624000]
 * Return Type  : void
 */
void permute(const double a[624000], double b[624000])
{
  int b_k;
  int k;
  for (k = 0; k < 48000; k++) {
    for (b_k = 0; b_k < 13; b_k++) {
      b[k + 48000 * b_k] = a[b_k + 13 * k];
    }
  }
}

/*
 * File trailer for permute.c
 *
 * [EOF]
 */

/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: modwtphaseshift.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "modwtphaseshift.h"
#include "ipermute.h"
#include "permute.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double Origcfs[624000]
 *                double cfs[624000]
 * Return Type  : void
 */
void modwtphaseshift(const double Origcfs[624000], double cfs[624000])
{
  static double Time[624000];
  static double b_x[624000];
  static double x[624000];
  static const short a[12] = {1,  2,   4,   8,   16,   32,
                              64, 128, 256, 512, 1024, 2048};
  static signed char Scale[624000];
  int Time_tmp;
  int b_k;
  int k;
  short b_ph[13];
  short ph[12];
  for (k = 0; k < 12; k++) {
    ph[k] = (short)floor(
        ((double)a[k] * 3.0000000000000009 - 0.85048094716118938) + 0.5);
  }
  for (k = 0; k < 13; k++) {
    for (b_k = 0; b_k < 48000; b_k++) {
      Time_tmp = b_k + 48000 * k;
      Time[Time_tmp] = ((double)b_k + 1.0) - 1.0;
      Scale[Time_tmp] = (signed char)k;
    }
  }
  for (k = 0; k < 12; k++) {
    b_ph[k] = ph[k];
  }
  b_ph[12] = 6966;
  for (k = 0; k < 13; k++) {
    for (b_k = 0; b_k < 48000; b_k++) {
      Time_tmp = b_k + 48000 * k;
      x[Time_tmp] = Time[Time_tmp] + (double)b_ph[k];
    }
  }
  for (k = 0; k < 624000; k++) {
    Time[k] = fmod((int)x[k], 48000.0);
  }
  permute(Origcfs, x);
  for (k = 0; k < 624000; k++) {
    b_x[k] = x[(int)((Time[k] + 1.0) + 48000.0 * (double)Scale[k]) - 1];
  }
  ipermute(b_x, cfs);
}

/*
 * File trailer for modwtphaseshift.c
 *
 * [EOF]
 */

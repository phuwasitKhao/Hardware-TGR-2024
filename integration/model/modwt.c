/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: modwt.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "modwt.h"
#include "fft.h"
#include "ifft.h"
#include "modwtphaseshift.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double x[48000]
 *                double wout[624000]
 * Return Type  : void
 */
void modwt(const double x[48000], double wout[624000])
{
  static creal_T G[48000];
  static creal_T H[48000];
  static creal_T Vhat[48000];
  static creal_T What[48000];
  static creal_T b_Vhat[48000];
  static const double dv[4] = {0.34150635094622, 0.59150635094587,
                               0.15849364905378, -0.09150635094587};
  static const double dv1[4] = {-0.09150635094587, -0.15849364905378,
                                0.59150635094587, -0.34150635094622};
  static double w[624000];
  long k;
  int i;
  int jj;
  memset(&w[0], 0, 624000U * sizeof(double));
  fft(dv, G);
  fft(dv1, H);
  b_fft(x, Vhat);
  for (jj = 0; jj < 12; jj++) {
    long upfactor;
    upfactor = 1L << jj;
    for (k = 0L; k < 48000L; k++) {
      double d;
      double d1;
      double d2;
      double d3;
      long idx;
      idx = upfactor * k;
      idx = (idx - (unsigned int)idx / 48000U * 48000L) + 1L;
      d = G[(int)idx - 1].re;
      d1 = Vhat[(int)(k + 1L) - 1].im;
      d2 = G[(int)idx - 1].im;
      d3 = Vhat[(int)(k + 1L) - 1].re;
      b_Vhat[(int)(k + 1L) - 1].re = d * d3 - d2 * d1;
      b_Vhat[(int)(k + 1L) - 1].im = d * d1 + d2 * d3;
      d = H[(int)idx - 1].re;
      d2 = H[(int)idx - 1].im;
      What[(int)(k + 1L) - 1].re = d * d3 - d2 * d1;
      What[(int)(k + 1L) - 1].im = d * d1 + d2 * d3;
    }
    memcpy(&Vhat[0], &b_Vhat[0], 48000U * sizeof(creal_T));
    ifft(What, b_Vhat);
    for (i = 0; i < 48000; i++) {
      w[jj + 13 * i] = b_Vhat[i].re;
    }
  }
  ifft(Vhat, b_Vhat);
  for (i = 0; i < 48000; i++) {
    w[13 * i + 12] = b_Vhat[i].re;
  }
  modwtphaseshift(w, wout);
}

/*
 * File trailer for modwt.c
 *
 * [EOF]
 */

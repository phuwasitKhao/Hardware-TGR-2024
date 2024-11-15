/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: ifft.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const creal_T x[48000]
 *                creal_T y[48000]
 * Return Type  : void
 */
void ifft(const creal_T x[48000], creal_T y[48000])
{
  static creal_T b_fv[131072];
  static creal_T fv[131072];
  static creal_T wwc[95999];
  static double costab[65537];
  static double sintab[65537];
  static double sintabinv[65537];
  double b_re_tmp;
  double c_re_tmp;
  double nt_im;
  double re_tmp;
  int k;
  int rt;
  c_FFTImplementationCallback_gen(costab, sintab, sintabinv);
  rt = 0;
  wwc[47999].re = 1.0;
  wwc[47999].im = 0.0;
  for (k = 0; k < 47999; k++) {
    int b_y;
    b_y = ((k + 1) << 1) - 1;
    if (96000 - rt <= b_y) {
      rt = (b_y + rt) - 96000;
    } else {
      rt += b_y;
    }
    nt_im = 3.1415926535897931 * (double)rt / 48000.0;
    wwc[47998 - k].re = cos(nt_im);
    wwc[47998 - k].im = -sin(nt_im);
  }
  for (k = 47998; k >= 0; k--) {
    wwc[k + 48000] = wwc[47998 - k];
  }
  for (k = 0; k < 48000; k++) {
    nt_im = wwc[k + 47999].re;
    re_tmp = x[k].im;
    b_re_tmp = wwc[k + 47999].im;
    c_re_tmp = x[k].re;
    y[k].re = nt_im * c_re_tmp + b_re_tmp * re_tmp;
    y[k].im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
  }
  f_FFTImplementationCallback_r2b(y, costab, sintab, fv);
  g_FFTImplementationCallback_r2b(wwc, costab, sintab, b_fv);
  for (rt = 0; rt < 131072; rt++) {
    nt_im = fv[rt].re;
    re_tmp = b_fv[rt].im;
    b_re_tmp = fv[rt].im;
    c_re_tmp = b_fv[rt].re;
    b_fv[rt].re = nt_im * c_re_tmp - b_re_tmp * re_tmp;
    b_fv[rt].im = nt_im * re_tmp + b_re_tmp * c_re_tmp;
  }
  h_FFTImplementationCallback_r2b(b_fv, costab, sintabinv, fv);
  for (k = 0; k < 48000; k++) {
    double ar;
    nt_im = wwc[k + 47999].re;
    re_tmp = fv[k + 47999].im;
    b_re_tmp = wwc[k + 47999].im;
    c_re_tmp = fv[k + 47999].re;
    ar = nt_im * c_re_tmp + b_re_tmp * re_tmp;
    nt_im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
    if (nt_im == 0.0) {
      y[k].re = ar / 48000.0;
      y[k].im = 0.0;
    } else if (ar == 0.0) {
      y[k].re = 0.0;
      y[k].im = nt_im / 48000.0;
    } else {
      y[k].re = ar / 48000.0;
      y[k].im = nt_im / 48000.0;
    }
  }
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */

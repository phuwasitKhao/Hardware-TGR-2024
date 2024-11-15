/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: fft.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "predictML_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double x[48000]
 *                creal_T y[48000]
 * Return Type  : void
 */
void b_fft(const double x[48000], creal_T y[48000])
{
  static creal_T wwc[47999];
  static double costab[65537];
  static double sintab[65537];
  static double sintabinv[65537];
  int k;
  int rt;
  c_FFTImplementationCallback_gen(costab, sintab, sintabinv);
  rt = 0;
  wwc[23999].re = 1.0;
  wwc[23999].im = 0.0;
  for (k = 0; k < 23999; k++) {
    double nt_im;
    int b_y;
    b_y = ((k + 1) << 1) - 1;
    if (48000 - rt <= b_y) {
      rt = (b_y + rt) - 48000;
    } else {
      rt += b_y;
    }
    nt_im = -3.1415926535897931 * (double)rt / 24000.0;
    wwc[23998 - k].re = cos(nt_im);
    wwc[23998 - k].im = -sin(nt_im);
  }
  for (k = 23998; k >= 0; k--) {
    wwc[k + 24000] = wwc[23998 - k];
  }
  e_FFTImplementationCallback_doH(x, y, wwc, costab, sintab, costab, sintabinv);
}

/*
 * Arguments    : const emxArray_real_T *x
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void c_fft(const emxArray_real_T *x, emxArray_creal_T *y)
{
  static double costab[8193];
  static double sintab[8193];
  static double sintabinv[8193];
  double costab1q[4097];
  int k;
  costab1q[0] = 1.0;
  for (k = 0; k < 2048; k++) {
    costab1q[k + 1] = cos(0.00038349519697141029 * ((double)k + 1.0));
  }
  for (k = 0; k < 2047; k++) {
    costab1q[k + 2049] =
        sin(0.00038349519697141029 * (4096.0 - ((double)k + 2049.0)));
  }
  costab1q[4096] = 0.0;
  costab[0] = 1.0;
  sintab[0] = 0.0;
  for (k = 0; k < 4096; k++) {
    double b_sintabinv_tmp;
    double sintabinv_tmp;
    sintabinv_tmp = costab1q[4095 - k];
    sintabinv[k + 1] = sintabinv_tmp;
    b_sintabinv_tmp = costab1q[k + 1];
    sintabinv[k + 4097] = b_sintabinv_tmp;
    costab[k + 1] = b_sintabinv_tmp;
    sintab[k + 1] = -sintabinv_tmp;
    costab[k + 4097] = -sintabinv_tmp;
    sintab[k + 4097] = -b_sintabinv_tmp;
  }
  c_FFTImplementationCallback_dob(x, costab, sintab, sintabinv, y);
}

/*
 * Arguments    : const double x[4]
 *                creal_T y[48000]
 * Return Type  : void
 */
void fft(const double x[4], creal_T y[48000])
{
  static creal_T wwc[47999];
  static double costab[65537];
  static double sintab[65537];
  static double sintabinv[65537];
  int k;
  int rt;
  c_FFTImplementationCallback_gen(costab, sintab, sintabinv);
  rt = 0;
  wwc[23999].re = 1.0;
  wwc[23999].im = 0.0;
  for (k = 0; k < 23999; k++) {
    double nt_im;
    int b_y;
    b_y = ((k + 1) << 1) - 1;
    if (48000 - rt <= b_y) {
      rt = (b_y + rt) - 48000;
    } else {
      rt += b_y;
    }
    nt_im = -3.1415926535897931 * (double)rt / 24000.0;
    wwc[23998 - k].re = cos(nt_im);
    wwc[23998 - k].im = -sin(nt_im);
  }
  for (k = 23998; k >= 0; k--) {
    wwc[k + 24000] = wwc[23998 - k];
  }
  memset(&y[0], 0, 48000U * sizeof(creal_T));
  d_FFTImplementationCallback_doH(x, y, wwc, costab, sintab, costab, sintabinv);
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */

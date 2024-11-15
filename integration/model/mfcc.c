/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: mfcc.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "mfcc.h"
#include "bsxfun.h"
#include "cepstralCoefficients.h"
#include "designMelFilterBank.h"
#include "fft.h"
#include "permute.h"
#include "predictML_emxutil.h"
#include "predictML_rtwutil.h"
#include "predictML_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *audioIn
 *                double varargout_1_data[]
 *                int varargout_1_size[2]
 * Return Type  : void
 */
void mfcc(const emxArray_real_T *audioIn, double varargout_1_data[],
          int varargout_1_size[2])
{
  static double filterBank[192000];
  emxArray_creal_T *x;
  emxArray_real_T *Z;
  emxArray_real_T *y;
  creal_T *x_data;
  double b_y_data[760];
  double b_varargout_1_data[266];
  double coeffs_data[247];
  double E_data[19];
  const double *audioIn_data;
  double bsum;
  double *Z_data;
  double *y_data;
  int b_varargout_1_size[3];
  int y_size[2];
  int hi;
  int i;
  int ib;
  int k;
  int xblockoffset;
  int xi;
  int xpageoffset;
  int y_tmp;
  audioIn_data = audioIn->data;
  y_tmp = (int)floor(((double)audioIn->size[0] - 4800.0) / 2400.0) + 1;
  emxInit_real_T(&y, 2);
  i = y->size[0] * y->size[1];
  y->size[0] = 4800;
  y->size[1] = y_tmp;
  emxEnsureCapacity_real_T(y, i);
  y_data = y->data;
  xpageoffset = 4800 * y_tmp;
  for (i = 0; i < xpageoffset; i++) {
    y_data[i] = 0.0;
  }
  for (xblockoffset = 0; xblockoffset < y_tmp; xblockoffset++) {
    for (k = 0; k < 4800; k++) {
      y_data[k + 4800 * xblockoffset] = audioIn_data[k + 2400 * xblockoffset];
    }
  }
  emxInit_real_T(&Z, 2);
  i = Z->size[0] * Z->size[1];
  Z->size[0] = 4800;
  Z->size[1] = y_tmp;
  emxEnsureCapacity_real_T(Z, i);
  Z_data = Z->data;
  for (i = 0; i < xpageoffset; i++) {
    bsum = y_data[i];
    Z_data[i] = rt_powd_snf(bsum, 2.0);
  }
  for (xi = 0; xi < y_tmp; xi++) {
    xpageoffset = xi * 4800;
    bsum = Z_data[xpageoffset];
    for (k = 0; k < 1023; k++) {
      bsum += Z_data[(xpageoffset + k) + 1];
    }
    E_data[xi] = bsum;
    for (ib = 0; ib < 4; ib++) {
      xblockoffset = xpageoffset + ((ib + 1) << 10);
      bsum = Z_data[xblockoffset];
      if (ib + 2 == 5) {
        hi = 704;
      } else {
        hi = 1024;
      }
      for (k = 2; k <= hi; k++) {
        bsum += Z_data[(xblockoffset + k) - 1];
      }
      E_data[xi] += bsum;
    }
  }
  for (k = 0; k < y_tmp; k++) {
    bsum = E_data[k];
    if (bsum == 0.0) {
      bsum = 2.2250738585072014E-308;
    }
    bsum = log(bsum);
    E_data[k] = bsum;
  }
  bsxfun(y, Z);
  emxFree_real_T(&y);
  emxInit_creal_T(&x);
  c_fft(Z, x);
  x_data = x->data;
  xpageoffset = 4800 * x->size[1];
  i = Z->size[0] * Z->size[1];
  Z->size[0] = 4800;
  hi = x->size[1];
  Z->size[1] = x->size[1];
  emxEnsureCapacity_real_T(Z, i);
  Z_data = Z->data;
  for (k = 0; k < xpageoffset; k++) {
    double b;
    bsum = fabs(x_data[k].re);
    b = fabs(x_data[k].im);
    if (bsum < b) {
      bsum /= b;
      Z_data[k] = b * sqrt(bsum * bsum + 1.0);
    } else if (bsum > b) {
      b /= bsum;
      Z_data[k] = bsum * sqrt(b * b + 1.0);
    } else if (rtIsNaN(b)) {
      Z_data[k] = rtNaN;
    } else {
      Z_data[k] = bsum * 1.4142135623730951;
    }
  }
  designMelFilterBank(filterBank);
  y_size[0] = 40;
  y_size[1] = x->size[1];
  emxFree_creal_T(&x);
  for (xi = 0; xi < hi; xi++) {
    xpageoffset = xi * 40;
    xblockoffset = xi * 4800;
    memset(&b_y_data[xpageoffset], 0, 40U * sizeof(double));
    for (k = 0; k < 4800; k++) {
      bsum = Z_data[xblockoffset + k];
      for (ib = 0; ib < 40; ib++) {
        i = xpageoffset + ib;
        b_y_data[i] += filterBank[ib * 4800 + k] * bsum;
      }
    }
  }
  int coeffs_size[2];
  emxFree_real_T(&Z);
  cepstralCoefficients(b_y_data, y_size, coeffs_data, coeffs_size);
  if (y_tmp - 1 >= 0) {
    memcpy(&varargout_1_data[0], &E_data[0],
           (unsigned int)y_tmp * sizeof(double));
  }
  for (i = 0; i < 13; i++) {
    for (xpageoffset = 0; xpageoffset < y_tmp; xpageoffset++) {
      varargout_1_data[xpageoffset + y_tmp * (i + 1)] =
          coeffs_data[xpageoffset + y_tmp * i];
    }
  }
  b_varargout_1_size[0] = y_tmp;
  b_varargout_1_size[1] = 1;
  b_varargout_1_size[2] = 14;
  xpageoffset = y_tmp * 14 - 1;
  if (xpageoffset >= 0) {
    memcpy(&b_varargout_1_data[0], &varargout_1_data[0],
           (unsigned int)(xpageoffset + 1) * sizeof(double));
  }
  b_permute(b_varargout_1_data, b_varargout_1_size, varargout_1_data,
            varargout_1_size);
}

/*
 * File trailer for mfcc.c
 *
 * [EOF]
 */

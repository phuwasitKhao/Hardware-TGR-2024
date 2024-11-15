/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: sound_freq_last.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 19:54:54
 */

/* Include Files */
#include "sound_freq_last.h"
#include "FFTImplementationCallback.h"
#include "abs.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double X[4096]
 *                double spectrum[2048]
 * Return Type  : void
 */
void sound_freq_last(const double X[4096], double spectrum[2048])
{
  creal_T yCol[4096];
  double mag[4096];
  int i;
  /*  Compute FFT */
  /*  Compute magnitude and normalize */
  c_FFTImplementationCallback_doH(X, yCol);
  b_abs(yCol, mag);
  for (i = 0; i < 4096; i++) {
    mag[i] /= 4096.0;
  }
  /*  Double non-DC terms for proper magnitude scaling */
  for (i = 0; i < 4094; i++) {
    mag[i + 1] *= 2.0;
  }
  /*  Return only first half (up to Nyquist Frequency) */
  memcpy(&spectrum[0], &mag[0], 2048U * sizeof(double));
}

/*
 * File trailer for sound_freq_last.c
 *
 * [EOF]
 */

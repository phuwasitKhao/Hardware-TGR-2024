/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: calcEMA.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 10:47:53
 */

/* Include Files */
#include "calcEMA.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double data[100]
 *                unsigned int N
 *                double ema[100]
 * Return Type  : void
 */
void calcEMA(const double data[100], unsigned int N, double ema[100])
{
  double alpha;
  int t;
  /*  https://www.investopedia.com/terms/e/ema.asp */
  alpha = 2.0 / ((double)N + 1.0);
  /*  Calculate the smoothing factor */
  memset(&ema[0], 0, 100U * sizeof(double));
  /*  Preallocate EMA array */
  ema[0] = data[0];
  /*  Initialize the first EMA value */
  /*  Calculate EMA using vector operations */
  for (t = 0; t < 99; t++) {
    ema[t + 1] = alpha * data[t + 1] + (1.0 - alpha) * ema[t];
  }
}

/*
 * File trailer for calcEMA.c
 *
 * [EOF]
 */

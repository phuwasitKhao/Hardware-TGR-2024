/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: AbstractMovingAverage.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "AbstractMovingAverage.h"
#include "predictML_internal_types.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : dsp_MovingRMS *obj
 *                const double u[48000]
 *                double y[48000]
 * Return Type  : void
 */
void c_AbstractMovingAverage_StepSli(dsp_MovingRMS *obj, const double u[48000],
                                     double y[48000])
{
  double csum;
  double cumRevIndex;
  double modValueRev;
  double z;
  int b_index;
  int k;
  int modIndex;
  cumRevIndex = obj->pCumRevIndex;
  csum = obj->pCumSum;
  modValueRev = obj->pModValueRev;
  modIndex = 0;
  z = 0.0;
  memset(&y[0], 0, 48000U * sizeof(double));
  for (b_index = 0; b_index < 48000; b_index++) {
    double d;
    d = u[b_index];
    csum += d;
    if (modValueRev == 0.0) {
      z = csum + obj->pCumSumRev[(int)cumRevIndex - 1];
    }
    obj->pCumSumRev[(int)cumRevIndex - 1] = d;
    if (cumRevIndex != 479999.0) {
      cumRevIndex++;
    } else {
      cumRevIndex = 1.0;
      csum = 0.0;
      for (k = 479997; k >= 0; k--) {
        obj->pCumSumRev[k] += obj->pCumSumRev[k + 1];
      }
    }
    if (modValueRev == 0.0) {
      y[modIndex] = z / 480000.0;
      modIndex++;
    }
    if (modValueRev > 0.0) {
      modValueRev--;
    } else {
      modValueRev = 0.0;
    }
  }
  obj->pCumSum = csum;
  obj->pCumRevIndex = cumRevIndex;
  obj->pModValueRev = modValueRev;
}

/*
 * File trailer for AbstractMovingAverage.c
 *
 * [EOF]
 */

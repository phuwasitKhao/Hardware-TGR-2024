/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML_rtwutil.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "predictML_rtwutil.h"
#include "predictML_types.h"
#include "rt_nonfinite.h"
#include "omp.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Variable Definitions */
static predictMLTLS *predictMLTLSGlobal;
#pragma omp threadprivate(predictMLTLSGlobal)

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void emlrtFreeThreadStackData(void)
{
  int b;
  int i;
  b = omp_get_max_threads();
#pragma omp parallel for schedule(static) num_threads(omp_get_max_threads())

  for (i = 1; i <= b; i++) {
    free(predictMLTLSGlobal);
  }
}

/*
 * Arguments    : void
 * Return Type  : predictMLTLS *
 */
predictMLTLS *emlrtGetThreadStackData(void)
{
  return predictMLTLSGlobal;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void emlrtInitThreadStackData(void)
{
  int b;
  int i;
  b = omp_get_max_threads();
#pragma omp parallel for schedule(static) num_threads(omp_get_max_threads())

  for (i = 1; i <= b; i++) {
    predictMLTLSGlobal =
        (predictMLTLS *)malloc((size_t)1U * sizeof(predictMLTLS));
  }
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    y = fabs(u0);
    d = fabs(u1);
    if (rtIsInf(u1)) {
      if (y == 1.0) {
        y = 1.0;
      } else if (y > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d == 0.0) {
      y = 1.0;
    } else if (d == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * File trailer for predictML_rtwutil.c
 *
 * [EOF]
 */

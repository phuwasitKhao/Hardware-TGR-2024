/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML_initialize.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "predictML_initialize.h"
#include "predictML_data.h"
#include "predictML_rtwutil.h"
#include "rt_nonfinite.h"
#include "omp.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void predictML_initialize(void)
{
  omp_init_nest_lock(&predictML_nestLockGlobal);
  emlrtInitThreadStackData();
  isInitialized_predictML = true;
}

/*
 * File trailer for predictML_initialize.c
 *
 * [EOF]
 */

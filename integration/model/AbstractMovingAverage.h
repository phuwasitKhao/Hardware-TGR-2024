/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: AbstractMovingAverage.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef ABSTRACTMOVINGAVERAGE_H
#define ABSTRACTMOVINGAVERAGE_H

/* Include Files */
#include "predictML_internal_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void c_AbstractMovingAverage_StepSli(dsp_MovingRMS *obj, const double u[48000],
                                     double y[48000]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for AbstractMovingAverage.h
 *
 * [EOF]
 */

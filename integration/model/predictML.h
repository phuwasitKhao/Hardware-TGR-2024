/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef PREDICTML_H
#define PREDICTML_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void predictML(const double audioData[48000], int positionArray_data[],
                      int positionArray_size[2],
                      unsigned char predictNumArray_data[],
                      int predictNumArray_size[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for predictML.h
 *
 * [EOF]
 */

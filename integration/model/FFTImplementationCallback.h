/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: FFTImplementationCallback.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

/* Include Files */
#include "predictML_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void c_FFTImplementationCallback_dob(const emxArray_real_T *x,
                                     const double costab[8193],
                                     const double sintab[8193],
                                     const double sintabinv[8193],
                                     emxArray_creal_T *y);

void c_FFTImplementationCallback_gen(double costab[65537], double sintab[65537],
                                     double sintabinv[65537]);

void d_FFTImplementationCallback_doH(const double x[4], creal_T y[48000],
                                     const creal_T wwc[47999],
                                     const double costab[65537],
                                     const double sintab[65537],
                                     const double costabinv[65537],
                                     const double sintabinv[65537]);

void e_FFTImplementationCallback_doH(const double x[48000], creal_T y[48000],
                                     const creal_T wwc[47999],
                                     const double costab[65537],
                                     const double sintab[65537],
                                     const double costabinv[65537],
                                     const double sintabinv[65537]);

void f_FFTImplementationCallback_r2b(const creal_T x[48000],
                                     const double costab[65537],
                                     const double sintab[65537],
                                     creal_T y[131072]);

void g_FFTImplementationCallback_r2b(const creal_T x[95999],
                                     const double costab[65537],
                                     const double sintab[65537],
                                     creal_T y[131072]);

void h_FFTImplementationCallback_r2b(const creal_T x[131072],
                                     const double costab[65537],
                                     const double sintab[65537],
                                     creal_T y[131072]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for FFTImplementationCallback.h
 *
 * [EOF]
 */

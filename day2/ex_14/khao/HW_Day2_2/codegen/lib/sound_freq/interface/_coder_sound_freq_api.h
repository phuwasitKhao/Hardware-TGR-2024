/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: _coder_sound_freq_api.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 12:01:25
 */

#ifndef _CODER_SOUND_FREQ_API_H
#define _CODER_SOUND_FREQ_API_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void sound_freq(real_T X[4096], real_T spectrum[2048]);

void sound_freq_api(const mxArray *prhs, const mxArray **plhs);

void sound_freq_atexit(void);

void sound_freq_initialize(void);

void sound_freq_terminate(void);

void sound_freq_xil_shutdown(void);

void sound_freq_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_sound_freq_api.h
 *
 * [EOF]
 */

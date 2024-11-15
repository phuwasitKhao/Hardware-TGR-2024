/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: SystemCore.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "SystemCore.h"
#include "AbstractMovingAverage.h"
#include "predictML_internal_types.h"
#include "predictML_rtwutil.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : b_dsp_MovingRMS *obj
 *                const double varargin_1[48000]
 *                double varargout_1[48000]
 * Return Type  : void
 */
void SystemCore_step(b_dsp_MovingRMS *obj, const double varargin_1[48000],
                     double varargout_1[48000])
{
  static double y[48000];
  double csum;
  double cumRevIndex;
  double modValueRev;
  double z;
  int b_index;
  int k;
  int modIndex;
  if (obj->isInitialized != 1) {
    obj->isInitialized = 1;
    obj->NumChannels = 1;
    obj->FrameLength = 48000;
    obj->isSetupComplete = true;
    obj->TunablePropsChanged = false;
    obj->pCumSum = 0.0;
    memset(&obj->pCumSumRev[0], 0, 95999U * sizeof(double));
    obj->pCumRevIndex = 1.0;
    obj->pModValueRev = 0.0;
  }
  if (obj->TunablePropsChanged) {
    obj->TunablePropsChanged = false;
  }
  cumRevIndex = obj->pCumRevIndex;
  csum = obj->pCumSum;
  modValueRev = obj->pModValueRev;
  modIndex = 0;
  z = 0.0;
  for (k = 0; k < 48000; k++) {
    y[k] = rt_powd_snf(fabs(varargin_1[k]), 2.0);
    varargout_1[k] = 0.0;
  }
  for (b_index = 0; b_index < 48000; b_index++) {
    double d;
    d = y[b_index];
    csum += d;
    if (modValueRev == 0.0) {
      z = csum + obj->pCumSumRev[(int)cumRevIndex - 1];
    }
    obj->pCumSumRev[(int)cumRevIndex - 1] = d;
    if (cumRevIndex != 95999.0) {
      cumRevIndex++;
    } else {
      cumRevIndex = 1.0;
      csum = 0.0;
      for (k = 95997; k >= 0; k--) {
        obj->pCumSumRev[k] += obj->pCumSumRev[k + 1];
      }
    }
    if (modValueRev == 0.0) {
      varargout_1[modIndex] = z / 96000.0;
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
  for (k = 0; k < 48000; k++) {
    varargout_1[k] = sqrt(varargout_1[k]);
  }
}

/*
 * Arguments    : dsp_MovingRMS *obj
 *                const double varargin_1[48000]
 *                double varargout_1[48000]
 * Return Type  : void
 */
void b_SystemCore_step(dsp_MovingRMS *obj, const double varargin_1[48000],
                       double varargout_1[48000])
{
  static double y[48000];
  int k;
  if (obj->isInitialized != 1) {
    obj->isInitialized = 1;
    obj->NumChannels = 1;
    obj->FrameLength = 48000;
    obj->isSetupComplete = true;
    obj->TunablePropsChanged = false;
    obj->pCumSum = 0.0;
    memset(&obj->pCumSumRev[0], 0, 479999U * sizeof(double));
    obj->pCumRevIndex = 1.0;
    obj->pModValueRev = 0.0;
  }
  if (obj->TunablePropsChanged) {
    obj->TunablePropsChanged = false;
  }
  for (k = 0; k < 48000; k++) {
    y[k] = rt_powd_snf(fabs(varargin_1[k]), 2.0);
  }
  c_AbstractMovingAverage_StepSli(obj, y, varargout_1);
  for (k = 0; k < 48000; k++) {
    varargout_1[k] = sqrt(varargout_1[k]);
  }
}

/*
 * File trailer for SystemCore.c
 *
 * [EOF]
 */

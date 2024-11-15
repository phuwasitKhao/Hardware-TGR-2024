/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "predictML.h"
#include "ClassificationTree.h"
#include "MovingRMS.h"
#include "SystemCore.h"
#include "diff.h"
#include "find.h"
#include "handle.h"
#include "mfcc.h"
#include "modwt.h"
#include "predictML_data.h"
#include "predictML_emxutil.h"
#include "predictML_initialize.h"
#include "predictML_internal_types.h"
#include "predictML_types.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double audioData[48000]
 *                int positionArray_data[]
 *                int positionArray_size[2]
 *                unsigned char predictNumArray_data[]
 *                int predictNumArray_size[2]
 * Return Type  : void
 */
void predictML(const double audioData[48000], int positionArray_data[],
               int positionArray_size[2], unsigned char predictNumArray_data[],
               int predictNumArray_size[2])
{
  static b_dsp_MovingRMS shortRMS;
  static dsp_MovingRMS longRMS;
  static double wt[624000];
  static double env1[48000];
  static double env2[48000];
  static double waveEx[48000];
  static bool b_env1[48000];
  cell_wrap_5 prediction;
  emxArray_int32_T *r;
  emxArray_real_T *b_audioData;
  emxArray_real_T *intersectionPoints;
  emxArray_real_T *r2;
  emxArray_uint16_T *r3;
  double coeffs_1D_data[532];
  double coeffs_data[266];
  double *audioData_data;
  double *intersectionPoints_data;
  int b_i;
  int i;
  int loop_ub;
  int numPoints;
  int trueCount;
  int *r1;
  unsigned short *r4;
  if (!isInitialized_predictML) {
    predictML_initialize();
  }
  shortRMS.matlabCodegenIsDeleted = true;
  longRMS.matlabCodegenIsDeleted = true;
  /*  Set parameters */
  modwt(audioData, wt);
  for (i = 0; i < 48000; i++) {
    waveEx[i] = wt[13 * i + 11];
  }
  /*  Moving RMS parameters */
  MovingRMS_MovingRMS(&shortRMS);
  b_MovingRMS_MovingRMS(&longRMS);
  /*  Calculate RMS */
  SystemCore_step(&shortRMS, waveEx, env1);
  b_SystemCore_step(&longRMS, waveEx, env2);
  for (i = 0; i < 48000; i++) {
    b_env1[i] = (env1[i] >= env2[i]);
  }
  emxInit_int32_T(&r);
  eml_find(b_env1, r);
  r1 = r->data;
  emxInit_real_T(&intersectionPoints, 1);
  loop_ub = r->size[0];
  i = intersectionPoints->size[0];
  intersectionPoints->size[0] = r->size[0];
  emxEnsureCapacity_real_T(intersectionPoints, i);
  intersectionPoints_data = intersectionPoints->data;
  for (i = 0; i < loop_ub; i++) {
    intersectionPoints_data[i] = r1[i];
  }
  emxFree_int32_T(&r);
  /*  Filter close points */
  emxInit_real_T(&r2, 1);
  diff(intersectionPoints, r2);
  audioData_data = r2->data;
  numPoints = r2->size[0] + 1;
  b_env1[0] = true;
  loop_ub = r2->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_env1[i + 1] = (audioData_data[i] > 24000.0);
  }
  emxFree_real_T(&r2);
  trueCount = 0;
  for (b_i = 0; b_i < numPoints; b_i++) {
    if (b_env1[b_i]) {
      trueCount++;
    }
  }
  emxInit_uint16_T(&r3);
  i = r3->size[0];
  r3->size[0] = trueCount;
  emxEnsureCapacity_uint16_T(r3, i);
  r4 = r3->data;
  trueCount = 0;
  for (b_i = 0; b_i < numPoints; b_i++) {
    if (b_env1[b_i]) {
      r4[trueCount] = (unsigned short)b_i;
      trueCount++;
    }
  }
  /*  Define clip settings */
  /*  Set maximum number of points to avoid dynamic resizing in C */
  numPoints = (int)fmin(r3->size[0], 100.0);
  /*  Limit to 100 points */
  /*  Predefine arrays with fixed size for code generation */
  positionArray_size[0] = 1;
  positionArray_size[1] = numPoints;
  predictNumArray_size[0] = 1;
  predictNumArray_size[1] = numPoints;
  if (numPoints - 1 >= 0) {
    memset(&predictNumArray_data[0], 0,
           (unsigned int)numPoints * sizeof(unsigned char));
  }
  /*  Fill the arrays with predictions */
  emxInit_real_T(&b_audioData, 1);
  for (b_i = 0; b_i < numPoints; b_i++) {
    double d;
    double d1;
    /*  Determine start and end sample */
    /*  Ensure clippedAudio is at least 4800 samples */
    /*  Calculate MFCC */
    d = intersectionPoints_data[r4[b_i]];
    d1 = fmax(d - 14400.0, 1.0);
    i = b_audioData->size[0];
    b_audioData->size[0] = 48001 - (int)d1;
    emxEnsureCapacity_real_T(b_audioData, i);
    audioData_data = b_audioData->data;
    loop_ub = -(int)d1;
    for (i = 0; i <= loop_ub + 48000; i++) {
      audioData_data[i] = audioData[((int)d1 + i) - 1];
    }
    int coeffs_size[2];
    mfcc(b_audioData, coeffs_data, coeffs_size);
    trueCount = coeffs_size[0] * coeffs_size[1];
    /*  Zero padding to ensure coeffs_1D has 266 elements */
    if (trueCount - 1 >= 0) {
      memcpy(&coeffs_1D_data[0], &coeffs_data[0],
             (unsigned int)trueCount * sizeof(double));
    }
    loop_ub = 266 - trueCount;
    if (loop_ub - 1 >= 0) {
      memset(&coeffs_1D_data[trueCount], 0,
             (unsigned int)loop_ub * sizeof(double));
    }
    /*  Make prediction */
    ClassificationTree_predict(&coeffs_1D_data[0], &prediction);
    /*  Store ASCII code in predictNumArray based on prediction result */
    if (prediction.f1.data[0] == 'N') {
      predictNumArray_data[b_i] = 78U;
      /*  ASCII for 'N' */
    } else if (prediction.f1.data[0] == 'F') {
      predictNumArray_data[b_i] = 70U;
      /*  ASCII for 'F' */
    }
    /*  Store the position */
    positionArray_data[b_i] = (int)d;
  }
  emxFree_real_T(&b_audioData);
  emxFree_uint16_T(&r3);
  emxFree_real_T(&intersectionPoints);
  b_handle_matlabCodegenDestructo(&longRMS);
  handle_matlabCodegenDestructor(&shortRMS);
}

/*
 * File trailer for predictML.c
 *
 * [EOF]
 */

/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: MovingRMS.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "MovingRMS.h"
#include "predictML_internal_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : b_dsp_MovingRMS *obj
 * Return Type  : b_dsp_MovingRMS *
 */
b_dsp_MovingRMS *MovingRMS_MovingRMS(b_dsp_MovingRMS *obj)
{
  b_dsp_MovingRMS *b_obj;
  b_obj = obj;
  b_obj->isInitialized = 0;
  b_obj->NumChannels = -1;
  b_obj->FrameLength = -1;
  b_obj->matlabCodegenIsDeleted = false;
  return b_obj;
}

/*
 * Arguments    : dsp_MovingRMS *obj
 * Return Type  : dsp_MovingRMS *
 */
dsp_MovingRMS *b_MovingRMS_MovingRMS(dsp_MovingRMS *obj)
{
  dsp_MovingRMS *b_obj;
  b_obj = obj;
  b_obj->isInitialized = 0;
  b_obj->NumChannels = -1;
  b_obj->FrameLength = -1;
  b_obj->matlabCodegenIsDeleted = false;
  return b_obj;
}

/*
 * File trailer for MovingRMS.c
 *
 * [EOF]
 */

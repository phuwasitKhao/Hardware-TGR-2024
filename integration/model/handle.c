/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: handle.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "handle.h"
#include "predictML_internal_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : dsp_MovingRMS *obj
 * Return Type  : void
 */
void b_handle_matlabCodegenDestructo(dsp_MovingRMS *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    if (obj->isInitialized == 1) {
      obj->isInitialized = 2;
      if (obj->isSetupComplete) {
        obj->NumChannels = -1;
        obj->FrameLength = -1;
      }
    }
  }
}

/*
 * Arguments    : b_dsp_MovingRMS *obj
 * Return Type  : void
 */
void handle_matlabCodegenDestructor(b_dsp_MovingRMS *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    if (obj->isInitialized == 1) {
      obj->isInitialized = 2;
      if (obj->isSetupComplete) {
        obj->NumChannels = -1;
        obj->FrameLength = -1;
      }
    }
  }
}

/*
 * File trailer for handle.c
 *
 * [EOF]
 */

/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "find.h"
#include "predictML_emxutil.h"
#include "predictML_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const bool x[48000]
 *                emxArray_int32_T *i
 * Return Type  : void
 */
void eml_find(const bool x[48000], emxArray_int32_T *i)
{
  int idx;
  int ii;
  int *i_data;
  bool exitg1;
  idx = 0;
  ii = i->size[0];
  i->size[0] = 48000;
  emxEnsureCapacity_int32_T(i, ii);
  i_data = i->data;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii < 48000)) {
    if (x[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= 48000) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  ii = i->size[0];
  if (idx < 1) {
    i->size[0] = 0;
  } else {
    i->size[0] = idx;
  }
  emxEnsureCapacity_int32_T(i, ii);
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */

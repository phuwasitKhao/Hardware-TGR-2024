/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: predictML_internal_types.h
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

#ifndef PREDICTML_INTERNAL_TYPES_H
#define PREDICTML_INTERNAL_TYPES_H

/* Include Files */
#include "predictML_types.h"
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_cell_wrap_4
#define typedef_cell_wrap_4
typedef struct {
  unsigned int f1[8];
} cell_wrap_4;
#endif /* typedef_cell_wrap_4 */

#ifndef typedef_dsp_MovingRMS
#define typedef_dsp_MovingRMS
typedef struct {
  bool matlabCodegenIsDeleted;
  int isInitialized;
  bool isSetupComplete;
  bool TunablePropsChanged;
  cell_wrap_4 inputVarSize[1];
  int NumChannels;
  int FrameLength;
  double pCumSum;
  double pCumSumRev[479999];
  double pCumRevIndex;
  double pModValueRev;
} dsp_MovingRMS;
#endif /* typedef_dsp_MovingRMS */

#ifndef typedef_b_dsp_MovingRMS
#define typedef_b_dsp_MovingRMS
typedef struct {
  bool matlabCodegenIsDeleted;
  int isInitialized;
  bool isSetupComplete;
  bool TunablePropsChanged;
  cell_wrap_4 inputVarSize[1];
  int NumChannels;
  int FrameLength;
  double pCumSum;
  double pCumSumRev[95999];
  double pCumRevIndex;
  double pModValueRev;
} b_dsp_MovingRMS;
#endif /* typedef_b_dsp_MovingRMS */

#ifndef struct_emxArray_char_T_1x1
#define struct_emxArray_char_T_1x1
struct emxArray_char_T_1x1 {
  char data[1];
};
#endif /* struct_emxArray_char_T_1x1 */
#ifndef typedef_emxArray_char_T_1x1
#define typedef_emxArray_char_T_1x1
typedef struct emxArray_char_T_1x1 emxArray_char_T_1x1;
#endif /* typedef_emxArray_char_T_1x1 */

#ifndef typedef_cell_wrap_5
#define typedef_cell_wrap_5
typedef struct {
  emxArray_char_T_1x1 f1;
} cell_wrap_5;
#endif /* typedef_cell_wrap_5 */

#endif
/*
 * File trailer for predictML_internal_types.h
 *
 * [EOF]
 */

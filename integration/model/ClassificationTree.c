/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: ClassificationTree.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 15-Nov-2024 05:22:48
 */

/* Include Files */
#include "ClassificationTree.h"
#include "predictML_internal_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double Xin[266]
 *                cell_wrap_5 *labels
 * Return Type  : void
 */
void ClassificationTree_predict(const double Xin[266], cell_wrap_5 *labels)
{
  static const double dv[58] = {0.40000000000000024,
                                0.19607843137254907,
                                0.59259259259259245,
                                0.091954022988505815,
                                0.79999999999999993,
                                0.83018867924528306,
                                0.36363636363636348,
                                0.04819277108433738,
                                1.0,
                                1.0,
                                0.0,
                                0.40000000000000008,
                                0.93023255813953487,
                                0.875,
                                0.15384615384615383,
                                0.36363636363636365,
                                0.0,
                                1.0,
                                0.0,
                                0.40000000000000008,
                                1.0,
                                1.0,
                                0.0,
                                0.083333333333333329,
                                1.0,
                                0.125,
                                1.0,
                                1.0,
                                0.0,
                                0.59999999999999976,
                                0.8039215686274509,
                                0.40740740740740755,
                                0.90804597701149425,
                                0.19999999999999998,
                                0.16981132075471686,
                                0.63636363636363646,
                                0.95180722891566261,
                                0.0,
                                0.0,
                                1.0,
                                0.6,
                                0.069767441860465074,
                                0.12500000000000003,
                                0.84615384615384615,
                                0.63636363636363635,
                                1.0,
                                0.0,
                                1.0,
                                0.6,
                                0.0,
                                0.0,
                                1.0,
                                0.91666666666666663,
                                0.0,
                                0.87499999999999989,
                                0.0,
                                0.0,
                                1.0};
  static const double dv1[29] = {0.19268186297302586,
                                 0.18505338076481159,
                                 0.17934004171849516,
                                 0.428507857107367,
                                 0.23484961379790953,
                                 0.11066612048393024,
                                 0.21315744715207802,
                                 0.0028278816251027915,
                                 0.0,
                                 0.0,
                                 0.0,
                                 4.2155320147335082,
                                 -0.099675482883124028,
                                 0.657409305536024,
                                 5.4745015391858169,
                                 -0.043078459751740711,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0788677107226233,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0,
                                 0.0};
  static const signed char kids[58] = {
      2, 3, 4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 0, 0, 0, 0,
      0, 0, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 0,  0,  0,  0,  0, 0, 0, 0,
      0, 0, 0,  0,  0,  0,  28, 29, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0};
  static const unsigned char t0_CutPredictorIndex[29] = {
      238U, 195U, 160U, 233U, 158U, 237U, 47U, 233U, 0U, 0U,
      0U,   4U,   201U, 49U,  11U,  33U,  0U,  0U,   0U, 0U,
      0U,   0U,   0U,   46U,  0U,   0U,   0U,  0U,   0U};
  static const signed char t0_PruneList[29] = {9, 6, 8, 5, 4, 4, 7, 2, 0, 0,
                                               0, 4, 1, 3, 4, 2, 0, 0, 0, 0,
                                               0, 0, 0, 4, 0, 0, 0, 0, 0};
  static const char cv[2] = {'F', 'N'};
  static const bool t0_NanCutPoints[29] = {
      false, false, false, false, false, false, false, false, true, true,
      true,  false, false, false, false, false, true,  true,  true, true,
      true,  true,  true,  false, true,  true,  true,  true,  true};
  int m;
  bool exitg1;
  m = 0;
  exitg1 = false;
  while (!(exitg1 || (t0_PruneList[m] <= 0))) {
    double d;
    d = Xin[t0_CutPredictorIndex[m] - 1];
    if (rtIsNaN(d) || t0_NanCutPoints[m]) {
      exitg1 = true;
    } else if (d < dv1[m]) {
      m = kids[m << 1] - 1;
    } else {
      m = kids[(m << 1) + 1] - 1;
    }
  }
  m = (dv[m + 29] > dv[m]);
  labels->f1.data[0] = cv[m];
}

/*
 * File trailer for ClassificationTree.c
 *
 * [EOF]
 */

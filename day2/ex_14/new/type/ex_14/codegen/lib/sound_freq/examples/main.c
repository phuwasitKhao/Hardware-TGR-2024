/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 24.2
 * C/C++ source code generated on  : 13-Nov-2024 12:01:25
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "rt_nonfinite.h"
#include "sound_freq.h"
#include "sound_freq_terminate.h"

/* Function Declarations */
static void argInit_1x4096_real_T(double result[4096]);

static double argInit_real_T(void);

/* Function Definitions */
/*
 * Arguments    : double result[4096]
 * Return Type  : void
 */
static void argInit_1x4096_real_T(double result[4096])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 4096; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_sound_freq();
  /* Terminate the application.
You do not need to do this more than one time. */
  sound_freq_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_sound_freq(void)
{
  double dv[4096];
  double spectrum[2048];
  /* Initialize function 'sound_freq' input arguments. */
  /* Initialize function input argument 'X'. */
  /* Call the entry-point 'sound_freq'. */
  argInit_1x4096_real_T(dv);
  sound_freq(dv, spectrum);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */

/*
 * File: filtfilter.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 01-Feb-2016 15:06:27
 */

/* Include files */
#include "filtfilter.h"

/* Function Declarations */
static void filter(double b[2], double a[2], const double x[406], double zi,
                   double y[406]);

/* Function Definitions */

/*
 * Arguments    : double b[2]
 *                double a[2]
 *                const double x[406]
 *                double zi
 *                double y[406]
 * Return Type  : void
 */
static void filter(double b[2], double a[2], const double x[406], double zi,
                   double y[406])
{
  int k;
  double dbuffer[2];
  int j;
  if ((a[0] == 0.0) || (!(a[0] != 1.0))) {
  } else {
    for (k = 0; k < 2; k++) {
      b[k] /= a[0];
    }

    a[1] /= a[0];
  }

  dbuffer[1] = zi;
  for (j = 0; j < 406; j++) {
    dbuffer[0] = dbuffer[1];
    dbuffer[1] = 0.0;
    for (k = 0; k < 2; k++) {
      dbuffer[k] += x[j] * b[k];
    }

    dbuffer[1] -= dbuffer[0] * a[1];
    y[j] = dbuffer[0];
  }
}

/*
 * codegen
 * Arguments    : const double x[400]
 *                double c[400]
 * Return Type  : void
 */
void filtfilter(const double x[400], double c[400])
{
  double xtmp;
  double d0;
  int i;
  double y[406];
  double dv0[2];
  double dv1[2];
  double b_y[406];
  double c_y[406];
  xtmp = 2.0 * x[0];
  d0 = 2.0 * x[399];
  for (i = 0; i < 3; i++) {
    y[i] = xtmp - x[3 - i];
  }

  memcpy(&y[3], &x[0], 400U * sizeof(double));
  for (i = 0; i < 3; i++) {
    y[i + 403] = d0 - x[398 - i];
  }

  for (i = 0; i < 2; i++) {
    dv0[i] = 1.0 + -2.0 * (double)i;
    dv1[i] = 1.0 + -1.98 * (double)i;
  }

  memcpy(&b_y[0], &y[0], 406U * sizeof(double));
  filter(dv0, dv1, b_y, -y[0], y);
  for (i = 0; i < 203; i++) {
    xtmp = y[i];
    y[i] = y[405 - i];
    y[405 - i] = xtmp;
  }

  for (i = 0; i < 2; i++) {
    dv0[i] = 1.0 + -2.0 * (double)i;
    dv1[i] = 1.0 + -1.98 * (double)i;
  }

  memcpy(&c_y[0], &y[0], 406U * sizeof(double));
  filter(dv0, dv1, c_y, -y[0], y);
  for (i = 0; i < 203; i++) {
    xtmp = y[i];
    y[i] = y[405 - i];
    y[405 - i] = xtmp;
  }

  memcpy(&c[0], &y[3], 400U * sizeof(double));
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void filtfilter_initialize(void)
{
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void filtfilter_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for filtfilter.c
 *
 * [EOF]
 */

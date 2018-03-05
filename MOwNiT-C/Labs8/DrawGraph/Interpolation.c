#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <math.h>

double almostLambaFunction(double x) {
  return
  /**
    Below You can place any function
    It will be used to create data
      ~ Example:
        rand()
        cos(x*x + x)
  */
  // x*x
  2*x*sin(5*x) + x*x*2
  // 1/(x*x + 21)
  ;
}

/**
  Explanation:
    beginning: beginning of the range
    end: end of the range
    pointsBetween: quantity of the points between beginning and end
 */
double* generateTableFilledWithEqualIntervals(
  double beginning, double end, int pointsBetween) {
  /**
    size explanation:
      2 - space needed for beginning and end
      pointsBetween - quantity of space needed for other points
   */
  int size = 2 + pointsBetween;
  double* table = malloc(size * sizeof(double));
  int i;
  for(i = 0; i < size; i++) {
    table[i] = beginning + i*(end - beginning)/(pointsBetween + 1);
  }
  return table;
  /**
   Example:
   double* a;
   a = generateTableFilledWithEqualIntervals(0,10,9);
      ~a: 0,1,2,...,9,10
   */
}

/**
  Based on generateTableFilledWithEqualIntervals()
  and almostLambaFunction()
 */
double* generateTableFilledWithNumbersBasedOnFunction(
  double beginning, double end, int pointsBetween) {

  /**
    Genereating table with equal intervals
   */
  double* table = generateTableFilledWithEqualIntervals(
    beginning, end, pointsBetween);

  /**
    Adapting created table to represent function value
   */
  int size = 2 + pointsBetween;
  int i;
  for(i = 0; i < size; i++) {
    table[i] =
    /**
      Based on almostLambaFunction()
    */
    almostLambaFunction(table[i]);
    ;
  }
  return table;
  /**
   Example:
   double* a;
   a = generateTableFilledWithNumbersBasedOnFunction(0,10,9);
   */
}

/**
  Creates "InterpolationCSplineGSL.dat" file and places there
  data needed to make graph
 */
void outputCSplineGSL(double* xAxis, double* yAxis,
  int sizeOfVector, double sizeOfInterval) {

  FILE * fp;
  fp = fopen ("InterpolationCSplineGSL.dat", "w+");
  double xi;
  double yi;
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, sizeOfVector);

  gsl_spline_init (spline, xAxis, yAxis, sizeOfVector);
  for (xi = xAxis[0]; xi < xAxis[sizeOfVector - 1]; xi += sizeOfInterval)
    {
      yi = gsl_spline_eval (spline, xi, acc);
      fprintf (fp, "%g %g\n", xi, yi);
    }

  fclose(fp);
  gsl_spline_free (spline);
  gsl_interp_accel_free (acc);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));

  double* x = generateTableFilledWithEqualIntervals(-6, 6, 150);
  double* y = generateTableFilledWithNumbersBasedOnFunction(-6, 6, 150);

  // double* x = generateTableFilledWithEqualIntervals(0, 1, 150);
  // double* y = generateTableFilledWithNumbersBasedOnFunction(0, 1, 150);


  outputCSplineGSL(x, y, 152, 0.001);

  return 0;
}

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
  // -5*x*x*x*x*x*x*x + 4*x*x*x + 7*x*x
  cos(x*x + x)

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
  Creates "InterpolationPolynomialGSL.dat" file and places there
  data needed to make graph
 */
void outputPolynomialGSL(double* xAxis, double* yAxis,
  int sizeOfVector, double sizeOfInterval) {

  FILE * fp;
  fp = fopen ("InterpolationPolynomialGSL.dat", "w+");
  double xi;
  double yi;
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_polynomial, sizeOfVector);

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

/**
  Creates "InterpolationAkimeGSL.dat" file and places there
  data needed to make graph
 */
void outputAkimaGSL(double* xAxis, double* yAxis,
  int sizeOfVector, double sizeOfInterval) {

  FILE * fp;
  fp = fopen ("InterpolationAkimaGSL.dat", "w+");
  double xi;
  double yi;
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_akima, sizeOfVector);

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

void createInterpolationTimeCSV(int depth, int pointsQuantity, int minPoints,
  int maxPoints) {

  FILE * fp;
  fp = fopen("InterpolationTime.csv", "w+");

  // char* result;
  // result = malloc(sizeof("Type,Intervals,ClockTime\n"));
  // strcat(result, "Type,Intervals,ClockTime\n");
  fprintf(fp, "Type,Intervals,ClockTime\n");

  double* x = generateTableFilledWithEqualIntervals(-100, 100,
    pointsQuantity - 2);
  double* y = generateTableFilledWithNumbersBasedOnFunction(-100, 100,
    pointsQuantity - 2);

  clock_t beginning;
  clock_t ending;

  int i;
  int tmpPoints;
  for(i = 1; i <= depth; i++) {
    tmpPoints = minPoints;
    while(tmpPoints <= maxPoints) {
      beginning = clock();
      outputPolynomialGSL(x, y, tmpPoints, 0.1);
      ending = clock();
      fprintf (fp, "0,%d,%f\n", tmpPoints, difftime(ending, beginning));

      beginning = clock();
      outputCSplineGSL(x, y, tmpPoints, 0.1);
      ending = clock();
      fprintf (fp, "1,%d,%f\n", tmpPoints, difftime(ending, beginning));

      beginning = clock();
      outputAkimaGSL(x, y, tmpPoints, 0.1);
      ending = clock();
      fprintf (fp, "2,%d,%f\n", tmpPoints, difftime(ending, beginning));

      tmpPoints += 1;
      printf("#");
    }
    printf("\n%d. loop done\n", i);
  }

  free(x);
  free(y);
  fclose(fp);
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));

  createInterpolationTimeCSV(10, 1000, 20, 900);

  // double* x = generateTableFilledWithEqualIntervals(-1.4, 1.61, 150);
  // double* y = generateTableFilledWithNumbersBasedOnFunction(-1.4, 1.61, 150);
  //
  // outputPolynomialGSL(x, y, 152, 0.1);
  // outputCSplineGSL(x, y, 152, 0.1);
  // outputAkimaGSL(x, y, 152, 0.1);

  return 0;
}

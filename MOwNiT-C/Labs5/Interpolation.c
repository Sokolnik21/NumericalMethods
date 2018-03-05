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
  -5*x*x*x*x*x*x*x + 4*x*x*x + 7*x*x
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
  Non optimal method, calcutaing function for every point
  So sad that there is no lamba function in C
 */
double interpolationLagrange(double* xAxis, double* yAxis,
  int sizeOfVector, double param) {

  double* tempResult;
  tempResult = malloc(sizeOfVector * sizeof(double));
  double result = 0;

  int iArgX;
  int iSpecX;
  for(iArgX = 0; iArgX < sizeOfVector; iArgX += 1) {
    tempResult[iArgX] = yAxis[iArgX];
    for(iSpecX = 0; iSpecX < sizeOfVector; iSpecX += 1) {
      if(iArgX != iSpecX) {
        tempResult[iArgX] = tempResult[iArgX] *
          (param - xAxis[iSpecX]) / (xAxis[iArgX] - xAxis[iSpecX]);
      }
    }
  }

  for(iArgX = 0; iArgX < sizeOfVector; iArgX += 1) {
    result += tempResult[iArgX];
  }

  free(tempResult);
  return result;
}

void outputLagrange(double* xAxis, double* yAxis,
  int sizeOfVector, double sizeOfInterval) {

  FILE * fp;
  fp = fopen ("InterpolationLagrange.dat", "w+");
  double xi;
  double yi;

  for (xi = xAxis[0]; xi < xAxis[sizeOfVector - 1]; xi += sizeOfInterval) {
    yi = interpolationLagrange(xAxis, yAxis, sizeOfVector, xi);
    fprintf (fp, "%f %f\n", xi, yi);
  }

  fclose(fp);
}

/**
  Based on: https://en.wikipedia.org/wiki/Divided_differences#Example
 */
double** dividedDifferencesTable(double* xAxis, double* yAxis,
  int sizeOfVector) {

  /** Invocation: table2D[i][j] */
  double** table2D = malloc(sizeOfVector * sizeof(double*));
  int i;
  for(i = 0; i < sizeOfVector; i++) {
    table2D[i] = malloc(sizeOfVector * sizeof(double));
  }

  /** Filling the first column with base parameters */
  for(i = 0; i < sizeOfVector; i++) {
    table2D[i][0] = yAxis[i];
  }

  /** Filling the rest cells with calcutaled divided difference */
  int j;
  for(j = 1; j < sizeOfVector; j++) {
    for(i = 0; i < sizeOfVector - j; i++) {
      table2D[i][j] = table2D[i + 1][j - 1] - table2D[i][j - 1];
      table2D[i][j] = table2D[i][j] / (xAxis[j + i] - xAxis[i]);
    }
  }

  return table2D;
  /**
    Example:
    double* testX = malloc(5 * sizeof(double));
    testX[0] = 0;
    testX[1] = 1;
    testX[2] = 2;
    testX[3] = 3;
    testX[4] = 4;

    double* testY = malloc(5 * sizeof(double));
    testY[0] = 1;
    testY[1] = 2.25;
    testY[2] = 3.75;
    testY[3] = 4.25;
    testY[4] = 5.81;

    double** testTable2D = dividedDifferencesTable(testX, testY, 5);
    int testI;
    for(testI = 0; testI < 5; testI++) {
      printf("%f\n", testTable2D[0][testI]);
    }

    Expected output:
    ~ Based on example from: http://www.nptel.ac.in/courses/122104019/numerical-analysis/Rathish-kumar/rathish-oct31/fratnode5.html
      1
      1.25
      0.125
      -0.20833
   */
}

double interpolationNewton(double* xAxis, double* yAxis,
  int sizeOfVector, double param) {

  double** dividedDifferencesParam = dividedDifferencesTable(
    xAxis, yAxis, sizeOfVector);

  /**
    Why do I need tempMultiplication variable?
    Newton's interpolation looks like this:
      a0 + a1(x - x0) + a2(x - x0)(x - x1) + ...
    so, as You can see, each sum contains value from previous expression
      ~ it's the (x - x0)...(x - xk) one
    It's good idea to store this
   */
  double tempMultiplication;
  double result = 0;
  double tempResult;

  int iArgX;
  int i;
  for(iArgX = 0; iArgX < sizeOfVector; iArgX += 1) {
    /** First, I take param from dividedDifferencesParam table */
    tempResult = dividedDifferencesParam[0][iArgX];
    for(i = 0; i < iArgX; i += 1) {
      tempResult = tempResult * (param - xAxis[i]);
    }
    result = result + tempResult;
  }

  free(dividedDifferencesParam);
  return result;
}

void outputNewton(double* xAxis, double* yAxis,
  int sizeOfVector, double sizeOfInterval) {

  FILE * fp;
  fp = fopen ("InterpolationNewton.dat", "w+");
  double xi;
  double yi;

  for (xi = xAxis[0]; xi < xAxis[sizeOfVector - 1]; xi += sizeOfInterval) {
    yi = interpolationNewton(xAxis, yAxis, sizeOfVector, xi);
    fprintf (fp, "%f %f\n", xi, yi);
  }

  fclose(fp);
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
      outputLagrange(x, y, tmpPoints, 0.1);
      ending = clock();
      fprintf (fp, "1,%d,%f\n", tmpPoints, difftime(ending, beginning));

      beginning = clock();
      outputNewton(x, y, tmpPoints, 0.1);
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

  // createInterpolationTimeCSV(10, 100, 20, 80);

  double* x = generateTableFilledWithEqualIntervals(-1.4, 1.61, 30);
  double* y = generateTableFilledWithNumbersBasedOnFunction(-1.4, 1.61, 30);

  outputPolynomialGSL(x, y, 32, 0.1);
  outputLagrange(x, y, 32, 0.1);
  outputNewton(x, y, 32, 0.1);

  return 0;
}

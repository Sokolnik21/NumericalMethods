#include <gsl/gsl_blas.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double* generateTableFilledWithRandomNumbers(int size) {
  double* table = malloc(size * sizeof(double));
  int i;
  for(i = 0; i < size; i++) {
    table[i] = rand();
  }
  return table;
  /**
   Example:
   double* a;
   a = generateTableFilledWithRandomNumbers(5);
   */
}

double multiplyVectorVector(int aSize, double a[], int bSize, double b[]) {
  gsl_vector_view aV = gsl_vector_view_array(a, aSize);
  gsl_vector_view bV = gsl_vector_view_array(b, bSize);
  double result;

  gsl_blas_ddot(&aV.vector, &bV.vector, &result);
  return result;
  /**
   Example:
   double a[] = { 1, 10, 100, 1000, 10000 };
   double b[] = { 1, 2, 3, 4, 5 };
   multiplyVectorVector(5, a, 5, b));

   printf("%lf\n", multiplyVectorVector(5, a, 5, b));
   */
}

double* multiplyMatrixVector(int aSize, double a[], int bSize, double b[]) {
  double* result;
  if(aSize % bSize != 0) {
    printf("Matrix doesn't match vector\nResult incorrect\n");
    /** Important line below! */
    result = malloc(sizeof *result);
    *result = -1;
    return result;
  }
  result = malloc(aSize/bSize * sizeof(double));
  gsl_matrix_view aM = gsl_matrix_view_array(a, aSize/bSize, bSize);
  gsl_vector_view bVM = gsl_vector_view_array(b, bSize);
  gsl_vector_view rFM = gsl_vector_view_array(result, aSize/bSize);

  gsl_blas_dgemv(CblasNoTrans, 1.0, &aM.matrix, &bVM.vector, 1.0, &rFM.vector);

  return result;
  /**
   Example:
   double aMatrix[] = { 1, 1, 1,
                        2, 2, 2 };

   double bVectorForMatrix[] = { 3, 4, 5 };

   double* resultForMatrix;
   resultForMatrix = multiplyMatrixVector(6, aMatrix, 3, bVectorForMatrix);
   printf ("[ %g, %g ]\n", resultForMatrix[0], resultForMatrix[1]);
   */
}

double getTimeToGenerateVectorVectorMultiplication(int size) {
  double* a = generateTableFilledWithRandomNumbers(size);
  double* b = generateTableFilledWithRandomNumbers(size);
  clock_t beginning;
  clock_t ending;
  beginning = clock();

  multiplyVectorVector(size, a, size, b);

  ending = clock();
  return difftime(ending, beginning);
}

double getTimeToGenerateMatrixVectorMultiplication(int size) {
  double* a = generateTableFilledWithRandomNumbers(size * size);
  double* b = generateTableFilledWithRandomNumbers(size);
  clock_t beginning;
  clock_t ending;
  beginning = clock();

  multiplyMatrixVector(size * size, a, size, b);

  ending = clock();
  return difftime(ending, beginning);
}

int sizeOfInteger(int a) {
  int result = 0;
  while(a > 0) {
    a = a/10;
    result++;
  }
  return result;
}

/**
 CSV format:
 Type | VectorSize | Time

 Type:
  0 - VectorVector
  1 - MetrixVector

 VectorSize:
  Equals the size of the vector
  For matrix it's the size of rows and columns - it's square matrix

 Time:
  This program uses clock() function from time.h library so the result
  will be equal to the processor clock time dedicated to calculate
  function.
  */
char* spitOutCSVcalculation(int step, int depth, int functionType) {
  char* result = malloc(sizeof *result);
  int a;

  char* tmp;
  int i;
  for(i = 1; i <= depth ; i ++) {
    if(functionType == 0) {
      strcat(result, "0,");
    }
    else {
      strcat(result, "1,");
    }
    tmp = malloc(sizeOfInteger(i * step) * sizeof(char));
    snprintf(tmp, sizeOfInteger(i * step) + 1, "%d", i * step);
    strcat(result, tmp);
    strcat(result, ",");
    if(functionType == 0) {
      a = (int)getTimeToGenerateVectorVectorMultiplication(i * step);
      /** Here is little mangle */
      if(a == 0) a = 1;
    }
    else {
      a = (int)getTimeToGenerateMatrixVectorMultiplication(i * step);
      /** Here is little mangle */
      if(a == 0) a = 1;
    }
    tmp = malloc(sizeOfInteger(a) * sizeof(char));
    snprintf(tmp, sizeOfInteger(a) + 1, "%d", a);
    strcat(result, tmp);
    strcat(result, "\n");
  }
  return result;
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  char* result;
  result = malloc(sizeof("Type,VectorSize,ClockTime\n"));
  strcat(result, "Type,VectorSize,ClockTime\n");

  int i;
  for(i = 1; i <= 10; i++) {
    // result = malloc(sizeof *result);
    strcat(result, spitOutCSVcalculation(500, 10, 0));
    strcat(result, spitOutCSVcalculation(500, 10, 1));
    printf("%d. loop created\n", i);
  }

  FILE *f = fopen("CalculationTime.csv", "w");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }
  fprintf(f, "%s", result);
  fclose(f);

  return 0;
}

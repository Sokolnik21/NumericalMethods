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

/**
 How does is work?
 // A - first matrix; B - second matrix; C - result //
 1. Iteration by A's rows (i)
  2. Iteration by B's columns (j)
    3. Iteration by A's columns (k)
      4. result[i,j]=result[i,j]+A[i,k]*B[k,j]
 */
double* multiplyNaive(int aRows, int aColumns, double a[], int bRows, int bColumns, double b[]) {
  double* result;
  if(aColumns != bRows) {
    printf("Matrix doesn't match\nResult incorrect\n");
    /** Important line below! */
    result = malloc(sizeof *result);
    *result = -1;
    return result;
  }

  /**
   Why calloc insted of malloc?
   becouse here I need to initialize fields with '0'
   */
  result = calloc(0, aRows * bColumns * sizeof(double));

  int i, j, k;
  for(i = 0; i < aRows; i++) {
    for(j = 0; j < bColumns; j++) {
      for(k = 0; k < aColumns; k++) {
        /**
         resultRows = aRows
         resultColumns = bColumns
         result[i,j]=result[i,j]+A[i,k]*B[k,j]
         */
        result[i * aRows + j] = result[i * aRows + j]
          + a[i * aRows + k] * b[k * bRows +j];
      }
    }
  }

  return result;
}

double getTimeToGenerateMultiplyNaive(int rowSize) {
  double* a = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* b = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* result;

  result = malloc(rowSize * rowSize * sizeof(double));
  int indexation;
  for(indexation = 0; indexation < rowSize * rowSize; indexation++) {
  	result[indexation] = 0;
  }

  clock_t beginning;
  clock_t ending;
  beginning = clock();

  int i, j, k;
  for(i = 0; i < rowSize; i++) {
    for(j = 0; j < rowSize; j++) {
      for(k = 0; k < rowSize; k++) {
        /**
         resultRows = aRows
         resultColumns = bColumns
         result[i,j]=result[i,j]+A[i,k]*B[k,j]
         */
        result[i * rowSize + j] = result[i * rowSize + j]
          + a[i * rowSize + k] * b[k * rowSize +j];
      }
    }
  }

  ending = clock();
  free(a);
  free(b);
  free(result);
  return difftime(ending, beginning);
}

double* multiplyBetter(int aRows, int aColumns, double a[], int bRows, int bColumns, double b[]) {
  /*
    # poprawiona funkcja korzytająca z powyższego oraz z faktu, że
    #można zmieniać kolejność operacji dodawania (a co za tym idzie kolejnosc petli).
  */
  double* result;
  if(aColumns != bRows) {
    printf("Matrix doesn't match\nResult incorrect\n");
    /** Important line below! */
    result = malloc(sizeof *result);
    *result = -1;
    return result;
  }

  /**
   Why calloc insted of malloc?
   becouse here I need to initialize fields with '0'
   */
  result = calloc(0, aRows * bColumns * sizeof(double));

  int i, j, k;
  for(j = 0; j < aRows; j++) {
    for(k = 0; k < bColumns; k++) {
      for(i = 0; i < aColumns; i++) {
        /**
         resultRows = aRows
         resultColumns = bColumns
         result[i,j]=result[i,j]+A[i,k]*B[k,j]
         */
        result[i * aRows + j] = result[i * aRows + j]
          + a[i * aRows + k] * b[k * bRows +j];
      }
    }
  }

  return result;
}

double getTimeToGenerateMultiplyBetter(int rowSize) {
  double* a = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* b = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* result;

  /** 
   Funny thing
   I cannot simply use calloc function becouse then I got core dumped
   while using free() function
   */
  // result = (double*) calloc(0, rowSize * rowSize * sizeof(double));
  result = malloc(rowSize * rowSize * sizeof(double));
  int indexation;
  for(indexation = 0; indexation < rowSize * rowSize; indexation++) {
  	result[indexation] = 0;
  }

  clock_t beginning;
  clock_t ending;
  beginning = clock();

  int i, j, k;
  for(j = 0; j < rowSize; j++) {
    for(k = 0; k < rowSize; k++) {
      for(i = 0; i < rowSize; i++) {
        /**
         resultRows = aRows
         resultColumns = bColumns
         result[i,j]=result[i,j]+A[i,k]*B[k,j]
         */
        result[i * rowSize + j] = result[i * rowSize + j]
          + a[i * rowSize + k] * b[k * rowSize +j];
      }
    }
  }

  ending = clock();
  free(a);
  free(b);
  free(result);
  return difftime(ending, beginning);
}

double* multiplyBLAS(int aRows, int aColumns, double a[], int bRows, int bColumns, double b[]) {
  double* result;
  if(aColumns != bRows) {
    printf("Matrix doesn't match\nResult incorrect\n");
    /** Important line below! */
    result = malloc(sizeof *result);
    *result = -1;
    return result;
  }

  result = malloc(aRows * bColumns * sizeof(double));
  gsl_matrix_view aM = gsl_matrix_view_array(a, aRows, aColumns);
  gsl_matrix_view bM = gsl_matrix_view_array(b, bRows, bColumns);
  gsl_matrix_view rM = gsl_matrix_view_array(result, aRows, bColumns);

  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, &aM.matrix, &bM.matrix, 1.0, &rM.matrix);

  return result;
  /**
   Example:
   double aMatrix[] = { 1, 1, 1,
                        2, 2, 2 };

   bVectorForMatrix[] = { 3, 4,
                          5, 6 ,
                          7, 8 };

   multiplyBLAS(2, 3, a, 3, 2, b);
   */
}

/**
 Assumptions:
 - Both matrixes are square matrixes
 - Time is measured in CPU Time
 - To get clear performance time I use some redundancy code(from multiplyBLAS())
    to separate data initialization from function execution
 */
double getTimeToGenerateMultiplyBLAS(int rowSize) {
  double* a = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* b = generateTableFilledWithRandomNumbers(rowSize * rowSize);
  double* result;

  result = malloc(rowSize * rowSize * sizeof(double));
  gsl_matrix_view aM = gsl_matrix_view_array(a, rowSize, rowSize);
  gsl_matrix_view bM = gsl_matrix_view_array(b, rowSize, rowSize);
  gsl_matrix_view rM = gsl_matrix_view_array(result, rowSize, rowSize);

  clock_t beginning;
  clock_t ending;
  beginning = clock();

  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1.0, &aM.matrix, &bM.matrix, 1.0, &rM.matrix);

  ending = clock();
  free(a);
  free(b);
  free(result);
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
 Type | MatrixRows | Time

 Type:
  0 - multiplyNaive
  1 - multiplyBetter
  2 - multiplyBLAS

 VectorSize:
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
  char buffer2[2];
  int i;
  for(i = 1; i <= depth ; i ++) {
    if(functionType == 0) {
	  buffer2[0] = '0';
	  buffer2[1] = ',';
	  strcat(result, buffer2);
    }
    else if(functionType == 1) {
	  buffer2[0] = '1';
	  buffer2[1] = ',';
	  strcat(result, buffer2);
    }
    else {
	  buffer2[0] = '2';
	  buffer2[1] = ',';
	  strcat(result, buffer2);
    }
    tmp = malloc(sizeOfInteger(i * step) * sizeof(char));
    snprintf(tmp, sizeOfInteger(i * step) + 1, "%d", i * step);
    strcat(result, tmp);
	  buffer2[0] = ',';
	  buffer2[1] = ' ';
    strcat(result, buffer2);
    if(functionType == 0) {
      a = (int)getTimeToGenerateMultiplyNaive(i * step);
      /** Here is little mangle */
      if(a == 0) a = 1;
    }
    else if(functionType == 1) {
      a = (int)getTimeToGenerateMultiplyBetter(i * step);
      /** Here is little mangle */
      if(a == 0) a = 1;
    }
    else {
      a = (int)getTimeToGenerateMultiplyBLAS(i * step);
      /** Here is little mangle */
      if(a == 0) a = 1;
    }
    tmp = malloc(sizeOfInteger(a) * sizeof(char));
    snprintf(tmp, sizeOfInteger(a) + 1, "%d", a);
    strcat(result, tmp);
	  buffer2[0] = '\n';
	  buffer2[1] = ' ';
    strcat(result, buffer2);
  }
  return result;
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));

  char* result;
  result = malloc(sizeof("Type,MatrixRows,ClockTime\n"));
  strcat(result, "Type,MatrixRows,ClockTime\n");
  
  int i;
  for(i = 1; i <= 10; i++) {
    // result = malloc(sizeof *result);
    strcat(result, spitOutCSVcalculation(10, 15, 0));
    strcat(result, spitOutCSVcalculation(10, 15, 1));
    strcat(result, spitOutCSVcalculation(10, 15, 2));
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

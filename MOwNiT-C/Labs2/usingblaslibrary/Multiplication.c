#include <gsl/gsl_blas.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  /** Vector multiplication */
  double a[] = { 1, 10, 100, 1000, 10000 };
  double b[] = { 1, 2, 3, 4, 5 };
  double resultForVector;

  gsl_vector_view aV = gsl_vector_view_array(a, 5);
  gsl_vector_view bV = gsl_vector_view_array(b, 5);

  gsl_blas_ddot(&aV.vector, &bV.vector, &resultForVector);

  printf("%lf\n", resultForVector);

  /** Matrix multiplication */
  double aMatrix[] = { 1, 1, 1,
                       2, 2, 2 };

  double bVectorForMatrix[] = { 3, 4, 5 };

  double resultForMatrix[2];

  gsl_matrix_view aM = gsl_matrix_view_array(aMatrix, 2, 3);
  gsl_vector_view bVM = gsl_vector_view_array(bVectorForMatrix, 3);
  gsl_vector_view rFM = gsl_vector_view_array(resultForMatrix, 2);

  gsl_blas_dgemv(CblasNoTrans, 1.0, &aM.matrix, &bVM.vector, 1.0, &rFM.vector);

  printf ("[ %g, %g ]\n", resultForMatrix[0], resultForMatrix[1]);

  return 0;
}

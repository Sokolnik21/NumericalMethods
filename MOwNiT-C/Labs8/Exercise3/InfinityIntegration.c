#include <gsl/gsl_integration.h>
#include <stdio.h>

/** To use specified functions in almostLambaFunction() */
#include <math.h>

double f (double x, void * params) {
  double alpha = *(double *) params;
  double f = 1/(x*x + 21);
  return f;
}

int main(int argc, char const *argv[]){
  double result, error;

  gsl_integration_workspace * w
    = gsl_integration_workspace_alloc (1000);

  double gsl_result, gsl_error;

  gsl_function F;
  F.function = &f;

  gsl_integration_qagi(&F, 0, 1e-7, 1000, w, &result, &error);

  /** openning file */
  FILE * fp3;
  fp3 = fopen ("CalculatingIntegrationInInfinity.txt", "w+");
  fprintf(fp3, "/// Exercise 3 \\\\\\\n");
  fprintf(fp3, "\\\\\\\\\\\\\\\\\\/////////\n\n");
  fprintf(fp3, "function: 1/(x*x + 21)\n");
  fprintf(fp3, "Numerical integration calculated on infinity range\n");
  fprintf(fp3, "result: %f\n", result);
  fprintf(fp3, "estimated error: %f\n", error);
  fprintf(fp3, "intervals: %zu\n", w->size);
  fprintf(fp3, "\n");

  fclose(fp3);

  gsl_integration_workspace_free (w);
  return 0;
}

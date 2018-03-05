#include <gsl/gsl_integration.h>
#include <stdio.h>

/** To use specified functions in almostLambaFunction() */
#include <math.h>

#define PI (3.141592653589793)

struct my_f_params { double a ; };

double f (double x, void * p) {
  struct my_f_params * params = (struct my_f_params *)p;
  double a = (params->a);

  return a*cos(a*x);
}

/** Differences between oscilating and non oscilating integration */
int main(int argc, char const *argv[]) {
  /** constants */
  double estimated_accuracy = 0.1;
  double begin = 0;
  double end = PI*9/2;

  /** openning file */
  FILE * fp;
  fp = fopen ("OscillatingNumericalIntegration.csv", "w+");
  fprintf(fp, "Type, a, Result, Error, Intervals\n");

  gsl_integration_workspace * w
    = gsl_integration_workspace_alloc (1000);

  double result, error;
  size_t neval;

  gsl_function F;

  int a;
  int i;
  for(i = 0; i < 100; i++){
    a = i + 1;
    struct my_f_params params = { a };
    F.function = &f;
    F.params = &params;

    /** Type: 0
      When "a" was higher than 10 gsl_integration_qng starts dumping core
     */
    if(a < 11) {
      gsl_integration_qng(&F, begin, end, estimated_accuracy, estimated_accuracy,
        &result, &error, &neval);
      fprintf(fp, "%d, %d, ", 0, a);
      fprintf(fp, "%f, %f, %zu\n", result, error, neval);
    }

    /** Type: 1 */
    gsl_integration_qag(&F, begin, end, estimated_accuracy, estimated_accuracy,
      1000, 1, w, &result, &error);
    fprintf(fp, "%d, %d, ", 1, a);
    fprintf(fp, "%f, %f, %zu\n", result, error, w->size);

    /** Type: 2 */
    if(a < 61) {
      gsl_integration_qawo_table * wf
        = gsl_integration_qawo_table_alloc(a, (end - begin), GSL_INTEG_SINE, 100);

      gsl_integration_qawo(&F, begin, estimated_accuracy, estimated_accuracy,
        1000, w, wf, &result, &error);
      fprintf(fp, "%d, %d, ", 2, a);
      fprintf(fp, "%f, %f, %zu\n", result, error, w->size);

      gsl_integration_qawo_table_free(wf);
    }
  }

  fclose(fp);

  gsl_integration_workspace_free (w);
  return 0;
}

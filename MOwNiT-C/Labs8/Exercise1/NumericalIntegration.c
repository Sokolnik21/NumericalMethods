#include <gsl/gsl_integration.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <string.h>

/** To use specified functions in almostLambaFunction() */
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
  // x*x*sin(x*x*sin(x))
  // x*x
  sqrt(x)
  // 2*x*sin(5*x) + x*x*2
  ;
}

double f (double x, void * params) {
  double alpha = *(double *) params;
  double f = almostLambaFunction(x);
  return f;
}

/**
  Based on almostLambaFunction()
  Calculates integration for that function in given range with specified
  quantity of intervals
  It's left type rectangleRule
 */
double rectangleRule(double begin, double end, double quantity) {
  double sum = 0;
  double interval = (end - begin)/quantity;

  int i;
  for(i = 0; i < quantity; i++) {
    sum += interval * almostLambaFunction(i * interval + begin);
  }

  return sum;
}

/**
  Here is little trick
  I'd like to increase my number of inverals without specifying exact
  evaluation of the function (if I haven't done this I'd have to MANUALLY
  change it every time I change base function or it range)
  So, to deal with it I'm using result from gsl_integration_qags function
  with range of acceptable aberration (measured from 0f to 1f) to stop
  interval incrementation when result of the rectangleRule will be in that
  space.
  The estimated error of this function is twice bigger than error from
  gsl_integration_qags
 */
int rectangleRuleBasedOnGslEvaluation(
  double begin, double end, double gsl_result, double acceptable_error,
    double * rec_result, int * intervals){

  /** Now I have estimated result with it's error I can work with */
  double sum;
  int i = 0;

  do {
    i++;
    sum = rectangleRule(begin, end, i);
  } while(!(sum > (gsl_result - acceptable_error) &&
      sum < (gsl_result + acceptable_error)));

  *rec_result = sum;
  *intervals = i;

  return 0;
}

int main(int argc, char const *argv[]) {
  /** constants */
  double estimated_accuracy = 0.001;
  double begin = 0;
  double end = 1;

  /** openning file */
  FILE * fp;
  fp = fopen ("NumericalIntegration.txt", "w+");
  fprintf(fp, "/// Exercise 1 \\\\\\\n");
  fprintf(fp, "\\\\\\\\\\\\\\\\\\/////////\n\n");
  fprintf(fp, "Differences in numerical integration functions\n");
  fprintf(fp, "estimated accuracy: %f\n", estimated_accuracy);
  fprintf(fp, "beginning: %f\n", begin);
  fprintf(fp, "ending: %f\n", end);
  fprintf(fp, "\n");

  gsl_integration_workspace * w
    = gsl_integration_workspace_alloc (1000);

  double gsl_result, gsl_error;

  gsl_function F;
  F.function = &f;

  gsl_integration_qags (&F, begin, end, 0, 1e-7, 1000,
                        w, &gsl_result, &gsl_error);

  fprintf(fp, "**High accuracy result**\n");
  fprintf(fp, "Based on gsl_integration_qags\n");
  fprintf(fp, "result: %f\n", gsl_result);
  fprintf(fp, "estimated error: %f\n", gsl_error);
  fprintf(fp, "intervals: %zu\n", w->size);
  fprintf(fp, "\n");

  double result, error;
  int interval;
  size_t neval;

  rectangleRuleBasedOnGslEvaluation(begin, end, gsl_result, estimated_accuracy,
    &result, &interval);
  fprintf(fp, "**Rectangle Rule**\n");
  fprintf(fp, "result: %f\n", result);
  fprintf(fp, "difference from gsl: %f\n", result - gsl_result);
  fprintf(fp, "intervals: %d\n", interval);
  fprintf(fp, "\n");

  gsl_integration_qng(&F, begin, end, estimated_accuracy, estimated_accuracy,
    &result, &error, &neval);
  fprintf(fp, "**gsl_integration_qng**\n");
  fprintf(fp, "result: %f\n", result);
  fprintf(fp, "difference from gsl: %f\n", result - gsl_result);
  fprintf(fp, "intervals: %zu\n", neval);
  fprintf(fp, "\n");

  gsl_integration_qag(&F, begin, end, estimated_accuracy, estimated_accuracy,
    1000, 1, w, &result, &error);
  fprintf(fp, "**gsl_integration_qag**\n");
  fprintf(fp, "GSL_INTEG_GAUSS15(key = 1)\n");
  fprintf(fp, "result: %f\n", result);
  fprintf(fp, "difference from gsl: %f\n", result - gsl_result);
  fprintf(fp, "intervals: %zu\n", w->size);
  fprintf(fp, "\n");

  gsl_integration_qags(&F, begin, end, estimated_accuracy, estimated_accuracy,
    1000, w, &result, &error);
  fprintf(fp, "**gsl_integration_qags**\n");
  fprintf(fp, "result: %f\n", result);
  fprintf(fp, "difference from gsl: %f\n", result - gsl_result);
  fprintf(fp, "intervals: %zu\n", w->size);
  fprintf(fp, "\n");

  double *pts = malloc(2 * sizeof(double));
  pts[0] = begin;
  pts[1] = end;

  gsl_integration_qagp(&F, pts, 2, estimated_accuracy, estimated_accuracy,
    1000, w, &result, &error);
  fprintf(fp, "**gsl_integration_qagp**\n");
  fprintf(fp, "result: %f\n", result);
  fprintf(fp, "difference from gsl: %f\n", result - gsl_result);
  fprintf(fp, "intervals: %zu\n", w->size);
  fprintf(fp, "\n");

  fclose(fp);

  gsl_integration_workspace_free (w);
  return 0;
}

#include <gsl/gsl_ieee_utils.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  float x = 1;
  x = x/3;
  float previous;
  while(x != previous){
    printf("%f\n", x);
    gsl_ieee_printf_float(&x);
    printf("\n");
    previous = x;
    x = x/2;
  }
  return 0;
}

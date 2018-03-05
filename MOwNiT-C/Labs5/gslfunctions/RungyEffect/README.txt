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



main:

double* x = generateTableFilledWithEqualIntervals(-1.4, 1.61, 150);
double* y = generateTableFilledWithNumbersBasedOnFunction(-1.4, 1.61, 150);

outputPolynomialGSL(x, y, 152, 0.1);
outputCSplineGSL(x, y, 152, 0.1);
outputAkimaGSL(x, y, 152, 0.1);

compile:
	gcc -I/usr/local/include -L/usr/local/lib Interpolation.c -o Interpolation -lgsl -lgslcblas -lm

run:
	./Interpolation

graphPolynomial:
	graph -T ps < InterpolationPolynomialGSL.dat > InterpolationPolynomialGSL.ps

graphLagrange:
	graph -T ps < InterpolationLagrange.dat > InterpolationLagrange.ps

graphNewton:
	graph -T ps < InterpolationNewton.dat > InterpolationNewton.ps

graphAll:
	graph -T ps < InterpolationPolynomialGSL.dat > InterpolationPolynomialGSL.ps
	graph -T ps < InterpolationLagrange.dat > InterpolationLagrange.ps
	graph -T ps < InterpolationNewton.dat > InterpolationNewton.ps

graphAllAtOne:
	graph -T ps -C -m 1 < InterpolationPolynomialGSL.dat > InterpolationAllAtOne.ps
	graph -T ps -C -m 2 < InterpolationLagrange.dat > InterpolationAllAtOne.ps
	graph -T ps -C -m 3 < InterpolationNewton.dat > InterpolationAllAtOne.ps

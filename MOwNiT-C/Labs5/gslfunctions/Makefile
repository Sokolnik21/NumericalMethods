compile:
	gcc -I/usr/local/include -L/usr/local/lib Interpolation.c -o Interpolation -lgsl -lgslcblas -lm

run:
	./Interpolation

graphPolynomial:
	graph -T ps < InterpolationPolynomialGSL.dat > InterpolationPolynomialGSL.ps

graphCSpline:
	graph -T ps < InterpolationCSpline.dat > InterpolationCSpline.ps

graphAkima:
	graph -T ps < InterpolationAkimaGSL.dat > InterpolationAkimaGSL.ps

graphAll:
	graph -T ps < InterpolationPolynomialGSL.dat > InterpolationPolynomialGSL.ps
	graph -T ps < InterpolationCSplineGSL.dat > InterpolationCSplineGSL.ps
	graph -T ps < InterpolationAkimaGSL.dat > InterpolationAkimaGSL.ps

compile:
	gcc -I/usr/local/include -L/usr/local/lib Interpolation.c -o Interpolation -lgsl -lgslcblas -lm

run:
	./Interpolation

graphCSpline:
	graph -T ps < InterpolationCSplineGSL.dat > InterpolationCSplineGSL.ps

# Makefile for ECE2036 NumericalIntegration  Assignment

all:	integration-float integration-double

integration-float:	integration-float.o  gthread.o
	$(CXX) -g -o integration-float integration-float.o  gthread.o -lpthread

integration-double:	integration-double.o  gthread.o
	$(CXX) -g -o integration-double integration-double.o  gthread.o -lpthread

integration-float.o:	integration.cc 
	$(CXX) -g -c -DMYFLOAT=float -o integration-float.o integration.cc

integration-double.o:	integration.cc 
	$(CXX) -g -c -DMYFLOAT=double -o integration-double.o integration.cc

gthread.o:	gthread.cc gthread.h
	$(CXX) -g -c gthread.cc

clean:
	@rm -f *.o integration-float integration-double


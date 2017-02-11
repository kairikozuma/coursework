# Makefile for ECE2036 Matrix class (Lab6) Assignment

test_matrix:	main.o matrix.o complex.o
	$CXX) -o test_matrix main.o matrix.o complex.o

main.o:		main.cc
		$(CXX) -c -g main.cc

matrix.o:	matrix.cc matrix.h
		$(CXX) -c -g matrix.cc

complex.o:	complex.cc complex.h
		$(CXX) -c -g complex.cc

clean:
		rm -f *.o test_matrix
		echo "Cleaned"


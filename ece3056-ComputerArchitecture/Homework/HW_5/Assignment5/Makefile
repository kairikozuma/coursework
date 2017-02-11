CFLAGS ?= -O3 -std=c99

cachesim: main.o cachesim.o -lm

cachesim.o: cachesim.c cachesim.h
main.o: main.c cachesim.h

clean:
	rm -f *.o *~ \#* cachesim

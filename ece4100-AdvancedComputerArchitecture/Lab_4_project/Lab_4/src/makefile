CFLAGS= -g -Wall -Wextra
SIM_SRC  = cache.cpp core.cpp dram.cpp memsys.cpp sim.cpp
SIM_OBJS = $(SIM_SRC:.cpp=.o)

all: $(SIM_SRC) sim

%.o: %.cpp
	g++ -Wall -c -o $@ $< $(CFLAGS)

sim: $(SIM_OBJS)
	g++ -Wall -o $@ $^ $(CFLAGS)

clean:
	rm sim *.o

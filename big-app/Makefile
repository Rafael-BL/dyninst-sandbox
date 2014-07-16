CFLAGS= -g -Wall -O0 -std=c++11

all: mutator

mutator: mutator.cpp
	g++ $(CFLAGS) -g $^ -ldyninstAPI -lsymtabAPI -ldl -o $@
clean:
	rm -f mutator


CFLAGS= -g -Wall -O0 -std=c++11

all: mutator mutatee

mutatee: mutatee.c
	gcc  $^ -lpthread -o $@
mutator: mutator.cpp
	g++ $(CFLAGS) -g $^ -ldyninstAPI -lpthread -o $@
clean:
	rm -f mutator dyn.so mutatee


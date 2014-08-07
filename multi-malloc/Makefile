CFLAGS= -g -Wall -O0 -std=c++11

all: mutator dyn.so mutatee

mutatee: mutatee.c
	gcc  $^ -o $@
mutator: mutator.cpp
	g++ $(CFLAGS) -g $^ -ldyninstAPI -o $@
dyn.so: dyn.c
	gcc -shared -fno-strict-aliasing -fPIC -DPIC $^ -ldl -o $@
clean:
	rm -f mutator dyn.so mutatee


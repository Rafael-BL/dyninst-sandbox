CFLAGS= -g -Wall -O0 -std=c++11

all: mutator mutatee

mutator: mutator.cpp
	g++ $(CFLAGS) -g $^ -ldyninstAPI  -o $@
mutatee: mutatee.c
	gcc -g -Wall -O2 $^ -o $@
clean:
	rm -f mutator mutatee
run: mutator mutatee
	env DYNINSTAPI_RT_LIB=/usr/lib/libdyninstAPI_RT.so  ./mutator

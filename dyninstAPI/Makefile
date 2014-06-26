all: mutator dyn.so

mutator: mutator.cpp
	g++ -g $^ -ldyninstAPI -lsymtabAPI -o $@
dyn.so: dyn.c
		gcc -shared -fno-strict-aliasing -fPIC -DPIC $^ -ldl -o $@
clean:
	rm -f mutator dyn.so


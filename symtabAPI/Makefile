CFLAGS= -g -Wall -O0
all: inspector

inspector: inspector.cpp
	g++ $(CFLAGS) inspector.cpp -lsymtabAPI -ldyninstAPI -o inspector
clean:
	rm suspect suspect.o my-lib.o inspector

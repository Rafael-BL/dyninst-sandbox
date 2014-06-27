CFLAGS= -g -Wall -O0

all: suspect
	cd symtabAPI && make && cd ..
	cd dyninstAPI && make && cd ..
suspect: suspect.c 
	gcc $(CFLAGS) suspect.c -o suspect
clean:
	rm -f  suspect
	cd symtabAPI && make clean && cd ..
	cd dyninstAPI && make clean && cd ..

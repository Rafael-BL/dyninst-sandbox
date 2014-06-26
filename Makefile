CFLAGS= -g -Wall -O0

all: suspect
	cd symtabAPI && make && cd ..
suspect: suspect.c 
	gcc $(CFLAGS) suspect.c -o suspect
clean:
	rm suspect

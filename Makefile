CFLAGS=-g -Wall -O0
all: suspect
	cd symtabAPI && make && cd ..
	cd dyninstAPI && make && cd ..
	cd big-app && make && cd ..
	cd list-functions && make && cd ..
debug-file: suspect
	objcopy --only-keep-debug $^ $@
	strip --strip-debug --strip-unneeded $^
	objcopy --add-gnu-debuglink=debug-file $^
suspect: suspect.c
	gcc $(CFLAGS) suspect.c -o suspect
clean:
	rm -f suspect
	cd symtabAPI && make clean && cd ..
	cd dyninstAPI && make clean && cd ..
	cd big-app && make clean && cd ..
	cd list-functions && make clean && cd ..

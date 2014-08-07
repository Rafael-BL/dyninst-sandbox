CFLAGS= -g -Wall -O0
all: suspect
	cd symtabAPI && make && cd ..
	cd dyninstAPI && make && cd ..
	cd big-app && make && cd ..
	cd allocate-str && make && cd ..
	cd max-malloc && make && cd ..
	cd onetimecode && make && cd ..
	cd list-functions && make && cd ..
	cd list-params && make && cd ..
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
	cd allocate-str && make clean && cd ..
	cd max-malloc && make clean && cd ..
	cd onetimecode && make clean && cd ..
	cd list-functions && make clean && cd ..
	cd list-params && make clean && cd ..
run: suspect
	lttng create
	lttng enable-event -a -u -x ust_baddr_statedump:soinfo,lttng_ust_tracef:event
	lttng enable-event "tp:quebec" --function ./suspect@quebec -u
#	lttng enable-event "dyn:sherbrook" --function ./suspect@sherbrook -u
	lttng start
	env LTTNG_UST_DEBUG=1 LTTNG_UST_REGISTER_TIMEOUT=-1 LD_PRELOAD=/usr/local/lib/liblttng-ust.so ./suspect
	lttng stop
	lttng view


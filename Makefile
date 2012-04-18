CFLAGS=-I/usr/include/python2.7 -fPIC -O2 -g -std=gnu99 -Wl,-R. -Wall -march=native -mtune=native

bench: bench.c libmycaller.so libmycallable.so libinterned.so
	gcc $(CFLAGS) -o bench -L. bench.c -lmycallable -lmycaller -linterned -lrt

libmycallable.so: mycallable.c Makefile cep1000test.h
	gcc $(CFLAGS) -S -o mycallable.S mycallable.c
	gcc $(CFLAGS) -shared -o libmycallable.so mycallable.c

libmycaller.so: mycaller.c Makefile cep1000test.h
	gcc $(CFLAGS) -S -o mycaller.S mycaller.c
	gcc $(CFLAGS) -shared -o libmycaller.so mycaller.c

libinterned.so: interned.c Makefile
	gcc $(CFLAGS) -shared -o libinterned.so interned.c

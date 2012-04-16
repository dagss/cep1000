CFLAGS=-I/usr/include/python2.7 -fPIC -O2 -g -std=gnu99 -Wl,-R. -Wall -march=native -mtune=native

bench: bench.c libmycaller.so libmycallable.so
	gcc $(CFLAGS) -o bench -L. bench.c -lmycallable -lmycaller -lrt

libmycallable.so: mycallable.c Makefile
	gcc $(CFLAGS) -shared -o libmycallable.so mycallable.c

libmycaller.so: mycaller.c Makefile
	gcc $(CFLAGS) -shared -o libmycaller.so mycaller.c


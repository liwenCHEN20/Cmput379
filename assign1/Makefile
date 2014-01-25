memchunk: memchunk.o
	gcc -Wall -std=c99 -o memchunk memchunk.o

memchunk.o: memchunk.c memchunk.h
	gcc -Wall -std=c99 -c memchunk.c
	gcc -Wall -std=c99 -c memchunk.h

clean:
	rm -rf *.o memchunk
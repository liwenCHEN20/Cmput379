memchunk: memchunk.o
	gcc -Wall -m32 -o memchunk memchunk.o

memchunk.o: memchunk.c memchunk.h
	gcc -Wall -m32 -c memchunk.c
	gcc -Wall -m32 -c memchunk.h

clean:
	rm -rf *.o memchunk

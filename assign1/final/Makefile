testprogram: testprogram.c memchunk.o
	gcc -Wall -m32 -o testprogram testprogram.c memchunk.c

memchunk.o: memchunk.c memchunk.h
	gcc -Wall -m32 -c memchunk.c
	gcc -Wall -m32 -c memchunk.h

clean:
	rm -rf *.o testprogram

#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "memchunk.h"

void main(int argc, char **argv) 
{
	int page_size = getpagesize();
	printf("Page size: %d\n", page_size);

}
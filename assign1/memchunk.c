#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "memchunk.h"

static sig_atomic_t RW_code = 0;
static sigjmp_buf env;

static void segfault_handler(int);


int main(int argc, char **argv)
{
	struct memchunk chunk_list[100];
	int size = 100;
	int actual_size = get_mem_layout(chunk_list, size);
	printf("total number of chunks: %d\n", actual_size);
	return 0;
}


int get_mem_layout (struct memchunk *chunk_list, int size)
{
	/* Variable declarations */
	unsigned long aux;
	int chunk_counter;
	int chunk_RW;	
	int page_size;
	int prev_RW;
	int save_chunk;
	int start_new_chunk;
	char arbitrary;
	char *addr;
	char *start_addr;


	/* Variable initializations */
	chunk_counter = 0;
	page_size = getpagesize();
	prev_RW = 0;
	save_chunk = 0;
	start_new_chunk = 0;
	addr = (char *)0xFFFFF000;


	/* Setting up the segfault handler function */
	struct sigaction act;
    	act.sa_handler = segfault_handler;
	sigemptyset(&act.sa_mask);
    	act.sa_flags = 0;
    	sigaction(SIGSEGV, &act, 0);


	/* Main loop */
	while (1)
	{
		/* segfault_handler returns control here */
		sigsetjmp(env,1);
	

		if (RW_code != prev_RW)
		{
			/* Place the chunk in the list */	
			if ((save_chunk)&&(chunk_counter <= size))
			{
				aux = ((addr - start_addr)/page_size);

				struct memchunk chunk;
				chunk.start = start_addr;
				chunk.length = aux;
				chunk.RW = chunk_RW;
				chunk_list[chunk_counter] = chunk;
								
				save_chunk = 0;

				printf("start = %p, length = %lu, RW = %d\n", chunk.start, chunk.length, chunk.RW);
			}
			
			if ((RW_code == 2)||(RW_code == 3))
			{
				save_chunk = 1;
				start_new_chunk = 1;
			}

			/* Record the starting address and the permission*/
			if (start_new_chunk)
			{
				chunk_counter++;
				start_addr = addr;
				
				if (RW_code == 2)
				chunk_RW = 0;
				else if (RW_code == 3)
				chunk_RW = 1;
				start_new_chunk = 0;
			}
	
		}

		prev_RW = RW_code;

		/* Think as if the loop execution starts here */

		addr = addr + page_size;
		
		if ((unsigned int)addr + page_size < (unsigned int)addr)
			break; /* Exit when address overflow is detected */
		else
		{ 
			RW_code = 1;
			arbitrary = *addr;		/* Read from memory */
			RW_code = 2;
			*addr = 'c';			/* Write to memory */
			RW_code = 3;
			*addr = arbitrary;		/* Set value back*/
		}
	}

	return chunk_counter;	
}



void segfault_handler(int sig) 
{
	siglongjmp(env, 1);
}

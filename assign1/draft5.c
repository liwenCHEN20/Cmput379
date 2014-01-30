#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "memchunk.h"

static int fail_to_read;
static int fail_to_write;
static int RW_code = 0; //sig_atomic_t RW_code;
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

	//the below code will have to be written somewhere else or deleted 
	////////////////
	
	
	////////////////	

	//char *address = 0;
	

	char *address = (char *)0xFFFFF000;
	int page_size = getpagesize();
	char arbitrary;
	int chunk_RW;
	int prev_RW = 0;
	//char *prev_address;
	int start_new_chunk = 0;
	char *start_address;
	//unsigned long length = 0;
	int chunk_counter = 0;
	int save_chunk = 0;
	//int RW;

	/* Setting up the segfault handler function */
	struct sigaction act;
    	act.sa_handler = segfault_handler;
	sigemptyset(&act.sa_mask);
    	act.sa_flags = 0;
    	sigaction(SIGSEGV, &act, 0);


	
	

	while (1)
	{
	
		sigsetjmp(env,1);		/* Control is given back to this point after segfault_handler*/
	

		if (RW_code != prev_RW)
		{
			
			if (save_chunk)
			{
				struct memchunk chunk;
				chunk.start = start_address;
				chunk.length = (unsigned long) ((address - start_address)/page_size);
				chunk.RW = chunk_RW;
				chunk_list[chunk_counter] = chunk;
				chunk_counter++;				
				save_chunk = 0;

				printf("start = %p, length = %lu, RW = %d\n", chunk.start, chunk.length, chunk.RW);
			}
			
			if ((RW_code == 2)||(RW_code == 3))
			{
				save_chunk = 1;
				start_new_chunk = 1;
			}


			if (start_new_chunk)	//Keep track of the starting address and its permission code
			{
				start_address = address;
				
				if (RW_code == 2)
				chunk_RW = 0;
				else if (RW_code == 3)
				chunk_RW = 1;
		
			}


			
		}




		prev_RW = RW_code;	//this should stay here, already thought of...tabun


		/* Think as if the loop execution starts here */




		address = address + page_size;
		
		if ((unsigned int)address + page_size < (unsigned int)address)
			break; /* Exits when address overflow is detected */
		else
		{ 

			fail_to_read = 0;
			fail_to_write = 0;
			RW_code = 1;
			arbitrary = *address;		/* Read from memory */
			RW_code = 2;
			*address = 'c';			/* Write to memory */
			RW_code = 3;
			*address = arbitrary;		/* Permission to write, set value back to original */
		
		}
	}

	return chunk_counter;
	
}




void segfault_handler(int sig) 
{
	
	if (RW_code == 1)
	{
		fail_to_read = 1;
		fail_to_write = 1;
	}
	else if (RW_code == 2)
	{
		fail_to_write = 1;
		fail_to_read = 0;
	}
	// Not gonna be called anyway...	
	else if (RW_code == 3)
	{
		fail_to_write = 0;
		fail_to_read = 0;
	}
		

	siglongjmp(env, 1);
}

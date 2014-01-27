#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int fail_to_read;
static int fail_to_write;
static int RW_code; //sig_atomic_t RW_code;
static sigjmp_buf env;

static void segfault_handler(int);


int main(int argc, char **argv)
{
	//char *address = 0;
	char *address = (char *)0xFFFFF000;
	int page_size = getpagesize();
	char arbitrary;
	int prev_RW;
	char *prev_address;

	//char *start_address;
	unsigned long length = 0;
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
	

		if ((RW_code == 2)||(RW_code == 3))
		{
			
			
			if (RW_code != prev_RW)
			{

				//print current chunk (for testing purposes)

				//start_new_chunk = 1;
			}


			//if (start_new_chunk == 1)
			//i.e. initialize start_address as the current address,
			//set length counter of the previous chunk to 0,
			//set RW in the new chunk struct to the current RW_code:
			/*
			if (RW_code == 2)
			RW = 0;
			else if (RW_code == 3)
			RW = 1;
			*/
			//start_new_chunk = 0;	reset start_new_chunk flag
			
				
			

			// increment memchunk.length++;
			



			
		}






		






		prev_RW = RW_code;	//this should stay here, already thought of...tabun


		/* Think as if the loop execution starts here */

		address = address + page_size;
		
		if ((unsigned int)address + page_size < (unsigned int)address)
			exit(1); /* Exits when address overflow is detected */
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

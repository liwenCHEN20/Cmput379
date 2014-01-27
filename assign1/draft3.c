#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static int fail_to_read;
static int fail_to_write;
static int RW_code;
static sigjmp_buf env;

static void segfault_handler(int);


int main(int argc, char **argv)
{
	//char *address = 0;
	char *address = (char *)0xFFFFF000;
	int page_size = 4096;
	//address = address - page_size;
	char arbitrary;


	/* Setting up the segfault handler function */
	struct sigaction act;
    	act.sa_handler = segfault_handler;
	sigemptyset(&act.sa_mask);
    	act.sa_flags = 0;
    	sigaction(SIGSEGV, &act, 0);


	
	

	while (1)
	{
	
		sigsetjmp(env,1);		/* Control is given back to this point after segfault_handler*/
	
		///////// the code dealing with chunks should maily be under this condition
		if ((RW_code == 2)||(RW_code == 33333))	//&&(RW_code != 0))
			printf("%p RW_code = %d\n", address, RW_code);

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
			RW_code = 33333;
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
	else if (RW_code == 33333)
	{
		fail_to_write = 0;
		fail_to_read = 0;
	}
		

	siglongjmp(env, 1);
}

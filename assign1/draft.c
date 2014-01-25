#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int fail_to_read;
static int fail_to_write;
static int RW_code;
static sigjmp_buf env;

static void segfault_handler(int);


int main(int argc, char **argv)
{
	char *address = 0;
	int page_size = 4096;
	char arbitrary;
	int permission;
	/*
	int page_size = getpagesize();
	int unsigned address_bound = 0xFFFFFFFF;
	int last_address = address_bound - page_size;
	printf("%d\n", last_address);
	*/


	struct sigaction act;
    act.sa_handler = segfault_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    
	
	while (1)//((int)address < 0xFFFFF000)
	{
		
		int x = sigsetjmp(env,1); ///////////////////

		fail_to_read = 0;
		fail_to_write = 0;
		RW_code = 1;
		arbitrary = *address;	/* Read from memory */
		RW_code = 2;
		*address = 'c';


		if (x == 0)
		{
			if (fail_to_write == 1)
				permission = 0;
			else if (fail_to_read == 1)
				permission = -1;
			else
				permission = 1;	
		}
		
		if (permission == -1)
			printf("%p RW = -1\n", address);
		else if (permission == 0)
			printf("%p RW = 0\n", address);
		else
			printf("%p RW = 1\n", address);

		

		if ((unsigned int)address + page_size < (unsigned int)address)
			break;


		address = address + page_size;
	}
	
}


void segfault_handler(int sig)
{
	if (RW_code == 1)
	{
		fail_to_read = 1;
		fail_to_write = 0;
	}

	else if (RW_code == 2)
	{
		fail_to_write = 1;
		fail_to_read = 0;
	}

	siglongjmp(env, 1);

}
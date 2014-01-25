#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static int fail_to_read;
static int fail_to_write;
static int RW_code;

static sigjmp_buf env;
static void segfault_handler(int);



int main() {

	char *address;
	int page_size = 4096;
	int x;
	char arbitrary;
	int permission;
	int start_parsing = 0;
	address = (char *) 0xFFFFF000; 
 
	printf("Program Starting\n");
	
	signal(SIGSEGV, segfault_handler);
	
	
		
	x = sigsetjmp(env,1);

	
	


    address = address + page_size;
	//printf("%p\n", address);	

	if (start_parsing)
	{
		if (fail_to_write == 1)
			permission = 0;
		else if (fail_to_read == 1)
			permission = -1;
		else
			permission = 1;	
	

	if (permission == -1)
		printf("%p RW = -1\n", address);
	else if (permission == 0)
		printf("%p RW = 0\n", address);
	else
		printf("%p RW = 1\n", address);

	}



	if((unsigned int)address + page_size < (unsigned int)address)
		exit(1);
	else
	{ 
		//raise(SIGSEGV);
		start_parsing = 1;
		fail_to_read = 0;
		fail_to_write = 0;
		RW_code = 1;
		arbitrary = *address;	
		RW_code = 2;
		*address = 'c';
	}

	

    //exit(0);
}

void segfault_handler(int signo) {
	//printf("SIGSEGV handler ...\n");

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

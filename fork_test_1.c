# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>


/*
 * System call fork() is used to create processes. It takes no arguments and returns 
 * a process ID. The purpose of fork() is to create a new process, which becomes the
 * child process of the caller. After a new child process is created, both processes
 * will execute the next instruction following the fork() system call.
 */


int main(void)
{
	fork();
	printf("Hello\n");
	fork();
	printf("Bye\n");
	return 0;
}

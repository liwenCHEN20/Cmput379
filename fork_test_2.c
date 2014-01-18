#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


/*
 * After a process has been created with fork(), the exec() system call overlays the memory space of
 * that process with another (a new) program, or the process remains a copy of the parent process.
 * However, since it's not a process that is created but rather just the memory of an already existing
 * process that is overlayed, the process ID remais the same.
 *
 * In the below example the child process created with fork() is recogmized with (pid == 0) and is 
 * overlayed ("transformed") by the pwd program with the execlp system call.
 * The instruction "printf("%d", pid);" after the execlp is not called because after a successful
 * exec system call, the process is killed. Hence, there is no child process to do the printf()?
 * But then the printf() instruction before the execlp is not called either. Why?
 */


int main()
{
	pid_t pid;					/* declare a process ID (of type pid_t) */
	
	pid = fork();				/* fork a child process */

	if (pid < 0)				/* error occurred in process creation */ 
	{
	fprintf(stderr, "Fork Failed");
	return 1;
	}
	
	else if (pid == 0)			/* child process */ 
	{
	printf("%d\n", pid);
	//execlp("/bin/pwd","pwd",NULL);
	//printf("%d\n", pid);
	}
	
	else						/* parent process */ 
	{
	wait(NULL);					/* parent will wait for the child to complete */
	printf("Child Complete");
	}

	return 0;
}

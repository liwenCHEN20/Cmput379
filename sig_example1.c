# include <signal.h>
# include <stdio.h>
# include <unistd.h>

void ouch(int sig)
{
	printf("OUCH -I got signal %d\n", sig);
	(void) signal(SIGINT, SIG_DFL);				/* The signal SIGINT is set back to its original function, which is to exit the process*/
	(void) signal(SIGTSTP, SIG_DFL);			/* The signal SIGTSTP is set back to its original function, which is to exit the process too*/
}												/* If you deleted either of the above, calling the interrupt will remain ouch, i.e. printing OUCH...*/

int main()
{
	(void) signal(SIGINT, ouch);				/* The signal SIGINT (Ctrl+C) is set to be handled by ouch()*/
	(void) signal(SIGTSTP, ouch);				/* The signal SIGTSTP (Ctrl+Z) is set to be handled by ouch() as well*/
	while(1)
	{
		printf("Hello World!\n");
		sleep(1);
	}
}
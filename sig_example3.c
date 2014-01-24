# include <signal.h>
# include <stdio.h>
# include <setjmp.h>

static jmp_buf env;

void sig_handler(int signo)
{
	longjmp(env,1);
}

int main()
{
	(void) signal(SIGSEGV, sig_handler);

	if(!setjmp(env))
		printf("Jump marker set\n");
	else
	{
		printf("Segmentation fault occurred");
		exit(-1);
	}

	char *str = "Hello";
	str[2] = 'Z';
}
# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>

int main(void)
{
	printf("Hello\n");
	fork();
	printf("Bye\n");
	return 0;
}

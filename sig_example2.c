# include <signal.h>
# include <stdio.h>
# include <unistd.h>


/* 
 *signal() and sigaction() are similar to each other. However, we tend to prefer 
 * using sigaction() because it is more robust: 
 * - sigaction() can block other signals until the current handler returns, while
 *   signal() does not block other signals from arriving while the current handler
 *   is executing.
 * - signal() resets the signal action back to SIG_DL for almost all signals. This
 *   means the signal() handler must reinstaall itself at its first action. It 
 *   therefore opens up a window of vulnerability between the time when the signal 
 *   is detected and the handler is reinstalling during which if a second instance 
 *   of the signal arrives, the default behaviour terminates. 
 * signal() was created first, therefore has availability in its favour. But it is
 * less robust.
 */

 /*
  * struct sigaction
  * {
  *		void (*) (int) sa_handler		// function, SIG_DFL, SIG_IGN
  *		sigset_t sa_mask				// signals to block in sa_handler
  *		int sa_flags					// signal action modifiers
  * }
  *
  */


void ouch(int sig)
{
	printf("OUCH! - I got signal %d\n", sig);
}

int main()
{
	struct sigaction act;

	act.sa_handler = ouch;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0);

	while (1)
	{
		printf("Hello World!\n");
		sleep(1);
	}

}
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dirent.h>


#define BUF_ASIZE 256
#define MAXCONN 256


/* states used in struct con. */
#define STATE_UNUSED 0
#define STATE_READING 1
#define STATE_WRITING 2






struct con {
	int sd; 	/* the socket for this connection */
	int state; 	/* the state of the connection */
	struct sockaddr_in sa; /* the sockaddr of the connection */
	size_t  slen;   /* the sockaddr length of the connection */
	char *buf;	/* a buffer to store the characters read in */
	char *bp;	/* where we are in the buffer */
	size_t bs;	/* total size of the buffer */
	size_t bl;	/* how much we have left to read/write */
	int req_valid;	/* the code of the request: 200, 400, 403, 404, 500 */
};


struct input_arg 
{
	int port;
	char *logfile;
	char *reqdir;
};

struct con connections[MAXCONN];
struct input_arg args;

static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(1);
}

static void kidhandler(int signum) {
	/* signal handler for SIGCHLD */
	waitpid(WAIT_ANY, NULL, WNOHANG);
}


struct con * get_free_conn()
{
	int i;
	for (i = 0; i < MAXCONN; i++) {
		if (connections[i].state == STATE_UNUSED)
			return(&connections[i]);
	}
	return(NULL); /* we're all full - indicate this to our caller */
}




void check_inputs(struct input_arg *args, int argc, char **argv)
{
	int portnb;
	char *log;
	char *req;

	//printf("nb args: %d\n", argc);
	
	if (argc != 4)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		exit(0);
	}
	
	
	/* checking the existence of the request directory */
	if (opendir(argv[2]) == NULL)
	{
		fprintf(stderr, "The directory does not exist\n");
		exit(0);
	}
	

	/* Store each argument in auxiliary variables */
	portnb = atoi(argv[1]);

	req = (char*) malloc(strlen(argv[2]));
	sscanf(argv[2], "%s", req);		

	log = (char*) malloc(strlen(argv[3]));
	sscanf(argv[3], "%s", log);
	
	
	/* Attributing each argument in our input_arg structure args */
	args->port = portnb;
	args->logfile = log;
	args->reqdir = req;
	
	
	//printf("%d\n", args->port);
	//printf("%s\n", args->logfile);
	//printf("%s\n", args->reqdir);	
}



void handle_read(struct con *cp)
{
		
}



void check_request(struct con *cp)
{
	
	
}







int main(int argc,  char *argv[])
{
	struct sockaddr_in sockname, client;
	char buffer[80], *ep;
	struct sigaction sa;
	socklen_t clientlen;
        int sd;
	u_short port;
	pid_t pid;
	u_long p;


	check_inputs(&args, argc, argv);


	/*
	 * first, figure out what port we will listen on - it should
	 * be our first parameter.
	 */
	/*
	if (argc != 2)
		usage();
		errno = 0;
        p = strtoul(argv[1], &ep, 10);
        if (*argv[1] == '\0' || *ep != '\0') {
		/* parameter wasn't a number, or was empty 
		fprintf(stderr, "%s - not a number\n", argv[1]);
		usage();
	}
        if ((errno == ERANGE && p == ULONG_MAX) || (p > USHRT_MAX)) {
		/* It's a number, but it either can't fit in an unsigned
		 * long, or is too big for an unsigned short
		 
		fprintf(stderr, "%s - value out of range\n", argv[1]);
		usage();
	}
	/* now safe to do this 
	port = p;
	*/


	



	/* the message we send the client */
	strncpy(buffer,
	    "What is the air speed velocity of a coconut laden swallow?\n",
	    sizeof(buffer));

	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(args.port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);
	sd=socket(AF_INET,SOCK_STREAM,0);
	if ( sd == -1)
		err(1, "socket failed");

	if (bind(sd, (struct sockaddr *) &sockname, sizeof(sockname)) == -1)
		err(1, "bind failed");

	if (listen(sd,3) == -1)
		err(1, "listen failed");

	/*
	 * we're now bound, and listening for connections on "sd" -
	 * each call to "accept" will return us a descriptor talking to
	 * a connected client
	 */


	/*
	 * first, let's make sure we can have children without leaving
	 * zombies around when they die - we can do this by catching
	 * SIGCHLD.
	 */
	sa.sa_handler = kidhandler;
        sigemptyset(&sa.sa_mask);
	/*
	 * we want to allow system calls like accept to be restarted if they
	 * get interrupted by a SIGCHLD
	 */
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1)
                err(1, "sigaction failed");

	/*
	 * finally - the main loop.  accept connections and deal with 'em
	 */
	printf("Server up and listening for connections on port %u\n", args.port);


	
	/* We daemonize the program */
	if (daemon(1, 1) == -1)
		err(1, "daemon() failed");




	for(;;) {
		int clientsd;
		clientlen = sizeof(&client);
		clientsd = accept(sd, (struct sockaddr *)&client, &clientlen);
		if (clientsd == -1)
			err(1, "accept failed");
		


		struct con *cp;
		cp = get_free_conn();





		/*
		 * We fork child to deal with each connection, this way more
		 * than one client can connect to us and get served at any one
		 * time.
		 */		
		pid = fork();
		if (pid == -1)
		     err(1, "fork failed");

		if(pid == 0) 
		{
			if (cp == NULL)
			{
				//char* reject_str = "cannot open connection";
				//write(clientsd, reject_str, strlen(reject_str));
				close(clientsd);
				continue;
			}
			else
			{
				cp->sd = clientsd;
				//cp->time = time(NULL);
				cp->slen = sizeof(client);				
				cp->state = STATE_READING;
				memcpy(&cp->sa, &client, sizeof(client));
				

				handle_read(cp);	/* get the request in the con struct */
				check_request(cp);	/* check validity of the request*/
		
			}

			/*			
			ssize_t written, w;
			/*
			 * write the message to the client, being sure to
			 * handle a short write, or being interrupted by
			 * a signal before we could write anything.
			 *
			w = 0;
			written = 0;
			while (written < strlen(buffer)) {
				w = write(clientsd, buffer + written,
				    strlen(buffer) - written);
				if (w == -1) {
					if (errno != EINTR)
						err(1, "write failed");
				}
				else
					written += w;
			}
			*/
			printf("Tralala\n");
			exit(0);
		}
		close(clientsd);
	}
}

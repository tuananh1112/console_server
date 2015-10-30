#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "mysql_update_usb.h"

#define FILE_PID "/root/test.pid"
#define SEGMENT 10000
#define TRUE 1
#define SLEEP_TIME 1000
#define SUCCESS 0
#define ERROR 1

void sig_set_handler (int signo, void *handler);
void signal_handler (int sig, siginfo_t *siginfo, void *context);
void make_pidfile(char *pidfile);

int main( void )
{
    make_pidfile(FILE_PID);
    sig_set_handler (SIGUSR1, &signal_handler);
    while (TRUE)
    {
        usleep (SLEEP_TIME);
    }

    return SUCCESS;
}

void sig_set_handler (int signo, void *handler)
{
    struct sigaction *act;
    act = malloc (sizeof (struct sigaction));
    if(NULL == act)
    {
        printf("ERROR memmory alocate!!!\n");
        exit(ERROR);
    }

    act->sa_sigaction = handler;
    act->sa_flags = SA_SIGINFO;
    
    sigaction(signo, act, NULL);
}

void signal_handler(int sig, siginfo_t *siginfo, void *context)
{
    int port, state;
    printf("\n rec signal with info \n");
    printf("\n value is = %d \n",siginfo->si_value);

	/* Seperate port and state */
    port = (siginfo->si_value.sival_int)/10 + SEGMENT;
    state = siginfo->si_value.sival_int % 10;

	/* Update to database */
    mysql_update_usb(port,state);
}

/*
 * Get pid of this process
 */
void make_pidfile(char *pidfile)
{
    FILE *fpidfile;
    if (0 == pidfile)
        return ERROR;
    fpidfile = fopen(pidfile, "w");
    if (0 == fpidfile) 
    {
        return ERROR;
    }
    fprintf(fpidfile, "%d\n", getpid());
    fclose(fpidfile);
}

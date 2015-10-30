#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define PATH "/root/test.pid"
#define SUCCESS 0
#define ERROR 1

int read_pid (char *pidfile);
void signal_gen (pid_t id, int data);
void sig_send_msg (pid_t id, int signo, char *msg);
void sig_send_val (pid_t id, int signo, int val);

int main(char argc, char *argv[])
{

    int fd;
    int id, data;
    id = read_pid(PATH);
    data = atoi(argv[1]);
    signal_gen(id, data);
    _exit(SUCCESS);
}

/*
 * Read pid of the process will receive signal SIGUSR1
 * */
int read_pid (char *pidfile)
{
    FILE *f;
    int pid;

    if (!(f=fopen(pidfile,"r")))
    return SUCCESS;
    fscanf(f,"%d", &pid);
    fclose(f);
    return pid;
}
void signal_gen (pid_t id, int data)
{
        sig_send_val (id, SIGUSR1, data );
}

/*
 * Send data to another process 
 */
void sig_send_val (pid_t id, int signo, int val)
{
    union sigval *sigdata;
    sigdata = malloc (sizeof(union sigval));
    if(NULL == sigdata)
    {
        printf("ERROR mommory alocate!!! \n");
        exit(ERROR);
    }
    sigdata->sival_int = val;
    sigqueue(id,signo, *sigdata);
    free (sigdata);
}





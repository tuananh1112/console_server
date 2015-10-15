#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "checkconnect.h"

#define TIMEOUT 3
#define BUF_LEN 1

int 
checkconnect (char *devname)
{
    int fd = 0;
    struct timeval tv;
    fd_set readfds;
    int ret;
	int r;
    char *s = malloc (sizeof(char));

	if (NULL == devname)
	{
		perror ("DEVNAME NULL\n");
		return -1;
	}

    fd = open(devname, O_RDWR | O_NONBLOCK);
    
	if (fd < 0)
    {
        perror("open\n");
        return -1;
    }
    
	FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    /*set timeout */
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

	/* non-block fd */

    /* select */ 
    r = write(fd,"h",BUF_LEN);

    ret = select (fd +1, &readfds, NULL, NULL, &tv);

    if (ret == -1)
    {
        perror ("select");
        return -1;
    }
    else if (0 == ret)
    {
        printf("Time out: %s is die!\n", devname);
		return 0;
    }

	printf("Connect is ok!\n");
    return 0;
}

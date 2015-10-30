#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "checkconnect.h"

#define DEV_LEN 50
#define SUR_LEN 20

int check_connect(int port)
{
    char devname[DEV_LEN];
    char surfix[SUR_LEN];
    char *prefix;
    prefix = "/dev/FT-";
    int fd = 0;
    struct timeval tv;
    fd_set readfds;
    int ret;
    int r = 0;
   
    /* Get devname from port*/
    strcpy(devname, prefix);
    sprintf(surfix, "%d", port);
    strcat (devname, surfix);

    fd = open(devname, O_RDWR | O_NONBLOCK);
    
    if (0 > fd)
    {
        error("Open FD failed");
    }
    
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    /*Set timeout*/
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    
    r =  write(fd,"h",MSG_LENGTH);
    if (0 == r)
    {
        error ("Write failed");
    }
    
    /*Wait available data in dev_fd*/
    ret = select (fd + 1, &readfds, NULL, NULL, &tv);

    if (-1 == ret)
    {
        error("Select");
    }
    else if (0 == ret)
    {
        return DISCONECT;
    }
    else
    {
        return CONNECTED;
    }
}








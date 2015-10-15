#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>

#include "checkconnect.h"

#define MAX_LINE_SIZE 256

int 
readconfig (char *filename)
{
	FILE *instream;
	char inbuf[MAX_LINE_SIZE];
	int lineno = 0;

	instream = fopen(filename, "r");
	if (NULL == instream)
	{
		printf("haksf\n");
		syslog(LOG_ERR, "Unable to open config file '%s'", filename);
		return -1;
	}

	while (fgets(inbuf, MAX_LINE_SIZE, instream) != NULL)
	{
		int len = strlen(inbuf);
		if (inbuf[len-1] != '\n')
		{
			lineno ++;
			syslog(LOG_ERR, "line %d is too long in config file", lineno);
			continue;
		}
		/*Remove the '\n'*/
		inbuf[len-1] = '\0';
		handle_config_line(inbuf, lineno);
	}
	
	fclose(instream);
	return 0;
}


void
handle_config_line(char *inbuf, int lineno)
{
	char *portnum, *state, *timeout, *devname, *devcfg;
	char *strtok_data = NULL;
	char *errstr;
	
	if ('#' == inbuf[0])
	{
		/*Ignore comments*/
		return; 
	}

	portnum = strtok_r(inbuf, ":", &strtok_data);
	if (NULL == portnum)
	{
		/*An empty line is ok */
		return;
	}

	/*Ignore un-related lines */
	
	if (0 == strcmp(portnum, "CONTROLPORT"))
	{
		return;
	}

	if (0 == strcmp(portnum, "BANNER"))
	{
		return;
	}

	if (0 == strcmp(portnum, "SIGNATURE"))
	{
		return;
	}

	if (0 == strcmp(portnum, "OPENSTR"))
	{
		return;
	}

	if (0 == strcmp(portnum, "CLOSESTR"))
	{
		return;
	}
	
	if (0 == strcmp(portnum, "TRACEFILE"))
	{
		return;
	}

	state = strtok_r (NULL, ":", &strtok_data);
	if (NULL == state)
	{
		syslog (LOG_ERR, "No state given on line %d", lineno);
		return;
	}

	timeout  = strtok_r (NULL, ":", &strtok_data);
	if (NULL == timeout)
	{
		syslog (LOG_ERR, "No timeout given on line %d", lineno);
		return;
	}
	
	devname = strtok_r (NULL, ":", &strtok_data);
	if (NULL == devname)
	{
		syslog (LOG_ERR, "No devname given on line %d", lineno);
		return;
	}
	
	devcfg = strtok_r (NULL, ":", &strtok_data);
	if (NULL == devcfg)
	{
		return;
	}

	printf("Device: %s\n",devname);
	checkconnect(devname);
}
















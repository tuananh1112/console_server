#ifndef CHECKCONNECT
#define CHECKCONNECT

void handle_config_line(char *inbuf, int lineno);

int readconfig(char *filename);

int checkconnect(char *devname);
#endif

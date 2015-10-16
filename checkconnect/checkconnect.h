#ifndef CHECKCONNECT
#define CHECKCONNECT

void handle_config_line (const char *inbuf, int lineno);

int readconfig (const char *filename);

int checkconnect (const char *devname);
#endif

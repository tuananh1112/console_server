#include <stdio.h>
#include "checkconnect.h"

int main()
{
	static char *config_file = "/etc/ser2net.conf";
	readconfig(config_file);
	return 0;
}

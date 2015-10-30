#ifndef _MYSQL_UPDATE_USB_
#define _MYSQL_UPDATE_USB_

#include <my_global.h>
#include <mysql.h>

#define SUCCESS 0
#define ERROR 1
#define COMM_LEN 512

/*
 * Exit when mysql query error
 * Then it close and exit
 */
void finish_with_error(MYSQL *con);

/*
 * Update to database when USB to Serial cable connected or disconneted
 * 1. Status = 0: Delete from website
 * 2. Status =1: Display on website
 * Update to database 
 */
void mysql_update_usb(int port_dev, int status);

#endif /* _MYSQL_UPDATE_USB_ */

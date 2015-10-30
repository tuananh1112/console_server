#include <stdio.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>

#include "mysql_update_usb.h"

/*
 * Exit when mysql query error
 * Then it close and exit
 */
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n",mysql_error(con));
    mysql_close(con);
    exit(ERROR);
}

/*
 * Update to database when USB to Serial cable connected or disconneted
 * 1. Status = 0: Delete from website
 * 2. Status =1: Display on website
 * Update to database 
 */
void mysql_update_usb(int port_dev,int status)
{

    MYSQL *con = mysql_init(NULL);
    
    char *server = "localhost";
    char *user = "root";
    char *password = "root123$%^";
    char *database = "dev_tmp";
    char port_str[10];
    
    char up_conn[COMM_LEN];
    char up_disconn[COMM_LEN];

    snprintf(up_conn, COMM_LEN, "UPDATE switch SET state = 'unconnected', \
            usb_active = 1, display_mode = 1 WHERE port = %d", port_dev);
    
    snprintf(up_disconn, COMM_LEN, "UPDATE switch SET state = 'unconnected', \
            usb_active = 0, display_mode = 0  WHERE port = %d", port_dev);
    
    /* Init fail */
    if (NULL == con)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(ERROR);
    }
    
    /* Connect to Database console */
    if (NULL == mysql_real_connect(con, server, user, password, database, \
        0, NULL, 0))
    {
       finish_with_error(con); 
    }
    
    /* Update information to Database */
    
    if (0 == status)
    {
        printf ("Disconnected\n");
        

        if (mysql_query(con, up_disconn))
        {
            finish_with_error(con);
        }
    } else
    {
        printf("Connected\n");

        if (mysql_query(con, up_conn))
        {
            finish_with_error(con);
        }
    }



}

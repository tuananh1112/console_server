#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>

#include "checkconnect.h"

/*
 * Close mysql and exit when mysql query error 
 */
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n",mysql_error(con));
    mysql_close(con);
    exit(ERROR);
}

/*
 * Update state of device to database
 * Connected or disconnected to USB to Serial cable
 */
void mysql_update_device()
{

    MYSQL *con = mysql_init(NULL);
    MYSQL_ROW row;

    /* Idenfify mysql database */
    char *server = "localhost";
    char *user = "root";
    char *password = "root123$%^";
    char *database = "dev_tmp";
    char port_str[10];
    
    int i, num_fields;
    int port_dev;
    int connected_dev;
    
	/* Command to update database */
    char str_conn[100] = "UPDATE switch SET display_mode = 1 \
                          WHERE port = ";
    char str_disconn[100] = "UPDATE switch SET display_mode = 2 \
                             WHERE port = ";
    char str_temp[100];
    
    /* Init fail */
    if (NULL == con)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(ERROR);
    }
    
    /* Connect to Database console */
    if (NULL == mysql_real_connect(con, server, user, password, \
                database, 0, NULL, 0))
    {
       finish_with_error(con); 
    }

    /* Select data from table */
    /* Select the ports with conditions: */
    /*    1. It's active - USB pluged*/
    /*    2. Has nobody telnet to */
    if (mysql_query(con,"SELECT port FROM switch WHERE state = \"unconnected\" \
                         AND usb_active = 1"))
    {
        finish_with_error(con);
    }
    
    MYSQL_RES *result = mysql_store_result(con);
    if (NULL == result)
    {
        finish_with_error(con);
    }

    while((row = mysql_fetch_row(result)))
    {
        printf("Find out: %s\n",row[0]);
        port_dev = atoi(row[0]);
        
        /* Check state of device whether is connected or not */
        connected_dev = check_connect(port_dev);

        /* Test case: assign device is disconnected */

        if (0 != connected_dev)
        {
            printf("Device at port %s is connected\n",row[0]);
            strcpy(str_temp, str_conn);
            strcat(str_temp, row[0]);
            if (mysql_query(con, str_temp))
            {
                finish_with_error(con);
            }


        } else
        {
            printf("Device at port %s is disconnected\n",row[0]);
            strcpy(str_temp, str_disconn);
            strcat(str_temp, row[0]);
            if (mysql_query(con, str_temp))
            {
                finish_with_error(con);
            }
        }
    }




}


int main()
{
    mysql_update_device();
    return 0;
}

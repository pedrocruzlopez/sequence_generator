#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define APP_NAME "Numeric Sequence & UUID Generator"

#define clear() printf("\033[H\033[J")

#define NEW_SEQ 1
#define UPDATE_SEQ 2
#define RESTART_SEQ 3
#define GET_SEQ 4
#define GEN_UUID 5

#define MYSQL_ID 1
#define POSTGRESQL_ID 2


/* Print help for commands */
void print_help(void);

/* 	Get credentials from user input for mysql or postgresql
	@params : database id

 */
void get_credentials_from_user_input(int database_id);

/* 	Save credentials in a config file for mysql or postgresql
	@params : username, password and database_id

 */
void set_credentials(const char *username, const char *password, int database_id);

/*	Get the configuration of credentials 
	@params : database id
	
*/
int get_credentials_config(int database_id);

/*
	display the database menu
	@params : database id
*/

void database_main_menu(int database_id);



/* Main menu of the app */
int main_menu(void);

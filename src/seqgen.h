/*
	seqgen.h - prototypes of the methods and functions
	created by Pedro Cruz 9/6/17

*/

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
#include <stdbool.h>

#define APP_NAME "Numeric Sequence & UUID Generator"

#define clear() printf("\033[H\033[J")

#define NEW_SEQ 1
#define UPDATE_SEQ 2
#define RESTART_SEQ 3
#define GET_SEQ 4
#define GEN_UUID 5

#define MYSQL_ID 1
#define POSTGRESQL_ID 2

#define MYSQL_BIN "mysql"
#define POSTGRESQL_BIN "psql"

#define	X_OK 1 

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


char *get_database_name(int database_id);



/* Main menu of the app */
int main_menu(void);



/* methods to install, remove modules and save state of sequences, in other words: interaction with the modules */

bool check_command (const char *cmd);

/* check if database server is installed */
int check_if_server_installed (int database_id);


/* this method is called every time that computer starts */
void insmod (int database_id);

/* this methods is called every time that computer shuts down */
void rmmod (int database_id);

/* this methods should be self-explanatory */
void create_sequence (int database_id, int sequence_number,  int initial_value);

void update_sequence (int database_id, int sequence_number, int new_value);

void restart_sequence (int database_id, int sequence_number);

unsigned int get_current_value (int database_id, int sequence_number);

char *generate_uuid(int database_id);


/* this method creates a file in disk to store the states of every sequence */

unsigned int backup_of_data (void);

/* the analog of the backup, this method is called every time that computer starts */

unsigned int restore_data (void);

/* method to generate and execute the ddl script of the UDF function on database */

void create_and_execute_ddl(int database_id);

/* method to install sequences */

void install_app ();

void save_install_state ();

unsigned int check_install_state ();


/* methods to execute query on each database */

unsigned int mysql_execute_query(char *query);

unsigned int postgresql_execute_query(char *query);




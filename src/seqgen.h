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
#include <uuid/uuid.h>


#define APP_NAME "seqgen"

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

#define SUCCESS_SYSTEM 0
#define FAIL_SYSTEM 256

#define MYSQL_ENVIRONMENT_INSTALLED "MYSQL_ENVIRONMENT_INSTALLED"
#define POSTGRESQL_ENVIRONMENT_INSTALLED "POSTGRESQL_ENVIRONMENT_INSTALLED"

#define INSTALLED 1
#define NOT_INSTALLED 0

#define SUCCESS_WRITE 1
#define FAIL_WRITE 0

#define CREATE_FUNCTION_MYSQL_QUERY "CREATE FUNCTION get_sequence RETURNS integer SONAME \"mysql_get_sequence.so\";"
#define SELECT_MYSQL_GET_SEQUENCE "select get_sequence(1);"
#define DROP_IF_EXISTS_MYSQL "DROP FUNCTION IF EXISTS get_sequence;"
#define SUCCESS 1
#define FAIL 0

#define CREATE_FUNCTION 1
#define SELECT_INITIAL 2

#define MYSQL_DEVICE_FILE_PATH "/dev/mysql_seq_dev"
#define MYSQL_HANDLER_FILE_PATH "/dev/mysql_seq_handler"


#ifdef DEV
#define COMPILE_MYSQL_MODULES_COMMAND "cd .. && cd modules/mysql && make"
#define INSMOD_MYSQL_MODULES_COMMAND "cd .. && cd modules/mysql && ./insmod.sh"
#define STATE_CONFIG_FILE_NAME "database_state.cnf"
#define LOG_PATH "log"
#define BACKUP_PATH "sequences.backup"
#else
#define COMPILE_MYSQL_MODULES_COMMAND "cd /etc/sequence_generator/mysql && make"
#define INSMOD_MYSQL_MODULES_COMMAND "cd /etc/sequence_generator/mysql && ./insmod.sh"
#define STATE_CONFIG_FILE_NAME "/etc/sequence_generator/database_state.cnf"
#define LOG_PATH "/etc/sequence_generator/log"
#define BACKUP_PATH "/etc/sequence_generator/sequences.backup"
#endif

#define SIZE_SEQUENCES 1024




struct sequence_request
{
	int offset; //Position of the n element to read or write
	int value ; //Value to send or comes from user
} ;

struct sequences_backup{
	int sequences[SIZE_SEQUENCES];
	int database_id;
	char time_string[25];
};

#define MAJOR_NUM 100
#define MAJOR_NUM_HANDLER 101

#define IOCTL_SET_SEQ _IOR(MAJOR_NUM, 0, struct sequence_request)

void write_log(const char *event);

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

int database_main_menu(int database_id);

void execute_option(int database_id, int option);
void execute_mysql_option(int option);
void execute_postgresql_option(int option);





char *get_database_name(int database_id);



/* Main menu of the app */
int main_menu(int mysql_installed, int postresql_installed);



/* methods to install, remove modules and save state of sequences, in other words: interaction with the modules */

bool check_command (const char *cmd);

/* check if database server is installed */
int check_if_server_installed (int database_id);

int compile_modules(int database_id);

/* this method is called every time that computer starts */
int insmod (int database_id);

/* this methods is called every time that computer shuts down */
int rmmod (int database_id);

/* this methods should be self-explanatory */
void create_sequence (int database_id, int sequence_number,  int initial_value);

void update_sequence (int database_id, int sequence_number, int new_value);

void restart_sequence (int database_id, int sequence_number);

void get_current_value (int database_id, int sequence_number);

char *generate_uuid(void);


void init_sequences(int database_id);

/* this method creates a file in disk to store the states of every sequence */

unsigned int backup_of_data (void);

/* the analog of the backup, this method is called every time that computer starts */

unsigned int restore_data (void);

/* method to generate and execute the ddl script of the UDF function on database */

void create_and_execute_ddl(int database_id);

/* method to install sequences */

void install_app ();

int read_database_state(int database_id);

int write_database_state (int database_id, int state);

unsigned int check_install_state ();


/* methods to execute query on each database */

unsigned int execute_query(int database_id, int type);

unsigned int mysql_execute_query(char *query);

unsigned int postgresql_execute_query(char *query);


void init_app (void);

/* ioctl methods */

int ioctl_get_seq(int file_desc, int sequence_offset);
int ioctl_set_seq(int file_desc, struct sequence_request *message);


/*** check methodos ***/

void check_clags_state(int database_id);


/**********************/
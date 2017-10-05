/*
	To compile in dev: gcc -o seqgen $(mysql_config --cflags) -I $(pg_config --includedir)  seqgen.c $(mysql_config --libs) -luuid -lpq  -D __MYSQL_H__  -D __POSTGRESQL_H__  -D DEV
    -I $(pg_config --includedir) -lpq
*/


#include "seqgen.h"
#include <unistd.h>
#include <sys/ioctl.h>		/* ioctl */
#include <time.h>

#ifdef __MYSQL_H__
#include <mysql.h>
#endif

#ifdef __POSTGRESQL_H__
#include <libpq-fe.h>
#endif
 
char uuid_str[37]; 

void check_cflags_state(int database_id){

	switch(database_id){
		case MYSQL_ID:
#ifdef __MYSQL_H__
            puts("MYSQL Cflags are present");
#else
            puts("MySQL Cflags are not present, I really wonder if you can do any work, so please install MYSQL and then reinstall this app");
#endif
			break;
		case POSTGRESQL_ID:
#ifdef __POSTGRESQL_H__
            puts("PostgreSQL Cflags are present");
#else
            puts("PostgreSQL Cflags are not present, I really wonder if you can do any work, so please install PostgreSQL and then reinstall this app");

#endif
			break;
		default:
			exit(EXIT_FAILURE);
	}

}

void write_log(const char *event){
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;

  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	char *string_time = asctime (timeinfo);

  	char *temp = malloc(strlen(event)+strlen(string_time)+1);
  	strcpy(temp, event);
  	strcat(temp, string_time);

  	printf ( "%s\n", temp );

   	
   	fp = fopen(LOG_PATH, "a");
   	fputs(temp, fp);
   	fclose(fp);
}

void execute_mysql_option(int option){
	clear();
	int offset;
	int exit_val;
	int new_value;
	switch(option){
		case NEW_SEQ:
			printf("%s\n", "Please select the number of sequence");
			scanf("%d", &offset);
			printf("%s\n", "Please insert a initial value");
			scanf("%d", &new_value);
			do{
				update_sequence(MYSQL_ID, offset, new_value);
				printf("%s\n", "Press any key");
				scanf("%d", &exit_val);
				exit_val = 0;
			} while (exit_val != 0); 
			break;
		case UPDATE_SEQ:
			printf("%s\n", "Please select the number of sequence");
			scanf("%d", &offset);
			printf("%s\n", "Please insert a new value");
			scanf("%d", &new_value);
			do{
				update_sequence(MYSQL_ID, offset, new_value);
				printf("%s\n", "Press any key");
				scanf("%d", &exit_val);
				exit_val = 0;
			} while (exit_val != 0); 
			break;
		case RESTART_SEQ:
			printf("%s\n", "Please select the number of sequence");
			scanf("%d", &offset);
			do{
				update_sequence(MYSQL_ID, offset, 1);
				printf("%s\n", "Press any key");
				scanf("%d", &exit_val);
				exit_val = 0;
			} while (exit_val != 0);
			
			break;
		case GET_SEQ:
			printf("%s\n", "Please select the number of sequence");
			scanf("%d", &offset);
			do{
				get_current_value(MYSQL_ID, offset);
				printf("%s\n", "Press any key");
				scanf("%d", &exit_val);
				exit_val = 0;
			} while (exit_val != 0);
			

			break;
		case GEN_UUID:
			do{
				printf("UUID generated = %s\n", generate_uuid());
				printf("%s\n", "Press any key");
				scanf("%d", &exit_val);
				exit_val = 0;
			} while (exit_val != 0);
			
			break;
		default:
			exit(EXIT_FAILURE);

	}
}
void execute_postgresql_option(int option){
	//TODO : implement postgreSQL options
	printf("%d\n", option);
}
void execute_option(int database_id, int option){
	switch(database_id){
		case MYSQL_ID:
			execute_mysql_option(option);
			break;
		case POSTGRESQL_ID:
			execute_postgresql_option(option);
			break;
		default:
			exit(EXIT_FAILURE);

	}
}

void get_credentials_from_user_input(int database_id) {

	if(get_credentials_config(database_id)){
		char username[20];
		printf("Please enter your %s username: \n", get_database_name(database_id));
		scanf("%s", username);
		char *password = getpass("Please enter your database password: \n");

		char choice_save[1];
		while(choice_save[0] != 'y' && choice_save[0] != 'n'){
			printf("%s\n", "Do you want to save your credentials and don't ask for them again? (y/n):");
			scanf("%s", choice_save);
			printf("%s\n", choice_save);
		}
		
		if(choice_save[0] == 'y')
			set_credentials(username, password, database_id);
		
	} 
	
	int exec_option ;
	do{
		 exec_option = database_main_menu(database_id);
		 execute_option(database_id, exec_option);
	} while(exec_option < 6);
	
	

  
}

void set_credentials(const char *username, const char *password, int database_id){

	//TODO: Save credentials of database
	printf("Username %s\n", username);
	printf("password %s\n", password);
	//printf("database_id %d\n", database_id);

}


int get_credentials_config(int database_id){
	//TODO: get the config of credencials
	
	return 1;
}


char *get_database_name(int database_id){

	char *database_name = "";
	switch (database_id){
			case MYSQL_ID:
				database_name = "MySQL";
				break;

			case POSTGRESQL_ID:
				database_name = "PostgreSQL";
				break;
			default:
				printf("%s\n", "Bye bye");

	}
	return database_name;

}

int database_main_menu(int database_id){
	clear();
	int option = 0;
	do{

		if(read_database_state(database_id)!=INSTALLED){

			if(compile_modules(database_id)==SUCCESS_SYSTEM && insmod(database_id)==SUCCESS_SYSTEM){

   				if(execute_query(database_id, CREATE_FUNCTION)==SUCCESS){

   					write_database_state(database_id, INSTALLED);
   					init_sequences(database_id);

   				} else {
   					//TODO: remove modules
   					printf("%s\n", "A fatal error has occurred with server, please reinstall the app");
					exit(1);
   				}

				
			} else {
				printf("%s\n", "A fatal error has occurred with modules, please reinstall the app");
				exit(1);
			}


		} 
		
		printf("%s\n", "Select option : ");
		printf("%s\n", "1) Create sequence");
		printf("%s\n", "2) Update sequence");
		printf("%s\n", "3) Restart sequence");
		printf("%s\n", "4) Get current number");
		printf("%s\n", "5) Generate UUID");
		printf("%s\n", "6) Exit");
		scanf("%d", &option);

	} while(option > 6);

	return option;
}




int main_menu(int mysql_installed, int postresql_installed){

	clear();
	int option = 0;


	do{

		
		if(mysql_installed && postresql_installed){
			printf("%s\n", "Please choose your database : ");
			printf("%s\n", "1) MySQL Server");
			printf("%s\n", "2) PostgreSQL");
			printf("%s\n", "3) Exit");	
			scanf("%d", &option);
		} else if( mysql_installed) {
			printf("%s\n", "Please choose your database : ");
			printf("%s\n", "1) MySQL Server");
			printf("%s\n", "2) Exit");

			scanf("%d", &option);
			if(option == 2)
				option = 3;
		} else if( postresql_installed){
			printf("%s\n", "Please choose your database : ");
			printf("%s\n", "1) PostgreSQL");
			printf("%s\n", "2) Exit");
			scanf("%d", &option);
			if(option == 1){
				option = 2;
			} else {
				option = 3;
			}
		} else{
			printf("%s\n", "Seems like you don't have any database server installed, press any key to exit");
		    scanf("%d", &option);
			option = 3;
		}
		


		

		

	}while(option > 3);

	return option;
}

bool check_command (const char *cmd){

	if(strchr(cmd, '/')) {
        return access(cmd, X_OK)==0;
    }
    const char *path = getenv("PATH");
    if(!path) return false; 
    char *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) return false; 
    for(; *path; ++path) {
        
        char *p = buf;
        
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        
        if(p==buf) *p++='.';
        
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        
        if(access(buf, X_OK)==0) {
            free(buf);
            return true;
        }
        // quit at last cycle
        if(!*path) break;
    }
    // not found
    free(buf);
    return false;

}

int check_if_server_installed (int database_id){

	int installed = 0;
	switch (database_id){
		case MYSQL_ID:
			installed = check_command(MYSQL_BIN);
			break;
		case POSTGRESQL_ID:
			installed = check_command(POSTGRESQL_BIN);
			break;
		default:
			installed = 0;

	}

	return installed;

}

int compile_modules(int database_id){

	int execute = FAIL_SYSTEM;
	switch(database_id){
		case MYSQL_ID:
			execute = system(COMPILE_MYSQL_MODULES_COMMAND);
			break;
		case POSTGRESQL_ID:
			//TODO: compile postgreSQL modules
			execute = system("cd .. && cd modules/mysql && make");
			break;
		default:
			puts("error with compile");
			execute = FAIL_SYSTEM;
	}
	return execute;
}

int insmod(int database_id){

	int execute = FAIL_SYSTEM;
	switch(database_id){
		case MYSQL_ID:
			execute = system(INSMOD_MYSQL_MODULES_COMMAND);
			break;
		case POSTGRESQL_ID:
			//TODO: ins postgreSQL modules
			execute = system("cd .. && cd modules/mysql && ./insmod.sh");
			break;
		default:
			puts("error with insmod");
			execute = FAIL_SYSTEM;
	}
	return execute;
}

int write_database_state (int database_id, int state){
	
	int other_state ;
	
	char *mysql_line = malloc(strlen(MYSQL_ENVIRONMENT_INSTALLED) + strlen("=")+ sizeof(char)*10 + 1);
	char *postgresql_line  = malloc(strlen(POSTGRESQL_ENVIRONMENT_INSTALLED) + strlen("=")+ sizeof(char)*10 + 1);	

	strcpy(mysql_line, MYSQL_ENVIRONMENT_INSTALLED);
    strcat(mysql_line, "=");
	
	strcpy(postgresql_line, POSTGRESQL_ENVIRONMENT_INSTALLED);
	strcat(postgresql_line, "=");

	char mysql_state[10];
	char postgresql_state[10];

	switch(database_id){
		case MYSQL_ID:
			other_state = read_database_state(POSTGRESQL_ID);
			sprintf(postgresql_state, "%d", other_state);
			strcat(postgresql_line, postgresql_state);
			sprintf(mysql_state, "%d", state);
			strcat(mysql_line, mysql_state);

			break;
		case POSTGRESQL_ID:
			other_state = read_database_state(MYSQL_ID);
			sprintf(mysql_state, "%d", other_state);
			strcat(mysql_line, mysql_state);
			sprintf(postgresql_state, "%d", state);
			strcat(postgresql_line, postgresql_state);
			break;
		default:
			return FAIL_WRITE;
	}

	FILE *config_file ;
	config_file = fopen(STATE_CONFIG_FILE_NAME, "w");
	if(!config_file)
		return FAIL_WRITE;
	fprintf(config_file, "%s\n", mysql_line);
	fprintf(config_file, "%s\n", postgresql_line);

	free(mysql_line);
	free(postgresql_line);

	fclose(config_file);
	return SUCCESS_WRITE;


}


int read_database_state (int database_id){

	char *database_config_var;
	switch (database_id){
		case MYSQL_ID:
			database_config_var = MYSQL_ENVIRONMENT_INSTALLED;
			break;
		case POSTGRESQL_ID:
			database_config_var = POSTGRESQL_ENVIRONMENT_INSTALLED;
			break;
		default:
			return -1;
	}

	FILE *config_file;
	char *database_line = NULL;
	config_file = fopen(STATE_CONFIG_FILE_NAME, "r");
	char read;
	size_t len = 0;
	int state;
	if (!config_file)
		return -1;
	while((read = getline(&database_line, &len, config_file)) != -1){
		char *temp_line=malloc(strlen(database_line)+1);
		strcpy(temp_line, database_line);
		char *var_name = strtok(temp_line, "=");
		if(strcmp(database_config_var, var_name)==0){
			char *state_token ;
			state_token = strtok(database_line, "=");
			int counter_tokens = 0;
			while(state_token!=NULL){
				if(counter_tokens == 1)
					sscanf(state_token, "%d", &state);
					

				state_token = strtok(NULL, "=");
				counter_tokens++;
			}	
		}
		free(temp_line);
	}

	if(database_line)
		free(database_line);
	
	fclose(config_file);

	return state;
	

}
unsigned int execute_query(int database_id, int type){
	unsigned int status = FAIL ;
	switch (database_id){
		case MYSQL_ID:
			switch(type){
				case CREATE_FUNCTION:
					mysql_execute_query(DROP_IF_EXISTS_MYSQL);
					status = mysql_execute_query(CREATE_FUNCTION_MYSQL_QUERY);
					break;
				case SELECT_INITIAL:
					status = mysql_execute_query(SELECT_MYSQL_GET_SEQUENCE);
					break;
				default:
					return FAIL;
			}

			break;
		case POSTGRESQL_ID:
			switch(type){
				case CREATE_FUNCTION:
					break;
				case SELECT_INITIAL:
					break;
				default:
					return FAIL;
			}
			break;
		default:
			return FAIL;

	}
	return status;
}

unsigned int mysql_execute_query(char *query){

#ifdef __MYSQL_H__
   MYSQL *conn;
   MYSQL_RES *res;
   

   char *server = "localhost";
   char *user = "root";
   char *password = "12345"; 
   char *database = "mysql";

   conn = mysql_init(NULL);

   /* Connect to database */
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   /* send SQL query */
   if (mysql_query(conn, query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   res = mysql_use_result(conn);

   /* close connection */
   mysql_free_result(res);
   mysql_close(conn);
   return SUCCESS;
#else
   printf("%s\n", "Seems like you don't have MySQL installed");
   return FAIL;
#endif

}

//PosgreSQL database methods connection:

unsigned int postgresql_execute_query(char *query){

#ifdef __POSTGRESQL_H__
    PGconn *conn = PQconnectdb("user=postgres password=12345 dbname=postgres");

    if (PQstatus(conn) == CONNECTION_BAD) {

        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *res = PQexec(conn, "SELECT VERSION()");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

        printf("No data retrieved\n");
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    printf("%s\n", PQgetvalue(res, 0, 0));

    PQclear(res);
    PQfinish(conn);

#else
    printf("%s\n", "Seems like you don't have MySQL installed");
    return FAIL;
#endif

}


void init_app(void){
	int mysql_state = read_database_state(MYSQL_ID);
	int postgresql_state = read_database_state(POSTGRESQL_ID);

	if(mysql_state==INSTALLED){

		if(insmod(MYSQL_ID)==SUCCESS_SYSTEM){
			write_log("modules inserted ok");
		}else{
			write_log("fail inserted modules");
			write_database_state(MYSQL_ID, NOT_INSTALLED);
		}
	}
	if(postgresql_state==INSTALLED){
		if(insmod(POSTGRESQL_ID)==SUCCESS_SYSTEM){
			write_log("modules inserted ok");
		} else {
			write_log("fail inserted modules");
			write_database_state(POSTGRESQL_ID, NOT_INSTALLED);
		}
	}
	

}


int ioctl_set_seq(int file_desc, struct sequence_request *message){

	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_SEQ, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
	return ret_val;
}

int ioctl_get_seq(int file_desc, int sequence_offset)
{
	int ret_val;
	int seq;

	ret_val = ioctl(file_desc, sequence_offset, &seq);

	if (ret_val < 0) {
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(EXIT_FAILURE);
	}

	printf("The current value of sequence is:%d\n", seq);
	return seq;
}
void update_sequence (int database_id, int sequence_number, int new_value){
	int file_desc;

	struct sequence_request seq_req;
	seq_req.offset = sequence_number;
	seq_req.value = new_value;


	switch(database_id){
		case MYSQL_ID:

			file_desc = open(MYSQL_HANDLER_FILE_PATH, 0);
			if (file_desc < 0) {
				printf("Can't open device file: %s\n", MYSQL_HANDLER_FILE_PATH);
				exit(-1);
			}
			
			ioctl_set_seq(file_desc, &seq_req);
			
			close(file_desc);

			break;
		case POSTGRESQL_ID:

			break;
		default:
			exit(EXIT_FAILURE);
	}

}
void get_current_value (int database_id, int sequence_number){
	int file_desc;

	switch (database_id){
		case MYSQL_ID:
			file_desc = open(MYSQL_HANDLER_FILE_PATH, 0);
			if (file_desc < 0) {
				printf("Can't open device file: %s\n", MYSQL_HANDLER_FILE_PATH);
				exit(-1);
			}

			
			ioctl_get_seq(file_desc, sequence_number);
			
			close(file_desc);
			break;
		case POSTGRESQL_ID:
			//TODO
			break;

		default:
			exit(EXIT_FAILURE);
	}
	
}

char *generate_uuid(void){

	uuid_t uuid;
	char *pointer;
    uuid_generate_time_safe(uuid);
    uuid_unparse_lower(uuid, uuid_str);
	pointer = uuid_str;
       
	return pointer;
}
void init_sequences(int database_id){
	int i=0;
	switch (database_id){

		case MYSQL_ID:

			for(i=0; i<SIZE_SEQUENCES; i++){
				update_sequence(MYSQL_ID, i, 0);
			}

			break;
		case POSTGRESQL_ID:
			for(i=0; i<SIZE_SEQUENCES; i++){
				update_sequence(POSTGRESQL_ID, i, 0);
			}

			break;
		default:
			exit(EXIT_FAILURE);
	}
}

unsigned int backup_of_data(void){
	
	if(read_database_state(MYSQL_ID)==INSTALLED){
		int file_desc;


		file_desc = open(MYSQL_HANDLER_FILE_PATH, 0);

		if (file_desc < 0) {
			printf("Can't open device file: %s\n", MYSQL_HANDLER_FILE_PATH);
			exit(-1);
		}

	  	int i = 0;
	  	int array[SIZE_SEQUENCES];
	  	for(i=0; i<SIZE_SEQUENCES; i++){
	  		array[i] = ioctl_get_seq(file_desc, i); 
	  	}

		close(file_desc);  	

	  	struct sequences_backup seq_backup;
	  	strcpy(seq_backup.time_string, "Prueba");
	  	seq_backup.database_id = MYSQL_ID;

	  	int j = 0;
	  	for (j = 0; j < SIZE_SEQUENCES; j++){
	  		seq_backup.sequences[j] = array[j];
	  	}
	  	

	  	FILE *backup_file;

	  	if((backup_file = fopen(BACKUP_PATH, "wb"))==NULL){
	  		puts("File could not be openend");
	  		return 0;
	  	} else{

	  		fwrite(&seq_backup, sizeof(struct sequences_backup), 1, backup_file);
	  		fclose(backup_file);
	  		puts("Backup created successfully!");
	  		return 1;
	  	}
  	}

 }

 unsigned int restore_data(void){

 	FILE *backup_file;
 	struct sequences_backup seq_backup;

 	if((backup_file = fopen(BACKUP_PATH, "rb"))==NULL){
 		puts("File could not be openend");
 		return 0;
 	}else{
 		
 			fread(&seq_backup, sizeof(struct sequences_backup), 1, backup_file);
 			int i;
 			for (i=0; i<SIZE_SEQUENCES; i++){
 				printf("Reading sequence No. %d Value %d\n", i, seq_backup.sequences[i]);
 				if(seq_backup.database_id == MYSQL_ID){
 					update_sequence(MYSQL_ID, i, seq_backup.sequences[i]);
 				} else if(seq_backup.database_id == POSTGRESQL_ID){
 					update_sequence(POSTGRESQL_ID, i, seq_backup.sequences[i]);
 				}
 				
 			}
 	
 		fclose(backup_file);
 		return 1;
 	}
 }

void print_help(void)
{
	printf("\n Usage: %s [OPTIONS]\n\n", APP_NAME);
	printf("  Options:\n");
	printf("   -h --help                 			     	Print this help\n\n");
	printf("   -c --cre_seq database number_sequence initial_value  Create a new sequence in the determined dbms with an initial value\n\n");
	printf("   -g --get_seq database number_sequence       		Get the current number of a sequence\n\n");
	printf("   -s --set_seq database number_sequence new_value      Set value for a sequence WARNING: It's recommended to stop the database service\n\n");
	printf("   -r --res_seq database number_sequence                Restart sequence WARNING: It's recommended to stop the database service\n\n");
	printf("   -d --del_seq database number_sequence                Delete sequence and clean space for a new one WARNING: It's recommended to stop the database service\n\n");
	printf("   -u --uuid                                            Generate UUID\n\n");
	printf("\n");
}

int main(int argc, char *argv[]){

    char *test_db = NULL;

    static struct option long_options[] = {
		{"cre_seq", required_argument, 0, 'c'},
		{"get_seq", required_argument, 0, 'g'},
		{"set_seq", required_argument, 0, 's'},
		{"res_seq", required_argument, 0, 'r'},
		{"del_seq", required_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{"uuid", no_argument, 0, 'u'},
		{"init", no_argument, 0, 'i'},
		{"end", no_argument, 0, 'e'},
		{"test", required_argument, 0, 't'},
		{"backup", no_argument, 0, 'b'},
		{"restore", no_argument, 0, 'y'},
        {"environment", no_argument, 0, 'v'},
		{NULL, 0, 0, 0}
	};

	int value, option_index = 0;
	
	while ((value = getopt_long(argc, argv, "c:g:s:r:d:huietbyv", long_options, &option_index)) != -1) {
		switch (value) {
			case 'c':
				printf("%s\n", "create seleted");
				break;
			case 'g':
				printf("%s\n", "get seleted");
				break;
			case 's':
				printf("%s\n", "set seleted");
				break;
			case 'r':
				printf("%s\n", "restart seleted");
				break;
			case 'd':
				printf("%s\n", "deleted seleted");
				break;
			case 'u':
				printf("%s\n", "uuid seleted");
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			case 'i':
				printf("%s\n", "init app");
				write_log("Init app ");
				init_app();
				return EXIT_SUCCESS;
			case 'e':
				printf("%s\n", "exit app");
				write_log("Exit app ");
				backup_of_data();
				return EXIT_SUCCESS;
			case 't':

                check_cflags_state(POSTGRESQL_ID);
                postgresql_execute_query("select");
				return EXIT_SUCCESS;
			case 'b':
				backup_of_data();
				return EXIT_SUCCESS;
			case 'y':
				restore_data();
				return EXIT_SUCCESS;
            case 'v':
#ifdef DEV
                puts("Environment is debug");
#else
                puts("Environment is release");
#endif
                return EXIT_SUCCESS;
			case '?':
				print_help();
				return EXIT_FAILURE;
			default:
				break;
		}
	}

	int choice_database;
	int mysql_installed = check_if_server_installed(MYSQL_ID);
	int postresql_installed = check_if_server_installed(POSTGRESQL_ID);
	do{
		choice_database = main_menu(mysql_installed, postresql_installed);
		switch (choice_database){
			case MYSQL_ID:
				clear();
				get_credentials_from_user_input(MYSQL_ID);
				break;

			case POSTGRESQL_ID:
				clear();
				get_credentials_from_user_input(POSTGRESQL_ID);
				break;
			default:
				printf("%s\n", "Bye bye");

		}

	}while(choice_database < 3);
}

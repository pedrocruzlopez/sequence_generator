
#include "seqgen.h"
#include <unistd.h>

#ifdef __MYSQL_H__
#include <mysql.h>
#endif
 


void print_help(void)
{
	printf("\n Usage: %s [OPTIONS]\n\n", APP_NAME);
	printf("  Options:\n");
	printf("   -h --help                 			     			Print this help\n");
	printf("   -c --cre_seq database number_sequence initial_value  Create a new sequence in the determined dbms with an initial value\n");
	printf("   -g --get_seq database number_sequence       		    Get the current number of a sequence\n");
	printf("   -s --set_seq database number_sequence new_value      Set value for a sequence WARNING: It's recommended to stop the database service\n");
	printf("   -r --res_seq database number_sequence                Restart sequence WARNING: It's recommended to stop the database service\n");
	printf("   -d --del_seq database number_sequence                Delete sequence and clean space for a new one WARNING: It's recommended to stop the database service\n");
	printf("   -u --uuid                                            Generate UUID\n");
	printf("\n");
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
		database_main_menu(database_id);
	} else {
		database_main_menu(database_id);
	}
	
	

  
}

void set_credentials(const char *username, const char *password, int database_id){

	printf("Username %s\n", username);
	printf("password %s\n", password);

}


int get_credentials_config(int database_id){
	return 1;
}


char *get_database_name(int database_id){

	char *database_name;
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

void database_main_menu(int database_id){
	clear();
	int option = 0;
	do{

		if(read_database_state(database_id)!=INSTALLED){

			if(insmod(database_id)==SUCCESS_INSMOD){

   				if(execute_query(database_id, CREATE_FUNCTION)==SUCCESS){

   					write_database_state(database_id, INSTALLED);
   				} else {
   					//remove modules
   					printf("%s\n", "A fatal error has occurred, please reinstall the app");
					exit(1);
   				}

				
			} else {
				printf("%s\n", "A fatal error has occurred, please reinstall the app");
				exit(1);
			}


		} 
		
		printf("%s\n", "Select option : ");
		printf("%s\n", "1) Create sequence");
		printf("%s\n", "2) Update sequence");
		printf("%s\n", "3) Restart sequence");
		printf("%s\n", "4) Get current number");
		printf("%s\n", "5) Generate UUID");
		scanf("%d", &option);

	} while(option > 5);
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

int insmod(int database_id){

	int execute = FAIL_INSMOD;
	switch(database_id){
		case MYSQL_ID:
			execute = system("cd .. && cd modules/mysql && ./insmod.sh");
			break;
		case POSTGRESQL_ID:
			execute = system("cd .. && cd modules/mysql && ./insmod.sh");
			break;
		default:
			execute = FAIL_INSMOD;
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
	unsigned int state;
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
}

unsigned int mysql_execute_query(char *query){

#ifdef __MYSQL_H__
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;

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


int main(int argc, char *argv[]){
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

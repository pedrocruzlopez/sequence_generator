
#include "seqgen.h"
#include <unistd.h>

 


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


void database_main_menu(int database_id){
	clear();
	int option = 0;
	do{

		printf("%s\n", "Select option : ");
		printf("%s\n", "1) Create sequence");
		printf("%s\n", "2) Update sequence");
		printf("%s\n", "3) Restart sequence");
		printf("%s\n", "4) Get current number");
		printf("%s\n", "5) Generate UUID");


		scanf("%d", &option);

		

	}while(option > 5);
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

	/*if(strchr(cmd, '/')){
				
		if(access(cmd, 1)==0)
			return 1;
	}

	const char *path = getenv("PATH");
	printf("%s\n", path);
	if(!path) return 0;

	char *buf = malloc(strlen(path)+strlen(cmd)+3);

	if(!buf) return 0;

	for(; *path; ++path){
		char *buf_ptr = buf;
		for(; *path && *path != ':' ; ++path, ++buf_ptr){
			*buf_ptr = *path;
		}
		if(buf_ptr==buf) *buf_ptr++='.';

		if(buf_ptr[-1]!='/') *buf_ptr++='/';
		strcpy(buf_ptr, cmd);

		if(access(buf, 1)==0){
			free(buf);
			return 0;
		}
		if(!*path) break;

	}
	free(buf);
	return 0;*/

	if(strchr(cmd, '/')) {
        // if cmd includes a slash, no path search must be performed,
        // go straight to checking if it's executable
        return access(cmd, X_OK)==0;
    }
    const char *path = getenv("PATH");
    if(!path) return false; // something is horribly wrong...
    // we are sure we won't need a buffer any longer
    char *buf = malloc(strlen(path)+strlen(cmd)+3);
    if(!buf) return false; // actually useless, see comment
    // loop as long as we have stuff to examine in path
    for(; *path; ++path) {
        // start from the beginning of the buffer
        char *p = buf;
        // copy in buf the current path element
        for(; *path && *path!=':'; ++path,++p) {
            *p = *path;
        }
        // empty path entries are treated like "."
        if(p==buf) *p++='.';
        // slash and command name
        if(p[-1]!='/') *p++='/';
        strcpy(p, cmd);
        // check if we can execute it
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


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
		printf("%s\n", "Please enter your database username: ");
		scanf("%s", username);
		char *password = getpass("Please enter your database password: \n");

		char choice_save[1];
		while(choice_save[0] != 'y' && choice_save[0] != 'n'){
			printf("%s\n", "Do you want to save your credentials and don't ask for them again? (y/n):");
			scanf("%s", choice_save);
			printf("%s\n", choice_save);
		}
		
		if(choice_save[0] == 'y')
			set_credentials(username, password, MYSQL_ID);
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



int main_menu(void){

	clear();
	int option = 0;

	do{

		printf("%s\n", "Please choose your database : ");
		printf("%s\n", "1) MySQL Server");
		printf("%s\n", "2) PostgreSQL");
		printf("%s\n", "3) Exit");


		scanf("%d", &option);

		

	}while(option > 3);

	return option;
}
int main(int argc, char *argv[]){


	
	int choice_database;	
	do{
		choice_database = main_menu();
		switch (choice_database){
			case 1:
				clear();
				get_credentials_from_user_input(MYSQL_ID);
				break;

			case 2:
				clear();
				get_credentials_from_user_input(POSTGRESQL_ID);
				break;
			case 3:
				printf("%s\n", "Bye bye");

		}

	}while(choice_database < 3);


	

}

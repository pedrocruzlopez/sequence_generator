
#include "seqgen.h"
#include <unistd.h>



void set_credentials(const char *username, const char *password){

	printf("Username %s\n", username);
	printf("password %s\n", password);

}

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

int mysql_get_credentials_config(void){
	return 0;
}


void database_main_menu(char *database){
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

void mysql_get_credentials(void) {

	if(mysql_get_credentials_config){
		char username[20];
		printf("%s\n", "Please enter your database username: ");
		scanf("%s", username);
		char *password = getpass("Please enter your database password: \n");

		char choiceSave[1];
		while(choiceSave[0] != 'y' && choiceSave[0] != 'n'){
			printf("%s\n", "Do want to save your credentials and don't ask for them again? (y/n):");
			scanf("%s", choiceSave);
			printf("%s\n", choiceSave);
		}
		
		if(choiceSave[0] == 'y')
			set_credentials(username, password);
		database_main_menu("mysql");
	} else {
		database_main_menu("mysql");
	}
	
	

  
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
				mysql_get_credentials();
				break;

			case 2:
				clear();
				break;
			case 3:
				printf("%s\n", "Bye bye");

		}

	}while(choice_database < 3);


	

}

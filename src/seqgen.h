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

void print_help(void);

int main_menu(void);

void mysql_get_credentials(void);

void postgresql_get_credentials(void);

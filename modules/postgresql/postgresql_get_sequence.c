#include "postgres.h"
#include <string.h>
#include "fmgr.h"
#include "utils/geo_decls.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "postgresql_sequence_header.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

int file_desc = -1;

    
unsigned int get_sequence(unsigned int arg);
unsigned int ioctl_get_msg(int file_desc, unsigned int sequence);

unsigned int ioctl_get_msg(int file_desc, unsigned int sequence){

	unsigned int seq;

	ioctl(file_desc, sequence, &seq);

	return seq;
}

unsigned int get_sequence(unsigned int arg)
{
	
	if(file_desc == -1){

		file_desc = open(DEVICE_FILE_PATH, 0);
	}
	


	return ioctl_get_msg(file_desc, arg);
}



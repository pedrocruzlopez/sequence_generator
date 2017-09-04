/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/* 
 * device specifics, such as ioctl numbers and the
 * major device file. 
 */
#include "mysql_sequence_header.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <linux/ioctl.h>		/* ioctl */

/* 
 * Functions for the ioctl calls 
 */



int ioctl_set_msg(int file_desc, char *message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_SEQ, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
	return ret_val;
}

/* 
 * Main - Call the ioctl functions 
 */
int main()
{
	int file_desc, ret_val;
	
	file_desc = open(HANDLER_FILE_PATH, 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", HANDLER_FILE_PATH);
		exit(-1);
	}
	char *message = "1-12";

	ioctl_set_msg(file_desc, message);


	close(file_desc);
}

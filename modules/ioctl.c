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
#include <sys/ioctl.h>		/* ioctl */

/* 
 * Functions for the ioctl calls 
 */



int ioctl_set_msg(int file_desc, int message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_SEQ, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
	return ret_val;
}

int ioctl_get_msg(int file_desc)
{
	int ret_val;
	int seq;
	

	/* 
	 * Warning - this is dangerous because we don't tell
	 * the kernel how far it's allowed to write, so it
	 * might overflow the buffer. In a real production
	 * program, we would have used two ioctls - one to tell
	 * the kernel the buffer length and another to give
	 * it the buffer to fill
	 */
	ret_val = ioctl(file_desc, 1, &seq);

	if (ret_val < 0) {
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(-1);
	}

	printf("get_msg message:%d\n", seq);
	return ret_val;
}


/* 
 * Main - Call the ioctl functions 
 */
int main()
{
	int file_desc, ret_val;
	
	file_desc = open(DEVICE_FILE_PATH, 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_PATH);
		exit(-1);
	}

	//ioctl_get_nth_byte(file_desc);
	ioctl_get_msg(file_desc);
	//ioctl_set_msg(file_desc, 90);

	//ioctl_get_msg(file_desc);

	close(file_desc);
}

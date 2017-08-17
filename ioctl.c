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
#include "sequence_driver.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <linux/ioctl.h>		/* ioctl */

/* 
 * Functions for the ioctl calls 
 */

int ioctl_set_msg(int file_desc, int message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

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
	ret_val = ioctl(file_desc, IOCTL_GET_MSG, &seq);

	if (ret_val < 0) {
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(-1);
	}

	printf("get_msg message:%d\n", seq);
	return ret_val;
}

int ioctl_get_nth_byte(int file_desc)
{
	int i;
	char c;

	printf("get_nth_byte message:");

	i = 0;
	do {
		c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++);

		if (c < 0) {
			printf
			    ("ioctl_get_nth_byte failed at the %d'th byte:\n",
			     i);
			exit(-1);
		}

		putchar(c);
	} while (c != 0);
	putchar('\n');
	return i;
}

/* 
 * Main - Call the ioctl functions 
 */
int main()
{
	int file_desc, ret_val;
	sequence seq_new;
	seq_new.value = 90;
	seq_new.status = 1;

	file_desc = open("/dev/seq_dev", 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	//ioctl_get_nth_byte(file_desc);
	ioctl_get_msg(file_desc);
	//ioctl_set_msg(file_desc, 90);

	//ioctl_get_msg(file_desc);

	close(file_desc);
}

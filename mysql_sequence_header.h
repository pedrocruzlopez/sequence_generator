/*
 *  mysql_sequence_header.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in mysql_sequence_driver.c) and the process calling ioctl (mysql_get_sequence.c)
 */

#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/kernel.h>
#include <linux/ioctl.h>



static int sequences[11];

struct sequence_request
{
	int offset; //Position of the n element to read or write
	int value ; //Value to send or comes from user
};

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100



/* 
 * Set the message of the device driver 
 */
#define IOCTL_SET_SEQ _IOR(MAJOR_NUM, 0, int)
/*
 * _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */

/* 
 * Get the message of the device driver 
 */
#define IOCTL_GET_SEQ _IOR(MAJOR_NUM, 1, int)
/* 
 * This IOCTL is used for output, to get the message 
 * of the device driver. However, we still need the 
 * buffer to place the message in to be input, 
 * as it is allocated by the process.
 */

/* 
 * Get the n'th byte of the message 
 */
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)
/* 
 * The IOCTL is used for both input and output. It 
 * receives from the user a number, n, and returns 
 * Message[n]. 
 */

/* 
 * The path of the device, in order to be accesible to everyone, will be stored in /dev/
 */
#define DEVICE_FILE_PATH "/dev/mysql_seq_dev"
#define HANDLER_FILE_PATH "/dev/mysql_seq_handler"

#endif

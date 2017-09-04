/*
 *  sequence_handler - handles update, restart of determined sequence[n]
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "mysql_sequence_header.h"
#define SUCCESS 0
#define DEVICE_NAME "mysql_seq_handler"

static int _device_open = 0;

extern int sequences[11];

static int device_open (struct inode *inode, struct file *file ){

#ifdef DEBUG
	printk(KERN_INFO, "device_open(%p)\n", file);
#endif

	if(_device_open)
		return -EBUSY;
	_device_open++;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}
 

static ssize_t device_read(struct file *file, sequence_request __user *buffer)
{

	
	
#ifdef DEBUG
	printk(KERN_INFO "device_read(%p,%p,%d)\n", file);
#endif

	
	//put_user(sequences[])
	
	return 1;
}


/* 
 * This function is called when somebody tries to
 * write into our device file. 
 */
static ssize_t device_write(struct file *file,
	     const int __user * buffer, size_t length, loff_t * offset)
{
	

#ifdef DEBUG
	printk(KERN_INFO "device_write(%p,%s,%d)", file, buffer, length);
#endif


	return 0;
}
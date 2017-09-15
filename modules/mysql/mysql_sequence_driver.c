/*
 *  sequence_driver.c - Create an input/output character device
 */

#include <linux/kernel.h>	
#include <linux/module.h>	
#include <linux/fs.h>
#include <asm/uaccess.h>	

#include "mysql_sequence_header.h"
#define SUCCESS 0
#define DEVICE_NAME "mysql_seq_dev"


/* 
 * Is the device open right now? Used to prevent
 * concurent access into the same device 
 */
static int __device_open = 0;


register int sequences[11];
EXPORT_SYMBOL(sequences);

/* 
 * This is called whenever a process attempts to open the device file 
 */
static int device_open(struct inode *inode, struct file *file){


#ifdef DEBUG
	printk(KERN_INFO "device_open(%p)\n", file);
#endif

	/* 
	 * We don't want to talk to two processes at the same time 
	 */
	if (__device_open)
		return -EBUSY;

	__device_open++;
		
	
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
#endif

	/* 
	 * We're now ready for our next caller 
	 */
	__device_open--;

	module_put(THIS_MODULE);
	return SUCCESS;
}

/* 
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */


/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
long  device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{

	__put_user(sequences[ioctl_num], (int *)ioctl_param);
	asm( "mov %1, %%ebx;"
 	     "inc %%ebx;"
         "mov %%ebx, %0" : "=r"(sequences[ioctl_num]) : "r" (sequences[ioctl_num]));

	return SUCCESS;
}

/* Module Declarations */

/* 
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions. 
 */
struct file_operations Fops = {
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,	/* a.k.a. close */
};

/* 
 * Initialize the module - Register the character device 
 */
int init_module()
{
	int ret_val;
	int i;
	for (i = 0 ; i < 11 ; i++){
		sequences[i] = 1;
	}

	/* 
	 * Register the character device (atleast try) 
	 */
	ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

	/* 
	 * Negative values signify an error 
	 */
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "Sorry, registering the character device ", ret_val);
		return ret_val;
	}

	printk(KERN_INFO "%s The major device number is %d.\n",
	       "Registeration is a success", MAJOR_NUM);
	printk(KERN_INFO "If you want to talk to the device driver,\n");
	printk(KERN_INFO "you'll have to create a device file. \n");
	printk(KERN_INFO "We suggest you use:\n");
	printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_PATH, MAJOR_NUM);
	printk(KERN_INFO "The device file name is important, because\n");
	printk(KERN_INFO "the ioctl program assumes that's the\n");
	printk(KERN_INFO "file you'll use.\n");

	return 0;
}

/* 
 * Cleanup - unregister the appropriate file from /proc 
 */
void cleanup_module()
{

	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);


}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pedro Enrique Cruz Lopez");
MODULE_DESCRIPTION("MYSQL Sequence driver module");

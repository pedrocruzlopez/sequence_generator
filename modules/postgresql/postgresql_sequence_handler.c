/*
 *  sequence_handler - handles update, restart of determined sequence[n]
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "postgresql_sequence_header.h"
#define SUCCESS 0
#define DEVICE_NAME "postgresql_seq_handler"

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
 

static int device_release(struct inode *inode, struct file *file)
{
#ifdef DEBUG
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
#endif

	/* 
	 * We're now ready for our next caller 
	 */
	_device_open--;

	module_put(THIS_MODULE);
	return SUCCESS;
}


long device_ioctl (struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){

	sequence_request seq_req;
	if(ioctl_num == IOCTL_SET_SEQ){

		if(copy_from_user(&seq_req,  (sequence_request *)ioctl_param, sizeof(sequence_request) )){
			return -EACCES;
		}

		sequences[seq_req.offset] = seq_req.value;

	} else {

		put_user(sequences[ioctl_num], (int *)ioctl_param);

	}


	return SUCCESS;
	
}


struct file_operations Fops = {
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,	/* a.k.a. close */
};

int init_module()
{
	int ret_val;
	/* 
	 * Register the character device (atleast try) 
	 */
	ret_val = register_chrdev(MAJOR_NUM_HANDLER, DEVICE_NAME, &Fops);

	/* 
	 * Negative values signify an error 
	 */
	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "Sorry, registering the character device ", ret_val);
		return ret_val;
	}

	printk(KERN_INFO "%s The major device number is %d.\n",
	       "Registeration is a success", MAJOR_NUM_HANDLER);
	printk(KERN_INFO "If you want to talk to the device driver,\n");
	printk(KERN_INFO "you'll have to create a device file. \n");
	printk(KERN_INFO "We suggest you use:\n");
	printk(KERN_INFO "mknod %s c %d 0\n", HANDLER_FILE_PATH, MAJOR_NUM_HANDLER);
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

	unregister_chrdev(MAJOR_NUM_HANDLER, DEVICE_NAME);


}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pedro Enrique Cruz Lopez");
MODULE_DESCRIPTION("PostgreSQL Sequence handler module");

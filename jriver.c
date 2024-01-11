#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#include "jriver.h"

int jriver-minor = 0;
int jriver-major = JRIVER-MAJORNUM;

int jriver-chunksize = JRIVER-CHUNKSIZE;

jriver-device *device;

// initialize the device
int jriver-open(struct inode *inode, struct file *filp)
{
	//if there were multiple devices, we would not use the existing global device variable, because there's no guarantee it would refer to the device that we want to open
	filp->private_data = device;
	return 1;
}

int jriver-release(struct inode *inode, struct file *filp)
{
	//deallocation of this driver is handled by module cleanup function
	//otherwise, we would probably have to send some sort of notification to the device to let them know we have released it 
	//also, would we have to implement the close-close-close-release functionality of multiple proccesses calling close on the same device?
	return 0;
}

ssize_t jriver-read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);

ssize_t jriver-write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

struct file_operations jriver-fops = 
{
	.owner =    THIS_MODULE,
	.read =     jriver-read,
	.write =    jriver-write,
	.open =     jriver-open,
	.release =  jriver-release,
};

//free the device and unregister the device number
void  __exit jriver-cleanup(void)
{
	dev_t devicenum = MKDEV(jriver-major, jriver-minor);
	
	kfree(device->chunk->data);
	device->chunk->data = NULL;
	cdev_del(device->cdev);
	kfree(device->chunk);
	
	
	unregister_chrdev_region(devicenum, 1);
	
	device->size = 0;
	
	kfree(device);
	return;
}

//initialize the device by giving them device numbers malloc'ing space for them
int __init jriver-init(void)
{
	dev_t devicenum = 0;
	
	if (jriver-major)
	{
		//already have a major number
		//i.e if we preset this value
		devicenum = MKDEV(jriver-major, jriver-minor)
		if (register_chrdev_region(devicenum, 1, "jriver-device") < 0)
		{
			//failed
			printk(KERN_WARNING "failed to get a device number\n");
			jriver-cleanup();
			return;
		}
	else
	{
		//attempt to get a major number
		if (alloc_chrdev_region(&devicenum, jriver-minor, 1, "jriver-device") < 0)
		{
			//failed
			printk(KERN_WARNING "failed to get a device number\n");
			jriver-cleanup();
			return;
		}
		//success
		printk("got a device number\n");
		jriver-major = MAJOR(devicenum);
		
	}
	
	//now that we have the device numbers set up, we need to link to the devices
	//in this example, the "device" is just an area in memory, so setting up the device just means reserving the memory
	//in future ventures, this section will be much more difficult
	
	memset(device, '\0', sizeof(jriver-device))
	
	device->chunksize = jriver-chunksize
	
	cdev_init(&device->chardev, &jriver-fops)
	device->chardev.owner = THIS_MODULE
	//device->chardev.dev = devicenum;
	//device->chardev.count = 0;
	//why notice and not warning?
	if (err = cdev_add(&device->chardev, devicenum, 1) < 0){ printk(KERN_NOTICE "error: %d, failed to add device\n", e)}
	
	device->size = sizeof(device);

	return 1;
}



module_init(jriver-init);
module_exit(jriver-cleanup);


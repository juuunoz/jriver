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

int JRIVER_MINORNUM = 0;
int JRIVER_MAJORNUM = JRIVER_MAJORNUMNUM;

jriver_device *device;

// initialize the device
int jriver_open(struct inode *inode, struct file *filp)
{
	//if there were multiple devices, we would not use the existing global device variable, because there's no guarantee it would refer to the device that we want to open
	//we would have to find it again using container_of() with the pointer being the cdev stored in inode->i_cdev
	filp->private_data = device;
	return 1;
}

int jriver_release(struct inode *inode, struct file *filp)
{
	//deallocation of this driver is handled by module cleanup function
	//otherwise, we would probably have to send some sort of notification to the device to let them know we have released it 
	//also, would we have to implement the close-close-close-release functionality of multiple proccesses calling close on the same device?
	return 0;
}

ssize_t jriver_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	jriver_device *dev = filp->private_data;
	chunk *data_container = dev->chunk;

	int size = dev->size;
	int retval = count;
	
	//EOF because no more data available
	if (*offp >= size) return 0;
	if (*offp + count >= size) retval = dev->size - *offp;
	
	//trying to read more than what exists
	if (count > dev->size) retval = dev->size;
	
	//data isn't there!
	if (data_container == NULL || data_container->data == NULL) return 0;
	
	// read the information
	if (copy_to_user(buf, data_container->data, retval) < 0 ) return -EFAULT;
	
	//move file pointer offset accordingly
	*offp += retval;
	
	return retval;
}

ssize_t jriver_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	jriver_device *dev = filp->private_data;
	chunk *data_container;

	int size = dev->size;
	int retval = count;
	
	//reserve the memory space, so we can start writing safely
	if (!data_container->data)
		{
			data_container->data = kmalloc(JDRIVER_BUFFERSIZE * sizeof(char *), GFP_KERNEL);
			memset(data_container->data, '\0', JDRIVER_BUFFERSIZE * sizeof(char *))
		}
	
	//trying to write more than we can hold
	if (count + *f_pos > JDRIVER_BUFFERSIZE ) retval = JDRIVER_BUFFERSIZE - *fpos;
	
	// write the information
	if (copy_from_user(buf, data_container->data, retval) < 0 ) return -EFAULT;
	
	//update size
	dev->size += retval;
	
	//move file pointer offset accordingly
	*f_pos += dev->size;
	
	return retval;

}

struct file_operations jriver_fops = 
{
	.owner =    THIS_MODULE,
	.read =     jriver_read,
	.write =    jriver_write,
	.open =     jriver_open,
	.release =  jriver_release,
};

//free the device and unregister the device number
void  __exit jriver_cleanup(void)
{
	dev_t devicenum = MKDEV(JRIVER_MAJORNUM, JRIVER_MINORNUM);
	
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
int __init jriver_init(void)
{
	dev_t devicenum = 0;
	
	if (JRIVER_MAJORNUM)
	{
		//already have a major number
		//i.e if we preset this value
		devicenum = MKDEV(JRIVER_MAJORNUM, JRIVER_MINORNUM)
		if (register_chrdev_region(devicenum, 1, "jriver_device") < 0)
		{
			//failed
			printk(KERN_WARNING "failed to get a device number\n");
			jriver_cleanup();
			return;
		}
	else
	{
		//attempt to get a major number
		if (alloc_chrdev_region(&devicenum, JRIVER_MINORNUM, 1, "jriver_device") < 0)
		{
			//failed
			printk(KERN_WARNING "failed to get a device number\n");
			jriver_cleanup();
			return;
		}
		//success
		printk("got a device number\n");
		JRIVER_MAJORNUM = MAJOR(devicenum);
		
	}
	
	//now that we have the device numbers set up, we need to link to the devices
	//in this example, the "device" is just an area in memory, so setting up the device just means reserving the memory
	//in future ventures, this section will be much more difficult
	
	memset(device, '\0', sizeof(jriver_device))
	
	cdev_init(&device->chardev, &jriver_fops)
	device->chardev.owner = THIS_MODULE;

	if (err = cdev_add(&device->chardev, devicenum, 1) < 0) 
	{
		printk(KERN_WARNING "error: %d, failed to add device\n", e);
		jriver_cleanup();
		return;
	}
	
	device->size = 0;

	return 1;
}

module_init(jriver_init);
module_exit(jriver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Juno Zhang");

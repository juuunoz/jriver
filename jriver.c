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

ssize_t jriver-read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t jriver-write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
int jriver-open(struct inode *inode, struct file *filp);
int jriver-release(struct inode *inode, struct file *filp);

struct file_operations jriver-fops = {
	.owner =    THIS_MODULE,
	.read =     jriver-read,
	.write =    jriver-write,
	.open =     jriver-open,
	.release =  jriver-release,
};

int __init jriver-init(void)
{
	return 1;
}

void  __exit jriver-cleanup(void)
{
	return;
}

module_init(jriver-init);
module_exit(jriver-cleanup);


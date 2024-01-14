#ifndef _JRIVER
#define _JRIVER

//define some constants
#ifndef JRIVER_MAJORNUM
#define JDRIVER_MAJORNUM 0  
#endif

#ifndef JRIVER_MINORNUM
#define JDRIVER_MINORNUM 0  
#endif

#ifndef JDRIVER_BUFFERSIZE
#define JDRIVER_BUFFERSIZE 4000
#endif

//device data structure
typedef struct chunk
{
	void *data;
	//struct chunkset *next;
}chunk;

//device informational structure
typedef struct jriver-device
{
	// data portion of struct
	struct chunk *chunk;
	
	//organizational portion of struct
	struct cdev chardev;
	
	long size;
	
	
}jriver-device;

int jriver-open(struct inode *inode, struct file *filp);
int jriver-release(struct inode *inode, struct file *filp);
ssize_t jriver-read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t jriver-write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

#endif
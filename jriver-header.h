#ifndef _JRIVER
#define _JRIVER

//define some constants
#ifndef JRIVER-MAJORNUM
#define JDRIVER-MAJORNUM 0  
#endif

#ifndef JRIVER-NUMDEVS
#define JRIVER-NUMDEVS 1
#endif

#ifndef JRIVER-CHUNKSIZE
#define JRIVER-CHUNKSIZE 4000
#endif

#ifndef JRIVER-CHUNKSETSIZE
#define JRIVER-CHUNKSETSIZE 1000
#endif

#ifndef JDRIVER-BUFFERSIZE
#define JDRIVER-BUFFERSIZE 4000
#endif

//device data structure
typedef struct chunk
{
	void **data;
	//struct chunkset *next;
} chunk;

//device informational structure
typedef struct jriver-device
{
	// data portion of struct
	struct chunk *chunk;
	
	//organizational portion of struct
	int chunksize;
	//int numchunks;
	
	struct cdev chardev;
	
	long size;
	
	
}jriver-device;

int jriver-open(struct inode *inode, struct file *filp);
int jriver-release(struct inode *inode, struct file *filp);
ssize_t jriver-read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t jriver-write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

#endif
#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024
static int openCounter = 0;
static int closeCounter = 0;
static char *device_buffer;



ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	
	//int numBytes = 0;
	int numBytesActual = 0;
	int numBytesLeft = BUFFER_SIZE - *offset;	// represents number of bytes remaining
	
	printk(KERN_ALERT "Buffer Size: %d \n", length);	
	if(numBytesLeft >= length)
	{
		printk(KERN_ALERT "BEFORE COPY_TO_USER \n");
		int numBytes = copy_to_user(buffer, device_buffer + *offset, length); 	// if successful, copy_to_user() will return 0. Returns number of bytes not read.
		numBytesActual = length - numBytes;
		*offset += numBytesActual;
	}
	else
	{
		printk(KERN_ALERT "User requested to read too many bytes: %d \n", -1);
	}

	printk(KERN_ALERT "Number of bytes read: %d \n", numBytesActual);
	return numBytesActual;
}



ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	
	int numBytesActual = 0;
	int numBytesLeft = BUFFER_SIZE - *offset;
	
	if(numBytesLeft >= length)
	{
		printk(KERN_ALERT "BEFORE COPY_FROM_USER \n");
		int numBytes = copy_from_user(device_buffer + *offset, buffer, length);
		numBytesActual = length - numBytes;
		*offset += numBytesActual;
	}
	else
	{
		printk(KERN_ALERT "User requested to read too many bytes: %d \n", -1);
	}

	printk(KERN_ALERT "Number of bytes written: %d \n", numBytesActual);
	return numBytesActual;
}


int pa2_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	openCounter++;
	printk("This driver opened %d times \n", openCounter);
	return 0;
}

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	closeCounter++;
	printk("This driver closed %d times \n", closeCounter);
	return 0;
}

loff_t pa2_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	loff_t location = 0;
	/* Update open file position according to the values of offset and whence */
	if(whence == SEEK_SET)
	{
		if(offset < BUFFER_SIZE && offset >= 0) // check p_loc variable
		{
			printk(KERN_ALERT "SEEK_SET \n");
			//pfile -> f_pos = offset;
			location = offset;
		}
		else
		{
			printk(KERN_ALERT "Error: SEEK_SET - %d \n", -1);
		}
	}
	else if (whence == SEEK_CUR)
	{
		if(pfile -> f_pos + offset < BUFFER_SIZE && pfile -> f_pos + offset >= 0)
		{
			printk(KERN_ALERT "SEEK_CUR \n");
			printk(KERN_ALERT "currentPos: %d \n", pfile -> f_pos);
			printk(KERN_ALERT "offset: %d \n", offset);
			//pfile -> f_pos = offset + (pfile -> f_pos);
			location = offset + pfile -> f_pos;
		}
		else
		{
			printk(KERN_ALERT "Error: SEEK_CUR - %d \n", -1);
		}
	}
	else if (whence == SEEK_END)
	{
		if(pfile -> f_pos + offset < BUFFER_SIZE && pfile -> f_pos + offset >= 0)
		{
			printk(KERN_ALERT "SEEK_END \n");
			//pfile -> f_pos = BUFFER_SIZE - offset;
			location = BUFFER_SIZE - offset;
		}
		else
		{
			printk(KERN_ALERT "Error: SEEK_END - %d \n", -1);
		}
	}
	else
	{
		printk(KERN_ALERT "Error: No SEEK accepted - %d \n", -1);
	}
	pfile -> f_pos = location;
	printk(KERN_ALERT "Current Position: %d \n", pfile -> f_pos);
	return pfile -> f_pos;
}

/* Define device_buffer and other global data structures you will need here */
struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
    	.open   = pa2_char_driver_open,       // int my_open  (struct inode *, struct file *);
    	.release = pa2_char_driver_close,      // int my_close (struct inode *, struct file *);
    	.read    = pa2_char_driver_read,       // ssize_t my_read  (struct file *, char __user *, size_t, loff_t *);
    	.write   = pa2_char_driver_write,      // ssize_t my_write (struct file *, const char __user *, size_t, loff_t *);
    	.llseek  = pa2_char_driver_seek        // loff_t  my_seek  (struct file *, loff_t, int);
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};


static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	/* register the device */
	device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
	register_chrdev(240, "part2Device", &pa2_char_driver_file_operations);
	return 0;
}

static void pa2_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
	kfree(device_buffer);
	unregister_chrdev(240, "part2Device");
}



/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(pa2_char_driver_init);
module_exit(pa2_char_driver_exit);

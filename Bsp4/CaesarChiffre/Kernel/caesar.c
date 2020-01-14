/*
 * main.c -- the bare caesar char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 */

/* #include <linux/config.h> */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
//#include <asm/system.h>		/* cli(), *_flags */
/* #include <asm/uaccess.h>	/\* copy_*_user *\/ */
#include <linux/uaccess.h>	/* copy_*_user */

#include "caesar.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */

int caesar_major =   CAESAR_MAJOR;
int caesar_minor =   0;
int caesar_nr_devs = CAESAR_NR_DEVS;	/* number of bare caesar devices */
int caesar_quantum = CAESAR_QUANTUM;
int caesar_qset =    CAESAR_QSET;
int shiftNum = 3;
int buffersize =  20;

static DEFINE_MUTEX(read_m0);
static DEFINE_MUTEX(write_m0);
static DEFINE_MUTEX(read_m1);
static DEFINE_MUTEX(write_m1);

module_param(buffersize, int, S_IRUGO);
module_param(caesar_minor, int, S_IRUGO);
module_param(shiftNum, int, S_IRUGO);

MODULE_AUTHOR("Lars Kowoll, Michael Babic");
MODULE_LICENSE("Dual BSD/GPL");

struct caesar_dev *caesar_devices;	/* allocated in caesar_init_module */

struct caesar_pipe {
        wait_queue_head_t inq, outq;       /* read and write queues */
        char *buffer, *end;                /* begin of buf, end of buf */
        int buffersize;                    /* used in pointer arithmetic */
        char *rp, *wp;                     /* where to read, where to write */
        int nreaders, nwriters;            /* number of openings for r/w */
        struct fasync_struct *async_queue; /* asynchronous readers */
        struct semaphore sem;              /* mutual exclusion semaphore */
        struct cdev cdev;                  /* Char device structure */
};


//caesar_p
static struct caesar_pipe *caesar_p0;
static struct caesar_pipe *caesar_p1;

static void encode(const char *input, char *output, int buffersize, int shiftNum);
static void decode(const char *input, char *output, int buffersize, int shiftNum);
static int is_ascii(char c);
static int shift_char(char* c, int shiftNum);
static int unshift_char(char* c, int shiftNum);
static int get_string_size(const char* string);



/*
 * Open and close
 */

int caesar_open(struct inode *inode, struct file *filp)
{
   struct caesar_dev *dev; /* device information */
   
    unsigned int minor_num = MINOR(inode -> i_rdev);
    switch (minor_num){
        case 0:
            if (filp->f_mode & FMODE_READ){
        
            if(!mutex_trylock(&read_m0)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                        /// returns 1 if successful and 0 if there is contention
                printk(KERN_ALERT "EBBChar: Device in use by another process");
                return -EBUSY;
            }
        
            }
            //dev->nreaders++; // todo, es darf maximal ein Reader existieren
            if (filp->f_mode & FMODE_WRITE){
        
                if(!mutex_trylock(&write_m0)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                        /// returns 1 if successful and 0 if there is contention
                    printk(KERN_ALERT "EBBChar: Device in use by another process");
                    return -EBUSY;
                    }
            
            }
            break;
        
        case 1:
            if (filp->f_mode & FMODE_READ){
        
            if(!mutex_trylock(&read_m1)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                        /// returns 1 if successful and 0 if there is contention
                printk(KERN_ALERT "EBBChar: Device in use by another process");
                return -EBUSY;
            }
        
            }
            //dev->nreaders++; // todo, es darf maximal ein Reader existieren
            if (filp->f_mode & FMODE_WRITE){
        
                if(!mutex_trylock(&write_m1)){    /// Try to acquire the mutex (i.e., put the lock on/down)
                                        /// returns 1 if successful and 0 if there is contention
                    printk(KERN_ALERT "EBBChar: Device in use by another process");
                    return -EBUSY;
                    }
            
            }
            break;
            
        default:
            return -EBUSY;
    }
    
   //TODO: HIER DIE ABFRAGE MIT DEV SPEZIFISCHER MUTEX
    /* use f_mode,not  f_flags: it's cleaner (fs/open.c tells why) */
    
	
		//dev->nwriters++; // todo, es darf maximal ein Writer existieren

   dev = container_of(inode->i_cdev, struct caesar_dev, cdev);
   filp->private_data = dev; /* for other methods */

   return 0;          /* success */
}

int caesar_release(struct inode *inode, struct file *filp)
{

    unsigned int minor_num = MINOR(inode -> i_rdev);
    switch (minor_num){
        case 0:
            if (filp->f_mode & FMODE_READ){
        
                mutex_unlock(&read_m0); 
        
            }
            //dev->nreaders++; // todo, es darf maximal ein Reader existieren
            if (filp->f_mode & FMODE_WRITE){
        
                mutex_unlock(&write_m0); 
            }
            break;
        
        case 1:
                if (filp->f_mode & FMODE_READ){
        
                    mutex_unlock(&read_m1); 
        
                }
                //dev->nreaders++; // todo, es darf maximal ein Reader existieren
                if (filp->f_mode & FMODE_WRITE){
        
                    mutex_unlock(&write_m1); 
                }
            break;
            
        default:
            return -EBUSY;
    }

		//dev->nwriters--; // todo, es darf maximal ein Writer existieren
	//if (dev->nreaders + dev->nwriters == 0) {
	//	kfree(dev->buffer);
	//	dev->buffer = NULL; /* the other fields are not checked on open */
	//}
	return 0;
}

/*
 * Data management: read and write
 */

ssize_t caesar_read(struct file *filp, char __user *buf, size_t count,
		   loff_t *f_pos)
{
   struct caesar_dev *priv_dev = filp->private_data;
   
    struct caesar_pipe *dev;

   switch (MINOR(priv_dev->cdev.dev)) {
		case 0:
				dev = caesar_p0;
				break;
		case 1:
				dev = caesar_p1;
				break;
		default:
				PDEBUG("The minor number is not correct");
				return -ERESTARTSYS;

	}
   
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	while (dev->rp == dev->wp) { /* nothing to read */
		up(&dev->sem); /* release the lock */
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		PDEBUG("\"%s\" reading: going to sleep\n", current->comm);
		if (wait_event_interruptible(dev->inq, (dev->rp != dev->wp)))
			return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
		/* otherwise loop, but first reacquire the lock */
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
	}
	/* ok, data is there, return something */
	if (dev->wp > dev->rp)
		count = min(count, (size_t)(dev->wp - dev->rp));
	else /* the write pointer has wrapped, return data up to dev->end */
		count = min(count, (size_t)(dev->end - dev->rp));
    dev->rp--;
    // Hier muss ein Aufruf der Funktion encode, decode erfolgen, je nach minior number
	switch (MINOR(priv_dev->cdev.dev)) {
		case 0:
				encode(buf, dev->rp, count, shiftNum);
				break;
		case 1:
				decode(buf, dev->rp, count, shiftNum);
				break;
		default:
				PDEBUG("The minor number is not correct");
				break;

	}

	if (copy_to_user(buf, dev->rp, count)) {
		up (&dev->sem);
		return -EFAULT;
	}
	dev->rp++;
	dev->rp += count;
	if (dev->rp == dev->end)
		dev->rp = dev->buffer; /* wrapped */
	up (&dev->sem);

	/* finally, awake any writers and return */
	wake_up_interruptible(&dev->outq);
	PDEBUG("\"%s\" did read %li bytes\n",current->comm, (long)count);
	return count;
}

/* How much space is free? */
//static int spacefree(struct caesar_pipe *dev);

static int spacefree(struct caesar_pipe *dev){
	if (dev->rp == dev->wp)
		return dev->buffersize - 1;
	return ((dev->rp + dev->buffersize - dev->wp) % dev->buffersize) - 1;
}

/* Wait for space for writing; caller must hold device semaphore.  On
 * error the semaphore will be released before returning. */
static int caesar_getwritespace(struct caesar_pipe *dev, struct file *filp)
{
	while (spacefree(dev) == 0) { /* full */
		DEFINE_WAIT(wait);
		
		up(&dev->sem);
		if (filp->f_flags & O_NONBLOCK)
			return -EAGAIN;
		PDEBUG("\"%s\" writing: going to sleep\n",current->comm);
		prepare_to_wait(&dev->outq, &wait, TASK_INTERRUPTIBLE);
		if (spacefree(dev) == 0)
			schedule();
		finish_wait(&dev->outq, &wait);
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
	}
	return 0;
}	



ssize_t caesar_write(struct file *filp, const char __user *buf, size_t count,
		    loff_t *f_pos)
{
    struct caesar_dev *priv_dev = filp->private_data;
    struct caesar_pipe *dev;
	int result;
    
    switch (MINOR(priv_dev->cdev.dev)) {
		case 0:
				dev = caesar_p0;
				break;
		case 1:
				dev = caesar_p1;
				break;
		default:
				PDEBUG("The minor number is not correct");
				return -EBUSY;

	}
    
   
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	/* Make sure there's space to write */
	result = caesar_getwritespace(dev, filp);
	if (result)
		return result; /* caesar_getwritespace called up(&dev->sem) */

	/* ok, space is there, accept something */
	count = min(count, (size_t)spacefree(dev));
	if (dev->wp >= dev->rp)
		count = min(count, (size_t)(dev->end - dev->wp)); /* to end-of-buf */
	else /* the write pointer has wrapped, fill up to rp-1 */
		count = min(count, (size_t)(dev->rp - dev->wp - 1));
	PDEBUG("Going to accept %li bytes to %p from %p\n", (long)count, dev->wp, buf);
	if (copy_from_user(dev->wp, buf, count)) {
		up (&dev->sem);
		return -EFAULT;
	}

    // Hier muss ein Aufruf der Funktion encode, decode erfolgen, je nach minior number
	switch (MINOR(priv_dev->cdev.dev)) {
		case 0:
				encode(buf, dev->wp, count, shiftNum);
				break;
		case 1:
				decode(buf, dev->wp, count, shiftNum);
				break;
		default:
				PDEBUG("The minor number is not correct");
				break;

	}

	dev->wp += count;
	if (dev->wp == dev->end)
		dev->wp = dev->buffer; /* wrapped */
	up(&dev->sem);

	/* finally, awake any reader */
	wake_up_interruptible(&dev->inq);  /* blocked in read() and select() */

	/* and signal asynchronous readers, explained late in chapter 5 */
	PDEBUG("\"%s\" did write %li bytes\n",current->comm, (long)count);
	return count;
}

struct file_operations caesar_fops = {
   .owner =    THIS_MODULE,
   .read =     caesar_read,
   .write =    caesar_write,
   .open =     caesar_open,
   .release =  caesar_release,
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void caesar_cleanup_module(void)
{
   int i = 0;
   dev_t devno = MKDEV(caesar_major, caesar_minor);
    mutex_destroy(&write_m0);
    mutex_destroy(&read_m0);
    mutex_destroy(&write_m1);
    mutex_destroy(&read_m1);
   /* Get rid of our char dev entries */
   if (caesar_devices) {
      for (i = 0; i < caesar_nr_devs; i++) {
	 cdev_del(&caesar_devices[i].cdev);
      }
      kfree(caesar_devices);
   }

   /* cleanup_module is never called if registering failed */
   unregister_chrdev_region(devno, caesar_nr_devs);

   /* and call the cleanup functions for friend devices */
   //caesar_p_cleanup();
}


/*
 * Set up the char_dev structure for this device.
 */
static void caesar_setup_cdev(struct caesar_dev *dev, int index)
{
   int err, devno = MKDEV(caesar_major, caesar_minor + index);
    
   cdev_init(&dev->cdev, &caesar_fops);
   dev->cdev.owner = THIS_MODULE;
   dev->cdev.ops = &caesar_fops;
   err = cdev_add (&dev->cdev, devno, 1);
   /* Fail gracefully if need be */
   if (err)
      printk(KERN_NOTICE "Error %d adding caesar%d", err, index);
}


static int init_pipe(struct caesar_pipe* caesar_p){
    
    int result = 0;
    
    caesar_p = kmalloc(sizeof(struct caesar_pipe), GFP_KERNEL);
    if (caesar_p == NULL) {
        goto fail;
    }
        
    memset(caesar_p, 0, sizeof(struct caesar_pipe));
    init_waitqueue_head(&(caesar_p->inq));
    init_waitqueue_head(&(caesar_p->outq));
    init_MUTEX(&caesar_p->sem);
    
    /* allocate the buffer */
    caesar_p->buffer = kmalloc(buffersize, GFP_KERNEL);
    if (!caesar_p->buffer) {
        result = -ENOMEM;
    }

    caesar_p->buffersize = buffersize;
    caesar_p->end = caesar_p->buffer + caesar_p->buffersize;
    caesar_p->rp = caesar_p->wp = caesar_p->buffer; /* rd and wr from the beginning */
    
    return result;
    
    fail:
    caesar_cleanup_module();
    return result;
}

int caesar_init_module(void)
{
   int result, i;
   dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
    mutex_init(&write_m0);
    mutex_init(&read_m0);
    mutex_init(&read_m1);
    mutex_init(&write_m1);

      result = alloc_chrdev_region(&dev, caesar_minor, caesar_nr_devs,
				   "caesar");
      caesar_major = MAJOR(dev); // Get a major number from linux

   if (result < 0) {
      printk(KERN_WARNING "caesar: can't get major %d\n", caesar_major);
      return result;
   }

   /* 
    * allocate the devices -- we can't have them static, as the number
    * can be specified at load time
    */
   caesar_devices = kmalloc(caesar_nr_devs * sizeof(struct caesar_dev), GFP_KERNEL);
   if (!caesar_devices) {
      result = -ENOMEM;
      goto fail;  /* Make this more graceful */
   }
   memset(caesar_devices, 0, caesar_nr_devs * sizeof(struct caesar_dev));

   /* Initialize each device. */
      for (i = 0; i < caesar_nr_devs; i++) {
      caesar_setup_cdev(&caesar_devices[i], i);
   }
        
    //result = init_pipe(caesar_p0);
    //result = init_pipe(caesar_p1);
    
    caesar_p0 = kmalloc(sizeof(struct caesar_pipe), GFP_KERNEL);
    if (caesar_p0 == NULL) {
        goto fail;
    }
        
    memset(caesar_p0, 0, sizeof(struct caesar_pipe));
    init_waitqueue_head(&(caesar_p0->inq));
    init_waitqueue_head(&(caesar_p0->outq));
    init_MUTEX(&caesar_p0->sem);
    
    /* allocate the buffer */
    caesar_p0->buffer = kmalloc(buffersize, GFP_KERNEL);
    if (!caesar_p0->buffer) {
        result = -ENOMEM;
    }

    caesar_p0->buffersize = buffersize;
    caesar_p0->end = caesar_p0->buffer + caesar_p0->buffersize;
    caesar_p0->rp = caesar_p0->wp = caesar_p0->buffer; /* rd and wr from the beginning */
    
    
    caesar_p1 = kmalloc(sizeof(struct caesar_pipe), GFP_KERNEL);
    if (caesar_p1 == NULL) {
        goto fail;
    }
        
    memset(caesar_p1, 0, sizeof(struct caesar_pipe));
    init_waitqueue_head(&(caesar_p1->inq));
    init_waitqueue_head(&(caesar_p1->outq));
    init_MUTEX(&caesar_p1->sem);
    
    /* allocate the buffer */
    caesar_p1->buffer = kmalloc(buffersize, GFP_KERNEL);
    if (!caesar_p1->buffer) {
        result = -ENOMEM;
    }

    caesar_p1->buffersize = buffersize;
    caesar_p1->end = caesar_p1->buffer + caesar_p1->buffersize;
    caesar_p1->rp = caesar_p1->wp = caesar_p1->buffer; /* rd and wr from the beginning */

    return 0; /* succeed */

fail:
    caesar_cleanup_module();
    return result;
}

static void encode(const char *input, char *output, int buffersize, int shiftNum){
    int i = 0;
    int inputSize = get_string_size(input);
    if(inputSize<buffersize){
        buffersize = inputSize;
    }
    for(i = 0; i < buffersize; i++){
        if(i == buffersize-1){
            output[i] = '\0';
        }
        else{
            char c = input[i];
            shift_char(&c,shiftNum);
            output[i] = c;
        } 
    }
}

static void decode(const char *input, char *output, int buffersize, int shiftNum){
    int i = 0;
    int inputSize = get_string_size(input);
    if(inputSize<buffersize){
        buffersize = inputSize;
    }
    for(i = 0; i < buffersize; i++){
        if(i == buffersize-1){
            output[i] = '\0';
        }
        else{
            char c = input[i];
            unshift_char(&c,shiftNum);
            output[i] = c;
        } 
    }
}

static int unshift_char(char* c, int shiftNum){
    //return if not in a-z or A-Z
    int i = 0;
    if(!is_ascii(*c)){
        return 0;
    }
    for( i = 0; i < shiftNum; i++){
        switch (*c){
            case 'a':
                *c = 'z';
                break;
            case 'A':
                *c = 'Z';
                break;
            default:
                *c -= 1;
                break;
        }   
    }
    return 0;
}

static int shift_char(char* c, int shiftNum){
    //return if not in a-z or A-Z
    int i = 0;
    if(!is_ascii(*c)){
        return 0;
    }
    for( i = 0; i < shiftNum; i++){
        switch (*c){
            case 'z':
                *c = 'a';
                break;
            case 'Z':
                *c = 'A';
                break;
            default:
                *c += 1;
                break;
        }   
    }
    return 0;
}

//returns 1 if is ascii, else 0
static int is_ascii(char c){
    if(c >= 'a' && c <= 'z'){
        return 1;
    }
    if(c >= 'A' && c <= 'Z'){
        return 1;
    }
    return 0;
}

static int get_string_size(const char* string){
    int i = 0;
    for( i = 0; i < __UINT32_MAX__;i++){
        if(string[i] == '\0'){
            return i+1;
        }
    }
    return 0;
}

module_init(caesar_init_module);
module_exit(caesar_cleanup_module);

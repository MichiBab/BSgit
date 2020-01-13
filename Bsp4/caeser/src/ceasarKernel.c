/*
 * main.c -- the bare ceasar char module
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

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

//#include <asm/system.h>		/* cli(), *_flags */
/* #include <asm/uaccess.h>	/\* copy_*_user *\/ */
#include <linux/uaccess.h>	/* copy_*_user */

#include "ceasar.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */

int ceasar_major =   CEASAR_MAJOR;
int ceasar_minor =   0;
int ceasar_nr_devs = CEASAR_NR_DEVS;	/* number of bare ceasar devices */
int ceasar_quantum = CEASAR_QUANTUM;
int ceasar_qset =    CEASAR_QSET;

module_param(ceasar_major, int, S_IRUGO);
module_param(ceasar_minor, int, S_IRUGO);
module_param(ceasar_nr_devs, int, S_IRUGO);
module_param(ceasar_quantum, int, S_IRUGO);
module_param(ceasar_qset, int, S_IRUGO);

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

struct ceasar_dev *ceasar_devices;	/* allocated in ceasar_init_module */

/*
 * Empty out the ceasar device; must be called with the device
 * semaphore held.
 */
int ceasar_trim(struct ceasar_dev *dev)
{
    // Funktion wird nicht benötigt, wir benutzen feste Groesse von Geraeten
   return 0;
}

/*
 * Open and close
 */

int ceasar_open(struct inode *inode, struct file *filp)
{
   struct ceasar_dev *dev; /* device information */

    /* use f_mode,not  f_flags: it's cleaner (fs/open.c tells why) */
	if (filp->f_mode & FMODE_READ)
		dev->nreaders++; // todo, es darf maximal ein Reader existieren
	if (filp->f_mode & FMODE_WRITE)
		dev->nwriters++; // todo, es darf maximal ein Writer existieren

   dev = container_of(inode->i_cdev, struct ceasar_dev, cdev);
   filp->private_data = dev; /* for other methods */

   return 0;          /* success */
}

int ceasar_release(struct inode *inode, struct file *filp)
{

	if (filp->f_mode & FMODE_READ)
		dev->nreaders--; // todo, es darf maximal ein Reader existieren
	if (filp->f_mode & FMODE_WRITE)
		dev->nwriters--; // todo, es darf maximal ein Writer existieren
	if (dev->nreaders + dev->nwriters == 0) {
		kfree(dev->buffer);
		dev->buffer = NULL; /* the other fields are not checked on open */
	}
	return 0;
}
/*
 * Follow the list
 */
struct ceasar_qset *ceasar_follow(struct ceasar_dev *dev, int n)
{
  // Funktion wird nicht benötigt, wir benutzen feste Groesse von Geraeten
   return 0;
}

/*
 * Data management: read and write
 */

ssize_t ceasar_read(struct file *filp, char __user *buf, size_t count,
		   loff_t *f_pos)
{
   struct ceasar_pipe *dev = filp->private_data;

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

    // Hier muss ein Aufruf der Funktion encode, decode erfolgen, je nach minior number

	if (copy_to_user(buf, dev->rp, count)) {
		up (&dev->sem);
		return -EFAULT;
	}
	dev->rp += count;
	if (dev->rp == dev->end)
		dev->rp = dev->buffer; /* wrapped */
	up (&dev->sem);

	/* finally, awake any writers and return */
	wake_up_interruptible(&dev->outq);
	PDEBUG("\"%s\" did read %li bytes\n",current->comm, (long)count);
	return count;
}

ssize_t ceasar_write(struct file *filp, const char __user *buf, size_t count,
		    loff_t *f_pos)
{
   struct ceasar_pipe *dev = filp->private_data;
	int result;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	/* Make sure there's space to write */
	result = ceasar_getwritespace(dev, filp);
	if (result)
		return result; /* ceasar_getwritespace called up(&dev->sem) */

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

/*
 * The ioctl() implementation
 */

int ceasar_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg)
{
    // no need for input/output controll
   return 0;

}


struct file_operations ceasar_fops = {
   .owner =    THIS_MODULE,
   .llseek =   ceasar_llseek,
   .read =     ceasar_read,
   .write =    ceasar_write,
   .open =     ceasar_open,
   .release =  ceasar_release,
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void ceasar_cleanup_module(void)
{
   int i;
   dev_t devno = MKDEV(ceasar_major, ceasar_minor);

   /* Get rid of our char dev entries */
   if (ceasar_devices) {
      for (i = 0; i < ceasar_nr_devs; i++) {
	 cdev_del(&ceasar_devices[i].cdev);
      }
      kfree(ceasar_devices);
   }

   /* cleanup_module is never called if registering failed */
   unregister_chrdev_region(devno, ceasar_nr_devs);

   /* and call the cleanup functions for friend devices */
   ceasar_p_cleanup();
}


/*
 * Set up the char_dev structure for this device.
 */
static void ceasar_setup_cdev(struct ceasar_dev *dev, int index)
{
   int err, devno = MKDEV(ceasar_major, ceasar_minor + index);
    
   cdev_init(&dev->cdev, &ceasar_fops);
   dev->cdev.owner = THIS_MODULE;
   dev->cdev.ops = &ceasar_fops;
   err = cdev_add (&dev->cdev, devno, 1);
   /* Fail gracefully if need be */
   if (err)
      printk(KERN_NOTICE "Error %d adding ceasar%d", err, index);
}


int ceasar_init_module(void)
{
   int result, i;
   dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
      result = alloc_chrdev_region(&dev, ceasar_minor, ceasar_nr_devs,
				   "ceasar");
      ceasar_major = MAJOR(dev); // Get a major number from linux

   if (result < 0) {
      printk(KERN_WARNING "ceasar: can't get major %d\n", ceasar_major);
      return result;
   }

   /* 
    * allocate the devices -- we can't have them static, as the number
    * can be specified at load time
    */
   ceasar_devices = kmalloc(ceasar_nr_devs * sizeof(struct ceasar_dev), GFP_KERNEL);
   if (!ceasar_devices) {
      result = -ENOMEM;
      goto fail;  /* Make this more graceful */
   }
   memset(ceasar_devices, 0, ceasar_nr_devs * sizeof(struct ceasar_dev));

   /* Initialize each device. */
      for (i = 0; i < ceasar_nr_devs; i++) {
      ceasar_setup_cdev(&ceasar_devices[i], i);
   }

   /* At this point call the init function for any friend device */
   dev = MKDEV(ceasar_major, ceasar_minor + ceasar_nr_devs);
   dev += ceasar_p_init(dev);

   return 0; /* succeed */

  fail:
   ceasar_cleanup_module();
   return result;
}

module_init(ceasar_init_module);
module_exit(ceasar_cleanup_module);



/*
 * caesar.h -- definitions for the char module
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
 * $Id: caesar.h,v 1.15 2004/11/04 17:51:18 rubini Exp $
 */

#ifndef _CAESAR_H_
#define _CAESAR_H_

#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */

#ifndef init_MUTEX
#define init_MUTEX(mutex) sema_init(mutex, 1)
#endif	/* init_MUTEX */

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef CAESAR_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "caesar: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */

#ifndef CAESAR_MAJOR
#define CAESAR_MAJOR 0   /* dynamic major by default */
#endif

#ifndef CAESAR_NR_DEVS
#define CAESAR_NR_DEVS 2    /* caesar0 through caesar3 */
#endif

#ifndef CAESAR_P_NR_DEVS
#define CAESAR_P_NR_DEVS 4  /* caesarpipe0 through caesarpipe3 */
#endif

/*
 * The bare device is a variable-length region of memory.
 * Use a linked list of indirect blocks.
 *
 * "caesar_dev->data" points to an array of pointers, each
 * pointer refers to a memory area of caesar_QUANTUM bytes.
 *
 * The array (quantum-set) is caesar_QSET long.
 */
#ifndef CAESAR_QUANTUM
#define CAESAR_QUANTUM 4000
#endif

#ifndef CAESAR_QSET
#define CAESAR_QSET    1000
#endif

/*
 * The pipe device is a simple circular buffer. Here its default size
 */
#ifndef CAESAR_P_BUFFER
#define CAESAR_P_BUFFER 4000
#endif

/*
 * Representation of caesar quantum sets.
 */
struct caesar_qset {
	void **data;
	struct caesar_qset *next;
};

struct caesar_dev {
	struct caesar_qset *data;  /* Pointer to first quantum set */
	int quantum;              /* the current quantum size */
	int qset;                 /* the current array size */
	unsigned long size;       /* amount of data stored here */
	unsigned int access_key;  /* used by caesaruid and caesarpriv */
	struct semaphore sem;     /* mutual exclusion semaphore     */
	struct cdev cdev;	  /* Char device structure		*/
};

/*
 * Split minors in two parts
 */
#define TYPE(minor)	(((minor) >> 4) & 0xf)	/* high nibble */
#define NUM(minor)	((minor) & 0xf)		/* low  nibble */


/*
 * The different configurable parameters
 */
extern int caesar_major;     /* main.c */
extern int caesar_nr_devs;
extern int caesar_quantum;
extern int caesar_qset;

extern int caesar_p_buffer;	/* pipe.c */


/*
 * Prototypes for shared functions
 */

int     caesar_p_init(dev_t dev);
void    caesar_p_cleanup(void);
int     caesar_access_init(dev_t dev);
void    caesar_access_cleanup(void);

int     caesar_trim(struct caesar_dev *dev);

ssize_t caesar_read(struct file *filp, char __user *buf, size_t count,
                   loff_t *f_pos);
ssize_t caesar_write(struct file *filp, const char __user *buf, size_t count,
                    loff_t *f_pos);
loff_t  caesar_llseek(struct file *filp, loff_t off, int whence);
int     caesar_ioctl(struct inode *inode, struct file *filp,
                    unsigned int cmd, unsigned long arg);


/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define CAESAR_IOC_MAGIC  'k'
/* Please use a different 8-bit number in your code */

#define CAESAR_IOCRESET    _IO(CAESAR_IOC_MAGIC, 0)

/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift": switch T and Q atomically
 */
#define CAESAR_IOCSQUANTUM _IOW(CAESAR_IOC_MAGIC,  1, int)
#define CAESAR_IOCSQSET    _IOW(CAESAR_IOC_MAGIC,  2, int)
#define CAESAR_IOCTQUANTUM _IO(CAESAR_IOC_MAGIC,   3)
#define CAESAR_IOCTQSET    _IO(CAESAR_IOC_MAGIC,   4)
#define CAESAR_IOCGQUANTUM _IOR(CAESAR_IOC_MAGIC,  5, int)
#define CAESAR_IOCGQSET    _IOR(CAESAR_IOC_MAGIC,  6, int)
#define CAESAR_IOCQQUANTUM _IO(CAESAR_IOC_MAGIC,   7)
#define CAESAR_IOCQQSET    _IO(CAESAR_IOC_MAGIC,   8)
#define CAESAR_IOCXQUANTUM _IOWR(CAESAR_IOC_MAGIC, 9, int)
#define CAESAR_IOCXQSET    _IOWR(CAESAR_IOC_MAGIC,10, int)
#define CAESAR_IOCHQUANTUM _IO(CAESAR_IOC_MAGIC,  11)
#define CAESAR_IOCHQSET    _IO(CAESAR_IOC_MAGIC,  12)

/*
 * The other entities only have "Tell" and "Query", because they're
 * not printed in the book, and there's no need to have all six.
 * (The previous stuff was only there to show different ways to do it.
 */
#define CAESAR_P_IOCTSIZE _IO(CAESAR_IOC_MAGIC,   13)
#define CAESAR_P_IOCQSIZE _IO(CAESAR_IOC_MAGIC,   14)
/* ... more to come */

#define CAESAR_IOC_MAXNR 14

#endif /* _caesar_H_ */

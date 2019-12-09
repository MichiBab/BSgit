/**
 * @file vmaccess.c
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date 2010
 * @brief The access functions to virtual memory.
 */

#include "vmaccess.h"
#include <sys/ipc.h>
#include <sys/shm.h>

#include "syncdataexchange.h"
#include "vmem.h"
#include "debug.h"
/*
 * functions
 */
static int calculatePage(int address);
static int getOffset(int address);
static void checkGcount(void);
static void checkVmemInit(void);

/*
 * static variables
 */

static struct vmem_struct *vmem = NULL; //!< Reference to virtual memory

/**
 * The progression of time is simulated by the counter g_count, which is incremented by 
 * vmaccess on each memory access. The memory manager will be informed by a command, whenever 
 * a fixed period of time has passed. Hence the memory manager must be informed, whenever 
 * g_count % TIME_WINDOW == 0. 
 * Based on this information, memory manager will update aging information
 */

static int g_count = 0;    //!< global acces counter as quasi-timestamp - will be increment by each memory access
#define TIME_WINDOW   20

/**
 *****************************************************************************************
 *  @brief      This function setup the connection to virtual memory.
 *              The virtual memory has to be created by mmanage.c module.
 *
 *  @return     void
 ****************************************************************************************/
static void vmem_init(void) {

    /* Create System V shared memory */
	key_t key = ftok(SHMKEY, SHMPROCID);
	TEST_AND_EXIT_ERRNO(key == -1, "Create key failed");

    /* We are only using the shm, don't set the IPC_CREAT flag */
	int shm_id = shmget(key, SHMSIZE, 0666); // https://stackoverflow.com/questions/40380327/what-is-the-use-of-ipc-creat-0666-flag-in-shmget-function-in-c
	TEST_AND_EXIT_ERRNO(shm_id == -1, "Invalid shared memory identifier");

    /* attach shared memory to vmem */
	vmem = shmat(shm_id, NULL, 0); // 0 Flag for reading and wirting
}

/**
 *****************************************************************************************
 *  @brief      This function puts a page into memory (if required). Ref Bit of page table
 *              entry will be updated.
 *              If the time window handle by g_count has reached, the window window message
 *              will be send to the memory manager. 
 *              To keep conform with this log files, g_count must be increased before 
 *              the time window will be checked.
 *              vmem_read and vmem_write call this function.
 *
 *  @param      address The page that stores the contents of this address will be 
 *              put in (if required).
 * 
 *  @return     void
 ****************************************************************************************/
static void vmem_put_page_into_mem(int address) {
	checkVmemInit();
	int page = calculatePage(address);

	if ((vmem->pt[page].flags & PTF_PRESENT) == 0) {
		struct msg message;
		message.cmd = CMD_PAGEFAULT; 
		message.value = page;
		message.g_count = g_count;
		sendMsgToMmanager(message);
	}
}

int vmem_read(int address) {
	vmem_put_page_into_mem(address);

	int page = calculatePage(address);
	int offset = getOffset(address);

	//set the flag
	vmem->pt[page].flags |= PTF_REF;

	checkGcount();
	return vmem->mainMemory[(vmem->pt[page].frame * VMEM_PAGESIZE) + offset];
}


void vmem_write(int address, int data) {
	vmem_put_page_into_mem(address);

	int page = calculatePage(address);
	int offset = getOffset(address);

	//set the flags
	vmem->pt[page].flags |= PTF_DIRTY; 
	vmem->pt[page].flags |= PTF_REF;
	checkGcount();
	vmem->mainMemory[(vmem->pt[page].frame * VMEM_PAGESIZE) + offset] = data;
}

static int calculatePage(int address) {
	int page = address / VMEM_PAGESIZE;
	TEST_AND_EXIT(page < 0 || page >= VMEM_NPAGES, (stderr, "Page is not in range\n"));
	return page;
}

static int getOffset(int address) {
	return (address % VMEM_PAGESIZE);
}

static void checkGcount() {
	g_count++; // increase before check
	if (g_count % TIME_WINDOW == 0) {
		struct msg message;
		message.cmd = CMD_TIME_INTER_VAL;
		message.g_count = g_count;
		sendMsgToMmanager(message);
	}
}

static void checkVmemInit() {
	if (vmem == NULL) {
		vmem_init();
	}
}

// EOF

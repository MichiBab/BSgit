/**
 * @file mmanage.c
 * @author Prof. Dr. Wolfgang Fohl, HAW Hamburg
 * @date  2014

 * @brief Memory Manager module of TI BSP A3 virtual memory
 * 
 * This is the memory manager process that
 * works together with the vmaccess process to
 * manage virtual memory management.
 *
 * mvappl sends synchronious command to memory manager via
 * module syncdataexchange.
 *
 * This process starts shared memory, so
 * it has to be started prior to the vmaccess process.
 *
 */

#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mmanage.h"
#include "debug.h"
#include "pagefile.h"
#include "logger.h"
#include "syncdataexchange.h"
#include "vmem.h"

/*
 * Signatures of private / static functions
 */

/**
 *****************************************************************************************
 *  @brief      This function fetchs a page from disk into memory. The page table 
 *              will be updated.
 *
 *  @param      page Number of the page that should be removed
 *  @param      frame Number of frame that should contain the page.
 * 
 *  @return     void 
 ****************************************************************************************/
static void fetchPage(int page, int frame);

/**
 *****************************************************************************************
 *  @brief      This function removes a page from main memory. If the page was modified,
 *              it will be written back to disk. The page table will be updated.
 *
 *  @param      page Number of the page that should be removed
 * 
 *  @return     void 
 ****************************************************************************************/
static void removePage(int page);

/**
 *****************************************************************************************
 *  @brief      This function initializes the virtual memory.
 *              In particular it creates the shared memory. The application just attachs 
 *              to the shared memory.
 *
 *  @return     void 
 ****************************************************************************************/
static void vmem_init(void);

/**
 *****************************************************************************************
 *  @brief      This function finds an unused frame. At the beginning all frames are 
 *              unused. A frame will never change it's state form used to unused.
 *
 *              Since the log files to be compared with contain the allocated frames, unused 
 *              frames must always be assigned the same way. Here, the frames are assigned 
 *              according to ascending frame number.
 *            
 *  @return     idx of the unused frame with the smallest idx. 
 *              If all frames are in use, VOID_IDX will be returned.
 ****************************************************************************************/
static int find_unused_frame();

/**
 *****************************************************************************************
 *  @brief      This function will be called when a page fault has occurred. It allocates 
 *              a new page into memory. If all frames are in use the corresponding page 
 *              replacement algorithm will be called.
 *              Please take into account that allocate_page must update the page table 
 *              and log the page fault as well.
 *
 *  @param      req_page  The page that must be allocated due to the page fault. 

 *  @param      g_count   Current g_count value
 *
 *  @return     void 
 ****************************************************************************************/
static void allocate_page(const int req_page, const int g_count);

/**
 *****************************************************************************************
 *  @brief      This function is the signal handler attached to system call sigaction
 *              for signal SIGUSR2 and SIGINT.
 *              These signals have the same signal handler. Based on the parameter 
 *              signo the corresponding action will be started.
 *
 *  @param      signo Current signal that has be be handled.
 * 
 *  @return     void 
 ****************************************************************************************/
static void sighandler(int signo);

/**
 *****************************************************************************************
 *  @brief      This function dumps the page table to stderr.
 *
 *  @return     void 
 ****************************************************************************************/
static void dump_pt(void);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm aging.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_aging(int page, int * removedPage, int *frame);
 
/**
 *****************************************************************************************
 *  @brief      This function does aging for aging page replacement algorithm.
 *              It will be called periodic based on g_count.
 *              This function must be used only when aging algorithm is activ.
 *              Otherwise update_age_reset_ref may interfere with other page replacement 
 *              alogrithms that base on PTF_REF bit.
 *
 *  @return     void
 ****************************************************************************************/
static void update_age_reset_ref(void);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm fifo.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_fifo(int page, int * removedPage, int *frame);

/**
 *****************************************************************************************
 *  @brief      This function implements page replacement algorithm clock.
 *
 *  @param      page Number of page that should be loaded into memory.
 *
 *  @param      removedPage Number of page that has been selected for replacement.
 *              If an unused frame has selected, this parameter will not be 
 *              modified.
 *
 *  @param      frame Number of frame that will be used to store the page.
 *
 ****************************************************************************************/
static void find_remove_clock(int page, int * removedPage, int *frame);

/**
 *****************************************************************************************
 *  @brief      This function cleans up when mmange runs out.
 *
 *  @return     void 
 ****************************************************************************************/
static void cleanup(void) ;

/**
 *****************************************************************************************
 *  @brief      This function scans all parameters of the porgram.
 *              The corresponding global variables page_rep_algo will be set.
 * 
 *  @param      argc number of parameter 
 *
 *  @param      argv parameter list 
 *
 *  @return     void 
 ****************************************************************************************/
static void scan_params(int argc, char **argv);

/**
 *****************************************************************************************
 *  @brief      This function prints an error message and the usage information of 
 *              this program.
 *
 *  @param      err_str pointer to the error string that should be printed.
 *  @param      programName pointer to the name of the the program
 *
 *  @return     void 
 ****************************************************************************************/
static void print_usage_info_and_exit(char *err_str, char * programName);

/*
 * variables for memory management
 */

static int pf_count = 0;               //!< page fault counter
static int shm_id = -1;                //!< shared memory id. Will be used to destroy shared memory when mmanage terminates

static void (*pageRepAlgo) (int, int*, int*) = NULL; //!< selected page replacement algorithm according to parameters of mmanage

/* information used for ageing replacement strategy. For each frame, which stores a valid page, 
 * the age and and the corresponding page will be stored.
 */

struct age {
   unsigned char age;  //!< 8 bit counter for aging page replacement algorithm
   int page;           //!< page belonging to this entry
 };

struct age age[VMEM_NFRAMES];

static struct vmem_struct *vmem = NULL; //!< Reference to shared memory

int main(int argc, char **argv) {
    struct sigaction sigact;
    init_pagefile(); // init page file
    open_logger();   // open logfile

    // Setup IPC for sending commands from vmapp to mmanager
    setupSyncDataExchange();

    // Create shared memory and init vmem structure 
    vmem_init();
    TEST_AND_EXIT_ERRNO(!vmem, "Error initialising vmem");
    PRINT_DEBUG((stderr, "vmem successfully created\n"));

    // init aging info
    for(int i = 0; i < VMEM_NFRAMES; i++) {
       age[i].page = VOID_IDX;
       age[i].age = 0;
    }

    // scan parameter 
    pageRepAlgo = find_remove_fifo;
    scan_params(argc, argv);

    /* Setup signal handler */
    sigact.sa_handler = sighandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART; // damit mq_receive man eine signal neu gestartet wird

    TEST_AND_EXIT_ERRNO(sigaction(SIGUSR2, &sigact, NULL) == -1, "Error installing signal handler for USR2");
    PRINT_DEBUG((stderr,  "USR2 handler successfully installed\n"));

    TEST_AND_EXIT_ERRNO(sigaction(SIGINT, &sigact, NULL) == -1, "Error installing signal handler for INT");
    PRINT_DEBUG((stderr, "INT handler successfully installed\n"));

    // Server Loop, waiting for commands from vmapp
    while(1) {
        struct msg m = waitForMsg();
        switch(m.cmd){
	   case CMD_PAGEFAULT:
                 allocate_page(m.value, m.g_count);
              break;
           case CMD_TIME_INTER_VAL:
                if (pageRepAlgo == find_remove_aging) {
                   update_age_reset_ref();
                }
              break;
           default:
              TEST_AND_EXIT(true, (stderr, "Unexpected command received from vmapp\n"));
        }
        sendAck();
    }
    return 0;
}

void scan_params(int argc, char **argv) {
    int i = 0;
    bool param_ok = false;
    char * programName = argv[0];

    // scan all parameters (argv[0] points to program name)
    if (argc > 2) print_usage_info_and_exit("Wrong number of parameters.\n", programName);

    for (i = 1; i < argc; i++) {
        param_ok = false;
        if (0 == strcasecmp("-fifo", argv[i])) {
            // page replacement strategies fifo selected 
            pageRepAlgo = find_remove_fifo;
            param_ok = true;
        }
        if (0 == strcasecmp("-clock", argv[i])) {
            // page replacement strategies clock selected 
            pageRepAlgo = find_remove_clock;
            param_ok = true;
        }
        if (0 == strcasecmp("-aging", argv[i])) {
            // page replacement strategies aging selected 
            pageRepAlgo = find_remove_aging;
            param_ok = true;
        }
        if (!param_ok) print_usage_info_and_exit("Undefined parameter.\n", programName); // undefined parameter found
    } // for loop
}

void print_usage_info_and_exit(char *err_str, char *programName) {
    fprintf(stderr, "Wrong parameter: %s\n", err_str);
    fprintf(stderr, "Usage : %s [OPTIONS]\n", programName);
    fprintf(stderr, " -fifo     : Fifo page replacement algorithm.\n");
    fprintf(stderr, " -clock    : Clock page replacement algorithm.\n");
    fprintf(stderr, " -aging    : Aging page replacement algorithm.\n");
    fprintf(stderr, " -pagesize=[8,16,32,64] : Page size.\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
}

void sighandler(int signo) {
if(signo == SIGUSR2) {
        dump_pt();
    } else if(signo == SIGINT) {
        cleanup();
        exit(EXIT_SUCCESS);
    }  
}

void dump_pt(void) {
    int i;
    int ncols = 8;

    fprintf(stderr,
            "\n======================================\n"
            "\tPage Table Dump\n");

    fprintf(stderr, "VIRT MEM SIZE    = \t %d\n", VMEM_VIRTMEMSIZE);
    fprintf(stderr, "PHYS MEM SIZE    = \t %d\n", VMEM_PHYSMEMSIZE);
    fprintf(stderr, "PAGESIZE         = \t %d\n", VMEM_PAGESIZE);
    fprintf(stderr, "Number of Pages  = \t %d\n", VMEM_NPAGES);
    fprintf(stderr, "Number of Frames = \t %d\n", VMEM_NFRAMES);

    fprintf(stderr, "======================================\n");
    fprintf(stderr, "shm_id: \t %x\n", shm_id);
    fprintf(stderr, "pf_count: \t %d\n", pf_count);
    for(i = 0; i < VMEM_NPAGES; i++) {
        fprintf(stderr,
                "Page %5d, Flags %x, Frame %10d, age 0x%2X,  \n", i,
                vmem->pt[i].flags, vmem->pt[i].frame, age[vmem->pt[i].frame].age);
    }
    fprintf(stderr,
            "\n\n======================================\n"
            "\tData Dump\n");
    for(i = 0; i < (VMEM_NFRAMES * VMEM_PAGESIZE); i++) {
        fprintf(stderr, "%10x", vmem->mainMemory[i]);
        if(i % ncols == (ncols - 1)) {
            fprintf(stderr, "\n");
        }
        else {
            fprintf(stderr, "\t");
        }
    }
}

/* Your code goes here... */

#define SET_ACCESS_PERMISSION 0666
#define START_AGE 0x80
#define CLEAR_FLAGS 0

static int unused_frame_counter = 0;

static int fifo_counter = 0;

static int clock_counter = 0;

static int init_pagetable(){

	for(int i = 0; i<VMEM_NPAGES; i++){
		//init all as INVALID PAGE
		vmem->pt[i].frame = VOID_IDX;
	}
	return 0;
}

void cleanup(void) {
	destroySyncDataExchange();
}

void vmem_init(void) {
    /* Create System V shared memory */
    //Pathname and Project Identifier to key
	key_t vmem_key = ftok(SHMKEY, SHMPROCID);
	TEST_AND_EXIT_ERRNO(vmem_key == -1, "vmem_init: Error on ftok");
    /* We are creating the shm, so set the IPC_CREAT flag */
	shm_id = shmget(vmem_key, SHMSIZE, IPC_CREAT | SET_ACCESS_PERMISSION);
	TEST_AND_EXIT_ERRNO(shm_id == -1, "vmem_init: Error on shmget");
    /* Attach shared memory to vmem (virtual memory) */
    //attaches the shared memory segment identified 
    //by shmid to the address space of the calling process
	vmem = shmat(shm_id, NULL, 0);
	TEST_AND_EXIT_ERRNO(vmem == (void *) -1,"vmem_init: Error on shmat");
    /* Fill with zeros */
    memset(vmem, 0, SHMSIZE);
    //init pagetable
    init_pagetable();
}

int find_unused_frame() {
	int temp = unused_frame_counter;
	if( temp < VMEM_NFRAMES){
		unused_frame_counter++;
		return temp;
	}

	return VOID_IDX;
}

void allocate_page(const int req_page, const int g_count) {

    struct logevent le;
    int frame = find_unused_frame();
    //for paging algos
    int removedPage = VOID_IDX;
    //if no free space
    if(frame == VOID_IDX){
    	//through scan method defined
    	pageRepAlgo(req_page, &removedPage, 
    					&frame);
    }
    pf_count++;//increment page fault counter
    fetchPage(req_page,frame);
    /* Log action */
    le.req_pageno = req_page;
    le.replaced_page = removedPage;
    le.alloc_frame = frame;
    le.g_count = g_count; 
    le.pf_count = pf_count;
    logger(le);
}

//-This function fetchs a page from disk into memory
//-@page: Number of page that should be store the frame
//-@frame: Number of frame that should contain the page.
void fetchPage(int page, int frame){
	int * page_frame = &vmem->mainMemory[frame * VMEM_PAGESIZE];
	fetch_page_from_pagefile(page, page_frame);
	vmem->pt[page].frame = frame;	//save which frame the page contain
	vmem->pt[page].flags |= PTF_PRESENT; //Tag the page as present
}

void removePage(int page) {
	//Check if the page contains dirty data 
	if ((vmem->pt[page].flags & PTF_DIRTY) == PTF_DIRTY) {  
		store_page_to_pagefile(page, 
			&vmem->mainMemory[vmem->pt[page].frame * VMEM_PAGESIZE]); 
	}
	vmem->pt[page].flags = CLEAR_FLAGS; //clear flags
	vmem->pt[page].frame = VOID_IDX;
}


void find_remove_fifo(int page, int * removedPage, int *frame){
	*frame = fifo_counter;
	for(int i = 0; i < VMEM_NPAGES ; i++){
		if(vmem->pt[i].frame == fifo_counter){
			*removedPage = i;
			i = VMEM_NPAGES;//break out
		}
	}
	removePage(*removedPage);
	//update fifo counter
	fifo_counter++;
	fifo_counter %= VMEM_NFRAMES;
	
}

static void find_remove_aging(int page, int * removedPage, int *frame){
	int smallest_index = 0;
	for(int i = 0; i<VMEM_NFRAMES;i++){
		if(age[i].page != VOID_IDX){
			if(age[i].age <= age[smallest_index].age){
				smallest_index = i;
			}
		}
	}
	*frame = smallest_index;
	*removedPage = age[smallest_index].page;
	removePage(*removedPage);
	age[smallest_index].page = page;
	age[smallest_index].age = START_AGE;

}

static void update_age_reset_ref(void) {
	for(int i = 0; i<VMEM_NPAGES;i++){
		if (vmem->pt[i].frame != VOID_IDX) { // -check if its valid
			age[vmem->pt[i].frame].age >>= 1; 
			age[vmem->pt[i].frame].page = i; // set page
			if ((vmem->pt[i].flags & PTF_REF) == PTF_REF) {//if referenced
				age[vmem->pt[i].frame].age |= START_AGE; //give age update
				vmem->pt[i].flags &= ~PTF_REF; //erase ref flag
			}
		}

	}
} 

//searches for first not referenced page. if a page is referenced, 
//it will be marked as unreferenced and the next one is checked in a cycle
static void find_remove_clock(int page, int * removedPage, int *frame){
	int exitbool = 1;
	while(exitbool){
		for (int i = 0; i < VMEM_NPAGES; i++) {
			if(vmem->pt[i].frame == clock_counter){
				if (!(vmem->pt[i].flags & PTF_REF)) { //-Check if the REF flag is NOT set
					*removedPage = i;
					*frame = clock_counter;
					exitbool = 0;
				}
				else {
					vmem->pt[i].flags &= ~PTF_REF; //-Reset REF flag
				}
				i = VMEM_NPAGES; //break out
			}
		}
		clock_counter++;
		clock_counter %= VMEM_NFRAMES ;
	}
	removePage(*removedPage);
}

// EOF


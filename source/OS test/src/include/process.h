#ifndef _PROCESSH
#define _PROCESSH
#define PROCESS_STATE_START     1
#define PROCESS_STATE_RUNNABLE  2
#define PROCESS_STATE_RUNNING   3
#define PROCESS_STATE_WAIT      4
#define PROCESS_STATE_STOPPED   5

#define PROCESS_SIGNAL_QUEUES   32

typedef struct {
   int pid;
   int state;
   int cpucount;
   struct {
     int clockticks;
   } stats;
   void *addr_table_pages;
   void *userst;
   int task_seg;
   void *next;
} st_process;

struct {
   st_process *running;
   st_process *runnable;
   st_process *wait[PROCESS_SIGNAL_QUEUES];
   st_process *stopped;
} st_process_list;

void mi_switch();

int create_process();

void start_time_share();

void runprocess(st_process *);

void waitprocess(st_process *, int signal);

void start_kernel_process();

void save_registers();
void restore_registers();
#endif

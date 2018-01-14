// File: process.h
// This file contains functions for
// processes
// Created: 2004-05-22 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <mm.h>
#include <pic.h>
#include <intel/tss.h>

void hlt_process();
st_process *find_last_runable_process();
int cur_pid = 0;

void mi_switch() 
{
  char str[10];
  putstring("mi_switch ");
  itoa(st_process_list.running, str , 10);
  putstring("Running ptr: ");
  putstring(str);
  putstring("\n");

/*  st_process *i;
  for ( i = st_process_list.runnable; i != 0; i = i->next)
  {
    itoa(i->pid, str , 10);;
    putstring(str);
    putstring(" ");
  }
  putstring("\n");  */
  if ( st_process_list.running != 0) 
  {
     itoa(st_process_list.running->pid, str , 10);;
     putstring("Executing process: ");
     putstring(str);
     putstring("\n");
     st_process_list.running->cpucount ++;
  }
  if ( /*st_process_list.running->cpucount%5 == 0 && */st_process_list.runnable != 0 )
  {
    putstring("Change process\n");
    st_process *last_process = (st_process *)find_last_runable_process();
    last_process->next = st_process_list.running;
    st_process_list.running->next = 0;
    st_process_list.running = st_process_list.runnable;
    st_process_list.runnable = st_process_list.runnable->next;
    itoa(st_process_list.running->pid, str , 10);;
    putstring("Jump to process: ");
    putstring(str);
    putstring("\n");
//    asm("hlt\n");
//    exit_irq();
    asm("ljmp $0x40, $0x0\n");
    
/*      :
        : "g" (st_process_list.running->task_seg)); */
/*    asm("pushl %0\n"
        "pushl %1\n"
        "pushl $0\n"
        :
        : "g" (((st_TSS32 *)(st_process_list.running->userst))->EFLAGS), "g" (st_process_list.running->task_seg));
    */
/*    asm("pushl %0\n"
        "pushl $0x40\n"
        "pushl $0x0\n"
        :
        : "g" (((st_TSS32 *)(st_process_list.running->userst))->EFLAGS)); 
    asm("iret\n");   */
  }
}

/* Creates a user process */
int create_process() 
{
  // Create table space
  // Allocate memory
  // Set process information
  // Make process runnable
  char str[10];
  st_process *new_process = kmem_alloc(sizeof(st_process));
  new_process->pid = cur_pid;
  cur_pid++;

  itoa(new_process->pid, str , 10);
  putstring("Process created no ");
  putstring(str);
  putstring("\n");
  runprocess(new_process);
  /* Create an other kernel process */
  new_process->addr_table_pages = (void *)0x1000;
  st_TSS32 *tss = kmem_alloc(sizeof(st_TSS32));
  tss->CS = 0x10;
  tss->EIP= (int)hlt_process;
  tss->SS = 0x20;
  tss->ES = 0x18;
  tss->DS = 0x18;
  tss->FS = 0x18;
  tss->GS = 0x18;
  tss->CR3 = 0x1000;
  tss->IO_MAP = 0x0;
  tss->EFLAGS = 0x200;
  new_process->userst = tss;
  new_process->task_seg = 0x40;
  st_TSS_desc *tss_desc = GDT_ADDR+8*8;
    tss_desc->ilowlimit    = 0x67;
    tss_desc->ilowbase1    = ((int)tss) & 0x0000ffff;
    tss_desc->ilowbase2    = (((int)tss) >> 16) & 0xff;
    tss_desc->itype        = TSS_TYPE;
    tss_desc->izero        = 0;
    tss_desc->idpl         = 0;
    tss_desc->ipresent     = 1;
    tss_desc->ihighlimit   = 0;
    tss_desc->iavl         = 1;
    tss_desc->izero2       = 0;
    tss_desc->igran        = 1;
    tss_desc->ihighbase    = ((int)tss) >> 24; 
/*  get_table_dir(&(new_process->addr_table_pages));
  itoa(new_process->addr_table_pages, str , 10);
  putstring("Table dir addr ");
  putstring(str);
  putstring("\n"); */
  /* Allocate 4 Mb to process*/
/*  set_mmap(new_process->addr_table_pages, 0, 0x40000); */
  return new_process->pid;
}

st_process *find_last_runable_process() 
{
  st_process *one_process;
  one_process = st_process_list.runnable;
  if ( st_process_list.runnable == 0 )
    return 0;
  while ( one_process->next != 0);
  return one_process;
}

void start_time_share() {
  st_process_list.runnable = 0;
  st_process_list.stopped = 0;
  int i;
  for(i=0; i< PROCESS_SIGNAL_QUEUES;i++)
    st_process_list.wait[i] = 0;
}

void runprocess(st_process *new_process)
{
//  putstring("runprocess  find last process  ");
  // Add new process at end of running process list
  st_process *last_process = (st_process *)find_last_runable_process();
  if ( last_process == 0 )
    st_process_list.runnable = new_process;
  else 
    last_process->next = new_process;
  new_process->next = 0;
  char str[10];  
  putstring("  processes ");
  st_process *i;
  for ( i = st_process_list.runnable; i != 0; i = i->next)
  {
    itoa(i->pid, str , 10);
    putstring(str);
    putstring(" ");
  }
  putstring("\n"); 
}

void runnableprocess(st_process *process)
{

}

void waitprocess(st_process *process, int signal)
{

}

void start_kernel_process()
{
  st_process *new_process = kmem_alloc(sizeof(st_process));
  new_process->pid = 0;
  cur_pid = 1;
  new_process->addr_table_pages = (void *)0x1000;
  new_process->userst = 0;
  new_process->next = 0;
  new_process->task_seg = 0x38;
  st_process_list.running = new_process;
  setup_kernel_process();
}
/*
void save_registers()
{
    asm("pusha\n");
    asm("pushf\n");
    st_user_struct *us = (st_user_struct *)st_process_list.running->userst;
    asm("popf\n");
    asm("popa\n"); 
*/
    /* Intel x86 */
/*
    asm("movl %%eax, %0\n"
        "movl %%ebx, %1\n"
        "movl %%ecx, %2\n"
        "movl %%edx, %3\n"
        "movl %%ds,  %4\n"
        "movl %%es,  %5\n"
        "movl %%fs,  %6\n"
        "movl %%gs,  %7\n"
        "movl %%ss,  %8\n"
        "movl %%esp,  %9\n"
        "movl %%ebp,  %10\n"
        "pop  %%ebx\n"      // Save return address
//        "pop  %%edx\n"      
        "pop %%eax\n"       // Get EIP
        "movl %%eax, %11\n"
        "pop %%eax\n"       // Get CS
        "movl %%eax, %12\n"
        "pop %%eax\n"       // Get EFLAGS
        "movl %%eax, %13\n"
//        "push %%edx\n"
        : "=g" (us->eax), "=g" (us->ebx), 
          "=g" (us->ecx), "=g" (us->edx), "=g" (us->ds),
          "=g" (us->es),  "=g" (us->fs),  "=g" (us->gs), "=g" (us->ss), "=g" (us->sp), "=g" (us->bp),
          "=g" (us->eip), "=g" (us->cs),  "=g" (us->flags) ); 
   asm("pushl %0\n"
       "pushl %1\n"
       "pushl %2\n"
       "pushl %%ebx\n"
       :
       : "g" (us->flags), "g" (us->cs), "g" (us->eip));
//    asm("jmp _ret_save\n");
}
*/

/*
void switch_task()
{
    st_user_struct *us = (st_user_struct *)st_process_list.running->userst;
*/
    /* Intel x86 */
/*
    asm("pushl %0\n"  
        "popl  %%eax\n"
        "pushl %1\n"  
        "popl  %%ebx\n"
        "pushl %2\n"  
        "popl  %%ecx\n"
        "pushl %3\n"  
        "popl  %%edx\n"
        "pushl %4\n"  
        "popl  %%ds\n" 
        "pushl %5\n"  
        "popl  %%es\n"  
        :
        : "g" (us->eax), 
          "g" (us->ebx), 
          "g" (us->ecx), "g" (us->edx), "g" (us->ds),
          "g" (us->es));

    asm("pushl %0\n"   
        "popl  %%fs\n" 
        "pushl %1\n"  
        "popl  %%gs\n"  
        "pushl %2\n"  
        "popl  %%ss\n" 
        "pushl %3\n"  
        "popl  %%esp\n" 
        "pushl %4\n"  
        "popl  %%ebp\n"
        "pushl %5\n" 
//        "popf\n" 
        :
        : "g" (us->fs), "g" (us->gs) , "g" (us->ss), "g" (us->sp), "g" (us->bp), "g" (us->flags));
    asm("movl %0, %%eax\n"
        "movl %%eax, %%cr3\n"      //Set correct page dir
//        "ljmp %7, %8\n"   
        "pushl %1\n"
        "pushl %2\n"
        "iret\n"
        :
        : 
          "g" ((int)st_process_list.running->addr_table_pages),  "g" (us->eip), "g" (us->cs));    
}
*/

void hlt_process()
{
    asm("movl   $0x6ffff, %eax");
    asm("movl   %eax, %esp");
    while(1)
    {
//        putstring("1");
    }
}

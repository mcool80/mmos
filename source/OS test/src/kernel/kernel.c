// 80x86
// File kernel.c
// Contains:
// Setup for the kernel
// This file is intel-specfic
// Created: 2003-12-22
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------
asm("jmp _kernel_startup\n");
#include <kernel.h>
#include <idt.h>
#include <pic.h>
#include <stdio.h>
#include <keyboard.h>
#include <pc.h>
#include <test.h>
//#include <page.h>
#include <stdlib.h>
#include <pit.h>
#include <process.h>

void kernel_startup(void)
{
   int c;
   // Setup pages for the kernel process
   kernel_page_init(32);
   init_frame_handle(getmemsize());
   // Setup memory heap use in kernel
   init_kmem(0x00060000, 0x00070000);   
   // Create a process for the kernel
   start_kernel_process();
   // Create interrupt list
   setidt(KERNEL_SEL, interrupt_0,0,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_1,1,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_2,2,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_3,3,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);

   setidt(KERNEL_SEL,interrupt_4,4,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_5,5,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_6,6,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_7,7,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);

   setidt(KERNEL_SEL,interrupt_8,8,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_9,9,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_10,10,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_11,11,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);

   setidt(KERNEL_SEL,interrupt_12,12,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_13,13,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_14,14,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_15,15,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);

   setidt(KERNEL_SEL,interrupt_16,16,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_17,17,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_18,18,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_19,19,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE);
   setidt(KERNEL_SEL,interrupt_20,20,IDTINTERRUPT_GATE, IDTPRESENT, IDTOS_MODE); 
   idt_init();
   // Start interrupt handling
   // Set up the PIC (Programmable Interrupt Controller)
   pic_init();

   // Start process handling
   pit_init();
   
   // Release control to idle process

// Test creating a new process...
create_process();
/*   clearscreen();
   char str[10];

   int memsize = getmemsize();
   itoa((int)memsize,str,10);
   putstring("\nMemorysize: ");
   putstring(str);
   putstring("\n");
  

  
//   start_test();
   init_kmem(0xff002000,0xff004000);
   start_time_share();
   putstring("Test:");
   itoa(0xff001234,str,16);
   putstring(str);
   
   
   void *ny1 = kmem_alloc(100);
   itoa((int)ny1,str,16);
   putstring("\nPointer:");
   putstring(str);
   void *ny2 = kmem_alloc(100);
   itoa((int)ny2,str,16);
   putstring("\nPointer:");
   putstring(str);
   
   void *ny3 = kmem_alloc(100);
   putstring("\nPointer:");
   itoa((int)ny3,str,16);
   putstring(str);  
   
   kmem_free(ny2);
   ny2 = kmem_alloc(101);
   itoa((int)ny2,str,16);
   putstring("\nPointer:");
   putstring(str);
   keyboard_setup();
//   start_time_share();
   disable_irq(0);
   create_process();
   create_process();
   create_process();
   create_process();
   create_process(); */
   c = 1;
   asm("jmp _dispatch\n"); //dispatch();
/*   while(c==1) {
    c = 1;
//      c = getch();
//      putchar(c);
   }   */
   
   asm("hlt");
}

int getmemsize() 
{
   int temp;
   // Read extended memory lo
   outportb(0x70, 0x17);
   temp = inportb(0x71);

   // Read extended memory hi
   outportb(0x70, 0x18);
   temp += inportb(0x71) << 8;

   memorysize = temp;
   return temp;
}

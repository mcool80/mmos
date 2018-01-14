// File: pic.c
// This file contains the functions
// for handleing chip 8259 - the PIC
// Programmable Interrupt Controller
// This is intel-specific code
// Some code from GazOS
// Created: 2001-11-19 Markus Svensson
// Modified:

#include <pic.h>
#include <conio.h>

int irq_mask=0xffff;

void enable_irq(int irq_no) 
{
   // Take the complement after setting the correct bit 
   // Every bit is connected to a irq 
   irq_mask &= ~(1 << irq_no);
      if(irq_no >= 8)			// IF the IRQ is GT 8 => slave PIC handels that 
         irq_mask &= ~(1 << 2);

   // Set up the new IRQ mask 
   outportb(PICMASTER+1, irq_mask & 0xFF);
   outportb(PICSLAVE+1, (irq_mask >> 8) & 0xFF);
}


void disable_irq(int irq_no) 
{
   // Take the complement after setting the correct bit 
   // Every bit is connected to a irq 
   irq_mask |= ~(1 << irq_no);
      if(irq_no >= 8)			// IF the IRQ is GT 8 => slave PIC handels that 
         irq_mask |= ~(1 << 2);

   // Set up the new IRQ mask 
   outportb(PICMASTER+1, irq_mask & 0xFF);
   outportb(PICSLAVE+1, (irq_mask >> 8) & 0xFF);
   irq_mask |= (1 << irq_no);
}

void exit_irq() 
{
  // Exits an interrupt call
  outportb(PICMASTER,PICEOI);	
}

void pic_init() 
{
   
   outportb(PICMASTER, PICICW1);    	// Start 8259 initialization
   outportb(PICSLAVE, PICICW1);

   outportb(PICMASTER+1, PICMASTERVEC); // Base interrupt vector
   outportb(PICSLAVE+1, PICSLAVEVEC);

   outportb(PICMASTER+1, 1<<2);     	// Bitmask for cascade on IRQ 2
   outportb(PICSLAVE+1, 2);        	// Cascade on IRQ 2
	
   outportb(PICMASTER+1, PICICW4);     	// Finish 8259 initialization
   outportb(PICSLAVE+1, PICICW4);

   outportb(PICMASTERIMR, 0xff);       	// Mask all interrupts
   outportb(PICSLAVEIMR, 0xff);
}

/* File: pic.c
 * This file contains the functions
 * for handleing chip 8259 - the PIC
 * Programmable Interrupt Controller
 * Some code from GazOS
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 */

#include <pic.h>
#include <conio.h>

int irq_mask=0xffff;

void enable_irq(int irq_no) {
   /* Take the complement after setting the correct bit */
   /* Every bit is connected to a irq */
   irq_mask &= ~(1 << irq_no);
      if(irq_no >= 8)			/* IF the IRQ is GT 8 => slave PIC handels that */
         irq_mask &= ~(1 << 2);

   /* Set up the new IRQ mask */
   outportb(MPIC+1, irq_mask & 0xFF);
   outportb(SPIC+1, (irq_mask >> 8) & 0xFF);
}

void disable_irq(int irq_no) {
   /* Take the complement after setting the correct bit */
   /* Every bit is connected to a irq */
   irq_mask |= ~(1 << irq_no);
      if(irq_no >= 8)			/* IF the IRQ is GT 8 => slave PIC handels that */
         irq_mask |= ~(1 << 2);

   /* Set up the new IRQ mask */
   outportb(MPIC+1, irq_mask & 0xFF);
   outportb(SPIC+1, (irq_mask >> 8) & 0xFF);
   irq_mask |= (1 << irq_no);
}

void pic_init() {
       
   outportb(MPIC, ICW1);    	/* Start 8259 initialization    */
   outportb(SPIC, ICW1);

   outportb(MPIC+1, MVEC);    	/* Base interrupt vector        */
   outportb(SPIC+1, SVEC);

   outportb(MPIC+1, 1<<2);     	/* Bitmask for cascade on IRQ 2 */
   outportb(SPIC+1, 2);        	/* Cascade on IRQ 2             */
	
   outportb(MPIC+1, ICW4);     	/* Finish 8259 initialization   */
   outportb(SPIC+1, ICW4);	

   outportb(MIMR, 0xff);       	/* Mask all interrupts          */
   outportb(SIMR, 0xff);
   puts("\eG..ok\n");
   puts("\en");
}

/* File: idt.c
 * This file contains the functions
 * for handling interrupts
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 * # 1.001  2001-12-02 Markus Svensson Add inits to the init function
 * # 0.002  2001-04-23 Markus Svensson Changing definitions adding mos.h
 */

#include <idt.h>
#include <mem.h>
#include <kernel.h>

// # 0.002
#include <mos.h>
// # 0.002

void idt_init(void) {
// # 1.001
   int i;
   
   /* Set up all exceptions interrupts to unhandle function */
   for (i=0;i < 0x20; i++)
      setidt(unhand, i, BITS32+INTERRUPT_GATE, 1, 0); 

   /* This will take care om execptions - in the future */
   setidt(isr0, 0, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr1, 1, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr2, 2, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr3, 3, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr4, 4, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr5, 5, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr6, 6, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr7, 7, BITS32+INTERRUPT_GATE, 1, 0);
   setidt(isr8, 8, BITS32+INTERRUPT_GATE, 1, 0);    
// # 1.001
   puts("\eGIDT..ok\n");
   puts("\en");

}

void setidt(void *handler,int interrupt_no,int type, int present, int privilige)
{
   
   /* Creates a IDT entry in memory */
   IDTR idtr;

   /* And adds all data given to that entry */
   idtr.low_offset=(unsigned short) (((unsigned long)handler&0xffff));	/* offsets for the function */
   idtr.high_offset=(unsigned short) (((unsigned long)handler) >> 16);

   idtr.present = present;						/* Present bit */
   idtr.DPL = privilige;						/* Type of privilege */
   idtr.type= type;							/* Type of interrupt, if it is a trap or interrupt */
   idtr.selector=0x10;							/* Selectorn is allways 0x10 - it will probably change in future versions */
   idtr.none=0;								/* Not used bits, set to zero */

   /* Put the data in a union so it can be written in equal chunks */
   union_idtr uidtr;							
   uidtr.idtr=idtr;
	
   /* Writes the data in the correct position in memory */
   unsigned int value = uidtr.mem[0];
// # 0.002
   writemem(DATA_KERNEL_SELECTOR, KERNEL_IDT+interrupt_no*8, value);	/* Write the first dword in ds:offset */
   value = uidtr.mem[1];
   writemem(DATA_KERNEL_SELECTOR, KERNEL_IDT+interrupt_no*8+4,value); 	/* Write the second dword in ds:offset */
// # 0.002
}

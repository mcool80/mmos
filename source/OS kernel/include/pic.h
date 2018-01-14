/* File: pic.h
 * This file contains the defintions
 * for handleing chip 8259 - the PIC
 * Programmable Interrupt Controller
 * Some code from GazOS
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 */

/* The chip is 8259 */
#ifndef _PICH
#define _PICH
#define MPIC	0x20		/* I/O Adress for Master PIC 		*/
#define SPIC	0xA0		/* I/O Adress for Master PIC 		*/
#define MIMR	0x21		/* Interrupt mask Routin		*/
#define MVEC	0x60		/* Master vector used when interrupt 	*/
#define SIMR	0xA1		/* Interrupt mask Routin		*/
#define SVEC	0x70		/* Slave vector used when interrupt 	*/
#define EOI	0x20		/* End of interrupt			*/
#define INTERRUPTNO  0x60  	/* First interrupt number		*/
#define ICW1   0x11     	/* Cascade, Edge triggered         	*/
                        	/* ICW2 is vector                  	*/
                        	/* ICW3 is slave bitmap or number  	*/

#define ICW4   0x01     	/* 8088 mode                       	*/

/* Function diable_irq
 * Fuction disable a specifik IRQ
 * In: inrq_no - The irq number that will be disabled
 */
void disable_irq(int irq_no);

/* Function enable_irq
 * Function enables a specifik IRQ
 * In: inrq_no - The irq number that will be enabled
 */
void enable_irq(int irq_no);

/* Funciton pic_init()
 * Function starts the PIC functions
 * It sets up the interrupt vector
 */
void pic_init();
#endif

/* File: idt.h
 * This file contains the defintions
 * for handleing interrupts
 * ie structs that descripes gates, traps etc
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 * # 1.001  2001-12-02 Markus Svensson Change the IDTR offset to 0x7400
 * # 0.002  2002-04-23 Markus Svensson Changing definitions so they can easy be found 
 */

#ifndef _IDTH
#define _IDTH
#define BITS32  	0x8	/* Use 32 bits		*/
#define INTERRUPT_GATE  0x6	/* A interrupt gate	*/
#define TRAP_GATE 	0x7	/* A trap gate		*/
#define PRESENT 	0x1	/* Present gate 	*/
#define OS_MODE		0x0	/* Request OS mode	*/
#define MODULE_MODE	0x1	/* Request module mode	*/
#define OS_USER		0x3	/* Request user mode	*/

// # 1.001
// # 0.002
//#define IDT_MEM_START	0x8000	
			        /* IDT starts i memory 	
				   This is the offset specified in IDTR */
// # 0.002				 
// # 1.001

/* Unhandled interrupts function */
extern void unhand(void);

/* Interrupts to exceptioons */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);


/* Structure of the IDT entry */

/* Every entry contains offset, selector, previllige type
 * and properties
 */
typedef struct {
	int     low_offset  : 16;       // Low offset
	int	selector    : 16;       // The segement selector
	int     none	    : 8;	// ZEROS
	int     type	    : 5;	// Type of descriptor
	int	DPL         : 2;        // Desciprotr Pevilige Level 0-3
	int	present	    : 1;	// Is the desciptor present
	int	high_offset : 16;	// High offset in memory 32bits
} IDTR;

/* Uses this struct to get the correct data to write to memory */
typedef union {
   int mem[2];
   IDTR idtr;
} union_idtr;

/* Funciton idt_init
 * Function sets up fundamental interrupts
 * All the execeptions specified 0x00-0x1f
 * All functions must be in the OS selector
 */
void idt_init(void);

/* Funciton setidt
 * Function sets up a IDT entry in memory
 * The parameters sent decides the type of the gate
 * All functions must be in the OS selector
 * In: handler		-	function to be called
 *     interrupt_no 	-	the number of the interrupt
 *     type		-	type of gate	
 *     present          -       present flag
 *     privillige	-	Requsted privillige
 */
void setidt(void *handler,int interrupt_no,int type, int present, int privilige);

#endif

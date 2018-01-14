// 80x86
// File: idt.h
// This file contains the defintions
// for handleing interrupts
// ie structs that descripes gates, traps etc
// Created: 2001-11-19 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------

#ifndef _IDTH
#define _IDTH
#define IDTOFFSET         0x0000 // IDT table offset
#define IDTLIMIT          0x7ff      // IDT limit
#define IDTENTRYSIZE      0x8	// The size of an IDT entry
#define IDTBITS32  	  0x8	// Use 32 bits		
#define IDTINTERRUPT_GATE 0x6	// A interrupt gate	
#define IDTTRAP_GATE 	  0x7	// A trap gate		
#define IDTPRESENT 	  0x1	// Present gate 
#define IDTOS_MODE	  0x0	// Request OS mode	
#define IDTMODULE_MODE	  0x1	// Request module mode	
#define IDTOS_USER	  0x3	// Request user mode	
#define IDT_TYPE_TASKGATE 0x5   // Type task gate

// Structure of the IDT entry 

//Every entry contains offset, selector, previllige type
// and properties
typedef struct {
	int     ilow_offset  : 16;      // Low offset
	int	iselector    : 16;      // The segement selector
	int     inone	     : 8;	// ZEROS
	int     itype	     : 5;	// Type of descriptor
	int	iDPL         : 2;       // Desciprotr Pevilige Level 0-3
	int	ipresent     : 1;	// Is the desciptor present
	int	ihigh_offset : 16;	// High offset in memory 32bits
} stIDTR;

// Uses this struct to get the correct data to write to memory 
typedef union {
   int imem[2];
   stIDTR idtr;
} union_idtr;

// Funciton idt_init
//   Function sets up fundamental interrupts
//   All the execeptions specified 0x00-0x1f
//   All functions must be in the OS selector
// Out:
//     error		-	0 if all ok, else not 0
int idt_init(void);

// Funciton setidt
// Function sets up a IDT entry in memory
// The parameters sent decides the type of the gate
// All functions must be in the OS selector
// In: handler		-	function to be called
//     interrupt_no 	-	the number of the interrupt
//     type		-	type of gate	
//     present          -       present flag
//     privillige	-	Requsted privillige
// Out:
//     error		-       0 if all is ok
//				not 0 if there is something wrong
int setidt(int selector, void *handler,int interrupt_no,int type, int present, int privilige);

// Function dummy_interrupt
//   Function that does nothing
//
extern void dummy_interrupt(void);
void interrupt_0(void);
void interrupt_1(void);
void interrupt_2(void);
void interrupt_3(void);

void interrupt_4(void);
void interrupt_5(void);
void interrupt_6(void);
void interrupt_7(void);

void interrupt_8(void);
void interrupt_9(void);
void interrupt_10(void);
void interrupt_11(void);

void interrupt_12(void);
void interrupt_13(void);
void interrupt_14(void);
void interrupt_15(void);

void interrupt_16(void);
void interrupt_17(void);
void interrupt_18(void);
void interrupt_19(void);
void interrupt_20(void);
#endif

/* File: kernel.h
 * This file contains the c-kernels definitions
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 */

#include "pic.h"
#include "idt.h"

#ifndef _KERNELH
#define _KERNELH


#define VERSION "1.00"
// extern void writehead(void);

/* Initialization of the kernels main functions
 * such as idt(interrupts descriptor table), keyboard,
 * pic(programmable interrupt handler)
 * and takes care of the system until restart
 */
void kernel_init(void);

typedef struct {
   int type :16;
   int offset;
} LDTR;
#endif

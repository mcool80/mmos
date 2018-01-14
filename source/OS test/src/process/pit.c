// File: pic.c
// This file contains the functions
// for handleing chip 8358 - the PIT
// Programmable Interrupt Timer
// This is intel-specific code
// Created: 2004-05-21 Markus Svensson
// Modified:

#include <pic.h>
#include <pit.h>
#include <idt.h>

extern void timerhandler();		// Asm code that is called on with an interrupt
extern void hardclock();

asm (
   ".globl _timerhandler   \n"
//   ".globl _ret_save       \n"
   "_timerhandler:         \n"
//   "   pusha               \n" // Save all registers
//   "   call  _save_registers\n"
//   "_ret_save:             \n"
//   "   popl %eax\n          "
//   "   popl %eax\n          "
   "   ljmp $0x38, $0x00    \n"
   "                       \n"
//   "   popa                \n"
   "   iret                \n" // Exit interrupt
);

void pit_init() 
{
  // Setup the PIT
  outportb(PITCONTROLWORD, 0x36);

  // Load it with correct counter
  outportb(PITCHANNEL0, 0x100);
  outportb(PITCHANNEL0, 0x100);

  // Set interrupt vector
//  setidt( timerhandler, /*0x21*/ PICMASTERVEC,IDTTRAP_GATE|IDTBITS32, IDTPRESENT, IDTOS_MODE);
  setidt( 0x38, 0x00, /*0x21*/ PICMASTERVEC,IDT_TYPE_TASKGATE, IDTPRESENT, IDTOS_MODE);

  // enable its irq       
  enable_irq(0);

}

// 80x86
// File idt.c
// Contains:
// Setup functions for Interrupt Descriptor tables
// This is intel-specific code
// Created: 2003-12-22
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------

#include <idt.h>

int idt_init(void)
{
  struct {
    unsigned int limit : 16;
    unsigned int looffset: 16;
    unsigned int hioffset: 16;
  } stIDT_pointer;
  stIDT_pointer.hioffset = IDTOFFSET >> 16;
  stIDT_pointer.looffset = IDTOFFSET &0xffff;
  stIDT_pointer.limit = IDTLIMIT; 

  // set the IDT pointer to given values
  asm("lidt %0\n"
  : "=m" (stIDT_pointer)
  );
  return 0;
}

int setidt(int selector, void *handler,int interrupt_no,int type, int present, int privilige)
{
  stIDTR IDTEntry;
  IDTEntry.ilow_offset = ((unsigned long)handler&0xffff);
  IDTEntry.ihigh_offset = ((unsigned long)handler) >> 16;

  // Warning constant
  IDTEntry.iselector   = selector;
  // Warning constant
  
  IDTEntry.inone = 0x00;
  IDTEntry.itype = type;
  IDTEntry.iDPL  = privilige;
  IDTEntry.ipresent = present;

  // copy the information to the correct position in memory
  memcpy(IDTOFFSET+IDTENTRYSIZE*interrupt_no, &IDTEntry, IDTENTRYSIZE);
  return 0;
}

void interrupt_0(void)
{
  asm("mov $0,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_1(void)
{
  asm("mov $1,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_2(void)
{
  asm("mov $2,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_3(void)
{
  asm("mov $3,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_4(void)
{
  asm("mov $4,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_5(void)
{
  asm("mov $5,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_6(void)
{
  asm("mov $6,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_7(void)
{
  asm("mov $7,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_8(void)
{
  asm("mov $8,%eax\n");
  asm("hlt\n");
  asm("iret\n");
  return;
}

void interrupt_9(void)
{
  asm("mov $9,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_10(void)
{
  asm("mov $10,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_11(void)
{
  asm("mov $11,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_12(void)
{
  asm("mov $12,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_13(void)
{
  asm("mov $13,%eax\n");
  asm("hlt\n");
  asm("iret\n");
  return;
}

void interrupt_14(void)
{
  asm("mov $14,%eax\n");
  asm("hlt\n");
  asm("iret\n");
  return;
}

void interrupt_15(void)
{
  asm("mov $15,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_16(void)
{
  asm("mov $16,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_17(void)
{
  asm("mov $17,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_18(void)
{
  asm("mov $18,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_19(void)
{
  asm("mov $19,%eax\n");
  asm("iret\n");
  return;
}

void interrupt_20(void)
{
  asm("mov $20,%eax\n");
  asm("iret\n");
  return;
}


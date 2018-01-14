// File page.c
// Contains:
// Timer routins
// Created: 2004-05-21
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------
#include <pic.h>
#include <process.h>
#include <timer.h>


/*void hardclock() 
{
   putstring("Timer\n");
   cnt++;
   exit_irq();
   if ( cnt == 4 )
   { 
       cnt = 0;
       asm("pushl $0x38\n"
           "pushl $0x0\n"); 
   } 
   
} */

void dispatch()
{
//asm("jmp _mi_switch\n");
while(1)
{   
//   cnt++;
//   putstring("0");
     putstring("Dispatch\n");
//     mi_switch();
     asm("jmp _mi_switch\n");
} 
//   putstring("End timer\n");   
//   exit_irq();
}

// File kernel.c
// Contains:
// Setup for the kernel
// Created: 2003-12-22
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------

#include <kernel.h>
#include <idt.h>
#include <pic.h>
#include <stdio.h>
#include <keyboard.h>
#include <salloc.h>
#include <buddy.h>
#include <pc.h>

void start_test(void)
{
   char str[10];
   // Testa salloc
   int sizetag;
   void *frame_block = 0x30000;
   setup_salloc_block(frame_block);
   void *p[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int size = 43, i;
   int n = 13;
   int m = 17;

//   while(1) 
      for ( i=0;i<18;i++) {
         size = 128; //(size*n+m) % 324;
         n=n*13;
     	 m++;
         itoa(size,str,10);
         putstring("\nsize: ");
         putstring(str); 
         if ( p[i] == 0 )
           p[i] = salloc(frame_block, size);
         itoa((int)p[i],str,16); 
         putstring("\nAllocation pointer: ");
         putstring(str); 
/*         if ( p[m%15] ) {
           itoa((int)p[m%15],str,16); 
           putstring("\nFreeing pointer: ");
           putstring(str); 
	   sfree(p[m%15]);
           p[m%15]=0;
         }  */
      }
}


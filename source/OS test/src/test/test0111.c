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
   addblock(frame_block,0x34000);
   addblock(frame_block,0x38000);
   void *p;
//   void *p[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int size = 43, i;
//   int n = 13;
//   int m = 17;
//   setup_buddy_system();

//   while(1) 
      for ( i=0;i<45;i++) {
         size = 128; //1 << 20; //(size*n+m) % 324;
/*         itoa(size,str,10);
         putstring("\nsize: ");
         putstring(str);  */
//         if ( p == 0 )
           p = salloc(frame_block, size);
         itoa((int)p,str,16); 
         putstring("  ");
         putstring(str); 
/*         itoa((int)sizetag,str,16); 
         putstring("\nSizetag: ");
         putstring(str); 
         free_block(p[i],sizetag,BD_TYPE_REGULAR);
*/
/*         if ( p[m%15] ) {
           itoa((int)p[m%15],str,16); 
           putstring("\nFreeing pointer: ");
           putstring(str); 
	   sfree(p[m%15]);
           p[m%15]=0;
         }  */
      }
}


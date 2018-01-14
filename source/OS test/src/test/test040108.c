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
   // Testa salloc
   int sizetag;
   void frame_block = 0x30000;
   setup_alloc_block(frame_block);
   void *p[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int size = 0, i;
   int n = 13;
   int m = 17;

//   while(c=1) 
      for ( i=0;i<15;i++) {
         size = 128; //(size+128) % 128;
         n--;
     	 m++;
         if ( p[i] != 0 )
           p[i] = salloc(frame_block, size);
//         if ( p[m%20] ) 
//	   sfree(p[m%20]);
      }
}


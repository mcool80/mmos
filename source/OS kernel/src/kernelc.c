/* File: kernel.c
 * This file contains the c-kernel it is
 * called from the asm-kernel on the startup
 * Created: 2001-11-19 Markus Svensson
 * Modified:
 * # 1.001  2001-12-02 Markus Svensson Remove setidt rows put in init_idt();
 */
#include <kernel.h>
#include <pic.h>
#include <keyboard.h>
//#include <SQL/SQLcmd.h>
//#include "logo.h"

void kernel_init(void) {
   char c[2]="\0\0";
   char c2;
   int i;
   puts("\e07Systemet är igång");

   while(1) {
      asm("int   $0x30\n"
	    : "=a" (c2)
            : "a" (1)); 
      c[0]=c2;
      puts(c);  
//      for( i=0; i<32000;i++);
   }
}

void loadSQLcmd() {
//   mainSQL();	
}

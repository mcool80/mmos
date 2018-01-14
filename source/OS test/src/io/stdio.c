// File stdio.c
// Contains:
// Memory functions
// Communication with vga
// Created: 2003-12-27
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------

#include <stdio.h>
int cursorpos= 0x0;

int putchar(int c) {
   char str_with_c[2];
   str_with_c[0]=c;
   str_with_c[1]='\0';
   putstring(str_with_c);
   return 0; // All ok
}

int putstring(char *str) {
   char *video=(char*)(VGABASEADDR+cursorpos*2);  
   while(*str != 0 ) // Until delimitatior of string
   {
      if ( *str=='\b' ) 
      { // Backspace
         // Only change the cursorposition if the cursor is not in the beginning of a line
         if ( cursorpos % SCRWIDTH != 0 ) {
           cursorpos--;
           video-=2;
         }
      } 
      else if ( *str=='\n' || *str == 13)
      {  // Newline
         cursorpos=((cursorpos/SCRWIDTH)+1)*SCRWIDTH;
         video = (char *)VGABASEADDR+cursorpos*2;
      } else {
         // Put the charachter on screen
         *video = *str;
         *(video+1) = DEFAULTATTR;
         video+=2;
         cursorpos++;
      }
      if ( cursorpos == SCRWIDTH*SCRHEIGHT ) {
         memcpy((char *)VGABASEADDR,(char *)(VGABASEADDR+SCRWIDTH*2),SCRWIDTH*(SCRHEIGHT-1)*2);
         cursorpos -= SCRWIDTH;
         video = (char *)VGABASEADDR+cursorpos*2;
      }
      str++;
   }
   // Update the cursorposition on screen
   update_cursor();
   return 0; // All ok
}

int clearscreen()
{
   int size = SCRWIDTH*(SCRHEIGHT+1);
   char *video=(char*)VGABASEADDR;
   for(;size>=0;size--) {
     *(video+size*2) = 0;
     *(video+size*2+1) = DEFAULTATTR;
   }
   return 0; // All ok
}

int update_cursor() {
   // cursor LOW port to vga INDEX register
   outportb(0x3D4, 0x0F);
   outportb(0x3D5, (int)(cursorpos&0xFF));
   // cursor HIGH port to vga INDEX register
   outportb(0x3D4, 0x0E);
   outportb(0x3D5, (int)((cursorpos>>8)&0xFF));
   return 0; // All ok
}

int getch() {
   int ret;
   // Call the function to get a character
   // Should be a syscall in the future
   asm("int   $0x30\n"
       : "=a" (ret)
       : "a" (1)); 
   return ret;
}

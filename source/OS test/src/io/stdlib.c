// File stdlib.c
// Contains:
// Memory function
// Created: 2003-12-23
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------

#include <stdlib.h>

#define do_div(n,base) ({ \
 int __res; \
 __res = ((unsigned long) n) % (unsigned) base; \
 n = ((unsigned long) n) / (unsigned) base; \
 __res; })


void *memcpy(void *d, void *s, int n)
{
  // copy byte by byte slow but easy
  asm (	"movl	%0, %%esi \n"
	"movl	%1, %%edi \n"
	"movl	%2, %%ecx \n"
	"rep 	movsw \n"
        : "=g" (s), "=g" (d), "=g" (n));
}

char* itoa( int value, char *string, short base)
{
	return ltoa((long)value,string,base);
}

char* ltoa( long value, char *string, short base)
{
   const char* digits="0123456789abcdefghijklmnopqrstuvwxyz";
   int i=0;

   if (base<2 || base>36)
     return 0;

   if (value==0)
     string[i++]='0';
   else 
     while (value!=0) {
        string[i++]=digits[do_div(value,base)];
        //value = value/base;
     }
   int t=i;
   for(t=0;t<i/2;t++) {
      char tmp;
      tmp = string[t];
      string[t] = string[i-t-1];
      string[i-t-1]=tmp;
   } 
   string[i] = '\0';
   return string;
}

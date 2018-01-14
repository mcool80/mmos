#include <pages.h>
#include <mos.h>

int write_to_mem(unsigned int addr, unsigned int data) {
    
   asm ("movl %%edx, %%ds:(%%edi) \n"
        : 
        : "d" (data), "D" (addr));
   return 1;
}

int write_to_mem_addr(unsigned int addr, unsigned int addr_data) {
   asm ("movsl\n"
        :
        : "c" (1), "S" (addr_data), "D" (addr));
   return 1;
}

int pages_init()
{
   PAGE_DIR pde;
   int *ppde;
   PAGE_TABLE pdt;
   int *ppdt;
   int i=0;
   unsigned int paddr, pdata;

   pde.present = 1;
   pde.RW = 1;
   pde.US = 0;
   pde.PWT = 0;
   pde.PCD = 0;
   pde.A = 0;
   pde.nolla = 0;
   pde.PS = 0;
   pde.G = 1;
   pde.AVAIL = 0;
   pde.PAGE_TABLE_BASE = 0x0009;

   paddr = 0x0000;
   write_to_mem_addr(paddr, (unsigned int)&pde);

   pdt.present = 1;
   pdt.RW = 1;
   pdt.US = 0;
   pdt.PWT = 0;
   pdt.PCD = 0;
   pdt.A = 0;
   pdt.D = 1;
   pdt.nolla = 0;
   pdt.G = 1;
   pdt.AVAIL = 0;

   for ( i=0;i<256;i++) {
      pdt.PAGE_BASE = 0x0000+i;
      paddr = 0x9000+i*4;
      
      write_to_mem_addr(paddr, (unsigned int)&pdt);
   }
}   


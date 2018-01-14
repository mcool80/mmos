#include <intel/tss.h>
#include <stdlib.h>

int setup_kernel_process()
{
    st_TSS32 *tss = kmem_alloc(sizeof(st_TSS32));
    st_TSS_desc *tss_desc = GDT_ADDR+8*7;
    int selector = 0x38;
    tss_desc->ilowlimit    = 0x67;
    tss_desc->ilowbase1    = ((int)tss) & 0x0000ffff;
    tss_desc->ilowbase2    = (((int)tss) >> 16) & 0xff;
    tss_desc->itype        = TSS_TYPE;
    tss_desc->izero        = 0;
    tss_desc->idpl         = 0;
    tss_desc->ipresent     = 1;
    tss_desc->ihighlimit   = 0;
    tss_desc->iavl         = 1;
    tss_desc->izero2       = 0;
    tss_desc->igran        = 1;
    tss_desc->ihighbase    = ((int)tss) >> 24; 
//    memcpy(GDT_ADDR+sizeof(st_TSS_desc)*7, &tss_desc, sizeof(st_TSS_desc));
/*    asm("ltr %0\n"
    :
    : "m" (selector)); */
    asm volatile("ltr %0": :"g" (*(unsigned short *)&selector));

}

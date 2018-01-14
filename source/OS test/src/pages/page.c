// 80x86
// File page.c
// Contains:
// Intel specific code to set up paging
// Created: 2004-04-21
// Created by: Markus Svensson
// Modified:
//
// ---------------------------------------------------------------------

#include <page.h>
#include <intel/page.h>

int allocate_frames(int cnt_frames, int *start_frame, int *cnt_frames_allocated, int dma_page)
{
    // If dma_page is requested use som page between 0x0080 0000-0x00A0 0000 on x86
    int i;
    *cnt_frames_allocated = 0;
    for (i=0; i < CNT_UNALLOCATED_PAGE_AREA; i++)
    {
        if ( unallocated_page_area[i].start_frame != 0 )
        {
//           printf("   allocate_frames: found a free slot at %d (%d - %d)\n", i, unallocated_page_area[i].start_frame, unallocated_page_area[i].end_frame);        
           *cnt_frames_allocated = unallocated_page_area[i].end_frame - unallocated_page_area[i].start_frame;
           *start_frame = unallocated_page_area[i].start_frame;
        }
        if ( *cnt_frames_allocated > cnt_frames )
        {
            unallocated_page_area[i].start_frame = unallocated_page_area[i].start_frame + cnt_frames;
            *cnt_frames_allocated = cnt_frames;
            zero_page((void *)((*start_frame)*USER_PAGESIZE), *cnt_frames_allocated);
            return 0;
        } 
        else if ( unallocated_page_area[i].start_frame != 0 )
        {
            unallocated_page_area[i].start_frame = 0;
            zero_page((void *)((*start_frame)*USER_PAGESIZE), *cnt_frames_allocated);
            return 0;
        }
    }
    return 1;
}

int compress_free_frames()
{
    return 0;
}

int deallocate_frames(page_area *pa)
{
    // Check that the frames really is in memory (less than memory physical size)
    int i;
    for (i=0; i < CNT_UNALLOCATED_PAGE_AREA; i++)
    {
        if ( unallocated_page_area[i].start_frame == 0 )
        {
//            printf("   deallocate_frames: found a free slot at %d\n", i);
            unallocated_page_area[i].start_frame = pa->start_frame;
            unallocated_page_area[i].end_frame = pa->end_frame;
            return 0;
        }
    }
    return 0;
}



int zero_page(void *page_offset, int cnt)
{
#ifndef TEST
    asm("pushl %edi         \n");
    asm("pushl %ecx         \n");
    asm("movl  %0, %%eax    \n"
        :
        : "g" (page_offset));
    asm("movl  %eax, %edi   \n");
    asm("movl  %0, %%ecx    \n"
        :
        : "g" (USER_PAGESIZE*cnt));
    asm("zero_loop:         \n");
    asm("movb  $0, %ds:(%edi)\n");
    asm("inc   %edi         \n");
    asm("loop  zero_loop    \n");
    asm("popl  %ecx         \n");
    asm("popl  %edi         \n");
#endif
    return 0;
}


/**
 * @param mem_to_map Number of Mbytes to map to pages
 */
int kernel_page_init(int mem_to_map)
{
   int i;
    page_4mpage_entry *pte;

    for ( i=2;i<mem_to_map/4;i++) 
    {
       pte = (page_4mpage_entry *)(DIRADDR+i*sizeof(page_table_entry));
       unsigned int memaddr = STARTADDR+KERNEL_PAGESIZE*i;
       pte->present = 1;
       pte->rw = 1;
       pte->user = 0;
       pte->write_through = 1;
       pte->cache_disable = 1;
       pte->accessed = 0;
       pte->global_page = 1;
       pte->page_base_addr = memaddr >> 22;
       // 4 meg size
       pte->page_size = 1;
    } 
    return 0;
}

void init_frame_handle(int memsize)
{
   // Remove 0x00A0 0000 - 0x00c0 0000 from the unallocated list, the Video memory on x86 lies here.
   int no_frames = memsize/4;
   unallocated_page_area[0].start_frame = 0x00800;
   unallocated_page_area[0].end_frame = no_frames;
}

#include <page.h>
#include <intel/page.h>
/**
 * Creates a page dir table in memory
 * @param addr Address where the page dir is allocated
 * @returns Returns 0 if all is ok
 */
int get_table_dir(void **addr)
{
    int c, frame;
    // Allocate a page in memory
    int a = allocate_frames(1, &frame, &c, 0);
    *addr = (void *)(frame * 0x1000);
    if ( a !=0 ) 
        return 1;
    return 0;
}
/**
 * Allocates pages to a process in a given range
 * If the adresses given not correspond to frame adresses the best fit is choosen.
 * @param table_dir_addr Adress where the page dir is located
 * @param start_vir_adress Virtual adress in memory where the allocation starts
 * @param end_vir_adress Virtual adress in memory where the allocation ends
 * @return Returns 0 if all is ok
 */
int set_mmap(void *table_dir_addr, long int start_vir_adress, long int end_vir_adresss)
{
    int frames_to_allocate = (end_vir_adresss-start_vir_adress) >> USER_PAGE_OFFSET_SIZE;
    int frames_allocated = 0;
    int start_frame, cnt_frames_allocated;
    /* Allocated frames until the adress interval is covered. */
    while ( frames_to_allocate - frames_allocated > 0 )
    {
        if ( allocate_frames(frames_to_allocate-frames_allocated, &start_frame, &cnt_frames_allocated, 0) != 0 )
            return 1;
        frames_allocated = frames_allocated - cnt_frames_allocated;
        /* Write the information to the page tables in memory */
    }
    return 0;
}

/**
 * Allocates pages to a process in a given range
 * @param table_dir_addr Adress where the page dir is located
 * @param start_vir_adress Virtual adress in memory where the allocation starts
 * @param end_vir_adress Virtual adress in memory where the allocation ends
 * @return Returns 0 if all is ok
 */
int reset_mmap(void *table_dir_addr, long int start_vir_adress, long int end_vir_adress)
{
    page_area pa;
    pa.start_frame = start_vir_adress >> USER_PAGE_OFFSET_SIZE;
    pa.end_frame = end_vir_adress >> USER_PAGE_OFFSET_SIZE;
    /* Remove pages in page tables */
    int c_frame;
    for ( c_frame = pa.start_frame; c_frame <= pa.end_frame; c_frame ++ )
        if ( remove_user_page(table_dir_addr, (void *)(c_frame << USER_PAGE_OFFSET_SIZE)) != 0 ) 
            return 1;
    /* Make these frames unallocated */
    
    return 0;
}

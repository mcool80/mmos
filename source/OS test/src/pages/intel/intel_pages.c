#include <page.h>

/**
 * Get pde entry from a table addr
 * @param page_dir Page dir
 * @param pageaddr Page adress
 */
int get_pde(void *page_dir, void *pageaddr, void **pde)
{
    int ipageaddr = *(int *)&pageaddr;
    *pde = page_dir + (((ipageaddr & USER_PAGE_TABLE_MASK) >> USER_PAGE_OFFSET_SIZE)*sizeof(page_table_entry));
}

/**
 * Write information about a page table in the page dir in memory
 *
 * @param page_dir Physical adress to page dir
 * @param pageaddr Physical adress to the page in memory connected to the table
 */
int set_user_table(void *page_dir, void *pageaddr, void **tableaddr)
{
    int table_frame, cnt_frames;
    page_dir_entry *pde;
    if ( get_pde(page_dir, pageaddr, (void **)&pde) != 0 )
        return 1;
    if ( allocate_frames(1, &table_frame, &cnt_frames, 0) != 0 )
        return 2;
    *tableaddr = (void *)(table_frame << USER_PAGE_OFFSET_SIZE);
    pde->present = 1; 
    pde->rw = 1;
    pde->user = 1;
    pde->write_through = 0;
    pde->cache_disable = 0;
    pde->accessed = 0;
    pde->page_size = 0;
    pde->global_page = 1;
    pde->avail = 0;
    pde->page_table_base_addr = table_frame;
    return 0;
}

/**
 * Get information about a page table in the page dir in memory
 * If the table is not found then a new page table is created
 *
 * @param page_dir Physical adress to page dir
 * @param pageaddr Physical adress to the page in memory connected to the table
 * @param tableaddr Physical adress to the tableaddress in memory
 */
int get_user_table(void *page_dir, void *pageaddr, void **tableaddr)
{
    page_dir_entry *pde;
    if ( get_pde(page_dir, pageaddr, (void **)&pde) != 0 )
        return 1;
    if ( pde->page_table_base_addr == 0 )
        return set_user_table(page_dir, pageaddr, tableaddr);
    *tableaddr = (void *)pde->page_table_base_addr;
    return 0;
}

/**
 * Write information about a page in the page table in memory
 *
 * @param page_dir Physical adress to page dir
 * @param pageaddr Physical adress to the page in memory
 */
int set_user_page(void *page_dir, void *pageaddr)
{
    void *tableaddr;
    if ( get_user_table(page_dir, pageaddr, &tableaddr) != 0 )
        return 1;
    page_dir_entry *pde;
    if ( get_pde(page_dir, pageaddr, (void **)&pde) != 0 )
        return 2;
    int ipageaddr = *(int *)&pageaddr;
    page_table_entry *pte = (void *)(pde->page_table_base_addr << USER_PAGE_OFFSET_SIZE);
    pte->present = 1; 
    pte->rw = 1;
    pte->user = 1;
    pte->write_through = 0;
    pte->cache_disable = 0;
    pte->accessed = 0;
    pte->dirty = 0;
    pte->global_page = 0;
    pte->avail = 0;
    pte->page_base_addr = ipageaddr >> USER_PAGE_OFFSET_SIZE;
    return 0;
}

/**
 * Remove information about a page in the page table in memory
 *
 * @param page_dir Physical adress to page dir
 * @param pageaddr Physical adress to the page in memory
 */
int remove_user_page(void *page_dir, void *pageaddr)
{
    void *tableaddr;
    if ( get_user_table(page_dir, pageaddr, &tableaddr) != 0 )
        return 1;
    page_dir_entry *pde;
    if ( get_pde(page_dir, pageaddr, (void **)&pde) != 0 )
        return 2;
    int ipageaddr = *(int *)&pageaddr;
    page_table_entry *pte = (void *)(pde->page_table_base_addr << USER_PAGE_OFFSET_SIZE);
    pte->present = 0; 
    pte->rw = 0;
    pte->user = 0;
    pte->write_through = 0;
    pte->cache_disable = 0;
    pte->accessed = 0;
    pte->dirty = 0;
    pte->global_page = 0;
    pte->avail = 0;
    pte->page_base_addr = 0;
    return 0;
}

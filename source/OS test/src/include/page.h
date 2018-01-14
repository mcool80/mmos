#ifndef _PAGEH
#define _PAGEH
#define CNT_UNALLOCATED_PAGE_AREA 64

typedef struct 
{
    int start_frame;
    int end_frame;
} page_area;

typedef struct 
{
    page_area *pa;
    void *next;
} page_list;

page_area unallocated_page_area[CNT_UNALLOCATED_PAGE_AREA];

int zero_page(void *page_offset, int cnt);

int kernel_page_init(int mem_to_map);

/*
 * @param memsize Memory size in kbytes
 */
void init_frame_handle(int memsize);


int allocate_frames(int cnt_frames, int *start_frame, int *cnt_frames_allocated, int dma_page);
#endif

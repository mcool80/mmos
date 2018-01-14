#include <page.h>

int main()
{
    // Init frames 32mb
    init_frame_handle(8392);
    // Loop
    int i, start_frame, no_frames, ret;
/*    for ( i = 0; i < 5000; i++)
    {
        // Allocate frames
        ret = allocate_frames(5, &start_frame, &no_frames, 0);
        printf("ret: %d, start_frame %d, no_frames %d\n", ret, start_frame, no_frames);
 
        page_area pa;
        pa.start_frame = 2048+(random()%8)*5;
        pa.end_frame = pa.start_frame + 5;
        deallocate_frames(&pa);
        printf("dealloc: start_frame %d, end_frame %d\n", pa.start_frame, pa.end_frame);
     } */
     void *addr;
     int a = get_table_dir(&addr);
     printf("Return: %d,  Addr: %x\n", a, addr);
    // create a process
    create_process();
    // Allocate frames
}

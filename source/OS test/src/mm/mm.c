#include <stdlib.h>
#include <stdio.h>

int init_kmem(int startmem, int endmem) 
{
    mem_chunk *startchunk = (void *)startmem;
    kernelmem.startmem = startmem;
    kernelmem.endmem = endmem;
    kernelmem.free = endmem-startmem;
    startchunk->next = (void *)0;
    startchunk->status = MEM_LAST;
}

void *kmem_alloc(int size) 
{
    mem_chunk *memchunk;
    // Is there room in the memory area
    if ( kernelmem.free < size )
        return 0;
    // Go through all memareas until the last or one free found
    memchunk = (void *)kernelmem.startmem;
    int o=0;
    while(memchunk->status != MEM_LAST ) {
        char str[10];
//        putstring("In kmem_alloc:\nFound a chunk at: ");
        itoa(memchunk,str,16);
//        putstring(str);
//        putstring("\n");  
        if ( memchunk->status == MEM_FREE && memchunk->size >= size ) {
            // This is a adequat area, use it
            memchunk->status = MEM_ALLOCATED;
            break;
        } 
        else 
        {
          int size = memchunk->size;
//          putstring("Size of chunk: ");
//          itoa(size,str,10);
//          putstring(str);
//          putstring("\n"); 
//          putstring("Size of memchunk: ");
//          itoa(sizeof(mem_chunk),str,10);
//          putstring(str);
//          putstring("\n");  
          memchunk = (void *)((int)memchunk+(sizeof(mem_chunk)+size));
        }
    }
    int oldstatus = memchunk->status;
    char str[10];
/*    putstring("In kmem_alloc:\nFound a free chunk at: ");
    itoa(memchunk,str,16);
    putstring(str);
    putstring("\n"); */
    if ( ((int)memchunk+size+sizeof(mem_chunk)) < kernelmem.endmem ) 
    {
        memchunk->status = MEM_ALLOCATED;
        memchunk->size = size;
        memchunk->next = (void *)((int)memchunk+sizeof(mem_chunk)+size);
        kernelmem.free = kernelmem.free - size - sizeof(mem_chunk);
        if ( oldstatus == MEM_LAST ) {
          mem_chunk *nextmemchunk = memchunk->next;
          nextmemchunk->status = MEM_LAST;
/*          putstring("In kmem_alloc:\nNext free chunk at: ");
          itoa(nextmemchunk,str,16);
          putstring(str);
          putstring("\n"); */
        }
    } 
    else 
        return 0;
    return (void *)((int)memchunk+sizeof(mem_chunk));
}

void kmem_free(void *pointer) 
{
    mem_chunk *memchunk;
    memchunk = pointer-sizeof(mem_chunk);
    memchunk->status = MEM_FREE;
    kernelmem.free += memchunk->size;
}


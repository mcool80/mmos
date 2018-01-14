// File: stdlib.h
// This file contains the defintions
// memory functions
// Created: 2002-12-23 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------

#ifndef _MMH
#define _MMH
#define MEM_FREE 0
#define MEM_ALLOCATED 1
#define MEM_LAST 2

struct {
    int startmem;
    int endmem;
    int free;
} kernelmem;

typedef struct {
    int status;
    int size;
    void *next;
} mem_chunk;

int init_kmem(int startmem, int endmem);
void *kmem_alloc(int size);
void kmem_free(void *pointer);
    
void *memcpy(void *d, void *s, int n);
char* itoa( int value, char *string, short base);
char* ltoa( long value, char *string, short base);
#endif

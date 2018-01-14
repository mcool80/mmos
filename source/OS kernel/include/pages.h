#ifndef _PAGESH
#define _PAGESH

#define PAGE_PRESENT 	0x001		// Is the page present
#define PAGE_READ	0x000		// Is the page readable
#define PAGE_WRITE	0x002		// Is the page writeable
#define PAGE_USER	0x004		// Is it a user page 
#define PAGE_SUPERUSER	0x000		// Is it a kernel page
//#define PAGE_PWT	0x008		// 		
//#define PAGE_PCD	0x010		//
#define PAGE_ACCESSED	0x020		// Is the page accessed 
#define PAGE_4KPAGES	0x000		// Use 4Kb pages
#define PAGE_4MPAGES	0x080		// Use 4Mb pages
//#define PAGE_G	0x100		//
#define PAGE_DIRTY	0x040		// A dirty page

typedef struct {
   int present	: 1;			// Is the page present
   int RW 	: 1;			// Is the page readable=, readable/writeable=1
   int US 	: 1;			// Is it a user page=1, superuser=0
   int PWT 	: 1;			// Page Write Table
   int PCD 	: 1;			// Page Cache 
   int A 	: 1;			// Accessed
   int nolla	: 1;			
   int PS 	: 1;			// Page size, 4Mb=1, 4Kb=0
   int G 	: 1;			// Gradiute
   int AVAIL 	: 3;

   unsigned int PAGE_TABLE_BASE : 20;	// Physical adress for page table (not the 20 LSB)
} PAGE_DIR;

typedef struct {
   int present	: 1;			// Is the page present
   int RW 	: 1;			// Is the page readable=, readable/writeable=1
   int US 	: 1;			// Is it a user page=1, superuser=0
   int PWT 	: 1;			// Page Write Table
   int PCD 	: 1;			// Page Cache 
   int A 	: 1;			// Accessed
   int D 	: 1;			// Dirty page=1, not dirty=0
   int nolla	: 1;
   int G 	: 1;

   int AVAIL 	: 3;
   int PAGE_BASE : 20;			// Frame number or physical address ( not the 20 LSB) )
} PAGE_TABLE;

int pages_init();
/*int create_dir();
int create_table(); */

#endif

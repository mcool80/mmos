//80x86
#ifndef _INTELPAGEH
#define _INTELPAGEH
#define KERNEL_PAGESIZE 0x400000
#define USER_PAGESIZE 0x1000
#define DIRADDR 0x1000
#define TABLEADDR 0x1000
#define STARTADDR 0x0000
#define KERNELADDR 0xFF000000
/* Number of bits used in offset */
#define USER_PAGE_OFFSET_SIZE 12
/* 0000 0000 1111 1111  1111 0000 0000 0000 */
#define USER_PAGE_TABLE_MASK 0x00FFF000

// Intel specific entries
typedef struct {
    int present : 1; 
    int rw : 1;
    int user : 1;
    int write_through : 1;
    int cache_disable : 1;
    int accessed : 1;
    int dirty : 1;
    int reserved :1;
    int global_page : 1;
    int avail : 3;
    int page_base_addr : 20;
} page_table_entry;

typedef struct {
    int present : 1; 
    int rw : 1;
    int user : 1;
    int write_through : 1;
    int cache_disable : 1;
    int accessed : 1;
    int reserved : 1;
    int page_size :1;
    int global_page : 1;
    int avail : 3;
    int page_table_base_addr : 20;   
} page_dir_entry;

typedef struct {
    int present : 1; 
    int rw : 1;
    int user : 1;
    int write_through : 1;
    int cache_disable : 1;
    int accessed : 1;
    int dirty : 1;
    int page_size :1;
    int global_page : 1;
    int avail : 3;
    int reserved : 10;
    int page_base_addr : 10;
} page_4mpage_entry;

#endif

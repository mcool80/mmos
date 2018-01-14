// Definitions for intel Task Switching
#ifndef _TSSH
#define _TSSH
#define TSS_TYPE 0x9
#define GDT_ADDR 0x4D00

typedef struct 
{
    void *prev_link;    // 32 bit (16 bits used)
    unsigned int  ESP0;
    unsigned int  SS0;
    unsigned int  ESP1;
    unsigned int  SS1;
    unsigned int  ESP2;
    unsigned int  SS2;
    unsigned int  CR3;
    unsigned int  EIP;
    unsigned int  EFLAGS;
    unsigned int  EAX;
    unsigned int  ECX;
    unsigned int  EDX;
    unsigned int  EBX;
    unsigned int  ESP;
    unsigned int  EBP;
    unsigned int  ESI;
    unsigned int  EDI;
    unsigned int  ES;
    unsigned int  CS;
    unsigned int  SS;
    unsigned int  DS;
    unsigned int  FS;
    unsigned int  GS;
    unsigned int  LDT;
    unsigned int  T        : 1;
    unsigned int  reserved : 15;
    unsigned int  IO_MAP   : 16;
} st_TSS32;

typedef struct
{
    int ilowlimit : 16;
    int ilowbase1 : 16;
    int ilowbase2 : 8;
    int itype     : 4;
    int izero     : 1;
    int idpl      : 2;
    int ipresent  : 1;
    int ihighlimit: 4;
    int iavl      : 1;
    int izero2    : 2;
    int igran     : 1;
    int ihighbase : 8;
} st_TSS_desc;


typedef struct
{
    int izero1    : 16;
    int itss_seg  : 16;
    int izero2    : 8;
    int itype     : 4;
    int izero3    : 1;
    int idpl      : 2;
    int ipresent  : 1;
    int izero4    : 16;
} st_TSS_gate_desc;

#endif

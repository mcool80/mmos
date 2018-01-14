// File: mm.h
// This file contains the defintions
// for memory apperence
// Created: 2002-12-23 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------

#ifndef _MMH
#define _MMH

#define MM_IDT_START		0x00000000	// Start of IDT in system
#define MM_KERNEL_PAGES		0x00001000	// Start of Kernel pages information
#define MM_KERNEL_OFFSET	0x00005000	// Start of kernel
#define MM_KERNEL_STACK		0x007fffff	// Start of kernel stack
#define MM_DMA_OFFSET		0x00800000	// Start of DMA allocation area
#define MM_DMA_LIMIT		0x10000000	// Limit for DMA to read from memory
#define MM_VIDEO_MEMORY		0x00A00000	// Start of mapped video memory
#define MM_END_VIDEO_MEMORY	0x00BFFFFF	// End of mapped video memory
#define MM_USER_OFFSET		0x00C00000	// Start for user/system memory
#define MM_DMA_AREA_SIZE	20		// Percent of memory mapped to DMA

struct {
    int  addr_start;
    int  addr_stop;
    int  object_offset;
    void *object;
    void *next;
} mmap;

struct {
    int partion;
    int dbid;
    int tableid;
    int offset;
    int size;
} object_struct;

#endif

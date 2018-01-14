/* File: mos.h
 * Definitions for the system
 * Created: 2002-04-23 Markus Svensson
 * Modified:
 */

#ifndef _MOSH
#define _MOSH

#define KERNEL_PAGE_DIR_BASE 0x0000    // In frames
#define KERNEL_PAGE_TABLE_BASE 0x0001  // In frames
#define KERNEL_STACK		0x4000
#define KERNEL_IDT 0x2000

#define VERSION "0.01"

#endif

/* File: mem.h
 * Memory functions
 * Created: 2001-11-20 Markus Svensson
 * Modified: 
 */
#ifndef _MEMH
#define _MEMH

#define KERNEL_SELECTOR 0x10		/* Global Selector that points att kernel code */
#define DATA_KERNEL_SELECTOR 0x18       /* Global Selecotr that points to data that kernel controlls */

/* Function writemem
 *
 */
extern void writemem(unsigned int selector, unsigned int offset, int value);

/* Function memcpy
 * Copys data from sorurce to desc
 * In: Source offset - offset for source
       Destination offset - offset for destination
       Count - Number of bytes to copy
 */
/* extern void memcpy(void * src, void * dest, unsigned int n);  */

extern void getdata(void *dataaddr, unsigned int selector);  
#endif

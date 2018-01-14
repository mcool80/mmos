/* File: keyboard.h
 * This file contains definitions for keyboard handeling
 * Created: 2001-11-13 Markus Svensson
 * Modified:
 */

#ifndef _KEYBOARDH
#define _KEYBOARDH
#define KEYB_PORT 	0x60		/* Keyboard port 		*/
#define BUFFER_SIZE 	0x9		/* Size fo the keybuffer 	*/
#define KEY_SHIFT	1		/* Shiftkey code		*/
#define	KEY_ALT		2		/* Altkey code			*/
#define KEY_CTRL	4		/* Controlkey code		*/

/* Interrupt to isr 0x30 - key handler (getch, kbhit) */
extern int isr30(void);
#define KEYISR		0x30

/* Function keyboard_init
 * Inits keyboard so IRQ will occur
 * Sets up interrupt 0x30 to be the "key" interrupt
 * Enables IRQ(1)
 */ 
void keyboard_init();

/* Function keyhandler
 * Reads the scan code from keyboard
 * and puts the ascii value in the keyboard buffer
 */ 
void keyhandler(void);
#endif


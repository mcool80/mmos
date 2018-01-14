// 80x86
// File: keyboard.c
// This file contains the fundamental keyboard functions
// This is intel-specific code
// Created: 2003-12-28 Markus Svensson
// Modified:
//

//#include <stdio.h>
#include <keyboard.h>
#include <pic.h>
#include <idt.h>
#include <stdlib.h>

// Checks if a controlkey (SHIFT,CTRL or ALT) has been pressed/released
void key_checkcontrolkeys(int k);

// The buffer that contains all pressed keys
unsigned int ibuffer[KEYBUFFER_SIZE];

// Pointer in the buffer if 0 indicates an empty buffer
int ibufferhead=0;
int ibuffertail=0;

// Indicates which control keys ( ALT, SHIFT, CTRL ) are pressed
unsigned char control_keys = 0;

// Table with diffrent combinations of keys pressed 
// Converts scancodes ( code from the keyboard ) to ascii 
// First column no SHIFT or ALT
// Second SHIFT PRESSED
int iscan2ascii_table[][2] = {
{'\0'	, '\0'		},
{'\x1B'	, '\x1B'	}, //ESC
{'1'	, '!'		},
{'2'	, '@'		},
{'3'	, '#'		},
{'4'	, '$'		},
{'5'	, '%'		},
{'6'	, '^'		},
{'7'	, '&'		},
{'8'	, '*'		},
{'9'	, '('		},
{'0'	, ')'		},
{'-'	, '_'		},
{'='	, '+'		},
{'\x8'	, '\x8' 	}, // BACKSPACE
{'\x9'	, '\x9' 	}, // TAB
{'q'	, 'Q'	 	},
{'w'	, 'W'	 	},
{'e'	, 'E'	 	},
{'r'	, 'R'	 	},
{'t'	, 'T'	 	},
{'y'	, 'Y'	 	},
{'u'	, 'U'	 	},
{'i'	, 'I'	 	},
{'o'	, 'O'	 	},
{'p'	, 'P'	 	},
{'['	, ']'	 	},
{']'	, '['	 	},
{'\xD'	, '\xD'		}, // ENTER
{'\0'	, '\0'		}, // Left CTRL
{'a'	, 'A'	 	},
{'s'	, 'S'	 	},	
{'d'	, 'D'	 	},
{'f'	, 'F'	 	},
{'g'	, 'G'	 	},
{'h'	, 'H'	 	},
{'j'	, 'J'	 	},
{'k'	, 'K'	 	},
{'l'	, 'L'	 	},
{';'	, ':'	 	},
{'\x27'	, '"'		}, // '
{'`'	, '~'		},
{'\0'	, '\0'		}, //LEFT SHIFT
{'\0'	, '\0'		},
{'z'	, 'Z'	 	},
{'x'	, 'X'	 	},
{'c'	, 'C'	 	},
{'v'	, 'V'	 	},
{'b'	, 'B'	 	},
{'n'	, 'N'	 	},
{'m'	, 'M'	 	},
{','	, ','	 	},
{'.'	, '.'	 	},
{'/'	, '?'		},
{'\0'	, '\0'		}, //RIGHT SHIFT
{'*'	, '*'		}, 
{'\0'	, '\0'		}, //LEFT ALT
{' '	, ' '		},
{'\0'	, '\0'		}, // CAPSLOCK
{'\0'	, '\0'		}, // F1
{'\0'	, '\0'		}, // F2
{'\0'	, '\0'		}, // F3
{'\0'	, '\0'		}, // F4
{'\0'	, '\0'		}, // F5
{'\0'	, '\0'		}, // F6
{'\0'	, '\0'		}, // F7
{'\0'	, '\0'		}, // F8
{'\0'	, '\0'		}, // F9
{'\0'	, '\0'		}  //F10
};
// 	ASCII -	Shift - Ctrl - 	Alt - 	Num - 	Caps - 	Shift Caps - 	Shift Num 

int i=0;

int isr30() {
   // Wait until the buffer is filled with something
   while(ibufferhead==ibuffertail);

   // Get the ascii
   int ascii = ibuffer[ibuffertail];
   // Change the tailpointer
   ibuffertail = (ibuffertail+1)%KEYBUFFER_SIZE;
   return ascii;
}

extern void keyboardhandler();		// Asm code that is called on with an interrupt

asm (
   ".globl _keyboardhandler \n"
   "_keyboardhandler:       \n"
   "   pusha               \n" // Save all registers
/*   "   pushw %ds           \n" // Set up the data segment
   "   pushw %es           \n"
   "   pushw %ss           \n" // Note that ss is always valid
   "   pushw %ss           \n"
   "   popw %ds            \n"
   "   popw %es            \n" */
   "                       \n"
   "   call _keyhandler    \n"
   "                       \n"
/*   "   popw %es            \n"
   "   popw %ds            \n" // Restore registers */
   "   popa                \n"
   "   iret                \n" // Exit interrupt
);

void keyhandler(void) {

   // Find out if any function/control keys are pressed or released 
   int key = inportb(KEYBASEPORT);
   int type = 0;
   key_checkcontrolkeys(key);

/*   char str[10];
   itoa(key&0xff,str,10);
   putstring(str);  */


   // If the buffer is not full
   if ( ibuffertail != (ibufferhead+1)%KEYBUFFER_SIZE 
        && !(key&0x80) ) {
      // Add the key
      if ( control_keys&KEYCONTROLSHIFT ) type = 1;
      ibuffer[ibufferhead] = iscan2ascii_table[key&0xff][type];
      ibufferhead=(ibufferhead+1)%KEYBUFFER_SIZE;
   }
   // End Of IRQ
   exit_irq();
}

void key_checkcontrolkeys(int k) {

  // Check if the key is released
  if ( ((k&0xff) & KEYRELEASED) ) {
    k = (k&0xff)-0x80; 
    switch(k) {
      case KEYLSHIFT: case KEYRSHIFT:
        // Shift pressed down
        control_keys &= (0xff-KEYCONTROLSHIFT);
        break;
    }
  } else {
    k = (k&0xff); 
    switch(k) {
      case KEYLSHIFT: case KEYRSHIFT:
        // Shift pressed down
        control_keys |= KEYCONTROLSHIFT;
        break;
    }
  }
}

void keyboard_setup() {

   // Set up the interrupt descriptor
   setidt( keyboardhandler, /*0x21*/ PICMASTERVEC+1,IDTTRAP_GATE|IDTBITS32, IDTPRESENT, IDTOS_MODE);

   // Enable Interrupt Requests on ch 1
   enable_irq(KEYIRQ);

   // Set up interrupt descriptor for int 0x30 -> getch, kbhit
   setidt( isr30, KEYISR,IDTTRAP_GATE|IDTBITS32, IDTPRESENT, IDTOS_MODE);
}





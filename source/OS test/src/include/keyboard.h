// 80x86
// File: keyboard.h
// This file contains the 
// defintion needed to use keyboard
// Created: 2003-12-28 Markus Svensson
// Modified:
//

#ifndef _KEYBOARDH
#define _KEYBOARDH

#define KEYBASEPORT	0x60	// Keyboard base port
#define KEYLCTRL	0x14	// Left Control key AT-keyboard
#define KEYLSHIFT	42	// Left shift key AT-keyboard
#define KEYRSHIFT	54	// Right shift key AT-keyboard
#define KEYCAPSLOCK	0x58	// Capslock key AT-keyboard
#define KEYALT		0x11	// Alt key AT-keyboard

#define KEYRELEASED	0x80	// Break code when a key is released
#define KEYEXTENDED	0xE0	// Break code if the key is extended

#define KEYBUFFER_SIZE	100	// Key Buffer

#define KEYISR		0x30	// Interrupt Service Request number
#define KEYIRQ		0x01	// Interrupt Request Line

#define KEYCONTROLSHIFT	0x01	// Controlkey shift on
#define KEYCONTROLCTRL	0x02	// Controlkey ctrl on
#define KEYCONTROLALT	0x04	// Controlkey alt on

// Set up for keyboard
void keyboard_setup();

#endif

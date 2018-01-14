// 80x86
// File: pic.h
// This file contains the defintions
// for handleing chip 8259 - the PIC
// Programmable Interrupt Controller
// Some code from GazOS
// Created: 2001-11-19 Markus Svensson
// Modified:
//

// The chip is 8253
#ifndef _PITH
#define _PITH
#define PITCHANNEL0 0x40
#define PITCHANNEL1 0x41
#define PITCHANNEL2 0x42
#define PITCONTROLWORD 0x43

void pit_init();
#endif

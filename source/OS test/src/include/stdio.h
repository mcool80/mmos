// File: stdio.h
// This file contains the defintions
// for standard input/output functions
// Created: 2002-12-27 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------

#ifndef _STDIOH
#define _STDIOH
#define SCRWIDTH	80	// Screen width standard
#define SCRHEIGHT	24	// Screen height standard
#define DEFAULTATTR	15	// White textcolor
#define VGABASEADDR	0xb8000	// Video memory address

// Function putchar
//   Puts a character on the screen - makes a call to putstring
// In: c - The character to put on screen
// Returns: 0 - if ok
int putchar(int c);

// Function putstring
// Print a intire string on the screen
// In: str - character string with \0 delimitor
// Returns: 0 - if ok
int putstring(char *str);

// Function update_cursor
// Moves the blinking cursor to the actual position on screen
// the position is remebered internally
// Returns: 0 - if ok
int update_cursor();

// Function update_cursor
//   Removes all characters on screen an puts cursor at top-left position
// Returns: 0 - if ok
int clearscreen();

// Function getch
//   Gets a character from keyboard
// Returns: character in asciicode
int getch();
#endif

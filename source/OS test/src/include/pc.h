// File: pc.h
// This file contains the defintions
// for pc-spceific instructions
// ie structs that descripes gates, traps etc
// Created: 2001-11-19 Markus Svensson
// Modified:
//
// ------------------------------------------------------------------

#ifndef _PCH
#define _PCH
int memorysize;	// memory size in kb
extern int inportb(int port);
extern int inportw(int port);
extern outportb(int port, int value);
extern outportw(int port, int value);
extern int getmemsize();
#endif

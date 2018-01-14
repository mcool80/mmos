#ifndef STDIO_H
#define STDIO_H

#include "types.h"
#include "stdarg.h"

#define VIDEO_ADDR	(ushort*)0xB8000
#define NR_ROWS 	(ushort) 25
#define NR_COLUMNS 	(ushort) 80

typedef struct {
	ushort x;
	ushort y;
} t_pos ;

extern int  printf(char *fmt, ...);
extern void inline putc  (char c);
extern void inline putca (char c, char attr);
extern void puts(const char *s);
extern void gotoxy(ushort x, ushort y);
extern void clrscr();
extern void beep(void);
extern void sound(int freq);
extern void settextcolor(byte color);
extern void setbgcolor(byte color);
       t_pos inline getpos(void);
       void  inline setpos(t_pos *pos);

extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int sprintf(char *buffer, const char *fmt, ...);

extern int gets(char* buf);
extern int getch(void);
extern int getsql(char* buf);

#endif



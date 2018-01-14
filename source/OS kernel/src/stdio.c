/* \eXX   	Sätter färgen till XX
 * \eN or \en 	Sätter färgen till vit (7)
 * \eR or \er	Sätter färgen till röd (12 or 4)
 * \eB or \eb	Sätter färgen till blå (9 or 1)
 * \eG or \eg	Sätter färgen till grön (10 or 2)
 */

//#include <kernel.h>
#include <stdarg.h>
//#include <gdt.h>
#include <string.h>	/* for memcpy */
#include <stdio.h>
#include <io.h>


byte attr=7;		/* attribute of the text */
bool start=false;

#define CRTC_COMMAND	(uint) 0x3D4
#define CRTC_DATA	(uint) 0x3D5
#define CRTC_CURLO	(uint) 0x0F
#define CRTC_CURHI	(uint) 0x0E

/* clear the screen */
void clrscr()
{
	int i;
    for (i = 0; i < NR_ROWS*NR_COLUMNS; i++)
        	((ushort*)VIDEO_ADDR)[i]=' ' +(attr << 8);
	gotoxy(1,1);
}

/* Set text color */
void settextcolor(byte color)
{	
	attr=(attr & 0xf0) | color;
}

/* Set back ground color */
void setbgcolor(byte color)
{
	attr=(attr & 0x0f) | (color<<4);
}

/* set cursor to position x, y */
void gotoxy(ushort x, ushort y)
{
	t_pos pos;
	pos.x=x;
	pos.y=y;
	setpos(&pos);
}

int sound_delay = 0;
/* play a sound (0 to stop it) */
void sound(int freq)
{
	if (freq>0) {	/* activate speaker */
                outportb(0x42, (unsigned char) freq);
                outportb(0x42, (unsigned char) (freq >> 8));
                outportb(0x61, inportb(0x61) | 3);
        }
        else
        {	/* stop speaker */
        	outportb(0x61, inportb(0x61) & !3);
        }
}

/* output a beep (automatically stopped) */
void beep(void)
{
        sound_delay = 10;
        sound(500);
}

/* write a string on the screen at current cursor position */
void puts(const char *s)
{
	int i;
	int c, color;
	char name[255];
	t_pos pos=getpos();

        while ((c=(unsigned char)(*s++)) !=0) {
                switch (c) {
                	/* special characters */
                	case '\e':	/* change text color */
                        	c = *s++;
                                switch (c) {
                                	case 'N':
                                        case 'n':
                                        	attr = 7;
                                                break;
                                        case 'r': attr = 4; break;
                                        case 'R': attr = 4 + 8; break;
                                        case 'b': attr = 1; break;
                                        case 'B': attr = 1 + 8; break;
                                        case 'g': attr = 2; break;
                                        case 'G': attr = 2 + 8; break;
                                        default:
	                                	color = (c - '0')*10;
        	                                c = *s++;
                	                        color += c - '0';
                        	                if (color>0 && color < 16) attr = color;
                                                break;
                                }
                                break;
                	case 7: /* beep */
                                sound_delay = 10;
                                sound(1000);
                                break;
                	case 10:
                        case 11:
                        case 12: /* next line */
                            gotoxy(pos.x=1,++pos.y);
							break;
			case 13: /* first column */
                        	gotoxy(pos.x=1,pos.y);
                                break;

                        case 8: /* \b: backspace */
                        	if (pos.x-1) gotoxy(--pos.x,pos.y);
							((ushort *) VIDEO_ADDR)[NR_COLUMNS*(pos.y-1) + (pos.x)-1] = ' ' + (attr << 8);
                                break;

			case 9: /* \t: tab */
                                c = 8 - (pos.x & 7);
                                gotoxy(pos.x += c,pos.y);
                                break;

                        default:
                        		((ushort *) VIDEO_ADDR)[NR_COLUMNS*(pos.y-1) + (pos.x++)-1] = c + (attr << 8);
								setpos(&pos);
                                break;
                } /* end of switch */
				if (pos.y > NR_ROWS) {
					//memcpy( (void*)VIDEO_ADDR, (void*)(VIDEO_ADDR+160), NR_COLUMNS*NR_ROWS*2);
					for(i = 0; i < 80*24; i++)	/* Scroll the screen up		*/
						((ushort *)VIDEO_ADDR)[i] = ((ushort*)VIDEO_ADDR)[i+80];
					for (c = 0; c < 80; c++)
						((ushort *) VIDEO_ADDR)[NR_COLUMNS*(NR_ROWS-1) + c] = ' ' + (attr << 8);
					gotoxy(pos.x,pos.y-1);
				}

        }
}

/* print formatted output */
/*int printf(char *fmt, ...)
{
	va_list args;
	int i;
	char buf[512];

	//attr=31;
	va_start(args, fmt);
	i = vsprintf(buf, fmt, args);	//copy the variables to the string
	va_end(args);

	puts(buf);						//Print the text out
        
	return i;
} */

//Put a letter on the screen
void inline putc(char c)
{
	t_pos spos=getpos();							//Get cursor position
	unsigned int pos=spos.x + spos.y*NR_COLUMNS;	
	
	((ushort *) VIDEO_ADDR)[pos] = c + (attr << 8);	//Put the letter on the screen
	spos.x++;										//Update the position
	setpos(&spos);									
}

/* Get cursor position */
t_pos inline getpos(void)
{
	t_pos tpos;
	unsigned short pos;
	
	outportb(CRTC_COMMAND, CRTC_CURHI);		/* Get cursor Y position	*/
	pos = inportb(         CRTC_DATA);
	pos <<= 8;
	outportb(CRTC_COMMAND, CRTC_CURLO);		/* And add cursor X position	*/
	pos += inportb(        CRTC_DATA);
	//pos<<= 1;

	tpos.x=pos % NR_COLUMNS + 1;
	tpos.y=pos / NR_COLUMNS + 1;
	
	return tpos;
}

/* Set cursor positon*/
void inline setpos(t_pos *_pos)
{
	unsigned int pos;

	// If x is bigger then the width of the screen, move down a row 
	if (_pos->x > NR_COLUMNS) {
		_pos->x = _pos->x % NR_COLUMNS+1;
		_pos->y+=  _pos->x / NR_COLUMNS+1;
	}
                     
	pos= ((_pos->y-1) * NR_COLUMNS + _pos->x - 1);
/*	outportb(CRTC_COMMAND, CRTC_CURLO);
    outportb(CRTC_DATA   , (unsigned char) pos);

	outportb(CRTC_COMMAND, CRTC_CURHI);
	outportb(CRTC_DATA   , (unsigned char) (pos >> 8));*/

   //pos >>= 1;			/* Set the new cursor position  */
   outportb(CRTC_COMMAND, CRTC_CURLO );
   outportb(CRTC_DATA   , pos & 0x0ff);
   outportb(CRTC_COMMAND, CRTC_CURHI );
   outportb(CRTC_DATA   , pos >> 8   );

}

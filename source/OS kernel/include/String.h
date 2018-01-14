#ifndef STRING_H
#define STRING_H

#include "types.h"

/*	Kopiera en sträng*/
extern inline char* strcpy (      char* dest, const char* src );
/*	Addera en sträng till en annan*/
extern inline char* strcat (      char* dest, const char* src );
/*	Jämför två strängar med varandra
	Den klara inte av att avgöra vilken som är störst*/
extern inline bool  strcmp (const char* str1, const char* str2);
/*	Returnera längden på en sträng*/
extern inline int   strlen (const char* str                   );


/* Kopiera en minesarea till en annan*/
extern inline void* memcpy (      void* dest, const void* src, int n);
/* Flytta en minesarea till en annan*/
extern inline void* memmove(      void* dest, const void* src, int n);


/* Returnera om tecknet är tabb, Carriage return, Line Feed, Form Feed eller mellanslag */
extern inline bool  iswhite(const char  src);
/* Gör tecknet stort*/
extern inline char  cupr   (      char  src);
/* Gör alla tecken i strängen till stora*/ 
extern inline char* strupr (      char* src);
/* Returnera vänstra delen av en sträng.*/
extern inline char* left   (      char* dest, const char* src, int n);
/* Returnera högra delen av en sträng.*/
extern inline char* right  (      char* dest, const char* src, int n);
/* Ta bort vita tecken och komman längst till vänster.*/
extern inline char* ltrim  (      char* dest, const char* src);

#endif

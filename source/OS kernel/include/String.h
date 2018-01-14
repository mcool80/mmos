#ifndef STRING_H
#define STRING_H

#include "types.h"

/*	Kopiera en str�ng*/
extern inline char* strcpy (      char* dest, const char* src );
/*	Addera en str�ng till en annan*/
extern inline char* strcat (      char* dest, const char* src );
/*	J�mf�r tv� str�ngar med varandra
	Den klara inte av att avg�ra vilken som �r st�rst*/
extern inline bool  strcmp (const char* str1, const char* str2);
/*	Returnera l�ngden p� en str�ng*/
extern inline int   strlen (const char* str                   );


/* Kopiera en minesarea till en annan*/
extern inline void* memcpy (      void* dest, const void* src, int n);
/* Flytta en minesarea till en annan*/
extern inline void* memmove(      void* dest, const void* src, int n);


/* Returnera om tecknet �r tabb, Carriage return, Line Feed, Form Feed eller mellanslag */
extern inline bool  iswhite(const char  src);
/* G�r tecknet stort*/
extern inline char  cupr   (      char  src);
/* G�r alla tecken i str�ngen till stora*/ 
extern inline char* strupr (      char* src);
/* Returnera v�nstra delen av en str�ng.*/
extern inline char* left   (      char* dest, const char* src, int n);
/* Returnera h�gra delen av en str�ng.*/
extern inline char* right  (      char* dest, const char* src, int n);
/* Ta bort vita tecken och komman l�ngst till v�nster.*/
extern inline char* ltrim  (      char* dest, const char* src);

#endif

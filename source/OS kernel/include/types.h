#ifndef TYPES_H
	#define TYPES_H

	typedef unsigned char	bool;
	typedef unsigned char	byte;
	typedef unsigned char	uchar;
	typedef unsigned short	ushort;
	typedef unsigned long	ulong;
	typedef unsigned int	uint;

	#define false	0
	#define true	1
	#define and		&&
	#define or		||
	#define	not		!

	#ifndef NULL
		#define NULL ( (void*)0 )
	#endif
#endif

/***************************************************************************
 *                                                                         *
 *   copyright (C) 2002, 2003, 2004 by Michael Buesch                      *
 *   email: mbuesch@freenet.de                                             *
 *                                                                         *
 *   tiger.c  -  The TIGER hash function                                   *
 *	Copyright (C) 1998, 2001, 2002 Free Software Foundation, Inc.      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 ***************************************************************************/

#ifndef TIGER_H
#define TIGER_H

#include "globalstuff.h"

#include <stdint.h>
#include <string>

using std::string;

#define TIGER_HASHLEN_BYTE	(192 / 8)

class Tiger
{
	struct TIGER_CONTEXT
	{
		uint64_t	a, b, c;
		byte		buf[64];
		int		count;
		uint32_t	nblocks;
	};

public:
	Tiger();
	virtual ~Tiger();
	static bool selfTest();

	string calcTiger(const string &buf);

protected:
	void burn_stack(int bytes);
	void tiger_init();
	void round( uint64_t *ra, uint64_t *rb, uint64_t *rc, uint64_t x, int mul );
	void pass( uint64_t *ra, uint64_t *rb, uint64_t *rc, uint64_t *x, int mul );
	void key_schedule( uint64_t *x );
	void transform(const byte *data );
	void tiger_write(const byte *inbuf, size_t inlen);
	byte * tiger_final();

protected:
	TIGER_CONTEXT *context;
};

#endif

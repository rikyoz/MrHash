/***************************************************************************
 *                                                                         *
 *   copyright (C) 2004 by Michael Buesch                                  *
 *   email: mbuesch@freenet.de                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 ***************************************************************************/

#ifndef RMD160_H
#define RMD160_H

#include "bithelp.h"
#include "globalstuff.h"

#include <stdint.h>
#include <string>
using std::string;

#define RMD160_HASHLEN_BYTE	(160 / 8)

class Rmd160
{
	struct RMD160_CONTEXT
	{
		uint32_t  h0,h1,h2,h3,h4;
		uint32_t  nblocks;
		byte buf[64];
		int  count;
	};

public:
	Rmd160();
	virtual ~Rmd160();
	static bool selfTest();

	string calcRmd160(const string &buf);

protected:
	void burn_stack(int bytes);
	void rmd160_init();
	void transform(const byte *data);
	void rmd160_write(const byte *inbuf, size_t inlen);
	byte * rmd160_final();

protected:
	RMD160_CONTEXT *context;
};

#endif

/***************************************************************************
 *                                                                         *
 *   copyright (C) 2002, 2003, 2004 by Michael Buesch                      *
 *   email: mbuesch@freenet.de                                             *
 *                                                                         *
 *   Public domain implementation by                                       *
 *   Paulo S.L.M. Barreto <pbarreto@nw.com.br>                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 ***************************************************************************/

#ifndef HAVAL_H
#define HAVAL_H

#include "globalstuff.h"

#include <stdint.h>
#include <string.h>
#include <string>
using std::string;

class Haval {
	struct havalContext
	{
		uint16_t passes, hashLength;	/* HAVAL parameters */
		uint32_t digest[8];		/* message digest (fingerprint) */
		byte	 block[128];		/* context data block */
		size_t	 occupied;		/* number of occupied bytes in the data block */
		uint32_t bitCount[2];		/* 64-bit message bit count */
		uint32_t temp[8];		/* temporary buffer */
	};

public:
	Haval() {}
	virtual ~Haval() {}
	static bool selfTest();

	string calcHaval(const string &buf, int bit, int passes);
protected:
	void havalTransform3(uint32_t E[8], const byte D[128], uint32_t T[8]);
	void havalTransform4(uint32_t E[8], const byte D[128], uint32_t T[8]);
	void havalTransform5(uint32_t E[8], const byte D[128], uint32_t T[8]);
	void mhash_bzero(void *s, int n)
			{ memset(s, '\0', n); }
	/** Initialize a HAVAL hashing context according to the desired
	  * number of passes and hash length.  Returns:
	  *	0: no error.
	  *	1: hcp is NULL.
	  *	2: invalid number of passes (must be 3, 4, or 5).
	  *	3: invalid hash length (must be 128, 160, 192, 224, or 256).
	  */
	int havalInit(havalContext *hcp, int passes, int length);
	/** Updates a HAVAL hashing context with a data block dataBuffer
	  * of length dataLength.  Returns:
	  *	0: no error.
	  *	1: hcp is NULL.
	  */
	int havalUpdate(havalContext *hcp, const byte *dataBuffer, size_t dataLength);
	/** Finished evaluation of a HAVAL digest, clearing the context.
	  * The digest buffer must be large enough to hold the desired
	  * hash length.  Returns:
	  *	0: no error.
	  *	1: hcp is NULL.
	  *	2: digest is NULL.
	  */
	int havalFinal(havalContext *hcp, byte *digest);
};

#endif

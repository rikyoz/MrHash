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
        struct HAVAL_CONTEXT {
            uint32_t digest[8];		/* message digest (fingerprint) */
            byte	 block[128];	/* context data block */
            size_t	 occupied;		/* number of occupied bytes in the data block */
            uint32_t bitCount[2];	/* 64-bit message bit count */
            uint32_t temp[8];		/* temporary buffer */
        };

    public:
        Haval( uint16_t hashLength, uint16_t passes );
        virtual ~Haval();

        string calcHaval( const string &buf );
    protected:
        void init();
        void write( const byte* dataBuffer, int dataLength );
        byte* final();

        int hash_length() const { return _length / 8; }

    private:
        void mhash_bzero( void* s, int n ) { memset( s, '\0', n ); }
        void havalTransform3( uint32_t E[8], const byte D[128], uint32_t T[8] );
        void havalTransform4( uint32_t E[8], const byte D[128], uint32_t T[8] );
        void havalTransform5( uint32_t E[8], const byte D[128], uint32_t T[8] );

        HAVAL_CONTEXT* context;
        uint16_t _passes, _length; /* HAVAL parameters */
};

#endif

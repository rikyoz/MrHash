/***************************************************************************
 *                                                                         *
 *   copyright (C) 2003, 2004 by Michael Buesch                            *
 *   email: mbuesch@freenet.de                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 ***************************************************************************/

#include "globalstuff.h"

#include <cstdio>
#include <sys/stat.h>
#include <cstring>

using std::string;

string charToHex( const char* buf, unsigned int len ) {
    // Hm, can be optimized... :)
    unsigned int i;
    string ret;
    char tmp[3], cur;

    for ( i = 0; i < len; ++i ) {
        cur = *( buf + i );
        if ( cur == 0 )
            ret += "00";
        else {
            sprintf( tmp, "%X", 0xFF & *( buf + i ) );
            if ( strlen( tmp ) == 1 )
                ret += '0';
            ret += tmp;
        }
    }

    return ret;
}

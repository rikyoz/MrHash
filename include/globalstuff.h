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

/** here is some useful stuff used globaly */

#ifndef GLOBALSTUFF_H
#define GLOBALSTUFF_H

#include <string>
#include <sstream>
#include <cstdint>

/** convert a char (array) to hex notation and return it as string. */
extern std::string charToHex(const char *buf, unsigned int len);

typedef uint8_t		byte;

#endif // GLOBALSTUFF_H

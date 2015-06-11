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
#include <stdint.h>

/** convert something to string using ostringstream */
template <class T> inline
std::string tostr(const T &t)
{
	std::ostringstream s;
	s << t;
	return s.str();
}

/** convert a char (array) to hex notation and return it as string. */
extern std::string charToHex(const char *buf, unsigned int len);
/** calculate the buffer-size for reading
  * if the "struct stat" of the file is given
  */
extern unsigned int calcBufSize(struct stat file_stat);

typedef uint8_t		byte;

#ifdef CONFIG_DEBUG
# define CALCCHECKSUM_DEBUG
#else
# undef CALCCHECKSUM_DEBUG
#endif

/** define likely() and unlikely() */
#if !defined(__GNUC__) || !defined(__GNUC_MINOR__) \
    || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
# define __builtin_expect(x, expected_value) (x)
#endif
#define likely(x)	__builtin_expect((x),1)
#define unlikely(x)	__builtin_expect((x),0)

/** delete the memory and NULL the pointer */
#define delete_and_null(x)	do {			\
					delete (x);	\
					(x) = 0;	\
				} while (0)
/** delete the memory if the pointer isn't a NULL pointer */
#define delete_ifnot_null(x)	do {					\
					if (x)				\
						delete_and_null(x);	\
				} while (0)

#define delete_and_null_array(x) do {			\
					delete [] (x);	\
					(x) = 0;	\
				} while (0)

#define delete_ifnot_null_array(x) do {					\
					if (x)				\
						delete_and_null_array(x); \
				} while (0)

/** return the number of elements in an array */
#define array_size(x)		(sizeof(x) / sizeof((x)[0]))

#endif // GLOBALSTUFF_H

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

#ifndef ERR_H
#define ERR_H

#include "globalstuff.h"

#include <iostream>
#include <string>
using std::string;
using std::cerr;
using std::cout;
using std::endl;


/** Assertion function */
#ifdef CALCCHECKSUM_DEBUG
# define CALCCHECKSUM_ASSERT(x)	do {						\
                if (!(x)) {					\
                    cerr << "CALCCHECKSUM_ASSERT failed: " << #x	\
                         << " in " << __FILE__		\
                         << ":" << __LINE__			\
                         << endl;				\
                }						\
            } while (0)
#else
# define CALCCHECKSUM_ASSERT(x) do {} while (0)
#endif

#define BUG()	do {					\
        cerr << "CalcChecksum BUG at "		\
             << __FILE__ << ":" << __LINE__	\
             << endl;				\
        } while (0)

#define TODO()	do {					\
        cerr << "CalcChecksum TODO at "		\
             << __FILE__ << ":" << __LINE__	\
             << endl;				\
        } while (0)

#define FIXME()	do {					\
        cerr << "CalcChecksum FIXME at "	\
             << __FILE__ << ":" << __LINE__	\
             << endl;				\
        } while (0)

#ifdef CALCCHECKSUM_DEBUG
inline
void printDebug(string msg)
{
    cout << "CalcChecksum DEBUG: " << msg << endl;
}
#else
# define printDebug(x)	do {} while (0)
#endif

inline
void printInfo(string msg)
{
    cout << "CalcChecksum INFO: " << msg << endl;
}

inline
void printWarn(string msg)
{
    cerr << "CalcChecksum WARNING: " << msg << endl;
}

inline
void printError(string msg)
{
    msg = "CalcChecksum generated a fatal fault:\n" + msg;
    cerr << "\n\n" << msg << endl;
}

#endif

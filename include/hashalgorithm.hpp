#ifndef HASHALGORITHM
#define HASHALGORITHM

#include <cstdint>

typedef uint8_t	byte;

class HashAlgorithm {
    public:
        virtual ~HashAlgorithm() {}

        virtual void init() = 0;
        virtual void write( const byte* inbuf, int inlen ) = 0;
        virtual byte* final() = 0;
        virtual unsigned int hash_length() const = 0;
};

#endif // HASHALGORITHM


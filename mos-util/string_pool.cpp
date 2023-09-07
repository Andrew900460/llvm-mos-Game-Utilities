#include "string_pool.hpp"

#include "mos-util.hpp"

// Plan:
// Implement a system for temporarily allocating strings that functions
// Similar to heap allocation, but specifically for strings

// There will be a 256 byte block of memory dedicated to strings
// It will be divided up into 8 memory chunks, each being a string of 32 chars.

// The idea is that when you need to generate a string from some other data,
// you can "request" the usage of one of these spaces (strings) to write the data.
// It would then be your responsibility to copy the resulting string elsewhere.
// Because there are only 8 total strings, meaning you can only work with
// 8 different strings at a time.

// Though, this system could also allow you to "request" 2 chunks that would represent
// 1 bigger string of 64 chars, if you know you will need the space.

// |||||||| |||||||| |||||||| |||||||| string 0
// |||||||| |||||||| |||||||| |||||||| string 1
// |||||||| |||||||| |||||||| |||||||| string 2
// |||||||| |||||||| |||||||| |||||||| string 3
// |||||||| |||||||| |||||||| |||||||| string 4
// |||||||| |||||||| |||||||| |||||||| string 5
// |||||||| |||||||| |||||||| |||||||| string 6
// |||||||| |||||||| |||||||| |||||||| string 7

byte _availableString = 0;
char _stringPool[256];

char* RequestStringFromPool() {
    const char* s = _stringPool+_availableString;
    _availableString += STRING_POOL_EACH_SIZE;
    return (char*)s;
}
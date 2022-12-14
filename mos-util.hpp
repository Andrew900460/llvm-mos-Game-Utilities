#pragma once

#include <cstdint>

// Right now this file contains common stuff that other source files may need,
// but it will probably grow to contain a bunch of stuff.

// essentially copies the bits from "data" to whatever the other variable is
// by forcefully changing the type by using pointer casting
// MyType A = force(B,MyType)
#define force(data,type) *(type*)&data

typedef unsigned char byte;
typedef uint16_t uint16;
typedef uint32_t uint32;

static const byte reverse_lookup[16] = {0x0,0x8,0x4,0xc,0x2,0xa,0x6,0xe,0x1,0x9,0x5,0xd,0x3,0xb,0x7,0xf};

// reverses the bits in a single byte
byte reverse_bits(byte n) {
   return (reverse_lookup[n&0b1111] << 4) | reverse_lookup[n>>4];
}


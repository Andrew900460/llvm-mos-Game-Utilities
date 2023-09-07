#pragma once

#include "types.hpp"

// Right now this file contains common stuff that other source files may need,
// but it will probably grow to contain a bunch of stuff.

// essentially copies the bits from "data" to whatever the other variable is
// by forcefully changing the type by using pointer casting
// MyType A = force(B,MyType)
#define force(data,type) *reinterpret_cast<type*>(&data)

template<typename T>
inline constexpr volatile T& ram(const uint16 address) {
	return *reinterpret_cast<T*>(address);
}

inline volatile byte& ram8(const uint16 address) {
	return *reinterpret_cast<byte*>(address);
}

inline volatile uint16& ram16(const uint16 address) {
	return *reinterpret_cast<uint16*>(address);
}

inline volatile uint32& ram32(const uint16 address) {
	return *reinterpret_cast<uint32*>(address);
}

extern "C" {

// Uses the fastest known way on the 6502 to multiply by 10
// Useful for converting from binary to ascii
byte mult_10(byte a);

}

// reverses the bits in a single byte
inline byte reverse_bits(byte n) {
	static const byte reverse_lookup[16] = {0x0,0x8,0x4,0xc,0x2,0xa,0x6,0xe,0x1,0x9,0x5,0xd,0x3,0xb,0x7,0xf};
	return ((byte)(reverse_lookup[n&0b1111] << 4)) | reverse_lookup[n>>4];
}

template<class T> 
void swap(T& a, T& b) {
	T temp(a);
	a=b;
	b=temp;
}

template<typename T>
void ReverseArrayInPlace(T arr[],const byte size) {
	const byte reverses = size >> 1; // divide by 2
	for(byte i=0; i<reverses; i++) {
		swap(arr[i],arr[size-1-i]);
	}
}


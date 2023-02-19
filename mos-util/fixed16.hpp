#pragma once

#include <stdio.h>
#include "mos-util.hpp"

// 16 Bit Fixed Point Value
// You will need this if you want to simulate physics or do animations-
// with fractional precision.
// It functions under the same ideas as "floating point" variables,
// but the "decimal point" has a fixed position, in this case, between 2 bytes

// data layout (each letter is 1 bit)
// (W = Whole part of the value [integer] ) (F = Fractional part of the value )
//  		WWWWWWWW-FFFFFFFF
// 0.500 =	00000000-10000000
// 3.625 =	00000011-10100000

struct fixed16 {
	union {
		uint16 value;
		struct {
			byte frac;
			byte whole;
		};
	};

	constexpr fixed16() : value(0) {}
	constexpr explicit fixed16(const uint16& v) : value(v) {}
	constexpr explicit fixed16(const uint16&& v) : value(v) {}
	explicit fixed16(const byte whole, const byte frac): frac(frac), whole(whole) {}
	fixed16(const fixed16& v) : value(v.value) {}

	explicit constexpr fixed16(const char str[]) : value(fixed16::from_string(str).value) {}

	fixed16& operator = (const uint16 rhs) {
		value = rhs;
		return *this;
	};
	
	fixed16 operator + (const fixed16 rhs) const { // 6502 clock time of ~[26,48]
		return fixed16(value+rhs.value);
	}
	fixed16 operator - (const fixed16 rhs) const { // 6502 clock time of ~[26,30]
		return fixed16(value-rhs.value);
	}
	fixed16 operator * (const fixed16 rhs) const { // 6502 clock time of ~[883,1357]
		uint32 t1 = (uint32)value;
		uint32 t2 = (uint32)rhs.value;
		return fixed16((uint16) ((t1*t2) >> 8));
	}
	fixed16 operator / (const fixed16 rhs) const { // 6502 clock time of ~[1940,2393]
		//fixed16 r;
		//r = (fixed16);
		return fixed16((uint16)(((uint32)value << 8) / (uint32)rhs.value));
	}

	fixed16& operator += (const fixed16 rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}
	fixed16& operator -= (const fixed16 rhs) {
		(*this) = (*this)-rhs;
		return *this;
	}
	fixed16& operator *= (const fixed16 rhs) {
		(*this) = (*this)*rhs;
		return *this;
	}
	fixed16& operator /= (const fixed16 rhs) {
		(*this) = (*this)/rhs;
		return *this;
	}

	bool operator == (const fixed16 rhs) const {
		return value == rhs.value;
	}
	bool operator != (const fixed16 rhs) const {
		return !(*this == rhs);
	}

	bool operator < (const fixed16 rhs) const {
		const fixed16 sub = (*this)-rhs;
		return sub.value > 0 && ((sub.whole & 0b10000000) == 0);
	}
	bool operator > (const fixed16 rhs) const {
		return rhs < *this;
	}
	bool operator <= (const fixed16 rhs) const {
		return !(*this > rhs);
	}
	bool operator >= (const fixed16 rhs) const {
		return !(*this < rhs);
	}

	// Is there a even better implementation than this?
	// 6502 clock time of ~[7599,12493] (results are ~40% faster with 'x.whole == x_old.whole')
	fixed16 sqrt() const {
		//fixed16 n = value;
		fixed16 x(0x1000);
		uint32 n_one = (uint32)this->value << 8;
		while(true) {
			fixed16 x_old = x;
			x = (x.value + (uint16)(n_one / x.value)) >> 1;
			if(x.whole == x_old.whole) break;
			// if you replace the above expression with just 'x == x_old'
			// it will go slower, but be a bit more accurate
		}
		return x;
	}

	fixed16 inverse() const { // basically does 1/a
		return fixed16(0x10000 / (uint32)this->value);
	}

	static constexpr fixed16 from_string(const char str[]) { // initial implementation, could obviously be better
		fixed16 result;

		byte p = 0;
		byte wholeDigits = 0;
		while(str[p]!=0) { // find decimal point
			if(str[p]=='.') break;
			p++;
		}
		wholeDigits = p;

		for(byte i=0; i<wholeDigits; i++) { // generate integer part
			byte digit = str[i]-'0';
			
			for(byte i2=0; i2<p-1; i2++) {digit*=10;}
			
			result.whole += digit;
			p--;
		}

		if(str[wholeDigits]!='.') {
			return result;
		}

		// generate fractional part
		const fixed16 one_over_ten(0x0019);
		fixed16 mult = one_over_ten;
		p=wholeDigits+1;
		while(str[p]!=0) {
			fixed16 digit;
			digit.whole = str[p]-'0';
			
			digit = digit*mult;
			result.frac = result.frac+digit.frac;
			mult = mult*one_over_ten;
			p++;
		}
		
		return result;
	}

	char* to_string(char buffer[], const byte size) { // initial implementation, could obviously be better
		char offset = sprintf(buffer, "%u", whole);
		
		char i = offset;
		buffer[i++] = '.';

		if(frac==0) {
			buffer[i++] = '0';
		} else {
			uint16 fracTemp = frac;
			while (i < 16 && fracTemp != 0) {
				fracTemp *= 10;
				char digit = (fracTemp >> 8) + 48;
				fracTemp &= 0x00FF;
				buffer[i] = digit;
				i++;
			}
		}
		buffer[i] = 0;
		return buffer;
	}

	// This function may change to not use heap allocation
	// And opt to use another form of string allocation that is faster
	char* to_string() { 
		char* output = new char[16];
		to_string(output,16);
		//delete[] output;
		return output;
	}

};

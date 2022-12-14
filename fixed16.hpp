#pragma once

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

	fixed16() { value=0; }
	fixed16(const uint16 v) { value=v; }
	fixed16(const byte whole, const byte frac): frac(frac), whole(whole) {}

	fixed16 operator = (const uint16 rhs) {
		this->value = rhs;
		return *this;
	}
	fixed16 operator + (const fixed16& rhs) { // 6502 clock time of ~[26,48]
		return fixed16 { (uint16)(value+rhs.value) };
	}
	fixed16 operator - (const fixed16& rhs) { // 6502 clock time of ~[26,30]
		return fixed16{ (uint16)(value-rhs.value) };
	}
	fixed16 operator * (const fixed16& rhs) { // 6502 clock time of ~[883,1357]
		uint32 t1 = (uint32)value;
		uint32 t2 = (uint32)rhs.value;
		return { (uint16) ((t1*t2) >> 8) };
	}
	fixed16 operator / (const fixed16& rhs) { // 6502 clock time of ~[1940,2393]
		fixed16 r;
		r = (fixed16) (((uint32)value << 8) / (uint32)rhs.value);
		return r;
	}
	static fixed16 inverse(const fixed16 a) { // basically does 1/a
		return 0x10000 / (uint32)a.value;
	}

	static fixed16 from_string(const char str[]) { // initial implementation, could obviously be better
		fixed16 result;

		byte p = 0;
		byte wholeDigits;
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
		const fixed16 one_over_ten = 0x0019;
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
	char* to_string() { // initial implementation, could obviously be better
		char* output = new char[16];

		char offset = sprintf(output, "%u", whole);
		
		char i = offset;
		output[i++] = '.';

		if(frac==0){
			output[i++] = '0';
		} else {
			uint16 fracTemp = frac;
			while (i < 16 && fracTemp != 0) {
				fracTemp *= 10;
				char digit = (fracTemp >> 8) + 48;
				fracTemp &= 0x00FF;
				output[i] = digit;
				i++;
			}
		}
		output[i] = 0;
		return output;
	}

};
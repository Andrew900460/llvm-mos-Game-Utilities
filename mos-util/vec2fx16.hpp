#pragma once

#include "fixed16.hpp"

struct vec2fx16 {

	fixed16 x;
	fixed16 y;

	vec2fx16() = default; //: x(0),y(0) {}
	vec2fx16(const fixed16 x, const fixed16 y): x(x),y(y) {}
	vec2fx16(const vec2fx16& v): x(v.x),y(v.y) {}

	vec2fx16& operator = (const vec2fx16& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	vec2fx16 operator + (const vec2fx16& rhs) { // 6502 clock time of ~88
		return vec2fx16 {x+rhs.x, y+rhs.y};
	}
	vec2fx16 operator - (const vec2fx16& rhs) { // 6502 clock time of ~88
		return vec2fx16 {x-rhs.x, y-rhs.y};
	}
	vec2fx16 operator * (const fixed16& scalarRHS) { // 6502 clock time of ~[1790,2224]
		return vec2fx16 {x*scalarRHS,y*scalarRHS};
	}
	vec2fx16 operator / (const fixed16& scalarRHS) { // 6502 clock time of ~[3524,3903]
		return vec2fx16 {x/scalarRHS,y/scalarRHS};
	}

	vec2fx16& operator += (const vec2fx16& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}
	vec2fx16& operator -= (const vec2fx16& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}
	vec2fx16& operator *= (const fixed16& scalarRHS) {
		(*this) = (*this)*scalarRHS;
		return *this;
	}
	vec2fx16& operator /= (const fixed16& scalarRHS) {
		(*this) = (*this)/scalarRHS;
		return *this;
	}

	bool operator == (const vec2fx16& rhs) {
		return x==rhs.x && y==rhs.y;
	}
	

};
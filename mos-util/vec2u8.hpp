#pragma once

#include "mos-util.hpp"

struct vec2u8 {

	byte x;
	byte y;

	vec2u8(): x(0),y(0) {}
	vec2u8(const byte x,const byte y): x(x),y(y) {}
	vec2u8(const vec2u8& v): x(v.x),y(v.y) {}

	void operator = (const vec2u8& rhs) {
		x = rhs.x;
		y = rhs.y;
	}
	vec2u8 operator + (const vec2u8& rhs) {
		return vec2u8(x+rhs.x,y+rhs.y);
	}
	vec2u8 operator - (const vec2u8& rhs) {
		return vec2u8(x-rhs.x, y-rhs.y);
	}
	vec2u8 operator * (const byte& scalarRHS) {
		return vec2u8(x*scalarRHS,y*scalarRHS);
	}
	vec2u8 operator / (const byte& scalarRHS) {
		return vec2u8(x/scalarRHS,y/scalarRHS);
	}

	void operator += (const vec2u8& rhs) {
		(*this) = (*this)+rhs;
	}
	void operator -= (const vec2u8& rhs) {
		(*this) = (*this)+rhs;
	}
	void operator *= (const byte& scalarRHS) {
		(*this) = (*this)*scalarRHS;
	}
	void operator /= (const byte& scalarRHS) {
		(*this) = (*this)/scalarRHS;
	}

	bool operator == (const vec2u8& rhs) {
		return x==rhs.x && y==rhs.y;
	}


	

};
#pragma once

#include "mos-util.hpp"

struct vec2u8 {

	byte x;
	byte y;

	vec2u8() = default; //: x(0),y(0) {}
	vec2u8(const byte x,const byte y): x(x),y(y) {}
	vec2u8(const vec2u8& v): x(v.x),y(v.y) {}

	vec2u8& operator = (const vec2u8& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
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

	vec2u8& operator += (const vec2u8& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}
	vec2u8& operator -= (const vec2u8& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}
	vec2u8& operator *= (const byte& scalarRHS) {
		(*this) = (*this)*scalarRHS;
		return *this;
	}
	vec2u8& operator /= (const byte& scalarRHS) {
		(*this) = (*this)/scalarRHS;
		return *this;
	}

	bool operator == (const vec2u8& rhs) {
		return x==rhs.x && y==rhs.y;
	}


	

};
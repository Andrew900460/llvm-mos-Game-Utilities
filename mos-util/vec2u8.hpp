#pragma once

#include "mos-util.hpp"

struct vec2u8 {

	byte x;
	byte y;

	constexpr vec2u8() : x(0),y(0) {}
	explicit vec2u8(const byte x,const byte y): x(x),y(y) {}
	explicit vec2u8(const vec2u8& v): x(v.x),y(v.y) {}

	vec2u8& operator = (const vec2u8& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	vec2u8 operator + (const vec2u8& rhs) const {
		return vec2u8(x+rhs.x,y+rhs.y);
	}
	vec2u8 operator - (const vec2u8& rhs) const {
		return vec2u8(x-rhs.x, y-rhs.y);
	}
	vec2u8 operator * (const byte& scalarRHS) const {
		return vec2u8(x*scalarRHS,y*scalarRHS);
	}
	vec2u8 operator / (const byte& scalarRHS) const {
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

	bool operator == (const vec2u8& rhs) const {
		return x==rhs.x && y==rhs.y;
	}


	

};
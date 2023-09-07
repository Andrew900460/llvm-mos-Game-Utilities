#include "vec2u8.hpp"

vec2u8 PositionToCenter(const vec2u8 point, const byte width, const byte height){
	return vec2u8(point.x-(width>>1),point.y+(height>>1));
}
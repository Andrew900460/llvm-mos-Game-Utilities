#include "fixed16.hpp"
#include <stdio.h>
#include "mos-util.hpp"
#include "string_pool.hpp"

char* fixed16::to_string() {
	const size_t bufferSize = STRING_POOL_EACH_SIZE;
	char* buffer = RequestStringFromPool();
	//char offset = sprintf(buffer, "%i", whole);
	size_t wholePartBytesWritten = snprintf(buffer, bufferSize, "%i", this->whole);

	size_t i = wholePartBytesWritten;
	buffer[i++] = '.';

	if(frac==0) {
		buffer[i++] = '0';
	} else {
		uint16 fracTemp = frac;
		while (i < bufferSize && fracTemp != 0) {
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
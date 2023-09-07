#include "fceux-debug.hpp"

#include <cstdarg>
#include <cstdio>
#include "mos-util.hpp"

#define MESSAGE_LOCATION 0x7000

typedef size_t (*StringWriter)(char* buffer, size_t bufferSize);

char fceux::outputBuffer[fceux::OUTPUT_BUFFER_SIZE] = "";

void fceux::FireBreakpoint() {
	ram<byte>(MESSAGE_LOCATION) = 2;
}

void fceux::DoTimeCheck() {
	ram<byte>(MESSAGE_LOCATION) = 8;
}

void fceux::WriteMessage(const char* format...) {

	va_list args;
	va_start(args,format);

	size_t totalCharsWritten = 0;

	// TODO: Add code that will look for %P and interpret vararg as a
	// function pointer that serves to output our string data
	// Or maybe, the function will write to the buffer itself.

	char ch;
	while((ch = *format++)) {
		const size_t bufferSizeLeft = OUTPUT_BUFFER_SIZE-totalCharsWritten;
		char* bufferOffset = outputBuffer+totalCharsWritten;

		if(bufferSizeLeft == 1) {
			*bufferOffset = 0;
			break;
		}

		if(ch == '%') {
			const char formatCode = (ch = *format++);
			if(formatCode == 'p') {
				StringWriter swFunc = (StringWriter)va_arg(args, void*);
				size_t charsWritten = swFunc(bufferOffset, bufferSizeLeft);
				totalCharsWritten += charsWritten;
			}else if(formatCode == 's') {
				char* str = va_arg(args, char*);
				size_t charsWritten = (size_t)snprintf(bufferOffset,bufferSizeLeft,"%s",str);
				totalCharsWritten+=charsWritten;
			}
		}else {
			*bufferOffset = ch;
			totalCharsWritten++;
		}
	}

	//vsnprintf(outputBuffer,64,format,args);

	ram<uint16>(MESSAGE_LOCATION+1) = (uint16)outputBuffer;
	ram<byte>(MESSAGE_LOCATION) = 4;
	ram<byte>(MESSAGE_LOCATION) = 1;

	va_end(args);
}



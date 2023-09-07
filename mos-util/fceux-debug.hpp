#pragma once

#include "types.hpp"

namespace fceux {
	constexpr uint16 OUTPUT_BUFFER_SIZE = 64;

	extern char outputBuffer[OUTPUT_BUFFER_SIZE];

	void FireBreakpoint();

	void DoTimeCheck();

	void WriteMessage(const char* format...);
}


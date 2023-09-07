#pragma once

#include "types.hpp"

/// This is an API for an "Extended Virtual Hardware" I made for helping
/// in debugging/testing my Nes game code with FCEUX's Lua scripting feature

/// Fceux (the emulator) allows you to run custom Lua scripts in tandom with
/// a running Nes game in the emulator. Which can allow you to do a variety
/// of things.
/// In this case, the Lua script will watch a certain Ram address, and when it's
/// written to, it will read the value and execute some command based on the value.
/// This ultimately allows me to "print" data from inside the ram, do performance profiling,
/// and execute a breakpoint to effectively pause the game at a certain moment in the code.

/// This is all happening due to the Lua script acting like a new piece of hardware for the Nes.
/// Like the PPU, APU, Joypads, etc
/// You can find the Lua script in this repo as well.

namespace fceux {
	constexpr uint16 OUTPUT_BUFFER_SIZE = 64;

	extern char outputBuffer[OUTPUT_BUFFER_SIZE];

	void FireBreakpoint();

	void DoTimeCheck();

	void WriteMessage(const char* format...);
}


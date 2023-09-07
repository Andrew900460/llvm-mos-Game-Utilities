#pragma once

#include <neslib.h>
#include "../mos-util.hpp"
#include "../mos-vec-math.hpp"

extern "C" {
byte ReadJoypad1(); // not really working 100%
byte ReadJoypad2(); // not really working 100%
}

struct Joypad {
	byte inputData;

	constexpr Joypad() : inputData(0) {}
	constexpr explicit Joypad(byte joypadData) : inputData(joypadData){}

	// void ReadInput(const byte joypadNumber=1) {
	// 	if(joypadNumber==1) {
	// 		inputData = ReadJoypad1();
	// 	}else if(joypadNumber==2){
	// 		inputData = ReadJoypad2();
	// 	}
	// }

	// constexpr vec2u8 DpadVector() {

	// }

	constexpr bool PressUp() {
		return (inputData & PAD_UP) != 0;
	}

	constexpr bool PressDown() {
		return (inputData & PAD_DOWN) != 0;
	}

	constexpr bool PressLeft() {
		return (inputData & PAD_LEFT) != 0;
	}

	constexpr bool PressRight() {
		return (inputData & PAD_RIGHT) != 0;
	}

	constexpr bool PressA() {
		return (inputData & PAD_A) != 0;
	}

	constexpr bool PressB() {
		return (inputData & PAD_B) != 0;
	}

	constexpr bool PressSelect() {
		return (inputData & PAD_SELECT) != 0;
	}

	constexpr bool PressStart() {
		return (inputData & PAD_START) != 0;
	}
};

inline Joypad ReadJoypad(const char joypadID) {
	return Joypad(pad_poll(joypadID));
}

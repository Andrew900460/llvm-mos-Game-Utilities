#pragma once


#define __NES__

#include <cstddef>
#include <nes.h>
#include <neslib.h>
#include "peekpoke.h"
#include "../vec2u8.hpp"

///** ATTENTION! This file is largely a work in progress. **///
///** 		 Many Things are bound to change here		  **///

// NES High-level and Abstracted Utilities
// This file is meant to "extend" the nes.h file, though there are a few "strange things"
// this file has to do to work with that file (because of the "#define PPU" line).

// Mainly, it has to "#undef PPU", so that I can create a class called "PPU" that is supposed to
// replace/extend the C-style-ish way of making a macro constant, which is still good.

// But the goal of this C++ file is to extend the "PPU Api" by adding member functions
// that clearly state what you are doing with the NES hardware, but also not sacrificing performance.
// If you #include this file with code that already uses <nes.h>, it should work.
// But if not, then this file should replace the C-style <nes.h> file.

// This file will also include other "high level" utilities to do common actions with nes hardware
// For example, if you want to set every tile in a nametable to the same sprite value,
// send an array of palettes to the PPU Palette memory all at once,
// Or collect all the info about controller input data into a single byte and returning that to you.

// This file is still a work in progress
// I may seperate this file into parts (PPU,APU,Controllers)

/// 9/6/2023 - Many breaking changes have happened to this file.
/// As I've said before, things are shifting around as I try to tidy up
/// this file and make it clean, concise, but contains all necesary things.
/// But I am trying to keep all the previous functions so people can still see them.
//// I am also considering that this file should be a "high level extention" of neslib/nesdoug.
//// Where I introduce descriptive types that you can use and pass to special functions.

namespace nes {

	typedef unsigned short PPU_ADDR;

	constexpr PPU_ADDR PPU_PAL_BG_UNIVERSAL = 0x3f00;
	constexpr PPU_ADDR PPU_PALETTE_START = 0x3f00;
	constexpr PPU_ADDR PPU_PAL_BG0 = PPU_PALETTE_START;
	constexpr PPU_ADDR PPU_PAL_BG1 = 0x3f04;
	constexpr PPU_ADDR PPU_PAL_BG2 = 0x3f08;
	constexpr PPU_ADDR PPU_PAL_BG3 = 0x3f0c;
	constexpr PPU_ADDR PPU_PAL_SPR0 = 0x3f10;
	constexpr PPU_ADDR PPU_PAL_SPR1 = 0x3f14;
	constexpr PPU_ADDR PPU_PAL_SPR2 = 0x3f18;
	constexpr PPU_ADDR PPU_PAL_SPR3 = 0x3f1c;

	struct Color {
		byte color;
		constexpr Color(): color(0) {}
		constexpr Color(const byte color): color(color) {}
		constexpr explicit Color(const char hue4b, const char val2b): color(hue4b | val2b<<4) {} // works best if the input's are const/literals
		
		operator byte() const { return color; }
	};

	struct Palette {
		Color color0;
		Color color1;
		Color color2;
		Color color3;

		constexpr Palette(): color0(),color1(),color2(),color3() {}

		constexpr Palette(
			const Color c0,
			const Color c1,
			const Color c2,
			const Color c3
		): color0(c0),color1(c1),color2(c2),color3(c3) {}

		constexpr explicit Palette(const byte palColors[4]):
			color0(palColors[0]),color1(palColors[1]),color2(palColors[2]),color3(palColors[3]) {}
	};

	struct SpriteAttributes { // Variables are in some order as the values would be in OAM
		byte tileID;
		byte attributes;

		constexpr explicit SpriteAttributes(const byte tileID, const byte attributes) :
		tileID(tileID), attributes(attributes) {}

		constexpr explicit SpriteAttributes(const byte tileID,
			const char paletteIndex, const bool isBehindBackground, const bool flipH, const bool flipV) :
			tileID(tileID) {
			SetAttributes(paletteIndex, isBehindBackground, flipH, flipV);
		}

		// Allows you to setup the info for sprites in a more human readable way. (works best with literals)
		constexpr void SetAttributes( // this may become a seperate function outside the struct
			const char paletteIndex, // number between [0,3]
			const bool isBehindBackground,
			const bool flipH,
			const bool flipV
		) { attributes = paletteIndex | isBehindBackground << 5 | flipH << 6 | flipV << 7; }
	};

	struct Sprite { // Variables are in some order as the values would be in OAM
		byte y;
		byte tileID;
		byte attributes;
		byte x;

		constexpr explicit Sprite(const byte x, const byte y, const byte tileID, const byte attributes) :
		x(x), y(y), tileID(tileID), attributes(attributes){}

		constexpr explicit Sprite(const byte x, const byte y, const byte tileID, 
			const char paletteIndex, const bool isBehindBackground, const bool flipH, const bool flipV) :
		x(x), y(y), tileID(tileID){
			SetAttributes(paletteIndex, isBehindBackground, flipH, flipV);
		}

		// Allows you to setup the info for sprites in a more human readable way. (works best with literals)
		constexpr void SetAttributes( // this may become a seperate function outside the struct
			const char paletteIndex, // number between [0,3]
			const bool isBehindBackground,
			const bool flipH,
			const bool flipV
		) { attributes = paletteIndex | isBehindBackground << 5 | flipH << 6 | flipV << 7; }
	};
	
	namespace ppu {
		inline void EnableBackground(const bool enable) {
			if(enable) PPU.mask = 0b00001000;
			else PPU.mask = 0b00000000;
		}

		inline void EnablePPU(const bool enable) {
			if(enable) ppu_on_all();
			else ppu_off();
		}

		// Setup PPU vram write address to start sending data to the PPU
		inline void SetVramWriteAddress(const PPU_ADDR addr) {
			(void)PPU.status;
			PPU.vram.address = addr >> 8;
			PPU.vram.address = addr & 0xff;
		}

		// Send 1 byte of data to the vram
		inline void WriteToVram(const byte data) {
			PPU.vram.data = data;
		}

		void WriteArrayToVram(const char* data, const unsigned short length);

		// Update specific palette on PPU
		void SendPalette(const PPU_ADDR paletteAddress, const Palette& palette);

		// Update All (background and sprite) palettes on PPU
		void SendAllPalettes(const Palette palettes[]);

		// Send Sprite OAM data to ppu to be rendered on screen.
		inline void UploadOAM() {
			PPU.sprite.address = 0x00;
			APU.sprite.dma = 0x02;
		}

		inline void WaitNMI() {
			ppu_wait_nmi();
		}

		void WaitNMI(byte nmiCount);

		inline void WaitVBlank() {
			while (!(PPU.status & 0x80));
		}

		void WaitVBlanks(byte vblanks);

		// floats aren't currently supported, but the compiler can still work with them :D
		// So we can use a const float and convert it to a byte at compile time. Now our code is more clear.
		void WaitVBlankSeconds(const float seconds);

		// Clear "Object Attribute Memory", a place in the Ram where Sprite info is stored
		// Ideally, 'oamRamAddr' should be a value where the lower byte is zero, for performance reasons. (0x0200) 
		void ClearOAM(const size_t oamRamAddr);

		// Most of the time, people will use address 0x0200 to hold the sprite data
		inline void ClearOAM() {
			ClearOAM(0x0200);
		}

		// Setup sprite data for a specific index in the OAM in the Ram
		// For now, we are assuming the data is at 0x0200
		inline void SetSprite(const char x, const char y, const char tileID, const char attrib, const char oamIndex) {
			POKE(0x0200 + (oamIndex << 2),y);
			POKE(0x0201 + (oamIndex << 2),tileID);
			POKE(0x0202 + (oamIndex << 2),attrib);
			POKE(0x0203 + (oamIndex << 2),x);
		}

		inline void SetSprite(const Sprite sprite, const char oamIndex) {
			SetSprite(sprite.x, sprite.y, sprite.tileID, sprite.attributes, oamIndex);
		}

		// Setup sprite data for sprite zero
		inline void SetSprite(const char x, const char y, const char tileID, const char attrib) {
			SetSprite(x,y,tileID,attrib,0);
		}
			
	}

	// All the code below here is meant to be an "extention" of neslib and nesdoug

	inline void SetAllPalettes(const Palette palettes[8]) {
		pal_all((void*)palettes);
	}

	inline void SetBackgroundPalettes(const Palette palettes[4]) {
		pal_bg((void*)palettes);
	}

	inline void SetSpritePalettes(const Palette palettes[4]) {
		pal_spr((void*)palettes);
	}

	inline void DrawSprite(const Sprite sprite) {
		oam_spr(sprite.x , sprite.y, sprite.tileID, sprite.attributes);
	}

	inline void DrawSpriteCentered(const vec2u8 centeredPos, const SpriteAttributes spr) {
		const vec2u8 pos = PositionToCenter(centeredPos, 8, 8);
		oam_spr(pos.x, pos.y, spr.tileID, spr.attributes);
	}

	inline void DrawSprite16x16(const vec2u8 pos, const vec2u8 anchorPos, const SpriteAttributes spr[4]) {
		// TODO: Implement
	}
};

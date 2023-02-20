#pragma once

#include <stdio.h>

#include <nes.h>
#include <neslib.h>
#include <peekpoke.h>

#include "mos-util.hpp"

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

#undef PPU // I am... Inevitable.
#define PPU_MEM (*reinterpret_cast<volatile struct __ppu*>(0x2000)) // Just in case you want to do this.

namespace NesUtil {

	typedef unsigned short PPU_ADDR;

	const PPU_ADDR PPU_PAL_BG_UNIVERSAL = 0x3f00;
	const PPU_ADDR PPU_PALETTE_START = 0x3f00;
	const PPU_ADDR PPU_PAL_BG0 = PPU_PALETTE_START;
	const PPU_ADDR PPU_PAL_BG1 = 0x3f04;
	const PPU_ADDR PPU_PAL_BG2 = 0x3f08;
	const PPU_ADDR PPU_PAL_BG3 = 0x3f0c;
	const PPU_ADDR PPU_PAL_SPR0 = 0x3f10;
	const PPU_ADDR PPU_PAL_SPR1 = 0x3f14;
	const PPU_ADDR PPU_PAL_SPR2 = 0x3f18;
	const PPU_ADDR PPU_PAL_SPR3 = 0x3f1c;

	struct PPU;
	struct Palette;
	struct NesColor;
	struct Sprite;
	volatile PPU& PPU = (*(volatile struct PPU*)0x2000);

	struct NesColor {
		byte color;
		NesColor(): color(0) {}
		explicit NesColor(const byte color): color(color) {}
		explicit NesColor(const char hue4b, const char val2b): color(hue4b | val2b<<4) {} // works best if the input's are const/literals
		
		operator byte() const { return color; }
	};

	struct Palette {
		NesColor color0;
		NesColor color1;
		NesColor color2;
		NesColor color3;

		Palette(): color0(0),color1(0),color2(0),color3(0) {}

		Palette(
			const NesColor c0,
			const NesColor c1,
			const NesColor c2,
			const NesColor c3
		): color0(c0),color1(c1),color2(c2),color3(c3) {}

		explicit Palette(
			const byte palColors[4]
		): color0(palColors[0]),color1(palColors[1]),color2(palColors[2]),color3(palColors[3]) {}
	};

	struct Sprite {
		byte x;
		byte y;
		byte tileID;
		byte attributes;

		// Allows you to setup the info for sprites in a more human readable way. (works best with literals)
		void SetAttributes( // this may become a seperate function outside the struct
			const char paletteIndex, 
			const bool isBehindBackground, 
			const bool flipH, 
			const bool flipV
		) { attributes = paletteIndex | isBehindBackground << 5 | flipH << 6 | flipV << 7; }
	};

	struct PPU : public __ppu {

		void EnableBackground(bool enable) volatile {
			if(enable) PPU_MEM.mask = 0b00001000;
			else PPU_MEM.mask = 0b00000000;
		}

		void EnablePPU(bool enable) volatile {
			if(enable) ppu_on_all();
			else ppu_off();
		}

		// Setup PPU vram write address to start sending data to the PPU
		void SetVramWriteAddress(const PPU_ADDR addr) volatile {
			(void)PPU_MEM.status;
			PPU_MEM.vram.address = addr >> 8;
			PPU_MEM.vram.address = addr & 0xff;
		}

		// Send 1 byte of data to the vram
		void WriteToVram(const byte data) volatile {
			PPU_MEM.vram.data = data;
		}

		void WriteArrayToVram(const char* data, const unsigned short length) volatile {
			for(unsigned short i=0;i<length;i++) {
				WriteToVram(data[i]);
			}
		}

		// 
		/*void SendPalette_NoAddressWrite(const Palette& palette) volatile {
			WriteToVram(palette.color0);
			WriteToVram(palette.color1);
			WriteToVram(palette.color2);
			WriteToVram(palette.color3);
		}*/

		// Update specific palette on PPU
		void SendPalette(PPU_ADDR paletteAddress, const Palette& palette) volatile {
			SetVramWriteAddress(paletteAddress);
			WriteToVram(palette.color0);
			WriteToVram(palette.color1);
			WriteToVram(palette.color2);
			WriteToVram(palette.color3);
		}

		// Update All (background and sprite) palettes on PPU
		void SendAllPalettes(const Palette palettes[]) volatile {
			SetVramWriteAddress(PPU_PALETTE_START);
			for (char i=0;i<32;i++) {
				WriteToVram(reinterpret_cast<byte*>(palettes)[i]);
			}
		}

		// Send Sprite OAM data to ppu to be rendered on screen.
		void UploadOAM() volatile {
			PPU_MEM.sprite.address = 0x00;
			APU.sprite.dma = 0x02;
		}

		volatile __ppu& operator ()() { // kinda redundant, but I'll leave it here for now
			return (*reinterpret_cast<volatile struct __ppu*>(0x2000));
		}
	};

	void WaitNMI() {
		ppu_wait_nmi();
	}

	void WaitNMI(byte nmiCount) {
		for (;nmiCount > 0; nmiCount--) {
			ppu_wait_nmi();
		}
	}

	void WaitVBlank() {
		while (!(PPU_MEM.status & 0x80));
	}

	void WaitVBlanks(byte vblanks) {
		for (;vblanks > 0; vblanks--) {
			WaitVBlank();
		}
	}

	// floats aren't currently supported, but the compiler can still work with them :D
	// So we can use a const float and convert it to a byte at compile time. Now our code is more clear.
	void WaitVBlankSeconds(const float seconds) {
		byte vblanks = (byte)(seconds*60.0f);
		WaitVBlanks(vblanks);
	}

	// Clear "Object Attribute Memory", a place in the Ram where Sprite info is stored
	// Ideally, 'oamRamAddr' should be a value where the lower byte is zero, for performance reasons. (0x0200) 
	void ClearOAM(const unsigned short oamRamAddr) {
		byte i=0;
		while(true) {
			POKE(oamRamAddr+i,0xff);
			i+=4;
			if(i==0) break;
		}
	}
	// Most of the time, people will use address 0x0200 to hold the sprite data
	void ClearOAM() {
		ClearOAM(0x0200);
	}

	// Setup sprite data for a specific index in the OAM in the Ram
	// For now, we are assuming the data is at 0x0200
	void SetSprite(const char x, const char y, const char tileID, const char attrib, const char oamIndex) {
		POKE(0x0200 + (oamIndex << 2),y);
		POKE(0x0201 + (oamIndex << 2),tileID);
		POKE(0x0202 + (oamIndex << 2),attrib);
		POKE(0x0203 + (oamIndex << 2),x);
	}

	void SetSprite(const Sprite sprite, const char oamIndex) {
		SetSprite(sprite.x, sprite.y, sprite.tileID, sprite.attributes, oamIndex);
	}

	// Setup sprite data for sprite zero
	void SetSprite(const char x, const char y, const char tileID, const char attrib) {
		SetSprite(x,y,tileID,attrib,0);
	}	

	char ReadJoypad() {
		POKE(0x4016,1);
		POKE(0x4016,0);
		char a = 0;
		for(char i=0;i<8;i++){
			char t = PEEK(0x4016);
			a <<= 1;
			a |= t;
		}
		return a;
	}

	__attribute__((leaf))
	char ReadJoypad2() { // Name to be changed
			char a;
			asm("ldx #01");
			asm volatile ("stx $4016");
			asm volatile ("stx __rc2");
			asm ("dex");
			asm volatile ("stx $4016");
			//asm ("loop:");
			asm ("lda $4016");
			asm ("lsr");
			asm volatile ("rol __rc2");
			asm goto ("bcc $F7");
			asm ("lda __rc2");
			asm ("sta %0":"=a"(a));
			return a;
	}

	void print(const char* format,...) { // Still a work in progress
		//char* buffer = new char[32];
		//va_list args;
		//va_start(args,format);
		//unsigned char printSize = vsnprintf(buffer,32,format,args);

		PPU.SetVramWriteAddress(NTADR_A(1, 2));
		PPU.WriteArrayToVram(format,32);
		//printY++;
		//delete[] buffer;
	}
	
};

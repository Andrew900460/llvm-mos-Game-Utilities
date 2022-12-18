#include <nes.h>

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

#undef PPU // I am... Inevitable.

#define PPU_MEM (*(volatile struct __ppu*)0x2000) // Just in case you want to do this.

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
	volatile PPU& PPU = (*(volatile struct PPU*)0x2000);

	struct NesColor {
		unsigned char color;
		NesColor(): color(0) {}
		NesColor(const unsigned char color): color(color) {}
		NesColor(const char hue4b, const char val2b): color(hue4b | val2b<<4) {} // works best if the input's are const/literals
		
		operator unsigned char() const { return color; }
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

		Palette(
			const unsigned char palColors[4]
		): color0(palColors[0]),color1(palColors[1]),color2(palColors[2]),color3(palColors[3]) {}
	};

	struct PPU : public __ppu {

		void EnableBackground(bool enable) {
			if(enable) PPU_MEM.mask = 0b00001000;
			else PPU_MEM.mask = 0b00000000;
		}

		void WriteAddress(PPU_ADDR addr) volatile {
			(void)PPU_MEM.status;
			PPU_MEM.vram.address = addr >> 8;
			PPU_MEM.vram.address = addr & 0xff;
		}

		void SendToVram(unsigned char data) volatile {
			PPU_MEM.vram.data = data;
		}

		void SendPalette_NoAddressWrite(const Palette& palette) volatile {
			SendToVram(palette.color0);
			SendToVram(palette.color1);
			SendToVram(palette.color2);
			SendToVram(palette.color3);
		}

		// Update specific palette on PPU
		void SendPalette(PPU_ADDR paletteAddress, const Palette& palette) volatile {
			WriteAddress(paletteAddress);
			SendPalette_NoAddressWrite(palette);
		}

		// Update All (background and sprite) palettes on PPU
		// I want to find a way to keep the compiler from unrolling the loop
		void SendAllPalettes(const Palette palettes[]) volatile {
			WriteAddress(PPU_PALETTE_START);
			for (char i=0;i<32;i++) {
				SendToVram(((unsigned char*)palettes)[i]);
			}
		}

		volatile __ppu& operator ()() { // kinda redundant, but I'll leave it here for now
			return (*(volatile struct __ppu*)0x2000);
		}
	};

	void WaitVBlank() {
		while (!(PPU_MEM.status & 0x80));
	}

	void WaitVBlanks(unsigned char vblanks) {
		for (;vblanks > 0; vblanks--) {
			WaitVBlank();
		}
	}

	// floats aren't currently supported, but the compiler can still work with them :D
	// So we can use a const float and convert it to a byte at compile time. Now our code is more clear.
	void WaitVBlankSeconds(const float seconds) {
		unsigned char vblanks = (unsigned char)(seconds*60.0f);
		WaitVBlanks(vblanks);
	}
	
};


#include "nes-util.hpp"
#include "../mos-util.hpp"

void nes::ppu::WriteArrayToVram(const char* data, const unsigned short length) {
	for(unsigned short i=0;i<length;i++) {
		WriteToVram(data[i]);
	}
}

/*void SendPalette_NoAddressWrite(const Palette& palette) {
	WriteToVram(palette.color0);
	WriteToVram(palette.color1);
	WriteToVram(palette.color2);
	WriteToVram(palette.color3);
}*/

// Update specific palette on PPU
void nes::ppu::SendPalette(const PPU_ADDR paletteAddress, const Palette& palette) {
	SetVramWriteAddress(paletteAddress);
	WriteToVram(palette.color0);
	WriteToVram(palette.color1);
	WriteToVram(palette.color2);
	WriteToVram(palette.color3);
}

// Update All (background and sprite) palettes on PPU
void nes::ppu::SendAllPalettes(const Palette palettes[]) {
	SetVramWriteAddress(PPU_PALETTE_START);
	for (char i=0;i<32;i++) {
		WriteToVram(((char*)palettes)[i]);
	}
}


void nes::ppu::WaitNMI(byte nmiCount) {
	for (;nmiCount > 0; nmiCount--) {
		ppu_wait_nmi();
	}
}

void nes::ppu::WaitVBlanks(byte vblanks) {
	for (;vblanks > 0; vblanks--) {
		WaitVBlank();
	}
}

// floats aren't currently supported, but the compiler can still work with them :D
// So we can use a const float and convert it to a byte at compile time. Now our code is more clear.
void nes::ppu::WaitVBlankSeconds(const float seconds) {
	byte vblanks = (byte)(seconds*60.0f);
	WaitVBlanks(vblanks);
}

// Clear "Object Attribute Memory", a place in the Ram where Sprite info is stored
// Ideally, 'oamRamAddr' should be a value where the lower byte is zero, for performance reasons. (0x0200) 
void nes::ppu::ClearOAM(const size_t oamRamAddr) {
	byte i=0;
	while(true) {
		ram<byte>(oamRamAddr+i) = 0xff;
		i+=4;
		if(i==0) break;
	}
}
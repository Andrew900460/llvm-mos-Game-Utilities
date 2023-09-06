.include "imag.inc"
.include "nes.inc"

.text

.global ReadJoypad1
ReadJoypad1:
	ldx #01
	stx APU_PAD1 ; tell the joypad we are going to read it's input (sent 1)
	stx __rc2 ; temporary byte register will store the result bits as they are recieved
	dex
	stx APU_PAD1 ; tell the joypad we are going to read it's input (sent 0)
joypad_loop:
	lda APU_PAD1 ; read first bit from joypad
	lsr
	rol __rc2 ; push the read joypad bit from the A register into our temp/result register
	bcc joypad_loop ; loop over that code until we reached the end (all 8 bits)
	lda __rc2 ; bring our result back into the A register (return value)
	rts

.global ReadJoypad2
ReadJoypad2:
	rts
.include "imag.inc"

.text
;.section .text._mult_10,"a",@progbits

.global mult_10
mult_10:
	asl
	sta __rc2
	asl
	asl
	clc
	adc __rc2
	rts
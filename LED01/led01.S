.text
.global _start

_start:

	ldr r0, =0x1400
	ldr r1, =0x56000050
	str r0, [r1]

	ldr r0, =0
	ldr r1, =0x56000054
	str r0, [r1]

halt:
	b halt
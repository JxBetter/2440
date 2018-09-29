.text
.global _start

_start:

# 关看门狗
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

# 初始化时钟
	ldr r0, =0x4c000000
	ldr r1, =0xffffffff
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4c000014
	ldr r1, =0x05
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置FCLK为400MHZ */
	ldr r0, =0x4c000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

# 初始化SDRAM
	ldr r0, =0x48000000
	adr r1, sdram_config
	add r3, r0, #(4*13)

loop:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne loop

# 重定位
	ldr sp, =0x34000000
	bl copy_to_sdram
	bl clean_bss

# 跳转到boot.c中的main
	ldr lr, =halt
	ldr pc, =main

halt:
	b halt

sdram_config:
	.long 0x22000000	 //BWSCON
	.long 0x00000700	 //BANKCON0
	.long 0x00000700	 //BANKCON1
	.long 0x00000700	 //BANKCON2
	.long 0x00000700	 //BANKCON3  
	.long 0x00000700	 //BANKCON4
	.long 0x00000700	 //BANKCON5
	.long 0x18001	     //BANKCON6
	.long 0x18001	     //BANKCON7
	.long 0x8404f5	     // REFRESH
	.long 0xb1	         //BANKSIZE
	.long 0x20	         //MRSRB6
	.long 0x20	         //MRSRB7

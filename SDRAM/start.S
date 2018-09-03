.equ SDRAM_BASE, 0x30000000
.equ WATCHDOG_BASE, 0x53000000
.equ MEMORY_BASE, 0x48000000

.text
.global _start

_start:
	bl disable_watchdog
	bl clk_init
	bl memory_init
	bl copy_from_sram_to_sdram
	ldr pc, =run

run:
	ldr sp, =0x34000000
	bl main
	

halt:
	b halt


disable_watchdog:
	/* 关闭看门狗 */
	ldr r0, =WATCHDOG_BASE
	mov r1, #0
	str r1, [r0]
	mov pc, lr
	
	
clk_init:
	/* 设置时钟MPLL，FCLK = 400M，HCLK = 100M，PCLK = 50M */
	/* 设置LOCKTIME */
	ldr r0 , = 0x4C000000
	ldr r1, = 0xFFFFFFFF
	str r1, [r0]
	
	/* 设置分频因子，寄存器为CLKDIVN */
	ldr r0, = 0x4C000014
	ldr r1, = 0x5
	str r1, [r0]
	
	/* 设置为异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	/* 设置FCLK，寄存器为MPLLCON */
	ldr r0, = 0x4C000004
	ldr r1, = (0x5c << 12) | (0x1 << 4) | (0x1 << 0)
	str r1, [r0]
	mov pc, lr


memory_init:
	/* 初始化内存管理器 */
	ldr r0, =MEMORY_BASE
	ldr r1, =0x22000000
	str r1, [r0]
	
	ldr r0, =(MEMORY_BASE + 0x1c)
	ldr r1, =0x00018001
	str r1, [r0]
	
	ldr r0, =(MEMORY_BASE + 0x24)
	ldr r1, =0x008404f5
	str r1, [r0]
	
	ldr r0, =(MEMORY_BASE + 0x28)
	ldr r1, =0x000000b1
	str r1, [r0]
	
	ldr r0, =(MEMORY_BASE + 0x2c)
	ldr r1, =0x00000020
	str r1, [r0]
	
	mov pc, lr
	
	


copy_from_sram_to_sdram:
	/* 复制SRAM中的4K数据到SDRAM中 */
	mov r0, #0
	ldr r1, = SDRAM_BASE
	ldr r2, = 4*1024

1:
	ldr r3, [r0], #4
	str r3, [r1], #4
	cmp r0, r2
	bne 1b
	mov pc, lr

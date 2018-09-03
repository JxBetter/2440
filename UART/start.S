.text
.global _start

_start:
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
	
	
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]
	
	mov r0, #0 /* 将要写入的数据0放在寄存器r0中 */
	ldr r1, [r0] /* 取出0地址中的值放在r1寄存器中 */
	str r0, [r0] /* 将r0寄存器中的数据0写入0地址中 */
	ldr r2, [r0] /* 取出0地址中的值放在r2寄存器中 */
	cmp r0, r2 /* 将从0地址取出的值和之前写入的数据0比较 */
	ldr sp, =0x40004096 /* 先假设是nor启动 */
	moveq sp, #4096 /* nand启动执行这句，nor启动不会执行这句，因为nor不能写，所以0地址处的值不会是0，第9句代码不会成立 */
	streq r1, [r0] /* 无论nand和nor启动都要恢复之前被改写的0地址的值，写入之前的值存在r1中 */
	
	bl main	/* 执行main函数 */

halt:
	b halt


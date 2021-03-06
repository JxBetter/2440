
.text
.global _start

_start:
	b reset /* 0x00 */
	b halt  /* 0x04 */
	b halt  /* 0x08 */
	b halt  /* 0x0c */
	b halt  /* 0x10 */
	b halt  /* 0x14 */
	ldr pc, irq  /* 0x18 */
	b halt  /* 0x18 */
	
irq:
	.word eint_interrupt

eint_interrupt:
	/* 1.设置栈指针，保存lr
	*  2.保存现场
	*  3.执行中断服务函数
	*  4.恢复现场
	*/
	ldr sp, =0x33000000
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}
	ldr lr, =irq_return
	ldr pc, =eint_process_function
	
irq_return:
	ldmia sp!, {r0-r12, pc}^

reset:
	/* 关闭看门狗 */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0X5, tFCLK:tHCLK:tPCLK = 1:4:8  */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* 设置CPU工作于异步模式 */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* 设置MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
	 *  m = MDIV+8 = 92+8=100
	 *  p = PDIV+2 = 1+2 = 3
	 *  s = SDIV = 1
	 *  FCLK = 2*m*Fin/(p*2^s) = 2*100*12/(3*2^1)=400M
	 */
	ldr r0, =0x4C000004
	ldr r1, =(92<<12)|(1<<4)|(1<<0)
	str r1, [r0]

	/* 一旦设置PLL, 就会锁定lock time直到PLL输出稳定
	 * 然后CPU工作于新的频率FCLK
	 */
	
	

	/* 设置内存: sp 栈 */
	/* 分辨是nor/nand启动
	 * 写0到0地址, 再读出来
	 * 如果得到0, 表示0地址上的内容被修改了, 它对应ram, 这就是nand启动
	 * 否则就是nor启动
	 */
	mov r1, #0
	ldr r0, [r1] /* 读出原来的值备份 */
	str r1, [r1] /* 0->[0] */ 
	ldr r2, [r1] /* r2=[0] */
	cmp r1, r2   /* r1==r2? 如果相等表示是NAND启动 */
	ldr sp, =0x40000000+4096 /* 先假设是nor启动 */
	moveq sp, #4096  /* nand启动 */
	streq r0, [r1]   /* 恢复原来的值 */
	
	bl sdram_init
	bl copy_to_sdram
	bl interrupt_init
	bl gpio_eint_init
	bl clean

	mrs r0, cpsr
	bic r0, r0, #0xf
	bic r0, r0, #(1<<7)
	msr cpsr, r0

	ldr sp, =0x34000000
	ldr pc, =main

halt:
	b halt
	

all:
	arm-linux-gcc -Iinclude -c -o uart.o uart.c
	arm-linux-gcc -Iinclude -c -o init.o init.c
	arm-linux-gcc -Iinclude -c -o main.o main.c
	arm-linux-gcc -Iinclude -c -o interrupt.o interrupt.c
	arm-linux-gcc -Iinclude -c -o start.o start.S
	#arm-linux-ld -Ttext 0 -Tdata 0x800 start.o init.o uart.o uart_app.o -o uart.elf
	arm-linux-ld -T sdram.lds start.o init.o uart.o main.o interrupt.o -o eint_interrupt.elf
	arm-linux-objcopy -O binary -S eint_interrupt.elf eint_interrupt.bin
	arm-linux-objdump -D eint_interrupt.elf > eint_interrupt.dis
	
clean:
	rm *.bin *.o *.elf *.dis
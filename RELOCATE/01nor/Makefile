all:
	arm-linux-gcc -Iinclude -c -o uart.o uart.c
	arm-linux-gcc -Iinclude -c -o init.o init.c
	arm-linux-gcc -Iinclude -c -o uart_app.o uart_app.c
	arm-linux-gcc -Iinclude -c -o start.o start.S
	#arm-linux-ld -Ttext 0 -Tdata 0x800 start.o init.o uart.o uart_app.o -o uart.elf
	arm-linux-ld -T sdram.lds start.o init.o uart.o uart_app.o -o uart.elf
	arm-linux-objcopy -O binary -S uart.elf relocate1.bin
	arm-linux-objdump -D uart.elf > uart.dis
	
clean:
	rm *.bin *.o *.elf *.dis
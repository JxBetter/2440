all:
	arm-linux-gcc -Iinclude -c -o uart.o uart.c
	arm-linux-gcc -Iinclude -c -o uart_app.o uart_app.c
	arm-linux-gcc -Iinclude -c -o start.o start.S
	arm-linux-ld -Ttext 0x30000000 start.o uart.o uart_app.o -o uart.elf
	arm-linux-objcopy -O binary -S uart.elf sdram.bin
	arm-linux-objdump -D uart.elf > uart.dis
	
clean:
	rm *.bin *.o *.elf *.dis
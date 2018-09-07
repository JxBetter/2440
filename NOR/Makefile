all:
	arm-linux-gcc -Iinclude -march=armv4 -c -o uart.o uart.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o init.o init.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o main.o main.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o timer.o timer.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o norflash.o norflash.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o string_utils.o string_utils.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o my_printf.o my_printf.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o interrupt.o interrupt.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o start.o start.S
	arm-linux-gcc -Iinclude -march=armv4 -c -o lib1funcs.o lib1funcs.S
	#arm-linux-ld -Ttext 0 -Tdata 0x800 start.o init.o uart.o uart_app.o -o uart.elf
	arm-linux-ld -T sdram.lds start.o init.o uart.o main.o timer.o norflash.o lib1funcs.o string_utils.o my_printf.o interrupt.o -o nor.elf
	arm-linux-objcopy -O binary -S nor.elf nor.bin
	arm-linux-objdump -D nor.elf > nor.dis
	
clean:
	rm *.bin *.o *.elf *.dis
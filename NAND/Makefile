all: start.o init.o interrupt.o nandflash.o norflash.o uart.o main.o timer.o  my_printf.o string_utils.o lib1funcs.o
	#arm-linux-ld -Ttext 0 -Tdata 0x30000000  start.o led.o uart.o init.o main.o -o sdram.elf
	arm-linux-ld -T sdram.lds $^ -o nand.elf
	arm-linux-objcopy -O binary -S nand.elf nand.bin
	arm-linux-objdump -D nand.elf > nand.dis
clean:
	rm *.bin *.o *.elf *.dis
	
%.o : %.c
	arm-linux-gcc -Iinclude -march=armv4 -c -o $@ $<

%.o : %.S
	arm-linux-gcc -march=armv4 -c -o $@ $<
	